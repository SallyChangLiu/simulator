/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "samples-routing.h"

namespace ns3
{

    boost::bimap<Ipv4Address, std::string> addr2Name;//all host ipv4address <----> host name
    uint32_t pkgseq = 0;//all packet number
} // namespace ns3
