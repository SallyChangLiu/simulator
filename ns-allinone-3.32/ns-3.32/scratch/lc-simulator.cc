#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/samples-routing-module.h"
#include "ns3/ipv4-address-generator.h"

#include <map>
#include <vector>
#include <random>
#include <time.h>
#include <fstream>

#include "json.hpp"
#include "csv.hpp"

using namespace ns3;
using namespace io;

using json = nlohmann::json;

/*******************************
 * Route calculation variables *
 *******************************/

struct Interface
{
    Ptr<SamplesRoutingNetDevice> device;
    Time delay;
    DataRate bandwidth;
    uint64_t queueSize;
};
std::map<Ptr<SamplesRoutingNode>, std::map<Ptr<SamplesRoutingNode>, Interface>> onewayOutDev;
std::map<Ptr<SamplesRoutingNode>, std::map<Ptr<SamplesRoutingNode>, std::vector<Ptr<SamplesRoutingNode>>>> nextHopTable;
std::map<Ptr<SamplesRoutingNode>, std::map<Ptr<SamplesRoutingNode>, Time>> pairDelay;
std::map<Ptr<SamplesRoutingNode>, std::map<Ptr<SamplesRoutingNode>, Time>> pairTxDelay;
std::map<Ptr<SamplesRoutingNode>, std::map<Ptr<SamplesRoutingNode>, DataRate>> pairBandwidth;

/***************************************
 * Help functions for simulation setup *
 ***************************************/

//configure
void ConfigForNet5(const json &conf);

//setup router table
void CalculateRoute();
void CalculateRoute(Ptr<SamplesRoutingNode> host);
void SetRoutingEntries();

//setup destination for app sending packet
void Start(const json &conf);

NS_LOG_COMPONENT_DEFINE("LC-SIMULATION");

int main(int argc, char *argv[])
{
    LogComponentEnable("LC-SIMULATION", LOG_INFO);
    
    if (argc < 2)
    {
        NS_LOG_UNCOND("ERROR: No config file");
        return 1;
    }

    std::ifstream file(argv[1]);

    json conf = json::parse(file);

    NS_LOG_UNCOND("=======HOST=======");
    ConfigForNet5(conf);
    NS_LOG_UNCOND("=======router=======");
    CalculateRoute();
    SetRoutingEntries();
    NS_LOG_UNCOND("=======start=======");
    //Start(conf);

    Simulator::Run();
    Simulator::Stop(Time("2s"));
    Simulator::Destroy();
}

/***************************************
 * Help functions for simulation setup *
 ***************************************/

void ConfigForNet5(const json &conf)
{
    uint32_t h_num = conf["Number"];
    for (uint32_t i = 0; i < h_num; i++)
    {
        //node
        std::string name = "host" + std::to_string(i);
        const Ptr<SamplesRoutingNode> node = CreateObject<SamplesRoutingNode>(Ipv4AddressGenerator::NextAddress(Ipv4Mask("255.0.0.0")));
        addr2Name.insert({node->GetAddress(), name});
        name2node.insert({name, node});

        //app
        Ptr<SamplesRoutingApp> app = CreateObject<SamplesRoutingApp>();
        std::string time = conf["AppSendInterval"];
        Time sendInterval = Time(time);
        app->SetSendInterval(sendInterval);
        node->AddApplication(app);
        node->AggregateObject(app);
        //router
        Ptr<SamplesRoutingRouter> router = CreateObject<SamplesRoutingRouter>();
        node->AddRouter(router);

        //register receive callback router--->app
        router->SetupRxCallBack(MakeCallback(&SamplesRoutingApp::HandleRx, app));
    }
    //netdevice
    std::string rt = conf["DataRate"];
    DataRate dev_rate = DataRate(rt);
    double tLow = conf["LinkDelayLow"];
    double tHigh = conf["LinkDelayHigh"];

    uint32_t seed = conf["seed"];
    std::default_random_engine dre(seed);
    std::uniform_real_distribution<double> dis(tLow, tHigh); //generate delay randomly (ms)
    Time delay = Time(dis(dre));

    io::CSVReader<2> linkConfig(conf["LinkConfigFile"]);
    linkConfig.read_header(io::ignore_no_column, "FromNode", "ToNode");
    while (true)
    {
        std::string fromNode, toNode;
        if (!linkConfig.read_row(fromNode, toNode))
            break;

        const auto snode = name2node.left.at(fromNode);
        const auto dnode = name2node.left.at(toNode);

        //netdevice
        Ptr<SamplesRoutingNetDevice> sdev = CreateObject<SamplesRoutingNetDevice>();
        Ptr<SamplesRoutingNetDevice> ddev = CreateObject<SamplesRoutingNetDevice>();
        node2port[snode].push_back(sdev);
        node2port[dnode].push_back(ddev);

        //queue
        Ptr<SamplesRoutingQueue> squeue = CreateObject<SamplesRoutingQueue>();
        Ptr<SamplesRoutingQueue> dqueue = CreateObject<SamplesRoutingQueue>();

        //setup for netdevice
        sdev->SetQueue(squeue);
        ddev->SetQueue(dqueue);
        sdev->SetDataRate(dev_rate);
        ddev->SetDataRate(dev_rate);

        //channel
        Ptr<SamplesRoutingChannel> ch = CreateObject<SamplesRoutingChannel>();
        ch->SetDelay(delay);

        //dev attach to channel
        sdev->Attach(ch);
        ddev->Attach(ch);

        //register callback
        sdev->SetRxCallBack(MakeCallback(&SamplesRoutingRouter::HandleMsg, snode->GetRouter()));
        ddev->SetRxCallBack(MakeCallback(&SamplesRoutingRouter::HandleMsg, dnode->GetRouter()));

        //setup onewayOutDev
        onewayOutDev[snode][dnode] = {
            .device = sdev,
            .delay = delay,
            .bandwidth = dev_rate};
        onewayOutDev[dnode][snode] = {
            .device = ddev,
            .delay = delay,
            .bandwidth = dev_rate};
    }

    //setup dev for every node
    for (uint32_t i = 0; i < h_num; i++)
    {
        std::string name = "host" + std::to_string(i);
        const auto node = name2node.left.at(name);

        for (uint32_t j = 0; j < node2port[node].size(); j++)
        {
            const auto dev = node2port[node][j];
            node->AddDevice(dev);
        }
    }

    //setup destination host address for every host
    for (const auto &name : conf["DestinationNode"])
    {
        for (uint32_t i = 0; i < name2node.size(); i++)
        {
            std::string app_nodename = "host" + std::to_string(i);
            if (app_nodename == name)
                continue;

            const auto app_node = name2node.left.at(app_nodename);
            const auto node = name2node.left.at(name);
            for (uint32_t j = 0; j < app_node->GetAppSize(); j++)
            {
                const auto app = app_node->GetApplication(j);
                app->SetupDestAddr(node->GetAddress());
            }
            app_nodename = "";
        }
    }
}

