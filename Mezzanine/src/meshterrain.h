//© Copyright 2010 - 2011 BlackTopp Studios Inc.
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

#ifndef _meshterrain_h
#define _meshterrain_h

#include "terrainbase.h"


///////////////////////////////////
// Forward Declares

class btRigidBody;
class btEmptyShape;

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


namespace phys
{

    class World;
    class ActorGraphicsSettings;
    class ActorBasePhysicsSettings;
    class ActorRigidPhysicsSettings;
    class CollisionShape;
    namespace internal
    {
        class PhysMotionState;
        struct MeshInfo;
    }

    class PHYS_LIB MeshTerrain : public TerrainBase
    {
        protected:
            /// @internal
            /// @brief The physics data.
            btRigidBody* RigidBody;
            //abstraction for other libraries
            /// @brief This class encapsulates the functionality of the Ogre::Entity using this
            Ogre::Entity* GraphicsObject;

            /// @brief This class encapsulates the functionality of the Ogre::SceneNode using this
            Ogre::SceneNode* GraphicsNode;

            /// @brief This class encapsulates the functionality of the btCollisionShape using this
            btCollisionShape* Shape;

            /// @brief This class encapsulates the functionality of the btCollisionObject using this
            btCollisionObject* CollisionObject;

            /// @brief This class excapsulates the functionality of the Ogre::SceneNode.
            WorldNode* TerrainWorldNode;

            /// @brief This class encapsulates the functionality of the PhysMotionState using this
            internal::PhysMotionState* MotionState;

            /// @brief This class encapsulates graphics specific configuration for this actor.
            //ActorGraphicsSettings* GraphicsSettings;

            /// @brief This class encapsulates physics specific configuration for this actor.
            //ActorBasePhysicsSettings* BasePhysicsSettings;

            /// @brief Stores whether or not the current physics shape has been saved in the manager.
            bool ShapeIsSaved;
        public:
            /// @brief Class constructor.
            /// @details The class constructor.
            /// @param InitPosition The location for this terrain.
            /// @param name The name of the terrain.
            /// @param file The 3d mesh file that contains the 3d model the terrain will use.
            /// @param group The resource group where the 3d mesh and other related files can be found.
            MeshTerrain(Vector3 InitPosition, String name, String file, String group);
            /// @brief Class destructor.
            /// @details The class destructor.
            virtual ~MeshTerrain();

//////////////////////////////////////////////////////////////////////////////
// Ogre Management Functions
///////////////////////////////////////

            /// @brief Sets the location of the graphical body.
            /// @details This will take a Vector3 and set the location of the terrain within the graphical world. @n
            /// This function is called on by the SetLocation function, and shouldn't be called manually.
            /// @param Place The Vector3 representing the location.
            void SetOgreLocation(const Vector3& Location);
            /// @brief Retrieves the location of the graphical body.
            /// @details This function will retrieve the location of the object within the graphical world. This should always match the physics world.
            /// @return This returns a phys::Vector3 with the location of the graphics.
            Vector3 GetOgreLocation() const;
            /// @brief Sets the orientation of the graphical body.
            /// @details This will take a PhysQuaternion and set the orientation of the terrain within the graphical world. @n
            /// This function is called on by the SetOrientation function, and shouldn't be called manually.
            /// @param Rotation The quaternion representing the rotation of the terrain.
            void SetOgreOrientation(const Quaternion& Rotation);
            /// @brief Gets the orientation of the graphical body.
            /// @return Returns a quaternion representing the rotation of the terrain.
            Quaternion GetOgreOrientation() const;

//////////////////////////////////////////////////////////////////////////////
// Bullet Management Functions
///////////////////////////////////////

            /// @brief Sets the location of the physics body.
            /// @details This will take a Vector3 and set the location of the actor within the physics world. @n
            /// This function is called on by the SetLocation function, and shouldn't be called manually.
            /// @param Location The Vector3 representing the location.
            void SetBulletLocation(const Vector3& Location);
            /// @brief Retrieves the location of the physics body.
            /// @details This function will retrieve the location of the object within the physics world.
            /// @return a phys::Vector3.
            Vector3 GetBulletLocation() const;
            /// @brief Sets the orientation of the physics body.
            /// @details This will take a PhysQuaternion and set the orientation of the terrain within the physics world. @n
            /// This function is called on by the SetOrientation function, and shouldn't be called manually.
            /// @param Rotation The quaternion representing the rotation of the terrain.
            void SetBulletOrientation(const Quaternion& Rotation);
            /// @brief Gets the orientation of the graphical body.
            /// @return Returns a quaternion representing the rotation of the terrain
            Quaternion GetBulletOrientation() const;

//////////////////////////////////////////////////////////////////////////////
// Other Management Functions
///////////////////////////////////////

