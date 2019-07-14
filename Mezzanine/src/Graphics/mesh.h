// © Copyright 2010 - 2017 BlackTopp Studios Inc.
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
#ifndef _graphicsmesh_h
#define _graphicsmesh_h

#include "datatypes.h"

namespace Ogre
{
    class Resource;
    typedef std::shared_ptr<Resource> ResourcePtr;
    class Mesh;
    typedef std::shared_ptr<Mesh> MeshPtr;
}//Ogre

namespace Mezzanine
{
    namespace Graphics
    {
        class SubMesh;
        class Skeleton;
        class ManualMeshLoader;
        ///////////////////////////////////////////////////////////////////////////////
        /// @brief This class is used to check and modify the properties of a graphics mesh.
        ///////////////////////////////////////
        class MEZZ_LIB Mesh
        {
        public:
            /// @brief A convenience type for the storage of SubMeshes in this class.
            typedef std::vector<SubMesh*>             SubMeshContainer;
            /// @brief An iterator type for SubMeshes being stored by this class.
            typedef SubMeshContainer::iterator        SubMeshIterator;
            /// @brief A const iterator type for SubMeshes being stored by this class.
            typedef SubMeshContainer::const_iterator  ConstSubMeshIterator;
        protected:
            /// @brief A pointer to the internal data this Mesh is based on.
            Ogre::MeshPtr InternalMesh;
            /// @brief A pointer to the loader that will make the Mesh data available to the graphics system.
            ManualMeshLoader* InternalLoader;
            /// @brief If Skeletal animations are enabled on this Mesh, this is a pointer to the Skeleton being used.
            Skeleton* MeshSkel;
            /// @brief A container storing all the SubMeshes in this Mesh.
            SubMeshContainer SubMeshes;

            /// @brief Constructs a Mezzanine wrapper for every SubMesh in the internal Mesh.
            void WrapAllSubMeshes();
            /// @brief Destroys every wrapped (but not the underlying instance) SubMesh in this Mesh.
            void DestroyAllWrappedSubMeshes();
        public:
            /// @internal
            /// @brief Internal Constructor.
            /// @param ToWrap The internal Mesh the instance will wrap.
            /// @param Loader The loader that will make the Mesh data available to the graphics system.
            Mesh(Ogre::MeshPtr ToWrap, ManualMeshLoader* Loader);
            /// @brief Class Destructor.
            ~Mesh();

            ///////////////////////////////////////////////////////////////////////////////
            // Asset Query

            /// @brief Gets the Name of this Mesh.
            /// @note If this Mesh originated from a file, usually the name of the Mesh will be the file name.
            /// @return Returns a const string reference containing the name of this Mesh.
            const String& GetName() const;
            /// @brief Gets the resource group this Mesh belongs to.
            /// @return Returns a const string reference containing the group this Mesh belongs to.
            const String& GetGroup() const;

            ///////////////////////////////////////////////////////////////////////////////
            // Utility Methods

            /// @brief Gets the number of Vertices in this Mesh.
            /// @return Returns the number of Vertices that make up all SubMeshes in this Mesh.
            Whole GetVertexCount() const;
            /// @brief Gets the number of Indices in this Mesh.
            /// @return Returns the number of Indices used to assemble the vertices in all SubMeshes in this Mesh.
            Whole GetIndexCount() const;

            ///////////////////////////////////////////////////////////////////////////////
            // SubMesh Methods

            /// @brief Gets a SubMesh by index.
            /// @param Index The index of the SubMesh to retrieve.
            /// @return Returns a pointer to the SubMesh at the specified index.
            SubMesh* GetSubMesh(const Whole Index) const;
            /// @brief Gets the number of SubMeshes in this Mesh.
            /// @return Returns a Whole representing the number of SubMeshes that make up this Mesh.
            Whole GetNumSubMeshes() const;

            ///////////////////////////////////////////////////////////////////////////////
            // Skeleton Methods\

            ///////////////////////////////////////////////////////////////////////////////
            // Internal Methods

            /// @internal
            /// @brief Gets the internal Mesh pointer.
            /// @return Returns a shared pointer pointing to the internal Mesh.
            Ogre::MeshPtr _GetInternalMesh() const;
            /// @internal
            /// @brief Casts an internal resource pointer to a MeshPtr.
            /// @param ToCast The pointer to be casted.
            /// @return Returns the casted MeshPtr.
            static Ogre::MeshPtr _Upcast(Ogre::ResourcePtr ToCast);
        };//Mesh
    }//Graphics
}//Mezzanine

#endif
