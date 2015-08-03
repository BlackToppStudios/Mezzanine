// © Copyright 2010 - 2014 BlackTopp Studios Inc.
/* This file is part of The Mezzanine Engine.

    The Mezzanine Engine is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The Mezzanine Engine is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with The Mezzanine Engine.  If not, see <http://www.gnu.org/licenses/>.
*/
/* The original authors have included a copy of the license specified above in the
   'Docs' folder. See 'gpl.txt'
*/
/* We welcome the use of the Mezzanine engine to anyone, including companies who wish to
   Build professional software and charge for their product.

   However there are some practical restrictions, so if your project involves
   any of the following you should contact us and we will try to work something
   out:
    - DRM or Copy Protection of any kind(except Copyrights)
    - Software Patents You Do Not Wish to Freely License
    - Any Kind of Linking to Non-GPL licensed Works
    - Are Currently In Violation of Another Copyright Holder's GPL License
    - If You want to change our code and not add a few hundred MB of stuff to
        your distribution

   These and other limitations could cause serious legal problems if you ignore
   them, so it is best to simply contact us or the Free Software Foundation, if
   you have any questions.

   Joseph Toppi - toppij@gmail.com
   John Blackwood - makoenergy02@gmail.com
*/

#ifdef MEZZNETWORK

#ifndef _networkudpsocket_h
#define _networkudpsocket_h

#include "Network/socket.h"
#include "Network/systemaddress.h"

namespace Mezzanine
{
    namespace Network
    {
        ///////////////////////////////////////////////////////////////////////////////
        /// @class UDPSocket
        /// @brief A socket class using the User Datagram Protocol.
        /// @details
        ///////////////////////////////////////
        class MEZZ_LIB UDPSocket : virtual public Socket
        {
        protected:
        public:
            /// @brief Class constructor.
            UDPSocket();
            /// @brief Class destructor.
            virtual ~UDPSocket();

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            /// @copydoc Socket::GetTransportLayerProtocol() const
            TransportLayerProtocol GetTransportLayerProtocol() const;

            ///////////////////////////////////////////////////////////////////////////////
            // Core Operations

            /// @brief Sends raw data out the socket to it's configured peer.
            /// @remarks This method is intended for use on unconnected sockets, although it's use on connected sockets is valid.
            /// @param Buffer A raw buffer to be sent.
            /// @param BufSize The size of the raw buffer to send.
            /// @param Address The address to send the packet to.
            /// @return Returns the number of bytes sent, or -1 if there was an error.
            Integer Send(const void* Buffer, const UInt32 BufSize, const SystemAddress& Address) const;

            /// @brief Reads raw data recieved on this socket from it's configured peer.
            /// @remarks This method is intended for use on unconnected sockets, although it's use on connected sockets is valid.
            /// @param Buffer A raw buffer to be written to.
            /// @param BufSize The size of the raw buffer to write to.
            /// @param Address The address of the packet to retreive.
            /// @return Returns the number of bytes received/written to the buffer, or -1 if there was an error.
            Integer Receive(void* Buffer, const UInt32 BufSize, SystemAddress& Address) const;
            /// @brief Reads raw data recieved on this socket from it's configured peer, but doesn't remove that data from the receive buffer.
            /// @remarks This method is intended for use on unconnected sockets, although it's use on connected sockets is valid.
            /// An additional call to Receive will return the same data returned by this call, as the data wasn't removed from the queue with this method.
            /// @param Buffer A raw buffer to be written to.
            /// @param BufSize The size of the raw buffer to write to.
            /// @param Address The address of the packet to retreive.
            /// @return Returns the number of bytes received/written to the buffer, or -1 if there was an error.
            Integer ReceivePeek(void* Buffer, const UInt32 BufSize, SystemAddress& Address) const;

            ///////////////////////////////////////////////////////////////////////////////
            // Multicasting

