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
#ifndef _vector2_cpp
#define _vector2_cpp

#include "serialization.h"
#include "stringtool.h"
#include "vector2.h"
#include "MathTools/mathtools.h"
#include "exception.h"

//#include <memory>

#include <Ogre.h>

namespace Mezzanine
{
    Vector2::Vector2()
        { this->SetIdentity(); }

    Vector2::Vector2(const Real& xy)
        { this->SetValues(xy, xy); }

    Vector2::Vector2(const Real& x, const Real& y)
        { this->SetValues(x,y); }

    Vector2::Vector2(const Ogre::Vector2& Vec)
        { this->ExtractOgreVector2(Vec); }

    Ogre::Vector2 Vector2::GetOgreVector2() const
    {
        Ogre::Vector2 Theirs;
        Theirs.x = this->X;
        Theirs.y = this->Y;
        return Theirs;
    }

    void Vector2::ExtractOgreVector2(const Ogre::Vector2& Ours)
    {
        this->X = Ours.x;
        this->Y = Ours.y;
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Prebuilt Vectors

    Vector2 Vector2::Unit_X()
        { return Vector2(1,0); }

    Vector2 Vector2::Unit_Y()
        { return Vector2(0,1); }

    Vector2 Vector2::Neg_Unit_X()
        { return Vector2(-1,0); }

    Vector2 Vector2::Neg_Unit_Y()
        { return Vector2(0,-1); }

    ///////////////////////////////////////////////////////////////////////////////
    // Utility

    void Vector2::SetIdentity()
    {
        this->X = 0;
        this->Y = 0;
    }

    void Vector2::SetValues(const Real& x, const Real& y)
    {
        this->X = x;
        this->Y = y;
    }

    Boole Vector2::IsZero() const
    {
        return ( this->X == 0.0 && this->Y == 0.0 );
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Equality Comparison operators

    Boole Vector2::operator==(const Mezzanine::Vector2& Vec2) const
        { return ( Vec2.X == this->X && Vec2.Y == this->Y ); }

    Boole Vector2::operator!=(const Mezzanine::Vector2& Vec2) const
        { return ( Vec2.X != this->X || Vec2.Y != this->Y ); }

    Boole Vector2::operator==(const Ogre::Vector2& Vec2) const
        { return ( Vec2.x == this->X && Vec2.y == this->Y ); }

    Boole Vector2::operator!=(const Ogre::Vector2& Vec2) const
        { return ( Vec2.x != this->X || Vec2.y != this->Y ); }

    Boole Vector2::operator<= (const Mezzanine::Vector2 &Vec) const
        { return ( this->X <= Vec.X && this->Y <= Vec.Y); }

    Boole Vector2::operator>= (const Mezzanine::Vector2 &Vec) const
        { return ( this->X >= Vec.X && this->Y >= Vec.Y); }

    ///////////////////////////////////////////////////////////////////////////////
    // Unary Operators

    Vector2 Vector2::operator- ()
        { return Vector2( -(this->X), -(this->Y) ); }

    ///////////////////////////////////////////////////////////////////////////////
    // Vector2 Arithmetic with Real

    Vector2 Vector2::operator* (const Real& scalar) const
    {
        return Vector2(
            this->X * scalar,
            this->Y * scalar
        );
    }

    Vector2 Vector2::operator/ (const Real& scalar) const
    {
        return Vector2(
            this->X / scalar,
            this->Y / scalar
        );
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Vector2 Arithmetic and assignment with Real

    Vector2& Vector2::operator*= (const Real& scalar)
    {
        this->X *= scalar;
        this->Y *= scalar;
        return *this;
    }

    Vector2& Vector2::operator/= (const Real& scalar)
    {
        this->X /= scalar;
        this->Y /= scalar;
        return *this;
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Arithmetic Operators

    Vector2 Vector2::operator+ (const Vector2& Vec2) const
    {
        Vector2 Temp(X,Y);
        Temp.X += Vec2.X;
        Temp.Y += Vec2.Y;
        return Temp;
    }

    Vector2 Vector2::operator- (const Vector2& Vec2) const
    {
        Vector2 Temp(X,Y);
        Temp.X -= Vec2.X;
        Temp.Y -= Vec2.Y;
        return Temp;
    }

    Vector2 Vector2::operator* (const Vector2& Vec2) const
    {
        Vector2 Temp(X,Y);
        Temp.X *= Vec2.X;
        Temp.Y *= Vec2.Y;
        return Temp;
    }

    Vector2 Vector2::operator/ (const Vector2& Vec2) const
    {
        Vector2 Temp(X,Y);
        Temp.X /= Vec2.X;
        Temp.Y /= Vec2.Y;
        return Temp;
    }

    Vector2& Vector2::operator+= (const Vector2& Vec2)
    {
        this->X += Vec2.X;
        this->Y += Vec2.Y;
        return *this;
    }

    Vector2& Vector2::operator-= (const Vector2& Vec2)
    {
        this->X -= Vec2.X;
        this->Y -= Vec2.Y;
        return *this;
    }

    Vector2& Vector2::operator*= (const Vector2& Vec2)
    {
        this->X *= Vec2.X;
        this->Y *= Vec2.Y;
        return *this;
    }

    Vector2& Vector2::operator/= (const Vector2& Vec2)
    {
        this->X /= Vec2.X;
        this->Y /= Vec2.Y;
        return *this;
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Fancy Math

    Real Vector2::CrossProduct(const Vector2& Other) const
        { return ( this->X * Other.Y ) - ( this->Y * Other.X ); }

    Real Vector2::DotProduct(const Vector2& Other) const
        { return ( this->X * Other.X ) + ( this->Y * Other.Y ); }

    Real Vector2::Distance(const Vector2& Other) const
        { return ( *this - Other ).Length(); }

    Real Vector2::SquaredDistance(const Vector2& Other) const
        { return ( *this - Other ).SquaredLength(); }

    Real Vector2::Length() const
        { return MathTools::Sqrt( this->SquaredLength() ); }

    Real Vector2::SquaredLength() const
        { return ( this->X * this->X + this->Y * this->Y ); }

    Vector2 Vector2::Perpendicular() const
        { return Vector2(-Y,X); }

    Vector2 Vector2::Reflect(const Vector2& Normal) const
        { return Vector2( *this - ( Normal * ( 2 * this->DotProduct(Normal) ) ) ); }

    Vector2& Vector2::Normalize()
    {
        Real Length = this->Length();

        if( Length > 1e-08 ) {
            Real InvLength = 1.0 / Length;
            X *= InvLength;
            Y *= InvLength;
        }

        return *this;
    }

    Vector2 Vector2::GetNormal() const
    {
        Vector2 Ret( *this );
        return Ret.Normalize();
    }

    Real Vector2::AngleTo(const Vector2& Other) const
    {
        Real Angle = this->AngleBetween(Other);

		if( this->CrossProduct(Other) < 0 )
			Angle = MathTools::GetTwoPi() - Angle;

		return Angle;
    }

    Real Vector2::AngleBetween(const Vector2& Other) const
    {
        Real LenProduct = this->Length() * Other.Length();
		// Divide by zero check
		if( LenProduct < 1e-6f )
			LenProduct = 1e-6f;

		Real f = this->DotProduct(Other) / LenProduct;

		f = MathTools::Clamp( f, (Real)-1.0, (Real)1.0 );
		return MathTools::ACos(f);
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Serialization

    void Vector2::ProtoSerialize(XML::Node& CurrentRoot) const
    {
        Mezzanine::XML::Node VecNode = CurrentRoot.AppendChild(GetSerializableName());
        VecNode.SetName(GetSerializableName());

        Mezzanine::XML::Attribute VersionAttr = VecNode.AppendAttribute("Version");
        Mezzanine::XML::Attribute XAttr = VecNode.AppendAttribute("X");
        Mezzanine::XML::Attribute YAttr = VecNode.AppendAttribute("Y");
        if( VersionAttr && XAttr && YAttr )
        {
            if( VersionAttr.SetValue("1") && XAttr.SetValue(this->X) && YAttr.SetValue(this->Y) )
            {
                return;
            }else{
                SerializeError("Create XML Attribute Values", GetSerializableName(),true);
            }
        }else{
            SerializeError("Create XML Attributes", GetSerializableName(),true);
        }
    }

    void Vector2::ProtoDeSerialize(const XML::Node& OneNode)
    {
        if ( Mezzanine::String(OneNode.Name())==Mezzanine::String(GetSerializableName()) )
        {
            if(OneNode.GetAttribute("Version").AsInt() == 1)
            {
                this->X=OneNode.GetAttribute("X").AsReal();
                this->Y=OneNode.GetAttribute("Y").AsReal();
            }else{
                MEZZ_EXCEPTION(ExceptionBase::INVALID_VERSION_EXCEPTION,"Incompatible XML Version for " + GetSerializableName() + ": Not Version 1.");
            }
        }else{
            MEZZ_EXCEPTION(ExceptionBase::II_IDENTITY_INVALID_EXCEPTION,"Attempting to deserialize a " + GetSerializableName() + ", found a " + String(OneNode.Name()) + ".");
        }
    }

    String Vector2::GetSerializableName()
        { return String("Vector2"); }

    ///////////////////////////////////////////////////////////////////////////////
    // Vector2LengthCompare methods

    Boole Vector2LengthCompare::operator()(const Vector2& First, const Vector2& Second) const
    {
        if( ( First - Second ).SquaredLength() < 1e-6 )
			return false;
		if( MathTools::Abs( First.X - Second.X ) > 1e-3 )
			return ( First.X < Second.X );
		return ( First.Y < Second.Y );
    }
}//Mezzanine

///////////////////////////////////////////////////////////////////////////////
// Class External << Operators for streaming or assignment
std::ostream& operator << (std::ostream& stream, const Mezzanine::Vector2& x)
{

        //stream << "<Vector2 Version=\"1\" X=\"" << x.X << "\" Y=\"" << x.Y << "\" />";
        Serialize(stream,x);
    return stream;
}

std::istream& MEZZ_LIB operator >> (std::istream& stream, Mezzanine::Vector2& Vec)
    { return DeSerialize(stream, Vec); }

void operator >> (const Mezzanine::XML::Node& OneNode, Mezzanine::Vector2& Vec)
    { Vec.ProtoDeSerialize(OneNode); }

#endif
