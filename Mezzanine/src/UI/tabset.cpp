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
#include "UI/screen.h"
#include "UI/uimanager.h"
#include "UI/button.h"
#include "Input/inputmanager.h"
#include "Input/mouse.h"

namespace Mezzanine
{
    namespace UI
    {
        const String TabSet::TypeName = "TabSet";

        TabSet::TabSet(Screen* Parent) :
            StackedContainer(Parent)
        {

        }

        TabSet::TabSet(const String& RendName, Screen* Parent) :
            StackedContainer(RendName,Parent)
        {

        }

        TabSet::TabSet(const String& RendName, const UnifiedRect& RendRect, Screen* Parent) :
            StackedContainer(RendName,RendRect,Parent)
        {

        }

        TabSet::TabSet(const XML::Node& XMLNode, Screen* Parent) :
            StackedContainer(Parent)
        {

        }

        TabSet::~TabSet()
        {

        }

        ///////////////////////////////////////////////////////////////////////////////
        // Utility Methods

        ///////////////////////////////////////////////////////////////////////////////
        // Visibility and Priority Methods

        ///////////////////////////////////////////////////////////////////////////////
        // TabSet Properties

        ///////////////////////////////////////////////////////////////////////////////
        // TabSet Configuration

        ///////////////////////////////////////////////////////////////////////////////
        // Serialization

        ///////////////////////////////////////////////////////////////////////////////
        // Internal Event Methods

        ///////////////////////////////////////////////////////////////////////////////
        // Internal Methods


        /*TabSet::TabSet(const String& name, const Rect& SetRect, Screen* parent) :
            Widget(name,parent),
            SetsAdded(0),
            VisibleSet(NULL)
        {
            Type = Widget::W_TabSet;
            TemplateSetRect.Relative = false;
            const Vector2& WinDim = ParentScreen->GetViewportDimensions();
            if(SetRect.Relative)
            {
                RelPosition = SetRect.Position;
                RelSize = SetRect.Size;
                TemplateSetRect.Position = SetRect.Position * WinDim;
                TemplateSetRect.Size = SetRect.Size * WinDim;
            }else{
                RelPosition = SetRect.Position / WinDim;
                RelSize = SetRect.Size / WinDim;
                TemplateSetRect = SetRect;
            }
        }

        TabSet::~TabSet()
        {
            DestroyAllRenderableSets();
        }

        void TabSet::UpdateImpl(bool Force)
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
