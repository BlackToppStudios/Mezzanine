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
#ifndef _mesh_h
#define _mesh_h

#include "datatypes.h"

namespace Ogre
{
    class MeshPtr;
}//ogre

namespace Mezzanine
{
    class InternalMeshData;
    ///////////////////////////////////////////////////////////////////////////////
    /// @class Mesh
    /// @headerfile mesh.h
    /// @brief This class is used to check and modify the properties of a graphics mesh.
    /// @details
    ///////////////////////////////////////
    class MEZZ_LIB Mesh
    {
        protected:
            InternalMeshData* IMD;
        public:
            /// @brief Class Constructor.
            Mesh();
            /// @internal
            /// @brief Internal Constructor.
            /// @param InternalMesh The internal mesh this mesh class is based on.
            Mesh(Ogre::MeshPtr InternalMesh);
            /// @brief Class Destructor.
            ~Mesh();
            /// @brief Gets the Name of this mesh.
            /// @note If this mesh originated from a file, usually the name of the mesh will be the file name.
            /// @return Returns a const string reference containing the name of this mesh.
            ConstString& GetName();
            /// @brief Gets the resource group this mesh belongs to.
            /// @return Returns a const string reference containing the group this mesh belongs to.
            ConstString& GetGroup();
            /// @internal
            /// @brief Gets the internal Ogre Mesh pointer.
            /// @return Returns an Ogre shared pointer pointing to the internal Ogre Mesh.
            Ogre::MeshPtr GetOgreMesh();
    };//Mesh
}//Mezzanine

#endif
