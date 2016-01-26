// © Copyright 2010 - 2016 BlackTopp Studios Inc.
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
#ifndef _uibutton_cpp
#define _uibutton_cpp

#include "UI/uimanager.h"
#include "UI/button.h"
#include "UI/screen.h"

namespace Mezzanine
{
    namespace UI
    {
        const String Button::TypeName = "Button";

        const String Button::EventActivated = "Activated";
        const String Button::EventStandby = "Standby";
        const String Button::EventDeactivated = "Deactivated";

        ///////////////////////////////////////////////////////////////////////////////
        // Button Methods

        Button::Button(Screen* Parent) :
            Widget(Parent),
            Activation(AS_Deactivated)
            {  }

        Button::Button(const String& RendName, Screen* Parent) :
            Widget(RendName,Parent),
            Activation(AS_Deactivated)
            { this->ConstructButton(); }

        Button::Button(const String& RendName, const UnifiedRect& RendRect, Screen* Parent) :
            Widget(RendName,RendRect,Parent),
            Activation(AS_Deactivated)
            { this->ConstructButton(); }

        Button::Button(const XML::Node& XMLNode, Screen* Parent) :
            Widget(Parent),
            Activation(AS_Deactivated)
            { this->ProtoDeSerialize(XMLNode); }

        Button::~Button()
            {  }

        void Button::CreateLayoutStrat()
            { this->LayoutStrat = NULL; }

        Boole Button::HandleInputImpl(const Input::MetaCode& Code)
        {
            // Check to see if this is an activation code
            if( Code.IsDeviceButton() && this->ActivationCodes.count( Input::MetaCodeKey(Code) ) ) {
                // Are we changing our activation state from a mouse code?
                if( Code.IsMouseButton() ) {
                    if( this->IsHovered() && Input::BUTTON_PRESSING == Code.GetMetaValue() ) {
                        Boole Result = this->Activate();
                        if(Result)
                            this->MouseActivated = true;
                        return true;
                    }else if( Input::BUTTON_LIFTING == Code.GetMetaValue() ) {
                        this->Deactivate();
                        this->MouseActivated = false;
                        return true;
                    }
                }else{
                    if( Input::BUTTON_PRESSING == Code.GetMetaValue() ) {
                        this->Activate();
                        return true;
                    }else if( Input::BUTTON_LIFTING == Code.GetMetaValue() ) {
                        this->Deactivate();
                        return true;
                    }
                }
            }
            return false;
        }

        void Button::ConstructButton()
        {
            // Add our button events
            this->AddEvent(Button::EventActivated);
            this->AddEvent(Button::EventStandby);
            this->AddEvent(Button::EventDeactivated);
            // Handle auto activation registration
            UIManager* Manager = this->ParentScreen->GetManager();
            if( Manager->ButtonAutoRegisterEnabled() )
            {
                const UIManager::InputContainer& Codes = Manager->GetAutoRegisteredCodes();
                for( Whole X = 0 ; X < Codes.size() ; X++ )
                    this->BindActivationKeyOrButton(Codes.at(X));
            }
        }

        Boole Button::VertifyActivationCode(const Input::InputCode Code)
        {
            return (Input::KEY_FIRST < Code && Input::KEY_LAST > Code) ||
                   (Input::MOUSEBUTTON_FIRST <= Code && Input::MOUSEBUTTON_LAST >= Code) ||
                   (Input::CONTROLLERBUTTON_FIRST <= Code && Input::CONTROLLERBUTTON_LAST >= Code);
        }

        Boole Button::Activate()
        {
            if( this->Activation == AS_Activated )
                return false;
            if( this->LockoutTimer.GetInitialTime() != 0 && !(this->LockoutTimer.IsStopped()) )
                return false;

            this->Activation = AS_Activated;
            this->_OnActivate();
            return true;
        }

        Boole Button::Deactivate()
        {
            if( this->Activation == AS_Deactivated )
                return false;
            if( this->LockoutTimer.GetInitialTime() != 0 )
            {
                this->LockoutTimer.Reset();
                this->LockoutTimer.Start();
            }

            this->Activation = AS_Deactivated;
            this->_OnDeactivate();
            return true;
        }

