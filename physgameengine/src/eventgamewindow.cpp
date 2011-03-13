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
#ifndef EVENTGAMEWINDOW_CPP
#define EVENTGAMEWINDOW_CPP

#include "eventgamewindow.h"

#include <cassert>

#include "SDL.h"

namespace phys
{
    /// @brief used to keep private in one place that is actually private.
    class EventGameWindowData
    {
        public:
            EventGameWindowData(EventGameWindow::GameWindowEventID GWEventID, int First_, int Second_):EventID(GWEventID),First(First_),Second(Second_)
            {}

            /// @brief What kind of change happened to this game window
            EventGameWindow::GameWindowEventID EventID;

            /// @brief store a piece of information about the event.
            int First;

            /// @brief store another piece of information about the event.
            int Second;
    };

    EventGameWindow::EventGameWindow(RawEvent Raw_)
        { construct(Raw_); }

    EventGameWindow::EventGameWindow(EventGameWindow::GameWindowEventID GWEventID, int First, int Second)
        { construct(GWEventID, First, Second); }

    EventBase::EventType EventGameWindow::GetType() const
        { return EventBase::GameWindow; }

    EventGameWindow::~EventGameWindow()
        { delete this->Data; }

    EventGameWindow::GameWindowEventID EventGameWindow::GetEventID() const
    {
        assert(this->Data);
        return this->Data->EventID;
    }

    int EventGameWindow::GetFirstEventData() const
    {
        assert(this->Data);
        return this->Data->First;
    }

    int EventGameWindow::GetSecondEventData() const
    {
        assert(this->Data);
        return this->Data->Second;
    }

    String EventGameWindow::GameWindowEventIDToString(EventGameWindow::GameWindowEventID GWEventID)
    {
        switch (GWEventID)
        {
            case GAME_WINDOW_NONE:
                return "GAME_WINDOW_NONE";
            case GAME_WINDOW_SHOWN:
                return "GAME_WINDOW_SHOWN";
            case GAME_WINDOW_HIDDEN:
                return "GAME_WINDOW_HIDDEN";
            case GAME_WINDOW_EXPOSED:
                return "GAME_WINDOW_EXPOSED";
            case GAME_WINDOW_MOVED:
                return "GAME_WINDOW_MOVED";
            case GAME_WINDOW_RESIZED:
                return "GAME_WINDOW_RESIZED";
            case GAME_WINDOW_SIZE_CHANGED:
                return "GAME_WINDOW_SIZE_CHANGED";
            case GAME_WINDOW_MINIMIZED:
                return "GAME_WINDOW_MINIMIZED";
            case GAME_WINDOW_MAXIMIZED:
                return "GAME_WINDOW_MAXIMIZED";
            case GAME_WINDOW_RESTORED:
                return "GAME_WINDOW_RESTORED";
            case GAME_WINDOW_ENTER:
                return "GAME_WINDOW_ENTER";
            case GAME_WINDOW_LEAVE:
                return "GAME_WINDOW_LEAVE";
            case GAME_WINDOW_FOCUS_GAINED:
                return "GAME_WINDOW_FOCUS_GAINED";
            case GAME_WINDOW_FOCUS_LOST:
                return "GAME_WINDOW_FOCUS_LOST";
            case GAME_WINDOW_CLOSE:
                return "GAME_WINDOW_CLOSE";
            default:
                break;//throw("Unhandled EventGameWindow::GameWindowEventID reached during eventid to String conversion.");
        }
    }

    bool EventGameWindow::IsEventIDValid() const
        { return ( GAME_WINDOW_FIRST <= this->GetEventID() && this->GetEventID() <= GAME_WINDOW_LAST ); }

    void EventGameWindow::construct(RawEvent Raw_)
    {
        assert((int)GAME_WINDOW_NONE==(int)SDL_WINDOWEVENT_NONE);
        assert((int)GAME_WINDOW_CLOSE==(int)SDL_WINDOWEVENT_CLOSE);
        EventGameWindow::GameWindowEventID GWEventID;
        switch (Raw_.window.event)
        {
            case SDL_WINDOWEVENT_NONE:
                GWEventID = GAME_WINDOW_NONE; break;
            case SDL_WINDOWEVENT_SHOWN:
                GWEventID = GAME_WINDOW_SHOWN; break;
            case SDL_WINDOWEVENT_HIDDEN:
                GWEventID = GAME_WINDOW_HIDDEN; break;
            case SDL_WINDOWEVENT_EXPOSED:
                GWEventID = GAME_WINDOW_EXPOSED; break;
            case SDL_WINDOWEVENT_MOVED:
                GWEventID = GAME_WINDOW_MOVED; break;
            case SDL_WINDOWEVENT_RESIZED:
                GWEventID = GAME_WINDOW_RESIZED; break;
            case SDL_WINDOWEVENT_SIZE_CHANGED:
                GWEventID = GAME_WINDOW_SIZE_CHANGED; break;
            case SDL_WINDOWEVENT_MINIMIZED:
                GWEventID = GAME_WINDOW_MINIMIZED; break;
            case SDL_WINDOWEVENT_MAXIMIZED:
                GWEventID = GAME_WINDOW_MAXIMIZED; break;
            case SDL_WINDOWEVENT_RESTORED:
                GWEventID = GAME_WINDOW_RESTORED; break;
            case SDL_WINDOWEVENT_ENTER:
                GWEventID = GAME_WINDOW_ENTER; break;
            case SDL_WINDOWEVENT_LEAVE:
                GWEventID = GAME_WINDOW_LEAVE; break;
            case SDL_WINDOWEVENT_FOCUS_GAINED:
                GWEventID = GAME_WINDOW_FOCUS_GAINED; break;
            case SDL_WINDOWEVENT_FOCUS_LOST:
                GWEventID = GAME_WINDOW_FOCUS_LOST; break;
            case SDL_WINDOWEVENT_CLOSE:
                GWEventID = GAME_WINDOW_CLOSE; break;
            default:
                throw("Unhandled EventGameWindow::GameWindowEventID reached during event creation.");
        }
        construct(GWEventID, Raw_.window.data1, Raw_.window.data2);
    }

    void EventGameWindow::construct(EventGameWindow::GameWindowEventID GWEventID, int First, int Second)
    {
        this->Data=new EventGameWindowData(GWEventID,First, Second);
    }
}
#endif

