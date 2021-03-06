// © Copyright 2010 - 2017 BlackTopp Studios Inc.
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
#ifndef _inputmouse_h
#define _inputmouse_h

#include "vector2.h"
#include "timer.h"
#include "Input/inputenumerations.h"
#include "Input/buttondevice.h"

namespace Mezzanine
{
    namespace Graphics
    {
        class Viewport;
        class GameWindow;
    }
    namespace Input
    {
        ///////////////////////////////////////////////////////////////////////////////
        /// @brief This class represents the mouse input device.
        ///////////////////////////////////////
        class MEZZ_LIB Mouse : public ButtonDevice<Input::MOUSE_BUTTONMAX>
        {
        protected:
            /// @brief The timer used to detect multi-clicks.
            Timer MulticlickTimer;
            /// @brief A copy of the current MetaCode being tracked for multi-click detection.
            MetaCode MulticlickCode;
            /// @brief The current screen position of the mouse cursor.
            Vector2 Position;
            /// @brief The change in the mouse cursor position since the last update.
            Vector2 Delta;
            /// @brief The viewport that the mouse cursor is currently in.
            Graphics::Viewport* CurrentViewport;
            /// @brief The current state of the horizontal mouse wheel (if present).
            Input::DirectionalMotionState HorizontalWheelState;
            /// @brief The current state of the vertical mouse wheel (if present).
            Input::DirectionalMotionState VerticalWheelState;

            /// @copydoc ButtonDevice::UpdateImpl(ConstMetaCodeIterator, ConstMetaCodeIterator)
            MetaCodeContainer UpdateImpl(ConstMetaCodeIterator DeltaBegin, ConstMetaCodeIterator DeltaEnd);
            /// @brief Resets the saved mouse wheel states to their defaults.
            void ResetWheelState();
            /// @brief Updates the current viewport (if applicable) based on the mouse cursor.
            void UpdateViewport();
            /// @brief Checks a MetaCode to see if it can generate multi-click codes and updates the state if necessary.
            /// @param ToCheck The MetaCode to verify for multi-click updates.
            void HandleMultiClick(const MetaCode& ToCheck);

            /// @brief Checks to see if this code pertains to a button we can track multiple clicks for.
            /// @param ToCheck The MetaCode to check.
            Boole IsMultiClickable(const MetaCode& ToCheck) const;
            /// @brief Converts a standard mouse button code to the appropriate multi-click code for that button.
            /// @param Code The InputCode to be converted.
            Input::InputCode ConvertToMultiClickCode(const Input::InputCode Code) const;
        public:
            /// @brief Class constructor.
            Mouse();
            /// @brief Copy constructor.
            /// @param Other The other Mouse to be copied.
            Mouse(const Mouse& Other) = delete;
            /// @brief Move constructor.
            /// @param Other The other Mouse to be moved.
            Mouse(Mouse&& Other) = delete;
            /// @brief Class destructor.
            virtual ~Mouse() = default;

            ///////////////////////////////////////////////////////////////////////////////
            // Operators

            /// @brief Assignment operator.
            /// @param Other The other Mouse to be copied.
            /// @return Returns a reference to this.
            Mouse& operator=(const Mouse& Other) = delete;
            /// @brief Move assignment operator.
            /// @param Other The other Mouse to be moved.
            /// @return Returns a reference to this.
            Mouse& operator=(Mouse&& Other) = delete;

            ///////////////////////////////////////////////////////////////////////////////
            // Query Methods

            /// @brief Gets a pointer to the current viewport the mouse cursor is hovered over.
            /// @return Returns a pointer to the hovered viewport.
            Graphics::Viewport* GetHoveredViewport() const;
            /// @brief Gets a pointer to the window with the current mouse focus.
            /// @return Returns a pointer to the window with the current focus.
            Graphics::GameWindow* GetHoveredWindow() const;

