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
    - Are Currently In Violation of AnOther Copyright Holder's GPL License
    - If You want to change our code and not add a few hundred MB of stuff to
        your distribution

   These and Other limitations could cause serious legal problems if you ignore
   them, so it is best to simply contact us or the Free Software Foundation, if
   you have any questions.

   Joseph Toppi - toppij@gmail.com
   John Blackwood - makoenergy02@gmail.com
*/
/*
-----------------------------------------------------------------------------
This source file is part of ogre-procedural

For the latest info, see http://www.ogreprocedural.org

Copyright (c) 2010-2013 Michael Broutin

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
-----------------------------------------------------------------------------
*/
#ifndef _triangle_cpp
#define _triangle_cpp

#include "triangle.h"
#include "linesegment.h"
#include "plane.h"
#include "ray.h"
#include "mathtools.h"
#include "exception.h"

namespace
{
    void isect( Mezzanine::Real VV0, Mezzanine::Real VV1, Mezzanine::Real VV2,
                Mezzanine::Real D0, Mezzanine::Real D1, Mezzanine::Real D2,
                Mezzanine::Real& isect0, Mezzanine::Real& isect1 )
    {
        isect0 = VV0 + ( VV1 - VV0 ) * D0 / ( D0 - D1 );
        isect1 = VV0 + ( VV2 - VV0 ) * D0 / ( D0 - D2 );
    }

    void ComputeIntervals( Mezzanine::Real VV0, Mezzanine::Real VV1, Mezzanine::Real VV2,
                           Mezzanine::Real D0, Mezzanine::Real D1, Mezzanine::Real D2,
                           Mezzanine::Real D0D1, Mezzanine::Real D0D2,
                           Mezzanine::Real& isect0, Mezzanine::Real& isect1 )
    {
        if( D0D1 > 0.0 ) {
            /* here we know that D0D2<=0.0 */
            /* that is D0, D1 are on the same side, D2 on the Other or on the plane */
            isect(VV2,VV0,VV1,D2,D0,D1,isect0,isect1);
        }else if( D0D2 > 0.0 ) {
            /* here we know that d0d1<=0.0 */
            isect(VV1,VV0,VV2,D1,D0,D2,isect0,isect1);
        }else if( D1 * D2 > 0.0f || D0 != 0.0 ) {
            /* here we know that d0d1<=0.0 or that D0!=0.0 */
            isect(VV0,VV1,VV2,D0,D1,D2,isect0,isect1);
        }else if( D1 != 0.0 ) {
            isect(VV1,VV0,VV2,D1,D0,D2,isect0,isect1);
        }else if( D2 != 0.0 ) {
            isect(VV2,VV0,VV1,D2,D0,D1,isect0,isect1);
        }else{
            /* triangles are coplanar */
            //return coplanar_tri_tri(N1,V0,V1,V2,U0,U1,U2);
        }
    }
}

namespace Mezzanine
{
    ///////////////////////////////////////////////////////////////////////////////
    // Triangle2D Methods

    ///////////////////////////////////////////////////////////////////////////////
    // Construction and Destruction

    Triangle2D::Triangle2D()
        {  }

    Triangle2D::Triangle2D(const Vector2& A, const Vector2& B, const Vector2& C) :
        PointA(A),
        PointB(B),
        PointC(C)
        {  }

    ///////////////////////////////////////////////////////////////////////////////
    // Utility

    Vector2& Triangle2D::operator[](const Whole& Index)
    {
        switch( Index )
        {
            case 0:  return this->PointA;  break;
            case 1:  return this->PointB;  break;
            case 2:  return this->PointC;  break;
            default:
                MEZZ_EXCEPTION(Exception::PARAMETERS_RANGE_EXCEPTION,"Triangle indexes are only valid if in the range of: 0-2.");
        }
        return this->PointA;
    }

