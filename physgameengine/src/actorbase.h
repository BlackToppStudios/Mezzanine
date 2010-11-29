//© Copyright 2010 BlackTopp Studios Inc.
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

#include "crossplatformexport.h"
#include "node.h"

///////////////////////////////////
// Forward Declares

class btCollisionShape;
class btCollisionObject;
class btTriangleMesh;

namespace Ogre
{
    class AnimationState;
    class Entity;
    class SceneNode;
    class Vector3;
    class ResourceGroupManager;
}

///////////////////////////////////
// Actual code
namespace phys
{
    class ActorContainerBase;
    class World;
    namespace internal
    {
        class PhysMotionState;
        struct MeshInfo;
    }
    ///////////////////////////////////////////////////////////////////////////////
    /// @class ActorBase
    /// @headerfile actorbase.h
    /// @brief This is the base class from which all the actors inherit.
    /// @details The actor classes store and manage all the relevant data regarding
    /// objects inside the World.  They serve as a binder between the physics and
    /// graphics for objects and have functions that allow the manipulation of objects
    /// loaded into the World.  Currently there are 5 actor classes: ActorBase,
    /// ActorRigid, ActorSoft, ActorTerrain, ActorRagDoll. @n
    /// ActorBase is a base class that serves as a template for the other four actor classes. @n
    /// ActorBase should never be created, as it lacks the functionality needed for most objects.
    ///////////////////////////////////////
    class PHYS_LIB ActorBase {
        private:
            friend class Node;
            friend class ActorContainerBase;
            friend class PhysicsManager;
            friend class ResourceManager;

        public:
            /// @enum ActorTypeName
            /// @brief A listing of Actor TypeNames
            /// @details These will be returned by ActorBase::GetType(), and will allow
            /// code using this to determine what type of Actor class they are working with
            /// and use this information to more safely cast to the correct Actor if needed.
            enum ActorTypeName
            {
                Actorbase,
                Actorragdoll,
                Actorrigid,
                Actorsoft,
                Actorterrain
            };

        protected:
            /// @brief A pointer to the World the actor will reside.
            World* GameWorld;

            //abstraction for other libraries
            ///@brief This class encapsulates the functionality of the Ogre::Entity using this
            Ogre::Entity* entity;

            ///@brief This class encapsulates the functionality of the Ogre::SceneNode using this
            Ogre::SceneNode* node;

            ///@brief This class encapsulates the functionality of the Ogre::AnimationState using this
            Ogre::AnimationState* Animation;

            ///@brief This class encapsulates the functionality of the btCollisionShape using this
            btCollisionShape* Shape;

            ///@brief This class encapsulates the functionality of the btCollisionObject using this
            btCollisionObject* CollisionObject;

            ///@brief This class encapsulates the functionality of the PhysMotionState using this
            internal::PhysMotionState* MotionState;

            /// @brief This variable stores the type of actor that this class is.
            ActorTypeName ActorType;

            /// @brief Stores whether or not the current physics shape has been saved in the manager.
            bool ShapeIsSaved;

            /// @brief Creates a trimesh shape from the mesh file.
            /// @details Makes a trimesh to be used as a collision shape in the physics world from a mesh file. @n
            /// This is automaticly called by the CreateShapeFromMesh function in child classes and shouldn't be called manually.
            btTriangleMesh* CreateTrimesh() const;

//////////////////////////////////////////////////////////////////////////////
// Ogre Management Functions
///////////////////////////////////////
            /// @brief Gets the ogre resource manager pointer.
            /// @details This function is needed for the ActorSoft implementation.
            /// @return Returns a pointer to the ogre resource group manager.
            Ogre::ResourceGroupManager* GetOgreResourceManager();

            /// @brief Gets the verticy information of the mesh this soft body is based on.
            /// @details This function will read the mesh provided and gather the verticies inside it for re-use.
            /// @param TheMesh The struct to populate with the information gathered.
            void GetMeshVerticies (internal::MeshInfo &TheMesh);

            /// @brief Gets the indicy information of the mesh this soft body is based on.
            /// @details This function will read the mesh provided and gather the indicies inside it for re-use.
            /// @param TheMesh The struct to populate with the information gathered.
            void GetMeshIndicies (internal::MeshInfo &TheMesh);

            /// @brief Gets the normals information of the mesh this soft body is based on.
            /// @details This function will read the mesh provided and gather the normals of each verticy inside it for re-use.
            /// @param TheMesh The struct to populate with the information gathered.
            void GetMeshNormals (internal::MeshInfo &TheMesh);

            /// @brief Gets the texture coordinates information of the mesh this soft body is based on.
            /// @details This function will read the mesh provided and gather the texture coordinates inside it for re-use.
            /// @param TheMesh The struct to populate with the information gathered.
            void GetMeshTextures (internal::MeshInfo &TheMesh);

            /// @brief Gets other information not related to the verticies from the mesh.
            /// @details This function will get the Render Operation, Material Name, and Entity Name from the entity for re-use.
            /// @param TheMesh The struct to populate with the information gathered.
            void GetOtherMeshInfo (internal::MeshInfo &TheMesh);

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
            Vector3 GetOgreLocation() const;

