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
#ifndef _worldobject_h
#define _worldobject_h

#include "enumerations.h"
#include "datatypes.h"
#include "collision.h"
#include "worldobjectgraphicssettings.h"

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
    ///////////////////////////////////////////////////////////////////////////////
    /// @class WorldObject
    /// @headerfile worldobject.h
    /// @brief This is the base class from which classes that are insertable into the physical world.
    /// @details In short a WorldObject is an object that has a physical representation and may also
    /// have a graphical representation and can be added and removed from the world.  In almost all
    /// occasions these objects can also collide with other objects.
    ///////////////////////////////////////
    class WorldObject
    {
        protected:
            friend class WorldObjectGraphicsSettings;
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

            /// @brief The name of the terrain
            String Name;

            /// @brief This member stores all existing collision events referencing this actor.
            std::set<Collision*> CurrentCollisions;

            /// @brief Makes the terrain visible.
            virtual void AttachToGraphics();

            /// @brief Makes the terrain invisible.
            virtual void DetachFromGraphics();
        public:
            /// @brief Class constructor.
            WorldObject();
            /// @brief Class destructor.
            virtual ~WorldObject();

            ///////////////////////////////////////////////////////////////////////////////
            // Utility and Configuration
            ///////////////////////////////////////

            /// @brief Gets the name of this terrain.
            /// @return Returns a string containing the name of this terrain.
            virtual String GetName() const;

            /// @brief Gets the type of the terrain instance
            /// @return Returns the type of the terrain instance
            virtual WorldObjectType GetType() const = 0;

            /// @brief Gets whether or not this object is currently in the world.
            /// @return Returns a bool indicating if this object has been added to the world.
            virtual bool IsInWorld() const;

            /// @brief Gets the graphics settings class associated with this World Object.
            /// @return Returns a pointer to the graphics settings class in use by this World Object.
            virtual WorldObjectGraphicsSettings* GetGraphicsSettings() const;

            ///////////////////////////////////////////////////////////////////////////////
            // Working with the World
            ///////////////////////////////////////

            /// @brief Adds the Terrain object to the World
            /// @details Attaches the Terrain to the World to be rendered and adds the collision object to
            /// the PhysicsManager @n
            /// This is automatically called by the CreateTerrain methods in the TerrainManager
            virtual void AddToWorld() = 0;
            /// @brief Removed the Terrain object from the World
            /// @details detaches the Terrain from the Graphics and removes the collision object from the PhysicsManager
            virtual void RemoveFromWorld() = 0;

            ///////////////////////////////////////////////////////////////////////////////
            // Internal Object functions
            ///////////////////////////////////////

            /// @internal
            /// @brief Utility function for altering or checking the actor every frame.
            virtual void _Update() = 0;

            /// @internal
            /// @brief Notifies this terrain of a collision that is occuring with it.
            /// @param Col A pointer to the collision pertaining to this terrain.
            /// @param State The state of the collision pertaining to this terrain.
            virtual void _NotifyCollisionState(Collision* Col, const Collision::CollisionState& State);

            /// @internal
            /// @brief Gets the internal physics object this terrain is based on.
            /// @return Returns a pointer to the internal Bullet object.
            virtual btCollisionObject* _GetBasePhysicsObject() const;

            /// @internal
            /// @brief Gets the internal graphics object this terrain is based on.
            /// @return Returns a pointer to the internal graphics object.
            virtual Ogre::Entity* _GetGraphicsObject() const;

            /// @internal
            /// @brief Gets the internal graphics node this terrain uses for it's graphics transform.
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

            /// @brief Get the name of the the XML tag that implementations of this class will use to save the serialized graphics settings.
            /// @return A string containing name of the serialized graphics settings.
            virtual String GraphicsSettingsSerializableName() const = 0;

        public:
            /// @brief Get the name of the the XML tag this class will leave behind as its instances are serialized.
            /// @return A string containing "Point2PointConstraint"
            static String SerializableName();
#endif
    };//WorldObject
}//Mezzanine

#endif
