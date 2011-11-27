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
#ifndef _LINEGROUP_cpp
#define _LINEGROUP_cpp

#include "linegroup.h"
#include "world.h"

#include <Ogre.h>
#include <vector>

using namespace std;

#define POSITION_BINDING 0
#define TEXCOORD_BINDING 1

namespace phys
{

///////////////////////////////////////////////////////////////////////////////
// phys::internal::Line3D
///////////////////////////////////////

    //We need to decalare an internal class to make this work.
    namespace internal
    {
        /// @internal
        /// @brief Does the bulk of the work that that the phys::LineGroup performs
        /// @details phys::LineGroup is a simple wrapper around this to perform precise
        /// low level interactions with Ogre, the rendering subsystem. This uses too much stuff
        /// from ogre to use publicly. so we need to hide it here in the phys::internal namespace.
        class Line3D: public Ogre::SimpleRenderable
        {
            public:
                /// @internal
                /// @brief Default Constructor
                /// @details This creates an empty Line3D.
                Line3D(void);

                /// @internal
                /// @brief Destructor
                /// @details This safely tears down the Line3D.
                ~Line3D(void);

                /// @internal
                /// @brief This adds a point to the list of what should be rendered.
                /// @param p The point to be added.
                void addPoint(const Vector3 &p);

                /// @internal
                /// @brief Access a specific point by index
                /// @param index The index.
                /// @details This really does just access the underlying vector.
                /// @return This Returns the specific Vector3 requested.
                const Vector3 &getPoint(Whole index) const;

                /// @internal
                /// @brief How many points are in this Line3D.
                /// @return This returns the amount of points stored in this class.
                Whole getNumPoints(void) const;

                /// @internal
                /// @brief Change an existing point.
                /// @details This replaces a point specified by index with a new point
                /// @param index The index of the point to replace.
                /// @param value A point to replace the existing point with
                void updatePoint(Whole index, const Vector3 &value);

                /// @internal
                /// @brief Adds two points
                /// @details This adds to points, to guarantee that a specific line segment is drawn.
                /// @param start The first point to be added
                /// @param end The first point to be added
                void drawLine(const Vector3 &start, const Vector3 &end);

                /// @internal
                /// @brief Renders this
                /// @details This does the actual rendering.
                void drawLines(void);        //Render this

                /// @internal
                /// @brief Not Used
                /// @details Not Used
                /// @param cam Not Used
                /// @return This returns a Real.
                Real getSquaredViewDepth(const Ogre::Camera *cam) const;

                /// @internal
                /// @brief How big would a circle need to be to encapsulate this
                /// @details This returns the radius the a circle would need to have to surround this line group.
                /// @return This returns a real number which indicates the radius.
                Real getBoundingRadius(void) const;
            protected:
                //void getWorldTransforms(Matrix4 *xform) const;
                /// @internal
                /// @brief Gets how rotated this is currently
                /// @details Returns a quaternion with the rotation
                /// @return Is a Ogre::Quaternion which stores the rotation information of this Line3D
                const Ogre::Quaternion &getWorldOrientation(void) const;

                /// @internal
                /// @brief Get the position of this Line3d
                /// @return This returns a Vector3 with the Position relative to the world Origin
                const Vector3 &getWorldPosition(void) const;

                /// @internal
                /// @brief This is a vector which stores the point data
                std::vector<Vector3> mPoints;

                /// @internal
                /// @brief This indicates whether or not the the line have been done yet.
                bool mDrawn;     //Has this been rendered yet?
        };

