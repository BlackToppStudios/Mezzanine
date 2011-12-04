//© Copyright 2010 - 2011 BlackTopp Studios Inc.
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
#ifndef _worldobject_cpp
#define _worldobject_cpp

#include "worldobject.h"

#include <Ogre.h>

namespace Mezzanine
{
    WorldObject::WorldObject()
        : GraphicsObject(NULL),
          GraphicsNode(NULL),
          CollisionObject(NULL),
          Shape(NULL),
          GraphicsSettings(NULL)
    {
    }

    WorldObject::~WorldObject()
    {
    }

    void WorldObject::AttachToGraphics()
    {
        if(GraphicsNode && GraphicsObject)
        {
            Ogre::Vector3 tempv;
            tempv << CollisionObject->getWorldTransform().getOrigin();
            Ogre::Quaternion tempq
            tempq << CollisionObject->getWorldTransform().getRotation();
            this->GraphicsNode->setPosition(tempv);
            this->GraphicsNode->setOrientation(tempq);
            this->GraphicsNode->attachObject(this->GraphicsObject);
        }
    }

    void WorldObject::DetachFromGraphics()
    {
        if(GraphicsNode && GraphicsObject)
            this->GraphicsNode->detachObject(this->GraphicsObject);
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Utility and Configuration
    ///////////////////////////////////////

    String WorldObject::GetName() const
    {
        return Name;
    }

    bool WorldObject::IsInWorld() const
    {
        return CollisionObject->getBroadphaseHandle() != 0;
    }

    ///////////////////////////////////
    // Internal Object Access functions

    void WorldObject::_NotifyCollisionState(Collision* Col, const Collision::CollisionState& State)
    {
        if(Collision::Col_Begin == State)
        {
            CurrentCollisions.insert(Col);
        }
        else if(Collision::Col_End == State)
        {
            std::set<Collision*>::iterator ColIt = CurrentCollisions.find(Col);
            if( ColIt != CurrentCollisions.end() ) CurrentCollisions.erase(ColIt);
        }
    }

    btCollisionObject* WorldObject::_GetBasePhysicsObject() const
    {
        return CollisionObject;
    }

    Ogre::Entity* WorldObject::_GetGraphicsObject() const
    {
        return GraphicsObject;
    }

    Ogre::SceneNode* WorldObject::_GetGraphicsNode() const
    {
        return GraphicsNode;
    }
    ///////////////////////////////////////////////////////////////////////////////
    // Serialization
    ///////////////////////////////////////
#ifdef MEZZXML
    void WorldObject::ThrowSerialError(const String& Fail) const
        { SerializeError(Fail, SerializableName()); }

    String WorldObject::SerializableName()
        {   return String("WorldObject"); }
#endif
}//Mezzanine

#endif
