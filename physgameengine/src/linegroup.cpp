//© Copyright 2010 Joseph Toppi and John Blackwood
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
#ifndef _LINEGROUP_cpp
#define _LINEGROUP_cpp

#include "linegroup.h"
#include "world.h"

#include <Ogre.h>
#include <vector>

using namespace Ogre;
using namespace std;

#define POSITION_BINDING 0
#define TEXCOORD_BINDING 1

namespace phys
{


    /// @intermal
    /// @namespace phys::internal
    /// @todo This whole internal namespace is a better place to put dirty hacks
    namespace internal
    {
        class Line3D:public SimpleRenderable
        {
            public:
               Line3D(void);
               ~Line3D(void);

               void addPoint(const Vector3 &p);
               const Vector3 &getPoint(unsigned short index) const;
               unsigned short getNumPoints(void) const;
               void updatePoint(unsigned short index, const Vector3 &value);
               void drawLine(Vector3 &start, Vector3 &end);
               void drawLines(void);

               Real getSquaredViewDepth(const Camera *cam) const;
               Real getBoundingRadius(void) const;
            protected:
               //void getWorldTransforms(Matrix4 *xform) const;
               const Ogre::Quaternion &getWorldOrientation(void) const;
               const Vector3 &getWorldPosition(void) const;

               std::vector<Vector3> mPoints;
               bool mDrawn;
        };

        Line3D::Line3D(void)
        {
           mRenderOp.vertexData = new VertexData();
           mDrawn = false;

           this->setMaterial("BaseWhiteNoLighting");
        }

        Line3D::~Line3D(void)
        {
           delete mRenderOp.vertexData;
        }

        void Line3D::addPoint(const Vector3 &p)
        {
           mPoints.push_back(p);
        }

        const Vector3 &Line3D::getPoint(unsigned short index) const
        {
           assert(index < mPoints.size() && "Point index is out of bounds!!");

           return mPoints[index];
        }

        unsigned short Line3D::getNumPoints(void) const
        {
           return (unsigned short)mPoints.size();
        }

        void Line3D::updatePoint(unsigned short index, const Vector3 &value)
        {
           assert(index < mPoints.size() && "Point index is out of bounds!!");

           mPoints[index] = value;
        }

        void Line3D::drawLine(Vector3 &start, Vector3 &end)
        {
           if(mPoints.size())
              mPoints.clear();

           mPoints.push_back(start);
           mPoints.push_back(end);

           drawLines();
        }

        void Line3D::drawLines(void)
        {
           if(mDrawn)
              return;
           else
              mDrawn = true;

           // Initialization stuff
           mRenderOp.indexData = 0;
           mRenderOp.vertexData->vertexCount = mPoints.size();
           mRenderOp.vertexData->vertexStart = 0;
           mRenderOp.operationType = RenderOperation::OT_LINE_STRIP; // OT_LINE_LIST, OT_LINE_STRIP
           mRenderOp.useIndexes = false;

           VertexDeclaration *decl = mRenderOp.vertexData->vertexDeclaration;
           VertexBufferBinding *bind = mRenderOp.vertexData->vertexBufferBinding;

           decl->addElement(POSITION_BINDING, 0, VET_FLOAT3, VES_POSITION);

           HardwareVertexBufferSharedPtr vbuf =
              HardwareBufferManager::getSingleton().createVertexBuffer(
                 decl->getVertexSize(POSITION_BINDING),
                 mRenderOp.vertexData->vertexCount,
                 HardwareBuffer::HBU_STATIC_WRITE_ONLY);

           bind->setBinding(POSITION_BINDING, vbuf);

           // Drawing stuff
           int size = mPoints.size();
           Vector3 vaabMin = mPoints[0];
           Vector3 vaabMax = mPoints[0];

           Real *prPos = static_cast<Real*>(vbuf->lock(HardwareBuffer::HBL_DISCARD));

           for(int i = 0; i < size; i++)
           {
              *prPos++ = mPoints[i].x;
              *prPos++ = mPoints[i].y;
              *prPos++ = mPoints[i].z;

              if(mPoints[i].x < vaabMin.x)
                 vaabMin.x = mPoints[i].x;
              if(mPoints[i].y < vaabMin.y)
                 vaabMin.y = mPoints[i].y;
              if(mPoints[i].z < vaabMin.z)
                 vaabMin.z = mPoints[i].z;

              if(mPoints[i].x > vaabMax.x)
                 vaabMax.x = mPoints[i].x;
              if(mPoints[i].y > vaabMax.y)
                 vaabMax.y = mPoints[i].y;
              if(mPoints[i].z > vaabMax.z)
                 vaabMax.z = mPoints[i].z;
           }

           vbuf->unlock();

           mBox.setExtents(vaabMin, vaabMax);
        }

        Real Line3D::getSquaredViewDepth(const Camera *cam) const
        {
           Vector3 vMin, vMax, vMid, vDist;
           vMin = mBox.getMinimum();
           vMax = mBox.getMaximum();
           vMid = ((vMin - vMax) * 0.5) + vMin;
           vDist = cam->getDerivedPosition() - vMid;

           return vDist.squaredLength();
        }

        Real Line3D::getBoundingRadius(void) const
        {
           return Math::Sqrt(max(mBox.getMaximum().squaredLength(), mBox.getMinimum().squaredLength()));
           //return mRadius;
        }
        /*
        void Line3D::getWorldTransforms(Matrix4 *xform) const
        {
           // return identity matrix to prevent parent transforms
           *xform = Matrix4::IDENTITY;
        }
        */
        const Ogre::Quaternion &Line3D::getWorldOrientation(void) const
        {
           return Ogre::Quaternion::IDENTITY;
        }

        const Vector3 &Line3D::getWorldPosition(void) const
        {
           return Vector3::ZERO;
        }
    }// /internal

    LineGroup::LineGroup(World *Parent_)
    {
        this->Parent = Parent_;
        this->LineData = new internal::Line3D();
    }

    LineGroup::~LineGroup(void)
    {
        delete this->LineData;
    }

    void LineGroup::addPoint(const PhysVector3 &p)
    {
        this->LineData->addPoint( p.GetOgreVector3());
    }

    const PhysVector3 LineGroup::getPoint(unsigned short index) const
    {
        PhysVector3 temp(this->LineData->getPoint(index));
        return temp;
    }

    unsigned short LineGroup::getNumPoints(void) const
    {
        return this->LineData->getNumPoints();
    }

    void LineGroup::updatePoint(unsigned short index, const PhysVector3 &value)
    {
        return this->LineData->updatePoint(index, value.GetOgreVector3());
    }

    void LineGroup::drawLine(const PhysVector3 &start, const PhysVector3 &end)
    {
        Ogre::Vector3 Ostart = start.GetOgreVector3();
        Ogre::Vector3 Oend = end.GetOgreVector3();
        this->LineData->drawLine(Ostart, Oend );
    }

    void LineGroup::drawLines(void)
    {
        this->LineData->drawLines();

        SceneNode *myNode = this->Parent->OgreSceneManager->getRootSceneNode()->createChildSceneNode();
        myNode->attachObject(this->LineData);
    }

    Real LineGroup::getBoundingRadius(void) const
    {
        return this->LineData->getBoundingRadius();
    }

}














#endif

