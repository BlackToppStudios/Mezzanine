
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
#ifndef _node_cpp
#define _node_cpp

#include "node.h"
#include "scenemanager.h"
#include "camera.h"
#include "light.h"
#include "particleeffect.h"
#include "actorbase.h"

#include <Ogre.h>

namespace phys
{
    Node::Node(const String& Name, SceneManager* manager)
    {
        Manager = manager;
        OgreNode = Manager->GetGraphicsWorldPointer()->createSceneNode(Name);
        Type = Node::Free;
    }

    Node::Node(Ogre::SceneNode* snode, SceneManager* manager)
    {
        OgreNode = snode;
        Manager = manager;
        Type = Node::Free;
    }

    Node::~Node()
    {
        Manager->GetGraphicsWorldPointer()->destroySceneNode(OgreNode);
    }

    String& Node::GetName()
    {
        return OgreNode->getName();
    }

    void Node::AttachElement(Attachable* Element)
    {
        Attachable::AttachableElement Type = Element->GetElementType();
        switch (Type)
        {
            case Attachable::ParticleEffect:
            {
                ParticleEffect* particle = static_cast< ParticleEffect* > (Element);
                OgreNode->addChild(particle->OgreNode);
                //OgreNode->attachObject(particle->OgreParticle);
            }
            break;
            case Attachable::Camera:
            {
                Camera* camera = static_cast< Camera* > (Element);
                OgreNode->attachObject(camera->Cam);
            }
            break;
            case Attachable::Light:
            {
                Light* light = static_cast< Light* > (Element);
                OgreNode->attachObject(light->OgreLight);
            }
            break;
            default:
                return;
        }
        Elements.push_back(Element);
        return;
    }

    void Node::DetachElement(Attachable* Element)
    {
        Attachable::AttachableElement Type = Element->GetElementType();
        switch (Type)
        {
            case Attachable::ParticleEffect:
            {
                ParticleEffect* particle = static_cast< ParticleEffect* > (Element);
                OgreNode->detachObject(particle->OgreParticle);
            }
            break;
            case Attachable::Camera:
            {
                Camera* camera = static_cast< Camera* > (Element);
                OgreNode->detachObject(camera->Cam);
            }
            break;
            case Attachable::Light:
            {
                Light* light = static_cast< Light* > (Element);
                OgreNode->detachObject(light->OgreLight);
            }
            break;
            default:
                return;
        }
        for( std::vector< Attachable* >::iterator it = Elements.begin() ; it != Elements.end() ; it++ )
        {
            if( Element == (*it) )
            {
                Elements.erase(it);
                return;
            }
        }
        return;
    }

    void Node::DetachAllElements()
    {
        OgreNode->detachAllObjects();
        Elements.clear();
    }

    Whole Node::GetNumAttachedElements()
    {
        return Elements.size();
    }

    void Node::SetPosition(Vector3 Position)
    {
        OgreNode->setPosition(Position.GetOgreVector3());
    }

    Vector3 Node::GetPosition()
    {
        Vector3 Pos(OgreNode->getPosition());
        return Pos;
    }

    void Node::SetOrientation(Quaternion Orientation)
    {
        OgreNode->setOrientation(Orientation.GetOgreQuaternion());
    }

    Quaternion Node::GetOrientation()
    {
        Quaternion Ori(OgreNode->getOrientation());
        return Ori;
    }

    void Node::LookAt(Vector3 LookAt)
    {
        OgreNode->lookAt(LookAt.GetOgreVector3(), Ogre::Node::TS_WORLD);
    }

    void Node::SetAutoTracking(Node* node, Vector3 Offset)
    {
        OgreNode->setAutoTracking(true, node->OgreNode, Ogre::Vector3::NEGATIVE_UNIT_Z, Offset.GetOgreVector3());
    }

    void Node::SetAutoTracking(ActorBase* Actor, Vector3 Offset)
    {
        OgreNode->setAutoTracking(true, Actor->node, Ogre::Vector3::NEGATIVE_UNIT_Z, Offset.GetOgreVector3());
    }

    void Node::DisableAutoTracking()
    {
        OgreNode->setAutoTracking(false);
    }

    void Node::IncrementOrbit(Real Radians)
    {
        if( Node::Orbit == this->Type )
        {
            Ogre::Radian Rad(Radians);
            OgreNode->getParentSceneNode()->yaw(Rad);
        }
    }

    void Node::SetType(Node::NodeType type)
    {
        Type = type;
    }

    Node::NodeType Node::GetType()
    {
        return Type;
    }
}

#endif
