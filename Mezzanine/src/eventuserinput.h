//© Copyright 2010 - 2011 BlackTopp Studios Inc.
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
#ifndef _EVENTUSERINPUT_H
#define _EVENTUSERINPUT_H
///////////////////////////////////////////////////////////////////////////////
// This will expose all keyboard and mouse, joystick and other userinput events
// to developers, we are using the SDL keymap to get us started, large portions
// are directly copy/pasta'd, so we included their license too
///////////////////////////////////////

//From SDL
/*
    SDL - Simple DirectMedia Layer
    Copyright (C) 1997-2009 Sam Lantinga

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

#include "metacode.h"
#include "eventbase.h"
#include "xml.h"

namespace Mezzanine
{

    ///////////////////////////////////////////////////////////////////////////////
    /// @class EventUserInput
    /// @headerfile eventuserinput.h
    /// @brief This is a container for MetaCodes that is used in the EventManager
    /// @details The EventUserInput is the container for information about how a
    /// user enters data and commands into a program. By Default one is inserted into
    /// event manager the with all the user input from each run of the main loop.
    /// These can be manually inserted into the EventManager to simulate input from
    /// other sources. If setup properly this can allow computer controlled
    /// characters to use the same interface players, allowing for more realistic
    /// response from them. \n \n
    /// This is implemented by inheriting from std::vector. Any feature of std::vector
    /// can be used to interact with the metacodes stored within. Using Iterators is
    /// great way to quickly and easily get what you need from this. For the most part
    /// any extra functions on this are for seamless interaction with the EventManager,
    /// or to convert incoming data into a more usable form.
    ///////////////////////////////////////////////////////////////////////////////
    class MEZZ_LIB EventUserInput : public EventBase, public std::vector<MetaCode>
    {
        private:
            //These private methods all accept a specific kind of RawEvent from SDL and will behave non-deterministically if
            //passed any other kind of data.

            /// @internal
            /// @brief Gets the MetaCode from RawInput Data
            /// @param RawEvent_ The event that contains only Mouse Motion data
            /// @return a vector of metacode that represents button presses
            std::vector<MetaCode> AddCodesFromSDLMouseMotion(const RawEvent &RawEvent_);

            /// @internal
            /// @brief Gets the MetaCode from RawInput Data
            /// @param RawEvent_ The event that contains only Joystick Motion data
            /// @return a vector of metacode that represents button presses
            std::vector<MetaCode> AddCodesFromSDLJoyStickMotion(const RawEvent &RawEvent_);

            /// @internal
            /// @brief Gets the MetaCode from RawInput Data
            /// @param RawEvent_ The event that contains only Joystick Hat data
            /// @return a vector of metacode that represents hat presses
            std::vector<MetaCode> AddCodeFromSDLJoyStickHat(const RawEvent &RawEvent_);

            /// @internal
            /// @brief Gets the MetaCode from RawInput Data
            /// @param RawEvent_ The event that contains only Joystick Ball data
            /// @return a vector of metacode that represents ball motions
            std::vector<MetaCode> AddCodeFromSDLJoyStickBall(const RawEvent &RawEvent_);

            /// @internal
            /// @brief Gets the MetaCode from RawInput Data
            /// @param RawEvent_ The event that contains only Mouse button data
            /// @return a metacode that represents a button press
            MetaCode AddCodeFromSDLMouseButton(const RawEvent &RawEvent_);

            /// @internal
            /// @brief Gets the MetaCode from RawInput Data
            /// @param RawEvent_ The event that contains only Joystick button data
            /// @return a metacode that represents a button press
            MetaCode AddCodeFromSDLJoyStickButton(const RawEvent &RawEvent_);

        public:

            /// @brief Default constructor
            /// @details This creates a perfectly functional, but empty EventUserInput.
            EventUserInput();

            /// @brief Single Data Point constructor
            /// @param Code_ This MetaCode will be added to the EventUserInput during creation.
            /// @details This creates a functional EventUserInput which already contains one metacode.
            EventUserInput(const MetaCode &Code_);

            /// @brief Multi Data Point constructor
            /// @param Codes_ The MetaCodes in this vecotor will be added to the EventUserInput during creation.
            /// @details This creates a ready to use EventUserInput which already contains all the metacodes included.
            EventUserInput(const std::vector<MetaCode> &Codes_);

            /// @brief Default desstructor
            /// @details This tears down the
            virtual ~EventUserInput();

            //code managment functions
            /// @brief Single Data Point constructor
            /// @return Index The requested MetaCode to return.
            /// @details This function simply retrieves the requested MetaCode. It can throw standard Out of bounds exceptions if attemped to reference a negative item or an item with Index higher than what exists
            /// \n This is useful for accessing each MetaCode stored in this UserInputEvent.
            const MetaCode& GetMetaCode(const unsigned int &Index);

            /// @brief Retrieves a count of the stored Metacodes
            /// @return The amount of codes stored in this EventUserInput.
            /// @details Retrieves a count of the stored Metacodes. Synonym for vector::size();
            size_t GetMetaCodeCount();

            /// @internal
            /// @brief Adds a MetaCode created from a RawEvent
            /// @param RawEvent_ The RawEvent which will be translated into exactly One MetaCode
            /// @details This will add MetaCode to this event which will be create from a RawEvent which can produce Exactly one MetaCode. This is used by engine internals, it is
            /// recommended to not use this in game code.
            /// @warning Do not use this without reading and fully understanding the warnings on MetaCode::MetaCode(const RawEvent &RawEvent_) . This function has all the same
            /// Restrictions. If game code is using RawEvents at all, the game logic should be scrutinized carefully, there is probably something wrong, but if it must it should use
            /// EventUserInput::AddCodesFromRawEvent instead, as it can make the needed determinations automatically and in a platform agnostic way.
            /// @return This returns a const reference to the MetaCode that was Added. This reference is valid for the lifetime of this EventUserInput.
            MetaCode AddCode(const RawEvent &RawEvent_);

            /// @brief Adds a MetaCode
            /// @param Code_ The User Input MetaCode tobe added
            /// @details This adds an existing metacode to this event.
            /// @return This returns a const reference to the MetaCode that was Added. This reference is valid for the lifetime of this EventUserInput.
            MetaCode AddCode(const MetaCode &Code_);

            /// @brief Adds a MetaCode Created From Raw Values
            /// @param MetaValue_ The MetaValue that will be in the MetaCode
            /// @param Code_ The InputCode that will be in the MetaCode
            /// @details This creates metacode a metacode and adds it to this event.
            /// @return This returns a const reference to the MetaCode that was Added. This reference is valid for the lifetime of this EventUserInput.
            MetaCode AddCode(const int &MetaValue_, const MetaCode::InputCode &Code_);

            /// @brief Add Several MetaCodes from a vector
            /// @param Codes A vector of MetaCodes to be added to this event
            /// @details This adds several existing metacodes to this event.
            void AddCodes(const std::vector<MetaCode> &Codes);

            /// @brief Adds all possible MetaCodes that can be created from the given RawEvent
            /// @param RawEvent_ The RawEvent which will be translated into a group of metacodes and added to this
            /// @details This will add MetaCode to this event which will be create from a RawEvent which can produce Exactly one MetaCode. This is used by engine internals, it is
            /// recommended to not use this in game code.
            /// @warning If game code is using RawEvents at all, the game logic should be scrutinized carefully, there is probably something wrong, but if it must them this is the correct
            /// function to use. This will work same on a all platforms. However, the binary format of the Rawevent could chnage meaning you would have to recompile the game code to work
            /// with new version of the engine
            /// \n This Function is currently incomplete, and does not yet process all events such as joysticks events and some mouse events.
            /// @return this returns a complete set of all the MetaCodes added.
            std::vector<MetaCode> AddCodesFromRawEvent(const RawEvent &RawEvent_);

            /// @brief Removes a specific code from storage.
            /// @param Code_ This will search for all matching copies of this
            /// @details All MetaCodes that are equal to Code_ will simply be erased.
            void EraseCode(const MetaCode &Code_);

            /// @brief Removes a specific code from storage.
            /// @param Index This is the location to removed from
            /// @details The MetaCode at and only at the given Index will be deleted.
            void EraseCode(const unsigned int &Index);

            /// @brief Returns the type of this event
            /// @return Returns EventType::UserInput
            virtual EventType GetType() const;
    };
}// /Mezz

///////////////////////////////////////////////////////////////////////////////
// Class External << Operators for streaming or assignment
#ifdef MEZZXML

/// @brief Serializes the passed Mezzanine::EventUserInput to XML
/// @param stream The ostream to send the xml to.
/// @param Ev the Mezzanine::EventUserInput to be serialized
/// @return this returns the ostream, now with the serialized data
std::ostream& MEZZ_LIB operator << (std::ostream& stream, const Mezzanine::EventUserInput& Ev);

/// @brief Deserialize a Mezzanine::EventUserInput
/// @param stream The istream to get the xml from to (re)make the Mezzanine::EventUserInput.
/// @param Ev the Mezzanine::EventUserInput to be deserialized.
/// @return this returns the ostream, advanced past the Mezzanine::EventUserInput that was recreated onto Ev.
std::istream& MEZZ_LIB operator >> (std::istream& stream, Mezzanine::EventUserInput& Ev);

/// @brief Set all values of a Mezzanine::EventUserInput from parsed xml.
/// @param OneNode The istream to get the xml from to (re)make the Mezzanine::EventUserInput.
/// @param Ev the Mezzanine::EventUserInput to be reset.
/// @return This returns thexml::Node that was passed in.
void MEZZ_LIB operator >> (const Mezzanine::xml::Node& OneNode, Mezzanine::EventUserInput& Ev);

#endif // \MEZZXML

#endif
