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
#ifndef _vector3_cpp
#define _vector3_cpp

#include "vector3.h"
#include "quaternion.h"
#include "exception.h"
#include "serialization.h"
#include "stringtool.h"
#include "MathTools/mathtools.h"
#ifndef SWIG
    #include "XML/xml.h"
#endif
#include "XML/xml.h"            // Needed for streaming to xml

#include <Ogre.h>
#include "btBulletDynamicsCommon.h"

#include <memory>

//remove this
#include <iostream>

namespace Mezzanine
{
    ///////////////////////////////////////////////////////////////////////////////
    // The Essentials

    Real Vector3::GetAxisValue(const StandardAxis& Axis) const
    {
        switch(Axis)
        {
            case 0: return this->X;
            case 1: return this->Y;
            case 2: return this->Z;
            default: { MEZZ_EXCEPTION(ExceptionBase::PARAMETERS_EXCEPTION,"Cannot retrieve invalid StandardAxis."); }
        }
    }

    Real Vector3::GetAxisValue(const Whole& Axis) const
        { return this->GetAxisValue((StandardAxis)Axis); }

    Real& Vector3::GetAxisValue(const StandardAxis& Axis)
    {
        switch(Axis)
        {
            case 0: return this->X;
            case 1: return this->Y;
            case 2: return this->Z;
            default: { MEZZ_EXCEPTION(ExceptionBase::PARAMETERS_EXCEPTION,"Cannot retrieve invalid StandardAxis."); }
        }
    }

    Real& Vector3::GetAxisValue(const Whole& Axis)
        { return this->GetAxisValue((StandardAxis)Axis); }

    Real Vector3::operator[] (const StandardAxis& Axis) const
        { return this->GetAxisValue(Axis); }

    Real Vector3::operator[] (const Whole& Axis) const
        { return this->GetAxisValue((StandardAxis)Axis); }

    Real& Vector3::operator[] (const StandardAxis& Axis)
        { return this->GetAxisValue(Axis); }

    Real& Vector3::operator[] (const Whole& Axis)
        { return this->GetAxisValue((StandardAxis)Axis); }

    ///////////////////////////////////////////////////////////////////////////////
    // Constructors

    Vector3::Vector3()
        { this->Zero(); }

    Vector3::Vector3(const Real& x, const Real& y, const Real& z)
        { this->SetValues(x,y,z); }

    Vector3::Vector3(const Ogre::Vector3& Vec)
        { this->ExtractOgreVector3(Vec); }

    Vector3::Vector3(const btVector3& Vec)
        { this->ExtractBulletVector3(Vec); }

    Vector3::Vector3(const Mezzanine::Vector3& Vec)
        { *this = Vec; }

    Vector3::Vector3(XML::Node OneNode)
        { this->ProtoDeSerialize(OneNode); }

    ///////////////////////////////////////////////////////////////////////////////
    // Prebuilt Vectors

    Vector3 Vector3::Unit_X()
        { return Vector3(1,0,0); }

    Vector3 Vector3::Unit_Y()
        { return Vector3(0,1,0);}

    Vector3 Vector3::Unit_Z()
        { return Vector3(0,0,1); }

    Vector3 Vector3::Neg_Unit_X()
        { return Vector3(-1,0,0); }

    Vector3 Vector3::Neg_Unit_Y()
        { return Vector3(0,-1,0); }

    Vector3 Vector3::Neg_Unit_Z()
        { return Vector3(0,0,-1); }

    Vector3 Vector3::UnitOnAxis(StandardAxis Axis)
    {
        switch(Axis)
        {
            case 0: return Vector3::Unit_X();
            case 1: return Vector3::Unit_Y();
            case 2: return Vector3::Unit_Z();
            default: { MEZZ_EXCEPTION(ExceptionBase::PARAMETERS_EXCEPTION,"Cannot convert invalid StandardAxis."); }
        }
    }

