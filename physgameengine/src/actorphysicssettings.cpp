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
#ifndef _actorphysicssettings_cpp
#define _actorphysicssettings_cpp

#include "actorphysicssettings.h"
#include "actorbase.h"
#include "actorrigid.h"
#include "actorsoft.h"
#include "world.h"

#ifdef PHYSXML
#include <memory>
#endif

#include "btBulletDynamicsCommon.h"
#include "BulletSoftBody/btSoftRigidDynamicsWorld.h"

namespace phys
{
    ActorBasePhysicsSettings::ActorBasePhysicsSettings(ActorBase* Actor, btCollisionObject* PhysicsObject)
        : Parent(Actor),
          ActorCO(PhysicsObject),
          CollisionGroup(0),
          CollisionMask(0)
    {
    }

    ActorBasePhysicsSettings::~ActorBasePhysicsSettings()
    {
    }

    void ActorBasePhysicsSettings::SetCollisionGroupAndMask(const Whole& Group, const Whole& Mask)
    {
        CollisionGroup = Group;
        CollisionMask = Mask;
        if(Parent->IsInWorld())
        {
            World* GameWorld = World::GetWorldPointer();
            Parent->RemoveObjectFromWorld(GameWorld);
            Parent->AddObjectToWorld(GameWorld);
        }
    }

    Whole ActorBasePhysicsSettings::GetCollisionGroup() const
    {
        return CollisionGroup;
    }

    Whole ActorBasePhysicsSettings::GetCollisionMask() const
    {
        return CollisionMask;
    }

    void ActorBasePhysicsSettings::SetFriction(const Real& Friction)
    {
        ActorCO->setFriction(Friction);
    }

    Real ActorBasePhysicsSettings::GetFriction() const
    {
        return ActorCO->getFriction();
    }

    void ActorBasePhysicsSettings::SetRestitution(const Real& Restitution)
    {
        ActorCO->setRestitution(Restitution);
    }

    Real ActorBasePhysicsSettings::GetRestitution() const
    {
        return ActorCO->getRestitution();
    }

    void ActorBasePhysicsSettings::SetKinematic()
    {
        ActorCO->setCollisionFlags(btCollisionObject::CF_KINEMATIC_OBJECT);
    }

    bool ActorBasePhysicsSettings::IsKinematic() const
    {
        return ActorCO->isKinematicObject();
    }

    void ActorBasePhysicsSettings::SetStatic()
    {
        ActorCO->setCollisionFlags(btCollisionObject::CF_STATIC_OBJECT);
    }

    bool ActorBasePhysicsSettings::IsStatic() const
    {
        return ActorCO->isStaticObject();
    }

    bool ActorBasePhysicsSettings::IsStaticOrKinematic() const
    {
        return ActorCO->isStaticOrKinematicObject();
    }

    void ActorBasePhysicsSettings::EnableCollisionResponse()
    {
        World* GameWorld = World::GetWorldPointer();
        switch (Parent->GetType())
        {
            case ActorBase::Actorrigid:
            case ActorBase::Actorsoft:
            case ActorBase::Actorterrain:
            {
                if(Parent->IsInWorld())
                {
                    Parent->RemoveObjectFromWorld(GameWorld);
                    ActorCO->setCollisionFlags(ActorCO->getCollisionFlags() - btCollisionObject::CF_NO_CONTACT_RESPONSE);
                    Parent->AddObjectToWorld(GameWorld);
                }else{
                    ActorCO->setCollisionFlags(ActorCO->getCollisionFlags() - btCollisionObject::CF_NO_CONTACT_RESPONSE);
                }
                break;
            }
            case ActorBase::Actorcharacter:
            {
                return;
                break;
            }
            default:
                return;
        }
    }

    void ActorBasePhysicsSettings::DisableCollisionResponse()
    {
        World* GameWorld = World::GetWorldPointer();
        switch (Parent->GetType())
        {
            case ActorBase::Actorrigid:
            case ActorBase::Actorsoft:
            case ActorBase::Actorterrain:
            {
                if(Parent->IsInWorld())
                {
                    Parent->RemoveObjectFromWorld(GameWorld);
                    ActorCO->setCollisionFlags(ActorCO->getCollisionFlags() + btCollisionObject::CF_NO_CONTACT_RESPONSE);
                    Parent->AddObjectToWorld(GameWorld);
                }else{
                    ActorCO->setCollisionFlags(ActorCO->getCollisionFlags() + btCollisionObject::CF_NO_CONTACT_RESPONSE);
                }
                break;
            }
            case ActorBase::Actorcharacter:
            {
                return;
                break;
            }
            default:
                return;
        }
    }

