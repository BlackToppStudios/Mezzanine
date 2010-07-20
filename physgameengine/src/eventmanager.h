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
#ifndef _physeventmanager_h
#define _physeventmanager_h
///////////////////////////////////////////////////////////////////////////////
//This will capture all event and store them for easy dispatching.
//There will be an instance of this class in the world.
///////////////////////////////////////

#include <list>
#include <queue>

using namespace std;

#include "eventbase.h"
#include "eventrendertime.h"
#include "eventuserinput.h"
#include "eventquit.h"

namespace phys
{
    class World;

    ///////////////////////////////////////////////////////////////////////////////
    /// @class EventManager
    /// @headerfile eventmanager.h
    /// @brief This is a container for Events and facilitates the transfer of data.
    /// @details The Event Manager Exists to passed important information about
    /// Gamestate from where it is generated to where it is needed. It is the Game
    /// Developers option whether they want to grab events directly using the get
    /// functions that have filters, or if they want to get all the events at once
    /// from a central location and dispatch form there. \n
    /// Since all User input comes in the form of events, this is also where user
    /// input Polling and optional input sources like Joysticks are controlled
    /// from. \n
    /// All of these event are stored in an internal Queue and order is preserved.
    /// So the First item In will be the First Out (FIFO). This is not strictly a
    /// FIFO buffer, there are a number of functions for getting of managing
    /// specific kinds of events. Generally these 'Filtered' management functions
    /// Still return the first of those kinds of event.
    /// @warning Delete pointers you get from this. Anything can create events and
    /// Put them here, and anything can get them out, This means the simple way to
    /// not cause memory leaks is to have the routines extracting the events delete
    /// the events.
    /// @warning Currently this is not thread safe, even though it should be.
    ///////////////////////////////////////////////////////////////////////////////
    class EventManager: ManagerBase
    {
        private:
            //The Default Physics worlds that this Eventmanager is expected to interact with
            //World* ParentWorld;

            //The Queue that all the events get stored in
            std::list<EventBase*> EventQueue;

            //Checks for quit messages and adds them to the queue
            void UpdateQuitEvents();

            // A list of the Mouse buttons being watched
            vector<int> WatchMouseKeys;

            //a List of the Keyboard keys being watch
            vector<MetaCode::InputCode> WatchKeyboardKeys;

            //These are use to decide if mouse location should be polled.
            bool PollMouseHor;
            bool PollMouseVert;

            //These does the heavy lifting during the polling operation
            //All of these poll the input subsystem and add MetaCodes to the vector they are passed.
            void PollKeyboard(vector<MetaCode> &CodeBag);
            void PollMouseButtons(vector<MetaCode> &CodeBag);
            void PollMouseLocation(vector<MetaCode> &CodeBag);

            //SDL event specific Items
            //This function will get all the events from SDL and Sort them into one of two Queues
            void PreProcessSDLEvents();
            queue<RawEvent*> SDL_WmEvents;
            queue<RawEvent*> SDL_UserInputEvents;

        public:
            /// @todo TODO build a deconstructor that deletes all the events still in the queue

            /// @brief Default constructor
            /// @param ParentWorld_ A pointer to the world that this world is working with Primarily
            /// @details This creates an empty PhysEventManger
            EventManager(World* ParentWorld_);

        ///////////////////////////////////////////////////////////////////////////////
        // Management functions - Work with all events
        ///////////////////////////////////////
            /// @brief Gets a count of events
            /// @details This returns a total count of all events stored in this PhysEventManager.
            /// @return This returns an unsigned integer with the amount of of total events
            unsigned int GetRemainingEventCount();

            /// @brief Return a pointer to the Next event
            /// @details This returns a pointer to the next PhysEvent. It is advisable to use this for performance reasons
            /// because it runs in constant time. However it does not return a specific kind of event, and must be cast
            /// in order to use the true content. This returns a pointer to 0 if there are no events in the que.
            /// @return A pointer to a PhysEvent, that still needs to be removed from the event manager and deleted.
            EventBase* GetNextEvent();

            /// @brief Return a pointer to the Next event, and removes the Event from storage
            /// @details This functions just like GetNextEvent , except that it also removes the item from the internal storage
            /// of the PhysEventManager. This returns a pointer to 0 if there are no events in the que.
            /// @return A pointer to a PhysEvent, that will need to be deleted once it has been used.
            EventBase* PopNextEvent();

            /// @brief Removes an Event From the que without looking at it.
            /// @details This together with GetNextEvent() are the same as call PopNextEvent().
            /// @warning If you did not call GetNextEvent() and haven't deleted or stored, or somehow dealt with this pointer, then this is a memory leak.
            /// Don't use this unless you are certain you have taken care of the pointer appropriately
            /// @exception This can throw any STL exception a que could. Any with likely throw some kind of except if called when there are no Events in the Que.
            void RemoveNextEvent();

