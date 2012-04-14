//© Copyright 2010 - 2011 BlackTopp Studios Inc.
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
#ifndef _eventmanager_h
#define _eventmanager_h
///////////////////////////////////////////////////////////////////////////////
//This will capture all event and store them for easy dispatching.
//There will be an instance of this class in the world.
///////////////////////////////////////

#include "managerbase.h"
#include "metacode.h"
#include "eventbase.h"
#include "singleton.h"
#include "vector2.h"
#include "xml.h"

///////////////////////////////////////////////////////////////////////////////
// Class External << Operators for streaming or assignment
#ifdef MEZZXML
namespace Mezzanine
{
    class EventManager; //forward declaration so we can use this in our << and >> operators
}

/// @brief Serializes the passed Mezzanine::EventManager to XML
/// @param stream The ostream to send the xml to.
/// @param Mgr the Mezzanine::EventManager to be serialized
/// @return this returns the ostream, now with the serialized data
std::ostream& MEZZ_LIB operator << (std::ostream& stream, const Mezzanine::EventManager& Mgr);

/// @brief Deserialize a Mezzanine::EventManager
/// @param stream The istream to get the xml from to (re)make the Mezzanine::EventManager.
/// @param Mgr the Mezzanine::EventManager to be deserialized.
/// @return this returns the ostream, advanced past the Mezzanine::EventManager that was recreated onto Ev.
std::istream& MEZZ_LIB operator >> (std::istream& stream, Mezzanine::EventManager& Mgr);

/// @brief Set all values of a Mezzanine::EventManager from parsed xml.
/// @param OneNode The istream to get the xml from to (re)make the Mezzanine::EventManager.
/// @param Mgr the Mezzanine::EventManager to be reset.
/// @return This returns thexml::Node that was passed in.
void MEZZ_LIB operator >> (const Mezzanine::xml::Node& OneNode, Mezzanine::EventManager& Mgr);

#endif // \MEZZXML

namespace Mezzanine
{
    class World;
    class EventCollision;
    class EventGameWindow;
    class EventRenderTime;
    class EventUserInput;
    class EventQuit;

    namespace internal {
        class EventManagerInternalData;
    }

    // Used by the scripting language binder to help create bindgings for this class. SWIG does know to creation template instances
    #ifdef SWIG
    %template(SingletonEventManager) Singleton<EventManager>;
    #endif

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
    class MEZZ_LIB EventManager: public ManagerBase, public Singleton<EventManager>
    {
        private:
            /// @internal
            /// @brief All the internal data for this EventManager
            internal::EventManagerInternalData* _Data;

            /// @internal
            /// @brief Checks for quit messages and adds them to the queue
            void UpdateQuitEvents();

            /// @brief Private copy semantics, because copying this will cause problems.
            /// @param Dummy A dummy argument
            /// @details Since copying, or having more than one event manager doesn't seem to make sense
            /// I just made it non-copyable.
            EventManager(const EventManager& Dummy)
            {}

            #ifdef MEZZXML
            friend std::ostream& MEZZ_LIB ::operator << (std::ostream& stream, const Mezzanine::EventManager& Mgr);
            friend std::istream& MEZZ_LIB ::operator >> (std::istream& stream, Mezzanine::EventManager& Mgr);
            friend void MEZZ_LIB ::operator >> (const Mezzanine::xml::Node& OneNode, Mezzanine::EventManager& Mgr);
            #endif // \MEZZXML

        public:
            /// @brief Default constructor
            /// @details This creates an empty EventManger
            EventManager();

#ifdef MEZZXML
            /// @brief XML constructor.
            /// @param XMLNode The node of the xml document to construct from.
            EventManager(xml::Node& XMLNode);
#endif

            /// @brief Default Deconstructor
            /// @details This deletes everything still in the event manager and tears it down.
            ~EventManager();

            /// @brief Look for Joysticks that are hooked up to the system
            /// @details Currently this will only find the first joystick. This only needs to be done once after the joystick has been connected and
            /// detected/configured by the operating system. Joystick events may not be added if this has not been called. The is called once at
            /// when the Event manager is contructed, but if the joystick was not connected yet then it might not be found.
            void DetectJoysticks();

        ///////////////////////////////////////////////////////////////////////////////
        // Management functions - Work with all events
        ///////////////////////////////////////
            /// @brief Gets a count of events
            /// @details This returns a total count of all events stored in this EventManager.
            /// @return This returns an unsigned integer with the amount of of total events
            size_t GetRemainingEventCount();

