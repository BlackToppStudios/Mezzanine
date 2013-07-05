//© Copyright 2010 - 2013 BlackTopp Studios Inc.
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

#include "linegroup.h"
#include "physicsmanager.h"
#include "entresol.h"
#include "vector3.h"
#include "actormanager.h"
#include "actorphysicssettings.h"
#include "vector3wactor.h"
#include "areaeffect.h"
#include "eventmanager.h"
#include "eventcollision.h"
#include "worldtrigger.h"
#include "worldobject.h"
#include "Physics/collision.h"
#include "scenemanager.h"
#include "Graphics/graphicsmanager.h"
#include "stringtool.h"
#include "crossplatform.h"

#include "Physics/collisiondispatcher.h.cpp"

#include <queue>
#include <algorithm>

#include <Ogre.h>

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
                /// @brief A pointer to Mezzanine::Entresol that this Debug Drawer works with
                Entresol* TheEntresol;

                /// @internal
                /// @brief How many wireframes do you want to keep around on the screen.
                Whole WireFrameCount;

                /// @internal
                /// @brief This queue stores The listing of of the wireframes still to be rendered.
                /// @details This stores an amount of wireframes up to the WireFrameCount. When this class is created or a
                /// new frame rendered a new Line group is a added to this queue.
                std::queue<Mezzanine::LineGroup*> WireFrames;

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
                /// @details This creates a basic Debug Drawer which works with the Mezzanine::Entresol that was passed. With a new
                InternalDebugDrawer(Mezzanine::Entresol* ParentEntresol_, Whole WireFrameCount_ = 2);

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
                /// Have redirected all input from this function to the Entresol::Log function.
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

        InternalDebugDrawer::InternalDebugDrawer( Mezzanine::Entresol* ParentEntresol_, Whole WireFrameCount_ )
        {
            this->DebugDrawing = 0;
            this->TheEntresol = ParentEntresol_;

            this->WireFrameCount = WireFrameCount_;

            //Mezzanine::LineGroup* temp = new Mezzanine::LineGroup();
            this->WireFrames.push(new Mezzanine::LineGroup());
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
            Mezzanine::LineGroup *myLine = this->WireFrames.back();

            //Convert btVectors to Vector3s
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
            this->TheEntresol->Log(temp);
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
            this->WireFrames.push(new Mezzanine::LineGroup());
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
        {
            if( this->TargetManager->SimulationIsPaused() )
                return;

            this->TargetManager->ThreadResources = &CurrentThreadStorage;
            Real FloatTime = this->TargetManager->TheEntresol->GetFrameTimeMilliseconds() * 0.001; //Convert from MilliSeconds to Seconds
            //Real IdealStep = static_cast<Real>( this->TargetManager->TheEntresol->GetTargetFrameTimeMilliseconds() ) * 0.001;
            //IdealStep /= this->TargetManager->SubstepModifier;
            //IdealStep = ( IdealStep < 1.0/240.0 ? 1.0/240.0 : IdealStep );
            int MaxSteps = ( FloatTime < this->TargetManager->StepSize ) ? 1 : int( FloatTime / this->TargetManager->StepSize ) + 1;
            this->TargetManager->BulletDynamicsWorld->stepSimulation( FloatTime, MaxSteps, this->TargetManager->StepSize );
            this->TargetManager->ThreadResources = NULL;
        }

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
        {
            // Do nothing
        }

        Whole SimulationMonopolyWorkUnit::UsingThreadCount()
        {
            return this->TargetManager->ThreadCount;
        }

        void SimulationMonopolyWorkUnit::DoWork(Threading::DefaultThreadSpecificStorage::Type& CurrentThreadStorage)
        {
            if( this->TargetManager->SimulationIsPaused() )
                return;

            this->TargetManager->ThreadResources = &CurrentThreadStorage;
            Real FloatTime = this->TargetManager->TheEntresol->GetFrameTimeMilliseconds() * 0.001; //Convert from MilliSeconds to Seconds
            //Real IdealStep = static_cast<Real>( this->TargetManager->TheEntresol->GetTargetFrameTimeMilliseconds() ) * 0.001;
            //IdealStep /= this->TargetManager->SubstepModifier;
            //IdealStep = ( IdealStep < 1.0/240.0 ? 1.0/240.0 : IdealStep );
            int MaxSteps = ( FloatTime < this->TargetManager->StepSize ) ? 1 : int( FloatTime / this->TargetManager->StepSize ) + 1;
            this->TargetManager->BulletDynamicsWorld->stepSimulation( FloatTime, MaxSteps, this->TargetManager->StepSize );
            this->TargetManager->ThreadResources = NULL;
        }

        ///////////////////////////////////////////////////////////
        // AreaEffectUpdateWorkUnit functions

        AreaEffectUpdateWorkUnit::AreaEffectUpdateWorkUnit(const AreaEffectUpdateWorkUnit& Other)
            {  }

        AreaEffectUpdateWorkUnit& AreaEffectUpdateWorkUnit::operator=(const AreaEffectUpdateWorkUnit& Other)
            { return *this; }

        AreaEffectUpdateWorkUnit::AreaEffectUpdateWorkUnit(PhysicsManager* Target) :
            TargetManager(Target) {  }

        AreaEffectUpdateWorkUnit::~AreaEffectUpdateWorkUnit()
            {  }

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        void AreaEffectUpdateWorkUnit::DoWork(Threading::DefaultThreadSpecificStorage::Type& CurrentThreadStorage)
        {
            // No real logging necessary
            this->TargetManager->ProcessAllEffects();
        }

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
                Drawer->PrepareForRendering();
                this->TargetManager->BulletDynamicsWorld->debugDrawWorld();
            }
        }

        ///////////////////////////////////////////////////////////
        // Physicsmanager functions

        template<> PhysicsManager* Singleton<PhysicsManager>::SingletonPtr = NULL;

        PhysicsManager::PhysicsManager() :
            SimulationPaused(false),
            SubstepModifier(1),
            ThreadCount(0),
            StepSize(1.0/60.0),

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
            AreaEffectUpdateWork(NULL),
            WorldTriggerUpdateWork(NULL),
            DebugDrawWork(NULL),
            ThreadResources(NULL)
        {
            ManagerConstructionInfo Info;
            Info.PhysicsFlags = (ManagerConstructionInfo::PCF_SoftRigidWorld | ManagerConstructionInfo::PCF_LimitlessWorld);
            this->Construct(Info);
        }

        PhysicsManager::PhysicsManager(const ManagerConstructionInfo& Info) :
            SimulationPaused(false),
            SubstepModifier(1),
            ThreadCount(0),
            StepSize(1.0/60.0),

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
            AreaEffectUpdateWork(NULL),
            WorldTriggerUpdateWork(NULL),
            DebugDrawWork(NULL),
            ThreadResources(NULL)
        {
            this->Construct(Info);
        }

        PhysicsManager::PhysicsManager(XML::Node& XMLNode) :
            SimulationPaused(false),
            SubstepModifier(1),
            ThreadCount(0),
            StepSize(1.0/60.0),

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
            AreaEffectUpdateWork(NULL),
            WorldTriggerUpdateWork(NULL),
            DebugDrawWork(NULL),
            ThreadResources(NULL)
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
                    CurrAttrib = WorldSettings.GetAttribute("MaxObjects");
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
                if(!CurrAttrib.Empty())
                {
                    SetSimulationSubstepModifier(CurrAttrib.AsWhole());
                }
            }

            XML::Node DebugRender = XMLNode.GetChild("DebugRendering");
            if(!DebugRender.Empty())
            {
                int RenderMode = 0;
                Whole WireCount = 2;
                CurrAttrib = WorldSettings.GetAttribute("RenderingMode");
                if(!CurrAttrib.Empty())
                    RenderMode = CurrAttrib.AsInt();
                CurrAttrib = WorldSettings.GetAttribute("WireCount");
                if(!CurrAttrib.Empty())
                    WireCount = CurrAttrib.AsWhole();

                if(0 != RenderMode)
                {
                    SetDebugPhysicsRendering(RenderMode);
                    SetDebugPhysicsWireCount(WireCount);
                }
            }
        }

        PhysicsManager::~PhysicsManager()
        {
            btCollisionObjectArray ObjectArray( BulletDynamicsWorld->getCollisionObjectArray() );
            for( Integer X = 0 ; X < BulletDynamicsWorld->getNumCollisionObjects() ; ++X )
            {
                WorldObject* WO = static_cast<WorldObject*>( ObjectArray[X]->getUserPointer() );
                WO->RemoveFromWorld();
            }

            DestroyAllConstraints();
            DestroyAllAreaEffects();
            DestroyAllWorldTriggers();
            //Destroy the physical world that we loved and cherished
            delete BulletDynamicsWorld;
            delete BulletDispatcher;
            delete BulletCollisionConfiguration;
            delete BulletSolver;
            delete BulletBroadphase;
            delete GhostCallback;
            if(BulletDrawer) delete BulletDrawer;
            if(BulletSolverThreads) delete BulletSolverThreads;
            if(BulletDispatcherThreads) delete BulletDispatcherThreads;
        }

        void PhysicsManager::Construct(const ManagerConstructionInfo& Info)
        {
            this->Priority = 20;
            this->ThreadCount = ( Info.PhysicsFlags & ManagerConstructionInfo::PCF_Multithreaded ? crossplatform::GetCPUCount() : 0 );

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
                this->BulletSolver = new btParallelConstraintSolver(this->BulletSolverThreads);
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
            this->AreaEffectUpdateWork = new AreaEffectUpdateWorkUnit(this);
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
                    (*AE)->_Update();
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
                WorldObject* ObjectA = NULL;
                WorldObject* ObjectB = NULL;
                /// @todo This is an absurd round-about way to get the data we need,
                /// and bullet will probably have to be extended to change this so it's actually good.
                btBroadphasePairArray& PairArray = BulletBroadphase->getOverlappingPairCache()->getOverlappingPairArray();
                for( Integer X = 0 ; X < PairArray.size() ; ++X )
                {
                    if( NewAlgo == PairArray[X].m_algorithm )
                    {
                        btCollisionObject* COA = (btCollisionObject*)PairArray[X].m_pProxy0->m_clientObject;
                        ObjectA = (WorldObject*)COA->getUserPointer();
                        btCollisionObject* COB = (btCollisionObject*)PairArray[X].m_pProxy1->m_clientObject;
                        ObjectB = (WorldObject*)COB->getUserPointer();
                        break;
                    }
                }

                if( (ObjectA && ObjectA->GetPhysicsSettings()->GetCollisionResponse()) &&
                    (ObjectB && ObjectB->GetPhysicsSettings()->GetCollisionResponse()) )
                {
                    // Creat the collision
                    ObjectPair NewPair(ObjectA,ObjectB);
                    PhysicsManager::CollisionIterator ColIt = Collisions.find(NewPair);
                    if(ColIt == Collisions.end())
                    {
                        Physics::Collision* NewCol = new Physics::Collision(ObjectA,ObjectB,NewAlgo);
                        //NewCol->GetActorA()->_NotifyCollisionState(NewCol,Physics::Collision::Col_Begin);
                        //NewCol->GetActorB()->_NotifyCollisionState(NewCol,Physics::Collision::Col_Begin);
                        Collisions.insert(std::pair<ObjectPair,Physics::Collision*>(NewPair,NewCol));
                    }
                }
                AlgoQueue->pop_front();
                if(AlgoQueue->size() > 0) NewAlgo = AlgoQueue->front();
                else NewAlgo = NULL;
            }//*/
        }

        void PhysicsManager::InternalTickCallback(btDynamicsWorld* world, btScalar timeStep)
        {
            PhysicsManager::GetSingletonPtr()->ProcessAllCollisions();
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Simulation Management

        void PhysicsManager::PauseSimulation(bool Pause)
        {
            this->SimulationPaused = Pause;
        }

        bool PhysicsManager::SimulationIsPaused()
        {
            return this->SimulationPaused;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Gravity Management

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
            if( Mezzanine::WSO_ActorRigid==Actor->GetType() )
            {
                btRigidBody* Rigid = static_cast < btRigidBody* >(Actor->_GetBasePhysicsObject());
                Rigid->setGravity(igrav.GetBulletVector3());
            }
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Constraint Management

        void PhysicsManager::AddConstraint(Physics::Constraint* Con, bool DisableCollisions)
        {
            this->BulletDynamicsWorld->addConstraint(Con->GetConstraintBase(), DisableCollisions);
            this->Constraints.push_back(Con);
        }

        Physics::Constraint* PhysicsManager::GetConstraint(const Whole& Index)
        {
            return this->Constraints[Index];
        }

        Whole PhysicsManager::GetNumConstraints()
        {
            return this->Constraints.size();
        }

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
        // AreaEffect Management

        void PhysicsManager::AddAreaEffect(AreaEffect* AE)
        {
            AE->AddToWorld();
            this->AreaEffects.push_back(AE);
        }

        AreaEffect* PhysicsManager::GetAreaEffect(const String& Name)
        {
            for( ConstAreaEffectIterator c = this->AreaEffects.begin() ; c != this->AreaEffects.end() ; c++ )
            {
                if( Name == (*c)->GetName() )
                {
                    return *c;
                }
            }
            return NULL;
        }

        AreaEffect* PhysicsManager::GetAreaEffect(const Whole& Index)
        {
            return this->AreaEffects[Index];
        }

        Whole PhysicsManager::GetNumAreaEffects()
        {
            return this->AreaEffects.size();
        }

        void PhysicsManager::RemoveAreaEffect(AreaEffect* AE)
        {
            AE->RemoveFromWorld();
            for( AreaEffectIterator c = this->AreaEffects.begin() ; c != this->AreaEffects.end() ; c++ )
            {
                if( AE == *c )
                {
                    this->AreaEffects.erase(c);
                    return;
                }
            }
        }

        void PhysicsManager::DestroyAllAreaEffects()
        {
            for( AreaEffectIterator AE = this->AreaEffects.begin() ; AE != this->AreaEffects.end() ; AE++ )
            {
                this->BulletDynamicsWorld->removeCollisionObject((*AE)->_GetBulletObject());
                delete (*AE);
            }
            this->AreaEffects.clear();
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Trigger Management

        void PhysicsManager::AddWorldTrigger(WorldTrigger* Trig)
        {
            this->Triggers.push_back(Trig);
        }

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
        {
            return this->Triggers.at(Index);
        }

        Whole PhysicsManager::GetNumWorldTriggers()
        {
            return this->Triggers.size();
        }

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

        Physics::Collision* PhysicsManager::GetCollision(ObjectPair* Pair)
        {
            ConstCollisionIterator ColIt = this->Collisions.find(*Pair);
            if(ColIt != this->Collisions.end()) return (*ColIt).second;
            else return NULL;
        }

        Whole PhysicsManager::GetNumCollisions()
        {
            return this->Collisions.size();
        }

        void PhysicsManager::RemoveCollision(Physics::Collision* Col)
        {
            //((CollisionDispatcher*)BulletDispatcher)->releaseManifoldManual(Col->Manifold);
            btBroadphasePair* btPair = this->BulletBroadphase->getOverlappingPairCache()->findPair(
                Col->ObjectA->_GetBasePhysicsObject()->getBroadphaseHandle(),
                Col->ObjectB->_GetBasePhysicsObject()->getBroadphaseHandle());
            this->BulletBroadphase->getOverlappingPairCache()->removeOverlappingPair(
                Col->ObjectA->_GetBasePhysicsObject()->getBroadphaseHandle(),
                Col->ObjectB->_GetBasePhysicsObject()->getBroadphaseHandle(),
                this->BulletDispatcher);// */
            this->BulletBroadphase->getOverlappingPairCache()->cleanOverlappingPair(*btPair,this->BulletDispatcher);
            delete btPair;
        }

        void PhysicsManager::RemoveCollisionsContainingObject(WorldObject* Object)
        {
            if(!Object->IsInWorld())
                return;

            this->BulletBroadphase->getOverlappingPairCache()->cleanProxyFromPairs(Object->_GetBasePhysicsObject()->getBroadphaseHandle(),this->BulletDispatcher);

            CollisionIterator ColIt = this->Collisions.begin();
            while( ColIt != this->Collisions.end() )
            {
                Physics::Collision* ToBeDestroyed = (*ColIt).second;
                if( Object == (*ColIt).second->ObjectA || Object == (*ColIt).second->ObjectB )
                {
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
        {
            return this->Collisions.begin();
        }

        PhysicsManager::CollisionIterator PhysicsManager::EndCollision()
        {
            return this->Collisions.end();
        }

        PhysicsManager::ConstCollisionIterator PhysicsManager::BeginCollision() const
        {
            return this->Collisions.begin();
        }

        PhysicsManager::ConstCollisionIterator PhysicsManager::EndCollision() const
        {
            return this->Collisions.end();
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Debug Management

        void PhysicsManager::SetDebugPhysicsRendering(int ToBeEnabled)
        {
            if(!BulletDrawer)
            {
                this->BulletDrawer = new debug::InternalDebugDrawer(this->TheEntresol);
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
                this->BulletDrawer = new debug::InternalDebugDrawer(this->TheEntresol);
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

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        void PhysicsManager::MainLoopInitialize()
        {
            // Configure our area effects so they have an updated list and apply their effects immediately.
            this->BulletDynamicsWorld->updateAabbs();
            this->BulletBroadphase->calculateOverlappingPairs(this->BulletDispatcher);
            this->BulletDispatcher->dispatchAllCollisionPairs(this->BulletDynamicsWorld->getPairCache(),this->BulletDynamicsWorld->getDispatchInfo(),this->BulletDispatcher);
            for( std::vector<AreaEffect*>::iterator AE = AreaEffects.begin() ; AE != AreaEffects.end() ; AE++ )
            {
                //this->BulletDispatcher->dispatchAllCollisionPairs(InternalGhost->getOverlappingPairCache(),this->BulletDynamicsWorld->getDispatchInfo(),this->BulletDispatcher);

                (*AE)->_Update();
                (*AE)->ApplyEffect();
            }

            // Set our ideal simulation step size
            //Real InvSubStepMod = 1.0 / this->SubstepModifier;
            //Real TargetTimeSeconds = static_cast<Real>( this->TheEntresol->GetTargetFrameTimeMicroseconds() ) * 0.000001;
            //this->StepSize = std::max( TargetTimeSeconds * InvSubStepMod, static_cast<Real>( 1.0 / 120.0 ) );
            //this->StepSize = TargetTimeSeconds * InvSubStepMod;
            this->StepSize = (static_cast<Real>( this->TheEntresol->GetTargetFrameTimeMicroseconds() ) * 0.000001) / this->SubstepModifier;
        }

        void PhysicsManager::ResetPhysicsWorld(ManagerConstructionInfo* Info)
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
            if(BulletSolverThreads) delete BulletSolverThreads;
            if(BulletDispatcherThreads) delete BulletDispatcherThreads;

            if(Info) this->Construct(*Info);
            else this->Construct(WorldConstructionInfo);
            if(DebugOn)
            {
                this->BulletDrawer = new debug::InternalDebugDrawer(this->TheEntresol);
                this->BulletDynamicsWorld->setDebugDrawer(this->BulletDrawer);
            }
        }

        void PhysicsManager::ClearPhysicsMetaData()
        {
            // Clean the broadphase of AABB data
            btOverlappingPairCache* Pairs = BulletBroadphase->getOverlappingPairCache();
            int NumPairs = Pairs->getNumOverlappingPairs();
            btBroadphasePairArray PairArray = Pairs->getOverlappingPairArray();
            for( Integer X = 0 ; X < NumPairs ; X++ )
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

        Threading::DefaultWorkUnit* PhysicsManager::GetSimulationWork()
        {
            return this->SimulationWork;
        }

        AreaEffectUpdateWorkUnit* PhysicsManager::GetAreaEffectUpdateWork()
        {
            return this->AreaEffectUpdateWork;
        }

        WorldTriggerUpdateWorkUnit* PhysicsManager::GetWorldTriggerUpdateWork()
        {
            return this->WorldTriggerUpdateWork;
        }

        DebugDrawWorkUnit* PhysicsManager::GetDebugDrawWork()
        {
            return this->DebugDrawWork;
        }

        btSoftRigidDynamicsWorld* PhysicsManager::GetPhysicsWorldPointer()
        {
            return this->BulletDynamicsWorld;
        }

        const btSoftRigidDynamicsWorld* PhysicsManager::GetPhysicsWorldPointer() const
        {
            return this->BulletDynamicsWorld;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Inherited from Managerbase

        void PhysicsManager::Initialize()
        {
            // Simulation work configuration
            if( this->WorldConstructionInfo.PhysicsFlags & ManagerConstructionInfo::PCF_Multithreaded ) {
                this->TheEntresol->GetScheduler().AddWorkUnitMonopoly( static_cast<Threading::MonopolyWorkUnit*>( this->SimulationWork ) );
            }else{
                this->TheEntresol->GetScheduler().AddWorkUnit( this->SimulationWork );
            }
            Graphics::GraphicsManager* GraphicsMan = this->TheEntresol->GetGraphicsManager();
            if( GraphicsMan )
                this->SimulationWork->AddDependency( GraphicsMan->GetRenderWorkUnit() );

            // Debug Draw work configuration
            this->TheEntresol->GetScheduler().AddWorkUnit( this->DebugDrawWork );
            this->DebugDrawWork->AddDependency( this->SimulationWork );

            // World Trigger Update work configuration
            this->TheEntresol->GetScheduler().AddWorkUnit( this->WorldTriggerUpdateWork );
            this->WorldTriggerUpdateWork->AddDependency( this->SimulationWork );

            // Area Effect Update work configuration
            this->TheEntresol->GetScheduler().AddWorkUnit( this->AreaEffectUpdateWork );
            this->AreaEffectUpdateWork->AddDependency( this->DebugDrawWork );

            Initialized = true;
        }

        void PhysicsManager::DoMainLoopItems()
        {

        }

        ManagerBase::ManagerType PhysicsManager::GetInterfaceType() const
            { return ManagerBase::PhysicsManager; }

        String PhysicsManager::GetImplementationTypeName() const
            { return "DefaultPhysicsManager"; }

        ///////////////////////////////////////////////////////////////////////////////
        // DefaultPhysicsManagerFactory Methods

        DefaultPhysicsManagerFactory::DefaultPhysicsManagerFactory()
        {
        }

        DefaultPhysicsManagerFactory::~DefaultPhysicsManagerFactory()
        {
        }

        String DefaultPhysicsManagerFactory::GetManagerTypeName() const
        {
            return "DefaultPhysicsManager";
        }

        ManagerBase* DefaultPhysicsManagerFactory::CreateManager(NameValuePairList& Params)
        {
            if(PhysicsManager::SingletonValid())
            {
                /// @todo Add something to log a warning that the manager exists and was requested to be constructed when we have a logging manager set up.
                return PhysicsManager::GetSingletonPtr();
            }else{
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
                    }
                    return new PhysicsManager(PhysInfo);
                }
            }
        }

        ManagerBase* DefaultPhysicsManagerFactory::CreateManager(XML::Node& XMLNode)
        {
            if(PhysicsManager::SingletonValid())
            {
                /// @todo Add something to log a warning that the manager exists and was requested to be constructed when we have a logging manager set up.
                return PhysicsManager::GetSingletonPtr();
            }else return new PhysicsManager(XMLNode);
        }

        void DefaultPhysicsManagerFactory::DestroyManager(ManagerBase* ToBeDestroyed)
        {
            delete ToBeDestroyed;
        }
    }//Physics
}//Mezzanine

#endif
