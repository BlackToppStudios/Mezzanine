// © Copyright 2010 - 2014 BlackTopp Studios Inc.
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
#ifndef _graphicsproceduralextruder_h
#define _graphicsproceduralextruder_h

#include "Graphics/Procedural/Mesh/meshgenerator.h"
#include "Graphics/Procedural/multipath.h"
#include "Graphics/Procedural/multishape.h"

namespace Mezzanine
{
    namespace Graphics
    {
        namespace Procedural
        {
            ///////////////////////////////////////////////////////////////////////////////
            /// @brief A generator that will create a 3D model by moving a 2D shape along a 3D Path.
            /// @details
            ///////////////////////////////////////
            class MEZZ_LIB Extruder : public MeshGenerator<Extruder>
            {
            public:
                /// @brief Convenience type for a Track that stores additional optional data.
                typedef Track< LinearInterpolator<Real> >    ParameterTrack;
                /// @brief Container type for storing ParameterTracks.
                typedef std::map< Whole, ParameterTrack* >   TrackMap;
                /// @brief Iterator type for ParameterTracks.
                typedef TrackMap::iterator                   TrackMapIterator;
                /// @brief Const Iterator type for ParameterTracks.
                typedef TrackMap::const_iterator             ConstTrackMapIterator;
            protected:
                /// @internal
                /// @brief A MultiPath storing all of the Paths the 2D shapes will be extruded along.
                MultiPath PathsToExtrude;
                /// @internal
                /// @brief A MultiShape storing all of the 2D shapes that will be extruded along the specified Paths.
                MultiShape ShapesToExtrude;
                /// @internal
                /// @brief A map of optional Tracks that store how the shapes on a given Path are to be scaled.
                TrackMap ScaleTracks;
                /// @internal
                /// @brief A map of optional Tracks that store how the shapes on a given Path are to be rotated.
                TrackMap RotationTracks;
                /// @internal
                /// @brief A map of optional Tracks that store how texture coordinates are to be assigned along the Paths being extruded along.
                TrackMap PathUTextureTracks;
                /// @internal
                /// @brief A map of optional Tracks that store how texture coordinates are to be assigned along the Shapes being extruded.
                TrackMap ShapeVTextureTracks;
                /// @internal
                /// @brief Whether or not the ends of each Path will be capped.
                Boole Capped;

                /// @internal
                /// @brief Creates appropriate UV coordinates for a given Vertex.
                /// @param PathIndex The index of the Path being extruded.
                /// @param ShapeIndex The index of the Shape being extruded.
                /// @param PathCoord The coordinate position on the current Path being extruded.
                /// @param ShapeCoord The coordinate position on the current Shape being extruded.
                /// @return Returns a Vector2 with Path and Shape appropriate UV coordinates.
                Vector2 GenerateUVs(const Whole PathIndex, const Whole ShapeIndex, const Real PathCoord, const Real ShapeCoord) const;
                /// @internal
                /// @brief Creates the caps at the end of each shape extruded along a given Path.
                /// @param Buffer The buffer to be modified.
                void GenerateExtrusionCaps(TriangleBuffer& Buffer) const;
                /// @internal
                /// @brief Creates a single part of a segment being extruded.
                /// @param Buffer The buffer to be modified.
                /// @param PathIndex The index of the Path being extruded.
                /// @param ShapeIndex The index of the Shape being extruded.
                /// @param PathCoord The coordinate position on the current Path being extruded.
                /// @param Position The position on the Path the Shape will be extruded at.
                /// @param LeftOri The rotation to be applied to Negative-X positioned points in the Shape being extruded.
                /// @param RightOri The rotation to be applied to Positive-X positioned points in the Shape being extruded.
                /// @param Scale The scaling to be applied to the Shape being extruded at this segment.
                /// @param ScaleLeftCorrect The scaling adjustment to apply on the Negative-X positioned points in the Shape being extruded.
                /// @param ScaleRightCorrect The scaling adjustment to apply on the Positive-X positioned points in the Shape being extruded.
                /// @param JoinWithNextShape Whether or not to set up the indicies to
                void GenerateExtrusionShape(TriangleBuffer& Buffer, const Whole PathIndex, const Whole ShapeIndex, const Real PathCoord, const Vector3& Position, const Quaternion& LeftOri,
                                            const Quaternion& RightOri, const Real Scale, const Real ScaleLeftCorrect, const Real ScaleRightCorrect, const Boole JoinWithNextShape) const;
                /// @internal
                /// @brief Creates a sequence of non-intersecting segments.
                /// @param Buffer The buffer to be modified.
                /// @param ShapeIndex The index of the Shape to be extruded.
                /// @param PathIndex The index of the Path to be extruded along.
                /// @param PathSegBegin The index of the start of the segment on the specified Path.
                /// @param PathSegEnd The index of the end of the segment on the specified Path.
                void GenerateExtrusionSegment(TriangleBuffer& Buffer, const Whole ShapeIndex, const Whole PathIndex, const Whole PathSegBegin, const Whole PathSegEnd) const;
                /// @internal
                /// @brief Creates an intersection of segments.
                /// @param Buffer The buffer to be modified.
                /// @param Intersection A single intersection of multiple segments in the MultiPath.
                /// @param ShapeIndex The index of the Shape to be extruded.
                void GenerateExtrusionIntersection(TriangleBuffer& Buffer, const MultiPath::PathIntersection& Intersection, const Whole ShapeIndex) const;
            public:
                /// @brief Class constructor.
                Extruder();
                /// @brief Class destructor.
                virtual ~Extruder();

