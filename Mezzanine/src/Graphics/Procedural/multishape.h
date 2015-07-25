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
/*
 -----------------------------------------------------------------------------
 This source file is part of ogre-procedural

 For the latest info, see http://code.google.com/p/ogre-procedural/

 Copyright (c) 2010-2013 Michael Broutin

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 -----------------------------------------------------------------------------
 */
#ifndef _graphicsproceduralmultishape_h
#define _graphicsproceduralmultishape_h

#include "linesegment.h"
#include "Graphics/Procedural/proceduraldatatypes.h"
#include "Graphics/Procedural/proceduralenumerations.h"

namespace Mezzanine
{
    namespace Graphics
    {
        class Mesh;
        namespace Procedural
        {
            class Shape;
            ///////////////////////////////////////////////////////////////////////////////
            /// @brief A grouping of individual 2D shapes used to express more elaborate shapes.
            /// @details There are a number of assumptions that are made and are not checked
            /// against; the shapes must not cross each other.
            ///////////////////////////////////////
            class MEZZ_LIB MultiShape
            {
            public:
                /// @brief Basic container type for Shape storage in this class.
                typedef std::vector< Shape >                    ShapeContainer;
                /// @brief Iterator type for Shape instances being stored in this class.
                typedef ShapeContainer::iterator                ShapeIterator;
                /// @brief Const Iterator type for Shape instances being stored in this class.
                typedef ShapeContainer::const_iterator          ConstShapeIterator;
                /// @brief Basic container type for LineSegment2D storage in this class.
                typedef std::vector< LineSegment2D >            LineSegmentContainer;
                /// @brief Iterator type for LineSegment2D instances being stored in this class.
                typedef LineSegmentContainer::iterator          LineSegmentIterator;
                /// @brief Const Iterator type for LineSegment2D instances being stored in this class.
                typedef LineSegmentContainer::const_iterator    ConstLineSegmentIterator;
            protected:
                /// @internal
                /// @brief Container storing all of the Shapes that form this MultiShape.
                ShapeContainer Shapes;
            public:
                /// @brief Blank constructor.
                MultiShape();
                /// @brief Single shape constructor.
                /// @param ToAdd The shape to be added to this MultiShape.
                MultiShape(const Shape& ToAdd);
                /// @brief Multiple shape constructor.
                /// @param ToAdd A container of shapes that will all be added to this MultiShape.
                MultiShape(const ShapeContainer& ToAdd);
                /// @brief Class destructor.
                ~MultiShape();

                ///////////////////////////////////////////////////////////////////////////////
                // Utility

                /// @brief Outputs a mesh representing this MultiShape.
                /// @param Name The name of the mesh to be generated.
                /// @param Group The name of the resource group to create the mesh in.
                /// @return Returns a pointer to the new mesh.
                Mesh* GenerateMesh(const String& Name, const String& Group);

                /// @brief Creates this MultiShape from a listing of segments.
                /// @param Segments The segments to form this MultiShape from.
                void BuildFromSegmentSoup(const LineSegmentContainer& Segments);
                /// @brief Gets a container of all the points in this MultiShape.
                /// @return Returns a vector of Vector2's representing each point in this MultiShape.
                Point2DContainer GetPoints() const;

                /// @brief Gets whether or not a point is inside this MultiShape.
                /// @note This assumes that all of the shapes in that multishape are closed and that
                /// they don't contradict each other, ie a point cannot be outside and inside at the same time.
                /// @param Point The location in 2D space to check.
                /// @return Returns true if the point is inside this MultiShape, false otherwise.
                Boole IsPointInside(const Vector2& Point) const;
                /// @brief Gets whether the currently set OutSide is the real Outside.
                /// @return Returns true if the detected OutSide matches the currently set OutSide, false otherwise.
                Boole IsOutsideRealOutside() const;

                /// @brief Closes each shape in this MultiShape.
                /// @return Returns a reference to this.
                MultiShape& Close();
                /// @brief Gets whether or not every Shape in this MultiShape is closed.
                /// @return Returns true if every Shape in this MultiShape is closed, false otherwise.
                Boole IsClosed() const;

                ///////////////////////////////////////////////////////////////////////////////
                // Shape Management

                /// @brief Adds a shape to this MultiShape.
                /// @param ToAdd The shape to be added.
                /// @return Returns a reference to this.
                MultiShape& AddShape(const Shape& ToAdd);
                /// @brief Append every shape of another MultiShape to this MultiShape.
                /// @param Other The other MultiShape to append.
                /// @return Returns a reference to this.
                MultiShape& AddMultiShape(const MultiShape& Other);
                /// @brief Gets a shape by index.
                /// @param Index The index of the shape to retrieve.
                /// @return Returns a reference to the shape at the specified index.
                Shape& GetShape(const Whole Index);
                /// @brief Gets a shape by index.
                /// @param Index The index of the shape to retrieve.
                /// @return Returns a const reference to the shape at the specified index.
                const Shape& GetShape(const Whole Index) const;
                /// @brief Gets the number of shapes in this MultiShape.
                /// @return Returns a Whole representing the number of shapes currently stored in this MultiShape.
                Whole GetNumShapes() const;
                /// @brief Removes all shapes from this MultiShape.
                void RemoveAllShapes();
            };//MultiShape
        }//Procedural
    }//Graphics
}//Mezzanine

#endif
