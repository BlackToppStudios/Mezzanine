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
#ifndef _actorphysicssettings_cpp
#define _actorphysicssettings_cpp

#include "actorphysicssettings.h"
#include "actorbase.h"
#include "actorrigid.h"
#include "actorsoft.h"
#include "collisionshape.h"
#include "collisionshapemanager.h"
#include "constraint.h"
#include "physicsmanager.h"
#include "serialization.h"
#include "world.h"
#include "stringtool.h"

#ifdef MEZZXML
#include <memory>
#endif

#include "btBulletDynamicsCommon.h"
#include "BulletSoftBody/btSoftRigidDynamicsWorld.h"

namespace Mezzanine
{
    ActorBasePhysicsSettings::ActorBasePhysicsSettings(ActorBase* Actor, btCollisionObject* PhysicsObject)
        : NonTriggerPhysicsSettings(Actor,PhysicsObject)
    {
    }

    ActorBasePhysicsSettings::~ActorBasePhysicsSettings()
    {
    }

    void ActorBasePhysicsSettings::SetCCDParams(const Real& MotionThreshold, const Real& SweptSphereRadius)
    {
        if(0==MotionThreshold)
        {
            WorldObjectCO->setCcdMotionThreshold(MotionThreshold);
            WorldObjectCO->setCcdSweptSphereRadius(MotionThreshold);
        }else{
            WorldObjectCO->setCcdMotionThreshold(MotionThreshold);
            if(0==SweptSphereRadius)
            {
                if(WorldObjectShape)
                {
                    btTransform Trans;
                    btVector3 AABBmin, AABBmax, AABBsize;
                    WorldObjectShape->GetBulletShape()->getAabb(Trans,AABBmin,AABBmax);
                    AABBsize = AABBmax - AABBmin;
                    Real ResultRadius = AABBsize.getX() < AABBsize.getY() ? (AABBsize.getX() < AABBsize.getZ() ? AABBsize.getX() : AABBsize.getZ()) : (AABBsize.getY() < AABBsize.getZ() ? AABBsize.getY() : AABBsize.getZ());
                    WorldObjectCO->setCcdSweptSphereRadius(ResultRadius*0.2);
                }else{
                    WorldObjectCO->setCcdSweptSphereRadius(1);
                }
            }else{
                WorldObjectCO->setCcdSweptSphereRadius(SweptSphereRadius);
            }
        }
    }

    Real ActorBasePhysicsSettings::GetCCDMotionThreshold() const
    {
        return WorldObjectCO->getCcdMotionThreshold();
    }

    Real ActorBasePhysicsSettings::GetCCDSphereRadius() const
    {
        return WorldObjectCO->getCcdSweptSphereRadius();
    }

    void ActorBasePhysicsSettings::SetKinematic()
    {
        WorldObjectCO->setCollisionFlags(btCollisionObject::CF_KINEMATIC_OBJECT);
    }

    void ActorBasePhysicsSettings::SetStatic()
    {
        WorldObjectCO->setCollisionFlags(btCollisionObject::CF_STATIC_OBJECT);
    }

    ActorBasePhysicsSettings* ActorBasePhysicsSettings::GetBasePointer()
        { return dynamic_cast<ActorBasePhysicsSettings*>(this); }

#ifdef MEZZXML
        // Serializable
        void ActorBasePhysicsSettings::ProtoSerialize(xml::Node& CurrentRoot) const
        {
            xml::Node BaseNode = CurrentRoot.AppendChild(this->ActorBasePhysicsSettings::SerializableName());
            if (!BaseNode)
                { SerializeError("Create BaseNode", SerializableName()); }

            Mezzanine::xml::Attribute Version = BaseNode.AppendAttribute("Version");                            // Version
            if (!Version)
                { SerializeError("Create Version Attribute", SerializableName()); }
            Version.SetValue(1);

            Mezzanine::xml::Attribute CCDMotionThreshold = BaseNode.AppendAttribute("CCDMotionThreshold");
            if (!CCDMotionThreshold)
                { SerializeError("Create CCDMotionThreshold Attribute", SerializableName()); }
            CCDMotionThreshold.SetValue(this->GetCCDMotionThreshold());

            Mezzanine::xml::Attribute CCDSphereRadius = BaseNode.AppendAttribute("CCDSphereRadius");
            if (!CCDSphereRadius)
                { SerializeError("Create CCDSphereRadius Attribute", SerializableName()); }
            CCDSphereRadius.SetValue(this->GetCCDSphereRadius());

            NonTriggerPhysicsSettings::ProtoSerialize(BaseNode);
        }