            /// @brief Makes the terrain visable.
            /// @details Adds the terrain to all the nessessary graphics elements to make it visable on screen. @n
            /// This is automaticly called by the CreateTerrain method in the TerrainManager.
            void AttachToGraphics();
            /// @brief Makes the terrain invisable.
            /// @details This is the inverse of the AttachToGraphics function.  This will effectively remove the object from the graphics world and make it no longer visable. @n
            /// This is automaticly called by the RemoveTerrain method in the TerrainManager
            void DetachFromGraphics();

///////////////////////////////////////////////////////////////////////////////
// Initialization
///////////////////////////////////////

            /// @brief Manually sets the location of the terrain.
            /// @details Calling this function prior to adding it to the World will have no effect. @n
            /// In most situations you won't want to use this function, and instead produce movement through physics functions.
            /// @param x Location on the X vector.
            /// @param y Location on the Y vector.
            /// @param z Location on the Z vector.
            void SetLocation(const Real& x, const Real& y, const Real& z);
            /// @brief Manually sets the location of the terrain.
            /// @details Calling this function prior to adding it to the World will have no effect. @n
            /// In most situations you won't want to use this function, and instead produce movement through physics functions.
            /// @param Place The Vector3 representing the location.
            void SetLocation(const Vector3& Place);
            /// @brief Retrieves the location of the object.
            /// @details This function will retrieve the location of the object within the world.
            /// @return A phys::Vector3 containing the location.
            Vector3 GetLocation() const;
            /// @brief Sets the orientation of the terrain.
            /// @details Sets the orientation of the terrain via Quaternion parameters.
            /// @param x Where the X vector is rotated about.
            /// @param y Where the Y vector is rotated about.
            /// @param z Where the Z vector is rotated about.
            /// @param w How much to about the x, y, z.
            void SetOrientation(const Real& x, const Real& y, const Real& z, const Real& w);
            /// @brief Sets the orientation of the terrain.
            /// @details Sets the orientation of the terrain via a Quaternion.
            /// @param Rotation The Quaternion representing the Rotation.
            void SetOrientation(const Quaternion& Rotation);
            /// @brief Gets the orientation of the terrain.
            /// @return Returns a quaternion representing the rotation of the terrain.
            Quaternion GetOrientation() const;

///////////////////////////////////////////////////////////////////////////////
// Utility and Configuration
///////////////////////////////////////

            /// @brief Gets the type of terrain this class is.
            /// @details This function will get the type of class that you are working with for checking and casting.
            /// @return TerrainTypeName The type of actor that this is.
            TerrainTypeName GetType() const;
            /// @brief Gets a WorldNode representing the position and orientation of this terrain.
            /// @details The WorldNode returned by this function is not stored in the scene manasger.
            /// @return Returns a WorldNode pointer pointing to this terrain's world node.
            WorldNode* GetTerrainNode() const;

            const bool GetShapeIsSaved() const;

            /// @brief Gets whether or not this object is currently in the world.
            /// @return Returns a bool indicating if this object has been added to the world.
            bool IsInWorld() const;
            /// @brief Sets the scale of the terrain.
            /// @details This function will alter the scaling/size of the terrain with the given vector3.
            /// @param scale The vector3 by which to apply the scale.  Will scale each axis' accordingly.
            void SetScaling(const Vector3& scaling);

            /// @brief Gets the internal physics object this actor is based on.
            /// @return Returns a pointer to the internal Bullet object.
            btCollisionObject* GetBaseBulletObject() const;

            /// @brief Gets the internal graphics object this terrain is based on.
            /// @return Returns a pointer to the internal graphics object.
            Ogre::Entity* GetOgreObject() const;

            /// @brief Uses value already passed into this to create the physics shapes.
            void CreateCollisionTerrain();


            void CreateShapeFromMeshStatic(bool UseAllSubmeshes = false);

            /// @brief Adds the terrain to the physics world.
            void AddTerrainToWorld ();
            /// @brief Removes the terrain from the physics world
            void RemoveTerrainFromWorld();
            /// @internal
            /// @brief Get the Physics data raw from the physic subsystem
            /// @return Currently this returns a pointer to a btSoftBody.
            btRigidBody* GetBulletObject();

            /// @brief Sets the shape to determine the collision for the terrain
            void SetCollisionShape( CollisionShape* shape );
        protected:
        private:
    };
}


#endif // _meshterrain_h
