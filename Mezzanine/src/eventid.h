// � Copyright 2010 - 2017 BlackTopp Studios Inc.
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
#ifndef _eventid_h
#define _eventid_h

#include "hashedstring.h"

namespace Mezzanine
{
    /// @addtogroup Events
    /// @{

    ///////////////////////////////////////////////////////////////////////////////
    /// @brief This is a simple class used to handle the identification of an event table in the event system.
    ///////////////////////////////////////
    struct MEZZ_LIB EventID
    {
        ///////////////////////////////////////////////////////////////////////////////
        // Data Types

        /// @brief The underlying type for the ID to be used.
        using IDType = HashedString32::HashType;
        /// @brief Value used to describe an ID that is invalid.
        static const IDType InvalidID = HashedString32::EmptyHash;

        ///////////////////////////////////////////////////////////////////////////////
        // Data Members

        /// @brief The stored Integer ID for the subscriber.
        IDType ID;

        ///////////////////////////////////////////////////////////////////////////////
        // Construction and Destruction

        /// @brief Blank constructor.
        EventID() = delete;
        /// @brief Int constructor.
        /// @param Identifier The unique value for the ID to be used.
        EventID(const IDType Identifier) :
            ID(Identifier)
            {  }
        /// @brief HashedString constructor.
        /// @param ToConvert The hashed string to fetch the hash from.
        EventID(const HashedString32& ToConvert) :
            ID(ToConvert.GetHash())
            {  }
        /// @brief Copy constructor.
        /// @param Other The other EventID to be copied.
        EventID(const EventID& Other) = default;
        /// @brief Move constructor.
        /// @param Other The other EventID to be moved.
        EventID(EventID&& Other) = default;

        ///////////////////////////////////////////////////////////////////////////////
        // Operators

        /// @brief Assignment operator.
        /// @param Other The other EventID to be copied.
        /// @return Returns a reference to this.
        EventID& operator=(const EventID& Other) = default;
        /// @brief Move Assignment operator.
        /// @param Other The other EventID to be moved.
        /// @return Returns a reference to this.
        EventID& operator=(EventID&& Other) = default;

        /// @brief Less-than operator.
        /// @param Other The other EventID to compare to.
        /// @return Returns true if this ID is supposed to be sorted before the other ID, false otherwise.
        Boole operator<(const EventID& Other) const
            { return this->ID < Other.ID; }

        /// @brief Equality comparison operator.
        /// @param Other The other ID to compare with.
        /// @return Returns true if this EventID is the same as the other, false otherwise.
        Boole operator==(const EventID& Other) const
            { return ( this->ID == Other.ID ); }
        /// @brief Inequality comparison operator.
        /// @param Other The other ID to compare with.
        /// @return Returns true if this EventID is not the same as the other, false otherwise.
        Boole operator!=(const EventID& Other) const
            { return ( this->ID != Other.ID ); }
    };//EventID

    /// @brief Output streaming operator for EventID.
    /// @param Stream The std stream to write to.
    /// @param Var The ID to be streamed.
    /// @return Returns a reference to the stream passed in.
    inline std::ostream& operator<<(std::ostream& Stream, const Mezzanine::EventID& Var)
        { return Stream << Var.ID; }

    /// @}
}//Mezzanine

#endif // _eventid_h
