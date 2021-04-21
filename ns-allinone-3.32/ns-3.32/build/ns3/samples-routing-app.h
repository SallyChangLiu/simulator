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

#ifndef SAMPLES_ROUTING_APPLICATION_H
#define SAMPLES_ROUTING_APPLICATION_H

#include "ns3/application.h"
#include "ns3/ptr.h"
#include "ns3/node.h"
#include "ns3/ipv4-address.h"
#include "ns3/core-module.h"
#include "samples-routing-net-device.h"
#include "samples-routing-router.h"
#include <vector>

namespace ns3
{
    /**
     * \ingroup samples-routing
     * \brief The samples-routing-app class is used for generating pacekts
     */

    class SamplesRoutingApp : public Application
    {
    public:
        /**
         * \brief Get the type ID.
         * \return the object TypeId
         */
        static TypeId GetTypeId(void);

        SamplesRoutingApp();
        virtual ~SamplesRoutingApp();

        /**
         * setup router for node
         * \param r the router
         */
        void SetRouter(Ptr<SamplesRoutingRouter> r);

    private:
        /**
         * \brief Application specific startup code
         *
         * The StartApplication method is called at the start time specified by Start
         * This method should be overridden by all or most application
         * subclasses.
         */
        void StartApplication(uint32_t psize);

        /**
         * \brief Application specific shutdown code
         *
         * The StopApplication method is called at the stop time specified by Stop
         * This method should be overridden by all or most application
         * subclasses.
         */
        void StopApplication(void);

        /**
         * receive a packet.
         * \param p the packet need to be received
         */
        void HandleRx(Ptr<SamplesRoutingPacket> p);

        /**
         * The trace source fired for Receiving a packet.
         *
         * \param Ptr pointer of packet
         */
        TracedCallback<Ptr<SamplesRoutingPacket>> m_PacketRxCompleteTrace;


        std::vector<Ipv4Address> m_destAddress; //vector for pacekt destIP
        Ptr<Node> m_node;
        Ptr<SamplesRoutingRouter> m_router;
        Ipv4Address m_myAddress;
        Time m_sentInterval;
    };

} // namespace ns3

#endif /* SAMPLES_ROUTING_APPLICATION_H */