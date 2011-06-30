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
#ifndef _physicsmanager_cpp
#define _physicsmanager_cpp

#include "linegroup.h"
#include "physicsmanager.h"
#include "world.h"
#include "vector3.h"
#include "actorcontainerbase.h"
#include "actormanager.h"
#include "eventcollision.h"
#include "vector3wactor.h"
#include "areaeffect.h"
#include "eventmanager.h"
#include "trigger.h"

#include <queue>

#include <Ogre.h>

#include <btBulletDynamicsCommon.h>
#include <BulletSoftBody/btSoftRigidDynamicsWorld.h>
#include <BulletSoftBody/btSoftBodyRigidBodyCollisionConfiguration.h>
#include "BulletCollision/Gimpact/btGImpactCollisionAlgorithm.h"
#include <BulletCollision/CollisionDispatch/btGhostObject.h>


namespace phys
{
    /// @internal
    /// @namespace phys::debug
    /// @brief This namespace is for internal debugging tools. In general it shouldn't be used in game code.
    /// @details This whole debug namespace is a dirty hack. This is where internal only classes and functions go
    /// that can and maybe should be ommited from release builds
    namespace debug
    {
        /// @internal
        /// @class InternalDebugDrawer
        /// @brief This is used to draw wireframse for the Physics subsystem
        class InternalDebugDrawer : public btIDebugDraw
        {
            private:
                /// @internal
                /// @brief A pointer to phys::World that this Debug Drawer works with
                World* ParentWorld;

                /// @internal
                /// @brief How many wireframes do you want to keep around on the screen.
                Whole WireFrameCount;

                /// @internal
                /// @brief This queue stores The listing of of the wireframes still to be rendered.
                /// @details This stores an amount of wireframes up to the WireFrameCount. When this class is created or a
                /// new frame rendered a new Line group is a added to this queue.
                std::queue<phys::LineGroup*> WireFrames;

                /// @internal
                /// @brief This stores whether or not to render physics debug lines
                /// @details This stores whether or not to render physics debud lines. 0 = Do not draw anything. 1 = Draw model wireframes.
                /// Later we will add support for contact drawing, individual modeling drawing, etc...
                int DebugDrawing;
            public:
                /// @internal
                /// @brief Basic Constructor
                /// @param ParentWorld_ This is a Pointer to the world to be rendered
                /// @param WireFrameCount_ This sets the amount of previous Wireframes to be rendered, see InternalDebugDrawer::SetWireFrameCount for details.
                /// @details This creates a basic Debug Drawer which works with the phys::World that was passed. With a new
                InternalDebugDrawer(phys::World *ParentWorld_, Whole WireFrameCount_ = 2);

                /// @internal
                /// @brief Destructor
                /// @details This deletes all the Wireframes and will stop wireframe rendering
                ~InternalDebugDrawer();

                /// @internal
                /// @brief This will prepare a line segment for being drawn
                /// @details This adds the points for a line to the internal list of points to be rendered.
                /// @param from The first point of the line
                /// @param to The second point of the line
                /// @param color Currently ignored
                virtual void drawLine(const btVector3& from,const btVector3& to,const btVector3& color);

                /// @internal
                /// @brief This add all the rendering information to the graphics subsystem
                /// @details This sends all the points in the list of lines to the rendering subsystem(currently ogre), where they will stay until deleted
                virtual void PrepareForRendering();

                /// @internal
                /// @brief Sets the amount of previous wireframes to leave visible.
                /// @details This will limit the amount of previous wireframes drawn. Setting this will cause all the extra wireframes to be deleted
                /// InternalDebugDrawer::PrepareForRendering() is next called, which should happen just before everything is rendered.
                /// @param WireFrameCount_ This is a whole number which is limit.
                virtual void SetWireFrameCount(Whole WireFrameCount_);

                /// @internal
                /// @brief This returns the amount of wireframes to be drawn
                /// @details This returns either 2 or the amount last set by InternalDebugDrawer::SetWireFrameCount .
                /// @return This returns a whole number with the wireframe limit.
                virtual Whole GetWireFrameCount();

                /// @internal
                /// @brief Currently Unused
                /// @details Currently Unused
                /// @param PointOnB Currently Unused
                /// @param normalOnB Currently Unused
                /// @param distance Currently Unused
                /// @param lifeTime Currently Unused
                /// @param color Currently Unused
                virtual void drawContactPoint(const btVector3& PointOnB,const btVector3& normalOnB,btScalar distance,int lifeTime,const btVector3& color);

