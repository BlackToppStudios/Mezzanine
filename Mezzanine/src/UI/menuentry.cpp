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
#ifndef _uimenuentry_cpp
#define _uimenuentry_cpp

#include "UI/menuentry.h"
#include "UI/stackbutton.h"
#include "UI/uimanager.h"
#include "UI/screen.h"
#include "UI/layoutstrategy.h"

#include "stringtool.h"
#include "exception.h"
#include "serialization.h"

namespace Mezzanine
{
    namespace UI
    {
        ///////////////////////////////////////////////////////////////////////////////
        // MenuEntry Static Members

        const String MenuEntry::TypeName                    = "MenuEntry";

        ///////////////////////////////////////////////////////////////////////////////
        // MenuEntry Methods

        MenuEntry::MenuEntry(Screen* Parent) :
            StackedContainer(Parent),
            MenuStack(NULL),
            PushButton(NULL),
            PopButton(NULL),
            AutoHideEntry(true)
            {  }

        MenuEntry::MenuEntry(const String& RendName, Screen* Parent) :
            StackedContainer(RendName,Parent),
            MenuStack(NULL),
            PushButton(NULL),
            PopButton(NULL),
            AutoHideEntry(true)
            {  }

        MenuEntry::MenuEntry(const String& RendName, const UnifiedRect& RendRect, Screen* Parent) :
            StackedContainer(RendName,RendRect,Parent),
            MenuStack(NULL),
            PushButton(NULL),
            PopButton(NULL),
            AutoHideEntry(true)
            {  }

        MenuEntry::MenuEntry(const XML::Node& XMLNode, Screen* Parent) :
            StackedContainer(Parent),
            MenuStack(NULL),
            PushButton(NULL),
            PopButton(NULL),
            AutoHideEntry(true)
            {  }

        MenuEntry::~MenuEntry()
            {  }

        Boole MenuEntry::PushOntoStack()
        {
            if( this->MenuStack ) {
                if( ( this->IsRootEntry() && this->MenuStack->empty() ) ||
                    ( !(this->MenuStack->empty()) && this->MenuStack->back() == this->ParentQuad ) )
                {
                    if( !(this->MenuStack->empty()) && this->MenuStack->back()->GetAutoHide() ) {
                        this->MenuStack->back()->Hide();
                    }
                    this->MenuStack->push_back(this);
                    this->Show();
                    return true;
                }
            }
            return false;
        }

