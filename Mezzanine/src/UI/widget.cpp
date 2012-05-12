//© Copyright 2010 - 2012 BlackTopp Studios Inc.
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
#include "UI/button.h"
#include "UI/layer.h"
#include "UI/screen.h"
#include "uimanager.h"
#include "inputquerytool.h"
#include "world.h"

namespace Mezzanine
{
    namespace UI
    {
        InputCaptureData::InputCaptureData()
        {
        }

        InputCaptureData::~InputCaptureData()
        {
        }

        void InputCaptureData::AddInput(const MetaCode::InputCode& Code)
        {
            this->insert(Code);
        }

        void InputCaptureData::AddInputRange(const MetaCode::InputCode& Lower, const MetaCode::InputCode& Upper)
        {
            for( MetaCode::InputCode Code = Lower ; Code > Upper ; Code + 1 )
                this->insert(Code);
        }

        Whole InputCaptureData::GetNumCapturedInputs()
        {
            return this->size();
        }

        std::vector<MetaCode::InputCode>* InputCaptureData::GetCapturedInputs()
        {
            return &CapturedCodes;
        }

        bool InputCaptureData::IsInputToBeCaptured(const MetaCode::InputCode& Code)
        {
            std::set<MetaCode::InputCode>::iterator it = this->find(Code);
            return it != this->end();
        }

        void InputCaptureData::UpdateCapturedInputs(std::vector<MetaCode::InputCode>& InputCodes)
        {
            CapturedCodes.clear();
            CapturedCodes.swap(InputCodes);
        }

        //-----------------------------------------------------

        Widget::Widget(const String& name, Layer* Parent)
            : ParentLayer(Parent),
              ParentWidget(NULL),
              HoveredButton(NULL),
              HoveredSubWidget(NULL),
              SubWidgetFocus(NULL),
              CaptureData(NULL),
              Callback(NULL),
              Visible(true),
              Hovered(false),
              Dirty(true),
              Priority(UI::RP_Medium),
              RelPosition(Vector2(0,0)),
              RelSize(Vector2(0,0)),
              Name(name)
        {
            Manager = UIManager::GetSingletonPtr();
        }

        Widget::~Widget()
        {
            SubRenderables.clear();
        }

        void Widget::Update(bool Force)
        {
            if(Callback)
                Callback->DoPreUpdateItems();
            SubWidgetUpdate(Force);
            UpdateImpl(Force);
            if(Callback)
                Callback->DoPostUpdateItems();
        }

        void Widget::SubWidgetUpdate(bool Force)
        {
            MetaCode::ButtonState State = InputQueryTool::GetMouseButtonState(1);
            if(HoveredSubWidget)
            {
                HoveredSubWidget->Update(Force);
                if(MetaCode::BUTTON_PRESSING == State)
                {
                    SubWidgetFocus = HoveredSubWidget;
                }
            }
            if(SubWidgetFocus && (SubWidgetFocus != HoveredSubWidget))
            {
                SubWidgetFocusUpdate(true);
            }
            else if(MetaCode::BUTTON_DOWN == State && Force)
            {
                SubWidgetFocusUpdate(Force);
            }
            if(MetaCode::BUTTON_LIFTING == State)
            {
                SubWidgetFocus = NULL;
            }
        }

        void Widget::SubWidgetFocusUpdate(bool Force)
        {
            if(SubWidgetFocus)
                SubWidgetFocus->Update(Force);
        }

        void Widget::AddSubRenderable(const Whole& Zorder, const RenderablePair& ToAdd)
        {
            if( ToAdd.first ) ToAdd.first->ParentWidget = this;
            else ToAdd.second->ParentWidget = this;
            SubRenderables[Zorder] = ToAdd;
        }

        void Widget::ProcessCapturedInputs()
        {
        }

        void Widget::SetVisible(bool visible)
        {
            if(this->Visible == visible)
                return;
            this->Visible = visible;
            SetVisibleImpl(visible);
            if(Callback)
                Callback->DoVisibilityChangeItems();
        }

        bool Widget::GetVisible() const
        {
            return Visible;
        }

        bool Widget::IsVisible() const
        {
            return Visible && ParentLayer->IsVisible() && ParentLayer->GetParent()->IsVisible();
        }

        void Widget::Show()
        {
            SetVisible(true);
        }

        void Widget::Hide()
        {
            SetVisible(false);
        }

