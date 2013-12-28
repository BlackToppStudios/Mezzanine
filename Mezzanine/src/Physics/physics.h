// © Copyright 2010 - 2013 BlackTopp Studios Inc.
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
#ifndef _physics_h
#define _physics_h

namespace Mezzanine
{
    /// @namespace Mezzanine::Physics
    /// @brief This namespace is for all the classes belonging to the Physics Subsystem.
    /// @details By default, this sub-system utilizies Bullet for it's physics acceleration.
    namespace Physics
    {

    }
}

#include "Physics/physicsenumerations.h"
#include "Physics/physicsmanager.h"
#include "Physics/collisionshapemanager.h"

#include "Physics/boxcollisionshape.h"
#include "Physics/capsulecollisionshape.h"
#include "Physics/collidablepair.h"
#include "Physics/collidableproxy.h"
#include "Physics/collision.h"
#include "Physics/collisionshape.h"
#include "Physics/compoundcollisionshape.h"
#include "Physics/conecollisionshape.h"
#include "Physics/conetwistconstraint.h"
#include "Physics/constraint.h"
#include "Physics/convexhullcollisionshape.h"
#include "Physics/cylindercollisionshape.h"
#include "Physics/dualtransformconstraint.h"
#include "Physics/dynamicmeshcollisionshape.h"
#include "Physics/fieldcollisionshape.h"
#include "Physics/gearconstraint.h"
#include "Physics/generic6dofconstraint.h"
#include "Physics/generic6dofspringconstraint.h"
#include "Physics/ghostproxy.h"
#include "Physics/heightfieldcollisionshape.h"
#include "Physics/hinge2constraint.h"
#include "Physics/hingeconstraint.h"
#include "Physics/managerconstructioninfo.h"
#include "Physics/meshcollisionshape.h"
#include "Physics/multispherecollisionshape.h"
#include "Physics/planecollisionshape.h"
#include "Physics/point2pointconstraint.h"
#include "Physics/primitivecollisionshape.h"
#include "Physics/rigidproxy.h"
#include "Physics/sliderconstraint.h"
#include "Physics/softcollisionshape.h"
#include "Physics/softproxy.h"
#include "Physics/spherecollisionshape.h"
#include "Physics/staticmeshcollisionshape.h"
#include "Physics/universalconstraint.h"

#endif
