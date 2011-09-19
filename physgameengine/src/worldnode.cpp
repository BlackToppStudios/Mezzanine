
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
#ifndef _worldnode_cpp
#define _worldnode_cpp

#include "worldnode.h"
#include "scenemanager.h"
#include "exception.h"
#include "camera.h"
#include "light.h"
#include "particleeffect.h"
#include "actorbase.h"
#include "world.h"

#include <Ogre.h>

#include <memory>

/// @file worldnode.cpp
/// @brief The implementation of the WorldNode, A class to facilitate navigation in 3d environment


namespace phys
{
    WorldNode::WorldNode(const String& Name, SceneManager* manager)
    {
        Manager = manager;
        if (Manager)
            { OgreNode = Manager->GetGraphicsWorldPointer()->createSceneNode(Name); }
        else
            { OgreNode = World::GetWorldPointer()->GetSceneManager()->GetGraphicsWorldPointer()->createSceneNode(Name); }

        Type = WorldNode::Free;
    }

    WorldNode::WorldNode(Ogre::SceneNode* snode, SceneManager* manager)
    {
        OgreNode = snode;
        Manager = manager;
        Type = WorldNode::Free;
    }

    WorldNode::~WorldNode()
    {
        if (Manager)
            { Manager->GetGraphicsWorldPointer()->destroySceneNode(OgreNode); }
        else
            { World::GetWorldPointer()->GetSceneManager()->GetGraphicsWorldPointer()->destroySceneNode(OgreNode); }
    }

    ///////////////////////////////////////////////////////////////////////////////
    /// Navigation
    void WorldNode::SetLocation(const Vector3& Location)
        { OgreNode->setPosition(Location.GetOgreVector3()); }

    Vector3 WorldNode::GetLocation() const
        { return Vector3(OgreNode->getPosition()); }

    void WorldNode::SetOrientation(Quaternion Orientation)
        { OgreNode->setOrientation(Orientation.GetOgreQuaternion()); }

    Quaternion WorldNode::GetOrientation() const
        { return Quaternion(OgreNode->getOrientation()); }

    void WorldNode::LookAt(Vector3 LookAt)
        { OgreNode->lookAt(LookAt.GetOgreVector3(), Ogre::Node::TS_WORLD); }

    void WorldNode::IncrementOrbit(Real Radians)
    {
        if( WorldNode::Orbit == this->Type )
        {
            Ogre::Radian Rad(Radians);
            OgreNode->getParentSceneNode()->yaw(Rad);
        }
    }

    ///////////////////////////////////////////////////////////////////////////////
    /// Auto tracking
    void WorldNode::SetAutoTracking(WorldNode* node, Vector3 Offset)
    {
        OgreNode->setAutoTracking(true, node->OgreNode, Ogre::Vector3::NEGATIVE_UNIT_Z, Offset.GetOgreVector3());
    }

    void WorldNode::SetAutoTracking(ActorBase* Actor, Vector3 Offset)
    {
        OgreNode->setAutoTracking(true, Actor->GraphicsNode, Ogre::Vector3::NEGATIVE_UNIT_Z, Offset.GetOgreVector3());
    }

    void WorldNode::DisableAutoTracking()
    {
        OgreNode->setAutoTracking(false);
    }

    ///////////////////////////////////////////////////////////////////////////////
    /// Basic Data

    void WorldNode::SetType(WorldNode::NodeType type)
    {
        Type = type;
    }

    WorldNode::NodeType WorldNode::GetType() const
    {
        return Type;
    }

    Attachable::AttachableElement WorldNode::GetAttachableType() const
        { return Attachable::WorldNode; }

    ConstString& WorldNode::GetName() const
        { return OgreNode->getName(); }

    ///////////////////////////////////////////////////////////////////////////////
    /// Attachment child management

    void WorldNode::AttachObject(Attachable* Target)
        { Target->AttachTo(this); }

    void WorldNode::DetachObject(Attachable* Target)
        { Target->DetachFrom(); }

    void WorldNode::AttachObjectFinal(Attachable* Target)
    {
        if(Target==this)
            { World::GetWorldPointer()->LogAndThrow("Cannot Attach WorldNode To itself"); }
        Target->AttachToFinal(OgreNode, this);
        Elements.push_back(Target);
    }

    void WorldNode::DetachObjectFinal(Attachable* Target)
    {
        Target->DetachFromFinal(OgreNode);
        for( std::vector< Attachable* >::iterator it = Elements.begin() ; it != Elements.end() ; it++ )
        {
            if( Target == (*it) )
            {
                Elements.erase(it);
                return;
            }
        }
    }

    Attachable* WorldNode::GetAttached(Whole Index) const
        { return Elements.at(Index); }

    WorldNode::iterator WorldNode::begin()
        { return Elements.begin(); }

    WorldNode::iterator WorldNode::end()
        { return Elements.end(); }

    WorldNode::const_iterator WorldNode::begin() const
        { return Elements.begin(); }

    WorldNode::const_iterator WorldNode::end() const
        { return Elements.end(); }

    void WorldNode::DetachAll()
    {
        //OgreNode->detachAllObjects();
        for( std::vector< Attachable* >::iterator it = Elements.begin() ; it != Elements.end() ; it++ )
            { this->DetachObject(*it); }
        Elements.clear();
    }

