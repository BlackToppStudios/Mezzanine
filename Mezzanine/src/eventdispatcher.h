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
#ifndef _eventdispatcher_h
#define _eventdispatcher_h

#include "event.h"

namespace Mezzanine
{
    ///////////////////////////////////////////////////////////////////////////////
    /// @brief This is a basic event dispatcher that does simple mindless forwarding.
    ///////////////////////////////////////
    template<class TableType>
    class MEZZ_LIB EmptyEventDispatcher
    {
    public:
        /// @brief Class constructor.
        EmptyEventDispatcher() = default;
        /// @brief Class destructor.
        ~EmptyEventDispatcher() = default;

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        /// @brief Fires an event.
        /// @param Args The arguments/event specific data related to this event.
        void DispatchEvent(const TableType& DispatchTable, EventPtr Args) const
            { DispatchTable.DispatchEvent(Args); }
    };//EmptyEventDispatcher

    ///////////////////////////////////////////////////////////////////////////////
    /// @brief This is an event dispatcher capable of blocking the dispatch of events.
    ///////////////////////////////////////
    template<class TableType>
    class MEZZ_LIB DefaultEventDispatcher
    {
    protected:
        /// @brief Controls whether or not the firing of events should be suppressed.
        Boole MuteEvents;
    public:
        /// @brief Class constructor.
        DefaultEventDispatcher() :
            MuteEvents(false)
            {  }
        /// @brief Class destructor.
        ~DefaultEventDispatcher() = default;

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        /// @brief Sets whether or not event firings by this publisher will be suppressed.
        /// @param Mute True to prevent events from firing, false for normal operation.
        void SetMuteEvents(const Boole Mute)
            { this->MuteEvents = Mute; }
        /// @brief Gets whether or not event firings by this publisher will be suppressed.
        /// @return Returns true if events are being suppressed, false if this publisher is operating normally.
        Boole GetMuteEvents() const
            { return this->MuteEvents; }

        /// @brief Fires an event.
        /// @param Args The arguments/event specific data related to this event.
        void DispatchEvent(const TableType& DispatchTable, EventPtr Args) const
        {
            if( !this->MuteEvents ) {
                DispatchTable.DispatchEvent(Args);
            }
        }
    };//DefaultEventDispatcher
}//Mezzanine

#endif