                /// @internal
                /// @brief Used by the physics subsystem to report errors using the renderer
                /// @details We *Believe* that this is used by the physics subsystem to report errors about rendering to the developer/user. As such, we
                /// Have redirected all input from this function to the World::Log function.
                /// @param warningString We *Believe* These are messagesfrom the physics subsystem, and that this should not directly called otherwise
                virtual void reportErrorWarning(const char* warningString);

                /// @internal
                /// @brief Currently Unused
                /// @details Currently Unused
                /// @param location Currently Unused
                /// @param textString Currently Unused
                virtual void draw3dText(const btVector3& location, const char* textString);

                /// @internal
                /// @brief This is used to decide how much the debug render should draw
                /// @details Currently this accepts btIDebugDraw::DBG_NoDebug or btIDebugDraw::DBG_DrawWireframe and setting these will either start or stop
                /// Wireframe rendering. All other btIDebugDraw values are ignored.
                /// @param debugMode An Int which contains either btIDebugDraw::DBG_NoDebug or btIDebugDraw::DBG_DrawWireframe
                virtual void setDebugMode(int debugMode);

                /// @internal
                /// @brief This will return the current debug mode.
                /// @details Currently this can only return btIDebugDraw::DBG_NoDebug or btIDebugDraw::DBG_DrawWireframe
                /// @return Returns the Current debug mode, currently either btIDebugDraw::DBG_NoDebug or btIDebugDraw::DBG_DrawWireframe
                virtual int getDebugMode() const;
        };

        InternalDebugDrawer::InternalDebugDrawer( phys::World *ParentWorld_, Whole WireFrameCount_ )
        {
            this->DebugDrawing = 0;
            this->ParentWorld = ParentWorld_;

            this->WireFrameCount = WireFrameCount_;

            //phys::LineGroup* temp = new phys::LineGroup(this->ParentWorld);
            this->WireFrames.push(new phys::LineGroup(this->ParentWorld));
        }

        InternalDebugDrawer::~InternalDebugDrawer()
        {
            while ( ! this->WireFrames.empty() )
            {
                delete this->WireFrames.front();
                this->WireFrames.pop();
            }
        }

        void InternalDebugDrawer::drawLine(const btVector3& from,const btVector3& to,const btVector3& color)
        {
            phys::LineGroup *myLine = this->WireFrames.back();

            //Convert btVectors to PhysVector3s
            Vector3 LineStart(from);
            Vector3 LineEnd(to);

            myLine->addPoint(LineStart);
            myLine->addPoint(LineEnd);
        }

        void InternalDebugDrawer::drawContactPoint(const btVector3& PointOnB,const btVector3& normalOnB,btScalar distance,int lifeTime,const btVector3& color)
        {

        }

        void InternalDebugDrawer::reportErrorWarning(const char* warningString)
        {
            String temp(warningString);
            this->ParentWorld->Log(temp);
        }

        void InternalDebugDrawer::draw3dText(const btVector3& location,const char* textString)
        {}

        void InternalDebugDrawer::setDebugMode(int debugMode)
        {
            this->DebugDrawing = debugMode;
        }

        int InternalDebugDrawer::getDebugMode() const
        {
            return this->DebugDrawing;
        }

        void InternalDebugDrawer::PrepareForRendering()
        {
            if(!this->WireFrames.empty())
            {
                this->WireFrames.back()->drawLines();
            }

            //Delete extra wireframes
            while ( this->WireFrames.size() > this->WireFrameCount )
            {
                delete this->WireFrames.front();
                this->WireFrames.pop();
            }

            //This will add the Ogre Scene Nodes to the world and set up a
            this->WireFrames.back()->PrepareForRendering();
            this->WireFrames.push(new phys::LineGroup(this->ParentWorld));
        }

        void InternalDebugDrawer::SetWireFrameCount(Whole WireFrameCount_)
        {
            this->WireFrameCount = WireFrameCount_;
        }

        Whole InternalDebugDrawer::GetWireFrameCount()
        {
            return this->WireFrameCount;
        }


    }// /debug



    PhysicsManager::PhysicsManager()
        : BulletDrawer(NULL)
    {
        this->Construct(Vector3(-10.0,-10.0,-10.0), Vector3(10.0,10.0,10.0), 10);
    }

    PhysicsManager::PhysicsManager(const Vector3 &GeographyLowerBounds_, const Vector3 &GeographyUpperbounds_, const unsigned short int &MaxPhysicsProxies_)
        : BulletDrawer(NULL)
    {
        this->Construct(GeographyLowerBounds_, GeographyUpperbounds_, MaxPhysicsProxies_);
    }

