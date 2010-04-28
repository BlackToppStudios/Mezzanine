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

#include "physworld.h"
#include "physeventmanager.h"

#include "SDL.h"
#include <boost/thread/thread.hpp>

using namespace phys;

///////////////////////////////////////////////////////////////////////////////
// Internal C style functions


int PhysSDLFilter( const RawEvent *event );

//If this is passed an event that points to 0 it will function as a method to tell us if an SDL_QUIT message has been thrown
//this will return 2 if it has not seen an SDL_quit, and a 4 if it has
int PhysSDLFilter( const SDL_Event *event )
{
    static bool DroppedQuit=false;

    if(event!=0)                //if this is a real event
    {
        if ( event->type == SDL_QUIT )
        {
            DroppedQuit=true;   //Drop all quit events, and track that we dropped them
            return 0;
        }
        return 1;
    }else{
        if(DroppedQuit)         //4 if we need to make a quit event
        {
            DroppedQuit=false;  //Reset this so we don't add more in the future by accident
            return 4;
        }
        return 2;
    }
}


/// @todo TODO: Make the PhysEventManager completely thread safe. IF this is completely thread safe, we can spawn numerous individual thread each accessing this and
/// and the performance gain would almost scale directly with cpu core count increases. Look at boost scoped_lock


PhysEventManager::PhysEventManager(PhysWorld* ParentWorld_)
{
    ParentWorld = ParentWorld_;
    PollMouseHor = false;
    PollMouseVert = false;
}

///////////////////////////////////////////////////////////////////////////////
//These functions will give you the next event or help you manage the events
///////////////////////////////////////
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
    return results;
}

PhysEvent* PhysEventManager::PopNextEvent()
{
    if(EventQueue.size()==0)
    {
            return 0;
    }
    PhysEvent* results = EventQueue.front();
    EventQueue.pop_front();
    return results;
}

void PhysEventManager::RemoveNextEvent()
{
    EventQueue.pop_front();
}

void PhysEventManager::AddEvent(PhysEvent* EventToAdd)
{
    EventQueue.push_back(EventToAdd);
}

void PhysEventManager::UpdateEvents()
{

//        ParentWorld->LogAndThrow("Event Filter Not Correctly Setting");

}

void PhysEventManager::UpdateSystemEvents()
{

    this->PreProcessSDLEvents();
    ParentWorld->Log("WM EventCount Pending:");
    ParentWorld->Log(SDL_WmEvents.size());
    //TODO: make Physevents for each of the events in SDL_WmEvents(and delete the SDL events)
}

void PhysEventManager::UpdateUserInputEvents()
{
    this->PreProcessSDLEvents();
    ParentWorld->Log("User Input EventCount Pending:");
    ParentWorld->Log(SDL_UserInputEvents.size());

    PhysEventUserInput* FromSDLEvent = new PhysEventUserInput();
    PhysEventUserInput* FromSDLPolling = this->PollForUserInputEvents();

    while( !SDL_UserInputEvents.empty() )
    {
        RawEvent* CurrentRawEvent = SDL_UserInputEvents.front();

        FromSDLEvent->AddCodesFromRawEvent( *CurrentRawEvent );
        delete CurrentRawEvent;
        SDL_UserInputEvents.pop(); //NEXT!!!
    }

    *FromSDLEvent += *FromSDLPolling;
    delete FromSDLPolling;

    if (0 < FromSDLEvent->GetMetaCodeCount())
    {
        this->AddEvent(FromSDLEvent); //Now FromSDL is some else's responsibility
    }else{
        delete FromSDLEvent;
    }
}

///////////////////////////////////////////////////////////////////////////////
// Filtered management functions - You choose YAYYYY!!!
///////////////////////////////////////
PhysEvent* PhysEventManager::GetNextSpecificEvent(PhysEvent::EventType SpecificType)
{
    PhysEvent* results = 0;
    for(list<PhysEvent*>::iterator Iter = EventQueue.begin(); Iter!=EventQueue.end(); Iter++)
    {
        if((*Iter)->getEventType()==SpecificType)
        {
            results = (*Iter);
            return results;
        }
    }
    return results;
}

PhysEvent* PhysEventManager::PopNextSpecificEvent(PhysEvent::EventType SpecificType)
{
    PhysEvent* results = 0;
    for(list<PhysEvent*>::iterator Iter = EventQueue.begin(); Iter!=EventQueue.end(); Iter++)
    {
        if((*Iter)->getEventType()==SpecificType)
        {
            results = (*Iter);
            EventQueue.erase(Iter);
            return results;
        }
    }
    return results;
}

