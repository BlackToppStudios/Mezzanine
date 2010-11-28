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
#ifndef PHYSEVENTUSERINPUT_H
#define PHYSEVENTUSERINPUT_H
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


#include <vector>

using namespace std;

#include "eventbase.h"
#include "datatypes.h"
#include "metacode.h"

namespace phys
{

    ///////////////////////////////////////////////////////////////////////////////
    /// @class EventUserInput
    /// @headerfile eventuserinput.h
    /// @brief This is a container for MetaCodes that is used in the physEventManager
    /// @details The EventUserInput is the container for information about how a
    /// user enters data and commands into a program. By Default one is inserted into
    /// event manager the with all the user input from the last run of the main loop.
    /// These can be manually inserted into the EventManager to simulate input from
    /// other sources. If setup properly this can allow computer controlled
    /// characters to use the same interface players, allowing for more realistic
    /// response from them. This is not limited to the tricks discussed here.
    ///////////////////////////////////////////////////////////////////////////////
    class EventUserInput : public EventBase
    {
        private:
            //Thse both accept a specific king of RawEvent from SDL and will behave non-deterministically if
            //passed any other kind of data.
            void AddCodesFromSDLMouseButton(const RawEvent &RawEvent_);
            void AddCodesFromSDLMouseMotion(const RawEvent &RawEvent_);

        protected:
            /// @brief This stores the MetaCodes in this event.
            /// @details This stores the MetaCodes in this event. This represents a snapshot of the user input for the moment.
            vector<MetaCode> Code;

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
            EventUserInput(const vector<MetaCode> &Codes_);


            /// @brief Default desstructor
            /// @details This tears down the
            virtual ~EventUserInput();

            //code managment functions
            /// @brief Single Data Point constructor
            /// @return Index The requested MetaCode to return.
            /// @details This function simply retrieves the requested MetaCode. It can throw standard Out of bounds exceptions if attemped to reference a negative item or an item with Index higher than what exists
            /// \n This is useful for accessing each MetaCode stored in this physUserInputEvent.
            const MetaCode& GetMetaCode(const unsigned int &Index);

            /// @brief Retrieves a count of the stored Metacodes
            /// @return The amount of codes stored in this physEventUserInput.
            /// @details Retrieves a count of the stored Metacodes
            unsigned int GetMetaCodeCount();

            /// @brief Adds a MetaCode
            /// @param Code_ The User Input MetaCode tobe added
            /// @details This adds an existing metacode to this event.
            void AddCode(const MetaCode &Code_);

            /// @brief Adds a Created From Raw Values
            /// @param MetaValue_ The MetaValue that will be in the MetaCode
            /// @param ID_ The ID that will be in the MetaCode
            /// @param Code_ The InputCode that will be in the MetaCode
            /// @details This creates metacode a metacode and adds it to this event.
            void AddCode(const int &MetaValue_, const short unsigned int &ID_, const MetaCode::InputCode &Code_);

            /// @brief Adds a MetaCode created from a RawEvent
            /// @param RawEvent_ The RawEvent which will be translated into exactly One MetaCode
            /// @details This will add MetaCode to this event which will be create from a RawEvent which can produce Exactly one MetaCode. This is used by engine internals, it is
            /// recommended to not use this in game code.
            /// @warning Do not use this without reading and fully understanding the warnings on MetaCode::MetaCode(const RawEvent &RawEvent_) . This function has all the same
            /// Restrictions. If game code is using RawEvents at all, the game logic should be scrutinized carefully, it is probably wrong, but if it must it should use
            /// EventUserInput::AddCodesFromRawEvent instead, as it can make the needed determinations automatically and in a platform agnostic way.
            void AddCode(const RawEvent &RawEvent_);

            /// @brief Add Several MetaCodes from a vector
            /// @param Codes A vector of MetaCodes to be added to this event
            /// @details This adds several existing metacodes to this event.
            void AddCode(const vector<MetaCode> &Codes);

            /// @brief Adds all possible MetaCodes that can be created from the given RawEvent
            /// @param RawEvent_ The RawEvent which will be translated into a group of metacodes and added to this
            /// @details This will add MetaCode to this event which will be create from a RawEvent which can produce Exactly one MetaCode. This is used by engine internals, it is
            /// recommended to not use this in game code.
            /// @warning If game code is using RawEvents at all, the game logic should be scrutinized carefully, it is probably wrong, but if it must them this is the correct
            /// function to use. This will work same on a all platforms. However, the binary format of the Rawevent could chnage meaning you would have to recompile the game code to work
            /// with new version of the engine
            /// \n This Function is currently incomplete, and does not yet process all events such as joysticks events and some mouse events.
            void AddCodesFromRawEvent(const RawEvent &RawEvent_);

            /// @brief Removes a specific code from storage.
            /// @param Code_ This will search for all matching copies of this
            /// @details All MetaCodes that are equal to Code_ will simply be erased.
            void EraseCode(const MetaCode &Code_);

            /// @brief Removes a specific code from storage.
            /// @param Index This is the location to removed from
            /// @details The MetaCode at and only at the given Index will be deleted.
            void EraseCode(const unsigned int &Index);

            /// @brief Removes a specific code or Adds it if not present.
            /// @param Code_ This will search for all matching copies of this.
            /// @details All MetaCodes that are equal to Code_ will simply be erased.
            void ToggleCode(const MetaCode &Code_);

            /// @brief This add all of one EventUserInput to another
            /// @param Add is the EventUserInput on the right hand side of +=
            /// @details This simply copies all the MetaCodes from one EventUserInput to the other.
            /// @return The EventUserInput on the left ahnd side will now contain a set of both items MetaCodes
            EventUserInput& operator += (const EventUserInput& Add);

            /// @brief Returns the type of this event
            /// @return Returns EventType::UserInput
            virtual EventType GetType() const;
    };

}// /phys


#endif
