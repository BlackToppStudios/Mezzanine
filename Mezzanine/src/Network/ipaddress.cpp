// © Copyright 2010 - 2015 BlackTopp Studios Inc.
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

#ifndef _networkipaddress_cpp
#define _networkipaddress_cpp

#include "Network/platformincludes.h.cpp"

#include "Network/ipaddress.h"

#include "exception.h"
#include "serialization.h"

#include <cstring>

namespace Mezzanine
{
    namespace Network
    {
        const UInt16 IPAddress::IPv4BinaryLength = 4;
        const UInt16 IPAddress::IPv6BinaryLength = 16;
        const UInt16 IPAddress::IPv4StringLength = INET_ADDRSTRLEN;
        const UInt16 IPAddress::IPv6StringLength = INET6_ADDRSTRLEN;

        const String IPAddress::IPv4LoopbackAddress = "127.0.0.1";
        const String IPAddress::IPv4BroadcastAddress = "255.255.255.255";
        const String IPAddress::IPv6LoopbackAddress = "::1";

        ///////////////////////////////////////////////////////////////////////////////
        // IPAddress Methods

        IPAddress::IPAddress()
            {  }

        IPAddress::IPAddress(const UInt32 Address, const Boole NBO)
            { this->SetV4Address(Address,NBO); }

        IPAddress::IPAddress(const String& Address)
            { this->SetBinaryAddress( IPAddress::Resolve(Address) ); }

        IPAddress::IPAddress(const AddressContainer& Address)
            { this->SetBinaryAddress(Address); }

        IPAddress::IPAddress(const IPAddress& Other)
            { this->SetBinaryAddress(Other.InternalAddress); }

        IPAddress::~IPAddress()
            {  }

        ///////////////////////////////////////////////////////////////////////////////
        // Static Helpers