        Boole MenuEntry::PopFromStack()
        {
            if( this->MenuStack ) {
                if( this->IsTopOfStack() ) {
                    this->MenuStack->pop_back();
                    if( !this->MenuStack->empty() ) {
                        this->MenuStack->back()->Show();
                    }
                    this->Hide();
                    return true;
                }
            }
            return false;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Utility Methods

        Boole MenuEntry::IsRootEntry() const
        {
            if( this->ParentQuad != NULL && this->ParentQuad->IsWidget() ) {
                return ( static_cast<Widget*>( this->ParentQuad )->GetTypeName() != MenuEntry::TypeName );
            }else{
                return true;
            }
        }

        Boole MenuEntry::IsTopOfStack() const
        {
            if( this->MenuStack && !(this->MenuStack->empty()) ) {
                return ( this->MenuStack->back() == this );
            }
            return false;
        }

        Whole MenuEntry::RollBackToEntry(MenuEntry* RollBackTo)
        {
            if( this->MenuStack ) {
                if( RollBackTo != NULL && !(this->MenuStack->empty()) ) {
                    Whole Ret = 0;
                    MenuEntryIterator MenuBeg = this->MenuStack->begin();
                    MenuEntryIterator MenuEnd = this->MenuStack->end();
                    while( MenuBeg != MenuEnd )
                    {
                        if( (*MenuBeg) == RollBackTo ) {
                            ++MenuBeg;
                            break;
                        }else{
                            ++MenuBeg;
                        }
                    }

                    for( MenuEntryIterator MenuIt = MenuBeg ; MenuIt != MenuEnd ; ++MenuIt )
                    {
                        (*MenuIt)->Hide();
                        ++Ret;
                    }
                    this->MenuStack->erase(MenuBeg,MenuEnd);
                    RollBackTo->Show();
                    return Ret;
                }
            }
            return 0;
        }

        void MenuEntry::ForceRootEntryVisible()
        {
            if( this->IsRootEntry() ) {
                this->PushOntoStack();
            }else{
                static_cast<MenuEntry*>( this->ParentQuad )->ForceRootEntryVisible();
            }
        }

        const String& MenuEntry::GetTypeName() const
        {
            return MenuEntry::TypeName;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Visibility and Priority Methods

        void MenuEntry::SetVisible(Boole CanSee)
        {
            if( CanSee ) {
                if( !this->AutoHideEntry || this->IsTopOfStack() ) {
                    this->Widget::SetVisible(CanSee);
                }
            }else{
                this->Widget::SetVisible(CanSee);
            }
        }

        void MenuEntry::Show()
        {
            if( !this->AutoHideEntry || this->IsTopOfStack() )
                this->Widget::Show();
        }

        void MenuEntry::Hide()
        {
            this->Widget::Hide();
        }

        ///////////////////////////////////////////////////////////////////////////////
        // MenuEntry Properties

        void MenuEntry::SetAutoHide(Boole AutoHide)
            { this->AutoHideEntry = AutoHide; }

        Boole MenuEntry::GetAutoHide() const
            { return this->AutoHideEntry; }

        ///////////////////////////////////////////////////////////////////////////////
        // Menu Configuration

        void MenuEntry::SetButtonConfig(const UInt16 Config, StackButton* ConfigButton)
        {
            switch( Config )
            {
                case MenuEntry::BC_ToggleButton:  this->SetToggleButton(ConfigButton);  break;
                case MenuEntry::BC_PushButton:    this->SetPushButton(ConfigButton);    break;
                case MenuEntry::BC_PopButton:     this->SetPopButton(ConfigButton);     break;
                default:
                { MEZZ_EXCEPTION(ExceptionBase::PARAMETERS_EXCEPTION,"Invalid config passed in while attempting to set a stack button configuration."); }  break;
            }
        }

        UInt16 MenuEntry::GetButtonConfig(const StackButton* ConfigButton) const
        {
            if( this->PushButton == ConfigButton && this->PopButton == ConfigButton ) {
                return MenuEntry::BC_ToggleButton;
            }else if( this->PushButton == ConfigButton ) {
                return MenuEntry::BC_PushButton;
            }else if( this->PopButton == ConfigButton ) {
                return MenuEntry::BC_PopButton;
            }else{
                return MenuEntry::BC_Error;
            }
        }

        void MenuEntry::SetPushButton(StackButton* Push)
        {
            if( this->PushButton != NULL ) {
                this->PushButton->_SetBoundContainer(NULL);
            }

            this->PushButton = Push;

            if( this->PushButton != NULL ) {
                this->PushButton->_SetBoundContainer(this);
            }
        }

        StackButton* MenuEntry::GetPushButton() const
        {
            return this->PushButton;
        }

        void MenuEntry::SetPopButton(StackButton* Pop)
        {
            if( this->PopButton != NULL ) {
                this->PopButton->_SetBoundContainer(NULL);
            }

            this->PopButton = Pop;

            if( this->PopButton != NULL ) {
                this->PopButton->_SetBoundContainer(this);
            }
        }

        StackButton* MenuEntry::GetPopButton() const
        {
            return this->PopButton;
        }

        void MenuEntry::SetToggleButton(StackButton* Toggle)
        {
            if( this->PushButton != NULL ) {
                this->PushButton->_SetBoundContainer(NULL);
            }
            if( this->PopButton != NULL ) {
                this->PopButton->_SetBoundContainer(NULL);
            }

            this->PushButton = Toggle;
            this->PopButton = Toggle;

            if( this->PushButton != NULL ) {
                this->PushButton->_SetBoundContainer(this);
            }
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Serialization

        void MenuEntry::ProtoSerializeProperties(XML::Node& SelfRoot) const
        {
            this->Widget::ProtoSerializeProperties(SelfRoot);

            XML::Node PropertiesNode = SelfRoot.AppendChild( MenuEntry::GetSerializableName() + "Properties" );

            if( PropertiesNode.AppendAttribute("Version").SetValue("1") &&
                PropertiesNode.AppendAttribute("AutoHideEntry").SetValue( this->GetAutoHide() ? "true" : "false" ) )
            {
                // Only if we have valid bindings
                if( this->PushButton != NULL ) {
                    if( PropertiesNode.AppendAttribute("PushButtonName").SetValue( this->PushButton->GetName() ) ) {
                        return;
                    }else{
                        SerializeError("Create XML Attribute Values",MenuEntry::GetSerializableName() + "Properties",true);
                    }
                }

                if( this->PopButton != NULL ) {
                    if( PropertiesNode.AppendAttribute("PopButtonName").SetValue( this->PopButton->GetName() ) ) {
                        return;
                    }else{
                        SerializeError("Create XML Attribute Values",MenuEntry::GetSerializableName() + "Properties",true);
                    }
                }
            }else{
                SerializeError("Create XML Attribute Values",MenuEntry::GetSerializableName() + "Properties",true);
            }
        }

        void MenuEntry::ProtoDeSerializeProperties(const XML::Node& SelfRoot)
        {
            this->Widget::ProtoDeSerializeProperties(SelfRoot);

            XML::Attribute CurrAttrib;
            XML::Node PropertiesNode = SelfRoot.GetChild( MenuEntry::GetSerializableName() + "Properties" );

            if( !PropertiesNode.Empty() ) {
                if(PropertiesNode.GetAttribute("Version").AsInt() == 1) {
                    CurrAttrib = PropertiesNode.GetAttribute("AutoHideEntry");
                    if( !CurrAttrib.Empty() )
                        this->SetAutoHide( StringTools::ConvertToBool( CurrAttrib.AsString(), true ) );

                    CurrAttrib = PropertiesNode.GetAttribute("PushButtonName");
                    if( !CurrAttrib.Empty() ) {
                        Widget* UncastedButton = this->ParentScreen->GetWidget( CurrAttrib.AsString() );
                        if( UncastedButton->GetTypeName() == StackButton::TypeName ) {
                            this->SetPushButton( static_cast<StackButton*>( UncastedButton ) );
                        }else{
                            MEZZ_EXCEPTION(ExceptionBase::PARAMETERS_EXCEPTION,"Expected name of MenuButton when deserializing.  Named widget is not MenuButton.");
                        }
                    }

                    CurrAttrib = PropertiesNode.GetAttribute("PopButtonName");
                    if( !CurrAttrib.Empty() ) {
                        Widget* UncastedButton = this->ParentScreen->GetWidget( CurrAttrib.AsString() );
                        if( UncastedButton->GetTypeName() == StackButton::TypeName ) {
                            this->SetPopButton( static_cast<StackButton*>( UncastedButton ) );
                        }else{
                            MEZZ_EXCEPTION(ExceptionBase::PARAMETERS_EXCEPTION,"Expected name of MenuButton when deserializing.  Named widget is not MenuButton.");
                        }
                    }
                }else{
                    MEZZ_EXCEPTION(ExceptionBase::INVALID_VERSION_EXCEPTION,"Incompatible XML Version for " + (MenuEntry::GetSerializableName() + "Properties") + ": Not Version 1.");
                }
            }else{
                MEZZ_EXCEPTION(ExceptionBase::II_IDENTITY_NOT_FOUND_EXCEPTION,MenuEntry::GetSerializableName() + "Properties" + " was not found in the provided XML node, which was expected.");
            }
        }

        String MenuEntry::GetSerializableName()
        {
            return MenuEntry::TypeName;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Internal Event Methods



        ///////////////////////////////////////////////////////////////////////////////
        // Internal Methods

        MenuEntry::MenuEntryContainer* MenuEntry::_GetMenuStack() const
        {
            return this->MenuStack;
        }

        void MenuEntry::_NotifyStack(MenuEntryContainer* NewStack)
        {
            if( this->MenuStack != NewStack ) {
                this->MenuStack = NewStack;
                this->Hide();

                for( ChildIterator ChildIt = this->ChildWidgets.begin() ; ChildIt != this->ChildWidgets.end() ; ++ChildIt )
                {
                    if( (*ChildIt)->GetTypeName() == MenuEntry::TypeName ) {
                        static_cast<MenuEntry*>( (*ChildIt) )->_NotifyStack(NewStack);
                    }
                }
            }
        }

        void MenuEntry::_NotifyButtonSelected(StackButton* Selected)
        {
            if( Selected != NULL && Selected->IsHovered() ) {
                if( this->PushButton == Selected && this->PopButton == Selected ) {
                    // Since we are toggling, attempt to push first.  It'll automatically do the checks needed for pushing.
                    Boole PushResult = this->PushOntoStack();
                    if( !PushResult ) {
                        // If it failed to push, try popping.
                        Boole PopResult = this->PopFromStack();
                        if( !PopResult ) {
                            // If even that failed, then we almost certainly need to do a rollback
                            Boole IsRoot = this->IsRootEntry();
                            this->RollBackToEntry( IsRoot ? this : static_cast<MenuEntry*>( this->ParentQuad ) );
                            if( !IsRoot ) {
                                // Last attempt
                                this->PushOntoStack();
                            }
                        }
                    }
                }else if( this->PushButton == Selected ) {
                    Boole PushResult = this->PushOntoStack();
                    if( !PushResult ) {
                        // Attempt a rollback
                        Boole IsRoot = this->IsRootEntry();
                        this->RollBackToEntry( IsRoot ? this : static_cast<MenuEntry*>( this->ParentQuad ) );
                        if( !IsRoot ) {
                            // Last attempt
                            this->PushOntoStack();
                        }
                    }
                }else if( this->PopButton == Selected ) {
                    this->PopFromStack();
                    /*Boole PopResult = this->PopFromStack();
                    if( !PopResult ) {
                        // Is there anything to do here?
                    }// */
                }
            }
        }

        void MenuEntry::_NotifyParenthood(QuadRenderable* NewParent)
        {
            if( this->ParentQuad != NewParent ) {
                this->QuadRenderable::_NotifyParenthood(NewParent);

                if( this->ParentQuad != NULL ) {
                    // Do the necessary checks to see if we have an updated stack.
                    MenuEntryContainer* UpdatedStack = ( this->IsRootEntry() ? new MenuEntryContainer() : static_cast<MenuEntry*>( this->ParentQuad )->_GetMenuStack() );
                    if( this->MenuStack != UpdatedStack ) {
                        // Preserve the Old stack for later.
                        MenuEntryContainer* OldStack = this->MenuStack;
                        // Notify all the children of the new stack.
                        this->_NotifyStack(UpdatedStack);
                        // Finally we can destroy the old one.
                        delete OldStack;
                    }
                }else{
                    // If we're here, we're likely being destroyed.  Even if not then a menu outside of the widget heirarchy has no use for a stack.
                    // So destroy what we have, and set the lower entries accordingly.  They shouldn't need to do the same when they get their turn running this logic.
                    if( this->MenuStack != NULL ) {
                        delete this->MenuStack;
                        this->MenuStack = NULL;
                    }
                    this->_NotifyStack(this->MenuStack);
                }
            }
        }

        ///////////////////////////////////////////////////////////////////////////////
        // MenuEntryFactory Methods

        String MenuEntryFactory::GetWidgetTypeName() const
            { return MenuEntry::TypeName; }

        MenuEntry* MenuEntryFactory::CreateMenuEntry(const String& RendName, Screen* Parent)
        {
            MenuEntry* Ret = new MenuEntry(RendName,Parent);
            Ret->_SetLayoutStrat( new LayoutStrategy() );
            return Ret;
        }

        MenuEntry* MenuEntryFactory::CreateMenuEntry(const String& RendName, const UnifiedRect& RendRect, Screen* Parent)
        {
            MenuEntry* Ret = new MenuEntry(RendName,RendRect,Parent);
            Ret->_SetLayoutStrat( new LayoutStrategy() );
            return Ret;
        }

        MenuEntry* MenuEntryFactory::CreateMenuEntry(const XML::Node& XMLNode, Screen* Parent)
        {
            MenuEntry* Ret = new MenuEntry(XMLNode,Parent);
            Ret->_SetLayoutStrat( new LayoutStrategy() );
            return Ret;
        }

        Widget* MenuEntryFactory::CreateWidget(Screen* Parent)
        {
            MenuEntry* Ret = new MenuEntry(Parent);
            Ret->_SetLayoutStrat( new LayoutStrategy() );
            return Ret;
        }

        Widget* MenuEntryFactory::CreateWidget(const String& RendName, const NameValuePairMap& Params, Screen* Parent)
            { return this->CreateMenuEntry(RendName,Parent); }

        Widget* MenuEntryFactory::CreateWidget(const String& RendName, const UnifiedRect& RendRect, const NameValuePairMap& Params, Screen* Parent)
            { return this->CreateMenuEntry(RendName,RendRect,Parent); }

        Widget* MenuEntryFactory::CreateWidget(const XML::Node& XMLNode, Screen* Parent)
            { return this->CreateMenuEntry(XMLNode,Parent); }

        void MenuEntryFactory::DestroyWidget(Widget* ToBeDestroyed)
            { delete static_cast<MenuEntry*>( ToBeDestroyed ); }
    }//UI
}//Mezzanine

#endif
