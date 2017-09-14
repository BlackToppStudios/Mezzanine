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
#ifndef _graphicsvertextools_cpp
#define _graphicsvertextools_cpp

#include "Graphics/vertextools.h"

#include <OgreVertexIndexData.h>

namespace Mezzanine
{
    namespace Graphics
    {
        namespace VertexTools
        {
            Vector3Vec GetVertexPositions(Ogre::VertexData* VertData)
            {
                Vector3Vec ToFill;
                Whole VertexCount = VertData->vertexCount;

                const Ogre::VertexElement* PositionElem = VertData->vertexDeclaration->findElementBySemantic(Ogre::VES_POSITION);
                if( PositionElem != NULL ) {
                    ToFill.resize( VertexCount );
                    Ogre::HardwareVertexBufferSharedPtr vBuffer = VertData->vertexBufferBinding->getBuffer(PositionElem->getSource());

                    float* pReal = NULL;
                    unsigned char* vertex = static_cast<unsigned char*>(vBuffer->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));
                    for( size_t j = VertData->vertexStart ; j < VertexCount ; j++, vertex += vBuffer->getVertexSize() )
                    {
                        PositionElem->baseVertexPointerToElement(vertex, &pReal);
                        ToFill[j].X = *pReal++;
                        ToFill[j].Y = *pReal++;
                        ToFill[j].Z = *pReal++;
                    }
                    vBuffer->unlock();
                }
                return ToFill;
            }

            Vector2Vec GetVertexTexCoords(Ogre::VertexData* VertData)
            {
                Vector2Vec ToFill;
                Whole VertexCount = VertData->vertexCount;

                const Ogre::VertexElement* TexCoordElem = VertData->vertexDeclaration->findElementBySemantic(Ogre::VES_TEXTURE_COORDINATES);
                if( TexCoordElem != NULL ) {
                    ToFill.resize( VertexCount );
                    Ogre::HardwareVertexBufferSharedPtr vBuffer = VertData->vertexBufferBinding->getBuffer(TexCoordElem->getSource());

                    float* pReal = NULL;
                    unsigned char* vertex = static_cast<unsigned char*>(vBuffer->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));
                    for( size_t j = VertData->vertexStart ; j < VertexCount ; j++, vertex += vBuffer->getVertexSize() )
                    {
                        TexCoordElem->baseVertexPointerToElement(vertex, &pReal);
                        ToFill[j].X = *pReal++;
                        ToFill[j].Y = *pReal++;
                    }
                    vBuffer->unlock();
                }
                return ToFill;
            }

            Vector3Vec GetVertexNormals(Ogre::VertexData* VertData)
            {
                Vector3Vec ToFill;
                Whole VertexCount = VertData->vertexCount;

                const Ogre::VertexElement* NormalElem = VertData->vertexDeclaration->findElementBySemantic(Ogre::VES_NORMAL);
                if( NormalElem != NULL ) {
                    ToFill.resize( VertexCount );
                    Ogre::HardwareVertexBufferSharedPtr vBuffer = VertData->vertexBufferBinding->getBuffer(NormalElem->getSource());

                    float* pReal = NULL;
                    unsigned char* vertex = static_cast<unsigned char*>(vBuffer->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));
                    for( size_t j = VertData->vertexStart ; j < VertexCount ; j++, vertex += vBuffer->getVertexSize() )
                    {
                        NormalElem->baseVertexPointerToElement(vertex, &pReal);
                        ToFill[j].X = *pReal++;
                        ToFill[j].Y = *pReal++;
                        ToFill[j].Z = *pReal++;
                    }
                    vBuffer->unlock();
                }
                return ToFill;
            }

            Vector3Vec GetVertexTangents(Ogre::VertexData* VertData)
            {
                Vector3Vec ToFill;
                Whole VertexCount = VertData->vertexCount;

                const Ogre::VertexElement* TangentElem = VertData->vertexDeclaration->findElementBySemantic(Ogre::VES_TANGENT);
                if( TangentElem != NULL ) {
                    ToFill.resize( VertexCount );
                    Ogre::HardwareVertexBufferSharedPtr vBuffer = VertData->vertexBufferBinding->getBuffer(TangentElem->getSource());

                    float* pReal = NULL;
                    unsigned char* vertex = static_cast<unsigned char*>(vBuffer->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));
                    for( size_t j = VertData->vertexStart ; j < VertexCount ; j++, vertex += vBuffer->getVertexSize() )
                    {
                        TangentElem->baseVertexPointerToElement(vertex, &pReal);
                        ToFill[j].X = *pReal++;
                        ToFill[j].Y = *pReal++;
                        ToFill[j].Z = *pReal++;
                    }
                    vBuffer->unlock();
                }
                return ToFill;
            }

            Vector3Vec GetVertexBinormals(Ogre::VertexData* VertData)
            {
                Vector3Vec ToFill;
                Whole VertexCount = VertData->vertexCount;

                const Ogre::VertexElement* BinormalElem = VertData->vertexDeclaration->findElementBySemantic(Ogre::VES_BINORMAL);
                if( BinormalElem != NULL ) {
                    ToFill.resize( VertexCount );
                    Ogre::HardwareVertexBufferSharedPtr vBuffer = VertData->vertexBufferBinding->getBuffer(BinormalElem->getSource());

                    float* pReal = NULL;
                    unsigned char* vertex = static_cast<unsigned char*>(vBuffer->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));
                    for( size_t j = VertData->vertexStart ; j < VertexCount ; j++, vertex += vBuffer->getVertexSize() )
                    {
                        BinormalElem->baseVertexPointerToElement(vertex, &pReal);
                        ToFill[j].X = *pReal++;
                        ToFill[j].Y = *pReal++;
                        ToFill[j].Z = *pReal++;
                    }
                    vBuffer->unlock();
                }
                return ToFill;
            }

            IntVec GetIndices(Ogre::IndexData* IdxData)
            {
                IntVec ToFill;
                Ogre::HardwareIndexBufferSharedPtr iBuffer = IdxData->indexBuffer;
                Boole use32bitindexes = ( iBuffer->getType() == Ogre::HardwareIndexBuffer::IT_32BIT );
                Whole IndexCount = IdxData->indexCount;
                ToFill.resize( IndexCount );

                long* pLong = static_cast<long*>( iBuffer->lock( Ogre::HardwareBuffer::HBL_READ_ONLY ) );
                if( use32bitindexes ) {
                    for( size_t CurrIndex = IdxData->indexStart ; CurrIndex < IndexCount ; ++CurrIndex )
                        { ToFill[CurrIndex] = pLong[CurrIndex]; }
                }else{
                    short* pShort = reinterpret_cast<short*>(pLong);
                    for( size_t CurrIndex = IdxData->indexStart ; CurrIndex < IndexCount ; ++CurrIndex )
                        { ToFill[CurrIndex] = static_cast<unsigned long>( pShort[CurrIndex] ); }
                }
                iBuffer->unlock();
                return ToFill;
            }
        }
    }//Graphics
}//Mezzanine

#endif