            /// @brief Gets the position of the mouse cursor relative to the origin of the window with the mouse focus.
            /// @return Returns a const reference to the mouse's current position on the window.
            const Vector2& GetWindowPosition() const;
            /// @brief Gets the X position of the mouse relative to the window.
            /// @return Returns a real with the current window position on the X axis.
            Real GetWindowX() const;
            /// @brief Gets the Y position of the mouse relative to the window.
            /// @return Returns a real with the current window position on the Y axis.
            Real GetWindowY() const;
            /// @brief Gets the position of the mouse cursor relative to the origin of the viewport in the window with the mouse focus.
            /// @return Returns a vector2 to the mouse's current position on the current viewport.
            Vector2 GetViewportPosition() const;
            /// @brief Gets the X position of the mouse relative to the viewport.
            /// @return Returns a real with the current viewport position on the X axis.
            Real GetViewportX() const;
            /// @brief Gets the Y position of the mouse relative to the viewport.
            /// @return Returns a real with the current viewport position on the Y axis.
            Real GetViewportY() const;
            /// @brief Gets the change in the mouse position from the previous update.
            /// @return Returns a const reference to the change in the mouse position.
            const Vector2& GetMouseDelta() const;
            /// @brief Gets the X delta of the mouse position from the last update.
            /// @return Returns a real with the offset from the previous update on the X axis.
            Real GetDeltaX() const;
            /// @brief Gets the Y delta of the mouse position from the last update.
            /// @return Returns a real with the offset from the previous update on the Y axis.
            Real GetDeltaY() const;

            /// @copydoc Device::GetDeviceID() const
            DeviceIDType GetDeviceID() const;
            /// @copydoc Device::GetDeviceType() const
            Input::InputDevice GetDeviceType() const;
            /// @copydoc ButtonDevice::GetButtonState(const UInt16) const
            Input::ButtonState GetButtonState(const UInt16 Button) const;
            /// @copydoc ButtonDevice::GetButtonState(const Input::InputCode) const
            Input::ButtonState GetButtonState(const Input::InputCode Button) const;
            /// @copydoc ButtonDevice::GetFirstButtonCode() const
            Input::InputCode GetFirstButtonCode() const;

            /// @brief Gets the current state of the vertical mouse wheel.
            /// @remarks If a mouse doesn't have a mouse wheel this will always report "DIRECTIONALMOTION_UNCHANGED".
            /// @return Returns a directional motion update of the mouse wheel.
            Input::DirectionalMotionState GetVerticalWheelState() const;
            /// @brief Gets the current state of the horizontal mouse wheel.
            /// @remarks If a mouse doesn't have a mouse wheel this will always report "DIRECTIONALMOTION_UNCHANGED".
            /// @return Returns a directional motion update of the mouse wheel.
            Input::DirectionalMotionState GetHorizontalWheelState() const;

            ///////////////////////////////////////////////////////////////////////////////
            // Configuration Methods

            /// @brief Sets the visibility of the mouse cursor.
            /// @param Visible Whether or not the cursor is to be visible.
            void SetCursorVisibility(Boole Visible);
            /// @brief Gets the current state of the visibility of the cursor.
            /// @return Returns true if the cursor is visible, false otherwise.
            Boole GetCursorVisibility();
            /// @brief Sets whether or not relative mode is enabled.
            /// @return Returns false if relative mode is not supported on this system.  True if the operation was a success.
            /// @remarks "Relative mode" entails hiding the cursor, locking the position, and only updating the mouse delta position each update.
            /// This is useful for First Person Shooter style games.
            /// @param Enable True to enable relative mode, false to disable.
            Boole SetRelativeMode(Boole Enable);
            /// @brief Gets whether or not relative mode is enabled.
            /// @return Returns true if relative mode is enabled, false otherwise.
            Boole GetRelativeMode();
            /// @brief Sets the mouse cursor.
            void SetMouseCursor();
            /// @brief Gets the current mouse cursor.
            void GetMouseCursor();

            ///////////////////////////////////////////////////////////////////////////////
            // Utility Methods

            /// @brief Sets the mouse cursor's position to the specified point in the specified window.
            /// @param Win The window to warp the cursor to.  Pointer is expected to be valid.
            /// @param Position The position on the specified window to warp the cursor to.
            void WarpCursorToPosition(Graphics::GameWindow* Win, const Vector2& Position);
        };//Mouse
    }//Input
}//Mezzanine

#endif