        // DeSerializable
        void ActorBasePhysicsSettings::ProtoDeSerialize(const xml::Node& OneNode)
        {
            if ( Mezzanine::String(OneNode.Name())==this->ActorBasePhysicsSettings::SerializableName() )
            {
                if(OneNode.GetAttribute("Version").AsInt() == 1)
                {
                    NonTriggerPhysicsSettings::ProtoDeSerialize(OneNode.GetChild(this->NonTriggerPhysicsSettings::SerializableName()));

                    this->SetCCDParams(OneNode.GetAttribute("CCDMotionThreshold").AsReal(),OneNode.GetAttribute("CCDSphereRadius").AsReal());
                }else{
                    throw( Mezzanine::Exception(String("Incompatible XML Version for")+ this->ActorBasePhysicsSettings::SerializableName() + ": Not Version 1"));
                }
            }else{
                throw( Mezzanine::Exception(StringTool::StringCat("Attempting to deserialize a ", this->ActorBasePhysicsSettings::SerializableName(),", found a ", OneNode.Name())));
            }
        }

        String ActorBasePhysicsSettings::SerializableName()
            { return String("ActorBasePhysicsSettings"); }
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////
// ActorRigidPhysicsSettings

    ActorRigidPhysicsSettings::ActorRigidPhysicsSettings(ActorRigid* Actor, btRigidBody* PhysicsObject)
        : ActorBasePhysicsSettings(Actor,PhysicsObject),
          RigidParent(Actor),
          ActorRB(PhysicsObject),
          StickyContacts(NULL)
    {
        bool Dynamic = !IsStaticOrKinematic();
        CollisionGroup = Dynamic ? Mezzanine::CF_GenericFilter : Mezzanine::CF_StaticFilter;
        CollisionMask = Dynamic ? Mezzanine::CF_AllFilter : Mezzanine::CF_AllFilter ^ Mezzanine::CF_StaticFilter;

        StickyContacts = new StickyData();
    }

    ActorRigidPhysicsSettings::~ActorRigidPhysicsSettings()
    {
        ClearStickyContacts();
        delete StickyContacts;
    }

    void ActorRigidPhysicsSettings::SetCollisionShape(CollisionShape* Shape)
    {
        AssignShape(Shape);
        if(CollisionShape::ST_StaticTriMesh != Shape->GetType())
        {
            btScalar mass = this->ActorRB->getInvMass();
            if(0 != mass)
                mass=1/mass;
            btVector3 inertia(0,0,0);
            Shape->GetBulletShape()->calculateLocalInertia(mass, inertia);
            this->ActorRB->setMassProps(mass,inertia);
            this->ActorRB->setCollisionShape(Shape->GetBulletShape());
            this->ActorRB->updateInertiaTensor();
        }else{
            this->ActorRB->setCollisionShape(Shape->GetBulletShape());
        }
        CollisionShapeManager::GetSingletonPtr()->StoreShape(Shape);
    }

    void ActorRigidPhysicsSettings::SetStickyData(const Whole& MaxNumContacts)
    {
        if(MaxNumContacts > 0) StickyContacts->MaxNumContacts = MaxNumContacts;
        else ClearStickyContacts();
    }

    void ActorRigidPhysicsSettings::ClearStickyContacts()
    {
        if(0 == StickyContacts->StickyConstraints.size())
            return;
        btDiscreteDynamicsWorld* BulletWorld = PhysicsManager::GetSingletonPtr()->GetPhysicsWorldPointer();
        for( std::vector<StickyConstraint*>::iterator SCit = StickyContacts->StickyConstraints.begin() ; SCit != StickyContacts->StickyConstraints.end() ; ++SCit )
        {
            BulletWorld->removeConstraint((*SCit)->GetConstraintBase());

            ActorRigid* OtherActor = (*SCit)->GetActorA() != Parent ? (*SCit)->GetActorA() : (*SCit)->GetActorB();
            StickyData* OtherSettings = OtherActor->GetPhysicsSettings()->GetStickyData();
            for( std::vector<StickyConstraint*>::iterator SCit2 = OtherSettings->StickyConstraints.begin() ; SCit2 != OtherSettings->StickyConstraints.end() ; ++SCit2 )
            {
                if( (*SCit) == (*SCit2) )
                {
                    OtherSettings->StickyConstraints.erase(SCit2);
                    break;
                }
            }

            delete (*SCit);
        }
        StickyContacts->StickyConstraints.clear();
        StickyContacts->CreationQueue.clear();
    }

