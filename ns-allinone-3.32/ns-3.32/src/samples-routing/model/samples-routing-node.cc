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

#include "samples-routing-node.h"
#include "ns3/ipv4-address.h"

namespace ns3
{
    NS_LOG_COMPONENT_DEFINE("SamplesRoutingNode");
    NS_OBJECT_ENSURE_REGISTERED(SamplesRoutingNode);

    TypeId
    SamplesRoutingNode::GetTypeId(void)
    {
        static TypeId tid = TypeId("ns3::SamplesRoutingNode")
                                .SetParent<Node>()
                                .SetGroupName("samples-routing")
                                .AddConstructor<SamplesRoutingNode>();
        return tid;
    }

    SamplesRoutingNode::SamplesRoutingNode()
    {
        NS_LOG_FUNCTION_NOARGS();
    }

    SamplesRoutingNode::SamplesRoutingNode(Ipv4Address addr):m_address(addr)
    {
        NS_LOG_FUNCTION_NOARGS();
    }

    SamplesRoutingNode::~SamplesRoutingNode()
    {
        NS_LOG_FUNCTION_NOARGS();
    }

    void SamplesRoutingNode::SetAddress(Ipv4Address addr)
    {
        NS_LOG_FUNCTION(addr);
        m_address = addr;
    }
        
    Ipv4Address SamplesRoutingNode::GetAddress()
    {
        return m_address;
    }

    void SamplesRoutingNode::AddRouter(Ptr<SamplesRoutingRouter> r)
    {
        NS_LOG_FUNCTION(r);
        m_router = r;
        m_router->SetNode(this);
    }

    Ptr<SamplesRoutingRouter> SamplesRoutingNode::GetRouter()
    {
        return m_router;
    }
}