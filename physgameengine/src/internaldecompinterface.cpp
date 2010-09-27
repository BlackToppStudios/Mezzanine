//© Copyright 2010 BlackTopp Studios Inc.
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
#ifndef _internaldecompinterface_cpp
#define _internaldecompinterface_cpp

#include "internaldecompinterface.h.cpp"

// Keeps this file form being documented by doxygen
/// @cond 0

namespace phys{
    namespace internal{
        PhysConvexDecomposition::PhysConvexDecomposition()
        {
            mBaseCount=0;
            mHullCount=0;
        }

        void PhysConvexDecomposition::ConvexDecompResult(ConvexDecomposition::ConvexResult &result)
        {
            btTriangleMesh* trimesh = new btTriangleMesh();

            btVector3 localScaling(1.f,1.f,1.f);

            //calc centroid, to shift vertices around center of mass
            btVector3 centroid;
            centroid.setValue(0,0,0);

            btAlignedObjectArray<btVector3> vertices;

            //const unsigned int *src = result.mHullIndices;
            for (unsigned int i=0; i<result.mHullVcount; i++)
            {
                    btVector3 vertex(result.mHullVertices[i*3],result.mHullVertices[i*3+1],result.mHullVertices[i*3+2]);
                    vertex *= localScaling;
                    centroid += vertex;
            }

            centroid *= 1.f/(float(result.mHullVcount) );

            //const unsigned int *src = result.mHullIndices;
            for (unsigned int i=0; i<result.mHullVcount; i++)
            {
                btVector3 vertex(result.mHullVertices[i*3],result.mHullVertices[i*3+1],result.mHullVertices[i*3+2]);
                vertex *= localScaling;
                vertex -= centroid ;
                vertices.push_back(vertex);
            }
            const unsigned int *src = result.mHullIndices;
            for (unsigned int i=0; i<result.mHullTcount; i++)
            {
                unsigned int index0 = *src++;
                unsigned int index1 = *src++;
                unsigned int index2 = *src++;

                btVector3 vertex0(result.mHullVertices[index0*3], result.mHullVertices[index0*3+1],result.mHullVertices[index0*3+2]);
                btVector3 vertex1(result.mHullVertices[index1*3], result.mHullVertices[index1*3+1],result.mHullVertices[index1*3+2]);
                btVector3 vertex2(result.mHullVertices[index2*3], result.mHullVertices[index2*3+1],result.mHullVertices[index2*3+2]);
                vertex0 *= localScaling;
                vertex1 *= localScaling;
                vertex2 *= localScaling;

                vertex0 -= centroid;
                vertex1 -= centroid;
                vertex2 -= centroid;

                trimesh->addTriangle(vertex0,vertex1,vertex2);

                index0+=mBaseCount;
                index1+=mBaseCount;
                index2+=mBaseCount;
            }

            btConvexHullShape* convexShape = new btConvexHullShape(&(vertices[0].getX()),vertices.size());

            convexShape->setMargin(0.01f);
            m_convexShapes.push_back(convexShape);
            m_convexCentroids.push_back(centroid);
            mBaseCount+=result.mHullVcount; // advance the 'base index' counter.
        }
    }
}


#endif