    StickyData* ActorRigidPhysicsSettings::GetStickyData() const
        { return StickyContacts; }

    void ActorRigidPhysicsSettings::SetDamping(const Real& LinDamping, const Real& AngDamping)
        { ActorRB->setDamping(LinDamping, AngDamping); }

    Real ActorRigidPhysicsSettings::GetLinearDamping() const
        { return ActorRB->getLinearDamping(); }

    Real ActorRigidPhysicsSettings::GetAngularDamping() const
        { return ActorRB->getAngularDamping(); }

    void ActorRigidPhysicsSettings::SetLinearVelocity(const Vector3& LinVel)
        { ActorRB->setLinearVelocity(LinVel.GetBulletVector3()); }

    Vector3 ActorRigidPhysicsSettings::GetLinearVelocity() const
        { return Vector3(ActorRB->getLinearVelocity()); }

    void ActorRigidPhysicsSettings::SetAngularVelocity(const Vector3& AngVel)
        { ActorRB->setAngularVelocity(AngVel.GetBulletVector3()); }

    Vector3 ActorRigidPhysicsSettings::GetAngularVelocity() const
        { return Vector3(ActorRB->getAngularVelocity()); }

    void ActorRigidPhysicsSettings::SetIndividualGravity(const Vector3& Gravity)
        { ActorRB->setGravity(Gravity.GetBulletVector3()); }

    Vector3 ActorRigidPhysicsSettings::GetIndividualGravity() const
        { return Vector3(ActorRB->getGravity()); }

    Vector3 ActorRigidPhysicsSettings::GetForce() const
        { return Vector3(ActorRB->getTotalForce()); }

    Vector3 ActorRigidPhysicsSettings::GetTorque() const
        { return Vector3(ActorRB->getTotalTorque()); }

    void ActorRigidPhysicsSettings::ApplyForce(const Vector3& Force)
        { ActorRB->applyCentralForce(Force.GetBulletVector3()); }

    void ActorRigidPhysicsSettings::ApplyTorque(const Vector3& Torque)
        { ActorRB->applyTorque(Torque.GetBulletVector3()); }

    Real ActorRigidPhysicsSettings::GetMass() const
        { return  ActorRB->getInvMass() != 0 ? 1/ActorRB->getInvMass() : 0; }

    Vector3 ActorRigidPhysicsSettings::GetLocalInertia() const
        { return  Vector3(ActorRB->getInvInertiaDiagLocal()).Inverse() ; }

    void ActorRigidPhysicsSettings::SetMass(Real NewMass)
        { ActorRB->setMassProps(NewMass, GetLocalInertia().GetBulletVector3()); }

