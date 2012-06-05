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
#ifndef _worldobject_h
#define _worldobject_h

#include "enumerations.h"
#include "vector3.h"
#include "quaternion.h"
#include "attachable.h"
#include "Physics/collision.h"
#include "worldobjectgraphicssettings.h"
#include "worldobjectphysicssettings.h"

///////////////////////////////////
// Forward Declares
class btRigidBody;
class btCollisionShape;
class btCollisionObject;

namespace Ogre
{
    class Entity;
    class SceneNode;
    class Vector3;
    class ResourceGroupManager;
}

namespace Mezzanine
{
    class World;
    class WorldNode;
    class ActorBasePhysicsSettings;
    namespace Audio
    {
        class SoundSet;
    }
    namespace Internal
    {
        class PhysMotionState;
        struct MeshInfo;
    }
    ///////////////////////////////////////////////////////////////////////////////
    /// @class WorldObject
    /// @headerfile worldobject.h
    /// @brief This is the base class from which classes that are insertable into the physical world.
    /// @details In short a WorldObject is an object that has a physical representation and may also
    /// have a graphical representation and can be added and removed from the world.  In almost all
    /// occasions these objects can also collide with other objects.
    ///////////////////////////////////////
    class MEZZ_LIB WorldObject
    {
        protected:
            friend class WorldNode;
            friend class WorldObjectGraphicsSettings;
            friend class WorldObjectPhysicsSettings;
            /// @brief This class encapsulates the functionality of the Ogre::Entity using this
            Ogre::Entity* GraphicsObject;

            /// @brief This class encapsulates the functionality of the Ogre::SceneNode using this
            Ogre::SceneNode* GraphicsNode;

            /// @brief This class encapsulates the functionality of the btCollisionShape using this
            btCollisionShape* PhysicsShape;

            /// @brief This class encapsulates the functionality of the btCollisionObject using this
            btCollisionObject* PhysicsObject;

            /// @brief This class encapsulates graphics specific configuration for this actor.
            WorldObjectGraphicsSettings* GraphicsSettings;

            /// @brief This class encapsulates physics specific configuration for this actor.
            WorldObjectPhysicsSettings* PhysicsSettings;

            ///@brief This is a collection of sounds for use with this actor.
            Audio::SoundSet* ObjectSounds;

            /// @brief The name of the terrain
            String Name;

            /// @brief This member stores all existing collision events referencing this actor.
            std::set<Physics::Collision*> CurrentCollisions;

            /// @brief Makes the terrain visible.
            virtual void AttachToGraphics();

            /// @brief Makes the terrain invisible.
            virtual void DetachFromGraphics();
        public:
            /// @brief Class constructor.
            WorldObject();
            /// @brief Class destructor.
            virtual ~WorldObject();

            /// @brief Manually sets the location of the World Object.
            /// @details Calling this function prior to adding it to the World will have no effect. @n
            /// In most situations you won't want to use this function, and instead produce movement through physics functions.
            /// @param x Location on the X vector.
            /// @param y Location on the Y vector.
            /// @param z Location on the Z vector.
            virtual void SetLocation(const Real& x, const Real& y, const Real& z);

            /// @brief Manually sets the location of the World Object.
            /// @details Calling this function prior to adding it to the World will have no effect. @n
            /// In most situations you won't want to use this function, and instead produce movement through physics functions.
            /// @param Location The Vector3 representing the location.
            virtual void SetLocation(const Vector3& Location);

            /// @brief Retrieves the location of the object.
            /// @details This function will retrieve the location of the object within the world.
            /// @return A Mezzanine::Vector3 containing the location.
            virtual Vector3 GetLocation() const;

            ///////////////////////////////////////////////////////////////////////////////
            // Utility and Configuration
            ///////////////////////////////////////

            /// @brief Gets the name of this World Object.
            /// @return Returns a string containing the name of this World Object.
            virtual ConstString& GetName() const;

            /// @brief Gets the type of the World Object instance
            /// @return Returns the type of the World Object instance
            virtual WorldAndSceneObjectType GetType() const = 0;

            /// @brief Checks of the World Object is static or kinematic.
            /// @details Checks of the World Object is static or kinematic, returns true if it is either.
            /// @return Returns true if the World Object is static or kinematic.
            virtual bool IsStaticOrKinematic() const;

