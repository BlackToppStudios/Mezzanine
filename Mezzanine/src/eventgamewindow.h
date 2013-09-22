//© Copyright 2010 - 2013 BlackTopp Studios Inc.
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
#ifndef EVENTGAMEWINDOW_H
#define EVENTGAMEWINDOW_H
///////////////////////////////////////////////////////////////////////////////
// This will store the amount of time since rendering events that occured recently
///////////////////////////////////////

#include "crossplatformexport.h"
#include "eventbase.h"
#ifndef SWIG
    #include "XML/xml.h"
#endif

namespace Mezzanine
{
    class EventGameWindowData;

    ///////////////////////////////////////////////////////////////////////////////
    /// @class EventGameWindow
    /// @headerfile eventgamewindow.h
    /// @brief Convey the message that Something happened to a game window
    /// @details
    class MEZZ_LIB EventGameWindow : public EventBase
    {
        public:
            /// @brief Used to identify the kind of change that has happened to the game window.
            typedef enum
            {
                GAME_WINDOW_NONE=0,                     /**< Never used */
                GAME_WINDOW_FIRST = GAME_WINDOW_NONE,   /**< Used only as the lower bounds of this enumeration*/

                GAME_WINDOW_SHOWN = 1,                  /**< Window has been shown */
                GAME_WINDOW_HIDDEN = 2,                 /**< Window has been hidden */
                GAME_WINDOW_EXPOSED = 3,                /**< Window has been exposed and should be redrawn */

                GAME_WINDOW_MOVED = 4,                  /**< Window has been moved to data1, data2 */

                GAME_WINDOW_RESIZED = 5,                /**< Window has been resized to data1xdata2 */
                GAME_WINDOW_SIZE_CHANGED = 6,           /**< The window size has changed, either as a result of an API call or through the system or user changing the window size. */
                GAME_WINDOW_MINIMIZED = 7,              /**< Window has been minimized */
                GAME_WINDOW_MAXIMIZED = 8,              /**< Window has been maximized */
                GAME_WINDOW_RESTORED = 9,               /**< Window has been restored to normal size and position */

                GAME_WINDOW_ENTER = 10,                 /**< Window has gained mouse focus */
                GAME_WINDOW_LEAVE = 11,                 /**< Window has lost mouse focus */
                GAME_WINDOW_FOCUS_GAINED = 12,          /**< Window has gained keyboard focus */
                GAME_WINDOW_FOCUS_LOST = 13,            /**< Window has lost keyboard focus */

                GAME_WINDOW_CLOSE = 14,                 /**< The window manager requests that the window be closed */
                GAME_WINDOW_LAST = GAME_WINDOW_CLOSE    /**< Used only as the Upper bounds of this enumeration*/
            } GameWindowEventID;

            /// @brief Creates an EventGameWindow from a Rawevent
            /// @param Raw_ The RawEvent to decompose to use for the values of this EventGameWindow
            explicit EventGameWindow(RawEvent Raw_);

            /// @brief Creates an EventGameWindow from manually assigned values
            /// @param GWEventID What kind of change happened
            /// @param First A parameter that is dependant on the kind of change to the game window
            /// @param Second A parameter that is dependant on the kind of change to the game window
            explicit EventGameWindow(GameWindowEventID GWEventID=GAME_WINDOW_NONE, int First=0, int Second=0);

            /// @brief Copy constructor
            /// @param Other The Other EventGameWindow to use in the creation on this
            EventGameWindow( const EventGameWindow& Other);

            /// @brief Deconstructs this EventGameWindow
            virtual ~EventGameWindow();

            /// @brief This returns EventType::GameWindow
            /// @details  This returns the kind of message this is, specifcally EventType::GameWindow . If
            /// this functions returns EventType::GameWindow, then and event pointer can safely be
            /// cast to Mezzanine::EventGameWindow . This method is inherited from Mezzanine::Event .
            virtual EventType GetType() const;

            /// @brief What just happened, what kind of event was it.
            /// @return A GameWindowEventID indicating what happened.
            GameWindowEventID GetEventID() const;

            /// @brief Where or how much happened, Get the first event dependent data.
            /// @return An int with some details about the the event that happened.
            /// @details Currently this provides the x values for the new window locations in the event that a window moves, or it
            /// is resized. If moved it has the new location, if resized it has the new size. This may be used in places in the future.
            int GetFirstEventData() const;

            /// @brief Where or how much happened, Get the second event dependent data.
            /// @details Currently this provides the y values for the new window locations in the event that a window moves, or it
            /// is resized. If moved it has the new location, if resized it has the new size. This may be used in places in the future.
            /// @return An int with some details about the the event that happened.
            int GetSecondEventData() const;

            /// @brief Converts GameWindowEventID To Strings
            /// @param GWEventID the GameWindowEventID to get as a string
            /// @return This returns a string containg the GameWindowEventID in a string as a developer would have typed it.
            static String GameWindowEventIDToString(EventGameWindow::GameWindowEventID GWEventID);

            /// @internal
            /// @brief Used to help determine if this Event is valid by checking the contents of the GameWindowEventID
            /// @return True if the event if is between GAME_WINDOW_FIRST and GAME_WINDOW_LAST, false otherwise
            bool IsEventIDValid() const;

            /// @brief Assignment of a this EventGameWindowData
            /// @param Other the other EventGameWindow to overwite this one.
            void operator=(const EventGameWindow& Other);

            /// @brief Equality comparison of two EventGameWindowData
            /// @param Other the other EventGameWindow to compare to this one
            /// @return True if identical, false if otherwise.
            bool operator==(const EventGameWindow& Other) const;

            /// @brief Equality comparison of this EventGameWindowData and a GameWindowEventID
            /// @param Other the other GameWindowEventID to compare to the one stored in this.
            /// @return True if the GameWindowEventID in this event matches the Other, false if otherwise.
            bool operator==(const GameWindowEventID& Other) const;

        protected:
            /// @internal
            /// @brief Holds all internal data
            EventGameWindowData* Data;

        private:
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

///////////////////////////////////////////////////////////////////////////////
// Class External << Operators for streaming or assignment

/// @brief Serializes the passed Mezzanine::EventGameWindow to XML
/// @param stream The ostream to send the xml to.
/// @param Ev the Mezzanine::EventGameWindow to be serialized
/// @return this returns the ostream, now with the serialized data
std::ostream& MEZZ_LIB operator << (std::ostream& stream, const Mezzanine::EventGameWindow& Ev);

/// @brief Deserialize a Mezzanine::EventGameWindow
/// @param stream The istream to get the xml from to (re)make the Mezzanine::EventGameWindow.
/// @param Ev the Mezzanine::EventGameWindow to be deserialized.
/// @return this returns the ostream, advanced past the Mezzanine::EventGameWindow that was recreated onto Ev.
std::istream& MEZZ_LIB operator >> (std::istream& stream, Mezzanine::EventGameWindow& Ev);

/// @brief Set all values of a Mezzanine::EventGameWindow from parsed xml.
/// @param OneNode The istream to get the xml from to (re)make the Mezzanine::EventGameWindow.
/// @param Ev the Mezzanine::EventGameWindow to be reset.
/// @return This returns theXML::Node that was passed in.
const Mezzanine::XML::Node& MEZZ_LIB operator >> (const Mezzanine::XML::Node& OneNode, Mezzanine::EventGameWindow& Ev);

Mezzanine::XML::Node& MEZZ_LIB operator >> (Mezzanine::XML::Node& OneNode, Mezzanine::EventGameWindow& Ev);

#endif

