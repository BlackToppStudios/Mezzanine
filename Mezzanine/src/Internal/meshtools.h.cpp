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
#ifndef _internalmeshtools_h_cpp
#define _internalmeshtools_h_cpp

// Keeps this file form being documented by doxygen
/// @cond DontDocumentInternal

#include <btBulletDynamicsCommon.h>
#include <Ogre.h>
#include "datatypes.h"

namespace Mezzanine
{
    namespace Internal
    {
        /// @internal
        /// @struct This struct helps stored collected info of a mesh.
        /// @headerfile internalmeshtools.h.cpp
        /// @brief This is a helper data structure for gathering mesh information.
        /// @details As the class is gathering information from the mesh to use in a soft body, it'll store the values here so that the entity can be safely removed.
        struct MEZZ_LIB MeshInfo
        {
            MeshInfo();
            ~MeshInfo();
            Ogre::Vector3* Verticies;
            int* Indicies;
            Ogre::Vector3* Normals;
            Ogre::Vector2* Textures;
            int VCount;
            int ICount;
            Ogre::String Name;
            Ogre::String Material;
            Ogre::String MaterialFile;
            Ogre::String Group;
            Ogre::RenderOperation::OperationType RendOp;
        };

        /// @internal
        /// @class MeshTools
        /// @headerfile internalmeshtools.h.cpp
        /// @brief This is a helper class to aid in mesh manipulation and information gathering.
        /// @details
        class MEZZ_LIB MeshTools
        {
            protected:
            public:
                /// @brief Class constructor.
                MeshTools();
                /// @brief Class destructor.
                ~MeshTools();
                /// @brief Gets the verticy information of the mesh.
                /// @details This function will read the mesh provided and gather the verticies inside it for re-use.
                /// @param TheEntity The entity from which to extract the information.
                /// @param TheMesh The struct to populate with the information gathered.
                static void GetMeshVerticies(Ogre::Entity* TheEntity, MeshInfo &TheMesh);
                /// @brief Gets the indicy information of the mesh.
                /// @details This function will read the mesh provided and gather the indicies inside it for re-use.
                /// @param TheEntity The entity from which to extract the information.
                /// @param TheMesh The struct to populate with the information gathered.
                static void GetMeshIndicies(Ogre::Entity* TheEntity, MeshInfo &TheMesh);
                /// @brief Gets the normals information of the mesh.
                /// @details This function will read the mesh provided and gather the normals of each verticy inside it for re-use.
                /// @param TheEntity The entity from which to extract the information.
                /// @param TheMesh The struct to populate with the information gathered.
                static void GetMeshNormals(Ogre::Entity* TheEntity, MeshInfo &TheMesh);
                /// @brief Gets the texture coordinates information of the mesh.
                /// @details This function will read the mesh provided and gather the texture coordinates inside it for re-use.
                /// @param TheEntity The entity from which to extract the information.
                /// @param TheMesh The struct to populate with the information gathered.
                static void GetMeshTextures(Ogre::Entity* TheEntity, MeshInfo &TheMesh);
                /// @brief Gets other information from the mesh.
                /// @details This function will get the Render Operation, Material Name, and Entity Name from the entity for re-use.
                /// @param TheEntity The entity from which to extract the information.
                /// @param TheMesh The struct to populate with the information gathered.
                static void GetOtherMeshInfo(Ogre::Entity* TheEntity, MeshInfo &TheMesh);
                /// @brief Creates a trimesh shape from the mesh file.
                /// @details Makes a trimesh to be used as a collision shape in the physics world from a mesh file.
                /// @param TheEntity The entity from which to extract the information.
                /// @param UseAllSubmeshes If true, this will use the geometry of all submeshes of the model to make the shape.  Otherwise it'll only use the first submesh.
                static btTriangleMesh* CreateBulletTrimesh(Ogre::Entity* TheEntity, Boolean UseAllSubmeshes = false);
                /// @brief used to get information about 3d graphical objects at specific locations
                static void GetMeshInformation(Ogre::Entity *entity,
                                    size_t &vertex_count,
                                    Ogre::Vector3* &vertices,
                                    size_t &index_count,
                                    unsigned long* &indices,
                                    const Ogre::Vector3 &position,
                                    const Ogre::Quaternion &orient,
                                    const Ogre::Vector3 &scale);
        };//internalmeshtool
    }//internal
}//Mezzanine

/// @endcond

#endif
