//© Copyright 2010 Joseph Toppi and John Blackwood
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
#ifndef _physeventmanager_cpp
#define _physeventmanager_cpp
///////////////////////////////////////////////////////////////////////////////
//This will capture all event and store them for easy dispatching.
//
//There will be an instance of this class in the physworld.
///////////////////////////////////////

#include "physeventmanager.h"
#include "SDL.h"

int PhysSDLFilter( const RawEvent *event );

bool PhysEventManager::IgnoreSDLQuitEvents;

/// @todo TODO: Make the PhysEventManager completely thread safe. IF this is completely thread safe, we can spawn numerous individual thread each accessing this and
/// and the performance gain would almost scale directly with cpu core count increases. Look at boost scoped_lock

PhysEventManager::PhysEventManager()
{
    SetIgnoreQuitEvents(false);
    SDL_SetEventFilter( PhysSDLFilter );
}

//These functions will give you the next event or help you manage the events
unsigned int PhysEventManager::GetRemainingEventCount()
{
    return EventQueue.size();
}

PhysEvent* PhysEventManager::GetNextEvent()
{
    if(EventQueue.size()==0)
    {
            return 0;
    }
    PhysEvent* results = EventQueue.front();
    EventQueue.pop_front();
    return results;
}

PhysEventRenderTime* PhysEventManager::GetNextRenderTimeEvent()
{
    PhysEventRenderTime* results = 0;
    for(list<PhysEvent*>::iterator Iter = EventQueue.begin(); Iter!=EventQueue.end(); Iter++)
    {
        if((*Iter)->getEventType()==RenderTime)
        {
            results = dynamic_cast<PhysEventRenderTime*> (*Iter);
            EventQueue.erase(Iter);
            return results;
        }
    }
    return results;
}

PhysEventUserInput* PhysEventManager::GetNextUserInputEvent()
{
    PhysEventUserInput* results = 0;
    for(list<PhysEvent*>::iterator Iter = EventQueue.begin(); Iter!=EventQueue.end(); Iter++)
    {
        if((*Iter)->getEventType()==UserInput)
        {
            results = dynamic_cast<PhysEventUserInput*> (*Iter);
            EventQueue.erase(Iter);
            return results;
        }
    }
    return results;
}

void PhysEventManager::AddEvent(PhysEvent* EventToAdd)
{
    EventQueue.push_back(EventToAdd);
}

bool PhysEventManager::DoQuitMessagesExist()
{
        return false;//This is system dependent.
}

bool PhysEventManager::IgnoreQuitEvents()
{
    return IgnoreSDLQuitEvents;
}

void PhysEventManager::SetIgnoreQuitEvents(bool Ignore)
{
    IgnoreSDLQuitEvents=Ignore;
}

int PhysSDLFilter( const RawEvent *event )
{
     if ( !PhysEventManager::IgnoreQuitEvents() && event->type == SDL_QUIT)
        { return 0; }
     return 1;
}

#endif
