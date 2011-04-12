//© Copyright 2010 - 2011 BlackTopp Studios Inc.
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
#ifndef _physgame_h
#define _physgame_h

///////////////////////////////////////////////////////////////////////////////
/// @file physgame.h
/// @brief A single file that includes all of the physgame engine
/// @details This file exists solely to make it easier for others to include
/// parts of the physgame engine in their project with out needing to know or
/// care about the internals of our project.
///////////////////////////////////////////////////////////////////////////////


                                        // The remarks in Column 41 are use to help with serializing and deserializing

// We put headers in our headers so we can include while we include
#include "actorbase.h"
#include "actorcontainerbase.h"
#include "actorcontainervector.h"
#include "actorgraphicssettings.h"
#include "actorphysicssettings.h"
#include "actorragdoll.h"
#include "actorrigid.h"
#include "actorsoft.h"
#include "actorterrain.h"
#include "areaeffect.h"
#include "attachable.h"                 // Target2 for output only
#include "camera.h"                     // done, must review after Mar27 upgrades
#include "cameracontroller.h"
#include "cameramanager.h"
#include "colourvalue.h"                // done
#include "constraint.h"
#include "crossplatform.h"              // nothing to do
#include "crossplatformexport.h"        // nothing to do
#include "datatypes.h"                  // nothing to do
#include "enumerations.h"               // nothing to do
#include "eventbase.h"                  // Done output only
#include "eventcollision.h"             // done
#include "eventgamewindow.h"            // done
#include "eventmanager.h"               // done
#include "eventquit.h"                  // done
#include "eventrendertime.h"            // done
#include "eventuserinput.h"             // done
#include "exception.h"
#include "extendedtimer.h"
#include "gamewindow.h"
#include "graphicsmanager.h"
#include "graphicssettings.h"
#include "inputquerytool.h"
#include "light.h"                      // done
#include "linegroup.h"
#include "managerbase.h"
#include "mathtool.h"
#include "meshgenerator.h"
#include "metacode.h"                   // done
#include "particleeffect.h"             // Target
#include "physicsmanager.h"
#include "plane.h"                      // done
#include "quaternion.h"                 // in progress
#include "ray.h"
#include "rayquerytool.h"               // nothing to do
#include "resourceinputstream.h"
#include "resourcemanager.h"
#include "scenemanager.h"
#include "simpletimer.h"
#include "sound.h"
#include "soundlistener.h"
#include "soundmanager.h"
#include "timer.h"
#include "timermanager.h"
#include "uibutton.h"
#include "uibuttonlistbox.h"
#include "uicaption.h"
#include "uicheckbox.h"
#include "uilayer.h"
#include "uilinelist.h"
#include "uilistbox.h"
#include "uimanager.h"
#include "uimarkuptext.h"
#include "uimenu.h"
#include "uimenuwindow.h"
#include "uirectangle.h"
#include "uiresizinginfo.h"
#include "uiscreen.h"
#include "uiscrollbar.h"
#include "uitextbutton.h"
#include "uiwidget.h"
#include "uiwindow.h"
#include "vector2.h"                    // done
#include "vector3.h"                    // done
#include "vector3wactor.h"
#include "viewport.h"
#include "world.h"
#include "worldgetset.h"
#include "worldnode.h"                  // done
#include "xml.h"                        // nothing to do
#include "xmldoc.h"                     // nothing to do

#endif
