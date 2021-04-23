/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "ns3/core-module.h"
#include "ns3/internet-module.h"
#include "ns3/samples-routing-module.h"
#include "ns3/callback.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE("Samples Routing Test");

int main(int argc, char *argv[])
{
    NS_LOG_UNCOND("Samples Routing Test");

    /**
     * host 1 --100Gbps-- host3 --1bps-- host 2
     */

    //node
    auto host1 = CreateObject<SamplesRoutingNode>(Ipv4AddressGenerator::NextAddress(Ipv4Mask("255.0.0.0")));
    auto host2 = CreateObject<SamplesRoutingNode>(Ipv4AddressGenerator::NextAddress(Ipv4Mask("255.0.0.0"))); // host 2
    auto host3 = CreateObject<SamplesRoutingNode>(Ipv4AddressGenerator::NextAddress(Ipv4Mask("255.0.0.0"))); // host 3

    //app
    auto h1_app = CreateObject<SamplesRoutingApp>();
    h1_app->SetSendInterval(Time("1ms"));

    auto h2_app = CreateObject<SamplesRoutingApp>();
    h2_app->SetSendInterval(Time("1ms"));

    auto h3_app = CreateObject<SamplesRoutingApp>();
    h3_app->SetSendInterval(Time("1ms"));

    //router
    auto h1_r = CreateObject<SamplesRoutingRouter>();
    auto h2_r = CreateObject<SamplesRoutingRouter>();
    auto h3_r = CreateObject<SamplesRoutingRouter>();

    //net device
    auto h1_dev = CreateObject<SamplesRoutingNetDevice>();
    auto h2_dev = CreateObject<SamplesRoutingNetDevice>();
    auto h3_h1_dev = CreateObject<SamplesRoutingNetDevice>();
    auto h3_h2_dev = CreateObject<SamplesRoutingNetDevice>();

    //queue
    auto h1_dev_q = CreateObject<SamplesRoutingQueue>();
    auto h2_dev_q = CreateObject<SamplesRoutingQueue>();
    auto h3_h1_dev_q = CreateObject<SamplesRoutingQueue>();
    auto h3_h2_dev_q = CreateObject<SamplesRoutingQueue>();

    //channel
    auto ch1 = CreateObject<SamplesRoutingChannel>();
    auto ch2 = CreateObject<SamplesRoutingChannel>();

    //build up network
    h1_dev->SetQueue(h1_dev_q);
    h2_dev->SetQueue(h2_dev_q);
    h3_h1_dev->SetQueue(h3_h1_dev_q);
    h3_h2_dev->SetQueue(h3_h2_dev_q);

    h1_dev->SetDataRate(DataRate("100Gbps"));
    h2_dev->SetDataRate(DataRate("1bps"));
    h3_h1_dev->SetDataRate(DataRate("100Gbps"));
    h3_h2_dev->SetDataRate(DataRate("1bps"));

    ch1->Attach(h1_dev);
    ch1->Attach(h3_h1_dev);
    ch2->Attach(h3_h2_dev);
    ch2->Attach(h2_dev);
    ch1->SetDelay(Time("1us"));
    ch2->SetDelay(Time("2us"));

    host1->AddDevice(h1_dev);
    host2->AddDevice(h2_dev);
    host3->AddDevice(h3_h1_dev);
    host3->AddDevice(h3_h2_dev);

    host1->AddApplication(h1_app);
    host2->AddApplication(h2_app);
    host3->AddApplication(h3_app);

    host1->AddRouter(h1_r);
    host2->AddRouter(h2_r);
    host3->AddRouter(h3_r);

    //register callback
    h1_r->SetupRxCallBack(MakeCallback(&SamplesRoutingApp::HandleRx, h1_app));
    h2_r->SetupRxCallBack(MakeCallback(&SamplesRoutingApp::HandleRx, h2_app));
    h3_r->SetupRxCallBack(MakeCallback(&SamplesRoutingApp::HandleRx, h3_app));

    h1_dev->SetRxCallBack(MakeCallback(&SamplesRoutingRouter::HandleMsg, h1_r));
    h2_dev->SetRxCallBack(MakeCallback(&SamplesRoutingRouter::HandleMsg, h2_r));
    h3_h1_dev->SetRxCallBack(MakeCallback(&SamplesRoutingRouter::HandleMsg, h3_r));
    h3_h2_dev->SetRxCallBack(MakeCallback(&SamplesRoutingRouter::HandleMsg, h3_r));

    //setup router
    h1_r->BuildRouterTable(host2->GetAddress(), h1_dev);
    h1_r->BuildRouterTable(host3->GetAddress(), h1_dev);
    h2_r->BuildRouterTable(host1->GetAddress(), h2_dev);
    h3_r->BuildRouterTable(host1->GetAddress(), h3_h1_dev);
    h3_r->BuildRouterTable(host2->GetAddress(), h3_h2_dev);

    h1_app->StartApplication(1500);
    h2_app->StartApplication(1500);
    h3_app->StartApplication(1500);

    //set dest address
    h1_app->SetupDestAddr(host2->GetAddress());
    h3_app->SetupDestAddr(host2->GetAddress());
    h2_app->SetupDestAddr(host1->GetAddress());

    Simulator::Run();
    Simulator::Destroy();
}