            /// @brief Adds an event of any kind to the end of the Event Queue
            /// @param EventToAdd This is a pointer to an Event.
            /// @details This adds the existing event to the Queue. Be careful this is not delete, and does not go out of scope. Deleting the Event is now the
            /// responsibilty of the code that pulls it out of Event Manager.
            void AddEvent(EventBase* EventToAdd);

            /// @brief Pulls Events from the all the subsystems for use in the EventManager.
            /// @details The work this function does is already performed in the main loop. This only really needs to be used
            /// If a game developer chooses to use his own main loop and does't use UpdateSystemEvents() or UpdateUserInputEvents()
            void UpdateEvents();

            /// @brief Pulls System and Window manager Events from the all the subsystems for use in the EventManager.
            /// @details This adds events, like EventQuit and Other Windows manager events. The work this function does is already performed
            /// in the main loop. This only really needs to be used If a game developer chooses to use his own main loop and does't use UpdateEvents()
            /// and plans on using UpdateUserInputEvents()
            void UpdateSystemEvents();

            /// @brief Pulls User Interface Events from the all the subsystems for use in the EventManager.
            /// @details This adds events, like EventUserInput to the eventmanager. The work this function does is already performed
            /// in the main loop. This only really needs to be used If a game developer chooses to use his own main loop and does't use UpdateEvents()
            /// and plans on using UpdateSystemEvents()
            void UpdateUserInputEvents();

            /// @brief This returns a complete list of all events in the event manager
            /// @details This simply returns a const pointer of the internal event queue.
            /// @warning The pointers contained in this list must be used carefully. Do not delete them, this will cause errors.
            /// @return This returns a const pointer the list<EventBase*> which is this classes event pointer list. Use this carefully, even though it is a const pointer it is still possible to mess around with internals in an innapropriate way. Treat this as if it were read only.
            const std::list<EventBase*>* GetAllEvents() const;

        ///////////////////////////////////////////////////////////////////////////////
        // Filtered management functions - RenderTime Events
        ///////////////////////////////////////
            /// @brief Returns a pointer to the Next Rendertime event
            /// @details This Filtered event management function returns a pointer to the next Rendertime event. It is inadvisable to use
            /// this for performance reasons becuase it runs in linear time relative to the amount of events. However, it will return an immediately
            /// usable pointer for case where an extreme level of performance is not required. This returns a pointer to 0 if there are no rendertime
            /// events in the que.
            /// @return A pointer to a PhysEventRenderTime, that still needs to be removed from the event manager and deleted.
            EventRenderTime* GetNextRenderTimeEvent();

            /// @brief Returns a pointer to the Next Rendertime event and removes it from the Que
            /// @details This Filtered event management function returns a pointer to the next Rendertime event. It is inadvisable to use
            /// this for performance reasons becuase it runs in linear time relative to the amount of events. However, it will return an immediately
            /// usable pointer for case where an extreme level of performance is not required. This returns a pointer to 0 if there are no rendertime
            /// events in the que. This also removes the returned pointer form the Que.
            /// @return A pointer to a PhysEventRenderTime, that still needs to be removed from the event manager and deleted.
            EventRenderTime* PopNextRenderTimeEvent();

            /// @brief Removes the First Rendertime Event From the que without looking at it.
            /// @details This together with GetNextRenderTimeEvent() are the pretty much same as call PopNextRenderTimeEvent().
            /// @warning If you did not call GetNextRenderTimeEvent() and haven't deleted or stored, or somehow dealt with this pointer, then this is a memory leak.
            /// Don't use this unless you are certain you have taken care of the pointer appropriately
            /// @exception This can throw any STL exception a queue could. And with likely throw some kind of except if called when there are no Events in the Que.
            void RemoveNextRenderTimeEvent();

            /// @brief This returns a complete list of all the Render Time events.
            /// @details This finds all the EventUserInput Events then creates a new list and returns that. This runs in linear time relative to the amounts of events.
            /// @return This returns a list<EventRenderTime*> pointer which is this a subset of this classes event pointer list. Use this carefully, it can cause errors if used improperly. Additionally this list pointer must be deleted, but not the events in it.
            std::list<EventRenderTime*>* GetAllRenderTimeEvents();

        ///////////////////////////////////////////////////////////////////////////////
        // Filtered management functions - User Input Events
        ///////////////////////////////////////
            /// @brief Returns a pointer to the Next User Input event
            /// @details This Filtered event management function returns a pointer to the next User Input event. It is inadvisable to use
            /// this for performance reasons becuase it runs in linear time relative to the amount of events. However, it will return an immediately
            /// usable pointer for case where an extreme level of performance is not required. This returns a pointer to 0 if there are no User Input
            /// events in the que.
            /// @return A pointer to a EventUserInput, that still needs to be removed from the event manager and deleted.
            EventUserInput* GetNextUserInputEvent();

