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
#ifndef _graphicsproceduralpath_h
#define _graphicsproceduralpath_h

#include "vector3.h"
#include "interpolator.h"
#include "track.h"

#include "Graphics/Procedural/proceduraldatatypes.h"

namespace Mezzanine
{
    namespace Graphics
    {
        class Mesh;
        namespace Procedural
        {
            class Path;
            class Shape;

            /// @typedef PathContainer
            /// @brief Basic container type for the storage of Paths.
            typedef std::vector< Path >                 PathContainer;

            ///////////////////////////////////////////////////////////////////////////////
            /// @brief A collection of interconnected 3D points used to express path through 3D space.
            /// @details
            ///////////////////////////////////////
            class MEZZ_LIB Path
            {
            protected:
                /// @internal
                /// @brief A container storing all of the points in this path.
                Point3DContainer Points;
                /// @internal
                /// @brief Whether or not the end of this path connects to the beginning of this path.
                Boole Closed;
            public:
                /// @brief Blank constructor.
                Path();
                /// @brief Create a Path from an iterator range.
                /// @param Begin An iterator pointing to the beginning of a range to copy.
                /// @param End an iterator pointing to one past the rang to copy.
                Path(Point3DIterator Begin, Point3DIterator End);
                /// @brief Create a Path from a Point3DContainer instance.
                /// @param DataSet A collection of points to copy.
                Path(const Point3DContainer& DataSet);
                /// @brief Class destructor.
                ~Path();

                ///////////////////////////////////////////////////////////////////////////////
                // Utility

                /// @brief Generates one or more paths from a series of segments.
                /// @param Segments The segments to use to generate paths.
                /// @param GeneratedPaths A container to be populated with the paths that are generated.
                static void BuildFromSegmentSoup(const LineSeg3DVec& Segments, PathContainer& GeneratedPaths);

                /// @brief Outputs a mesh representing this path.
                /// @param Name The name of the mesh to be generated.
                /// @param Group The name of the resource group to create the mesh in.
                /// @return Returns a pointer to the new mesh.
                Mesh* GenerateMesh(const String& Name, const String& Group) const;

                /// @brief Appends all the points from another path to this path.
                /// @param Other The other path to append to this path.
                /// @return Returns a reference to this.
                Path& AppendPath(const Path& Other);
                /// @brief Appends all the points from another path to this path with their positions relative to the position of the last point in this path.
                /// @param Other The other path to append to this path.
                /// @return Returns a reference to this.
                Path& AppendPathRel(const Path& Other);
                /// @brief Extracts a part of the path as a new path.
                /// @param First The first index of this path to be in the new path.
                /// @param Last The last index of this path to be in the new path.
                /// @return Returns a reference to this.
                Path ExtractSubPath(const Whole First, const Whole Last);

                /// @brief Appends the contents of a 3D track to this shape.
                /// @remarks Low values for NumPoints will cause curves to look blocky.  How how it should be depends on the resolution of the curve you want.
                /// @param Curve The 3D curved track of points to append to this shape.
                /// @param NumPoints The number of points along the curve to generate for this shape.
                template< template<class> class Interpolator >
                void AppendTrack(const Track< Interpolator< Vector3 > >& Curve, const Whole NumPoints)
                {
                    // Minus one because we want to include the end of the curve without going over the number of points requested
                    for( Whole CurrPoint = 0 ; CurrPoint < NumPoints ; ++CurrPoint )
                    {
                        Real InterpolateVal = static_cast<Real>( CurrPoint ) / static_cast<Real>( NumPoints - 1 );
                        Vector3 CurvePoint = Curve.GetInterpolated(InterpolateVal);
                        this->AddPoint(CurvePoint);
                    }
                }

                /// @brief Gets the number of segments in this path.
                /// @return Returns a Whole containing the number of segments that form this path.
                Integer GetSegCount() const;
                /// @brief Gets the total length of all segments in this path.
                /// @return Returns a Real representing the combined length of each segment in this path.
                Real GetTotalLength() const;
                /// @brief Gets the total length to the point in this path at an index.
                /// @param PointIndex The point to retrieve the length to.
                /// @return Returns a Real representing the combined length of each segment to the point at the specified index.
                Real GetLengthAtPoint(const Whole PointIndex) const;

                /// @brief Reflect all points in this path against a zero-origined plane with a given normal.
                /// @param Normal The normal to reflect all points on.
                /// @return Returns a reference to this.
                Path& Reflect(const Vector3& Normal);

                /// @brief Reverses direction/ordering of the segments in this path.
                /// @return Returns a reference to this.
                Path& Reverse();

                /// @brief Connects the last point in this path to the first point.
                /// @return Returns a reference to this.
                Path& Close();
                /// @brief Gets whether or not the final point in this path connects to the first point.
                /// @return Returns true if this path is currently closed.
                Boole IsClosed() const;

