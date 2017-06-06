// © Copyright 2010 - 2016 BlackTopp Studios Inc.
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

#ifndef _appeventdispatcher_cpp
#define _appeventdispatcher_cpp

#include "appeventdispatcher.h"
#include "exception.h"

#include "SDL.h"

#include <cassert>

namespace
{
    using namespace Mezzanine;

    /// @brief Handles the "AppTerminating" application event from internal sources.
    /// @param Dispatcher A pointer to the dispatcher that till notify interested subscribers of the event.
    int HandleAppTerminating(AppEventDispatcher* Dispatcher)
    {
        EventPtr AppEv = std::make_shared<Event>(AppEventDispatcher::EventAppTerminating);
        Dispatcher->DispatchEvent(AppEv);
        return 0;
    }

    /// @brief Handles the "AppLowMemory" application event from internal sources.
    /// @param Dispatcher A pointer to the dispatcher that till notify interested subscribers of the event.
    int HandleAppLowMemory(AppEventDispatcher* Dispatcher)
    {
        EventPtr AppEv = std::make_shared<Event>(AppEventDispatcher::EventAppLowMemory);
        Dispatcher->DispatchEvent(AppEv);
        return 0;
    }

    /// @brief Handles the "AppWillEnterBackground" application event from internal sources.
    /// @param Dispatcher A pointer to the dispatcher that till notify interested subscribers of the event.
    int HandleAppWillEnterBackground(AppEventDispatcher* Dispatcher)
    {
        EventPtr AppEv = std::make_shared<Event>(AppEventDispatcher::EventAppWillEnterBackground);
        Dispatcher->DispatchEvent(AppEv);
        return 0;
    }

    /// @brief Handles the "AppDidEnterBackground" application event from internal sources.
    /// @param Dispatcher A pointer to the dispatcher that till notify interested subscribers of the event.
    int HandleAppDidEnterBackground(AppEventDispatcher* Dispatcher)
    {
        EventPtr AppEv = std::make_shared<Event>(AppEventDispatcher::EventAppDidEnterBackground);
        Dispatcher->DispatchEvent(AppEv);
        return 0;
    }

    /// @brief Handles the "AppWillEnterForeground" application event from internal sources.
    /// @param Dispatcher A pointer to the dispatcher that till notify interested subscribers of the event.
    int HandleAppWillEnterForeground(AppEventDispatcher* Dispatcher)
    {
        EventPtr AppEv = std::make_shared<Event>(AppEventDispatcher::EventAppWillEnterForeground);
        Dispatcher->DispatchEvent(AppEv);
        return 0;
    }

    /// @brief Handles the "AppDidEnterForeground" application event from internal sources.
    /// @param Dispatcher A pointer to the dispatcher that till notify interested subscribers of the event.
    int HandleAppDidEnterForeground(AppEventDispatcher* Dispatcher)
    {
        EventPtr AppEv = std::make_shared<Event>(AppEventDispatcher::EventAppDidEnterForeground);
        Dispatcher->DispatchEvent(AppEv);
        return 0;
    }

    /// @brief A custom event filter for SDL to enable immediate dispatch of application events.
    /// @param userdata A pointer to the user set data for processing the event, in this case the AppEventDispatcher.
    /// @param event The SDL event being dispatched.
    int AppEventFilter(void* userdata, SDL_Event* event)
    {
        AppEventDispatcher* Dispatcher = static_cast<AppEventDispatcher*>(userdata);
        switch( event->type )
        {
            case SDL_APP_TERMINATING:         return HandleAppTerminating(Dispatcher);         break;
            case SDL_APP_LOWMEMORY:           return HandleAppLowMemory(Dispatcher);           break;
            case SDL_APP_WILLENTERBACKGROUND: return HandleAppWillEnterBackground(Dispatcher); break;
            case SDL_APP_DIDENTERBACKGROUND:  return HandleAppDidEnterBackground(Dispatcher);  break;
            case SDL_APP_WILLENTERFOREGROUND: return HandleAppWillEnterForeground(Dispatcher); break;
            case SDL_APP_DIDENTERFOREGROUND:  return HandleAppDidEnterForeground(Dispatcher);  break;
            default:                          return 1;                                        break;
        }
        return 1;
    }
}

