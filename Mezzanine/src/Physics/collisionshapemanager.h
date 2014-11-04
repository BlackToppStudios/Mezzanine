// © Copyright 2010 - 2014 BlackTopp Studios Inc.
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
#ifndef _collisionshapemanager_h
#define _collisionshapemanager_h

#include "datatypes.h"
#include "Physics/collisionshape.h"
#ifndef SWIG
    #include "entresolmanager.h"
    #include "managerfactory.h"
#endif
#include "singleton.h"

class btTriangleMesh;
class btCollisionShape;

namespace Mezzanine
{
    // Used by the scripting language binder to help create bindgings for this class.
    #ifdef SWIG
    %template(SingletonCollisionShapeManager) Mezzanine::Singleton<CollisionShapeManager>;
    #endif

    namespace Graphics
    {
        class Mesh;
    }
    namespace Physics
    {
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
        class MEZZ_LIB CollisionShapeManager : public EntresolManager, public Singleton<CollisionShapeManager>
        {
        public:
            /// @brief Map container type for CollisionShape storage by this class.
            typedef std::map< String, CollisionShape* >   ShapeMap;
            /// @brief Map Iterator type for CollisionShape instances stored by this class.
            typedef ShapeMap::iterator                    ShapeMapIterator;
            /// @brief Const Map Iterator type for CollisionShape instances stored by this class.
            typedef ShapeMap::const_iterator              ConstShapeMapIterator;
            /// @brief Vector container type for CollisionShape storage by this class.
            typedef std::vector< CollisionShape* >        ShapeVector;
            /// @brief Vector Iterator type for CollisionShape instances stored by this class.
            typedef ShapeVector::iterator                 ShapeVectorIterator;
            /// @brief Const Vector Iterator type for CollisionShape instances stored by this class.
            typedef ShapeVector::const_iterator           ConstShapeVectorIterator;
        protected:
            /// @internal
            /// @brief This stores the names and collision Shapes
            ShapeMap CollisionShapes;
            /// @internal
            /// @brief Stores shapes that have not been given a name.
            ShapeVector UnnamedShapes;

            /// @brief Creates a TriMesh to be used in TriMesh based collision shapes.
            btTriangleMesh* CreateBulletTrimesh(Graphics::Mesh* ObjectMesh, Boole UseAllSubmeshes);
            /// @brief Creates a wrapper for an internal bullet shape.
            CollisionShape* WrapShape(const String& Name, btCollisionShape* InternalShape);
        public:
            /// @brief Class constructor.
            CollisionShapeManager();
            /// @brief XML constructor.
            /// @param XMLNode The node of the xml document to construct from.
            CollisionShapeManager(XML::Node& XMLNode);
            /// @brief Class destructor.
            virtual ~CollisionShapeManager();

            ///////////////////////////////////////////////////////////////////////////////
            // Generic Shape Management

            /// @brief Stores a pre-made shape in this manager.
            /// @param Shape The shape to be stored.
            virtual void StoreShape(CollisionShape* Shape);
            /// @brief Gets a shape already stored in this manager.
            /// @param Name the name of the desired shape.
            /// @return Returns a pointer to the desired shape, or NULL if no such shape exists.
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

            /// @brief Gets an Iterator to the first CollisionShape in this manager.
            /// @return Returns an Iterator to the first CollisionShape.
            ShapeMapIterator BeginCollisionShape();
            /// @brief Gets an Iterator to one passed the last CollisionShape in this manager.
            /// @return Returns an Iterator to one passed the last CollisionShape.
            ShapeMapIterator EndCollisionShape();
            #ifndef SWIG
            /// @brief Gets a Const Iterator to the first CollisionShape in this manager.
            /// @return Returns a Const Iterator to the first CollisionShape.
            ConstShapeMapIterator BeginCollisionShape() const;
            /// @brief Gets a Const Iterator to one passed the last CollisionShape in this manager.
            /// @return Returns a Const Iterator to one passed the last CollisionShape.
            ConstShapeMapIterator EndCollisionShape() const;
            #endif
            ///////////////////////////////////////////////////////////////////////////////
            // Shape Creation Utilities

