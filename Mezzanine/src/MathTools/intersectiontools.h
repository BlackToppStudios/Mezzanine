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
#ifndef _mathtoolintersectiontools_h
#define _mathtoolintersectiontools_h

#include "vector2.h"
#include "vector3.h"
#include "plane.h"

namespace Mezzanine
{
    class AxisAlignedBox;
    class Plane;
    class Ray;
    class Sphere;
    class LineSegment2D;
    class LineSegment3D;
    /// @namespace Mezzanine::MathTools
    /// @brief This namespace is the home of a number of utility variables and methods to facilitate various math related tasks.
    namespace MathTools
    {
        /// @brief This is a math helper for finding intersections on bounded parts of Planes
        /// @details Because any plane can be thought of in terms of having an X and a Y axis, some convention
        /// for converting is required. Where ever possible existing axis names are retained. This make the
        /// case of working on a Z axis aligned rectangle easy, but means that the Z axis might be considered
        /// As the X or Y axis for X or Y aligned planes. This causes some logical rotation but should be no
        /// issue if the convention is followed. Use static function DropAxisToCreateVector2 to ease this.
        class PlaneAlignedQuad
        {
            public:
                StandardAxis AlignedOn;     ///< This axis this plane exists on, Since the quad must be axis aligned we can forgo all but a AA unit-vector here (no quaternions needed).
                Real DistanceFromOrigin;    ///< How far is our rectangle from the origin.
                Vector2 MinExtents;         ///< The lowest extents of the rect on the plane concerned. Both values here must be lower than their respective max values
                Vector2 MaxExtents;         ///< The highest extents of the rect.

                /// @brief Simply drop one axis to create a two dimensional coordinate on an implied plane.
                /// @param Point A Vector3 that needs a dimension dropped.
                /// @param AxisToDrop The axis to be dropped which must then be implied to have the resulting Vector2 make Sense in 3d space
                /// @return A Vector2.
                static Vector2 DropAxisToCreateVector2(Vector3 Point, StandardAxis AxisToDrop)
                {
                    switch(AxisToDrop)
                    {
                        case Axis_X: return Vector2(Point.Z, Point.Y);
                        case Axis_Y: return Vector2(Point.X, Point.Z);
                        case Axis_Z: return Vector2(Point.X, Point.Y);
                        default:
                            MEZZ_EXCEPTION(Exception::PARAMETERS_RANGE_EXCEPTION, "Failed to dropped dimension from Vector3 based on Axis while creating a Vector2");
                    }
                }

                /// @brief Add the axis back into the Point to transform it from a 2d coordinate on an implied plane, to one actually in 3d space.
                /// @param Point A Vector3 that needs a dimension dropped.
                /// @param AxisToRegain The axis to be returned to an explicit state.
                /// @param Missing The value for the new axis.
                /// @return A Vector3 un-mutated back into 3d space
                static Vector3 DropAxisToCreateVector2(Vector2 Point, StandardAxis AxisToRegain, Real Missing)
                {
                    switch(AxisToDrop)
                    {
                        case Axis_X: return Vector3(Missing, Point.Y, Point.X);
                        case Axis_Y: return Vector3(Point.X, Missing, Point.Y);
                        case Axis_Z: return Vector3(Point.X, Point.Y, Missing);
                        default:
                            MEZZ_EXCEPTION(Exception::PARAMETERS_RANGE_EXCEPTION, "");
                    }
                }

                /// @brief Direct Advanced Constructor - Uses Vectors already on the implied plan
                /// @param PlanarAlignment The raw StandardAxis to be used in the constructed PlaneAlignedQuad
                /// @param Distance The raw Real to be used in the constructed PlaneAlignedQuad
                /// @param Min The raw Vector2 to be used in the constructed PlaneAlignedQuad as the Minimum extent
                /// @param Max The raw Vector2 to be used in the constructed PlaneAlignedQuad as the Maximum extent
                PlaneAlignedQuad(const StandardAxis& PlanarAlignment = Axis_X,
                                 const Real& Distance = 0.0,
                                 const Vector2& Min = Vector2(),
                                 const Vector2& Max = Vector2())
                    : AlignedOn(PlanarAlignment),
                      DistanceFromOrigin(Distance),
                      MinExtents(Min),
                      MaxExtents(Max)
                {}

                /// @brief Common Constructor - Create using 3d Points.
                /// @param PlanarAlignment The plane this must be aligned on.
                /// @param Min A 3d point
                /// @param Max
                PlaneAlignedQuad(const StandardAxis& PlanarAlignment = Axis_X,
                                 const Vector3& Min = Vector3(),
                                 const Vector3& Max = Vector3())
                    : AlignedOn(PlanarAlignment),
                      DistanceFromOrigin(Min[PlanarAlignment]),
                      MinExtents(PlaneAlignedQuad::DropAxisToCreateVector2(Min,PlanarAlignment)),
                      MaxExtents(PlaneAlignedQuad::DropAxisToCreateVector2(Max,PlanarAlignment))
                {
                    if( Min[PlanarAlignment] != Max[PlanarAlignment] )
                    {
                        MEZZ_EXCEPTION(Exception::PARAMETERS_RANGE_EXCEPTION, "Failed to dropped dimension from Vector3 based on Axis while creating a Vector2");
                    }
                }

                bool OverlapsWith(Vector3 Other) const
                {
                    if(DistanceFromOrigin == Other[AlignedOn])
                        { return OverlapsWithPointOnPl(); }
                    return false;
                }

                bool OverlapsWithPointOnPl() const
                {

                }
        };
    }//MathTools
}//Mezzanine

#endif
