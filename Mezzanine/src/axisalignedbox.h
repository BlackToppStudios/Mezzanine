// © Copyright 2010 - 2013 BlackTopp Studios Inc.
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
#ifndef _axisalignedbox_h
#define _axisalignedbox_h

/// @file
/// @brief This file contains the AxisAlignedBox class for representing AABB's of objects in the world.

#include "vector3.h"
#include "plane.h"
#include "ray.h"

namespace Mezzanine
{
    ///////////////////////////////////////////////////////////////////////////////
    /// @brief This is a utility class used to represent the Axis-Aligned Bounding Boxes of objects in various subsystems.
    /// @details
    ///////////////////////////////////////
    class MEZZ_LIB AxisAlignedBox
    {
    public:
        /// @enum AxisExtent
        /// @brief This enum is used to help make querys for data on the AABB.
        enum AxisExtent
        {
            AE_Min = 1,
            AE_Max = 2
        };
        /// @brief This is a type used for the return of a ray intersection test.
        /// @details This type provides more verbose return data that can be used for further tests.
        typedef std::pair<Bool,Ray> RayTestResult;
    protected:
        /// @internal
        /// @brief The minimum extents on each axis in world space.
        Vector3 AABBMin;
        /// @internal
        /// @brief The maximum extents on each axis in world space.
        Vector3 AABBMax;
    public:
        /// @brief Blank constructor.
        AxisAlignedBox();
        /// @brief Extents constructor.
        /// @param Min The minimum extents on each axis in world space.
        /// @param Max The maximum extents on each axis in world space.
        AxisAlignedBox(const Vector3& Min, const Vector3& Max);
        /// @brief Class destructor.
        ~AxisAlignedBox();

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        /// @brief Checks to see if a point is inside this AABB.
        /// @param ToCheck The location to check to see if it is within this AABB.
        /// @return Returns true if the point provided is within this AABB, false otherwise.
        Bool Intersects(const Vector3& ToCheck) const;
        /// @brief Checks to see if another AABB overlaps with this one.
        /// @param ToCheck The other AABB to check for overlap.
        /// @return Returns true if the two AABB's overlap, false otherwise.
        Bool Intersects(const AxisAlignedBox& ToCheck) const;
        /// @brief Checks to see if a plane intersects this AABB.
        /// @param ToCheck The plane to check for intersection.
        /// @return Returns true if the provided plane intersects with this AABB, false otherwise.
        Bool Intersects(const Plane& ToCheck) const;
        /// @brief Checks to see if a ray intersects this AABB.
        /// @param ToCheck The ray to check for a hit.
        /// @return Returns a std::pair containing whether or not the ray hit, and if it did the subsection of the ray that went through the AABB.
        RayTestResult Intersects(const Ray& ToCheck) const;

        ///////////////////////////////////////////////////////////////////////////////
        // Extents Methods

        /// @brief Sets the minimum and maximum extents of this AABB.
        /// @param Min The minimum extents on each axis in world space.
        /// @param Max The maximum extents on each axis in world space.
        void SetExtents(const Vector3& Min, const Vector3& Max);

        /// @brief Sets the minimum extent of this AABB.
        /// @param Min The minimum extent on each axis.
        void SetMin(const Vector3& Min);
        /// @brief Gets the minimum extents on each axis.
        /// @return Returns a const reference to the minimum extents of this AABB.
        const Vector3& GetMin() const;
        /// @brief Sets the maximum extent of this AABB.
        /// @param Max The maximum extent on each axis.
        void SetMax(const Vector3& Max);
        /// @brief Gets the maximum extents on each axis.
        /// @return Returns a const reference to the maximum extents of this AABB.
        const Vector3& GetMax() const;

        /// @brief Gets the size of this AABB.
        /// @return Returns a Vector3 representing the size of this AABB.
        Vector3 GetSize() const;
        /// @brief Gets the center of this AABB.
        /// @return Returns a Vector3 containing the center of this AABB.
        Vector3 GetCenter() const;
        /// @brief Gets the location of the specified corner
        /// @param XEx The extent for the X axis to retrieve.
        /// @param YEx The extent for the Y axis to retrieve.
        /// @param ZEx The extent for the Z axis to retrieve.
        /// @return Returns a Vector3 representing the position of the specified corner.
        Vector3 GetCorner(const AxisExtent XEx, const AxisExtent YEx, const AxisExtent ZEx) const;

        ///////////////////////////////////////////////////////////////////////////////
        // Operators

        /// @brief Equality operator.
        /// @param Other The other AABB to compare with.
        /// @return Returns true if this AABB is the same as the other provided AABB, false otherwise.
        Bool operator==(const AxisAlignedBox& Other) const;
        /// @brief Inequality operator.
        /// @param Other The other AABB to compare with.
        /// @return Returns true if this AABB is not the same as the other provided AABB, false otherwise.
        Bool operator!=(const AxisAlignedBox& Other) const;
    };//AxisAlignedBox
}//Mezzanine

#endif
