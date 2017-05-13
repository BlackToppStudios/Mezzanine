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
#ifndef _inputmouse_cpp
#define _inputmouse_cpp

#include "Input/mouse.h"
#include "Graphics/viewport.h"
#include "Graphics/gamewindow.h"
#include "exception.h"

#include <limits>

#include "SDL.h"
#include "../src/video/SDL_sysvideo.h"

namespace Mezzanine
{
    namespace Input
    {
        Mouse::Mouse() :
            CurrentViewport(NULL),
            HorizontalWheelState(Input::DIRECTIONALMOTION_UNCHANGED),
            VerticalWheelState(Input::DIRECTIONALMOTION_UNCHANGED)
        {
            //this->Buttons.resize( (Input::MOUSEBUTTON_LAST - Input::MOUSEBUTTON_FIRST) + 1, Input::BUTTON_UP );

            this->MulticlickTimer.SetCurrentTimeInMilliseconds(500);
            this->MulticlickTimer.SetCountMode(Mezzanine::CM_CountDown);
        }

        MetaCodeContainer Mouse::UpdateImpl(ConstMetaCodeIterator DeltaBegin, ConstMetaCodeIterator DeltaEnd)
        {
            // First save our position.
            SDL_Window* Focus = SDL_GetMouseFocus();
            Vector2 OldPosition = this->Position;

            this->ResetWheelState();
            // Update our states
            for( ConstMetaCodeIterator CurrDelta = DeltaBegin ; CurrDelta != DeltaEnd ; ++CurrDelta )
            {
                const MetaCode& CurrCode = (*CurrDelta);
                const Input::InputCode ICode = CurrCode.GetCode();
                if( ICode >= Input::MOUSEBUTTON_FIRST && ICode <= Input::MOUSEBUTTON_LAST ) {
                    // Mark the index for transition on the next update and then place it's state in the button vector
                    TransitioningIndexes.push_back( ICode - Input::MOUSEBUTTON_FIRST );
                    Buttons.at( ICode - Input::MOUSEBUTTON_FIRST ) = static_cast<Input::ButtonState>(CurrCode.GetMetaValue());
                    this->HandleMultiClick(CurrCode);
                }else if( Input::MOUSEWHEELVERTICAL == ICode ) {
                    this->VerticalWheelState = static_cast<Input::DirectionalMotionState>(CurrCode.GetMetaValue());
                }else if( Input::MOUSEWHEELHORIZONTAL == ICode ) {
                    this->HorizontalWheelState = static_cast<Input::DirectionalMotionState>(CurrCode.GetMetaValue());
                }
                // Only if we're on a window
                if( Focus != NULL ) {
                    if( Input::MOUSEABSOLUTEVERTICAL == ICode ) {
                        this->Position.Y = (Real)(CurrCode.GetMetaValue());
                    }else if( Input::MOUSEABSOLUTEHORIZONTAL == ICode ) {
                        this->Position.X = (Real)(CurrCode.GetMetaValue());
                    }
                }
            }

            // Update our current window
            this->UpdateViewport();
            this->Delta = this->Position - OldPosition;

            // Do our sequence updates
            MetaCodeContainer Ret = std::move( this->Sequences.Update(DeltaBegin,DeltaEnd) );

            // Do our Multiclick check
            if( this->MulticlickCode.GetMetaValue() > 1 )
                Ret.push_back(MulticlickCode);

            return Ret;
        }

        void Mouse::ResetWheelState()
        {
            /// @todo Wheel update events only occur when the wheel goes up or down.  So left on it's own it will constantly report up or down after one
            /// is fired.  Resetting it allows us more accurate reporting, but isn't particularly graceful.  More research in SDL ought to be done to see
            /// if a cleaner solution exists.
            this->VerticalWheelState = Input::DIRECTIONALMOTION_UNCHANGED;
            this->HorizontalWheelState = Input::DIRECTIONALMOTION_UNCHANGED;
        }

        void Mouse::UpdateViewport()
        {
            /// @todo Getting the mouse focus doesn't do what I originally thought it would.  When a mouse leaves a window, the focus isn't set to NULL
            /// as expected.  If you have only one window, then it just stays pointing to that window.  If you have two then it will stay on the first
            /// window until you mouse over the second, even if there is a space between the windows.  This should be updated somehow so that we can set
            /// "HoveredWindow" to NULL when none of our windows are being hovered.
            SDL_Window* Focus = SDL_GetMouseFocus();
            if( NULL != Focus ) {
                Graphics::GameWindow* Win = static_cast<Graphics::GameWindow*>(Focus->data->data);
                for( Whole VPIndex = Win->GetNumViewports() ; VPIndex > 0 ; --VPIndex )
                {
                    Graphics::Viewport* VP = Win->GetViewport( VPIndex - 1 );
                    if( VP->IsWithinBounds(this->Position) ) {
                        this->CurrentViewport = VP;
                        break;
                    }
                }
            }else{
                this->CurrentViewport = NULL;
                this->Position.SetIdentity();
                this->Delta.SetIdentity();
            }
        }