    void ActorRigidPhysicsSettings::SetMass(Real NewMass,const Vector3& NewInertia)
        { ActorRB->setMassProps(NewMass, NewInertia.GetBulletVector3()); }

#ifdef MEZZXML
        // Serializable
    void ActorRigidPhysicsSettings::ProtoSerialize(xml::Node& CurrentRoot) const
    {
        xml::Node BaseNode = CurrentRoot.AppendChild(this->ActorRigidPhysicsSettings::SerializableName());
        if (!BaseNode)
            { SerializeError("Create BaseNode", SerializableName()); }

        Mezzanine::xml::Attribute Version = BaseNode.AppendAttribute("Version");                            // Version
        if (!Version)
            { SerializeError("Create Version Attribute", SerializableName()); }
        Version.SetValue(1);

        Mezzanine::xml::Attribute Mass = BaseNode.AppendAttribute("Mass");
        if (!Mass)
            { SerializeError("Create Mass Attribute", SerializableName()); }
        Mass.SetValue(this->GetMass());

        Mezzanine::xml::Attribute AngularDamping = BaseNode.AppendAttribute("AngularDamping");
        if (!AngularDamping)
            { SerializeError("Create AngularDamping Attribute", SerializableName()); }
        AngularDamping.SetValue(this->GetAngularDamping());

        Mezzanine::xml::Attribute LinearDamping = BaseNode.AppendAttribute("LinearDamping");
        if (!LinearDamping)
            { SerializeError("Create LinearDamping Attribute", SerializableName()); }
        LinearDamping.SetValue(this->GetLinearDamping());

        xml::Node AngularVelocity = BaseNode.AppendChild("AngularVelocity");
        if (!AngularVelocity)
            { SerializeError("Create AngularVelocity Node", SerializableName()); }
        this->GetAngularVelocity().ProtoSerialize(AngularVelocity);

        xml::Node LinearVelocity = BaseNode.AppendChild("LinearVelocity");
        if (!LinearVelocity)
            { SerializeError("Create LinearVelocity Node", SerializableName()); }
        this->GetLinearVelocity().ProtoSerialize(LinearVelocity);

        xml::Node IndividualGravity = BaseNode.AppendChild("IndividualGravity");
        if (!IndividualGravity)
            { SerializeError("Create IndividualGravity Node", SerializableName()); }
        this->GetIndividualGravity().ProtoSerialize(IndividualGravity);

        xml::Node TotalTorque = BaseNode.AppendChild("TotalTorque");
        if (!TotalTorque)
            { SerializeError("Create TotalTorque Node", SerializableName()); }
        this->GetTorque().ProtoSerialize(TotalTorque);

        xml::Node TotalForce = BaseNode.AppendChild("TotalForce");
        if (!TotalForce)
            { SerializeError("Create TotalForce Node", SerializableName()); }
        this->GetForce().ProtoSerialize(TotalForce);

        xml::Node Inertia = BaseNode.AppendChild("LocalInertia");
        if (!Inertia)
            { SerializeError("Create Inertia Node", SerializableName()); }
        this->GetLocalInertia().ProtoSerialize(Inertia);

        this->ActorBasePhysicsSettings::ProtoSerialize(BaseNode);
    }

    // DeSerializable
    void ActorRigidPhysicsSettings::ProtoDeSerialize(const xml::Node& OneNode)
    {
        if ( Mezzanine::String(OneNode.Name())==this->ActorRigidPhysicsSettings::SerializableName() )
        {
            if(OneNode.GetAttribute("Version").AsInt() == 1)
            {
                this->SetDamping(OneNode.GetAttribute("LinearDamping").AsReal(),OneNode.GetAttribute("AngularDamping").AsReal());

                Mezzanine::Vector3 TempVec;

                for(Mezzanine::xml::Node Child = OneNode.GetFirstChild(); Child!=0; Child = Child.GetNextSibling())
                {
                    Mezzanine::String Name(Child.Name());
                    switch(Name[5])
                    {
                        case 'B':   //ActorBasePhysicsSettings
                            if(Name==Mezzanine::String("ActorBasePhysicsSettings"))
                            {
                                this->ActorBasePhysicsSettings::ProtoDeSerialize(Child);
                            }else{
                                throw( Mezzanine::Exception(StringTool::StringCat("Incompatible XML Version for ActorRigidPhysicsSettings: Includes unknown Element B-\"",Name,"\"")) );
                            }
                            break;
                        case 'a':   //AngularVelocity
                            if(Name==Mezzanine::String("AngularVelocity"))
                            {
                                Child.GetFirstChild() >> TempVec;
                                this->SetAngularVelocity(TempVec);
                            }else{
                                throw( Mezzanine::Exception(StringTool::StringCat("Incompatible XML Version for ActorRigidPhysicsSettings: Includes unknown Element a-\"",Name,"\"")) );
                            }
                            break;
                        case 'r':   //LinearVelocity
                            if(Name==Mezzanine::String("LinearVelocity"))
                            {
                                Child.GetFirstChild() >> TempVec;
                                this->SetLinearVelocity(TempVec);
                            }else{
                                throw( Mezzanine::Exception(StringTool::StringCat("Incompatible XML Version for ActorRigidPhysicsSettings: Includes unknown Element r-\"",Name,"\"")) );
                            }
                            break;
                        case 'i':   //IndividualGravity
                            if(Name==Mezzanine::String("IndividualGravity"))
                            {
                                Child.GetFirstChild() >> TempVec;
                                this->SetIndividualGravity(TempVec);
                            }else{
                                throw( Mezzanine::Exception(StringTool::StringCat("Incompatible XML Version for ActorRigidPhysicsSettings: Includes unknown Element i-\"",Name,"\"")) );
                            }
                            break;
                        case 'T':   //TotalTorque
                            if(Name==Mezzanine::String("TotalTorque"))
                            {
                                Child.GetFirstChild() >> TempVec;
                                this->ApplyTorque(TempVec);
                            }else{
                                throw( Mezzanine::Exception(StringTool::StringCat("Incompatible XML Version for ActorRigidPhysicsSettings: Includes unknown Element T-\"",Name,"\"")) );
                            }
                            break;
                        case 'F':   //TotalForce
                            if(Name==Mezzanine::String("TotalForce"))
                            {
                                Child.GetFirstChild() >> TempVec;
                                this->ApplyForce(TempVec);
                            }else{
                                throw( Mezzanine::Exception(StringTool::StringCat("Incompatible XML Version for ActorRigidPhysicsSettings: Includes unknown Element F-\"",Name,"\"")) );
                            }
                            break;
                        case 'I':   //Inertia
                            if(Name==Mezzanine::String("LocalInertia"))
                            {
                                Child.GetFirstChild() >> TempVec;
                                this->SetMass(OneNode.GetAttribute("Mass").AsReal(), TempVec);
                            }else{
                                throw( Mezzanine::Exception(StringTool::StringCat("Incompatible XML Version for ActorRigidPhysicsSettings: Includes unknown Element I-\"",Name,"\"")) );
                            }
                            break;
                        default:
                            throw( Mezzanine::Exception(StringTool::StringCat("Incompatible XML Version for Light: Includes unknown Element default-\"",Name,"\"")) );
                            break;
                    }
                }

            }else{
                throw( Mezzanine::Exception(String("Incompatible XML Version for")+ this->ActorRigidPhysicsSettings::SerializableName() + ": Not Version 1"));
            }
        }else{
            throw( Mezzanine::Exception(String("Attempting to deserialize a ") + this->ActorRigidPhysicsSettings::SerializableName() + ", found a ", OneNode.Name()));
        }
    }

