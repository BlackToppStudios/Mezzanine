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
#ifndef _plane_h
#define _plane_h

#include "vector3.h"

#ifndef SWIG
    #include "XML/xml.h"
#endif

namespace Ogre
{
    class Plane;
}

namespace Mezzanine
{
    class AxisAlignedBox;
    class Ray;
    class Sphere;
    ///////////////////////////////////////////////////////////////////////////////
    /// @brief This is used to represent a flat infinite slice of the game world
    /// @details The Normal value represents how rotated the plane will be, and
    /// The Distance with represent how far you need to move down a line perpendicular
    /// to the plane, (ie the normal, which is defined by the Normal value) from the
    /// Origin.
    class MEZZ_LIB Plane
    {
    public:
        /// @enum Side
        /// @brief An enum used to describe which side of the plane the result of a query is on.
        enum Side
        {
            S_None     = 0,
            S_Positive = 1,
            S_Negative = 2,
            S_Both     = 3
        };
        /// @brief This is a type used for the return of a ray intersection test.
        /// @details This type provides more verbose return data that can be used for further tests.  @n @n
        /// The first member stores whether or not there was a hit.  The second member stores the point where it was hit.
        typedef std::pair<Boolean,Vector3> RayTestResult;

        ///////////////////////////////////////////////////////////////////////////////
        // Public Data Members

        /// @brief The rotation of the plane
        Vector3 Normal;
        /// @brief How from from the origin the plane is
        Real Distance;

        ///////////////////////////////////////////////////////////////////////////////
        // Construction and Destruction

        /// @brief Default constructor
        Plane();
        /// @brief Copy constructor.
        /// @param Other The other plane to copy from.
        Plane(const Plane& Other);
        /// @brief Descriptive constructor.
        /// @param Norm The positive direction of the plane.
        /// @param Constant The Constant with which to project the plane.
        Plane(const Vector3& Norm, const Real Constant);
        /// @brief Dual Vector constructor.
        /// @param Norm The positive direction of the plane.
        /// @param Point The "origin" point of the plane in 3D space.
        Plane(const Vector3& Norm, const Vector3& Point);
        /// @brief Triangle constructor.
        /// @param First This is one point in the triangle.
        /// @param Second This is another point in the triangle.
        /// @param Third This is one point in the triangle.
        Plane(const Vector3& First, const Vector3& Second, const Vector3& Third);
        /// @brief Compatibility constructor.
        /// @param InternalPlane This is the Ogre::Plane to copy from.
        explicit Plane(const Ogre::Plane& InternalPlane);
        /// @brief Class destructor.
        ~Plane();

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        /// @brief Defines the dimensions of this plane explicitly.
        /// @param Norm The positive direction of the plane.
        /// @param Constant The Constant with which to project the plane.
        void Define(const Vector3& Norm, const Real Constant);
        /// @brief Defines the dimensions of this plane based on an origin and direction.
        /// @param Norm The positive direction of the plane.
        /// @param Point The "origin" point of the plane in 3D space.
        void Define(const Vector3& Norm, const Vector3& Point);
        /// @brief Defines the dimensions of this plane from a triangle in 3D space.
        /// @param First This is one point in the triangle.
        /// @param Second This is another point in the triangle.
        /// @param Third This is one point in the triangle.
        void Define(const Vector3& First, const Vector3& Second, const Vector3& Third);

        /// @brief Gets which side of the plane a point in 3D space is.
        /// @param Point The point in 3D space to determine on which side of the plane it lies.
        /// @return Returns a Side enum value, indicating which side of the plane the point is on.
        Side GetSide(const Vector3& Point) const;
        /// @brief Gets which side of the plane a box shape is.
        /// @param Center The point in 3D space where the center of the box is.
        /// @param HalfSize Half of the total size on each axis.
        /// @return Returns a Side enum value indicating which side of the plane the box is on.
        Side GetSide(const Vector3& Center, const Vector3& HalfSize) const;
        /// @brief Gets the distance from the plane to a point in 3D space.
        /// @note This function will only return a true unit distance if the Normal member of this class is properly normalized.  Take care when altering it's value.
        /// @param Point The point in 3D space to get the distance to.
        /// @return Returns the distance from the plane to the specified point.  Positive values mean the point is on the positive side, and vice versa.
        Real GetDistance(const Vector3& Point) const;

