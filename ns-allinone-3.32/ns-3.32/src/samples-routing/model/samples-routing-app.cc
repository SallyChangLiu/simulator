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
#include "ns3/ipv4-address.h"
#include "ns3/network-module.h"
#include "samples-routing-app.h"
#include "samples-routing-packet.h"
#include "stdlib.h"
#include "ctime"

namespace ns3
{
    NS_LOG_COMPONENT_DEFINE("SamplesRoutingApp");
    NS_OBJECT_ENSURE_REGISTERED(SamplesRoutingApp);

    TypeId
    SamplesRoutingApp::GetTypeId(void)
    {
        static TypeId tid = TypeId("ns3::SamplesRoutingApp")
                                .SetParent<Application>()
                                .SetGroupName("samples-routing")
                                .AddConstructor<SamplesRoutingApp>()
                                .AddTraceSource("PacketRxComplete", "Completing Receiving a packet",
                                                MakeTraceSourceAccessor(&SamplesRoutingApp::m_PacketRxCompleteTrace), "Ptr<SamplesRoutingPacket>");
        return tid;
    }

    SamplesRoutingApp::SamplesRoutingApp()
    {
        NS_LOG_FUNCTION_NOARGS();
    }

    SamplesRoutingApp::~SamplesRoutingApp()
    {
        NS_LOG_FUNCTION_NOARGS();
    }

    void SamplesRoutingApp::SetRouter(Ptr<SamplesRoutingRouter> r)
    {
        NS_LOG_FUNCTION(this << r);
        m_router = r;
    }

    void SamplesRoutingApp::SetSendInterval(Time t)
    {
        NS_LOG_FUNCTION(this << t);
        m_sentInterval = t;
    }

    void SamplesRoutingApp::SetNode(Ptr<SamplesRoutingNode> nd)
    {
        m_node = nd;
    }

    void SamplesRoutingApp::SetupDestAddr(Ipv4Address dstaddr)
    {
        m_destAddress.push_back(dstaddr);
    }

    void SamplesRoutingApp::StartApplication(uint32_t psize)
    {
        srand((unsigned)time(NULL));

        int idx = rand() % (m_destAddress.size());
        Ipv4Address destAddr = m_destAddress[idx];
        Ipv4Address myAddr = m_node->GetAddress();

        Ptr<SamplesRoutingPacket> p = CreateObject<SamplesRoutingPacket>(psize, myAddr, destAddr);
        p->SetCreateTime(Simulator::Now());
        m_router->HandleMsg(p);

        Simulator::Schedule(m_sentInterval, &SamplesRoutingApp::StartApplication, this, psize); //call for next send event
    }

    //TODO:fix
    void SamplesRoutingApp::StopApplication(void)
    {
        m_destAddress.clear();
        m_router = NULL;
        m_sentInterval = Time(0);
    }

    void SamplesRoutingApp::HandleRx(Ptr<SamplesRoutingPacket> p)
    {
        m_PacketRxCompleteTrace(p);
    }

    void SamplesRoutingApp::SetPkgRxCompleteTraceCallback(TracedCallback<Ptr<SamplesRoutingPacket>> cb)
    {
         m_PacketRxCompleteTrace = cb;
    }

} // namespace ns3
