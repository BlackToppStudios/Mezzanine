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
#ifndef _physicscollisiondispatcher_h_cpp
#define _physicscollisiondispatcher_h_cpp

//#define BT_THREADSAFE 1

#include <BulletCollision/CollisionDispatch/btCollisionDispatcherMt.h>

#include <vector>

namespace Mezzanine
{
    namespace Physics
    {
        // internal forward declarations
        class PhysicsManager;

        /// @brief Convenience type for a collection of Collision Algorithms.
        using AlgoContainer = std::vector<btCollisionAlgorithm*>;

        ///////////////////////////////////////////////////////////////////////////////
        /// @internal
        /// @brief Used to provide better reporting of collisions.
        ///////////////////////////////////////
        class CollisionDispatcher : public btCollisionDispatcher
        {
        protected:
            /// @brief A container of all the algorithms that have been created and need processing.
            AlgoContainer AlgoCreationQueue;
            /// @brief A pointer to the physics manager to report the collisions to.
            PhysicsManager* PhysMan;
        public:
            /// @brief Class constructor.
            /// @param Phys A pointer to the physics manager to report the collisions to.
            /// @param CollisionConfig The collision configuration for the world being created.
            CollisionDispatcher(PhysicsManager* Phys, btCollisionConfiguration* CollisionConfig);
            /// @brief Class destructor.
            virtual ~CollisionDispatcher();

            ///////////////////////////////////////////////////////////////////////////////
            // Implementation based on Algorithm creation

            /// @brief Allocates space for a new Collision Algorithm between two objects.
            /// @note This usually means that a collision has started between two objects.
            /// @param size The size needed for the new Collision Algorithm.
            void* allocateCollisionAlgorithm(int size);
            /// @brief Frees up the space belonging to a Collision Algorithm that is no longer needed.
            /// @note This usually means that a collision has ended between two objects.
            /// @param ptr A pointer to Collision Algorithim that is being removed.
            void freeCollisionAlgorithm(void* ptr);
            /// @brief Gets all of the algorithms that have been created and need processing.
            /// @return Returns a reference to the container of algorithms that need processing.
            AlgoContainer& GetAlgoCreationQueue();
        };//CollisionDispatcher

        ///////////////////////////////////////////////////////////////////////////////
        /// @internal
        /// @brief Used to provide better reporting of collisions in a multi-threaded environment.
        ///////////////////////////////////////
        class ParallelCollisionDispatcher : public btCollisionDispatcherMt
        {
        protected:
            /// @brief A container of all the algorithms that have been created and need processing.
            AlgoContainer AlgoCreationQueue;
            /// @brief A mutex to protect access to the creation queue in multi-threaded environments.
            btSpinMutex QueueMutex;
            /// @brief A pointer to the physics manager to report the collisions to.
            PhysicsManager* PhysMan;
        public:
            /// @brief Class constructor.
            /// @param Phys A pointer to the physics manager to report the collisions to.
            /// @param CollisionConfig The collision configuration for the world being created.
            ParallelCollisionDispatcher(PhysicsManager* Phys, btCollisionConfiguration* CollisionConfig, int GrainSize = 40);
            /// @brief Class destructor.
            virtual ~ParallelCollisionDispatcher();

            ///////////////////////////////////////////////////////////////////////////////
            // Implementation based on Algorithm creation

            /// @brief Allocates space for a new Collision Algorithm between two objects.
            /// @note This usually means that a collision has started between two objects.
            /// @param size The size needed for the new Collision Algorithm.
            void* allocateCollisionAlgorithm(int size);
            /// @brief Frees up the space belonging to a Collision Algorithm that is no longer needed.
            /// @note This usually means that a collision has ended between two objects.
            /// @param ptr A pointer to Collision Algorithm that is being removed.
            void freeCollisionAlgorithm(void* ptr);
            /// @brief Gets all of the algorithms that have been created and need processing.
            /// @return Returns a reference to the container of algorithms that need processing.
            AlgoContainer& GetAlgoCreationQueue();
        };//CollisionDispatcher
    }//Physics
}//Mezzanine

#endif
