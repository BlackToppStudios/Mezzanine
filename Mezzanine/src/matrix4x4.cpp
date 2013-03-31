//© Copyright 2010 - 2013 BlackTopp Studios Inc.
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
#ifndef _matrix4x4_cpp
#define _matrix4x4_cpp

#include "matrix4x4.h"
#include "mathtool.h"
#include "entresol.h"

#include <OgreMatrix3.h>
#include <OgreMatrix4.h>

namespace Mezzanine
{
    Matrix4x4::Matrix4x4()
    {
        SetIdentity();
    }

    Matrix4x4::~Matrix4x4()
    {
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Additional Constructors

    Matrix4x4::Matrix4x4(const Real& XX, const Real& XY, const Real& XZ, const Real& XW, const Real& YX, const Real& YY, const Real& YZ, const Real& YW,
                         const Real& ZX, const Real& ZY, const Real& ZZ, const Real& ZW, const Real& WX, const Real& WY, const Real& WZ, const Real& WW)
    {
        SetValues(XX,XY,XZ,XW,
                  YX,YY,YZ,YW,
                  ZX,ZY,ZZ,ZW,
                  WX,WY,WZ,WW);
    }

    Matrix4x4::Matrix4x4(const Ogre::Matrix4& Mat)
    {
        ExtractOgreMatrix4x4(Mat);
    }

    Matrix4x4::Matrix4x4(const Vector3& Position, const Vector3& Scale, const Quaternion& Rotation)
    {
        SetTransform(Position,Scale,Rotation);
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Set From Other Data Functions

    void Matrix4x4::SetValues(const Real& XX, const Real& XY, const Real& XZ, const Real& XW, const Real& YX, const Real& YY, const Real& YZ, const Real& YW,
                              const Real& ZX, const Real& ZY, const Real& ZZ, const Real& ZW, const Real& WX, const Real& WY, const Real& WZ, const Real& WW)
    {
        Matrix[0][0] = XX; Matrix[0][1] = XY; Matrix[0][2] = XZ; Matrix[0][3] = XW;
        Matrix[1][0] = YX; Matrix[1][1] = YY; Matrix[1][2] = YZ; Matrix[1][3] = YW;
        Matrix[2][0] = ZX; Matrix[2][1] = ZY; Matrix[2][2] = ZZ; Matrix[2][3] = ZW;
        Matrix[3][0] = WX; Matrix[3][1] = WY; Matrix[3][2] = WZ; Matrix[3][3] = WW;
    }

    void Matrix4x4::SetTransform(const Vector3& Position, const Vector3& Scale, const Quaternion& Rotation)
    {
        Matrix3x3 Mat3x3(Rotation);

        Matrix[0][0] = Scale.X * Mat3x3.Matrix[0][0]; Matrix[0][1] = Scale.Y * Mat3x3.Matrix[0][1]; Matrix[0][2] = Scale.Z * Mat3x3.Matrix[0][2]; Matrix[0][3] = Position.X;
        Matrix[1][0] = Scale.X * Mat3x3.Matrix[1][0]; Matrix[1][1] = Scale.Y * Mat3x3.Matrix[1][1]; Matrix[1][2] = Scale.Z * Mat3x3.Matrix[1][2]; Matrix[1][3] = Position.Y;
        Matrix[2][0] = Scale.X * Mat3x3.Matrix[2][0]; Matrix[2][1] = Scale.Y * Mat3x3.Matrix[2][1]; Matrix[2][2] = Scale.Z * Mat3x3.Matrix[2][2]; Matrix[2][3] = Position.Z;

        Matrix[3][0] = 0; Matrix[3][1] = 0; Matrix[3][2] = 0; Matrix[3][3] = 1;
    }

    void Matrix4x4::SetIdentity()
    {
        SetValues(1,0,0,0,
                  0,1,0,0,
                  0,0,1,0,
                  0,0,0,1);
    }

    void Matrix4x4::SetZero()
    {
        SetValues(0,0,0,0,
                  0,0,0,0,
                  0,0,0,0,
                  0,0,0,0);
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Utility and Conversion Functions

    Real Matrix4x4::GetDeterminant() const
    {
        return Matrix[0][0] * Minor(1,2,3,1,2,3) -
               Matrix[0][1] * Minor(1,2,3,0,2,3) +
               Matrix[0][2] * Minor(1,2,3,0,1,3) -
               Matrix[0][3] * Minor(1,2,3,0,1,2);
    }

    /*void Matrix4x4::Decompose(Vector3& Position, Vector3& Scale, Quaternion& Rotation) const
    {

    }// */

    Quaternion Matrix4x4::GetRotationAsQuaternion() const
    {
        Matrix3x3 Convert = this->GetRotationAsMatrix3x3();
        return Convert.GetAsQuaternion();
    }

    Matrix3x3 Matrix4x4::GetRotationAsMatrix3x3() const
    {
        Matrix3x3 Ret;

        Ret.Matrix[0][0] = Matrix[0][0];
        Ret.Matrix[0][1] = Matrix[0][1];
        Ret.Matrix[0][2] = Matrix[0][2];
        Ret.Matrix[1][0] = Matrix[1][0];
        Ret.Matrix[1][1] = Matrix[1][1];
        Ret.Matrix[1][2] = Matrix[1][2];
        Ret.Matrix[2][0] = Matrix[2][0];
        Ret.Matrix[2][1] = Matrix[2][1];
        Ret.Matrix[2][2] = Matrix[2][2];

        return Ret;
    }

    void Matrix4x4::ExtractOgreMatrix4x4(const Ogre::Matrix4& temp)
    {
        Matrix[0][0] = temp[0][0]; Matrix[0][1] = temp[0][1]; Matrix[0][2] = temp[0][2]; Matrix[0][3] = temp[0][3];
        Matrix[1][0] = temp[1][0]; Matrix[1][1] = temp[1][1]; Matrix[1][2] = temp[1][2]; Matrix[1][3] = temp[1][3];
        Matrix[2][0] = temp[2][0]; Matrix[2][1] = temp[2][1]; Matrix[2][2] = temp[2][2]; Matrix[2][3] = temp[2][3];
        Matrix[3][0] = temp[3][0]; Matrix[3][1] = temp[3][1]; Matrix[3][2] = temp[3][2]; Matrix[3][3] = temp[3][3];
    }

    Ogre::Matrix4 Matrix4x4::GetOgreMatrix4x4() const
    {
        return Ogre::Matrix4(Matrix[0][0], Matrix[1][0], Matrix[2][0], Matrix[3][0],
                             Matrix[0][1], Matrix[1][1], Matrix[2][1], Matrix[3][1],
                             Matrix[0][2], Matrix[1][2], Matrix[2][2], Matrix[3][2],
                             Matrix[0][3], Matrix[1][3], Matrix[2][3], Matrix[3][3]);
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Comparison Operators

    bool Matrix4x4::operator==(const Matrix4x4& Other) const
    {
        for( Whole Row = 0 ; Row < 4 ; ++Row )
        {
            for( Whole Col = 0 ; Col < 4 ; ++Col )
            {
                if(Matrix[Row][Col] != Other.Matrix[Row][Col])
                    return false;
            }
        }
        return true;
    }

    bool Matrix4x4::operator!=(const Matrix4x4& Other) const
    {
        for( Whole Row = 0 ; Row < 4 ; ++Row )
        {
            for( Whole Col = 0 ; Col < 4 ; ++Col )
            {
                if(Matrix[Row][Col] != Other.Matrix[Row][Col])
                    return true;
            }
        }
        return false;
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Arithmetic Operators With Matrix4x4

    Matrix4x4 Matrix4x4::operator+(const Matrix4x4& Other) const
    {
        Matrix4x4 Ret;

        Ret.Matrix[0][0] = Matrix[0][0] + Other.Matrix[0][0];
        Ret.Matrix[0][1] = Matrix[0][1] + Other.Matrix[0][1];
        Ret.Matrix[0][2] = Matrix[0][2] + Other.Matrix[0][2];
        Ret.Matrix[0][3] = Matrix[0][3] + Other.Matrix[0][3];

        Ret.Matrix[1][0] = Matrix[1][0] + Other.Matrix[1][0];
        Ret.Matrix[1][1] = Matrix[1][1] + Other.Matrix[1][1];
        Ret.Matrix[1][2] = Matrix[1][2] + Other.Matrix[1][2];
        Ret.Matrix[1][3] = Matrix[1][3] + Other.Matrix[1][3];

        Ret.Matrix[2][0] = Matrix[2][0] + Other.Matrix[2][0];
        Ret.Matrix[2][1] = Matrix[2][1] + Other.Matrix[2][1];
        Ret.Matrix[2][2] = Matrix[2][2] + Other.Matrix[2][2];
        Ret.Matrix[2][3] = Matrix[2][3] + Other.Matrix[2][3];

        Ret.Matrix[3][0] = Matrix[3][0] + Other.Matrix[3][0];
        Ret.Matrix[3][1] = Matrix[3][1] + Other.Matrix[3][1];
        Ret.Matrix[3][2] = Matrix[3][2] + Other.Matrix[3][2];
        Ret.Matrix[3][3] = Matrix[3][3] + Other.Matrix[3][3];

        return Ret;
    }

    Matrix4x4 Matrix4x4::operator-(const Matrix4x4& Other) const
    {
        Matrix4x4 Ret;

        Ret.Matrix[0][0] = Matrix[0][0] - Other.Matrix[0][0];
        Ret.Matrix[0][1] = Matrix[0][1] - Other.Matrix[0][1];
        Ret.Matrix[0][2] = Matrix[0][2] - Other.Matrix[0][2];
        Ret.Matrix[0][3] = Matrix[0][3] - Other.Matrix[0][3];

        Ret.Matrix[1][0] = Matrix[1][0] - Other.Matrix[1][0];
        Ret.Matrix[1][1] = Matrix[1][1] - Other.Matrix[1][1];
        Ret.Matrix[1][2] = Matrix[1][2] - Other.Matrix[1][2];
        Ret.Matrix[1][3] = Matrix[1][3] - Other.Matrix[1][3];

        Ret.Matrix[2][0] = Matrix[2][0] - Other.Matrix[2][0];
        Ret.Matrix[2][1] = Matrix[2][1] - Other.Matrix[2][1];
        Ret.Matrix[2][2] = Matrix[2][2] - Other.Matrix[2][2];
        Ret.Matrix[2][3] = Matrix[2][3] - Other.Matrix[2][3];

        Ret.Matrix[3][0] = Matrix[3][0] - Other.Matrix[3][0];
        Ret.Matrix[3][1] = Matrix[3][1] - Other.Matrix[3][1];
        Ret.Matrix[3][2] = Matrix[3][2] - Other.Matrix[3][2];
        Ret.Matrix[3][3] = Matrix[3][3] - Other.Matrix[3][3];

        return Ret;
    }

    Matrix4x4 Matrix4x4::operator*(const Matrix4x4& Other) const
    {
        return Concatenate(Other);
    }

    Matrix4x4& Matrix4x4::operator+=(const Matrix4x4& Other)
    {
        Matrix[0][0] += Other.Matrix[0][0];
        Matrix[0][1] += Other.Matrix[0][1];
        Matrix[0][2] += Other.Matrix[0][2];
        Matrix[0][3] += Other.Matrix[0][3];

        Matrix[1][0] += Other.Matrix[1][0];
        Matrix[1][1] += Other.Matrix[1][1];
        Matrix[1][2] += Other.Matrix[1][2];
        Matrix[1][3] += Other.Matrix[1][3];

        Matrix[2][0] += Other.Matrix[2][0];
        Matrix[2][1] += Other.Matrix[2][1];
        Matrix[2][2] += Other.Matrix[2][2];
        Matrix[2][3] += Other.Matrix[2][3];

        Matrix[3][0] += Other.Matrix[3][0];
        Matrix[3][1] += Other.Matrix[3][1];
        Matrix[3][2] += Other.Matrix[3][2];
        Matrix[3][3] += Other.Matrix[3][3];

        return *this;
    }

    Matrix4x4& Matrix4x4::operator-=(const Matrix4x4& Other)
    {
        Matrix[0][0] -= Other.Matrix[0][0];
        Matrix[0][1] -= Other.Matrix[0][1];
        Matrix[0][2] -= Other.Matrix[0][2];
        Matrix[0][3] -= Other.Matrix[0][3];

        Matrix[1][0] -= Other.Matrix[1][0];
        Matrix[1][1] -= Other.Matrix[1][1];
        Matrix[1][2] -= Other.Matrix[1][2];
        Matrix[1][3] -= Other.Matrix[1][3];

        Matrix[2][0] -= Other.Matrix[2][0];
        Matrix[2][1] -= Other.Matrix[2][1];
        Matrix[2][2] -= Other.Matrix[2][2];
        Matrix[2][3] -= Other.Matrix[2][3];

        Matrix[3][0] -= Other.Matrix[3][0];
        Matrix[3][1] -= Other.Matrix[3][1];
        Matrix[3][2] -= Other.Matrix[3][2];
        Matrix[3][3] -= Other.Matrix[3][3];

        return *this;
    }

    Matrix4x4& Matrix4x4::operator*=(const Matrix4x4& Other)
    {
        *this = Concatenate(Other);
        return *this;
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Arithmetic Operators With Other Datatypes

    Vector3 Matrix4x4::operator*(const Vector3& Vec) const
    {
        Vector3 Ret;

        Real InvW = 1.0f / ( Matrix[3][0] * Vec.X + Matrix[3][1] * Vec.Y + Matrix[3][2] * Vec.Z + Matrix[3][3] );

        Ret.X = ( Matrix[0][0] * Vec.X + Matrix[0][1] * Vec.Y + Matrix[0][2] * Vec.Z + Matrix[0][3] ) * InvW;
        Ret.Y = ( Matrix[1][0] * Vec.X + Matrix[1][1] * Vec.Y + Matrix[1][2] * Vec.Z + Matrix[1][3] ) * InvW;
        Ret.Z = ( Matrix[2][0] * Vec.X + Matrix[2][1] * Vec.Y + Matrix[2][2] * Vec.Z + Matrix[2][3] ) * InvW;

        return Ret;
    }

    Matrix4x4 Matrix4x4::operator*(const Real& Scalar) const
    {
        return Matrix4x4(
                Scalar * Matrix[0][0], Scalar * Matrix[0][1], Scalar * Matrix[0][2], Scalar * Matrix[0][3],
                Scalar * Matrix[1][0], Scalar * Matrix[1][1], Scalar * Matrix[1][2], Scalar * Matrix[1][3],
                Scalar * Matrix[2][0], Scalar * Matrix[2][1], Scalar * Matrix[2][2], Scalar * Matrix[2][3],
                Scalar * Matrix[3][0], Scalar * Matrix[3][1], Scalar * Matrix[3][2], Scalar * Matrix[3][3]);
    }

    Matrix4x4& Matrix4x4::operator*=(const Real& Scalar)
    {
        Matrix[0][0] *= Scalar; Matrix[0][1] *= Scalar; Matrix[0][2] *= Scalar; Matrix[0][3] *= Scalar;
        Matrix[1][0] *= Scalar; Matrix[1][1] *= Scalar; Matrix[1][2] *= Scalar; Matrix[1][3] *= Scalar;
        Matrix[2][0] *= Scalar; Matrix[2][1] *= Scalar; Matrix[2][2] *= Scalar; Matrix[2][3] *= Scalar;
        Matrix[3][0] *= Scalar; Matrix[3][1] *= Scalar; Matrix[3][2] *= Scalar; Matrix[3][3] *= Scalar;
        return *this;
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Other Operators

    void Matrix4x4::operator=(const Matrix4x4& Other)
    {
        for( Whole Row = 0 ; Row < 4 ; ++Row )
        {
            for( Whole Col = 0 ; Col < 4 ; ++Col )
            {
                Matrix[Row][Col] = Other.Matrix[Row][Col];
            }
        }
    }

    void Matrix4x4::operator=(const Matrix3x3& Other)
    {
        Matrix[0][0] = Other.Matrix[0][0]; Matrix[0][1] = Other.Matrix[0][1]; Matrix[0][2] = Other.Matrix[0][2];
        Matrix[1][0] = Other.Matrix[1][0]; Matrix[1][1] = Other.Matrix[1][1]; Matrix[1][2] = Other.Matrix[1][2];
        Matrix[2][0] = Other.Matrix[2][0]; Matrix[2][1] = Other.Matrix[2][1]; Matrix[2][2] = Other.Matrix[2][2];
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Fancy Math

    Matrix4x4 Matrix4x4::Transpose() const
    {
        return Matrix4x4(Matrix[0][0], Matrix[1][0], Matrix[2][0], Matrix[3][0],
                         Matrix[0][1], Matrix[1][1], Matrix[2][1], Matrix[3][1],
                         Matrix[0][2], Matrix[1][2], Matrix[2][2], Matrix[3][2],
                         Matrix[0][3], Matrix[1][3], Matrix[2][3], Matrix[3][3]);
    }

    Matrix4x4 Matrix4x4::Adjoint() const
    {
        return Matrix4x4( Minor(1,2,3,1,2,3),-Minor(0,2,3,1,2,3),Minor(0,1,3,1,2,3),-Minor(0,1,2,1,2,3),
                          -Minor(1,2,3,0,2,3),Minor(0,2,3,0,2,3),-Minor(0,1,3,0,2,3),Minor(0,1,2,0,2,3),
                          Minor(1,2,3,0,1,3),-Minor(0,2,3,0,1,3),Minor(0,1,3,0,1,3),-Minor(0,1,2,0,1,3),
                          -Minor(1,2,3,0,1,2),Minor(0,2,3,0,1,2),-Minor(0,1,3,0,1,2),Minor(0,1,2,0,1,2));
    }

    Matrix4x4 Matrix4x4::Inverse() const
    {
        Real m00 = Matrix[0][0], m01 = Matrix[0][1], m02 = Matrix[0][2], m03 = Matrix[0][3];
        Real m10 = Matrix[1][0], m11 = Matrix[1][1], m12 = Matrix[1][2], m13 = Matrix[1][3];
        Real m20 = Matrix[2][0], m21 = Matrix[2][1], m22 = Matrix[2][2], m23 = Matrix[2][3];
        Real m30 = Matrix[3][0], m31 = Matrix[3][1], m32 = Matrix[3][2], m33 = Matrix[3][3];

        Real v0 = m20 * m31 - m21 * m30;
        Real v1 = m20 * m32 - m22 * m30;
        Real v2 = m20 * m33 - m23 * m30;
        Real v3 = m21 * m32 - m22 * m31;
        Real v4 = m21 * m33 - m23 * m31;
        Real v5 = m22 * m33 - m23 * m32;

        Real t00 = + (v5 * m11 - v4 * m12 + v3 * m13);
        Real t10 = - (v5 * m10 - v2 * m12 + v1 * m13);
        Real t20 = + (v4 * m10 - v2 * m11 + v0 * m13);
        Real t30 = - (v3 * m10 - v1 * m11 + v0 * m12);

        Real invDet = 1 / (t00 * m00 + t10 * m01 + t20 * m02 + t30 * m03);

        Real d00 = t00 * invDet;
        Real d10 = t10 * invDet;
        Real d20 = t20 * invDet;
        Real d30 = t30 * invDet;

        Real d01 = - (v5 * m01 - v4 * m02 + v3 * m03) * invDet;
        Real d11 = + (v5 * m00 - v2 * m02 + v1 * m03) * invDet;
        Real d21 = - (v4 * m00 - v2 * m01 + v0 * m03) * invDet;
        Real d31 = + (v3 * m00 - v1 * m01 + v0 * m02) * invDet;

        v0 = m10 * m31 - m11 * m30;
        v1 = m10 * m32 - m12 * m30;
        v2 = m10 * m33 - m13 * m30;
        v3 = m11 * m32 - m12 * m31;
        v4 = m11 * m33 - m13 * m31;
        v5 = m12 * m33 - m13 * m32;

        Real d02 = + (v5 * m01 - v4 * m02 + v3 * m03) * invDet;
        Real d12 = - (v5 * m00 - v2 * m02 + v1 * m03) * invDet;
        Real d22 = + (v4 * m00 - v2 * m01 + v0 * m03) * invDet;
        Real d32 = - (v3 * m00 - v1 * m01 + v0 * m02) * invDet;

        v0 = m21 * m10 - m20 * m11;
        v1 = m22 * m10 - m20 * m12;
        v2 = m23 * m10 - m20 * m13;
        v3 = m22 * m11 - m21 * m12;
        v4 = m23 * m11 - m21 * m13;
        v5 = m23 * m12 - m22 * m13;

        Real d03 = - (v5 * m01 - v4 * m02 + v3 * m03) * invDet;
        Real d13 = + (v5 * m00 - v2 * m02 + v1 * m03) * invDet;
        Real d23 = - (v4 * m00 - v2 * m01 + v0 * m03) * invDet;
        Real d33 = + (v3 * m00 - v1 * m01 + v0 * m02) * invDet;

        return Matrix4x4(
            d00, d01, d02, d03,
            d10, d11, d12, d13,
            d20, d21, d22, d23,
            d30, d31, d32, d33);
    }

    Matrix4x4 Matrix4x4::Concatenate(const Matrix4x4& Mat) const
    {
        Matrix4x4 Ret;
        Ret.Matrix[0][0] = Matrix[0][0] * Mat.Matrix[0][0] + Matrix[0][1] * Mat.Matrix[1][0] + Matrix[0][2] * Mat.Matrix[2][0] + Matrix[0][3] * Mat.Matrix[3][0];
        Ret.Matrix[0][1] = Matrix[0][0] * Mat.Matrix[0][1] + Matrix[0][1] * Mat.Matrix[1][1] + Matrix[0][2] * Mat.Matrix[2][1] + Matrix[0][3] * Mat.Matrix[3][1];
        Ret.Matrix[0][2] = Matrix[0][0] * Mat.Matrix[0][2] + Matrix[0][1] * Mat.Matrix[1][2] + Matrix[0][2] * Mat.Matrix[2][2] + Matrix[0][3] * Mat.Matrix[3][2];
        Ret.Matrix[0][3] = Matrix[0][0] * Mat.Matrix[0][3] + Matrix[0][1] * Mat.Matrix[1][3] + Matrix[0][2] * Mat.Matrix[2][3] + Matrix[0][3] * Mat.Matrix[3][3];

        Ret.Matrix[1][0] = Matrix[1][0] * Mat.Matrix[0][0] + Matrix[1][1] * Mat.Matrix[1][0] + Matrix[1][2] * Mat.Matrix[2][0] + Matrix[1][3] * Mat.Matrix[3][0];
        Ret.Matrix[1][1] = Matrix[1][0] * Mat.Matrix[0][1] + Matrix[1][1] * Mat.Matrix[1][1] + Matrix[1][2] * Mat.Matrix[2][1] + Matrix[1][3] * Mat.Matrix[3][1];
        Ret.Matrix[1][2] = Matrix[1][0] * Mat.Matrix[0][2] + Matrix[1][1] * Mat.Matrix[1][2] + Matrix[1][2] * Mat.Matrix[2][2] + Matrix[1][3] * Mat.Matrix[3][2];
        Ret.Matrix[1][3] = Matrix[1][0] * Mat.Matrix[0][3] + Matrix[1][1] * Mat.Matrix[1][3] + Matrix[1][2] * Mat.Matrix[2][3] + Matrix[1][3] * Mat.Matrix[3][3];

        Ret.Matrix[2][0] = Matrix[2][0] * Mat.Matrix[0][0] + Matrix[2][1] * Mat.Matrix[1][0] + Matrix[2][2] * Mat.Matrix[2][0] + Matrix[2][3] * Mat.Matrix[3][0];
        Ret.Matrix[2][1] = Matrix[2][0] * Mat.Matrix[0][1] + Matrix[2][1] * Mat.Matrix[1][1] + Matrix[2][2] * Mat.Matrix[2][1] + Matrix[2][3] * Mat.Matrix[3][1];
        Ret.Matrix[2][2] = Matrix[2][0] * Mat.Matrix[0][2] + Matrix[2][1] * Mat.Matrix[1][2] + Matrix[2][2] * Mat.Matrix[2][2] + Matrix[2][3] * Mat.Matrix[3][2];
        Ret.Matrix[2][3] = Matrix[2][0] * Mat.Matrix[0][3] + Matrix[2][1] * Mat.Matrix[1][3] + Matrix[2][2] * Mat.Matrix[2][3] + Matrix[2][3] * Mat.Matrix[3][3];

        Ret.Matrix[3][0] = Matrix[3][0] * Mat.Matrix[0][0] + Matrix[3][1] * Mat.Matrix[1][0] + Matrix[3][2] * Mat.Matrix[2][0] + Matrix[3][3] * Mat.Matrix[3][0];
        Ret.Matrix[3][1] = Matrix[3][0] * Mat.Matrix[0][1] + Matrix[3][1] * Mat.Matrix[1][1] + Matrix[3][2] * Mat.Matrix[2][1] + Matrix[3][3] * Mat.Matrix[3][1];
        Ret.Matrix[3][2] = Matrix[3][0] * Mat.Matrix[0][2] + Matrix[3][1] * Mat.Matrix[1][2] + Matrix[3][2] * Mat.Matrix[2][2] + Matrix[3][3] * Mat.Matrix[3][2];
        Ret.Matrix[3][3] = Matrix[3][0] * Mat.Matrix[0][3] + Matrix[3][1] * Mat.Matrix[1][3] + Matrix[3][2] * Mat.Matrix[2][3] + Matrix[3][3] * Mat.Matrix[3][3];

        return Ret;
    }

    Real Matrix4x4::Minor(const Whole& Row1, const Whole& Row2, const Whole& Row3, const Whole& Col1, const Whole& Col2, const Whole& Col3) const
    {
        return Matrix[Row1][Col1] * (Matrix[Row2][Col2] * Matrix[Row3][Col3] - Matrix[Row3][Col2] * Matrix[Row2][Col3]) -
               Matrix[Row1][Col2] * (Matrix[Row2][Col1] * Matrix[Row3][Col3] - Matrix[Row3][Col1] * Matrix[Row2][Col3]) +
               Matrix[Row1][Col3] * (Matrix[Row2][Col1] * Matrix[Row3][Col2] - Matrix[Row3][Col1] * Matrix[Row2][Col2]);
    }
}//Mezzanine

#endif
