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
#ifndef _eventsubscriberid_h
#define _eventsubscriberid_h

namespace Mezzanine
{
    /// @addtogroup Events
    /// @{

    ///////////////////////////////////////////////////////////////////////////////
    /// @brief This is a simple class used to handle the identification of a subscriber in the event system.
    /// @details This is mostly a wrapper over a uintptr_t, but has the added ability to accept a pointer
    /// to be converted into an int value.  Use of pointers are not necessary and the ID values can follow
    /// whatever convention you desire.  Pointer conversion simple exists as a convenience.
    ///////////////////////////////////////
    struct MEZZ_LIB EventSubscriberID
    {
        ///////////////////////////////////////////////////////////////////////////////
        // Data Types

        /// @brief The underlying type for the ID to be used.
        using IDType = uintptr_t;

        ///////////////////////////////////////////////////////////////////////////////
        // Data Members

        /// @brief The stored Integer ID for the subscriber.
        IDType ID;

        ///////////////////////////////////////////////////////////////////////////////
        // Construction and Destruction

        /// @brief Blank constructor.
        EventSubscriberID() = delete;
        /// @brief Int constructor.
        /// @param Identifier The unique value for the ID to be used.
        EventSubscriberID(const IDType Identifier) :
            ID(Identifier)
            {  }
        /// @brief Pointer constructor.
        /// @remarks This exists as a convenience for when pointers are usable as ID's.
        /// Initially void pointers were used but this caused ambiguity when using normal ints as IDs
        /// in some code.
        /// @tparam The type for the pointer to convert.
        /// @param PtrIdentifier The pointer to be converted into an ID.
        template<class PtrType>
        EventSubscriberID(PtrType* PtrIdentifier) :
            ID(reinterpret_cast<IDType>(PtrIdentifier))
            {  }
        /// @brief Copy constructor.
        /// @param Other The other EventSubscriberID to be copied.
        EventSubscriberID(const EventSubscriberID& Other) = default;
        /// @brief Move constructor.
        /// @param Other The other EventSubscriberID to be moved.
        EventSubscriberID(EventSubscriberID&& Other) = default;

        ///////////////////////////////////////////////////////////////////////////////
        // Operators

        /// @brief Assignment operator.
        /// @param Other The other EventSubscriberID to be copied.
        /// @return Returns a reference to this.
        EventSubscriberID& operator=(const EventSubscriberID& Other) = default;
        /// @brief Move Assignment operator.
        /// @param Other The other EventSubscriberID to be moved.
        /// @return Returns a reference to this.
        EventSubscriberID& operator=(EventSubscriberID&& Other) = default;

        /// @brief Equality comparison operator.
        /// @param Other The other subscriber ID to compare with.
        /// @return Returns true if this EventSubscriberID is the same as the other, false otherwise.
        Boole operator==(const EventSubscriberID& Other) const
            { return ( this->ID == Other.ID ); }
        /// @brief Inequality comparison operator.
        /// @param Other The other subscriber ID to compare with.
        /// @return Returns true if this EventSubscriberID is not the same as the other, false otherwise.
        Boole operator!=(const EventSubscriberID& Other) const
            { return ( this->ID != Other.ID ); }
    };//EventSubscriberID

    /// @}
}//Mezzanine

#endif
