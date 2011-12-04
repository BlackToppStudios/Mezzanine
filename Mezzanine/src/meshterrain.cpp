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

#ifndef _meshterrain_cpp
#define _meshterrain_cpp

#include <Ogre.h>
#include "btBulletDynamicsCommon.h"
#include "BulletSoftBody/btSoftRigidDynamicsWorld.h"

#include "collisionshape.h"
#include "meshterrain.h"
#include "worldnode.h"
#include "physicsmanager.h"
#include "actorgraphicssettings.h"
#include "objectreference.h"
#include "internalmotionstate.h.cpp"
#include "internalmeshtools.h.cpp"
#include <map>


namespace phys
{
    // Actor Terrain Remnants
     MeshTerrain::MeshTerrain(Vector3 InitPosition, String name, String file, String group)
            : TerrainBase(name),
		  ShapeIsSaved(false)
		  //BasePhysicsSettings(NULL)
    {
        TerrainType = TerrainBase::MeshTerrain;

        // this isn't required to operate, but it does allow the mesh manager to know what is loaded.
        MeshManager::GetSingletonPtr()->LoadMesh(file,group);

		// From Actorbase
		this->GraphicsNode = World::GetWorldPointer()->GetSceneManager()->GetGraphicsWorldPointer()->getRootSceneNode()->createChildSceneNode();
        this->TerrainWorldNode = new WorldNode(GraphicsNode,World::GetWorldPointer()->GetSceneManager());
        this->Shape = new btEmptyShape();

		// From TerrainActor
        this->GraphicsObject = World::GetWorldPointer()->GetSceneManager()->GetGraphicsWorldPointer()->createEntity(name, file, group);
        this->MotionState = new internal::PhysMotionState(GraphicsNode);
        this->MotionState->SetPosition(InitPosition);
        CreateCollisionTerrain();
    }

    MeshTerrain::~MeshTerrain()
    {
		// From ActorBase
        DetachFromGraphics();
		delete MotionState;
        if(!ShapeIsSaved)
        {
            delete Shape;
        }
		World::GetWorldPointer()->GetSceneManager()->GetGraphicsWorldPointer()->destroyEntity(GraphicsObject);
		delete TerrainWorldNode;
		if(CollisionObject)
        {
            delete CollisionObject;
        }
    }

	///////////////////////////////////
    // Ogre Management Functions

    void MeshTerrain::SetOgreLocation(const Vector3& Location)
    {
        this->GraphicsNode->setPosition(Location.GetOgreVector3());
    }

    Vector3 MeshTerrain::GetOgreLocation() const
    {
        Vector3 temp(this->GraphicsNode->getPosition());
        return temp;
    }

    void MeshTerrain::SetOgreOrientation(const Quaternion& Rotation)
    {
        this->GraphicsNode->setOrientation(Rotation.GetOgreQuaternion());
    }

    Quaternion MeshTerrain::GetOgreOrientation() const
    {
        Quaternion temp(GraphicsNode->getOrientation());
        return temp;
    }

	///////////////////////////////////
    // Bullet Management Functions

    void MeshTerrain::SetBulletLocation(const Vector3& Location)
    {
        //btTransform* temp = this->CollisionObject->getWorldTransform();
        this->CollisionObject->getWorldTransform().setOrigin(Location.GetBulletVector3());
        this->CollisionObject->getInterpolationWorldTransform().setOrigin(Location.GetBulletVector3());
    }

    Vector3 MeshTerrain::GetBulletLocation() const
    {
        Vector3 temp(this->CollisionObject->getWorldTransform().getOrigin());
        return temp;
    }

    void MeshTerrain::SetBulletOrientation(const Quaternion& Rotation)
    {
        this->CollisionObject->getWorldTransform().setRotation(Rotation.GetBulletQuaternion(true));
    }

    Quaternion MeshTerrain::GetBulletOrientation() const
    {
        Quaternion temp(CollisionObject->getWorldTransform().getRotation());
        return temp;
    }

	///////////////////////////////////
    // Other Management Functions

    void MeshTerrain::AttachToGraphics()
    {
        Vector3 tempv(CollisionObject->getWorldTransform().getOrigin());
        Quaternion tempq(CollisionObject->getWorldTransform().getRotation());
        this->GraphicsNode->setPosition(tempv.GetOgreVector3());
        this->GraphicsNode->setOrientation(tempq.GetOgreQuaternion());
        this->GraphicsNode->attachObject(this->GraphicsObject);
    }

