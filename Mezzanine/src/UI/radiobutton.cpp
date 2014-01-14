// © Copyright 2010 - 2012 BlackTopp Studios Inc.
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
#ifndef _uiradiobutton_cpp
#define _uiradiobutton_cpp

#include "UI/radiobutton.h"
#include "UI/screen.h"

#include "stringtool.h"
#include "serialization.h"
#include "exception.h"

#include <algorithm>

namespace Mezzanine
{
    namespace UI
    {
        ///////////////////////////////////////////////////////////////////////////////
        // RadioButtonGroup Static Members

        const String RadioButtonGroup::EventGroupButtonSelected = "GroupButtonSelected";

        ///////////////////////////////////////////////////////////////////////////////
        // RadioButtonGroup Methods

        RadioButtonGroup::RadioButtonGroup() :
            CurrentSelection(NULL)
            { this->AddEvent(RadioButtonGroup::EventGroupButtonSelected); }

        RadioButtonGroup::~RadioButtonGroup()
            {  }

        ///////////////////////////////////////////////////////////////////////////////
        // Utility Methods

        void RadioButtonGroup::AddButtonToGroup(RadioButton* ToAdd)
        {
            RadioButtonIterator RadioIt = std::find(this->GroupButtons.begin(),this->GroupButtons.end(),ToAdd);
            if( RadioIt == this->GroupButtons.end() ) {
                this->GroupButtons.push_back( ToAdd );
            }
        }

        Whole RadioButtonGroup::GetNumButtons() const
        {
            return this->GroupButtons.size();
        }

        void RadioButtonGroup::RemoveButtonFromGroup(RadioButton* ToRemove)
        {
            RadioButtonIterator RadioIt = std::find(this->GroupButtons.begin(),this->GroupButtons.end(),ToRemove);
            if( RadioIt != this->GroupButtons.end() ) {
                this->GroupButtons.erase( RadioIt );
            }
        }

        void RadioButtonGroup::SelectButton(RadioButton* ToSelect)
        {
            this->DeselectOtherButtons(ToSelect);
            this->CurrentSelection = ToSelect;
            if( this->CurrentSelection != NULL ) {
                this->CurrentSelection->ManualSelect(true);
            }
        }

        void RadioButtonGroup::DeselectOtherButtons(RadioButton* Exclude)
        {
            for( RadioButtonIterator RadioIt = this->GroupButtons.begin() ; RadioIt != this->GroupButtons.end() ; ++RadioIt )
            {
                if( (*RadioIt) != Exclude ) {
                    (*RadioIt)->ManualSelect(false);
                }
            }
        }

        RadioButton* RadioButtonGroup::GetCurrentSelection() const
            { return this->CurrentSelection; }

        RadioButtonGroup::RadioButtonIterator RadioButtonGroup::RadioButtonBegin()
            { return this->GroupButtons.begin(); }

        RadioButtonGroup::RadioButtonIterator RadioButtonGroup::RadioButtonEnd()
            { return this->GroupButtons.end(); }

        RadioButtonGroup::ConstRadioButtonIterator RadioButtonGroup::RadioButtonBegin() const
            { return this->GroupButtons.begin(); }

        RadioButtonGroup::ConstRadioButtonIterator RadioButtonGroup::RadioButtonEnd() const
            { return this->GroupButtons.end(); }

        ///////////////////////////////////////////////////////////////////////////////
        // Internal Methods

        void RadioButtonGroup::_NotifyButtonSelected(RadioButton* Selected)
        {
            if( this->CurrentSelection != Selected ) {
                this->CurrentSelection = Selected;
                this->DeselectOtherButtons(Selected);

                WidgetEventArguments Args(RadioButtonGroup::EventGroupButtonSelected,Selected->GetName());
                this->FireEvent(Args);
            }
        }

        ///////////////////////////////////////////////////////////////////////////////
        // RadioButton Static Members

        const String RadioButton::TypeName = "RadioButton";

        ///////////////////////////////////////////////////////////////////////////////
        // RadioButton Methods

        RadioButton::RadioButton(Screen* Parent) :
            CheckBox(Parent)
            {  }

        RadioButton::RadioButton(const String& RendName, Screen* Parent) :
            CheckBox(RendName,Parent)
            {  }

        RadioButton::RadioButton(const String& RendName, const UnifiedRect& RendRect, Screen* Parent) :
            CheckBox(RendName,RendRect,Parent)
            {  }

        RadioButton::RadioButton(const XML::Node& XMLNode, Screen* Parent) :
            CheckBox(Parent)
            { this->ProtoDeSerialize(XMLNode); }

