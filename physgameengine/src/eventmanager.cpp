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
#include "eventbase.h"
#include "eventrendertime.h"
#include "eventuserinput.h"
#include "eventquit.h"
#include "metacode.h"
#include "world.h"

#include <map>

#include "SDL.h"
//#include <boost/thread/thread.hpp> //will use this when this becomes multithreaded

namespace phys
{
    /// @internal
    /// @namespace phys::internal
    /// @brief This namespace is used for internal helper classes, and in general it should be ignored by game developers
    /// @details This whole internal namespace is a home for dirty hacks and internal dependant code. This is where code goes that must implement classes or functions for the various subsytems the Physgame engine draws on.
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

        /// @internal
        /// @brief Used to increase encapsulation, just a bit.
        struct EventManagerInternalData
        {

            //a List of the Keyboard keys being watch
            vector<MetaCode::InputCode> WatchKeyboardKeys;
            // A list of the Mouse buttons being watched
            vector<MetaCode::InputCode> WatchMouseKeys;
            //These are use to decide if mouse location should be polled.
            bool PollMouseHor;
            bool PollMouseVert;
            // the cache of mouse buttons so that events can be thrown the entire time the mouse button is down
            std::vector< std::pair<bool,bool> > MouseButtonCache;
            //an internal queue of Window management events that happened during the frame that need to be converted into phys::Events
            queue<RawEvent*> SDL_WmEvents;
            // and internal queue of userinput events
            queue<RawEvent*> SDL_UserInputEvents;
            // what events are coming from SDL
            //std::list<RawEvent*> SDL_EventQ;

            ///////////////////////////////////////////////////////////////////////////////
            // New datastructures to replace old ones


            //The Queue that all the events get stored in
            std::list<EventBase*> EventQ;

            /// @internal
            /// @brief The kinds of data that can be stored in Manual Check
            enum PollingType{
                Polling     =1,
                Keypress    =2

            };

            // A unified polling and event repeater
            // if true the item is to be removed when the key is lifted, if false it remains until the polling check is removed
            // the Inputcode is the kind of event to check for each frame.
            std::map<MetaCode::InputCode, PollingType> ManualCheck;

            /// @internal
            typedef std::map<MetaCode::InputCode, PollingType>::iterator ManualCheckIterator;

            /// @internal
            /// @brief Adds one type of polling check
            /// @param OneCode The code that will be check for each frame, under the new condition
            /// @param _PollingCheck This is inserted into a new polling check or it is bitwise or'ed into an existing one, and this will trigger other parts of the code to insert event later on
            void AddInputCodeToManualCheck(const MetaCode::InputCode& OneCode, PollingType _PollingCheck)
            {
                ManualCheckIterator Which = ManualCheck.find(OneCode);
                if(ManualCheck.end() == Which )
                {
                    ManualCheck[OneCode] = _PollingCheck;
                }else{
                    Which->second = (PollingType)(Which->second | _PollingCheck); //An Item can be multiple kinds of polling checks
                }
            }

            /// @internal
            /// @brief Used to insert Codes into the list of items to be manually checked
            /// @param Transport A vector of every Meta that may need to be added to the list
            void AddMetaCodesToManualCheck(vector<MetaCode> Transport, PollingType _PollingCheck)
            {
                for ( vector<MetaCode>::iterator Iter=Transport.begin(); Iter!=Transport.end(); ++Iter)
                {
                    AddInputCodeToManualCheck(Iter->GetCode(), _PollingCheck);
                }
            }

            /// @internal
            /// @brief Removes one type of polling check
            /// @param OneCode The code that will no longer be checked each frame, under the given condition
            /// @param _PollingCheck This is the polling into a new polling check or it is bitwise or'ed into an existing one, and this will trigger other parts of the code to insert event later on
            void RemoveInputCodeToManualCheck(const MetaCode::InputCode& OneCode, PollingType _PollingCheck)
            {
                ManualCheckIterator Which = ManualCheck.find(OneCode);
                if( ManualCheck.end() != Which )
                {
                    if(Which->second == _PollingCheck)
                        { ManualCheck.erase(Which); }                           // if they are are equal then this
                    else
                        { Which->second = (PollingType)(Which->second & (~_PollingCheck)); }
                }
            }

