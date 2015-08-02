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
#ifndef _matrix3x3_cpp
#define _matrix3x3_cpp

#include "matrix3x3.h"
#include "MathTools/mathtools.h"
#include "entresol.h"

#include "btBulletDynamicsCommon.h"
#include <OgreMatrix3.h>

namespace Mezzanine
{
    Matrix3x3::Matrix3x3()
    {
        SetIdentity();
    }

    Matrix3x3::~Matrix3x3()
    {
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Additional Constructors

    Matrix3x3::Matrix3x3(const Real& XX, const Real& XY, const Real& XZ, const Real& YX, const Real& YY, const Real& YZ, const Real& ZX, const Real& ZY, const Real& ZZ)
    {
        SetValues(XX,XY,XZ,YX,YY,YZ,ZX,ZY,ZZ);
    }

    Matrix3x3::Matrix3x3(const Real& Yaw, const Real& Pitch, const Real& Roll)
    {
        SetFromEulerZYX(Yaw,Pitch,Roll);
    }

    Matrix3x3::Matrix3x3(const Quaternion& Rot)
    {
        SetFromQuaternion(Rot);
    }

    Matrix3x3::Matrix3x3(const Vector3& Axis, const Real& Angle)
    {
        SetFromAxisAngle(Axis,Angle);
    }

    Matrix3x3::Matrix3x3(const btMatrix3x3& Mat)
    {
        ExtractBulletMatrix3x3(Mat);
    }

    Matrix3x3::Matrix3x3(const Ogre::Matrix3& Mat)
    {
        ExtractOgreMatrix3x3(Mat);
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Set From Other Data Functions

    void Matrix3x3::SetValues(const Real& XX, const Real& XY, const Real& XZ, const Real& YX, const Real& YY, const Real& YZ, const Real& ZX, const Real& ZY, const Real& ZZ)
    {
        Matrix[0][0] = XX;
        Matrix[0][1] = XY;
        Matrix[0][2] = XZ;
        Matrix[1][0] = YX;
        Matrix[1][1] = YY;
        Matrix[1][2] = YZ;
        Matrix[2][0] = ZX;
        Matrix[2][1] = ZY;
        Matrix[2][2] = ZZ;
    }

    void Matrix3x3::SetFromEulerZYX(const Real& Yaw, const Real& Pitch, const Real& Roll)
    {
        Real Cos, Sin;

        Cos = MathTools::Cos(Yaw);
        Sin = MathTools::Sin(Yaw);
        Matrix3x3 ZMat(Cos,-Sin,0.0,Sin,Cos,0.0,0.0,0.0,1.0);

        Cos = MathTools::Cos(Pitch);
        Sin = MathTools::Sin(Pitch);
        Matrix3x3 YMat(Cos,0.0,Sin,0.0,1.0,0.0,-Sin,0.0,Cos);

        Cos = MathTools::Cos(Roll);
        Sin = MathTools::Sin(Roll);
        Matrix3x3 XMat(1.0,0.0,0.0,0.0,Cos,-Sin,0.0,Sin,Cos);

        *this = ZMat * (YMat * XMat);
    }

    void Matrix3x3::SetFromQuaternion(const Quaternion& Rot)
    {
        Real Dist = Rot.LengthSqrd();
        if(Dist == Real(0.0))
            { MEZZ_EXCEPTION(ExceptionBase::ARITHMETIC_EXCEPTION,"Attempting to set Matrix3x3 with zero length Quaternion, this is bad."); }
        Real Sc = Real(2.0) / Dist;
        Real XS = Rot.X * Sc,  YS = Rot.Y * Sc,  ZS = Rot.Z * Sc;
        Real WX = Rot.W * XS,  WY = Rot.W * YS,  WZ = Rot.W * ZS;
        Real XX = Rot.X * XS,  XY = Rot.X * YS,  XZ = Rot.X * ZS;
        Real YY = Rot.Y * YS,  YZ = Rot.Y * ZS,  ZZ = Rot.Z * ZS;
        SetValues(Real(1.0) - (YY + ZZ), XY - WZ, XZ + WY,
                  XY + WZ, Real(1.0) - (XX + ZZ), YZ - WX,
                  XZ - WY, YZ + WX, Real(1.0) - (XX + YY));
    }

    void Matrix3x3::SetFromAxisAngle(const Vector3& Axis, const Real& Angle)
    {
        Real Cos = MathTools::Cos(Angle);
        Real Sin = MathTools::Sin(Angle);
        Real OneMinusCos = 1.0 - Cos;
        Real X2 = Axis.X * Axis.X;
        Real Y2 = Axis.Y * Axis.Y;
        Real Z2 = Axis.Z * Axis.Z;
        Real XYM = Axis.X * Axis.Y * OneMinusCos;
        Real XZM = Axis.X * Axis.Z * OneMinusCos;
        Real YZM = Axis.Y * Axis.Z * OneMinusCos;
        Real XSin = Axis.X * Sin;
        Real YSin = Axis.Y * Sin;
        Real ZSin = Axis.Z * Sin;

        Matrix[0][0] = X2 * OneMinusCos + Cos;
        Matrix[0][1] = XYM - ZSin;
        Matrix[0][2] = XZM + YSin;
        Matrix[1][0] = XYM + ZSin;
        Matrix[1][1] = Y2 * OneMinusCos + Cos;
        Matrix[1][2] = YZM - XSin;
        Matrix[2][0] = XZM - YSin;
        Matrix[2][1] = YZM + XSin;
        Matrix[2][2] = Z2 * OneMinusCos + Cos;
    }

    void Matrix3x3::SetIdentity()
    {
        SetValues(1,0,0,
                  0,1,0,
                  0,0,1);
    }

    void Matrix3x3::SetZero()
    {
        SetValues(0,0,0,
                  0,0,0,
                  0,0,0);
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Utility and Conversion Functions

    Real Matrix3x3::GetDeterminant() const
    {
        Real Cofactor00 = Matrix[1][1] * Matrix[2][2] - Matrix[1][2] * Matrix[2][1];
        Real Cofactor10 = Matrix[1][2] * Matrix[2][0] - Matrix[1][0] * Matrix[2][2];
        Real Cofactor20 = Matrix[1][0] * Matrix[2][1] - Matrix[1][1] * Matrix[2][0];

        Real Det = Matrix[0][0] * Cofactor00 + Matrix[0][1] * Cofactor10 + Matrix[0][2] * Cofactor20;

        return Det;
    }

    Quaternion Matrix3x3::GetAsQuaternion() const
    {
        Real Trace = Matrix[0][0] + Matrix[1][1] + Matrix[2][2];
        Quaternion Ret;

        if (Trace > Real(0.0))
        {
            Real Sc = MathTools::Sqrt(Trace + Real(1.0));
            Ret.W = (Sc * Real(0.5));
            Sc = Real(0.5) / Sc;

            Ret.X = ( (Matrix[2][1] - Matrix[1][2]) * Sc );
            Ret.Y = ( (Matrix[0][2] - Matrix[2][0]) * Sc );
            Ret.Z = ( (Matrix[1][0] - Matrix[0][1]) * Sc );
        }
        else
        {
            int I = Matrix[0][0] < Matrix[1][1] ?
                  ( Matrix[1][1] < Matrix[2][2] ? 2 : 1 ) :
                  ( Matrix[0][0] < Matrix[2][2] ? 2 : 0 );
            int J = (I + 1) % 3;
            int K = (I + 2) % 3;

            Real Sc = MathTools::Sqrt(Matrix[I][I] - Matrix[J][J] - Matrix[K][K] + Real(1.0));
            Ret[I] = Sc * Real(0.5);
            Sc = Real(0.5) / Sc;

            Ret[J] = ( Matrix[J][I] + Matrix[I][J] ) * Sc;
            Ret[K] = ( Matrix[K][I] + Matrix[I][K] ) * Sc;
            Ret[3] = ( Matrix[K][J] - Matrix[J][K] ) * Sc;
        }
        return Ret;
    }

    void Matrix3x3::ExtractBulletMatrix3x3(const btMatrix3x3& temp)
    {
        btVector3 Col1 = temp.getColumn(0);
        btVector3 Col2 = temp.getColumn(1);
        btVector3 Col3 = temp.getColumn(2);

        Matrix[0][0] = Col1.x();
        Matrix[0][1] = Col2.x();
        Matrix[0][2] = Col3.x();
        Matrix[1][0] = Col1.y();
        Matrix[1][1] = Col2.y();
        Matrix[1][2] = Col3.y();
        Matrix[2][0] = Col1.z();
        Matrix[2][1] = Col2.z();
        Matrix[2][2] = Col3.z();
    }

    btMatrix3x3 Matrix3x3::GetBulletMatrix3x3() const
    {
        return btMatrix3x3(Matrix[0][0],Matrix[0][1],Matrix[0][2],Matrix[1][0],Matrix[1][1],Matrix[1][2],Matrix[2][0],Matrix[2][1],Matrix[2][2]);
    }

    void Matrix3x3::ExtractOgreMatrix3x3(const Ogre::Matrix3& temp)
    {
        Ogre::Vector3 Col1 = temp.GetColumn(0);
        Ogre::Vector3 Col2 = temp.GetColumn(1);
        Ogre::Vector3 Col3 = temp.GetColumn(2);

        Matrix[0][0] = Col1.x;
        Matrix[0][1] = Col2.x;
        Matrix[0][2] = Col3.x;
        Matrix[1][0] = Col1.y;
        Matrix[1][1] = Col2.y;
        Matrix[1][2] = Col3.y;
        Matrix[2][0] = Col1.z;
        Matrix[2][1] = Col2.z;
        Matrix[2][2] = Col3.z;
    }

    Ogre::Matrix3 Matrix3x3::GetOgreMatrix3x3() const
    {
        return Ogre::Matrix3(Matrix[0][0],Matrix[0][1],Matrix[0][2],Matrix[1][0],Matrix[1][1],Matrix[1][2],Matrix[2][0],Matrix[2][1],Matrix[2][2]);
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Comparison Operators

    Boole Matrix3x3::operator==(const Matrix3x3& Other) const
    {
        for( Whole Row = 0 ; Row < 3 ; ++Row )
        {
            for( Whole Col = 0 ; Col < 3 ; ++Col )
            {
                if( Matrix[Row][Col] != Other.Matrix[Row][Col] )
                    return false;
            }
        }
        return true;
    }

    Boole Matrix3x3::operator!=(const Matrix3x3& Other) const
    {
        for( Whole Row = 0 ; Row < 3 ; ++Row )
        {
            for( Whole Col = 0 ; Col < 3 ; ++Col )
            {
                if( Matrix[Row][Col] == Other.Matrix[Row][Col] )
                    return false;
            }
        }
        return true;
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Arithmetic Operators With Matrix3x3

    Matrix3x3 Matrix3x3::operator+(const Matrix3x3& Other) const
    {
        Matrix3x3 Ret;
        for( Whole Row = 0 ; Row < 3 ; ++Row )
        {
            for( Whole Col = 0 ; Col < 3 ; ++Col )
            {
                Ret.Matrix[Row][Col] = Matrix[Row][Col] + Other.Matrix[Row][Col];
            }
        }
        return Ret;
    }

    Matrix3x3 Matrix3x3::operator-(const Matrix3x3& Other) const
    {
        Matrix3x3 Ret;
        for( Whole Row = 0 ; Row < 3 ; ++Row )
        {
            for( Whole Col = 0 ; Col < 3 ; ++Col )
            {
                Ret.Matrix[Row][Col] = Matrix[Row][Col] - Other.Matrix[Row][Col];
            }
        }
        return Ret;
    }

    Matrix3x3 Matrix3x3::operator*(const Matrix3x3& Other) const
    {
        Matrix3x3 Ret;
        for( Whole Row = 0 ; Row < 3 ; ++Row )
        {
            for( Whole Col = 0 ; Col < 3 ; ++Col )
            {
                Ret.Matrix[Row][Col] =
                    Matrix[Row][0] * Other.Matrix[0][Col] +
                    Matrix[Row][1] * Other.Matrix[1][Col] +
                    Matrix[Row][2] * Other.Matrix[2][Col];
            }
        }
        return Ret;
    }

    Matrix3x3& Matrix3x3::operator+=(const Matrix3x3& Other)
    {
        for( Whole Row = 0 ; Row < 3 ; ++Row )
        {
            for( Whole Col = 0 ; Col < 3 ; ++Col )
            {
                Matrix[Row][Col] += Other.Matrix[Row][Col];
            }
        }
        return *this;
    }

    Matrix3x3& Matrix3x3::operator-=(const Matrix3x3& Other)
    {
        for( Whole Row = 0 ; Row < 3 ; ++Row )
        {
            for( Whole Col = 0 ; Col < 3 ; ++Col )
            {
                Matrix[Row][Col] -= Other.Matrix[Row][Col];
            }
        }
        return *this;
    }

    Matrix3x3& Matrix3x3::operator*=(const Matrix3x3& Other)
    {
        (*this) = (*this) * Other;
        return *this;
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Arithmetic Operators With Other Datatypes

    Vector3 Matrix3x3::operator*(const Vector3& Vec) const
    {
        Vector3 Ret;

        Ret.X = Matrix[0][0] * Vec.X + Matrix[0][1] * Vec.Y + Matrix[0][2] * Vec.Z;
        Ret.Y = Matrix[1][0] * Vec.X + Matrix[1][1] * Vec.Y + Matrix[1][2] * Vec.Z;
        Ret.Z = Matrix[2][0] * Vec.X + Matrix[2][1] * Vec.Y + Matrix[2][2] * Vec.Z;

        return Ret;
    }

    Matrix3x3 Matrix3x3::operator*(const Real& Scaler) const
    {
        Matrix3x3 Ret;
        for( Whole Row = 0 ; Row < 3 ; ++Row )
        {
            for( Whole Col = 0 ; Col < 3 ; ++Col )
            {
                Ret.Matrix[Row][Col] = Matrix[Row][Col] * Scaler;
            }
        }
        return Ret;
    }

    Matrix3x3& Matrix3x3::operator*=(const Real& Scaler)
    {
        for( Whole Row = 0 ; Row < 3 ; ++Row )
        {
            for( Whole Col = 0 ; Col < 3 ; ++Col )
            {
                Matrix[Row][Col] *= Scaler;
            }
        }
        return *this;
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Other Operators

    void Matrix3x3::operator=(const Matrix3x3& Other)
    {
        for( Whole Row = 0 ; Row < 3 ; ++Row )
        {
            for( Whole Col = 0 ; Col < 3 ; ++Col )
            {
                Matrix[Row][Col] = Other.Matrix[Row][Col];
            }
        }
    }

    Matrix3x3 Matrix3x3::operator-() const
    {
        Matrix3x3 Ret;
        for( Whole Row = 0 ; Row < 3 ; ++Row )
        {
            for( Whole Col = 0 ; Col < 3 ; ++Col )
            {
                Ret.Matrix[Row][Col] =  -Matrix[Row][Col];
            }
        }
        return Ret;
    }

    Real* Matrix3x3::operator[](const Whole Row)
        { return this->Matrix[Row]; }

    const Real* Matrix3x3::operator[](const Whole Row) const
        { return this->Matrix[Row]; }

    ///////////////////////////////////////////////////////////////////////////////
    // Fancy Math

    Matrix3x3 Matrix3x3::Transpose() const
    {
        Matrix3x3 Ret;
        for( Whole Row = 0 ; Row < 3 ; Row++ )
        {
            for( Whole Col = 0 ; Col < 3 ; Col++ )
                Ret.Matrix[Row][Col] = Matrix[Col][Row];
        }
        return Ret;
    }

    Matrix3x3 Matrix3x3::Adjoint() const
    {
        return Matrix3x3(CoFactor(1,1,2,2), CoFactor(0,2,2,1), CoFactor(0,1,1,2),
                         CoFactor(1,2,2,0), CoFactor(0,0,2,2), CoFactor(0,2,1,0),
                         CoFactor(1,0,2,1), CoFactor(0,1,2,0), CoFactor(0,0,1,1));
    }

    Matrix3x3 Matrix3x3::Inverse() const
    {
        Matrix3x3 Ret;
        Ret.Matrix[0][0] = Matrix[1][1] * Matrix[2][2] - Matrix[1][2] * Matrix[2][1];
        Ret.Matrix[0][1] = Matrix[0][2] * Matrix[2][1] - Matrix[0][1] * Matrix[2][2];
        Ret.Matrix[0][2] = Matrix[0][1] * Matrix[1][2] - Matrix[0][2] * Matrix[1][1];
        Ret.Matrix[1][0] = Matrix[1][2] * Matrix[2][0] - Matrix[1][0] * Matrix[2][2];
        Ret.Matrix[1][1] = Matrix[0][0] * Matrix[2][2] - Matrix[0][2] * Matrix[2][0];
        Ret.Matrix[1][2] = Matrix[0][2] * Matrix[1][0] - Matrix[0][0] * Matrix[1][2];
        Ret.Matrix[2][0] = Matrix[1][0] * Matrix[2][1] - Matrix[1][1] * Matrix[2][0];
        Ret.Matrix[2][1] = Matrix[0][1] * Matrix[2][0] - Matrix[0][0] * Matrix[2][1];
        Ret.Matrix[2][2] = Matrix[0][0] * Matrix[1][1] - Matrix[0][1] * Matrix[1][0];

        Real Det = Matrix[0][0] * Ret.Matrix[0][0] + Matrix[0][1] * Ret.Matrix[1][0] + Matrix[0][2] * Ret.Matrix[2][0];

        if( 0 == Det )
            { MEZZ_EXCEPTION(ExceptionBase::ARITHMETIC_EXCEPTION,"Determinant calculated in Matrix3x3::Inverse is zero.  Avoiding divide by zero."); }

        Real InvDet = 1.0 / Det;
        for( Whole Row = 0 ; Row < 3 ; Row++)
        {
            for( Whole Col = 0 ; Col < 3 ; Col++)
                Ret.Matrix[Row][Col] *= InvDet;
        }
        return Ret;
    }

    Real Matrix3x3::CoFactor(const Whole& Row1, const Whole& Col1, const Whole& Row2, const Whole& Col2) const
    {
        return Matrix[Row1][Col1] * Matrix[Row2][Col2] - Matrix[Row1][Col2] * Matrix[Row2][Col1];
    }

    void Matrix3x3::SetScale(const Vector3& Scaling)
    {
        Matrix[0][0] *= Scaling.X;
        Matrix[0][1] *= Scaling.Y;
        Matrix[0][2] *= Scaling.Z;
        Matrix[1][0] *= Scaling.X;
        Matrix[1][1] *= Scaling.Y;
        Matrix[1][2] *= Scaling.Z;
        Matrix[2][0] *= Scaling.X;
        Matrix[2][1] *= Scaling.Y;
        Matrix[2][2] *= Scaling.Z;
    }

    Boole Matrix3x3::HasScaling() const
    {
        Real Test = Matrix[0][0] * Matrix[0][0] + Matrix[1][0] * Matrix[1][0] + Matrix[2][0] * Matrix[2][0];
        if (!MathTools::WithinTolerance(Test,1.0,(Real)1e-04))
            return true;
        Test = Matrix[0][1] * Matrix[0][1] + Matrix[1][1] * Matrix[1][1] + Matrix[2][1] * Matrix[2][1];
        if (!MathTools::WithinTolerance(Test,1.0,(Real)1e-04))
            return true;
        Test = Matrix[0][2] * Matrix[0][2] + Matrix[1][2] * Matrix[1][2] + Matrix[2][2] * Matrix[2][2];
        if (!MathTools::WithinTolerance(Test,1.0,(Real)1e-04))
            return true;

        return false;
    }
}//Mezzanine

#endif