    bool ActorBasePhysicsSettings::GetCollisionResponse() const
    {
        return !(ActorCO->getCollisionFlags() & btCollisionObject::CF_NO_CONTACT_RESPONSE);
    }

    bool ActorBasePhysicsSettings::IsActive() const
    {
        int Activation = ActorCO->getActivationState();
        if( ACTIVE_TAG == Activation )
        {
            return true;
        }else{
            return false;
        }
    }

    void ActorBasePhysicsSettings::SetActivationState(phys::ActorActivationState State, bool Force)
    {
        if(Force)
            ActorCO->forceActivationState(State);
        else
            ActorCO->setActivationState(State);
    }


    ActorRigidPhysicsSettings::ActorRigidPhysicsSettings(ActorRigid* Actor, btRigidBody* PhysicsObject)
        : ActorBasePhysicsSettings(Actor,PhysicsObject),
          RigidParent(Actor),
          ActorRB(PhysicsObject)
    {
        bool Dynamic = !IsStaticOrKinematic();
        CollisionGroup = Dynamic ? phys::CF_GenericFilter : phys::CF_StaticFilter;
        CollisionMask = Dynamic ? phys::CF_AllFilter : phys::CF_AllFilter ^ phys::CF_StaticFilter;
    }

    ActorRigidPhysicsSettings::~ActorRigidPhysicsSettings()
    {
    }

    void ActorRigidPhysicsSettings::SetDamping(const Real& LinDamping, const Real& AngDamping)
    {
        ActorRB->setDamping(LinDamping, AngDamping);
    }

    void ActorRigidPhysicsSettings::SetLinearVelocity(const Vector3& LinVel)
    {
        ActorRB->setLinearVelocity(LinVel.GetBulletVector3());
    }

    Vector3 ActorRigidPhysicsSettings::GetLinearVelocity() const
    {
        Vector3 LinVel(ActorRB->getLinearVelocity());
        return LinVel;
    }

    void ActorRigidPhysicsSettings::SetAngularVelocity(const Vector3& AngVel)
    {
        ActorRB->setAngularVelocity(AngVel.GetBulletVector3());
    }

    Vector3 ActorRigidPhysicsSettings::GetAngularVelocity() const
    {
        Vector3 AngVel(ActorRB->getAngularVelocity());
        return AngVel;
    }

    void ActorRigidPhysicsSettings::SetIndividualGravity(const Vector3& Gravity)
    {
        ActorRB->setGravity(Gravity.GetBulletVector3());
    }

    Vector3 ActorRigidPhysicsSettings::GetIndividualGravity() const
    {
        Vector3 Grav(ActorRB->getGravity());
        return Grav;
    }


    ActorSoftPhysicsSettings::ActorSoftPhysicsSettings(ActorSoft* Actor, btSoftBody* PhysicsObject)
        : ActorBasePhysicsSettings(Actor,PhysicsObject),
          SoftParent(Actor),
          ActorSB(PhysicsObject)
    {
        CollisionGroup = phys::CF_GenericFilter;
        CollisionMask = phys::CF_AllFilter;
    }

    ActorSoftPhysicsSettings::~ActorSoftPhysicsSettings()
    {
    }

}

#ifdef PHYSXML
std::ostream& operator<< (std::ostream& stream, const phys::ActorBasePhysicsSettings& Ev)
{
    stream      << "<ActorBasePhysicsSettings Version=\"1\" "
                << "Friction=\"" << Ev.GetFriction() << "\""
                << "Restitution=\"" << Ev.GetRestitution() << "\""
                << "Kinematic=\"" << Ev.IsKinematic() << "\""
                << "Friction=\"" << Ev.IsStatic() << "\""
                << "CollisionResponse=\"" << Ev.GetCollisionResponse() << "\""
                << "Active=\"" << Ev.IsActive() << "\" />";
    return stream;
}

std::istream& PHYS_LIB operator >> (std::istream& stream, phys::ActorBasePhysicsSettings& Ev)
{
    phys::String OneTag( phys::xml::GetOneTag(stream) );
    std::auto_ptr<phys::xml::Document> Doc( phys::xml::PreParseClassFromSingleTag("phys::", "ActorBasePhysicsSettings", OneTag) );

    Doc->GetFirstChild() >> Ev;

    return stream;
}


