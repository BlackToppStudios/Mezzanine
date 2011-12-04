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

#ifndef _actorbase_cpp
#define _actorbase_cpp

#include <Ogre.h>
#include "btBulletDynamicsCommon.h"
#include <sstream>
#include <memory>

#include "actorbase.h"
#include "actorrigid.h"
#include "actorsoft.h"
#include "actorphysicssettings.h"
#include "serialization.h"
#include "audiomanager.h"
#include "audiosoundset.h"
#include "physicsmanager.h"
#include "world.h"
#include "xml.h"
#include "internalmotionstate.h.cpp"
#include "internalmeshtools.h.cpp"

/// @file
/// @brief Code used by all actors is implemented here.

namespace Mezzanine
{
    ///////////////////////////////////
    // ActorBase class fuctions
    ActorBase::ActorBase()
        : BasePhysicsSettings(NULL),
          MotionState(NULL),
          ActorSounds(NULL),
          Animation(NULL)
    {
        //this->GameWorld = World::GetWorldPointer();
        this->GraphicsNode = SceneManager::GetSingletonPtr()->GetGraphicsWorldPointer()->getRootSceneNode()->createChildSceneNode();
        this->ActorWorldNode = new WorldNode(GraphicsNode,SceneManager::GetSingletonPtr());
    }

    ActorBase::~ActorBase()
    {
        PhysicsManager* PhysMan = PhysicsManager::GetSingletonPtr();
        SceneManager* SceneMan = SceneManager::GetSingletonPtr();

        DetachFromGraphics();
        delete MotionState;
        //delete GraphicsObject;
        SceneMan->GetGraphicsWorldPointer()->destroyEntity(GraphicsObject);
        delete PhysicsObject;

        if (SceneMan)
        {
            if (0!=this->ActorWorldNode && !SceneMan->GetNode(this->ActorWorldNode->GetName()) )    //If the current worldnode is not null and it is not in the manager, then delete it
            { delete this->ActorWorldNode; }
        } else
            { delete this->ActorWorldNode; }
    }

    ///////////////////////////////////
    // Ogre Management Functions

    void ActorBase::SetOgreLocation(const Vector3& Location)
    {
        this->GraphicsNode->setPosition(Location.GetOgreVector3());
    }

    Vector3 ActorBase::GetOgreLocation() const
    {
        Vector3 temp(this->GraphicsNode->getPosition());
        return temp;
    }

    void ActorBase::SetOgreOrientation(const Quaternion& Rotation)
    {
        this->GraphicsNode->setOrientation(Rotation.GetOgreQuaternion());
    }

    Quaternion ActorBase::GetOgreOrientation() const
    {
        Quaternion temp(GraphicsNode->getOrientation());
        return temp;
    }

    ///////////////////////////////////
    // Bullet Management Functions

    void ActorBase::SetBulletLocation(const Vector3& Location)
    {
        //btTransform* temp = this->CollisionObject->getWorldTransform();
        this->PhysicsObject->getWorldTransform().setOrigin(Location.GetBulletVector3());
        this->PhysicsObject->getInterpolationWorldTransform().setOrigin(Location.GetBulletVector3());
    }

    Vector3 ActorBase::GetBulletLocation() const
    {
        Vector3 temp(this->PhysicsObject->getWorldTransform().getOrigin());
        return temp;
    }

    void ActorBase::SetBulletOrientation(const Quaternion& Rotation)
    {
        this->PhysicsObject->getWorldTransform().setRotation(Rotation.GetBulletQuaternion(true));
    }

    Quaternion ActorBase::GetBulletOrientation() const
    {
        Quaternion temp(PhysicsObject->getWorldTransform().getRotation());
        return temp;
    }

    ///////////////////////////////////
    // Creation, Destruction and Initialization

    void ActorBase::SetLocation(const Real& x, const Real& y, const Real& z)
    {
        Vector3 temp(x,y,z);
        this->SetLocation(temp);
    }

    void ActorBase::SetLocation(const Vector3& Place)
    {
        this->SetBulletLocation(Place);
        this->SetOgreLocation(Place);
    }

    Vector3 ActorBase::GetLocation() const
    {
        return this->GetBulletLocation();
    }

    void ActorBase::SetOrientation(const Real& x, const Real& y, const Real& z, const Real& w)
    {
        Quaternion temp(x,y,z,w);
        this->SetOrientation(temp);
    }

    void ActorBase::SetOrientation(const Quaternion& Rotation)
    {
        this->SetBulletOrientation(Rotation);
        this->SetOgreOrientation(Rotation);
    }

