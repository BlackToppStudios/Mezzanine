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
#ifndef _physicscollisiondispatcher_cpp
#define _physicscollisiondispatcher_cpp

#include "Physics/collisiondispatcher.h.cpp"
#include "Physics/physicsmanager.h"
#include "Physics/collision.h"

namespace Mezzanine
{
    namespace Physics
    {
        ///////////////////////////////////////////////////////////
        // CollisionDispatcher functions

        CollisionDispatcher::CollisionDispatcher(PhysicsManager* Phys, btCollisionConfiguration* CollisionConfig) :
            btCollisionDispatcher(CollisionConfig),
            PhysMan(Phys)
            {  }

        CollisionDispatcher::~CollisionDispatcher()
            {  }

        ///////////////////////////////////////////////////////////////////////////////
        // Implementation based on Algorithm creation

        void* CollisionDispatcher::allocateCollisionAlgorithm(int size)
        {
            void* ToReturn = btCollisionDispatcher::allocateCollisionAlgorithm(size);
            btCollisionAlgorithm* Casted = (btCollisionAlgorithm*)ToReturn;
            AlgoCreationQueue.push_back(Casted);
            return ToReturn;
        }

        void CollisionDispatcher::freeCollisionAlgorithm(void* ptr)
        {
            btCollisionAlgorithm* Casted = (btCollisionAlgorithm*)ptr;
            // first check the queue
            if( !AlgoCreationQueue.empty() ) {
                for( AlgoContainer::iterator QueIt = AlgoCreationQueue.begin() ; QueIt != AlgoCreationQueue.end() ; QueIt++ )
                {
                    if( Casted == (*QueIt) ) {
                        AlgoCreationQueue.erase(QueIt);
                        btCollisionDispatcher::freeCollisionAlgorithm(ptr);
                        return;
                    }
                }
            }
            // now check the already generated collisions
            for( Physics::PhysicsManager::CollisionMapIterator ColIt = this->PhysMan->Collisions.begin() ; ColIt != PhysMan->Collisions.end() ; ++ColIt )
            {
                if( Casted == (*ColIt).second->InternalAlgo ) {
                    delete (*ColIt).second;
                    this->PhysMan->Collisions.erase(ColIt);
                    break;
                }
            }
            btCollisionDispatcher::freeCollisionAlgorithm(ptr);
        }

        AlgoContainer& CollisionDispatcher::GetAlgoCreationQueue()
            { return this->AlgoCreationQueue; }

        ///////////////////////////////////////////////////////////
        // ParallelCollisionDispatcher functions

        ParallelCollisionDispatcher::ParallelCollisionDispatcher(PhysicsManager* Phys, btCollisionConfiguration* CollisionConfig, int GrainSize) :
            btCollisionDispatcherMt(CollisionConfig,GrainSize),
            PhysMan(Phys)
            {  }

        ParallelCollisionDispatcher::~ParallelCollisionDispatcher()
            {  }

        ///////////////////////////////////////////////////////////////////////////////
        // Implementation based on Algorithm creation

        void* ParallelCollisionDispatcher::allocateCollisionAlgorithm(int size)
        {
            void* ToReturn = btCollisionDispatcher::allocateCollisionAlgorithm(size);
            btCollisionAlgorithm* Casted = (btCollisionAlgorithm*)ToReturn;
            btMutexLock(&QueueMutex);
            this->AlgoCreationQueue.push_back(Casted);
            btMutexUnlock(&QueueMutex);
            return ToReturn;
        }
        void ParallelCollisionDispatcher::freeCollisionAlgorithm(void* ptr)
        {
            btCollisionAlgorithm* Casted = (btCollisionAlgorithm*)ptr;
            // first check the queue
            if( !AlgoCreationQueue.empty() ) {
                for( AlgoContainer::iterator QueIt = this->AlgoCreationQueue.begin() ; QueIt != this->AlgoCreationQueue.end() ; QueIt++ )
                {
                    if( Casted == (*QueIt) ) {
                        btMutexLock(&QueueMutex);
                        this->AlgoCreationQueue.erase(QueIt);
                        btMutexUnlock(&QueueMutex);
                        btCollisionDispatcher::freeCollisionAlgorithm(ptr);
                        return;
                    }
                }
            }
            // now check the already generated collisions
            for( Physics::PhysicsManager::CollisionMapIterator ColIt = this->PhysMan->Collisions.begin() ; ColIt != PhysMan->Collisions.end() ; ++ColIt )
            {
                if( Casted == (*ColIt).second->InternalAlgo ) {
                    delete (*ColIt).second;
                    btMutexLock(&QueueMutex);
                    this->PhysMan->Collisions.erase(ColIt);
                    btMutexUnlock(&QueueMutex);
                    break;
                }
            }
            btCollisionDispatcher::freeCollisionAlgorithm(ptr);
        }

        AlgoContainer& ParallelCollisionDispatcher::GetAlgoCreationQueue()
            { return this->AlgoCreationQueue; }
    }//Physics
}//Mezzanine

#endif
