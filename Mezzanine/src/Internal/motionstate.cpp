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
#ifndef _internalmotionstate_cpp
#define _internalmotionstate_cpp

#include "Internal/motionstate.h.cpp"

#include "physicsmanager.h"
#include "areaeffect.h"

// Keeps this file form being documented by doxygen
/// @cond 0

namespace Mezzanine
{
    namespace Internal
    {
        ///////////////////////////////////////////////////////////////////////////////
        // PhysMotionState methods

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
        }

        void PhysMotionState::SetNode(Ogre::SceneNode* scenenode)
        {
            this->snode = scenenode;
        }

        void PhysMotionState::SetPosition(const Vector3& position)
        {
            this->worldtrans.setOrigin(position.GetBulletVector3());
        }

        void PhysMotionState::SetOrientation(const Quaternion& orientation)
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

        ///////////////////////////////////////////////////////////////////////////////
        // AttachableMotionState methods

        AttachableMotionState::AttachableMotionState()
        {
            WorldTrans.setIdentity();
        }

        AttachableMotionState::AttachableMotionState(NonStaticWorldObject* PO)
        {
            ParentObject = PO;
            WorldTrans.setIdentity();
        }

        AttachableMotionState::~AttachableMotionState()
        {
        }

        void AttachableMotionState::SetParentObject(NonStaticWorldObject* PO)
        {
            ParentObject = PO;
        }

        void AttachableMotionState::SetPosition(const Vector3& Position)
        {
            this->WorldTrans.setOrigin(Position.GetBulletVector3());
        }

        void AttachableMotionState::SetOrientation(const Quaternion& Orientation)
        {
            this->WorldTrans.setRotation(Orientation.GetBulletQuaternion());
        }

        void AttachableMotionState::getWorldTransform(btTransform &worldTrans) const
        {
            worldTrans = this->WorldTrans;
        }

        void AttachableMotionState::setWorldTransform(const btTransform &worldTrans)
        {
            btQuaternion rotation = worldTrans.getRotation();
            this->ParentObject->_GetGraphicsNode()->setOrientation(rotation.w(), rotation.x(), rotation.y(), rotation.z());
            btVector3 position = worldTrans.getOrigin();
            this->ParentObject->_GetGraphicsNode()->setPosition(position.x(), position.y(), position.z());

            this->ParentObject->_RecalculateAllChildTransforms();
        }
    }// /internal
}// /Mezz

/// @endcond

#endif
