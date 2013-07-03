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

#ifndef _inputmetacode_h
#define _inputmetacode_h

///////////////////////////////////////////////////////////////////////////////
// This is a container for user the smallest possible unit of userinput
///////////////////////////////////////

//From SDL
/*
    SDL - Simple DirectMedia Layer
    Copyright (C) 1997-2011 Sam Lantinga

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

    Sam Lantinga
    slouken@libsdl.org
*/

#include "datatypes.h"
#include "XML/xml.h"
#include "Input/inputenumerations.h"

namespace Mezzanine
{
    ///////////////////////////////////////////////////////////////////////////////
    /// @class MetaCode
    /// @headerfile metacode.h
    /// @brief This Determines the kind of user input.
    /// @details A Metacode contains the data that is passed around with an input
    /// event. It stores one type of button press or analog representation (Mouse
    /// move, joystick tilt, wheel spin, etc...). If it is an analog representation
    /// it will also store how far or how it is pushed, pressed, rotated, or
    /// whatever. Several of these can be used in combination to represent button
    /// combinations, or complex input combinations (like portions of fighter game
    /// moves).
    ///////////////////////////////////////
    class MEZZ_LIB MetaCode
    {
        private:
            //Optional value for handling input devices such as controllers.
            UInt16 DeviceIndex;
            //The values that makes a MetaCode useful.
            int MetaValue;
            Input::InputCode Code;

            //function that the constructors can call aid initial setup
            void Construct(const RawEvent& RawEvent_);
            void Construct(const int& MetaValue_, const Input::InputCode& Code_);

            //This assigns a value to Inputcode by doing an efficient low level copy of the correct portion of an SDL_event
            //If mishandled this function can corrupt the MetaCode, and it can throw any error memcpy could throw.
            static Input::InputCode GetInputCodeFromSDL_KEY(const RawEvent &RawEvent_);
            static Input::InputCode GetInputCodeFromSDL_MOUSE(const RawEvent &RawEvent_);
            static Input::InputCode GetInputCodeFromSDL_JOYSTICK(const RawEvent &RawEvent_);

        public:
            /// @brief Default constructor
            /// @details This sets nothing on the MetaCode and leaves it completely unassigned. Accessing a data member could cause problems
            MetaCode();

            /// @brief Descriptive Constructor
            /// @details This sets all values in the MetaCode, leaving it in completely ready state. This is the ideal constructor for simulating user input.
            /// @param MetaValue_ How much is something moving, tilting, rotating or whatever. For buttons a positive value is pushed, and a negative value is becoming unpressed, and 0 is unpressed.
            /// @param Code_ Which key or which type of input was pressed. Sqeaky, thinks this has partial unicode support.
            MetaCode(const int& MetaValue_, const Input::InputCode& Code_);

            /// @brief Descriptive Constructor
            /// @details This sets all values in the MetaCode, leaving it in completely ready state. This is the ideal constructor for simulating user input.
            /// @param MetaValue_ How much is something moving, tilting, rotating or whatever. For buttons a positive value is pushed, and a negative value is becoming unpressed, and 0 is unpressed.
            /// @param Code_ Which key or which type of input was pressed. Sqeaky, thinks this has partial unicode support.
            /// @param DeviceIndex_ The index of the device this metacode is describing.
            MetaCode(const int& MetaValue_, const Input::InputCode& Code_, const UInt16& DeviceIndex_);

            /// @brief The Heavy Lifting Constructor
            /// @details This contructor accepts a RawEvent from the input event subsystem internal to the engine. This converts all the required information
            /// from the lower level format and store what is needed in the event that is created. This is used heavily by engine internals. \n
            /// This constructor expects to receive a type of RawEvent that can be converted into exactly one kind of Metacode. Depending on the
            /// User input subsystem, this could be all RawEvents, or even just some RawEvents.
            /// @exception "RawEvent which creates Multiple Metacodes inserted into Metacode" - Thrown when passed a certain (system dependant) incorrect type of RawEvent.
            /// @exception "Unknown User Input Inserted into Metacode" - Thrown when receiving either a corrupt, improperly handle, or unsupported RawEvent.
            /// @warning We recomend against using this Constructor, because the binary format of RawEvent could change if the input event SubSystem Changes. In
            /// that event you would have to recompile your application to get it working with a new version of Mezzanine. Using this function in Game code removes any gaurantees of Game Code
            /// Portability.
            MetaCode(const RawEvent& RawEvent_);

            /// @brief This Returns the Inputcode
            /// @details This Value can be use to determine what keyboard button has been pressed, or what specific kind of Joystick or mouse event has occurred.
            /// This value can be set with @ref SetCode .
            /// @return This returns the input code for this MetaCode
            Input::InputCode GetCode() const;

            /// @brief This Sets The InputCode
            /// @details See @ref GetCode to see exactly what the Code is. This will Set the code stored in this MetaCode. This value can be retrieved with @ref GetCode .
            /// @param Code_ The value you want the stored code to become.
            void SetCode(const Input::InputCode& Code_);

