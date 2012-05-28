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
#ifndef _worldobjectphysicssettings_cpp
#define _worldobjectphysicssettings_cpp

#include "worldobjectphysicssettings.h"
#include "worldobject.h"
#include "collisionshape.h"
#include "collisionshapemanager.h"
#include "collision.h"
#include "physicsmanager.h"
#include "serialization.h"
#include "stringtool.h"
#include "xml.h"

#ifdef MEZZXML
#include <memory>
#endif

#include "btBulletDynamicsCommon.h"
#include "BulletSoftBody/btSoftRigidDynamicsWorld.h"

namespace Mezzanine
{
    WorldObjectPhysicsSettings::WorldObjectPhysicsSettings(WorldObject* WO, btCollisionObject* PhysicsObject)
        : Parent(WO),
          WorldObjectCO(PhysicsObject)
    {

    }

    WorldObjectPhysicsSettings::~WorldObjectPhysicsSettings()
    {

    }

    void WorldObjectPhysicsSettings::AssignShape(CollisionShape* Shape)
    {
        this->WorldObjectShape = Shape;
        Parent->PhysicsShape = Shape->GetBulletShape();
    }

    void WorldObjectPhysicsSettings::SetCollisionShape(CollisionShape* Shape)
    {
        AssignShape(Shape);
        WorldObjectCO->setCollisionShape(Shape->GetBulletShape());
    }

    CollisionShape* WorldObjectPhysicsSettings::GetCollisionShape() const
    {
        return WorldObjectShape;
    }

    void WorldObjectPhysicsSettings::SetCollisionGroupAndMask(const Whole& Group, const Whole& Mask)
    {
        CollisionGroup = Group;
        CollisionMask = Mask;
        if(Parent->IsInWorld())
        {
            Parent->RemoveFromWorld();
            Parent->AddToWorld();
        }
    }

    Whole WorldObjectPhysicsSettings::GetCollisionGroup() const
    {
        return CollisionGroup;
    }

    Whole WorldObjectPhysicsSettings::GetCollisionMask() const
    {
        return CollisionMask;
    }

    bool WorldObjectPhysicsSettings::IsKinematic() const
    {
        return WorldObjectCO->isKinematicObject();
    }

    bool WorldObjectPhysicsSettings::IsStatic() const
    {
        return WorldObjectCO->isStaticObject();
    }

    bool WorldObjectPhysicsSettings::IsStaticOrKinematic() const
    {
        return WorldObjectCO->isStaticOrKinematicObject();
    }

    bool WorldObjectPhysicsSettings::GetCollisionResponse() const
    {
        return !(WorldObjectCO->getCollisionFlags() & btCollisionObject::CF_NO_CONTACT_RESPONSE);
    }

    bool WorldObjectPhysicsSettings::IsActive() const
    {
        int Activation = WorldObjectCO->getActivationState();
        if( ACTIVE_TAG == Activation ) return true;
        else return false;
    }

    void WorldObjectPhysicsSettings::SetActivationState(const WorldObjectActivationState& State, bool Force)
    {
        if(Force) WorldObjectCO->forceActivationState(State);
        else WorldObjectCO->setActivationState(State);
    }

