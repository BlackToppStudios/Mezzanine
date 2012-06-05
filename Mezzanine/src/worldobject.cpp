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
#ifndef _worldobject_cpp
#define _worldobject_cpp

#include "worldobject.h"
#include "serialization.h"
#include "audiomanager.h"
#include "Audio/soundset.h"
#include "physicsmanager.h"
#include "scenemanager.h"
#include "worldnode.h"
#include "world.h"

#include <Ogre.h>
#include "btBulletDynamicsCommon.h"
#include <sstream>
#include <memory>

namespace Mezzanine
{
    WorldObject::WorldObject()
        : GraphicsObject(NULL),
          GraphicsNode(NULL),
          PhysicsObject(NULL),
          PhysicsShape(NULL),
          GraphicsSettings(NULL),
          PhysicsSettings(NULL),
          ObjectSounds(NULL)
    {
        this->GraphicsNode = SceneManager::GetSingletonPtr()->GetGraphicsWorldPointer()->getRootSceneNode()->createChildSceneNode();
    }

    WorldObject::~WorldObject()
    {
    }

    void WorldObject::AttachToGraphics()
    {
        if(GraphicsNode && GraphicsObject)
        {
            Vector3 tempv(PhysicsObject->getWorldTransform().getOrigin());
            Quaternion tempq(PhysicsObject->getWorldTransform().getRotation());
            this->GraphicsNode->setPosition(tempv.GetOgreVector3());
            this->GraphicsNode->setOrientation(tempq.GetOgreQuaternion());
            this->GraphicsNode->attachObject(this->GraphicsObject);
        }
    }

    void WorldObject::DetachFromGraphics()
    {
        if(GraphicsNode && GraphicsObject)
            this->GraphicsNode->detachObject(this->GraphicsObject);
    }

    void WorldObject::SetLocation(const Real& x, const Real& y, const Real& z)
    {
        Vector3 temp(x,y,z);
        this->SetLocation(temp);
    }

    void WorldObject::SetLocation(const Vector3& Location)
    {
        //Bullet
        this->PhysicsObject->getWorldTransform().setOrigin(Location.GetBulletVector3());
        this->PhysicsObject->getInterpolationWorldTransform().setOrigin(Location.GetBulletVector3());
        //Ogre
        this->GraphicsNode->setPosition(Location.GetOgreVector3());
    }

