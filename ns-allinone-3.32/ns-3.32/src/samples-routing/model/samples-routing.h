/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
#ifndef SAMPLES_ROUTING_H
#define SAMPLES_ROUTING_H

#include <boost/bimap.hpp>
#include "ns3/ipv4-address.h"
#include <string>

namespace ns3
{

    extern boost::bimap<Ipv4Address, std::string> addr2Name;
    extern uint32_t pkgseq;
}

#endif /* SAMPLES_ROUTING_H */