        Line3D::Line3D(void)
        {
           mRenderOp.vertexData = new Ogre::VertexData();
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

        const Vector3 &Line3D::getPoint(Whole index) const
        {
           assert(index < mPoints.size() && "Point index is out of bounds!!");

           return mPoints[index];
        }

        Whole Line3D::getNumPoints(void) const
        {
           return mPoints.size();
        }

        void Line3D::updatePoint(Whole index, const Vector3 &value)
        {
           assert(index < mPoints.size() && "Point index is out of bounds!!");

           mPoints[index] = value;
        }

        void Line3D::drawLine(const Vector3 &start, const Vector3 &end)
        {
           /// @todo TODO: when using this function there should be a break in the line segment rendering. Not sure abot the best way to implement that, but it should happen
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
            mRenderOp.operationType = Ogre::RenderOperation::OT_LINE_STRIP; // OT_LINE_LIST, OT_LINE_STRIP
            mRenderOp.useIndexes = false;

            Ogre::VertexDeclaration *decl = mRenderOp.vertexData->vertexDeclaration;
            Ogre::VertexBufferBinding *bind = mRenderOp.vertexData->vertexBufferBinding;

            decl->addElement(POSITION_BINDING, 0, Ogre::VET_FLOAT3, Ogre::VES_POSITION);

            Ogre::HardwareVertexBufferSharedPtr vbuf =
              Ogre::HardwareBufferManager::getSingleton().createVertexBuffer(
                 decl->getVertexSize(POSITION_BINDING),
                 mRenderOp.vertexData->vertexCount,
                 Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY);

            bind->setBinding(POSITION_BINDING, vbuf);

            // Drawing stuff
            int size = mPoints.size();
            if( size > 0 )
            {
               Vector3 vaabMin = mPoints[0];
               Vector3 vaabMax = mPoints[0];

               Real *prPos = static_cast<Real*>(vbuf->lock(Ogre::HardwareBuffer::HBL_DISCARD));

               for(int i = 0; i < size; i++)
               {
                  *prPos++ = mPoints[i].X;
                  *prPos++ = mPoints[i].Y;
                  *prPos++ = mPoints[i].Z;

                  if(mPoints[i].X < vaabMin.X)
                     vaabMin.X = mPoints[i].X;
                  if(mPoints[i].Y < vaabMin.Y)
                     vaabMin.Y = mPoints[i].Y;
                  if(mPoints[i].Z < vaabMin.Z)
                     vaabMin.Z = mPoints[i].Z;

                  if(mPoints[i].X > vaabMax.X)
                     vaabMax.X = mPoints[i].X;
                  if(mPoints[i].Y > vaabMax.Y)
                     vaabMax.Y = mPoints[i].Y;
                  if(mPoints[i].Z > vaabMax.Z)
                     vaabMax.Z = mPoints[i].Z;
               }

               vbuf->unlock();

               mBox.setExtents(vaabMin.GetOgreVector3(), vaabMax.GetOgreVector3());
            }

        }

        Real Line3D::getSquaredViewDepth(const Ogre::Camera *cam) const
        {
           Ogre::Vector3 vMin, vMax, vMid, vDist;
           vMin = mBox.getMinimum();
           vMax = mBox.getMaximum();
           vMid = ((vMin - vMax) * 0.5) + vMin;
           vDist = cam->getDerivedPosition() - vMid;

           return vDist.squaredLength();
        }

        Real Line3D::getBoundingRadius(void) const
        {
           return Ogre::Math::Sqrt(max(mBox.getMaximum().squaredLength(), mBox.getMinimum().squaredLength()));
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

        /*const Vector3 &Line3D::getWorldPosition(void) const
        {
           return Vector3::ZERO;
        }*/
    }// /internal


///////////////////////////////////////////////////////////////////////////////
// phys::LineGroup
///////////////////////////////////////
    LineGroup::LineGroup(World *Parent_)
    {
        this->Parent = Parent_;
        this->LineData = new internal::Line3D();
    }

    LineGroup::~LineGroup(void)
    {
        delete this->LineData;
    }

    void LineGroup::addPoint(const Vector3 &p)
    {
        this->LineData->addPoint( p);
    }

    const Vector3 LineGroup::getPoint(Whole index) const
    {
        Vector3 temp(this->LineData->getPoint(index));
        return temp;
    }

    Whole LineGroup::getNumPoints(void) const
    {
        return this->LineData->getNumPoints();
    }

    void LineGroup::updatePoint(Whole index, const Vector3 &value)
    {
        return this->LineData->updatePoint(index, value);
    }

    void LineGroup::drawLine(const Vector3 &start, const Vector3 &end)
    {
        //Ogre::Vector3 Ostart = start.GetOgreVector3();
        //Ogre::Vector3 Oend = end.GetOgreVector3();
        this->LineData->drawLine(start, end);
    }

    void LineGroup::drawLines(void)
    {
        this->LineData->drawLines();
    }

    Real LineGroup::getBoundingRadius(void) const
    {
        return this->LineData->getBoundingRadius();
    }

    void LineGroup::PrepareForRendering()
    {
        Ogre::SceneNode *myNode = SceneManager::GetSingletonPtr()->GetGraphicsWorldPointer()->getRootSceneNode()->createChildSceneNode();
        myNode->attachObject(this->LineData);
    }
}

#endif
