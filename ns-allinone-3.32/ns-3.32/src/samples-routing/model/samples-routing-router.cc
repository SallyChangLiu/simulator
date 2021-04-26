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

#include "samples-routing-router.h"
#include "ns3/ipv4-address.h"
#include "ns3/callback.h"
#include "stdlib.h"
#include <time.h>


namespace ns3
{
    NS_LOG_COMPONENT_DEFINE("SamplesRoutingRouter");
    NS_OBJECT_ENSURE_REGISTERED(SamplesRoutingRouter);

    TypeId
    SamplesRoutingRouter::GetTypeId(void)
    {
        static TypeId tid = TypeId("ns3::SamplesRoutingRouter")
                                .SetParent<Object>()
                                .SetGroupName("samples-routing")
                                .AddConstructor<SamplesRoutingRouter>();
        return tid;
    }
    SamplesRoutingRouter::SamplesRoutingRouter()
    {
        NS_LOG_FUNCTION_NOARGS();

        m_dropBytes = 0;
        m_routerTable.clear();
    }

    SamplesRoutingRouter::~SamplesRoutingRouter()
    {
        NS_LOG_FUNCTION_NOARGS();

        m_dropBytes = 0;
        m_routerTable.clear();
    }

    void SamplesRoutingRouter::DoDispose()
    {
        NS_LOG_FUNCTION(this);
        m_node = 0;
        m_routerTable.clear();
        m_dropBytes = 0;
        Object::DoDispose();
    }

    void SamplesRoutingRouter::HandleMsg(Ptr<SamplesRoutingPacket> p)
    {NS_LOG_UNCOND("\nin router handleMSg");
        Ipv4Address destIp = p->GetDstIp();
        
        if (destIp == m_node->GetAddress()) //send to app receiver
        {
            //TODO:record the outputIf of the packet
            m_rxCallBack(p);
        }
        else //send out
        {
            srand((unsigned)time(NULL));

            uint32_t idx = rand() % (m_routerTable[destIp].size());
            Ptr<SamplesRoutingNetDevice> dev = m_routerTable[destIp][idx];
            p->SetHopCount(p->GetHopCount()+1);
            //TODO:record the outputIf of the packet
            dev->TransmitStart(p);
        }
    }

    void SamplesRoutingRouter::BuildRouterTable(Ipv4Address dstIp, Ptr<SamplesRoutingNetDevice> outPort)
    {
        m_routerTable[dstIp].push_back(outPort);
    }

    void SamplesRoutingRouter::SetupRxCallBack(RxCallBack cc)
    {
        m_rxCallBack = cc;
    }

    void SamplesRoutingRouter::SetNode(Ptr<SamplesRoutingNode> nd)
    {
        m_node = nd;
    }
} // namespace  ns3
