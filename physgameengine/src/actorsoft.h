//© Copyright 2010 Joseph Toppi and John Blackwood
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
#ifndef _physactorsoft_h
#define _physactorsoft_h

//#include "vector3.h"
//#include "quaternion.h"
//#include "world.h"

///////////////////////////////////
// Forward Declares
//
//class btMotionState;
//class btQuaternion;
//class btVector3;
//class btRigidBody;
class btSoftBody;
//class btTransform;
//class btDiscreteDynamicsWorld;
//class btDefaultMotionState;
//class btCollisionShape;
class btSoftBodyWorldInfo;
//class btCollisionObject;
//class btSoftRigidDynamicsWorld;
//class btTriangleMesh;
typedef float btScalar;
//
//namespace Ogre
//{
//    class Entity;
//    class SceneManager;
//    class SceneNode;
//}

#include "actorbase.h"

///////////////////////////////////
// Actual code
namespace phys
{
    ///////////////////////////////////////////////////////////////////////////////
    /// @class ActorSoft
    /// @headerfile actorbase.h
    /// @brief This is the actor class for Soft Objects.
    /// @details This class should be used to make any soft object that, like ActorRigid,
    /// can be moved or manipulated as a result of force.  Examples of soft objects are: Paper,
    /// Rope, and Cloth.  Semi Rigid bodies that are still somewhat deformable, like Jello,
    /// should be made as a soft object.
    /// @warning Currently this is not fully implemented and will likely cause errors if used.
    ///////////////////////////////////////
    class ActorSoft: public ActorBase {
        protected:
            ///@brief Used to simulate the functionality of a btSoftBody for use with the physics subsystem.
            btSoftBody* physsoftbody;
            /// @brief Creates a soft object for the actor.
            /// @details Creates a soft object to be placed in the physics world later. @n
            /// This is automaticly called by the Constructor and shouldn't be called manually.
            /// @param softworldinfo Currently Unused
            /// @param nodecount Currently Unused
            /// @param nodearray Currently Unused
            /// @param massarray Currently Unused
            void CreateSoftObject (btSoftBodyWorldInfo* softworldinfo, int nodecount, btVector3* nodearray, btScalar* massarray);
            /// @brief Adds the actor to the physics world.
            /// @details Adds the actor to the physics world. @n
            /// This is automaticly called by the PhysWorlds AddActor function and shouldn't be called manually.
            /// @param TargetWorld Pointer to the World class.
            /// @param btWorld Pointer to the physics world.
            void AddObjectToWorld (World *TargetWorld, btSoftRigidDynamicsWorld* btWorld);

        public:
            ActorSoft();
            /// @brief Destructor.
            /// @details The class destructor.
            virtual ~ActorSoft();
            /// @brief Creates a collision shape from mesh file.
            /// @details This function will read the location of every verticy in the mesh file and use that to
            /// construct a triangle mesh shape and attach it to this objects collision shape.
            void CreateShapeFromMesh();
    };
}
#endif