            /// @brief Sets the orientation of the graphical body.
            /// @details This will take a PhysQuaternion and set the orientation of the actor within the graphical world. @n
            /// This function is called on by the SetOrientation function, and shouldn't be called manually.
            /// @param Rotation The quaternion representing the rotation of the actor.
            void SetOgreOrientation (Quaternion Rotation);

            /// @brief Makes the actor visable.
            /// @details Adds the actor to all the nessessary graphics elements to make it visable on screen. @n
            /// This is automaticly called by the PhysWorlds AddActor function and shouldn't ever need to be called manually.
            virtual void AttachToGraphics();

            /// @brief Makes the actor invisable.
            /// @details This is the inverse of the AttachToGraphics function.  This will effectively remove the object from the graphics world and make it no longer visable. @n
            /// This is automaticly called by the PhysWorlds RemoveActor function and shouldn't ever need to be called manually.
            virtual void DetachFromGraphics();

//////////////////////////////////////////////////////////////////////////////
// Bullet Management
///////////////////////////////////////
            /// @brief Creates a motion state for use with syncronizing physics and graphics.
            /// @details This is a helper function that will create a motionstate for the class.  This is called on by the constructor.
            /// @param Node The node to be syncronized with physics data.
            virtual void CreateMotionState(Ogre::SceneNode* Node);

            /// @brief Creates a motion state for use with syncronizing physics and graphics.
            /// @details This is a helper function that will create a motionstate for the class.  This is called on by the constructor.
            /// @param Node The node to be syncronized with physics data.
            /// @param InitPosition The starting location for the object that will be given this motionstate.
            virtual void CreateMotionState(Ogre::SceneNode* Node, Vector3 InitPosition);

            /// @brief Creates a motion state for use with syncronizing physics and graphics.
            /// @details This is a helper function that will create a motionstate for the class.  This is called on by the constructor.
            /// @param Node The node to be syncronized with physics data.
            /// @param InitPosition The starting location for the object that will be given this motionstate.
            /// @param InitOrientation The starting orientation for the object that will be given this motionstate.
            virtual void CreateMotionState(Ogre::SceneNode* Node, Vector3 InitPosition, Quaternion InitOrientation);

            /// @brief Sets the location of the physics body.
            /// @details This will take a Vector3 and set the location of the actor within the physics world. @n
            /// This function is called on by the SetLocation function, and shouldn't be called manually.
            /// @param Location The Vector3 representing the location.
            virtual void SetBulletLocation (Vector3 Location);

            /// @brief Retrieves the location of the physics body.
            /// @details This function will retrieve the location of the object within the physics world.
            virtual Vector3 GetBulletLocation() const;

            /// @brief Sets the orientation of the physics body.
            /// @details This will take a PhysQuaternion and set the orientation of the actor within the physics world. @n
            /// This function is called on by the SetOrientation function, and shouldn't be called manually.
            /// @param Rotation The quaternion representing the rotation of the actor.
            virtual void SetBulletOrientation (Quaternion Rotation);

        public:
            ///@brief This is a collection of sounds for use with this actor.
            SoundSet* ActorSounds;

            /// @brief Gets the type of actor this class is.
            /// @details This function will get the type of class that you are working with for checking and casting.
            /// @return ActorTypeName The type of actor that this is.
            virtual int GetType();

            /// @brief Gets whether this actors current shape has been saved or not.
            /// @details This function will tell you if it's current physics shape has been saved for later use or not.
            /// @return Returns whether or not the shape of this actor has been saved.
            const bool GetShapeIsSaved();

            /// @brief Sets the basic parameters for more realistic collision behavior.
            /// @details This function will set the Friction and Resititution of the actor, which will enable it to collide
            /// and interact with other actors in a game world more realistically.
            /// @param Friction Should be self explanitory, higher values will resist sliding across surfaces.  This number is the
            /// coefficient of friction.  Range is from 0.0 to 2.0.  Behavior in this regard is determined by the friction of both
            /// colliding bodies.  @n Default: 0.5
            /// @param Restitution The coefficient of restitution determines how much energy is left after a collision with an object.
            /// Range is from 0.0 to 1.0.  Behavior in this regard is determined by the restitution of both colliding bodies.
            /// @n Default: 0.0
            void SetBasicCollisionParams(Real Friction, Real Restitution);

            /// @brief Sets the animation for this object.
            /// @details This function will get the specified animation for this object stored in the mesh file, and will loop the
            /// animation if specified.
            /// @param AnimationName Name of the stored animation to be loaded.
            /// @param Loop Whether or not you want the animation to loop.  For example, you would want an idle animation to loop,
            /// but not a death animation.
            void SetAnimation(String &AnimationName, bool Loop);

            /// @brief Enables the animation if one is set.
            /// @details This function will enable the animation if passed true, making the object animate.  If passed false will
            /// disable the animation.
            /// @param Enable True to enable the animation or false to disable the animation.
            void EnableAnimation(bool Enable);

