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
#ifndef _graphicslinegroupproxy_h
#define _graphicslinegroupproxy_h

#include "colourvalue.h"
#include "Graphics/renderableproxy.h"

namespace Mezzanine
{
    namespace Graphics
    {
        class Line3D;
        class SceneManager;
        ///////////////////////////////////////////////////////////////////////////////
        /// @brief A small declaration of what our vertices look like for when we write to the buffers.
        ///////////////////////////////////////
        struct LineVertex
        {
            ///////////////////////////////////////////////////////////////////////////////
            // Data Members

            /// @brief Vertex position.
            Vector3 Location;
            /// @brief Vertex colour.
            ColourValue Colour;

            ///////////////////////////////////////////////////////////////////////////////
            // Constructor and Destruction

            /// @brief Blank constructor.
            LineVertex() = delete;
            /// @brief Copy constructor.
            /// @param Other The other LineVertex to be copied.
            LineVertex(const LineVertex& Other) = default;
            /// @brief Move constructor.
            /// @param Other The other LineVertex to be moved.
            LineVertex(LineVertex&& Other) = default;
            /// @brief Class constructor.
            /// @param Pos The position to be assigned to this vertex.
            /// @param Col The colour of this vertex.
            LineVertex(const Vector3& Pos, const ColourValue& Col) :
                Location(Pos),
                Colour(Col)
                {  }
            /// @brief Class destructor.
            ~LineVertex() = default;

            ///////////////////////////////////////////////////////////////////////////////
            // Operators

            /// @brief Assignment operator.
            /// @param Other The other LineVertex to be copied.
            /// @return Returns a reference to this.
            LineVertex& operator=(const LineVertex& Other) = default;
            /// @brief Move assignment operator.
            /// @param Other The other LineVertex to be moved.
            /// @return Returns a reference to this.
            LineVertex& operator=(LineVertex&& Other) = default;
        };//LineVertex

        ///////////////////////////////////////////////////////////////////////////////
        /// @brief This is a group of consecutive line segments to be rendered together.
        /// @details This class stores a listing of points and renders them as one
        /// object into the scene provided.
        ///////////////////////////////////////
        class MEZZ_LIB LineGroupProxy : public RenderableProxy
        {
        protected:
            /// @brief A Pointer to the internal class that actually does the work.
            Line3D* GraphicsLineData;

            /// @brief Creates an internal line group to be used by the calling instance.
            virtual void CreateLineGroup();
            /// @brief Destroys the internal line group in use by this proxy.
            virtual void DestroyLineGroup();
        public:
            /// @brief Blank constructor.
            /// @param ID The unique ID assigned to this LineGroupProxy.
            /// @param Creator A pointer to the manager that created this proxy.
            LineGroupProxy(const UInt32 ID, SceneManager* Creator);
            /// @brief XML constructor.
            /// @param SelfRoot An XML::Node containing the data to populate the new instance with.
            /// @param Creator A pointer to the manager that created this proxy.
            LineGroupProxy(const XML::Node& SelfRoot, SceneManager* Creator);
            /// @brief Class destructor.
            virtual ~LineGroupProxy();

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            /// @copydoc EntityProxy::GetComponentType() const
            virtual Mezzanine::ComponentType GetComponentType() const override;
            /// @copydoc EntityProxy::IsStatic() const
            virtual Boole IsStatic() const override;

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

            /// @brief Removes all points from this line grouping.
            void ClearPoints();
            /// @brief Updates the internal buffers as necessary for rendering.
            /// @note This must be called after any updates are done to the line configuration in order to see the changes.
            void UpdateBuffers();

            ///////////////////////////////////////////////////////////////////////////////
            // Serialization

            /// @copydoc EntityProxy::ProtoSerializeProperties(XML::Node& SelfRoot) const
            virtual void ProtoSerializeProperties(XML::Node& SelfRoot) const override;
            /// @copydoc EntityProxy::ProtoDeSerializeProperties(const XML::Node& SelfRoot)
            virtual void ProtoDeSerializeProperties(const XML::Node& SelfRoot) override;

            /// @copydoc EntityProxy::GetDerivedSerializableName() const
            virtual String GetDerivedSerializableName() const override;
            /// @copydoc EntityProxy::GetSerializableName()
            static String GetSerializableName();

            ///////////////////////////////////////////////////////////////////////////////
            // Internal Methods

            /// @internal
            /// @brief Accessor for the internal line group.
            /// @return Returns a pointer to the internal line group this proxy is based on.
            virtual Line3D* _GetGraphicsObject() const;
            /// @copydoc RenderableProxy::_GetBaseGraphicsObject() const
            virtual Ogre::MovableObject* _GetBaseGraphicsObject() const override;
        };//LineGroupProxy
    }//Graphics
}//Mezzanine

#endif
