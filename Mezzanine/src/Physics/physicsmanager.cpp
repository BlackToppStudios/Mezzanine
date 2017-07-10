// © Copyright 2010 - 2017 BlackTopp Studios Inc.
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

#include "Physics/physicsmanager.h"
#include "Physics/collision.h"

#include "Physics/ghostproxy.h"
#include "Physics/rigidproxy.h"
#include "Physics/softproxy.h"

#include "Physics/conetwistconstraint.h"
#include "Physics/gearconstraint.h"
#include "Physics/generic6dofconstraint.h"
#include "Physics/generic6dofspringconstraint.h"
#include "Physics/hingeconstraint.h"
#include "Physics/hinge2constraint.h"
#include "Physics/point2pointconstraint.h"
#include "Physics/sliderconstraint.h"
#include "Physics/universalconstraint.h"

#include "Graphics/graphicsmanager.h"

// Entity Manager includes are here for the debug draw work unit dependency setting
#include "entitymanager.h"

#include "stringtool.h"
#include "vector3.h"
#include "worldtrigger.h"
#include "entity.h"
#include "crossplatform.h"
#include "entresol.h"
#include "world.h"
#include "timer.h"

// Graphics includes exist for the debug drawer
#include "Graphics/linegroupproxy.h"
#include "Graphics/scenemanager.h"

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

#ifdef MEZZ_WINDOWS
#include <BulletMultiThreaded/Win32ThreadSupport.h>
#else
#include <BulletMultiThreaded/PosixThreadSupport.h>
#endif

#include <BulletMultiThreaded/btParallelConstraintSolver.h>
#include <BulletMultiThreaded/SpuGatheringCollisionDispatcher.h>
#include <BulletMultiThreaded/SpuNarrowPhaseCollisionTask/SpuGatheringCollisionTask.h>


namespace Mezzanine
{
    #ifdef GetObject
    #undef GetObject
    #endif
    namespace Physics
    {
        /// @internal
        /// @class InternalDebugDrawer
        /// @brief This is used to draw wireframse for the Physics subsystem
        class InternalDebugDrawer : public btIDebugDraw
        {
        private:
            /// @internal
            /// @brief A pointer to the scene manager that the drawer will render to.
            Graphics::SceneManager* SceneMan;
            /// @internal
            /// @brief This stores the wireframe being used for rendering.
            Graphics::LineGroupProxy* WireFrame;
            /// @internal
            /// @brief A pointer to the safe logger for debug output.
            Logger* ErrorLogger;
            /// @internal
            /// @brief This stores whether or not to render physics debug lines
            /// @details This stores whether or not to render physics debud lines. 0 = Do not draw anything. 1 = Draw model wireframes.
            /// Later we will add support for contact drawing, individual modeling drawing, etc...
            int DebugDrawing;
        public:
            /// @internal
            /// @brief Class constructor.
            /// @param A pointer to the scene manager that the drawer will render to.
            InternalDebugDrawer(Graphics::SceneManager* Manager);
            /// @internal
            /// @brief Class destructor.
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
            /// @brief Sets the safe logger to sent debug output to.
            /// @param Logger A pointer to the safe logger for debug output.
            virtual void SetLogger(Logger* Logger);
            /// @internal
            /// @brief Used by the physics subsystem to report errors using the renderer
            /// @details We *Believe* that this is used by the physics subsystem to report errors about rendering to the developer/user. As such, we
            /// Have redirected all input from this function to the Entresol::Log function.
            /// @param warningString We *Believe* These are messagesfrom the physics subsystem, and that this should not directly called otherwise
            virtual void reportErrorWarning(const char* warningString);
        };

        InternalDebugDrawer::InternalDebugDrawer(Graphics::SceneManager* Manager) :
            SceneMan(Manager),
            WireFrame(NULL),
            ErrorLogger(NULL),
            DebugDrawing(Physics::DDM_NoDebug)
            {  }//this->WireFrame = new Graphics::LineGroupProxy(0,this->SceneMan); }//SceneMan->CreateLineGroupProxy(); }

        InternalDebugDrawer::~InternalDebugDrawer()
            { if( this->WireFrame != NULL ) delete this->WireFrame; }//SceneMan->DestroyComponent( this->WireFrame ); }

        void InternalDebugDrawer::PrepareForUpdate()
        {
            /// @todo As you can see from the commented code I originally tried making the WireFrame on construction, rather
            /// than on demand in order to remove unnecessary NULL checks.  However this prevented it from rendering despite
            /// all the obvious data being valid while debugging (visibility masks, in scene graph, buffer populated with
            /// valid data, etc.).  This should be investigated but my best theory at the time of the writing is that hardware
            /// buffers we get when made around the time managers are initialized (since this is created at physics manager
            /// initialization) aren't valid somehow.
            if( this->WireFrame == NULL ) {
                this->WireFrame = new Graphics::LineGroupProxy(0,this->SceneMan);
            }
            this->WireFrame->AddToWorld();
            this->WireFrame->ClearPoints();
        }

        void InternalDebugDrawer::FinalizeUpdate()
        {
            this->WireFrame->UpdateBuffers();
        }

        void InternalDebugDrawer::drawLine(const btVector3& from,const btVector3& to,const btVector3& color)
            { this->WireFrame->AddLine( Vector3(from), Vector3(to), ColourValue(color.getX(),color.getY(),color.getZ()) ); }