    PhysicsManager::~PhysicsManager()
    {
        DestroyAllConstraints();
        DestroyAllAreaEffects();
        DestroyAllPhysicsShapes();
        //Destroy the physical world that we loved and cherished
        delete BulletDynamicsWorld;
        delete BulletDispatcher;
        delete BulletCollisionConfiguration;
        delete BulletSolver;
        delete BulletBroadphase;
        delete BulletDrawer;
    }

    void PhysicsManager::Construct(const Vector3 &GeographyLowerBounds_, const Vector3 &GeographyUpperbounds_, const unsigned short int &MaxPhysicsProxies_)
    {
        this->Priority = -30;
        this->CollisionAge = 1;
        this->Impulse = 1.0;
        this->SimulationPaused = false;

        //instantiate the Physics engine and related items
        //this->PhysicsStepsize = btScalar(1.)/btScalar(60.);

        this->GeographyLowerBounds = GeographyLowerBounds_;
        this->GeographyUpperBounds = GeographyUpperbounds_;
        this->MaxPhysicsProxies = MaxPhysicsProxies_;

        /*this->BulletBroadphase = new btAxisSweep3(  GeographyLowerBounds.GetBulletVector3(),
                                                    GeographyUpperBounds.GetBulletVector3(),
                                                    MaxPhysicsProxies
                                                 );*/
        this->BulletBroadphase = new btDbvtBroadphase();

        this->GhostCallback = new btGhostPairCallback();
        this->BulletBroadphase->getOverlappingPairCache()->setInternalGhostPairCallback(GhostCallback);

        this->BulletSolver = new btSequentialImpulseConstraintSolver;
        //this->BulletCollisionConfiguration = new btDefaultCollisionConfiguration();
        this->BulletCollisionConfiguration = new btSoftBodyRigidBodyCollisionConfiguration();
        this->BulletDispatcher = new btCollisionDispatcher(BulletCollisionConfiguration);
        btGImpactCollisionAlgorithm::registerAlgorithm(BulletDispatcher);

        this->BulletDynamicsWorld = new btSoftRigidDynamicsWorld(
                                                    BulletDispatcher,
                                                    BulletBroadphase,
                                                    BulletSolver,
                                                    BulletCollisionConfiguration);

        this->BulletDynamicsWorld->getWorldInfo().m_dispatcher = this->BulletDispatcher;
        this->BulletDynamicsWorld->getWorldInfo().m_broadphase = this->BulletBroadphase;
        this->BulletDynamicsWorld->getWorldInfo().m_sparsesdf.Initialize();

        this->BulletDynamicsWorld->getDispatchInfo().m_enableSPU = true;
    }

    void PhysicsManager::ProcessAllEffects()
    {
        if( !AreaEffects.empty() )
        {
            for( std::vector<AreaEffect*>::iterator AE = AreaEffects.begin() ; AE != AreaEffects.end() ; AE++ )
            {
                (*AE)->UpdateActorList();
                (*AE)->ApplyEffect();
            }
        }
    }

    void PhysicsManager::ProcessAllTriggers()
    {
        if( !Triggers.empty() )
        {
            for( std::vector<Trigger*>::iterator Trig = Triggers.begin() ; Trig != Triggers.end() ; Trig++ )
            {
                if((*Trig)->ConditionsAreMet())
                    (*Trig)->ApplyTrigger();
            }
        }
    }

    void PhysicsManager::PauseSimulation(bool Pause)
    {
        SimulationPaused = Pause;
    }

    bool PhysicsManager::SimulationIsPaused()
    {
        return SimulationPaused;
    }

    void PhysicsManager::SetGravity(Vector3 pgrav)
    {
        this->BulletDynamicsWorld->setGravity(pgrav.GetBulletVector3());
    }

    Vector3 PhysicsManager::GetGravity()
    {
        Vector3 grav(this->BulletDynamicsWorld->getGravity());
        return grav;
    }

    void PhysicsManager::SetSoftGravity(Vector3 sgrav)
    {
        this->BulletDynamicsWorld->getWorldInfo().m_gravity = sgrav.GetBulletVector3();
    }

    Vector3 PhysicsManager::GetSoftGravity()
    {
        Vector3 sgrav(this->BulletDynamicsWorld->getWorldInfo().m_gravity);
        return sgrav;
    }

