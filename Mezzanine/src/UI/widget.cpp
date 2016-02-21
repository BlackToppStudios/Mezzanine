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
#ifndef _uiwidget_cpp
#define _uiwidget_cpp

#include "UI/widget.h"
#include "UI/screen.h"
#include "UI/renderlayergroup.h"
#include "UI/layoutstrategy.h"

namespace Mezzanine
{
    namespace UI
    {
        ///////////////////////////////////////////////////////////////////////////////
        // Widget Static Members

        const String Widget::TypeName              = "GenericWidget";
        const String Widget::EventMouseEnter       = "MouseEnter";
        const String Widget::EventMouseExit        = "MouseExit";
        const String Widget::EventMouseDragStart   = "MouseDragStart";
        const String Widget::EventMouseDragEnd     = "MouseDragEnd";
        const String Widget::EventFocusGained      = "FocusGained";
        const String Widget::EventFocusLost        = "FocusLost";
        const String Widget::EventFocusLocked      = "FocusLocked";
        const String Widget::EventFocusUnlocked    = "FocusUnlocked";
        const String Widget::EventVisibilityShown  = "VisibilityShown";
        const String Widget::EventVisibilityHidden = "VisibilityHidden";

        ///////////////////////////////////////////////////////////////////////////////
        // Widget Methods

        Widget::Widget(Screen* Parent) :
            QuadRenderable(Parent),
            HoveredSubWidget(NULL),
            State(WS_Untouched)
            { /* Do nothing to leave it blank */ }

        Widget::Widget(const String& RendName, Screen* Parent) :
            QuadRenderable(RendName,Parent),
            HoveredSubWidget(NULL),
            State(WS_Untouched)
            { this->ConstructWidget(); }

        Widget::Widget(const String& RendName, const UnifiedRect& RendRect, Screen* Parent) :
            QuadRenderable(RendName,RendRect,Parent),
            HoveredSubWidget(NULL),
            State(WS_Untouched)
            { this->ConstructWidget(); }

        Widget::Widget(const XML::Node& XMLNode, Screen* Parent) :
            QuadRenderable(Parent),
            HoveredSubWidget(NULL),
            State(WS_Untouched)
            { this->ProtoDeSerialize(XMLNode); }

        Widget::~Widget()
            {  }

        void Widget::ProtoSerializeImpl(XML::Node& SelfRoot) const
        {
            this->QuadRenderable::ProtoSerializeImpl(SelfRoot);
            this->ProtoSerializeStateGroupBindings(SelfRoot);
            this->ProtoSerializeEvents(SelfRoot);
            /// @todo Seriailze subscribed events?  Scripts at least.
        }

        void Widget::ProtoDeSerializeImpl(const XML::Node& SelfRoot)
        {
            this->QuadRenderable::ProtoDeSerializeImpl(SelfRoot);
            this->ProtoDeSerializeStateGroupBindings(SelfRoot);
            this->ProtoDeSerializeEvents(SelfRoot);
        }

        Boole Widget::HandleInputImpl(const Input::MetaCode& Code)
        {
            return false;
        }

        void Widget::HandleChildStateChangeImpl(Widget* Child, const UInt32& OldState, const UInt32& NewState)
        {
            // Default to nothing
        }