void PhysEventManager::RemoveNextSpecificEvent(PhysEvent::EventType SpecificType)
{
    for(list<PhysEvent*>::iterator Iter = EventQueue.begin(); Iter!=EventQueue.end(); Iter++)
    {
        if((*Iter)->getEventType()==SpecificType)
        {
            EventQueue.erase(Iter);
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
// Filtered management functions - RenderTime Events
///////////////////////////////////////
PhysEventRenderTime* PhysEventManager::GetNextRenderTimeEvent()
{
    return dynamic_cast<PhysEventRenderTime*> (this->GetNextSpecificEvent(PhysEvent::RenderTime));
}

PhysEventRenderTime* PhysEventManager::PopNextRenderTimeEvent()
{
    return dynamic_cast<PhysEventRenderTime*> (this->PopNextSpecificEvent(PhysEvent::RenderTime));
}

void PhysEventManager::RemoveNextRenderTimeEvent()
{
    this->RemoveNextSpecificEvent(PhysEvent::RenderTime);
}

///////////////////////////////////////////////////////////////////////////////
// Filtered management functions - User Input Events
///////////////////////////////////////

PhysEventUserInput* PhysEventManager::GetNextUserInputEvent()
{
    return dynamic_cast<PhysEventUserInput*> (this->GetNextSpecificEvent(PhysEvent::UserInput));
}

PhysEventUserInput* PhysEventManager::PopNextUserInputEvent()
{
    return dynamic_cast<PhysEventUserInput*> (this->PopNextSpecificEvent(PhysEvent::UserInput));
}

void PhysEventManager::RemoveNextUserInputEvent()
{
    this->RemoveNextSpecificEvent(PhysEvent::UserInput);
}

///////////////////////////////////////////////////////////////////////////////
// Filtered management functions - Quit Event
///////////////////////////////////////

EventQuit* PhysEventManager::GetNextQuitEvent()
{
    return dynamic_cast<EventQuit*> (this->GetNextSpecificEvent(PhysEvent::QuitMessage));
}

EventQuit* PhysEventManager::PopNextQuitEvent()
{
    return dynamic_cast<EventQuit*> (this->PopNextSpecificEvent(PhysEvent::QuitMessage));
}

void PhysEventManager::RemoveNextQuitEvent()
{
    this->RemoveNextSpecificEvent(PhysEvent::QuitMessage);
}

///////////////////////////////////////////////////////////////////////////////
// Polling Functions
///////////////////////////////////////

void PhysEventManager::AddPollingCheck(const MetaCode &InputToTryPolling)
{
    bool ItFailed = true;

    //Check for keyboard code
    if ( MetaCode::KEY_LAST > InputToTryPolling.GetCode() && InputToTryPolling.GetCode() > MetaCode::KEY_FIRST)
    {
        this->WatchKeyboardKeys.push_back(InputToTryPolling.GetCode());
        ItFailed=false;
    }

    //if it is a specific mouse button, then
    if ( MetaCode::MOUSEBUTTON == InputToTryPolling.GetCode())
    {
        this->WatchMouseKeys.push_back(InputToTryPolling.GetID());
        ItFailed=false;
    }

    //Mouse Movement
    if ( MetaCode::MOUSEABSOLUTEVERTICAL == InputToTryPolling.GetCode())
    {
        PollMouseVert = true;
        ItFailed=false;
    }
    if ( MetaCode::MOUSEABSOLUTEHORIZONTAL == InputToTryPolling.GetCode())
    {
        PollMouseHor = true;
        ItFailed=false;
    }

    if (ItFailed)
        this->ParentWorld->LogAndThrow("Unsupported Polling Check on this Platform");
}

void RemovePollingCheck(const MetaCode &InputToStopPolling)
{

}


PhysEventUserInput* PhysEventManager::PollForUserInputEvents()
{
    vector<MetaCode> MetaBag;

    //Call the private Polling routines
    PollKeyboard(MetaBag);
    PollMouseButtons(MetaBag);
    PollMouseLocation(MetaBag);

    PhysEventUserInput* test = new PhysEventUserInput(MetaBag);
    return test;
}

//Internal private Polling routine
void PhysEventManager::PollKeyboard(vector<MetaCode> &CodeBag)
{
    if(this->WatchKeyboardKeys.size()>0)
    {
        int* KeyCount = 0;
        Uint8* KeyboardSnapshot = SDL_GetKeyState(KeyCount);

        vector<MetaCode::InputCode>::iterator iter;
        for(iter = this->WatchKeyboardKeys.begin(); iter != (this->WatchKeyboardKeys.end()) ; iter++)
        {
            if( *iter < MetaCode::KEY_LAST )//Is it a valid keycode
            {
                if(KeyboardSnapshot[*iter]) // is it up or down
                {//down
                    MetaCode temp(MetaCode::BUTTON_DOWN,0,*iter);
                    CodeBag.push_back(temp);
                }else{//up
                    MetaCode temp(MetaCode::BUTTON_UP,0,*iter);
                    CodeBag.push_back(temp);
                }
            }
        }
    }
}

//Internal private Polling routine
void PhysEventManager::PollMouseButtons(vector<MetaCode> &CodeBag)
{
    if(this->WatchMouseKeys.size()>0)
    {
        vector<int>::iterator iter;
        for(iter = this->WatchMouseKeys.begin(); iter != (this->WatchMouseKeys.end()) ; iter++)
        {
            if(SDL_GetMouseState(NULL, NULL)&SDL_BUTTON(*iter))
            {
                    MetaCode temp(MetaCode::BUTTON_DOWN,*iter,MetaCode::MOUSEBUTTON);
                    CodeBag.push_back(temp);
            }else{
                    MetaCode temp(MetaCode::BUTTON_UP,*iter,MetaCode::MOUSEBUTTON);
                    CodeBag.push_back(temp);
            }
        }
    }

}

void PhysEventManager::PollMouseLocation(vector<MetaCode> &CodeBag)
{
    if( PollMouseHor || PollMouseVert )
    {
        int Vert=0;
        int Hor=0;
        SDL_GetMouseState(&Hor, &Vert);
        if( PollMouseVert )
        {
            MetaCode temp(Vert,0,MetaCode::MOUSEABSOLUTEVERTICAL);
            CodeBag.push_back(temp);
        }
        if( PollMouseHor )
        {
            MetaCode temp(Hor,0,MetaCode::MOUSEABSOLUTEHORIZONTAL);
            CodeBag.push_back(temp);
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
// Standard interupt basd user input

//This function will get all the events from SDL and Sort them into one of two Queues
void PhysEventManager::PreProcessSDLEvents()
{
    UpdateQuitEvents(); //quit skips the preprocessing Queue and goes straight into the the main Queue

    RawEvent temp;
    RawEvent* FromSDL=&temp;
	while(SDL_PollEvent(FromSDL))
	{
	    RawEvent* ScopeHolder = new RawEvent;
	    *ScopeHolder = temp;
        switch(FromSDL->type)
        {
            case SDL_ACTIVEEVENT:   //when the window gains focus
            case SDL_VIDEORESIZE:   //when the screen is resized
            case SDL_VIDEOEXPOSE:   //when the windows goes from being hidden to being shown
            case SDL_QUIT:          //when SDL closes
            case SDL_SYSWMEVENT:
                SDL_WmEvents.push(ScopeHolder);
                break;
            case SDL_KEYDOWN:
            case SDL_KEYUP:
            case SDL_MOUSEMOTION:
            case SDL_MOUSEBUTTONDOWN:
            case SDL_MOUSEBUTTONUP:
            case SDL_JOYAXISMOTION:
            case SDL_JOYBUTTONDOWN:
            case SDL_JOYBUTTONUP:
            case SDL_JOYBALLMOTION:
            case SDL_JOYHATMOTION:
                SDL_UserInputEvents.push(ScopeHolder);
                break;
            case SDL_USEREVENT://Never thrown by SDL, but could be added by a user
            default:
                throw ("Unknown SDL Event Inserted");
                break;
        }
	}
}

void PhysEventManager::UpdateQuitEvents()
{
    if (NULL == SDL_GetEventFilter())           //Verify the Event filter is installed, if not, then install it.
    {
        SDL_SetEventFilter( PhysSDLFilter );
    }else{
        if(4==PhysSDLFilter(0))                 //Pass it a null pointer to get it to "Not Callback Mode"
        {
            this->AddEvent(new EventQuit());    //We need to make a quit event
        }else{
            //all clear
        }
    }
}

#endif
