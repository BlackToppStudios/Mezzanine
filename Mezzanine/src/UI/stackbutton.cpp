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
#ifndef _uistackbutton_cpp
#define _uistackbutton_cpp

#include "UI/stackbutton.h"
#include "UI/stackedcontainer.h"
#include "UI/screen.h"

#include "stringtool.h"
#include "exception.h"
#include "serialization.h"

namespace Mezzanine
{
    namespace UI
    {
        ///////////////////////////////////////////////////////////////////////////////
        // StackButton Static Members

        const String StackButton::TypeName = "StackButton";

        ///////////////////////////////////////////////////////////////////////////////
        // StackButton Methods

        StackButton::StackButton(Screen* Parent) :
            Button(Parent),
            BoundContainer(NULL)
            {  }

        StackButton::StackButton(const String& RendName, Screen* Parent) :
            Button(RendName,Parent),
            BoundContainer(NULL)
            {  }

        StackButton::StackButton(const String& RendName, const UnifiedRect& RendRect, Screen* Parent) :
            Button(RendName,RendRect,Parent),
            BoundContainer(NULL)
            {  }

        StackButton::StackButton(const XML::Node& XMLNode, Screen* Parent) :
            Button(Parent),
            BoundContainer(NULL)
            { this->ProtoDeSerialize(XMLNode); }

        StackButton::~StackButton()
            {  }

        ///////////////////////////////////////////////////////////////////////////////
        // Utility Methods

        StackedContainer* StackButton::GetBoundContainer() const
            { return this->BoundContainer; }

        const String& StackButton::GetTypeName() const
            { return StackButton::TypeName; }

        ///////////////////////////////////////////////////////////////////////////////
        // Serialization

        void StackButton::ProtoSerializeProperties(XML::Node& SelfRoot) const
        {
            this->Button::ProtoSerializeProperties(SelfRoot);

            XML::Node PropertiesNode = SelfRoot.AppendChild( StackButton::GetSerializableName() + "Properties" );

            if( PropertiesNode.AppendAttribute("Version").SetValue("1") )
            {
                // Only if we have a valid binding
                if( this->BoundContainer != NULL ) {
                    if( PropertiesNode.AppendAttribute("ContainerName").SetValue( this->BoundContainer->GetName() ) &&
                        PropertiesNode.AppendAttribute("ButtonConfig").SetValue( this->BoundContainer->GetButtonConfig(this) ) )
                    {
                        return;
                    }else{
                        SerializeError("Create XML Attribute Values",StackButton::GetSerializableName() + "Properties",true);
                    }
                }
            }else{
                SerializeError("Create XML Attribute Values",StackButton::GetSerializableName() + "Properties",true);
            }
        }

        void StackButton::ProtoDeSerializeProperties(const XML::Node& SelfRoot)
        {
            this->Button::ProtoDeSerializeProperties(SelfRoot);

            XML::Attribute CurrAttrib;
            XML::Node PropertiesNode = SelfRoot.GetChild( StackButton::GetSerializableName() + "Properties" );

            if( !PropertiesNode.Empty() ) {
                if(PropertiesNode.GetAttribute("Version").AsInt() == 1) {
                    String ContainerName;
                    UInt16 ContainerConfig;

                    CurrAttrib = PropertiesNode.GetAttribute("ContainerName");
                    if( !CurrAttrib.Empty() )
                        ContainerName = CurrAttrib.AsString();

                    CurrAttrib = PropertiesNode.GetAttribute("ButtonConfig");
                    if( !CurrAttrib.Empty() )
                        ContainerConfig = static_cast<UInt16>( CurrAttrib.AsUint() );

                    if( !ContainerName.empty() ) {
                        Widget* UncastedMenu = this->ParentScreen->GetWidget(ContainerName);
                        if( UncastedMenu->GetTypeName() == StackedContainer::TypeName ) {
                            static_cast<StackedContainer*>( UncastedMenu )->SetButtonConfig(ContainerConfig,this);
                        }else{
                            MEZZ_EXCEPTION(ExceptionBase::PARAMETERS_EXCEPTION,"Named Widget that was expected to be a MenuEntry is not a MenuEntry.");
                        }
                    }
                }else{
                    MEZZ_EXCEPTION(ExceptionBase::INVALID_VERSION_EXCEPTION,"Incompatible XML Version for " + (StackButton::GetSerializableName() + "Properties") + ": Not Version 1.");
                }
            }else{
                MEZZ_EXCEPTION(ExceptionBase::II_IDENTITY_NOT_FOUND_EXCEPTION,StackButton::GetSerializableName() + "Properties" + " was not found in the provided XML node, which was expected.");
            }
        }

        String StackButton::GetSerializableName()
            { return StackButton::TypeName; }

        ///////////////////////////////////////////////////////////////////////////////
        // Internal Event Methods

        void StackButton::_OnActivate()
        {
            this->Button::_OnActivate();
            // Currently this needs nothing, may change
        }

        void StackButton::_OnDeactivate()
        {
            this->Button::_OnDeactivate();

            if( this->IsHovered() && this->BoundContainer ) {
                this->BoundContainer->_NotifyButtonSelected(this);
            }
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Internal Methods

        void StackButton::_SetBoundContainer(StackedContainer* ToBeBound)
            { this->BoundContainer = ToBeBound; }

        ///////////////////////////////////////////////////////////////////////////////
        // StackButtonFactory Methods

        String StackButtonFactory::GetWidgetTypeName() const
            { return StackButton::TypeName; }

        StackButton* StackButtonFactory::CreateStackButton(const String& RendName, Screen* Parent)
            { return new StackButton(RendName,Parent); }

        StackButton* StackButtonFactory::CreateStackButton(const String& RendName, const UnifiedRect& RendRect, Screen* Parent)
            { return new StackButton(RendName,RendRect,Parent); }

        StackButton* StackButtonFactory::CreateStackButton(const XML::Node& XMLNode, Screen* Parent)
            { return new StackButton(XMLNode,Parent); }

        Widget* StackButtonFactory::CreateWidget(Screen* Parent)
            { return new StackButton(Parent); }

        Widget* StackButtonFactory::CreateWidget(const String& RendName, const NameValuePairMap& Params, Screen* Parent)
            { return this->CreateStackButton(RendName,Parent); }

        Widget* StackButtonFactory::CreateWidget(const String& RendName, const UnifiedRect& RendRect, const NameValuePairMap& Params, Screen* Parent)
            { return this->CreateStackButton(RendName,RendRect,Parent); }

        Widget* StackButtonFactory::CreateWidget(const XML::Node& XMLNode, Screen* Parent)
            { return this->CreateStackButton(XMLNode,Parent); }

        void StackButtonFactory::DestroyWidget(Widget* ToBeDestroyed)
            { delete static_cast<StackButton*>( ToBeDestroyed ); }
    }//UI
}//Mezzanine

#endif