phys::xml::Node& operator >> (const phys::xml::Node& OneNode, phys::ActorBasePhysicsSettings& Ev)
{
    /*if ( phys::String(OneNode.Name())==phys::String("ActorBasePhysicsSettings") )
    {
        if(OneNode.GetAttribute("Version").AsInt() == 1)
        {

        }else{
            throw( phys::Exception("Incompatible XML Version for ActorBasePhysicsSettings: Not Version 1"));
        }
    }else{
        throw( phys::Exception(phys::StringCat("Attempting to deserialize a ActorBasePhysicsSettings, found a ", OneNode.Name())));
    }*/
}

std::ostream& operator << (std::ostream& stream, const phys::ActorRigidPhysicsSettings& Ev)
{
    stream      << "<ActorRigidPhysicsSettings Version=\"1\" "
                << "Friction=\"" << Ev.GetFriction() << "\""
                << "Restitution=\"" << Ev.GetRestitution() << "\""
                << "Kinematic=\"" << Ev.IsKinematic() << "\""
                << "Friction=\"" << Ev.IsStatic() << "\""
                << ">";

        operator<<(stream, static_cast<const phys::ActorBasePhysicsSettings>(Ev));

    stream      << "</ActorRigidPhysicsSettings>";
    return stream;
}

std::istream& PHYS_LIB operator >> (std::istream& stream, phys::ActorRigidPhysicsSettings& Ev)
{
    phys::String OneTag( phys::xml::GetOneTag(stream) );
    std::auto_ptr<phys::xml::Document> Doc( phys::xml::PreParseClassFromSingleTag("phys::", "ActorRigidPhysicsSettings", OneTag) );

    Doc->GetFirstChild() >> Ev;

    return stream;
}


phys::xml::Node& operator >> (const phys::xml::Node& OneNode, phys::ActorRigidPhysicsSettings& Ev)
{
    /*if ( phys::String(OneNode.Name())==phys::String("ActorRigidPhysicsSettings") )
    {
        if(OneNode.GetAttribute("Version").AsInt() == 1)
        {
            phys::ColourValue TempColour;

            for(phys::Whole Counter = 0; Ev.GetNumSubmeshes()>Counter; ++Counter)
            {
                Ev.SetMaterialAmbient(TempColour, Counter);
                Ev.SetMaterialSpecular(TempColour, Counter);
                Ev.SetMaterialDiffuse(TempColour, Counter);
            }

            for(phys::xml::Node Child = OneNode.GetFirstChild(); Child!=0; Child = Child.GetNextSibling())
            {
                phys::String Name(Child.Name());
                switch(Name[0])
                {
                    case 'A':   //fDiffuseColour
                        if(Name==phys::String("AmbientMaterial"))
                        {
                            Child.GetFirstChild() >> TempColour;
                            Ev.SetMaterialAmbient(TempColour, Child.GetAttribute("Submesh").AsWhole());
                        }else{
                            throw( phys::Exception(phys::StringCat("Incompatible XML Version for ActorRigidPhysicsSettings: Includes unknown Element A-\"",Name,"\"")) );
                        }
                        break;
                    case 'S':   //fDiffuseColour
                        if(Name==phys::String("SpecularMaterial"))
                        {
                            Child.GetFirstChild() >> TempColour;
                            Ev.SetMaterialSpecular(TempColour, Child.GetAttribute("Submesh").AsWhole());
                        }else{
                            throw( phys::Exception(phys::StringCat("Incompatible XML Version for ActorRigidPhysicsSettings: Includes unknown Element S-\"",Name,"\"")) );
                        }
                        break;
                    case 'D':   //fDiffuseColour
                        if(Name==phys::String("DiffuseMaterial"))
                        {
                            Child.GetFirstChild() >> TempColour;
                            Ev.SetMaterialDiffuse(TempColour, Child.GetAttribute("Submesh").AsWhole());
                        }else{
                            throw( phys::Exception(phys::StringCat("Incompatible XML Version for ActorRigidPhysicsSettings: Includes unknown Element D-\"",Name,"\"")) );
                        }
                        break;
                    default:
                        throw( phys::Exception(phys::StringCat("Incompatible XML Version for ActorRigidPhysicsSettings: Includes unknown Element default-\"",Name,"\"")) );
                        break;
                }
            }

        }else{
            throw( phys::Exception("Incompatible XML Version for ActorRigidPhysicsSettings: Not Version 1"));
        }
    }else{
        throw( phys::Exception(phys::StringCat("Attempting to deserialize a ActorRigidPhysicsSettings, found a ", OneNode.Name())));
    }*/
}
#endif // \PHYSXML


#endif
