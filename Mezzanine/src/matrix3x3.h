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
#ifndef _matrix3x3_h
#define _matrix3x3_h

#include "vector3.h"
#include "quaternion.h"

class btMatrix3x3;

namespace Ogre
{
    class Matrix3;
}

namespace Mezzanine
{
    /// @class Matrix3x3
    /// @headerfile matrix3x3.h
    /// @brief This is a 3x3 Matrix class used for representing rotations and scaling in an object.
    /// @details The utility of this class overlaps with that of the Quaternion, for the most part the use of
    /// either class is a matter of preference.  The Mezzanine engine for the most part prefer's use of the
    /// Quaternion class since it deals with fewer numbers, but this class is still made available for others
    /// that may prefer it.
    class MEZZ_LIB Matrix3x3
    {
        public:
            /// @brief The Matrix.  Fo' Reals.
            Real Matrix[3][3];

            /// @brief Non-Initialization constructor.
            Matrix3x3();

            /// @brief Class destructor.
            ~Matrix3x3();

            ///////////////////////////////////////////////////////////////////////////////
            // Additional Constructors

            /// @brief Explict Initialization constructor.
            /// @details Provides initialization for every number in the matrix.
            Matrix3x3(const Real& XX, const Real& XY, const Real& XZ, const Real& YX, const Real& YY, const Real& YZ, const Real& ZX, const Real& ZY, const Real& ZZ);

            /// @brief Euler Initialization constructor.
            /// @param Yaw The number of degree's on the Yaw in Degrees to rotate.
            /// @param Pitch The number of degree's on the Pitch in Degrees to rotate.
            /// @param Roll The number of degree's on the Roll in Degrees to rotate.
            Matrix3x3(const Real& Yaw, const Real& Pitch, const Real& Roll);

            /// @brief Quaternion Initialization constructor.
            /// @param Rot The rotation to apply to this Matrix expressed as a Quaternion.
            Matrix3x3(const Quaternion& Rot);

            /// @brief Axis Angle Initialization constructor.
            /// @param Axis The axis on which to apply the rotation.
            /// @param Angle The amount of rotation to apply in Radians.
            Matrix3x3(const Vector3& Axis, const Real& Angle);

            /// @brief Bullet Matrix3x3 constructor.
            /// @param Mat The Bullet 3x3 Matrix.
            Matrix3x3(const btMatrix3x3& Mat);

            /// @brief Ogre Matrix3x3 constructor.
            /// @param Mat The Ogre 3x3 Matrix.
            Matrix3x3(const Ogre::Matrix3& Mat);

            ///////////////////////////////////////////////////////////////////////////////
            // Set From Other Data Functions

            /// @brief Sets the values for every number in the matrix.
            void SetValues(const Real& XX, const Real& XY, const Real& XZ, const Real& YX, const Real& YY, const Real& YZ, const Real& ZX, const Real& ZY, const Real& ZZ);

            /// @brief Sets the Matrix based on Euler angles.
            /// @param Yaw The number of degree's on the Yaw in Degrees to rotate.
            /// @param Pitch The number of degree's on the Pitch in Degrees to rotate.
            /// @param Roll The number of degree's on the Roll in Degrees to rotate.
            void SetFromEulerZYX(const Real& Yaw, const Real& Pitch, const Real& Roll);

            /// @brief Sets the Matrix from a quaternion.
            /// @param Rot The rotation to apply to this Matrix expressed as a Quaternion.
            void SetFromQuaternion(const Quaternion& Rot);

            /// @brief Sets the Matrix from an Axis Angle.
            /// @param Axis The axis on which to apply the rotation.
            /// @param Angle The amount of rotation to apply in Radians.
            void SetFromAxisAngle(const Vector3& Axis, const Real& Angle);

            /// @brief Sets all values in this Matrix to Identity values.
            /// @details Identity values for a 4x4 matrix is all zeros except for the values at [0][0], [1][1], [2][2], [3][3], which are set to one.
            void SetIdentity();

            /// @brief Sets all values in this Matrix to zero.
            void SetZero();

            ///////////////////////////////////////////////////////////////////////////////
            // Utility and Conversion Functions

            /// @brief Gets the Determinant of this Matrix.
            /// @return Returns a Real representing the Determinant of this Matrix.
            Real GetDeterminant() const;

            /// @brief Gets this Matrix as a Quaternion.
            /// @return Returns a Quaternion that expresses the same rotation as this Matrix.
            Quaternion GetAsQuaternion() const;

            /// @brief Gets the data from a Bullet Matrix3x3 and applies it to this.
            /// @param temp The Matrix3x3 to copy from.
            void ExtractBulletMatrix3x3(const btMatrix3x3& temp);

            /// @brief Gets a Bullet copy of this Matrix3x3.
            /// @return Returns a Bullet Matrix3x3 with the same values as this Matrix3x3.
            btMatrix3x3 GetBulletMatrix3x3() const;

            /// @brief Gets the data from an Ogre Matrix3x3 and applies it to this.
            /// @param temp The Matrix3x3 to copy from.
            void ExtractOgreMatrix3x3(const Ogre::Matrix3& temp);

