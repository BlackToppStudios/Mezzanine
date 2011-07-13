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
#ifndef _physactorbase_cpp
#define _physactorbase_cpp

#include <Ogre.h>
#include "btBulletDynamicsCommon.h"
#include <sstream>
#include <memory>

#include "actorbase.h"
#include "actorrigid.h"
#include "actorterrain.h"
#include "actorsoft.h"
#include "actorgraphicssettings.h"
#include "actorphysicssettings.h"
#include "serializable.h"
#include "soundmanager.h"
#include "world.h"
#include "xml.h"
#include "internalmotionstate.h.cpp"
#include "internalmeshtools.h.cpp"

namespace phys{
    ///////////////////////////////////
    // ActorBase class fuctions
    ActorBase::ActorBase(const String& name, const String& file, const String& group)
        : GraphicsObject(NULL),
          GraphicsSettings(NULL),
          BasePhysicsSettings(NULL),
          MotionState(NULL),
          ActorSounds(NULL),
          Animation(NULL),
          ShapeIsSaved(false),
          ActorType(ActorBase::Actorbase)
    {
        //this->GameWorld = World::GetWorldPointer();
        this->GraphicsNode = World::GetWorldPointer()->GetSceneManager()->GetGraphicsWorldPointer()->getRootSceneNode()->createChildSceneNode();
        this->ActorWorldNode = new WorldNode(GraphicsNode,World::GetWorldPointer()->GetSceneManager());
        this->Shape = new btEmptyShape();
    }

    ActorBase::~ActorBase()
    {
        DetachFromGraphics();
        delete MotionState;
        if(!ShapeIsSaved)
        {
            delete Shape;
        }
        //delete GraphicsObject;
        World::GetWorldPointer()->GetSceneManager()->GetGraphicsWorldPointer()->destroyEntity(GraphicsObject);
        delete ActorWorldNode;
        if(CollisionObject)
        {
            delete CollisionObject;
        }
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
        this->CollisionObject->getWorldTransform().setOrigin(Location.GetBulletVector3());
        this->CollisionObject->getInterpolationWorldTransform().setOrigin(Location.GetBulletVector3());
    }

    Vector3 ActorBase::GetBulletLocation() const
    {
        Vector3 temp(this->CollisionObject->getWorldTransform().getOrigin());
        return temp;
    }

    void ActorBase::SetBulletOrientation(const Quaternion& Rotation)
    {
        this->CollisionObject->getWorldTransform().setRotation(Rotation.GetBulletQuaternion(true));
    }

    Quaternion ActorBase::GetBulletOrientation() const
    {
        Quaternion temp(CollisionObject->getWorldTransform().getRotation());
        return temp;
    }

    ///////////////////////////////////
    // Other Management Functions

    void ActorBase::AttachToGraphics()
    {
        Vector3 tempv(CollisionObject->getWorldTransform().getOrigin());
        Quaternion tempq(CollisionObject->getWorldTransform().getRotation());
        this->GraphicsNode->setPosition(tempv.GetOgreVector3());
        this->GraphicsNode->setOrientation(tempq.GetOgreQuaternion());
        this->GraphicsNode->attachObject(this->GraphicsObject);
    }

    void ActorBase::DetachFromGraphics()
    {
        this->GraphicsNode->detachObject(this->GraphicsObject);
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

    int ActorBase::GetType() const
    {
        return this->ActorType;
    }

    WorldNode* ActorBase::GetActorNode() const
    {
        return ActorWorldNode;
    }

    const bool ActorBase::GetShapeIsSaved() const
    {
        return ShapeIsSaved;
    }

    bool ActorBase::IsInWorld() const
    {
        return CollisionObject->getBroadphaseHandle() != 0;
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
        this->Shape->setLocalScaling(scaling.GetBulletVector3());
    }

    Vector3 ActorBase::GetActorScaling() const
    {
        Vector3 Scale(this->Shape->getLocalScaling());
        return Scale;
    }

    ActorGraphicsSettings* ActorBase::GetGraphicsSettings() const
    {
        return GraphicsSettings;
    }

    ActorBasePhysicsSettings* ActorBase::GetPhysicsSettings() const
    {
        return BasePhysicsSettings;
    }

    ///////////////////////////////////
    // Internal Object Access functions

    btCollisionObject* ActorBase::GetBaseBulletObject() const
    {
        return CollisionObject;
    }

    Ogre::Entity* ActorBase::GetOgreObject() const
    {
        return GraphicsObject;
    }
}// /phys


    // add logic here to identify actor
    //Location
    //orientation
    //graphicssettings
    //physicssettings
    //name
    //Scaling

    //Constructor Items

    //soundset


    //Shapeissaved
    //createshapefrommesh
    //is in world
    //animation?

