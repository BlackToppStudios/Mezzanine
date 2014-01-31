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
#include "UI/uimanager.h"
#include "UI/button.h"
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
            this->Visible = CanSee;
            if(CanSee) {
                this->_OnVisibilityShown();
                this->VisibleChild->SetVisible(CanSee);
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
            this->Visible = true;
            this->_OnVisibilityShown();
            this->VisibleChild->Show();
        }

        void TabSet::Hide()
        {
            if( this->Visible == false )
                return;
            this->Visible = false;
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
                TabbedSubSet* NewVisible = this->GetChild( (*SubIt).second );
                if( NewVisible != NULL ) {
                    this->VisibleChild = NewVisible;
                    this->VisibleChild->SetVisible( this->IsVisible() );
                    this->_MarkDirty();
                }
            }
        }

        void TabSet::_AppendRenderDataCascading(ScreenRenderData& RenderData)
        {
            if( this->VertexCache ) {
                if( this->Dirty || this->AllLayersDirty ) {
                    this->VertexCache->Clear();
                    this->_AppendRenderData(*VertexCache);
                    if( this->VisibleChild->_HasAvailableRenderData() ) {
                        this->VisibleChild->_AppendRenderDataCascading(*VertexCache);
                    }
                }
                RenderData.Append(*VertexCache);
            }else{
                this->_AppendRenderData(RenderData);
                if( this->VisibleChild->_HasAvailableRenderData() ) {
                    this->VisibleChild->_AppendRenderDataCascading(RenderData);
                }
            }
        }

        ///////////////////////////////////////////////////////////////////////////////
        // TabSetFactory Methods

        String TabSetFactory::GetWidgetTypeName() const
            { return TabSet::TypeName; }

        TabSet* TabSetFactory::CreateTabSet(const String& RendName, Screen* Parent)
            { return new TabSet(RendName,Parent); }

        TabSet* TabSetFactory::CreateTabSet(const String& RendName, const UnifiedRect& RendRect, Screen* Parent)
            { return new TabSet(RendName,RendRect,Parent); }

        TabSet* TabSetFactory::CreateTabSet(const XML::Node& XMLNode, Screen* Parent)
            { return new TabSet(XMLNode,Parent); }

        Widget* TabSetFactory::CreateWidget(Screen* Parent)
            { return new TabSet(Parent); }

        Widget* TabSetFactory::CreateWidget(const String& RendName, const NameValuePairMap& Params, Screen* Parent)
            { return this->CreateTabSet(RendName,Parent); }

        Widget* TabSetFactory::CreateWidget(const String& RendName, const UnifiedRect& RendRect, const NameValuePairMap& Params, Screen* Parent)
            { return this->CreateTabSet(RendName,RendRect,Parent); }

        Widget* TabSetFactory::CreateWidget(const XML::Node& XMLNode, Screen* Parent)
            { return this->CreateTabSet(XMLNode,Parent); }

        void TabSetFactory::DestroyWidget(Widget* ToBeDestroyed)
            { delete static_cast<TabSet*>( ToBeDestroyed ); }

        /*void TabSet::UpdateImpl(bool Force)
        {
            Input::ButtonState State = InputManager::GetSingletonPtr()->GetSystemMouse()->GetButtonState(1);
            if( HoveredSubWidget && (Widget::W_Button == HoveredSubWidget->GetType()) )
            {
                if(Input::BUTTON_PRESSING == State)
                {
                    RenderableSetData* ClickedSet = NULL;
                    for( std::vector<RenderableSetData*>::iterator it = Sets.begin() ; it != Sets.end() ; it++ )
                    {
                        if(HoveredSubWidget == (*it)->Accessor)
                            ClickedSet = (*it);
                    }
                    if(!ClickedSet)
                        return;
                    if(VisibleSet)
                        VisibleSet->Collection->Hide();
                    VisibleSet = ClickedSet;
                    VisibleSet->Collection->Show();
                }
            }
        }

        void TabSet::SetVisibleImpl(bool visible)
        {
            for( std::vector<RenderableSetData*>::iterator it = Sets.begin() ; it != Sets.end() ; it++ )
                (*it)->Accessor->SetVisible(visible);
            if(VisibleSet)
                VisibleSet->Collection->SetVisible(visible);
        }

        bool TabSet::CheckMouseHoverImpl()
        {
            for( std::vector<RenderableSetData*>::iterator it = Sets.begin() ; it != Sets.end() ; it++ )
            {
                if((*it)->Accessor->CheckMouseHover())
                {
                    HoveredSubWidget = (*it)->Accessor;
                    return true;
                }
            }
            if(VisibleSet->Collection->CheckMouseHover())
            {
                HoveredSubWidget = VisibleSet->Collection;
                return true;
            }
            return false;
        }

        RenderableSetData* TabSet::CreateRenderableSet(const String& Name, const Rect& AccessorRect, const Real& GlyphHeight, const String& Text)
        {
            const Vector2& WinDim = ParentScreen->GetViewportDimensions();

            Whole ActHeight = (Whole)(GlyphHeight * WinDim.Y);
            std::pair<Whole,Real> GlyphInfo = Manager->SuggestGlyphIndex(ActHeight,ParentScreen->GetPrimaryAtlas());
            Button* NewAccessor = ParentScreen->CreateButton(Name+"Access",AccessorRect,GlyphInfo.first,Text);
            if(1 != GlyphInfo.second)
                NewAccessor->GetClickable()->SetTextScale(GlyphInfo.second);
            NewAccessor->SetVisible(Visible);
            RenderableCollection* NewCollection = ParentScreen->CreateEnclosedRenderableContainerWidget(Name+"Set",TemplateSetRect);
            NewCollection->SetVisible(Visible);

            RenderableSetData* NewSetData = new RenderableSetData(Name,NewAccessor,NewCollection);
            if(0 == Sets.size()) VisibleSet = NewSetData;
            else NewSetData->Collection->Hide();
            Sets.push_back(NewSetData);
            AddSubRenderable(SetsAdded,NewSetData->Collection);
            AddSubRenderable(SetsAdded+1,NewSetData->Accessor);
            SetsAdded+=2;
            return NewSetData;
        }

        RenderableSetData* TabSet::GetRenderableSetData(const Whole& Index)
        {
            return Sets.at(Index);
        }

        RenderableSetData* TabSet::GetRenderableSetData(const String& SetDataName)
        {
            for( std::vector<RenderableSetData*>::iterator it = Sets.begin() ; it != Sets.end() ; it++ )
            {
                if(SetDataName == (*it)->Name)
                    return (*it);
            }
            return NULL;
        }

        RenderableCollection* TabSet::GetRenderableCollection(const Whole& Index)
        {
            return Sets.at(Index)->Collection;
        }

        RenderableCollection* TabSet::GetRenderableCollection(const String& SetDataName)
        {
            RenderableSetData* SetData = GetRenderableSetData(SetDataName);
            if(SetData) return SetData->Collection;
            else return NULL;
        }

        RenderableCollection* TabSet::GetRenderableCollection(Button* Accessor)
        {
            for( std::vector<RenderableSetData*>::iterator it = Sets.begin() ; it != Sets.end() ; it++ )
            {
                if(Accessor == (*it)->Accessor)
                    return (*it)->Collection;
            }
            return NULL;
        }

        Button* TabSet::GetAccessor(const Whole& Index)
        {
            return Sets.at(Index)->Accessor;
        }

        Button* TabSet::GetAccessor(const String& SetDataName)
        {
            RenderableSetData* SetData = GetRenderableSetData(SetDataName);
            if(SetData) return SetData->Accessor;
            else return NULL;
        }

        Button* TabSet::GetAccessor(RenderableCollection* Collection)
        {
            for( std::vector<RenderableSetData*>::iterator it = Sets.begin() ; it != Sets.end() ; it++ )
            {
                if(Collection == (*it)->Collection)
                    return (*it)->Accessor;
            }
            return NULL;
        }

        Whole TabSet::GetNumRenderableSets()
        {
            return Sets.size();
        }

        void TabSet::DestroyRenderableSet(RenderableSetData* ToBeDestroyed)
        {
            for( std::vector<RenderableSetData*>::iterator it = Sets.begin() ; it != Sets.end() ; it++ )
            {
                if(ToBeDestroyed == (*it))
                {
                    if(ToBeDestroyed == VisibleSet)
                        VisibleSet = NULL;
                    Sets.erase(it);
                    break;
                }
            }
            for( RenderableIterator it = SubRenderables.begin() ; it != SubRenderables.end() ; ++it )
            {
                if(ToBeDestroyed->Collection == (*it))
                {
                    SubRenderables.erase(it);
                    break;
                }
            }
            delete ToBeDestroyed;
        }

        void TabSet::DestroyAllRenderableSets()
        {
            for( std::vector<RenderableSetData*>::iterator it = Sets.begin() ; it != Sets.end() ; it++ )
            {
                RenderableSetData* CurrSet = (*it);
                ParentScreen->DestroyWidget(CurrSet->Accessor);
                ParentScreen->DestroyWidget(CurrSet->Collection);
                delete CurrSet;
            }
            Sets.clear();
            HoveredSubWidget = NULL;
            VisibleSet = NULL;
            SubRenderables.clear();
        }//*/
    }//UI
}//Mezzanine

#endif
