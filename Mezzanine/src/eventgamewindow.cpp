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
#ifndef EVENTGAMEWINDOW_CPP
#define EVENTGAMEWINDOW_CPP

#include "eventgamewindow.h"
#include "stringtool.h"

#ifdef MEZZDEBUG
#include "world.h"
#endif

#include <cassert>
#include <memory>

#include "SDL.h"

namespace Mezzanine
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
        { Data=0; construct(Raw_); }

    EventGameWindow::EventGameWindow(EventGameWindow::GameWindowEventID GWEventID, int First, int Second)
        { Data=0; construct(GWEventID, First, Second); }

    EventGameWindow::EventGameWindow( const EventGameWindow& Other)
    {
        Data=0;
        assert(Other.Data);
        construct(Other.Data->EventID, Other.Data->First, Other.Data->Second);
    }

    EventBase::EventType EventGameWindow::GetType() const
        { return EventBase::GameWindow; }

    EventGameWindow::~EventGameWindow()
    {
        #ifdef MEZZDEBUG
        World::GetWorldPointer()->Log("De-Constructing internal data for EventGameWindow");
        #endif
        delete this->Data;
    }

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
                throw("Unhandled EventGameWindow::GameWindowEventID reached during eventid to String conversion.");
        }
    }

    bool EventGameWindow::IsEventIDValid() const
        { return ( GAME_WINDOW_FIRST <= this->GetEventID() && this->GetEventID() <= GAME_WINDOW_LAST ); }

    void EventGameWindow::operator=(const EventGameWindow& Other)
    {
        if(this->Data==Other.Data)
            { return; }
        this->Data->EventID=Other.Data->EventID;
        this->Data->First=Other.Data->First;
        this->Data->Second=Other.Data->Second;
    }

    bool EventGameWindow::operator==(const EventGameWindow& Other) const
    {
        assert(this->Data);
        return ( this->Data->EventID==Other.Data->EventID && this->Data->First==Other.Data->First && this->Data->Second==Other.Data->Second );
    }

    bool EventGameWindow::operator==(const GameWindowEventID& Other) const
        { return GetEventID()==Other; }

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
        #ifdef MEZZDEBUG
        World::GetWorldPointer()->Log("Constructing internal data for EventGameWindow");
        #endif
        this->Data=new EventGameWindowData(GWEventID,First, Second);
    }
}

///////////////////////////////////////////////////////////////////////////////
// Class External << Operators for streaming or assignment
#ifdef MEZZXML
std::ostream& operator << (std::ostream& stream, const Mezzanine::EventGameWindow& Ev)
{
    stream << "<EventGameWindow Version=\"1\" EventID=\"" << Ev.GetEventID() << "\" First=\"" << Ev.GetFirstEventData() << "\" Second=\"" << Ev.GetSecondEventData() << "\" />";
    return stream;
}

std::istream& MEZZ_LIB operator >> (std::istream& stream, Mezzanine::EventGameWindow& Ev)
{
    Mezzanine::String OneTag( Mezzanine::xml::GetOneTag(stream) );
    std::auto_ptr<Mezzanine::xml::Document> Doc( Mezzanine::xml::PreParseClassFromSingleTag("phys::", "EventGameWindow", OneTag) );

    Doc->GetFirstChild() >> Ev;

    return stream;
}

Mezzanine::xml::Node& operator >> (const Mezzanine::xml::Node& OneNode, Mezzanine::EventGameWindow& Ev)
{
    if ( Mezzanine::String(OneNode.Name())==Mezzanine::String("EventGameWindow") )
    {
        if(OneNode.GetAttribute("Version").AsInt() == 1)
        {
            Ev = Mezzanine::EventGameWindow( (Mezzanine::EventGameWindow::GameWindowEventID)OneNode.GetAttribute("EventID").AsInt(),
                                    OneNode.GetAttribute("First").AsInt(),
                                    OneNode.GetAttribute("Second").AsInt() );
        }else{
            throw( Mezzanine::Exception("Incompatible XML Version for EventGameWindow: Not Version 1"));
        }
    }else{
        throw( Mezzanine::Exception(Mezzanine::StringTool::StringCat("Attempting to deserialize a EventGameWindow, found a ", OneNode.Name())));
    }

}
#endif // \MEZZXML

#endif

