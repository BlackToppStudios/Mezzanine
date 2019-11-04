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
#ifndef _graphicsmesh_cpp
#define _graphicsmesh_cpp

#include "Graphics/mesh.h"
#include "Graphics/submesh.h"

#include <OgreMesh.h>

namespace Mezzanine
{
    namespace Graphics
    {
        ///////////////////////////////////////////////////////////////////////////////
        // Mesh Methods

        Mesh::Mesh(Ogre::MeshPtr ToWrap, ManualMeshLoader* Loader) :
            InternalMesh(ToWrap),
            InternalLoader(Loader)
        {
            this->WrapAllSubMeshes();
        }

        Mesh::~Mesh()
        {
            if( this->InternalLoader ) {
                delete this->InternalLoader;
            }
            this->DestroyAllWrappedSubMeshes();
        }

        void Mesh::WrapAllSubMeshes()
        {
            // Clear out our current SubMeshes if they exist.
            this->DestroyAllWrappedSubMeshes();

            // Go over each SubMesh and wrap them all.
            if( this->InternalMesh ) {
                Whole SubMeshCount = this->InternalMesh->getNumSubMeshes();
                for( Whole SubMeshIndex = 0 ; SubMeshIndex < SubMeshCount ; ++SubMeshIndex )
                {
                    this->SubMeshes.push_back( new SubMesh( this->InternalMesh->getSubMesh(SubMeshIndex) ) );
                }
            }
        }

        void Mesh::DestroyAllWrappedSubMeshes()
        {
            for( SubMeshIterator SubMeshIt = this->SubMeshes.begin() ; SubMeshIt != this->SubMeshes.end() ; ++SubMeshIt )
                { delete (*SubMeshIt); }
            this->SubMeshes.clear();
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Asset Methods

        const String& Mesh::GetName() const
            { return this->InternalMesh->getName(); }

        const String& Mesh::GetGroup() const
            { return this->InternalMesh->getGroup(); }

        ///////////////////////////////////////////////////////////////////////////////
        // Utility Methods

        Whole Mesh::GetVertexCount() const
        {
            Whole Ret = 0;
            for( ConstSubMeshIterator SubMeshIt = this->SubMeshes.begin() ; SubMeshIt != this->SubMeshes.end() ; ++SubMeshIt )
                { Ret += (*SubMeshIt)->GetVertexCount(); }
            return Ret;
        }

        Whole Mesh::GetIndexCount() const
        {
            Whole Ret = 0;
            for( ConstSubMeshIterator SubMeshIt = this->SubMeshes.begin() ; SubMeshIt != this->SubMeshes.end() ; ++SubMeshIt )
                { Ret += (*SubMeshIt)->GetIndexCount(); }
            return Ret;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // SubMesh Methods

        SubMesh* Mesh::GetSubMesh(const Whole Index) const
            { return this->SubMeshes.at(Index); }

        Whole Mesh::GetNumSubMeshes() const
            { return this->SubMeshes.size(); }

        ///////////////////////////////////////////////////////////////////////////////
        // Skeleton Methods

        String Mesh::GetSkeletonName() const
            { return this->InternalMesh->getSkeletonName(); }

        ///////////////////////////////////////////////////////////////////////////////
        // Internal Methods

        Ogre::MeshPtr Mesh::_GetInternalMesh() const
            { return this->InternalMesh; }

        Ogre::MeshPtr Mesh::_Upcast(Ogre::ResourcePtr ToCast)
            { return std::static_pointer_cast<Ogre::Mesh>(ToCast); }
    }//Graphics
}//Mezzanine

#endif
