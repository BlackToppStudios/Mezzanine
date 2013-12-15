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
#ifndef _uicheckbox_cpp
#define _uicheckbox_cpp

#include "UI/uimanager.h"
#include "UI/checkbox.h"
#include "UI/screen.h"

#include "stringtool.h"
#include "exception.h"

namespace Mezzanine
{
    namespace UI
    {
        const String CheckBox::TypeName = "CheckBox";

        const String CheckBox::EventChecked = "Checked";
        const String CheckBox::EventUnchecked = "Unchecked";

        ///////////////////////////////////////////////////////////////////////////////
        // CheckBox Methods

        CheckBox::CheckBox(Screen* Parent) :
            Button(Parent),
            CheckLock(true)
            {  }

        CheckBox::CheckBox(const String& RendName, Screen* Parent) :
            Button(RendName,Parent),
            CheckLock(true)
            { this->ConstructCheckbox(); }

        CheckBox::CheckBox(const String& RendName, const UnifiedRect& RendRect, Screen* Parent) :
            Button(RendName,RendRect,Parent),
            CheckLock(true)
            { this->ConstructCheckbox(); }

        CheckBox::CheckBox(const XML::Node& XMLNode, Screen* Parent) :
            Button(Parent),
            CheckLock(true)
            { this->ProtoDeSerialize(XMLNode); }

        CheckBox::~CheckBox()
            {  }

        void CheckBox::ConstructCheckbox()
        {
            // Add our new events
            this->AddEvent(CheckBox::EventChecked);
            this->AddEvent(CheckBox::EventUnchecked);
            // Add some more render layer groups
            RenderLayerGroup* CheckedNormalGroup = this->CreateRenderLayerGroup("CheckedNormal");
            RenderLayerGroup* CheckedHoveredGroup = this->CreateRenderLayerGroup("CheckedHovered");

            this->StateGroupBindings[ WS_Checked ] = CheckedNormalGroup;
            this->StateGroupBindings[ WS_Checked | WS_Hovered ] = CheckedHoveredGroup;
            this->StateGroupBindings[ WS_Checked | WS_Focused ] = CheckedNormalGroup;
            this->StateGroupBindings[ WS_Checked | WS_Dragged ] = CheckedNormalGroup;
            this->StateGroupBindings[ WS_Checked | WS_Hovered | WS_Focused ] = CheckedHoveredGroup;
            this->StateGroupBindings[ WS_Checked | WS_Focused | WS_Dragged ] = CheckedNormalGroup;
            this->StateGroupBindings[ WS_Checked | WS_Dragged | WS_Hovered ] = CheckedHoveredGroup;
            this->StateGroupBindings[ WS_Checked | WS_Hovered | WS_Focused | WS_Dragged ] = CheckedHoveredGroup;
        }

