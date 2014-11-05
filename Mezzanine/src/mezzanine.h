// © Copyright 2010 - 2014 BlackTopp Studios Inc.
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
/// @file
/// @brief A single file that includes all of the Mezzanine engine
/// @details This file exists primarily to make it easier for engine users to include
/// parts of the Mezzanine engine in their project with out needing to know or
/// care about the internals of our project.
/// \n
// We put headers in our headers so we can include while we include
///////////////////////////////////////////////////////////////////////////////

// Tell SWIG to implement scripting language specific stuff, set compiler macros used else where, must be included first
#include "swig.h"

// This is required for the swig parser to skip the items that don't support scripting yet.
#ifndef SWIG
    #include "Audio/audio.h"
    #include "Threading/dagframescheduler.h"
    #include "Graphics/graphics.h"
    #include "Graphics/Procedural/procedural.h"
    #include "Input/input.h"
    #include "Physics/physics.h"
    #include "Resource/resource.h"
    #include "Scripting/scripting.h"
    #include "XML/xml.h"
    #include "UI/ui.h"
#endif

#include "actor.h"
#include "actormanager.h"
#include "areaeffect.h"
#include "areaeffectmanager.h"
#include "attachable.h"
#include "axisalignedbox.h"
#include "binarybuffer.h"
#include "cameracontroller.h"
#include "circle.h"
#include "colourvalue.h"
#include "countedptr.h"
#include "crossplatform.h"
#include "crossplatformexport.h"
#include "datatypes.h"
#include "debris.h"
#include "debrismanager.h"

#ifndef SWIG
#include "entresol.h"
#endif

#include "enumerations.h"
#include "event.h"
#include "eventarguments.h"


#include "eventbase.h"
#include "eventgamewindow.h"
#include "eventmanager.h"
#include "eventpublisher.h"
#include "eventquit.h"


#include "eventsubscriber.h"
#include "eventsubscriberslot.h"

#ifndef SWIG
#include "eventuserinput.h"
#endif

#include "exception.h"

#ifndef SWIG
#include "fieldofforce.h"
#include "gravityfield.h"
#include "gravitywell.h"
#include "hashedstring.h"
#include "interpolator.h"
#include "linegroup.h"
#include "linesegment.h"
#include "macros.h"
#include "managedptr.h"
#endif

#include "managerbase.h"
#include "managerfactory.h"
#include "mathtools.h"

#ifndef SWIG
#include "meshterrain.h"
#include "network.h"
#include "networkmanager.h"
#include "objectsettings.h"
#endif

#include "plane.h"
#include "quaternion.h"
#include "ray.h"
#include "rayquerytool.h"

#ifndef SWIG
#include "rigiddebris.h"
#include "serialization.h"
#include "singleton.h"
#include "softdebris.h"
#endif

#include "sphere.h"

#ifndef SWIG
#include "streamlogging.h"
#endif

#include "stringtool.h"

#ifndef SWIG
#include "track.h"
#include "trackiterator.h"
#include "triangle.h"
#include "terrainbase.h"
#include "terrainmanager.h"
#endif // \no SWIG

#include "timer.h"
#include "transform.h"
#include "vector2.h"
#include "vector3.h"

#include "worldobject.h"

#ifndef SWIG
#include "worldtrigger.h"
#endif // \no SWIG

#endif // \ _mezzanine_h
