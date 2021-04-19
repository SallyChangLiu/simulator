/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2006 Georgia Tech Research Corporation
 * Copyright (c) 2020 Nanjing University
 *
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
 *
 * Author: ChangLiu  <liuchang_1307@163.com>
 */

#ifndef SAMPLES_ROUTING_PACKET_H
#define SAMPLES_ROUTING_APPLICATION_H

#include "ns3/object.h"
#include "ns3/ipv4-address.h"
#include "ns3/simulator.h"
#include "ns3/network-module.h"

namespace ns3
{
    /**
     * \ingroup samples-routing
     * \class SamplesRoutingPacket
     * \brief packet.
     */

    class SamplesRoutingPacket : public Object
    {
    public:
        uint32_t m_size;
        Ipv4Address m_sIp, m_dIp; //!< source IP, dst IP
        uint32_t m_hopCount;
        Time m_createTime;
        Time m_inTime; //packet inqueue time stamp

        static TypeId GetTypeId(void);
        SamplesRoutingPacket();

        /**
          * constractor
          * \param psize packet size
          * \param sIP source IP
          * \param dIP destination IP
          */
        SamplesRoutingPacket(uint32_t psize, Ipv4Address sIP, Ipv4Address dIP);

    };
} // namespace ns3
#endif