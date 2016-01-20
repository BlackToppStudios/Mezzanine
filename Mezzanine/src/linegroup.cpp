// © Copyright 2010 - 2015 BlackTopp Studios Inc.
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
#ifndef _LINEGROUP_cpp
#define _LINEGROUP_cpp

#include "linegroup.h"
#include "Graphics/scenemanager.h"
#include "entresol.h"
#include "world.h"

#include <Ogre.h>
#include <vector>

namespace Mezzanine
{

///////////////////////////////////////////////////////////////////////////////
// Mezzanine::Internal::Line3D
///////////////////////////////////////

    //We need to decalare an internal class to make this work.
    namespace Internal
    {
        /// @internal
        /// @brief A small declaration of what our vertexs look like for when we write to the buffers.
        struct LineVertex
        {
            /// @internal
            /// @brief Vertex position.
            Vector3 Position;
            /// @internal
            /// @brief Vertex colour.
            ColourValue Colour;

            /// @internal
            /// @brief Class constructor.
            /// @param Pos The position to be assigned to this vertex.
            /// @param Col The colour of this vertex.
            LineVertex(const Vector3& Pos, const ColourValue& Col)
                : Position(Pos), Colour(Col) {  }
            /// @internal
            /// @brief Class destructor.
            ~LineVertex() {  }
        };//LineVertex

        /// @internal
        /// @brief Does the bulk of the work that that the Mezzanine::LineGroup performs
        /// @details Mezzanine::LineGroup is a simple wrapper around this to perform precise
        /// low level interactions with Ogre, the rendering subsystem. This uses too much stuff
        /// from ogre to use publicly. so we need to hide it here in the Mezzanine::internal namespace.
        class Line3D : public Ogre::SimpleRenderable
        {
        protected:
            /// @internal
            /// @brief Gets how rotated this is currently
            /// @details Returns a quaternion with the rotation
            /// @return Is a Ogre::Quaternion which stores the rotation information of this Line3D
            const Ogre::Quaternion getWorldOrientation(void) const;
            /// @internal
            /// @brief Get the position of this Line3d
            /// @return This returns a Ogre::Vector3 with the Position relative to the world Origin
            const Ogre::Vector3 getWorldPosition(void) const;
            /// @internal
            /// @brief Resizes the Vertex Buffer.
            /// @note The Vertex Buffer will not shrink, only grow.  Passing in a smaller size will do nothing.
            /// @param RequestedSize The new size for the existing buffer.
            void ResizeVertexBuffer(const Whole RequestedSize);

            /// @internal
            /// @brief This is a vector which stores the point data
            std::vector<LineVertex> Points;
            /// @internal
            /// @brief Internal pointer to the vertex buffer for this object.
            Ogre::HardwareVertexBufferSharedPtr VertexBuffer;
            /// @internal
            /// @brief Pointer to the node that will be used exclusively for this renderable.
            Ogre::SceneNode* SelfNode;
            /// @brief Parent World to locate SceneManager
            World * ParentWorld;
        public:
            /// @internal
            /// @brief Default Constructor
            Line3D(World * ParentWorld);
            /// @internal
            /// @brief Destructor
            ~Line3D();

            /// @internal
            /// @brief This adds a point to the list of what should be rendered.
            /// @param NewPoint The point to be added.
            void AddPoint(const Vector3& NewPoint, const ColourValue& Colour);
            /// @internal
            /// @brief Access a specific point by index
            /// @param index The index.
            /// @details This really does just access the underlying vector.
            /// @return This Returns the specific Vector3 requested.
            const Vector3& GetPoint(const Whole Index) const;
            /// @internal
            /// @brief How many points are in this Line3D.
            /// @return This returns the amount of points stored in this class.
            Whole GetNumPoints() const;
            /// @internal
            /// @brief This changes a specific point.
            /// @details This replaces a point specified by index with a new point.
            /// @param Index The index of the point to replace.
            /// @param NewValue A point to replace the existing point with.
            void UpdatePoint(const Whole Index, const Vector3& NewValue);
            /// @brief Clears all data pertaining to points in this line group.
            void ClearPoints();

            /// @internal
            /// @brief This adds Two points to the list.
            /// @param Start The first point to be added.
            /// @param End The second point to be added.
            /// @param Colour The colour of the line being added.
            void DrawLine(const Vector3& Start, const Vector3& End, const ColourValue& Colour);
            /// @internal
            /// @brief Updates the internal buffers as necessary for rendering.
            void DrawLines();

            /// @internal
            /// @brief Not Used
            /// @param Cam Not Used
            /// @return This returns a Real.
            Real getSquaredViewDepth(const Ogre::Camera* Cam) const;
            /// @internal
            /// @brief How big would a circle need to be to encapsulate this
            /// @details This returns the radius the a circle would need to have to surround this line group.
            /// @return This returns a real number which indicates the radius.
            Real getBoundingRadius(void) const;