    Whole WorldNode::GetNumAttached() const
        { return Elements.size(); }


    ///////////////////////////////////////////////////////////////////////////////
    /// Attachment Parent management

    void WorldNode::AttachToFinal(Ogre::SceneNode* RawTarget, phys::WorldNode* Target)
    {
        Attachable::AttachToFinal(RawTarget, Target);

        if(this->OgreNode->getParent())
            { this->OgreNode->getParentSceneNode()->removeChild(this->OgreNode); }
        RawTarget->addChild(this->OgreNode);
    }

    void WorldNode::DetachFromFinal(Ogre::SceneNode* RawTarget)
    {
        Attachable::DetachFromFinal(RawTarget);
        RawTarget->removeChild(this->OgreNode);
    }

}

///////////////////////////////////////////////////////////////////////////////
// Class External << Operators for streaming or assignment
#ifdef PHYSXML
std::ostream& operator << (std::ostream& stream, const phys::WorldNode& Ev)
{
    stream      << "<WorldNode Version=\"1\" Name=\"" << Ev.GetName()
                << "\" AttachedTo=\"" << ( Ev.GetAttachedTo() ? Ev.GetAttachedTo()->GetName() : "" )
                << "\" StandingType=\"" << Ev.GetType()
                << "\">";

                for(phys::WorldNode::const_iterator Iter = Ev.begin(); Iter!=Ev.end(); ++Iter)
                {
                    stream << "<Attached Name=\"" << (*Iter)->GetName() << "\"/>";
                }

    stream      << "<Orientation>" << Ev.GetOrientation() << "</Orientation>"
                << "<Location>" << Ev.GetLocation() << "</Location>"
                << "</WorldNode>";
    return stream;
}

std::istream& PHYS_LIB operator >> (std::istream& stream, phys::WorldNode& Ev)
{
    phys::String OneTag( phys::xml::GetOneTag(stream) );
    std::auto_ptr<phys::xml::Document> Doc( phys::xml::PreParseClassFromSingleTag("phys::", "WorldNode", OneTag) );

    Doc->GetFirstChild() >> Ev;

    return stream;
}

phys::xml::Node& operator >> (const phys::xml::Node& OneNode, phys::WorldNode& Ev)
{
    if ( phys::String(OneNode.Name())==phys::String("WorldNode") )
    {
        if(OneNode.GetAttribute("Version").AsInt() == 1)
        {
            phys::WorldNode * AttachPtr = phys::World::GetWorldPointer()->GetSceneManager()->GetNode( OneNode.GetAttribute("AttachedTo").AsString() );
            if (AttachPtr)
                { AttachPtr->AttachObject(&Ev); }

            phys::Vector3 TempVec(0,0,0);
            phys::Quaternion TempQuat(0,0,0,0);
            for(phys::xml::Node Child = OneNode.GetFirstChild(); Child!=0; Child = Child.GetNextSibling())
            {
                phys::String Name(Child.Name());
                switch(Name[0])
                {
                    case 'A':
                        if(Name==phys::String("Attached"))
                        {
                            phys::String AttributeName(OneNode.GetAttribute("Name").AsString());
                            phys::WorldNode * AttachPtr = phys::World::GetWorldPointer()->GetSceneManager()->GetNode( Child.GetAttribute("Name").AsString() );

                            if (AttachPtr)  // fail silently, because if we don't find it then that means it just hasn't been deserialized yeat
                            {
                                if (0==AttachPtr->GetAttachedTo())// unless it is attached to something else
                                {
                                    Ev.AttachObject(AttachPtr);
                                }else{
                                    throw( phys::Exception(phys::StringCat("Cannot reconcile WorldNode with the current state of the world: Attachable \"",AttachPtr->GetName()," needs to be attached, but is already attached to ",AttachPtr->GetAttachedTo()->GetName() )) );
                                }
                            }

                        }else{
                            throw( phys::Exception(phys::StringCat("Incompatible XML Version for WorldNode: Includes unknown Element D-\"",Name,"\"")) );
                        }
                        break;
                    case 'O':   //Orientation
                        if(Name==phys::String("Orientation"))
                        {
                            Child.GetFirstChild() >> TempQuat;
                            Ev.SetOrientation(TempQuat);
                        }else{
                            throw( phys::Exception(phys::StringCat("Incompatible XML Version for WorldNode: Includes unknown Element D-\"",Name,"\"")) );
                        }
                        break;
                    case 'L':   //Location
                        if(Name==phys::String("Location"))
                        {
                            Child.GetFirstChild() >> TempVec;
                            Ev.SetLocation(TempVec);
                        }else{
                            throw( phys::Exception(phys::StringCat("Incompatible XML Version for WorldNode: Includes unknown Element L-\"",Name,"\"")) );
                        }
                        break;
                    default:
                        throw( phys::Exception(phys::StringCat("Incompatible XML Version for WorldNode: Includes unknown Element default-\"",Name,"\"")) );
                        break;
                }
            }
        }else{
            throw( phys::Exception("Incompatible XML Version for WorldNode: Not Version 1"));
        }
    }else{
        throw( phys::Exception(phys::StringCat("Attempting to deserialize a WorldNode, found a ", OneNode.Name())));
    }

}
#endif //PHYSXML



#endif