        void InternalDebugDrawer::drawContactPoint(const btVector3& PointOnB,const btVector3& normalOnB,btScalar distance,int lifeTime,const btVector3& color)
            {  }

        void InternalDebugDrawer::draw3dText(const btVector3& location,const char* textString)
            {  }

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

        void InternalDebugDrawer::SetLogger(Logger* Logger)
            { this->ErrorLogger = Logger; }

        void InternalDebugDrawer::reportErrorWarning(const char* warningString)
            { (*this->ErrorLogger) << warningString << std::endl; }

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
            InternalDebugDrawer* Drawer = this->TargetManager->BulletDrawer;
            Drawer->SetLogger( &CurrentThreadStorage.GetUsableLogger() );
            if( Drawer && Drawer->getDebugMode() )        //this part is responsible for drawing the wireframes
            {
                Drawer->PrepareForUpdate();
                this->TargetManager->BulletDynamicsWorld->debugDrawWorld();
                Drawer->FinalizeUpdate();
            }
            Drawer->SetLogger(NULL);
        }

        ///////////////////////////////////////////////////////////
        // Physicsmanager functions

        const String PhysicsManager::ImplementationName = "DefaultPhysicsManager";
        const ManagerBase::ManagerType PhysicsManager::InterfaceType = ManagerBase::MT_PhysicsManager;

        PhysicsManager::PhysicsManager(World* Creator) :
            EntityComponentManager(Creator),
            StepSize(1.0/60.0),
            TimeMultiplier(1.0),
            DebugRenderMode(0),
            SubstepModifier(1),
            ThreadCount(0),
            SimulationPaused(false),

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

        PhysicsManager::PhysicsManager(World* Creator, const ManagerConstructionInfo& Info) :
            EntityComponentManager(Creator),
            StepSize(1.0/60.0),
            TimeMultiplier(1.0),
            DebugRenderMode(0),
            SubstepModifier(1),
            ThreadCount(0),
            SimulationPaused(false),

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

