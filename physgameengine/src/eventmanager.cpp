//© Copyright 2010 - 2011 BlackTopp Studios Inc.
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
#include "eventgamewindow.h"
#include "eventrendertime.h"
#include "eventuserinput.h"
#include "eventquit.h"
#include "metacode.h"
#include "world.h"
#include "inputquerytool.h"

#include <map>
#include <memory>
#include <cassert>

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
        /// @param userdata Nothing, a dummy argument that could be used by SDL
        /// @warning Do not use this. It can only cause problems. This is for SDL, the user input subsystem, to filter certain events.
        /// @return This will always return either 0 or 1 to SDL. 0 if it should drop the event, which it does to all SDL_quit events, 1 if the event should be allowed, which it does to all events which are not SDL_quit events. If a null pointer was passed, then this will return 4 if it dropped an SDL_Quit, and 2 if it has not droppped an SDL_quit.
        //int PhysSDLFilter(void *userdata, const SDL_Event *event,  );
        //If this is passed an event that points to 0 it will function as a method to tell us if an SDL_QUIT message has been thrown
        //this will return 2 if it has not seen an SDL_quit, and a 4 if it has
        int PhysSDLFilter(void *userdata, SDL_Event *event )
        {
            //We need to protect this thing with a Mutex, SDL 1.3 says this filter could run in different thread.
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
            /// @internal
            /// @brief The Queue that all the events get stored in
            std::list<EventBase*> EventQ;

            /// @internal
            /// @brief The kinds of data that can be stored in Manual Check
            enum PollingType{
                Polling     =1,
                Keypress    =2,

                PollingKeyPress = Polling|Keypress
            };

            /// @internal
            /// @brief A unified polling and event repeater
            /// the Inputcode is the kind of event to check for each frame. The PollingType is used to control what can turn on and off the pollingcheck check.
            std::map<MetaCode::InputCode, PollingType> ManualCheck;

            /// @internal
            /// @brief an Iterator suitable for use with internal structures that correlate polling type and metacodes
            typedef std::map<MetaCode::InputCode, PollingType>::iterator ManualCheckIterator;

            /// @internal
            /// @brief Adds one type of polling check
            /// @param OneCode The code that will be check for each frame, under the new condition
            /// @param _PollingCheck This is inserted into a new polling check or it is bitwise or'ed into an existing one, and this will trigger other parts of the code to insert event later on
            void AddInputCodeToManualCheck(MetaCode::InputCode OneCode, PollingType _PollingCheck)
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
            /// @brief Adds one type of polling check where the data is only available as Ints
            /// @param OneCode The code that will be check for each frame, under the new condition
            /// @param _PollingCheck This is inserted into a new polling check or it is bitwise or'ed into an existing one, and this will trigger other parts of the code to insert event later on
            void AddInputCodesToManualCheck(int OneCode, int _PollingCheck)
            {
                this->AddInputCodeToManualCheck(
                        static_cast<phys::MetaCode::InputCode>(OneCode),
                        static_cast<phys::internal::EventManagerInternalData::PollingType>(_PollingCheck)
                );
            }

            /// @internal
            /// @brief Adds one type of polling check where the data is only available as Ints
            /// @param OneCode The metacode that contains the input that will be checked for each frame
            /// @param _PollingCheck This is inserted into a new polling check or it is bitwise or'ed into an existing one, and this will trigger other parts of the code to insert event later on
            void AddMetaCodesToManualCheck(const MetaCode& OneCode, PollingType _PollingCheck)
            { AddInputCodesToManualCheck(OneCode.GetCode(),_PollingCheck); }

            /// @internal
            /// @brief Used to insert Codes into the list of items to be manually checked
            /// @param Transport A vector of every Meta that may need to be added to the list
            /// @param _PollingCheck This is inserted into a new polling check or it is bitwise or'ed into an existing one, and this will trigger other parts of the code to insert event later on
            void AddMetaCodesToManualCheck(vector<MetaCode> Transport, PollingType _PollingCheck)
            {
                for ( vector<MetaCode>::iterator Iter=Transport.begin(); Iter!=Transport.end(); ++Iter)
                {
                    MetaCode::InputCode temp = Iter->GetCode();
                    AddInputCodeToManualCheck(Iter->GetCode(), _PollingCheck);
                }
            }

            /// @internal
            /// @brief Remove a kind polling check from an item or remove item if that is the last check
            /// @param Reduce An Iterator Referencing an item in manual check
            /// @param _PollingCheck What kind of Polling Type to Remove.
            void RemovePollingCheck(ManualCheckIterator Reduce, PollingType _PollingCheck)
            {
                if( ManualCheck.end() != Reduce )
                {
                    if(Reduce->second == _PollingCheck)
                        { ManualCheck.erase(Reduce); }
                    else
                        { Reduce->second = (PollingType)(Reduce->second & (~_PollingCheck)); }
                }
            }

            /// @internal
            /// @brief Removes one type of polling check
            /// @param OneCode The code that will no longer be checked each frame, under the given condition
            /// @param _PollingCheck If this matches via bitwise or with the kind of polling check check stored for the existing InputCode then the it will be removed.
            void RemoveInputCodeToManualCheck(const MetaCode::InputCode& OneCode, PollingType _PollingCheck)
            {
                ManualCheckIterator Which = ManualCheck.find(OneCode);
                RemovePollingCheck( Which, _PollingCheck );
            }

            /// @internal
            /// @brief Removes one type of polling check
            /// @param OneCode A metacode that contains the the inputcode to remove
            /// @param _PollingCheck If this matches via bitwise or with the kind of polling check check stored for the existing InputCode then the it will be removed.
            void RemoveMetaCodesToManualCheck(const MetaCode& OneCode, PollingType _PollingCheck)
                { RemoveInputCodeToManualCheck(OneCode.GetCode(),_PollingCheck); }

            /// @internal
            /// @brief Remove Items form the internal manual check list
            /// @param Transport A vector of every MetaCode that may need to be removed to the list
            /// @param _PollingCheck If this matches via bitwise or with the kind of polling check check stored for the existing InputCode then the it will be removed.
            void RemoveMetaCodesToManualCheck(vector<MetaCode> Transport, PollingType _PollingCheck)
            {
                for ( vector<MetaCode>::iterator Iter=Transport.begin(); Iter!=Transport.end(); ++Iter)
                    { RemoveInputCodeToManualCheck(Iter->GetCode(), _PollingCheck); }
            }

            /// @internal
            /// @brief Drops all keypresses from the list of items to be perpetuated each frame.
            void DropAllKeyPresses()
            {
                for (ManualCheckIterator Which = ManualCheck.begin(); Which!=ManualCheck.end(); ++Which)
                    { RemovePollingCheck(Which, Keypress); }
            }

            /// @internal
            /// @brief This will identify the main Joystick
            SDL_Joystick *Joy0;

            /// @internal
            /// @brief Constructor, it only inits pointers to 0
            EventManagerInternalData()
            {
                Joy0=0;
            }

        };

    } // /internal

    /// @todo TODO: Make the EventManager completely thread safe. IF this is completely thread safe, we can spawn numerous individual thread each accessing this and
    /// and the performance gain would almost scale directly with cpu core count increases. Look at boost scoped_lock
    EventManager::EventManager()
    {
        this->Priority=-40;
        this->_Data = new internal::EventManagerInternalData;

        SDL_InitSubSystem(SDL_INIT_JOYSTICK);
        this->DetectJoysticks();

        //Remove GameWorld Pointer From everything
        this->GameWorld = World::GetWorldPointer();
    }

    EventManager::~EventManager()
    {
        //EndRelativeMouseMode();

        if(SDL_JoystickOpened(0))
            { SDL_JoystickClose(this->_Data->Joy0); }

        for(std::list<EventBase*>::iterator Iter = _Data->EventQ.begin(); Iter!=_Data->EventQ.end(); Iter++)
            { delete *Iter; }
        delete _Data;
    }

    void EventManager::DetectJoysticks()
    {
        if( SDL_NumJoysticks()>0 )
            { this->_Data->Joy0=SDL_JoystickOpen(0); }
    }

    ///////////////////////////////////////////////////////////////////////////////
    //These functions will give you the next event or help you manage the events
    ///////////////////////////////////////
    size_t EventManager::GetRemainingEventCount()
        { return _Data->EventQ.size(); }

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
        { _Data->EventQ.pop_front(); }

    void EventManager::AddEvent(EventBase* EventToAdd)
        { _Data->EventQ.push_back(EventToAdd); }

    void EventManager::UpdateEvents()
    {
        UpdateQuitEvents(); //quit events skips the preprocessing step and goes straight into the the main Queue, becuase of how we need to get them from sdl

        RawEvent FromSDLRaw;                                    //used to hold data as we go through loop
        EventUserInput* FromSDLEvent = new EventUserInput();    //Used to build up all of our userinput data into one event
        bool ClearKeyPresses=false;                             //if true All the keypresses will be dropped and all keys will be assumed to be up

        // Here we iterate through manual check to insert any requested polling checks and perpetuate button and key down events
        for(internal::EventManagerInternalData::ManualCheckIterator Iter=_Data->ManualCheck.begin(); _Data->ManualCheck.end()!=Iter; ++Iter)
        {
            if(internal::EventManagerInternalData::Keypress & Iter->second)     //if the keypress event is in there, then the key must be down
                { FromSDLEvent->AddCode(MetaCode::BUTTON_DOWN, Iter->first); }
            else
                { FromSDLEvent->AddCode(MetaCode::BUTTON_UP, Iter->first); }    //It must be just a polling check
        }

        /* Here is a list of SDL event which aren't coded yet.
        //event types
        Tabbed items are fully working
        items with one space are partially implemented of known to not work
        other items are unimplemented
            SDL_FIRSTEVENT				unused (do not remove)		Application events
            SDL_QUIT				user-requested quit		Window events
            SDL_WINDOWEVENT				window state change
         SDL_SYSWMEVENT				system specific event		Keyboard events
            SDL_KEYDOWN				key pressed
            SDL_KEYUP				key released
        SDL_TEXTEDITING				keyboard text editing (composition)
        SDL_TEXTINPUT				keyboard text input		Mouse events
            SDL_MOUSEMOTION				mouse moved
            SDL_MOUSEBUTTONDOWN				mouse button pressed
            SDL_MOUSEBUTTONUP				mouse button released
        SDL_MOUSEWHEEL				mouse wheel motion		Tablet or multiple mice input device events
        SDL_INPUTMOTION				input moved
        SDL_INPUTBUTTONDOWN				input button pressed
        SDL_INPUTBUTTONUP				input button released
        SDL_INPUTWHEEL				input wheel motion
        SDL_INPUTPROXIMITYIN				input pen entered proximity
        SDL_INPUTPROXIMITYOUT				input pen left proximity		Joystick events
            SDL_JOYAXISMOTION				joystick axis motion
         SDL_JOYBALLMOTION				joystick trackball motion
            SDL_JOYHATMOTION				joystick hat position change
            SDL_JOYBUTTONDOWN				joystick button pressed
            SDL_JOYBUTTONUP				joystick button released		Touch events
        SDL_FINGERDOWN
        SDL_FINGERUP
        SDL_FINGERMOTION
        SDL_TOUCHBUTTONDOWN
        SDL_TOUCHBUTTONUP				Gesture events
        SDL_DOLLARGESTURE
        SDL_DOLLARRECORD
        SDL_MULTIGESTURE				Clipboard events
        SDL_CLIPBOARDUPDATE				the clipboard changed		Obsolete events
        SDL_EVENT_COMPAT1				SDL 1.2 events for compatibility
        SDL_EVENT_COMPAT2				SDL 1.2 events for compatibility
        SDL_EVENT_COMPAT3				SDL 1.2 events for compatibility		These are for your use, and should be allocated with
        SDL_RegisterEvents()
        SDL_USEREVENT				a user-specified event
        SDL_LASTEVENT				only for bounding internal arrays
        */

        while(SDL_PollEvent(&FromSDLRaw))
        {
            switch(FromSDLRaw.type)
            {
        //Events and User input sorted by estimate frequency
                case SDL_MOUSEBUTTONUP:     case SDL_KEYUP:             case SDL_JOYBUTTONUP:{
                    MetaCode ResultCode(FromSDLRaw);
                    _Data->RemoveMetaCodesToManualCheck( FromSDLEvent->AddCode(ResultCode), internal::EventManagerInternalData::Keypress);
                    break;}

                case SDL_KEYDOWN: {
                    MetaCode ResultCode(FromSDLRaw);
                    if ( !(_Data->ManualCheck[ResultCode.GetCode()]) )              //This checks for operating system level key repititions and skips adding them
                        { _Data->AddMetaCodesToManualCheck( FromSDLEvent->AddCode(ResultCode), internal::EventManagerInternalData::Keypress); }
                    break; }

                case SDL_MOUSEBUTTONDOWN:   case SDL_JOYBUTTONDOWN:
                    _Data->AddMetaCodesToManualCheck( FromSDLEvent->AddCode(FromSDLRaw), internal::EventManagerInternalData::Keypress);
                    break;

                case SDL_MOUSEMOTION:       case SDL_JOYAXISMOTION:     case SDL_JOYHATMOTION:      case SDL_JOYBALLMOTION: // What is a joyball, like the bowling games maybe, or the ORBit controller
                    FromSDLEvent->AddCodesFromRawEvent(FromSDLRaw);
                    break;

                case SDL_WINDOWEVENT: {
                    EventGameWindow* React = new EventGameWindow(FromSDLRaw);
                    if(EventGameWindow::GAME_WINDOW_FOCUS_LOST==React->GetEventID())        //we dropp all keypresses when windows are switched
                        { ClearKeyPresses = true; }
                    this->AddEvent(React);
                    break; }

                case SDL_SYSWMEVENT:
                        // call a function with ifdefs here
                    break;

        // Error conditions
                case SDL_FIRSTEVENT:  //capture and ignore or throw error
                    World::GetWorldPointer()->LogAndThrow("Unexpected 'FIRSTEVENT' event in event manager. User input seems corrupted.");
                    break;

                case SDL_QUIT:          //when SDL closes, but this really should be handled somewhere else, like the UpdateQuitEvents() function
                    World::GetWorldPointer()->LogAndThrow("Unexpected Quit event in event manager.");
                    break;

                default:                //Never thrown by SDL, but could be added by a user
                    //World::GetWorldPointer()->LogAndThrow("Unknown SDL Event Inserted.");
                    World::GetWorldPointer()->Log("Unknown SDL Event Inserted. Likely an unhandled SDL 1.3 event");
                    break;
            }
        }

        if(ClearKeyPresses)
            { this->_Data->DropAllKeyPresses(); }

        #ifdef PHYSDEBUG
        World::GetWorldPointer()->Log("User Input entered this Frame");
        for(EventUserInput::iterator LIter=FromSDLEvent->begin(); FromSDLEvent->end()!=LIter; ++LIter)
        {
            World::GetWorldPointer()->Log(*LIter);
        }
        World::GetWorldPointer()->Log("End Of User Input entered this Frame");
        #endif

        //Check to see if we should add a User input event or not. We wouldn't want to pass an empty event
        if(FromSDLEvent->GetMetaCodeCount()==0)
        {
            delete FromSDLEvent;
        }else{
            _Data->EventQ.push_back(FromSDLEvent);
        }
    }

    void EventManager::UpdateQuitEvents()
    {
//        if (NULL == SDL_GetEventFilter())                       //Verify the Event filter is installed, if not, then install it.
        if(SDL_FALSE == SDL_GetEventFilter(0, 0))
        {
            SDL_SetEventFilter( internal::PhysSDLFilter, 0);
        }else{
            if(4==internal::PhysSDLFilter(0,0))                   //Pass it a null pointer to get it to "Not Callback Mode"
            {
                this->AddEvent(new EventQuit());                //We need to make a quit event
            }else{
                //all clear
            }
        }
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
                { _Data->EventQ.erase(Iter); }
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
    // Filtered management functions - Collision Events
    ///////////////////////////////////////
    EventCollision* EventManager::GetNextCollisionEvent()
        { return dynamic_cast<EventCollision*> (this->GetNextSpecificEvent(EventBase::Collision)); }

    EventCollision* EventManager::PopNextCollisionEvent()
        { return dynamic_cast<EventCollision*> (this->PopNextSpecificEvent(EventBase::Collision)); }

    void EventManager::RemoveNextCollisionEvent()
        { this->RemoveNextSpecificEvent(EventBase::Collision); }

    std::list<EventCollision*>* EventManager::GetAllCollisionEvents()
        { return (std::list<EventCollision*>*)this->GetAllSpecificEvents(EventBase::Collision); }


    ///////////////////////////////////////////////////////////////////////////////
    // Filtered management functions - GameWindow Events
    ///////////////////////////////////////
    EventGameWindow* EventManager::GetNextGameWindowEvent()
        { return dynamic_cast<EventGameWindow*> (this->GetNextSpecificEvent(EventBase::GameWindow)); }

    EventGameWindow* EventManager::PopNextGameWindowEvent()
        { return dynamic_cast<EventGameWindow*> (this->PopNextSpecificEvent(EventBase::GameWindow)); }

    void EventManager::RemoveNextGameWindowEvent()
        { this->RemoveNextSpecificEvent(EventBase::GameWindow); }

    std::list<EventGameWindow*>* EventManager::GetAllGameWindowEvents()
        { return (std::list<EventGameWindow*>*)this->GetAllSpecificEvents(EventBase::GameWindow); }

    ///////////////////////////////////////////////////////////////////////////////
    // Filtered management functions - RenderTime Events
    ///////////////////////////////////////
    EventRenderTime* EventManager::GetNextRenderTimeEvent()
        { return dynamic_cast<EventRenderTime*> (this->GetNextSpecificEvent(EventBase::RenderTime)); }

    EventRenderTime* EventManager::PopNextRenderTimeEvent()
        { return dynamic_cast<EventRenderTime*> (this->PopNextSpecificEvent(EventBase::RenderTime)); }

    void EventManager::RemoveNextRenderTimeEvent()
        { this->RemoveNextSpecificEvent(EventBase::RenderTime); }

    std::list<EventRenderTime*>* EventManager::GetAllRenderTimeEvents()
        { return (std::list<EventRenderTime*>*)this->GetAllSpecificEvents(EventBase::RenderTime); }

    ///////////////////////////////////////////////////////////////////////////////
    // Filtered management functions - User Input Events
    ///////////////////////////////////////
    EventUserInput* EventManager::GetNextUserInputEvent()
        { return dynamic_cast<EventUserInput*> (this->GetNextSpecificEvent(EventBase::UserInput)); }

    EventUserInput* EventManager::PopNextUserInputEvent()
        { return dynamic_cast<EventUserInput*> (this->PopNextSpecificEvent(EventBase::UserInput)); }

    void EventManager::RemoveNextUserInputEvent()
        { this->RemoveNextSpecificEvent(EventBase::UserInput); }

    std::list<EventUserInput*>* EventManager::GetAllUserInputEvents()
        { return (std::list<EventUserInput*>*)this->GetAllSpecificEvents(EventBase::UserInput); }

    ///////////////////////////////////////////////////////////////////////////////
    // Filtered management functions - Quit Event
    ///////////////////////////////////////

    EventQuit* EventManager::GetNextQuitEvent()
        { return dynamic_cast<EventQuit*> (this->GetNextSpecificEvent(EventBase::QuitMessage)); }

    EventQuit* EventManager::PopNextQuitEvent()
        { return dynamic_cast<EventQuit*> (this->PopNextSpecificEvent(EventBase::QuitMessage)); }

    void EventManager::RemoveNextQuitEvent()
        { this->RemoveNextSpecificEvent(EventBase::QuitMessage); }

    std::list<EventQuit*>* EventManager::GetAllQuitEvents()
        { return (std::list<EventQuit*>*)this->GetAllSpecificEvents(EventBase::QuitMessage); }

    ///////////////////////////////////////////////////////////////////////////////
    // Polling Functions
    ///////////////////////////////////////
    void EventManager::AddPollingCheck(const MetaCode &InputToTryPolling)
    {
        if(InputToTryPolling.IsPollable())
        {
            this->_Data->AddInputCodeToManualCheck(InputToTryPolling.GetCode(), internal::EventManagerInternalData::Polling);
        }else{
            World::GetWorldPointer()->LogAndThrow("Unsupported Polling Check on this Platform");
        }
    }

    void EventManager::RemovePollingCheck(const MetaCode &InputToStopPolling)
    {
        this->_Data->RemoveInputCodeToManualCheck(InputToStopPolling.GetCode(), internal::EventManagerInternalData::Polling);
    }

/*    void EventManager::StartRelativeMouseMode()
    {
        if(SDL_SetRelativeMouseMode (SDL_TRUE))
        {
            //it failed
//            World::GetWorldPointer()->LogAndThrow("Failed to Grab Mouse");
        }
    }

    void EventManager::EndRelativeMouseMode()
    {
        if(SDL_SetRelativeMouseMode (SDL_FALSE))
        {
            //it failed
            World::GetWorldPointer()->LogAndThrow("Failed to Release Mouse");
        }
    }
*/

    //Inherited From ManagerBase
    void EventManager::Initialize()
        {}

    void EventManager::DoMainLoopItems()
    {
        this->UpdateEvents();
        InputQueryTool::GatherEvents();
    }

    ManagerBase::ManagerTypeName EventManager::GetType() const
        { return ManagerBase::EventManager; }

} // /phys