        void Widget::ConstructWidget()
        {
            // Create our events.
            this->AddEvent(Widget::EventMouseEnter);
            this->AddEvent(Widget::EventMouseExit);
            this->AddEvent(Widget::EventMouseDragStart);
            this->AddEvent(Widget::EventMouseDragEnd);
            this->AddEvent(Widget::EventFocusGained);
            this->AddEvent(Widget::EventFocusLost);
            this->AddEvent(Widget::EventFocusLocked);
            this->AddEvent(Widget::EventFocusUnlocked);
            this->AddEvent(Widget::EventVisibilityShown);
            this->AddEvent(Widget::EventVisibilityHidden);

            // Create our render groups and bind them
            RenderLayerGroup* NormalGroup = this->CreateRenderLayerGroup(Widget::WG_Normal);
            RenderLayerGroup* HoveredGroup = this->CreateRenderLayerGroup(Widget::WG_Hovered);

            this->BindGroupToState( WS_Untouched, NormalGroup);
            this->BindGroupToState( WS_Hovered, HoveredGroup);
            this->BindGroupToState( WS_Focused, NormalGroup);
            this->BindGroupToState( WS_Dragged, NormalGroup);
            this->BindGroupToState( WS_Hovered | WS_Focused, HoveredGroup);
            this->BindGroupToState( WS_Hovered | WS_Dragged, HoveredGroup);
            this->BindGroupToState( WS_Focused | WS_Dragged, NormalGroup);
            this->BindGroupToState( WS_Hovered | WS_Focused | WS_Dragged, HoveredGroup);
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Utility Methods

        Renderable::RenderableType Widget::GetRenderableType() const
            { return Renderable::RT_Widget; }

        const String& Widget::GetTypeName() const
            { return Widget::TypeName; }

        Boole Widget::IsHovered() const
            { return (this->State & WS_Hovered); }

        Boole Widget::HasFocus() const
            { return (this->State & WS_Focused); }

        Boole Widget::IsDragged() const
            { return (this->State & WS_Dragged); }

        void Widget::ForceState(const UInt32 NewState)
        {
            if( this->State != NewState ) {
                UInt32 OldState = this->State;
                this->State = NewState;
                this->SetGroupFromState(this->State);

                if( this->ParentQuad && this->ParentQuad->IsWidget() ) {
                    static_cast<UI::Widget*>(this->ParentQuad)->_NotifyChildStateChange(this,OldState,NewState);
                }
            }
        }

        UInt32 Widget::GetState() const
        {
            return this->State;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // State-LayerGroup Binding Methods

        void Widget::BindGroupToState(const UInt32 BindState, RenderLayerGroup* ToBind)
        {
            this->StateGroupBindings[BindState] = ToBind;
        }

        RenderLayerGroup* Widget::GetGroupBoundToState(const UInt32 BindState) const
        {
            ConstStateLayerGroupIterator It = this->StateGroupBindings.find(BindState);
            if( It != this->StateGroupBindings.end() ) return (*It).second;
            else return NULL;
        }

        Boole Widget::SetGroupFromState(const UInt32 BindState)
        {
            StateLayerGroupIterator It = this->StateGroupBindings.find(BindState);
            if( It != this->StateGroupBindings.end() )
            {
                this->SetActiveGroup( (*It).second );
                return true;
            }
            else return false;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Fetch Methods

        Widget* Widget::GetHoveredSubWidget() const
        {
            return this->HoveredSubWidget;
        }

        Widget* Widget::GetBottomMostHoveredWidget()
        {
            if( this->HoveredSubWidget ) return this->HoveredSubWidget->GetBottomMostHoveredWidget();
            else return this;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Visibility and Priority Methods

        void Widget::SetVisible(Boole CanSee)
        {
            if( this->Visible != CanSee ) {
                if(CanSee) {
                    this->_OnVisibilityShown();
                }else{
                    this->_OnVisibilityHidden();
                }

                for( ChildIterator It = this->ChildWidgets.begin() ; It != this->ChildWidgets.end() ; ++It )
                    { (*It)->SetVisible(CanSee); }
            }
        }

        Boole Widget::GetVisible() const
        {
            return this->Visible;
        }

        Boole Widget::IsVisible() const
        {
            if( this->ParentQuad ) {
                return this->Visible && this->ParentQuad->IsVisible();
            }else{
                return this->Visible;
            }
        }

        void Widget::Show()
        {
            if( this->Visible == false ) {
                this->_OnVisibilityShown();
                for( ChildIterator It = this->ChildWidgets.begin() ; It != this->ChildWidgets.end() ; ++It )
                    { (*It)->Show(); }
            }
        }

        void Widget::Hide()
        {
            if( this->Visible == true ) {
                this->_OnVisibilityHidden();
                for( ChildIterator It = this->ChildWidgets.begin() ; It != this->ChildWidgets.end() ; ++It )
                    { (*It)->Hide(); }
            }
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Serialization

        void Widget::ProtoSerializeProperties(XML::Node& SelfRoot) const
        {
            this->QuadRenderable::ProtoSerializeProperties(SelfRoot);
            XML::Node PropertiesNode = SelfRoot.AppendChild( Widget::GetSerializableName() + "Properties" );

            if( PropertiesNode.AppendAttribute("Version").SetValue("1") &&
                PropertiesNode.AppendAttribute("State").SetValue( this->State ) )
            {
                return;
            }else{
                SerializeError("Create XML Attribute Values",Widget::GetSerializableName() + "Properties",true);
            }
        }

        void Widget::ProtoSerializeStateGroupBindings(XML::Node& SelfRoot) const
        {
            XML::Node BindingsNode = SelfRoot.AppendChild( "StateGroupBindings" );

            if( BindingsNode.AppendAttribute("Version").SetValue("1") ) {
                for( ConstStateLayerGroupIterator BindingIt = this->StateGroupBindings.begin() ; BindingIt != this->StateGroupBindings.end() ; ++BindingIt )
                {
                    XML::Node BindingNode = BindingsNode.AppendChild( "StateGroupBinding" );

                    if( BindingNode.AppendAttribute("Version").SetValue("1") &&
                        BindingNode.AppendAttribute("StateID").SetValue( (*BindingIt).first ) &&
                        BindingNode.AppendAttribute("LayerGroupID").SetValue( (*BindingIt).second->GetGroupID() ) )
                    {
                        continue;
                    }else{
                        SerializeError("Create XML Version Attribute","StateGroupBinding",true);
                    }
                }
            }else{
                SerializeError("Create XML Version Attribute","StateGroupBindings",true);
            }
        }

        void Widget::ProtoSerializeEvents(XML::Node& SelfRoot) const
        {
            XML::Node EventsNode = SelfRoot.AppendChild( "Events" );

            if( EventsNode.AppendAttribute("Version").SetValue("1") ) {
                for( ConstEventIterator EvIt = this->Events.begin() ; EvIt != this->Events.end() ; ++EvIt )
                {
                    XML::Node BindingNode = EventsNode.AppendChild( "Event" );

                    if( BindingNode.AppendAttribute("Version").SetValue("1") &&
                        BindingNode.AppendAttribute("Name").SetValue( (*EvIt).first ) )
                    {
                        continue;
                    }else{
                        SerializeError("Create XML Version Attribute","Event",true);
                    }
                }
            }else{
                SerializeError("Create XML Version Attribute","Events",true);
            }
        }

        void Widget::ProtoDeSerializeProperties(const XML::Node& SelfRoot)
        {
            this->QuadRenderable::ProtoDeSerializeProperties(SelfRoot);

            XML::Attribute CurrAttrib;
            XML::Node PropertiesNode = SelfRoot.GetChild( Widget::GetSerializableName() + "Properties" );

            if( !PropertiesNode.Empty() ) {
                if(PropertiesNode.GetAttribute("Version").AsInt() == 1) {
                    CurrAttrib = PropertiesNode.GetAttribute("State");
                    if( !CurrAttrib.Empty() )
                        this->State = CurrAttrib.AsUint();
                }else{
                    MEZZ_EXCEPTION(ExceptionBase::INVALID_VERSION_EXCEPTION,"Incompatible XML Version for " + (Widget::GetSerializableName() + "Properties") + ": Not Version 1.");
                }
            }else{
                MEZZ_EXCEPTION(ExceptionBase::II_IDENTITY_NOT_FOUND_EXCEPTION,Widget::GetSerializableName() + "Properties" + " was not found in the provided XML node, which was expected.");
            }
        }

        void Widget::ProtoDeSerializeStateGroupBindings(const XML::Node& SelfRoot)
        {
            this->StateGroupBindings.clear();

            XML::Attribute CurrAttrib;
            XML::Node BindingsNode = SelfRoot.GetChild( "StateGroupBindings" );

            if( !BindingsNode.Empty() ) {
                if( BindingsNode.GetAttribute("Version").AsInt() == 1 ) {
                    for( XML::NodeIterator BindingNodeIt = BindingsNode.begin() ; BindingNodeIt != BindingsNode.end() ; ++BindingNodeIt )
                    {
                        if( (*BindingNodeIt).GetAttribute("Version").AsInt() == 1 ) {
                            UInt32 StateID = 0;

                            CurrAttrib = (*BindingNodeIt).GetAttribute("StateID");
                            if( !CurrAttrib.Empty() )
                                StateID = CurrAttrib.AsUint();

                            CurrAttrib = (*BindingNodeIt).GetAttribute("LayerGroupID");
                            if( !CurrAttrib.Empty() ) {
                                UInt16 LayerGroupID = CurrAttrib.AsUint();
                                RenderLayerGroup* NamedGroup = this->GetRenderLayerGroup( LayerGroupID );
                                if( NamedGroup != NULL ) {
                                    this->StateGroupBindings.insert( std::pair<UInt32,RenderLayerGroup*>(StateID,NamedGroup) );
                                }else{
                                    StringStream ExceptionStream;
                                    ExceptionStream << "Named RenderLayerGroup \"" << LayerGroupID << "\" not found when deserializing Widget named \"" << this->GetName() << "\".";
                                    MEZZ_EXCEPTION(ExceptionBase::PARAMETERS_EXCEPTION,ExceptionStream.str());
                                }
                            }
                        }else{
                            MEZZ_EXCEPTION(ExceptionBase::INVALID_VERSION_EXCEPTION,"Incompatible XML Version for " + String("StateGroupBindings") + ": Not Version 1.");
                        }
                    }
                }else{
                    MEZZ_EXCEPTION(ExceptionBase::INVALID_VERSION_EXCEPTION,"Incompatible XML Version for " + String("StateGroupBindings") + ": Not Version 1.");
                }
            }
        }

        void Widget::ProtoDeSerializeEvents(const XML::Node& SelfRoot)
        {
            this->RemoveAllEvents();

            XML::Attribute CurrAttrib;
            XML::Node EventsNode = SelfRoot.GetChild( "Events" );

            if( !EventsNode.Empty() ) {
                if( EventsNode.GetAttribute("Version").AsInt() == 1 ) {
                    for( XML::NodeIterator EvNodeIt = EventsNode.begin() ; EvNodeIt != EventsNode.end() ; ++EvNodeIt )
                    {
                        if( (*EvNodeIt).GetAttribute("Version").AsInt() == 1 ) {
                            String EvName;

                            CurrAttrib = (*EvNodeIt).GetAttribute("Name");
                            if( !CurrAttrib.Empty() )
                                EvName = CurrAttrib.AsString();

                            if( !EvName.empty() ) {
                                this->AddEvent(EvName);
                            }
                        }else{
                            MEZZ_EXCEPTION(ExceptionBase::INVALID_VERSION_EXCEPTION,"Incompatible XML Version for " + String("Events") + ": Not Version 1.");
                        }
                    }
                }else{
                    MEZZ_EXCEPTION(ExceptionBase::INVALID_VERSION_EXCEPTION,"Incompatible XML Version for " + String("Events") + ": Not Version 1.");
                }
            }
        }

        String Widget::GetDerivedSerializableName() const
        {
            return this->GetTypeName();
        }

        String Widget::GetSerializableName()
        {
            return Widget::TypeName;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Internal Event Methods

        void Widget::_OnMouseEnter()
        {
            if( !this->IsHovered() ) {
                UInt32 NewState = this->State | WS_Hovered;
                this->ForceState(NewState);

                WidgetEventArgumentsPtr Args( new WidgetEventArguments(Widget::EventMouseEnter,this->Name) );
                this->FireEvent(Args);
            }
        }

        void Widget::_OnMouseExit()
        {
            if( this->IsHovered() ) {
                UInt32 NewState = this->State & ~WS_Hovered;
                this->ForceState(NewState);

                WidgetEventArgumentsPtr Args( new WidgetEventArguments(Widget::EventMouseExit,this->Name) );
                this->FireEvent(Args);
            }
        }

        void Widget::_OnMouseDragStart()
        {
            if( !this->IsDragged() ) {
                UInt32 NewState = this->State | WS_Dragged;
                this->ForceState(NewState);

                WidgetEventArgumentsPtr Args( new WidgetEventArguments(Widget::EventMouseDragStart,this->Name) );
                this->FireEvent(Args);
            }
        }

        void Widget::_OnMouseDragEnd()
        {
            if( this->IsDragged() ) {
                UInt32 NewState = this->State & ~WS_Dragged;
                this->ForceState(NewState);

                WidgetEventArgumentsPtr Args( new WidgetEventArguments(Widget::EventMouseDragEnd,this->Name) );
                this->FireEvent(Args);
            }
        }

        void Widget::_OnFocusGained()
        {
            if( !this->HasFocus() ) {
                UInt32 NewState = this->State | WS_Focused;
                this->ForceState(NewState);

                WidgetEventArgumentsPtr Args( new WidgetEventArguments(Widget::EventFocusGained,this->Name) );
                this->FireEvent(Args);
            }
        }

        void Widget::_OnFocusLost()
        {
            if( this->HasFocus() ) {
                UInt32 NewState = this->State & ~WS_Focused;
                this->ForceState(NewState);

                WidgetEventArgumentsPtr Args( new WidgetEventArguments(Widget::EventFocusLost,this->Name) );
                this->FireEvent(Args);
            }
        }

        void Widget::_OnFocusLocked()
        {
            WidgetEventArgumentsPtr Args( new WidgetEventArguments(Widget::EventFocusLocked,this->Name) );
            this->FireEvent(Args);
        }

        void Widget::_OnFocusUnlocked()
        {
            WidgetEventArgumentsPtr Args( new WidgetEventArguments(Widget::EventFocusUnlocked,this->Name) );
            this->FireEvent(Args);
        }

        void Widget::_OnVisibilityShown()
        {
            this->Visible = true;
            this->_MarkDirty();

            WidgetEventArgumentsPtr Args( new WidgetEventArguments(Widget::EventVisibilityShown,this->Name) );
            this->FireEvent(Args);
        }

        void Widget::_OnVisibilityHidden()
        {
            this->Visible = false;
            this->_MarkDirty();

            WidgetEventArgumentsPtr Args( new WidgetEventArguments(Widget::EventVisibilityHidden,this->Name) );
            this->FireEvent(Args);
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Internal Methods

        Boole Widget::_HandleInput(const Input::MetaCode& Code)
        {
            if( this->HandleInputImpl(Code) ) {
                return true;
            }else{
                if( this->ParentQuad && this->ParentQuad->IsWidget() )
                    return static_cast<Widget*>(this->ParentQuad)->_HandleInput(Code);
                else return false;
            }
        }

        void Widget::_NotifyChildStateChange(Widget* Child, const UInt32& OldState, const UInt32& NewState)
        {
            this->HandleChildStateChangeImpl(Child,OldState,NewState);
            if( this->ParentQuad && this->ParentQuad->IsWidget() )
                static_cast<Widget*>(this->ParentQuad)->_NotifyChildStateChange(Child,OldState,NewState);
        }

        void Widget::_NotifyEvent(EventArgumentsPtr Args)
        {
            // Default to doing nothing, must be overridden to add logic if a widget needs it
        }

        ///////////////////////////////////////////////////////////////////////////////
        // ButtonFactory Methods

        String GenericWidgetFactory::GetWidgetTypeName() const
            { return Widget::TypeName; }

        Widget* GenericWidgetFactory::CreateWidget(const String& RendName, Screen* Parent)
        {
            Widget* Ret = new Widget(RendName,Parent);
            Ret->_SetLayoutStrat( new LayoutStrategy() );
            return Ret;
        }

        Widget* GenericWidgetFactory::CreateWidget(const String& RendName, const UnifiedRect& RendRect, Screen* Parent)
        {
            Widget* Ret = new Widget(RendName,RendRect,Parent);
            Ret->_SetLayoutStrat( new LayoutStrategy() );
            return Ret;
        }

        Widget* GenericWidgetFactory::CreateWidget(Screen* Parent)
        {
            Widget* Ret = new Widget(Parent);
            Ret->_SetLayoutStrat( new LayoutStrategy() );
            return Ret;
        }

        Widget* GenericWidgetFactory::CreateWidget(const String& RendName, const NameValuePairMap& Params, Screen* Parent)
            { return this->CreateWidget(RendName,Parent); }

        Widget* GenericWidgetFactory::CreateWidget(const String& RendName, const UnifiedRect& RendRect, const NameValuePairMap& Params, Screen* Parent)
            { return this->CreateWidget(RendName,RendRect,Parent); }

        Widget* GenericWidgetFactory::CreateWidget(const XML::Node& XMLNode, Screen* Parent)
        {
            Widget* Ret = new Widget(XMLNode,Parent);
            Ret->_SetLayoutStrat( new LayoutStrategy() );
            return Ret;
        }

        void GenericWidgetFactory::DestroyWidget(Widget* ToBeDestroyed)
            { delete ToBeDestroyed; }
    }//UI
}//Mezzanine

#endif
