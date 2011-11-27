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
#include "actorphysicssettings.h"
#include "eventcollision.h"
#include "vector3wactor.h"
#include "areaeffect.h"
#include "eventmanager.h"
#include "eventcollision.h"
#include "worldtrigger.h"
#include "objectreference.h"

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
    }// debug

    #ifdef GetObject
    #undef GetObject
    #endif

    class CollisionDispatcher : public btCollisionDispatcher
    {
        protected:
            std::list<btPersistentManifold*> ManifoldCreationQueue;
            std::list<PhysicsManager::CollisionIterator> ManifoldDestructionQueue;
        public:
            CollisionDispatcher(btCollisionConfiguration* CollisionConfig)
                : btCollisionDispatcher(CollisionConfig)
            {
            }
            ~CollisionDispatcher()
            {
            }
            btPersistentManifold* getNewManifold(void* b0, void* b1)
            {
                // Get the manifold
                btPersistentManifold* NewManifold = btCollisionDispatcher::getNewManifold(b0,b1);
                // Store the manifold for processing later
                ManifoldCreationQueue.push_back(NewManifold);
                return NewManifold;
            }
            void releaseManifold(btPersistentManifold* manifold)
            {
                // first check the queue
                if(!ManifoldCreationQueue.empty())
                {
                    for(std::list<btPersistentManifold*>::iterator QueIt = ManifoldCreationQueue.begin() ; QueIt != ManifoldCreationQueue.end() ; QueIt++ )
                    {
                        if(manifold == (*QueIt))
                        {
                            ManifoldCreationQueue.erase(QueIt);
                            btCollisionDispatcher::releaseManifold(manifold);
                            return;
                        }
                    }
                }
                // now check the already generated collisions
                PhysicsManager* PhysMan = World::GetWorldPointer()->GetPhysicsManager();
                for( PhysicsManager::CollisionIterator ColIt = PhysMan->Collisions.begin() ; ColIt != PhysMan->Collisions.end() ; ++ColIt )
                {
                    if(manifold == (*ColIt).second->Manifold)
                    {
                        //ManifoldDestructionQueue.push_back(ColIt);
                        //Collision* ToBeDestroyed = (*ColIt).second;
                        //ToBeDestroyed->GetActorA()->_NotifyCollisionState(ToBeDestroyed,Collision::Col_End);
                        //ToBeDestroyed->GetActorB()->_NotifyCollisionState(ToBeDestroyed,Collision::Col_End);
                        delete (*ColIt).second;
                        PhysMan->Collisions.erase(ColIt);
                        //delete ToBeDestroyed;
                        break;
                    }
                }// */
                btCollisionDispatcher::releaseManifold(manifold);
            }
            void releaseManifoldManual(btPersistentManifold* manifold)
            {
                PhysicsManager* PhysMan = World::GetWorldPointer()->GetPhysicsManager();
                for( PhysicsManager::CollisionIterator ColIt = PhysMan->Collisions.begin() ; ColIt != PhysMan->Collisions.end() ; ++ColIt )
                {
                    if(manifold == (*ColIt).second->Manifold)
                    {
                        Collision* ToBeDestroyed = (*ColIt).second;
                        //ToBeDestroyed->GetActorA()->_NotifyCollisionState(ToBeDestroyed,Collision::Col_End);
                        //ToBeDestroyed->GetActorB()->_NotifyCollisionState(ToBeDestroyed,Collision::Col_End);
                        PhysMan->Collisions.erase(ColIt);
                        delete ToBeDestroyed;
                        break;
                    }
                }// */
                btCollisionDispatcher::releaseManifold(manifold);
            }
            std::list<btPersistentManifold*>& GetManifoldCreationQueue()
            {
                return ManifoldCreationQueue;
            }
            std::list<PhysicsManager::CollisionIterator>& GetManifoldDestructionQueue()
            {
                return ManifoldDestructionQueue;
            }
    };// CollisionDispatcher

    PhysicsConstructionInfo::PhysicsConstructionInfo()
        : PhysicsFlags(0),
          MaxProxies(0),
          EventFilterAge(1),
          EventFilterImpulse(1.0),
          GeographyLowerBounds(Vector3()),
          GeographyUpperBounds(Vector3()),
          Gravity(Vector3())
    {
    }

    PhysicsConstructionInfo::~PhysicsConstructionInfo()
    {
    }

    PhysicsConstructionInfo& PhysicsConstructionInfo::operator=(const PhysicsConstructionInfo& Other)
    {
        this->PhysicsFlags = Other.PhysicsFlags;
        this->MaxProxies = Other.MaxProxies;
        this->EventFilterAge = Other.EventFilterAge;
        this->EventFilterImpulse = Other.EventFilterImpulse;
        this->GeographyLowerBounds = Other.GeographyLowerBounds;
        this->GeographyUpperBounds = Other.GeographyUpperBounds;
        this->Gravity = Other.Gravity;
    }

    PhysicsManager::PhysicsManager()
        : BulletDrawer(NULL),
          SimulationPaused(false),
          SubstepModifier(1)
    {
        PhysicsConstructionInfo Info;
        Info.PhysicsFlags = (PhysicsConstructionInfo::PCF_SoftRigidWorld | PhysicsConstructionInfo::PCF_LimitlessWorld);
        this->Construct(Info);
    }

    PhysicsManager::PhysicsManager(const PhysicsConstructionInfo& Info)
        : BulletDrawer(NULL),
          SimulationPaused(false),
          SubstepModifier(1)
    {
        this->Construct(Info);
    }

    PhysicsManager::~PhysicsManager()
    {
        DestroyAllConstraints();
        DestroyAllAreaEffects();
        DestroyAllWorldTriggers();
        //Destroy the physical world that we loved and cherished
        delete BulletDynamicsWorld;
        delete BulletDispatcher;
        delete BulletCollisionConfiguration;
        delete BulletSolver;
        delete BulletBroadphase;
        if(BulletDrawer) delete BulletDrawer;
    }

    void PhysicsManager::Construct(const PhysicsConstructionInfo& Info)
    {
        this->Priority = -30;
        this->CollisionAge = Info.EventFilterAge;
        this->Impulse = Info.EventFilterImpulse;

        if(Info.PhysicsFlags & PhysicsConstructionInfo::PCF_LimitlessWorld)
        {
            this->BulletBroadphase = new btDbvtBroadphase();
        }else{
            if(Info.MaxProxies < 65536)
            {
                this->BulletBroadphase = new btAxisSweep3(Info.GeographyLowerBounds.GetBulletVector3(),
                                                          Info.GeographyUpperBounds.GetBulletVector3(),
                                                          Info.MaxProxies);
            }else{
                this->BulletBroadphase = new bt32BitAxisSweep3(Info.GeographyLowerBounds.GetBulletVector3(),
                                                               Info.GeographyUpperBounds.GetBulletVector3(),
                                                               Info.MaxProxies);
            }
        }

        this->GhostCallback = new btGhostPairCallback();
        this->BulletBroadphase->getOverlappingPairCache()->setInternalGhostPairCallback(GhostCallback);
        this->BulletSolver = new btSequentialImpulseConstraintSolver;

        //if(Info.PhysicsFlags & PhysicsConstructionInfo::PCF_SoftRigidWorld)
        //{
            this->BulletCollisionConfiguration = new btSoftBodyRigidBodyCollisionConfiguration();
            this->BulletDispatcher = new CollisionDispatcher(BulletCollisionConfiguration);
            btGImpactCollisionAlgorithm::registerAlgorithm(BulletDispatcher);

            this->BulletDynamicsWorld = new btSoftRigidDynamicsWorld(
                                                        BulletDispatcher,
                                                        BulletBroadphase,
                                                        BulletSolver,
                                                        BulletCollisionConfiguration);

            this->BulletDynamicsWorld->getWorldInfo().m_dispatcher = this->BulletDispatcher;
            this->BulletDynamicsWorld->getWorldInfo().m_broadphase = this->BulletBroadphase;
            this->BulletDynamicsWorld->getWorldInfo().m_sparsesdf.Initialize();
        /*}else{
            this->BulletCollisionConfiguration = new btDefaultCollisionConfiguration();
            this->BulletDispatcher = new btCollisionDispatcher(BulletCollisionConfiguration);
            btGImpactCollisionAlgorithm::registerAlgorithm(BulletDispatcher);

            this->BulletDynamicsWorld = new btDiscreteDynamicsWorld(
                                                        BulletDispatcher,
                                                        BulletBroadphase,
                                                        BulletSolver,
                                                        BulletCollisionConfiguration);
        }// */
        this->BulletDynamicsWorld->getDispatchInfo().m_enableSPU = true;
        this->BulletDynamicsWorld->getDispatchInfo().m_useContinuous = true;
        //this->BulletDynamicsWorld->getSolverInfo().m_splitImpulse = true;
        this->BulletDynamicsWorld->getSolverInfo().m_numIterations = 20;
        this->SetGravity(Info.Gravity);
        this->SetSoftGravity(Info.Gravity);
        this->WorldConstructionInfo = Info;
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
            for( std::vector<WorldTrigger*>::iterator Trig = Triggers.begin() ; Trig != Triggers.end() ; Trig++ )
            {
                if((*Trig)->ConditionsAreMet())
                    (*Trig)->ApplyTrigger();
            }
        }
    }

    void PhysicsManager::ProcessAllCollisions()
    {
        CollisionDispatcher* Dispatch = (CollisionDispatcher*)this->BulletDispatcher;
        //Destroy the collisions that need to be destroyed
        /*std::list<PhysicsManager::CollisionIterator>& DestroyQueue = Dispatch->GetManifoldDestructionQueue();
        for( std::list<PhysicsManager::CollisionIterator>::iterator MetaIt = DestroyQueue.begin() ; MetaIt != DestroyQueue.end() ; ++MetaIt )
        {
            Collision* ToBeDestroyed = (*(*MetaIt)).second;
            Collisions.erase(*MetaIt);
            delete ToBeDestroyed;
        }
        DestroyQueue.clear();// */
        //Update the collisions that already exist as necessary
        for( PhysicsManager::CollisionIterator ColIt = Collisions.begin() ; ColIt != Collisions.end() ; ColIt++ )
            (*ColIt).second->Update();
        //Process the collisions that are in the creation queue
        std::list<btPersistentManifold*>& ManifoldQueue = Dispatch->GetManifoldCreationQueue();
        if(ManifoldQueue.empty())
            return;
        #ifdef PHYSDEBUG
        std::stringstream logstream;
        logstream << "Processing " << ManifoldQueue.size() << " manifolds for collisions." << endl;
        logstream << "Showing a total of " << BulletDispatcher->getNumManifolds() << " manifolds in the dispatcher." << endl;
        World::GetWorldPointer()->Log(logstream.str());
        //World::GetWorldPointer()->DoMainLoopLogging();
        #endif
        btPersistentManifold* NewManifold = ManifoldQueue.front();
        while( NewManifold != NULL )
        {
            // Get the object references
            ObjectReference* ObjectA = (ObjectReference*)((btCollisionObject*)NewManifold->getBody0())->getUserPointer();
            ObjectReference* ObjectB = (ObjectReference*)((btCollisionObject*)NewManifold->getBody1())->getUserPointer();
            // Verify they are actors, then cast appropriately
            ActorBase* ActorA = (ObjectA->GetType() >= WOT_ActorFirst) && (ObjectA->GetType() <= WOT_ActorLast) ? static_cast<ActorBase*>(ObjectA->GetObject()) : NULL;
            ActorBase* ActorB = (ObjectB->GetType() >= WOT_ActorFirst) && (ObjectB->GetType() <= WOT_ActorLast) ? static_cast<ActorBase*>(ObjectB->GetObject()) : NULL;
            // Verify the cast went well
            if( ActorA && ActorB )
            {
                if( !ActorA->GetPhysicsSettings()->GetCollisionResponse() || !ActorB->GetPhysicsSettings()->GetCollisionResponse() )
                {
                    ManifoldQueue.pop_front();
                    if(ManifoldQueue.size() > 0) NewManifold = ManifoldQueue.front();
                    else NewManifold = NULL;
                    continue;
                }
                // Creat the collision
                ActorPair NewPair(ActorA,ActorB);
                PhysicsManager::CollisionIterator ColIt = Collisions.find(NewPair);
                if(ColIt == Collisions.end())
                {
                    Collision* NewCol = new Collision(ActorA,ActorB,NewManifold);
                    //NewCol->GetActorA()->_NotifyCollisionState(NewCol,Collision::Col_Begin);
                    //NewCol->GetActorB()->_NotifyCollisionState(NewCol,Collision::Col_Begin);
                    Collisions.insert(std::pair<ActorPair,Collision*>(NewPair,NewCol));
                }
            }
            ManifoldQueue.pop_front();
            if(ManifoldQueue.size() > 0) NewManifold = ManifoldQueue.front();
            else NewManifold = NULL;
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

    void PhysicsManager::SetGravity(const Vector3& pgrav)
    {
        this->BulletDynamicsWorld->setGravity(pgrav.GetBulletVector3());
    }

    Vector3 PhysicsManager::GetGravity()
    {
        Vector3 grav(this->BulletDynamicsWorld->getGravity());
        return grav;
    }

    void PhysicsManager::SetSoftGravity(const Vector3& sgrav)
    {
        this->BulletDynamicsWorld->getWorldInfo().m_gravity = sgrav.GetBulletVector3();
    }

    Vector3 PhysicsManager::GetSoftGravity()
    {
        Vector3 sgrav(this->BulletDynamicsWorld->getWorldInfo().m_gravity);
        return sgrav;
    }

    void PhysicsManager::SetIndividualGravity(ActorBase* Actor, const Vector3& igrav)
    {
        if (ActorBase::Actorrigid==Actor->GetType())
        {
            btRigidBody* Rigid = static_cast < btRigidBody* >(Actor->_GetBasePhysicsObject());
            Rigid->setGravity(igrav.GetBulletVector3());
        }
    }

    void PhysicsManager::AddConstraint(TypedConstraint* Constraint, bool DisableCollisions)
    {
        this->BulletDynamicsWorld->addConstraint(Constraint->GetConstraintBase(), DisableCollisions);
        Constraints.push_back(Constraint);
    }

    TypedConstraint* PhysicsManager::GetConstraint(const Whole& Index)
    {
        return Constraints[Index];
    }

    Whole PhysicsManager::GetNumConstraints()
    {
        return Constraints.size();
    }

    void PhysicsManager::RemoveConstraint(TypedConstraint* Constraint)
    {
        this->BulletDynamicsWorld->removeConstraint(Constraint->GetConstraintBase());
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
            this->BulletDynamicsWorld->removeConstraint((*Con)->GetConstraintBase());
            delete (*Con);
        }
        Constraints.clear();
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

    AreaEffect* PhysicsManager::GetAreaEffect(const Whole& Index)
    {
        return AreaEffects[Index];
    }

    Whole PhysicsManager::GetNumAreaEffects()
    {
        return AreaEffects.size();
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

    void PhysicsManager::AddWorldTrigger(WorldTrigger* Trig)
    {
        Triggers.push_back(Trig);
    }

    WorldTrigger* PhysicsManager::GetWorldTrigger(const String& Name)
    {
        for( vector<WorldTrigger*>::iterator Trig = Triggers.begin() ; Trig != Triggers.end() ; Trig++ )
        {
            if ( Name == (*Trig)->GetName() )
            {
                return *Trig;
            }
        }
        return NULL;
    }

    WorldTrigger* PhysicsManager::GetWorldTrigger(const Whole& Index)
    {
        return Triggers[Index];
    }

    Whole PhysicsManager::GetNumWorldTriggers()
    {
        return Triggers.size();
    }

    void PhysicsManager::RemoveWorldTrigger(WorldTrigger* Trig)
    {
        for( vector<WorldTrigger*>::iterator T = Triggers.begin() ; T != Triggers.end() ; T++ )
        {
            if ( Trig == (*T) )
            {
                Triggers.erase(T);
                return;
            }
        }
    }

    void PhysicsManager::DestroyAllWorldTriggers()
    {
        for( std::vector<WorldTrigger*>::iterator Trig = Triggers.begin() ; Trig != Triggers.end() ; Trig++ )
            delete (*Trig);
        Triggers.clear();
    }

    Collision* PhysicsManager::GetCollision(ActorPair* Pair)
    {
        PhysicsManager::CollisionIterator ColIt = Collisions.find(Pair);
        if(ColIt != Collisions.end()) return (*ColIt).second;
        else return NULL;
    }

    Whole PhysicsManager::GetNumCollisions()
    {
        return Collisions.size();
    }

    void PhysicsManager::RemoveCollision(Collision* Col)
    {
        //((CollisionDispatcher*)BulletDispatcher)->releaseManifoldManual(Col->Manifold);
        /*BulletBroadphase->getOverlappingPairCache()->removeOverlappingPair(
            Col->GetActorA()->_GetBasePhysicsObject()->getBroadphaseHandle(),
            Col->GetActorB()->_GetBasePhysicsObject()->getBroadphaseHandle(),
            BulletDispatcher);// */
    }

    void PhysicsManager::RemoveCollisionsContainingActor(ActorBase* Actor)
    {
        if(!Actor->IsInWorld())
            return;

        BulletBroadphase->getOverlappingPairCache()->cleanProxyFromPairs(Actor->_GetBasePhysicsObject()->getBroadphaseHandle(),BulletDispatcher);

        PhysicsManager::CollisionIterator ColIt = Collisions.begin();
        while( ColIt != Collisions.end() )
        {
            Collision* ToBeDestroyed = (*ColIt).second;
            if( Actor == (*ColIt).second->GetActorA() || Actor == (*ColIt).second->GetActorB() )
            {
                PhysicsManager::CollisionIterator Delete = ColIt;
                //Collision* ToBeDestroyed = (*Delete).second;
                ++ColIt;
                Collisions.erase(Delete);
                //BulletDispatcher->btCollisionDispatcher::releaseManifold(ToBeDestroyed->Manifold);
                delete ToBeDestroyed;
            }else{
                ++ColIt;
            }
        }
    }

    void PhysicsManager::DestroyAllCollisions()
    {
        for( PhysicsManager::CollisionIterator ColIt = Collisions.begin() ; ColIt != Collisions.end() ; ++ColIt )
        {
            Collision* ToBeDestroyed = (*ColIt).second;
            BulletDispatcher->releaseManifold(ToBeDestroyed->Manifold);
        }
        Collisions.clear();
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

    void PhysicsManager::ResetPhysicsWorld(PhysicsConstructionInfo* Info)
    {
        delete BulletDynamicsWorld;
        delete BulletDispatcher;
        delete BulletCollisionConfiguration;
        delete BulletSolver;
        delete BulletBroadphase;
        delete GhostCallback;
        bool DebugOn = false;
        if(BulletDrawer)
        {
            delete BulletDrawer;
            DebugOn = true;
        }

        if(Info) this->Construct(*Info);
        else this->Construct(WorldConstructionInfo);
        if(DebugOn)
        {
            this->BulletDrawer = new debug::InternalDebugDrawer(this->GameWorld);
            this->BulletDynamicsWorld->setDebugDrawer(this->BulletDrawer);
        }
    }

    void PhysicsManager::ClearPhysicsMetaData()
    {
        // Clean the broadphase of AABB data
        btOverlappingPairCache* Pairs = BulletBroadphase->getOverlappingPairCache();
        int NumPairs = Pairs->getNumOverlappingPairs();
        btBroadphasePairArray PairArray = Pairs->getOverlappingPairArray();
        for( Whole X = 0 ; X < NumPairs ; X++ )
        {
            btBroadphasePair& CurrPair = PairArray.at(X);
            Pairs->cleanOverlappingPair(CurrPair,BulletDispatcher);
            Pairs->removeOverlappingPair(CurrPair.m_pProxy0,CurrPair.m_pProxy1,BulletDispatcher);
        }

        // Clean the dispatcher(narrowphase) of shape data
        int numManifolds = BulletDynamicsWorld->getDispatcher()->getNumManifolds();
        for ( int i = 0 ; i < numManifolds ; i++ )
        {
            BulletDispatcher->releaseManifold(BulletDispatcher->getManifoldByIndexInternal(i));
        }

        BulletBroadphase->resetPool(BulletDispatcher);
        BulletSolver->reset();
        BulletDynamicsWorld->stepSimulation(1.f/60.f,1,1.f/60.f);
    }

    void PhysicsManager::SetSimulationSubstepModifier(const Whole& Modifier)
    {
        SubstepModifier = Modifier;
    }

    void PhysicsManager::DoMainLoopItems(const Real &TimeElapsed)
    {
        if(SimulationPaused)
            return;
        #ifdef PHYSPROFILE
        static Ogre::Timer* Profiler = new Ogre::Timer();
        Profiler->reset();
        #endif

        Real FloatTime = TimeElapsed * 0.001; //Convert from MilliSeconds to Seconds
        Real IdealStep = this->GameWorld->GetTargetFrameTime() * 0.001;
        IdealStep /= SubstepModifier;
        int MaxSteps = (FloatTime<IdealStep) ? 1 : int(FloatTime/IdealStep)+1;
        #ifdef PHYSPROFILE
        Profiler->reset();
        #endif
        GameWorld->LogStream << "Entering StepSimulation.";
        GameWorld->DoMainLoopLogging();
        this->BulletDynamicsWorld->stepSimulation( FloatTime, MaxSteps, IdealStep);
        #ifdef PHYSPROFILE
        GameWorld->LogStream << "StepSimulation() took " << Profiler->getMicroseconds() << " microseconds.";
        GameWorld->DoMainLoopLogging();
        #endif // */

        #ifdef PHYSPROFILE
        Profiler->reset();
        #endif
        ProcessAllCollisions();
        #ifdef PHYSPROFILE
        GameWorld->LogStream << "Collisions took " << Profiler->getMicroseconds() << " microseconds.";
        GameWorld->DoMainLoopLogging();
        #endif // */

        #ifdef PHYSPROFILE
        Profiler->reset();
        #endif
        ProcessAllEffects();
        #ifdef PHYSPROFILE
        GameWorld->LogStream << "AreaEffects took " << Profiler->getMicroseconds() << " microseconds.";
        GameWorld->DoMainLoopLogging();
        #endif // */

        #ifdef PHYSPROFILE
        Profiler->reset();
        #endif
        ProcessAllTriggers();
        #ifdef PHYSPROFILE
        GameWorld->LogStream << "Triggers took " << Profiler->getMicroseconds() << " microseconds.";
        GameWorld->DoMainLoopLogging();
        #endif // */

        // This is supposedly to speed up the performance of soft bodies, if any are in the simulation.
        //this->BulletDynamicsWorld->getWorldInfo().m_sparsesdf.GarbageCollect();

        /*#ifdef PHYSDEBUG
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
                if (pt.m_lifeTime == CollisionAge && pt.m_appliedImpulse >= Impulse && pt.m_distance1 < 0)
                {
                    /// @todo This chunk of code won't take the upcoming terrain system into account, and should be modified accordingly
                    btCollisionObject* objectA = static_cast<btCollisionObject*>(contactManifold->getBody0());
                    btCollisionObject* objectB = static_cast<btCollisionObject*>(contactManifold->getBody1());
                    ActorBase* ActA = this->GameWorld->GetActorManager()->GetActorContainer()->FindActor(objectA);
                    ActorBase* ActB = this->GameWorld->GetActorManager()->GetActorContainer()->FindActor(objectB);

                    if( !ActA || !ActB )
                        continue;

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
        GameWorld->DoMainLoopLogging();
        #endif // */

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
        GameWorld->DoMainLoopLogging();
        #endif // */
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
