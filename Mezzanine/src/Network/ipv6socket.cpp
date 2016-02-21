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

#ifndef _networkipv6socket_cpp
#define _networkipv6socket_cpp

#include "Network/platformsocket.h.cpp"

#include "Network/ipv6socket.h"

namespace Mezzanine
{
    namespace Network
    {
        IPv6Socket::IPv6Socket()
            {  }

        IPv6Socket::~IPv6Socket()
            {  }

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        NetworkLayerProtocol IPv6Socket::GetNetworkLayerProtocol() const
            { return Network::NLP_IPv6; }

        ///////////////////////////////////////////////////////////////////////////////
        // Configuration

        void IPv6Socket::SetNumUnicastHops(const Integer Hops)
        {
            #if defined(MEZZ_WINDOWS)
            DWORD Value = ( Hops < 0 ? 0 : Hops );
            this->InternalSocket->SetSocketOption(Network::Sol_IPv6,Network::IPv6Opts_UnicastHops,(char*)&Value,sizeof(Value));
            #elif defined(MEZZ_MACOSX) || defined(MEZZ_LINUX)
            this->InternalSocket->SetSocketOption(Network::Sol_IPv6,Network::IPv6Opts_UnicastHops,(char*)&Hops,sizeof(Hops));
            #endif
        }

        Integer IPv6Socket::GetNumUnicastHops() const
        {
            #if defined(MEZZ_WINDOWS)
            DWORD Value = 0;
            #elif defined(MEZZ_MACOSX) || defined(MEZZ_LINUX)
            int Value = 0;
            #endif
            AddrLen Size;
            this->InternalSocket->GetSocketOption(Network::Sol_IPv6,Network::IPv6Opts_UnicastHops,(char*)&Value,&Size);
            return static_cast<Integer>(Value);
        }

        void IPv6Socket::SetV6Only(const Boole V6Only)
        {
            int Value = ( V6Only ? 1 : 0 );
            this->InternalSocket->SetSocketOption(Network::Sol_IPv6,Network::IPv6Opts_V6Only,(char*)&Value,sizeof(Value));
        }

        Boole IPv6Socket::GetV6Only() const
        {
            int Value = 0;
            AddrLen Size;
            this->InternalSocket->GetSocketOption(Network::Sol_IPv6,Network::IPv6Opts_V6Only,(char*)&Value,&Size);
            return ( Value != 0 );
        }
    }//Network
}//Mezzanine

#endif

#endif //MEZZNETWORK