    Vector3 WorldObject::GetLocation() const
    {
        Vector3 temp(this->PhysicsObject->getWorldTransform().getOrigin());
        return temp;
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Utility and Configuration
    ///////////////////////////////////////

    ConstString& WorldObject::GetName() const
    {
        return Name;
    }

    bool WorldObject::IsStaticOrKinematic() const
    {
        return PhysicsSettings->IsStaticOrKinematic();
    }

    bool WorldObject::IsInWorld() const
    {
        return PhysicsObject->getBroadphaseHandle() != 0;
    }

    void WorldObject::SetScaling(const Vector3& Scale)
    {
        this->GraphicsNode->setScale(Scale.GetOgreVector3());
        this->PhysicsShape->setLocalScaling(Scale.GetBulletVector3());
    }

    Vector3 WorldObject::GetScaling() const
    {
        Vector3 Scale(this->PhysicsShape->getLocalScaling());
        return Scale;
    }

    WorldObjectGraphicsSettings* WorldObject::GetGraphicsSettings() const
    {
        return GraphicsSettings;
    }

    WorldObjectPhysicsSettings* WorldObject::GetPhysicsSettings() const
    {
        return PhysicsSettings;
    }

    void WorldObject::SetSounds(Audio::SoundSet* Sounds)
    {
        ObjectSounds = Sounds;
    }

    Audio::SoundSet* WorldObject::GetSounds() const
    {
        return ObjectSounds;
    }

    ///////////////////////////////////
    // Internal Object Access functions

    void WorldObject::_NotifyCollisionState(Physics::Collision* Col, const Physics::Collision::CollisionState& State)
    {
        if(Physics::Collision::Col_Begin == State)
        {
            CurrentCollisions.insert(Col);
        }
        else if(Physics::Collision::Col_End == State)
        {
            std::set<Physics::Collision*>::iterator ColIt = CurrentCollisions.find(Col);
            if( ColIt != CurrentCollisions.end() ) CurrentCollisions.erase(ColIt);
        }
    }

    btCollisionObject* WorldObject::_GetBasePhysicsObject() const
    {
        return PhysicsObject;
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

    void WorldObject::ProtoSerialize(XML::Node& CurrentRoot) const
    {
        XML::Node WorldObjectNode = CurrentRoot.AppendChild("WorldObject");
        if (!WorldObjectNode) { ThrowSerialError("create WorldObjectNode");}

        XML::Node LocationNode = WorldObjectNode.AppendChild("Location");
        if (!LocationNode) { ThrowSerialError("create LocationNode"); }
        this->GetLocation().ProtoSerialize(LocationNode);

        XML::Node ScalingNode = WorldObjectNode.AppendChild("Scaling");
        if (!ScalingNode) { ThrowSerialError("create ScalingNode"); }
        this->GetScaling().ProtoSerialize(ScalingNode);

        this->GetGraphicsSettings()->ProtoSerialize(WorldObjectNode);
        this->GetPhysicsSettings()->ProtoSerialize(WorldObjectNode);

        XML::Attribute WorldObjectName = WorldObjectNode.AppendAttribute("Name");
            WorldObjectName.SetValue(this->GetName());
        XML::Attribute WorldObjectVersion = WorldObjectNode.AppendAttribute("Version");
            WorldObjectVersion.SetValue(1);
        XML::Attribute WorldObjectIsInWorld = WorldObjectNode.AppendAttribute("IsInWorld");
            WorldObjectIsInWorld.SetValue(this->IsInWorld());
        if ( !(WorldObjectName && WorldObjectVersion && WorldObjectIsInWorld) )
            { ThrowSerialError("create WorldObjectNode Attributes"); }

        XML::Attribute WorldObjectSoundSetName = WorldObjectNode.AppendAttribute("SoundSet");
        if(this->GetSounds())
        {
            WorldObjectSoundSetName.SetValue(this->GetSounds()->GetName());
        }else{
            WorldObjectSoundSetName.SetValue("");
        }
    }

    void WorldObject::ProtoDeSerialize(const XML::Node& OneNode)
    {
        if ( Mezzanine::String(OneNode.Name())==this->WorldObject::SerializableName() )
        {
            if(OneNode.GetAttribute("Version").AsInt() == 1)
            {
                Vector3 TempVec;
                XML::Node LocationNode = OneNode.GetChild("Location").GetFirstChild();
                if(!LocationNode)
                    { DeSerializeError("locate Location node",SerializableName()); }
                TempVec.ProtoDeSerialize(LocationNode);
                this->SetLocation(TempVec);

                XML::Node GraphicsSettingsNode = OneNode.GetChild(this->GraphicsSettingsSerializableName());
                if(!GraphicsSettingsNode)
                    { DeSerializeError("locate Graphics Settings node",SerializableName()); }
                this->GetGraphicsSettings()->ProtoDeSerialize(GraphicsSettingsNode);

                XML::Node PhysicsSettingsNode = OneNode.GetChild(this->PhysicsSettingsSerializableName());
                if(!PhysicsSettingsNode)
                    { DeSerializeError(String("locate Physics Settings node, ")+this->PhysicsSettingsSerializableName()+", ",SerializableName()); }
                this->GetPhysicsSettings()->ProtoDeSerialize(PhysicsSettingsNode);

                XML::Node ScalingNode = OneNode.GetChild("Scaling").GetFirstChild();
                if(!ScalingNode)
                    { DeSerializeError("locate Scaling node",SerializableName()); }
                TempVec.ProtoDeSerialize(ScalingNode);
                this->SetScaling(TempVec);

                if( this->IsInWorld() != OneNode.GetAttribute("IsInWorld").AsBool() )
                {
                    if(this->IsInWorld())
                        { this->RemoveFromWorld(); }
                    else
                        { this->AddToWorld(); }
                }

                if( 0!=OneNode.GetAttribute("SoundSet") && ""!=OneNode.GetAttribute("SoundSet").AsString())
                    { this->ObjectSounds = AudioManager::GetSingletonPtr()->GetSoundSet(OneNode.GetAttribute("SoundSet").AsString()); }
                else
                    { this->ObjectSounds = 0; }
            }else{
                DeSerializeError("find usable serialization version",SerializableName());
            }
        }else{
            DeSerializeError(String("find correct class to deserialize, found a ")+OneNode.Name(),SerializableName());
        }
    }

    String WorldObject::SerializableName()
        {   return String("WorldObject"); }

    ///////////////////////////////////////////////////////////////////////////////
    // NonStaticWorldObject functions
    ///////////////////////////////////////

    NonStaticWorldObject::NonStaticWorldObject()
    {
    }

    NonStaticWorldObject::~NonStaticWorldObject()
    {
    }

    void NonStaticWorldObject::InternalSetOrientation(const Quaternion& Rotation)
    {
        //Bullet
        this->PhysicsObject->getWorldTransform().setRotation(Rotation.GetBulletQuaternion(true));
        this->PhysicsObject->getInterpolationWorldTransform().setRotation(Rotation.GetBulletQuaternion(true));
        //Ogre
        this->GraphicsNode->setOrientation(Rotation.GetOgreQuaternion());
    }

    void NonStaticWorldObject::SetLocation(const Vector3& Location)
    {
        WorldObject::SetLocation(Location);

        _RecalculateAllChildTransforms();
    }

    Vector3 NonStaticWorldObject::GetLocation() const
    {
        return WorldObject::GetLocation();
    }

    void NonStaticWorldObject::SetOrientation(const Real& x, const Real& y, const Real& z, const Real& w)
    {
        Quaternion temp(x,y,z,w);
        this->SetOrientation(temp);
    }

    void NonStaticWorldObject::SetOrientation(const Quaternion& Rotation)
    {
        InternalSetOrientation(Rotation);

        _RecalculateAllChildTransforms();
    }

    Quaternion NonStaticWorldObject::GetOrientation() const
    {
        Quaternion temp(PhysicsObject->getWorldTransform().getRotation());
        return temp;
    }

    ConstString& NonStaticWorldObject::GetName() const
    {
        return WorldObject::GetName();
    }

    void NonStaticWorldObject::SetScaling(const Vector3& Scale)
    {
        WorldObject::SetScaling(Scale);

        _RecalculateAllChildTransforms();
    }

    Vector3 NonStaticWorldObject::GetScaling() const
    {
        return WorldObject::GetScaling();
    }

    void NonStaticWorldObject::ThrowSerialError(const String& Fail) const
        { SerializeError(Fail, SerializableName()); }

    void NonStaticWorldObject::ProtoSerialize(XML::Node& CurrentRoot) const
    {
        XML::Node NonStaticWorldObjectNode = CurrentRoot.AppendChild("NonStaticWorldObject");
        if (!NonStaticWorldObjectNode) { ThrowSerialError("create NonStaticWorldObjectNode");}

        XML::Attribute Version = NonStaticWorldObjectNode.AppendAttribute("Version");
        Version.SetValue(1);

        XML::Node OrientationNode = NonStaticWorldObjectNode.AppendChild("Orientation");
        if(!OrientationNode)  { ThrowSerialError("create OrientationNode"); }

        this->GetOrientation().ProtoSerialize(OrientationNode);

        // if actor node is in scenemanager just save a name
        /*if( SceneManager::GetSingletonPtr()->GetNode( this->ObjectWorldNode->GetName() ) )
        {
            XML::Attribute NonStaticWorldObjectNodeAttrib = NonStaticWorldObjectNode.AppendAttribute("WorldNode");
            if(!NonStaticWorldObjectNodeAttrib.SetValue(this->ObjectWorldNode->GetName()))
                {ThrowSerialError("store WorldNode Name");}
        }else{
            SloppyProtoSerialize( *(this->ObjectWorldNode),NonStaticWorldObjectNode);                                   //Serialization performed in older style
        }// */

        WorldObject::ProtoSerialize(NonStaticWorldObjectNode);
    }

    void NonStaticWorldObject::ProtoDeSerialize(const XML::Node& OneNode)
    {
        if ( Mezzanine::String(OneNode.Name())==this->NonStaticWorldObject::SerializableName() )
        {
            if(OneNode.GetAttribute("Version").AsInt() == 1)
            {
                this->WorldObject::ProtoDeSerialize(OneNode.GetChild(this->WorldObject::SerializableName()));

                Quaternion TempQuat;
                XML::Node OrientationNode = OneNode.GetChild("Orientation").GetFirstChild();
                if(!OrientationNode)
                    { DeSerializeError("locate Orientation node",SerializableName()); }
                TempQuat.ProtoDeSerialize(OrientationNode);
                this->SetOrientation(TempQuat);

                /*if(0==OneNode.GetAttribute("WorldNode"))         // Are we dealing with a WorldNode Node or WorldNode Attribute.
                {
                    //Since the Attribute didn't exist we must have a node
                    XML::Node ObjectWorldNode = OneNode.GetChild("WorldNode");                               // Assumption made base on old style serialization
                    if(!ObjectWorldNode)
                        { DeSerializeError("locate ObjectWorldNode node",SerializableName()); }
                    if (0!=this->ObjectWorldNode && !SceneManager::GetSingletonPtr()->GetNode(this->ObjectWorldNode->GetName()) )    //If the current worldnode is not null and it is not in the manager, then delete it
                        { delete this->ObjectWorldNode; }
                    this->ObjectWorldNode = new WorldNode(ObjectWorldNode.GetAttribute("Name").AsString(),0);
                    ObjectWorldNode >> *(this->ObjectWorldNode);                                              // Deserialized with old style serialization
                }else{
                    WorldNode *TempWorldNode = SceneManager::GetSingletonPtr()->GetNode(OneNode.GetAttribute("WorldNode").AsString());
                    if( TempWorldNode == this->ObjectWorldNode )
                        { return; }                                                                         //This already has the correct node we are done
                    if (0!=this->ObjectWorldNode && !SceneManager::GetSingletonPtr()->GetNode(this->ObjectWorldNode->GetName()) )    //If the current worldnode is not null and it is not in the manager, then delete it
                        { delete this->ObjectWorldNode; }
                    this->ObjectWorldNode = TempWorldNode;                                                   // The old node has bee cleaned up and the new node is in place
                    if (0==this->ObjectWorldNode)
                        { DeSerializeError("locate ObjectWorldNode attribute",SerializableName()); }
                }//*/
            }else{
                DeSerializeError("find usable serialization version",SerializableName());
            }
        }else{
            DeSerializeError(String("find correct class to deserialize, found a ")+OneNode.Name(),SerializableName());
        }
    }

    String NonStaticWorldObject::SerializableName()
        { return String("NonStaticWorldObject"); }
#endif
}//Mezzanine

#endif
