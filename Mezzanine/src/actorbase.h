//© Copyright 2010 - 2011 BlackTopp Studios Inc.
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
#ifndef _actorbase_h
#define _actorbase_h

#include "crossplatformexport.h"
#include "worldnode.h"
#include "collision.h"

/// @file
/// @brief The base class for all Actors is defined here


///////////////////////////////////
// Forward Declares
class btRigidBody;
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
namespace Mezzanine
{
    class ActorContainerBase;
    class World;
    class ActorGraphicsSettings;
    class ActorBasePhysicsSettings;

    namespace Audio
    {
        class SoundSet;
    }

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
    /// ActorRigid, ActorSoft, ActorTerrain, ActorCharacter. @n
    /// ActorBase is a base class that serves as a template for the other four actor classes. @n
    /// ActorBase should never be created, as it lacks the functionality needed for most objects.
    ///////////////////////////////////////
    class MEZZ_LIB ActorBase {
        private:
            friend class WorldNode;
            friend class ActorGraphicsSettings;
            friend class ActorBasePhysicsSettings;

        public:
            /// @enum ActorTypeName
            /// @brief A listing of Actor TypeNames
            /// @details These will be returned by ActorBase::GetType(), and will allow
            /// code using this to determine what type of Actor class they are working with
            /// and use this information to more safely cast to the correct Actor if needed.
            enum ActorTypeName
            {
                Actorbase,
                Actorcharacter,
                Actorrigid,
                Actorsoft,
                Actorterrain
            };

        protected:
            /// @brief This class encapsulates the functionality of the Ogre::Entity using this
            Ogre::Entity* GraphicsObject;

            /// @brief This class encapsulates the functionality of the Ogre::SceneNode using this
            Ogre::SceneNode* GraphicsNode;

            /// @brief This class encapsulates the functionality of the Ogre::AnimationState using this
            Ogre::AnimationState* Animation;

            /// @brief This class encapsulates the functionality of the btCollisionShape using this
            btCollisionShape* Shape;

            /// @brief This class encapsulates the functionality of the btCollisionObject using this
            btCollisionObject* CollisionObject;

            /// @brief This class encapsulates the functionality of the PhysMotionState using this
            internal::PhysMotionState* MotionState;

            /// @brief This class excapsulates the functionality of the Ogre::SceneNode.
            WorldNode* ActorWorldNode;

            /// @brief This class encapsulates graphics specific configuration for this actor.
            ActorGraphicsSettings* GraphicsSettings;

            /// @brief This class encapsulates physics specific configuration for this actor.
            ActorBasePhysicsSettings* BasePhysicsSettings;

            /// @brief This variable stores the type of actor that this class is.
            ActorTypeName ActorType;

            /// @brief This member stores all existing collision events referencing this actor.
            std::set<Collision*> CurrentCollisions;

//////////////////////////////////////////////////////////////////////////////
// Ogre Management Functions
///////////////////////////////////////
            /// @brief Sets the location of the graphical body.
            /// @details This will take a Vector3 and set the location of the actor within the graphical world. @n
            /// This function is called on by the SetLocation function, and shouldn't be called manually.
            /// @param Place The Vector3 representing the location.
            virtual void SetOgreLocation(const Vector3& Place);

            /// @brief Retrieves the location of the graphical body.
            /// @details This function will retrieve the location of the object within the graphical world. This should always match the physics world.
            /// @return This returns a Mezzanine::Vector3 with the location of the graphics.
            virtual Vector3 GetOgreLocation() const;

            /// @brief Sets the orientation of the graphical body.
            /// @details This will take a PhysQuaternion and set the orientation of the actor within the graphical world. @n
            /// This function is called on by the SetOrientation function, and shouldn't be called manually.
            /// @param Rotation The quaternion representing the rotation of the actor.
            virtual void SetOgreOrientation(const Quaternion& Rotation);

            /// @brief Gets the orientation of the graphical body.
            /// @return Returns a quaternion representing the rotation of the actor.
            virtual Quaternion GetOgreOrientation() const;

//////////////////////////////////////////////////////////////////////////////
// Bullet Management Functions
///////////////////////////////////////
            /// @brief Sets the location of the physics body.
            /// @details This will take a Vector3 and set the location of the actor within the physics world. @n
            /// This function is called on by the SetLocation function, and shouldn't be called manually.
            /// @param Location The Vector3 representing the location.
            virtual void SetBulletLocation (const Vector3& Location);

            /// @brief Retrieves the location of the physics body.
            /// @details This function will retrieve the location of the object within the physics world.
            /// @return a Mezzanine::Vector3.
            virtual Vector3 GetBulletLocation() const;

            /// @brief Sets the orientation of the physics body.
            /// @details This will take a Quaternion and set the orientation of the actor within the physics world. @n
            /// This function is called on by the SetOrientation function, and shouldn't be called manually.
            /// @param Rotation The quaternion representing the rotation of the actor.
            virtual void SetBulletOrientation (const Quaternion& Rotation);

