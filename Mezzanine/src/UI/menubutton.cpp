//© Copyright 2010 - 2013 BlackTopp Studios Inc.
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
#ifndef _uimenubutton_cpp
#define _uimenubutton_cpp

#include "UI/menubutton.h"
#include "UI/menuentry.h"
#include "UI/screen.h"
#include "stringtool.h"
#include "exception.h"
#include "serialization.h"

namespace Mezzanine
{
    namespace UI
    {
        const String MenuButton::TypeName = "MenuButton";

        ///////////////////////////////////////////////////////////////////////////////
        // MenuButton Methods

        MenuButton::MenuButton(Screen* Parent) :
            Button(Parent),
            BoundMenu(NULL)
            {  }

        MenuButton::MenuButton(const String& RendName, Screen* Parent) :
            Button(RendName,Parent),
            BoundMenu(NULL)
            {  }

        MenuButton::MenuButton(const String& RendName, const UnifiedRect& RendRect, Screen* Parent) :
            Button(RendName,RendRect,Parent),
            BoundMenu(NULL)
            {  }

        MenuButton::MenuButton(const XML::Node& XMLNode, Screen* Parent) :
            Button(Parent),
            BoundMenu(NULL)
            { this->ProtoDeSerialize(XMLNode); }

        MenuButton::~MenuButton()
            {  }

        ///////////////////////////////////////////////////////////////////////////////
        // Serialization

        void MenuButton::ProtoSerializeProperties(XML::Node& SelfRoot) const
        {
            this->Button::ProtoSerializeProperties(SelfRoot);

            XML::Node PropertiesNode = SelfRoot.AppendChild( MenuButton::GetSerializableName() + "Properties" );

            if( PropertiesNode.AppendAttribute("Version").SetValue("1") )
            {
                // Only if we have a valid binding
                if( this->BoundMenu != NULL ) {
                    if( PropertiesNode.AppendAttribute("MenuEntryName").SetValue( this->BoundMenu->GetName() ) &&
                        PropertiesNode.AppendAttribute("IsPushButton").SetValue( this->BoundMenu->GetEntryPushButton() == this ? "true" : "false" ) )
                    {
                        SerializeError("Create XML Attribute Values",MenuButton::GetSerializableName() + "Properties",true);
                    }
                }
            }else{
                SerializeError("Create XML Attribute Values",MenuButton::GetSerializableName() + "Properties",true);
            }
        }

        void MenuButton::ProtoDeSerializeProperties(const XML::Node& SelfRoot)
        {
            this->Button::ProtoDeSerializeProperties(SelfRoot);

            XML::Attribute CurrAttrib;
            XML::Node PropertiesNode = SelfRoot.GetChild( MenuButton::GetSerializableName() + "Properties" );

            if( !PropertiesNode.Empty() ) {
                if(PropertiesNode.GetAttribute("Version").AsInt() == 1) {
                    String MenuName;
                    Bool IsPush = true;

                    CurrAttrib = PropertiesNode.GetAttribute("MenuEntryName");
                    if( !CurrAttrib.Empty() )
                        MenuName = CurrAttrib.AsString();

                    CurrAttrib = PropertiesNode.GetAttribute("MenuEntryName");
                    if( !CurrAttrib.Empty() )
                        IsPush = StringTools::ConvertToBool( CurrAttrib.AsString() );

                    if( !MenuName.empty() ) {
                        Widget* UncastedMenu = this->ParentScreen->GetWidget(MenuName);
                        if( UncastedMenu->GetTypeName() == "MenuEntry" ) {
                            this->BoundMenu = static_cast<MenuEntry*>( UncastedMenu );
                        }else{
                            MEZZ_EXCEPTION(Exception::PARAMETERS_EXCEPTION,"Named Widget that was expected to be a MenuEntry is not a MenuEntry.");
                        }
                    }
                }else{
                    MEZZ_EXCEPTION(Exception::INVALID_VERSION_EXCEPTION,"Incompatible XML Version for " + (MenuButton::GetSerializableName() + "Properties") + ": Not Version 1.");
                }
            }else{
                MEZZ_EXCEPTION(Exception::II_IDENTITY_NOT_FOUND_EXCEPTION,MenuButton::GetSerializableName() + "Properties" + " was not found in the provided XML node, which was expected.");
            }
        }

        String MenuButton::GetSerializableName()
        {
            return MenuButton::TypeName;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // MenuButtonFactory Methods

        String MenuButtonFactory::GetWidgetTypeName() const
            { return Button::TypeName; }

        MenuButton* MenuButtonFactory::CreateMenuButton(const String& RendName, Screen* Parent)
            { return new MenuButton(RendName,Parent); }

        MenuButton* MenuButtonFactory::CreateMenuButton(const String& RendName, const UnifiedRect& RendRect, Screen* Parent)
            { return new MenuButton(RendName,RendRect,Parent); }

        MenuButton* MenuButtonFactory::CreateMenuButton(const XML::Node& XMLNode, Screen* Parent)
            { return new MenuButton(XMLNode,Parent); }

        Widget* MenuButtonFactory::CreateWidget(Screen* Parent)
            { return new MenuButton(Parent); }

        Widget* MenuButtonFactory::CreateWidget(const String& RendName, const NameValuePairMap& Params, Screen* Parent)
            { return this->CreateMenuButton(RendName,Parent); }

        Widget* MenuButtonFactory::CreateWidget(const String& RendName, const UnifiedRect& RendRect, const NameValuePairMap& Params, Screen* Parent)
            { return this->CreateMenuButton(RendName,RendRect,Parent); }

        Widget* MenuButtonFactory::CreateWidget(const XML::Node& XMLNode, Screen* Parent)
            { return this->CreateMenuButton(XMLNode,Parent); }

        void MenuButtonFactory::DestroyWidget(Widget* ToBeDestroyed)
            { delete static_cast<MenuButton*>( ToBeDestroyed ); }
    }//UI
}//Mezzanine

#endif
