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
#ifndef _quaternion_cpp
#define _quaternion_cpp

#include <Ogre.h>
#include "btBulletDynamicsCommon.h"

#include "quaternion.h"

namespace phys
{
    Quaternion::Quaternion()
    {
        X=0;
        Y=0;
        Z=0;
        W=0;
    }

    Quaternion::Quaternion(const Real &x, const Real &y, const Real &z, const Real &w)
    {
        X=x;
        Y=y;
        Z=z;
        W=w;
    }

    btQuaternion Quaternion::GetBulletQuaternion(bool normalize)
    {
        btQuaternion Theirs;
        Theirs.setX(this->X);
        Theirs.setY(this->Y);
        Theirs.setZ(this->Z);
        Theirs.setW(this->W);
        if(normalize)
        {
            Theirs.normalize();
        }
        return Theirs;
    }

    void Quaternion::ExtractBulletQuaternion(const btQuaternion &Ours)
    {
        this->X=Ours.x();
        this->Y=Ours.y();
        this->Z=Ours.z();
        this->W=Ours.w();
    }

    Ogre::Quaternion Quaternion::GetOgreQuaternion(bool normalize)
    {
        Ogre::Quaternion Theirs;
        Theirs.x=this->X;
        Theirs.y=this->Y;
        Theirs.z=this->Z;
        Theirs.w=this->W;
        if(normalize)
        {
            Theirs.normalise();
        }
        return Theirs;
    }

    void Quaternion::ExtractOgreQuaternion(const Ogre::Quaternion &Ours)
    {
        this->X=Ours.x;
        this->Y=Ours.y;
        this->Z=Ours.z;
        this->W=Ours.w;
    }
}

#endif
