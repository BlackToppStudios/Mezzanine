// © Copyright 2010 - 2014 BlackTopp Studios Inc.
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
#ifndef _physicsphysicsmanager_cpp
#define _physicsphysicsmanager_cpp

using namespace std;

#include "Physics/physicsmanager.h"
#include "Physics/collision.h"

#include "Physics/ghostproxy.h"
#include "Physics/rigidproxy.h"
#include "Physics/softproxy.h"

#include "Graphics/graphicsmanager.h"

// WorldObject Manager includes are here for the debug draw work unit dependency setting
#include "actormanager.h"
#include "areaeffectmanager.h"
#include "debrismanager.h"

#include "stringtool.h"
#include "linegroup.h"
#include "vector3.h"
#include "worldtrigger.h"
#include "worldobject.h"
#include "crossplatform.h"
#include "entresol.h"

#include "Physics/collisiondispatcher.h.cpp"

#include <queue>
#include <algorithm>

#include <btBulletDynamicsCommon.h>
#include <BulletSoftBody/btSoftRigidDynamicsWorld.h>
#include <BulletSoftBody/btSoftBodyRigidBodyCollisionConfiguration.h>
#include <BulletCollision/Gimpact/btGImpactCollisionAlgorithm.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>

// This define is needed to avoid a declaration collision for uint64_t between a bullet typedef and the one in stdint.h
#define __PHYSICS_COMMON_H__ 1

#ifdef WINDOWS
#include <BulletMultiThreaded/Win32ThreadSupport.h>
#else
#include <BulletMultiThreaded/PosixThreadSupport.h>
#endif

#include <BulletMultiThreaded/btParallelConstraintSolver.h>
#include <BulletMultiThreaded/SpuGatheringCollisionDispatcher.h>
#include <BulletMultiThreaded/SpuNarrowPhaseCollisionTask/SpuGatheringCollisionTask.h>


namespace Mezzanine
{
    /// @internal
    /// @namespace Mezzanine::debug
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
            /// @brief This stores the wireframe being used for rendering.
            Mezzanine::LineGroup* WireFrame;
            /// @internal
            /// @brief This stores whether or not to render physics debug lines
            /// @details This stores whether or not to render physics debud lines. 0 = Do not draw anything. 1 = Draw model wireframes.
            /// Later we will add support for contact drawing, individual modeling drawing, etc...
            int DebugDrawing;
        public:
            /// @internal
            /// @brief Basic Constructor
            InternalDebugDrawer();
            /// @internal
            /// @brief Destructor
            virtual ~InternalDebugDrawer();

            /// @internal
            /// @brief Clears data as necessary for updating debug geometry.
            virtual void PrepareForUpdate();
            /// @internal
            /// @brief Copies all the line data to render buffers so they can be seen on screen.
            virtual void FinalizeUpdate();

            /// @internal
            /// @brief This will prepare a line segment for being drawn
            /// @details This adds the points for a line to the internal list of points to be rendered.
            /// @param from The first point of the line
            /// @param to The second point of the line
            /// @param color Currently ignored
            virtual void drawLine(const btVector3& from,const btVector3& to,const btVector3& color);
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

