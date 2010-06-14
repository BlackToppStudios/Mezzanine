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
#ifndef _physactorbase_h
#define _physactorbase_h

//#include "vector3.h"
#include "quaternion.h"
#include "world.h"

///////////////////////////////////
// Forward Declares

class btCollisionShape;
class btCollisionObject;
class btTriangleMesh;

namespace Ogre
{
    class Entity;
    class SceneNode;
    class Vector3;
}

///////////////////////////////////
// Actual code
namespace phys
{
    class ActorContainerBase;
    namespace internal
    {
        class PhysMotionState;
    }
    ///////////////////////////////////////////////////////////////////////////////
    /// @class ActorBase
    /// @headerfile actorbase.h
    /// @brief This is the base class from which all the actors inherit.
    /// @details The actor classes store and manage all the relevant data regarding
    /// objects inside the World.  They serve as a binder between the physics and
    /// graphics for objects and have functions that allow the manipulation of objects
    /// loaded into the World.  Currently there are 3 actor classes: ActorBase,
    /// ActorRigid, ActorSoft. @n
    /// ActorBase is a base class that serves as a template for the other three actor classes. @n
    /// ActorBase should never be created, as it lacks the functionality needed for most objects.
    ///////////////////////////////////////
    class ActorBase {
        private:
            friend class World;
            friend class ActorContainerBase;

        protected:

            /// @brief A pointer to the World the actor will reside.
            World* GameWorld;

            //abstraction for other libraries
            ///@brief This class encapsulates the functionality of the Ogre::Entity using this
            Ogre::Entity* entity;

            ///@brief This class encapsulates the functionality of the Ogre::SceneNode using this
            Ogre::SceneNode* node;

            ///@brief This class encapsulates the functionality of the btCollisionShape using this
            btCollisionShape* Shape;

            ///@brief This class encapsulates the functionality of the btCollisionObject using this
            btCollisionObject* CollisionObject;

            ///@brief This class encapsulates the functionality of the PhysMotionState using this
            internal::PhysMotionState* MotionState;

            /// @brief Adds the actor to the physics world.
            /// @details Adds the actor to the physics world. @n
            /// This is automatically called by the phys::Worlds::AddActor function and shouldn't be called manually.
            /// @param TargetWorld Pointer to the World class.
            /// @param btWorld Pointer to the physics world.
            virtual void AddObjectToWorld(World* TargetWorld, btSoftRigidDynamicsWorld* btWorld) = 0;

            /// @brief Removes the actor from the physics world.
            /// @details Removes the actor from the physics world. @n
            /// This is automatically called by the phys::World::RemoveActor function and shouldn't be called manually.
            /// @param TargetWorld Pointer to the World class.
            /// @param btWorld Pointer to the physics world.
            virtual void RemoveObjectFromWorld(World* TargetWorld, btSoftRigidDynamicsWorld* btWorld) = 0;

            /// @brief Creates a trimesh shape from the mesh file.
            /// @details Makes a trimesh to be used as a collision shape in the physics world from a mesh file. @n
            /// This is automaticly called by the CreateShapeFromMesh function in child classes and shouldn't be called manually.
            btTriangleMesh* CreateTrimesh();

//////////////////////////////////////////////////////////////////////////////
// Ogre Management Functions
///////////////////////////////////////
            /// @brief Creates an entity for the mesh file to be placed on a scene node.
            /// @details Creates an entity in the scene manager from the mesh file provided to be attached to a node in the graphical world. @n
            /// This function is called on by the Constructor, and shouldn't be called manually.
            /// @param name Name of the actor.
            /// @param file File name of the graphical mesh to be used.
            /// @param group Resource group where the graphical mesh can be found.
            void CreateEntity(String name, String file, String group);

            /// @brief Creates a node for the entity in the graphical world.
            /// @details Creates a node in the scene manager to attach the actor's entity to within the graphical world. @n
            /// This function is called on by the Constructor, and shouldn't be called manually.

            void CreateSceneNode();
            /// @brief Sets the location of the graphical body.
            /// @details This will take a Vector3 and set the location of the actor within the graphical world. @n
            /// This function is called on by the SetLocation function, and shouldn't be called manually.
            /// @param Place The Vector3 representing the location.
            void SetOgreLocation(Vector3 Place);

            /// @brief Retrieves the location of the graphical body.
            /// @details This function will retrieve the location of the object within the graphical world. This should always match the physics world.
            /// @return This returns a phys::Vector3 with the location of the graphics.
            Vector3 GetOgreLocation();

            /// @brief Sets the orientation of the graphical body.
            /// @details This will take a PhysQuaternion and set the orientation of the actor within the graphical world. @n
            /// This function is called on by the SetOrientation function, and shouldn't be called manually.
            /// @param Rotation The quaternion representing the rotation of the actor.
            void SetOgreOrientation (Quaternion Rotation);

            /// @brief Makes the actor visable.
            /// @details Adds the actor to all the nessessary graphics elements to make it visable on screen. @n
            /// This is automaticly called by the PhysWorlds AddActor function and shouldn't ever need to be called manually.
            void AttachToGraphics();

