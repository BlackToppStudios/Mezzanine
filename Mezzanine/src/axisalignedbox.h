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
#ifndef _axisalignedbox_h
#define _axisalignedbox_h

/// @file
/// @brief This file contains the AxisAlignedBox class for representing AABB's of objects in the world.

#include "vector3.h"
#include "plane.h"

namespace Ogre
{
    class AxisAlignedBox;
}

namespace Mezzanine
{
    class Plane;
    class Ray;
    class Sphere;
    ///////////////////////////////////////////////////////////////////////////////
    /// @brief This is a utility class used to represent the Axis-Aligned Bounding Boxes of objects in various subsystems.
    /// @details
    ///////////////////////////////////////
    class MEZZ_LIB AxisAlignedBox
    {
    public:
        /// @brief This enum is used to help make querys for data on the AABB.
        enum AxisExtent
        {
            AE_Min = 1,
            AE_Max = 2
        };
        /// @brief This is a type used for the return of a ray intersection test.
        /// @details This type provides more verbose return data that can be used for further tests.  @n @n
        /// The first member stores whether or not there was a hit.  The second member stores ray containing
        /// the points where the ray entered and exited the AABB.
        typedef std::pair<Boole,Ray> RayTestResult;

        /// @brief Used to uniquely identify sides of an axis aligned box.
        enum Side
        {
            SideMinX = 0,
            SideMinY = 1,
            SideMinZ = 2,
            SideMaxX = 3,
            SideMaxY = 4,
            SideMaxZ = 5
        };


        ///////////////////////////////////////////////////////////////////////////////
        // Public Data Members

        /// @internal
        /// @brief The minimum extents on each axis in world space.
        Vector3 MinExt;
        /// @internal
        /// @brief The maximum extents on each axis in world space.
        Vector3 MaxExt;

        ///////////////////////////////////////////////////////////////////////////////
        // Construction and Destruction

        /// @brief Blank constructor.
        AxisAlignedBox();
        /// @brief Copy constructor.
        /// @param Other The other AABB to copy.
        AxisAlignedBox(const AxisAlignedBox& Other);
        /// @brief Extents constructor.
        /// @param Min The minimum extents on each axis in world space.
        /// @param Max The maximum extents on each axis in world space.
        AxisAlignedBox(const Vector3& Min, const Vector3& Max);
        /// @brief Internal constructor.
        /// @param InternalAABB The internal Ogre AxisAlignedBox to construct this AxisAlignedBox from.
        explicit AxisAlignedBox(const Ogre::AxisAlignedBox& InternalAABB);
        /// @brief Class destructor.
        ~AxisAlignedBox();

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        /// @brief Gets whether or not this AABB has no size.
        /// @return Returns true if this AABB has no size/is invalid, false otherwise.
        Boole IsZero() const;
        /// @brief Gets the total volume of this AABB.
        /// @return Returns the volume of this AABB expressed as cubic length units.
        Real GetVolume() const;
        /// @brief Gets an AABB that has the dimensions of the overlap between this AABB and another AABB.
        /// @param Other The AABB to get the overlap for.
        /// @return Returns a new AABB containing the overlap between this and another AABB.
        AxisAlignedBox GetOverlap(const AxisAlignedBox& Other) const;
        /// @brief Expands the size of this AABB to encompass it's current bounds plus a point in 3D space.
        /// @param Point The point in 3D space to encompass.
        void Expand(const Vector3& Point);
        /// @brief Expands the size of this AABB to encompass it's current bounds plus another AABB.
        /// @param Other The other AABB to encompass.
        void Expand(const AxisAlignedBox& Other);

        /// @brief Checks to see if a point is inside this AABB.
        /// @param ToCheck The location to check to see if it is within this AABB.
        /// @return Returns true if the point provided is within this AABB, false otherwise.
        Boole IsInside(const Vector3& ToCheck) const;
        /// @brief Checks to see if a sphere overlaps with this AABB.
        /// @param ToCheck The sphere to check for overlap.
        /// @return Returns true if the provided sphere overlaps with this AABB, false otherwise.
        Boole IsOverlapping(const Sphere& ToCheck) const;
        /// @brief Checks to see if another AABB overlaps with this one.
        /// @param ToCheck The other AABB to check for overlap.
        /// @return Returns true if the two AABB's overlap, false otherwise.
        Boole IsOverlapping(const AxisAlignedBox& ToCheck) const;
        /// @brief Checks to see if a plane intersects this AABB.
        /// @param ToCheck The plane to check for intersection.
        /// @return Returns true if the provided plane intersects with this AABB, false otherwise.
        Boole IsOverlapping(const Plane& ToCheck) const;
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