    WorldObjectActivationState WorldObjectPhysicsSettings::GetActivationState() const
    {
        return (Mezzanine::WorldObjectActivationState)WorldObjectCO->getActivationState();
    }

#ifdef MEZZXML
    ///////////////////////////////////////////////////////////////////////////////
    // Serialization
    void WorldObjectPhysicsSettings::ProtoSerialize(xml::Node& CurrentRoot) const
    {
        xml::Node BaseNode = CurrentRoot.AppendChild(this->WorldObjectPhysicsSettings::SerializableName());
            if (!BaseNode)
                { SerializeError("Create BaseNode", SerializableName()); }

            Mezzanine::xml::Attribute Version = BaseNode.AppendAttribute("Version");                            // Version
            if (!Version)
                { SerializeError("Create Version Attribute", SerializableName()); }
            Version.SetValue(1);

            Mezzanine::xml::Attribute CollisionShape = BaseNode.AppendAttribute("CollisionShape");
            if (!CollisionShape)
                { SerializeError("Create CollisionShape Attribute", SerializableName()); }
            CollisionShape.SetValue(this->GetCollisionShape()->GetName());

            Mezzanine::xml::Attribute Kinematic = BaseNode.AppendAttribute("Kinematic");
            if (!Kinematic)
                { SerializeError("Create Kinematic Attribute", SerializableName()); }
            Kinematic.SetValue(this->IsKinematic());

            Mezzanine::xml::Attribute Static = BaseNode.AppendAttribute("Static");
            if (!Static)
                { SerializeError("Create Static Attribute", SerializableName()); }
            Static.SetValue(this->IsStatic());

            Mezzanine::xml::Attribute CollisionResponse = BaseNode.AppendAttribute("CollisionResponse");
            if (!CollisionResponse)
                { SerializeError("Create CollisionResponse Attribute", SerializableName()); }
            CollisionResponse.SetValue(this->GetCollisionResponse());

            Mezzanine::xml::Attribute CollisionGroup = BaseNode.AppendAttribute("CollisionGroup");
            if (!CollisionGroup)
                { SerializeError("Create CollisionGroup Attribute", SerializableName()); }
            CollisionGroup.SetValue(this->GetCollisionGroup());

            Mezzanine::xml::Attribute CollisionMask = BaseNode.AppendAttribute("CollisionMask");
            if (!CollisionMask)
                { SerializeError("Create CollisionMask Attribute", SerializableName()); }
            CollisionMask.SetValue(this->GetCollisionMask());

            Mezzanine::xml::Attribute ActivationState = BaseNode.AppendAttribute("ActivationState");
            if (!ActivationState)
                { SerializeError("Create ActivationState Attribute", SerializableName()); }
            ActivationState.SetValue(this->GetActivationState());
    }

    void WorldObjectPhysicsSettings::ProtoDeSerialize(const xml::Node& OneNode)
    {
        if ( Mezzanine::String(OneNode.Name())==this->WorldObjectPhysicsSettings::SerializableName() )
            {
                if(OneNode.GetAttribute("Version").AsInt() == 1)
                {
                    CollisionShape* Shapeptr = Mezzanine::CollisionShapeManager::GetSingletonPtr()->GetShape(  OneNode.GetAttribute("CollisionShape").AsString());
                    if(!Shapeptr)
                        { DeSerializeError("Find the correct Collision Shape",this->WorldObjectPhysicsSettings::SerializableName()); }
                    this->SetCollisionShape( Shapeptr );
                    if (OneNode.GetAttribute("Kinematic").AsBool())
                    {
                        int Flags = WorldObjectCO->getCollisionFlags();
                        WorldObjectCO->setCollisionFlags(Flags | btCollisionObject::CF_KINEMATIC_OBJECT);
                    }
                    if (OneNode.GetAttribute("Static").AsBool())
                    {
                        int Flags = WorldObjectCO->getCollisionFlags();
                        WorldObjectCO->setCollisionFlags(Flags | btCollisionObject::CF_STATIC_OBJECT);
                    }
                    this->SetCollisionGroupAndMask(OneNode.GetAttribute("CollisionGroup").AsWhole(),OneNode.GetAttribute("CollisionMask").AsWhole());
                    this->SetActivationState((Mezzanine::WorldObjectActivationState)OneNode.GetAttribute("ActivationState").AsInt());
                }else{
                    MEZZ_EXCEPTION(Exception::INVALID_VERSION_EXCEPTION,"Incompatible XML Version for" + WorldObjectPhysicsSettings::SerializableName() + ": Not Version 1.");
                }
            }else{
                MEZZ_EXCEPTION(Exception::II_IDENTITY_INVALID_EXCEPTION,"Attempting to deserialize a " + WorldObjectPhysicsSettings::SerializableName() + ", found a " + String(OneNode.Name()) + ".");
            }
    }

    String WorldObjectPhysicsSettings::SerializableName()
        { return String("WorldObjectBasePhysicsSettings"); }
#endif

    ///////////////////////////////////////////////////////////////////////////////
    // NonTriggerPhysicsSettings functions
    ///////////////////////////////////////

