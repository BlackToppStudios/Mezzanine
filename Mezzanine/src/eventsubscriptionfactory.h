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
#ifndef _eventsubscriptionfactory_h
#define _eventsubscriptionfactory_h

#include "eventsubscriberbinding.h"
#include "eventsubscriptiontabletraits.h"

namespace Mezzanine
{
    /// @addtogroup Events
    /// @{

    ///////////////////////////////////////////////////////////////////////////////
    /// @brief This is an empty factory that is the basis from which factories are specialized.
    ///////////////////////////////////////
    template<class TableType, class SubType, SubscriptionFactoryType FactoryType>
    class EventSubscriptionFactory
        {  };

    ///////////////////////////////////////////////////////////////////////////////
    /// @brief This serves as a common interface to create data necessary for event subscriptions.
    ///////////////////////////////////////
    template<class TableType, class SubType>
    class EventSubscriptionFactory<TableType,SubType,SubscriptionFactoryType::SFT_Subscriber>
    {
    public:
        /// @brief Retrievable type for querying the type of callable interface this table works with.
        using SubscriberType = SubType;

        /// @brief Convenience type and check for what exactly will be stored by this container.
        using StoredType = SubscriberType;
        /// @brief Convenience type for passing the subscriber as an argument to the Subscribe method.
        using SubscribeArg = typename std::conditional<std::is_pointer<SubscriberType>::value,SubscriberType,const SubscriberType&>::type;
        /// @brief Convenience type for what is passed back to the user for tracking the subscription.
        /// @remarks This is allowed to be different from StoredType, but StoredType must be implicitly convertible to this type.
        using SubscribeRet = SubscriberType;
        /// @brief Convenience type for passing the subscriber as an argument to the Subscribe method.
        using SubscriptionGet = typename std::conditional<std::is_pointer<SubscriberType>::value,SubscriberType,SubscriberType&>::type;
    public:
        /// @brief Creates a subscription from a subscriber.
        /// @param Sub The subscriber with which to create the subscription.
        /// @param Table A pointer to the table creating the subscription.
        /// @return Returns the Sub argument.
        static StoredType CreateSubscription(SubscribeArg Sub, TableType* Table)
            { return Sub; }
    };//EventSubscriptionFactory

    ///////////////////////////////////////////////////////////////////////////////
    /// @brief This serves as a common interface to create data necessary for event subscriptions.
    ///////////////////////////////////////
    template<class TableType, class SubType>
    class EventSubscriptionFactory<TableType,SubType,SubscriptionFactoryType::SFT_Binding>
    {
    public:
        /// @brief Retrievable type for querying the type of callable interface this table works with.
        using SubscriberType = SubType;

        /// @brief Convenience type for the base binding class that is returned.
        using BindingType = EventSubscriberBinding<SubscriberType>;
        /// @brief Convenience type for the base binding class that is returned wrapped in a shared ptr.
        using BindingPtrType = EventSubscriberBindingPtr<SubscriberType>;
        /// @brief Convenience type for the actual binding implementation used by this table.
        using BindingImplType = EventSubscriberBindingImpl<TableType,SubscriberType>;
        /// @brief Convenience type for the actual binding implementation used by this table wrapped in a shared ptr.
        using BindingImplPtrType = EventSubscriberBindingImplPtr<TableType,SubscriberType>;

        /// @brief Convenience type and check for what exactly will be stored by this container.
        using StoredType = EventSubscriberBindingStorable<BindingImplPtrType>;
        /// @brief Convenience type for passing the subscriber as an argument to the Subscribe method.
        using SubscribeArg = typename std::conditional<std::is_pointer<SubscriberType>::value,SubscriberType,const SubscriberType&>::type;
        /// @brief Convenience type for what is passed back to the user for tracking the subscription.
        /// @remarks This is allowed to be different from StoredType, but StoredType must be implicitly convertible to this type.
        using SubscribeRet = BindingPtrType;
        /// @brief Convenience type for passing the subscriber as an argument to the Subscribe method.
        using SubscriptionGet = BindingPtrType;
    public:
        /// @brief Creates a subscription from a subscriber.
        /// @param Sub The subscriber with which to create the subscription.
        /// @param Table A pointer to the table creating the subscription.
        /// @return Returns a subscription that will bind the table and subscriber.
        static StoredType CreateSubscription(SubscribeArg Sub, TableType* Table)
            { return BindingImplPtrType( new BindingImplType(Sub,Table) ); }
    };//EventSubscriptionFactory

    /// @}
}//Mezzanine

#endif
