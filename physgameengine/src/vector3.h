//© Copyright 2010 BlackTopp Studios Inc.
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
#ifndef _vector3_h
#define _vector3_h

#include "datatypes.h"

//Forward Declarations for wierd compatibility functions
class btVector3;
namespace Ogre
{
    class Vector3;
}

namespace phys
{
    ///////////////////////////////////////////////////////////////////////////////
    /// @class Vector3
    /// @headerfile vector3.h
    /// @brief This is used to represent a point in space, or a vector through space
    /// @details This contains an X, Y and a Z value used to represent coordinates.
    /// This also has a number of facilities to make converting from Physics subsystem
    /// vectors or graphics subsystems as easy as possible
    class Vector3
    {
    public:

        ///////////////////////////////////////////////////////////////////////////////
        //`The Essentials

        /// @brief Coordinate on the X vector.
        Real X;
        /// @brief Coordinate on the Y vector.
        Real Y;
        /// @brief Coordinate on the Z vector.
        Real Z;

        /// @brief Default Constructor.
        /// @details Basic all zero initialization constructor.
        Vector3();

        /// @brief Real value Constructor.
        /// @details Constructor that sets all three vectors.
        /// @param X Coordinate on the X vector.
        /// @param Y Coordinate on the Y vector.
        /// @param Z Coordinate on the Z vector.
        Vector3(Real X, Real Y, Real Z);

        /// @brief Ogre Value Constructor.
        /// @details Constructor that sets all values to match the Ogre vector.
        /// @param Vec The vector to be copied to make this vector
        Vector3(Ogre::Vector3 Vec);

        /// @brief Bullet Value Constructor.
        /// @details Constructor that sets all values to match the Bullet vector.
        /// @param Vec The vector to be copied to make this vector
        Vector3(btVector3 Vec);

        ///////////////////////////////////////////////////////////////////////////////
        // Assignment Operators

        /// @brief Assignment operator to convert from Bullet Vectors
        /// @details This copies the x,y and z values from the bullet into this vector
        /// @param bt3 This is a btVector3 that will be copied
        void operator= (const btVector3 &bt3);

        /// @brief Assignment operator to convert from Ogre Vectors
        /// @details This copies the x,y and z values from the bullet into this vector
        /// @param OVec3 This is a Ogre::Vector3 that will be copied.
        void operator= (const Ogre::Vector3 &OVec3);

        ///////////////////////////////////////////////////////////////////////////////
        // Equality Comparison operators

        /// @brief Equality Comparison Operator
        /// @details Returns true if X==X, Y==Y and Z==Z. If any of those do not match this returns false.
        /// @param Vec2 This is the other phys::Vector3.
        bool operator== (const phys::Vector3 &Vec2);

        /// @brief Equality Comparison Operator
        /// @details Returns true if X==getX(), Y==getY() and Z==getZ(). If any of those do not match this returns false.
        /// @param Vec2 This is an btVector3 that needs to be compared with this.
        bool operator== (const btVector3 &Vec2);

        /// @brief Equality Comparison Operator
        /// @details Returns true if X==x, Y==y and Z==z. If any of those do not match this returns false.
        /// @param Vec2 This is an Ogre::Vector3 that needs to be compared with this.
        bool operator== (const Ogre::Vector3 &Vec2);

        ///////////////////////////////////////////////////////////////////////////////
        // Unary Operators
        /// @brief Additive Inverse Operator
        /// @details Returns the opposite Vector3 relative to 0,0,0
        Vector3 operator- ();

        ///////////////////////////////////////////////////////////////////////////////
        // Vector3 Arithmetic with Real
        /// @brief Scaling by multiplication
        /// @details This Multiplies X, Y and Z by scalar
        /// @return This returns a Vector3 that has been scaled
        /// @param scalar This is the amount to scale the Vector3 by
        Vector3 operator* (const Real &scalar) const;

        /// @brief Scaling by Division
        /// @details This Diisionn X, Y and Z by scalar
        /// @return This returns a Vector3 that has been scaled
        /// @param scalar This is the amount to scale the Vector3 by
        Vector3 operator/ (const Real &scalar) const;

        ///////////////////////////////////////////////////////////////////////////////
        // Vector3 Arithmetic and assignment with Real
        /// @brief Scaling by multiplication.
        /// @details This Multiplies X, Y and Z by scalar and stores the changes in this Vector3.
        /// @param scalar This is the amount to scale the Vector3 by.
        void operator*= (const Real &scalar);

        /// @brief Scaling by Division
        /// @details This Division X, Y and Z by scalar and and stores the changes in this Vector3.
        /// @param scalar This is the amount to scale the Vector3 by
        void operator/= (const Real &scalar);

        ///////////////////////////////////////////////////////////////////////////////
        // Arithmetic Operators
        /// @brief Addition Operator
        /// @details Allows for addition from a phys::Vector3
        /// @param Vec2 This is the other phys::Vector3
        Vector3 operator+ (const Vector3 &Vec2) const;

        /// @brief Subraction Operator
        /// @details Allows for subtraction from a phys::Vector3
        /// @param Vec2 This is the other phys::Vector3
        Vector3 operator- (const Vector3 &Vec2) const;

        /// @brief Multiplaction Operator
        /// @details Allows for multiplaction from a phys::Vector3
        /// @param Vec2 This is the other phys::Vector3
        Vector3 operator* (const Vector3 &Vec2) const;

        /// @brief Division Operator
        /// @details Allows for division from a phys::Vector3
        /// @param Vec2 This is the other phys::Vector3
        Vector3 operator/ (const Vector3 &Vec2) const;