            /// @brief Subscribes this socket to a multicast group.
            /// @param GroupAddress The remote address of the multicast group to subscribe to.
            /// @param Interface The index of the local interface on which the multicast group should be joined.  This can be zero to accept any viable interface.
            void JoinMulticastGroup(const SystemAddress& GroupAddress, const UInt32 Interface = 0);
            /// @brief Unsubscribes this socket from a multicast group.
            /// @param GroupAddress The remote address of the multicast group to subscribe to.
            /// @param Interface The index of the local interface on which the multicast group should be dropped.  This can be zero to accept any viable interface.
            void LeaveMulticastGroup(const SystemAddress& GroupAddress, const UInt32 Interface = 0);
            /// @brief Subscribes this socket to specific source within a multicast group.
            /// @param GroupAddress The remote address of the multicast group to subscribe to.
            /// @param SourceAddress The remote address of a specific multicast transmitter to listen to.
            /// @param Interface The index of the local interface on which the multicast group source should be joined.  This can be zero to accept any viable interface.
            void JoinMulticastGroupSource(const SystemAddress& GroupAddress, const SystemAddress& SourceAddress, const UInt32 Interface = 0);
            /// @brief Unsubscribes this socket from a specific source within a multicast group.
            /// @param GroupAddress The remote address of the multicast group to subscribe to.
            /// @param SourceAddress The remote address of a specific multicast transmitter to stop listening to.
            /// @param Interface The index of the local interface on which the multicast group source should be dropped.  This can be zero to accept any viable interface.
            void LeaveMulticastGroupSource(const SystemAddress& GroupAddress, const SystemAddress& SourceAddress, const UInt32 Interface = 0);
            /// @brief Block transmissions from a specific source within a multicast group.
            /// @param GroupAddress The remote address of the multicast group to subscribe to.
            /// @param SourceAddress The remote address of a specific multicast transmitter to ignore.
            /// @param Interface The index of the local interface on which the multicast group source should be blocked.  This can be zero to accept any viable interface.
            void BlockMulticastSource(const SystemAddress& GroupAddress, const SystemAddress& SourceAddress, const UInt32 Interface = 0);
            /// @brief Unlock transmissions from a specific source within a multicast group.
            /// @param GroupAddress The remote address of the multicast group to subscribe to.
            /// @param SourceAddress The remote address of a specific multicast transmitter to stop ignoring.
            /// @param Interface The index of the local interface on which the multicast group source should be unblocked.  This can be zero to accept any viable interface.
            void UnblockMulticastSource(const SystemAddress& GroupAddress, const SystemAddress& SourceAddress, const UInt32 Interface = 0);

            ///////////////////////////////////////////////////////////////////////////////
            // Configuration

            /// @brief Sets whether or not the socket is allowed to send broadcast messages.
            /// @param Broadcast Whether or not to enable broadcasting on this socket.
            void SetBroadcast(const Boole Broadcast);
            /// @brief Gets whether or not the socket is allowed to send broadcast messages.
            /// @return Returns true if broadcasts are allowed on this socket, false otherwise.
            Boole GetBroadcast() const;

            /// @brief Sets the number of hops permitted when sending to a multicast address before the message is destroyed.
            /// @param Hops The number of hops a multicast message sent from this socket is allowed to make.  Cannot be greater than 255.
            virtual void SetNumMulticastHops(const Integer Hops) = 0;
            /// @brief Gets the number of hops permitted when sending to a multicast address before the message is destroyed.
            /// @return Returns the number of times a message from this socket sent to a multicast address is allowed to be routed before being destroyed.
            virtual Integer GetNumMulticastHops() const = 0;
            /// @brief Sets whether or not this socket will receive multicast trasmissions it sends.
            /// @param Loop Whether or not multicast messages sent by this socket will also be received by this socket.
            virtual void SetMulticastLoop(const Boole Loop) = 0;
            /// @brief Gets whether or not this socket will receive multicast trasmissions it sends.
            /// @return Returns true if this socket will receive it's own multicast transmissions, false otherwise.
            virtual Boole GetMulticastLoop() const = 0;
        };//UDPSocket
    }//Network
}//Mezzanine

#endif

#endif //MEZZNETWORK
