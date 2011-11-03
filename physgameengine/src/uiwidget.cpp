//© Copyright 2010 - 2011 BlackTopp Studios Inc.
/* This file is part of The PhysGame Engine.

    The PhysGame Engine is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The PhysGame Engine is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with The PhysGame Engine.  If not, see <http://www.gnu.org/licenses/>.
*/
/* The original authors have included a copy of the license specified above in the
   'Docs' folder. See 'gpl.txt'
*/
/* We welcome the use of The PhysGame anyone, including companies who wish to
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

#include "uiwidget.h"
#include "uibutton.h"
#include "uilayer.h"
#include "uiscreen.h"
#include "inputquerytool.h"
#include "world.h"

namespace phys
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

        Widget::Widget(const String& name, Layer* parent)
            : Parent(parent),
              HoveredButton(NULL),
              HoveredSubWidget(NULL),
              SubWidgetFocus(NULL),
              CaptureData(NULL),
              Callback(NULL),
              Visible(true),
              Hovered(false),
              RelPosition(Vector2(0,0)),
              RelSize(Vector2(0,0)),
              Name(name)
        {
            Manager = World::GetWorldPointer()->GetUIManager();
        }

        Widget::~Widget()
        {
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
            return Visible && Parent->IsVisible() && Parent->GetParent()->IsVisible();
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
            return RelPosition * Parent->GetParent()->GetViewportDimensions();
        }

        Vector2 Widget::GetSize() const
        {
            return RelSize;
        }

        Vector2 Widget::GetActualSize() const
        {
            return RelSize * Parent->GetParent()->GetViewportDimensions();
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
            return Parent;
        }

        InputCaptureData* Widget::GetInputCaptureData() const
        {
            return CaptureData;
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
}//phys

#endif
