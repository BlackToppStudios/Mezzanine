// © Copyright 2010 - 2015 BlackTopp Studios Inc.
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
#ifndef _matrix4x4_h
#define _matrix4x4_h

#include "matrix3x3.h"
#include "plane.h"

namespace Ogre
{
    class Matrix4;
}

namespace Mezzanine
{
    ///////////////////////////////////////////////////////////////////////////////
    /// @class Matrix4x4
    /// @headerfile matrix4x4.h
    /// @brief A 4x4 matrix math class for the representation of full transforms.
    /// @details 4x4 matricies are commonly used by graphics subsystems.
    ///////////////////////////////////////
    class MEZZ_LIB Matrix4x4
    {
    public:
        ///////////////////////////////////////////////////////////////////////////////
        // Public Data Members

        /// @brief The bigger Matrix.  Fo' Reals.
        Real Matrix[4][4];

        ///////////////////////////////////////////////////////////////////////////////
        // Construction and Destruction

        /// @brief Non-Initialization constructor.
        Matrix4x4();
        /// @brief Explict Initialization constructor.
        /// @details Provides initialization for every number in the matrix.
        Matrix4x4(const Real& XX, const Real& XY, const Real& XZ, const Real& XW, const Real& YX, const Real& YY, const Real& YZ, const Real& YW,
                  const Real& ZX, const Real& ZY, const Real& ZZ, const Real& ZW, const Real& WX, const Real& WY, const Real& WZ, const Real& WW);
        /// @brief Ogre Matrix Initialization constructor.
        /// @param Mat The Ogre Matrix to build this Matrix from.
        Matrix4x4(const Ogre::Matrix4& Mat);
        /// @brief Transform Information Initialization constructor.
        /// @param Position The position of the transform.
        /// @param Scale The scale of the transform.
        /// @param Rotation The rotation of the transform.
        Matrix4x4(const Vector3& Position, const Vector3& Scale, const Quaternion& Rotation);
        /// @brief Class destructor.
        ~Matrix4x4();

        ///////////////////////////////////////////////////////////////////////////////
        // Set From Other Data Functions

        /// @brief Sets the values for every number in the matrix.
        void SetValues(const Real& XX, const Real& XY, const Real& XZ, const Real& XW, const Real& YX, const Real& YY, const Real& YZ, const Real& YW,
                       const Real& ZX, const Real& ZY, const Real& ZZ, const Real& ZW, const Real& WX, const Real& WY, const Real& WZ, const Real& WW);
        /// @brief Sets the Matrix based on a provided position, scale, and rotation.
        /// @param Position The position of the transform.
        /// @param Scale The scale of the transform.
        /// @param Rotation The rotation of the transform.
        void SetTransform(const Vector3& Position, const Vector3& Scale, const Quaternion& Rotation);
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

        /*/// @brief Gets the individual data in this Matrix and populates other data classes with their values.
        /// @param Position The vector to be populated with position data.
        /// @param Scale The vector to be populated with scaling data.
        /// @param Rotation The quaternion to be populated with rotation data.
        void Decompose(Vector3& Position, Vector3& Scale, Quaternion& Rotation) const;// */

        /// @brief Gets the rotation portion of this Matrix as a Quaternion.
        /// @return Returns a Quaternion that expresses the rotation of this Matrix.
        Quaternion GetRotationAsQuaternion() const;
        /// @brief Gets the rotation portion of this Matrix as a Matrix3x3.
        /// @return Returns a Matrix3x3 that expresses the rotation of this Matrix.
        Matrix3x3 GetRotationAsMatrix3x3() const;

        /// @brief Gets the data from an Ogre Matrix4x4 and applies it to this.
        /// @param temp The Matrix4x4 to copy from.
        void ExtractOgreMatrix4x4(const Ogre::Matrix4& temp);
        /// @brief Gets an Ogre copy of this Matrix4x4.
        /// @return Returns an Ogre Matrix4x4 with the same values as this Matrix4x4.
        Ogre::Matrix4 GetOgreMatrix4x4() const;

        ///////////////////////////////////////////////////////////////////////////////
        // Comparison Operators

        /// @brief Equality comparison operator.
        /// @param Other The other Matrix4x4 to compare against.
        /// @return Returns true if the two Matrix4x4's are equal, false otherwise.
        Boole operator==(const Matrix4x4& Other) const;
        /// @brief Inequality comparison operator.
        /// @param Other The other Matrix4x4 to compare against.
        /// @return Returns true if the two Matrix4x4's are not equal, false otherwise.
        Boole operator!=(const Matrix4x4& Other) const;

        ///////////////////////////////////////////////////////////////////////////////
        // Arithmetic Operators With Matrix4x4