        IPAddress::AddressContainer IPAddress::Resolve(const String& Address)
        {
            AddressContainer Ret;

            in_addr v4Test;
            memset(&v4Test,0,sizeof(in_addr));
            #if defined(MEZZ_WINDOWS)
            if( InetPton( AF_INET, Address.c_str(), &v4Test ) > 0 ) {
            #elif defined(MEZZ_MACOSX) || defined(MEZZ_LINUX)
            if( inet_pton( AF_INET, Address.c_str(), &v4Test ) > 0 ) {
            #endif
                Ret.resize(IPv4BinaryLength,0);
                memcpy(&Ret[0],&v4Test.s_addr,IPv4BinaryLength);
                return Ret;
            }

            in6_addr v6Test;
            memset(&v6Test,0,sizeof(in6_addr));
            #if defined(MEZZ_WINDOWS)
            if( InetPton( AF_INET6, Address.c_str(), &v6Test ) > 0 ) {
            #elif defined(MEZZ_MACOSX) || defined(MEZZ_LINUX)
            if( inet_pton( AF_INET6, Address.c_str(), &v6Test ) > 0 ) {
            #endif
                Ret.resize(IPv6BinaryLength,0);
                memcpy(&Ret[0],&v6Test.s6_addr[0],IPv6BinaryLength);
                return Ret;
            }

            return Ret;
        }

        String IPAddress::Resolve(const IPAddress::AddressContainer& Address)
        {
            Network::NetworkLayerProtocol AddressProtocol = Network::NLP_Invalid;
            switch( Address.size() )
            {
                case IPAddress::IPv4BinaryLength:   AddressProtocol = Network::NLP_IPv4;     break;
                case IPAddress::IPv6BinaryLength:   AddressProtocol = Network::NLP_IPv6;     break;
                case 0:
                default:                            AddressProtocol = Network::NLP_Invalid;  break;
            }

            if( AddressProtocol == Network::NLP_IPv4 ) {
                char* AddStr = new char[IPv4StringLength];
                #if defined(MEZZ_WINDOWS)
                if( InetNtop( AF_INET, const_cast<Int8*>(&Address[0]), AddStr, IPv4StringLength ) != NULL ) {
                #elif defined(MEZZ_MACOSX) || defined(MEZZ_LINUX)
                if( inet_ntop( AF_INET, &Address[0], AddStr, IPv4StringLength ) != NULL ) {
                #endif
                    String Ret(AddStr,strlen(AddStr));
                    delete[] AddStr;
                    return Ret;
                }
            }else if( AddressProtocol == Network::NLP_IPv6 ) {
                char* AddStr = new char[IPv6StringLength];
                #if defined(MEZZ_WINDOWS)
                if( InetNtop( AF_INET6, const_cast<Int8*>(&Address[0]), AddStr, IPv6StringLength ) != NULL ) {
                #elif defined(MEZZ_MACOSX) || defined(MEZZ_LINUX)
                if( inet_ntop( AF_INET6, &Address[0], AddStr, IPv6StringLength ) != NULL ) {
                #endif
                    String Ret(AddStr,strlen(AddStr));
                    delete[] AddStr;
                    return Ret;
                }
            }

            return String();
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        Network::NetworkLayerProtocol IPAddress::GetProtocol() const
        {
            switch( this->InternalAddress.size() )
            {
                case IPAddress::IPv4BinaryLength:   return Network::NLP_IPv4;     break;
                case IPAddress::IPv6BinaryLength:   return Network::NLP_IPv6;     break;
                case 0:
                default:                            return Network::NLP_Invalid;  break;
            }
        }

        String IPAddress::GetAsString() const
            { return IPAddress::Resolve(this->InternalAddress); }

        void IPAddress::SetBinaryAddress(const AddressContainer& Address)
            { this->InternalAddress.assign(Address.begin(),Address.end()); }

        const IPAddress::AddressContainer& IPAddress::GetBinaryAddress() const
            { return this->InternalAddress; }

        void IPAddress::SetV4Address(const UInt32 Address, const Boole NBO)
        {
            this->InternalAddress.resize(IPAddress::IPv4BinaryLength);
            *reinterpret_cast<UInt32*>( &this->InternalAddress[0] ) = ( NBO ? Address : htonl(Address) );
        }

        UInt32 IPAddress::GetV4Address(const Boole NBO) const
        {
            if( this->InternalAddress.size() == IPAddress::IPv4BinaryLength ) {
                UInt32 Ret = *reinterpret_cast<const UInt32*>( &this->InternalAddress[0] );
                if( NBO ) {
                    return Ret;
                }else{
                    return ntohl(Ret);
                }
            }
            return 0;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Operators

        void IPAddress::operator=(const IPAddress& Other)
            { this->SetBinaryAddress( Other.InternalAddress ); }

        Boole IPAddress::operator==(const IPAddress& Other) const
        {
            if( this->InternalAddress.size() == Other.InternalAddress.size() ) {
                for( Whole X = 0 ; X < this->InternalAddress.size() ; ++X )
                {
                    if( this->InternalAddress[X] != Other.InternalAddress[X] ) {
                        return false;
                    }
                }
                return true;
            }
            return false;
        }

        Boole IPAddress::operator!=(const IPAddress& Other) const
        {
            return !(this->operator==(Other));
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Serialization

        void IPAddress::ProtoSerialize(XML::Node& ParentNode) const
        {
            XML::Node SelfRoot = ParentNode.AppendChild( IPAddress::GetSerializableName() );

            if( SelfRoot.AppendAttribute("Version").SetValue("1") &&
                SelfRoot.AppendAttribute("Address").SetValue( this->GetAsString() ) )
            {
                return;
            }else{
                SerializeError("Create XML Attribute Values",IPAddress::GetSerializableName(),true);
            }
        }

        void IPAddress::ProtoDeSerialize(const XML::Node& SelfRoot)
        {
            XML::Attribute CurrAttrib;

            if( !SelfRoot.Empty() && SelfRoot.Name() == IPAddress::GetSerializableName() ) {
                if( SelfRoot.GetAttribute("Version").AsInt() == 1 ) {
                    CurrAttrib = SelfRoot.GetAttribute("Address");
                    if( !CurrAttrib.Empty() )
                        this->SetBinaryAddress( IPAddress::Resolve( CurrAttrib.AsString() ) );
                }else{
                    MEZZ_EXCEPTION(ExceptionBase::INVALID_VERSION_EXCEPTION,"Incompatible XML Version for " + IPAddress::GetSerializableName() + ": Not Version 1.");
                }
            }else{
                MEZZ_EXCEPTION(ExceptionBase::II_IDENTITY_NOT_FOUND_EXCEPTION,IPAddress::GetSerializableName() + " was not found in the provided XML node, which was expected.");
            }
        }

        String IPAddress::GetSerializableName()
            { return "IPAddress"; }
    }//Network
}//Mezzanine

#endif

#endif //MEZZNETWORK
