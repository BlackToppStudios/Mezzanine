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
#ifndef _axisalignedboxtests_h
#define _axisalignedboxtests_h

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
/// @brief This file runs a series of tests for all methods on the AxisAlignedBox geometry class.

using namespace Mezzanine;
using namespace Mezzanine::Testing;

/// @brief A small series of sample tests, which can be used as a boilerplate so creating new test groups
class axisalignedboxtests : public UnitTestGroup
{
public:
    /// @copydoc Mezzanine::Testing::UnitTestGroup::Name
    /// @return Returns a String containing "AxisAlignedBox"
    virtual String Name()
        { return String("AxisAlignedBox"); }

    /// @brief Test most features on the AxisAlignedBox.
    void RunAutomaticTests()
    {
        {
            AxisAlignedBox Blank;
            TEST(Blank.MinExt == Vector3(0.0,0.0,0.0), "AxisAlignedBox()_min");
            TEST(Blank.MaxExt == Vector3(0.0,0.0,0.0), "AxisAlignedBox()_max");
        }

        {
            Vector3 Min(-58.0,-34.0,-53.0);
            Vector3 Max(64.0,34.0,57.0);
            AxisAlignedBox Vector(Min,Max);
            TEST(Vector.MinExt == Min, "AxisAlignedBox(const_Vector3&,_const_Vector3&)_min");
            TEST(Vector.MaxExt == Max, "AxisAlignedBox(const_Vector3&,_const_Vector3&)_max");
        }

        {
            Vector3 Min(-45.0,-23.0,-19.0);
            Vector3 Max(38.0,16.0,15.0);
            AxisAlignedBox Box(Min,Max);
            AxisAlignedBox Copy(Box);
            TEST(Copy.MinExt == Min, "AxisAlignedBox(const_AxisAlignedBox&)_min");
            TEST(Copy.MaxExt == Max, "AxisAlignedBox(const_AxisAlignedBox&)_max");
        }

        {
            Ogre::Vector3 Min(-45.0,-23.0,-19.0);
            Ogre::Vector3 Max(38.0,16.0,15.0);
            Ogre::AxisAlignedBox Box(Min,Max);
            AxisAlignedBox Copy(Box);
            TEST(Copy.MinExt == Min, "AxisAlignedBox(const_Ogre::AxisAlignedBox&)_min");
            TEST(Copy.MaxExt == Max, "AxisAlignedBox(const_Ogre::AxisAlignedBox&)_max");
        }

        {
            AxisAlignedBox ZeroTest1(Vector3(2.0,2.0,3.0),Vector3(2.0,2.0,3.0));
            AxisAlignedBox ZeroTest2(Vector3(-2.0,-1.0,-1.0),Vector3(1.0,0.0,1.0));
            TEST(ZeroTest1.IsZero(), "IsZero()_const_positive");
            TEST(!ZeroTest2.IsZero(), "IsZero()_const_negative");
        }

        {
            AxisAlignedBox Volume(Vector3(0.0,0.0,0.0),Vector3(5.0,5.0,5.0));
            TEST(Volume.GetVolume() == 125.0 ,"GetVolume()_const");
        }

        {
            AxisAlignedBox Overlap1(Vector3(-4.0,-4.0,-4.0),Vector3(1.0,1.0,1.0));
            AxisAlignedBox Overlap2(Vector3(-1.0,-1.0,-1.0),Vector3(4.0,4.0,4.0));
            TEST(Overlap1.GetOverlap(Overlap2) == AxisAlignedBox(Vector3(-1.0,-1.0,-1.0),Vector3(1.0,1.0,1.0)), "GetOverlap(const_AxisAlignedBox&)_const");
        }

        {
            Vector3 Point(-1.0,3.0,6.0);
            AxisAlignedBox Expanded(Vector3(0.0,0.0,0.0),Vector3(5.0,5.0,5.0));
            Expanded.Expand(Point);
            TEST(Expanded == AxisAlignedBox(Vector3(-1.0,0.0,0.0),Vector3(5.0,5.0,6.0)) ,"Expand(const_Vector3&)");
        }

        {
            AxisAlignedBox Box(Vector3(-3.0,-3.0,-3.0),Vector3(1.0,1.0,1.0));
            AxisAlignedBox Expanded(Vector3(0.0,0.0,0.0),Vector3(5.0,5.0,5.0));
            Expanded.Expand(Box);
            TEST(Expanded == AxisAlignedBox(Vector3(-3.0,-3.0,-3.0),Vector3(5.0,5.0,5.0)) ,"Expand(const_AxisAlignedBox&)");
        }

        {
            Vector3 PassPoint(1.0,1.0,2.0);
            Vector3 FailPoint(-2.0,1.0,-3.0);
            AxisAlignedBox Box(Vector3(0.0,0.0,0.0),Vector3(5.0,5.0,5.0));
            TEST(Box.IsInside(PassPoint), "IsInside(const_Vector3&)_const_positive");
            TEST(!Box.IsInside(FailPoint), "IsInside(const_Vector3&)_const_negative");
        }

        {
            Sphere PassBall(Vector3(-0.5,5.5,-0.5),2);
            Sphere FailBall(Vector3(10.0,10.0,10.0),3);
            AxisAlignedBox Box(Vector3(0.0,0.0,0.0),Vector3(5.0,5.0,5.0));
            TEST(Box.IsOverlapping(PassBall), "IsOverlapping(const_Sphere&)_const_positive");
            TEST(!Box.IsOverlapping(FailBall), "IsOverlapping(const_Sphere&)_const_negative");
        }

        {
            AxisAlignedBox OverlapPass1(Vector3(-4.0,-4.0,-4.0),Vector3(1.0,1.0,1.0));
            AxisAlignedBox OverlapPass2(Vector3(-1.0,-1.0,-1.0),Vector3(4.0,4.0,4.0));
            AxisAlignedBox OverlapFail1(Vector3(-5.0,-5.0,-5.0),Vector3(1.0,1.0,1.0));
            AxisAlignedBox OverlapFail2(Vector3(2.0,2.0,2.0),Vector3(5.0,5.0,5.0));
            TEST(OverlapPass1.IsOverlapping(OverlapPass2), "IsOverlapping(const_AxisAlignedBox&)_const_positive");
            TEST(!OverlapFail1.IsOverlapping(OverlapFail2), "IsOverlapping(const_AxisAlignedBox&)_const_negative");
        }

        {
            Plane PassSurface(Vector3(1.0,-1.0,1.0).Normalize(),2);
            Plane FailSurface(Vector3(2.0,3.0,-1.0).Normalize(),10);
            AxisAlignedBox Box(Vector3(0.0,0.0,0.0),Vector3(5.0,5.0,5.0));
            TEST(Box.IsOverlapping(PassSurface), "IsOverlapping(const_Plane&)_const");
            TEST(!Box.IsOverlapping(FailSurface), "IsOverlapping(const_Plane&)_const");
        }

        {
            AxisAlignedBox Box(Vector3(-4.0,-4.0,-4.0),Vector3(4.0,4.0,4.0));

            Ray PassRayOriginInside(Vector3(1.0,1.0,1.0),Vector3(20.0,20.0,20.0));
            AxisAlignedBox::RayTestResult PassResultOriginInside = Box.Intersects(PassRayOriginInside);
            TEST(PassResultOriginInside.first,"IntersectsOriginInside(const_Ray&)_const_boole_positive");
            TEST(PassResultOriginInside.second == Ray(Vector3(1.0,1.0,1.0),Vector3(4.0,4.0,4.0)), "IntersectsOriginInside(const_Ray&)_const_ray_positive");

            Ray PassRayHighOnX(Vector3(-20.0,0.0,0.0),Vector3(20.0,0.0,0.0));
            AxisAlignedBox::RayTestResult PassResultHighOnX = Box.Intersects(PassRayHighOnX);
            TEST(PassResultHighOnX.first,"IntersectsHighOnX(const_Ray&)_const_boole_positive");
            TEST(PassResultHighOnX.second == Ray(Vector3(-4.0,0.0,0.0),Vector3(4.0,0.0,0.0)), "IntersectsHighOnX(const_Ray&)_const_ray_positive");

            Ray PassRayLowOnX(Vector3(20.0,0.0,0.0),Vector3(-20.0,0.0,0.0));
            AxisAlignedBox::RayTestResult PassResultLowOnX = Box.Intersects(PassRayLowOnX);
            TEST(PassResultLowOnX.first,"IntersectsHighOnX(const_Ray&)_const_boole_positive");
            TEST(PassResultLowOnX.second == Ray(Vector3(4.0,0.0,0.0),Vector3(-4.0,0.0,0.0)), "IntersectsLowOnX(const_Ray&)_const_ray_positive");

            Ray PassRayHighOnY(Vector3(0.0,-20.0,0.0),Vector3(0.0,20.0,0.0));
            AxisAlignedBox::RayTestResult PassResultHighOnY = Box.Intersects(PassRayHighOnY);
            TEST(PassResultHighOnY.first,"IntersectsHighOnY(const_Ray&)_const_boole_positive");
            TEST(PassResultHighOnY.second == Ray(Vector3(0.0,-4.0,0.0),Vector3(0.0,4.0,0.0)), "IntersectsHighOnY(const_Ray&)_const_ray_positive");

            Ray PassRayLowOnY(Vector3(0.0,20.0,0.0),Vector3(0.0,-20.0,0.0));
            AxisAlignedBox::RayTestResult PassResultLowOnY = Box.Intersects(PassRayLowOnY);
            TEST(PassResultLowOnY.first,"IntersectsHighOnY(const_Ray&)_const_boole_positive");
            TEST(PassResultLowOnY.second == Ray(Vector3(0.0,4.0,0.0),Vector3(0.0,-4.0,0.0)), "IntersectsLowOnY(const_Ray&)_const_ray_positive");

            Ray PassRayHighOnZ(Vector3(0.0,0.0,-20.0),Vector3(0.0,0.0,20.0));
            AxisAlignedBox::RayTestResult PassResultHighOnZ = Box.Intersects(PassRayHighOnZ);
            TEST(PassResultHighOnZ.first,"IntersectsHighOnZ(const_Ray&)_const_boole_positive");
            TEST(PassResultHighOnZ.second == Ray(Vector3(0.0,0.0,-4.0),Vector3(0.0,0.0,4.0)), "IntersectsHighOnZ(const_Ray&)_const_ray_positive");

            Ray PassRayLowOnZ(Vector3(0.0,0.0,20.0),Vector3(0.0,0.0,-20.0));
            AxisAlignedBox::RayTestResult PassResultLowOnZ = Box.Intersects(PassRayLowOnZ);
            TEST(PassResultLowOnZ.first,"IntersectsHighOnZ(const_Ray&)_const_boole_positive");
            TEST(PassResultLowOnZ.second == Ray(Vector3(0.0,0.0,4.0),Vector3(0.0,0.0,-4.0)), "IntersectsLowOnZ(const_Ray&)_const_ray_positive");


            Ray FailRay(Vector3(100.0,40.0,50.0),Vector3(80.0,60.0,50.0));
            AxisAlignedBox::RayTestResult FailResult = Box.Intersects(FailRay);
            TEST(!FailResult.first, "Intersects(const_Ray&)_const_boole_negative");
            TEST(FailResult.second == Ray(), "Intersects(const_Ray&)_const_ray_negative");
        }

        {
            Vector3 Min(-58.0,-34.0,-53.0);
            Vector3 Max(64.0,34.0,57.0);
            AxisAlignedBox Extents;
            Extents.SetExtents(Min,Max);
            TEST(Extents.MinExt == Min, "SetExtents(const_Vector3&,_const_Vector3&)_min");
            TEST(Extents.MaxExt == Max, "SetExtents(const_Vector3&,_const_Vector3&)_max");
        }

        {
            AxisAlignedBox Size(Vector3(-4.0,-6.0,1.0),Vector3(12.0,10.0,4.0));
            TEST( Size.GetSize() == Vector3(16.0,16.0,3.0) ,"GetSize()_const");
        }

        {
            AxisAlignedBox HalfSize(Vector3(-4.0,-6.0,1.0),Vector3(12.0,10.0,4.0));
            TEST( HalfSize.GetHalfSize() == Vector3(8.0,8.0,1.5) ,"GetHalfSize()_const");
        }

        {
            AxisAlignedBox Center(Vector3(-4.0,-6.0,1.0),Vector3(12.0,10.0,4.0));
            TEST( Center.GetCenter() == Vector3(4.0,2.0,2.5) ,"GetCenter()_const");
        }

        {
            AxisAlignedBox Box(Vector3(-5.0,-8.0,-4.0),Vector3(3.0,10.0,4.0));
            TEST( Box.GetCorner(AxisAlignedBox::AE_Min,AxisAlignedBox::AE_Max,AxisAlignedBox::AE_Min) == Vector3(-5.0,10.0,-4.0),
                  "GetCorner(const_AxisExtent,_const_AxisExtent,_const_AxisExtent)_const");
        }

        {
            Ogre::AxisAlignedBox Extract(Ogre::Vector3(-4.0,-4.0,-4.0),Ogre::Vector3(4.0,4.0,4.0));
            AxisAlignedBox Box;
            Box.ExtractOgreAABB(Extract);
            TEST( Box == AxisAlignedBox(Vector3(-4.0,-4.0,-4.0),Vector3(4.0,4.0,4.0)) ,"ExtractOgreAABB(const_Ogre::AxisAlignedBox&)");
        }

        {
            AxisAlignedBox Convert(Vector3(-4.0,-4.0,-4.0),Vector3(4.0,4.0,4.0));
            TEST( Convert.GetOgreAABB() == Ogre::AxisAlignedBox(Ogre::Vector3(-4.0,-4.0,-4.0),Ogre::Vector3(4.0,4.0,4.0)) ,"GetOgreAABB()_const");
        }

        {
            String Expected( "<?xml version=\"1.0\"?><AxisAlignedBox Version=\"1\"><MinExt><Vector3 Version=\"1\" X=\"-1.5\" Y=\"-1.5\" Z=\"-1.5\" /></MinExt><MaxExt><Vector3 Version=\"1\" X=\"2.5\" Y=\"2.5\" Z=\"2.5\" /></MaxExt></AxisAlignedBox>" );

            XML::Document Doc;
            AxisAlignedBox Box(Vector3(-1.5,-1.5,-1.5),Vector3(2.5,2.5,2.5));
            Box.ProtoSerialize(Doc);
            StringStream Buffer;
            Doc.Save(Buffer);

            TEST( Expected == String( Buffer.str() ) ,"ProtoSerialize(XML::Node&)_const");
        }

        {
            String Source( "<?xml version=\"1.0\"?><AxisAlignedBox Version=\"1\"><MinExt><Vector3 Version=\"1\" X=\"-1.5\" Y=\"-1.5\" Z=\"-1.5\" /></MinExt><MaxExt><Vector3 Version=\"1\" X=\"2.5\" Y=\"2.5\" Z=\"2.5\" /></MaxExt></AxisAlignedBox>" );

            XML::Document Doc;
            StringStream Buffer;
            Buffer.str( Source );
            Doc.Load(Buffer);

            AxisAlignedBox Box;
            Box.ProtoDeSerialize(Doc.GetFirstChild());

            TEST( Box == AxisAlignedBox(Vector3(-1.5,-1.5,-1.5),Vector3(2.5,2.5,2.5)) ,"ProtoDeSerialize(const_XML::Node&)");
        }

        {
            String ClassName("AxisAlignedBox");
            TEST( ClassName == AxisAlignedBox::GetSerializableName() ,"GetSerializableName()");
        }

        {
            Vector3 Min(-45.0,-23.0,-19.0);
            Vector3 Max(38.0,16.0,15.0);
            AxisAlignedBox Box(Min,Max);
            AxisAlignedBox Copy;
            Copy = Box;
            TEST( Copy.MinExt == Min && Copy.MaxExt == Max ,"operator=(const_AxisAlignedBox&)");
        }

        {
            Ogre::Vector3 Min(-45.0,-23.0,-19.0);
            Ogre::Vector3 Max(38.0,16.0,15.0);
            Ogre::AxisAlignedBox Box(Min,Max);
            AxisAlignedBox Copy;
            Copy = Box;
            TEST(Copy.MinExt == Vector3(Min), "operator=(const_Ogre::AxisAlignedBox&)_min");
            TEST(Copy.MaxExt == Vector3(Max), "operator=(const_Ogre::AxisAlignedBox&)_max");
        }

        {
            AxisAlignedBox GreaterVolume(Vector3(0.0,0.0,0.0),Vector3(7.0,7.0,7.0));
            AxisAlignedBox LesserVolume(Vector3(0.0,0.0,0.0),Vector3(5.0,5.0,5.0));
            TEST(GreaterVolume > LesserVolume, "operator>(const_AxisAlignedBox&)_const_positive");
            TEST(!(LesserVolume > GreaterVolume), "operator>(const_AxisAlignedBox&)_const_negative");
        }

        {
            AxisAlignedBox GreaterVolume(Vector3(0.0,0.0,0.0),Vector3(6.0,6.0,6.0));
            AxisAlignedBox LesserVolume(Vector3(-3.0,-3.0,-3.0),Vector3(1.0,1.0,1.0));
            TEST(LesserVolume < GreaterVolume, "operator<(const_AxisAlignedBox&)_const_positive");
            TEST(!(GreaterVolume < LesserVolume), "operator<(const_AxisAlignedBox&)_const_negative");
        }

        {
            AxisAlignedBox Box(Vector3(-2.0,-2.0,-2.0),Vector3(2.0,2.0,2.0));
            AxisAlignedBox GreaterVolume(Vector3(-3.0,-3.5,-4.0),Vector3(3.0,3.5,4.0));
            AxisAlignedBox EqualVolume(Vector3(-2.0,-2.0,-2.0),Vector3(2.0,2.0,2.0));
            AxisAlignedBox LesserVolume(Vector3(-1.0,-0.5,-1.0),Vector3(1.0,0.5,1.0));
            TEST(GreaterVolume >= Box, "operator>=(const_AxisAlignedBox&)_const_positive_greater");
            TEST(EqualVolume >= Box, "operator>=(const_AxisAlignedBox&)_const_positive_equal");
            TEST(!(LesserVolume >= Box), "operator>=(const_AxisAlignedBox&)_const_negative");
        }

        {
            AxisAlignedBox Box(Vector3(-2.0,-2.0,-2.0),Vector3(2.0,2.0,2.0));
            AxisAlignedBox GreaterVolume(Vector3(-3.0,-3.5,-4.0),Vector3(3.0,3.5,4.0));
            AxisAlignedBox EqualVolume(Vector3(-2.0,-2.0,-2.0),Vector3(2.0,2.0,2.0));
            AxisAlignedBox LesserVolume(Vector3(-1.0,-0.5,-1.0),Vector3(1.0,0.5,1.0));
            TEST(LesserVolume <= Box, "operator<=(const_AxisAlignedBox&)_const_greater");
            TEST(EqualVolume <= Box, "operator<=(const_AxisAlignedBox&)_const_equal");
            TEST(!(GreaterVolume <= Box), "operator<=(const_AxisAlignedBox&)_const_negative");
        }

        {
            AxisAlignedBox Box(Vector3(0.0,0.0,0.0),Vector3(5.0,5.0,5.0));
            AxisAlignedBox BoxPass(Vector3(0.0,0.0,0.0),Vector3(5.0,5.0,5.0));
            AxisAlignedBox BoxFail(Vector3(-4.0,-4.0,-4.0),Vector3(4.0,4.0,4.0));
            TEST(Box == BoxPass, "operator==(const_AxisAlignedBox&)_const_positive");
            TEST(!(Box == BoxFail), "operator==(const_AxisAlignedBox&)_const_negative");
        }

        {
            AxisAlignedBox Box(Vector3(0.0,0.0,0.0),Vector3(5.0,5.0,5.0));
            AxisAlignedBox BoxPass(Vector3(-4.0,-4.0,-4.0),Vector3(4.0,4.0,4.0));
            AxisAlignedBox BoxFail(Vector3(0.0,0.0,0.0),Vector3(5.0,5.0,5.0));
            TEST(Box != BoxPass, "operator!=(const_AxisAlignedBox&)_const_positive");
            TEST(!(Box != BoxFail), "operator!=(const_AxisAlignedBox&)_const_negative");
        }
    }

    /// @brief Since RunAutomaticTests is implemented so is this.
    /// @return returns true
    virtual bool HasAutomaticTests() const
        { return true; }
};//axisalignedboxtests

#endif