                ///////////////////////////////////////////////////////////////////////////////
                // Utility

                /// @copydoc MeshGenerator::AddToTriangleBuffer(TriangleBuffer&) const
                /// @exception If neither the shape or multishape are defined an INVALID_STATE_EXCEPTION will be thrown.
                virtual void AddToTriangleBuffer(TriangleBuffer& Buffer) const;

                ///////////////////////////////////////////////////////////////////////////////
                // Configuration

                /// @brief Sets a single Path to be extruded along.
                /// @param ToExtrude A Path to be extruded along.
                /// @return Returns a reference to this.
                Extruder& SetPathToExtrude(const Path& ToExtrude);
                /// @brief Sets a MultiPath to be extruded along.
                /// @param ToExtrude A MultiPath storing all of the Paths the 2D shapes will be extruded along.
                /// @return Returns a reference to this.
                Extruder& SetMultiPathToExtrude(const MultiPath& ToExtrude);
                /// @brief Sets a single Shape to be extruded.
                /// @param ToExtrude A Shape to be extruded.
                /// @return Returns a reference to this.
                Extruder& SetShapeToExtrude(const Shape& ToExtrude);
                /// @brief Sets a Multishape to be extruded.
                /// @param ToExtrude A MultiShape storing all of the 2D shapes that will be extruded along the specified Paths.
                /// @return Returns a reference to this.
                Extruder& SetMultiShapeToExtrude(const MultiShape& ToExtrude);

                /// @brief Sets how shapes are to be scaled on a specific Path.
                /// @param PathIndex The index of the Path this Track is to be applied to.
                /// @param Scale A pointer to an optional Track that stores how the shapes on a given Path are to be scaled.  Initial Value: NULL.
                /// @return Returns a reference to this.
                Extruder& SetScaleTrack(const Whole PathIndex, ParameterTrack* Scale);
                /// @brief Sets how shapes are to be rotated on a specific Path.
                /// @param PathIndex The index of the Path this Track is to be applied to.
                /// @param Rotation A pointer to an optional Track that stores how the shapes on a given Path are to be rotated.  Initial Value: NULL.
                /// @return Returns a reference to this.
                Extruder& SetRotationTrack(const Whole PathIndex, ParameterTrack* Rotation);
                /// @brief Sets how the "U" component texture coordinates are to be generated for extruded shapes.
                /// @param PathIndex The index of the Path this Track is to be applied to.
                /// @param PathU A pointer to an optional Track that stores how texture coordinates are to be assigned along the Path being extruded along.  Initial Value: NULL.
                /// @return Returns a reference to this.
                Extruder& SetPathUTextureTrack(const Whole PathIndex, ParameterTrack* PathU);
                /// @brief Sets how the "V" component texture coordinates are to be generated for extruded shapes.
                /// @param ShapeIndex The index of the Shape this Track is to be applied to.
                /// @param ShapeV A pointer to an optional Track that stores how texture coordinates are to be assigned along the shape being extruded.  Initial Value: NULL.
                /// @return Returns a reference to this.
                Extruder& SetShapeVTextureTrack(const Whole ShapeIndex, ParameterTrack* ShapeV);

                /// @brief Sets whether or not the ends of each Path extruded are to be enclosed.
                /// @param Cap True to enclose the Mesh at the ends of each Path, false otherwise.
                /// @return Returns a reference to this.
                Extruder& SetCapped(const Boole Cap);
            };//Extruder
        }//Procedural
    }//Graphics
}//Mezzanine

#endif
