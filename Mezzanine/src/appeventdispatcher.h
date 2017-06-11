// © Copyright 2010 - 2017 BlackTopp Studios Inc.
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

#ifndef _appeventdispatcher_h
#define _appeventdispatcher_h

#include "eventpublisher.h"

namespace Mezzanine
{
    ///////////////////////////////////////////////////////////////////////////////
    /// @brief This is the event class for application drop events.
    ///////////////////////////////////////
    class MEZZ_LIB AppDropEvent : public Event
    {
    public:
        ///////////////////////////////////////////////////////////////////////////////
        // Public Data Members

        /// @brief The text or filename being dropped.
        const String DropText;
        /// @brief The unique identifier for the window that is being dropped upon.
        const UInt32 DropWindowID;

        ///////////////////////////////////////////////////////////////////////////////
        // Construction and Destruction

        /// @brief Text-less constructor.
        /// @param Name The name of the event being fired.
        /// @param WindowID The unique identifier for the window that is being dropped upon.
        AppDropEvent(const EventNameType& Name, const UInt32 WindowID) :
            Event(Name), DropWindowID(WindowID) {  }
        /// @brief Full constructor.
        /// @param Name The name of the event being fired.
        /// @param Text The text or filename being dropped.
        /// @param WindowID The unique identifier for the window that is being dropped upon.
        AppDropEvent(const EventNameType& Name, const String& Text, const UInt32 WindowID) :
            Event(Name), DropText(Text), DropWindowID(WindowID) {  }
        /// @brief Class destructor.
        virtual ~AppDropEvent() = default;
    };//AppDropEvent

    /// @brief Convenience type for passing around AppDropEvent.
    using AppDropEventPtr = std::shared_ptr<AppDropEvent>;

    ///////////////////////////////////////////////////////////////////////////////
    /// @brief This class will check for high level application events from internal sources and dispatch them to subscribers.
    /// @details Drop events are when either text or a file is hovered over the application window and released.  There
    /// are two main types of drop events to reflect which one is which.  In some cases multiple drop events can be triggered
    /// from the same action.  There is no event to represent a bundle of drops, so instead EventAppDropBegin and 
    /// EventAppDropComplete exist to help indicate the limits.  They do not, however, get used only when multiple drop 
    /// events occur.  EventAppDropBegin and EventAppDropComplete can and will fire even if there is just one drop 
    /// occurring.  So it is best to ignore them if you aren't trying to track multiple drops from a single action.
    ///////////////////////////////////////
    class MEZZ_LIB AppEventDispatcher : public EventPublisher
    {
    public:
        /// @brief Event name for when a file is dragged over the application from an external source.
        /// @remarks Events with this name are of type: AppDropEvent.
        static const EventNameType EventAppDropFile;
        /// @brief Event name for when text is dragged over the application from an external source.
        /// @remarks Events with this name are of type: AppDropEvent.
        static const EventNameType EventAppDropText;
        /// @brief Event name for when drop events are about to occur.  This could precede one or more drop events.
        /// @remarks Events with this name are of type: AppDropEvent.
        static const EventNameType EventAppDropBegin;
        /// @brief Event name for when drop events are done firing (for now).  This could follow one or more drop events.
        /// @remarks Events with this name are of type: AppDropEvent.
        static const EventNameType EventAppDropEnd;
        /// @brief Event name for when the application has been asked to quit by the user.
        /// @remarks Events with this name are of type: Event.
        static const EventNameType EventAppQuit;
        /// @brief Event name for when the OS is about to terminate the application.
        /// @remarks Events with this name are of type: Event.  This is primary used on mobile platforms. @n
        /// On iOS this is called in applicationWillTerminate().  On Android this is called in onDestroy(). @n
        /// This event is dispatched immediately and does not go into a queue.  Reason being that any delay in response
        /// would be too late to respond.
        static const EventNameType EventAppTerminating;
        /// @brief Event name for when the OS is low on memory.
        /// @remarks Events with this name are of type: Event.  This is primary used on mobile platforms. @n
        /// On iOS this is called in applicationDidReceiveMemoryWarning().  On Android this is called in onLowMemory(). @n
        /// This event is dispatched immediately and does not go into a queue.  Reason being that any delay in response
        /// would be too late to respond.
        static const EventNameType EventAppLowMemory;
        /// @brief Event name for when the OS is about to make the application inactive.
        /// @remarks Events with this name are of type: Event.  This is primary used on mobile platforms. @n
        /// On iOS this is called in applicationWillResignActive().  On Android this is called in onPause(). @n
        /// This event is dispatched immediately and does not go into a queue.  Reason being that any delay in response
        /// would be too late to respond.
        static const EventNameType EventAppWillEnterBackground;
        /// @brief Event name for when the OS has made the application inactive.
        /// @remarks Events with this name are of type: Event.  This is primary used on mobile platforms. @n
        /// On iOS this is called in applicationDidEnterBackground().  On Android this is called in onPause(). @n
        /// This event is dispatched immediately and does not go into a queue.  Reason being that any delay in response
        /// would be too late to respond.
        static const EventNameType EventAppDidEnterBackground;
        /// @brief Event name for when the OS is about to make the application interactive.
        /// @remarks Events with this name are of type: Event.  This is primary used on mobile platforms. @n
        /// On iOS this is called in applicationWillEnterForeground().  On Android this is called in onResume(). @n
        /// This event is dispatched immediately and does not go into a queue.  Reason being that any delay in response
        /// would be too late to respond.
        static const EventNameType EventAppWillEnterForeground;
        /// @brief Event name for when the OS has made the application interactive.
        /// @remarks Events with this name are of type: Event.  This is primary used on mobile platforms. @n
        /// On iOS this is called in applicationDidBecomeActive().  On Android this is called in onResume(). @n
        /// This event is dispatched immediately and does not go into a queue.  Reason being that any delay in response
        /// would be too late to respond.
        static const EventNameType EventAppDidEnterForeground;
    protected:
        /// @brief Handles the "AppDropFile" application event from internal sources.
        /// @param WindowID The unique identifier for the window that is being dropped upon.
        /// @param PathAndFile The full path and filename of the file being dropped.
        void HandleAppDropFile(const UInt32 WindowID, const String& PathAndFile);
        /// @brief Handles the "AppDropText" application event from internal sources.
        /// @param WindowID The unique identifier for the window that is being dropped upon.
        /// @param Text The text being dropped.
        void HandleAppDropText(const UInt32 WindowID, const String& Text);
        /// @brief Handles the "AppDropBegin" application event from internal sources.
        /// @param WindowID The unique identifier for the window that is being dropped upon.
        void HandleAppDropBegin(const UInt32 WindowID);
        /// @brief Handles the "AppDropEnd" application event from internal sources.
        /// @param WindowID The unique identifier for the window that is being dropped upon.
        void HandleAppDropEnd(const UInt32 WindowID);
        /// @brief Handles the "AppQuit" application event from internal sources.
        void HandleAppQuit();
    public:
        /// @brief Class constructor.
        AppEventDispatcher();
        /// @brief Class destructor.
        ~AppEventDispatcher();

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        /// @brief Pumps events from internal sources and dispatches events accordingly.
        void PumpEvents();
    };//AppEventDispatcher
}//Mezzanine

#endif
