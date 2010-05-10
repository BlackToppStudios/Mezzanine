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
#ifndef _physmisc_cpp
#define _physmisc_cpp

#include "SDL.h"
#include "datatypes.h"
#include "physmisc.h"

String GetNameOfEventFrom( RawEvent* event )
{
        switch(event->type)
        {
            case SDL_ACTIVEEVENT:   //when the window gains focus
                return "SDL_ACTIVEEVENT";
            case SDL_VIDEORESIZE:   //when the screen is resized
                return "SDL_VIDEORESIZE";
            case SDL_VIDEOEXPOSE:   //when the windows goes from being hidden to being shown
                return "SDL_VIDEOEXPOSE";
            case SDL_QUIT:          //when SDL closes
                return "SDL_QUIT";
            case SDL_SYSWMEVENT:
                return "SDL_SYSWMEVENT";
            case SDL_KEYDOWN:
                return "SDL_KEYDOWN";
            case SDL_KEYUP:
                return "SDL_KEYUP";
            case SDL_MOUSEMOTION:
                return "SDL_MOUSEMOTION";
            case SDL_MOUSEBUTTONDOWN:
                return "SDL_MOUSEBUTTONDOWN";
            case SDL_MOUSEBUTTONUP:
                return "SDL_MOUSEBUTTONUP";
            case SDL_JOYAXISMOTION:
                return "SDL_JOYAXISMOTION";
            case SDL_JOYBUTTONDOWN:
                return "SDL_JOYBUTTONDOWN";
            case SDL_JOYBUTTONUP:
                return "SDL_JOYBUTTONUP";
            case SDL_JOYBALLMOTION:
                return "SDL_JOYBALLMOTION";
            case SDL_JOYHATMOTION:
                return "SDL_JOYHATMOTION";
            case SDL_USEREVENT://Never thrown by SDL, but could be added by a user
                return "SDL_USEREVENT";
            default:
                return "Unknown SDL Event Inserted";
        }
}





#endif