    //Actor worldnode name

///////////////////////////////////////////////////////////////////////////////
// Class External << Operators for streaming or assignment
#ifdef PHYSXML
std::ostream& operator << (std::ostream& stream, const phys::ActorBase& Ev)
{
    stream      << ""; /*<ActorBase Version=\"1\" Name=\"" << Ev.GetName()
                    << "\" AttachedTo=\"" << ( Ev.GetAttachedTo() ? Ev.GetAttachedTo()->GetName() : "" )
                    << "\" Type=\"" << Ev.GetType()
                    << "\" PowerScale=\"" << Ev.GetPowerScale()
                    << "\" AttenuationRange=\"" << Ev.GetAttenuationRange()
                    << "\" AttenuationConstant=\"" << Ev.GetAttenuationConstant()
                    << "\" AttenuationQuadric=\"" << Ev.GetAttenuationQuadric()
                    << "\" AttenuationLinear=\"" << Ev.GetAttenuationLinear()
                    << "\" SpotActorBaseInnerAngle=\"" << Ev.GetSpotActorBaseInnerAngle()
                    << "\" SpotActorBaseOuterAngle=\"" << Ev.GetSpotActorBaseOuterAngle()
                    << "\" SpotActorBaseFalloff=\"" << Ev.GetSpotActorBaseFalloff()
                << "\">"
                << "<Direction>" << Ev.GetDirection() << "</Direction>"
                << "<Location>" << Ev.GetLocation() << "</Location>"
                << "<Orientation>" << Ev.GetLocation() << "</Orientation>"
                << "</ActorBase>";*/
    return stream;
}

std::istream& PHYS_LIB operator >> (std::istream& stream, phys::ActorBase& Ev)
{
    phys::String OneTag( phys::xml::GetOneTag(stream) );
    std::auto_ptr<phys::xml::Document> Doc( phys::xml::PreParseClassFromSingleTag("phys::", "ActorBase", OneTag) );

    Doc->GetFirstChild() >> Ev;

    return stream;
}

phys::xml::Node& operator >> (const phys::xml::Node& OneNode, phys::ActorBase& Ev)
{
    if ( phys::String(OneNode.Name())==phys::String("ActorBase") )
    {
        if(OneNode.GetAttribute("Version").AsInt() == 1)
        {
        /*    Ev.SetType(static_cast<phys::ActorBase::ActorBaseType>(OneNode.GetAttribute("Type").AsInt()));
            Ev.SetPowerScale(OneNode.GetAttribute("PowerScale").AsReal());
            Ev.SetAttenuation(OneNode.GetAttribute("AttenuationRange").AsReal(), OneNode.GetAttribute("AttenuationConstant").AsReal(), OneNode.GetAttribute("AttenuationLinear").AsReal(), OneNode.GetAttribute("AttenuationQuadric").AsReal());
            Ev.SetSpotActorBaseInnerAngle(OneNode.GetAttribute("SpotActorBaseInnerAngle").AsReal());
            Ev.SetSpotActorBaseOuterAngle(OneNode.GetAttribute("SpotActorBaseOuterAngle").AsReal());
            Ev.SetSpotActorBaseFalloff(OneNode.GetAttribute("SpotActorBaseFalloff").AsReal());
            phys::WorldNode * AttachPtr = phys::World::GetWorldPointer()->GetSceneManager()->GetNode( OneNode.GetAttribute("AttachedTo").AsString() );
            if (AttachPtr)
                { AttachPtr->AttachObject(&Ev); }

            phys::ColourValue TempColour(0,0,0,0);
            phys::Vector3 TempVec(0,0,0);

            for(phys::xml::Node Child = OneNode.GetFirstChild(); Child!=0; Child = Child.GetNextSibling())
            {
                phys::String Name(Child.Name());
                switch(Name[0])
                {
                    case 'f':   //fDiffuseColour
                        if(Name==phys::String("fDiffuseColour"))
                        {
                            Child.GetFirstChild() >> TempColour;
                            Ev.SetDiffuseColour(TempColour);
                        }else{
                            throw( phys::Exception(phys::StringCat("Incompatible XML Version for ActorBase: Includes unknown Element f-\"",Name,"\"")) );
                        }
                        break;
                    case 'S':   //SpecularColour
                        if(Name==phys::String("SpecularColour"))
                        {
                            Child.GetFirstChild() >> TempColour;
                            Ev.SetSpecularColour(TempColour);
                        }else{
                            throw( phys::Exception(phys::StringCat("Incompatible XML Version for ActorBase: Includes unknown Element S-\"",Name,"\"")) );
                        }
                        break;
                    case 'D':   //Direction
                        if(Name==phys::String("Direction"))
                        {
                            Child.GetFirstChild() >> TempVec;
                            Ev.SetDirection(TempVec);
                        }else{
                            throw( phys::Exception(phys::StringCat("Incompatible XML Version for ActorBase: Includes unknown Element D-\"",Name,"\"")) );
                        }
                        break;
                    case 'L':   //Location
                        if(Name==phys::String("Location"))
                        {
                            Child.GetFirstChild() >> TempVec;
                            Ev.SetLocation(TempVec);
                        }else{
                            throw( phys::Exception(phys::StringCat("Incompatible XML Version for ActorBase: Includes unknown Element L-\"",Name,"\"")) );
                        }
                        break;
                    default:
                        throw( phys::Exception(phys::StringCat("Incompatible XML Version for ActorBase: Includes unknown Element default-\"",Name,"\"")) );
                        break;
                }
            }
*/
        }else{
            throw( phys::Exception("Incompatible XML Version for ActorBase: Not Version 1"));
        }
    }else{
        throw( phys::Exception(phys::StringCat("Attempting to deserialize a ActorBase, found a ", OneNode.Name())));
    }
}
#endif // \PHYSXML



#endif
