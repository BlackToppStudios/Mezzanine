//© Copyright 2010 - 2011 BlackTopp Studios Inc.
/* This file is part of The PhysGame Engine.

    The PhysGame Engine is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The PhysGame Engine is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with The PhysGame Engine.  If not, see <http://www.gnu.org/licenses/>.
*/
/* The original authors have included a copy of the license specified above in the
   'Docs' folder. See 'gpl.txt'
*/
/* We welcome the use of The PhysGame anyone, including companies who wish to
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

namespace Ogre
{
    class Ray;
}


namespace phys
{
    ///////////////////////////////////////////////////////////////////////////////
    /// @class Ray
    /// @headerfile ray.h
    /// @brief This is used to indicate a line with one end.
    /// @details Currently this is just a collection of two vector3s with a couple
    /// of constructors.
    class PHYS_LIB Ray
    {
        public:
            /// @brief The origin point of the Vector
            Vector3 From;

            /// @brief A point on the line of the ray.
            Vector3 To;

            ///////////////////////////////////////////////////////////////////////////////
            // Constructors

            /// @brief Default constructor
            /// @details This create a ray starting at 0,0,0 pointing to 0,1,0,
            Ray();

            /// @brief Thorough constructor
            /// @details This accepts 2 Vector3s and uses them to build the ray
            /// @param From_ The origin fo the new Ray
            /// @param To_ A point along the line for the destination line.
            Ray(Vector3 From_, Vector3 To_);

            /// @brief Compatibily constructor
            /// @details This accepts an Ogre Ray, (graphics subsystem) to make ray
            /// @param Ray2 This is the Ogre::Ray
            Ray(Ogre::Ray Ray2);

            ///////////////////////////////////////////////////////////////////////////////
            // Manual Conversions and adjsutments

            /// @brief Gets an Ogre::Ray that contains this Rays information
            /// @details Used to aid interopability, this will return afresh Ogre::Ray with the same data as this Ray
            /// @return This returns an Ogre::Ray that contains the same information as this rays information
            Ogre::Ray GetOgreRay() const;

            /// @brief Measures the distance distance of this ray
            /// @details This uses the distance method of Vector3 to determine the current length of this ray.
            /// @return This returns a real value which contains the length
            Real Length() const;

            /// @brief This returns the normal for this ray
            /// @details This will get a ray that is 1 unit in length with the same From point as this ray, pointing in the same direction as this ray.
            /// @return At a ray that is the normal for this array
            Ray GetNormal() const;

            /// @brief Turns this Ray into its own normal
            /// @details This will change the ray to be exactly 1 unit long, but pointing the same direction.
            // /// @return The is the length of the Ray after Normalizing. This could be useful for finding problems with 0 length rays.
            void Normalize();

            ///////////////////////////////////////////////////////////////////////////////
            // Real Operators and assignments

            /// @brief This returns a ray lengthened by the given multiple.
            /// @details This returns a ray changed in length by the amount in the scalar. Specifically this subracts the From value
            /// out of the To value, then multiplies the To value by the scalar, then adds the from value back in.
            /// @return This returns a copy of the ray scaled by the requested amount.
            /// @param scalar The amount to multiply by.
            Ray operator* (const Real &scalar) const;

            /// @brief This returns a ray scaled by the given divisor.
            /// @details This returns an ray divided in length by the amount in the scalar. Specifically this subracts the From value
            /// out of the To value, then divides the To value by the scalar, then adds the from value back in.
            /// @return This returns a copy of the ray scaled by the requested amount.
            /// @param scalar The amount to divide by.
            Ray operator/ (const Real &scalar) const;

            /// @brief This lengthens the Ray by the given multiple.
            /// @details This ray  ischanged in length by the amount in the scalar. Specifically this subracts the From value
            /// out of the To value, then multiplies the To value by the scalar, then adds the from value back in.
            /// @param scalar The amount to multiply by.
            void operator*= (const Real &scalar);

            /// @brief This scales the Ray by the given divisor.
            /// @details This ray is divided in length by the amount in the scalar. Specifically this subracts the From value
            /// out of the To value, then divides the To value by the scalar, then adds the from value back in.
            /// @param scalar The amount to divide by.
            void operator/= (const Real &scalar);

    }; // /Ray

}// /phys

/// @brief Streaming output operator
/// @details This converts the data of the Ray into a stream Ideal for sending to a log or cout
/// @param stream This is the stream we send our data to.
/// @return This returns an std::ostream which now contains our data.
std::ostream& PHYS_LIB operator << (std::ostream& stream, const phys::Ray& x);


#endif