    StandardAxis Vector3::IsStandardUnitAxis() const
    {
        if (1.0==this->X && 0.0==this->Y && 0.0==this->Z)
        {
            return Axis_X;
        } else if (0.0==this->X) {                                         // Not Unit_X
            if (1.0==this->Y && 0.0==this->Z)
            {
                return Axis_Y;
            } else if (0.0==this->Y && 1.0==this->Z) {                                     // Not Unit_Y so hopefully it is Z
                return Axis_Z;
            }
        }
        return Axis_Invalid;
        //MEZZ_EXCEPTION(ExceptionBase::INVALID_STATE_EXCEPTION,"Cannot convert Vector3 to StandardAxis, Vector3 may not be Axis Aligned or may not be Unit Length.");
    }


    ///////////////////////////////////////////////////////////////////////////////
    // Assignment Operators

    Vector3& Vector3::operator= (const btVector3 &Vec)
    {
        this->X=Vec.getX();
        this->Y=Vec.getY();
        this->Z=Vec.getZ();
        return *this;
    }

    Vector3& Vector3::operator= (const Ogre::Vector3 &Vec)
    {
        this->X=Vec.x;
        this->Y=Vec.y;
        this->Z=Vec.z;
        return *this;
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Unary Operators

    Vector3 Vector3::operator- ()
        { return Vector3(-X,-Y,-Z); }

    ///////////////////////////////////////////////////////////////////////////////
    // Vector3 Arithmetic with Real

    Vector3 Vector3::operator* (const Real &scalar) const
        { return Vector3(this->X * scalar, this->Y * scalar, this->Z * scalar); }

    Vector3 Vector3::operator/ (const Real &scalar) const
        { return Vector3(this->X / scalar, this->Y / scalar, this->Z / scalar); }

    ///////////////////////////////////////////////////////////////////////////////
    // Vector3 Arithmetic and assignment with Real

    Vector3& Vector3::operator*= (const Real &scalar)
    {
        this->X *= scalar;
        this->Y *= scalar;
        this->Z *= scalar;
        return *this;
    }

    Vector3& Vector3::operator/= (const Real &scalar)
    {
        this->X /= scalar;
        this->Y /= scalar;
        this->Z /= scalar;
        return *this;
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Equality Comparison operators

    Boole Vector3::operator== (const Vector3 &Vec) const
        { return( Vec.X == this->X && Vec.Y == this->Y && Vec.Z == this->Z ); }

    Boole Vector3::operator== (const btVector3 &Vec) const
        { return( Vec.getX() == this->X && Vec.getY() == this->Y && Vec.getZ() == this->Z ); }

    Boole Vector3::operator== (const Ogre::Vector3 &Vec) const
        { return ( Vec.x == this->X && Vec.y == this->Y && Vec.z == this->Z ); }


    Boole Vector3::operator!= (const Vector3 &Vec) const
        { return ( Vec.X != this->X || Vec.Y != this->Y || Vec.Z != this->Z ); }

    Boole Vector3::operator!= (const btVector3 &Vec) const
        { return ( Vec.getX() != this->X || Vec.getY() != this->Y || Vec.getZ() != this->Z ); }

    Boole Vector3::operator!= (const Ogre::Vector3 &Vec) const
        { return ( Vec.x != this->X || Vec.y != this->Y || Vec.z != this->Z ); }

    Boole Vector3::operator<= (const Mezzanine::Vector3 &Vec) const
        { return ( this->X <= Vec.X && this->Y <= Vec.Y && this->Z <= Vec.Z); }
    Boole Vector3::operator>= (const Mezzanine::Vector3 &Vec) const
        { return ( this->X >= Vec.X && this->Y >= Vec.Y && this->Z >= Vec.Z); }

    ///////////////////////////////////////////////////////////////////////////////
    // Arithmetic Operators

    Vector3 Vector3::operator+ (const Vector3& Vec) const
        { return Vector3(X+Vec.X, Y+Vec.Y, Z+Vec.Z ); }

    Vector3 Vector3::operator- (const Vector3& Vec) const
        { return Vector3(X-Vec.X, Y-Vec.Y, Z-Vec.Z ); }

    Vector3 Vector3::operator* (const Vector3& Vec) const
        { return Vector3(X*Vec.X, Y*Vec.Y, Z*Vec.Z ); }

    Vector3 Vector3::operator/ (const Vector3& Vec) const
        { return Vector3(X/Vec.X, Y/Vec.Y, Z/Vec.Z ); }

    Vector3& Vector3::operator+= (const Vector3& Vec)
    {
        this->X += Vec.X;
        this->Y += Vec.Y;
        this->Z += Vec.Z;
        return *this;
    }

    Vector3& Vector3::operator-= (const Vector3& Vec)
    {
        this->X -= Vec.X;
        this->Y -= Vec.Y;
        this->Z -= Vec.Z;
        return *this;
    }

    Vector3& Vector3::operator*= (const Vector3& Vec)
    {
        this->X *= Vec.X;
        this->Y *= Vec.Y;
        this->Z *= Vec.Z;
        return *this;
    }

    Vector3& Vector3::operator/= (const Vector3& Vec)
    {
        this->X /= Vec.X;
        this->Y /= Vec.Y;
        this->Z /= Vec.Z;
        return *this;
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Arithmetic Operators with btVector3

    Vector3 Vector3::operator+ (const btVector3  &Vec) const
        { return Vector3(X+Vec.getX(), Y+Vec.getY(), Z+Vec.getZ()); }

    Vector3 Vector3::operator- (const btVector3  &Vec) const
        { return Vector3(X-Vec.getX(), Y-Vec.getY(), Z-Vec.getZ()); }

    Vector3 Vector3::operator* (const btVector3  &Vec) const
        { return Vector3(X*Vec.getX(), Y*Vec.getY(), Z*Vec.getZ()); }

    Vector3 Vector3::operator/ (const btVector3  &Vec) const
        { return Vector3(X/Vec.getX(), Y/Vec.getY(), Z/Vec.getZ()); }

    ///////////////////////////////////////////////////////////////////////////////
    // Arithmetic Operators with Ogre::Vector3

    Vector3 Vector3::operator+ (const Ogre::Vector3 &Vec) const
        { return Vector3(X+Vec.x, Y+Vec.y, Z+Vec.z); }

    Vector3 Vector3::operator- (const Ogre::Vector3 &Vec) const
        { return Vector3(X-Vec.x, Y-Vec.y, Z-Vec.z); }

    Vector3 Vector3::operator* (const Ogre::Vector3 &Vec) const
        { return Vector3(X*Vec.x, Y*Vec.y, Z*Vec.z); }

    Vector3 Vector3::operator/ (const Ogre::Vector3 &Vec) const
        { return Vector3(X/Vec.x, Y/Vec.y, Z/Vec.z); }

    ///////////////////////////////////////////////////////////////////////////////
    // Fancy Math

    Vector3 Vector3::CrossProduct( const Vector3& Vec ) const
    {
        return Vector3(                                        // 1,2,3 . 4,5,6
                this->Y * Vec.Z - this->Z * Vec.Y,             // 2*6-3*5 = -3
                this->Z * Vec.X - this->X * Vec.Z,             // 3*4-1*6 = 6
                this->X * Vec.Y - this->Y * Vec.X              // 1*5-2*4 = -3
            );
    }

    Real Vector3::DotProduct(const Vector3& Vec) const
    {
        return this->X * Vec.X + this->Y * Vec.Y + this->Z * Vec.Z;
    }

    Vector3& Vector3::Normalize()
    {
        Real TempLength = this->Distance( Vector3(0.0f,0.0f,0.0f) );
        if( 0 != TempLength ) {
             (*this) /= TempLength;
        }else{
            MEZZ_EXCEPTION(ExceptionBase::ARITHMETIC_EXCEPTION,"Cannot Normalize Vector3(0,0,0).");
        }
        return *this;
    }

    Vector3 Vector3::GetNormal() const
    {
        Real TempLength = this->Distance( Vector3(0.0f,0.0f,0.0f) );
        if( 0 != TempLength ) {
            return (*this) / TempLength;
        }else{
            MEZZ_EXCEPTION(ExceptionBase::ARITHMETIC_EXCEPTION,"Cannot Get the Normal of Vector3(0,0,0).");
        }
    }

    Real Vector3::AngleBetween(const Vector3& Direction) const
    {
        Real LengthProduct = this->Length() * Direction.Length();
        // Divide by zero check
        if( LengthProduct < 1e-6f ) {
            LengthProduct = 1e-6f;
        }

        // Sorry about the variable name.  :(
        Real Temp = this->DotProduct(Direction) / LengthProduct;
        Temp = MathTools::Clamp(Temp, Real(-1.0), Real(1.0) );
        return MathTools::ACos(Temp);
    }

    Vector3& Vector3::Permute()
    {
        *this = this->GetPermute();
        return *this;
    }

    Vector3 Vector3::GetPermute() const
    {
        return Vector3(this->Z,this->X,this->Y);
    }

    Vector3& Vector3::AntiPermute()
    {
        *this = this->GetAntiPermute();
        return *this;
    }

    Vector3 Vector3::GetAntiPermute() const
    {
        return Vector3(this->Y,this->Z,this->X);
    }

    Vector3 Vector3::GetDirection(const Vector3& Destination) const
    {
        return (Destination - *this).Normalize();
    }

    Vector3 Vector3::Perpendicular() const
    {
        static const Real fSquareZero = (Real)(1e-06 * 1e-06);

        Vector3 Perp = this->CrossProduct( Vector3::Unit_X() );
        if( Perp.SquaredLength() < fSquareZero ) {
            // If we're here, then this is vector is on the X axis already.  Use another axis.
            Perp = this->CrossProduct( Vector3::Unit_Y() );
        }
        Perp.Normalize();

        return Perp;
    }

    Boole Vector3::IsPerpendicular(const Vector3& Perp) const
    {
        return ( this->DotProduct(Perp) == 0 );
    }

    Vector3 Vector3::Inverse()
    {
        if( X != 0 )
            X = 1 / X;
        if( Y != 0 )
            Y = 1 / Y;
        if( Z != 0 )
            Z = 1 / Z;
        return *this;
    }

    Vector3 Vector3::Reflect(const Vector3& Normal)
    {
        return Vector3( *this - ( Normal * (2 * this->DotProduct(Normal) ) ) );
    }

    Real Vector3::Distance(const Vector3& OtherVec) const
    {
        return (*this - OtherVec).Length();
    }

    Real Vector3::SquaredDistance(const Vector3& OtherVec) const
    {
        return (*this - OtherVec).SquaredLength();
    }

    Real Vector3::Length() const
    {
        return MathTools::Sqrt(X * X + Y * Y + Z * Z);
    }

    Real Vector3::SquaredLength() const
    {
        return (X * X + Y * Y + Z * Z);
    }

    Boole Vector3::IsZeroLength() const
    {
        return SquaredLength() < (1e-06 * 1e-06);
    }

    Quaternion Vector3::GetRotationToAxis(const Vector3& Axis, const Vector3& FallBackAxis) const
    {
        Quaternion Ret;
        Vector3 Vec1 = *this;
        Vector3 Vec2 = Axis;
        Vec1.Normalize();
        Vec2.Normalize();

        Real Dot = Vec1.DotProduct(Vec2);
        if( Dot >= 1.0 )
        {
            return Ret;
        }
        if( Dot < (1e-6 - 1.0) )
        {
            if( FallBackAxis != Vector3() )
            {
                Ret.SetFromAxisAngle(MathTools::GetPi(),FallBackAxis);
            }else{
                Vector3 CrossAxis = Vector3::Unit_X().CrossProduct(*this);
                if(CrossAxis.IsZeroLength())
                    CrossAxis = Vector3::Unit_Y().CrossProduct(*this);
                CrossAxis.Normalize();
                Ret.SetFromAxisAngle(MathTools::GetPi(),CrossAxis);
            }
        }else{
            Real Sqr = MathTools::Sqrt( (1+Dot)*2 );
            Real InvSqr = 1 / Sqr;

            Vector3 Cross = Vec1.CrossProduct(Vec2);

            Ret.X = Cross.X * InvSqr;
            Ret.Y = Cross.Y * InvSqr;
            Ret.Z = Cross.Z * InvSqr;
            Ret.W = Sqr * 0.5f;
            Ret.Normalize();
        }
        return Ret;
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Utility Functions

    void Vector3::Zero()
    {
        this->X = 0;
        this->Y = 0;
        this->Z = 0;
    }

    void Vector3::SetValues(const Real& X, const Real& Y, const Real& Z)
    {
        this->X = X;
        this->Y = Y;
        this->Z = Z;
    }

    Boole Vector3::IsZero() const
    {
        return ( this->X == 0.0 && this->Y == 0.0 && this->Z == 0.0 );
    }

    Vector3& Vector3::Ceil(const Vector3& Other)
    {
        if( Other.X > this->X ) this->X = Other.X;
        if( Other.Y > this->Y ) this->Y = Other.Y;
        if( Other.Z > this->Z ) this->Z = Other.Z;
        return *this;
    }

    Vector3& Vector3::Floor(const Vector3& Other)
    {
        if( Other.X < this->X ) this->X = Other.X;
        if( Other.Y < this->Y ) this->Y = Other.Y;
        if( Other.Z < this->Z ) this->Z = Other.Z;
        return *this;
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Manual Conversions

    btVector3 Vector3::GetBulletVector3() const
    {
        btVector3 Theirs;
        Theirs.setX(this->X);
        Theirs.setY(this->Y);
        Theirs.setZ(this->Z);
        Theirs.setW(0);
        return Theirs;
    }

    void Vector3::ExtractBulletVector3(const btVector3& Ours)
    {
        this->X=Ours.getX();
        this->Y=Ours.getY();
        this->Z=Ours.getZ();
    }

    Ogre::Vector3 Vector3::GetOgreVector3() const
    {
        Ogre::Vector3 Theirs;
        Theirs.x=this->X;
        Theirs.y=this->Y;
        Theirs.z=this->Z;
        return Theirs;
    }

    void Vector3::ExtractOgreVector3(const Ogre::Vector3& Ours)
    {
        this->X=Ours.x;
        this->Y=Ours.y;
        this->Z=Ours.z;
    }

    void Vector3::ProtoSerialize(XML::Node& CurrentRoot) const
    {
        Mezzanine::XML::Node VecNode = CurrentRoot.AppendChild(GetSerializableName());

        if(VecNode)
        {
            Mezzanine::XML::Attribute VersionAttr = VecNode.AppendAttribute("Version");
            Mezzanine::XML::Attribute XAttr = VecNode.AppendAttribute("X");
            Mezzanine::XML::Attribute YAttr = VecNode.AppendAttribute("Y");
            Mezzanine::XML::Attribute ZAttr = VecNode.AppendAttribute("Z");
            if( VersionAttr && XAttr && YAttr && ZAttr )
            {
                if( VersionAttr.SetValue("1") && XAttr.SetValue(this->X) && YAttr.SetValue(this->Y) && ZAttr.SetValue(this->Z))
                {
                    return;
                }else{
                    SerializeError("Create XML Attribute Values", GetSerializableName(),true);
                }
            }else{
                SerializeError("Create XML Attributes", GetSerializableName(),true);
            }
        }else{
            SerializeError("Create XML Serialization Node", GetSerializableName(),true);
        }
    }

    void Vector3::ProtoDeSerialize(const XML::Node& OneNode)
    {
        if ( Mezzanine::String(OneNode.Name())==Mezzanine::String(GetSerializableName()) )
        {
            if(OneNode.GetAttribute("Version").AsInt() == 1)
            {
                this->X=OneNode.GetAttribute("X").AsReal();
                this->Y=OneNode.GetAttribute("Y").AsReal();
                this->Z=OneNode.GetAttribute("Z").AsReal();
            }else{
                MEZZ_EXCEPTION(ExceptionBase::INVALID_VERSION_EXCEPTION,"Incompatible XML Version for " + GetSerializableName() + ": Not Version 1.");
            }
        }else{
            MEZZ_EXCEPTION(ExceptionBase::II_IDENTITY_INVALID_EXCEPTION,"Attempting to deserialize a " + GetSerializableName() + ", found a " + String(OneNode.Name()) + ".");
        }
    }

    String Vector3::GetSerializableName()
        { return String("Vector3"); }

    const char* Vector3::__str__()
    {
        const Whole BufferSize=64; // longest possible vector3 is 59: <Vector3 Version="1" X="1.33333" Y="2.33333" Z="3.33333" />
        static char buffer[BufferSize];
        String Temp(ToString(*this));
        assert(Temp.size()<BufferSize);
        strncpy(buffer,Temp.c_str(),BufferSize);
        return buffer;
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Right Hand Arithmetic Operators

    Mezzanine::Vector3 operator+ (const btVector3  &Vec, const Mezzanine::Vector3& lhs)
        { return lhs + Vec; }
    Mezzanine::Vector3 operator- (const btVector3  &Vec, const Mezzanine::Vector3& lhs)
        { return Mezzanine::Vector3(Vec.getX()-lhs.X, Vec.getY()-lhs.Y, Vec.getZ()-lhs.Z); }
    Mezzanine::Vector3 operator* (const btVector3  &Vec, const Mezzanine::Vector3& lhs)
        { return lhs * Vec; }
    Mezzanine::Vector3 operator/ (const btVector3  &Vec, const Mezzanine::Vector3& lhs)
        { return Mezzanine::Vector3(Vec.getX()/lhs.X, Vec.getY()/lhs.Y, Vec.getZ()/lhs.Z); }

    Mezzanine::Vector3 operator+ (const Ogre::Vector3 &Vec, const Mezzanine::Vector3& lhs)
        { return lhs + Vec; }
    Mezzanine::Vector3 operator- (const Ogre::Vector3 &Vec, const Mezzanine::Vector3& lhs)
        { return Mezzanine::Vector3(Vec.x-lhs.X, Vec.y-lhs.Y, Vec.z-lhs.Z); }
    Mezzanine::Vector3 operator* (const Ogre::Vector3 &Vec, const Mezzanine::Vector3& lhs)
        { return lhs * Vec; }
    Mezzanine::Vector3 operator/ (const Ogre::Vector3 &Vec, const Mezzanine::Vector3& lhs)
        { return Mezzanine::Vector3(Vec.x/lhs.X, Vec.y/lhs.Y, Vec.z/lhs.Z); }

    ///////////////////////////////////////////////////////////////////////////////
    // Vector2LengthCompare methods

    Boole Vector3LengthCompare::operator()(const Vector3& First, const Vector3& Second) const
    {
        if( ( First - Second ).SquaredLength() < 1e-6 )
			return false;
		if( MathTools::Abs( First.X - Second.X ) > 1e-3 )
			return ( First.X < Second.X );
		if( MathTools::Abs( First.Y - Second.Y ) > 1e-3 )
			return ( First.Y < Second.Y );
		return ( First.Z < Second.Z );
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Class External << Operators for streaming or assignment

    std::ostream& operator << (std::ostream& stream, const Mezzanine::Vector3& x)
    {
        //stream << "<Vector3 Version=\"1\" X=\"" << x.X << "\" Y=\"" << x.Y << "\" Z=\"" << x.Z << "\"/>";
        Serialize(stream,x);
        return stream;
    }

    std::istream& operator >> (std::istream& stream, Mezzanine::Vector3& Vec)
        { return DeSerialize(stream, Vec); }

    void operator >> (const Mezzanine::XML::Node& OneNode, Mezzanine::Vector3& Vec)
        { Vec.ProtoDeSerialize(OneNode); }

    Ogre::Vector3& operator << (Ogre::Vector3& VecTo, const Mezzanine::Vector3& VecFrom)
    {
        VecTo = VecFrom.GetOgreVector3();
        return VecTo;
    }

    Ogre::Vector3& operator << (Ogre::Vector3& VecTo, const btVector3& VecFrom)
    {
        VecTo.x=VecFrom.getX();
        VecTo.y=VecFrom.getY();
        VecTo.z=VecFrom.getZ();
        return VecTo;
    }

    btVector3& operator << (btVector3& VecTo, const Ogre::Vector3& VecFrom)
    {
        VecTo.setX(VecFrom.x);
        VecTo.setY(VecFrom.y);
        VecTo.setZ(VecFrom.z);
        VecTo.setW(0);
        return VecTo;
    }

    btVector3& operator << (btVector3& VecTo, const Mezzanine::Vector3& VecFrom)
    {
        VecTo=VecFrom.GetBulletVector3();
        return VecTo;
    }

    Mezzanine::Vector3& operator << (Mezzanine::Vector3& VecTo, const Ogre::Vector3& VecFrom)
    {
        VecTo=VecFrom;
        return VecTo;
    }

    Mezzanine::Vector3& operator << (Mezzanine::Vector3& VecTo, const btVector3& VecFrom)
    {
        VecTo=VecFrom;
        return VecTo;
    }
}

#endif
