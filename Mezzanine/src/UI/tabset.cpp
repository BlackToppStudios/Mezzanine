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
#ifndef _uitabset_cpp
#define _uitabset_cpp

#include "UI/tabset.h"
#include "UI/stackbutton.h"
#include "UI/screen.h"
#include "UI/layoutstrategy.h"

#include "Input/inputmanager.h"
#include "Input/mouse.h"

namespace Mezzanine
{
    namespace UI
    {
        ///////////////////////////////////////////////////////////////////////////////
        // TabSet Static Members

        const String TabSet::TypeName = "TabSet";

        ///////////////////////////////////////////////////////////////////////////////
        // TabSet Methods

        TabSet::TabSet(Screen* Parent) :
            StackedContainer(Parent),
            VisibleChild(NULL)
            {  }

        TabSet::TabSet(const String& RendName, Screen* Parent) :
            StackedContainer(RendName,Parent),
            VisibleChild(NULL)
            {  }

        TabSet::TabSet(const String& RendName, const UnifiedRect& RendRect, Screen* Parent) :
            StackedContainer(RendName,RendRect,Parent),
            VisibleChild(NULL)
            {  }

        TabSet::TabSet(const XML::Node& XMLNode, Screen* Parent) :
            StackedContainer(Parent),
            VisibleChild(NULL)
            { this->ProtoDeSerialize(XMLNode); }

        TabSet::~TabSet()
        {
            this->VisibleChild = NULL;
            this->SubSetBindings.clear();
        }

        void TabSet::ProtoSerializeImpl(XML::Node& SelfRoot) const
        {
            this->Widget::ProtoSerializeImpl(SelfRoot);
            this->ProtoSerializeButtonBindings(SelfRoot);
        }

