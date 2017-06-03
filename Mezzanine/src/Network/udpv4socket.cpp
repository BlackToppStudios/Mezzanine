// © Copyright 2010 - 2017 BlackTopp Studios Inc.
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

#ifndef _networkudpv4socket_cpp
#define _networkudpv4socket_cpp

#include "Network/platformsocket.h.cpp"

#include "Network/udpv4socket.h"

namespace Mezzanine
{
    namespace Network
    {
        UDPv4Socket::UDPv4Socket()
            { this->InternalSocket = new PlatformSocket(PF_INET,SOCK_DGRAM,0); }

        UDPv4Socket::UDPv4Socket(PlatformSocket* InternalSock)
            { this->InternalSocket = InternalSock; }

        UDPv4Socket::UDPv4Socket(const UInt16 Port)
            { this->InternalSocket = new PlatformSocket(PF_INET,SOCK_DGRAM,0,Port); }

        UDPv4Socket::~UDPv4Socket()
        {
            delete this->InternalSocket;
            this->InternalSocket = NULL;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Configuration

        /*void UDPv4Socket::SetDontFragment(const Boole DontFrag)
        {
            int Value = ( DontFrag ? Network::MFV_Frag_Disable : Network::MFV_Frag_Enable );
            this->InternalSocket->SetSocketOption(Network::Sol_IPv4,Network::IPv4Opts_Fragmentation,(char*)&Value,sizeof(Value));
        }

        Boole UDPv4Socket::GetDontFragment() const
        {
            int Value = 0;
            AddrLen Size;
            this->InternalSocket->GetSocketOption(Network::Sol_IPv4,Network::IPv4Opts_Fragmentation,(char*)&Value,&Size);
            return ( Value != 0 );
        }// */

        void UDPv4Socket::SetNumMulticastHops(const Integer Hops)
        {
            #if defined(MEZZ_WINDOWS)
            DWORD Value = ( Hops < 0 ? 0 : Hops );
            this->InternalSocket->SetSocketOption(Network::Sol_IPv4,Network::IPv4Opts_MulticastTTL,(char*)&Value,sizeof(Value));
            #elif defined(MEZZ_MACOSX) || defined(MEZZ_LINUX)
            this->InternalSocket->SetSocketOption(Network::Sol_IPv4,Network::IPv4Opts_MulticastTTL,(char*)&Hops,sizeof(Hops));
            #endif
        }

        Integer UDPv4Socket::GetNumMulticastHops() const
        {
            #if defined(MEZZ_WINDOWS)
            DWORD Value = 0;
            #elif defined(MEZZ_MACOSX) || defined(MEZZ_LINUX)
            int Value = 0;
            #endif
            AddrLen Size;
            this->InternalSocket->GetSocketOption(Network::Sol_IPv4,Network::IPv4Opts_MulticastTTL,(char*)&Value,&Size);
            return static_cast<Integer>(Value);
        }

        void UDPv4Socket::SetMulticastLoop(const Boole Loop)
        {
            int Value = ( Loop ? 1 : 0 );
            this->InternalSocket->SetSocketOption(Network::Sol_IPv4,Network::IPv4Opts_MulticastLoop,(char*)&Value,sizeof(Value));
        }

        Boole UDPv4Socket::GetMulticastLoop() const
        {
            int Value = 0;
            AddrLen Size;
            this->InternalSocket->GetSocketOption(Network::Sol_IPv4,Network::IPv4Opts_MulticastLoop,(char*)&Value,&Size);
            return ( Value != 0 );
        }
    }//Network
}//Mezzanine

#endif
