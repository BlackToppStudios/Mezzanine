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
#ifndef _eventsubscriptiontable_h
#define _eventsubscriptiontable_h

#include "hashedstring.h"
#include "eventhelper.h"
#include "eventid.h"
#include "eventdispatcher.h"

namespace Mezzanine
{
    /// @addtogroup Events
    /// @{

    ///////////////////////////////////////////////////////////////////////////////
    /// @brief This class represents a given event that can be subscribed to and/or fired.
    /// @tparam Config A struct of types and values to use for the configuration of this table.
    /// @pre Config is expected to be a valid EventSubscriptionTableConfig suitable to configure
    /// the types on EventSubscriptionTableTraits.  See @ref EventSubscriptionTableTraits for more info. @n
    /// Additionally, Config is expected to have "ContainerType", "DispatcherType", and "DispatchIDType".
    ///     "ContainerType" is expected to be an enum value that has a corresponding valid template specialization of EventSubscriptionContainer.
    ///     "DispatcherType" is expected to be an enum value that has a corresponding valid template specialization of EventSubscriptionDispatcher.
    ///     "DispatchIDType" is expected to be less-than comparable (<) and suitable to uniquely identify EventSubscriptionTable instances.
    ///////////////////////////////////////
    template<class Config>
    class EventSubscriptionTable :
        public EventSubscriptionContainer<EventSubscriptionTable<Config>,EventSubscriptionTableTraits<EventSubscriptionTable<Config>,Config>,Config::ContainerType>,
        public EventDispatcher<EventSubscriptionTable<Config>,EventSubscriptionTableTraits<EventSubscriptionTable<Config>,Config>,Config::DispatcherType>
    {
    public:
        /// @brief Convenience type for describing the type of this.
        using SelfType = EventSubscriptionTable<Config>;
        /// @brief The type for traits used by this table.
        using TraitsType = EventSubscriptionTableTraits<SelfType,Config>;
        /// @brief The type for the container inherited by this table.
        using ContainerType = EventSubscriptionContainer<SelfType,TraitsType,Config::ContainerType>;
        /// @brief Retrievable type for the traits given to this table.
        using TableConfig = Config;
        /// @brief The type to use for uniquely identifying this table among a group of like tables.
        using DispatchIDType = typename Config::DispatchIDType;
    protected:
        /// @brief The hash of the Event the subscribers in this table are subscribed to.
        DispatchIDType DisID;
    public:
        /// @brief Blank constructor.
        EventSubscriptionTable() = delete;
        /// @brief Copy constructor.
        /// @param Other The other table to NOT be copied.
        EventSubscriptionTable(const SelfType& Other) = delete;
        /// @brief Move constructor.
        /// @param Other The other table to be moved.
        EventSubscriptionTable(SelfType&& Other) :
            ContainerType( std::move(Other) ),
            DisID( std::move(Other.DisID) )
        {
            for( typename ContainerType::StorageIterator StorIt = this->begin() ; StorIt != this->end() ; ++StorIt )
                { ContainerType::FactoryType::MoveSubscription( *StorIt, this ); }
        }
        /// @brief Identifier constructor.
        /// @param ID The unique ID of the event this table will store subscriptions for.
        EventSubscriptionTable(const DispatchIDType ID) :
            DisID(ID)
            {  }
        /// @brief Class destructor.
        ~EventSubscriptionTable()
        {
            for( typename ContainerType::StorageIterator StorIt = this->begin() ; StorIt != this->end() ; ++StorIt )
                { ContainerType::FactoryType::InvalidateSubscription( *StorIt ); }
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Operators

        /// @brief Assignment operator.
        /// @param Other The other table to NOT be copied.
        /// @return Returns a reference to this.
        SelfType& operator=(const SelfType& Other) = delete;
        /// @brief Move assignment operator.
        /// @param Other The other table to be moved.
        /// @return Returns a reference to this.
        SelfType& operator=(SelfType&& Other)
        {
            this->ContainerType::operator=( std::move(Other) );
            this->DisID = std::move(Other.DisID);
            for( typename ContainerType::StorageIterator StorIt = this->begin() ; StorIt != this->end() ; ++StorIt )
                { ContainerType::FactoryType::MoveSubscription( *StorIt, this ); }
            return *this;
        }

        /// @brief Less-than operator.
        /// @param Other The other table to be compared to.
        /// @return Returns true if this table is considered less than the other, or should be sorted before the other table.
        Boole operator<(const SelfType& Other) const
            { return this->DisID < Other.DisID; }

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        /// @brief Gets the ID of the event associated with this table.
        /// @return Returns the unique ID of this event.
        DispatchIDType GetID() const
            { return this->DisID; }
    };//EventSubscriptionTable

    /// @}
}//Mezzanine

#endif
