//© Copyright 2010 - 2013 BlackTopp Studios Inc.
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
#include "UI/basicrenderable.h"
#include "UI/button.h"
#include "UI/screen.h"
#include "uimanager.h"
#include "entresol.h"
#include "Input/inputmanager.h"
#include "Input/mouse.h"

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

        void InputCaptureData::AddInput(const Input::InputCode& Code)
        {
            this->insert(Code);
        }

        void InputCaptureData::AddInputRange(const Input::InputCode& Lower, const Input::InputCode& Upper)
        {
            for( Input::InputCode Code = Lower ; Code > Upper ; Code + 1 )
                this->insert(Code);
        }

        Whole InputCaptureData::GetNumCapturedInputs()
        {
            return this->size();
        }

        std::vector<Input::InputCode>* InputCaptureData::GetCapturedInputs()
        {
            return &CapturedCodes;
        }

        bool InputCaptureData::IsInputToBeCaptured(const Input::InputCode& Code)
        {
            std::set<Input::InputCode>::iterator it = this->find(Code);
            return it != this->end();
        }

        void InputCaptureData::UpdateCapturedInputs(std::vector<Input::InputCode>& InputCodes)
        {
            CapturedCodes.clear();
            CapturedCodes.swap(InputCodes);
        }

        //-----------------------------------------------------

        Widget::Widget(const String& name, Screen* Parent)
            : Renderable(name,Parent),
              HoveredSubWidget(NULL),
              SubWidgetFocus(NULL),
              CaptureData(NULL),
              Hovered(false)
        {
        }

        Widget::~Widget()
        {
            SubRenderables.clear();
            if(!Listeners.empty())
            {
                for( ListenerIterator It = Listeners.begin() ; It != Listeners.end() ; ++It )
                    delete (*It);
            }
            Listeners.clear();
        }

        void Widget::Update(bool Force)
        {
            if(!Listeners.empty())
            {
                for( ListenerIterator It = Listeners.begin() ; It != Listeners.end() ; ++It )
                {
                    (*It)->DoPreUpdateItems();
                }
            }

            SubWidgetUpdate(Force);
            UpdateImpl(Force);

            if(!Listeners.empty())
            {
                for( ListenerIterator It = Listeners.begin() ; It != Listeners.end() ; ++It )
                {
                    (*It)->DoPostUpdateItems();
                }
            }
        }

        void Widget::SubWidgetUpdate(bool Force)
        {
            Input::ButtonState State = Input::InputManager::GetSingletonPtr()->GetSystemMouse()->GetButtonState(1);
            if(HoveredSubWidget)
            {
                HoveredSubWidget->Update(Force);
                if(Input::BUTTON_PRESSING == State)
                {
                    SubWidgetFocus = HoveredSubWidget;
                }
            }
            if(SubWidgetFocus && (SubWidgetFocus != HoveredSubWidget))
            {
                SubWidgetFocusUpdate(true);
            }
            else if(Input::BUTTON_DOWN == State && Force)
            {
                SubWidgetFocusUpdate(Force);
            }
            if(Input::BUTTON_LIFTING == State)
            {
                SubWidgetFocus = NULL;
            }
        }

        void Widget::SubWidgetFocusUpdate(bool Force)
        {
            if(SubWidgetFocus)
                SubWidgetFocus->Update(Force);
        }

        void Widget::AddSubRenderable(const UInt16& Zorder, Renderable* ToAdd)
        {
            for( RenderableIterator RendIt = SubRenderables.begin() ; RendIt != SubRenderables.end() ; ++RendIt )
            {
                if( (*RendIt)->GetZOrder() > Zorder )
                {
                    SubRenderables.insert(RendIt,ToAdd);
                    ToAdd->ParentWidget = this;
                    ToAdd->_SetZOrder(Zorder);
                    return;
                }
            }
            SubRenderables.push_back(ToAdd);
            ToAdd->ParentWidget = this;
            ToAdd->_SetZOrder(Zorder);
            return;
        }

        void Widget::ProcessCapturedInputs()
        {
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Utility Methods

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

        bool Widget::CheckMouseHover()
        {
            if(!IsVisible())
            {
                HoveredSubWidget = NULL;
                Hovered = false;
            }
            else if(CheckMouseHoverImpl())
            {
                Hovered = true;
            }
            else
            {
                HoveredSubWidget = NULL;
                Hovered = false;
            }
            if(!Listeners.empty())
            {
                for( ListenerIterator It = Listeners.begin() ; It != Listeners.end() ; ++It )
                {
                    (*It)->DoHoverItems();
                }
            }
            return Hovered;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Visibility Methods

        void Widget::SetVisible(bool visible)
        {
            if(this->Visible == visible)
                return;
            this->Visible = visible;
            SetVisibleImpl(visible);
            if(!Listeners.empty())
            {
                for( ListenerIterator It = Listeners.begin() ; It != Listeners.end() ; ++It )
                {
                    (*It)->DoVisibilityChangeItems();
                }
            }
        }

        bool Widget::GetVisible() const
        {
            return Visible;
        }

        bool Widget::IsVisible() const
        {
            return Visible && ParentScreen->IsVisible();
        }

        void Widget::Show()
        {
            SetVisible(true);
        }

        void Widget::Hide()
        {
            SetVisible(false);
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Transform Methods

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
            return RelPosition * ParentScreen->GetViewportDimensions();
        }

        Vector2 Widget::GetSize() const
        {
            return RelSize;
        }

        Vector2 Widget::GetActualSize() const
        {
            return RelSize * ParentScreen->GetViewportDimensions();
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Render Priority Methods

        void Widget::SetRenderPriority(const UI::RenderPriority& Priority)
        {
            Renderable::SetRenderPriority(Priority);
            for( RenderableIterator it = SubRenderables.begin() ; it != SubRenderables.end() ; ++it )
            {
                (*it)->SetRenderPriority(this->Priority);
            }
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Listener Methods

        void Widget::AddWidgetListener(WidgetListener* Listener)
        {
            for( ListenerIterator It = Listeners.begin() ; It != Listeners.end() ; ++It )
            {
                if( (*It) == Listener )
                {
                    MEZZ_EXCEPTION(Exception::II_DUPLICATE_IDENTITY_EXCEPTION,"Listener being added to widget is already registered with widget \"" + Name + "\".");
                }
            }
            Listeners.push_back(Listener);
            Listener->SetCaller(this);
        }

        void Widget::RemoveWidgetListener(WidgetListener* Listener)
        {
            for( ListenerIterator It = Listeners.begin() ; It != Listeners.end() ; ++It )
            {
                if( (*It) == Listener )
                {
                    Listeners.erase(It);
                    Listener->SetCaller(NULL);
                    return;
                }
            }
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Fetch Methods

        Widget* Widget::GetHoveredSubWidget() const
        {
            return HoveredSubWidget;
        }

        Widget* Widget::GetBottomMostHoveredWidget()
        {
            if(HoveredSubWidget) return HoveredSubWidget->GetBottomMostHoveredWidget();
            else return this;
        }

        Widget* Widget::GetTopMostWidget()
        {
            if(ParentWidget) return ParentWidget->GetTopMostWidget();
            else return this;
        }

        Screen* Widget::GetParent() const
        {
            return ParentScreen;
        }

        InputCaptureData* Widget::GetInputCaptureData() const
        {
            return CaptureData;
        }

        Widget::RenderableIterator Widget::BeginRenderable()
        {
            return SubRenderables.begin();
        }

        Widget::RenderableIterator Widget::EndRenderable()
        {
            return SubRenderables.end();
        }

        Widget::ConstRenderableIterator Widget::BeginRenderable() const
        {
            return SubRenderables.begin();
        }

        Widget::ConstRenderableIterator Widget::EndRenderable() const
        {
            return SubRenderables.end();
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Internal Methods

        void Widget::_MarkDirty()
        {
            Dirty = true;
            if(ParentWidget)
                ParentWidget->_MarkDirty();
            else
                ParentScreen->_RequestIndexRedraw(ZOrder);
        }

        void Widget::_Redraw()
        {
            for( RenderableIterator it = SubRenderables.begin() ; it != SubRenderables.end() ; ++it )
            {
                if( (*it)->Dirty )
                    (*it)->_Redraw();
            }
        }

        void Widget::_AppendVertices(ScreenVertexData& Vertices)
        {
            for( RenderableIterator it = SubRenderables.begin() ; it != SubRenderables.end() ; ++it )
            {
                if( (*it)->IsVisible() )
                    (*it)->_AppendVertices(Vertices);
            }
        }

        //-----------------------------------------------------

        WidgetListener::WidgetListener()
            : Caller(NULL)
        {
        }

        WidgetListener::~WidgetListener()
        {
        }

        void WidgetListener::SetCaller(Widget* Caller)
        {
            this->Caller = Caller;
        }
    }//UI
}//Mezzanine

#endif
