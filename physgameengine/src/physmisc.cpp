#ifndef _physmisc_cpp
#define _physmisc_cpp

#include "SDL.h"
#include "physdatatypes.h"
#include "physmisc.h"

PhysString GetNameOfEventFrom( RawEvent event )
{
        switch(event.type)
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