    Quaternion ActorBase::GetOrientation() const
    {
        return this->GetBulletOrientation();
    }

    ///////////////////////////////////
    // Utility and Configuration

    WorldNode* ActorBase::GetActorNode() const
    {
        return ActorWorldNode;
    }

    bool ActorBase::IsStaticOrKinematic() const
    {
        return BasePhysicsSettings->IsStaticOrKinematic();
    }

    void ActorBase::SetAnimation(ConstString& AnimationName, bool Loop)
    {
        if(this->IsAnimated())
        {
            Animation->setEnabled(false);
        }
        Animation = GraphicsObject->getAnimationState(AnimationName);
        Animation->setLoop(Loop);
    }

    void ActorBase::EnableAnimation(bool Enable)
    {
        if(Animation)
        {
            Animation->setEnabled(Enable);
        }
    }

    bool ActorBase::IsAnimated() const
    {
        if(Animation)
        {
            return Animation->getEnabled();
        }else{
            return false;
        }
    }

    void ActorBase::AdvanceAnimation(const Real& Time)
    {
        if(Animation)
        {
            Animation->addTime(Time);
        }
    }

    void ActorBase::RemoveSetAnimation()
    {
        if(Animation)
        {
            Animation->setEnabled(false);
            Animation = NULL;
        }
    }

    void ActorBase::SetActorScaling(const Vector3& scaling)
    {
        this->GraphicsNode->setScale(scaling.GetOgreVector3());
        this->PhysicsShape->setLocalScaling(scaling.GetBulletVector3());
    }

    Vector3 ActorBase::GetActorScaling() const
    {
        Vector3 Scale(this->PhysicsShape->getLocalScaling());
        return Scale;
    }

    ActorBasePhysicsSettings* ActorBase::GetPhysicsSettings() const
    {
        return BasePhysicsSettings;
    }

///////////////////////////////////////////////////////////////////////////////
// Serialization
///////////////////////////////////////
#ifdef MEZZXML
    void ActorBase::ThrowSerialError(const String& Fail) const
        { SerializeError(Fail, SerializableName()); }

    void ActorBase::ProtoSerialize(xml::Node& CurrentRoot) const
    {
        xml::Node ActorNode = CurrentRoot.AppendChild("ActorBase");
        if (!ActorNode) { ThrowSerialError("create ActorNode");}

        xml::Node LocationNode = ActorNode.AppendChild("Location");
        if (!LocationNode) { ThrowSerialError("create LocationNode"); }
        this->GetLocation().ProtoSerialize(LocationNode);

        xml::Node ScalingNode = ActorNode.AppendChild("Scaling");
        if (!ScalingNode) { ThrowSerialError("create ScalingNode"); }
        this->GetActorScaling().ProtoSerialize(ScalingNode);

        xml::Node OrientationNode = ActorNode.AppendChild("Orientation");
        if(!OrientationNode)  { ThrowSerialError("create OrientationNode"); }

        this->GetOrientation().ProtoSerialize(OrientationNode);
        this->GetGraphicsSettings()->ProtoSerialize(ActorNode);
        this->GetPhysicsSettings()->ProtoSerialize(ActorNode);

        xml::Attribute ActorName = ActorNode.AppendAttribute("Name");
            ActorName.SetValue(this->GetName());
        xml::Attribute ActorVersion = ActorNode.AppendAttribute("Version");
            ActorVersion.SetValue(1);
        xml::Attribute ActorIsInWorld = ActorNode.AppendAttribute("IsInWorld");
            ActorIsInWorld.SetValue(this->IsInWorld());
        if ( !(ActorName && ActorVersion && ActorIsInWorld) )
            { ThrowSerialError("create ActorNode Attributes"); }

        xml::Attribute ActorSoundSetName = ActorNode.AppendAttribute("SoundSet");
        if(this->ActorSounds)
        {
            ActorSoundSetName.SetValue(this->ActorSounds->GetName());
        }else{
            ActorSoundSetName.SetValue("");
        }

        // if actor node is in scenemanager just save a name
        if( SceneManager::GetSingletonPtr()->GetNode( this->ActorWorldNode->GetName() ) )
        {
            xml::Attribute ActorWorldNode = ActorNode.AppendAttribute("WorldNode");
            if(!ActorWorldNode.SetValue(this->ActorWorldNode->GetName()))
                {ThrowSerialError("store WorldNode Name");}
        }else{
            SloppyProtoSerialize( *(this->ActorWorldNode),ActorNode);                                   //Serialization performed in older style
        }
    }