    void PhysicsManager::SetIndividualGravity(ActorBase* Actor, Vector3 igrav)
    {
        if (ActorBase::Actorrigid==Actor->GetType())
        {
            btRigidBody* Rigid = static_cast < btRigidBody* >(Actor->CollisionObject);
            Rigid->setGravity(igrav.GetBulletVector3());
        }
    }

    void PhysicsManager::AddConstraint(TypedConstraint* Constraint, bool DisableCollisions)
    {
        this->BulletDynamicsWorld->addConstraint(Constraint->ConstraintBase, DisableCollisions);
        Constraints.push_back(Constraint);
    }

    TypedConstraint* PhysicsManager::GetConstraint(Whole Index)
    {
        return Constraints[Index];
    }

    Whole PhysicsManager::GetNumConstraints()
    {
        return Constraints.size();
    }

    void PhysicsManager::RemoveConstraint(TypedConstraint* Constraint)
    {
        this->BulletDynamicsWorld->removeConstraint(Constraint->ConstraintBase);
        for( std::vector<TypedConstraint*>::iterator Con = Constraints.begin() ; Con < Constraints.end() ; Con++ )
        {
            if( (*Con) == Constraint )
            {
                Constraints.erase(Con);
                return;
            }
        }
    }

    void PhysicsManager::DestroyAllConstraints()
    {
        for( std::vector<TypedConstraint*>::iterator Con = Constraints.begin() ; Con != Constraints.end() ; Con++ )
        {
            this->BulletDynamicsWorld->removeConstraint((*Con)->ConstraintBase);
            delete (*Con);
        }
        Constraints.clear();
    }

    void PhysicsManager::StorePhysicsShape(ActorBase* Actor, const String& ShapeName)
    {
        this->PhysicsShapes[ShapeName] = Actor->Shape;
        Actor->ShapeIsSaved = true;
    }

    void PhysicsManager::ApplyPhysicsShape(ActorBase* Actor, const String& ShapeName)
    {
        int Type = Actor->GetType();
        switch(Type)
        {
            case ActorBase::Actorrigid:
            case ActorBase::Actorterrain:
            {
                btVector3 Inertia(0,0,0);
                btRigidBody* ActorObject = btRigidBody::upcast(Actor->CollisionObject);
                btCollisionShape* ActorShape = this->PhysicsShapes[ShapeName];
                btScalar Mass = ActorObject->getInvMass();
                if(0 != Mass)
                    Mass=1/Mass;
                ActorShape->calculateLocalInertia(Mass,Inertia);
                ActorObject->setCollisionShape(ActorShape);
                Actor->Shape = ActorShape;
                ActorObject->setMassProps(Mass,Inertia);
                ActorObject->updateInertiaTensor();
                Actor->ShapeIsSaved = true;
                return;
            }
            case ActorBase::Actorsoft:
            default:
                return;
        }
    }

    void PhysicsManager::DestroyAllPhysicsShapes()
    {
        for( std::map<String,btCollisionShape*>::iterator PS = PhysicsShapes.begin() ; PS != PhysicsShapes.end() ; PS++ )
            delete (*PS).second;
        PhysicsShapes.clear();
    }

    void PhysicsManager::AddAreaEffect(AreaEffect* AE)
    {
        this->AreaEffects.push_back(AE);
        short CollisionGroup = btBroadphaseProxy::SensorTrigger;
        short CollisionMask = AE->IsStatic() ? btBroadphaseProxy::AllFilter ^ (btBroadphaseProxy::SensorTrigger|btBroadphaseProxy::StaticFilter) : btBroadphaseProxy::AllFilter ^ btBroadphaseProxy::SensorTrigger;
        this->BulletDynamicsWorld->addCollisionObject(AE->Ghost,CollisionGroup,CollisionMask);
    }

    AreaEffect* PhysicsManager::GetAreaEffect(const String& Name)
    {
        for( vector<AreaEffect*>::iterator c = AreaEffects.begin() ; c != AreaEffects.end() ; c++ )
        {
            if ( Name == (*c)->GetName() )
            {
                return *c;
            }
        }
        return NULL;
    }

    void PhysicsManager::RemoveAreaEffect(AreaEffect* AE)
    {
        this->BulletDynamicsWorld->removeCollisionObject(AE->Ghost);
        for( vector<AreaEffect*>::iterator c = AreaEffects.begin() ; c != AreaEffects.end() ; c++ )
        {
            if ( AE == *c )
            {
                AreaEffects.erase(c);
                return;
            }
        }
    }

