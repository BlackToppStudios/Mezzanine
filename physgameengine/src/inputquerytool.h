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
#ifndef _inputquerytool_h
#define _inputquerytool_h

#include "crossplatformexport.h"
#include "datatypes.h"
#include "metacode.h"
#include "vector2.h"

#include <vector>

namespace phys
{
    class World;
    ///////////////////////////////////////////////////////////////////////////////
    /// @class InputQueryTool
    /// @headerfile inputquerytool.h
    /// @brief This provides a number of utilities for getting input information.
    /// @details This is useful for quickly and easily getting common input information.
    ///////////////////////////////////////
    class PHYS_LIB InputQueryTool
    {
        protected:
            /// @internal
            /// @brief A place to store which keys and mouse buttons are pressed or not.
            /// @details In the constructor we force the insertion INPUTEVENT_LAST
            std::vector<int> CodeCache;

        public:
            /// @brief Basic Constructor.
            /// @details This creates a InputQueryTool ready to run queries on the input.
            InputQueryTool();

            /// @brief Destructor
            /// @details Deletes everything in the input query tool.
            ~InputQueryTool();

            /// @brief This gets the X coordinate of the mouse
            /// @details This gets the X location of this mouse. This runs in constant time.
            /// @return This returns a Whole number which represents the X coordinate of the mouse.
            int GetMouseX();

            /// @brief This gets the Y coordinate of the mouse
            /// @details This gets the Y location of this mouse. This runs in constant time.
            /// @return This returns a Whole number which represents the Y coordinate of the mouse.
            int GetMouseY();

            /// @brief This gets a vector2 containing both X and Y coordinates of the mouse.
            /// @details If precision is important then you should use the individual coordinate fetching
            /// functions instead.
            /// @return This returns a Vector2 containing Reals to represent both mouse coordinates.
            Vector2 GetMouseCoordinates();

            /// @brief This gets the offset location of the mouse since the previous frame.
            /// @details This Vector2 is only accurate if this class gathers events every frame.  Avoid calling this function otherwise.
            /// @return This returns a vector2 that stores how the mouse has moved since the previous frame.
            Vector2 GetMousePrevFrameOffset();

            /// @brief Get an unfiltered uninterpretted version of the value for an user input
            /// @param AnyCode The InputCode to retrieve the value for
            /// @return An int which may or may not correspond to a ButtonState, MouseWheelState, or some other Value.
            int GetRawMetaValue(MetaCode::InputCode AnyCode);

            /// @brief Returns whether a specific Mouse button is pushed
            /// @details This runs in constant time and returns a true is the requested mouse button is pressed. Buttons that are being pressed
            /// are considered pressed, and buttons that are being lifted are considered unpressed.
            /// @param MouseButton This is the mouse button that is being checked
            /// @return This returns a bool which is set to true if the requested button is pressed or held down, and false otherwise.
            bool IsMouseButtonPushed(short unsigned int MouseButton);

            /// @brief Returns whether a specific Joystick button is pushed
            /// @details This runs in constant time and returns a true is the requested Joystick button is pressed. Buttons that are being pressed
            /// are considered pressed, and buttons that are being lifted are considered unpressed.
            /// @param JoyStickButton This is the Joystick button that is being checked
            /// @return This returns a bool which is set to true if the requested button is pressed or held down, and false otherwise.
            bool IsJoystickButtonPushed(short unsigned int JoyStickButton);

            /// @brief Returns whether a specific Keyboard button is pushed
            /// @details This runs in constant time and returns a true is the requested mouse button is pressed. Buttons that are being pressed
            /// are considered pressed, and buttons that are being lifted are considered unpressed.
            /// @param KeyboardButton This is the button that is being checked.
            /// @return This returns a bool which is set to true if the requested button is pressed or held down, and false otherwise.
            bool IsKeyboardButtonPushed(MetaCode::InputCode KeyboardButton);

            /// @brief Gets the button state of the provided mouse button ID.
            /// @param MouseButton The mouse button ID(up to 16) you wish to query.
            /// @details Far from all mice have 16 buttons, be sure you know how many buttons the mouse has
            /// As this function doesn't perform that check.
            /// @return Returns a button state enum representing the button state.
            MetaCode::ButtonState GetMouseButtonState(short unsigned int MouseButton);

            /// @brief Gets the button state of the provided keyboard button ID.
            /// @param KeyboardButton The Input code for the keyboard button you wish to query.
            /// @return Returns a button state enum representing the button state.
            MetaCode::ButtonState GetKeyboardButtonState(MetaCode::InputCode KeyboardButton);

            /// @brief Gets the current status of the mouse wheel.
            /// @return Returns an enum value representing the current state of the mouse wheel.
            MetaCode::DirectionalMotionState GetMouseWheelState();

            /// @brief This gathers any user-input/event data that might be queryed
            /// @details This should be called periodcally (ideally in the post user input callback) to allow this
            /// to gather data from the phys::World 's event manager. When called this will drop prior event data
            /// and any relevant queries will come from this new data. At the caller's discretion this method
            /// can properly delete any events pulled from the event manager. \n \n
            /// This runs in linear time relative to the events in the event manager. This will usually be a trivial amount
            /// if this is run each iteration and excess events are removed (either by this method or some other form of event cleanup)
            /// @param ClearEventsFromEventMgr If set to true, This method will properly remove any events it pulls from the event manager.
            void GatherEvents(bool ClearEventsFromEventMgr = false);
    };
}

#endif