                ///////////////////////////////////////////////////////////////////////////////
                // Point Management

                /// @brief Adds a point to this path.
                /// @param ToAdd The location of the point to add.
                /// @return Returns a reference to this.
                Path& AddPoint(const Vector3& ToAdd);
                /// @brief Adds a point to this path.
                /// @param X The location of the point to add on the X axis.
                /// @param Y The location of the point to add on the Y axis.
                /// @param Z The location of the point to add on the Z axis.
                /// @return Returns a reference to this.
                Path& AddPoint(const Real X, const Real Y, const Real Z);

                /// @brief Inserts a point to the path.
                /// @param Index the index before the inserted point.
                /// @param X The position of the point to insert on the X axis.
                /// @param Y The position of the point to insert on the Y axis.
                /// @param Y The position of the point to insert on the Z axis.
                /// @return Returns a reference to this.
                Path& InsertPoint(const Whole Index, const Real X, const Real Y, const Real Z);
                /// @brief Inserts a point to the path.
                /// @param Index the index before the inserted point.
                /// @param Point The point to be inserted.
                /// @return Returns a reference to this.
                Path& InsertPoint(const Whole Index, const Vector3& Point);

                /// @brief Safely gets a point in this path.
                /// @param Index The index of the point to retrieve.  Negative values will be clamped to zero.
                /// @return Returns a const reference to the Vector3 at the specified index.
                const Vector3& GetPoint(const Integer Index) const;

                /// @brief Gets all of the points in this path.
                /// @return Returns a reference to the container storing each point in this path.
                Point3DContainer& GetPoints();
                /// @brief Gets all of the points in this path.
                /// @return Returns a const reference to the container storing each point in this path.
                const Point3DContainer& GetPoints() const;

                ///////////////////////////////////////////////////////////////////////////////
                // Transform

                /// @brief Applies the given translation to all the points in this path.
                /// @note This will only transfrom the points currently in this path, not any additional points made after calling this.
                /// @param Trans The amount of translation to apply.
                /// @return Returns a reference to this.
                Path& Translate(const Vector3& Trans);
                /// @brief Applies the given translation to all the points in this path.
                /// @note This will only transfrom the points currently in this path, not any additional points made after calling this.
                /// @param TransX The amount of translation to apply on the X axis.
                /// @param TransY The amount of translation to apply on the Y axis.
                /// @param TransZ The amount of translation to apply on the Z axis.
                /// @return Returns a reference to this.
                Path& Translate(const Real TransX, const Real TransY, const Real TransZ);
                /// @brief Applies the given scale to all the points in this path.
                /// @note This will only transfrom the points currently in this path, not any additional points made after calling this.
                /// @param Scaling The amount of scaling to apply to each axis.
                /// @return Returns a reference to this.
                Path& Scale(const Real Scaling);
                /// @brief Applies the given scale to all the points in this path.
                /// @note This will only transfrom the points currently in this path, not any additional points made after calling this.
                /// @param ScaleX The amount of scaling to be applied on the X axis.
                /// @param ScaleY The amount of scaling to be applied on the Y axis.
                /// @param ScaleZ The amount of scaling to be applied on the Z axis.
                /// @return Returns a reference to this.
                Path& Scale(const Real ScaleX, const Real ScaleY, const Real ScaleZ);
                /// @brief Applies the given scale to all the points in this path.
                /// @note This will only transfrom the points currently in this path, not any additional points made after calling this.
                /// @param Scaling The amount of scaling to apply to each axis.
                /// @return Returns a reference to this.
                Path& Scale(const Vector3& Scaling);

                ///////////////////////////////////////////////////////////////////////////////
                // Directions and Normals

                /// @brief Gets the direction of a point to the next point in the sequence.
                /// @param Index The index of the point to retrieve the direction for.
                /// @return Returns a Vector3 containing the direction from the specified point to the next point.
                Vector3 GetDirectionAfter(const Whole Index) const;
                /// @brief Gets the direction from a point to the previous point in the sequence.
                /// @param Index The index of the point to retrieve the direction for.
                /// @return Returns a Vector3 containing the direction from the specified point to the previous point.
                Vector3 GetDirectionBefore(const Whole Index) const;
                /// @brief Gets the averaged direction from the specified point to both the next and previous points in the sequence.
                /// @param Index The index of the point to retrieve the direction for.
                /// @return Returns a Vector3 containing the averaged direction for both the next and previous points from the specified point.
                Vector3 GetAvgDirection(const Whole Index) const;

                ///////////////////////////////////////////////////////////////////////////////
                // Conversion

                /// @brief Converts the path to a 2D shape, dropping the Z component.
                /// @return Returns a shape where the X and Y components are copied, and the Z component is dropped for each point.
                Shape ConvertToShape() const;
            };//Path
        }//Procedural
    }//Graphics
}//Mezzanine

#endif
