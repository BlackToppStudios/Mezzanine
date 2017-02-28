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
#ifndef _eventsubscribertoken_h
#define _eventsubscribertoken_h

namespace Mezzanine
{
    class Event;
	///////////////////////////////////////////////////////////////////////////////
    /// @brief This class represents a given event that can be subscribed to and/or fired.
    ///////////////////////////////////////
    class MEZZ_LIB EventSubscriptionToken
    {
    protected:
        /// @brief A pointer to the subscribed Event.
        Event* SubbedEvent;
        /// @brief Whether or not the subscription to the event is valid.
        Boole Valid;
    public:
        /// @brief Class constructor.
        /// @param Subbed A pointer to the subscribed Event.
        /// @param ID The ID of this token to be used for unsubscribing.
        EventSubscriptionToken(Event* Subbed);
        /// @brief Move constructor.
        /// @param Other The other token to be moved.
        EventSubscriptionToken(EventSubscriptionToken&& Other) = default;
        /// @brief Class destructor.
        ~EventSubscriptionToken() = default;

        /// @brief Copy constructor.
        /// @param Other The other token to not be copied.
        EventSubscriptionToken(const EventSubscriptionToken& Other) = delete;

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        /// @brief Gets whether or not the subscription to the event is valid.
        /// @return Returns a Boole representing whether or not a subscriber is still subscribed to the publisher.
        Whole IsValid();
        /// @brief Unsubscribes the subscriber associated with this token from the event subscribed to.
        void Unsubscribe();

        ///////////////////////////////////////////////////////////////////////////////
        // Operators

        /// @brief Move assignment operator.
        /// @param Other The other token to be moved.
        /// @return Returns a reference to this.
        EventSubscriptionToken& operator=(EventSubscriptionToken&& Other) = default;

        /// @brief Copy constructor.
        /// @param Other The other token to not be copied.
        /// @return Returns a reference to this.
        EventSubscriptionToken& operator=(const EventSubscriptionToken& Other) = delete;
    };//EventSubscriptionToken
}//Mezzanine

#endif
