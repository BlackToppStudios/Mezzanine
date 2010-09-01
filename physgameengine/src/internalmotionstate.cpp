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
#ifndef _internalmotionstate_cpp
#define _internalmotionstate_cpp

#include "internalmotionstate.h.cpp"

namespace phys{
    namespace internal{
        ///////////////////////////////////
        // PhysMotionState
        PhysMotionState::PhysMotionState()
        {
            this->worldtrans.setIdentity();
        }

        PhysMotionState::PhysMotionState(Ogre::SceneNode* scenenode)
        {
            this->snode = scenenode;
            this->worldtrans.setIdentity();
        }

        PhysMotionState::~PhysMotionState()
        {
            if (snode!=NULL)
            {
                delete snode;
            }
        }

        void PhysMotionState::SetNode(Ogre::SceneNode* scenenode)
        {
            this->snode = scenenode;
        }

        void PhysMotionState::SetPosition(Vector3 position)
        {
            this->worldtrans.setOrigin(position.GetBulletVector3());
        }

        void PhysMotionState::SetOrientation(Quaternion orientation)
        {
            this->worldtrans.setRotation(orientation.GetBulletQuaternion());
        }

        void PhysMotionState::getWorldTransform(btTransform &worldTrans) const
        {
            worldTrans = this->worldtrans;
        }

        void PhysMotionState::setWorldTransform(const btTransform &worldTrans)
        {
            btQuaternion rotation = worldTrans.getRotation();
            this->snode->setOrientation(rotation.w(), rotation.x(), rotation.y(), rotation.z());
            btVector3 position = worldTrans.getOrigin();
            this->snode->setPosition(position.x(), position.y(), position.z());
        }
    }// /internal
}// /phys
#endif