            /// @internal
            /// @brief Remove Items form the internal manual check list
            /// @param Transport A vector of every MetaCode that may need to be removed to the list
            void RemoveMetaCodesToManualCheck(vector<MetaCode> Transport, PollingType _PollingCheck)
            {
                for ( vector<MetaCode>::iterator Iter=Transport.begin(); Iter!=Transport.end(); ++Iter)
                {
                    RemoveInputCodeToManualCheck(Iter->GetCode(), _PollingCheck);
                }
            }

        };

    } // /internal

    /// @todo TODO: Make the EventManager completely thread safe. IF this is completely thread safe, we can spawn numerous individual thread each accessing this and
    /// and the performance gain would almost scale directly with cpu core count increases. Look at boost scoped_lock
    EventManager::EventManager()
    {
        this->Priority=-40;
        this->_Data = new internal::EventManagerInternalData;

        this->GameWorld = World::GetWorldPointer();

        this->_Data->PollMouseHor = false;
        this->_Data->PollMouseVert = false;
        _Data->MouseButtonCache.resize(16);
        _Data->MouseButtonCache.insert(_Data->MouseButtonCache.begin(),16,std::pair<bool,bool>(false,false));
    }

    EventManager::~EventManager()
    {
        for(std::list<EventBase*>::iterator Iter = _Data->EventQ.begin(); Iter!=_Data->EventQ.end(); Iter++)
            { delete *Iter; }
        delete _Data;
    }

    ///////////////////////////////////////////////////////////////////////////////
    //These functions will give you the next event or help you manage the events
    ///////////////////////////////////////
    size_t EventManager::GetRemainingEventCount()
    {
        return _Data->EventQ.size();
    }

    EventBase* EventManager::GetNextEvent()
    {
        if(_Data->EventQ.empty())
            { return 0; }
        EventBase* results = _Data->EventQ.front();
        return results;
    }

    EventBase* EventManager::PopNextEvent()
    {
        if(_Data->EventQ.empty())
            { return 0; }
        EventBase* results = _Data->EventQ.front();
        _Data->EventQ.pop_front();
        return results;
    }

    void EventManager::RemoveNextEvent()
    {
        _Data->EventQ.pop_front();
    }

    void EventManager::AddEvent(EventBase* EventToAdd)
    {
        _Data->EventQ.push_back(EventToAdd);
    }

    void EventManager::UpdateEvents()
    {
        UpdateQuitEvents(); //quit events skips the preprocessing step and goes straight into the the main Queue, becuase of how we need to get them from sdl

        RawEvent FromSDLRaw;                                    //used to hold data as we go through loop
        EventUserInput* FromSDLEvent = new EventUserInput();    //Used to build up all of our userinput data into one event

        while(SDL_PollEvent(&FromSDLRaw))
        {
            switch(FromSDLRaw.type)
            {
                case SDL_ACTIVEEVENT:   //when the window gains focus
                case SDL_VIDEORESIZE:   //when the screen is resized
                case SDL_VIDEOEXPOSE:   //when the windows goes from being hidden to being shown
                case SDL_SYSWMEVENT:
                case SDL_USEREVENT:
                    /// @todo handle unhandled user system events
                    //_Data->EventQ.push_back(FromSDLEvent);
                    break;

                case SDL_MOUSEBUTTONUP:     case SDL_KEYUP:             case SDL_JOYBUTTONUP:
                    _Data->RemoveMetaCodesToManualCheck( FromSDLEvent->AddCodesFromRawEvent(FromSDLRaw),  internal::EventManagerInternalData::Keypress);
                    break;

                case SDL_MOUSEBUTTONDOWN:   case SDL_KEYDOWN:           case SDL_JOYBUTTONDOWN:
                    _Data->AddMetaCodesToManualCheck( FromSDLEvent->AddCodesFromRawEvent(FromSDLRaw), internal::EventManagerInternalData::Keypress);
                    break;

                case SDL_MOUSEMOTION:       case SDL_JOYAXISMOTION:     case SDL_JOYBALLMOTION:     case SDL_JOYHATMOTION:
                    FromSDLEvent->AddCodesFromRawEvent(FromSDLRaw);
                    break;

                case SDL_QUIT:          //when SDL closes, but this really should be handled somewhere else, like the UpdateQuitEvents() function
                    World::GetWorldPointer()->LogAndThrow("Unexpected Quit event in event manager.");
                    break;
                //Never thrown by SDL, but could be added by a user
                default:
                    World::GetWorldPointer()->LogAndThrow("Unknown SDL Event Inserted.");
                    break;

            }
        }

        // Here we need to iterate through manualcheck and make sure each item there actually is in FromSDLEvent
        //for()
        World::GetWorldPointer()->Log("Temp");

        //Check to see if we should add a User i
        if(FromSDLEvent->GetMetaCodeCount()==0)
        {
            delete FromSDLEvent;
        }else{
            _Data->EventQ.push_back(FromSDLEvent);
            /// @todo for each user input event update the ManualCheck list
            //
        }

        ////////

//      EventUserInput* FromSDLPolling = this->PollForUserInputEvents();

        //read through the pending user input events and add those codes
//        while( !_Data->SDL_UserInputEvents.empty() )
//        {
//            RawEvent* CurrentRawEvent = _Data->SDL_UserInputEvents.front();
//
//            if(CurrentRawEvent->type == SDL_MOUSEBUTTONDOWN || CurrentRawEvent->type == SDL_MOUSEBUTTONUP)
//            {
//                FromSDLEvent->AddCode(CurrentRawEvent->button.x, MetaCode::MOUSEABSOLUTEHORIZONTAL);
//                FromSDLEvent->AddCode(CurrentRawEvent->button.y, MetaCode::MOUSEABSOLUTEVERTICAL);
//                if ( SDL_BUTTON_WHEELUP==CurrentRawEvent->button.button)
//                {
//                    FromSDLEvent->AddCode(MetaCode::MOUSEWHEEL_UP,MetaCode::MOUSEWHEELVERTICAL);
//                }else if( SDL_BUTTON_WHEELDOWN==CurrentRawEvent->button.button ){
//                    FromSDLEvent->AddCode(MetaCode::MOUSEWHEEL_DOWN,MetaCode::MOUSEWHEELVERTICAL);
//                }else{
//                    if(CurrentRawEvent->button.state==SDL_PRESSED /*&& !MouseButtonCache[CurrentRawEvent->button.button]*/){
//                        /// @todo verify this works after id removal from metacode.
//                        FromSDLEvent->AddCode(MetaCode::BUTTON_PRESSING, MetaCode::GetMouseButtonCode(CurrentRawEvent->button.button));
//                        //FromSDLEvent->AddCode(MetaCode::BUTTON_PRESSING, CurrentRawEvent->button.button, MetaCode::MOUSEBUTTON);
//                        _Data->MouseButtonCache[CurrentRawEvent->button.button].first = true; //changed this frame
//                        _Data->MouseButtonCache[CurrentRawEvent->button.button].second = true;//is pressed
//                    /*}else if(CurrentRawEvent->button.state==SDL_PRESSED && MouseButtonCache[CurrentRawEvent->button.button]){
//                        FromSDLEvent->AddCode(MetaCode::BUTTON_DOWN, CurrentRawEvent->button.button, MetaCode::MOUSEBUTTON);*/
//                    }else if(CurrentRawEvent->button.state==SDL_RELEASED /*&& MouseButtonCache[CurrentRawEvent->button.button]*/){
//                        /// @todo verify this works after id removal from metacode.
//                        FromSDLEvent->AddCode(MetaCode::BUTTON_LIFTING, MetaCode::GetMouseButtonCode(CurrentRawEvent->button.button));
////                        FromSDLEvent->AddCode(MetaCode::BUTTON_LIFTING, CurrentRawEvent->button.button, MetaCode::MOUSEBUTTON);
//                        _Data->MouseButtonCache[CurrentRawEvent->button.button].first = true; //changed this frame
//                        _Data->MouseButtonCache[CurrentRawEvent->button.button].second = false;//is pressed
//                    }
//                }
//            }else{
//                FromSDLEvent->AddCodesFromRawEvent( *CurrentRawEvent );
//            }
//
//            delete CurrentRawEvent;
//            _Data->SDL_UserInputEvents.pop(); //NEXT!!!
//        }

//        /// @todo This isn't pretty, should be replaced with a more elegant solution that'll work on keyboard events as well.
//        unsigned int x=0;
//        for(std::vector<std::pair<bool,bool> >::iterator it = _Data->MouseButtonCache.begin();it!=_Data->MouseButtonCache.end();it++)
//        {
//            if(!((*it).first) && (*it).second)
//            {
//                FromSDLEvent->AddCode(MetaCode::BUTTON_DOWN, MetaCode::GetMouseButtonCode(x));
//            }
//            x++;
//        }
//
//        *FromSDLEvent += *FromSDLPolling;
//        delete FromSDLPolling;
//
//        //Did atleast one userinput get generates, or atleast one polling request
//        if (0 < FromSDLEvent->GetMetaCodeCount())
//        {
//            //check for mouse input before we send along it all to the event holding ground
//            for (Whole c=0;c<FromSDLEvent->GetMetaCodeCount();++c)
//            {
//                /*if(FromSDLEvent->GetMetaCode(c).GetCode() == MetaCode::MOUSEABSOLUTEHORIZONTAL)
//                    { this->CurrentMouseCoords.X = FromSDLEvent->GetMetaCode(c).GetMetaValue(); }
//                if(FromSDLEvent->GetMetaCode(c).GetCode() == MetaCode::MOUSEABSOLUTEVERTICAL)
//                    { this->CurrentMouseCoords.Y = FromSDLEvent->GetMetaCode(c).GetMetaValue(); }*/
//            }
//            this->AddEvent(FromSDLEvent); //Now FromSDL is some else's responsibility
//        }else{
//            delete FromSDLEvent;
//        }
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Filtered management functions - You choose YAYYYY!!!
    ///////////////////////////////////////
    EventBase* EventManager::GetNextSpecificEvent(EventBase::EventType SpecificType)
    {
        EventBase* results = 0;
        for(std::list<EventBase*>::iterator Iter = _Data->EventQ.begin(); Iter!=_Data->EventQ.end(); ++Iter)
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
        for(std::list<EventBase*>::iterator Iter = _Data->EventQ.begin(); Iter!=_Data->EventQ.end(); ++Iter)
        {
            if((*Iter)->GetType()==SpecificType)
            {
                results = (*Iter);
                _Data->EventQ.erase(Iter);
                return results;
            }
        }
        return results;
    }

    void EventManager::RemoveNextSpecificEvent(EventBase::EventType SpecificType)
    {
        for(std::list<EventBase*>::iterator Iter = _Data->EventQ.begin(); Iter!=_Data->EventQ.end(); ++Iter)
        {
            if((*Iter)->GetType()==SpecificType)
            {
                _Data->EventQ.erase(Iter);
            }
        }
    }

    std::list<EventBase*>* EventManager::GetAllSpecificEvents(EventBase::EventType SpecificType)
    {
        std::list<EventBase*>* TempList = new std::list<EventBase*>;

        for(std::list<EventBase*>::iterator Iter = _Data->EventQ.begin(); Iter!=_Data->EventQ.end(); ++Iter)
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
        for(std::list<EventBase*>::iterator Iter = _Data->EventQ.begin(); Iter!=_Data->EventQ.end(); ++Iter)
        {
            if((*Iter)->GetType()==SpecificType)
            {
                this->_Data->EventQ.remove(*Iter);
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
        #ifdef PHYSDEBUG
        World::GetWorldPointer()->Log(InputToTryPolling);
        #endif


        //Check for keyboard code
        if ( MetaCode::KEY_LAST >= InputToTryPolling.GetCode() && InputToTryPolling.GetCode() >= MetaCode::KEY_FIRST)
        {
            this->_Data->WatchKeyboardKeys.push_back(InputToTryPolling.GetCode());
            return;
        }

        //if it is a specific mouse button, then
        if ( MetaCode::MOUSEBUTTON_LAST >= InputToTryPolling.GetCode() && InputToTryPolling.GetCode() >= MetaCode::MOUSEBUTTON_FIRST )
        {
            this->_Data->WatchMouseKeys.push_back(InputToTryPolling.GetCode());
            return;
        }

        //Mouse Movement
        if ( MetaCode::MOUSEABSOLUTEVERTICAL == InputToTryPolling.GetCode())
        {
            this->_Data->PollMouseVert = true;
            return;
        }

        if ( MetaCode::MOUSEABSOLUTEHORIZONTAL == InputToTryPolling.GetCode())
        {
            this->_Data->PollMouseHor = true;
            return;
        }

        // No polling check added
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
            for(KeyIter = this->_Data->WatchKeyboardKeys.begin(); KeyIter!=this->_Data->WatchKeyboardKeys.end(); KeyIter++) //Check Each
            {
                if( *KeyIter == InputToStopPolling.GetCode())
                {
                    this->_Data->WatchKeyboardKeys.erase(KeyIter);
                    ItFailed=false;
                }
            }
        }

        //if it is a specific mouse button, then
        if ( MetaCode::MOUSEBUTTON == InputToStopPolling.GetCode())
        {
            supported=true;

            vector<MetaCode::InputCode>::iterator MouseIter;
            for(MouseIter = this->_Data->WatchMouseKeys.begin(); MouseIter!=this->_Data->WatchMouseKeys.end(); MouseIter++) //Check Each
            {
                if( *MouseIter == InputToStopPolling.GetCode())
                {
                    this->_Data->WatchMouseKeys.erase(MouseIter);
                    ItFailed=false;
                }
            }
        }

        //Mouse Movement
        if ( MetaCode::MOUSEABSOLUTEVERTICAL == InputToStopPolling.GetCode())
        {
            supported=true;
            if(this->_Data->PollMouseVert == true)
                {ItFailed=false;}
            this->_Data->PollMouseVert = false;

        }

        if ( MetaCode::MOUSEABSOLUTEHORIZONTAL == InputToStopPolling.GetCode())
        {
            supported=true;
            if(this->_Data->PollMouseHor == true)
                {ItFailed=false;}
            this->_Data->PollMouseHor = false;
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
        if(this->_Data->WatchKeyboardKeys.size()>0)
        {
            int* KeyCount = 0;
            Uint8* KeyboardSnapshot = SDL_GetKeyState(KeyCount);

            vector<MetaCode::InputCode>::iterator iter;
            for(iter = this->_Data->WatchKeyboardKeys.begin(); iter != (this->_Data->WatchKeyboardKeys.end()) ; iter++)
            {
                if( *iter < MetaCode::KEY_LAST )//Is it a valid keycode
                {
                    if(KeyboardSnapshot[*iter]) // is it up or down
                    {//down
                        MetaCode temp(MetaCode::BUTTON_DOWN, *iter);
                        CodeBag.push_back(temp);
                    }else{//up
                        MetaCode temp(MetaCode::BUTTON_UP, *iter);
                        CodeBag.push_back(temp);
                    }
                }
            }
        }
    }

    //Internal private Polling routine
    void EventManager::PollMouseButtons(vector<MetaCode> &CodeBag)
    {
        if(this->_Data->WatchMouseKeys.size()>0)
        {
            vector<MetaCode::InputCode>::iterator iter;
            for(iter = this->_Data->WatchMouseKeys.begin(); iter != (this->_Data->WatchMouseKeys.end()) ; iter++)
            {
                /// @todo verify/fix mouse event polling after removal of metacode ID
                if(SDL_GetMouseState(NULL, NULL)&SDL_BUTTON(*iter))
                {
                        MetaCode temp(MetaCode::BUTTON_DOWN,*iter);
                        CodeBag.push_back(temp);
                }else{
                        MetaCode temp(MetaCode::BUTTON_UP,*iter);
                        CodeBag.push_back(temp);
                }
            }
        }

    }

    void EventManager::PollMouseLocation(vector<MetaCode> &CodeBag)
    {
        if( this->_Data->PollMouseHor || this->_Data->PollMouseVert )
        {
            int Vert=0;
            int Hor=0;
            SDL_GetMouseState(&Hor, &Vert);
            if( this->_Data->PollMouseVert )
            {
                MetaCode temp(Vert,MetaCode::MOUSEABSOLUTEVERTICAL);
                CodeBag.push_back(temp);
            }
            if( this->_Data->PollMouseHor )
            {
                MetaCode temp(Hor,MetaCode::MOUSEABSOLUTEHORIZONTAL);
                CodeBag.push_back(temp);
            }
        }
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Standard interupt basd user input

    //This function will get all the events from SDL and Sort them into one of two Queues
   /* void EventManager::PreProcessSDLEvents()
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
                    _Data->SDL_WmEvents.push(ScopeHolder);
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
                    _Data->SDL_UserInputEvents.push(ScopeHolder);
                    break;
                case SDL_USEREVENT://Never thrown by SDL, but could be added by a user
                default:
                    throw ("Unknown SDL Event Inserted");
                    break;
            }
//            _Data->SDL_EventQ.push_back(ScopeHolder);
        }
    }*/

    void EventManager::UpdateQuitEvents()
    {
        if (NULL == SDL_GetEventFilter())                       //Verify the Event filter is installed, if not, then install it.
        {
            SDL_SetEventFilter( internal::PhysSDLFilter );
        }else{
            if(4==internal::PhysSDLFilter(0))                   //Pass it a null pointer to get it to "Not Callback Mode"
            {
                this->AddEvent(new EventQuit());                //We need to make a quit event
            }else{
                //all clear
            }
        }
    }

    //Inherited From ManagerBase
    void EventManager::Initialize()
        {}

    void EventManager::DoMainLoopItems()
        { this->UpdateEvents(); }

    ManagerBase::ManagerTypeName EventManager::GetType() const
        { return ManagerBase::EventManager; }

} // /phys

#endif
