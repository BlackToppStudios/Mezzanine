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
#ifndef _uiwindow_cpp
#define _uiwindow_cpp

#include "UI/window.h"
#include "UI/uimanager.h"
#include "UI/screen.h"
#include "entresol.h"

namespace Mezzanine
{
    namespace UI
    {
        /*Window::Window(ConstString& name, const Rect& RendRect, Screen* PScreen) :
            EnclosedRenderableContainerWidget(name,RendRect,PScreen),
            BorderWidth(0),
            CurrentRM(RM_None)
        {
            Type = Widget::W_Window;
        }

        Window::~Window()
        {
        }

        void Window::BorderAreaCheck(const Vector2& ScreenLoc)
        {
            if(!BorderWidth)
                return;
            Vector2 Size = WidgetBack->GetActualSize();
            Vector2 Position = WidgetBack->GetActualPosition();
            bool Left = ScreenLoc.X >= Position.X && ScreenLoc.X <= Position.X + BorderWidth;
            bool Right = ScreenLoc.X >= Position.X + Size.X && ScreenLoc.X <= (Position.X + Size.X) - BorderWidth;
            bool Top = ScreenLoc.Y >= Position.Y && ScreenLoc.Y <= Position.Y + BorderWidth;
            bool Bottom = ScreenLoc.Y >= Position.Y + Size.Y && ScreenLoc.Y <= (Position.Y + Size.Y) - BorderWidth;
            if( Top && Left ) CurrentRM = Window::RM_TopLeft;
            else if( Top && Right ) CurrentRM = Window::RM_TopRight;
            else if( Bottom && Left ) CurrentRM = Window::RM_BottomLeft;
            else if( Bottom && Right ) CurrentRM = Window::RM_BottomRight;
            else if( Left ) CurrentRM = Window::RM_Left;
            else if( Right ) CurrentRM = Window::RM_BottomRight;
            else if( Top ) CurrentRM = Window::RM_BottomRight;
            else if( Bottom ) CurrentRM = Window::RM_BottomRight;
        }

        void Window::UpdateImpl(bool Force)
        {
            EnclosedRenderableContainerWidget::UpdateImpl(Force);
        }

        void Window::SetVisibleImpl(bool visible)
        {
            EnclosedRenderableContainerWidget::SetVisibleImpl(visible);
        }

        bool Window::CheckMouseHoverImpl()
        {
            return EnclosedRenderableContainerWidget::CheckMouseHoverImpl();
        }

        void Window::SetPosition(const Vector2& Position)
        {
            EnclosedRenderableContainerWidget::SetPosition(Position);
        }

        void Window::SetActualPosition(const Vector2& Position)
        {
            EnclosedRenderableContainerWidget::SetActualPosition(Position);
        }

        void Window::SetSize(const Vector2& Size)
        {
            EnclosedRenderableContainerWidget::SetSize(Size);
        }

        void Window::SetActualSize(const Vector2& Size)
        {
            EnclosedRenderableContainerWidget::SetActualSize(Size);
        }

        void Window::UpdateDimensions()
        {
            EnclosedRenderableContainerWidget::UpdateDimensions();
        }

        Rectangle* Window::GetWindowBack()
        {
            return WidgetBack;
        }//*/
    }//UI
}//Mezzanine

#endif
