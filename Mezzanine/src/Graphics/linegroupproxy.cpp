// © Copyright 2010 - 2016 BlackTopp Studios Inc.
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
#ifndef _graphicslinegroupproxy_cpp
#define _graphicslinegroupproxy_cpp

#include "linegroupproxy.h"
#include "Graphics/scenemanager.h"

#include <Ogre.h>
#include <vector>

namespace Mezzanine
{
    namespace Graphics
    {
        ///////////////////////////////////////////////////////////////////////////////
        /// @brief Does the bulk of the work that that the LineGroupProxy performs.
        ///////////////////////////////////////
        class Line3D : public Ogre::SimpleRenderable
        {
        public:
            /// @brief Convenience type for the container storing vertices to be rendered.
            using LineVertexContainer = std::vector<LineVertex>;
            /// @brief Iterator type for stored vertices in this class.
            using LineVertexIterator = LineVertexContainer::iterator;
            /// @brief Const Iterator type for stored vertices in this class.
            using ConstLineVertexIterator = LineVertexContainer::const_iterator;
        protected:
            /// @brief Resizes the Vertex Buffer.
            /// @remarks The Vertex Buffer will not shrink, only grow.  Passing in a smaller size will do nothing.
            /// @param RequestedSize The new size for the existing buffer.
            void ResizeVertexBuffer(const Whole RequestedSize);

            /// @brief This is a vector which stores the point data
            LineVertexContainer Points;
            /// @brief Internal pointer to the vertex buffer for this object.
            Ogre::HardwareVertexBufferSharedPtr VertexBuffer;
        public:
            /// @brief Default Constructor
            Line3D();
            /// @brief Copy constructor.
            /// @param Other The other Line3D to be copied.
            Line3D(const Line3D& Other) = delete;
            /// @brief Move constructor.
            /// @param Other The other Line3D to be moved.
            Line3D(Line3D&& Other) = delete;
            /// @brief Destructor
            virtual ~Line3D();

            ///////////////////////////////////////////////////////////////////////////////
            // Operators

            /// @brief Assignment operator.
            /// @param Other The other Line3D to be copied.
            /// @return Returns a reference to this.
            Line3D& operator=(const Line3D& Other) = delete;
            /// @brief Move assignment operator.
            /// @param Other The other Line3D to be moved.
            /// @return Returns a reference to this.
            Line3D& operator=(Line3D&& Other) = delete;

            ///////////////////////////////////////////////////////////////////////////////
            // Point Management

            /// @brief This adds a point to the list of what should be rendered.
            /// @note The renderer will assemble the lines with the assumption that the points are defined in pairs.
            /// For example, indexes 0 and 1 are a line.  Indexes 2 and 3 are another.  But indexes 1 and 2 are NOT a
            /// line, and will not connect.  Be mindful of this when altering individual points.
            /// @param Location The point to be added.
            /// @param Colour The colour of the point.  The colour will be used to determine the line colour.
            void AddPoint(const Vector3& Location, const ColourValue& Colour);
            /// @brief This adds Two points to the list.
            /// @param Start The first point to be added.
            /// @param End The second point to be added.
            /// @param Colour The colour of the line being added.
            void AddLine(const Vector3& Start, const Vector3& End, const ColourValue& Colour);
            /// @brief Updates the vertex information of an existing point at a specific index.
            /// @param Index The index of the point to update.
            /// @param Location The updated location of the point at the specified index.
            void UpdatePoint(const Whole Index, const Vector3& Location);
            /// @brief Updates the vertex information of an existing point at a specific index.
            /// @param Index The index of the point to update.
            /// @param Location The updated location of the point at the specified index.
            /// @param Colour The colour of the updated point.  The colour will be used to determine the line colour.
            void UpdatePoint(const Whole Index, const Vector3& Location, const ColourValue& Colour);

            /// @brief Gets the number of points in this line grouping.
            /// @return This returns the amount of points that compose the line(s) being generated.
            Whole GetNumPoints() const;
            /// @brief Gets the position of the point at the specified index.
            /// @param Index The index of the point to retrieve.
            /// @return Returns the location of the point at the specified index.
            const Vector3& GetPointLocation(const Whole Index) const;
            /// @brief Gets the colour of the point at the specified index.
            /// @param Index The index of the point to retrieve.
            /// @return Returns the colour of the point at specified index.
            const ColourValue& GetPointColour(const Whole Index) const;
            /// @brief Gets the vertex of the point at the specified index.
            /// @param Index The index of the point to retrieve.
            /// @return Returns the vertex specified index.
            const LineVertex& GetPoint(const Whole Index) const;