            /// @brief Gets whether or not this object is currently in the world.
            /// @return Returns a bool indicating if this object has been added to the world.
            virtual bool IsInWorld() const;

            /// @brief Sets the scale of the World Object.
            /// @details This function will alter the scaling/size of the World Object with the given vector3.
            /// @param Scale The vector3 by which to apply the scale.  Will scale each axis' accordingly.
            virtual void SetScaling(const Vector3& Scale);

            /// @brief Gets the current scaling being applied to the World Object.
            /// @return Returns a vector3 representing the scaling being applied on all axes of this World Object.
            virtual Vector3 GetScaling() const;

            /// @brief Gets the graphics settings class associated with this World Object.
            /// @return Returns a pointer to the graphics settings class in use by this World Object.
            virtual WorldObjectGraphicsSettings* GetGraphicsSettings() const;

            /// @brief Gets the physics settings class associated with this World Object.
            /// @return Returns a pointer to the physics settings class in use by this World Object.
            virtual WorldObjectPhysicsSettings* GetPhysicsSettings() const;

            /// @brief Sets the sounds to be used by this object.
            /// @param Sounds The soundset to be assigned to this object.
            virtual void SetSounds(Audio::SoundSet* Sounds);

            /// @brief Gets the set of sounds associated with this World Object.
            /// @return Returns a pointer to a soundset containing all the sounds this World Object uses.
            virtual Audio::SoundSet* GetSounds() const;

            ///////////////////////////////////////////////////////////////////////////////
            // Working with the World
            ///////////////////////////////////////

            /// @brief Adds the World Object to the World.
            virtual void AddToWorld() = 0;
            /// @brief Removes the World Object from the World.
            virtual void RemoveFromWorld() = 0;

            ///////////////////////////////////////////////////////////////////////////////
            // Internal Object functions
            ///////////////////////////////////////

            /// @internal
            /// @brief Utility function for altering or checking the World Object every frame.
            virtual void _Update() = 0;

            /// @internal
            /// @brief Notifies this World Object of a collision that is occuring with it.
            /// @param Col A pointer to the collision pertaining to this World Object.
            /// @param State The state of the collision pertaining to this World Object.
            virtual void _NotifyCollisionState(Physics::Collision* Col, const Physics::Collision::CollisionState& State);

            /// @internal
            /// @brief Gets the internal physics object this World Object is based on.
            /// @return Returns a pointer to the internal Bullet object.
            virtual btCollisionObject* _GetBasePhysicsObject() const;

            /// @internal
            /// @brief Gets the internal graphics object this World Object is based on.
            /// @return Returns a pointer to the internal graphics object.
            virtual Ogre::Entity* _GetGraphicsObject() const;

            /// @internal
            /// @brief Gets the internal graphics node this World Object uses for it's graphics transform.
            /// @return Returns a pointer to the internal graphics node.
            virtual Ogre::SceneNode* _GetGraphicsNode() const;

            ///////////////////////////////////////////////////////////////////////////////
            // Serialization
            ///////////////////////////////////////
#ifdef MEZZXML
        protected:
            /// @internal
            /// @brief A Helper function that assembles strings and throws an exception
            /// @param Fail The item that failed.
            virtual void ThrowSerialError(const String& Fail) const;

            /// @brief Get the name of the the XML tag that implementations of this class will use to save the serialized graphics settings.
            /// @return A string containing name of the serialized graphics settings.
            virtual String GraphicsSettingsSerializableName() const = 0;

            /// @brief Get the name of the the XML tag that implementations of this class will use to save the serialized s settings.
            /// @return A string containing name of the serialized graphics settings.
            virtual String PhysicsSettingsSerializableName() const = 0;

            // Serializable
            /// @brief Convert this class to an XML::Node ready for serialization
            /// @param CurrentRoot The point in the XML hierarchy that all this vectorw should be appended to.
            virtual void ProtoSerialize(XML::Node& CurrentRoot) const;

            // DeSerializable
            /// @brief Take the data stored in an XML and overwrite this instance of this object with it
            /// @param OneNode and XML::Node containing the data.
            /// @warning A precondition of using this is that all of the actors intended for use must already be Deserialized.
            virtual void ProtoDeSerialize(const XML::Node& OneNode);

        public:
            /// @brief Get the name of the the XML tag this class will leave behind as its instances are serialized.
            /// @return A string containing "Point2PointConstraint"
            static String SerializableName();
#endif
    };//WorldObject