namespace Mezzanine
{
    ///////////////////////////////////////////////////////////////////////////////
    // AppEventDispatcher Static Members

    const EventNameType AppEventDispatcher::EventAppDropFile            = "AppDropFile";
    const EventNameType AppEventDispatcher::EventAppDropText            = "AppDropText";
    const EventNameType AppEventDispatcher::EventAppDropBegin           = "AppDropBegin";
    const EventNameType AppEventDispatcher::EventAppDropEnd             = "AppDropEnd";
    const EventNameType AppEventDispatcher::EventAppQuit                = "AppQuit";
    const EventNameType AppEventDispatcher::EventAppTerminating         = "AppTerminating";
    const EventNameType AppEventDispatcher::EventAppLowMemory           = "AppLowMemory";
    const EventNameType AppEventDispatcher::EventAppWillEnterBackground = "AppWillEnterBackground";
    const EventNameType AppEventDispatcher::EventAppDidEnterBackground  = "AppDidEnterBackground";
    const EventNameType AppEventDispatcher::EventAppWillEnterForeground = "AppWillEnterForeground";
    const EventNameType AppEventDispatcher::EventAppDidEnterForeground  = "AppDidEnterForeground";

    ///////////////////////////////////////////////////////////////////////////////
    // AppEventDispatcher Methods

    AppEventDispatcher::AppEventDispatcher()
    {
        this->AddSubscriptionTable(AppEventDispatcher::EventAppDropFile);
        this->AddSubscriptionTable(AppEventDispatcher::EventAppDropText);
        this->AddSubscriptionTable(AppEventDispatcher::EventAppDropBegin);
        this->AddSubscriptionTable(AppEventDispatcher::EventAppDropEnd);
        this->AddSubscriptionTable(AppEventDispatcher::EventAppQuit);
        this->AddSubscriptionTable(AppEventDispatcher::EventAppTerminating);
        this->AddSubscriptionTable(AppEventDispatcher::EventAppLowMemory);
        this->AddSubscriptionTable(AppEventDispatcher::EventAppWillEnterBackground);
        this->AddSubscriptionTable(AppEventDispatcher::EventAppDidEnterBackground);
        this->AddSubscriptionTable(AppEventDispatcher::EventAppWillEnterForeground);
        this->AddSubscriptionTable(AppEventDispatcher::EventAppDidEnterForeground);

        if( !SDL_GetEventFilter(nullptr,nullptr) ) {
            SDL_SetEventFilter(AppEventFilter,this);
        }else{
            MEZZ_EXCEPTION(ExceptionBase::INVALID_STATE_EXCEPTION,"An internal EventFilter has already been set.");
        }
    }

    AppEventDispatcher::~AppEventDispatcher()
    {
        SDL_SetEventFilter(nullptr,nullptr);
    }

    void AppEventDispatcher::HandleAppDropFile(const UInt32 WindowID, const String& PathAndFile)
    {
        AppDropEventPtr AppEv = std::make_shared<AppDropEvent>(EventAppDropFile,PathAndFile,WindowID);
        this->DispatchEvent(AppEv);
    }

    void AppEventDispatcher::HandleAppDropText(const UInt32 WindowID, const String& Text)
    {
        AppDropEventPtr AppEv = std::make_shared<AppDropEvent>(EventAppDropText,Text,WindowID);
        this->DispatchEvent(AppEv);
    }

    void AppEventDispatcher::HandleAppDropBegin(const UInt32 WindowID)
    {
        AppDropEventPtr AppEv = std::make_shared<AppDropEvent>(EventAppDropBegin,WindowID);
        this->DispatchEvent(AppEv);
    }

