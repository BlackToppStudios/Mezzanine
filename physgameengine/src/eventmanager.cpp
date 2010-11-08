//© Copyright 2010 BlackTopp Studios Inc.
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
//There will be an instance of this class in the world.
///////////////////////////////////////

#include "world.h"
#include "eventmanager.h"
#include "metacode.h"

#include "SDL.h"
//#include <boost/thread/thread.hpp> //will use this when this becomes multithreaded

namespace phys
{
    /// @internal
    /// @namespace phys::internal
    /// @brief This namespace is used for internal helper classes, and in general it should be ignored by game developers
    /// @details This whole internal namespace is a dirty hack. This is where code goes that must implement classes or functions for the various subsytems the Physgame engine draws on.
    namespace internal
    {
        /// @internal
        /// @brief SDL uses this to filter events it presents to applications
        /// @details This is used used to filter out SQL_quit messages, and generate appropriate messages for the game developer to use.
        /// This will always drop quit events, and store that information for later use.
        /// @param event This is the event SDL expects use to filters, To get real data from this we setup it up so that if the event is a null pointer the function will return data about quit messages
        /// @warning Do not use this. It can only cause problems. This is for SDL, the user input subsystem, to filter certain events.
        /// @return This will always return either 0 or 1 to SDL. 0 if it should drop the event, which it does to all SDL_quit events, 1 if the event should be allowed, which it does to all events which are not SDL_quit events. If a null pointer was passed, then this will return 4 if it dropped an SDL_Quit, and 2 if it has not droppped an SDL_quit.
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
    } // /internal

    /// @todo TODO: Make the EventManager completely thread safe. IF this is completely thread safe, we can spawn numerous individual thread each accessing this and
    /// and the performance gain would almost scale directly with cpu core count increases. Look at boost scoped_lock
    EventManager::EventManager()
    {
        this->GameWorld = World::GetWorldPointer();
        PollMouseHor = false;
        PollMouseVert = false;
        this->Priority=-40;
    }

    ///////////////////////////////////////////////////////////////////////////////
    //These functions will give you the next event or help you manage the events
    ///////////////////////////////////////
    unsigned int EventManager::GetRemainingEventCount()
    {
        return EventQueue.size();
    }

    EventBase* EventManager::GetNextEvent()
    {
        if(EventQueue.size()==0)
        {
                return 0;
        }
        EventBase* results = EventQueue.front();
        return results;
    }

    EventBase* EventManager::PopNextEvent()
    {
        if(EventQueue.size()==0)
        {
                return 0;
        }
        EventBase* results = EventQueue.front();
        EventQueue.pop_front();
        return results;
    }

    void EventManager::RemoveNextEvent()
    {
        EventQueue.pop_front();
    }

    void EventManager::AddEvent(EventBase* EventToAdd)
    {
        EventQueue.push_back(EventToAdd);
    }

    const std::list<EventBase*>* EventManager::GetAllEvents() const
    {
        return &(this->EventQueue);
    }



    void EventManager::UpdateEvents()
    {
        /// @todo There has got to be a more efficient way to do UpdateEvents()
        this->UpdateSystemEvents();
        this->UpdateUserInputEvents();
    }

    void EventManager::UpdateSystemEvents()
    {
        this->PreProcessSDLEvents();
        this->GameWorld->Log("WM EventCount Pending:");
        this->GameWorld->Log(SDL_WmEvents.size());
        /// @todo make Physevents for each of the events in SDL_WmEvents(and delete the SDL events)
    }

