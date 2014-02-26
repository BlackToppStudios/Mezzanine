// © Copyright 2010 - 2014 BlackTopp Studios Inc.
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
#ifndef _eventsubscriberslot_cpp
#define _eventsubscriberslot_cpp

#include "eventsubscriberslot.h"
#include "eventsubscriber.h"

namespace Mezzanine
{
    ///////////////////////////////////////////////////////////////////////////////
    // EventSubscriberSlot Methods

    EventSubscriberSlot::EventSubscriberSlot(Event* Ev) :
        SubbedEvent(Ev)
        {  }

    EventSubscriberSlot::~EventSubscriberSlot()
        {  }

    ///////////////////////////////////////////////////////////////////////////////
    // Utility

    Event* EventSubscriberSlot::GetEvent() const
        { return this->SubbedEvent; }

    ///////////////////////////////////////////////////////////////////////////////
    // Internal Methods

    ///////////////////////////////////////////////////////////////////////////////
    // CustomEventSubscriberSlot Methods

    CustomSubscriberSlot::CustomSubscriberSlot(Event* Ev, EventSubscriber* Sub) :
        EventSubscriberSlot(Ev),
        Subscriber(Sub)
        {  }

    CustomSubscriberSlot::~CustomSubscriberSlot()
        {  }

    ///////////////////////////////////////////////////////////////////////////////
    // Utility

    EventSubscriber* CustomSubscriberSlot::GetSubscriber() const
        { return this->Subscriber; }

    EventSubscriberSlot::SlotType CustomSubscriberSlot::GetType() const
        { return EventSubscriberSlot::ST_Custom; }

    ///////////////////////////////////////////////////////////////////////////////
    // Internal Methods

    void CustomSubscriberSlot::_NotifyEvent(EventArgumentsPtr Args)
        { this->Subscriber->_NotifyEvent(Args); }

    ///////////////////////////////////////////////////////////////////////////////
    // FunctorSubscriberSlot Methods

    FunctorSubscriberSlot::FunctorSubscriberSlot(Event* Ev, EventSlotFunctorDefinition* Funct, Boole CleanUpAfter) :
        EventSubscriberSlot(Ev),
        Functor(Funct),
        CleanUp(CleanUpAfter)
        {  }

    FunctorSubscriberSlot::~FunctorSubscriberSlot()
        {  }

    ///////////////////////////////////////////////////////////////////////////////
    // Utility

    EventSlotFunctorDefinition* FunctorSubscriberSlot::GetFunctor() const
        { return this->Functor; }

    EventSubscriberSlot::SlotType FunctorSubscriberSlot::GetType() const
        { return EventSubscriberSlot::ST_Functor; }

    ///////////////////////////////////////////////////////////////////////////////
    // Internal Methods

    void FunctorSubscriberSlot::_NotifyEvent(EventArgumentsPtr Args)
        { this->Functor->operator()(Args); }

    ///////////////////////////////////////////////////////////////////////////////
    // CFunctionSubscriberSlot Methods

    CFunctionSubscriberSlot::CFunctionSubscriberSlot(Event* Ev, SubscriberFunction* Funct) :
        EventSubscriberSlot(Ev),
        Function(Funct)
        {  }

    CFunctionSubscriberSlot::~CFunctionSubscriberSlot()
        {  }

    ///////////////////////////////////////////////////////////////////////////////
    // Utility

    CFunctionSubscriberSlot::SubscriberFunction* CFunctionSubscriberSlot::GetFunction() const
        { return this->Function; }

    EventSubscriberSlot::SlotType CFunctionSubscriberSlot::GetType() const
        { return EventSubscriberSlot::ST_CFunction; }

    ///////////////////////////////////////////////////////////////////////////////
    // Internal Methods

    void CFunctionSubscriberSlot::_NotifyEvent(EventArgumentsPtr Args)
        { this->Function(Args); }

    ///////////////////////////////////////////////////////////////////////////////
    // ScriptSubscriberSlot Methods

    ScriptSubscriberSlot::ScriptSubscriberSlot(Event* Ev, Scripting::iScript* SubScript) :
        EventSubscriberSlot(Ev),
        SubscriberScript(SubScript)
        {  }

    ScriptSubscriberSlot::~ScriptSubscriberSlot()
        {  }

    ///////////////////////////////////////////////////////////////////////////////
    // Utility

    Scripting::iScript* ScriptSubscriberSlot::GetScript() const
        { return this->SubscriberScript; }

    EventSubscriberSlot::SlotType ScriptSubscriberSlot::GetType() const
        { return EventSubscriberSlot::ST_Script; }

    ///////////////////////////////////////////////////////////////////////////////
    // Internal Methods

    void ScriptSubscriberSlot::_NotifyEvent(EventArgumentsPtr Args)
    {
        /// @todo This needs to be implemented.
    }
}//Mezzanine

#endif