            /// @brief Return a pointer to the Next event
            /// @details This returns a pointer to the next Event. It is advisable to use this for performance reasons
            /// because it runs in constant time. However it does not return a specific kind of event, and must be cast
            /// in order to use the true content. This returns a pointer to 0 if there are no events in the queue.
            /// @return A pointer to a Event, that still needs to be removed from the event manager and deleted.
            EventBase* GetNextEvent();

            /// @brief Return a pointer to the Next event, and removes the Event from storage
            /// @details This functions just like GetNextEvent , except that it also removes the item from the internal storage
            /// of the EventManager. This returns a pointer to 0 if there are no events in the que.
            /// @return A pointer to a Event, that will need to be deleted once it has been used.
            EventBase* PopNextEvent();

            /// @brief Removes an Event From the que without looking at it.
            /// @details This together with GetNextEvent() are the same as call PopNextEvent().
            /// @warning If you did not call GetNextEvent() and haven't deleted or stored, or somehow dealt with this pointer, then this is a memory leak.
            /// Don't use this unless you are certain you have taken care of the pointer appropriately
            /// @exception This can throw any STL exception a que could. Any with likely throw some kind of except if called when there are no Events in the Que.
            void RemoveNextEvent();

            /// @brief Adds an event of any kind to the end of the Event Queue.
            /// @param EventToAdd This is a pointer to an Event.
            /// @details This adds the existing event to the Queue. Be careful this is not delete, and does not go out of scope. Deleting the Event is now the
            /// responsibilty of the code that pulls it out of Event Manager.
            void AddEvent(EventBase* EventToAdd);

            /// @brief Removes an event of any kind from the Event Queue.
            /// @param EventToRemove Pointer to the event that will be removed.
            /// @details In most cases you will want to use the PopNextEvent() methods when going through events.  In some expert use cases however you may want
            /// to remove a specific event at an arbitrary place in the Queue.  This is the method for doing so.
            void RemoveEvent(EventBase* EventToRemove);

            /// @brief Pulls Events from the all the subsystems for use in the EventManager.
            /// @details The work this function does is already performed in the main loop. This only really needs to be used
            /// If a game developer chooses to use his own main loop. This adds system events, like EventQuit and Other Windows manager events,
            /// and if any user input event actions, this generates one EventUserInput that stores everythin that happened.
            void UpdateEvents();

        ///////////////////////////////////////////////////////////////////////////////
        // Filtered management functions - Collision Events
        ///////////////////////////////////////
            /// @brief Returns a pointer to the Next Collision event
            /// @details This Filtered event management function returns a pointer to the next Collision event. It is inadvisable to use
            /// this for performance reasons because it runs in linear time relative to the amount of events. However, it will return an immediately
            /// usable pointer for case where an extreme level of performance is not required. This returns a pointer to 0 if there are no Collision
            /// events in the queue.
            /// @return A pointer to a EventCollision, that still needs to be removed from the event manager and deleted.
            EventCollision* GetNextCollisionEvent();

            /// @brief Returns a pointer to the Next Collision event and removes it from the Que
            /// @details This Filtered event management function returns a pointer to the next Collision event. It is inadvisable to use
            /// this for performance reasons because it runs in linear time relative to the amount of events. However, it will return an immediately
            /// usable pointer for case where an extreme level of performance is not required. This returns a pointer to 0 if there are no Collision
            /// events in the que. This also removes the returned pointer form the Queue.
            /// @return A pointer to a EventRenderTime, that still needs to be removed from the event manager and deleted.
            EventCollision* PopNextCollisionEvent();

            /// @brief Removes the First Collision Event From the que without looking at it.
            /// @details This together with GetNextCollisionEvent() are the pretty much same as call PopNextCollisionEvent().
            /// @warning If you did not call GetNextCollisionEvent() and haven't deleted or stored, or somehow dealt with this pointer, then this is a memory leak.
            /// Don't use this unless you are certain you have taken care of the pointer appropriately.
            /// @exception This can throw any STL exception a queue could. And with likely throw some kind of except if called when there are no Events in the Que.
            void RemoveNextCollisionEvent();

            /// @brief This returns a complete list of all the Collision Events.
            /// @details This finds all the Collision Events then creates a new list and returns that. This runs in linear time relative to the amounts of events.
            /// @return This returns a list<EventCollision*> pointer which is this a subset of this classes event pointer list. Use this carefully, it can cause errors if used improperly. This list pointer must be deleted, but not the events in it.
            std::list<EventCollision*>* GetAllCollisionEvents();

