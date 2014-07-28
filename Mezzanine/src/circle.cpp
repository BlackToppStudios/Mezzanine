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
#ifndef _circle_cpp
#define _circle_cpp

#include "circle.h"

namespace Mezzanine
{
    ///////////////////////////////////////////////////////////////////////////////
    // Construction and Destruction

    Circle::Circle() :
        Radius(0)
        { this->Center.SetIdentity(); }

    Circle::Circle(const Vector2& CircleCenter, const Real CircleRadius) :
        Center(CircleCenter),
        Radius(CircleRadius)
        {  }

    Circle::Circle(const Vector2& Point1, const Vector2& Point2, const Vector2& Point3)
    {
        Vector2 c1 = ( Point1 + Point2 ) * 0.5;
        Vector2 d1 = ( Point2 - Point1 ).Perpendicular();
        Real a1 = d1.Y;
        Real b1 = -d1.X;
        Real g1 = d1.X * c1.Y - d1.Y * c1.X;

        Vector2 c3 = ( Point2 + Point3 ) * 0.5;
        Vector2 d3 = ( Point3 - Point2 ).Perpendicular();
        Real a2 = d3.Y;
        Real b2 = -d3.X;
        Real g2 = d3.X * c3.Y - d3.Y * c3.X;

        Real IntersectX = ( b2 * g1 - b1 * g2 ) / ( b1 * a2 - b2 * a1 );
        Real IntersectY = ( a2 * g1 - a1 * g2 ) / ( a1 * b2 - a2 * b1 );

        Vector2 Intersect(IntersectX,IntersectY);

        this->Center = Intersect;
        this->Radius = ( Intersect - Point1 ).Length();
    }

    Circle::~Circle()
        {  }

    ///////////////////////////////////////////////////////////////////////////////
    // Utility

    Boole Circle::IsInside(const Vector2& Point) const
        { return ( Point - this->Center ).Length() < this->Radius; }
}//Mezzanine

#endif