    void PhysicsManager::DestroyAllAreaEffects()
    {
        for( std::vector<AreaEffect*>::iterator AE = AreaEffects.begin() ; AE != AreaEffects.end() ; AE++ )
        {
            this->BulletDynamicsWorld->removeCollisionObject((*AE)->Ghost);
            delete (*AE);
        }
        AreaEffects.clear();
    }

    void PhysicsManager::AddTrigger(Trigger* Trig)
    {
        Triggers.push_back(Trig);
    }

    Trigger* PhysicsManager::GetTrigger(const String& Name)
    {
        for( vector<Trigger*>::iterator Trig = Triggers.begin() ; Trig != Triggers.end() ; Trig++ )
        {
            if ( Name == (*Trig)->GetName() )
            {
                return *Trig;
            }
        }
        return NULL;
    }

    void PhysicsManager::RemoveTrigger(Trigger* Trig)
    {
        for( vector<Trigger*>::iterator T = Triggers.begin() ; T != Triggers.end() ; T++ )
        {
            if ( Trig == (*T) )
            {
                Triggers.erase(T);
                return;
            }
        }
    }

    void PhysicsManager::DestroyAllTriggers()
    {
        for( std::vector<Trigger*>::iterator Trig = Triggers.begin() ; Trig != Triggers.end() ; Trig++ )
            delete (*Trig);
        Triggers.clear();
    }

    void PhysicsManager::SetCollisionParams(unsigned short int Age, Real Force)
    {
        CollisionAge=Age;
        Impulse=Force;
    }

    unsigned short int PhysicsManager::GetCollisionAge()
    {
        return CollisionAge;
    }

    Real PhysicsManager::GetImpulse()
    {
        return Impulse;
    }

    //Bullet Debug Drawing
    void PhysicsManager::SetDebugPhysicsRendering(int ToBeEnabled)
    {
        if(!BulletDrawer)
        {
            this->BulletDrawer = new debug::InternalDebugDrawer(this->GameWorld);
            this->BulletDynamicsWorld->setDebugDrawer(this->BulletDrawer);
        }

        if(ToBeEnabled)
        {
            this->BulletDynamicsWorld->getDebugDrawer()->setDebugMode(btIDebugDraw::DBG_DrawWireframe);
        }else{
            this->BulletDynamicsWorld->getDebugDrawer()->setDebugMode(btIDebugDraw::DBG_NoDebug);
        }
    }

    int PhysicsManager::GetDebugPhysicsRendering()
    {
        if(!BulletDrawer)
            return 0;

        if(this->BulletDrawer->getDebugMode()==btIDebugDraw::DBG_DrawWireframe)
        {
            return 1;
        }else{
            return 0;
        }
    }

    void PhysicsManager::SetDebugPhysicsWireCount(Whole WireFrameCount_)
    {
        if(!BulletDrawer)
        {
            this->BulletDrawer = new debug::InternalDebugDrawer(this->GameWorld);
            this->BulletDynamicsWorld->setDebugDrawer(this->BulletDrawer);
        }

        this->BulletDrawer->SetWireFrameCount(WireFrameCount_);
    }

    Whole PhysicsManager::GetDebugPhysicsWireCount()
    {
        if(BulletDrawer)
            return this->BulletDrawer->GetWireFrameCount();
        else
            return 0;
    }

    Vector3 PhysicsManager::GetActorOffset(const Vector3WActor &OffsetInfo)
    {
        if(ActorBase::Actorrigid==OffsetInfo.Actor->GetType())
        {
            btRigidBody* Rigid = static_cast< btRigidBody* > (OffsetInfo.Actor->CollisionObject);
            Vector3 Offset(Rigid->getCenterOfMassTransform().inverse() * OffsetInfo.Vector.GetBulletVector3());
            return Offset;
        }else{
            Vector3 Offset(0,0,0);
            return Offset;
        }
    }