        Boole Button::Standby()
        {
            if(this->Activation == AS_Activation_Standby)
                return false;

            this->Activation = AS_Activation_Standby;
            this->_OnStandby();
            return true;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Utility Methods

        void Button::SetLockoutTime(const UInt32& Milliseconds)
            { this->LockoutTimer.SetInitialTimeInMilliseconds(Milliseconds); }

        const StopWatchTimer& Button::GetLockoutTimer() const
            { return this->LockoutTimer; }

        Boole Button::IsActivated() const
            { return ( this->Activation == AS_Activated ); }

        Boole Button::IsOnStandby() const
            { return ( this->Activation == AS_Activation_Standby ); }

        Boole Button::IsDeactivated() const
            { return ( this->Activation == AS_Deactivated ); }

        const String& Button::GetTypeName() const
            { return Button::TypeName; }

        ///////////////////////////////////////////////////////////////////////////////
        // Binding Methods

        void Button::BindActivationKeyOrButton(const Input::MetaCode& Code)
        {
            if( this->VertifyActivationCode( Code.GetCode() ) ) {
                this->ActivationCodes.insert(Input::MetaCodeKey(Code));
            }else{
                MEZZ_EXCEPTION(ExceptionBase::PARAMETERS_EXCEPTION,"Attempting to bind an unsupported InputCode to a button.");
            }
        }

        void Button::UnbindActivationKeyOrButton(const Input::MetaCode& Code)
        {
            this->ActivationCodes.erase(Input::MetaCodeKey(Code));
        }

        void Button::UnbindAllKeyboardActivationKeys()
        {
            for( ActivationCodeIterator ActIt = this->ActivationCodes.begin() ; ActIt != this->ActivationCodes.end() ;  )
            {
                if( (*ActIt).GetKeyData().IsKeyboardButton() ) {
                    this->ActivationCodes.erase(ActIt++);
                }else{
                    ++ActIt;
                }
            }
        }

        void Button::UnbindAllMouseActivationButtons()
        {
            for( ActivationCodeIterator ActIt = this->ActivationCodes.begin() ; ActIt != this->ActivationCodes.end() ;  )
            {
                if( (*ActIt).GetKeyData().IsMouseButton() ) {
                    this->ActivationCodes.erase(ActIt++);
                }else{
                    ++ActIt;
                }
            }
        }

        void Button::UnbindAllControllerActivationButtons()
        {
            for( ActivationCodeIterator ActIt = this->ActivationCodes.begin() ; ActIt != this->ActivationCodes.end() ;  )
            {
                if( (*ActIt).GetKeyData().IsControllerButton() ) {
                    this->ActivationCodes.erase(ActIt++);
                }else{
                    ++ActIt;
                }
            }
        }

        void Button::UnbindAllActivationKeysAndButtons()
        {
            this->ActivationCodes.clear();
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Fetch Methods

        const Button::ActivationCodeContainer& Button::GetActivationCodes() const
        {
            return this->ActivationCodes;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Serialization

        void Button::ProtoSerializeProperties(XML::Node& SelfRoot) const
        {
            this->Widget::ProtoSerializeProperties(SelfRoot);

            XML::Node PropertiesNode = SelfRoot.AppendChild( Button::GetSerializableName() + "Properties" );

            if( PropertiesNode.AppendAttribute("Version").SetValue("1") )
            {
                // Only if we have a valid timer
                if( this->LockoutTimer.GetInitialTime() != 0 ) {
                    if( PropertiesNode.AppendAttribute("LockoutTime").SetValue( this->LockoutTimer.GetInitialTime() ) == false ) {
                        SerializeError("Create XML Attribute Values",Button::GetSerializableName() + "Properties",true);
                    }
                }

                XML::Node CodesNode = PropertiesNode.AppendChild( "ActivationCodes" );
                for( ActivationCodeIterator CodeIt = this->ActivationCodes.begin() ; CodeIt != this->ActivationCodes.end() ; ++CodeIt )
                {
                    (*CodeIt).GetKeyData().ProtoSerialize(CodesNode);
                }

                return;
            }else{
                SerializeError("Create XML Attribute Values",Button::GetSerializableName() + "Properties",true);
            }
        }

        void Button::ProtoDeSerializeProperties(const XML::Node& SelfRoot)
        {
            this->Widget::ProtoDeSerializeProperties(SelfRoot);

            XML::Attribute CurrAttrib;
            XML::Node PropertiesNode = SelfRoot.GetChild( Button::GetSerializableName() + "Properties" );

            if( !PropertiesNode.Empty() ) {
                if(PropertiesNode.GetAttribute("Version").AsInt() == 1) {
                    CurrAttrib = PropertiesNode.GetAttribute("LockoutTime");
                    if( !CurrAttrib.Empty() )
                        this->SetLockoutTime( CurrAttrib.AsUint() );

                    XML::Node CodesNode = PropertiesNode.GetChild( "ActivationCodes" );
                    if( !CodesNode.Empty() ) {
                        for( XML::NodeIterator CodeNodeIt = CodesNode.begin() ; CodeNodeIt != CodesNode.end() ; ++CodeNodeIt )
                        {
                            Input::MetaCode ActivationCode;
                            ActivationCode.ProtoDeSerialize( (*CodeNodeIt) );
                            this->BindActivationKeyOrButton(ActivationCode);
                        }
                    }
                }else{
                    MEZZ_EXCEPTION(ExceptionBase::INVALID_VERSION_EXCEPTION,"Incompatible XML Version for " + (Button::GetSerializableName() + "Properties") + ": Not Version 1.");
                }
            }else{
                MEZZ_EXCEPTION(ExceptionBase::II_IDENTITY_NOT_FOUND_EXCEPTION,Button::GetSerializableName() + "Properties" + " was not found in the provided XML node, which was expected.");
            }
        }

        String Button::GetSerializableName()
        {
            return Button::TypeName;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Internal Event Methods

        void Button::_OnMouseEnter()
        {
            Widget::_OnMouseEnter();

            if( this->MouseActivated && this->Activation == AS_Activation_Standby ) {
                this->Activate();
            }
        }

        void Button::_OnMouseExit()
        {
            Widget::_OnMouseExit();

            if( this->MouseActivated && this->Activation == AS_Activated ) {
                this->Standby();
            }
        }

        void Button::_OnActivate()
        {
            EventArgumentsPtr Args( new WidgetEventArguments(Button::EventActivated,this->Name) );
            this->FireEvent(Args);
        }

        void Button::_OnStandby()
        {
            EventArgumentsPtr Args( new WidgetEventArguments(Button::EventStandby,this->Name) );
            this->FireEvent(Args);
        }

        void Button::_OnDeactivate()
        {
            EventArgumentsPtr Args( new WidgetEventArguments(Button::EventDeactivated,this->Name) );
            this->FireEvent(Args);
        }

        ///////////////////////////////////////////////////////////////////////////////
        // ButtonFactory Methods

        String ButtonFactory::GetWidgetTypeName() const
            { return Button::TypeName; }

        Button* ButtonFactory::CreateButton(const String& RendName, Screen* Parent)
            { return new Button(RendName,Parent); }

        Button* ButtonFactory::CreateButton(const String& RendName, const UnifiedRect& RendRect, Screen* Parent)
            { return new Button(RendName,RendRect,Parent); }

        Button* ButtonFactory::CreateButton(const XML::Node& XMLNode, Screen* Parent)
            { return new Button(XMLNode,Parent); }

        Widget* ButtonFactory::CreateWidget(Screen* Parent)
            { return new Button(Parent); }

        Widget* ButtonFactory::CreateWidget(const String& RendName, const NameValuePairMap& Params, Screen* Parent)
            { return this->CreateButton(RendName,Parent); }

        Widget* ButtonFactory::CreateWidget(const String& RendName, const UnifiedRect& RendRect, const NameValuePairMap& Params, Screen* Parent)
            { return this->CreateButton(RendName,RendRect,Parent); }

        Widget* ButtonFactory::CreateWidget(const XML::Node& XMLNode, Screen* Parent)
            { return this->CreateButton(XMLNode,Parent); }

        void ButtonFactory::DestroyWidget(Widget* ToBeDestroyed)
            { delete static_cast<Button*>( ToBeDestroyed ); }
    }//UI
}//Mezzanine

#endif
