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

#include "samples-routing-packet.h"
#include "ns3/log.h"

namespace ns3
{
    NS_LOG_COMPONENT_DEFINE("SamplesRoutingPacket");
    NS_OBJECT_ENSURE_REGISTERED(SamplesRoutingPacket);

    TypeId
    SamplesRoutingPacket::GetTypeId(void)
    {
        static TypeId tid = TypeId("ns3::SamplesRoutingPacket")
                                .SetParent<Object>()
                                .SetGroupName("samples-routing")
                                .AddConstructor<SamplesRoutingPacket>();
        return tid;
    }

    SamplesRoutingPacket::SamplesRoutingPacket()
    {
        NS_LOG_FUNCTION_NOARGS();
    }

    SamplesRoutingPacket::SamplesRoutingPacket(uint32_t psize, Ipv4Address sIP, Ipv4Address dIP)
        : m_size(psize),
          m_sIp(sIP),
          m_dIp(dIP),
          m_hopCount(0),
          m_createTime(Time(0))
    {
        NS_LOG_FUNCTION(this << psize << sIP << dIP);
    }

} // namespace ns3