    void MeshTerrain::DetachFromGraphics()
    {
        this->GraphicsNode->detachObject(this->GraphicsObject);
    }

	///////////////////////////////////
    // Initialization

    void MeshTerrain::SetLocation(const Real& x, const Real& y, const Real& z)
    {
        Vector3 temp(x,y,z);
        this->SetLocation(temp);
    }

    void MeshTerrain::SetLocation(const Vector3& Place)
    {
        this->SetBulletLocation(Place);
        this->SetOgreLocation(Place);
    }

    Vector3 MeshTerrain::GetLocation() const
    {
        return this->GetBulletLocation();
    }

    void MeshTerrain::SetOrientation(const Real& x, const Real& y, const Real& z, const Real& w)
    {
        Quaternion temp(x,y,z,w);
        this->SetOrientation(temp);
    }

    void MeshTerrain::SetOrientation(const Quaternion& Rotation)
    {
        this->SetBulletOrientation(Rotation);
        this->SetOgreOrientation(Rotation);
    }

    Quaternion MeshTerrain::GetOrientation() const
    {
        return this->GetBulletOrientation();
    }

    ///////////////////////////////////
    // Utility and Configuration

    TerrainBase::TerrainTypeName MeshTerrain::GetType() const
    {
        return this->TerrainType;
    }

    WorldNode* MeshTerrain::GetTerrainNode() const
    {
        return TerrainWorldNode;
    }

    const bool MeshTerrain::GetShapeIsSaved() const
    {
        return ShapeIsSaved;
    }

    bool MeshTerrain::IsInWorld() const
    {
        return CollisionObject->getBroadphaseHandle() != 0;
    }

    void MeshTerrain::SetScaling(const Vector3& scaling)
    {
        this->GraphicsNode->setScale(scaling.GetOgreVector3());
        this->Shape->setLocalScaling(scaling.GetBulletVector3());
    }

	///////////////////////////////////
    // Internal Object Access functions

    btCollisionObject* MeshTerrain::GetBaseBulletObject() const
    {
        return CollisionObject;
    }

    Ogre::Entity* MeshTerrain::GetOgreObject() const
    {
        return GraphicsObject;
    }

	////////////////////////////////////

    void MeshTerrain::CreateCollisionTerrain()
    {
        CollisionObject = new btCollisionObject();
        btScalar mass = 0.f;
        RigidBody = new btRigidBody(mass, this->MotionState, this->Shape);
        CollisionObject = RigidBody;
        ObjectReference* TerrainRef = new ObjectReference(phys::WOT_MeshTerrain,this);
        Ogre::Any OgreRef(TerrainRef);
        GraphicsObject->setUserAny(OgreRef);
        CollisionObject->setUserPointer(TerrainRef);
    }

    void MeshTerrain::CreateShapeFromMeshStatic(bool UseAllSubmeshes)
    {
        if(!ShapeIsSaved)
        {
            delete Shape;
        }
        /// @todo - Check for thread safety
        btBvhTriangleMeshShape *tmpshape = new btBvhTriangleMeshShape(internal::MeshTools::CreateBulletTrimesh(GraphicsObject,UseAllSubmeshes),true);
        this->Shape=tmpshape;
        ShapeIsSaved = false;
        this->Shape->setLocalScaling(btVector3(1.0,1.0,1.0));
        this->CollisionObject->setCollisionShape(this->Shape);
        this->RigidBody->updateInertiaTensor();
    }

    void MeshTerrain::AddTerrainToWorld ()
    {
        World::GetWorldPointer()->GetPhysicsManager()->GetPhysicsWorldPointer()->addCollisionObject(this->CollisionObject);
        this->AttachToGraphics();
    }

    void MeshTerrain::RemoveTerrainFromWorld()
    {
        World::GetWorldPointer()->GetPhysicsManager()->GetPhysicsWorldPointer()->removeCollisionObject(this->CollisionObject);
        this->DetachFromGraphics();
    }

    btRigidBody* MeshTerrain::GetBulletObject()
    {
        return RigidBody;
    }

    void MeshTerrain::SetCollisionShape( CollisionShape* shape )
    {
        this->CollisionObject->setCollisionShape(shape->GetBulletShape());
    }
}

#endif // _meshterrain_cpp