        /////////////////////////////////////////////////////////////////////
        // Arithmetic Operators with btVector3
        /// @brief Bullet Addition Operator
        /// @details Allows for addition  between a phys::Vector3 and a btVector3
        /// @param Vec2 This is the btVector3 to be added
        Vector3 operator+ (const btVector3  &Vec2);

        /// @brief Bullet Subtraction Operator
        /// @details Allows for subtraction  between a phys::Vector3 and a btVector3
        /// @param Vec2 This is the btVector3 to be subtracted
        Vector3 operator- (const btVector3  &Vec2);

        /// @brief Bullet Multiplication Operator
        /// @details Allows for multiplication  between a phys::Vector3 and a btVector3
        /// @param Vec2 This is the btVector3 to be multiplied
        Vector3 operator* (const btVector3  &Vec2);

        /// @brief Bullet Division Operator
        /// @details Allows for division  between a phys::Vector3 and a btVector3
        /// @param Vec2 This is the btVector3 to be divided
        Vector3 operator/ (const btVector3  &Vec2);

        ///////////////////////////////////////////////////////////////////////////////
        // Arithmetic Operators with Ogre::Vector3
        /// @brief Ogre Addition Operator
        /// @details Allows for addition  between a phys::Vector3 and a Ogre::Vector3
        /// @param Vec2 This is the Ogre::Vector3 to be added
        Vector3 operator+ (const Ogre::Vector3 &Vec2);

        /// @brief Bullet Subtraction Operator
        /// @details Allows for subtraction  between a phys::Vector3 and a Ogre::Vector3
        /// @param Vec2 This is the Ogre::Vector3 to be subtracted
        Vector3 operator- (const Ogre::Vector3 &Vec2);

        /// @brief Bullet Multiplication Operator
        /// @details Allows for multiplying  between a phys::Vector3 and a Ogre::Vector3
        /// @param Vec2 This is the Ogre::Vector3 to be multiplied
        Vector3 operator* (const Ogre::Vector3 &Vec2);

        /// @brief Bullet Division Operator
        /// @details Allows for division  between a phys::Vector3 and a Ogre::Vector3
        /// @param Vec2 This is the Ogre::Vector3 to be divided
        Vector3 operator/ (const Ogre::Vector3 &Vec2);

        ///////////////////////////////////////////////////////////////////////////////
        // Fancy Math

        /// @brief This is used to calculate the crossproduct of this and another vector
        /// @details This creates a third vector, which should be on a line perpendicular
        /// to lines that contain the origin and the other vectors \n\n
        /// Thanks to the guys at Ogre3d for the well written version of this function
        /// that we based this on.
        /// @param rkVector the Vector to work with to create the cross product
        /// @return This is the crossproduct of this vector and rkVector
        Vector3 CrossProduct( const Vector3& rkVector ) const;

        /// @brief This is used to calculate the dotproduct of this and another vector
        /// @details This calculates the sum of the products of X, Y and Z. \n\n
        /// Thanks to the guys at Ogre3d for the well written version of this function
        /// that we based this on.
        /// @param vec The vector to work with to create the cross product
        /// @return This is the dotproduct of this vector and vec
        Real dotProduct(const Vector3& vec) const;

        /// @brief This will change this point into it's own normal relative to the origin
        /// @details This will change this vector into one that is the same direction from the origin, but only one unit a away.
        void Normalize();

        /// @brief This returns the normal for this relative to the origin
        /// @details This will return a vector that is 1 unit in away from the origin, if a line were starting and the origin it would pass through
        /// both the normal and the original point.
        /// @return At a vector3 that is the normal of this Vector3 or 0,0,0 if the current Vector is all 0s
        Vector3 GetNormal() const;

        /// @brief This will inverse the reals in the vector.
        /// @details This function will inverse all the reals in the vector.
        void Inverse();

        ///////////////////////////////////////////////////////////////////////////////
        // Manual Conversions
        /// @brief Gets a Bullet vector3.
        /// @details Creates a Bullet vector3 with values equal to this class and returns it.
        btVector3 GetBulletVector3() const;

        /// @brief Copies an existing Bullet vector3.
        /// @details This function will copy the values stored in an existing Bullet vector3
        /// and set the values of this class to be the same.
        /// @param temp The vector3 to be extracted.
        void ExtractBulletVector3(btVector3 temp);

        /// @brief Gets a Ogre vector3.
        /// @details Creates a Ogre vector3 with values equal to this class and returns it.
        Ogre::Vector3 GetOgreVector3() const;

        /// @brief Copies an existing Ogre vector3.
        /// @details This function will copy the values stored in an existing Ogre vector3
        /// and set the values of this class to be the same.
        /// @param temp The vector3 to be extracted.
        void ExtractOgreVector3(Ogre::Vector3 temp);

        /// @brief This return the distance between this point and another
        /// @details This uses a 3d extension of pythagoras thereom to calculate the distance between
        /// This Vector3 and another.
        /// @param Vec2 this is the other point to measure against
        /// @return This returns a Real number which is the distance.
        Real Distance(const Vector3 &Vec2) const;

    };
}// /phys

///////////////////////////////////////////////////////////////////////////////
// Class External << Operators for streaming or assignment

std::ostream& operator << (std::ostream& stream, const phys::Vector3& x);

Ogre::Vector3& operator << (Ogre::Vector3& VecTo, const phys::Vector3& VecFrom);
Ogre::Vector3& operator << (Ogre::Vector3& VecTo, const btVector3& VecFrom);

btVector3& operator << (btVector3& VecTo, const Ogre::Vector3& VecFrom);
btVector3& operator << (btVector3& VecTo, const phys::Vector3& VecFrom);

phys::Vector3& operator << (phys::Vector3& VecTo, const Ogre::Vector3& VecFrom);
phys::Vector3& operator << (phys::Vector3& VecTo, const btVector3& VecFrom);

#endif