            /// @brief Reserves space for a specific number of points.
            /// @param NumPoints The number of points to reserve space for.
            void ReservePoints(const Whole NumPoints);
            /// @brief Removes all points from this line grouping.
            void ClearPoints();
            /// @brief Updates the internal buffers as necessary for rendering.
            /// @note This must be called after any updates are done to the line configuration in order to see the changes.
            void UpdateBuffers();

            ///////////////////////////////////////////////////////////////////////////////
            // Compatibility Methods

            /// @brief Not Used
            /// @param Cam Not Used
            /// @return This returns a Real.
            Real getSquaredViewDepth(const Ogre::Camera* Cam) const override;
            /// @brief How big would a circle need to be to encapsulate this
            /// @details This returns the radius the a circle would need to have to surround this line group.
            /// @return This returns a real number which indicates the radius.
            Real getBoundingRadius(void) const override;
        };//Line3D

        ///////////////////////////////////////////////////////////////////////////////
        // Construction and Destruction

        Line3D::Line3D()
        {
            mRenderOp.vertexData = OGRE_NEW Ogre::VertexData();

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
            { OGRE_DELETE mRenderOp.vertexData; }

        ///////////////////////////////////////////////////////////////////////////////
        // Protected Methods

        void Line3D::ResizeVertexBuffer(const Whole RequestedSize)
        {
            if( RequestedSize > this->VertexBuffer->getNumVertices() ) {
                // Update only by powers of 2
                Whole NewVertexBufferSize = 1;
                while( NewVertexBufferSize < RequestedSize )
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

        ///////////////////////////////////////////////////////////////////////////////
        // Operators

        ///////////////////////////////////////////////////////////////////////////////
        // Point Management

        void Line3D::AddPoint(const Vector3& Location, const ColourValue& Colour)
        {
            this->Points.emplace_back( Location, Colour );
        }

        void Line3D::AddLine(const Vector3& Start, const Vector3& End, const ColourValue& Colour)
        {
            this->Points.emplace_back( Start, Colour );
            this->Points.emplace_back( End, Colour );
        }

        void Line3D::UpdatePoint(const Whole Index, const Vector3& Location)
        {
            assert(Index < this->Points.size() && "Point index is out of bounds!!");
            this->Points[Index].Location = Location;
        }

        void Line3D::UpdatePoint(const Whole Index, const Vector3& Location, const ColourValue& Colour)
        {
            assert(Index < this->Points.size() && "Point index is out of bounds!!");
            this->Points[Index].Location = Location;
            this->Points[Index].Colour = Colour;
        }

        Whole Line3D::GetNumPoints() const
        {
            return this->Points.size();
        }

        const Vector3& Line3D::GetPointLocation(const Whole Index) const
        {
            assert(Index < this->Points.size() && "Point index is out of bounds!!");
            return this->Points[Index].Location;
        }

        const ColourValue& Line3D::GetPointColour(const Whole Index) const
        {
            assert(Index < this->Points.size() && "Point index is out of bounds!!");
            return this->Points[Index].Colour;
        }

        const LineVertex& Line3D::GetPoint(const Whole Index) const
        {
            assert(Index < this->Points.size() && "Point index is out of bounds!!");
            return this->Points[Index];
        }

        void Line3D::ReservePoints(const Whole NumPoints)
        {
            this->Points.reserve(NumPoints);
        }

        void Line3D::ClearPoints()
        {
            this->Points.clear();
        }

        void Line3D::UpdateBuffers()
        {
            // Drawing stuff
            Whole NumPoints = this->Points.size();
            if( NumPoints > 0 ) {
                this->ResizeVertexBuffer(NumPoints);

                Vector3 vaabMin = this->Points[0].Location;
                Vector3 vaabMax = this->Points[0].Location;

                Real* prPos = static_cast<Real*>(this->VertexBuffer->lock(Ogre::HardwareBuffer::HBL_DISCARD));
                for( Whole Index = 0 ; Index < NumPoints ; ++Index )
                {
                    const LineVertex& CurrVertex = this->Points[Index];

                    // Location assignment
                    *prPos++ = CurrVertex.Location.X;
                    *prPos++ = CurrVertex.Location.Y;
                    *prPos++ = CurrVertex.Location.Z;

                    // Colour assignment
                    *prPos++ = CurrVertex.Colour.RedChannel;
                    *prPos++ = CurrVertex.Colour.GreenChannel;
                    *prPos++ = CurrVertex.Colour.BlueChannel;
                    *prPos++ = CurrVertex.Colour.AlphaChannel;

                    // Bounds checking
                    if(CurrVertex.Location.X < vaabMin.X)
                        vaabMin.X = CurrVertex.Location.X;
                    if(CurrVertex.Location.Y < vaabMin.Y)
                        vaabMin.Y = CurrVertex.Location.Y;
                    if(CurrVertex.Location.Z < vaabMin.Z)
                        vaabMin.Z = CurrVertex.Location.Z;

                    if(CurrVertex.Location.X > vaabMax.X)
                        vaabMax.X = CurrVertex.Location.X;
                    if(CurrVertex.Location.Y > vaabMax.Y)
                        vaabMax.Y = CurrVertex.Location.Y;
                    if(CurrVertex.Location.Z > vaabMax.Z)
                        vaabMax.Z = CurrVertex.Location.Z;
                }
                this->VertexBuffer->unlock();
                this->mRenderOp.vertexData->vertexCount = NumPoints;

                mBox.setExtents(vaabMin.GetOgreVector3(), vaabMax.GetOgreVector3());
            }
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Compatibility Methods

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

        ///////////////////////////////////////////////////////////////////////////////
        // LineGroup Methods

        LineGroupProxy::LineGroupProxy(const UInt32 ID, SceneManager* Creator) :
            RenderableProxy(Creator),
            GraphicsLineData(NULL)
            { this->CreateLineGroup(); }

        LineGroupProxy::LineGroupProxy(const XML::Node& SelfRoot, SceneManager* Creator) :
            RenderableProxy(Creator),
            GraphicsLineData(NULL)
        {
            this->CreateLineGroup();
            this->ProtoDeSerialize(SelfRoot);
        }

        LineGroupProxy::~LineGroupProxy()
            { this->DestroyLineGroup(); }

        void LineGroupProxy::CreateLineGroup()
        {
            this->GraphicsLineData = new Line3D();
            this->GraphicsNode->attachObject( this->GraphicsLineData );
            this->GraphicsLineData->MovableObject::getUserObjectBindings().setUserAny( Ogre::Any( static_cast<RenderableProxy*>( this ) ) );
            this->GraphicsLineData->setVisibilityFlags(0);
            this->GraphicsLineData->setQueryFlags(0);
        }

        void LineGroupProxy::DestroyLineGroup()
        {
            if( this->GraphicsLineData ) {
                this->GraphicsNode->detachObject( this->GraphicsLineData );
                delete this->GraphicsLineData;
            }
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        Mezzanine::ProxyType LineGroupProxy::GetProxyType() const
            { return Mezzanine::PT_Graphics_LineGroupProxy; }

        Boole LineGroupProxy::IsStatic() const
            { return false; }

        ///////////////////////////////////////////////////////////////////////////////
        // Point Management

        void LineGroupProxy::AddPoint(const Vector3& Location, const ColourValue& Colour)
            { this->GraphicsLineData->AddPoint(Location,Colour); }

        void LineGroupProxy::AddLine(const Vector3& Start, const Vector3& End, const ColourValue& Colour)
            { this->GraphicsLineData->AddLine(Start,End,Colour); }

        void LineGroupProxy::UpdatePoint(const Whole Index, const Vector3& Location)
            { this->GraphicsLineData->UpdatePoint(Index,Location); }

        void LineGroupProxy::UpdatePoint(const Whole Index, const Vector3& Location, const ColourValue& Colour)
            { this->GraphicsLineData->UpdatePoint(Index,Location,Colour); }

        Whole LineGroupProxy::GetNumPoints() const
            { return this->GraphicsLineData->GetNumPoints(); }

        const Vector3& LineGroupProxy::GetPointLocation(const Whole Index) const
            { return this->GraphicsLineData->GetPointLocation(Index); }

        const ColourValue& LineGroupProxy::GetPointColour(const Whole Index) const
            { return this->GraphicsLineData->GetPointColour(Index); }

        const LineVertex& LineGroupProxy::GetPoint(const Whole Index) const
            { return this->GraphicsLineData->GetPoint(Index); }

        void LineGroupProxy::ClearPoints()
            { this->GraphicsLineData->ClearPoints(); }

        void LineGroupProxy::UpdateBuffers()
            { this->GraphicsLineData->UpdateBuffers(); }

        ///////////////////////////////////////////////////////////////////////////////
        // Serialization

        void LineGroupProxy::ProtoSerializeProperties(XML::Node& SelfRoot) const
        {
            this->RenderableProxy::ProtoSerializeProperties(SelfRoot);

            XML::Node PropertiesNode = SelfRoot.AppendChild( LineGroupProxy::GetSerializableName() + "Properties" );

            if( PropertiesNode.AppendAttribute("Version").SetValue("1") )
            {
                XML::Node PointsNode = PropertiesNode.AppendChild("Points");
                if( PointsNode.AppendAttribute("PointCount").SetValue( this->GetNumPoints() ) ) {
                    for( Whole CurrPoint = 0 ; CurrPoint < this->GetNumPoints() ; ++CurrPoint )
                    {
                        XML::Node PointNode = PointsNode.AppendChild("Point");
                        this->GetPointLocation(CurrPoint).ProtoSerialize(PointNode);
                        this->GetPointColour(CurrPoint).ProtoSerialize(PointNode);
                    }
                }else{
                    SerializeError("Create XML Attribute Values",LineGroupProxy::GetSerializableName() + "PointsNode",true);
                }

                return;
            }else{
                SerializeError("Create XML Attribute Values",LineGroupProxy::GetSerializableName() + "Properties",true);
            }
        }

        void LineGroupProxy::ProtoDeSerializeProperties(const XML::Node& SelfRoot)
        {
            this->RenderableProxy::ProtoDeSerializeProperties(SelfRoot);
            this->ClearPoints();

            XML::Attribute CurrAttrib;
            XML::Node PropertiesNode = SelfRoot.GetChild( LineGroupProxy::GetSerializableName() + "Properties" );

            if( !PropertiesNode.Empty() ) {
                if(PropertiesNode.GetAttribute("Version").AsInt() == 1) {
                    XML::Node PointsNode = PropertiesNode.GetChild("PointsNode");
                    CurrAttrib = PropertiesNode.GetAttribute("PointCount");
                    if( !CurrAttrib.Empty() )
                        this->GraphicsLineData->ReservePoints( CurrAttrib.AsWhole() );

                    for( XML::NodeIterator PointNodeIt = PointsNode.begin() ; PointNodeIt != PointsNode.end() ; ++PointNodeIt )
                    {
                        Vector3 PointLocation( (*PointNodeIt).GetChild("Vector3") );
                        ColourValue PointColour( (*PointNodeIt).GetChild("ColourValue") );

                        this->AddPoint(PointLocation,PointColour);
                    }

                    this->UpdateBuffers();
                }else{
                    MEZZ_EXCEPTION(ExceptionBase::INVALID_VERSION_EXCEPTION,"Incompatible XML Version for " + (LineGroupProxy::GetSerializableName() + "Properties" ) + ": Not Version 1.");
                }
            }else{
                MEZZ_EXCEPTION(ExceptionBase::II_IDENTITY_NOT_FOUND_EXCEPTION,LineGroupProxy::GetSerializableName() + "Properties" + " was not found in the provided XML node, which was expected.");
            }
        }

        String LineGroupProxy::GetDerivedSerializableName() const
            { return LineGroupProxy::GetSerializableName(); }

        String LineGroupProxy::GetSerializableName()
            { return "LineGroupProxy"; }

        ///////////////////////////////////////////////////////////////////////////////
        // Internal Methods

        Line3D* LineGroupProxy::_GetGraphicsObject() const
            { return this->GraphicsLineData; }

        Ogre::MovableObject* LineGroupProxy::_GetBaseGraphicsObject() const
            { return this->GraphicsLineData; }
    }//Graphics
}//Mezzanine

#endif