            /// @brief Tells whether this actor is animated or not.
            /// @details This function will return true if the actor has an animation set and it is enabled.
            /// @return Returns true if an animation is set and enabled.
            bool IsAnimated();

            /// @brief Advances the animation, making it animate.
            /// @details You need to call this every frame while the actor is to be animated, otherwise even with the animation
            /// enabled you will see no change in the animation.
            void AdvanceAnimation(Real Time);

            /// @brief Unloads a loaded animation.
            /// @details This function will remove the existing set animation.
            void RemoveSetAnimation();

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
            ActorBase (String name, String file, String group);

            /// @brief Sets the starting location of the actor.
            /// @details Calling this function after adding it to the World will have no effect. @n
            /// This function will set where the actor will be located in the World when it is first placed inside the world.
            /// @param Location The Vector3 representing the location.
            virtual void SetInitLocation(Vector3 Location);

            /// @brief Sets the starting orientation of the actor.
            /// @details Calling this function after adding it to the World will have no effect. @n
            /// This function will set where the actor is facing in the World when it is first placed inside the world.
            /// @param Orientation The PhysQuaternion representing the Orientation.
            virtual void SetInitOrientation(Quaternion Orientation);

            /// @brief Manually sets the location of the actor.
            /// @details Calling this function prior to adding it to the World will have no effect. @n
            /// In most situations you won't want to use this function, and instead produce movement through physics functions.
            /// @param x Location on the X vector.
            /// @param y Location on the Y vector.
            /// @param z Location on the Z vector.
            virtual void SetLocation(Real x, Real y, Real z);

            /// @brief Manually sets the location of the actor.
            /// @details Calling this function prior to adding it to the World will have no effect. @n
            /// In most situations you won't want to use this function, and instead produce movement through physics functions.
            /// @param Place The Vector3 representing the location.
            virtual void SetLocation(Vector3 Place);

            /// @brief Retrieves the location of the object.
            /// @details This function will retrieve the location of the object within the world.
            virtual Vector3 GetLocation() const;

            /// @brief Retrieves the name of the object.
            /// @details This function will retrieve the name of the object,
            virtual std::string GetName() const = 0;

            /// @brief Sets the orientation of the actor.
            /// @details Sets the orientation of the actor via Quaternion parameters.
            /// @param x Where the X vector is rotated about.
            /// @param y Where the Y vector is rotated about.
            /// @param z Where the Z vector is rotated about.
            /// @param w How much to about the x, y, z.
            virtual void SetOrientation(Real x, Real y, Real z, Real w);

            /// @brief Sets the orientation of the actor.
            /// @details Sets the orientation of the actor via a Quaternion.
            /// @param Rotation The Quaternion representing the Rotation.
            virtual void SetOrientation(Quaternion Rotation);

            /// @brief Sets the scale of the actor.
            /// @details This function will alter the scaling/size of the actor with the given vector3.
            /// @param scale The vector3 by which to apply the scale.  Will scale each axis' accordingly.
            void SetActorScaling(Vector3 scale);

            /// @brief Creates a collision shape from mesh file.
            /// @details This function will read the location of every verticy in the mesh file and use that to
            /// construct a triangle mesh shape and attach it to this objects collision shape.  This shoiuld
            /// be used with only with Dynamic objects.
            /// @param accuracy A short unsigned int, the higher the more accurate, but the more resource intensive. This is Actor dependent
            virtual void CreateShapeFromMeshDynamic(short unsigned int accuracy ) = 0;

///////////////////////////////////////////////////////////////////////////////
// Working with the World
///////////////////////////////////////
            /// @brief Adds the actor to the physics world.
            /// @details Adds the actor to the physics world. @n
            /// This is automatically called by the phys::Actors::AddActor function and Doesn't neet to be called manually.
            /// @param TargetWorld Pointer to the World class.
            virtual void AddObjectToWorld(World* TargetWorld) = 0;

            /// @brief Removes the actor from the physics world.
            /// @details Removes the actor from the physics world. @n
            /// This is automatically called by the phys::Actors::AddActor function and Doesn't neet to be called manually.
            /// @param TargetWorld Pointer to the World class.
            virtual void RemoveObjectFromWorld(World* TargetWorld) = 0;

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

            /// @brief Checks of the actor is static or kinematic.
            /// @details Checks of the actor is static or kinematic, returns true if it is either.
            /// @return Returns true if the actor is static or kinematic.
            bool IsStaticOrKinematic();

            /// @brief Sets the actor to be able to collide with other objects in the world.
            /// @details By default collision response is enabled.  Only call this function if you have disabled collision response.
            void EnableCollisionResponse();

            /// @brief Sets the actor to be unable to collide with other objects in the world.
            /// @details By default collision response is enabled.  Be sure to reactivate collision response if you want your objects to collide again.
            void DisableCollisionResponse();

            /// @brief Checks if the object is active in the simulation.
            /// @return Returns true if the object is active, false if it's deactivated(at rest).
            bool CheckActivation();
    };

} // /phys

std::ostream& operator << (std::ostream& stream, const phys::ActorBase& x);

#endif