///////////////////////////////////////////////////////////////////////////////
// Class External << Operators for streaming or assignment
#ifdef PHYSXML
std::ostream& operator << (std::ostream& stream, const phys::EventManager& Mgr)
{
    stream << "<EventManager Version=\"1\">";
    for(std::list<phys::EventBase*>::iterator Iter = Mgr._Data->EventQ.begin(); Iter!=Mgr._Data->EventQ.end(); ++Iter)
        { stream << **Iter; }
    for(phys::internal::EventManagerInternalData::ManualCheckIterator Iter=Mgr._Data->ManualCheck.begin(); Iter!=Mgr._Data->ManualCheck.end(); ++Iter)
        { stream << "<ManualCheck Version=\"1\" PollingType=\"" << Iter->second << "\" InputCode=\"" << Iter->first << "\" />"; }
    stream << "</EventManager>";

    return stream;
}

std::istream& PHYS_LIB operator >> (std::istream& stream, phys::EventManager& Mgr)
{
    phys::String OneTag( phys::xml::GetOneTag(stream) );
    std::auto_ptr<phys::xml::Document> Doc( phys::xml::PreParseClassFromSingleTag("phys::", "EventManager", OneTag) );

    Doc->GetFirstChild() >> Mgr;

    return stream;
}

void operator >> (const phys::xml::Node& OneNode, phys::EventManager& Mgr)
{
    if(phys::String(OneNode.Name()) == phys::String("EventManager"))
    {
        if (OneNode.GetAttribute("Version").AsInt() == 1)
        {
            phys::xml::Node Child = OneNode.GetFirstChild();
            while(Child)
            {
                phys::String TagName(Child.Name());
                if(TagName.length()>6)                  // I am pretty sure that the easiest wat to identify an event is by looking at the 6th
                {                                       // Character and seeing what the actual name of the event is. So that is what this does.
                    switch(TagName[5])
                    {
                        case 'l':{
                            Mgr._Data->AddInputCodesToManualCheck(
                                (Child.GetAttribute("InputCode").AsInt()),
                                (Child.GetAttribute("PollingType").AsInt())
                            );}
                            break;
                        case 'C':{
                            phys::EventCollision *temp = new phys::EventCollision();
                            Child >> *temp;
                            Mgr.AddEvent(temp); }
                            break;
                        case 'G':{
                            phys::EventGameWindow *temp = new phys::EventGameWindow();
                            Child >> *temp;
                            Mgr.AddEvent(temp); }
                            break;
                        case 'Q':{
                            phys::EventQuit *temp = new phys::EventQuit();
                            Child >> *temp;
                            Mgr.AddEvent(temp); }
                            break;
                        case 'R':{
                            phys::EventRenderTime *temp = new phys::EventRenderTime();
                            Child >> *temp;
                            Mgr.AddEvent(temp); }
                            break;
                        case 'U':{
                            phys::EventUserInput *temp = new phys::EventUserInput();
                            Child >> *temp;
                            Mgr.AddEvent(temp); }
                            break;
                        case 'O':
                            throw phys::Exception ("Attemping to serialize a phys::Event::Other... not sure what you are trying to serialize.");
                            //throw Exception ("How did you instantiate an abstract base class?! Attemping to serialize a phys::EventBase");
                            break;
                        default:
                            throw phys::Exception ("Attemping to serialize a phys::Event... not sure what you are trying to serialize.");
                            break;
                    }
                }else{
                    throw phys::Exception ("Invalid event, name is not long enough to identify event.");
                } // end if name length
                Child = Child.GetNextSibling();
            } // end while
        }else{
            throw( phys::Exception("Incompatible XML Version for EventManager: Not Version 1"));
        } // if version
    }else{
        throw( phys::Exception("Attempting to deserialize an EventManager, event mananger not found.") );
    }// if event
}
#endif // \PHYSXML





#endif