        PhysicsManager::PhysicsManager(World* Creator, const XML::Node& XMLNode) :
            EntityComponentManager(Creator),
            StepSize(1.0/60.0),
            TimeMultiplier(1.0),
            DebugRenderMode(0),
            SubstepModifier(1),
            ThreadCount(0),
            SimulationPaused(false),

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
            if(!WorldSettings.Empty()) {
                CurrAttrib = WorldSettings.GetAttribute("LimitlessWorld");
                if(!CurrAttrib.Empty()) {
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
                if(!CurrAttrib.Empty()) {
                    Info.PhysicsFlags = (Info.PhysicsFlags | ManagerConstructionInfo::PCF_SoftRigidWorld);
                }
                CurrAttrib = WorldSettings.GetAttribute("MultiThreaded");
                if(!CurrAttrib.Empty()) {
                    Info.PhysicsFlags = (Info.PhysicsFlags | ManagerConstructionInfo::PCF_Multithreaded);
                }
            }

            this->Construct(Info);

            XML::Node StepModifier = XMLNode.GetChild("SubStepModifier");
            if(!StepModifier.Empty()) {
                CurrAttrib = WorldSettings.GetAttribute("Modifier");
                if(!CurrAttrib.Empty()) {
                    SetSimulationSubstepModifier(CurrAttrib.AsWhole());
                }
            }

            XML::Node DebugRender = XMLNode.GetChild("DebugRendering");
            if(!DebugRender.Empty()) {
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
            this->DestroyAllComponents();
            this->DestroyAllWorldTriggers();

            this->Deinitialize();

            //Destroy the physical world that we loved and cherished
            this->Destroy();
        }

        void PhysicsManager::Construct(const ManagerConstructionInfo& Info)
        {
            this->ThreadCount = ( Info.PhysicsFlags & ManagerConstructionInfo::PCF_Multithreaded) ? crossplatform::GetCPUCount() : 0;

            // Create the broadphase
            if( Info.PhysicsFlags & ManagerConstructionInfo::PCF_LimitlessWorld ) {
                this->BulletBroadphase = new btDbvtBroadphase();
            }else{
                if( Info.MaxProxies < 65536 ) {
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
            }// */

            // Create the dispatcher (narrowphase)
            if( Info.PhysicsFlags & ManagerConstructionInfo::PCF_Multithreaded ) {
                #ifdef MEZZ_WINDOWS
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
                this->BulletDispatcher = new ParallelCollisionDispatcher(this, this->BulletDispatcherThreads,ThreadCount,this->BulletCollisionConfiguration);
            }else{
                this->BulletDispatcher = new CollisionDispatcher(this, this->BulletCollisionConfiguration);
            }

            // Create the constraint solver
            /*if( Info.PhysicsFlags & ManagerConstructionInfo::PCF_Multithreaded ) {
                #ifdef MEZZ_WINDOWS
                Win32ThreadSupport::Win32ThreadConstructionInfo BulletThreadInfo( "SolverThreads",
                                                                                  SolverThreadFunc,
                                                                                  SolverlsMemoryFunc,
                                                                                  ThreadCount );
                this->BulletSolverThreads = new Win32ThreadSupport(BulletThreadInfo);
                #else //WINDOWS
                PosixThreadSupport::ThreadConstructionInfo BulletThreadInfo( "SolverThreads",
                                                                             SolverThreadFunc,
                                                                             SolverlsMemoryFunc,
                                                                             ThreadCount );
                this->BulletSolverThreads = new PosixThreadSupport(BulletThreadInfo);
                #endif //WINDOWS
                this->BulletSolver = new btParallelConstraintSolver(this->BulletSolverThreads);
            }else{// */
                this->BulletSolver = new btSequentialImpulseConstraintSolver();
            //}

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
            }// */

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

            this->BulletDynamicsWorld->setInternalTickCallback((btInternalTickCallback)PhysicsManager::InternalPreTickCallback,this,true);
            this->BulletDynamicsWorld->setInternalTickCallback((btInternalTickCallback)PhysicsManager::InternalPostTickCallback,this,false);

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
            if( this->BulletDrawer ) {
                delete this->BulletDrawer;
                this->BulletDrawer = NULL;
            }
            if( this->BulletSolverThreads ) {
                delete this->BulletSolverThreads;
                this->BulletSolverThreads = NULL;
            }
            if( this->BulletDispatcherThreads ) {
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
            for( PhysicsManager::CollisionMapIterator ColIt = Collisions.begin() ; ColIt != Collisions.end() ; ColIt++ )
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
            Entresol::GetSingletonPtr()->DoMainLoopLogging();// */
            #endif
            btCollisionAlgorithm* NewAlgo = AlgoQueue->front();
            while( NewAlgo != NULL )
            {
                /*for( PhysicsManager::CollisionMapIterator ColIt = this->Collisions.begin() ; ColIt != this->Collisions.end() ; ++ColIt )
                {
                    if( NewAlgo == (*ColIt).second->InternalAlgo ) {

                        break;
                    }
                }// */
                // Old method involving detecting the actual Entity pair
                CollidableProxy* ProxA = NULL;
                CollidableProxy* ProxB = NULL;
                /// @todo This is an absurd round-about way to get the data we need,
                /// and bullet will probably have to be extended to change this so it's actually good.
                btBroadphasePairArray& PairArray = BulletBroadphase->getOverlappingPairCache()->getOverlappingPairArray();
                for( Integer X = 0 ; X < PairArray.size() ; ++X )
                {
                    if( NewAlgo == PairArray[X].m_algorithm ) {
                        btCollisionObject* COA = (btCollisionObject*)PairArray[X].m_pProxy0->m_clientObject;
                        ProxA = static_cast<CollidableProxy*>( COA->getUserPointer() );
                        btCollisionObject* COB = (btCollisionObject*)PairArray[X].m_pProxy1->m_clientObject;
                        ProxB = static_cast<CollidableProxy*>( COB->getUserPointer() );
                        break;
                    }
                }

                if( ( ProxA != NULL && ProxA->GetCollisionResponse() ) && ( ProxB != NULL && ProxB->GetCollisionResponse() ) ) {
                    // Create the collision
                    CollidablePair NewPair(ProxA,ProxB);
                    /*PhysicsManager::CollisionMapIterator ColIt = Collisions.find(NewPair);
                    if(ColIt == Collisions.end()) {
                        Physics::Collision* NewCol = new Physics::Collision(ProxA,ProxB,NewAlgo);
                        //NewCol->GetActorA()->_NotifyCollisionState(NewCol,Physics::Collision::Col_Begin);
                        //NewCol->GetActorB()->_NotifyCollisionState(NewCol,Physics::Collision::Col_Begin);
                        Collisions.insert( CollisionSortPair(NewPair,NewCol) );
                    }// */
                    PhysicsManager::CollisionMapIterator ColIt = this->Collisions.begin();
                    while( ColIt != this->Collisions.end() )
                    {
                        if( NewAlgo == (*ColIt).second->InternalAlgo )
                            break;
                        ++ColIt;
                    }

                    if( ColIt == this->Collisions.end() ) {
                        Physics::Collision* NewCol = new Physics::Collision(ProxA,ProxB,NewAlgo);
                        //NewCol->GetActorA()->_NotifyCollisionState(NewCol,Physics::Collision::Col_Begin);
                        //NewCol->GetActorB()->_NotifyCollisionState(NewCol,Physics::Collision::Col_Begin);
                        Collisions.insert( CollisionSortPair(NewPair,NewCol) );
                    }
                }// */
                AlgoQueue->pop_front();
                if(AlgoQueue->size() > 0) NewAlgo = AlgoQueue->front();
                else NewAlgo = NULL;
            }// */
            AlgoQueue->clear();
        }

        void PhysicsManager::InternalPreTickCallback(btDynamicsWorld* world, btScalar timeStep)
        {
            //PhysicsManager* Physman = static_cast<PhysicsManager*>( world->getWorldUserInfo() );
            //if( Physman != NULL ) {

            //}
        }

        void PhysicsManager::InternalPostTickCallback(btDynamicsWorld* world, btScalar timeStep)
        {
            PhysicsManager* Physman = static_cast<PhysicsManager*>( world->getWorldUserInfo() );
            if( Physman != NULL ) {
                Physman->ProcessAllCollisions();
            }
        }

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
            GhostProxy* NewProxy = new GhostProxy(this->ProxyIDGen.GenerateID(),this);
            this->Proxies.push_back(NewProxy);
            return NewProxy;
        }

        GhostProxy* PhysicsManager::CreateGhostProxy(CollisionShape* Shape)
        {
            GhostProxy* NewProxy = new GhostProxy(this->ProxyIDGen.GenerateID(),Shape,this);
            this->Proxies.push_back(NewProxy);
            return NewProxy;
        }

        GhostProxy* PhysicsManager::CreateGhostProxy(const XML::Node& SelfRoot)
        {
            GhostProxy* NewProxy = new GhostProxy(SelfRoot,this);
            this->ProxyIDGen.ReserveID(NewProxy->GetComponentID());
            this->Proxies.push_back(NewProxy);
            return NewProxy;
        }

        RigidProxy* PhysicsManager::CreateRigidProxy(const Real Mass)
        {
            RigidProxy* NewProxy = new RigidProxy(this->ProxyIDGen.GenerateID(),Mass,this);
            this->Proxies.push_back(NewProxy);
            return NewProxy;
        }

        RigidProxy* PhysicsManager::CreateRigidProxy(const Real Mass, CollisionShape* Shape)
        {
            RigidProxy* NewProxy = new RigidProxy(this->ProxyIDGen.GenerateID(),Mass,Shape,this);
            this->Proxies.push_back(NewProxy);
            return NewProxy;
        }

        RigidProxy* PhysicsManager::CreateRigidProxy(const XML::Node& SelfRoot)
        {
            RigidProxy* NewProxy = new RigidProxy(SelfRoot,this);
            this->ProxyIDGen.ReserveID(NewProxy->GetComponentID());
            this->Proxies.push_back(NewProxy);
            return NewProxy;
        }

        SoftProxy* PhysicsManager::CreateSoftProxy(const Real Mass)
        {
            SoftProxy* NewProxy = new SoftProxy(this->ProxyIDGen.GenerateID(),Mass,this);
            this->Proxies.push_back(NewProxy);
            return NewProxy;
        }

        SoftProxy* PhysicsManager::CreateSoftProxy(const XML::Node& SelfRoot)
        {
            SoftProxy* NewProxy = new SoftProxy(SelfRoot,this);
            this->ProxyIDGen.ReserveID(NewProxy->GetComponentID());
            this->Proxies.push_back(NewProxy);
            return NewProxy;
        }

        EntityComponent* PhysicsManager::CreateComponent(const XML::Node& SelfRoot)
        {
            if( SelfRoot.Name() == RigidProxy::GetSerializableName() ) return this->CreateRigidProxy(SelfRoot);
            else if( SelfRoot.Name() == GhostProxy::GetSerializableName() ) return this->CreateGhostProxy(SelfRoot);
            else if( SelfRoot.Name() == SoftProxy::GetSerializableName() ) return this->CreateSoftProxy(SelfRoot);
            else return NULL;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Proxy Management

        CollidableProxy* PhysicsManager::GetProxy(const UInt32 Index) const
            { return this->Proxies.at(Index); }

        EntityComponent* PhysicsManager::GetComponentByID(const UInt32 ID) const
        {
            for( ConstProxyIterator ProxIt = this->Proxies.begin() ; ProxIt != this->Proxies.end() ; ++ProxIt )
            {
                if( (*ProxIt)->GetComponentID() == ID ) {
                    return (*ProxIt);
                }
            }
            return NULL;
        }

        UInt32 PhysicsManager::GetNumComponents() const
        {
            return this->Proxies.size();
        }

        UInt32 PhysicsManager::GetNumComponents(const UInt32 Types) const
        {
            UInt32 Count = 0;
            for( ConstProxyIterator ProxIt = this->Proxies.begin() ; ProxIt != this->Proxies.end() ; ++ProxIt )
            {
                if( (*ProxIt)->GetComponentType() == Types ) {
                    ++Count;
                }
            }
            return Count;
        }

        EntityComponentManager::ComponentVec PhysicsManager::GetComponents() const
        {
            return ComponentVec(this->Proxies.begin(),this->Proxies.end());
        }

        void PhysicsManager::DestroyComponent(EntityComponent* ToBeDestroyed)
        {
            for( ProxyIterator ProxIt = this->Proxies.begin() ; ProxIt != this->Proxies.end() ; ++ProxIt )
            {
                if( ToBeDestroyed == (*ProxIt) ) {
                    Entity* Parent = (*ProxIt)->GetParentEntity();
                    if( Parent )
                        Parent->RemoveComponent( (*ProxIt) );

                    this->ProxyIDGen.ReleaseID( ToBeDestroyed->GetComponentID() );
                    delete (*ProxIt);
                    this->Proxies.erase(ProxIt);
                    return;
                }
            }
        }

        void PhysicsManager::DestroyAllComponents(const UInt32 Types)
        {
            ProxyContainer ToKeep;
            for( ProxyIterator ProxIt = this->Proxies.begin() ; ProxIt != this->Proxies.end() ; ++ProxIt )
            {
                if( (*ProxIt)->GetComponentType() & Types ) {
                    Entity* Parent = (*ProxIt)->GetParentEntity();
                    if( Parent )
                        Parent->RemoveComponent( (*ProxIt) );

                    this->ProxyIDGen.ReleaseID( (*ProxIt)->GetComponentID() );
                    delete (*ProxIt);
                }else{
                    ToKeep.push_back( *ProxIt );
                }
            }
            this->Proxies.clear();
            this->Proxies.swap(ToKeep);
        }

        void PhysicsManager::DestroyAllComponents()
        {
            for( ProxyIterator ProxIt = this->Proxies.begin() ; ProxIt != this->Proxies.end() ; ++ProxIt )
            {
                Entity* Parent = (*ProxIt)->GetParentEntity();
                if( Parent )
                    Parent->RemoveComponent( (*ProxIt) );

                this->ProxyIDGen.ReleaseID( (*ProxIt)->GetComponentID() );
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
        // Constraint Creation

        ConeTwistConstraint* PhysicsManager::CreateConeTwistConstraint(RigidProxy* ProxyA, RigidProxy* ProxyB, const Transform& TransA, const Transform& TransB)
        {
            ConeTwistConstraint* NewConstraint = new ConeTwistConstraint(this->ConstraintIDGen.GenerateID(),ProxyA,ProxyB,TransA,TransB,this);
            this->Constraints.push_back(NewConstraint);
            return NewConstraint;
        }

        ConeTwistConstraint* PhysicsManager::CreateConeTwistConstraint(RigidProxy* ProxyA, const Transform& TransA)
        {
            ConeTwistConstraint* NewConstraint = new ConeTwistConstraint(this->ConstraintIDGen.GenerateID(),ProxyA,TransA,this);
            this->Constraints.push_back(NewConstraint);
            return NewConstraint;
        }

        ConeTwistConstraint* PhysicsManager::CreateConeTwistConstraint(const XML::Node& SelfRoot)
        {
            ConeTwistConstraint* NewConstraint = new ConeTwistConstraint(SelfRoot,this);
            this->ConstraintIDGen.ReserveID(NewConstraint->GetConstraintID());
            this->Constraints.push_back(NewConstraint);
            return NewConstraint;
        }

        GearConstraint* PhysicsManager::CreateGearConstraint(RigidProxy* ProxyA, RigidProxy* ProxyB, const Vector3& AxisA, const Vector3& AxisB)
        {
            GearConstraint* NewConstraint = new GearConstraint(this->ConstraintIDGen.GenerateID(),ProxyA,ProxyB,AxisA,AxisB,this);
            this->Constraints.push_back(NewConstraint);
            return NewConstraint;
        }

        GearConstraint* PhysicsManager::CreateGearConstraint(RigidProxy* ProxyA, RigidProxy* ProxyB, const Vector3& AxisA, const Vector3& AxisB, const Real Ratio)
        {
            GearConstraint* NewConstraint = new GearConstraint(this->ConstraintIDGen.GenerateID(),ProxyA,ProxyB,AxisA,AxisB,Ratio,this);
            this->Constraints.push_back(NewConstraint);
            return NewConstraint;
        }

        GearConstraint* PhysicsManager::CreateGearConstraint(const XML::Node& SelfRoot)
        {
            GearConstraint* NewConstraint = new GearConstraint(SelfRoot,this);
            this->ConstraintIDGen.ReserveID(NewConstraint->GetConstraintID());
            this->Constraints.push_back(NewConstraint);
            return NewConstraint;
        }

        Generic6DofConstraint* PhysicsManager::CreateGeneric6DofConstraint(RigidProxy* ProxyA, RigidProxy* ProxyB, const Transform& TransA, const Transform& TransB)
        {
            Generic6DofConstraint* NewConstraint = new Generic6DofConstraint(this->ConstraintIDGen.GenerateID(),ProxyA,ProxyB,TransA,TransB,this);
            this->Constraints.push_back(NewConstraint);
            return NewConstraint;
        }

        Generic6DofConstraint* PhysicsManager::CreateGeneric6DofConstraint(RigidProxy* ProxyB, const Transform& TransB)
        {
            Generic6DofConstraint* NewConstraint = new Generic6DofConstraint(this->ConstraintIDGen.GenerateID(),ProxyB,TransB,this);
            this->Constraints.push_back(NewConstraint);
            return NewConstraint;
        }

        Generic6DofConstraint* PhysicsManager::CreateGeneric6DofConstraint(const XML::Node& SelfRoot)
        {
            Generic6DofConstraint* NewConstraint = new Generic6DofConstraint(SelfRoot,this);
            this->ConstraintIDGen.ReserveID(NewConstraint->GetConstraintID());
            this->Constraints.push_back(NewConstraint);
            return NewConstraint;
        }

        Generic6DofSpringConstraint* PhysicsManager::CreateGeneric6DofSpringConstraint(RigidProxy* ProxyA, RigidProxy* ProxyB, const Transform& TransA, const Transform& TransB)
        {
            Generic6DofSpringConstraint* NewConstraint = new Generic6DofSpringConstraint(this->ConstraintIDGen.GenerateID(),ProxyA,ProxyB,TransA,TransB,this);
            this->Constraints.push_back(NewConstraint);
            return NewConstraint;
        }

        Generic6DofSpringConstraint* PhysicsManager::CreateGeneric6DofSpringConstraint(const XML::Node& SelfRoot)
        {
            Generic6DofSpringConstraint* NewConstraint = new Generic6DofSpringConstraint(SelfRoot,this);
            this->ConstraintIDGen.ReserveID(NewConstraint->GetConstraintID());
            this->Constraints.push_back(NewConstraint);
            return NewConstraint;
        }

        HingeConstraint* PhysicsManager::CreateHingeConstraint(RigidProxy* ProxyA, RigidProxy* ProxyB, const Vector3& PivotInA, const Vector3& PivotInB, const Vector3& AxisInA, const Vector3& AxisInB)
        {
            HingeConstraint* NewConstraint = new HingeConstraint(this->ConstraintIDGen.GenerateID(),ProxyA,ProxyB,PivotInA,PivotInB,AxisInA,AxisInB,this);
            this->Constraints.push_back(NewConstraint);
            return NewConstraint;
        }

        HingeConstraint* PhysicsManager::CreateHingeConstraint(RigidProxy* ProxyA, RigidProxy* ProxyB, const Transform& TransA, const Transform& TransB)
        {
            HingeConstraint* NewConstraint = new HingeConstraint(this->ConstraintIDGen.GenerateID(),ProxyA,ProxyB,TransA,TransB,this);
            this->Constraints.push_back(NewConstraint);
            return NewConstraint;
        }

        HingeConstraint* PhysicsManager::CreateHingeConstraint(RigidProxy* ProxyA, const Vector3& PivotInA, const Vector3& AxisInA)
        {
            HingeConstraint* NewConstraint = new HingeConstraint(this->ConstraintIDGen.GenerateID(),ProxyA,PivotInA,AxisInA,this);
            this->Constraints.push_back(NewConstraint);
            return NewConstraint;
        }

        HingeConstraint* PhysicsManager::CreateHingeConstraint(RigidProxy* ProxyA, const Transform& TransA)
        {
            HingeConstraint* NewConstraint = new HingeConstraint(this->ConstraintIDGen.GenerateID(),ProxyA,TransA,this);
            this->Constraints.push_back(NewConstraint);
            return NewConstraint;
        }

        HingeConstraint* PhysicsManager::CreateHingeConstraint(const XML::Node& SelfRoot)
        {
            HingeConstraint* NewConstraint = new HingeConstraint(SelfRoot,this);
            this->ConstraintIDGen.ReserveID(NewConstraint->GetConstraintID());
            this->Constraints.push_back(NewConstraint);
            return NewConstraint;
        }

        Hinge2Constraint* PhysicsManager::CreateHinge2Constraint(RigidProxy* ProxyA, RigidProxy* ProxyB, const Vector3& Anchor, const Vector3& Axis1, const Vector3& Axis2)
        {
            Hinge2Constraint* NewConstraint = new Hinge2Constraint(this->ConstraintIDGen.GenerateID(),ProxyA,ProxyB,Anchor,Axis1,Axis2,this);
            this->Constraints.push_back(NewConstraint);
            return NewConstraint;
        }

        Hinge2Constraint* PhysicsManager::CreateHinge2Constraint(RigidProxy* ProxyA, RigidProxy* ProxyB, const Transform& TransA, const Transform& TransB)
        {
            Hinge2Constraint* NewConstraint = new Hinge2Constraint(this->ConstraintIDGen.GenerateID(),ProxyA,ProxyB,TransA,TransB,this);
            this->Constraints.push_back(NewConstraint);
            return NewConstraint;
        }

        Hinge2Constraint* PhysicsManager::CreateHinge2Constraint(const XML::Node& SelfRoot)
        {
            Hinge2Constraint* NewConstraint = new Hinge2Constraint(SelfRoot,this);
            this->ConstraintIDGen.ReserveID(NewConstraint->GetConstraintID());
            this->Constraints.push_back(NewConstraint);
            return NewConstraint;
        }

        Point2PointConstraint* PhysicsManager::CreatePoint2PointConstraint(RigidProxy* ProxyA, RigidProxy* ProxyB, const Vector3& PivotA, const Vector3& PivotB)
        {
            Point2PointConstraint* NewConstraint = new Point2PointConstraint(this->ConstraintIDGen.GenerateID(),ProxyA,ProxyB,PivotA,PivotB,this);
            this->Constraints.push_back(NewConstraint);
            return NewConstraint;
        }

        Point2PointConstraint* PhysicsManager::CreatePoint2PointConstraint(RigidProxy* ProxyA, const Vector3& PivotA)
        {
            Point2PointConstraint* NewConstraint = new Point2PointConstraint(this->ConstraintIDGen.GenerateID(),ProxyA,PivotA,this);
            this->Constraints.push_back(NewConstraint);
            return NewConstraint;
        }

        Point2PointConstraint* PhysicsManager::CreatePoint2PointConstraint(const XML::Node& SelfRoot)
        {
            Point2PointConstraint* NewConstraint = new Point2PointConstraint(SelfRoot,this);
            this->ConstraintIDGen.ReserveID(NewConstraint->GetConstraintID());
            this->Constraints.push_back(NewConstraint);
            return NewConstraint;
        }

        SliderConstraint* PhysicsManager::CreateSliderConstraint(RigidProxy* ProxyA, RigidProxy* ProxyB, const Transform& TransA, const Transform& TransB)
        {
            SliderConstraint* NewConstraint = new SliderConstraint(this->ConstraintIDGen.GenerateID(),ProxyA,ProxyB,TransA,TransB,this);
            this->Constraints.push_back(NewConstraint);
            return NewConstraint;
        }

        SliderConstraint* PhysicsManager::CreateSliderConstraint(RigidProxy* ProxyA, const Transform& TransA)
        {
            SliderConstraint* NewConstraint = new SliderConstraint(this->ConstraintIDGen.GenerateID(),ProxyA,TransA,this);
            this->Constraints.push_back(NewConstraint);
            return NewConstraint;
        }

        SliderConstraint* PhysicsManager::CreateSliderConstraint(const XML::Node& SelfRoot)
        {
            SliderConstraint* NewConstraint = new SliderConstraint(SelfRoot,this);
            this->ConstraintIDGen.ReserveID(NewConstraint->GetConstraintID());
            this->Constraints.push_back(NewConstraint);
            return NewConstraint;
        }

        UniversalConstraint* PhysicsManager::CreateUniversalConstraint(RigidProxy* ProxyA, RigidProxy* ProxyB, const Vector3& Anchor, const Vector3& Axis1, const Vector3& Axis2)
        {
            UniversalConstraint* NewConstraint = new UniversalConstraint(this->ConstraintIDGen.GenerateID(),ProxyA,ProxyB,Anchor,Axis1,Axis2,this);
            this->Constraints.push_back(NewConstraint);
            return NewConstraint;
        }

        UniversalConstraint* PhysicsManager::CreateUniversalConstraint(RigidProxy* ProxyA, RigidProxy* ProxyB, const Transform& TransA, const Transform& TransB)
        {
            UniversalConstraint* NewConstraint = new UniversalConstraint(this->ConstraintIDGen.GenerateID(),ProxyA,ProxyB,TransA,TransB,this);
            this->Constraints.push_back(NewConstraint);
            return NewConstraint;
        }

        UniversalConstraint* PhysicsManager::CreateUniversalConstraint(const XML::Node& SelfRoot)
        {
            UniversalConstraint* NewConstraint = new UniversalConstraint(SelfRoot,this);
            this->ConstraintIDGen.ReserveID(NewConstraint->GetConstraintID());
            this->Constraints.push_back(NewConstraint);
            return NewConstraint;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Constraint Management

        Constraint* PhysicsManager::GetConstraint(const Whole& Index)
            { return this->Constraints[Index]; }

        Whole PhysicsManager::GetNumConstraints()
            { return this->Constraints.size(); }

        void PhysicsManager::DestroyConstraint(Constraint* Con)
        {
            for( ConstraintIterator ConIt = this->Constraints.begin() ; ConIt < this->Constraints.end() ; ConIt++ )
            {
                if( (*ConIt) == Con ) {
                    (*ConIt)->EnableConstraint(false);
                    this->ConstraintIDGen.ReleaseID((*ConIt)->GetConstraintID());
                    delete (*ConIt);
                    this->Constraints.erase(ConIt);
                    return;
                }
            }
        }

        void PhysicsManager::DestroyAllConstraints()
        {
            for( ConstraintIterator ConIt = this->Constraints.begin() ; ConIt != this->Constraints.end() ; ConIt++ )
            {
                (*ConIt)->EnableConstraint(false);
                this->ConstraintIDGen.ReleaseID((*ConIt)->GetConstraintID());
                delete (*ConIt);
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
                if( Name == (*Trig)->GetName() ) {
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
                if( Trig == (*T) ) {
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

        Physics::Collision* PhysicsManager::GetCollision(CollidableProxy* A, CollidableProxy* B)
        {
            CollidablePair Pair(A,B);
            return this->GetCollision(&Pair);
        }

        Physics::Collision* PhysicsManager::GetCollision(CollidablePair* Pair)
        {
            ConstCollisionMapIterator ColIt = this->Collisions.find(*Pair);
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
            // A proxy not in the world can't have collisions
            if( Proxy->IsInWorld() ) {
                this->BulletBroadphase->getOverlappingPairCache()->cleanProxyFromPairs( Proxy->_GetBasePhysicsObject()->getBroadphaseHandle(), this->BulletDispatcher );

                CollisionMapIterator ColIt = this->Collisions.begin();
                while( ColIt != this->Collisions.end() )
                {
                    Physics::Collision* ToBeDestroyed = (*ColIt).second;
                    if( Proxy == (*ColIt).second->ProxyA || Proxy == (*ColIt).second->ProxyB ) {
                        CollisionMapIterator Delete = ColIt;
                        ++ColIt;
                        this->Collisions.erase(Delete);
                        delete ToBeDestroyed;
                    }else{
                        ++ColIt;
                    }
                }
            }
        }

        void PhysicsManager::DestroyAllCollisions()
        {
            for( CollisionMapIterator ColIt = this->Collisions.begin() ; ColIt != this->Collisions.end() ; ++ColIt )
            {
                delete (*ColIt).second;
            }
            this->Collisions.clear();
        }

        PhysicsManager::CollisionMapIterator PhysicsManager::BeginCollision()
            { return this->Collisions.begin(); }

        PhysicsManager::CollisionMapIterator PhysicsManager::EndCollision()
            { return this->Collisions.end(); }

        PhysicsManager::ConstCollisionMapIterator PhysicsManager::BeginCollision() const
            { return this->Collisions.begin(); }

        PhysicsManager::ConstCollisionMapIterator PhysicsManager::EndCollision() const
            { return this->Collisions.end(); }

        ///////////////////////////////////////////////////////////////////////////////
        // Debug Management

        void PhysicsManager::SetDebugRenderingMode(const Integer DebugRenderingMode)
        {
            this->DebugRenderMode = DebugRenderingMode;
            if( this->BulletDrawer ) {
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
            if( Info != NULL ) {
                this->Construct( *Info );
            }else{
                this->Construct( WorldConstructionInfo );
            }

            if( this->Initialized && this->BulletDrawer == NULL ) {
                Graphics::SceneManager* SceneMan = static_cast<Graphics::SceneManager*>( this->ParentWorld->GetManager(ManagerBase::MT_SceneManager) );
                this->BulletDrawer = new InternalDebugDrawer(SceneMan);
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
            if( !this->Initialized ) {
                WorldManager::Initialize();

                // Create the debugdrawer
                Graphics::SceneManager* SceneMan = static_cast<Graphics::SceneManager*>( this->ParentWorld->GetManager(ManagerBase::MT_SceneManager) );
                this->BulletDrawer = new InternalDebugDrawer(SceneMan);
                this->BulletDrawer->setDebugMode( this->DebugRenderMode );
                this->BulletDynamicsWorld->setDebugDrawer( this->BulletDrawer );

                // Simulation work configuration
                if( this->WorldConstructionInfo.PhysicsFlags & ManagerConstructionInfo::PCF_Multithreaded ) {
                    this->TheEntresol->GetScheduler().AddWorkUnitMonopoly( static_cast<Threading::MonopolyWorkUnit*>( this->SimulationWork ), "SimulationWorkMonopoly" );
                }else{
                    this->TheEntresol->GetScheduler().AddWorkUnitMain( this->SimulationWork, "SimulationWorkMain" );
                }
                Graphics::GraphicsManager* GraphicsMan = static_cast<Graphics::GraphicsManager*>( this->TheEntresol->GetManager(ManagerBase::MT_GraphicsManager) );
                if( GraphicsMan )
                    this->SimulationWork->AddDependency( GraphicsMan->GetRenderWork() );

                EntityManager* EntMan = static_cast<EntityManager*>( this->ParentWorld->GetManager(ManagerBase::MT_EntityManager) );
                // Debug Draw work configuration
                // Must add as affinity since it manipulates raw buffers and makes rendersystem calls under the hood.
                this->TheEntresol->GetScheduler().AddWorkUnitAffinity( this->DebugDrawWork, "DebugDrawWork" );
                this->DebugDrawWork->AddDependency( this->SimulationWork );
                if( EntMan ) {
                    this->DebugDrawWork->AddDependency( EntMan->GetActorUpdateWork() );
                    this->DebugDrawWork->AddDependency( EntMan->GetAreaEffectUpdateWork() );
                    this->DebugDrawWork->AddDependency( EntMan->GetDebrisUpdateWork() );
                }

                // World Trigger Update work configuration
                this->TheEntresol->GetScheduler().AddWorkUnitMain( this->WorldTriggerUpdateWork, "WorldTriggerUpdateWork" );
                this->WorldTriggerUpdateWork->AddDependency( this->SimulationWork );
                if( EntMan ) {
                    this->WorldTriggerUpdateWork->AddDependency( EntMan->GetDebrisUpdateWork() );
                }

                this->Initialized = true;
            }
        }

        void PhysicsManager::Deinitialize()
        {
            if( this->Initialized ) {
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

                if( this->TheEntresol ) {
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

            Logger& ThreadLog = CurrentThreadStorage.GetUsableLogger();
            ThreadLog << "Attempting to step the physics simulation by " << FloatTime << " seconds with " << MaxSteps << " maximum substeps." << std::endl;
            Timer PhysicsTimer;
            PhysicsTimer.Start();

            this->BulletDynamicsWorld->stepSimulation( FloatTime, MaxSteps, this->StepSize );

            PhysicsTimer.Stop();
            ThreadLog << "StepSimulation took " << PhysicsTimer.GetCurrentTimeInMilliseconds() << " milliseconds." << std::endl;
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
            { return PhysicsManager::InterfaceType; }

        String PhysicsManager::GetImplementationTypeName() const
            { return PhysicsManager::ImplementationName; }

        ///////////////////////////////////////////////////////////////////////////////
        // Internal Methods

        btSoftRigidDynamicsWorld* PhysicsManager::_GetPhysicsWorldPointer()
            { return this->BulletDynamicsWorld; }

        const btSoftRigidDynamicsWorld* PhysicsManager::_GetPhysicsWorldPointer() const
            { return this->BulletDynamicsWorld; }

        ///////////////////////////////////////////////////////////////////////////////
        // DefaultPhysicsManagerFactory Methods

        DefaultPhysicsManagerFactory::DefaultPhysicsManagerFactory()
            {  }

        DefaultPhysicsManagerFactory::~DefaultPhysicsManagerFactory()
            {  }

        String DefaultPhysicsManagerFactory::GetManagerImplName() const
            { return PhysicsManager::ImplementationName; }

        ManagerBase::ManagerType DefaultPhysicsManagerFactory::GetManagerType() const
            { return PhysicsManager::InterfaceType; }

        WorldManager* DefaultPhysicsManagerFactory::CreateManager(World* Creator, const NameValuePairList& Params)
        {
            if( Params.empty() ) {
                return new PhysicsManager(Creator);
            }
            ManagerConstructionInfo PhysInfo;
            for( NameValuePairList::const_iterator ParIt = Params.begin() ; ParIt != Params.end() ; ++ParIt )
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
            return new PhysicsManager(Creator,PhysInfo);
        }

        WorldManager* DefaultPhysicsManagerFactory::CreateManager(World* Creator, const XML::Node& XMLNode)
            { return new PhysicsManager(Creator,XMLNode); }

        void DefaultPhysicsManagerFactory::DestroyManager(WorldManager* ToBeDestroyed)
            { delete ToBeDestroyed; }
    }//Physics
}//Mezzanine

#endif