    void AppEventDispatcher::HandleAppDropEnd(const UInt32 WindowID)
    {
        AppDropEventPtr AppEv = std::make_shared<AppDropEvent>(EventAppDropEnd,WindowID);
        this->DispatchEvent(AppEv);
    }

    void AppEventDispatcher::HandleAppQuit()
    {
        EventPtr AppEv = std::make_shared<Event>(EventAppQuit);
        this->DispatchEvent(AppEv);
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Utility

    void AppEventDispatcher::PumpEvents()
    {
        /* A listing of all the SDL_Events we care about in the Input System.
        // A * next to the Event denotes working support for that event does not yet exist.
        // A - next to the Event denotes that the event is disabled by Mezzanine.
        // Events starting with SDL_APP_* need to be handled by event filters for immediate processing.
        SDL_QUIT           = 0x100,  // User-requested quit
        // These application events have special meaning on iOS, see README-ios.md for details
        SDL_APP_TERMINATING,         // The application is being terminated by the OS
        SDL_APP_LOWMEMORY,           // The application is low on memory, free memory if possible.
        SDL_APP_WILLENTERBACKGROUND, // The application is about to enter the background
        SDL_APP_DIDENTERBACKGROUND,  // The application did enter the background and may not get CPU for some time
        SDL_APP_WILLENTERFOREGROUND, // The application is about to enter the foreground
        SDL_APP_DIDENTERFOREGROUND,  // The application is now interactive
        // Drag and drop events
        SDL_DROPFILE       = 0x1000, // The system requests a file open
        SDL_DROPTEXT,                // text/plain drag-and-drop event
        SDL_DROPBEGIN,               // A new set of drops is beginning (NULL filename)
        SDL_DROPCOMPLETE,            // Current set of drops is now complete (NULL filename)
        */

        // Make a fixed size array and grab our events.
        // Internally the event queue is capped at 128 events.  16 seems sane for this, but may need modifying.
        // SDL_QUIT to SDL_APP_DIDENTERFORGROUND and SDL_DROPFILE to SDL_DROPCOMPLETE are all application events.
        // Everything in between those sets values, as well as those values, will be pulled.
        std::array<SDL_Event,16> InternalEvents;
        Integer QuitEvents = SDL_PeepEvents(InternalEvents.data(),InternalEvents.size(),SDL_GETEVENT,SDL_QUIT,SDL_QUIT);
        assert( QuitEvents >= 0 && SDL_GetError() );
        Integer NumEvents = SDL_PeepEvents(InternalEvents.data(),InternalEvents.size(),SDL_GETEVENT,SDL_DROPFILE,SDL_DROPCOMPLETE);
        assert( NumEvents >= 0 && SDL_GetError() );
        NumEvents += QuitEvents;

        for( Integer CurrEv = 0 ; CurrEv < NumEvents ; ++CurrEv )
        {
            UInt32 EventType = InternalEvents[CurrEv].type;
            if( EventType == SDL_QUIT ) {
                this->HandleAppQuit();
            }else{
                SDL_DropEvent Drop = InternalEvents[CurrEv].drop;
                switch( EventType )
                {
                    case SDL_DROPFILE:      this->HandleAppDropFile(Drop.windowID,Drop.file);  break;
                    case SDL_DROPTEXT:      this->HandleAppDropText(Drop.windowID,Drop.file);  break;
                    case SDL_DROPBEGIN:     this->HandleAppDropBegin(Drop.windowID);           break;
                    case SDL_DROPCOMPLETE:  this->HandleAppDropEnd(Drop.windowID);             break;
                    default:                                                                   break;
                }

                if( Drop.file ) {
                    SDL_free(Drop.file);
                    Drop.file = nullptr;
                }
            }
        }
    }
}

#endif
