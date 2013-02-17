//© Copyright 2010 - 2012 BlackTopp Studios Inc.
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
#ifndef _mezzanine_h
#define _mezzanine_h

///////////////////////////////////////////////////////////////////////////////
/// @file mezzanine.h
/// @brief A single file that includes all of the Mezzanine engine
/// @details This file exists primarily to make it easier for engine users to include
/// parts of the Mezzanine engine in their project with out needing to know or
/// care about the internals of our project.
/// \n
/// This file is also u
///////////////////////////////////////////////////////////////////////////////

// Tell SWIG to implement scripting language specific stuff, set compiler macros used else where, must be included first
#include "swig.h"

// This is required for the swig parser to skip the items that don't support scripting yet.
#ifndef SWIG
                                        // The remarks in Column 41 are use to help with tracking progress on serializing and deserializing

// We put headers in our headers so we can include while we include
#include "actorbase.h"                  // serializes
#include "actorcharacter.h"
#include "actormanager.h"
#include "actorphysicssettings.h"       // done
#include "actorrigid.h"                 // Serializes
#include "actorserializer.h"            // nothing to do
#include "actorsoft.h"
#include "areaeffect.h"
#include "attachable.h"                 // Target for output only
#include "audio.h"
#include "audiomanager.h"
#include "camera.h"                     // done, must review after Mar27 upgrades
#include "cameracontroller.h"
#include "cameramanager.h"
#include "collisionshapemanager.h"
#include "colourvalue.h"                // done
#include "crossplatform.h"              // nothing to do
#include "crossplatformexport.h"        // nothing to do
#include "datatypes.h"                  // nothing to do
#include "entity.h"
#include "entresol.h"
#include "enumerations.h"               // nothing to do
#include "eventbase.h"                  // done/serializes
#include "eventcollision.h"             // done
#include "eventgamewindow.h"            // done
#include "eventmanager.h"               // done
#include "eventquit.h"                  // done
#include "eventrendertime.h"            // done
#include "eventuserinput.h"             // done
#include "exception.h"
#include "extendedtimer.h"
#include "graphics.h"
#include "graphicsmanager.h"
#include "input.h"
#include "inputmanager.h"
#include "light.h"                      // done
#include "linegroup.h"
#include "managerbase.h"
#include "mathtool.h"
#include "meshmanager.h"
#include "meshterrain.h"
#include "network.h"
#include "networkmanager.h"
#include "objectpair.h"
#include "objectreference.h"
#include "particleaffector.h"
#include "particleeffect.h"             // done
#include "particleemitter.h"
#include "physics.h"
#include "physicsmanager.h"
#include "plane.h"                      // done
#include "quaternion.h"                 // done
#include "ray.h"
#include "rayquerytool.h"               // nothing to do
#include "resource.h"
#include "resourcemanager.h"
#include "scenemanager.h"               // Done
#include "scripting.h"
#include "serialization.h"              // nothing to do
#include "simpletimer.h"
#include "singleton.h"                  // nothing to do
#include "smartptr.h"
#include "stringtool.h"
#include "terrainbase.h"
#include "terrainmanager.h"
#include "timer.h"
#include "timermanager.h"
#include "transform.h"                  // done
#include "ui.h"
#include "uimanager.h"
#include "vector2.h"                    // done
#endif // \no SWIG

#include "vector3.h"                    // done

// more stuff swig shouldn't read yet
#ifndef SWIG
#include "vector3wactor.h"
#include "Graphics/viewport.h"
#include "entresol.h"
#include "worldnode.h"                  // done
#include "worldobjectgraphicssettings.h"// done
#include "worldobject.h"
#include "worldobjectphysicssettings.h"
#include "worldtrigger.h"
#include "xmldoc.h"
#include "xml.h"                        // nothing to do


#endif // \no SWIG

#endif // \ _mezzanine_h
