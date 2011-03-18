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
#ifndef EVENTBASE_H
#define EVENTBASE_H

#include "crossplatformexport.h"
#include <ostream>

///////////////////////////////////////////////////////////////////////////////
// This is a parent class that all events must derive from. This exists solely
//to normalize the output of the event manager class.
///////////////////////////////////////

namespace phys
{

    ///////////////////////////////////////////////////////////////////////////////
    /// @class EventBase
    /// @headerfile eventbase.h
    /// @brief The base class for all events.
    /// @details All Events used in the Event Manager, will inherit from this. While
    /// not absolutely required by the game programmer to write their own events, it
    /// it could be useful. Instances of this class cannot be made, and all classes
    /// that inherit from this are expected to implement getEventType().
    class PHYS_LIB EventBase
    {
        public:
            /// @enum EventType
            /// @brief A listing of values that can be used to identify Events.
            /// @details A listing of values that can be used to identify Events.
            enum EventType
            {
                Collision,      /**< Indicates the Event is a Physics Collision Event */
                GameWindow,     /**< Indicates the Event is a Game Window Management Event */
                QuitMessage,    /**< Indicates the Event is a phys::EventQuit */
                RenderTime,     /**< Indicates the Event is a PhysEventRenderTime */
                UserInput,      /**< Indicates the Event is a EventUserInput */
                //SystemMessage,  /**< Indicates the Event has not been coded yet */
                Other
            };

            /// @brief This will aid in identifying all classes that inherit from this class.
            /// @details All Classes derived form this calls will return an Event::EventType that correspond the
            /// the data/class type they actually are.
            /// @return This returns an eventype that will correspend with the actual event type. This can be used on all
            /// Phys game provided class to safely cast a pointer to the correct event type.
            virtual EventBase::EventType GetType() const = 0;

    };
}

/// @brief Serializes the passed phys::Event to XML
/// @param stream The ostream to send the xml to.
/// @param Ev the phys::Event to be serialized
/// @return this returns the ostream, now with the serialized data
std::ostream& PHYS_LIB operator << (std::ostream& stream, const phys::EventBase& Ev);


#endif
