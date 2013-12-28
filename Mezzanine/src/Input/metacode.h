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
#include "Input/inputenumerations.h"
#ifndef SWIG
    #include "XML/xml.h"
#endif

namespace Mezzanine
{
    namespace Input
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
        protected:
            //The values that makes a MetaCode useful.
            Int32 MetaValue;
            //Optional value for handling input devices such as controllers.
            UInt16 DeviceIndex;
            Input::InputCode Code;

            //function that the constructors can call aid initial setup
            void Construct(const RawEvent& RawEvent_);
            void Construct(const Int32& Value, const Input::InputCode& NewCode);

            //This assigns a value to Inputcode by doing an efficient low level copy of the correct portion of an SDL_event
            //If mishandled this function can corrupt the MetaCode, and it can throw any error memcpy could throw.
            static Input::InputCode GetInputCodeFromSDL_KEY(const RawEvent &RawEvent_);
            static Input::InputCode GetInputCodeFromSDL_MOUSE(const RawEvent &RawEvent_);
            static Input::InputCode GetInputCodeFromSDL_JOYSTICK(const RawEvent &RawEvent_);

        public:
            /// @brief Default constructor.
            /// @details This sets nothing on the MetaCode and leaves it completely unassigned. Accessing a data member could cause problems.
            MetaCode();
            /// @brief Copy constructor.
            /// @details This sets all values in the MetaCode, leaving it in completely ready state.
            /// @param Other The other MetaCode to copy from.
            MetaCode(const MetaCode& Other);
            /// @brief Descriptive Constructor.
            /// @details This sets all values in the MetaCode, leaving it in completely ready state. This is the ideal constructor for simulating user input.
            /// @param Value How much is something moving, tilting, rotating or whatever. For buttons a positive value is pushed, and a negative value is becoming unpressed, and 0 is unpressed.
            /// @param NewCode Which key or which type of input was pressed. Sqeaky, thinks this has partial unicode support.
            MetaCode(const Int32& Value, const Input::InputCode& NewCode);
            /// @brief Descriptive Constructor.
            /// @details This sets all values in the MetaCode, leaving it in completely ready state. This is the ideal constructor for simulating user input.
            /// @param Value How much is something moving, tilting, rotating or whatever. For buttons a positive value is pushed, and a negative value is becoming unpressed, and 0 is unpressed.
            /// @param NewCode Which key or which type of input was pressed. Sqeaky, thinks this has partial unicode support.
            /// @param Index The index of the device this metacode is describing.
            MetaCode(const Int32& Value, const Input::InputCode& NewCode, const UInt16& Index);
            /// @brief The Heavy Lifting Constructor.
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

            ///////////////////////////////////////////////////////////////////////////////
            // Gets and Sets

            /// @brief This Sets The InputCode.
            /// @details See @ref GetCode to see exactly what the Code is. This will Set the code stored in this MetaCode. This value can be retrieved with @ref GetCode .
            /// @param NewCode The value you want the stored code to become.
            void SetCode(const Input::InputCode& NewCode);
            /// @brief This Sets The InputCode using an Int32.
            /// @warning This will cast an Int32 into an InputCode. Be careful, it is possible to put impossible or ridiculous values, in with
            /// this. For example Accidentally stuffing in the result of MOUSEBUTTON + 22 looks like it would give you MOUSEBUTTON_22. But that
            /// Doesn't exist, at the time of this writing you would get MOUSEABSOLUTEVERTICAL. Be careful, or skip this alltogether and use one of
            /// the provided functions that do the math for you like.
            /// @param NewCode The value you want the stored code to become.
            void SetCode(const Int32& NewCode);
            /// @brief This Returns the Inputcode.
            /// @details This Value can be use to determine what keyboard button has been pressed, or what specific kind of Joystick or mouse event has occurred.
            /// This value can be set with @ref SetCode .
            /// @return This returns the input code for this MetaCode.
            Input::InputCode GetCode() const;
            /// @brief This Sets The MetaValue.
            /// @details See @ref GetMetaValue to see exactly what the MetaValue is. This will set the MetaValue stored in this MetaCode. This value can be retrieved with @ref GetMetaValue .
            /// @param Value The value you want the stored MetaValue to become. No bounds checking will be done. You can supply a completely invalid value if you choose to.
            void SetMetaValue(const Int32& Value);
            /// @brief This Returns the MetaValue.
            /// @details The MetaValue can be use to determine how far something is tilted, pushed, rotated, or other analog value.
            /// This value can be set with @ref SetMetaValue .
            /// @return This returns the input code for this MetaCode. This could return any number inside a range (depending on hardware and configuration)
            /// to represent how tilted a joystick or how much a mouse moved.
            Int32 GetMetaValue() const;
            /// @brief Sets the device index if applicable.
            /// @param Index The index of the device this metacode applies to.
            void SetDeviceIndex(const UInt16& Index);
            /// @brief Gets the currently set device index.
            /// @remarks If no device is set or applicable, this will return the max value for a UInt16 (-1).
            /// @return Returns a UInt16 that is the for the device this metacode applies to.
            UInt16 GetDeviceIndex() const;
            /// @brief Sets all the values of this MetaCode to Null values.
            void SetNullValues();