            /// @brief Returns a pointer to the Next User Input event and removes it from the Que
            /// @details This Filtered event management function returns a pointer to the next User Input event. It is inadvisable to use
            /// this for performance reasons becuase it runs in linear time relative to the amount of events. However, it will return an immediately
            /// usable pointer for case where an extreme level of performance is not required. This returns a pointer to 0 if there are no User Input
            /// events in the que. This also removes the returned pointer form the Que.
            /// @return A pointer to a EventUserInput, that still needs to be removed from the event manager and deleted.
            EventUserInput* PopNextUserInputEvent();

            /// @brief Removes the First User Input Event From the que without looking at it.
            /// @details This together with GetNextUserInputEvent() are the pretty much same as call PopNextUserInputEvent().
            /// @warning If you did not call GetNextUserInputEvent() and haven't deleted or stored, or somehow dealt with this pointer, then this is a memory leak.
            /// Don't use this unless you are certain you have taken care of the pointer appropriately
            /// @exception This can throw any STL exception a queue could. And with likely throw some kind of except if called when there are no Events in the Que.
            void RemoveNextUserInputEvent();

            /// @brief This returns a complete list of all the User Input events.
            /// @details This finds all the EventUserInput Events then creates a new list and returns that. This runs in linear time relative to the amounts of events.
            /// @return This returns a std::list<EventUserInput*> pointer which is this a subset of this classes event pointer list. Use this carefully, it can cause errors if used improperly. Additionally this list pointer must be deleted, but not the events in it.
            std::list<EventUserInput*>* GetAllUserInputEvents();

        ///////////////////////////////////////////////////////////////////////////////
        // Filtered management functions - Quit Events
        ///////////////////////////////////////
            /// @brief Returns a pointer to the Next EventQuit
            /// @details This Filtered event management function returns a pointer to the next EventQuit. It is inadvisable to use
            /// this for performance reasons becuase it runs in linear time relative to the amount of events. However, it will return an immediately
            /// usable pointer for case where an extreme level of performance is not required. This returns a pointer to 0 if there are no EventQuit
            /// events in the que.
            /// @return A pointer to a EventQuit, that still needs to be removed from the event manager and deleted.
            EventQuit* GetNextQuitEvent();

            /// @brief Returns a pointer to the Next EventQuit and removes it from the Que.
            /// @details This Filtered event management function returns a pointer to the next EventQuit. It is inadvisable to use
            /// this for performance reasons becuase it runs in linear time relative to the amount of events. However, it will return an immediately
            /// usable pointer for case where an extreme level of performance is not required. This returns a pointer to 0 if there are no EventQuit
            /// events in the que. This also removes the returned pointer form the Que.
            /// @return A pointer to a EventQuit, that still needs to be removed from the event manager and deleted.
            EventQuit* PopNextQuitEvent();

            /// @brief Removes the First EventQuit From the que without looking at it.
            /// @details This together with GetNextQuitEvent() are the pretty much same as call PopNextQuitEvent().
            /// @warning If you did not call GetNextQuitEvent() and haven't deleted or stored, or somehow dealt with this pointer, then this is a memory leak.
            /// Don't use this unless you are certain you have taken care of the pointer appropriately
            /// @exception This can throw any STL exception a queue could. And with likely throw some kind of except if called when there are no Events in the Que.
            void RemoveNextQuitEvent();

            /// @brief This returns a complete list of all the quit events.
            /// @details This finds all the EventQuit Events then creates a new list and returns that. This runs in linear time relative to the amounts of events.
            /// @warning Something is wrong if you have more than a few quit events. These should be checked for in each iteration of the main loop.
            /// @return This returns a std::list<EventQuit*> pointer which is this a subset of this classes event pointer list. Use this carefully, it can cause errors if used improperly. Additionally this list pointer must be deleted, but not the events in it.
            std::list<EventQuit*>* GetAllQuitEvents();

        ///////////////////////////////////////////////////////////////////////////////
        // Filtered management functions - You choose YAYYYY!!!
        ///////////////////////////////////////
            /// @brief Returns a pointer to the Next kind event of the Specified type
            /// @param SpecificType This is a PhysEvent::EventType that defines the type you want this to work with
            /// @details This and the other NextSpecificEvent functions are the core of the Event Filtering System. In general the other filtering
            /// functions call one of these and does very little work on their own. \n This performs a linear search starting with the oldest (first entered
            /// Events) and simply checks if it the of the correct type. Then this returns a pointer to the next event of the specified type, or returns
            /// a pointer to 0 if there are none of the correct pointers in the Que. It is inadvisable to use
            /// this for performance reasons becuase it runs in linear time relative to the amount of events.
            /// @return A pointer to a EventUserInput, that still needs to be removed from the event manager and deleted.
            EventBase* GetNextSpecificEvent(EventBase::EventType SpecificType);