        void CheckBox::SetChecked(bool Check)
        {
            if( Check ) this->State |= WS_Checked;
            else this->State &= ~WS_Checked;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Utility Methods

        bool CheckBox::IsChecked()
        {
            return (State & WS_Checked);
        }

        bool CheckBox::IsLocked()
        {
            return CheckLock;
        }

        void CheckBox::ManualCheck(bool Check)
        {
            if( IsChecked() == Check )
                return;
            SetChecked( Check );
        }

        void CheckBox::SetCheckLock(bool Lock)
        {
            this->CheckLock = Lock;
        }

        const String& CheckBox::GetTypeName() const
        {
            return CheckBox::TypeName;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Serialization

        void CheckBox::ProtoSerializeProperties(XML::Node& SelfRoot) const
        {
            this->Button::ProtoSerializeProperties(SelfRoot);

            XML::Node PropertiesNode = SelfRoot.AppendChild( CheckBox::GetSerializableName() + "Properties" );

            if( PropertiesNode.AppendAttribute("Version").SetValue("1") &&
                PropertiesNode.AppendAttribute("CheckLock").SetValue( this->CheckLock ? "true" : "false" ) )
            {
                return;
            }else{
                SerializeError("Create XML Attribute Values",CheckBox::GetSerializableName() + "Properties",true);
            }
        }

        void CheckBox::ProtoDeSerializeProperties(const XML::Node& SelfRoot)
        {
            this->Button::ProtoDeSerializeProperties(SelfRoot);

            XML::Attribute CurrAttrib;
            XML::Node PropertiesNode = SelfRoot.GetChild( CheckBox::GetSerializableName() + "Properties" );

            if( !PropertiesNode.Empty() ) {
                if(PropertiesNode.GetAttribute("Version").AsInt() == 1) {
                    CurrAttrib = PropertiesNode.GetAttribute("LockoutTime");
                    if( !CurrAttrib.Empty() )
                        this->CheckLock = StringTools::ConvertToBool( CurrAttrib.AsString() );
                }else{
                    MEZZ_EXCEPTION(Exception::INVALID_VERSION_EXCEPTION,"Incompatible XML Version for " + (CheckBox::GetSerializableName() + "Properties") + ": Not Version 1.");
                }
            }else{
                MEZZ_EXCEPTION(Exception::II_IDENTITY_NOT_FOUND_EXCEPTION,CheckBox::GetSerializableName() + "Properties" + " was not found in the provided XML node, which was expected.");
            }
        }

        String CheckBox::GetSerializableName()
        {
            return CheckBox::TypeName;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Internal Event Methods

        void CheckBox::_OnActivate()
        {
            Button::_OnActivate();
            // Currently this needs nothing, may change
        }

        void CheckBox::_OnDeactivate()
        {
            Button::_OnDeactivate();

            if( this->IsHovered() && !CheckLock )
            {
                bool NewState = !this->IsChecked();
                if( NewState ) this->_OnChecked();
                else this->_OnUnchecked();
            }
        }

        void CheckBox::_OnChecked()
        {
            this->State |= WS_Checked;
            this->SetGroupFromState(State);

            WidgetEventArguments Args(CheckBox::EventChecked,this->Name);
            this->FireEvent(Args);
        }

        void CheckBox::_OnUnchecked()
        {
            if( State & WS_Checked )
            {
                State &= ~WS_Checked;
                this->SetGroupFromState(State);
            }

            WidgetEventArguments Args(CheckBox::EventUnchecked,this->Name);
            this->FireEvent(Args);
        }

        ///////////////////////////////////////////////////////////////////////////////
        // CheckBoxFactory Methods

        String CheckBoxFactory::GetWidgetTypeName() const
            { return CheckBox::TypeName; }

        CheckBox* CheckBoxFactory::CreateCheckBox(const String& RendName, Screen* Parent)
            { return new CheckBox(RendName,Parent); }

        CheckBox* CheckBoxFactory::CreateCheckBox(const String& RendName, const UnifiedRect& RendRect, Screen* Parent)
            { return new CheckBox(RendName,RendRect,Parent); }

        CheckBox* CheckBoxFactory::CreateCheckBox(const XML::Node& XMLNode, Screen* Parent)
            { return new CheckBox(XMLNode,Parent); }

        Widget* CheckBoxFactory::CreateWidget(Screen* Parent)
            { return new CheckBox(Parent); }

        Widget* CheckBoxFactory::CreateWidget(const String& RendName, const NameValuePairMap& Params, Screen* Parent)
            { return this->CreateCheckBox(RendName,Parent); }

        Widget* CheckBoxFactory::CreateWidget(const String& RendName, const UnifiedRect& RendRect, const NameValuePairMap& Params, Screen* Parent)
            { return this->CreateCheckBox(RendName,RendRect,Parent); }

        Widget* CheckBoxFactory::CreateWidget(const XML::Node& XMLNode, Screen* Parent)
            { return this->CreateCheckBox(XMLNode,Parent); }

        void CheckBoxFactory::DestroyWidget(Widget* ToBeDestroyed)
            { delete static_cast<CheckBox*>( ToBeDestroyed ); }
    }//UT
}//Mezzanine

#endif