        RadioButton::~RadioButton()
            { this->RemoveFromButtonGroup(); }

        void RadioButton::ProtoSerializeImpl(XML::Node& SelfRoot) const
        {
            this->Widget::ProtoSerializeImpl(SelfRoot);
            this->ProtoSerializeGroupButtons(SelfRoot);
        }

        void RadioButton::ProtoDeSerializeImpl(const XML::Node& SelfRoot)
        {
            this->Widget::ProtoDeSerializeImpl(SelfRoot);
            this->ProtoDeSerializeGroupButtons(SelfRoot);
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Utility Methods

        ///////////////////////////////////////////////////////////////////////////////
        // RadioButton Properties

        ///////////////////////////////////////////////////////////////////////////////
        // RadioButton Configuration

        void RadioButton::AddToButtonGroup(RadioButton* ToAdd)
        {
            RadioButtonGroup* OtherGroup = ToAdd->_GetButtonGroup();
            if( this->ButtonGroup != NULL ) {
                if( OtherGroup == NULL ) {
                    this->ButtonGroup->AddButtonToGroup(ToAdd);
                }else{
                    MEZZ_EXCEPTION(Exception::INVALID_STATE_EXCEPTION,"Attempting to add RadioButton to another group when it already has one.");
                }
            }else{
                if( OtherGroup != NULL ) {
                    OtherGroup->AddButtonToGroup(this);
                }else{
                    OtherGroup = new RadioButtonGroup();
                    OtherGroup->AddButtonToGroup(this);
                    OtherGroup->AddButtonToGroup(ToAdd);
                }
            }
        }

        void RadioButton::RemoveFromButtonGroup()
        {
            if( this->ButtonGroup != NULL ) {
                this->ButtonGroup->RemoveButtonFromGroup(this);

                if( this->ButtonGroup->GetNumButtons() == 0 ) {
                    delete this->ButtonGroup;
                }

                this->ButtonGroup = NULL;
            }
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Serialization

        void RadioButton::ProtoSerializeGroupButtons(XML::Node& SelfRoot) const
        {
            XML::Node ButtonsNode = SelfRoot.AppendChild( "GroupButtons" );

            if( ButtonsNode.AppendAttribute("Version").SetValue("1") ) {
                RadioButtonGroup::ConstRadioButtonIterator ButBeg = this->ButtonGroup->RadioButtonBegin();
                RadioButtonGroup::ConstRadioButtonIterator ButEnd = this->ButtonGroup->RadioButtonEnd();
                for( RadioButtonGroup::ConstRadioButtonIterator ButIt = ButBeg ; ButIt != ButEnd ; ++ButIt )
                {
                    XML::Node ButtonNode = ButtonsNode.AppendChild( "GroupButton" );

                    if( ButtonNode.AppendAttribute("Version").SetValue("1") &&
                        ButtonNode.AppendAttribute("GroupButtonName").SetValue( (*ButIt)->GetName() ) )
                    {
                        continue;
                    }else{
                        SerializeError("Create XML Version Attribute","GroupButton",true);
                    }
                }
            }else{
                SerializeError("Create XML Version Attribute","GroupButtons",true);
            }
        }

        void RadioButton::ProtoSerializeProperties(XML::Node& SelfRoot) const
        {
            this->CheckBox::ProtoSerializeProperties(SelfRoot);
        }

        void RadioButton::ProtoDeSerializeGroupButtons(const XML::Node& SelfRoot)
        {
            this->RemoveFromButtonGroup();

            XML::Attribute CurrAttrib;
            XML::Node ButtonsNode = SelfRoot.GetChild( "GroupButtons" );

            if( !ButtonsNode.Empty() ) {
                if( ButtonsNode.GetAttribute("Version").AsInt() == 1 ) {
                    for( XML::NodeIterator ButtonNodeIt = ButtonsNode.begin() ; ButtonNodeIt != ButtonsNode.end() ; ++ButtonNodeIt )
                    {
                        if( (*ButtonNodeIt).GetAttribute("Version").AsInt() == 1 ) {
                            String GroupButtonName;

                            CurrAttrib = (*ButtonNodeIt).GetAttribute("GroupButtonName");
                            if( !CurrAttrib.Empty() )
                                GroupButtonName = CurrAttrib.AsString();

                            if( !GroupButtonName.empty() ) {
                                Widget* NamedButton = this->ParentScreen->GetWidget(GroupButtonName);
                                if( NamedButton != NULL && NamedButton->GetTypeName() == RadioButton::TypeName ) {
                                    this->AddToButtonGroup( static_cast<RadioButton*>( NamedButton ) );
                                }else{
                                    StringStream ExceptionStream;
                                    ExceptionStream << "Named Widget \"" << GroupButtonName << "\" not found or not a RadioButton when deserializing Widget named \"" << this->GetName() << "\".";
                                    MEZZ_EXCEPTION(Exception::PARAMETERS_EXCEPTION,ExceptionStream.str());
                                }
                            }
                        }else{
                            MEZZ_EXCEPTION(Exception::INVALID_VERSION_EXCEPTION,"Incompatible XML Version for " + String("GroupButtons") + ": Not Version 1.");
                        }
                    }
                }else{
                    MEZZ_EXCEPTION(Exception::INVALID_VERSION_EXCEPTION,"Incompatible XML Version for " + String("GroupButtons") + ": Not Version 1.");
                }
            }
        }

        void RadioButton::ProtoDeSerializeProperties(const XML::Node& SelfRoot)
        {
            this->CheckBox::ProtoDeSerializeProperties(SelfRoot);

            XML::Attribute CurrAttrib;
            XML::Node PropertiesNode = SelfRoot.GetChild( RadioButton::GetSerializableName() + "Properties" );

            if( !PropertiesNode.Empty() ) {
                if(PropertiesNode.GetAttribute("Version").AsInt() == 1) {
                    CurrAttrib = PropertiesNode.GetAttribute("LockoutTime");
                    if( !CurrAttrib.Empty() )
                        this->SelectLock = StringTools::ConvertToBool( CurrAttrib.AsString() );
                }else{
                    MEZZ_EXCEPTION(Exception::INVALID_VERSION_EXCEPTION,"Incompatible XML Version for " + (RadioButton::GetSerializableName() + "Properties") + ": Not Version 1.");
                }
            }else{
                MEZZ_EXCEPTION(Exception::II_IDENTITY_NOT_FOUND_EXCEPTION,RadioButton::GetSerializableName() + "Properties" + " was not found in the provided XML node, which was expected.");
            }
        }

        String RadioButton::GetSerializableName()
        {
            return RadioButton::TypeName;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Internal Event Methods

        void RadioButton::_OnSelected()
        {
            if( this->ButtonGroup != NULL ) {
                this->ButtonGroup->_NotifyButtonSelected(this);
            }
            this->CheckBox::_OnSelected();
        }

        void RadioButton::_OnDeselected()
        {
            // The updated selection button is written to the group first, so we should be able to tell if we need to block deselection.
            // Radio buttons shouldn't deselect themselves.  The initial state should be the only time a radio button group should be without a selection.
            if( this->ButtonGroup ) {
                if( this->ButtonGroup->GetCurrentSelection() != this ) {
                    this->CheckBox::_OnDeselected();
                }
            }else{
                this->CheckBox::_OnDeselected();
            }
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Internal Methods

        RadioButtonGroup* RadioButton::_GetButtonGroup() const
        {
            return this->ButtonGroup;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // RadioButtonFactory Methods

        String RadioButtonFactory::GetWidgetTypeName() const
            { return RadioButton::TypeName; }

        RadioButton* RadioButtonFactory::CreateRadioButton(const String& RendName, Screen* Parent)
            { return new RadioButton(RendName,Parent); }

        RadioButton* RadioButtonFactory::CreateRadioButton(const String& RendName, const UnifiedRect& RendRect, Screen* Parent)
            { return new RadioButton(RendName,RendRect,Parent); }

        RadioButton* RadioButtonFactory::CreateRadioButton(const XML::Node& XMLNode, Screen* Parent)
            { return new RadioButton(XMLNode,Parent); }

        Widget* RadioButtonFactory::CreateWidget(Screen* Parent)
            { return new RadioButton(Parent); }

        Widget* RadioButtonFactory::CreateWidget(const String& RendName, const NameValuePairMap& Params, Screen* Parent)
            { return this->CreateRadioButton(RendName,Parent); }

        Widget* RadioButtonFactory::CreateWidget(const String& RendName, const UnifiedRect& RendRect, const NameValuePairMap& Params, Screen* Parent)
            { return this->CreateRadioButton(RendName,RendRect,Parent); }

        Widget* RadioButtonFactory::CreateWidget(const XML::Node& XMLNode, Screen* Parent)
            { return this->CreateRadioButton(XMLNode,Parent); }

        void RadioButtonFactory::DestroyWidget(Widget* ToBeDestroyed)
            { delete static_cast<RadioButton*>( ToBeDestroyed ); }
    }//UI
}//Mezzanine

#endif