            /// @internal
            /// @brief Retrieves the scene node that will be used to attach this object to the scenegraph.
            /// @return Returns a pointer to this LineDatas scenenode.
            Ogre::SceneNode* GetNode() const;
        };//Line3D

        Line3D::Line3D(World * ParentWorldArg) 
            : ParentWorld(ParentWorldArg)
        {
            mRenderOp.vertexData = new Ogre::VertexData();
            this->SelfNode = this->ParentWorld->GetSceneManager()->_GetGraphicsWorldPointer()->getRootSceneNode()->createChildSceneNode();

            // Initialization stuff
            mRenderOp.indexData = 0;
            mRenderOp.vertexData->vertexCount = 1024;
            mRenderOp.vertexData->vertexStart = 0;
            mRenderOp.operationType = Ogre::RenderOperation::OT_LINE_LIST;
            mRenderOp.useIndexes = false;

            Ogre::VertexDeclaration* VDecl = mRenderOp.vertexData->vertexDeclaration;
            Ogre::VertexBufferBinding* VBind = mRenderOp.vertexData->vertexBufferBinding;

            size_t Offset = 0;
            // Position.
            VDecl->addElement(0,0,Ogre::VET_FLOAT3,Ogre::VES_POSITION);
            Offset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT3);

            // Colour
            VDecl->addElement(0,Offset,Ogre::VET_FLOAT4,Ogre::VES_DIFFUSE);
            Offset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT4);

            this->VertexBuffer = Ogre::HardwareBufferManager::getSingleton().createVertexBuffer(
                    VDecl->getVertexSize(0),
                    mRenderOp.vertexData->vertexCount,
                    Ogre::HardwareBuffer::HBU_DYNAMIC_WRITE_ONLY_DISCARDABLE,
                    false);

            VBind->setBinding(0,this->VertexBuffer);