    void PhysicsManager::DoMainLoopItems(const Real &TimeElapsed)
    {
        if(SimulationPaused)
            return;
        #ifdef PHYSPROFILE
        static Ogre::Timer* Profiler = new Ogre::Timer();
        Profiler->reset();
        #endif
        ProcessAllEffects();
        #ifdef PHYSPROFILE
        GameWorld->LogStream << "AreaEffects took " << Profiler->getMicroseconds() << " microseconds.";
        GameWorld->Log();
        #endif

        Real FloatTime = TimeElapsed;
        FloatTime *= 0.001;    //Convert from MilliSeconds to Seconds

        Real IdealStep = this->GameWorld->GetTargetFrameTime();
        IdealStep *= 0.001;

        //int MaxSteps = (FloatTime<IdealStep) ? 1 : int(FloatTime/IdealStep+1);
        int MaxSteps = (FloatTime<IdealStep) ? 1 : int(FloatTime/IdealStep+2);  //used 2 simply to be extra safe
        #ifdef PHYSPROFILE
        Profiler->reset();
        #endif
        this->BulletDynamicsWorld->stepSimulation( FloatTime, MaxSteps, IdealStep);
        #ifdef PHYSPROFILE
        GameWorld->LogStream << "StepSimulation() took " << Profiler->getMicroseconds() << " microseconds.";
        GameWorld->Log();
        #endif

        // This is supposedly to speed up the performance of soft bodies, if any are in the simulation.
        //this->BulletDynamicsWorld->getWorldInfo().m_sparsesdf.GarbageCollect();

        #ifdef PHYSPROFILE
        Profiler->reset();
        #endif
        if( this->BulletDrawer && this->BulletDrawer->getDebugMode() )        //this part is responsible for drawing the wireframes
        {
            this->BulletDrawer->PrepareForRendering();
            this->BulletDynamicsWorld->debugDrawWorld();
        }
        #ifdef PHYSPROFILE
        GameWorld->LogStream << "DebugDrawer took " << Profiler->getMicroseconds() << " microseconds.";
        GameWorld->Log();
        #endif

        #ifdef PHYSDEBUG
        this->GameWorld->Log("Checking for Collisions.");
        #endif

        #ifdef PHYSPROFILE
        Profiler->reset();
        #endif
        int numManifolds = BulletDynamicsWorld->getDispatcher()->getNumManifolds();
        for (int i=0;i<numManifolds;i++)
        {
            btPersistentManifold* contactManifold = BulletDynamicsWorld->getDispatcher()->getManifoldByIndexInternal(i);
            int numContacts = contactManifold->getNumContacts();
            for (int j=0;j<numContacts;j++)
            {
                btManifoldPoint& pt = contactManifold->getContactPoint(j);
                if (pt.m_lifeTime==CollisionAge && pt.m_appliedImpulse>=Impulse)
                {
                    btCollisionObject* objectA = static_cast<btCollisionObject*>(contactManifold->getBody0());
                    btCollisionObject* objectB = static_cast<btCollisionObject*>(contactManifold->getBody1());
                    ActorBase* ActA = this->GameWorld->GetActorManager()->GetActorContainer()->FindActor(objectA);
                    ActorBase* ActB = this->GameWorld->GetActorManager()->GetActorContainer()->FindActor(objectB);
                    Vector3 WorldLoc((pt.getPositionWorldOnA() + pt.getPositionWorldOnB()) * 0.5);
                    Vector3 ActALoc(pt.m_localPointA);
                    Vector3 ActBLoc(pt.m_localPointB);
                    EventCollision* ColEvent = new EventCollision(ActA,ActB,ActALoc,ActBLoc,WorldLoc,pt.m_appliedImpulse);
                    //create collision event
                    this->GameWorld->GetEventManager()->AddEvent(ColEvent);
                    #ifdef PHYSDEBUG
                    this->GameWorld->Log("Collision Event:");
                    this->GameWorld->Log(*ColEvent);
                    #endif
                }
            }
        }
        #ifdef PHYSPROFILE
        GameWorld->LogStream << "Contact Manifold Iteration took " << Profiler->getMicroseconds() << " microseconds.";
        GameWorld->Log();

        Profiler->reset();
        #endif
        ProcessAllTriggers();
        #ifdef PHYSPROFILE
        GameWorld->LogStream << "Triggers took " << Profiler->getMicroseconds() << " microseconds.";
        GameWorld->Log();
        #endif
    }

    btSoftRigidDynamicsWorld* PhysicsManager::GetPhysicsWorldPointer()
    {
        return this->BulletDynamicsWorld;
    }

    //Inherited From ManagerBase
    void PhysicsManager::Initialize()
    {
        // This came from the Game init function, and may need to go to a game init.
    }

    void PhysicsManager::DoMainLoopItems()
    {
        this->DoMainLoopItems(this->GameWorld->GetFrameTime());
    }

    ManagerBase::ManagerTypeName PhysicsManager::GetType() const
        { return ManagerBase::PhysicsManager; }
}// \phys

#endif