            /// @brief Generates a Convex Hull from a provided mesh.
            /// @return Returns a pointer to the created shape.
            /// @param Name The name to give the created shape.
            /// @param ObjectMesh The mesh to base this shape off of.
            /// @param UseAllSubmeshes Whether or not you want to use all submesh information when generating this shape.
            virtual ConvexHullCollisionShape* GenerateConvexHull(const String& Name, Graphics::Mesh* ObjectMesh, Boole UseAllSubmeshes = false);
            /// @brief Generates a Convex Hull from a provided mesh.
            /// @return Returns a pointer to the created shape.
            /// @param Name The name to give the created shape.
            /// @param MeshName The name of the mesh to base this shape off of.
            /// @param Group The resource group where the mesh can be found.
            /// @param UseAllSubmeshes Whether or not you want to use all submesh information when generating this shape.
            virtual ConvexHullCollisionShape* GenerateConvexHull(const String& Name, const String& MeshName, const String& Group, Boole UseAllSubmeshes = false);
            /// @brief Generates a mesh shape for dynamic objects.
            /// @note Dynamic Mesh shapes cannot be scaled per object, only globally.  If you are generating this shape and intend to scale it, you will need to make a separate copy of
            /// the shape for each object you intend to set to a different scaling.
            /// @return Returns a pointer to the created shape.
            /// @param Name The name to give the created shape.
            /// @param ObjectMesh The mesh to base this shape off of.
            /// @param UseAllSubmeshes Whether or not you want to use all submesh information when generating this shape.
            virtual DynamicMeshCollisionShape* GenerateDynamicTriMesh(const String& Name, Graphics::Mesh* ObjectMesh, Boole UseAllSubmeshes = false);
            /// @brief Generates a mesh shape for dynamic objects.
            /// @note Dynamic Mesh shapes cannot be scaled per object, only globally.  If you are generating this shape and intend to scale it, you will need to make a separate copy of
            /// the shape for each object you intend to set to a different scaling.
            /// @return Returns a pointer to the created shape.
            /// @param Name The name to give the created shape.
            /// @param MeshName The name of the mesh to base this shape off of.
            /// @param Group The resource group where the mesh can be found.
            /// @param UseAllSubmeshes Whether or not you want to use all submesh information when generating this shape.
            virtual DynamicMeshCollisionShape* GenerateDynamicTriMesh(const String& Name, const String& MeshName, const String& Group, Boole UseAllSubmeshes = false);
            /// @brief Generates a mesh shape for static objects.
            /// @return Returns a pointer to the created shape.
            /// @param Name The name to give the created shape.
            /// @param ObjectMesh The mesh to base this shape off of.
            /// @param UseAllSubmeshes Whether or not you want to use all submesh information when generating this shape.
            virtual StaticMeshCollisionShape* GenerateStaticTriMesh(const String& Name, Graphics::Mesh* ObjectMesh, Boole UseAllSubmeshes = false);
            /// @brief Generates a mesh shape for static objects.
            /// @return Returns a pointer to the created shape.
            /// @param Name The name to give the created shape.
            /// @param MeshName The name of the mesh to base this shape off of.
            /// @param Group The resource group where the mesh can be found.
            /// @param UseAllSubmeshes Whether or not you want to use all submesh information when generating this shape.
            virtual StaticMeshCollisionShape* GenerateStaticTriMesh(const String& Name, const String& MeshName, const String& Group, Boole UseAllSubmeshes = false);
            /// @brief Generates a compound shape of Convex Hulls from a provided mesh.
            /// @note Compound shapes cannot be scaled per object, only globally.  If you are generating this shape and intend to scale it, you will need to make a separate copy of
            /// the shape for each object you intend to set to a different scaling.
            /// @return Returns a pointer to the created shape.
            /// @param Name The name to give the created shape.
            /// @param ObjectMesh The mesh to base this shape off of.
            /// @param UseAllSubmeshes Whether or not you want to use all submesh information when generating this shape.
            virtual CompoundCollisionShape* PerformConvexDecomposition(const String& Name, Graphics::Mesh* ObjectMesh, Whole Depth, Real CPercent, Real PPercent, Boole UseAllSubmeshes = false);
            /// @brief Generates a compound shape of Convex Hulls from a provided mesh.
            /// @note Compound shapes cannot be scaled per object, only globally.  If you are generating this shape and intend to scale it, you will need to make a separate copy of
            /// the shape for each object you intend to set to a different scaling.
            /// @return Returns a pointer to the created shape.
            /// @param Name The name to give the created shape.
            /// @param MeshName The name of the mesh to base this shape off of.
            /// @param Group The resource group where the mesh can be found.
            /// @param UseAllSubmeshes Whether or not you want to use all submesh information when generating this shape.
            virtual CompoundCollisionShape* PerformConvexDecomposition(const String& Name, const String& MeshName, const String& Group, Whole Depth, Real CPercent, Real PPercent, Boole UseAllSubmeshes = false);

            ///////////////////////////////////////////////////////////////////////////////
            // Shape Saving/Loading Utilities

