//© Copyright 2010 - 2012 BlackTopp Studios Inc.
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

#include "crossplatformexport.h"
#include "vector3.h"
#include "xml/xml.h"

namespace Ogre
{
    class Plane;
}

namespace Mezzanine
{
    ///////////////////////////////////////////////////////////////////////////////
    /// @class Plane
    /// @headerfile plane.h
    /// @brief This is used to represent a flat infinite slice of the game world
    /// @details The Normal value represents how rotated the plane will be, and
    /// The Distance with represent how far you need to move down a line perpendicular
    /// to the plane, (ie the normal, which is defined by the Normal value) from the
    /// Origin.
    class MEZZ_LIB Plane
    {
        public:
            /// @brief The rotation of the plane
            Vector3 Normal;

            /// @brief How from from the origin the plane is
            Real Distance;

            ///////////////////////////////////////////////////////////////////////////////
            // Constructors
            /// @brief Default constructor
            /// @details This create a Plane with gimbals 0,0,0 and 0 distance from the Origin
            Plane();

            /// @brief Thorough constructor
            /// @details This accepts 2 Vector3s and uses them to build the ray
            /// @param Normal_ The rotation of the plane
            /// @param Distance_ Distance from origin to the plane
            Plane(Vector3 Normal_, Real Distance_);

            /// @brief Compatibily constructor
            /// @details This accepts an Ogre Plane, (graphics subsystem) to make this Plane
            /// @param Plane_ This is the Ogre::Plane
            explicit Plane(Ogre::Plane Plane_);

            /// @brief Triangle constructor
            /// @details This determines the plane the triangle this resides on and uses that plane here
            /// @param rkPoint0 This is one point in the triangle
            /// @param rkPoint1 This is another point in the triangle
            /// @param rkPoint2 This is one point in the triangle
            Plane(const Vector3& rkPoint0, const Vector3& rkPoint1, const Vector3& rkPoint2);

            ///////////////////////////////////////////////////////////////////////////////
            // Conversions and adjustments
            /// @brief Gets an Ogre::Plane that contains this Plane's information
            /// @details Used to aid interopability, this will return a fresh Ogre::Plane with the same data as this Plane
            /// @return This returns an Ogre::Plnae that contains the same information as this Plane's information
            Ogre::Plane GetOgrePlane() const;

            /// @brief Changes this Plane to match the Ogre Plane
            /// @details Used to aid interopability, this will result in this Plane exactly matching the Ogre::Plane
            /// @param Plane2 The Ogre::Plane to copy
            void ExtractOgrePlane(const Ogre::Plane& Plane2);

            /// @brief The assignment operator from Ogre::Plane to Mezzanine::Plane
            /// @details this simply call Mezzanine::Plane::ExtractOgrePlane
            /// @param Plane2 The Ogre::Plane to take data from.
            void operator=(const Ogre::Plane& Plane2);
    }; // /Plane
}// /Mezz

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
Mezzanine::XML::Node& MEZZ_LIB operator >> (const Mezzanine::XML::Node& OneNode, Mezzanine::Plane& x);


#endif
