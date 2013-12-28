// © Copyright 2010 - 2013 BlackTopp Studios Inc.
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
#ifndef _graphicsmesh_cpp
#define _graphicsmesh_cpp

#include "Graphics/mesh.h"
#include <Ogre.h>

namespace Mezzanine
{
    namespace Graphics
    {
        ///////////////////////////////////////////////////////////////////////////////
        /// @brief This class is used to store the internal structures needed by the Mesh class.
        /// @details Specifically, this class stores a shared pointer to the Ogre Mesh and only
        /// exists because shared pointers can't be forward declared without compromising how they
        /// work.
        ///////////////////////////////////////
        class MEZZ_LIB InternalMeshData
        {
            public:
                Ogre::MeshPtr GraphicsMesh;
        };//InternalMeshData

        ///////////////////////////////////////////////////////////////////////////////
        // Mesh Methods

        /*Mesh::Mesh()
        {
            this->IMD = new InternalMeshData();
            this->IMD->GraphicsMesh.setNull();
        }//*/

        Mesh::Mesh(Ogre::MeshPtr InternalMesh)
        {
            this->IMD = new InternalMeshData();
            this->IMD->GraphicsMesh = InternalMesh;
        }

        Mesh::~Mesh()
        {
            delete IMD;
        }

        ConstString& Mesh::GetName() const
        {
            return _GetInternalMesh()->getName();
        }

        ConstString& Mesh::GetGroup() const
        {
            return _GetInternalMesh()->getGroup();
        }

        Ogre::MeshPtr Mesh::_GetInternalMesh() const
        {
            return this->IMD->GraphicsMesh;
        }
    }//Graphics
}//Mezzanine

#endif
