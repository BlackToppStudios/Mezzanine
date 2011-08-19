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
#ifndef _collisionshapemanager_h
#define _collisionshapemanager_h

#include "datatypes.h"
#include "managerbase.h"

class btTriangleMesh;

namespace phys
{
    class Mesh;
    class CollisionShape;
    class ConvexHullCollisionShape;
    class DynamicMeshCollisionShape;
    class StaticMeshCollisionShape;
    class CompoundCollisionShape;
    ///////////////////////////////////////////////////////////////////////////////
    /// @class CollisionShapeManager
    /// @headerfile collisionshapemanager.h
    /// @brief This manager is for the storage of all shapes and creation of mesh shapes.
    /// @details Collision shapes do not need to be stored in this manager, but can be re-used
    /// across multiple World objects with physics representations.  For performance reasons, it is
    /// recommended to store and re-use a collision shape anytime you need it in multiple objects,
    /// rather then re-creating the same shape again and again.
    ///////////////////////////////////////
    class PHYS_LIB CollisionShapeManager : public ManagerBase
    {
        protected:
            std::map<String,CollisionShape*> CollisionShapes;

            /// @brief Creates a TriMesh to be used in TriMesh based collision shapes.
            btTriangleMesh* CreateBulletTrimesh(Mesh* ObjectMesh, bool UseAllSubmeshes);
        public:
            /// @brief Class constructor.
            CollisionShapeManager();
            /// @brief Class destructor.
            virtual ~CollisionShapeManager();

            ///////////////////////////////////////////////////////////////////////////////
            // Generic Shape Management

            /// @brief Stores a pre-made shape in this manager.
            /// @param Shape The shape to be stored.
            virtual void StoreShape(CollisionShape* Shape);
            /// @brief Gets a shape already stored in this manager.
            /// @return Returns a pointer to the desired shape.
            /// @param Name the name of the desired shape.
            virtual CollisionShape* GetShape(const String& Name);
            /// @brief Gets the number of stored shapes in this manager.
            /// @return Returns a whole representing how many shapes this manager is storing.
            virtual Whole GetNumStoredShapes();
            /// @brief Removes a shape from this manager without deleting it.
            /// @param Shape Pointer to the shape to be removed.
            virtual void RemoveShape(CollisionShape* Shape);
            /// @brief Removes a shape from this manager without deleting it.
            /// @param Name The name of the shape to be removed.
            virtual void RemoveShape(const String& Name);
            /// @brief Removes all shapes from the manager without deleting them.
            virtual void RemoveAllShapes();
            /// @brief Removes a shape from this manager and deletes it.
            /// @param Shape Pointer to the shape to be destroyed.
            virtual void DestroyShape(CollisionShape* Shape);
            /// @brief Removes a shape from this manager and deletes it.
            /// @param Name The name of the shape to be destroyed.
            virtual void DestroyShape(const String& Name);
            /// @brief Removes all shapes from the manager and then deletes them.
            virtual void DestroyAllShapes();

            ///////////////////////////////////////////////////////////////////////////////
            // Shape Creation Utilities