            /// @brief Makes the actor invisable.
            /// @details This is the inverse of the AttachToGraphics function.  This will effectively remove the object from the graphics world and make it no longer visable. @n
            /// This is automaticly called by the PhysWorlds RemoveActor function and shouldn't ever need to be called manually.
            void DetachFromGraphics();

//////////////////////////////////////////////////////////////////////////////
// Bullet Management
///////////////////////////////////////
            /// @brief Sets the location of the physics body.
            /// @details This will take a Vector3 and set the location of the actor within the physics world. @n
            /// This function is called on by the SetLocation function, and shouldn't be called manually.
            /// @param Location The Vector3 representing the location.
            virtual void SetBulletLocation (Vector3 Location);

            /// @brief Retrieves the location of the physics body.
            /// @details This function will retrieve the location of the object within the physics world.
            virtual Vector3 GetBulletLocation();

            /// @brief Sets the orientation of the physics body.
            /// @details This will take a PhysQuaternion and set the orientation of the actor within the physics world. @n
            /// This function is called on by the SetOrientation function, and shouldn't be called manually.
            /// @param Rotation The quaternion representing the rotation of the actor.
            virtual void SetBulletOrientation (Quaternion Rotation);

        public:
///////////////////////////////////////////////////////////////////////////////
// Creation, Destruction and Initialization
///////////////////////////////////////

            /// @brief Destructor.
            /// @details The class destructor.
            virtual ~ActorBase ();

            /// @brief Descriptive constructor.
            /// @details This constructor contains the basic information needed to make an actor.
            /// @param name The name of the actor.
            /// @param file The 3d mesh file that contains the 3d model the actor will use.
            /// @param group The resource group where the 3d mesh and other related files can be found.
            /// @param _World Pointer to the World this object will be added to.
            ActorBase (String name, String file, String group, World* _World);

            /// @brief Sets the starting location of the actor.
            /// @details Calling this function after adding it to the World will have no effect. @n
            /// This function will set where the actor will be located in the World when it is first placed inside the world.
            /// @param Location The Vector3 representing the location.
            void SetInitLocation(Vector3 Location);

            /// @brief Sets the starting orientation of the actor.
            /// @details Calling this function after adding it to the World will have no effect. @n
            /// This function will set where the actor is facing in the World when it is first placed inside the world.
            /// @param Orientation The PhysQuaternion representing the Orientation.
            void SetInitOrientation(Quaternion Orientation);


            /// @brief Manually sets the location of the actor.
            /// @details Calling this function prior to adding it to the World will have no effect. @n
            /// In most situations you won't want to use this function, and instead produce movement through physics functions.
            /// @param x Location on the X vector.
            /// @param y Location on the Y vector.
            /// @param z Location on the Z vector.
            void SetLocation(Real x, Real y, Real z);

            /// @brief Manually sets the location of the actor.
            /// @details Calling this function prior to adding it to the World will have no effect. @n
            /// In most situations you won't want to use this function, and instead produce movement through physics functions.
            /// @param Place The Vector3 representing the location.
            void SetLocation(Vector3 Place);

            /// @brief Retrieves the location of the object.
            /// @details This function will retrieve the location of the object within the world.
            Vector3 GetLocation();

            /// @brief Retrieves the name of the object.
            /// @details This function will retrieve the name of the object,
            std::string GetName ();

            /// @brief Sets the orientation of the actor.
            /// @details Sets the orientation of the actor via Quaternion parameters.
            /// @param x Where the X vector is rotated about.
            /// @param y Where the Y vector is rotated about.
            /// @param z Where the Z vector is rotated about.
            /// @param w How much to about the x, y, z.
            void SetOrientation(Real x, Real y, Real z, Real w);

            /// @brief Sets the orientation of the actor.
            /// @details Sets the orientation of the actor via a Quaternion.
            /// @param Rotation The Quaternion representing the Rotation.
            void SetOrientation(Quaternion Rotation);

            /// @brief Creates a collision shape from mesh file.
            /// @details This function will read the location of every verticy in the mesh file and use that to
            /// construct a triangle mesh shape and attach it to this objects collision shape.  This shoiuld
            /// be used with only with Dynamic objects.
            /// @param accuracy A short unsigned int, the higher the more accurate, but the more resource intensive. This is Actor dependent
            virtual void CreateShapeFromMeshDynamic(short unsigned int accuracy ) = 0;

///////////////////////////////////////////////////////////////////////////////
// Public Collision flag functions
///////////////////////////////////////
            /// @brief Sets the state of the object to Kinematic.
            /// @details This function will set the object to a Kinematic Object. @n
            /// Kinematic Objects are like Static Objects but are also able to be moved directly by character controllers.
            void SetKinematic();

            /// @brief Sets the state of the object to Static.
            /// @details This function will set the object to a Static Object. @n
            /// Static Objects don't move or have any force applied to them, but are cabable of exerting force on other objects.
            void SetStatic();
    };

} // /phys
#endif
