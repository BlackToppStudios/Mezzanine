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
#ifndef EVENTGAMEWINDOW_H
#define EVENTGAMEWINDOW_H
///////////////////////////////////////////////////////////////////////////////
// This will store the amount of time since rendering events that occured recently
///////////////////////////////////////

#include "datatypes.h"
#include "crossplatformexport.h"
#include "eventbase.h"

using namespace std;

namespace phys
{
    class EventGameWindowData;

    ///////////////////////////////////////////////////////////////////////////////
    /// @class EventGameWindow
    /// @headerfile eventgamewindow.h
    /// @brief This is intended to convey the message that quitting needs to happen
    /// @details
    class PHYS_LIB EventGameWindow : public EventBase
    {
        public:

            /// @brief Used to identify the kind of change that has happened to the game window.
            typedef enum
            {
                GAME_WINDOW_NONE,                       /**< Never used */
                GAME_WINDOW_FIRST = GAME_WINDOW_NONE,   /**< Used only as the lower bounds of this enumeration*/

                GAME_WINDOW_SHOWN,                      /**< Window has been shown */
                GAME_WINDOW_HIDDEN,                     /**< Window has been hidden */
                GAME_WINDOW_EXPOSED,                    /**< Window has been exposed and should be redrawn */

                GAME_WINDOW_MOVED,                      /**< Window has been moved to data1, data2 */

                GAME_WINDOW_RESIZED,                    /**< Window has been resized to data1xdata2 */
                GAME_WINDOW_SIZE_CHANGED,               /**< The window size has changed, either as a result of an API call or through the system or user changing the window size. */
                GAME_WINDOW_MINIMIZED,                  /**< Window has been minimized */
                GAME_WINDOW_MAXIMIZED,                  /**< Window has been maximized */
                GAME_WINDOW_RESTORED,                   /**< Window has been restored to normal size and position */

                GAME_WINDOW_ENTER,                      /**< Window has gained mouse focus */
                GAME_WINDOW_LEAVE,                      /**< Window has lost mouse focus */
                GAME_WINDOW_FOCUS_GAINED,               /**< Window has gained keyboard focus */
                GAME_WINDOW_FOCUS_LOST,                 /**< Window has lost keyboard focus */

                GAME_WINDOW_CLOSE,                      /**< The window manager requests that the window be closed */
                GAME_WINDOW_LAST = GAME_WINDOW_CLOSE    /**< Used only as the Upper bounds of this enumeration*/
            } GameWindowEventID;

            /// @brief Creates an EventGameWindow from a Rawevent
            /// @param Raw_ The RawEvent to decompose to use for the values of this EventGameWindow
            EventGameWindow(RawEvent Raw_);

            /// @brief Creates an EventGameWindow from manually assigned values
            /// @param GWEventID What kind of change happened
            /// @param First A parameter that is dependant on the kind of change to the game window
            /// @param Second A parameter that is dependant on the kind of change to the game window
            EventGameWindow(GameWindowEventID GWEventID, int First=0, int Second=0);

            /// @brief Deconstructs this EventGameWindow
            ~EventGameWindow();

            /// @brief This returns EventType::GameWindow
            /// @details  This returns the kind of message this is, specifcally EventType::GameWindow . If
            /// this functions returns EventType::GameWindow, then and event pointer can safely be
            /// cast to phys::EventGameWindow . This method is inherited from phys::Event .
            virtual EventType GetType() const;

            /// @brief What just happened, what kind of event was it.
            /// @return A GameWindowEventID indicating what happened.
            GameWindowEventID GetEventID() const;

            /// @brief Where or how much happened, Get the first event dependent data.
            /// @return An int with some details about the the event that happened.
            /// @todo document GetFirstEventData further.
            int GetFirstEventData() const;

            /// @brief Where or how much happened, Get the second event dependent data.
            /// @return An int with some details about the the event that happened.
            /// @todo document GetSecondEventData further.
            int GetSecondEventData() const;

            /// @brief converts GameWindowEventID To Strings
            /// @param GWEventID the GameWindowEventID to get as a string
            /// @return This returns a string containg the GameWindowEventID in a string as a developer would have typed it.
            static String GameWindowEventIDToString(EventGameWindow::GameWindowEventID GWEventID);

            /// @internal
            /// @brief Used to help determine if this Event is valid by checking the contents of the GameWindowEventID
            /// @return True if the event if is between GAME_WINDOW_FIRST and GAME_WINDOW_LAST, false otherwise
            bool IsEventIDValid() const;

        private:

            /// @internal
            /// @brief Holds all internal data
            EventGameWindowData* Data;

            /// @internal
            /// @brief Sets the internal values.
            /// @param Raw_ The RawEvent to decompose to use for the values of this EventGameWindow
            void construct(RawEvent Raw_);

            /// @internal
            /// @brief Sets the internal values.
            /// @param GWEventID What kind of change happened
            /// @param First A parameter that is dependant on the kind of change to the game window
            /// @param Second A parameter that is dependant on the kind of change to the game window
            void construct(GameWindowEventID GWEventID, int First, int Second);
    };
}

#endif