/*********************
 * Route calculation *
 *********************/
void CalculateRoute()
{
    for (uint32_t i = 0; i < name2node.size(); i++)
    {
        std::string name = "host" + std::to_string(i);
        const auto host = name2node.left.at(name);

        CalculateRoute(host);
    }
}

void CalculateRoute(Ptr<SamplesRoutingNode> host)
{
    std::vector<Ptr<SamplesRoutingNode>> bfsQueue;          // Queue for the BFS
    std::map<Ptr<SamplesRoutingNode>, int> distances;       // Distance from the host to each node
    std::map<Ptr<SamplesRoutingNode>, Time> delays;         // Delay from the host to each node
    std::map<Ptr<SamplesRoutingNode>, Time> txDelays;       // Transmit delay from the host to each node
    std::map<Ptr<SamplesRoutingNode>, DataRate> bandwidths; // Bandwidth from the host to each node
    // Init BFS
    bfsQueue.push_back(host);
    distances[host] = 0;
    delays[host] = Time(0);
    txDelays[host] = Time(0);
    bandwidths[host] = DataRate(UINT64_MAX);
    // Do BFS
    for (size_t i = 0; i < bfsQueue.size(); i++)
    {
        const auto currNode = bfsQueue[i];
        for (const auto &next : onewayOutDev[currNode])
        {
            const auto nextNode = next.first;
            const auto nextInterface = next.second;
            // If 'nextNode' have not been visited.
            if (distances.find(nextNode) == distances.end())
            {
                distances[nextNode] = distances[currNode] + 1;
                delays[nextNode] = delays[currNode] + nextInterface.delay;
                txDelays[nextNode] =
                    txDelays[currNode] + nextInterface.bandwidth.CalculateBytesTxTime(MTU);
                bandwidths[nextNode] = std::min(bandwidths[currNode], nextInterface.bandwidth);

                bfsQueue.push_back(nextNode);
            }
            // if 'currNode' is on the shortest path from 'nextNode' to 'host'.
            if (distances[currNode] + 1 == distances[nextNode])
            {
                nextHopTable[nextNode][host].push_back(currNode);
            }
        }
    }
    for (const auto &it : delays)
        pairDelay[it.first][host] = it.second;
    for (const auto &it : txDelays)
        pairTxDelay[it.first][host] = it.second;
    for (const auto &it : bandwidths)
        pairBandwidth[it.first][host] = it.second;
}

void SetRoutingEntries()
{
    // For each node
    for (const auto &nextHopEntry : nextHopTable)
    {
        const auto fromNode = nextHopEntry.first;
        const auto toNodeTable = nextHopEntry.second;
        for (const auto &toNodeEntry : toNodeTable)
        {
            // The destination node
            const auto toNode = toNodeEntry.first;
            // The next hops towards the destination
            const auto nextNodeTable = toNodeEntry.second;
            // The IP address of the destination
            Ipv4Address dstAddr = toNode->GetAddress();
            for (const auto &nextNode : nextNodeTable)
            {
                const auto device = onewayOutDev[fromNode][nextNode].device;

                const auto router = fromNode->GetRouter();
                router->BuildRouterTable(dstAddr, device);
            }
        }
    }
}

void Start(const json &conf)
{
    std::string name = "host4";
    const auto node = name2node.left.at(name);
    const auto app = node->GetApplication(0);
}