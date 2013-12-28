// © Copyright 2010 - 2013 BlackTopp Studios Inc.
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
#ifndef _spheretests_h
#define _spheretests_h

#include "mezztest.h"

#include "axisalignedbox.h"
#include "plane.h"
#include "ray.h"
#include "sphere.h"
#include "XML/xml.h"

#include <stdexcept> //only used to throw for TEST_THROW

#include <Ogre.h>

// Needed because windows sucks and uses macros improperly
#ifdef CreateDirectory
#undef CreateDirectory
#endif
// Needed because windows sucks and uses macros improperly
#ifdef RemoveDirectory
#undef RemoveDirectory
#endif

/// @file
/// @brief This file should be used as template/example for building future Unit Tests

using namespace Mezzanine;
using namespace Mezzanine::Testing;

/// @brief A small series of sample tests, which can be used as a boilerplate so creating new test groups
class spheretests : public UnitTestGroup
{
public:
    /// @copydoc Mezzanine::Testing::UnitTestGroup::Name
    /// @return Returns a String containing "Sphere"
    virtual String Name()
        { return String("Sphere"); }

    /// @brief Test most features on the Sphere.
    void RunAutomaticTests()
    {
        {
            Sphere Blank;
            TEST( Blank.Center == Vector3(0.0,0.0,0.0) && Blank.Radius == 0.0 ,"Sphere()");
        }

        {
            Vector3 Cen(10.0,10.0,10.0);
            Real Rad = 10.0;
            Sphere Ball(Cen,Rad);
            Sphere Copy(Ball);
            TEST( Copy.Center == Vector3(10.0,10.0,10.0) && Copy.Radius == 10.0 ,"Sphere(const_Sphere&)");
        }

        {
            Real Rad = 10.0;
            Sphere Ball(Rad);
            TEST( Ball.Center == Vector3(0.0,0.0,0.0) && Ball.Radius == 10.0 ,"Sphere(const_Real)");
        }

        {
            Vector3 Cen(10.0,10.0,10.0);
            Real Rad = 10.0;
            Sphere Ball(Cen,Rad);
            TEST( Ball.Center == Vector3(10.0,10.0,10.0) && Ball.Radius == 10.0 ,"Sphere(const_Vector3&,_const_Real)");
        }

        {
            Ogre::Vector3 Cen(10.0,10.0,10.0);
            Ogre::Real Rad = 10.0;
            Ogre::Sphere Ball(Cen,Rad);
            Sphere Copy(Ball);
            TEST( Copy.Center == Vector3(10.0,10.0,10.0) && Copy.Radius == 10.0 ,"Sphere(const_Ogre::Sphere&)");
        }

        {
            Sphere Ball(Vector3(10.0,10.0,10.0),10.0);
            Vector3 PassPoint(5.0,7.0,5.0);
            Vector3 FailPoint(1.0,1.0,1.0);
            TEST( Ball.IsInside(PassPoint) && !Ball.IsInside(FailPoint) ,"IsInside(const_Vector3&)_const");
        }

        {
            Sphere OverlapPass1(Vector3(0.0,-5.0,0.0),6.0);
            Sphere OverlapPass2(Vector3(0.0,5.0,0.0),6.0);
            Sphere OverlapFail1(Vector3(-5.0,-5.0,-5.0),5.0);
            Sphere OverlapFail2(Vector3(5.0,20.0,15.0),10.0);
            TEST( OverlapPass1.IsOverlapping(OverlapPass2) && !OverlapFail1.IsOverlapping(OverlapFail2) ,"IsOverlapping(const_Sphere&)_const");
        }

        {
            AxisAlignedBox PassBox(Vector3(3.0,3.0,3.0),Vector3(8.0,8.0,8.0));
            AxisAlignedBox FailBox(Vector3(-5.0,-5.0,-5.0),Vector3(1.0,1.0,1.0));
            Sphere Ball(Vector3(10.0,10.0,10.0),10.0);
            TEST( Ball.IsOverlapping(PassBox) && !Ball.IsOverlapping(FailBox) ,"IsOverlapping(const_AxisAlignedBox&)_const");
        }

        {
            Plane PassSurface(Vector3(1.0,1.0,1.0).Normalize(),10);
            Plane FailSurface(Vector3(3.0,1.0,9.0).Normalize(),50);
            Sphere Ball(Vector3(10.0,10.0,10.0),10.0);
            TEST( Ball.IsOverlapping(PassSurface) && !Ball.IsOverlapping(FailSurface) ,"IsOverlapping(const_Plane&)_const");
        }

        {
            Ray PassRay(Vector3(-20.0,10.0,10.0),Vector3(40.0,10.0,10.0));
            Ray FailRay(Vector3(100.0,40.0,50.0),Vector3(80.0,60.0,50.0));
            Sphere Ball(Vector3(10.0,10.0,10.0),10.0);
            Sphere::RayTestResult PassResult = Ball.Intersects(PassRay);
            Sphere::RayTestResult FailResult = Ball.Intersects(FailRay);
            TEST( ( PassResult.first && PassResult.second == Ray(Vector3(0.0,10.0,10.0),Vector3(20.0,10.0,10.0)) ) && ( !FailResult.first && FailResult.second == Ray() ) ,"Intersects(const_Ray&)_const");
        }

        {
            Ogre::Vector3 Cen(10.0,10.0,10.0);
            Ogre::Real Rad = 10.0;
            Ogre::Sphere Ball(Cen,Rad);
            Sphere Copy;
            Copy.ExtractOgreSphere(Ball);
            TEST( Copy.Center == Vector3(10.0,10.0,10.0) && Copy.Radius == 10.0 ,"ExtractOgreSphere(const_Ogre::Sphere&)");
        }

        {
            Vector3 Cen(10.0,10.0,10.0);
            Real Rad = 10.0;
            Sphere Ball(Cen,Rad);
            Ogre::Sphere Converted = Ball.GetOgreSphere();
            TEST( Converted.getCenter() == Ogre::Vector3(10.0,10.0,10.0) && Converted.getRadius() == 10.0 ,"GetOgreSphere()_const");
        }

        {
            String Expected( "<?xml version=\"1.0\"?><Sphere Version=\"1\" Radius=\"8.5\"><Center><Vector3 Version=\"1\" X=\"-4.5\" Y=\"12.5\" Z=\"7.5\" /></Center></Sphere>" );

            XML::Document Doc;
            Sphere Ball(Vector3(-4.5,12.5,7.5),8.5);
            Ball.ProtoSerialize(Doc);
            StringStream Buffer;
            Doc.Save(Buffer);

            TEST( Expected == String( Buffer.str() ) ,"ProtoSerialize(XML::Node&)_const");
        }

        {
            String Source( "<?xml version=\"1.0\"?><Sphere Version=\"1\" Radius=\"8.5\"><Center><Vector3 Version=\"1\" X=\"-4.5\" Y=\"12.5\" Z=\"7.5\" /></Center></Sphere>" );

            XML::Document Doc;
            StringStream Buffer;
            Buffer.str( Source );
            Doc.Load(Buffer);

            Sphere Ball;
            Ball.ProtoDeSerialize(Doc.GetFirstChild());

            TEST( Ball == Sphere(Vector3(-4.5,12.5,7.5),8.5) ,"ProtoDeSerialize(const_XML::Node&)");
        }

        {
            String ClassName("Sphere");
            TEST( ClassName == Sphere::GetSerializableName() ,"GetSerializableName()");
        }

        {
            Vector3 Cen(10.0,10.0,10.0);
            Real Rad = 10.0;
            Sphere Ball(Cen,Rad);
            Sphere Copy(Ball);
            Copy = Ball;
            TEST( Copy.Center == Vector3(10.0,10.0,10.0) && Copy.Radius == 10.0 ,"operator=(const_Sphere&)");
        }

        {
            Ogre::Vector3 Cen(10.0,10.0,10.0);
            Ogre::Real Rad = 10.0;
            Ogre::Sphere Ball(Cen,Rad);
            Sphere Copy;
            Copy = Ball;
            TEST( Copy.Center == Vector3(10.0,10.0,10.0) && Copy.Radius == 10.0 ,"operator=(const_Ogre::Sphere&)");
        }

        {
            Sphere GreaterBall(Vector3(20.0,5.0,10.0),25.0);
            Sphere LesserBall(Vector3(10.0,15.0,5.0),5.0);
            TEST( ( GreaterBall > LesserBall ) && !( LesserBall > GreaterBall ) ,"operator>(const_Sphere&)_const");
        }

        {
            Sphere GreaterBall(Vector3(20.0,5.0,10.0),25.0);
            Sphere LesserBall(Vector3(10.0,15.0,5.0),5.0);
            TEST( ( LesserBall < GreaterBall ) && !( GreaterBall < LesserBall ) ,"operator<(const_Sphere&)_const");
        }

        {
            Sphere Ball(Vector3(10.0,10.0,10.0),10.0);
            Sphere GreaterBall(Vector3(20.0,5.0,10.0),25.0);
            Sphere EqualBall(Vector3(10.0,10.0,10.0),10.0);
            Sphere LesserBall(Vector3(10.0,15.0,5.0),5.0);
            TEST( ( GreaterBall >= Ball && EqualBall >= Ball ) && !( LesserBall >= Ball ) ,"operator>=(const_Sphere&)_const");
        }

        {
            Sphere Ball(Vector3(10.0,10.0,10.0),10.0);
            Sphere GreaterBall(Vector3(20.0,5.0,10.0),25.0);
            Sphere EqualBall(Vector3(10.0,10.0,10.0),10.0);
            Sphere LesserBall(Vector3(10.0,15.0,5.0),5.0);
            TEST( ( LesserBall <= Ball && EqualBall <= Ball ) && !( GreaterBall <= Ball ) ,"operator<=(const_Sphere&)_const");
        }

        {
            Sphere Ball(Vector3(10.0,10.0,10.0),10.0);
            Sphere BallPass(Vector3(10.0,10.0,10.0),10.0);
            Sphere BallFail(Vector3(20.0,5.0,10.0),25.0);
            TEST( ( Ball == BallPass ) && !( Ball == BallFail ) ,"operator==(const_Sphere&)_const");
        }

        {
            Sphere Ball(Vector3(10.0,10.0,10.0),10.0);
            Sphere BallPass(Vector3(20.0,5.0,10.0),25.0);
            Sphere BallFail(Vector3(10.0,10.0,10.0),10.0);
            TEST( ( Ball != BallPass ) && !( Ball != BallFail ) ,"operator!=(const_Sphere&)_const");
        }
    }

    /// @brief Since RunAutomaticTests is implemented so is this.
    /// @return returns true
    virtual bool HasAutomaticTests() const
        { return true; }
};//spheretests

#endif