        Widget::WidgetType Widget::GetType() const
        {
            return Type;
        }

        bool Widget::IsInputCaptureWidget() const
        {
            return NULL != CaptureData;
        }

        bool Widget::IsHovered() const
        {
            return Hovered;
        }

        ConstString& Widget::GetName() const
        {
            return Name;
        }

        void Widget::SetWidgetCallback(WidgetCallback* CB)
        {
            if(Callback != CB && Callback)
                delete Callback;
            CB->SetCaller(this);
            Callback = CB;
        }

        bool Widget::CheckMouseHover()
        {
            if(!IsVisible())
            {
                HoveredSubWidget = NULL;
                HoveredButton = NULL;
                Hovered = false;
            }
            else if(CheckMouseHoverImpl())
            {
                Hovered = true;
            }
            else
            {
                HoveredSubWidget = NULL;
                HoveredButton = NULL;
                Hovered = false;
            }
            if(Callback)
                Callback->DoHoverItems();
            return Hovered;
        }

        void Widget::SetRect(const RenderableRect& Rect)
        {
            if(Rect.Relative)
            {
                SetSize(Rect.Size);
                SetPosition(Rect.Position);
            }else{
                SetActualSize(Rect.Size);
                SetActualPosition(Rect.Position);
            }
        }

        RenderableRect Widget::GetRect(bool Relative) const
        {
            if(Relative) return RenderableRect(GetPosition(),GetSize(),Relative);
            else return RenderableRect(GetActualPosition(),GetActualSize(),Relative);
        }

        Vector2 Widget::GetPosition() const
        {
            return RelPosition;
        }

        Vector2 Widget::GetActualPosition() const
        {
            return RelPosition * ParentLayer->GetParent()->GetViewportDimensions();
        }

        Vector2 Widget::GetSize() const
        {
            return RelSize;
        }

        Vector2 Widget::GetActualSize() const
        {
            return RelSize * ParentLayer->GetParent()->GetViewportDimensions();
        }

        void Widget::SetRenderPriority(const UI::RenderPriority& Priority)
        {
            this->Priority = Priority;
            for( RenderableMap::iterator it = SubRenderables.begin() ; it != SubRenderables.end() ; ++it )
            {
                if( (*it).second.first ) (*it).second.first->SetRenderPriority(this->Priority);
                else (*it).second.second->SetRenderPriority(this->Priority);
            }
        }

        UI::RenderPriority Widget::GetRenderPriority() const
        {
            return Priority;
        }

        Button* Widget::GetHoveredButton() const
        {
            return HoveredButton;
        }

        Widget* Widget::GetHoveredSubWidget() const
        {
            return HoveredSubWidget;
        }

        Layer* Widget::GetLayer() const
        {
            return ParentLayer;
        }

        InputCaptureData* Widget::GetInputCaptureData() const
        {
            return CaptureData;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Internal Functions
        ///////////////////////////////////////

        void Widget::_MarkDirty()
        {
            Dirty = true;
            ParentLayer->_MarkDirty();
            if(ParentWidget)
                ParentWidget->_MarkDirty();
        }

        void Widget::_Redraw()
        {
            for( RenderableMap::iterator it = SubRenderables.begin() ; it != SubRenderables.end() ; ++it )
            {
                if( (*it).second.first )
                {
                    if( (*it).second.first->Dirty )
                        (*it).second.first->_Redraw();
                }else{
                    if( (*it).second.second->Dirty )
                        (*it).second.second->_Redraw();
                }
            }
        }

        void Widget::_AppendVertices(ScreenVertexData& Vertices)
        {
            for( RenderableMap::iterator it = SubRenderables.begin() ; it != SubRenderables.end() ; ++it )
            {
                if( (*it).second.first )
                {
                    if( (*it).second.first->IsVisible() )
                        (*it).second.first->_AppendVertices(Vertices);
                }else{
                    if( (*it).second.second->IsVisible() )
                        (*it).second.second->_AppendVertices(Vertices);
                }
            }
        }

        //-----------------------------------------------------

        WidgetCallback::WidgetCallback()
            : Caller(NULL)
        {
        }

        WidgetCallback::~WidgetCallback()
        {
        }

        void WidgetCallback::SetCaller(Widget* Caller)
        {
            this->Caller = Caller;
        }
    }//UI
}//Mezzanine

#endif
