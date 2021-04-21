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

#include "ns3/log.h"
#include "ns3/queue.h"
#include "ns3/simulator.h"
#include "ns3/mac48-address.h"
#include "ns3/llc-snap-header.h"
#include "ns3/error-model.h"
#include "ns3/trace-source-accessor.h"
#include "ns3/uinteger.h"
#include "ns3/pointer.h"
#include "ns3/object.h"
#include "samples-routing-channel.h"
#include "samples-routing-packet.h"
#include "samples-routing-net-device.h"
#include "samples-routing-queue.h"

namespace ns3
{

    NS_LOG_COMPONENT_DEFINE("SamplesRoutingNetDevice");
    NS_OBJECT_ENSURE_REGISTERED(SamplesRoutingNetDevice);

    TypeId
    SamplesRoutingNetDevice::GetTypeId(void)
    {
        static TypeId tid = TypeId("ns3::SamplesRoutingNetDevice")
                                .SetParent<NetDevice>()
                                .SetGroupName("samples-routing")
                                .AddConstructor<SamplesRoutingNetDevice>()
                                .AddAttribute("Address", "The MAC address of this device.", Mac48AddressValue(Mac48Address("ff:ff:ff:ff:ff:ff")),
                                              MakeMac48AddressAccessor(&SamplesRoutingNetDevice::m_address),
                                              MakeMac48AddressChecker())
                                .AddAttribute("TxQueue", "A queue to use as the transmit queue in the device.",
                                              PointerValue(), MakePointerAccessor(&SamplesRoutingNetDevice::m_queue),
                                              MakePointerChecker<Queue<SamplesRoutingPacket>>())
                                .AddAttribute("DataRate", "The default data rate for point to point links",
                                              DataRateValue(DataRate("32768b/s")),
                                              MakeDataRateAccessor(&SamplesRoutingNetDevice::m_rate), MakeDataRateChecker());

        return tid;
    }

    SamplesRoutingNetDevice::SamplesRoutingNetDevice()
    {
        NS_LOG_FUNCTION(this);
    }

    SamplesRoutingNetDevice::~SamplesRoutingNetDevice()
    {
        NS_LOG_FUNCTION(this);
    }

    void
    SamplesRoutingNetDevice::SetQueue(Ptr<Queue<SamplesRoutingPacket>> q)
    {
        NS_LOG_FUNCTION(this << q);
        m_queue = q;
    }

    Ptr<Queue<SamplesRoutingPacket>>
    SamplesRoutingNetDevice::GetQueue(void) const
    {
        NS_LOG_FUNCTION(this);
        return m_queue;
    }

    void SamplesRoutingNetDevice::TransmitStart(Ptr<SamplesRoutingPacket> p)
    {
        NS_LOG_FUNCTION(this << p);
        //TODO:m_queue push in the packet
    }
    void SamplesRoutingNetDevice::Receive(Ptr<SamplesRoutingPacket> p)
    {
        NS_LOG_FUNCTION(this << p);

        uint16_t protocol = 0;

        //TODO: rigister call back (app receiver)
        //m_rxCallback(this, p, protocol, GetRemote()); // Other protocols
    }

    void SamplesRoutingNetDevice::SetDataRate(DataRate rate)
    {
        NS_LOG_FUNCTION(this << rate);
        m_rate = rate;
    }

    DataRate SamplesRoutingNetDevice::GetDataRate()
    {
        return m_rate;
    }

} // namespace ns3