            /// @brief This Sets The InputCode using an int.
            /// @details This will cast an int into an InputCode. Be careful, it is possible to put impossible or ridiculous values, in with
            /// this. For example Accidentally stuffing in the result of MOUSEBUTTON + 22 looks like it would give you MOUSEBUTTON_22. But that
            /// Doesn't exist, at the time of this writing you would get MOUSEABSOLUTEVERTICAL. Be careful, or skip this alltogether and use one of
            /// the provided functions that do the math for you like
            /// @param Code_ The value you want the stored code to become.
            void SetCode(int Code_);

            /// @brief This Returns the MetaValue
            /// @details The MetaValue can be use to determine how far something is tilted, pushed, rotated, or other analog value.
            /// This value can be set with @ref SetMetaValue .
            /// @return This returns the input code for this MetaCode. This could return any number inside a range (depending on hardware and configuration)
            /// to represent how tilted a joystick or how much a mouse moved.
            int GetMetaValue() const;                     //How much is being done? How far has the mouse moved, how much is the throttle pushed.

            /// @brief Sets the device index if applicable.
            /// @param Index The index of the device this metacode applies to.
            void SetDeviceIndex(const UInt16& Index);

            /// @brief Gets the currently set device index.
            /// @remarks If no device is set or applicable, this will return the max value for a UInt16 (-1).
            /// @return Returns a UInt16 that is the for the device this metacode applies to.
            UInt16 GetDeviceIndex() const;

            /// @brief Get the MetaValue as a Input::ButtonState.
            /// @return This returns the appropriate button state or throws an Mezzanine::Exception if an invalid button state is stored in the MetaValue
            /// @throw This throws a Mezzanine::Exception if the MetaValue is less than BUTTON_LIFTING or greater than BUTTON_DOWN.
            Input::ButtonState GetMetaValueAsButtonState() const;

            /// @brief Get the MetaValue as a Input::DirectionalMotionState.
            /// @return This returns the appropriate MouseWheel state or throws an Mezzanine::Exception if an invalid state is stored in the MetaValue
            /// @throw This throws a Mezzanine::Exception if the MetaValue is less than MOUSEWHEEL_DOWN or greater than MOUSEWHEEL_UP.
            Input::DirectionalMotionState GetMetaValueAsDirectionalMotionState() const;

            /// @brief This Sets The MetaValue
            /// @details See @ref GetMetaValue to see exactly what the MetaValue is. This will set the MetaValue stored in this MetaCode. This value can be retrieved with @ref GetMetaValue .
            /// @param MetaValue_ The value you want the stored MetaValue to become. No bounds checking will be done. You can supply a completely invalid value if you choose to.
            void SetMetaValue(const int& MetaValue_);      //For button presses a positive value is pushed and a zero means unpushed

            /// @brief Accepts a int and returns the InputCode for the Corresponding Mouse button
            /// @param ButtonerNumber The number of the button you want the code for
            /// @return When passed 0 this returns Input::MOUSEBUTTON, otherwise this returns Input::MOUSEBUTTON_X where X is the number that was passed in
            /// @throw This will throw a @ref InvalidParametersException if a value outside of the range betweeb @ref Input::MOUSEBUTTON_FIRST and @ref Input::MOUSEBUTTON_LAST is passed.
            static Input::InputCode GetMouseButtonCode(short unsigned int ButtonNumber);

            /// @brief Accepts a int and returns the InputCode for the Corresponding Joystick button
            /// @param ButtonerNumber The number of the button you want the code for
            /// @return When passed 0 this returns Input::JOYSTICKBUTTON, otherwise this returns Input::JOYSTICKBUTTON_X where X is the number that was passed in
            /// @throw This will throw a @ref InvalidParametersException if a value outside of the range betweeb @ref Input::CONTROLLERBUTTON_FIRST and @ref Input::CONTROLLERBUTTON_LAST is passed.
            static Input::InputCode GetJoystickButtonCode(short unsigned int ButtonNumber);

            /// @brief Accepts a int and returns the InputCode for the Corresponding Joystick Axis
            /// @param AxisNumber The number of the button you want the code for
            /// @return When passed 0 this returns Input::JOYSTICKAXIS, otherwise this returns Input::JOYSTICKAXIS_X where X is the number that was passed in
            /// @throw This will throw a @ref InvalidParametersException if a value outside of the range betweeb @ref Input::CONTROLLERAXIS_FIRST and @ref Input::CONTROLLERAXIS_LAST is passed.
            static Input::InputCode GetJoystickAxisCode(short unsigned int AxisNumber);

            /// @brief Does this MetaCode Represent a state of a keyboard key
            /// @details Returns true if this MetaCode pertains to a keyboard key being up, polled, down, pressed, or lifted.
            /// @return This returns a bool which will be true if this is keyboard event.
            bool IsKeyboardButton() const;

