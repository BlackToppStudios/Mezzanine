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
		PhysEventManager();

        //These functions will give you the next event or help you manage the events
        //Whenever and event is gotten it is removed form the event queue
        //any getfunction cannot find an appropriate event it returns a pointer to 0
        unsigned int GetRemainingEventCount();
		PhysEvent* GetNextEvent();

		//This gets the first/next available PhysEventRenderTime* in the Queue, then removes it.
		PhysEventRenderTime* GetNextRenderTimeEvent();
        PhysEventUserInput* GetNextUserInputEvent();

        //By and large the Game won't use this, but there is no reason it shouldn't
		void AddEvent(PhysEvent* EventToAdd);

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