    void ActorBase::ProtoDeSerialize(const xml::Node& OneNode)
    {
        if ( Mezzanine::String(OneNode.Name())==this->ActorBase::SerializableName() )
        {
            if(OneNode.GetAttribute("Version").AsInt() == 1)
            {
                Vector3 TempVec;
                xml::Node LocationNode = OneNode.GetChild("Location").GetFirstChild();
                if(!LocationNode)
                    { DeSerializeError("locate Location node",SerializableName()); }
                TempVec.ProtoDeSerialize(LocationNode);
                this->SetLocation(TempVec);

                xml::Node GraphicsSettingsNode = OneNode.GetChild(this->GraphicsSettingsSerializableName());
                if(!GraphicsSettingsNode)
                    { DeSerializeError("locate Graphics Settings node",SerializableName()); }
                this->GetGraphicsSettings()->ProtoDeSerialize(GraphicsSettingsNode);

                xml::Node PhysicsSettingsNode = OneNode.GetChild(this->PhysicsSettingsSerializableName());
                if(!PhysicsSettingsNode)
                    { DeSerializeError(String("locate Physics Settings node, ")+this->PhysicsSettingsSerializableName()+", ",SerializableName()); }
                this->GetPhysicsSettings()->ProtoDeSerialize(PhysicsSettingsNode);

                xml::Node ScalingNode = OneNode.GetChild("Scaling").GetFirstChild();
                if(!ScalingNode)
                    { DeSerializeError("locate Scaling node",SerializableName()); }
                TempVec.ProtoDeSerialize(ScalingNode);
                this->SetActorScaling(TempVec);

                Quaternion TempQuat;
                xml::Node OrientationNode = OneNode.GetChild("Orientation").GetFirstChild();
                if(!OrientationNode)
                    { DeSerializeError("locate Orientation node",SerializableName()); }
                TempQuat.ProtoDeSerialize(OrientationNode);
                this->SetOrientation(TempQuat);

                if( this->IsInWorld() != OneNode.GetAttribute("IsInWorld").AsBool() )
                {
                    if(this->IsInWorld())
                        { this->RemoveFromWorld(); }
                    else
                        { this->AddToWorld(); }
                }

                if( 0!=OneNode.GetAttribute("SoundSet") && ""!=OneNode.GetAttribute("SoundSet").AsString())
                    { this->ActorSounds = AudioManager::GetSingletonPtr()->GetSoundSet(OneNode.GetAttribute("SoundSet").AsString()); }
                else
                    { this->ActorSounds = 0; }

                if(0==OneNode.GetAttribute("WorldNode"))         // Are we dealing with a WorldNode Node or WorldNode Attribute.
                {
                    //Since the Attribute didn't exist we must have a node
                    xml::Node ActorWorldNode = OneNode.GetChild("WorldNode");                               // Assumption made base on old style serialization
                    if(!ActorWorldNode)
                        { DeSerializeError("locate ActorWorldNode node",SerializableName()); }
                    if (0!=this->ActorWorldNode && !SceneManager::GetSingletonPtr()->GetNode(this->ActorWorldNode->GetName()) )    //If the current worldnode is not null and it is not in the manager, then delete it
                        { delete this->ActorWorldNode; }
                    this->ActorWorldNode = new WorldNode(ActorWorldNode.GetAttribute("Name").AsString(),0);
                    ActorWorldNode >> *(this->ActorWorldNode);                                              // Deserialized with old style serialization
                }else{
                    WorldNode *TempWorldNode = SceneManager::GetSingletonPtr()->GetNode(OneNode.GetAttribute("WorldNode").AsString());
                    if( TempWorldNode == this->ActorWorldNode )
                        { return; }                                                                         //This already has the correct node we are done
                    if (0!=this->ActorWorldNode && !SceneManager::GetSingletonPtr()->GetNode(this->ActorWorldNode->GetName()) )    //If the current worldnode is not null and it is not in the manager, then delete it
                        { delete this->ActorWorldNode; }
                    this->ActorWorldNode = TempWorldNode;                                                   // The old node has bee cleaned up and the new node is in place
                    if (0==this->ActorWorldNode)
                        { DeSerializeError("locate ActorWorldNode attribute",SerializableName()); }
                }
            }else{
                DeSerializeError("find usable serialization version",SerializableName());
            }
        }else{
            DeSerializeError(String("find correct class to deserialize, found a ")+OneNode.Name(),SerializableName());
        }
    }

    String ActorBase::SerializableName()
        {   return String("ActorBase"); }

#endif  // \mezzxml
}// /mezz



#endif