            this->setMaterial("BaseWhiteNoLighting");
        }

        Line3D::~Line3D()
        {
            this->ParentWorld->GetSceneManager()->_GetGraphicsWorldPointer()->destroySceneNode(this->SelfNode);
            delete mRenderOp.vertexData;
        }

        const Ogre::Quaternion Line3D::getWorldOrientation() const
            { return this->SelfNode->_getDerivedOrientation(); }

        const Ogre::Vector3 Line3D::getWorldPosition() const
            { return this->SelfNode->_getDerivedPosition(); }

        void Line3D::ResizeVertexBuffer(const Whole RequestedSize)
        {
            if(RequestedSize > this->VertexBuffer->getNumVertices() ) {
                // Update only by powers of 2
                Whole NewVertexBufferSize = 1;
                while(NewVertexBufferSize < RequestedSize)
                    NewVertexBufferSize <<= 1;

                this->VertexBuffer = Ogre::HardwareBufferManager::getSingletonPtr()->createVertexBuffer(
                    this->mRenderOp.vertexData->vertexDeclaration->getVertexSize(0),
                    NewVertexBufferSize,
                    Ogre::HardwareBuffer::HBU_DYNAMIC_WRITE_ONLY_DISCARDABLE,
                    false);

                this->mRenderOp.vertexData->vertexStart = 0;
                this->mRenderOp.vertexData->vertexBufferBinding->setBinding(0,this->VertexBuffer);
            }
        }

        void Line3D::AddPoint(const Vector3& NewPoint, const ColourValue& Colour)
        {
            this->Points.push_back( LineVertex(NewPoint,Colour) );
        }

        const Vector3& Line3D::GetPoint(const Whole Index) const
        {
            assert(Index < this->Points.size() && "Point index is out of bounds!!");

            return this->Points[Index].Position;
        }

        Whole Line3D::GetNumPoints() const
        {
            return this->Points.size();
        }

        void Line3D::UpdatePoint(const Whole Index, const Vector3& NewValue)
        {
            assert(Index < this->Points.size() && "Point index is out of bounds!!");

            this->Points[Index].Position = NewValue;
        }

        void Line3D::ClearPoints()
        {
            this->Points.clear();
        }

        void Line3D::DrawLine(const Vector3& Start, const Vector3& End, const ColourValue& Colour)
        {
            this->Points.push_back( LineVertex(Start,Colour) );
            this->Points.push_back( LineVertex(End,Colour) );
        }

        void Line3D::DrawLines()
        {
            // Drawing stuff
            Whole NumPoints = this->Points.size();
            if( NumPoints > 0 ) {
                this->ResizeVertexBuffer(NumPoints);
                /*if( mRenderOp.vertexData->vertexCount < NumPoints ) {
                    Whole NewVertexBufferSize = 1;
                    while(NewVertexBufferSize < NumPoints)
                        NewVertexBufferSize <<= 1;

                    this->VertexBuffer = Ogre::HardwareBufferManager::getSingleton().createVertexBuffer(
                        mRenderOp.vertexData->vertexDeclaration->getVertexSize(0),
                        NewVertexBufferSize,
                        Ogre::HardwareBuffer::HBU_DYNAMIC_WRITE_ONLY_DISCARDABLE,
                        false);

                    mRenderOp.vertexData->vertexStart = 0;
                    mRenderOp.vertexData->vertexBufferBinding->setBinding(0,this->VertexBuffer);
                }// */

                Vector3 vaabMin = this->Points[0].Position;
                Vector3 vaabMax = this->Points[0].Position;

                Real* prPos = static_cast<Real*>(this->VertexBuffer->lock(Ogre::HardwareBuffer::HBL_DISCARD));
                for( Whole Index = 0 ; Index < NumPoints ; ++Index )
                {
                    const LineVertex& CurrVertex = this->Points.at(Index);

                    // Position assignment
                    *prPos++ = CurrVertex.Position.X;
                    *prPos++ = CurrVertex.Position.Y;
                    *prPos++ = CurrVertex.Position.Z;

                    // Colour assignment
                    *prPos++ = CurrVertex.Colour.RedChannel;
                    *prPos++ = CurrVertex.Colour.GreenChannel;
                    *prPos++ = CurrVertex.Colour.BlueChannel;
                    *prPos++ = CurrVertex.Colour.AlphaChannel;

                    // Bounds checking
                    if(CurrVertex.Position.X < vaabMin.X)
                        vaabMin.X = CurrVertex.Position.X;
                    if(CurrVertex.Position.Y < vaabMin.Y)
                        vaabMin.Y = CurrVertex.Position.Y;
                    if(CurrVertex.Position.Z < vaabMin.Z)
                        vaabMin.Z = CurrVertex.Position.Z;

                    if(CurrVertex.Position.X > vaabMax.X)
                        vaabMax.X = CurrVertex.Position.X;
                    if(CurrVertex.Position.Y > vaabMax.Y)
                        vaabMax.Y = CurrVertex.Position.Y;
                    if(CurrVertex.Position.Z > vaabMax.Z)
                        vaabMax.Z = CurrVertex.Position.Z;
                }
                this->VertexBuffer->unlock();
                this->mRenderOp.vertexData->vertexCount = NumPoints;

                mBox.setExtents(vaabMin.GetOgreVector3(), vaabMax.GetOgreVector3());
            }
        }

        Real Line3D::getSquaredViewDepth(const Ogre::Camera* Cam) const
        {
           Ogre::Vector3 vMin, vMax, vMid, vDist;
           vMin = mBox.getMinimum();
           vMax = mBox.getMaximum();
           vMid = ((vMin - vMax) * 0.5) + vMin;
           vDist = Cam->getDerivedPosition() - vMid;

           return vDist.squaredLength();
        }

        Real Line3D::getBoundingRadius() const
            { return Ogre::Math::Sqrt(std::max(mBox.getMaximum().squaredLength(), mBox.getMinimum().squaredLength())); }

        Ogre::SceneNode* Line3D::GetNode() const
            { return this->SelfNode; }
    }// /internal


    ///////////////////////////////////////////////////////////////////////////////
    // Mezzanine::LineGroup

    LineGroup::LineGroup(World * ParentWorld)
        : ParentWorld(ParentWorld)
    {
        this->LineData = new Internal::Line3D(this->ParentWorld);
    }

    LineGroup::~LineGroup(void)
    {
        this->RemoveFromWorld();
        delete this->LineData;
    }

    void LineGroup::AddPoint(const Vector3& NewPoint, const ColourValue& Colour)
        { this->LineData->AddPoint(NewPoint,Colour); }

    const Vector3 LineGroup::GetPoint(const Whole Index) const
        { return Vector3(this->LineData->GetPoint(Index)); }

    Whole LineGroup::GetNumPoints(void) const
        { return this->LineData->GetNumPoints(); }

    void LineGroup::UpdatePoint(const Whole Index, const Vector3& NewValue)
        { return this->LineData->UpdatePoint(Index,NewValue); }

    void LineGroup::ClearLines()
        { this->LineData->ClearPoints(); }

    void LineGroup::DrawLine(const Vector3& Start, const Vector3& End, const ColourValue& Colour)
        { this->LineData->DrawLine(Start,End,Colour); }

    void LineGroup::DrawLines()
        { this->LineData->DrawLines(); }

    void LineGroup::AddToWorld()
    {
        if( this->LineData->isAttached() == false )
            this->LineData->GetNode()->attachObject(this->LineData);
    }

    void LineGroup::RemoveFromWorld()
    {
        if( this->LineData->isAttached() == true )
            this->LineData->GetNode()->detachObject(this->LineData);
    }

    Real LineGroup::GetBoundingRadius() const
    {
        return this->LineData->getBoundingRadius();
    }
}

#endif