    void EventManager::UpdateUserInputEvents()
    {
        this->PreProcessSDLEvents();
        this->GameWorld->Log("User Input EventCount Pending:");
        this->GameWorld->Log(SDL_UserInputEvents.size());

        EventUserInput* FromSDLEvent = new EventUserInput();
        EventUserInput* FromSDLPolling = this->PollForUserInputEvents();

        //read through the pending user input events and add those codes
        while( !SDL_UserInputEvents.empty() )
        {
            RawEvent* CurrentRawEvent = SDL_UserInputEvents.front();

            FromSDLEvent->AddCodesFromRawEvent( *CurrentRawEvent );

            delete CurrentRawEvent;
            SDL_UserInputEvents.pop(); //NEXT!!!
        }

        *FromSDLEvent += *FromSDLPolling;
        delete FromSDLPolling;

        //Did atleast one userinput get generates, or atleast one polling request
        if (0 < FromSDLEvent->GetMetaCodeCount())
        {
            //check for mouse input before we send along it all to the event holding ground
            for (Whole c=0;c<FromSDLEvent->GetMetaCodeCount();++c)
            {
                if(FromSDLEvent->GetMetaCode(c).GetCode() == MetaCode::MOUSEABSOLUTEHORIZONTAL)
                    { this->CurrentMouseCoords.X = FromSDLEvent->GetMetaCode(c).GetMetaValue(); }
                if(FromSDLEvent->GetMetaCode(c).GetCode() == MetaCode::MOUSEABSOLUTEVERTICAL)
                    { this->CurrentMouseCoords.Y = FromSDLEvent->GetMetaCode(c).GetMetaValue(); }
            }
            this->AddEvent(FromSDLEvent); //Now FromSDL is some else's responsibility
        }else{
            delete FromSDLEvent;
        }
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Filtered management functions - You choose YAYYYY!!!
    ///////////////////////////////////////
    EventBase* EventManager::GetNextSpecificEvent(EventBase::EventType SpecificType)
    {
        EventBase* results = 0;
        for(std::list<EventBase*>::iterator Iter = EventQueue.begin(); Iter!=EventQueue.end(); Iter++)
        {
            if((*Iter)->GetType()==SpecificType)
            {
                results = (*Iter);
                return results;
            }
        }
        return results;
    }

    EventBase* EventManager::PopNextSpecificEvent(EventBase::EventType SpecificType)
    {
        EventBase* results = 0;
        for(std::list<EventBase*>::iterator Iter = EventQueue.begin(); Iter!=EventQueue.end(); Iter++)
        {
            if((*Iter)->GetType()==SpecificType)
            {
                results = (*Iter);
                EventQueue.erase(Iter);
                return results;
            }
        }
        return results;
    }

    void EventManager::RemoveNextSpecificEvent(EventBase::EventType SpecificType)
    {
        for(std::list<EventBase*>::iterator Iter = EventQueue.begin(); Iter!=EventQueue.end(); Iter++)
        {
            if((*Iter)->GetType()==SpecificType)
            {
                EventQueue.erase(Iter);
            }
        }
    }

    std::list<EventBase*>* EventManager::GetAllSpecificEvents(EventBase::EventType SpecificType)
    {
        std::list<EventBase*>* TempList = new std::list<EventBase*>;

        for(std::list<EventBase*>::iterator Iter = EventQueue.begin(); Iter!=EventQueue.end(); Iter++)
        {
            if((*Iter)->GetType()==SpecificType)
            {
                TempList->push_back(*Iter);
            }
        }
        return TempList;
    }

    void EventManager::RemoveAllSpecificEvents(EventBase::EventType SpecificType)
    {
        for(std::list<EventBase*>::iterator Iter = EventQueue.begin(); Iter!=EventQueue.end(); Iter++)
        {
            if((*Iter)->GetType()==SpecificType)
            {
                this->EventQueue.remove(*Iter);
            }
        }
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Filtered management functions - RenderTime Events
    ///////////////////////////////////////
    EventRenderTime* EventManager::GetNextRenderTimeEvent()
    {
        return dynamic_cast<EventRenderTime*> (this->GetNextSpecificEvent(EventBase::RenderTime));
    }

    EventRenderTime* EventManager::PopNextRenderTimeEvent()
    {
        return dynamic_cast<EventRenderTime*> (this->PopNextSpecificEvent(EventBase::RenderTime));
    }

    void EventManager::RemoveNextRenderTimeEvent()
    {
        this->RemoveNextSpecificEvent(EventBase::RenderTime);
    }

    std::list<EventRenderTime*>* EventManager::GetAllRenderTimeEvents()
    {
        std::list<EventBase*>* TempList = this->GetAllSpecificEvents(EventBase::UserInput);
        std::list<EventRenderTime*>* Results= new std::list<EventRenderTime*>;

        for(std::list<EventBase*>::iterator Iter = TempList->begin(); Iter!=TempList->end(); Iter++)
        {
            Results->push_back( dynamic_cast<EventRenderTime*> (*Iter) );
        }

        return Results;
    }


    ///////////////////////////////////////////////////////////////////////////////
    // Filtered management functions - User Input Events
    ///////////////////////////////////////

    EventUserInput* EventManager::GetNextUserInputEvent()
    {
        return dynamic_cast<EventUserInput*> (this->GetNextSpecificEvent(EventBase::UserInput));
    }

    EventUserInput* EventManager::PopNextUserInputEvent()
    {
        return dynamic_cast<EventUserInput*> (this->PopNextSpecificEvent(EventBase::UserInput));
    }

    void EventManager::RemoveNextUserInputEvent()
    {
        this->RemoveNextSpecificEvent(EventBase::UserInput);
    }

    std::list<EventUserInput*>* EventManager::GetAllUserInputEvents()
    {
        std::list<EventBase*>* TempList = this->GetAllSpecificEvents(EventBase::UserInput);
        std::list<EventUserInput*>* Results= new std::list<EventUserInput*>;

        for(std::list<EventBase*>::iterator Iter = TempList->begin(); Iter!=TempList->end(); Iter++)
        {
            Results->push_back( dynamic_cast<EventUserInput*> (*Iter) );
        }

        return Results;
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Filtered management functions - Quit Event
    ///////////////////////////////////////

    EventQuit* EventManager::GetNextQuitEvent()
    {
        return dynamic_cast<EventQuit*> (this->GetNextSpecificEvent(EventBase::QuitMessage));
    }

    EventQuit* EventManager::PopNextQuitEvent()
    {
        return dynamic_cast<EventQuit*> (this->PopNextSpecificEvent(EventBase::QuitMessage));
    }

    void EventManager::RemoveNextQuitEvent()
    {
        this->RemoveNextSpecificEvent(EventBase::QuitMessage);
    }

    std::list<EventQuit*>* EventManager::GetAllQuitEvents()
    {
        std::list<EventBase*>* TempList = this->GetAllSpecificEvents(EventBase::QuitMessage);
        std::list<EventQuit*>* Results= new std::list<EventQuit*>;

        for(std::list<EventBase*>::iterator Iter = TempList->begin(); Iter!=TempList->end(); Iter++)
        {
            Results->push_back( dynamic_cast<EventQuit*> (*Iter) );
        }

        return Results;
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Polling Functions
    ///////////////////////////////////////

    void EventManager::AddPollingCheck(const MetaCode &InputToTryPolling)
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
            this->GameWorld->LogAndThrow("Unsupported Polling Check on this Platform");
    }

    void EventManager::RemovePollingCheck(const MetaCode &InputToStopPolling)
    {
        bool ItFailed = true; // who cares why it failed, it did, and should be reported
        bool supported = false; //used to help determine if the metacode passed in was nonsense or could have been valid

        //Check for keyboard code
        if ( MetaCode::KEY_LAST > InputToStopPolling.GetCode() && InputToStopPolling.GetCode() > MetaCode::KEY_FIRST)
        {
            supported=true;

            vector<MetaCode::InputCode>::iterator KeyIter;
            for(KeyIter = this->WatchKeyboardKeys.begin(); KeyIter!=this->WatchKeyboardKeys.end(); KeyIter++) //Check Each
            {
                if( *KeyIter == InputToStopPolling.GetCode())
                {
                    this->WatchKeyboardKeys.erase(KeyIter);
                    ItFailed=false;
                }
            }
        }

        //if it is a specific mouse button, then
        if ( MetaCode::MOUSEBUTTON == InputToStopPolling.GetCode())
        {
            supported=true;

            vector<int>::iterator MouseIter;
            for(MouseIter = this->WatchMouseKeys.begin(); MouseIter!=this->WatchMouseKeys.end(); MouseIter++) //Check Each
            {
                if( *MouseIter == InputToStopPolling.GetCode())
                {
                    this->WatchMouseKeys.erase(MouseIter);
                    ItFailed=false;
                }
            }
        }

        //Mouse Movement
        if ( MetaCode::MOUSEABSOLUTEVERTICAL == InputToStopPolling.GetCode())
        {
            supported=true;
            if(PollMouseVert == true)
                {ItFailed=false;}
            PollMouseVert = false;

        }

        if ( MetaCode::MOUSEABSOLUTEHORIZONTAL == InputToStopPolling.GetCode())
        {
            supported=true;
            if(PollMouseHor == true)
                {ItFailed=false;}
            PollMouseHor = false;
        }

        if (!supported)
            this->GameWorld->LogAndThrow("Unsupported Polling Check on this Platform, Cannot Remove");
        if (ItFailed)
            this->GameWorld->LogAndThrow("Polling Check did not exist, Cannot Remove");
    }


    EventUserInput* EventManager::PollForUserInputEvents()
    {
        vector<MetaCode> MetaBag;

        //Call the private Polling routines
        PollKeyboard(MetaBag);
        PollMouseButtons(MetaBag);
        PollMouseLocation(MetaBag);

        EventUserInput* test = new EventUserInput(MetaBag);
        return test;
    }

    //Internal private Polling routine
    void EventManager::PollKeyboard(vector<MetaCode> &CodeBag)
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
    void EventManager::PollMouseButtons(vector<MetaCode> &CodeBag)
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

    void EventManager::PollMouseLocation(vector<MetaCode> &CodeBag)
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
    void EventManager::PreProcessSDLEvents()
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

    void EventManager::UpdateQuitEvents()
    {
        if (NULL == SDL_GetEventFilter())           //Verify the Event filter is installed, if not, then install it.
        {
            SDL_SetEventFilter( internal::PhysSDLFilter );
        }else{
            if(4==internal::PhysSDLFilter(0))                 //Pass it a null pointer to get it to "Not Callback Mode"
            {
                this->AddEvent(new EventQuit());    //We need to make a quit event
            }else{
                //all clear
            }
        }
    }

    //Inherited From ManagerBase
    void EventManager::Initialize()
        {}

    void EventManager::DoMainLoopItems()
    {
        this->UpdateSystemEvents();
        this->UpdateUserInputEvents();
    }

    ManagerBase::ManagerTypeName EventManager::GetType() const
        { return ManagerBase::EventManager; }

    // Misc functions
    Vector2 EventManager::GetMouseCoords()
        { return this->CurrentMouseCoords; }

} // /phys

#endif