    ///////////////////////////////////////////////////////////////////////////////
    /// @class NonStaticWorldObject
    /// @headerfile worldobject.h
    /// @brief This is a small extention of the WorldObject class that adds features common to non-static objects.
    /// @details Many world objects are capable of being moved and oriented dynamically and at run time, but there
    /// are some objects that you don't want to have the ability or API to do so as it may break the intent of the
    /// object.  This class is here to be avoided for those objects, and instead inherit directly from the
    /// WorldObject class while all other's inherit from this class.  It's important to note that this class doesn't
    /// forbid static objects as the name may suggest, but rather it makes static status optional instead of forcing
    /// them to be static.
    ///////////////////////////////////////
    class MEZZ_LIB NonStaticWorldObject : public WorldObject, public AttachableParent
    {
        public:
            using WorldObject::SetLocation;
        protected:
            /// @brief Sets the orientation of the World Object.
            void InternalSetOrientation(const Quaternion& Rotation);
        public:
            ///////////////////////////////////////////////////////////////////////////////
            // Creation, Destruction and Initialization
            ///////////////////////////////////////
            /// @brief Class constructor.
            NonStaticWorldObject();
            /// @brief Class destructor.
            virtual ~NonStaticWorldObject();

            /// @copydoc WorldObject::SetLocation(Vector3&)
            virtual void SetLocation(const Vector3& Location);

            /// @copydoc WorldObject::GetLocation()
            virtual Vector3 GetLocation() const;

            /// @brief Sets the orientation of the World Object.
            /// @details Sets the orientation of the World Object via Quaternion parameters.
            /// @param x Where the X vector is rotated about.
            /// @param y Where the Y vector is rotated about.
            /// @param z Where the Z vector is rotated about.
            /// @param w How much to about the x, y, z.
            virtual void SetOrientation(const Real& x, const Real& y, const Real& z, const Real& w);

            /// @brief Sets the orientation of the World Object.
            /// @details Sets the orientation of the World Object via a Quaternion.
            /// @param Rotation The Quaternion representing the Rotation.
            virtual void SetOrientation(const Quaternion& Rotation);

            /// @brief Gets the orientation of the World Object.
            /// @return Returns a quaternion representing the rotation of the World Object.
            virtual Quaternion GetOrientation() const;

            ///////////////////////////////////////////////////////////////////////////////
            // Utility and Configuration
            ///////////////////////////////////////

            /// @copydoc WorldObject::GetName()
            virtual ConstString& GetName() const;

            /// @copydoc WorldObject::SetScaling(Vector3&)
            virtual void SetScaling(const Vector3& Scale);

            /// @copydoc WorldObject::GetScaling()
            virtual Vector3 GetScaling() const;

            ///////////////////////////////////////////////////////////////////////////////
            // Serialization
            ///////////////////////////////////////
#ifdef MEZZXML
        protected:
            /// @internal
            /// @brief a Helper function that assembles strings and throws an exception
            /// @param Fail The item that failed.
            virtual void ThrowSerialError(const String& Fail) const;

            /// @brief Get the name of the the XML tag that implementations of this class will use to save the serialized graphics settings.
            /// @return A string containing name of the serialized graphics settings.
            virtual String GraphicsSettingsSerializableName() const = 0;

            /// @brief Get the name of the the XML tag that implementations of this class will use to save the serialized s settings.
            /// @return A string containing name of the serialized graphics settings.
            virtual String PhysicsSettingsSerializableName() const = 0;

            // Serializable
            /// @brief Convert this class to an XML::Node ready for serialization
            /// @param CurrentRoot The point in the XML hierarchy that all this vectorw should be appended to.
            virtual void ProtoSerialize(XML::Node& CurrentRoot) const;

            // DeSerializable
            /// @brief Take the data stored in an XML and overwrite this instance of this object with it
            /// @param OneNode and XML::Node containing the data.
            /// @warning A precondition of using this is that all of the actors intended for use must already be Deserialized.
            virtual void ProtoDeSerialize(const XML::Node& OneNode);

        public:
            /// @brief Get the name of the the XML tag this class will leave behind as its instances are serialized.
            /// @return A string containing "Point2PointConstraint"
            static String SerializableName();
#endif
    };//NonStaticWorldObject
}//Mezzanine

#endif
