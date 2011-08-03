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
              Visible(true),
              RelPosition(Vector2(0,0)),
              RelSize(Vector2(0,0)),
              Name(name)
        {
            Manager = World::GetWorldPointer()->GetUIManager();
        }

        Widget::~Widget()
        {
        }

        void Widget::SubWidgetUpdate(bool Force)
        {
            if(HoveredSubWidget)
                HoveredSubWidget->Update(Force);
        }

        void Widget::SubWidgetFocusUpdate(bool Force)
        {
            if(SubWidgetFocus)
                SubWidgetFocus->Update(Force);
        }

        void Widget::ProcessCapturedInputs()
        {
        }

        Widget::WidgetType Widget::GetType() const
        {
            return Type;
        }

        bool Widget::IsInputCaptureWidget()
        {
            return NULL != CaptureData;
        }

        String& Widget::GetName()
        {
            return Name;
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

        RenderableRect Widget::GetRect(bool Relative)
        {
            if(Relative) return RenderableRect(GetPosition(),GetSize(),Relative);
            else return RenderableRect(GetActualPosition(),GetActualSize(),Relative);
        }

        void Widget::UpdateDimensions(const Vector2& OldViewportSize)
        {
            const Vector2& WinDim = Parent->GetParent()->GetViewportDimensions();
            this->SetActualPosition(RelPosition * WinDim);
            this->SetActualSize(RelSize * WinDim);
        }

        Button* Widget::GetHoveredButton()
        {
            return HoveredButton;
        }

        Widget* Widget::GetHoveredSubWidget()
        {
            return HoveredSubWidget;
        }

        Layer* Widget::GetLayer()
        {
            return Parent;
        }

        InputCaptureData* Widget::GetInputCaptureData()
        {
            return CaptureData;
        }
    }//UI
}//phys

#endif
