//© Copyright 2010 - 2012 BlackTopp Studios Inc.
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

#include "worldnode.h"
#include "worldobject.h"
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
    namespace Internal
    {
        class PhysMotionState;
        class AttachableMotionState;
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
    class MEZZ_LIB ActorBase : public NonStaticWorldObject
    {
        private:
            friend class WorldNode;
            friend class ActorGraphicsSettings;
            friend class ActorBasePhysicsSettings;
        protected:
            /// @brief This class encapsulates the functionality of the Ogre::AnimationState using this
            Ogre::AnimationState* Animation;

            /// @brief This class encapsulates the functionality of the PhysMotionState using this
            Internal::AttachableMotionState* MotionState;
        public:
            ///////////////////////////////////////////////////////////////////////////////
            // Creation, Destruction and Initialization
            ///////////////////////////////////////
            /// @brief Constructor.
            /// @details This constructor contains the basic information needed to make an actor.
            ActorBase ();

            /// @brief Destructor.
            /// @details The class destructor.
            virtual ~ActorBase ();

            ///////////////////////////////////////////////////////////////////////////////
            // Utility and Configuration
            ///////////////////////////////////////
            /// @copydoc WorldObject::GetType()
            virtual WorldAndSceneObjectType GetType() const = 0;

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

            ///////////////////////////////////////////////////////////////////////////////
            // Working with the World
            ///////////////////////////////////////
            /// @brief Adds the actor to the physics world.
            virtual void AddToWorld() = 0;

            /// @brief Removes the actor from the physics world.
            virtual void RemoveFromWorld() = 0;

            ///////////////////////////////////////////////////////////////////////////////
            // Internal Object functions
            ///////////////////////////////////////
            /// @internal
            /// @brief Utility function for altering or checking the actor every frame.
            virtual void _Update() = 0;

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