        /// @brief Checks to see if a sphere overlaps with this Plane.
        /// @param ToCheck The sphere to check for overlap.
        /// @return Returns true if the provided sphere overlaps with this Plane, false otherwise.
        Boolean IsOverlapping(const Sphere& ToCheck) const;
        /// @brief Checks to see if an AABB overlaps with this Plane.
        /// @param ToCheck The AABB to check for overlap.
        /// @return Returns true if the AABB overlaps with this Plane, false otherwise.
        Boolean IsOverlapping(const AxisAlignedBox& ToCheck) const;
        /// @brief Checks to see if another Plane intersects this one.
        /// @param ToCheck The other Plane to check for intersection.
        /// @return Returns true if the provided Plane intersects with this Plane, false otherwise.
        Boolean IsOverlapping(const Plane& ToCheck) const;
        /// @brief Checks to see if a ray intersects this Plane.
        /// @param ToCheck The ray to check for a hit.
        /// @return Returns true if the ray intersects this Plane, false otherwise.
        RayTestResult Intersects(const Ray& ToCheck) const;

        ///////////////////////////////////////////////////////////////////////////////
        // Conversion Methods

        /// @brief Changes this Plane to match the Ogre Plane.
        /// @param InternalPlane The Ogre::Plane to copy.
        void ExtractOgrePlane(const Ogre::Plane& InternalPlane);
        /// @brief Gets an Ogre::Plane that contains this Planes information.
        /// @return This returns an Ogre::Plane that contains the same information as this Planes information.
        Ogre::Plane GetOgrePlane() const;

        ///////////////////////////////////////////////////////////////////////////////
        // Serialization

        /// @brief Convert this class to an XML::Node ready for serialization.
        /// @param ParentNode The point in the XML hierarchy that all this renderable should be appended to.
        void ProtoSerialize(XML::Node& ParentNode) const;
        /// @brief Take the data stored in an XML Node and overwrite this object with it.
        /// @param SelfRoot An XML::Node containing the data to populate this class with.
        void ProtoDeSerialize(const XML::Node& SelfRoot);

        /// @brief Get the name of the the XML tag the proxy class will leave behind as its instances are serialized.
        /// @return A string containing the name of this class.
        static String GetSerializableName();

        ///////////////////////////////////////////////////////////////////////////////
        // Operators

        /// @brief Assignment operator.
        /// @param Other The other Plane to copy from.
        void operator=(const Plane& Other);

        /// @brief The assignment operator from Ogre::Plane to Mezzanine::Plane.
        /// @param InternalPlane The Ogre::Plane to take data from.
        void operator=(const Ogre::Plane& InternalPlane);

        /// @brief Equality operator.
        /// @param Other The other Plane to compare with.
        /// @return Returns true if this Plane is the same as the other provided Plane, false otherwise.
        Boolean operator==(const Plane& Other) const;
        /// @brief Inequality operator.
        /// @param Other The other Plane to compare with.
        /// @return Returns true if this Plane is not the same as the other provided Plane, false otherwise.
        Boolean operator!=(const Plane& Other) const;
    };//Plane
}//Mezzanine

#ifndef SWIG
/// @brief Streaming output operator
/// @details This converts the data of the Plane into a snippet of xml Ideal for sending to a log or cout.
/// @param stream This is the stream we send our data to.
/// @return This returns an std::ostream which now contains our data.
std::ostream& MEZZ_LIB operator << (std::ostream& stream, const Mezzanine::Plane& x);

/// @brief Used to de-serialize from a stream.
/// @details This reads XML input and attempts to Populate a plane.
/// @param stream This is the stream we get our data from.
/// @param x The Mezzanine::Plane to store the deserialized Plane in.
/// @return This returns an std::ostream to allow operator chaining.
/// @throw Can throw any exception that any function in the Mezzanine::xml namespace could throw in addition to a Mezzanine::Exception if the serialization version doesn't match.
std::istream& MEZZ_LIB operator >> (std::istream& stream, Mezzanine::Plane& x);

/// @brief Converts an XML node into a Mezzanine::Plane
/// @details TThis will convert an XML::Node will a valid serialized Mezzanine::Vector3 into a Mezzanine::Vector3
/// @param OneNode This is the XML::Node we get our data from. It should contain a serialized Mezzanine::Plane.
/// @param x The Mezzanine::Plane to store the deserialized Plane in.
/// @return This returns an XML::Node refernce to allow operator chaining.
/// @throw Can throw any exception that any function in the Mezzanine::xml namespace could throw in addition to a Mezzanine::Exception if the serialization version doesn't match.
void MEZZ_LIB operator >> (const Mezzanine::XML::Node& OneNode, Mezzanine::Plane& x);
#endif

#endif
