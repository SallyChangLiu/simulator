#include "ns3/core-module.h"

using namespace ns3;

int main(int argc, char *argv[])
{
    Simulator::Run();
    Simulator::Destroy();
    return 0;
}