            /// @brief Returns a pointer to the Next kind event of the Specified type, and removes it from the Que
            /// @param SpecificType This is a PhysEvent::EventType that defines the type you want this to work with
            /// @details This is just like GetNextSpecificEvent(PhysEvent::EventType SpecificType) but it also removes the item from the Que.
            /// @return A pointer to a EventUserInput, that still needs to be removed from the event manager and deleted.
            EventBase* PopNextSpecificEvent(EventBase::EventType SpecificType);

            /// @brief Returns a pointer to the Next kind event of the Specified type, and removes it from the Que
            /// @param SpecificType This is a PhysEvent::EventType that defines the type you want this to work with
            /// @details This is just like PopNextSpecificEvent(PhysEvent::EventType SpecificType) but exept it doesn't bother with any of the needed
            /// structure involved with returning data, and just removes the specific event from the Queue.
            /// @warning If you did not call GetNextSpecificEvent(PhysEvent::EventType SpecificType) and haven't deleted or stored, or somehow dealt with
            /// this pointer, then this is a memory leak. Don't use this unless you are certain you have taken care of the pointer appropriately.
            /// @exception This can throw any STL exception a queue could. And with likely throw some kind of except if called when there are no Events in the Que.
            void RemoveNextSpecificEvent(EventBase::EventType SpecificType);

            /// @brief This returns a complete list of all the specified events.
            /// @details This finds all the events that are of the specified type in the event manager, then creates a new list
            /// and return that. This runs in linear time relative to the amounts of events.
            /// @warning The pointers contained in this list must be used carefully. Do not delete them, this will cause errors.
            /// @return This returns a std::list<EventBase*> pointer which is this a subset of this classes event pointer list. Use this carefully, it can cause errors if used improperly. Additionally this list pointer must be deleted, but not the events in it.
            std::list<EventBase*>* GetAllSpecificEvents(EventBase::EventType SpecificType);

            /// @brief This removes all the events of the specified type.
            /// @details This finds all the events that are of the specified type in the event manager, then removes them.
            /// @warning This does not delete the events. This is a memory leak unless used with GetAllSpecificEvents so that the events can be tracked indeendantly, and deleted.
            void RemoveAllSpecificEvents(EventBase::EventType SpecificType);

        ///////////////////////////////////////////////////////////////////////////////
        // Polling management functions
        ///////////////////////////////////////
            /// @brief Generates extra events each iteration of the main loop, based on user input polling
            /// @param InputToTryPolling This accepts a MetaCode and will try to watch for occurences like this one
            /// @details This will trigger the input system to generate an event (or add to an exiting event) when polling for the given kind of event. Each
            /// Iteration of the main loop there will be a EventUserInput that created. That Event will Include all the normal metacodes for user input
            /// that happened, and it will also have a meta code for each time this function was called. The added metacode may be partialky ignored, the
            /// Metavalue is almost always ignored, and in a situation where the can only be one of a given input on a system, the ID is ignore and 0 is assumed.
            /// @exception "Unsupported Polling Check on this Platform" When the metacode passed cannot be polled on this platform
            void AddPollingCheck(const MetaCode &InputToTryPolling);

            /// @brief Removes Events from the list(s) of what needs to be polled
            /// @param InputToStopPolling This accepts a MetaCode and will try to Remove Watches like this one
            /// @details This will remove any check for polling that share the same inputcode and ID. This
            /// @exception "Polling check not present" Is thrown
            void RemovePollingCheck(const MetaCode &InputToStopPolling);

            /// @brief This activates the polling routines of the user input subsystems
            /// @details This checks the current state of user input devices that have been added by AddPollingCheck(const MetaCode &InputToTryPolling).
            /// This is called automatically by main loop processing, but there is no harm in calling it several times.
            /// @return This returns a pointer to a EventUserInput that contains the desired metacodes
            EventUserInput* PollForUserInputEvents();

        ///////////////////////////////////////////////////////////////////////////////
        // Inherited From ManagerBase
        ///////////////////////////////////////
            /// @brief Empty Initializor
            /// @details This specific initializor is unneeded, but we implement it for compatibility. It also exists
            /// in case a derived class wants to override it for some reason
            virtual void Initialize();

            /// @brief This returns the type of this manager.
            /// @return This returns ManagerTypeName::EventManager
            virtual ManagerTypeName GetType() const;

    };
}
#endif