        void Mouse::HandleMultiClick(const MetaCode& ToCheck)
        {
            if( this->IsMultiClickable(ToCheck) && Input::BUTTON_PRESSING == ToCheck.GetMetaValueAsButtonState() ) {
                if( this->MulticlickTimer.IsStopped() ) {
                    this->MulticlickTimer.SetCurrentTime(500 * 1000);
                    this->MulticlickCode.SetMetaValue(0);
                }else{
                    this->MulticlickTimer.Reset(500 * 1000);
                }

                const Input::InputCode MCICode = this->ConvertToMultiClickCode(ToCheck.GetCode());
                if( this->MulticlickCode.GetCode() != MCICode ) {
                    this->MulticlickCode.SetMetaValue(1);
                    this->MulticlickCode.SetCode(MCICode);
                }else{
                    Int32 ClickCount = this->MulticlickCode.GetMetaValue();
                    this->MulticlickCode.SetMetaValue( ++ClickCount );
                }
                this->MulticlickTimer.Start();
            }
        }

        Boole Mouse::IsMultiClickable(const MetaCode& ToCheck) const
        {
            Input::InputCode Code = ToCheck.GetCode();
            return (Input::MOUSEBUTTON_1 <= Code && Input::MOUSEBUTTON_2 >= Code);
        }

        Input::InputCode Mouse::ConvertToMultiClickCode(const Input::InputCode Code) const
        {
            switch(Code)
            {
                case Input::MOUSEBUTTON_1: return Input::COMPOUNDINPUT_MOUSELEFTMULTICLICK;  break;
                case Input::MOUSEBUTTON_2: return Input::COMPOUNDINPUT_MOUSERIGHTMULTICLICK; break;
                default:
                {
                    MEZZ_EXCEPTION(ExceptionBase::PARAMETERS_EXCEPTION,"Attempting to convert unsupported mouse button code into a multiclick code.");
                }
            }
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Query Methods

        Graphics::Viewport* Mouse::GetHoveredViewport() const
            { return this->CurrentViewport; }

        Graphics::GameWindow* Mouse::GetHoveredWindow() const
            { return ( this->CurrentViewport ? this->CurrentViewport->GetParentWindow() : NULL ); }

        const Vector2& Mouse::GetWindowPosition() const
            { return this->Position; }

        Real Mouse::GetWindowX() const
            { return this->Position.X; }

        Real Mouse::GetWindowY() const
            { return this->Position.Y; }

        Vector2 Mouse::GetViewportPosition() const
            { return ( this->CurrentViewport ? this->Position - Vector2( this->CurrentViewport->GetLeft(),this->CurrentViewport->GetTop() ) : Vector2(0,0) ); }

        Real Mouse::GetViewportX() const
            { return this->GetViewportPosition().X; }

        Real Mouse::GetViewportY() const
            { return this->GetViewportPosition().Y; }

        const Vector2& Mouse::GetMouseDelta() const
            { return this->Delta; }

        Real Mouse::GetDeltaX() const
            { return this->Delta.X; }

        Real Mouse::GetDeltaY() const
            { return this->Delta.Y; }

        DeviceIDType Mouse::GetDeviceID() const
            { return std::numeric_limits<DeviceIDType>::max(); }

        Input::InputDevice Mouse::GetDeviceType() const
            { return Input::DEVICE_MOUSE; }

        Input::ButtonState Mouse::GetButtonState(const UInt16 Button) const
            { return this->Buttons.at( Button - 1 ); }

        Input::ButtonState Mouse::GetButtonState(const Input::InputCode Button) const
            { return this->Buttons.at( Button - Input::MOUSEBUTTON_FIRST ); }

        Input::InputCode Mouse::GetFirstButtonCode() const
            { return Input::MOUSEBUTTON_FIRST; }

        Input::DirectionalMotionState Mouse::GetVerticalWheelState() const
            { return this->VerticalWheelState; }

        Input::DirectionalMotionState Mouse::GetHorizontalWheelState() const
            { return this->HorizontalWheelState; }

        ///////////////////////////////////////////////////////////////////////////////
        // Configuration Methods

        void Mouse::SetCursorVisibility(Boole Visible)
            { SDL_ShowCursor(Visible); }

        Boole Mouse::GetCursorVisibility()
            { return 0 != SDL_ShowCursor(-1); }

        Boole Mouse::SetRelativeMode(Boole Enable)
            { return 0 == SDL_SetRelativeMouseMode( (Enable?SDL_TRUE:SDL_FALSE) ); }

        Boole Mouse::GetRelativeMode()
            { return SDL_GetRelativeMouseMode(); }

        void Mouse::SetMouseCursor()
            { MEZZ_EXCEPTION(ExceptionBase::NOT_IMPLEMENTED_EXCEPTION,"Mouse cursor setting is currently not implemented."); } /// @todo Implement this.

        void Mouse::GetMouseCursor()
            { MEZZ_EXCEPTION(ExceptionBase::NOT_IMPLEMENTED_EXCEPTION,"Mouse cursor getting is currently not implemented."); } /// @todo Implement this.

        ///////////////////////////////////////////////////////////////////////////////
        // Utility Methods

        void Mouse::WarpCursorToPosition(Graphics::GameWindow* Win, const Vector2& Position)
            { SDL_WarpMouseInWindow(Win->_GetSDLWindowPointer(),(int)Position.X,(int)Position.Y); }
    }//Input
}//Mezzanine

#endif
