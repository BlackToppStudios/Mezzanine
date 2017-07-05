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
#ifndef _entityproxy_cpp
#define _entityproxy_cpp

#include "entityproxy.h"
#include "entity.h"

#include "enumerations.h"
#include "exception.h"
#include "serialization.h"
#include "stringtool.h"

namespace Mezzanine
{
    EntityProxy::EntityProxy()
        {  }

    EntityProxy::EntityProxy(const UInt32 ID) :
        EntityComponent(ID)
        {  }

    ///////////////////////////////////////////////////////////////////////////////
    // Utility

    ///////////////////////////////////////////////////////////////////////////////
    // Serialization

    void EntityProxy::ProtoSerialize(XML::Node& ParentNode) const
    {
        XML::Node SelfRoot = ParentNode.AppendChild(this->GetDerivedSerializableName());
        this->ProtoSerializeInWorld(SelfRoot);
        this->ProtoSerializeProperties(SelfRoot);
    }

    void EntityProxy::ProtoSerializeInWorld(XML::Node& SelfRoot) const
    {
        if( !SelfRoot.AppendAttribute("InWorld").SetValue( this->IsInWorld() ? "true" : "false" ) ) {
            SerializeError("Create XML Attribute Values",EntityProxy::GetSerializableName(),true);
        }
    }

    void EntityProxy::ProtoSerializeProperties(XML::Node& SelfRoot) const
    {
        XML::Node PropertiesNode = SelfRoot.AppendChild( EntityProxy::GetSerializableName() + "Properties" );

        if( PropertiesNode.AppendAttribute("Version").SetValue("1") )
        {
            XML::Node LocationNode = PropertiesNode.AppendChild("Location");
            this->GetLocation().ProtoSerialize( LocationNode );
            XML::Node OrientationNode = PropertiesNode.AppendChild("Orientation");
            this->GetOrientation().ProtoSerialize( OrientationNode );
            XML::Node ScaleNode = PropertiesNode.AppendChild("Scale");
            this->GetScale().ProtoSerialize( ScaleNode );

            return;
        }else{
            SerializeError("Create XML Attribute Values",EntityProxy::GetSerializableName() + "Properties",true);
        }
    }

    void EntityProxy::ProtoDeSerialize(const XML::Node& SelfRoot)
    {
        this->ProtoDeSerializeProperties(SelfRoot);
        this->ProtoDeSerializeInWorld(SelfRoot);
    }

    void EntityProxy::ProtoDeSerializeInWorld(const XML::Node& SelfRoot)
    {
        XML::Attribute InWorldAttrib = SelfRoot.GetAttribute("InWorld");
        if( !InWorldAttrib.Empty() && StringTools::ConvertToBool( InWorldAttrib.AsString() ) ) {
            this->AddToWorld();
        }
    }

    void EntityProxy::ProtoDeSerializeProperties(const XML::Node& SelfRoot)
    {
        XML::Attribute CurrAttrib;
        XML::Node PropertiesNode = SelfRoot.GetChild( EntityProxy::GetSerializableName() + "Properties" );

        if( !PropertiesNode.Empty() ) {
            if(PropertiesNode.GetAttribute("Version").AsInt() == 1) {
                XML::Node PositionNode = PropertiesNode.GetChild("Location").GetFirstChild();
                if( !PositionNode.Empty() ) {
                    Vector3 Loc(PositionNode);
                    this->SetLocation(Loc);
                }

                XML::Node OrientationNode = PropertiesNode.GetChild("Orientation").GetFirstChild();
                if( !PositionNode.Empty() ) {
                    Quaternion Rot(OrientationNode);
                    this->SetOrientation(Rot);
                }

                XML::Node ScaleNode = PropertiesNode.GetChild("Scale").GetFirstChild();
                if( !PositionNode.Empty() ) {
                    Vector3 Scale(ScaleNode);
                    this->SetScale(Scale);
                }
            }else{
                MEZZ_EXCEPTION(ExceptionBase::INVALID_VERSION_EXCEPTION,"Incompatible XML Version for " + (EntityProxy::GetSerializableName() + "Properties" ) + ": Not Version 1.");
            }
        }else{
            MEZZ_EXCEPTION(ExceptionBase::II_IDENTITY_NOT_FOUND_EXCEPTION,EntityProxy::GetSerializableName() + "Properties" + " was not found in the provided XML node, which was expected.");
        }
    }

    String EntityProxy::GetDerivedSerializableName() const
        { return EntityProxy::GetSerializableName(); }

    String EntityProxy::GetSerializableName()
        { return "EntityProxy"; }
}//Mezzanine

#endif
