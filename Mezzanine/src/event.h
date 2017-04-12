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
#ifndef _event_h
#define _event_h

#include <memory>

#include "hashedstring.h"

namespace Mezzanine
{
    /// @addtogroup Events
    /// @{

    /// @brief The type to use for the names of events.
    using EventNameType = HashedString32;
    /// @brief The type to use for the hashed generated from the names of events.
    using EventHashType = EventNameType::HashType;

    ///////////////////////////////////////////////////////////////////////////////
    /// @brief This is a common class to represent all possible arguments for a given event that is fired.
    ///////////////////////////////////////
	class MEZZ_LIB Event
	{
    public:
        ///////////////////////////////////////////////////////////////////////////////
        // Public Data Members

        /// @brief The name of the event being fired.
        const EventNameType EventName;

        ///////////////////////////////////////////////////////////////////////////////
        // Construction and Destruction

        /// @brief Class constructor.
        /// @param Name The name of the event being fired.
        Event(const EventNameType& Name) :
            EventName(Name)
            {  }
        /// @brief Class destructor.
        virtual ~Event() = default;
	};//Event

	/// @brief Convenience type for passing around Events.
	using EventPtr = std::shared_ptr<Event>;

    /// @}
}//Mezzanine

#endif