    NonTriggerPhysicsSettings::NonTriggerPhysicsSettings(WorldObject* WO, btCollisionObject* PhysicsObject)
        : WorldObjectPhysicsSettings(WO,PhysicsObject)
    {
    }

    NonTriggerPhysicsSettings::~NonTriggerPhysicsSettings()
    {
    }

    void NonTriggerPhysicsSettings::SetCollisionResponse(bool Enable)
    {
        if(Enable == this->GetCollisionResponse())
            return;
        switch (Parent->GetType())
        {
            case Mezzanine::WSO_ActorRigid:
            case Mezzanine::WSO_ActorSoft:
            {
                if(Enable) WorldObjectCO->setCollisionFlags(WorldObjectCO->getCollisionFlags() + btCollisionObject::CF_NO_CONTACT_RESPONSE);
                else WorldObjectCO->setCollisionFlags(WorldObjectCO->getCollisionFlags() - btCollisionObject::CF_NO_CONTACT_RESPONSE);
                break;
            }
            case Mezzanine::WSO_ActorCharacter:
            {
                return;
                break;
            }
            default:
                return;
        }
    }

    void NonTriggerPhysicsSettings::SetFriction(const Real& Friction)
        { WorldObjectCO->setFriction(Friction); }

    Real NonTriggerPhysicsSettings::GetFriction() const
        { return WorldObjectCO->getFriction(); }

    void NonTriggerPhysicsSettings::SetRestitution(const Real& Restitution)
        { WorldObjectCO->setRestitution(Restitution); }

    Real NonTriggerPhysicsSettings::GetRestitution() const
        { return WorldObjectCO->getRestitution(); }

#ifdef MEZZXML
        // Serializable
        void NonTriggerPhysicsSettings::ProtoSerialize(xml::Node& CurrentRoot) const
        {
            xml::Node BaseNode = CurrentRoot.AppendChild(this->NonTriggerPhysicsSettings::SerializableName());
            if (!BaseNode)
                { SerializeError("Create BaseNode", SerializableName()); }

            Mezzanine::xml::Attribute Version = BaseNode.AppendAttribute("Version");                            // Version
            if (!Version)
                { SerializeError("Create Version Attribute", SerializableName()); }
            Version.SetValue(1);

            Mezzanine::xml::Attribute Friction = BaseNode.AppendAttribute("Friction");
            if (!Friction)
                { SerializeError("Create Friction Attribute", SerializableName()); }
            Friction.SetValue(this->GetFriction());

            Mezzanine::xml::Attribute Restitution = BaseNode.AppendAttribute("Restitution");
            if (!Restitution)
                { SerializeError("Create Restitution Attribute", SerializableName()); }
            Restitution.SetValue(this->GetRestitution());

            WorldObjectPhysicsSettings::ProtoSerialize(BaseNode);
        }

        // DeSerializable
        void NonTriggerPhysicsSettings::ProtoDeSerialize(const xml::Node& OneNode)
        {
            if ( Mezzanine::String(OneNode.Name())==this->NonTriggerPhysicsSettings::SerializableName() )
            {
                if(OneNode.GetAttribute("Version").AsInt() == 1)
                {
                    WorldObjectPhysicsSettings::ProtoDeSerialize(OneNode.GetChild(this->WorldObjectPhysicsSettings::SerializableName()));

                    this->SetFriction(OneNode.GetAttribute("Friction").AsReal());
                    this->SetRestitution(OneNode.GetAttribute("Restitution").AsReal());
                }else{
                    MEZZ_EXCEPTION(Exception::INVALID_VERSION_EXCEPTION,"Incompatible XML Version for" + NonTriggerPhysicsSettings::SerializableName() + ": Not Version 1.");
                }
            }else{
                MEZZ_EXCEPTION(Exception::II_IDENTITY_INVALID_EXCEPTION,"Attempting to deserialize a " + NonTriggerPhysicsSettings::SerializableName() + ", found a " + String(OneNode.Name()) + ".");
            }
        }

        String NonTriggerPhysicsSettings::SerializableName()
            { return String("NonTriggerPhysicsSettings"); }
#endif
}

#endif