    const Vector2& Triangle2D::operator[](const Whole& Index) const
    {
        switch( Index )
        {
            case 0:  return this->PointA;  break;
            case 1:  return this->PointB;  break;
            case 2:  return this->PointC;  break;
            default:
                MEZZ_EXCEPTION(Exception::PARAMETERS_RANGE_EXCEPTION,"Triangle indexes are only valid if in the range of: 0-2.");
        }
        return this->PointA;
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Triangle3D Methods

    ///////////////////////////////////////////////////////////////////////////////
    // Construction and Destruction

    Triangle3D::Triangle3D()
        {  }

    Triangle3D::Triangle3D(const Vector3& A, const Vector3& B, const Vector3& C) :
        PointA(A),
        PointB(B),
        PointC(C)
        {  }

    ///////////////////////////////////////////////////////////////////////////////
    // Utility

    LineSegment3D Triangle3D::GetOverlap(const Triangle3D& Other) const
    {
        // Compute plane equation of first triangle
        Vector3 e1 = this->PointB - this->PointA;
        Vector3 e2 = this->PointC - this->PointA;
        Vector3 n1 = e1.CrossProduct(e2);
        Real d1 = -( n1.DotProduct(this->PointA) );

        // Put second triangle in first plane equation to compute distances to the plane
        Real du[3];
        for( short i = 0 ; i < 3; i++ )
        {
            Vector3 Vec;
            if( i == 0 ) Vec = Other.PointA;
            else if( i == 1 ) Vec = Other.PointB;
            else if( i == 2 ) Vec = Other.PointC;
            du[i] = n1.DotProduct( Vec ) + d1;
            if( MathTools::Fabs( du[i] ) < 1e-6 )
                du[i] = 0.0;
        }

        Real du0du1 = du[0] * du[1];
        Real du0du2 = du[0] * du[2];

        if( du0du1 > 0.0 && du0du2 > 0.0 ) /* same sign on all of them + not equal 0 ? */
            return LineSegment3D(Vector3(0,0,0),Vector3(0,0,0)); /* no intersection occurs */

        // Compute plane equation of escond triangle
        e1 = Other.PointB - Other.PointA;
        e2 = Other.PointC - Other.PointA;
        Vector3 n2 = e1.CrossProduct(e2);
        Real d2 = -( n2.DotProduct(Other.PointA) );

        // Put first triangle in second plane equation to compute distances to the plane
        Real dv[3];
        for( short i = 0 ; i < 3 ; i++ )
        {
            Vector3 Vec;
            if( i == 0 ) Vec = this->PointA;
            else if( i == 1 ) Vec = this->PointB;
            else if( i == 2 ) Vec = this->PointC;
            dv[i] = n2.DotProduct( Vec ) + d2;
            if( MathTools::Fabs( dv[i] ) < 1e-6 )
                dv[i] = 0.0;
        }

        Real dv0dv1 = dv[0] * dv[1];
        Real dv0dv2 = dv[0] * dv[2];

        if( dv0dv1 > 0.0 && dv0dv2 > 0.0 ) /* same sign on all of them + not equal 0 ? */
            return LineSegment3D(Vector3(0,0,0),Vector3(0,0,0)); /* no intersection occurs */

        //Compute the direction of intersection line
        Vector3 d = n1.CrossProduct(n2);

        // We don't do coplanar triangles
        if( d.SquaredLength() < 1e-6 )
            return LineSegment3D(Vector3(0,0,0),Vector3(0,0,0));

        // Project triangle points onto the intersection line

        /* compute and index to the largest component of D */
        Real Max = MathTools::Fabs( d[0] );
        Integer index = 0;
        Real b = MathTools::Fabs( d[1] );
        Real c = MathTools::Fabs( d[2] );
        if ( b > Max )
            Max = b, index = 1;
        if ( c > Max )
            Max = c, index = 2;

        /* this is the simplified projection onto L*/
        Real vp0 = this->PointA[index];
        Real vp1 = this->PointB[index];
        Real vp2 = this->PointC[index];

        Real up0 = Other.PointA[index];
        Real up1 = Other.PointB[index];
        Real up2 = Other.PointC[index];

        Real isect1[2];
        Real isect2[2];
        /* compute interval for triangle 1 */
        ComputeIntervals(vp0,vp1,vp2,dv[0],dv[1],dv[2],dv0dv1,dv0dv2,isect1[0],isect1[1]);

        /* compute interval for triangle 2 */
        ComputeIntervals(up0,up1,up2,du[0],du[1],du[2],du0du1,du0du2,isect2[0],isect2[1]);

        if( isect1[0] > isect1[1] )
            std::swap(isect1[0],isect1[1]);
        if( isect2[0] > isect2[1] )
            std::swap(isect2[0],isect2[1]);

        if( isect1[1] < isect2[0] || isect2[1] < isect1[0] )
            return LineSegment3D(Vector3(0,0,0),Vector3(0,0,0));

        // Deproject segment onto line
        Real r1 = std::max( isect1[0], isect2[0] );
        Real r2 = std::min( isect1[1], isect2[1] );

        Plane pl1(n1, d1);
        Plane pl2(n2, d2);
        Ray InterLine = pl1.GetOverlap(pl2);
        Vector3 p = InterLine.GetOrigin();
        d.Normalize();
        Vector3 v1 = p + d * ( ( r1 - p[index] ) / d[index] );
        Vector3 v2 = p + d * ( ( r2 - p[index] ) / d[index] );

        return LineSegment3D(v1,v2);
    }

    Vector3& Triangle3D::operator[](const Whole& Index)
    {
        switch( Index )
        {
            case 0:  return this->PointA;  break;
            case 1:  return this->PointB;  break;
            case 2:  return this->PointC;  break;
            default:
                MEZZ_EXCEPTION(Exception::PARAMETERS_RANGE_EXCEPTION,"Triangle indexes are only valid if in the range of: 0-2.");
        }
        return this->PointA;
    }

    const Vector3& Triangle3D::operator[](const Whole& Index) const
    {
        switch( Index )
        {
            case 0:  return this->PointA;  break;
            case 1:  return this->PointB;  break;
            case 2:  return this->PointC;  break;
            default:
                MEZZ_EXCEPTION(Exception::PARAMETERS_RANGE_EXCEPTION,"Triangle indexes are only valid if in the range of: 0-2.");
        }
        return this->PointA;
    }
}//Mezzanine

#endif