        /// @brief Gets the size of this AABB.
        /// @return Returns a Vector3 representing the size of this AABB.
        Vector3 GetSize() const;
        /// @brief Gets half the size of this AABB.
        /// @return Returns a Vector3 representing half the size of this AABB.
        Vector3 GetHalfSize() const;
        /// @brief Gets the center of this AABB.
        /// @return Returns a Vector3 containing the center of this AABB.
        Vector3 GetCenter() const;
        /// @brief Gets the location of the specified corner.
        /// @param XEx The extent for the X axis to retrieve.
        /// @param YEx The extent for the Y axis to retrieve.
        /// @param ZEx The extent for the Z axis to retrieve.
        /// @return Returns a Vector3 representing the position of the specified corner.
        Vector3 GetCorner(const AxisExtent XEx, const AxisExtent YEx, const AxisExtent ZEx) const;


        Real GetSideExtent(Side WhichSideExtent) const;
        /// @brief Get a plane corresponding to a side of the box.
        /// @param WhichSidePlane Which side do you want a plane for?
        Plane GetSidePlane(Side WhichSidePlane) const;

        ///////////////////////////////////////////////////////////////////////////////
        // Conversion Methods

        /// @brief Changes this AxisAlignedBox to match the Ogre AxisAlignedBox.
        /// @param InternalAABB The Ogre::AxisAlignedBox to copy.
        void ExtractOgreAABB(const Ogre::AxisAlignedBox& InternalAABB);
        /// @brief Gets an Ogre::AxisAlignedBox that contains this Spheres information.
        /// @return This returns an Ogre::AxisAlignedBox that contains the same information as this AxisAlignedBoxes information.
        Ogre::AxisAlignedBox GetOgreAABB() const;

        ///////////////////////////////////////////////////////////////////////////////
        // Serialization

        /// @brief Convert this class to an XML::Node ready for serialization.
        /// @param ParentNode The point in the XML hierarchy that all this shape should be appended to.
        void ProtoSerialize(XML::Node& ParentNode) const;
        /// @brief Take the data stored in an XML Node and overwrite this object with it.
        /// @param SelfRoot An XML::Node containing the data to populate the new instance with.
        void ProtoDeSerialize(const XML::Node& SelfRoot);

        /// @brief Get the name of the the XML tag this class will leave behind as its instances are serialized.
        /// @return A string containing the name of this class.
        static String GetSerializableName();

        ///////////////////////////////////////////////////////////////////////////////
        // Operators

        /// @brief Assignment operator.
        /// @param Other The other AABB to copy from.
        void operator=(const AxisAlignedBox& Other);

        /// @brief The assignment operator from Ogre::AxisAlignedBox to Mezzanine::AxisAlignedBox.
        /// @param InternalAABB The Ogre::AxisAlignedBox to take data from.
        void operator=(const Ogre::AxisAlignedBox& InternalAABB);

        /// @brief Greater-than operator.
        /// @note This operator compares the volume of both AABBs.
        /// @param Other The other AABB to compare with.
        /// @return Returns true if this AABB is larger than the other provided AABB, false otherwise.
        Boole operator>(const AxisAlignedBox& Other) const;
        /// @brief Greater-than or equals-to operator.
        /// @note This operator compares the volume of both AABBs.
        /// @param Other The other AABB to compare with.
        /// @return Returns true if this AABB is larger than or equal to the other provided AABB, false otherwise.
        Boole operator<(const AxisAlignedBox& Other) const;
        /// @brief Less-than operator.
        /// @note This operator compares the volume of both AABBs.
        /// @param Other The other AABB to compare with.
        /// @return Returns true if this AABB is smaller than the other provided AABB, false otherwise.
        Boole operator>=(const AxisAlignedBox& Other) const;
        /// @brief Less-than or equals-to operator.
        /// @note This operator compares the volume of both AABBs.
        /// @param Other The other AABB to compare with.
        /// @return Returns true if this AABB is smaller than or equal to the other provided AABB, false otherwise.
        Boole operator<=(const AxisAlignedBox& Other) const;

        /// @brief Equality operator.
        /// @param Other The other AABB to compare with.
        /// @return Returns true if this AABB is the same as the other provided AABB, false otherwise.
        Boole operator==(const AxisAlignedBox& Other) const;
        /// @brief Inequality operator.
        /// @param Other The other AABB to compare with.
        /// @return Returns true if this AABB is not the same as the other provided AABB, false otherwise.
        Boole operator!=(const AxisAlignedBox& Other) const;
    };//AxisAlignedBox
}//Mezzanine

#endif
