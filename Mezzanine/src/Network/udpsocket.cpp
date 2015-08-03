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

#ifndef _networkudpsocket_cpp
#define _networkudpsocket_cpp

#include "Network/platformsocket.h.cpp"

#include "Network/udpsocket.h"

// Because MinGW doesn't have it's shit together
#ifdef MEZZ_WIN_MINGW
#ifndef MCAST_JOIN_GROUP
#define MCAST_JOIN_GROUP          41
#endif//MCAST_JOIN_GROUP
#ifndef MCAST_LEAVE_GROUP
#define MCAST_LEAVE_GROUP         42
#endif//MCAST_LEAVE_GROUP
#ifndef MCAST_BLOCK_SOURCE
#define MCAST_BLOCK_SOURCE        43
#endif//MCAST_BLOCK_SOURCE
#ifndef MCAST_UNBLOCK_SOURCE
#define MCAST_UNBLOCK_SOURCE      44
#endif//MCAST_UNBLOCK_SOURCE
#ifndef MCAST_JOIN_SOURCE_GROUP
#define MCAST_JOIN_SOURCE_GROUP   45
#endif//MCAST_JOIN_SOURCE_GROUP
#ifndef MCAST_LEAVE_SOURCE_GROUP
#define MCAST_LEAVE_SOURCE_GROUP  46
#endif//MCAST_LEAVE_SOURCE_GROUP
#ifndef MCAST_MSFILTER
#define MCAST_MSFILTER            47
#endif//MCAST_MSFILTER
#endif//MEZZ_WIN_MINGW

namespace Mezzanine
{
    namespace Network
    {
        UDPSocket::UDPSocket()
            {  }

        UDPSocket::~UDPSocket()
            {  }

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        TransportLayerProtocol UDPSocket::GetTransportLayerProtocol() const
            { return Network::TLP_UDP; }

        ///////////////////////////////////////////////////////////////////////////////
        // Core Operations

        Integer UDPSocket::Send(const void* Buffer, const UInt32 BufSize, const SystemAddress& Address) const
            { return this->InternalSocket->SendTo(Buffer,BufSize,0,Address); }

        Integer UDPSocket::Receive(void* Buffer, const UInt32 BufSize, SystemAddress& Address) const
            { return this->InternalSocket->ReceiveFrom(Buffer,BufSize,0,Address); }

        Integer UDPSocket::ReceivePeek(void* Buffer, const UInt32 BufSize, SystemAddress& Address) const
            { return this->InternalSocket->ReceiveFrom(Buffer,BufSize,MSG_PEEK,Address); }

        ///////////////////////////////////////////////////////////////////////////////
        // Multicasting

        void UDPSocket::JoinMulticastGroup(const SystemAddress& GroupAddress, const UInt32 Interface)
        {
            group_req MCast;
            MCast.gr_interface = Interface;
            MCast.gr_group = ConvertToSocketStorage(GroupAddress);
            this->InternalSocket->SetSocketOption(ConvertLayer3ProtocolType(this->GetNetworkLayerProtocol()),MCAST_JOIN_GROUP,(char*)&MCast,sizeof(MCast));
        }

        void UDPSocket::LeaveMulticastGroup(const SystemAddress& GroupAddress, const UInt32 Interface)
        {
            group_req MCast;
            MCast.gr_interface = Interface;
            MCast.gr_group = ConvertToSocketStorage(GroupAddress);
            this->InternalSocket->SetSocketOption(ConvertLayer3ProtocolType(this->GetNetworkLayerProtocol()),MCAST_LEAVE_GROUP,(char*)&MCast,sizeof(MCast));
        }

        void UDPSocket::JoinMulticastGroupSource(const SystemAddress& GroupAddress, const SystemAddress& SourceAddress, const UInt32 Interface)
        {
            group_source_req MCast;
            MCast.gsr_interface = Interface;
            MCast.gsr_group = ConvertToSocketStorage(GroupAddress);
            MCast.gsr_source = ConvertToSocketStorage(SourceAddress);
            this->InternalSocket->SetSocketOption(ConvertLayer3ProtocolType(this->GetNetworkLayerProtocol()),MCAST_JOIN_SOURCE_GROUP,(char*)&MCast,sizeof(MCast));
        }

        void UDPSocket::LeaveMulticastGroupSource(const SystemAddress& GroupAddress, const SystemAddress& SourceAddress, const UInt32 Interface)
        {
            group_source_req MCast;
            MCast.gsr_interface = Interface;
            MCast.gsr_group = ConvertToSocketStorage(GroupAddress);
            MCast.gsr_source = ConvertToSocketStorage(SourceAddress);
            this->InternalSocket->SetSocketOption(ConvertLayer3ProtocolType(this->GetNetworkLayerProtocol()),MCAST_LEAVE_SOURCE_GROUP,(char*)&MCast,sizeof(MCast));
        }

        void UDPSocket::BlockMulticastSource(const SystemAddress& GroupAddress, const SystemAddress& SourceAddress, const UInt32 Interface)
        {
            group_source_req MCast;
            MCast.gsr_interface = Interface;
            MCast.gsr_group = ConvertToSocketStorage(GroupAddress);
            MCast.gsr_source = ConvertToSocketStorage(SourceAddress);
            this->InternalSocket->SetSocketOption(ConvertLayer3ProtocolType(this->GetNetworkLayerProtocol()),MCAST_BLOCK_SOURCE,(char*)&MCast,sizeof(MCast));
        }

        void UDPSocket::UnblockMulticastSource(const SystemAddress& GroupAddress, const SystemAddress& SourceAddress, const UInt32 Interface)
        {
            group_source_req MCast;
            MCast.gsr_interface = Interface;
            MCast.gsr_group = ConvertToSocketStorage(GroupAddress);
            MCast.gsr_source = ConvertToSocketStorage(SourceAddress);
            this->InternalSocket->SetSocketOption(ConvertLayer3ProtocolType(this->GetNetworkLayerProtocol()),MCAST_UNBLOCK_SOURCE,(char*)&MCast,sizeof(MCast));
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Configuration

        void UDPSocket::SetBroadcast(const Boole Broadcast)
        {
            int Value = ( Broadcast ? 1 : 0 );
            this->InternalSocket->SetSocketOption(Network::Sol_Socket,Network::SockOpts_Broadcast,(char*)&Value,sizeof(Value));
        }

        Boole UDPSocket::GetBroadcast() const
        {
            int Value = 0;
            AddrLen Size;
            this->InternalSocket->GetSocketOption(Network::Sol_Socket,Network::SockOpts_Broadcast,(char*)&Value,&Size);
            return ( Value != 0 );
        }
    }//Network
}//Mezzanine

#endif

#endif //MEZZNETWORK
