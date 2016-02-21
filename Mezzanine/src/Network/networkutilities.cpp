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

#ifndef _networkutilities_cpp
#define _networkutilities_cpp

#include "Network/platformsocket.h.cpp"

#include "Network/networkutilities.h"

#include "stringtool.h"

namespace Mezzanine
{
    namespace Network
    {
        UInt16 ConvertHostToNetworkByteOrder(const UInt16 ToConvert)
            { return htons(ToConvert); }

        UInt32 ConvertHostToNetworkByteOrder(const UInt32 ToConvert)
            { return htonl(ToConvert); }

        UInt16 ConvertNetworkToHostByteOrder(const UInt16 ToConvert)
            { return ntohs(ToConvert); }

        UInt32 ConvertNetworkToHostByteOrder(const UInt32 ToConvert)
            { return ntohl(ToConvert); }

        ///////////////////////////////////////////////////////////////////////////////
        // DNS Lookup

        ///////////////////////////////////////////////////////////////////////////////
        // SocketResultContainer Methods

        SocketResultContainer::SocketResultContainer(addrinfo* Results) :
            RefCount(new Integer(1)),
            Addresses(Results),
            CurrAddress(Results)
            {  }

        SocketResultContainer::SocketResultContainer(const SocketResultContainer& Other)
            { this->Aquire(Other); }

        SocketResultContainer::~SocketResultContainer()
            { this->Release(); }

        void SocketResultContainer::Aquire(const SocketResultContainer& Other)
        {
            this->RefCount = Other.RefCount;
            this->Addresses = Other.Addresses;
            this->CurrAddress = Other.Addresses;
            ++(*RefCount);
        }

        void SocketResultContainer::Release()
        {
            if( --(*RefCount) <= 0 ) {
                delete this->RefCount;
                freeaddrinfo(this->Addresses);
            }
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Element Access

        const SocketDescription* SocketResultContainer::GetNextResult()
        {
            if( this->CurrAddress != NULL ) {
                this->CurrDescription = ConvertToSocketDescription( (sockaddr_storage&)(*this->CurrAddress->ai_addr) );
                this->CurrDescription.SocketProtocol = ConvertLayer4ProtocolType(this->CurrAddress->ai_protocol);

                this->CurrAddress = this->CurrAddress->ai_next;
                return &(this->CurrDescription);
            }
            return NULL;
        }

        void SocketResultContainer::Reset()
            { this->CurrAddress = this->Addresses; }

        ///////////////////////////////////////////////////////////////////////////////
        // Resolve Methods

        SocketResultContainer ResolveNumericHost(const IPAddress& Address, const UInt16 Port, const TransportLayerProtocol TLP)
        {
            addrinfo Hints;
            addrinfo* Results;

            memset(&Hints,0,sizeof(Hints));
            Hints.ai_flags = AI_NUMERICHOST;
            Hints.ai_family = ConvertLayer3ProtocolType(Address.GetProtocol());
            Hints.ai_socktype = ConvertToSocketType(TLP);
            Hints.ai_protocol = ( TLP == 0 ? 0 : ConvertLayer4ProtocolType(TLP) );

            int Result = getaddrinfo(Address.GetAsString().c_str(),( Port != 0 ? StringTools::ConvertToString(Port).c_str() : NULL ),&Hints,&Results);
            if( Result != 0 ) {
                MEZZ_EXCEPTION(ExceptionBase::INTERNAL_EXCEPTION,gai_strerror(Result));
            }

            return SocketResultContainer(Results);
        }

        SocketResultContainer ResolveDomainName(const String& DomainName, const UInt16 Port, const NetworkLayerProtocol NLP, const TransportLayerProtocol TLP)
        {
            addrinfo Hints;
            addrinfo* Results;

            memset(&Hints,0,sizeof(Hints));
            Hints.ai_family = ConvertLayer3ProtocolType(NLP);
            Hints.ai_socktype = ConvertToSocketType(TLP);
            Hints.ai_protocol = ( TLP == 0 ? 0 : ConvertLayer4ProtocolType(TLP) );

            int Result = getaddrinfo(DomainName.c_str(),( Port != 0 ? StringTools::ConvertToString(Port).c_str() : NULL ),&Hints,&Results);
            if( Result != 0 ) {
                MEZZ_EXCEPTION(ExceptionBase::INTERNAL_EXCEPTION,gai_strerror(Result));
            }

            return SocketResultContainer(Results);
        }
    }//Network
}//Mezzanine

#endif

#endif //MEZZNETWORK
