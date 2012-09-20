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
#ifndef _inputmouse_cpp
#define _inputmouse_cpp

#include "Input/mouse.h"
#include "Graphics/viewport.h"
#include "Graphics/gamewindow.h"

#include <cstring>

#include "SDL.h"
#include "../src/video/SDL_sysvideo.h"

namespace Mezzanine
{
    namespace Input
    {
        Mouse::Mouse()
            : CurrentViewport(NULL),
              VerticalWheelState(Input::DIRECTIONALMOTION_UNCHANGED),
              HorizontalWheelState(Input::DIRECTIONALMOTION_UNCHANGED)
        {
            Buttons.resize( (Input::MOUSEBUTTON_LAST - Input::MOUSEBUTTON_FIRST) + 1 );
            memset( &Buttons[0],Input::BUTTON_UP,sizeof(Buttons[0]) * Buttons.size() );
        }

        Mouse::~Mouse()
        {
        }

        void Mouse::UpdateImpl(std::vector<MetaCode>& Codes)
        {
            /// @todo Getting the mouse focus doesn't do what I originally thought it would.  When a mouse leaves a window, the focus isn't set to NULL
            /// as expected.  If you have only one window, then it just stays pointing to that window.  If you have two then it will stay on the first
            /// window until you mouse over the second, even if there is a space between the windows.  This should be updated somehow so that we can set
            /// "HoveredWindow" to NULL when none of our windows are being hovered.
            // First do some setup.  Get the window and zero out button presses(Key::Up isn't reported).
            SDL_Window* Focus = SDL_GetMouseFocus();
            // Now do the actual updating.
            for( Whole X = 0 ; X < Codes.size() ; ++X )
            {
                MetaCode& CurrCode = Codes[X];
                if( CurrCode.GetCode() >= Input::MOUSEBUTTON_FIRST && CurrCode.GetCode() <= Input::MOUSEBUTTON_LAST )
                {
                    TransitioningIndexes.push_back( CurrCode.GetCode() - Input::MOUSEBUTTON_FIRST );
                    Buttons.at( CurrCode.GetCode() - Input::MOUSEBUTTON_FIRST ) = static_cast<Input::ButtonState>(CurrCode.GetMetaValue());
                }
                else if( Input::MOUSEWHEELVERTICAL == CurrCode.GetCode() )
                {
                    VerticalWheelState = static_cast<Input::DirectionalMotionState>(CurrCode.GetMetaValue());
                }
                else if( Input::MOUSEWHEELHORIZONTAL == CurrCode.GetCode() )
                {
                    HorizontalWheelState = static_cast<Input::DirectionalMotionState>(CurrCode.GetMetaValue());
                }
                // Only if we're on a window
                if( Focus )
                {
                    if( Input::MOUSEABSOLUTEVERTICAL == CurrCode.GetCode() )
                    {
                        Position.Y = (Real)(CurrCode.GetMetaValue());
                    }
                    else if( Input::MOUSEABSOLUTEHORIZONTAL == CurrCode.GetCode() )
                    {
                        Position.X = (Real)(CurrCode.GetMetaValue());
                    }
                    else if( Input::MOUSEVERTICAL == CurrCode.GetCode() )
                    {
                        Delta.Y = (Real)(CurrCode.GetMetaValue());
                    }
                    else if( Input::MOUSEHORIZONTAL == CurrCode.GetCode() )
                    {
                        Delta.X = (Real)(CurrCode.GetMetaValue());
                    }
                }
            }
            if( NULL != Focus )
            {
                Graphics::GameWindow* Win = static_cast<Graphics::GameWindow*>(Focus->data->data);
                for( Graphics::GameWindow::ViewportIterator ViewIt = Win->BeginViewport() ; ViewIt != Win->EndViewport() ; ++ViewIt )
                {
                    if( (Position.X >= (Real)((*ViewIt)->GetActualLeft()) && Position.X <= (Real)((*ViewIt)->GetActualLeft() + (*ViewIt)->GetActualWidth())) &&
                        (Position.Y >= (Real)((*ViewIt)->GetActualTop()) && Position.Y <= (Real)((*ViewIt)->GetActualTop() + (*ViewIt)->GetActualHeight()) ) )
                    {
                        CurrentViewport = (*ViewIt);
                        break;
                    }
                }
            }else{
                CurrentViewport = NULL;
                Position.SetIdentity();
                Delta.SetIdentity();
            }
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Query Methods

        Graphics::Viewport* Mouse::GetHoveredViewport() const
        {
            return CurrentViewport;
        }

        Graphics::GameWindow* Mouse::GetHoveredWindow() const
        {
            if(CurrentViewport) return CurrentViewport->GetParentWindow();
            else return NULL;
        }

        const Vector2& Mouse::GetWindowPosition() const
        {
            return Position;
        }

        Real Mouse::GetWindowX() const
        {
            return Position.X;
        }

        Real Mouse::GetWindowY() const
        {
            return Position.Y;
        }

        Vector2 Mouse::GetViewportPosition() const
        {
            if(CurrentViewport) return ( Position - Vector2(CurrentViewport->GetLeft(),CurrentViewport->GetTop()) );
            else return Vector2(0,0);
        }

        Real Mouse::GetViewportX() const
        {
            return GetViewportPosition().X;
        }

        Real Mouse::GetViewportY() const
        {
            return GetViewportPosition().Y;
        }

        const Vector2& Mouse::GetMouseDelta() const
        {
            return Delta;
        }

        Real Mouse::GetDeltaX() const
        {
            return Delta.X;
        }

        Real Mouse::GetDeltaY() const
        {
            return Delta.Y;
        }

        const Input::ButtonState& Mouse::GetButtonState(const UInt16 Button) const
        {
            return Buttons.at( Button - 1 );
        }

        const Input::ButtonState& Mouse::GetButtonState(const Input::InputCode& Button) const
        {
            return Buttons.at( Button - Input::MOUSEBUTTON_FIRST );
        }

        const Input::DirectionalMotionState& Mouse::GetVerticalWheelState() const
        {
            return VerticalWheelState;
        }

        const Input::DirectionalMotionState& Mouse::GetHorizontalWheelState() const
        {
            return HorizontalWheelState;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Configuration Methods

        void Mouse::SetCursorVisibility(bool Visible)
        {
            SDL_ShowCursor(Visible);
        }

        bool Mouse::GetCursorVisibility()
        {
            return 0 != SDL_ShowCursor(-1);
        }

        bool Mouse::SetRelativeMode(bool Enable)
        {
            return 0 == SDL_SetRelativeMouseMode( (Enable?SDL_TRUE:SDL_FALSE) );
        }

        bool Mouse::GetRelativeMode()
        {
            return SDL_GetRelativeMouseMode();
        }

        void Mouse::SetMouseCursor()
        {
            /// @todo Implement this.
            MEZZ_EXCEPTION(Exception::NOT_IMPLEMENTED_EXCEPTION,"Mouse cursor setting is currently not implemented.");
        }

        void Mouse::GetMouseCursor()
        {
            /// @todo Implement this.
            MEZZ_EXCEPTION(Exception::NOT_IMPLEMENTED_EXCEPTION,"Mouse cursor setting is currently not implemented.");
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Utility Methods

        void Mouse::WarpCursorToPosition(Graphics::GameWindow* Win, const Vector2& Position)
        {
            if(Win)
                SDL_WarpMouseInWindow(Win->_GetSDLWindowPointer(),(int)Position.X,(int)Position.Y);
        }
    }//Input
}//Mezzanine

#endif