            /// @brief Gets the orientation of the graphical body.
            /// @return Returns a quaternion representing the rotation of the actor.
            virtual Quaternion GetBulletOrientation() const;

//////////////////////////////////////////////////////////////////////////////
// Other Management Functions
///////////////////////////////////////
            /// @brief Makes the actor visable.
            /// @details Adds the actor to all the nessessary graphics elements to make it visable on screen. @n
            /// This is automaticly called by the Worlds AddActor function and shouldn't ever need to be called manually.
            virtual void AttachToGraphics();

            /// @brief Makes the actor invisable.
            /// @details This is the inverse of the AttachToGraphics function.  This will effectively remove the object from the graphics world and make it no longer visable. @n
            /// This is automaticly called by the Worlds RemoveActor function and shouldn't ever need to be called manually.
            virtual void DetachFromGraphics();

        public:
///////////////////////////////////////////////////////////////////////////////
// Creation, Destruction and Initialization
///////////////////////////////////////
            /// @brief Constructor.
            /// @details This constructor contains the basic information needed to make an actor.
            // @param name The name of the actor.
            // @param file The 3d mesh file that contains the 3d model the actor will use.
            // @param group The resource group where the 3d mesh and other related files can be found.
            ActorBase ();

            /// @brief Destructor.
            /// @details The class destructor.
            virtual ~ActorBase ();

            /// @brief Retrieves the name of the object.
            /// @details This function will retrieve the name of the object.
            /// @return This should return a String containing the Name
            virtual String GetName() const = 0;

            /// @brief Manually sets the location of the actor.
            /// @details Calling this function prior to adding it to the World will have no effect. @n
            /// In most situations you won't want to use this function, and instead produce movement through physics functions.
            /// @param x Location on the X vector.
            /// @param y Location on the Y vector.
            /// @param z Location on the Z vector.
            virtual void SetLocation(const Real& x, const Real& y, const Real& z);

            /// @brief Manually sets the location of the actor.
            /// @details Calling this function prior to adding it to the World will have no effect. @n
            /// In most situations you won't want to use this function, and instead produce movement through physics functions.
            /// @param Place The Vector3 representing the location.
            virtual void SetLocation(const Vector3& Place);

            /// @brief Retrieves the location of the object.
            /// @details This function will retrieve the location of the object within the world.
            /// @return A Mezzanine::Vector3 containing the location.
            virtual Vector3 GetLocation() const;

            /// @brief Sets the orientation of the actor.
            /// @details Sets the orientation of the actor via Quaternion parameters.
            /// @param x Where the X vector is rotated about.
            /// @param y Where the Y vector is rotated about.
            /// @param z Where the Z vector is rotated about.
            /// @param w How much to about the x, y, z.
            virtual void SetOrientation(const Real& x, const Real& y, const Real& z, const Real& w);

            /// @brief Sets the orientation of the actor.
            /// @details Sets the orientation of the actor via a Quaternion.
            /// @param Rotation The Quaternion representing the Rotation.
            virtual void SetOrientation(const Quaternion& Rotation);

            /// @brief Gets the orientation of the actor.
            /// @return Returns a quaternion representing the rotation of the actor.
            virtual Quaternion GetOrientation() const;

///////////////////////////////////////////////////////////////////////////////
// Utility and Configuration
///////////////////////////////////////
            ///@brief This is a collection of sounds for use with this actor.
            Audio::SoundSet* ActorSounds;

            /// @brief Gets the type of actor this class is.
            /// @details This function will get the type of class that you are working with for checking and casting.
            /// @return ActorTypeName The type of actor that this is.
            virtual int GetType() const;

            /// @brief Gets a WorldNode representing the position and orientation of this actor.
            /// @details The WorldNode returned by this function is not stored in the scene manasger.
            /// @return Returns a WorldNode pointer pointing to this actor's world node.
            virtual WorldNode* GetActorNode() const;

            /// @brief Gets whether or not this object is currently in the world.
            /// @return Returns a bool indicating if this object has been added to the world.
            virtual bool IsInWorld() const;

            /// @brief Checks of the actor is static or kinematic.
            /// @details Checks of the actor is static or kinematic, returns true if it is either.
            /// @return Returns true if the actor is static or kinematic.
            virtual bool IsStaticOrKinematic() const;

            /// @brief Sets the animation for this object.
            /// @details This function will get the specified animation for this object stored in the mesh file, and will loop the
            /// animation if specified.
            /// @param AnimationName Name of the stored animation to be loaded.
            /// @param Loop Whether or not you want the animation to loop.  For example, you would want an idle animation to loop,
            /// but not a death animation.
            virtual void SetAnimation(ConstString& AnimationName, bool Loop);