            /// @brief Loads all shapes saved in an existing XML file, and stores them in this manager.
            /// @param FileName The name of the XML file to load shapes from.
            /// @param Group The resource group the .bullet file is located in.
            virtual void LoadAllShapesFromXMLFile(const String& FileName, const String& Group);
            /// @brief Takes all the shapes currently stored this manager and saves them to a XML file.
            /// @param FileName The name of the file to save the shapes to.
            virtual void SaveAllStoredShapesToXMLFile(const String& FileName);
            /// @brief Saves all shapes contained in a vector and saves them to a XML file.
            /// @param FileName The name of the file to save the shapes to.
            /// @param ShapesToSave A vector of collisions shapes that will be saved.
            virtual void SaveShapesToXMLFile(const String& FileName, ShapeVector& ShapesToSave);

            /// @brief Loads all shapes saved in an existing binary file, and stores them in this manager.
            /// @param FileName The name of the binary file to load shapes from.
            /// @param Group The resource group the .bullet file is located in.
            virtual void LoadAllShapesFromBinaryFile(const String& FileName, const String& Group);
            /// @brief Takes all the shapes currently stored this manager and saves them to a binary file.
            /// @remarks The binary file the shapes will be saved to is a file generated by the Bullet Physics library, typically with a .bullet extension.
            /// @param FileName The name of the file to save the shapes to.
            virtual void SaveAllStoredShapesToBinaryFile(const String& FileName);
            /// @brief Saves all shapes contained in a vector and saves them to a binary file.
            /// @remarks The binary file the shapes will be saved to is a file generated by the Bullet Physics library, typically with a .bullet extension.
            /// @param FileName The name of the file to save the shapes to.
            /// @param ShapesToSave A vector of collisions shapes that will be saved.
            virtual void SaveShapesToBinaryFile(const String& FileName, ShapeVector& ShapesToSave);

            ///////////////////////////////////////////////////////////////////////////////
            // Unnamed Shape Management

            /// @brief Returns a vector of unnamed shapes stored in this manager.
            /// @return Returns a reference to a vector storing all the unnamed shapes loaded from files.
            /// @details Shapes created in code require a name to be constructed.  However, sometimes when loading a file
            /// a shape may not have a name, since one isn't required by the .bullet file format in order for a shape
            /// to be serialized.  When that happens those shapes go here, and from there can be handled by the game
            /// programmer however they see fit.
            ShapeVector& GetUnnamedShapes();
            /// @brief Assigns a name to an unnamed shape.
            /// @param NewName The new name to be assigned to a shape.
            /// @param Shape The shape to be given the new name.  This shape must be a valid shape currently stored in the
            /// set of unnamed shapes.  Calling this fucntion will not remove it from that set, but will move it into
            /// the named collision shape map.  If you want the shape removed from the Unnamed set, you must do it yourself.
            void SetNameForUnnamedShape(const String& NewName, CollisionShape* Shape);

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            /// @copydoc ManagerBase::Initialize()
            virtual void Initialize();
            /// @copydoc ManagerBase::Deinitialize()
            virtual void Deinitialize();

            ///////////////////////////////////////////////////////////////////////////////
            // Type Identifier Methods

            /// @copydoc ManagerBase::GetInterfaceType()
            virtual ManagerBase::ManagerType GetInterfaceType() const;
            /// @copydoc ManagerBase::GetImplementationTypeName()
            virtual String GetImplementationTypeName() const;
        };// CollisionShapeManager

        ///////////////////////////////////////////////////////////////////////////////
        /// @class DefaultCollisionShapeManagerFactory
        /// @headerfile collisionshapemanager.h
        /// @brief A factory responsible for the creation and destruction of the default collisionshapemanager.
        ///////////////////////////////////////
        class MEZZ_LIB DefaultCollisionShapeManagerFactory : public ManagerFactory
        {
        public:
            /// @brief Class constructor.
            DefaultCollisionShapeManagerFactory();
            /// @brief Class destructor.
            virtual ~DefaultCollisionShapeManagerFactory();

            /// @copydoc ManagerFactory::GetManagerTypeName()
            String GetManagerTypeName() const;

            /// @copydoc ManagerFactory::CreateManager(NameValuePairList&)
            ManagerBase* CreateManager(NameValuePairList& Params);
            /// @copydoc ManagerFactory::CreateManager(XML::Node&)
            ManagerBase* CreateManager(XML::Node& XMLNode);
            /// @copydoc ManagerFactory::DestroyManager(ManagerBase*)
            void DestroyManager(ManagerBase* ToBeDestroyed);
        };//DefaultCollisionShapeManagerFactory
    }//Physics
}//Mezzanine

#endif
