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
#ifndef EVENTBASE_CPP
#define EVENTBASE_CPP

#include "eventbase.h"
#include "eventcollision.h"
#include "eventgamewindow.h"
#include "eventquit.h"
#include "eventuserinput.h"
#include "exception.h"

std::ostream& MEZZ_LIB operator << (std::ostream& stream, const Mezzanine::EventBase& Ev)
{
    switch(Ev.GetType())
    {
        case Mezzanine::EventBase::Collision:
            stream << *dynamic_cast<const Mezzanine::EventCollision*> (&Ev);
            break;
        case Mezzanine::EventBase::GameWindow:
            stream << *dynamic_cast<const Mezzanine::EventGameWindow*> (&Ev);
            break;
        case Mezzanine::EventBase::QuitMessage:
            stream << *dynamic_cast<const Mezzanine::EventQuit*> (&Ev);
            break;
        case Mezzanine::EventBase::UserInput:
            stream << *dynamic_cast<const Mezzanine::EventUserInput*> (&Ev);
            break;
        case Mezzanine::EventBase::Other:
            MEZZ_EXCEPTION(Mezzanine::Exception::PARAMETERS_EXCEPTION,"Attemping to serialize a Mezzanine::Event::Other... not sure what you are trying to serialize.");
            break;
        default:
            MEZZ_EXCEPTION(Mezzanine::Exception::PARAMETERS_EXCEPTION,"Attemping to serialize a Mezzanine::Event... not sure what you are trying to serialize.");
            break;
    }
    return stream;
}

#endif
