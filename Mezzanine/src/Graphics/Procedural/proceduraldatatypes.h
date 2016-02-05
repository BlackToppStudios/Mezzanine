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
#ifndef _graphicsproceduraldatatypes_h
#define _graphicsproceduraldatatypes_h

#include "vector2.h"
#include "linesegment.h"
#include "track.h"

namespace Mezzanine
{
    namespace Graphics
    {
        namespace Procedural
        {
            ///////////////////////////////////////////////////////////////////////////////
            // Curve Types

            /// @brief A track of 1 dimensional points that are linearly interpolated between.
            typedef Track< LinearInterpolator< Real > >     LinearCurve1D;
            /// @brief A track of 2 dimensional points that are linearly interpolated between.
            typedef Track< LinearInterpolator< Vector2 > >  LinearCurve2D;
            /// @brief A track of 3 dimensional points that are linearly interpolated between.
            typedef Track< LinearInterpolator< Vector3 > >  LinearCurve3D;
            /// @brief A track of 1 dimentional points that are interpolated between with a Bezier interpolator.
            typedef Track< BezierInterpolator< Real > >     BezierCurve1D;
            /// @brief A track of 2 dimentional points that are interpolated between with a Bezier interpolator.
            typedef Track< BezierInterpolator< Vector2 > >  BezierCurve2D;
            /// @brief A track of 3 dimentional points that are interpolated between with a Bezier interpolator.
            typedef Track< BezierInterpolator< Vector3 > >  BezierCurve3D;

            ///////////////////////////////////////////////////////////////////////////////
            // Container and Iterator Types

            /// @brief Basic container type for the storage of 2D points.
            typedef std::vector<Vector2>                Point2DContainer;
            /// @brief Iterator type for a Point2DContainer.
            typedef Point2DContainer::iterator          Point2DIterator;
            /// @brief Const Iterator type for a Point2DContainer.
            typedef Point2DContainer::const_iterator    ConstPoint2DIterator;

            /// @brief Basic container type for the storage of 3D points.
            typedef std::vector<Vector3>                Point3DContainer;
            /// @brief Iterator type for a Point3DContainer.
            typedef Point3DContainer::iterator          Point3DIterator;
            /// @brief Const Iterator type for a Point3DContainer.
            typedef Point3DContainer::const_iterator    ConstPoint3DIterator;

            /// @brief Basic container type for the storage of 2D line segments.
            typedef std::vector<LineSegment2D>          LineSeg2DVec;
            /// @brief Iterator type for a LineSeg2DVec.
            typedef LineSeg2DVec::iterator              LineSeg2DIterator;
            /// @brief Const Iterator type for a LineSeg2DVec.
            typedef LineSeg2DVec::const_iterator        ConstLineSeg2DIterator;

            /// @brief Basic container type for the storage of 3D line segments.
            typedef std::vector<LineSegment3D>          LineSeg3DVec;
            /// @brief Iterator type for a LineSeg3DVec.
            typedef LineSeg3DVec::iterator              LineSeg3DIterator;
            /// @typedef ConstLineSeg3DIterator
            /// @brief Const Iterator type for a LineSeg3DVec.
            typedef LineSeg3DVec::const_iterator        ConstLineSeg3DIterator;

            /// @brief A container of Integers used to represent the indicies of a shape.
            typedef std::vector<Integer>                IndexContainer;
            /// @brief Iterator type for a IndexContainer.
            typedef IndexContainer::iterator            IndexIterator;

            /// @brief Const Iterator type for a IndexContainer.
            typedef IndexContainer::const_iterator      ConstIndexIterator;
        }//Procedural
    }//Graphics
}//Mezzanine

#endif