            ///////////////////////////////////////////////////////////////////////////////
            // Conversion and Casting Functions

            /// @brief Get the MetaValue as a Input::ButtonState.
            /// @return This returns the appropriate button state or throws an Mezzanine::Exception if an invalid button state is stored in the MetaValue.
            /// @throw This throws a Mezzanine::Exception if the MetaValue is less than BUTTON_LIFTING or greater than BUTTON_DOWN.
            Input::ButtonState GetMetaValueAsButtonState() const;
            /// @brief Get the MetaValue as a Input::DirectionalMotionState.
            /// @return This returns the appropriate MouseWheel state or throws an Mezzanine::Exception if an invalid state is stored in the MetaValue
            /// @throw This throws a Mezzanine::Exception if the MetaValue is less than MOUSEWHEEL_DOWN or greater than MOUSEWHEEL_UP.
            Input::DirectionalMotionState GetMetaValueAsDirectionalMotionState() const;

            /// @brief Accepts a int and returns the InputCode for the Corresponding Mouse button.
            /// @param ButtonerNumber The number of the button you want the code for.
            /// @return When passed 0 this returns Input::MOUSEBUTTON, otherwise this returns Input::MOUSEBUTTON_X where X is the number that was passed in.
            static Input::InputCode GetMouseButtonCode(const UInt16 ButtonNumber);
            /// @brief Accepts a int and returns the InputCode for the Corresponding Controller button.
            /// @param ButtonerNumber The number of the button you want the code for.
            /// @return When passed 0 this returns Input::JOYSTICKBUTTON, otherwise this returns Input::CONTROLLERBUTTON_X where X is the number that was passed in.
            static Input::InputCode GetControllerButtonCode(const UInt16 ButtonNumber);
            /// @brief Accepts a int and returns the InputCode for the Corresponding Controller Axis.
            /// @param AxisNumber The number of the button you want the code for.
            /// @return When passed 0 this returns Input::JOYSTICKAXIS, otherwise this returns Input::CONTROLLERAXIS_X where X is the number that was passed in.
            static Input::InputCode GetControllerAxisCode(const UInt16 AxisNumber);

            ///////////////////////////////////////////////////////////////////////////////
            // Utility Checks

            /// @brief Does this MetaCode Represent a state of a keyboard key.
            /// @return This returns a bool which will be true if this is a keyboard event.
            bool IsKeyboardButton() const;
            /// @brief Does this MetaCode Represent a state of a Mouse button.
            /// @return This returns a bool which will be true if this is a MOUSEBUTTON_X event.
            bool IsMouseButton() const;
            /// @brief Does this MetaCode Represent a state of a Controller button.
            /// @return This returns a bool which will be true if this is a CONTROLLERBUTTON_X event.
            bool IsControllerButton() const;
            /// @brief Does this MetaCode Represent a state of any button on an input device.
            /// @return This returns a bool which will be true if this is event pertains to any button on any input device.
            bool IsDeviceButton() const;

            /// @brief Does this MetaCode Represent a state of a keyboard Event.
            /// @return This returns a bool which will be true if this is between KEY_FIRST and KEY_LAST.
            bool IsKeyboardEvent() const;
            /// @brief Does this MetaCode Represent a state of a mouse Event.
            /// @return This returns a bool which will be true if this is between MOUSE_FIRST and MOUSE_LAST.
            bool IsMouseEvent() const;
            /// @brief Does this MetaCode Represent movement of the mouse or mouse wheel.
            /// @return This returns a bool which will be true if this is between MOUSEMOTION_FIRST and MOUSEMOTION_LAST.
            bool IsMouseMotionEvent() const;
            /// @brief Does this MetaCode Represent multiple clicks of a mouse button.
            /// @return This returns a bool which will be true if this is between the first mouse multiclick input code and the last mouse multiclick input code.
            bool IsMouseMultiClickEvent() const;
            /// @brief Does this MetaCode Represent a state of a multitouch device.
            /// @return This returns a bool which will be true if this is between the MULTITOUCH_FIRST and MULTITOUCH_LAST.
            bool IsMultitouchEvent() const;
            /// @brief Does this MetaCode Represent a state of a Controller Event
            /// @return This returns a bool which will be true if this is between CONTROLLER_FIRST and CONTROLLER_LAST.
            bool IsControllerEvent() const;
            /// @brief Does this MetaCode Represent an axis position on a controller.
            /// @return This returns a bool which will be true if this is between CONTROLLERAXIS_FIRST and CONTROLLERAXIS_LAST.
            bool IsControllerAxisEvent() const;
            /// @brief Does this MetaCode Represent a hat position on a controller.
            /// @return This returns a bool which will be true if this is between CONTROLLERHAT_FIRST and CONTROLLERHAT_LAST.
            bool IsControllerHatEvent() const;
            /// @brief Does this MetaCode Represent some other (non-keyboard and non-mouse button).
            /// @return This returns a bool which will be true if this is between INPUTEVENT_FIRST and INPUTEVENT_LAST.
            bool IsInputEvent() const;