            /// @brief Gets an Ogre copy of this Matrix3x3.
            /// @return Returns an Ogre Matrix3x3 with the same values as this Matrix3x3.
            Ogre::Matrix3 GetOgreMatrix3x3() const;

            ///////////////////////////////////////////////////////////////////////////////
            // Comparison Operators

            /// @brief Equality comparison operator.
            /// @param Other The other Matrix3x3 to compare against.
            /// @return Returns true if the two Matrix3x3's are equal, false otherwise.
            Boolean operator==(const Matrix3x3& Other) const;

            /// @brief Inequality comparison operator.
            /// @param Other The other Matrix3x3 to compare against.
            /// @return Returns true if the two Matrix3x3's are not equal, false otherwise.
            Boolean operator!=(const Matrix3x3& Other) const;

            ///////////////////////////////////////////////////////////////////////////////
            // Arithmetic Operators With Matrix3x3

            /// @brief Addition operator.
            /// @param Other The other Matrix3x3 to add to this.
            /// @return Returns a fresh Matrix3x3.
            Matrix3x3 operator+(const Matrix3x3& Other) const;

            /// @brief Subtraction operator.
            /// @param Other The other Matrix3x3 to subtract from this.
            /// @return Returns a fresh Matrix3x3.
            Matrix3x3 operator-(const Matrix3x3& Other) const;

            /// @brief Multiplication operator.
            /// @param Other The other Matrix3x3 to multiply this by.
            /// @return Returns a fresh Matrix3x3.
            Matrix3x3 operator*(const Matrix3x3& Other) const;

            /// @brief Addition Assignment operator.
            /// @param Other The other Matrix3x3 to add to this.
            /// @return Returns a reference to *this.
            Matrix3x3& operator+=(const Matrix3x3& Other);

            /// @brief Subtraction Assignment operator.
            /// @param Other The other Matrix3x3 to subtract from this.
            /// @return Returns a reference to *this.
            Matrix3x3& operator-=(const Matrix3x3& Other);

            /// @brief Multiplication Assignment operator.
            /// @param Other The other Matrix3x3 to add to this.
            /// @return Returns a reference to *this.
            Matrix3x3& operator*=(const Matrix3x3& Other);

            ///////////////////////////////////////////////////////////////////////////////
            // Arithmetic Operators With Other Datatypes

            /// @brief Multiply by Vector3 operator.
            /// @param Vec The Vector to be rotated.
            /// @return Returns a Vector3 with the rotation of this Matrix applied to it.
            Vector3 operator*(const Vector3& Vec) const;

            /// @brief Multiply by Real operator.
            /// @param Scaler The Real to multiply each member of this Matrix by.
            /// @return Returns a new Matrix3x3 that is a copy of this Matrix3x3 with each of it's members multiplied by the scaler.
            Matrix3x3 operator*(const Real& Scaler) const;

            /// @brief Multiply Assignment by Real operator.
            /// @param Scaler The Real to multiply each member of this Matrix by.
            /// @return Returns a reference to *this, being the modified Matrix3x3.
            Matrix3x3& operator*=(const Real& Scaler);

            ///////////////////////////////////////////////////////////////////////////////
            // Other Operators

            /// @brief Assignment operator.
            /// @param Other The other Matrix3x3 to copy from.
            void operator=(const Matrix3x3& Other);

            /// @brief Negative Unary operator.
            /// @return Returns a copy of this Matrix3x3 with each of it's members flipped.
            Matrix3x3 operator-() const;

            ///////////////////////////////////////////////////////////////////////////////
            // Fancy Math

            /// @brief Gets the Transpose of this Matrix.
            /// @todo I'm not gonna lie, I have no idea what the hell the Transpose of a 3x3 Matrix is or what it is used for...this doc could use a touchup.  In fact most of the doc's on this class could use a review.
            /// @return Returns a new Matrix3x3 that is a Transposed copy of this.
            Matrix3x3 Transpose() const;

            /// @brief Gets the Adjoint of this Matrix.
            /// @return Returns a new Matrix3x3 that is the Adjoint of this.
            Matrix3x3 Adjoint() const;

            /// @brief Gets the Inverse of this Matrix.
            /// @return Returns a new Matrix3x3 that is this Matrix inversed.
            Matrix3x3 Inverse() const;

            /// @brief Gets the cofactor between two sets of rows/columns.
            /// @param Row1 Row for the first Real of the calculated cofactor.
            /// @param Col1 Column for the first Real of the calculated cofactor.
            /// @param Row2 Row for the second Real of the calculated cofactor.
            /// @param Col2 Column for the second Real of the calculated cofactor.
            /// @return Returns the CoFactor of the provided rows/columns.
            Real CoFactor(const Whole& Row1, const Whole& Col1, const Whole& Row2, const Whole& Col2) const;

            /// @brief Scales this Matrix.
            /// @param Scaling A Vector3 containing the scaling to be applied to this Matrix.
            void SetScale(const Vector3& Scaling);

            /// @brief Checks to see if this Matrix has any scaling applied to it.
            /// @return Returns true if this Matrix is scaled, false otherwise.
            Boolean HasScaling() const;
    };//Matrix3x3
}//Mezzanine

#endif