            /// @internal
            /// @brief Used by the physics subsystem to report errors using the renderer
            /// @details We *Believe* that this is used by the physics subsystem to report errors about rendering to the developer/user. As such, we
            /// Have redirected all input from this function to the Entresol::Log function.
            /// @param warningString We *Believe* These are messagesfrom the physics subsystem, and that this should not directly called otherwise
            virtual void reportErrorWarning(const char* warningString);
        };

        InternalDebugDrawer::InternalDebugDrawer()
        {
            this->DebugDrawing = Physics::DDM_NoDebug;
            this->WireFrame = new Mezzanine::LineGroup();
        }

        InternalDebugDrawer::~InternalDebugDrawer()
            { delete this->WireFrame; }

        void InternalDebugDrawer::PrepareForUpdate()
        {
            if( this->WireFrame != NULL ) {
                this->WireFrame->AddToWorld();
                this->WireFrame->ClearLines();
            }
        }

        void InternalDebugDrawer::FinalizeUpdate()
        {
            if( this->WireFrame != NULL ) {
                this->WireFrame->DrawLines();
            }
        }

        void InternalDebugDrawer::drawLine(const btVector3& from,const btVector3& to,const btVector3& color)
            { this->WireFrame->DrawLine( Vector3(from), Vector3(to), ColourValue(color.getX(),color.getY(),color.getZ()) ); }

        void InternalDebugDrawer::drawContactPoint(const btVector3& PointOnB,const btVector3& normalOnB,btScalar distance,int lifeTime,const btVector3& color)
            {}

        void InternalDebugDrawer::draw3dText(const btVector3& location,const char* textString)
            {}

        void InternalDebugDrawer::setDebugMode(int debugMode)
        {
            this->DebugDrawing = debugMode;
            if( this->WireFrame != NULL ) {
                if( this->DebugDrawing != Physics::DDM_NoDebug ) {
                    this->WireFrame->AddToWorld();
                }else{
                    this->WireFrame->RemoveFromWorld();
                }
            }
        }

        int InternalDebugDrawer::getDebugMode() const
            { return this->DebugDrawing; }

        void InternalDebugDrawer::reportErrorWarning(const char* warningString)
        {
            String temp(warningString);
            Mezzanine::Entresol::GetSingletonPtr()->Log(temp);
        }
    }// debug

    #ifdef GetObject
    #undef GetObject
    #endif
    namespace Physics
    {
        ///////////////////////////////////////////////////////////
        // SimulationWorkUnit functions

        SimulationWorkUnit::SimulationWorkUnit(const SimulationWorkUnit& Other)
            {  }

        SimulationWorkUnit& SimulationWorkUnit::operator=(const SimulationWorkUnit& Other)
            { return *this; }

        SimulationWorkUnit::SimulationWorkUnit(PhysicsManager* Target) :
            TargetManager(Target) {  }

        SimulationWorkUnit::~SimulationWorkUnit()
            {  }

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        void SimulationWorkUnit::DoWork(Threading::DefaultThreadSpecificStorage::Type& CurrentThreadStorage)
            { this->TargetManager->DoPerFrameWork(CurrentThreadStorage); }

        ///////////////////////////////////////////////////////////
        // SimulationMonopolyWorkUnit functions

        SimulationMonopolyWorkUnit::SimulationMonopolyWorkUnit(const SimulationMonopolyWorkUnit& Other)
            {  }

        SimulationMonopolyWorkUnit& SimulationMonopolyWorkUnit::operator=(const SimulationMonopolyWorkUnit& Other)
            { return *this; }

        SimulationMonopolyWorkUnit::SimulationMonopolyWorkUnit(PhysicsManager* Target) :
            TargetManager(Target) {  }

        SimulationMonopolyWorkUnit::~SimulationMonopolyWorkUnit()
            {  }

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        void SimulationMonopolyWorkUnit::UseThreads(const Whole& AmountToUse)
            {}

        Whole SimulationMonopolyWorkUnit::UsingThreadCount()
            { return this->TargetManager->ThreadCount; }

        void SimulationMonopolyWorkUnit::DoWork(Threading::DefaultThreadSpecificStorage::Type& CurrentThreadStorage)
            { this->TargetManager->DoPerFrameWork(CurrentThreadStorage); }

        ///////////////////////////////////////////////////////////
        // WorldTriggerUpdate functions

        WorldTriggerUpdateWorkUnit::WorldTriggerUpdateWorkUnit(const WorldTriggerUpdateWorkUnit& Other)
            {  }

        WorldTriggerUpdateWorkUnit& WorldTriggerUpdateWorkUnit::operator=(const WorldTriggerUpdateWorkUnit& Other)
            { return *this; }

        WorldTriggerUpdateWorkUnit::WorldTriggerUpdateWorkUnit(PhysicsManager* Target) :
            TargetManager(Target) {  }

        WorldTriggerUpdateWorkUnit::~WorldTriggerUpdateWorkUnit()
            {  }

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        void WorldTriggerUpdateWorkUnit::DoWork(Threading::DefaultThreadSpecificStorage::Type& CurrentThreadStorage)
        {
            // No real logging necessary
            this->TargetManager->ProcessAllTriggers();
        }

        ///////////////////////////////////////////////////////////
        // DebugDrawWorkUnit functions

        DebugDrawWorkUnit::DebugDrawWorkUnit(const DebugDrawWorkUnit& Other)
            {  }

        DebugDrawWorkUnit& DebugDrawWorkUnit::operator=(const DebugDrawWorkUnit& Other)
            { return *this; }

        DebugDrawWorkUnit::DebugDrawWorkUnit(PhysicsManager* Target) :
            TargetManager(Target) {  }

        DebugDrawWorkUnit::~DebugDrawWorkUnit()
            {  }

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        void DebugDrawWorkUnit::DoWork(Threading::DefaultThreadSpecificStorage::Type& CurrentThreadStorage)
        {
            // No real logging necessary
            debug::InternalDebugDrawer* Drawer = this->TargetManager->BulletDrawer;
            if( Drawer && Drawer->getDebugMode() )        //this part is responsible for drawing the wireframes
            {
                Drawer->PrepareForUpdate();
                this->TargetManager->BulletDynamicsWorld->debugDrawWorld();
                Drawer->FinalizeUpdate();
            }
        }

        ///////////////////////////////////////////////////////////
        // Physicsmanager functions

        PhysicsManager::PhysicsManager() :
            SimulationPaused(false),
            DebugRenderMode(0),
            SubstepModifier(1),
            ThreadCount(0),
            StepSize(1.0/60.0),
            TimeMultiplier(1.0),

            GhostCallback(NULL),
            BulletSolverThreads(NULL),
            BulletDispatcherThreads(NULL),
            BulletBroadphase(NULL),
            BulletCollisionConfiguration(NULL),
            BulletDispatcher(NULL),
            BulletSolver(NULL),
            BulletDynamicsWorld(NULL),
            BulletDrawer(NULL),

            SimulationWork(NULL),
            WorldTriggerUpdateWork(NULL),
            DebugDrawWork(NULL)
        {
            ManagerConstructionInfo Info;
            Info.PhysicsFlags = (ManagerConstructionInfo::PCF_SoftRigidWorld | ManagerConstructionInfo::PCF_LimitlessWorld);
            this->Construct(Info);
        }

        PhysicsManager::PhysicsManager(const ManagerConstructionInfo& Info) :
            SimulationPaused(false),
            DebugRenderMode(0),
            SubstepModifier(1),
            ThreadCount(0),
            StepSize(1.0/60.0),
            TimeMultiplier(1.0),

            GhostCallback(NULL),
            BulletSolverThreads(NULL),
            BulletDispatcherThreads(NULL),
            BulletBroadphase(NULL),
            BulletCollisionConfiguration(NULL),
            BulletDispatcher(NULL),
            BulletSolver(NULL),
            BulletDynamicsWorld(NULL),
            BulletDrawer(NULL),

            SimulationWork(NULL),
            WorldTriggerUpdateWork(NULL),
            DebugDrawWork(NULL)
        {
            this->Construct(Info);
        }

        PhysicsManager::PhysicsManager(XML::Node& XMLNode) :
            SimulationPaused(false),
            DebugRenderMode(0),
            SubstepModifier(1),
            ThreadCount(0),
            StepSize(1.0/60.0),
            TimeMultiplier(1.0),

            GhostCallback(NULL),
            BulletSolverThreads(NULL),
            BulletDispatcherThreads(NULL),
            BulletBroadphase(NULL),
            BulletCollisionConfiguration(NULL),
            BulletDispatcher(NULL),
            BulletSolver(NULL),
            BulletDynamicsWorld(NULL),
            BulletDrawer(NULL),

            SimulationWork(NULL),
            WorldTriggerUpdateWork(NULL),
            DebugDrawWork(NULL)
        {
            ManagerConstructionInfo Info;
            XML::Attribute CurrAttrib;

            XML::Node WorldSettings = XMLNode.GetChild("WorldSettings");
            if(!WorldSettings.Empty())
            {
                CurrAttrib = WorldSettings.GetAttribute("LimitlessWorld");
                if(!CurrAttrib.Empty())
                {
                    Info.PhysicsFlags = (Info.PhysicsFlags | ManagerConstructionInfo::PCF_LimitlessWorld);
                }else{
                    CurrAttrib = WorldSettings.GetAttribute("WorldUpperBounds");
                    if(!CurrAttrib.Empty())
                        Info.GeographyUpperBounds = StringTools::ConvertToVector3(CurrAttrib.AsString());
                    CurrAttrib = WorldSettings.GetAttribute("WorldLowerBounds");
                    if(!CurrAttrib.Empty())
                        Info.GeographyLowerBounds = StringTools::ConvertToVector3(CurrAttrib.AsString());
                    CurrAttrib = WorldSettings.GetAttribute("MaxProxies");
                    if(!CurrAttrib.Empty())
                        Info.MaxProxies = CurrAttrib.AsWhole();
                }
                CurrAttrib = WorldSettings.GetAttribute("SoftRigidWorld");
                if(!CurrAttrib.Empty())
                {
                    Info.PhysicsFlags = (Info.PhysicsFlags | ManagerConstructionInfo::PCF_SoftRigidWorld);
                }
                CurrAttrib = WorldSettings.GetAttribute("MultiThreaded");
                if(!CurrAttrib.Empty())
                {
                    Info.PhysicsFlags = (Info.PhysicsFlags | ManagerConstructionInfo::PCF_Multithreaded);
                }
            }

            this->Construct(Info);

            XML::Node StepModifier = XMLNode.GetChild("SubStepModifier");
            if(!StepModifier.Empty())
            {
                CurrAttrib = WorldSettings.GetAttribute("Modifier");
                if(!CurrAttrib.Empty()) {
                    SetSimulationSubstepModifier(CurrAttrib.AsWhole());
                }
            }

            XML::Node DebugRender = XMLNode.GetChild("DebugRendering");
            if(!DebugRender.Empty())
            {
                int RenderMode = 0;
                CurrAttrib = WorldSettings.GetAttribute("RenderingMode");
                if(!CurrAttrib.Empty())
                    RenderMode = CurrAttrib.AsInt();

                if(0 != RenderMode) {
                    this->SetDebugRenderingMode(RenderMode);
                }
            }
        }

        PhysicsManager::~PhysicsManager()
        {
            btCollisionObjectArray ObjectArray( BulletDynamicsWorld->getCollisionObjectArray() );
            for( Integer X = 0 ; X < BulletDynamicsWorld->getNumCollisionObjects() ; ++X )
            {
                CollidableProxy* Prox = static_cast<CollidableProxy*>( ObjectArray[X]->getUserPointer() );
                Prox->RemoveFromWorld();
            }

            this->DestroyAllConstraints();
            this->DestroyAllProxies();
            this->DestroyAllWorldTriggers();

            this->Deinitialize();

            //Destroy the physical world that we loved and cherished
            this->Destroy();
        }

        void PhysicsManager::Construct(const ManagerConstructionInfo& Info)
        {
            this->CallBackWorld = NULL;
            this->ThreadCount = ( Info.PhysicsFlags & ManagerConstructionInfo::PCF_Multithreaded) ? crossplatform::GetCPUCount() : 0;

            // Create the broadphase
            if( Info.PhysicsFlags & ManagerConstructionInfo::PCF_LimitlessWorld ) {
                this->BulletBroadphase = new btDbvtBroadphase();
            }else{
                if( Info.MaxProxies < 65536 )
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

            // Create the collision configuration
            //if( Info.PhysicsFlags & ManagerConstructionInfo::PCF_SoftRigidWorld ) {
                this->BulletCollisionConfiguration = new btSoftBodyRigidBodyCollisionConfiguration();
            /*}else{
                this->BulletCollisionConfiguration = new btDefaultCollisionConfiguration();
            }//*/

            // Create the dispatcher (narrowphase)
            if( Info.PhysicsFlags & ManagerConstructionInfo::PCF_Multithreaded ) {
                #ifdef WINDOWS
                Win32ThreadSupport::Win32ThreadConstructionInfo BulletThreadInfo( "DispatcherThreads",
                                                                                  processCollisionTask,
                                                                                  createCollisionLocalStoreMemory,
                                                                                  ThreadCount );
                this->BulletDispatcherThreads = new Win32ThreadSupport(BulletThreadInfo);
                #else //WINDOWS
                PosixThreadSupport::ThreadConstructionInfo BulletThreadInfo( "DispatcherThreads",
                                                                             processCollisionTask,
                                                                             createCollisionLocalStoreMemory,
                                                                             ThreadCount );
                this->BulletDispatcherThreads = new PosixThreadSupport(BulletThreadInfo);
                #endif //WINDOWS
                this->BulletDispatcher = new ParallelCollisionDispatcher(this->BulletDispatcherThreads,ThreadCount,this->BulletCollisionConfiguration);
            }else{
                this->BulletDispatcher = new CollisionDispatcher(this->BulletCollisionConfiguration);
            }

            // Create the constraint solver
            if( Info.PhysicsFlags & ManagerConstructionInfo::PCF_Multithreaded ) {
                #ifdef WINDOWS
                Win32ThreadSupport::Win32ThreadConstructionInfo BulletThreadInfo( "SolverThreads",
                                                                                  SolverThreadFunc,
                                                                                  SolverlsMemoryFunc,
                                                                                  ThreadCount );
                this->BulletSolverThreads = new Win32ThreadSupport(BulletThreadInfo);
                this->BulletSolverThreads->startSPU();
                #else //WINDOWS
                PosixThreadSupport::ThreadConstructionInfo BulletThreadInfo( "SolverThreads",
                                                                             SolverThreadFunc,
                                                                             SolverlsMemoryFunc,
                                                                             ThreadCount );
                this->BulletSolverThreads = new PosixThreadSupport(BulletThreadInfo);
                #endif //WINDOWS
                //this->BulletSolver = new btParallelConstraintSolver(this->BulletSolverThreads);
                this->BulletSolver = new btSequentialImpulseConstraintSolver();
            }else{
                this->BulletSolver = new btSequentialImpulseConstraintSolver();
            }

            // Create the world
            //if( Info.PhysicsFlags & ManagerConstructionInfo::PCF_SoftRigidWorld ) {
                this->BulletDynamicsWorld = new btSoftRigidDynamicsWorld( this->BulletDispatcher,
                                                                          this->BulletBroadphase,
                                                                          this->BulletSolver,
                                                                          this->BulletCollisionConfiguration);
            /*}else{
                this->BulletDynamicsWorld = new btDiscreteDynamicsWorld( this->BulletDispatcher,
                                                                         this->BulletBroadphase,
                                                                         this->BulletSolver,
                                                                         this->BulletCollisionConfiguration);
            }//*/

            // Set up the work units
            if( Info.PhysicsFlags & ManagerConstructionInfo::PCF_Multithreaded ) {
                this->SimulationWork = new SimulationMonopolyWorkUnit(this);
            }else{
                this->SimulationWork = new SimulationWorkUnit(this);
            }
            this->WorldTriggerUpdateWork = new WorldTriggerUpdateWorkUnit(this);
            this->DebugDrawWork = new DebugDrawWorkUnit(this);

            // Configure the extra data
            btGImpactCollisionAlgorithm::registerAlgorithm(this->BulletDispatcher);

            this->GhostCallback = new btGhostPairCallback();
            this->BulletBroadphase->getOverlappingPairCache()->setInternalGhostPairCallback(this->GhostCallback);

            this->BulletDynamicsWorld->setInternalTickCallback((btInternalTickCallback)PhysicsManager::InternalTickCallback,0,false);

            this->BulletDynamicsWorld->getWorldInfo().m_dispatcher = this->BulletDispatcher;
            this->BulletDynamicsWorld->getWorldInfo().m_broadphase = this->BulletBroadphase;
            this->BulletDynamicsWorld->getWorldInfo().m_sparsesdf.Initialize();

            this->BulletDynamicsWorld->getDispatchInfo().m_enableSPU = true;
            this->BulletDynamicsWorld->getDispatchInfo().m_useContinuous = true;
            //this->BulletDynamicsWorld->getSolverInfo().m_splitImpulse = true;
            //this->BulletDynamicsWorld->getSolverInfo().m_numIterations = 20;
            //this->BulletDynamicsWorld->getSolverInfo().m_globalCfm = 0.15;
            //this->BulletDynamicsWorld->getSolverInfo().m_erp = 0.4;

            this->SetWorldGravity(Info.Gravity);
            this->SetWorldSoftGravity(Info.Gravity);
            this->WorldConstructionInfo = Info;
        }

        void PhysicsManager::Destroy()
        {
            delete this->BulletDynamicsWorld;
            this->BulletDynamicsWorld = NULL;
            delete this->BulletDispatcher;
            this->BulletDispatcher = NULL;
            delete this->BulletCollisionConfiguration;
            this->BulletCollisionConfiguration = NULL;
            delete this->BulletSolver;
            this->BulletSolver = NULL;
            delete this->BulletBroadphase;
            this->BulletBroadphase = NULL;
            delete this->GhostCallback;
            this->GhostCallback = NULL;
            if(BulletDrawer) {
                delete this->BulletDrawer;
                this->BulletDrawer = NULL;
            }
            if(BulletSolverThreads) {
                delete this->BulletSolverThreads;
                this->BulletSolverThreads = NULL;
            }
            if(BulletDispatcherThreads) {
                delete this->BulletDispatcherThreads;
                this->BulletDispatcherThreads = NULL;
            }

            delete this->SimulationWork;
            this->SimulationWork = NULL;

            delete this->WorldTriggerUpdateWork;
            this->WorldTriggerUpdateWork = NULL;

            delete this->DebugDrawWork;
            this->DebugDrawWork = NULL;
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
            //Update the collisions that already exist as necessary
            for( PhysicsManager::CollisionIterator ColIt = Collisions.begin() ; ColIt != Collisions.end() ; ColIt++ )
                (*ColIt).second->Update();
            //Process the collisions that are in the creation queue
            AlgoList* AlgoQueue = ( this->WorldConstructionInfo.PhysicsFlags & ManagerConstructionInfo::PCF_Multithreaded ?
                                    static_cast<ParallelCollisionDispatcher*>( this->BulletDispatcher )->GetAlgoCreationQueue() :
                                    static_cast<CollisionDispatcher*>( this->BulletDispatcher )->GetAlgoCreationQueue() );
            if(AlgoQueue->empty())
                return;
            #ifdef MEZZDEBUG
            /*StringStream logstream;
            logstream << "Processing " << AlgoQueue->size() << " algorithms for collisions.";
            Entresol::GetSingletonPtr()->Log(logstream.str());
            Entresol::GetSingletonPtr()->DoMainLoopLogging();//*/
            #endif
            btCollisionAlgorithm* NewAlgo = AlgoQueue->front();
            while( NewAlgo != NULL )
            {
                CollidableProxy* ProxA = NULL;
                CollidableProxy* ProxB = NULL;
                /// @todo This is an absurd round-about way to get the data we need,
                /// and bullet will probably have to be extended to change this so it's actually good.
                btBroadphasePairArray& PairArray = BulletBroadphase->getOverlappingPairCache()->getOverlappingPairArray();
                for( Integer X = 0 ; X < PairArray.size() ; ++X )
                {
                    if( NewAlgo == PairArray[X].m_algorithm )
                    {
                        btCollisionObject* COA = (btCollisionObject*)PairArray[X].m_pProxy0->m_clientObject;
                        ProxA = static_cast<CollidableProxy*>( COA->getUserPointer() );
                        btCollisionObject* COB = (btCollisionObject*)PairArray[X].m_pProxy1->m_clientObject;
                        ProxB = static_cast<CollidableProxy*>( COB->getUserPointer() );
                        break;
                    }
                }

                if( ( ProxA != NULL && ProxA->GetCollisionResponse() ) && ( ProxB != NULL && ProxB->GetCollisionResponse() ) )
                {
                    // Create the collision
                    CollidablePair NewPair(ProxA,ProxB);
                    PhysicsManager::CollisionIterator ColIt = Collisions.find(NewPair);
                    if(ColIt == Collisions.end())
                    {
                        Physics::Collision* NewCol = new Physics::Collision(ProxA,ProxB,NewAlgo);
                        //NewCol->GetActorA()->_NotifyCollisionState(NewCol,Physics::Collision::Col_Begin);
                        //NewCol->GetActorB()->_NotifyCollisionState(NewCol,Physics::Collision::Col_Begin);
                        Collisions.insert( CollisionSortPair(NewPair,NewCol) );
                    }
                }
                AlgoQueue->pop_front();
                if(AlgoQueue->size() > 0) NewAlgo = AlgoQueue->front();
                else NewAlgo = NULL;
            }//*/
        }

        PhysicsManager* PhysicsManager::CallBackWorld;
        void PhysicsManager::InternalTickCallback(btDynamicsWorld* world, btScalar timeStep)
            { CallBackWorld->ProcessAllCollisions(); }

        ///////////////////////////////////////////////////////////////////////////////
        // Simulation Management

        void PhysicsManager::PauseSimulation(Boole Pause)
            { this->SimulationPaused = Pause; }

        Boole PhysicsManager::SimulationIsPaused()
            { return this->SimulationPaused; }

        Real PhysicsManager::GetTimeMultiplier() const
            { return TimeMultiplier; }

        void PhysicsManager::SetTimeMultiplier(const Real &value)
            { TimeMultiplier = value; }

        ///////////////////////////////////////////////////////////////////////////////
        // Gravity Management

        void PhysicsManager::SetWorldGravity(const Vector3& pgrav)
            { this->BulletDynamicsWorld->setGravity(pgrav.GetBulletVector3()); }

        Vector3 PhysicsManager::GetWorldGravity()
        {
            Vector3 grav(this->BulletDynamicsWorld->getGravity());
            return grav;
        }

        void PhysicsManager::SetWorldSoftGravity(const Vector3& sgrav)
            { this->BulletDynamicsWorld->getWorldInfo().m_gravity = sgrav.GetBulletVector3(); }

        Vector3 PhysicsManager::GetWorldSoftGravity()
        {
            Vector3 sgrav(this->BulletDynamicsWorld->getWorldInfo().m_gravity);
            return sgrav;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Creating Proxies

        GhostProxy* PhysicsManager::CreateGhostProxy()
        {
            GhostProxy* NewProxy = new GhostProxy(this);
            this->Proxies.push_back(NewProxy);
            return NewProxy;
        }

        GhostProxy* PhysicsManager::CreateGhostProxy(const XML::Node& SelfRoot)
        {
            GhostProxy* NewProxy = new GhostProxy(SelfRoot,this);
            this->Proxies.push_back(NewProxy);
            return NewProxy;
        }

        RigidProxy* PhysicsManager::CreateRigidProxy(const Real Mass)
        {
            RigidProxy* NewProxy = new RigidProxy(Mass,this);
            this->Proxies.push_back(NewProxy);
            return NewProxy;
        }

        RigidProxy* PhysicsManager::CreateRigidProxy(const XML::Node& SelfRoot)
        {
            RigidProxy* NewProxy = new RigidProxy(SelfRoot,this);
            this->Proxies.push_back(NewProxy);
            return NewProxy;
        }

        SoftProxy* PhysicsManager::CreateSoftProxy(const Real Mass)
        {
            SoftProxy* NewProxy = new SoftProxy(Mass,this);
            this->Proxies.push_back(NewProxy);
            return NewProxy;
        }

        SoftProxy* PhysicsManager::CreateSoftProxy(const XML::Node& SelfRoot)
        {
            SoftProxy* NewProxy = new SoftProxy(SelfRoot,this);
            this->Proxies.push_back(NewProxy);
            return NewProxy;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Proxy Management

        CollidableProxy* PhysicsManager::GetProxy(const UInt32 Index) const
            { return this->Proxies.at(Index); }

        UInt32 PhysicsManager::GetNumProxies() const
            { return this->Proxies.size(); }

        void PhysicsManager::DestroyProxy(CollidableProxy* ToBeDestroyed)
        {
            for( ProxyIterator ProxIt = this->Proxies.begin() ; ProxIt != this->Proxies.end() ; ++ProxIt )
            {
                if( ToBeDestroyed == (*ProxIt) ) {
                    WorldObject* Parent = (*ProxIt)->GetParentObject();
                    if( Parent )
                        Parent->_NotifyProxyDestroyed( (*ProxIt) );

                    delete (*ProxIt);
                    this->Proxies.erase(ProxIt);
                    return;
                }
            }
        }

        void PhysicsManager::DestroyAllProxies()
        {
            for( ProxyIterator ProxIt = this->Proxies.begin() ; ProxIt != this->Proxies.end() ; ++ProxIt )
            {
                WorldObject* Parent = (*ProxIt)->GetParentObject();
                if( Parent )
                    Parent->_NotifyProxyDestroyed( (*ProxIt) );

                delete (*ProxIt);
            }
            this->Proxies.clear();
        }

        PhysicsManager::ProxyIterator PhysicsManager::BeginCollidableProxy()
            { return this->Proxies.begin(); }

        PhysicsManager::ProxyIterator PhysicsManager::EndCollidableProxy()
            { return this->Proxies.end(); }

        PhysicsManager::ConstProxyIterator PhysicsManager::BeginCollidableProxy() const
            { return this->Proxies.begin(); }

        PhysicsManager::ConstProxyIterator PhysicsManager::EndCollidableProxy() const
            { return this->Proxies.end(); }

        ///////////////////////////////////////////////////////////////////////////////
        // Constraint Management

        void PhysicsManager::AddConstraint(Physics::Constraint* Con, Boole DisableCollisions)
        {
            this->BulletDynamicsWorld->addConstraint(Con->GetConstraintBase(), DisableCollisions);
            this->Constraints.push_back(Con);
        }

        Physics::Constraint* PhysicsManager::GetConstraint(const Whole& Index)
            { return this->Constraints[Index]; }

        Whole PhysicsManager::GetNumConstraints()
            { return this->Constraints.size(); }

        void PhysicsManager::RemoveConstraint(Physics::Constraint* Con)
        {
            this->BulletDynamicsWorld->removeConstraint(Con->GetConstraintBase());
            for( ConstraintIterator ConIt = this->Constraints.begin() ; ConIt < this->Constraints.end() ; ConIt++ )
            {
                if( (*ConIt) == Con )
                {
                    this->Constraints.erase(ConIt);
                    return;
                }
            }
        }

        void PhysicsManager::DestroyAllConstraints()
        {
            for( ConstraintIterator Con = this->Constraints.begin() ; Con != this->Constraints.end() ; Con++ )
            {
                this->BulletDynamicsWorld->removeConstraint((*Con)->GetConstraintBase());
                delete (*Con);
            }
            this->Constraints.clear();
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Trigger Management

        void PhysicsManager::AddWorldTrigger(WorldTrigger* Trig)
            { this->Triggers.push_back(Trig); }

        WorldTrigger* PhysicsManager::GetWorldTrigger(const String& Name)
        {
            for( ConstWorldTriggerIterator Trig = this->Triggers.begin() ; Trig != this->Triggers.end() ; Trig++ )
            {
                if( Name == (*Trig)->GetName() )
                {
                    return *Trig;
                }
            }
            return NULL;
        }

        WorldTrigger* PhysicsManager::GetWorldTrigger(const Whole& Index)
            { return this->Triggers.at(Index); }

        Whole PhysicsManager::GetNumWorldTriggers()
            { return this->Triggers.size(); }

        void PhysicsManager::RemoveWorldTrigger(WorldTrigger* Trig)
        {
            for( WorldTriggerIterator T = this->Triggers.begin() ; T != this->Triggers.end() ; T++ )
            {
                if( Trig == (*T) )
                {
                    this->Triggers.erase(T);
                    return;
                }
            }
        }

        void PhysicsManager::DestroyAllWorldTriggers()
        {
            for( WorldTriggerIterator Trig = this->Triggers.begin() ; Trig != this->Triggers.end() ; Trig++ )
                delete (*Trig);
            this->Triggers.clear();
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Collision Management

        Physics::Collision* PhysicsManager::GetCollision(CollidablePair* Pair)
        {
            ConstCollisionIterator ColIt = this->Collisions.find(*Pair);
            if(ColIt != this->Collisions.end()) return (*ColIt).second;
            else return NULL;
        }

        Whole PhysicsManager::GetNumCollisions()
            { return this->Collisions.size(); }

        void PhysicsManager::RemoveCollision(Physics::Collision* Col)
        {
            //((CollisionDispatcher*)BulletDispatcher)->releaseManifoldManual(Col->Manifold);
            btBroadphasePair* btPair = this->BulletBroadphase->getOverlappingPairCache()->findPair(
                Col->ProxyA->_GetBasePhysicsObject()->getBroadphaseHandle(),
                Col->ProxyB->_GetBasePhysicsObject()->getBroadphaseHandle());
            this->BulletBroadphase->getOverlappingPairCache()->removeOverlappingPair(
                Col->ProxyA->_GetBasePhysicsObject()->getBroadphaseHandle(),
                Col->ProxyB->_GetBasePhysicsObject()->getBroadphaseHandle(),
                this->BulletDispatcher);// */
            this->BulletBroadphase->getOverlappingPairCache()->cleanOverlappingPair(*btPair,this->BulletDispatcher);
            delete btPair;
        }

        void PhysicsManager::RemoveCollisionsContainingProxy(CollidableProxy* Proxy)
        {
            if( !Proxy->IsInWorld() )
                return;

            this->BulletBroadphase->getOverlappingPairCache()->cleanProxyFromPairs( Proxy->_GetBasePhysicsObject()->getBroadphaseHandle(), this->BulletDispatcher );

            CollisionIterator ColIt = this->Collisions.begin();
            while( ColIt != this->Collisions.end() )
            {
                Physics::Collision* ToBeDestroyed = (*ColIt).second;
                if( Proxy == (*ColIt).second->ProxyA || Proxy == (*ColIt).second->ProxyB ) {
                    CollisionIterator Delete = ColIt;
                    ++ColIt;
                    this->Collisions.erase(Delete);
                    delete ToBeDestroyed;
                }else{
                    ++ColIt;
                }
            }
        }

        void PhysicsManager::DestroyAllCollisions()
        {
            for( CollisionIterator ColIt = this->Collisions.begin() ; ColIt != this->Collisions.end() ; ++ColIt )
            {
                delete (*ColIt).second;
            }
            this->Collisions.clear();
        }

        PhysicsManager::CollisionIterator PhysicsManager::BeginCollision()
            { return this->Collisions.begin(); }

        PhysicsManager::CollisionIterator PhysicsManager::EndCollision()
            { return this->Collisions.end(); }

        PhysicsManager::ConstCollisionIterator PhysicsManager::BeginCollision() const
            { return this->Collisions.begin(); }

        PhysicsManager::ConstCollisionIterator PhysicsManager::EndCollision() const
            { return this->Collisions.end(); }

        ///////////////////////////////////////////////////////////////////////////////
        // Debug Management

        void PhysicsManager::SetDebugRenderingMode(const Integer DebugRenderingMode)
        {
            if( this->BulletDrawer ) {
                this->DebugRenderMode = DebugRenderingMode;
                this->BulletDrawer->setDebugMode( DebugRenderingMode );
            }
        }

        Integer PhysicsManager::GetDebugRenderingMode() const
            { return this->DebugRenderMode; }

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        void PhysicsManager::ResetPhysicsWorld(ManagerConstructionInfo* Info)
        {
            this->Destroy();
            if(Info) this->Construct(*Info);
            else this->Construct(WorldConstructionInfo);

            if( this->Initialized && this->BulletDrawer == NULL ) {
                this->BulletDrawer = new debug::InternalDebugDrawer();
                this->BulletDrawer->setDebugMode( this->DebugRenderMode );
                this->BulletDynamicsWorld->setDebugDrawer( this->BulletDrawer );
            }
        }

        void PhysicsManager::ClearPhysicsMetaData()
        {
            // Clean the broadphase of AABB data
            btOverlappingPairCache* Pairs = this->BulletBroadphase->getOverlappingPairCache();
            int NumPairs = Pairs->getNumOverlappingPairs();
            btBroadphasePairArray PairArray = Pairs->getOverlappingPairArray();
            for( Integer X = 0 ; X < NumPairs ; X++ )
            {
                btBroadphasePair& CurrPair = PairArray.at(X);
                Pairs->cleanOverlappingPair(CurrPair,this->BulletDispatcher);
                Pairs->removeOverlappingPair(CurrPair.m_pProxy0,CurrPair.m_pProxy1,this->BulletDispatcher);
            }

            // Clean the dispatcher(narrowphase) of shape data
            int numManifolds = this->BulletDispatcher->getNumManifolds();
            for ( int i = 0 ; i < numManifolds ; i++ )
            {
                this->BulletDispatcher->releaseManifold(this->BulletDispatcher->getManifoldByIndexInternal(i));
            }

            this->BulletBroadphase->resetPool(this->BulletDispatcher);
            this->BulletSolver->reset();
            this->BulletDynamicsWorld->stepSimulation(1.f/60.f,1,1.f/60.f);
        }

        void PhysicsManager::SetSimulationSubstepModifier(const Whole& Modifier)
            { SubstepModifier = Modifier; }

        void PhysicsManager::Pause(const UInt32 PL)
        {
            // Implement later
        }

        void PhysicsManager::MainLoopInitialize()
        {
            // Configure our area effects so they have an updated list and apply their effects immediately.
            this->BulletDynamicsWorld->updateAabbs();
            this->BulletBroadphase->calculateOverlappingPairs(this->BulletDispatcher);
            this->BulletDispatcher->dispatchAllCollisionPairs(this->BulletDynamicsWorld->getPairCache(),this->BulletDynamicsWorld->getDispatchInfo(),this->BulletDispatcher);

            // Set our ideal simulation step size
            //Real InvSubStepMod = 1.0 / this->SubstepModifier;
            //Real TargetTimeSeconds = static_cast<Real>( this->TheEntresol->GetTargetFrameTimeMicroseconds() ) * 0.000001;
            //this->StepSize = std::max( TargetTimeSeconds * InvSubStepMod, static_cast<Real>( 1.0 / 120.0 ) );
            //this->StepSize = TargetTimeSeconds * InvSubStepMod;
            this->StepSize = (static_cast<Real>( this->TheEntresol->GetTargetFrameTimeMicroseconds() ) * 0.000001) / this->SubstepModifier;
        }

        void PhysicsManager::Initialize()
        {
            if( !this->Initialized )
            {
                //WorldManager::Initialize();

                // Create the debugdrawer
                this->BulletDrawer = new debug::InternalDebugDrawer();
                this->BulletDrawer->setDebugMode( this->DebugRenderMode );
                this->BulletDynamicsWorld->setDebugDrawer( this->BulletDrawer );

                // Simulation work configuration
                if( this->WorldConstructionInfo.PhysicsFlags & ManagerConstructionInfo::PCF_Multithreaded ) {
                    this->TheEntresol->GetScheduler().AddWorkUnitMonopoly( static_cast<Threading::MonopolyWorkUnit*>( this->SimulationWork ), "SimulationWorkMonopoly" );
                }else{
                    this->TheEntresol->GetScheduler().AddWorkUnitMain( this->SimulationWork, "SimulationWorkMain" );
                }
                Graphics::GraphicsManager* GraphicsMan = this->TheEntresol->GetGraphicsManager();
                if( GraphicsMan )
                    this->SimulationWork->AddDependency( GraphicsMan->GetRenderWork() );

                Mezzanine::ActorManager* ActorMan = this->TheEntresol->GetActorManager();
                Mezzanine::AreaEffectManager* AEMan = this->TheEntresol->GetAreaEffectManager();
                Mezzanine::DebrisManager* DebrisMan = this->TheEntresol->GetDebrisManager();
                // Debug Draw work configuration
                // Must add as affinity since it manipulates raw buffers and makes rendersystem calls under the hood.
                this->TheEntresol->GetScheduler().AddWorkUnitAffinity( this->DebugDrawWork, "DebugDrawWork" );
                this->DebugDrawWork->AddDependency( this->SimulationWork );
                if( ActorMan )
                    this->DebugDrawWork->AddDependency( ActorMan->GetActorUpdateWork() );
                if( AEMan )
                    this->DebugDrawWork->AddDependency( AEMan->GetAreaEffectUpdateWork() );
                if( DebrisMan )
                    this->DebugDrawWork->AddDependency( DebrisMan->GetDebrisUpdateWork() );

                // World Trigger Update work configuration
                this->TheEntresol->GetScheduler().AddWorkUnitMain( this->WorldTriggerUpdateWork, "WorldTriggerUpdateWork" );
                this->WorldTriggerUpdateWork->AddDependency( this->SimulationWork );
                if( DebrisMan )
                    this->WorldTriggerUpdateWork->AddDependency( DebrisMan->GetDebrisUpdateWork() );

                this->Initialized = true;
            }
        }

        void PhysicsManager::Deinitialize()
        {
            if( this->Initialized )
            {
                // Destroy the debugdrawer
                delete this->BulletDrawer;
                this->BulletDrawer = NULL;
                this->BulletDynamicsWorld->setDebugDrawer( this->BulletDrawer );

                // Simulation work configuration
                if( (this->WorldConstructionInfo.PhysicsFlags & ManagerConstructionInfo::PCF_Multithreaded) && this->TheEntresol) {
                    this->TheEntresol->GetScheduler().RemoveWorkUnitMonopoly( static_cast<Threading::MonopolyWorkUnit*>( this->SimulationWork ) );
                }else{
                    this->TheEntresol->GetScheduler().RemoveWorkUnitMain( this->SimulationWork );
                }
                this->SimulationWork->ClearDependencies();

                if(this->TheEntresol)
                {
                    if(this->ThreadCount){
                        this->TheEntresol->GetScheduler().RemoveWorkUnitMonopoly( static_cast<Threading::MonopolyWorkUnit*>( this->SimulationWork ) );
                    }else{
                        this->TheEntresol->GetScheduler().RemoveWorkUnitMain( this->SimulationWork );
                    }
                    this->TheEntresol->GetScheduler().RemoveWorkUnitMain( this->WorldTriggerUpdateWork );
                    this->TheEntresol->GetScheduler().RemoveWorkUnitMain( this->DebugDrawWork );
                }



                // Debug Draw work configuration
                // Must add as affinity since it manipulates raw buffers and makes rendersystem calls under the hood.
                this->TheEntresol->GetScheduler().RemoveWorkUnitAffinity( this->DebugDrawWork );
                this->DebugDrawWork->ClearDependencies();

                // World Trigger Update work configuration
                this->TheEntresol->GetScheduler().RemoveWorkUnitMain( this->WorldTriggerUpdateWork );
                this->WorldTriggerUpdateWork->ClearDependencies();

                this->Initialized = false;
            }
        }

        void PhysicsManager::DoPerFrameWork(Threading::DefaultThreadSpecificStorage::Type &CurrentThreadStorage)
        {
            if( this->SimulationIsPaused() )
                return;

            Real FloatTime = Real(CurrentThreadStorage.GetLastFrameTime()) * 0.000001 * this->GetTimeMultiplier(); // Convert from MicroSeconds to Seconds
            int MaxSteps = ( FloatTime < this->StepSize ) ? 1 : int( FloatTime / this->StepSize ) + 1;
            CallBackWorld = this;
            this->BulletDynamicsWorld->stepSimulation( FloatTime, MaxSteps, this->StepSize );
            CallBackWorld = NULL;
        }

        Threading::DefaultWorkUnit* PhysicsManager::GetSimulationWork()
            { return this->SimulationWork; }

        WorldTriggerUpdateWorkUnit* PhysicsManager::GetWorldTriggerUpdateWork()
            { return this->WorldTriggerUpdateWork; }

        DebugDrawWorkUnit* PhysicsManager::GetDebugDrawWork()
            { return this->DebugDrawWork; }

        ///////////////////////////////////////////////////////////////////////////////
        // Type Identifier Methods

        ManagerBase::ManagerType PhysicsManager::GetInterfaceType() const
            { return ManagerBase::MT_PhysicsManager; }

        String PhysicsManager::GetImplementationTypeName() const
            { return "DefaultPhysicsManager"; }

        ///////////////////////////////////////////////////////////////////////////////
        // Internal Methods

        btSoftRigidDynamicsWorld* PhysicsManager::_GetPhysicsWorldPointer()
            { return this->BulletDynamicsWorld; }

        const btSoftRigidDynamicsWorld* PhysicsManager::_GetPhysicsWorldPointer() const
            { return this->BulletDynamicsWorld; }

        ///////////////////////////////////////////////////////////////////////////////
        // DefaultPhysicsManagerFactory Methods

        DefaultPhysicsManagerFactory::DefaultPhysicsManagerFactory()
            {}

        DefaultPhysicsManagerFactory::~DefaultPhysicsManagerFactory()
            {}

        String DefaultPhysicsManagerFactory::GetManagerTypeName() const
            { return "DefaultPhysicsManager"; }

        ManagerBase* DefaultPhysicsManagerFactory::CreateManager(NameValuePairList& Params)
        {
            if(Params.empty()) return new PhysicsManager();
            else
            {
                ManagerConstructionInfo PhysInfo;
                for( NameValuePairList::iterator ParIt = Params.begin() ; ParIt != Params.end() ; ++ParIt )
                {
                    String Lower = (*ParIt).first;
                    StringTools::ToLowerCase(Lower);
                    if( "geographyupperbounds" == Lower )
                    {
                        PhysInfo.GeographyUpperBounds = StringTools::ConvertToVector3( (*ParIt).second );
                    }
                    else if( "geographylowerbounds" == Lower )
                    {
                        PhysInfo.GeographyLowerBounds = StringTools::ConvertToVector3( (*ParIt).second );
                    }
                    else if( "maxproxies" == Lower )
                    {
                        PhysInfo.MaxProxies = StringTools::ConvertToUInt32( (*ParIt).second );
                    }
                    else if( "gravity" == Lower )
                    {
                        PhysInfo.Gravity = StringTools::ConvertToVector3( (*ParIt).second );
                    }
                    else if( "softrigidworld" == Lower )
                    {
                        if(StringTools::ConvertToBool( (*ParIt).second ))
                            PhysInfo.PhysicsFlags = (PhysInfo.PhysicsFlags | ManagerConstructionInfo::PCF_SoftRigidWorld);
                    }
                    else if( "limitlessworld" == Lower )
                    {
                        if(StringTools::ConvertToBool( (*ParIt).second ))
                            PhysInfo.PhysicsFlags = (PhysInfo.PhysicsFlags | ManagerConstructionInfo::PCF_LimitlessWorld);
                    }
                    else if( "multithreaded" == Lower )
                    {
                        if(StringTools::ConvertToBool( (*ParIt).second ))
                            PhysInfo.PhysicsFlags = (PhysInfo.PhysicsFlags | ManagerConstructionInfo::PCF_Multithreaded);
                    }
                }
                return new PhysicsManager(PhysInfo);
            }
        }

        ManagerBase* DefaultPhysicsManagerFactory::CreateManager(XML::Node& XMLNode)
            { return new PhysicsManager(XMLNode); }

        void DefaultPhysicsManagerFactory::DestroyManager(ManagerBase* ToBeDestroyed)
            { delete ToBeDestroyed; }
    }//Physics
}//Mezzanine

#endif
