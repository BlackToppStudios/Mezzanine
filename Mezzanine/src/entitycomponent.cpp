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
#ifndef _entitycomponent_cpp
#define _entitycomponent_cpp

#include "entitycomponent.h"
#include "serialization.h"
#include "exception.h"

namespace Mezzanine
{
    EntityComponent::EntityComponent() :
        ParentEntity(nullptr),
        ComponentID(0)
        {  }

    EntityComponent::EntityComponent(const EntityComponentID& ID) :
        ParentEntity(nullptr),
        ComponentID(ID)
        {  }

    ///////////////////////////////////////////////////////////////////////////////
    // Utility

    Entity* EntityComponent::GetParentEntity() const
        { return this->ParentEntity; }

    EntityComponentID EntityComponent::GetComponentID() const
        { return this->ComponentID; }

    ///////////////////////////////////////////////////////////////////////////////
    // Serialization

    void EntityComponent::ProtoSerialize(XML::Node& ParentNode) const
    {
        XML::Node SelfRoot = ParentNode.AppendChild(this->GetDerivedSerializableName());
        this->ProtoSerializeProperties(SelfRoot);
    }

    void EntityComponent::ProtoSerializeProperties(XML::Node& SelfRoot) const
    {
        XML::Node PropertiesNode = SelfRoot.AppendChild( EntityComponent::GetSerializableName() + "Properties" );

        if( PropertiesNode.AppendAttribute("Version").SetValue("1") ) {
            XML::Node IDNode = PropertiesNode.AppendChild("ID");
            this->ComponentID.ProtoSerialize( IDNode );

            return;
        }else{
            SerializeError("Create XML Attribute Values",EntityComponent::GetSerializableName() + "Properties",true);
        }
    }

    void EntityComponent::ProtoDeSerialize(const XML::Node& SelfRoot)
    {
        this->ProtoDeSerializeProperties(SelfRoot);
    }

    void EntityComponent::ProtoDeSerializeProperties(const XML::Node& SelfRoot)
    {
        XML::Attribute CurrAttrib;
        XML::Node PropertiesNode = SelfRoot.GetChild( EntityComponent::GetSerializableName() + "Properties" );

        if( !PropertiesNode.Empty() ) {
            if( PropertiesNode.GetAttribute("Version").AsInt() == 1 ) {
                XML::Node IDNode = PropertiesNode.GetChild("ID").GetFirstChild();
                if( !IDNode.Empty() ) {
                    this->ComponentID.ProtoDeSerialize(IDNode);
                }
            }else{
                MEZZ_EXCEPTION( ExceptionBase::INVALID_VERSION_EXCEPTION,
                                "Incompatible XML Version for " + (EntityComponent::GetSerializableName() + "Properties") + ": Not Version 1." );
            }
        }else{
            MEZZ_EXCEPTION( ExceptionBase::II_IDENTITY_NOT_FOUND_EXCEPTION,
                            EntityComponent::GetSerializableName() + "Properties" + " was not found in the provided XML node, which was expected." );
        }
    }

    String EntityComponent::GetDerivedSerializableName() const
        { return EntityComponent::GetSerializableName(); }

    String EntityComponent::GetSerializableName()
        { return "EntityComponent"; }

    ///////////////////////////////////////////////////////////////////////////////
    // Internal Methods

    void EntityComponent::_Bind(Entity* NewParent)
    {
        if( this->ParentEntity != NewParent ) {
            this->ParentEntity = NewParent;
            /// @todo Notify something?  Perhaps use the new event system?
        }
    }
}//Mezzanine

#endif
