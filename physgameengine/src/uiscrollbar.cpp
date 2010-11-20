//© Copyright 2010 BlackTopp Studios Inc.
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
#ifndef _uiscrollbar_cpp
#define _uiscrollbar_cpp

#include "uiscrollbar.h"
#include "uilayer.h"
#include "uimanager.h"
#include "uibutton.h"
#include "uirectangle.h"

namespace phys
{
    namespace UI
    {
        Scrollbar::Scrollbar(String& name, Vector2 Position, Vector2 Size, Scrollbar::BarStyle Style, UILayer* parent)
            : Widget(name, parent)
        {
            Type = Widget::Scrollbar;
            RelPosition = Position;
            RelSize = Size;
            Position = Position * Manager->GetWindowDimensions();
            Size = Size * Manager->GetWindowDimensions();
            if(Size.Y > Size.X * 2)
            {
                CreateVerticalScrollbar(Position, Size, Style);
            }
            else if(Size.X > Size.Y * 2)
            {
                CreateHorizontalScrollbar(Position, Size, Style);
            }
            else
            {
                //World::GetWorldPointer()->LogAndThrow("Scrollbar dimensions incompatible with this widget.");
            }
        }

        Scrollbar::~Scrollbar()
        {
            delete Scroller;
            delete ScrollBack;
            delete UpLeftButton;
            delete DownRightButton;
        }

        void Scrollbar::CreateHorizontalScrollbar(Vector2 Position, Vector2 Size, Scrollbar::BarStyle Style)
        {
            //Get the size for all items involved
            Vector2 ScrollBackSize, LeftButtonSize, RightButtonSize;
            if(Scrollbar::NoButtons==Style)
            {
                ScrollBackSize = Size;
            }else{
                ScrollBackSize.X = Size.X - (Size.Y * 2);
                ScrollBackSize.Y = Size.Y;
                LeftButtonSize.X = Size.Y;
                LeftButtonSize.Y = Size.Y;
                RightButtonSize.X = Size.Y;
                RightButtonSize.Y = Size.Y;
            }
            //Get the position for all items involved
            Vector2 ScrollBackPosition, LeftButtonPosition, RightButtonPosition;
            if(Scrollbar::Separate==Style)
            {
                ScrollBackPosition.X = Position.X + LeftButtonSize.X;
                ScrollBackPosition.Y = Position.Y;
                LeftButtonPosition = Position;
                RightButtonPosition.X = Position.X + LeftButtonSize.X + ScrollBackSize.X;
                RightButtonPosition.Y = Position.Y;
            }
            else if(Scrollbar::TogetherDownRight==Style)
            {
                ScrollBackPosition = Position;
                LeftButtonPosition.X = Position.X + ScrollBackSize.X;
                LeftButtonPosition.Y = Position.Y;
                RightButtonPosition.X = Position.X + ScrollBackSize.X + LeftButtonSize.X;
                RightButtonPosition.Y = Position.Y;
            }
            else if(Scrollbar::TogetherUpLeft==Style)
            {
                ScrollBackPosition.X = Position.X + LeftButtonSize.X + RightButtonSize.X;
                ScrollBackPosition.Y = Position.Y;
                LeftButtonPosition = Position;
                RightButtonPosition.X = Position.X + LeftButtonSize.X;
                RightButtonPosition.Y = Position.Y;
            }
            else
            {
                ScrollBackPosition = Position;
            }
            //Now create the objects, since we have all the position and size data we need
        }

        void Scrollbar::CreateVerticalScrollbar(Vector2 Position, Vector2 Size, Scrollbar::BarStyle Style)
        {
            //Get the size for all items involved
            Vector2 ScrollBackSize, UpButtonSize, DownButtonSize;
            if(Scrollbar::NoButtons==Style)
            {
                ScrollBackSize = Size;
            }else{
                ScrollBackSize.X = Size.X;
                ScrollBackSize.Y = Size.Y - (Size.X * 2);
                UpButtonSize.X = Size.X;
                UpButtonSize.Y = Size.X;
                DownButtonSize.X = Size.X;
                DownButtonSize.Y = Size.X;
            }
            //Get the position for all items involved
            Vector2 ScrollBackPosition, UpButtonPosition, DownButtonPosition;
            if(Scrollbar::Separate==Style)
            {
                ScrollBackPosition.X = Position.X;
                ScrollBackPosition.Y = Position.Y + UpButtonSize.Y;
                UpButtonPosition = Position;
                DownButtonPosition.X = Position.X;
                DownButtonPosition.Y = Position.Y + UpButtonSize.Y + ScrollBackSize.Y;
            }
            else if(Scrollbar::TogetherDownRight==Style)
            {
                ScrollBackPosition = Position;
                UpButtonPosition.X = Position.X;
                UpButtonPosition.Y = Position.Y + ScrollBackSize.Y;
                DownButtonPosition.X = Position.X;
                DownButtonPosition.Y = Position.Y + ScrollBackSize.Y + UpButtonSize.Y;
            }
            else if(Scrollbar::TogetherUpLeft==Style)
            {
                ScrollBackPosition.X = Position.X;
                ScrollBackPosition.Y = Position.Y + UpButtonSize.Y + DownButtonSize.Y;
                UpButtonPosition = Position;
                DownButtonPosition.X = Position.X;
                DownButtonPosition.Y = Position.Y + UpButtonSize.Y;
            }
            else
            {
                ScrollBackPosition = Position;
            }
            //Now create the objects, since we have all the position and size data we need
        }

        bool Scrollbar::CheckMouseHover()
        {
            return false;
        }

        void Scrollbar::SetPosition(Vector2 Position)
        {

        }

        Vector2 Scrollbar::GetPosition()
        {
            return RelPosition;
        }

        void Scrollbar::SetActualPosition(Vector2 Position)
        {

        }

        Vector2 Scrollbar::GetActualPosition()
        {
            return Vector2(0,0);
        }

        void Scrollbar::SetSize(Vector2 Size)
        {

        }

        Vector2 Scrollbar::GetSize()
        {
            return RelSize;
        }

        void Scrollbar::SetActualSize(Vector2 Size)
        {

        }

        Vector2 Scrollbar::GetActualSize()
        {
            return Vector2(0,0);
        }
    }
}

#endif
