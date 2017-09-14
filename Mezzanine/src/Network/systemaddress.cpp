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

#ifndef _networksystemaddress_cpp
#define _networksystemaddress_cpp

#include "Network/platformincludes.h.cpp"

#include "Network/systemaddress.h"

#include "exception.h"
#include "serialization.h"

namespace Mezzanine
{
    namespace Network
    {
        SystemAddress::SystemAddress() :
            V6Flow(0),
            V6Scope(0),
            Port(0)
            {  }

        SystemAddress::SystemAddress(const IPAddress& TargetAddress) :
            Address(TargetAddress),
            V6Flow(0),
            V6Scope(0),
            Port(0)
            {  }

        SystemAddress::SystemAddress(const UInt16 TargetPort, const Boole NBO) :
            V6Flow(0),
            V6Scope(0)
            { this->SetPort(TargetPort,NBO); }

        SystemAddress::SystemAddress(const IPAddress& TargetAddress, const UInt16 TargetPort, const Boole NBO) :
            Address(TargetAddress),
            V6Flow(0),
            V6Scope(0)
            { this->SetPort(TargetPort,NBO); }

        SystemAddress::SystemAddress(const SystemAddress& Other) :
            Address(Other.Address),
            V6Flow(Other.V6Flow),
            V6Scope(Other.V6Scope),
            Port(Other.Port)
            {  }

        SystemAddress::~SystemAddress()
            {  }

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        void SystemAddress::SetAddress(const IPAddress& TargetAddress)
            { this->Address = TargetAddress; }

        const IPAddress& SystemAddress::GetAddress() const
            { return this->Address; }

        IPAddress& SystemAddress::GetAddress()
            { return this->Address; }

        void SystemAddress::SetV6Flow(const UInt32 Flow)
            { this->V6Flow = Flow; }

        UInt32 SystemAddress::GetV6Flow() const
            { return this->V6Flow; }

        void SystemAddress::SetV6Scope(const UInt32 Scope)
            { this->V6Scope = Scope; }

        UInt32 SystemAddress::GetV6Scope() const
            { return this->V6Scope; }

        void SystemAddress::SetPort(const UInt16 TargetPort, const Boole NBO)
            { this->Port = ( NBO ? TargetPort : htons(TargetPort) ); }

        UInt16 SystemAddress::GetPort(const Boole NBO) const
            { return ( NBO ? this->Port : ntohs(this->Port) ); }

        ///////////////////////////////////////////////////////////////////////////////
        // Operators

        void SystemAddress::operator=(const SystemAddress& Other)
        {
            this->Address = Other.Address;
            this->Port = Other.Port;
            this->V6Flow = Other.V6Flow;
            this->V6Scope = Other.V6Scope;
        }

        Boole SystemAddress::operator==(const SystemAddress& Other) const
        {
            return ( this->Address == Other.Address ) &&
                   ( this->Port == Other.Port ) &&
                   ( this->V6Flow == Other.V6Flow ) &&
                   ( this->V6Scope == Other.V6Scope );
        }

        Boole SystemAddress::operator!=(const SystemAddress& Other) const
        {
            return ( this->Address != Other.Address ) ||
                   ( this->Port != Other.Port ) ||
                   ( this->V6Flow != Other.V6Flow ) ||
                   ( this->V6Scope != Other.V6Scope );
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Serialization

        void SystemAddress::ProtoSerialize(XML::Node& ParentNode) const
        {
            XML::Node SelfRoot = ParentNode.AppendChild( SystemAddress::GetSerializableName() );

            if( SelfRoot.AppendAttribute("Version").SetValue("1") &&
                SelfRoot.AppendAttribute("Port").SetValue( this->GetPort() ) &&
                SelfRoot.AppendAttribute("V6Flow").SetValue( this->V6Flow ) &&
                SelfRoot.AppendAttribute("V6Scope").SetValue( this->V6Scope ) )
            {
                this->Address.ProtoSerialize(SelfRoot);

                return;
            }else{
                SerializeError("Create XML Attribute Values",SystemAddress::GetSerializableName(),true);
            }
        }

        void SystemAddress::ProtoDeSerialize(const XML::Node& SelfRoot)
        {
            XML::Attribute CurrAttrib;

            if( !SelfRoot.Empty() && SelfRoot.Name() == SystemAddress::GetSerializableName() ) {
                if( SelfRoot.GetAttribute("Version").AsInt() == 1 ) {
                    CurrAttrib = SelfRoot.GetAttribute("Port");
                    if( !CurrAttrib.Empty() )
                        this->SetPort( static_cast<UInt16>( CurrAttrib.AsUint() ) );

                    CurrAttrib = SelfRoot.GetAttribute("V6Flow");
                    if( !CurrAttrib.Empty() )
                        this->V6Flow = static_cast<UInt32>( CurrAttrib.AsUint() );

                    CurrAttrib = SelfRoot.GetAttribute("V6Scope");
                    if( !CurrAttrib.Empty() )
                        this->V6Scope = static_cast<UInt32>( CurrAttrib.AsUint() );

                    XML::Node AddressNode = SelfRoot.GetFirstChild();
                    if( !AddressNode.Empty() && AddressNode.Name() == IPAddress::GetSerializableName() ) {
                        this->Address.ProtoDeSerialize(AddressNode);
                    }
                }else{
                    MEZZ_EXCEPTION(ExceptionBase::INVALID_VERSION_EXCEPTION,"Incompatible XML Version for " + SystemAddress::GetSerializableName() + ": Not Version 1.");
                }
            }else{
                MEZZ_EXCEPTION(ExceptionBase::II_IDENTITY_NOT_FOUND_EXCEPTION,SystemAddress::GetSerializableName() + " was not found in the provided XML node, which was expected.");
            }
        }

        String SystemAddress::GetSerializableName()
            { return "SystemAddress"; }
    }
}

#endif
