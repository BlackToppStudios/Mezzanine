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
#ifndef _physeventmanager_h
#define _physeventmanager_h
///////////////////////////////////////////////////////////////////////////////
//This will capture all event and store them for easy dispatching.
//
//There will be an instance of this class in the physworld.
///////////////////////////////////////
#include <list>

using namespace std;

#include "physevent.h"
#include "physeventrendertime.h"
#include "physeventuserinput.h"

///////////////////////////////////////////////////////////////////////////////
/// @class PhysEventManager
/// @headerfile physeventmanager.h
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
/// @warning Currently thi is not thread safe, even though it should be.
///////////////////////////////////////////////////////////////////////////////
class PhysEventManager
{
	private:
		list<PhysEvent*> EventQueue;
        static bool IgnoreSDLQuitEvents;

        // A list of the Mouse buttons being watched
        vector<int> WatchMouseKeys;

        //a List of the
        vector<int> WatchKeyboardKeys;

	public:
        /// @brief Default constructor
        /// @details This creates an empty PhysEventManger
		PhysEventManager();

        //These functions will give you the next event or help you manage the events
        //Whenever and event is gotten it is removed form the event queue
        //any getfunction cannot find an appropriate event it returns a pointer to 0

        /// @brief Gets a count of events
        /// @details This returns a total count of all events stored in this PhysEventManager.
		/// @return This returns an unsigned integer with the amount of of total events
        unsigned int GetRemainingEventCount();

        /// @brief Return a pointer to the Next event
        /// @details This returns a pointer to the next PhysEvent. It is advisable to use this for performance reasons
        /// because it runs in constant time. However it does not return a specific kind of event, and must be cast
        /// in order to use the true content. This returns a pointer to 0 if there are no events in the que.
        /// @return A pointer to a PhysEvent, that still needs to be removed from the event manager and deleted.
		PhysEvent* GetNextEvent();

        /// @brief Return a pointer to the Next event, and removes the Event from storage
        /// @details This functions just like GetNextEvent , except that it also removes the item from the internal storage
        /// of the PhysEventManager. This returns a pointer to 0 if there are no events in the que.
        /// @return A pointer to a PhysEvent, that will need to be deleted once it has been used.
        PhysEvent* PopNextEvent();

        /// @brief Removes an Event From the que without looking at it.
        /// @details This together with GetNextEvent() are the same as call PopNextEvent().
        /// @warning If you did not call GetNextEvent() and haven't delete or stored, or somehow dealt with this pointer, then this is a memory leak.
        /// Don't use this unless you are certain you have taken care of the pointer appropriately
        /// @exception This can throw any STL exception a que could. Any with likely throw some kind of except if called when there are no Events in the Que.
        void RemoveNextEvent();

        ///////////////////////////////////////////////////////////////////////////////
        // Filtered management functions - RenderTime Events
        ///////////////////////////////////////
        /// @brief Returns a pointer to the Next Rendertime event
        /// @details This Filtered event management function returns a pointer to the next Rendertime event. It is inadvisable to use
        /// this for performance reasons becuase it runs in linear time relative to the amount of events. However, it will return an immediately
        /// usable pointer for case where an extreme level of performance is not required. This returns a pointer to 0 if there are no redertime
        /// events in the que.
        /// @return A pointer to a PhysEventRenderTime, that still needs to be removed from the event manager and deleted.
		PhysEventRenderTime* GetNextRenderTimeEvent();


        /// @brief Returns a pointer to the Next Rendertime event and removes it from the Que
        /// @details This Filtered event management function returns a pointer to the next Rendertime event. It is inadvisable to use
        /// this for performance reasons becuase it runs in linear time relative to the amount of events. However, it will return an immediately
        /// usable pointer for case where an extreme level of performance is not required. This returns a pointer to 0 if there are no redertime
        /// events in the que.
        /// @return A pointer to a PhysEventRenderTime, that still needs to be removed from the event manager and deleted.
		PhysEventRenderTime* PopNextRenderTimeEvent();

		/// @brief Removes the First Rendertime Event From the que without looking at it.
        /// @details This together with GetNextRenderTimeEvent() are the pretty much same as call PopNextRenderTimeEvent().
        /// @warning If you did not call GetNextRenderTimeEvent() and haven't delete or stored, or somehow dealt with this pointer, then this is a memory leak.
        /// Don't use this unless you are certain you have taken care of the pointer appropriately
        /// @exception This can throw any STL exception a que could. Any with likely throw some kind of except if called when there are no Events in the Que.
        void RemoveNextRenderTimeEvent();

        ///////////////////////////////////////////////////////////////////////////////
        // Filtered management functions - User Input Events
        ///////////////////////////////////////

        PhysEventUserInput* GetNextUserInputEvent();
        PhysEventUserInput* PopNextUserInputEvent();
        void RemoveNextUserInputEvent();

        //By and large the Game won't use this, but there is no reason it shouldn't
		void AddEvent(PhysEvent* EventToAdd);

        ///////////////////////////////////////////////////////////////////////////////
        // Filtered management functions - You choose YAYYYY!!!
        ///////////////////////////////////////
        PhysEvent* GetNextSpecificEvent();
        PhysEvent* PopNextSpecificEvent();
        PhysEvent* RemoveNextSpecificEvent();

        /// @brief Generates extra events each iteration of the main loop, based on user input polling
        /// @param InputToTryPolling By default this accepts a MetaCode and will try to watch for occurences like this one
        /// @details This will trigger the input system to generate an event (or add to an exiting event) when polling for the given kind of event. Each
        /// Iteration of the main loop there will be a PhysEventUserInput that created. That Event will Include all the normal metacodes for user input
        /// that happened, and it will also have a meta code for each time this function was called. The added metacode
        void AddPollingCheck(const MetaCode &InputToTryPolling);

        //different platforms treat exiting the application differently, to work around that
        //we can use this to help identify if it comes accross as a normal event, or if we must
        //quit without generating one
        bool DoQuitMessagesExist();
        static bool IgnoreQuitEvents();                //if true, don't exit
        static void SetIgnoreQuitEvents(bool Ignore);  //if false exit when x is clicked.

};

#endif
