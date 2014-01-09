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
#ifndef _uirenderable_cpp
#define _uirenderable_cpp

#include "UI/renderable.h"
#include "UI/uimanager.h"
#include "stringtool.h"
#include "serialization.h"

namespace Mezzanine
{
    namespace UI
    {
        Renderable::Renderable(Screen* Parent) :
            ParentScreen(Parent),
            Visible(true),
            Dirty(true)
            {  }

        Renderable::Renderable(const String& RendName, Screen* Parent) :
            ParentScreen(Parent),
            Visible(true),
            Dirty(true),
            Name(RendName)
            {  }

        Renderable::~Renderable()
            {  }

        void Renderable::ProtoSerializeImpl(XML::Node& SelfRoot) const
            { this->ProtoSerializeProperties(SelfRoot); }

        void Renderable::ProtoDeSerializeImpl(const XML::Node& SelfRoot)
            { this->ProtoDeSerializeProperties(SelfRoot); }

        ///////////////////////////////////////////////////////////////////////////////
        // Utility Methods

        const String& Renderable::GetName() const
            { return this->Name; }

        Screen* Renderable::GetScreen() const
            { return this->ParentScreen; }

        Boolean Renderable::IsWidget() const
            { return this->GetRenderableType() == Renderable::RT_Widget; }

        Boolean Renderable::IsScreen() const
            { return this->GetRenderableType() == Renderable::RT_Screen; }

        ///////////////////////////////////////////////////////////////////////////////
        // Serialization

        void Renderable::ProtoSerialize(XML::Node& ParentNode) const
        {
            XML::Node SelfRoot = ParentNode.AppendChild(this->GetDerivedSerializableName());

            this->ProtoSerializeImpl(SelfRoot);
        }

        void Renderable::ProtoSerializeProperties(XML::Node& SelfRoot) const
        {
            XML::Node PropertiesNode = SelfRoot.AppendChild( Renderable::GetSerializableName() + "Properties" );

            if( PropertiesNode.AppendAttribute("Version").SetValue("1") &&
                PropertiesNode.AppendAttribute("Name").SetValue(this->Name) &&
                PropertiesNode.AppendAttribute("Visible").SetValue( Visible ? "true" : "false" ) )
            {
                return;
            }else{
                SerializeError("Create XML Attribute Values",Renderable::GetSerializableName() + "Properties",true);
            }
        }

        void Renderable::ProtoDeSerialize(const XML::Node& SelfRoot)
        {
            this->ProtoDeSerializeImpl(SelfRoot);
        }

        void Renderable::ProtoDeSerializeProperties(const XML::Node& SelfRoot)
        {
            XML::Attribute CurrAttrib;
            XML::Node PropertiesNode = SelfRoot.GetChild( Renderable::GetSerializableName() + "Properties" );

            if( !PropertiesNode.Empty() ) {
                if(PropertiesNode.GetAttribute("Version").AsInt() == 1) {
                    CurrAttrib = PropertiesNode.GetAttribute("Name");
                    if( !CurrAttrib.Empty() )
                        this->Name = CurrAttrib.AsString();

                    CurrAttrib = PropertiesNode.GetAttribute("Visible");
                    if( !CurrAttrib.Empty() )
                        this->Visible = CurrAttrib.AsBool();
                }else{
                    MEZZ_EXCEPTION(Exception::INVALID_VERSION_EXCEPTION,"Incompatible XML Version for " + (Renderable::GetSerializableName() + "Properties") + ": Not Version 1.");
                }
            }else{
                MEZZ_EXCEPTION(Exception::II_IDENTITY_NOT_FOUND_EXCEPTION,Renderable::GetSerializableName() + "Properties" + " was not found in the provided XML node, which was expected.");
            }
        }

        String Renderable::GetDerivedSerializableName() const
            { return Renderable::GetSerializableName(); }

        String Renderable::GetSerializableName()
            { return "Renderable"; }

        ///////////////////////////////////////////////////////////////////////////////
        // Internal Methods

        bool Renderable::_IsDirty() const
            { return Dirty; }
    }//UI
}//Mezzanine

#endif