            /// @brief Is this a left or right Alt key.
            /// @return This returns a bool which will be true if this is a left or right Alt key.
            bool IsAltKey() const;
            /// @brief Is this a left or right Ctrl key.
            /// @return This returns a bool which will be true if this is a left or right Ctrl key.
            bool IsCtrlKey() const;
            /// @brief Is this a left or right Shift key.
            /// @return This returns a bool which will be true if this is a left or right Shift key.
            bool IsShiftKey() const;
            /// @brief Is this a left or right Super key (Windows logo key, Apple logo key, etc...).
            /// @return This returns a bool which will be true if this is a left or right Super key.
            bool IsSuperKey() const;

            /// @brief Is this metacode a pollable event.
            /// @return if this metacode stores a device button then this is a pollable event.
            bool IsPollable() const;

            /// @brief Gets the device-type this MetaCode is representing.
            /// @return Returns an Input::InputDevice value for the device this MetaCode is representing.
            Input::InputDevice GetDeviceType() const;

            ///////////////////////////////////////////////////////////////////////////////
            // Operators

            /// @brief Assignment operator.
            /// @param Other The other MetaCode to assign to this.
            /// @return Returns a reference to this.
            MetaCode& operator=(const MetaCode& Other);
            /// @brief Compares two MetaCode's for equality.
            /// @param Other The other MetaCode to compare with.
            /// @return Returns true if the two MetaCode's are equal, false otherwise.
            bool operator==(const MetaCode& Other) const;
            /// @brief Compares two MetaCode's for inequality.
            /// @param Other The other MetaCode to compare with.
            /// @return Returns true if the two MetaCode's are not equal, false otherwise.
            bool operator!=(const MetaCode& Other) const;
            /// @brief Compares two MetaCode's to see if this is less.
            /// @note This only compares the InputCode contained in both MetaCode's.
            /// @param Other The other MetaCode to compare with.
            /// @return Returns true if this MetaCode is less then the MetaCode being compared, false otherwise.
            bool operator<(const MetaCode& Other) const;
            /// @brief Compares two MetaCode's to see if this is greater.
            /// @note This only compares the InputCode contained in both MetaCode's.
            /// @param Other The other MetaCode to compare with.
            /// @return Returns true if this MetaCode is greater then the MetaCode being compared, false otherwise.
            bool operator>(const MetaCode& Other) const;

            ///////////////////////////////////////////////////////////////////////////////
            // Serialization

            // Serializable
            /// @brief Convert this class to an XML::Node ready for serialization.
            /// @param CurrentRoot The point in the XML hierarchy that all this vector3 should be appended to.
            void ProtoSerialize(XML::Node& CurrentRoot) const;
            // DeSerializable
            /// @brief Take the data stored in an XML and overwrite this instance of this object with it.
            /// @param OneNode and XML::Node containing the data.
            void ProtoDeSerialize(const XML::Node& OneNode);
            /// @brief Get the name of the the XML tag this class will leave behind as its instances are serialized.
            /// @return A string containing "Vector3".
            String SerializableName() const;
        };//MetaCode
    }//Input
    // Commonly used data types
    typedef std::vector< Input::MetaCode >     MetaCodeContainer;
    typedef MetaCodeContainer::iterator        MetaCodeIterator;
    typedef MetaCodeContainer::const_iterator  ConstMetaCodeIterator;
}//Mezzanine

/// @brief Allows for streaming of MetaCodes
/// @details If it can be streamed, then it can be logged Holds true for the MetaCode.
std::ostream& MEZZ_LIB operator << (std::ostream& stream, const Mezzanine::Input::MetaCode& x);

/// @brief Used to de-serialize an Mezzanine::MetaCode from a stream
/// @details This reads in the xml and sets the target MetaCode according to values from the stream.
/// @param x The Mezzanine::MetaCode that will accept the values from the xml
/// @param stream The place to get the characters from, that define the Mezzanine::MetaCode.
/// @return Get an std::ostream that was read from, this allow chaining of the >> operators.
/// @throw Can throw any exception that any function in the Mezzanine::xml namespace could throw in addition to a Mezzanine::Exception if the serialization version doesn't match.
std::istream& MEZZ_LIB operator >> (std::istream& stream, Mezzanine::Input::MetaCode& x);

/// @brief Converts an XML node into a Mezzanine::MetaCode
/// @details This will convert an XML::Node will a valid serialized Mezzanine::MetaCode into a Mezzanine::MetaCode
/// @param OneNode An XML Node containing the the text of a MetaCode
/// @param x the Mezzanine::MetaCode to store the deserialized MetaCode
/// @return This returns a reference to the XML::Node for operator chaining or whatever.
/// @throw Can throw any exception that any function in the Mezzanine::xml namespace could throw in addition to a Mezzanine::Exception if the serialization version doesn't match.
Mezzanine::XML::Node& MEZZ_LIB  operator >> (Mezzanine::XML::Node& OneNode, Mezzanine::Input::MetaCode& x);

#endif