        void TabSet::ProtoDeSerializeImpl(const XML::Node& SelfRoot)
        {
            this->Widget::ProtoDeSerializeImpl(SelfRoot);
            this->ProtoDeSerializeButtonBindings(SelfRoot);
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Utility Methods

        void TabSet::SetVisibleSubSet(TabSet::TabbedSubSet* MakeVisible)
        {
            this->SetVisibleSubSet( MakeVisible->GetZOrder() );
        }

        void TabSet::SetVisibleSubSet(const UInt16 Binding)
        {
            TabbedSubSet* NewVisible = this->GetChild( Binding );
            this->VisibleChild = NewVisible;
            if( this->VisibleChild != NULL ) {
                this->VisibleChild->SetVisible( this->IsVisible() );
            }
            this->_MarkDirty();
        }

        TabSet::TabbedSubSet* TabSet::GetVisibleSubSet() const
        {
            return this->VisibleChild;
        }

        const String& TabSet::GetTypeName() const
        {
            return TabSet::TypeName;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Visibility and Priority Methods

        void TabSet::SetVisible(Boolean CanSee)
        {
            if( this->Visible == CanSee )
                return;
            if(CanSee) {
                this->_OnVisibilityShown();
                if( this->VisibleChild != NULL ) {
                    this->VisibleChild->SetVisible(CanSee);
                }
            }else{
                this->_OnVisibilityHidden();
                for( ChildIterator It = this->ChildWidgets.begin() ; It != this->ChildWidgets.end() ; ++It )
                {
                    (*It)->SetVisible(CanSee);
                }
            }
        }

        void TabSet::Show()
        {
            if( this->Visible == true )
                return;
            this->_OnVisibilityShown();
            if( this->VisibleChild != NULL ) {
                this->VisibleChild->Show();
            }
        }

        void TabSet::Hide()
        {
            if( this->Visible == false )
                return;
            this->_OnVisibilityHidden();
            for( ChildIterator It = this->ChildWidgets.begin() ; It != this->ChildWidgets.end() ; ++It )
            {
                (*It)->Hide();
            }
        }

        ///////////////////////////////////////////////////////////////////////////////
        // TabSet Properties

        ///////////////////////////////////////////////////////////////////////////////
        // TabSet Configuration

        void TabSet::SetButtonConfig(const UInt16 Config, StackButton* ConfigButton)
        {
            this->SubSetBindings.insert( std::pair<StackButton*,UInt16>(ConfigButton,Config) );
            //if( ConfigButton->GetBoundContainer() != this ) {
            //}
            ConfigButton->_SetBoundContainer(this);
        }

        UInt16 TabSet::GetButtonConfig(const StackButton* ConfigButton) const
        {
            ConstTabbedSubSetIterator SubIt = this->SubSetBindings.find( const_cast<StackButton*>(ConfigButton) );
            if( SubIt != this->SubSetBindings.end() ) {
                return (*SubIt).second;
            }else{
                return 0;
            }
        }

        void TabSet::RemoveButtonConfig(StackButton* ConfigButton)
        {
            TabbedSubSetIterator BindIt = this->SubSetBindings.find(ConfigButton);
            if( BindIt != this->SubSetBindings.end() ) {
                this->SubSetBindings.erase( BindIt );
                ConfigButton->_SetBoundContainer(NULL);
            }
        }

        TabSet::TabbedSubSet* TabSet::CreateTabbedSubSet(const String& Name, const UInt16 ChildZOrder)
        {
            TabbedSubSet* NewSet = this->ParentScreen->CreateWidget(Name,UnifiedRect(0,0,1,1,0,0,0,0));
            this->AddChild(NewSet,ChildZOrder);
            NewSet->Hide();
            return NewSet;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Serialization

        void TabSet::ProtoSerializeProperties(XML::Node& SelfRoot) const
        {
            this->StackedContainer::ProtoSerializeProperties(SelfRoot);
        }

        void TabSet::ProtoSerializeButtonBindings(XML::Node& SelfRoot) const
        {
            XML::Node BindingsNode = SelfRoot.AppendChild( "SubSetBindings" );

            if( BindingsNode.AppendAttribute("Version").SetValue("1") ) {
                for( ConstTabbedSubSetIterator BindingIt = this->SubSetBindings.begin() ; BindingIt != this->SubSetBindings.end() ; ++BindingIt )
                {
                    XML::Node BindingNode = BindingsNode.AppendChild( "SubSetBinding" );

                    if( BindingNode.AppendAttribute("Version").SetValue("1") &&
                        BindingNode.AppendAttribute("ConfigID").SetValue( (*BindingIt).second ) &&
                        BindingNode.AppendAttribute("ButtonName").SetValue( (*BindingIt).first->GetName() ) )
                    {
                        continue;
                    }else{
                        SerializeError("Create XML Version Attribute","SubSetBinding",true);
                    }
                }
            }else{
                SerializeError("Create XML Version Attribute","SubSetBindings",true);
            }
        }

        void TabSet::ProtoDeSerializeProperties(const XML::Node& SelfRoot)
        {
            this->StackedContainer::ProtoDeSerializeProperties(SelfRoot);
        }

        void TabSet::ProtoDeSerializeButtonBindings(const XML::Node& SelfRoot)
        {
            this->SubSetBindings.clear();

            XML::Attribute CurrAttrib;
            XML::Node BindingsNode = SelfRoot.GetChild( "SubSetBindings" );

            if( !BindingsNode.Empty() ) {
                if( BindingsNode.GetAttribute("Version").AsInt() == 1 ) {
                    for( XML::NodeIterator BindingNodeIt = BindingsNode.begin() ; BindingNodeIt != BindingsNode.end() ; ++BindingNodeIt )
                    {
                        if( (*BindingNodeIt).GetAttribute("Version").AsInt() == 1 ) {
                            UInt16 ConfigID = 0;
                            String ButtonName;

                            CurrAttrib = (*BindingNodeIt).GetAttribute("ConfigID");
                            if( !CurrAttrib.Empty() )
                                ConfigID = static_cast<UInt16>( CurrAttrib.AsUint() );

                            CurrAttrib = (*BindingNodeIt).GetAttribute("ButtonName");
                            if( !CurrAttrib.Empty() )
                                ButtonName = CurrAttrib.AsString();

                            if( !ButtonName.empty() ) {
                                Widget* NamedButton = this->ParentScreen->GetWidget(ButtonName);
                                if( NamedButton != NULL && NamedButton->GetTypeName() == StackButton::TypeName ) {
                                    this->SetButtonConfig(ConfigID,static_cast<StackButton*>(NamedButton));
                                }else{
                                    StringStream ExceptionStream;
                                    ExceptionStream << "Named StackButton \"" << ButtonName << "\" not found when deserializing Widget named \"" << this->GetName() << "\".";
                                    MEZZ_EXCEPTION(Exception::PARAMETERS_EXCEPTION,ExceptionStream.str());
                                }
                            }
                        }else{
                            MEZZ_EXCEPTION(Exception::INVALID_VERSION_EXCEPTION,"Incompatible XML Version for " + String("SubSetBindings") + ": Not Version 1.");
                        }
                    }
                }else{
                    MEZZ_EXCEPTION(Exception::INVALID_VERSION_EXCEPTION,"Incompatible XML Version for " + String("SubSetBindings") + ": Not Version 1.");
                }
            }
        }

        String TabSet::GetSerializableName()
        {
            return TabSet::TypeName;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Internal Event Methods

        ///////////////////////////////////////////////////////////////////////////////
        // Internal Methods

        void TabSet::_NotifyButtonSelected(StackButton* Selected)
        {
            TabbedSubSetIterator SubIt = this->SubSetBindings.find(Selected);
            if( SubIt != this->SubSetBindings.end() ) {
                this->SetVisibleSubSet( (*SubIt).second );
            }
        }

        ///////////////////////////////////////////////////////////////////////////////
        // TabSetFactory Methods

        String TabSetFactory::GetWidgetTypeName() const
            { return TabSet::TypeName; }

        TabSet* TabSetFactory::CreateTabSet(const String& RendName, Screen* Parent)
        {
            TabSet* Ret = new TabSet(RendName,Parent);
            Ret->_SetLayoutStrat( new LayoutStrategy() );
            return Ret;
        }

        TabSet* TabSetFactory::CreateTabSet(const String& RendName, const UnifiedRect& RendRect, Screen* Parent)
        {
            TabSet* Ret = new TabSet(RendName,RendRect,Parent);
            Ret->_SetLayoutStrat( new LayoutStrategy() );
            return Ret;
        }

        TabSet* TabSetFactory::CreateTabSet(const XML::Node& XMLNode, Screen* Parent)
        {
            TabSet* Ret = new TabSet(XMLNode,Parent);
            Ret->_SetLayoutStrat( new LayoutStrategy() );
            return Ret;
        }

        Widget* TabSetFactory::CreateWidget(Screen* Parent)
        {
            TabSet* Ret = new TabSet(Parent);
            Ret->_SetLayoutStrat( new LayoutStrategy() );
            return Ret;
        }

        Widget* TabSetFactory::CreateWidget(const String& RendName, const NameValuePairMap& Params, Screen* Parent)
            { return this->CreateTabSet(RendName,Parent); }

        Widget* TabSetFactory::CreateWidget(const String& RendName, const UnifiedRect& RendRect, const NameValuePairMap& Params, Screen* Parent)
            { return this->CreateTabSet(RendName,RendRect,Parent); }

        Widget* TabSetFactory::CreateWidget(const XML::Node& XMLNode, Screen* Parent)
            { return this->CreateTabSet(XMLNode,Parent); }

        void TabSetFactory::DestroyWidget(Widget* ToBeDestroyed)
            { delete static_cast<TabSet*>( ToBeDestroyed ); }
    }//UI
}//Mezzanine

#endif