            /// @brief Enables the animation if one is set.
            /// @details This function will enable the animation if passed true, making the object animate.  If passed false will
            /// disable the animation.
            /// @param Enable True to enable the animation or false to disable the animation.
            virtual void EnableAnimation(bool Enable);

            /// @brief Tells whether this actor is animated or not.
            /// @details This function will return true if the actor has an animation set and it is enabled.
            /// @return Returns true if an animation is set and enabled.
            virtual bool IsAnimated() const;

            /// @brief Advances the animation, making it animate.
            /// @param Time The amount of time to advance the animation.
            /// @details You need to call this every frame while the actor is to be animated, otherwise even with the animation
            /// enabled you will see no change in the animation.
            virtual void AdvanceAnimation(const Real& Time);

            /// @brief Unloads a loaded animation.
            /// @details This function will remove the existing set animation.
            virtual void RemoveSetAnimation();

            /// @brief Sets the scale of the actor.
            /// @details This function will alter the scaling/size of the actor with the given vector3.
            /// @param scale The vector3 by which to apply the scale.  Will scale each axis' accordingly.
            virtual void SetActorScaling(const Vector3& scale);

            /// @brief Gets the current scaling being applied to the actor.
            /// @return Returns a vector3 representing the scaling being applied on all axes of this actor.
            virtual Vector3 GetActorScaling() const;

            /// @brief Gets all current collisions that apply to this actor.
            /// @return Returns a const reference to a set containing all collisions events containing this actor.
            virtual const std::set<Collision*>& GetCurrentCollisions();

            /// @brief Gets the graphics settings class associated with this actor.
            /// @return Returns a pointer to the graphics settings class in use by this actor.
            virtual ActorGraphicsSettings* GetGraphicsSettings() const;

            /// @brief Gets the physics settings class associated with this actor.
            /// @return Returns a pointer to the physics settings class in use by this actor.
            virtual ActorBasePhysicsSettings* GetPhysicsSettings() const;

///////////////////////////////////////////////////////////////////////////////
// Working with the World
///////////////////////////////////////
            /// @brief Adds the actor to the physics world.
            virtual void AddObjectToWorld() = 0;

            /// @brief Removes the actor from the physics world.
            virtual void RemoveObjectFromWorld() = 0;

///////////////////////////////////////////////////////////////////////////////
// Internal Object functions
///////////////////////////////////////
            /// @internal
            /// @brief Utility function for altering or checking the actor every frame.
            virtual void _Update() = 0;

            /// @internal
            /// @brief Notifies this actor of a collision that is occuring with it.
            /// @param Col A pointer to the collision pertaining to this actor.
            /// @param State The state of the collision pertaining to this actor.
            virtual void _NotifyCollisionState(Collision* Col, const Collision::CollisionState& State);

            /// @internal
            /// @brief Gets the internal physics object this actor is based on.
            /// @return Returns a pointer to the internal Bullet object.
            virtual btCollisionObject* _GetBasePhysicsObject() const;

            /// @internal
            /// @brief Gets the internal graphics object this actor is based on.
            /// @return Returns a pointer to the internal graphics object.
            virtual Ogre::Entity* _GetGraphicsObject() const;

            /// @internal
            /// @brief Gets the internal graphics node this actor uses for it's graphics transform.
            /// @return Returns a pointer to the internal graphics node.
            virtual Ogre::SceneNode* _GetGraphicsNode() const;

///////////////////////////////////////////////////////////////////////////////
// Serialization
///////////////////////////////////////
#ifdef MEZZXML
        protected:
            /// @internal
            /// @brief a Helper function that assembles strings and throws an exception
            /// @param Fail The item that failed.
            virtual void ThrowSerialError(const String& Fail) const;

        public:
            /// @brief Get the name of the the XML tag that implementations of this class will use to save the serialized graphics settings.
            /// @return A string containing name of the serialized graphics settings.
            virtual String GraphicsSettingsSerializableName() const = 0;

            /// @brief Get the name of the the XML tag that implementations of this class will use to save the serialized s settings.
            /// @return A string containing name of the serialized graphics settings.
            virtual String PhysicsSettingsSerializableName() const = 0;

            // Serializable
            /// @brief Convert this class to an xml::Node ready for serialization
            /// @param CurrentRoot The point in the XML hierarchy that all this vectorw should be appended to.
            virtual void ProtoSerialize(xml::Node& CurrentRoot) const;

            // DeSerializable
            /// @brief Take the data stored in an XML and overwrite this instance of this object with it
            /// @param OneNode and xml::Node containing the data.
            /// @warning A precondition of using this is that all of the actors intended for use must already be Deserialized.
            virtual void ProtoDeSerialize(const xml::Node& OneNode);

            /// @brief Get the name of the the XML tag this class will leave behind as its instances are serialized.
            /// @return A string containing "Point2PointConstraint"
            static String SerializableName();
#endif
    };

} // /phys


#endif