        /// @brief Addition operator.
        /// @param Other The other Matrix4x4 to add to this.
        /// @return Returns a fresh Matrix4x4.
        Matrix4x4 operator+(const Matrix4x4& Other) const;
        /// @brief Subtraction operator.
        /// @param Other The other Matrix4x4 to subtract from this.
        /// @return Returns a fresh Matrix4x4.
        Matrix4x4 operator-(const Matrix4x4& Other) const;
        /// @brief Multiplication operator.
        /// @param Other The other Matrix4x4 to multiply this by.
        /// @return Returns a fresh Matrix4x4.
        Matrix4x4 operator*(const Matrix4x4& Other) const;
        /// @brief Addition Assignment operator.
        /// @param Other The other Matrix4x4 to add to this.
        /// @return Returns a reference to *this.
        Matrix4x4& operator+=(const Matrix4x4& Other);
        /// @brief Subtraction Assignment operator.
        /// @param Other The other Matrix4x4 to subtract from this.
        /// @return Returns a reference to *this.
        Matrix4x4& operator-=(const Matrix4x4& Other);
        /// @brief Multiplication Assignment operator.
        /// @param Other The other Matrix4x4 to add to this.
        /// @return Returns a reference to *this.
        Matrix4x4& operator*=(const Matrix4x4& Other);

        ///////////////////////////////////////////////////////////////////////////////
        // Arithmetic Operators With Other Datatypes

        /// @brief Multiply by Vector3 operator.
        /// @param Vec The Vector to be rotated.
        /// @return Returns a Vector3 with the rotation and translation of this Matrix applied to it.
        Vector3 operator*(const Vector3& Vec) const;
        /// @brief Multiply by Real operator.
        /// @param Scalar The Real to multiply each member of this Matrix by.
        /// @return Returns a new Matrix4x4 that is a copy of this Matrix4x4 with each of it's members multiplied by the scaler.
        Matrix4x4 operator*(const Real& Scalar) const;
        /// @brief Multiply Assignment by Real operator.
        /// @param Scalar The Real to multiply each member of this Matrix by.
        /// @return Returns a reference to *this, being the modified Matrix4x4.
        Matrix4x4& operator*=(const Real& Scalar);

        ///////////////////////////////////////////////////////////////////////////////
        // Other Operators

        /// @brief Assignment operator.
        /// @param Other The other Matrix4x4 to copy from.
        void operator=(const Matrix4x4& Other);
        /// @brief Assignment operator.
        /// @details This will copy the rotation portions of the other Matrix into this Matrix.
        /// @param Other The other Matrix3x3 to copy from.
        void operator=(const Matrix3x3& Other);

        ///////////////////////////////////////////////////////////////////////////////
        // Fancy Math

        /// @brief Gets the Transpose of this Matrix.
        /// @todo I'm not gonna lie, I have no idea what the hell the Transpose of a 4x4 Matrix is or what it is used for...this doc could use a touchup.  In fact most of the doc's on this class could use a review.
        /// @return Returns a new Matrix4x4 that is a Transposed copy of this.
        Matrix4x4 Transpose() const;
        /// @brief Gets the Adjoint of this Matrix.
        /// @return Returns a new Matrix4x4 that is the Adjoint of this.
        Matrix4x4 Adjoint() const;
        /// @brief Gets the Inverse of this Matrix.
        /// @return Returns a new Matrix4x4 that is this Matrix inversed.
        Matrix4x4 Inverse() const;
        /// @brief Combines the translation/rotation of two Matricies.
        /// @param Mat The other Matrix to combine with this.
        /// @return Returns the product of the two Matricies.
        Matrix4x4 Concatenate(const Matrix4x4& Mat) const;
        /// @brief Gets the Minor of the specified rows/columns of this Matrix.
        /// @param Row1 Row for the first Real of the calculated minor.
        /// @param Row2 Row for the second Real of the calculated minor.
        /// @param Row3 Row for the third Real of the calculated minor.
        /// @param Col1 Column for the first Real of the calculated minor.
        /// @param Col2 Column for the second Real of the calculated minor.
        /// @param Col3 Column for the third Real of the calculated minor.
        /// @return Returns a Real containing the minor of the Matrix.
        Real Minor(const Whole& Row1, const Whole& Row2, const Whole& Row3, const Whole& Col1, const Whole& Col2, const Whole& Col3) const;

        ///////////////////////////////////////////////////////////////////////////////
        // Serialization

        /// @brief Convert this class to an XML::Node ready for serialization
        /// @param CurrentRoot The point in the XML hierarchy that all this Matrix4x4 should be appended to.
        void ProtoSerialize(XML::Node& CurrentRoot) const;
        /// @brief Take the data stored in an XML and overwrite this instance of this object with it
        /// @param OneNode and XML::Node containing the data.
        void ProtoDeSerialize(const XML::Node& OneNode);

        /// @brief Get the name of the the XML tag this class will leave behind as its instances are serialized.
        /// @return A string containing "Matrix4x4".
        static String GetSerializableName();
    };//Matrix4x4
}//Mezzanine

#endif
