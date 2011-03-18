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
#ifndef EVENTBASE_CPP
#define EVENTBASE_CPP

#include "eventbase.h"
#include "eventcollision.h"
#include "eventgamewindow.h"
#include "eventquit.h"
#include "eventrendertime.h"
#include "eventuserinput.h"
#include "exception.h"

std::ostream& PHYS_LIB operator << (std::ostream& stream, const phys::EventBase& Ev)
{
    switch(Ev.GetType())
    {
        case phys::EventBase::Collision:
            stream << *dynamic_cast<const phys::EventCollision*> (&Ev);
            break;
        case phys::EventBase::GameWindow:
            stream << *dynamic_cast<const phys::EventGameWindow*> (&Ev);
            break;
        case phys::EventBase::QuitMessage:
            stream << *dynamic_cast<const phys::EventQuit*> (&Ev);
            break;
        case phys::EventBase::RenderTime:
            stream << *dynamic_cast<const phys::EventRenderTime*> (&Ev);
            break;
        case phys::EventBase::UserInput:
            stream << *dynamic_cast<const phys::EventUserInput*> (&Ev);
            break;
        case phys::EventBase::Other:
            throw phys::Exception ("Attemping to serialize a phys::Event::Other... not sure what you are trying to serialize.");
            //throw Exception ("How did you instantiate an abstract base class?! Attemping to serialize a phys::EventBase");
            break;
        default:
            throw phys::Exception ("Attemping to serialize a phys::Event... not sure what you are trying to serialize.");
            break;
    }
    return stream;
}

#endif
