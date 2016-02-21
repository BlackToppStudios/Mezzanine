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
#ifndef _graphicsproceduralmultipath_h
#define _graphicsproceduralmultipath_h

#include "datatypes.h"

namespace Mezzanine
{
    namespace Graphics
    {
        namespace Procedural
        {
            class Path;
            ///////////////////////////////////////////////////////////////////////////////
            /// @brief A convenience class for tracking points in a MultiPath.
            /// @details
            ///////////////////////////////////////
            struct MEZZ_LIB PathCoordinate
            {
                ///////////////////////////////////////////////////////////////////////////////
                // Data Members

                /// @brief The index of the path where this coordinate is located.
                Whole PathIndex;
                /// @brief The index of the point in the path where this coordinate is located.
                Whole PointIndex;

                ///////////////////////////////////////////////////////////////////////////////
                // Construction and Destruction

                /// @brief Blank constructor.
                PathCoordinate();
                /// @brief Descriptive constructor.
                /// @param Path The index of the path where this coordinate is located.
                /// @param Point The index of the point in the path where this coordinate is located.
                PathCoordinate(const Whole Path, const Whole Point);
                /// @brief Class destructor.
                ~PathCoordinate();

                ///////////////////////////////////////////////////////////////////////////////
                // Comparison

                /// @brief Less-than comparison operator.
                /// @param Other The other PathCoordinate to compare against.
                /// @return Returns true if this PathCoordinate is considered less than the other PathCoordinate, false otherwise.
                Boole operator<(const PathCoordinate& Other) const;
            };//PathCoordinate

            ///////////////////////////////////////////////////////////////////////////////
            /// @brief A grouping of individual 3D curves used to express complicated structures.
            /// @details
            ///////////////////////////////////////
            class MEZZ_LIB MultiPath
            {
            public:
                /// @brief Basic container type for Path storage in this class.
                typedef std::vector< Path >                        PathContainer;
                /// @brief Iterator type for Path instances being stored in this class.
                typedef PathContainer::iterator                    PathIterator;
                /// @brief Const Iterator type for Path instances being stored in this class.
                typedef PathContainer::const_iterator              ConstPathIterator;
                /// @brief Convenience type used to represent the intersection of two or more paths.
                typedef std::vector< PathCoordinate >              PathIntersection;
                /// @brief Container type for the storage of intersections.
                typedef std::vector< PathIntersection >            IntersectionVector;
                /// @brief Container type for the storage of unique intersections.
                typedef std::map<PathCoordinate,PathIntersection>  IntersectionMap;
                /// @brief Convenience type for a pair of indexes to express a segment of a Path.
                typedef std::pair<Whole,Whole>                     PathSegment;
                /// @brief Container type for segments in a Path.
                typedef std::vector<PathSegment>                   PathSegmentContainer;
            protected:
                /// @internal
                /// @brief Container storing all of the Paths that form this MultiPath.
                PathContainer Paths;
                /// @internal
                /// @brief Container storing all known intersections.
                IntersectionVector Intersections;
                /// @internal
                /// @brief Container storing all intersections based on their coordinate.
                IntersectionMap UniqueIntersections;
            public:
                /// @brief Class constructor.
                MultiPath();
                /// @brief Class destructor.
                ~MultiPath();

                ///////////////////////////////////////////////////////////////////////////////
                // Utility

                /// @brief Sets a path at a specific index.
                /// @remarks This does no bounds checking and may throw an out of bounds exception.
                /// @param PathIndex The index to place the specified path at.
                /// @param ToSet The path to be set at the specified index.
                /// @return Returns a reference to this.
                MultiPath& SetPath(const Whole PathIndex, const Path& ToSet);

                /// @brief Generates all the known intersections between the paths in this MultiPath.
                /// @return Returns a reference to this.
                MultiPath& CalculateIntersections();
                /// @brief Gets segments from a specific sub-path that do not intersect other Paths of this MultiPath.
                /// @remarks "CalculateIntersections" should be called prior to this to get valid results.
                /// @param PathIndex The index of the path to get non-intersectings parts of.
                /// @return Returns a vector of segments stored as indexes that are non-intersecting parts of the specified Path.
                PathSegmentContainer GetNoIntersectionParts(const Whole PathIndex) const;

                ///////////////////////////////////////////////////////////////////////////////
                // Path Management

                /// @brief Adds a path to this MultiPath.
                /// @param ToAdd The path to be added.
                /// @return Returns a reference to this.
                MultiPath& AddPath(const Path& ToAdd);
                /// @brief Append every path of another MultiShape to this MultiPath.
                /// @param Other The other MultiShape to append.
                /// @return Returns a reference to this.
                MultiPath& AddMultiPath(const MultiPath& Other);
                /// @brief Gets a path by index.
                /// @param Index The index of the path to retrieve.
                /// @return Returns a reference to the path at the specified index.
                Path& GetPath(const Whole Index);
                /// @brief Gets a path by index.
                /// @param Index The index of the path to retrieve.
                /// @return Returns a const reference to the path at the specified index.
                const Path& GetPath(const Whole Index) const;
                /// @brief Gets the number of shapes in this MultiPath.
                /// @return Returns a Whole representing the number of paths currently stored in this MultiPath.
                Whole GetNumPaths() const;
                /// @brief Removes all shapes from this MultiPath.
                void RemoveAllPaths();

                ///////////////////////////////////////////////////////////////////////////////
                // Intersection Management

                /// @brief Gets the container storing all calculated intersections.
                /// @return Returns a const reference to a vector storing all known intersections in this MultiPath.
                const IntersectionVector& GetIntersections() const;
                /// @brief Gets the container storing all calculated intersections based on their path coordinate.
                /// @return Returns a const reference to a map storing all unique intersections in this MultiPath.
                const IntersectionMap& GetIntersectionsMap() const;
            };//MultiPath
        }//Procedural
    }//Graphics
}//Mezzanine

#endif
