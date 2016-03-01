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

#ifndef _networktcpsocket_cpp
#define _networktcpsocket_cpp

#include "Network/platformsocket.h.cpp"

#include "Network/tcpsocket.h"
#include "Network/tcpv4socket.h"
#include "Network/tcpv6socket.h"

namespace Mezzanine
{
    namespace Network
    {
        TCPSocket::TCPSocket()
            {  }

        TCPSocket::~TCPSocket()
            {  }

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        TransportLayerProtocol TCPSocket::GetTransportLayerProtocol() const
            { return Network::TLP_TCP; }

        ///////////////////////////////////////////////////////////////////////////////
        // Core Operations

        Boole TCPSocket::Listen(const Integer Backlog)
            { return this->InternalSocket->Listen(Backlog); }

        TCPSocket* TCPSocket::Accept(SystemAddress& Address)
        {
            PlatformSocket* SockPtr = this->InternalSocket->Accept(Address);
            if( SockPtr != NULL ) {
                if( this->GetNetworkLayerProtocol() == Network::NLP_IPv4 ) {
                    return new TCPv4Socket( SockPtr );
                }else if( this->GetNetworkLayerProtocol() == Network::NLP_IPv6 ) {
                    return new TCPv6Socket( SockPtr );
                }
            }
            return NULL;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Configuration

        void TCPSocket::SetKeepAlive(const Boole Alive)
        {
            int Value = ( Alive ? 1 : 0 );
            this->InternalSocket->SetSocketOption(Network::Sol_Socket,Network::SockOpts_KeepAlive,(char*)&Value,sizeof(Value));
        }

        Boole TCPSocket::GetKeepAlive() const
        {
            int Value = 0;
            AddrLen Size;
            this->InternalSocket->GetSocketOption(Network::Sol_Socket,Network::SockOpts_KeepAlive,(char*)&Value,&Size);
            return ( Value != 0 );
        }

        void TCPSocket::SetNoDelay(const Boole NoDelay)
        {
            int Value = ( NoDelay ? 1 : 0 );
            this->InternalSocket->SetSocketOption(Network::Sol_TCP,Network::TCPOpts_NoDelay,(char*)&Value,sizeof(Value));
        }

        Boole TCPSocket::GetNoDelay() const
        {
            int Value = 0;
            AddrLen Size;
            this->InternalSocket->GetSocketOption(Network::Sol_TCP,Network::TCPOpts_NoDelay,(char*)&Value,&Size);
            return ( Value != 0 );
        }
    }//Network
}//Mezzanine

#endif