        ///////////////////////////////////////////////////////////////////////////////
        // Filtered management functions - GameWindow Events
        ///////////////////////////////////////
            /// @brief Returns a pointer to the Next GameWindow event
            /// @details This Filtered event management function returns a pointer to the next GameWindow event. It is inadvisable to use
            /// this for performance reasons because it runs in linear time relative to the amount of events. However, it will return an immediately
            /// usable pointer for case where an extreme level of performance is not required. This returns a pointer to 0 if there are no GameWindow
            /// events in the que.
            /// @return A pointer to a EventGameWindow, that still needs to be removed from the event manager and deleted.
            EventGameWindow* GetNextGameWindowEvent();

            /// @brief Returns a pointer to the Next GameWindow event and removes it from the Que
            /// @details This Filtered event management function returns a pointer to the next GameWindow event. It is inadvisable to use
            /// this for performance reasons because it runs in linear time relative to the amount of events. However, it will return an immediately
            /// usable pointer for case where an extreme level of performance is not required. This returns a pointer to 0 if there are no GameWindow
            /// events in the que. This also removes the returned pointer form the Que.
            /// @return A pointer to a EventRenderTime, that still needs to be removed from the event manager and deleted.
            EventGameWindow* PopNextGameWindowEvent();

            /// @brief Removes the First GameWindow Event From the que without looking at it.
            /// @details This together with GetNextGameWindowEvent() are the pretty much same as call PopNextGameWindowEvent().
            /// @warning If you did not call GetNextGameWindowEvent() and haven't deleted or stored, or somehow dealt with this pointer, then this is a memory leak.
            /// Don't use this unless you are certain you have taken care of the pointer appropriately
            /// @exception This can throw any STL exception a queue could. And with likely throw some kind of except if called when there are no Events in the Que.
            void RemoveNextGameWindowEvent();

            /// @brief This returns a complete list of all the Render Time events.
            /// @details This finds all the EventUserInput Events then creates a new list and returns that. This runs in linear time relative to the amounts of events.
            /// @return This returns a list<EventGameWindow*> pointer which is this a subset of this classes event pointer list. Use this carefully, it can cause errors if used improperly. This list pointer must be deleted, but not the events in it.
            std::list<EventGameWindow*>* GetAllGameWindowEvents();

        ///////////////////////////////////////////////////////////////////////////////
        // Filtered management functions - RenderTime Events
        ///////////////////////////////////////
            /// @brief Returns a pointer to the Next Rendertime event
            /// @details This Filtered event management function returns a pointer to the next Rendertime event. It is inadvisable to use
            /// this for performance reasons because it runs in linear time relative to the amount of events. However, it will return an immediately
            /// usable pointer for case where an extreme level of performance is not required. This returns a pointer to 0 if there are no EventRenderTime
            /// events in the que.
            /// @return A pointer to a EventRenderTime, that still needs to be removed from the event manager and deleted.
            EventRenderTime* GetNextRenderTimeEvent();

            /// @brief Returns a pointer to the Next Rendertime event and removes it from the Que
            /// @details This Filtered event management function returns a pointer to the next Rendertime event. It is inadvisable to use
            /// this for performance reasons because it runs in linear time relative to the amount of events. However, it will return an immediately
            /// usable pointer for case where an extreme level of performance is not required. This returns a pointer to 0 if there are no rendertime
            /// events in the que. This also removes the returned pointer form the Que.
            /// @return A pointer to a EventRenderTime, that still needs to be removed from the event manager and deleted.
            EventRenderTime* PopNextRenderTimeEvent();

            /// @brief Removes the First Rendertime Event From the que without looking at it.
            /// @details This together with GetNextRenderTimeEvent() are the pretty much same as call PopNextRenderTimeEvent().
            /// @warning If you did not call GetNextRenderTimeEvent() and haven't deleted or stored, or somehow dealt with this pointer, then this is a memory leak.
            /// Don't use this unless you are certain you have taken care of the pointer appropriately
            /// @exception This can throw any STL exception a queue could. And with likely throw some kind of except if called when there are no Events in the Que.
            void RemoveNextRenderTimeEvent();

            /// @brief This returns a complete list of all the Render Time events.
            /// @details This finds all the EventRenderTime Events then creates a new list and returns that. This runs in linear time relative to the amounts of events.
            /// @return This returns a list<EventRenderTime*> pointer which is this a subset of this classes event pointer list. Use this carefully, it can cause errors if used improperly. Additionally this list pointer must be deleted, but not the events in it.
            std::list<EventRenderTime*>* GetAllRenderTimeEvents();