            /// @brief Is this a left or right Alt key
            /// @details Returns true if this MetaCode pertains to a either keyboard Alt key.
            /// @return This returns a bool which will be true if this is a left or right Alt key.
            bool IsAltKey() const;

            /// @brief Is this a left or right Ctrl key
            /// @details Returns true if this MetaCode pertains to a either keyboard Ctrl key.
            /// @return This returns a bool which will be true if this is a left or right Ctrl key.
            bool IsCtrlKey() const;

            /// @brief Is this a left or right Shift key
            /// @details Returns true if this MetaCode pertains to a either keyboard Shift key.
            /// @return This returns a bool which will be true if this is a left or right Shift key.
            bool IsShiftKey() const;

            /// @brief Is this a left or right Super key (Windows logo key, Apple logo key, etc...)
            /// @details Returns true if this MetaCode pertains to a either keyboard Super key.
            /// @return This returns a bool which will be true if this is a left or right Super key.
            bool IsSuperKey() const;

            /// @brief Does this MetaCode Represent a state of a Mouse button
            /// @details Returns true if this MetaCode pertains to a mouse button being up, polled, down, pressed, or lifted.
            /// @return This returns a bool which will be true if this is MOUSEBUTTON_X event.
            bool IsMouseButton() const;

            /// @brief Is this metacode a pollable event
            /// @return if this metacode stores a Keyboard button, Mouse Button or joystick button, then this is a pollable event
            bool IsPollable() const;

            /// @brief Does this MetaCode Represent a state of a Joystick Event
            /// @details Returns true if this MetaCode pertains to a joystick button being twisted, tilted, up, polled, down, pressed, or lifted, or whatever else you can do to a joystick.
            /// @return This returns a bool which will be true if this is between JOYSTICK_FIRST and JOYSTICK_LAST.
            bool IsJoyStickEvent() const;

            /// @brief Does this MetaCode Represent some other (non-keyboard and non-mouse button)
            /// @details Returns true if this MetaCode pertains to any being up, polled, down, pressed, or lifted.
            /// @return This returns a bool which will be true if this is between INPUTEVENT_FIRST and INPUTEVENT_LAST.
            bool IsOtherInputEvent() const;

            /// @brief Compares two MetaCodes for equality
            /// @details This returns true if the MetaValue and Code are the Same, this ignores ID.
            bool operator==(const MetaCode& other) const;
        ///////////////////////////////////////////////////////////////////////////////
        // Serialization

        // Serializable
        /// @brief Convert this class to an XML::Node ready for serialization
        /// @param CurrentRoot The point in the XML hierarchy that all this vector3 should be appended to.
        virtual void ProtoSerialize(XML::Node& CurrentRoot) const;

        // DeSerializable
        /// @brief Take the data stored in an XML and overwrite this instance of this object with it
        /// @param OneNode and XML::Node containing the data.
        /// @throw This throws an @ref InstanceIdentityInvalidException if the XML node passed defines another class. This throws @ref InvalidVersionException if the xml is of a newer version than 1.
        virtual void ProtoDeSerialize(const XML::Node& OneNode);

        /// @brief Get the name of the the XML tag this class will leave behind as its instances are serialized.
        /// @return A string containing "Vector3"
        String SerializableName() const;
    };//MetaCode
}//Mezzanine

/// @brief Allows for streaming of MetaCodes
/// @details If it can be streamed, then it can be logged Holds true for the MetaCode.
std::ostream& MEZZ_LIB operator << (std::ostream& stream, const Mezzanine::MetaCode& x);

/// @brief Used to de-serialize an Mezzanine::MetaCode from a stream
/// @details This reads in the xml and sets the target MetaCode according to values from the stream.
/// @param x The Mezzanine::MetaCode that will accept the values from the xml
/// @param stream The place to get the characters from, that define the Mezzanine::MetaCode.
/// @return Get an std::ostream that was read from, this allow chaining of the >> operators.
/// @throw Can throw any exception that any function in the Mezzanine::xml namespace could throw in addition to a Mezzanine::Exception if the serialization version doesn't match.
std::istream& MEZZ_LIB operator >> (std::istream& stream, Mezzanine::MetaCode& x);

/// @brief Converts an XML node into a Mezzanine::MetaCode
/// @details This will convert an XML::Node will a valid serialized Mezzanine::MetaCode into a Mezzanine::MetaCode
/// @param OneNode An XML Node containing the the text of a MetaCode
/// @param x the Mezzanine::MetaCode to store the deserialized MetaCode
/// @return This returns a reference to the XML::Node for operator chaining or whatever.
/// @throw Can throw any exception that any function in the Mezzanine::xml namespace could throw in addition to a Mezzanine::Exception if the serialization version doesn't match.
Mezzanine::XML::Node MEZZ_LIB  operator >> (Mezzanine::XML::Node OneNode, Mezzanine::MetaCode& x);


#endif