            /// @brief Generates a Convex Hull from a provided mesh.
            /// @return Returns a pointer to the created shape.
            /// @param Name The name to give the created shape.
            /// @param ObjectMesh The mesh to base this shape off of.
            /// @param UseAllSubMeshes Whether or not you want to use all submesh information when generating this shape.
            virtual ConvexHullCollisionShape* GenerateConvexHull(const String& Name, Mesh* ObjectMesh, bool UseAllSubmeshes = false);
            /// @brief Generates a Convex Hull from a provided mesh.
            /// @return Returns a pointer to the created shape.
            /// @param Name The name to give the created shape.
            /// @param MeshName The name of the mesh to base this shape off of.
            /// @param Group The resource group where the mesh can be found.
            /// @param UseAllSubMeshes Whether or not you want to use all submesh information when generating this shape.
            virtual ConvexHullCollisionShape* GenerateConvexHull(const String& Name, const String& MeshName, const String& Group, bool UseAllSubmeshes = false);
            /// @brief Generates a mesh shape for dynamic objects.
            /// @return Returns a pointer to the created shape.
            /// @param Name The name to give the created shape.
            /// @param ObjectMesh The mesh to base this shape off of.
            /// @param UseAllSubMeshes Whether or not you want to use all submesh information when generating this shape.
            virtual DynamicMeshCollisionShape* GenerateDynamicTriMesh(const String& Name, Mesh* ObjectMesh, bool UseAllSubmeshes = false);
            /// @brief Generates a mesh shape for dynamic objects.
            /// @return Returns a pointer to the created shape.
            /// @param Name The name to give the created shape.
            /// @param MeshName The name of the mesh to base this shape off of.
            /// @param Group The resource group where the mesh can be found.
            /// @param UseAllSubMeshes Whether or not you want to use all submesh information when generating this shape.
            virtual DynamicMeshCollisionShape* GenerateDynamicTriMesh(const String& Name, const String& MeshName, const String& Group, bool UseAllSubmeshes = false);
            /// @brief Generates a mesh shape for static objects.
            /// @return Returns a pointer to the created shape.
            /// @param Name The name to give the created shape.
            /// @param ObjectMesh The mesh to base this shape off of.
            /// @param UseAllSubMeshes Whether or not you want to use all submesh information when generating this shape.
            virtual StaticMeshCollisionShape* GenerateStaticTriMesh(const String& Name, Mesh* ObjectMesh, bool UseAllSubmeshes = false);
            /// @brief Generates a mesh shape for static objects.
            /// @return Returns a pointer to the created shape.
            /// @param Name The name to give the created shape.
            /// @param MeshName The name of the mesh to base this shape off of.
            /// @param Group The resource group where the mesh can be found.
            /// @param UseAllSubMeshes Whether or not you want to use all submesh information when generating this shape.
            virtual StaticMeshCollisionShape* GenerateStaticTriMesh(const String& Name, const String& MeshName, const String& Group, bool UseAllSubmeshes = false);
            /// @brief Generates a compound shape of Convex Hulls from a provided mesh.
            /// @return Returns a pointer to the created shape.
            /// @param Name The name to give the created shape.
            /// @param ObjectMesh The mesh to base this shape off of.
            /// @param UseAllSubMeshes Whether or not you want to use all submesh information when generating this shape.
            virtual CompoundCollisionShape* PerformConvexDecomposition(const String& Name, Mesh* ObjectMesh, Whole Depth, Real CPercent, Real PPercent, bool UseAllSubmeshes = false);
            /// @brief Generates a compound shape of Convex Hulls from a provided mesh.
            /// @return Returns a pointer to the created shape.
            /// @param Name The name to give the created shape.
            /// @param MeshName The name of the mesh to base this shape off of.
            /// @param Group The resource group where the mesh can be found.
            /// @param UseAllSubMeshes Whether or not you want to use all submesh information when generating this shape.
            virtual CompoundCollisionShape* PerformConvexDecomposition(const String& Name, const String& MeshName, const String& Group, Whole Depth, Real CPercent, Real PPercent, bool UseAllSubmeshes = false);

            ///////////////////////////////////////////////////////////////////////////////
            // Shape Saving/Loading Utilities

            /// @brief Loads all shapes saved in an existing .bullet file, and stores them in this manager.
            /// @param FileName The name of the file to load shapes from.
            /// @param Group The resource group the .bullet file is located in.
            virtual void LoadAllShapesFromFile(const String& FileName, const String& Group);
            /// @brief Takes all the shapes currently stored this manager and saves them to a .bullet file.
            /// @param FileName The name of the file to save the shapes to.
            virtual void SaveAllStoredShapesToFile(const String& FileName);
            /// @brief Saves all shapes contained in a vector and saves them to a .bullet file.
            /// @param FileName The name of the file to save the shapes to.
            /// @param ShapesToSave A vector of collisions shapes that will be saved.
            virtual void SaveShapesToFile(const String& FileName, std::vector<CollisionShape*>& ShapesToSave);

            ///////////////////////////////////////////////////////////////////////////////
            // Inherited from Managerbase

            /// @copydoc phys::ManagerBase::Initialize()
            virtual void Initialize();

            /// @copydoc phys::ManagerBase::DoMainLoopItems()
            virtual void DoMainLoopItems();

            /// @copydoc phys::ManagerBase::GetType()
            virtual ManagerBase::ManagerTypeName GetType() const;
    };//collisionshapemanager
}//phys

#endif
