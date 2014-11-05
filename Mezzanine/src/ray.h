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
#ifndef _ray_h
#define _ray_h

#include "crossplatformexport.h"
#include "vector3.h"
#include "mathtools.h"

namespace Ogre
{
    class Ray;
}


namespace Mezzanine
{
    class AxisAlignedBox;
    class Plane;
    class Sphere;
    ///////////////////////////////////////////////////////////////////////////////
    /// @brief This represents a line placed in 3D space and is used with spacial queries.
    /// @details
    class MEZZ_LIB Ray
    {
    public:
        /// @brief This is a type used for the ray intersection tests performed on Planes.
        /// @details This type provides more verbose return data that can be used for further tests.
        typedef std::pair<Boole,Vector3> PlaneRayTestResult;
        /// @brief This is a type used for the return of a ray intersection test.
        /// @details This type provides more verbose return data that can be used for further tests.
        //typedef MathTools::PointPointInterection GeometryRayTestResult;
        typedef MathTools::GeometryRayTestResult GeometryRayTestResult;
        ///////////////////////////////////////////////////////////////////////////////
        // Public Data Members

        /// @brief The origin point of the Vector.
        Vector3 Origin;
        /// @brief The direction this ray is pointing.
        Vector3 Normal;

        ///////////////////////////////////////////////////////////////////////////////
        // Construction and Destruction

        /// @brief Default constructor.
        /// @details This create a ray starting at 0,0,0 pointing to 0,1,0.
        Ray();
        /// @brief Copy constructor.
        /// @param Other The other Ray to copy from.
        Ray(const Ray& Other);
        /// @brief Destination constructor.
        /// @details This keeps the origin at 0,0,0.
        /// @param Dir The direction this Ray is pointing in.
        Ray(const Vector3& Dir);
        /// @brief Descriptive constructor.
        /// @param From The origin for the new Ray.
        /// @param Dir The direction this Ray is pointing in.
        Ray(const Vector3& From, const Vector3& Dir);
        /// @brief Internal constructor.
        /// @param InternalRay This is the Ogre::Ray to copy from.
        explicit Ray(const Ogre::Ray& InternalRay);
        /// @brief Class destructor.
        ~Ray();

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        /// @brief Gets the normal of this ray.
        /// @return Returns a vector3 that is the direction this ray is pointing.
        const Vector3& GetNormal() const;
        /// @brief Gets the origin of this ray.
        /// @return Returns a vector3 containing the origin of this ray.
        const Vector3& GetOrigin() const;
        /// @brief Gets a point on the ray at a distance.
        /// @param Distance The distance from the origin to get the position of.
        /// @return Returns a point in 3D space that is on the ray at the specified distance from the origin.
        Vector3 GetPointAtDistance(const Real& Distance);

        /// @brief Checks to see if this ray intersects a plane.
        /// @param ToCheck The plane to check for a hit.
        /// @return Returns a std::pair containing whether or not the ray hit, and if it did the point in 3D space where it hit.
        PlaneRayTestResult Intersects(const Plane& ToCheck) const;
        /// @brief Checks to see if this ray intersects a sphere.
        /// @param ToCheck The sphere to check for a hit.
        /// @return Returns a std::pair containing whether or not the ray hit, and if it did the subsection of the ray that went through the sphere.
        GeometryRayTestResult Intersects(const Sphere& ToCheck) const;
        /// @brief Checks to see if this ray intersects an AABB.
        /// @param ToCheck The AABB to check for a hit.
        /// @return Returns a std::pair containing whether or not the ray hit, and if it did the subsection of the ray that went through the AABB.
        GeometryRayTestResult Intersects(const AxisAlignedBox& ToCheck) const;

        ///////////////////////////////////////////////////////////////////////////////
        // Conversion Methods

        /// @brief Changes this Ray to match the Ogre Ray.
        /// @param InternalRay The Ogre::Ray to copy.
        void ExtractOgreRay(const Ogre::Ray& InternalRay);
        /// @brief Gets an Ogre::Ray that contains this Rays information.
        /// @return This returns an Ogre::Ray that contains the same information as this Rays information.
        Ogre::Ray GetOgreRay() const;

        ///////////////////////////////////////////////////////////////////////////////
        // Serialization

        /// @brief Convert this class to an XML::Node ready for serialization.
        /// @param ParentNode The point in the XML hierarchy that all this ray should be appended to.
        void ProtoSerialize(XML::Node& ParentNode) const;
        /// @brief Take the data stored in an XML Node and overwrite this object with it.
        /// @param SelfRoot An XML::Node containing the data to populate this class with.
        void ProtoDeSerialize(const XML::Node& SelfRoot);

        /// @brief Get the name of the the XML tag this class will leave behind as its instances are serialized.
        /// @return A string containing the name of this class.
        static String GetSerializableName();

        ///////////////////////////////////////////////////////////////////////////////
        // Operators

        /// @brief Assignment operator.
        /// @param Other The other Ray to copy from.
        void operator=(const Ray& Other);

        /// @brief Equality operator.
        /// @param Other The other Ray to compare with.
        /// @return Returns true if this Ray is the same as the other provided Ray, false otherwise.
        Boole operator==(const Ray& Other) const;
        /// @brief Inequality operator.
        /// @param Other The other Ray to compare with.
        /// @return Returns true if this Ray is not the same as the other provided Ray, false otherwise.
        Boole operator!=(const Ray& Other) const;
    };//Ray
}//Mezzanine

#ifndef SWIG
/// @brief Streaming output operator
/// @details This converts the data of the Ray into a stream Ideal for sending to a log or cout
/// @param stream This is the stream we send our data to.
/// @return This returns an std::ostream which now contains our data.
std::ostream& MEZZ_LIB operator << (std::ostream& stream, const Mezzanine::Ray& x);
#endif

#endif
