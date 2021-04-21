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

    SamplesRoutingQueue::SamplesRoutingQueue() : m_dropBytes(0), m_isBusy(false), m_isLinkUp(false), m_rxBytes(0), m_txBytes(0)
    {
        NS_LOG_FUNCTION(this);
        m_queue.clear();
        m_channel = NULL;
    }
    SamplesRoutingQueue::~SamplesRoutingQueue()
    {
        NS_LOG_FUNCTION(this);
        m_channel = NULL;
        m_queue.clear();
    }

    bool SamplesRoutingQueue::Attach(Ptr<SamplesRoutingChannel> ch)
    {
        NS_LOG_FUNCTION(this << &ch);

        m_channel = ch;
        m_isLinkUp = true;
        m_channel->Attach(this);
    }

    void SamplesRoutingQueue::HandleTxMsg(Ptr<SamplesRoutingPacket> p)
    {
        if (m_isBusy == true) // We are currently busy, so just queue up the packet.
        {
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
        else //we are not busy can sent the packet right now
        {
            //TODO: set queuing time = 0
            StartTransmitting(p);
        }
    }

    void SamplesRoutingQueue::HandleRxMsg(Ptr<SamplesRoutingPacket> p)
    {
        m_rxBytes += p->GetSize();
        //TODO: rx bytes signal

        m_dev->Receive(p);//call for dev receiver (pass up)
    }

    void SamplesRoutingQueue::StartTransmitting(Ptr<SamplesRoutingPacket> p)
    {
        m_isBusy = true;
        Time txTime = m_dev->GetDataRate().CalculateBytesTxTime(p->GetSize());

        Simulator::Schedule(txTime, &SamplesRoutingQueue::CompleteTransimit, this);
        //TODO:call for channel transmit func
        m_channel->TransmitStart (p, this, txTime);
    }

    void SamplesRoutingQueue::CompleteTransimit()
    {
        m_isBusy = false;

        if(m_queue.size() > 0)
        {
            Ptr<SamplesRoutingPacket> p = m_queue.front();
            m_queue.pop_back();
            //TODO: queue length signal

            //TODO: caculate the queuing time for p
            StartTransmitting(p);
        }
        
    }
} // namespace ns3