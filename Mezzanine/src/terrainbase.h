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

#ifndef _terrainbase_h
#define _terrainbase_h

#include "worldobject.h"

namespace Mezzanine
{
    ///////////////////////////////////////////////////////////////////////////////
    /// @class TerrainBase
    /// @headerfile terrainbase.h
    /// @brief This is the base class from which all the terrains inherit.
    /// @details The terrain classes store and manage all the relevant data regarding
    /// terrains inside the World.  They serve as a binder between the physics and
    /// graphics for terrain and have functions that allow the manipulation of terrain
    /// loaded into the World.  Currently there is 1 terrain class: MeshTerrain.
    ///////////////////////////////////////
    class MEZZ_LIB TerrainBase : public WorldObject
    {
        public:
            /// @enum TerrainTypeName
            /// @brief A listing of Terrain TypeNames
            /// @details These will be returned by TerrainBase::GetType(), and will allow
            /// code using this to determine what type of terrain class they are working with
            /// and use this information to more safely cast to the correct terrain if needed.
            enum TerrainTypeName
            {
                Terrainbase,
                Meshterrain
            };
        protected:
            /// @brief This class encapsulates the functionality of the Ogre::Entity using this
            Ogre::Entity* GraphicsObject;

            /// @brief This class encapsulates the functionality of the Ogre::SceneNode using this
            Ogre::SceneNode* GraphicsNode;

            /// @brief This class encapsulates the functionality of the btCollisionShape using this
            btCollisionShape* Shape;

            /// @brief This class encapsulates the functionality of the btCollisionObject using this
            btCollisionObject* CollisionObject;

            /// @brief Stores the type of terrain.
            TerrainTypeName TerrainType;

            /// @brief The name of the terrain
            String Name;

            /// @brief This member stores all existing collision events referencing this actor.
            std::set<Collision*> CurrentCollisions;

            /// @brief Makes the terrain visible.
            virtual void AttachToGraphics();

            /// @brief Makes the terrain invisible.
            virtual void DetachFromGraphics();
        public:
            ///////////////////////////////////////////////////////////////////////////////
            // Creation, Destruction and Initialization
            ///////////////////////////////////////
            /// @brief Class constructor.
            TerrainBase();
            /// @brief Class destructor.
            virtual ~TerrainBase();

            ///////////////////////////////////////////////////////////////////////////////
            // Utility and Configuration
            ///////////////////////////////////////

            /// @brief Gets the name of this terrain.
            /// @return Returns a string containing the name of this terrain.
            String GetName();

            /// @brief Gets the type of the terrain instance
            /// @return Returns the type of the terrain instance
            virtual TerrainTypeName GetType() const = 0;

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
    };
}//Mezzanine

#endif // _terrainbase_h
