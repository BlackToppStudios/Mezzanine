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
        ///////////////////////////////////////////////////////////////////////////////
        // CheckBox Static Members

        const String CheckBox::TypeName = "CheckBox";

        const String CheckBox::EventSelected = "Selected";
        const String CheckBox::EventDeselected = "Deselected";

        ///////////////////////////////////////////////////////////////////////////////
        // CheckBox Methods

        CheckBox::CheckBox(Screen* Parent) :
            Button(Parent),
            SelectLock(false)
            {  }

        CheckBox::CheckBox(const String& RendName, Screen* Parent) :
            Button(RendName,Parent),
            SelectLock(false)
            { this->ConstructCheckbox(); }

        CheckBox::CheckBox(const String& RendName, const UnifiedRect& RendRect, Screen* Parent) :
            Button(RendName,RendRect,Parent),
            SelectLock(false)
            { this->ConstructCheckbox(); }

        CheckBox::CheckBox(const XML::Node& XMLNode, Screen* Parent) :
            Button(Parent),
            SelectLock(false)
            { this->ProtoDeSerialize(XMLNode); }

        CheckBox::~CheckBox()
            {  }

        void CheckBox::ConstructCheckbox()
        {
            // Add our new events
            this->AddEvent(CheckBox::EventSelected);
            this->AddEvent(CheckBox::EventDeselected);
            // Add some more render layer groups
            RenderLayerGroup* SelectedNormalGroup = this->CreateRenderLayerGroup("SelectedNormal");
            RenderLayerGroup* SelectedHoveredGroup = this->CreateRenderLayerGroup("SelectedHovered");

            this->StateGroupBindings[ WS_Selected ] = SelectedNormalGroup;
            this->StateGroupBindings[ WS_Selected | WS_Hovered ] = SelectedHoveredGroup;
            this->StateGroupBindings[ WS_Selected | WS_Focused ] = SelectedNormalGroup;
            this->StateGroupBindings[ WS_Selected | WS_Dragged ] = SelectedNormalGroup;
            this->StateGroupBindings[ WS_Selected | WS_Hovered | WS_Focused ] = SelectedHoveredGroup;
            this->StateGroupBindings[ WS_Selected | WS_Focused | WS_Dragged ] = SelectedNormalGroup;
            this->StateGroupBindings[ WS_Selected | WS_Dragged | WS_Hovered ] = SelectedHoveredGroup;
            this->StateGroupBindings[ WS_Selected | WS_Hovered | WS_Focused | WS_Dragged ] = SelectedHoveredGroup;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Utility Methods

        Boole CheckBox::IsSelected()
        {
            return (this->State & WS_Selected);
        }

        Boole CheckBox::IsLocked()
        {
            return this->SelectLock;
        }

        void CheckBox::ManualSelect(Boole Select)
        {
            if( this->IsSelected() != Select ) {
                Boole NewState = !this->IsSelected();
                if( NewState ) this->_OnSelected();
                else this->_OnDeselected();
            }
        }

        void CheckBox::SetSelectLock(Boole Lock)
        {
            this->SelectLock = Lock;
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
                PropertiesNode.AppendAttribute("SelectLock").SetValue( this->SelectLock ? "true" : "false" ) )
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
                        this->SelectLock = StringTools::ConvertToBool( CurrAttrib.AsString() );
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
            this->Button::_OnActivate();
            // Currently this needs nothing, may change
        }

        void CheckBox::_OnDeactivate()
        {
            this->Button::_OnDeactivate();

            if( this->IsHovered() && !this->SelectLock ) {
                Boole NewState = !this->IsSelected();
                if( NewState ) this->_OnSelected();
                else this->_OnDeselected();
            }
        }

        void CheckBox::_OnSelected()
        {
            this->State |= WS_Selected;
            this->SetGroupFromState(this->State);

            EventArgumentsPtr Args( new WidgetEventArguments(CheckBox::EventSelected,this->Name) );
            this->FireEvent(Args);
        }

        void CheckBox::_OnDeselected()
        {
            this->State &= ~WS_Selected;
            this->SetGroupFromState(this->State);

            EventArgumentsPtr Args( new WidgetEventArguments(CheckBox::EventDeselected,this->Name) );
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