        ///////////////////////////////////////////////////////////////////////////////
        // Filtered management functions - User Input Events
        ///////////////////////////////////////
            /// @brief Returns a pointer to the Next User Input event
            /// @details This Filtered event management function returns a pointer to the next User Input event. It is inadvisable to use
            /// this for performance reasons because it runs in linear time relative to the amount of events. However, it will return an immediately
            /// usable pointer for case where an extreme level of performance is not required. This returns a pointer to 0 if there are no User Input
            /// events in the que.
            /// @return A pointer to a EventUserInput, that still needs to be removed from the event manager and deleted.
            EventUserInput* GetNextUserInputEvent();

            /// @brief Returns a pointer to the Next User Input event and removes it from the Que
            /// @details This Filtered event management function returns a pointer to the next User Input event. It is inadvisable to use
            /// this for performance reasons because it runs in linear time relative to the amount of events. However, it will return an immediately
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
            /// this for performance reasons because it runs in linear time relative to the amount of events. However, it will return an immediately
            /// usable pointer for case where an extreme level of performance is not required. This returns a pointer to 0 if there are no EventQuit
            /// events in the que.
            /// @return A pointer to a EventQuit, that still needs to be removed from the event manager and deleted.
            EventQuit* GetNextQuitEvent();

            /// @brief Returns a pointer to the Next EventQuit and removes it from the Que.
            /// @details This Filtered event management function returns a pointer to the next EventQuit. It is inadvisable to use
            /// this for performance reasons because it runs in linear time relative to the amount of events. However, it will return an immediately
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
            /// @param SpecificType This is a Event::EventType that defines the type you want this to work with
            /// @details This and the other NextSpecificEvent functions are the core of the Event Filtering System. In general the other filtering
            /// functions call one of these and does very little work on their own. \n This performs a linear search starting with the oldest (first entered
            /// Events) and simply checks if it the of the correct type. Then this returns a pointer to the next event of the specified type, or returns
            /// a pointer to 0 if there are none of the correct pointers in the Que. It is inadvisable to use
            /// this for performance reasons because it runs in linear time relative to the amount of events.
            /// @return A pointer to a EventUserInput, that still needs to be removed from the event manager and deleted.
            EventBase* GetNextSpecificEvent(EventBase::EventType SpecificType);

            /// @brief Returns a pointer to the Next kind event of the Specified type, and removes it from the Que
            /// @param SpecificType This is a Event::EventType that defines the type you want this to work with
            /// @details This is just like GetNextSpecificEvent(Event::EventType SpecificType) but it also removes the item from the Que.
            /// @return A pointer to a EventUserInput, that still needs to be removed from the event manager and deleted.
            EventBase* PopNextSpecificEvent(EventBase::EventType SpecificType);

            /// @brief Returns a pointer to the Next kind event of the Specified type, and removes it from the Que
            /// @param SpecificType This is a Event::EventType that defines the type you want this to work with
            /// @details This is just like PopNextSpecificEvent(Event::EventType SpecificType) but exept it doesn't bother with any of the needed
            /// structure involved with returning data, and just removes the specific event from the Queue.
            /// @warning If you did not call GetNextSpecificEvent(Event::EventType SpecificType) and haven't deleted or stored, or somehow dealt with
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

            /*/// @brief Should Hide The mouse Cursor and report on relative movements
            /// @details useful for games that will use the mouse to directly control a camera
            /// @warning Simply crashes.
            void StartRelativeMouseMode();

            /// @brief unhide the cursor and report mouse movements normally
            void EndRelativeMouseMode();*/

        ///////////////////////////////////////////////////////////////////////////////
        // Inherited From ManagerBase
        ///////////////////////////////////////
            /// @brief Empty Initializer
            /// @details This specific initializor is unneeded, but we implement it for compatibility. It also exists
            /// in case a derived class wants to override it for some reason
            virtual void Initialize();

            /// @brief Empty MainLoopItems
            /// @details This class implements this for the sake of entension and compatibility this function does nothing. This is just empty during this round of refactoring,
            /// and this will get all the functionality that currently should be here, but is in the world
            virtual void DoMainLoopItems();

            /// @brief This returns the type of this manager.
            /// @return This returns ManagerTypeName::EventManager
            virtual ManagerTypeName GetType() const;
    };
}

#endif
