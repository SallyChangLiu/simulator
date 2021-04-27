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
#include "samples-routing-queue.h"

namespace ns3
{

    NS_LOG_COMPONENT_DEFINE("SamplesRoutingQueue");
    NS_OBJECT_ENSURE_REGISTERED(SamplesRoutingQueue);

    TypeId
    SamplesRoutingQueue::GetTypeId(void)
    {
        static TypeId tid = TypeId("ns3::SamplesRoutingQueue")
                                .SetParent<Object>()
                                .SetGroupName("samples-routing")
                                .AddConstructor<SamplesRoutingQueue>();

        return tid;
    }

    SamplesRoutingQueue::SamplesRoutingQueue()
    {
        NS_LOG_FUNCTION(this);
        m_dropBytes = 0;
        m_rxBytes = 0;
        m_txBytes = 0;
        m_queue.clear();
    }
    SamplesRoutingQueue::~SamplesRoutingQueue()
    {
        NS_LOG_FUNCTION(this);
        m_queue.clear();
    }

    void SamplesRoutingQueue::DoDispose()
    {
        NS_LOG_FUNCTION(this);
        m_queue.clear();
        m_rxBytes = 0;
        m_txBytes = 0;
        m_dev = 0;
        m_dropBytes = 0;
        m_frameCapacity = 0;
        Object::DoDispose();
    }

    Ptr<SamplesRoutingPacket> SamplesRoutingQueue::DequeuePkg()
    {NS_LOG_UNCOND("\n in queue dequeue pkg");
        if (m_queue.size() > 0)
        {
            Ptr<SamplesRoutingPacket> p = m_queue.front();
            m_queue.pop_back();
            //TODO: queue length signal
            return p;
        }
        else
        {
            return 0;
        }
    }
    void SamplesRoutingQueue::InqueuPkg(Ptr<SamplesRoutingPacket> p)
    {NS_LOG_UNCOND("\nin queue inqueue pkg");
        if (m_frameCapacity && m_queue.size() >= m_frameCapacity) //transmitter busy and queue full: discarding
        {
            m_dropBytes += p->GetSize();
            //TODO: drop bytes signal
            return;
        }
        else //queuing up
        {
            Time inTime = Simulator::Now();
            p->SetInTime(inTime);
            m_queue.push_back(p);
            //TODO: queue length signal
        }
    }

    uint32_t SamplesRoutingQueue::GetQueueLength()
    {
        NS_LOG_FUNCTION(this);

        return m_queue.size();
    }

    void SamplesRoutingQueue::SetCapacity(uint32_t capacity)
    {
        m_frameCapacity = capacity;
    }
} // namespace ns3