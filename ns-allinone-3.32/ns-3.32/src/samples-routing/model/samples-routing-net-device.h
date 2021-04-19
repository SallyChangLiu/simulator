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

#ifndef SAMPLES_ROUTING_NETDEVICE_H
#define SAMPLES_ROUTING_NETDEVICE_H

#include "ns3/address.h"
#include "ns3/node.h"
#include "ns3/net-device.h"
#include "ns3/callback.h"
#include "ns3/traced-callback.h"
#include "ns3/nstime.h"
#include "ns3/data-rate.h"
#include "ns3/ptr.h"
#include "ns3/mac48-address.h"
#include "samples-routing-queue.h"
#include "samples-routing-packet.h"
#include <queue>

namespace ns3
{

    /**
     * \ingroup samples-routing
     * \brief The samples-routing-net-device class specializes the NetDevice abstract
     * base class.
     */

    class SamplesRoutingNetDevice : public NetDevice
    {
    public:
        /**
         * \brief Get the TypeId
         *
         * \return The TypeId for this class
         */
        static TypeId GetTypeId(void);

        /**
         * Construct a SamplesRoutingNetDevice
         *
         * This is the constructor for the SamplesRoutingNetDevice.  It takes as a
         * parameter a pointer to the Node to which this device is connected,
         * as well as an optional DataRate object.
         */
        SamplesRoutingNetDevice();

        /**
         * Destroy a SamplesRoutingNetDevice
         *
         * This is the destructor for the SamplesRoutingNetDevice.
         */
        virtual ~SamplesRoutingNetDevice();

        /**
         * Attach a queue to the SamplesRoutingNetDevice.
         *
         * The SamplesRoutingNetDevice "owns" a queue that implements a queueing
         * method such as DropTailQueue or RedQueue
         *
         * \param queue Ptr to the new queue.
         */
        void SetQueue(Ptr<Queue<SamplesRoutingPacket>> queue);

        /**
         * Get a copy of the attached Queue.
         *
         * \returns Ptr to the queue.
         */
        Ptr<Queue<SamplesRoutingPacket>> GetQueue(void) const;

        /**
         * Attach the device to a channel.
         *
         * \param ch Ptr to the channel to which this object is being attached.
         * \return true if the operation was successful (always true actually)
         */
        bool Attach (Ptr<SamplesRoutingChannel> ch);

        /**
         * Start transmit process.
         */
        void TransmitStart (Ptr<SamplesRoutingPacket> p);

        /**
         * Receive a packet from a connected Channel.
         *
         * The NetDevice receives packets from its connected channel
         * and forwards them up the protocol stack.  This is the public method
         * used by the channel to indicate that the last bit of a packet has
         * arrived at the device.
         *
         * \param p Ptr to the received packet.
         */
        void Receive (Ptr<SamplesRoutingPacket> p);

        virtual void SetReceiveCallback (NetDevice::ReceiveCallback cb);

    private:
        Ptr<Queue<SamplesRoutingPacket>> m_queue;
        Ptr<Node> m_node;
        Ptr<SamplesRoutingChannel> m_channel;
        bool m_linkUp;          //!< Identify if the link is up or not
        Mac48Address m_address; //!< Mac48Address of this NetDevice

        NetDevice::ReceiveCallback m_rxCallback; //!< Receive callback
    };

} // namespace ns3

#endif