    String ActorRigidPhysicsSettings::SerializableName()
        { return String("ActorRigidPhysicsSettings"); }
#endif


///////////////////////////////////////////////////////////////////////////////////////////////////
// ActorSoftPhysicsSettings

    ActorSoftPhysicsSettings::ActorSoftPhysicsSettings(ActorSoft* Actor, btSoftBody* PhysicsObject)
        : ActorBasePhysicsSettings(Actor,PhysicsObject),
          SoftParent(Actor),
          ActorSB(PhysicsObject)
    {
        CollisionGroup = Mezzanine::CF_GenericFilter;
        CollisionMask = Mezzanine::CF_AllFilter;
    }

    ActorSoftPhysicsSettings::~ActorSoftPhysicsSettings()
    {
    }

    void ActorSoftPhysicsSettings::SetCollisionShape(CollisionShape* Shape)
    {
        // do nothing, cause soft bodies get unique custom shapes.
    }

} // \Namespace Mezzanine

#ifdef MEZZXML
std::ostream& operator<< (std::ostream& stream, const Mezzanine::ActorBasePhysicsSettings& Ev)
    { return Serialize(stream, Ev); }

std::istream& MEZZ_LIB operator >> (std::istream& stream, Mezzanine::ActorBasePhysicsSettings& Ev)
    { return DeSerialize(stream,Ev); }

void operator >> (const Mezzanine::xml::Node& OneNode, Mezzanine::ActorBasePhysicsSettings& Ev)
    { Ev.ProtoDeSerialize(OneNode); }

std::ostream& operator << (std::ostream& stream, const Mezzanine::ActorRigidPhysicsSettings& Ev)
    { return Serialize(stream, Ev); }

std::istream& MEZZ_LIB operator >> (std::istream& stream, Mezzanine::ActorRigidPhysicsSettings& Ev)
    { return DeSerialize(stream,Ev); }

void operator >> (const Mezzanine::xml::Node& OneNode, Mezzanine::ActorRigidPhysicsSettings& Ev)
    { Ev.ProtoDeSerialize(OneNode); }
#endif // \MEZZXML

#endif
