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
#ifndef _planetests_h
#define _planetests_h

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
class planetests : public UnitTestGroup
{
public:
    /// @copydoc Mezzanine::Testing::UnitTestGroup::Name
    /// @return Returns a String containing "Plane"
    virtual String Name()
        { return String("Plane"); }

    /// @brief Test most features on the Plane.
    void RunAutomaticTests()
    {
        {
            Plane Blank;
            TEST( Blank.Normal == Vector3(0,0,0) && Blank.Distance == 0 ,"Plane()");
        }

        {
            Vector3 Norm(-1.0,1.0,-1.0);
            Norm.Normalize();
            Real Const = 15;
            Plane Surface(Norm,Const);
            Plane Copy(Surface);
            TEST( Copy.Normal == Norm && Copy.Distance == -Const ,"Plane(const_Plane&)");
        }

        {
            Vector3 Norm(-1.0,1.0,-1.0);
            Norm.Normalize();
            Real Const = 15;
            Plane Surface(Norm,Const);
            TEST( Surface.Normal == Norm && Surface.Distance == -Const ,"Plane(const_Vector3&,_const_Real)");
        }

        {
            Vector3 Norm(0.0,1.0,0.0);
            Vector3 Point(10.0,1.0,0.0);
            Plane Surface(Norm,Point);
            TEST( Surface.Normal == Vector3(0.0,1.0,0.0) && Surface.Distance == -1.0 ,"Plane(const_Vector3&,_const_Vector3&)");
        }

        {
            Vector3 PointA(0.0,1.0,1.0);
            Vector3 PointB(-0.5,0.0,1.0);
            Vector3 PointC(0.5,0.0,1.0);
            Plane Surface(PointA,PointB,PointC);
            TEST( Surface.Normal == Vector3(0.0,0.0,1.0) && Surface.Distance == -1 ,"Plane(const_Vector3&,_const_Vector3&,_const_Vector3&)");
        }

        {
            Ogre::Vector3 Norm(-1.0,1.0,-1.0);
            Ogre::Real Const = 15;
            Ogre::Plane Surface(Norm,Const);
            Plane Copy(Surface);
            TEST( Copy.Normal == Vector3(-1.0,1.0,-1.0) && Copy.Distance == -15 ,"Plane(const_Ogre::Plane&)");
        }

        {
            Vector3 Norm(-1.0,1.0,-1.0);
            Norm.Normalize();
            Real Const = 15;
            Plane Surface;
            Surface.Define(Norm,Const);
            TEST( Surface.Normal == Norm && Surface.Distance == -Const ,"Define(const_Vector3&,_const_Real)");
        }

        {
            Vector3 Norm(0.0,1.0,0.0);
            Vector3 Point(10.0,1.0,0.0);
            Plane Surface;
            Surface.Define(Norm,Point);
            TEST( Surface.Normal == Vector3(0.0,1.0,0.0) && Surface.Distance == -1.0 ,"Define(const_Vector3&,_const_Vector3&)");
        }

        {
            Vector3 PointA(0.0,1.0,1.0);
            Vector3 PointB(-0.5,0.0,1.0);
            Vector3 PointC(0.5,0.0,1.0);
            Plane Surface;
            Surface.Define(PointA,PointB,PointC);
            TEST( Surface.Normal == Vector3(0.0,0.0,1.0) && Surface.Distance == -1 ,"Define(const_Vector3&,_const_Vector3&,_const_Vector3&)");
        }

        {
            Vector3 PosSide(15.0,10.0,-10.0);
            Vector3 NegSide(-1.0,0.5,-10.0);
            Plane Surface(Vector3(1.0,1.0,1.0).Normalize(),1);
            TEST( Surface.GetSide(PosSide) == Plane::S_Positive && Surface.GetSide(NegSide) == Plane::S_Negative ,"GetSide(const_Vector3&)_const");
        }

        {
            Vector3 Size(2.0,4.0,2.0);
            Vector3 PosCenter(10.0,20.0,10.0);
            Vector3 NegCenter(-10.0,-20.0,-10.0);
            Vector3 BothCenter(0.0,0.0,0.0);
            Plane Surface(Vector3(1.0,1.0,1.0).Normalize(),1);
            TEST( Surface.GetSide(PosCenter,Size) == Plane::S_Positive &&
                  Surface.GetSide(NegCenter,Size) == Plane::S_Negative &&
                  Surface.GetSide(BothCenter,Size) == Plane::S_Both
                  ,"GetSide(const_Vector3&,_const_Vector3&)_const");
        }

        {
            Vector3 Point(0.0,10.0,0.0);
            Plane Surface(Vector3(0.0,1.0,0.0),2);
            TEST( Surface.GetDistance(Point) == 8.0 ,"GetDistance(const_Vector3&)_const");
        }

        /// @todo Write a unit test for "Ray GetOverlap(const Plane& Other) const"

        {
            Sphere PassBall(Vector3(-55.0,25,30.0),10);
            Sphere FailBall(Vector3(10.0,10.0,10.0),5);
            Plane Surface(Vector3(0.0,1.0,0.0),20);
            TEST( Surface.IsOverlapping(PassBall) && !Surface.IsOverlapping(FailBall) ,"IsOverlapping(const_Sphere&)_const");
        }

        {
            AxisAlignedBox PassBox(Vector3(10.0,10.0,10.0),Vector3(40.0,40.0,40.0));
            AxisAlignedBox FailBox(Vector3(0.0,0.0,0.0),Vector3(5.0,5.0,5.0));
            Plane Surface(Vector3(0.0,1.0,0.0),20);
            TEST( Surface.IsOverlapping(PassBox) && !Surface.IsOverlapping(FailBox) ,"IsOverlapping(const_AxisAlignedBox&)_const");
        }

        {
            Plane OverlapPass1(Vector3(0.0,1.0,0.0),20);
            Plane OverlapPass2(Vector3(1.0,0.0,0.0),35);
            Plane OverlapFail1(Vector3(0.0,1.0,0.0),20);
            Plane OverlapFail2(Vector3(0.0,1.0,0.0),10);
            TEST( OverlapPass1.IsOverlapping(OverlapPass2) && !OverlapFail1.IsOverlapping(OverlapFail2) ,"IsOverlapping(const_Plane&)_const");
        }

        {
            Ray PassRay(Vector3(0.0,50.0,0.0),Vector3(0.0,-20.0,0.0));
            Ray FailRay(Vector3(0.0,10.0,50.0),Vector3(0.0,10.0,-50.0));
            Plane Surface(Vector3(0.0,1.0,0.0),20);
            Plane::RayTestResult PassResult = Surface.Intersects(PassRay);
            Plane::RayTestResult FailResult = Surface.Intersects(FailRay);
            TEST( ( PassResult.first && PassResult.second == Vector3(0.0,20.0,0.0) ) && ( !FailResult.first && FailResult.second == Vector3(0.0,0.0,0.0) ) ,"Intersects(const_Ray&)_const");
        }

        {
            Ogre::Plane Extract(Ogre::Vector3(-5.5,3.0,9.5).normalisedCopy(),18);
            Plane Surface;
            Surface.ExtractOgrePlane(Extract);
            TEST( Surface == Plane(Vector3(-5.5,3.0,9.5).Normalize(),18) ,"ExtractOgrePlane(const_Ogre::Plane&)");
        }

        {
            Plane Convert(Vector3(-5.5,3.0,9.5).Normalize(),18);
            TEST( Convert.GetOgrePlane() == Ogre::Plane(Ogre::Vector3(-5.5,3.0,9.5).normalisedCopy(),18) ,"GetOgrePlane()_const");
        }

        {
            String Expected( "<?xml version=\"1.0\"?><Plane Version=\"1\" Distance=\"-1.5\"><Normal><Vector3 Version=\"1\" X=\"0\" Y=\"0\" Z=\"1\" /></Normal></Plane>" );

            XML::Document Doc;
            Plane Surface(Vector3(0.0,0.0,1.0),1.5);
            Surface.ProtoSerialize(Doc);
            StringStream Buffer;
            Doc.Save(Buffer);

            TEST( Expected == String( Buffer.str() ) ,"ProtoSerialize(XML::Node&)_const");
        }

        {
            String Source( "<?xml version=\"1.0\"?><Plane Version=\"1\" Distance=\"-1.5\"><Normal><Vector3 Version=\"1\" X=\"0\" Y=\"0\" Z=\"1\" /></Normal></Plane>" );

            XML::Document Doc;
            StringStream Buffer;
            Buffer.str( Source );
            Doc.Load(Buffer);

            Plane Surface;
            Surface.ProtoDeSerialize(Doc.GetFirstChild());

            TEST( Surface == Plane(Vector3(0.0,0.0,1.0),1.5) ,"ProtoDeSerialize(const_XML::Node&)");
        }

        {
            String ClassName("Plane");
            TEST( ClassName == Plane::GetSerializableName() ,"GetSerializableName()");
        }

        {
            Vector3 Norm(-1.0,1.0,-1.0);
            Norm.Normalize();
            Real Const = 15;
            Plane Surface(Norm,Const);
            Plane Copy;
            Copy = Surface;
            TEST( Copy.Normal == Norm && Copy.Distance == -Const ,"operator=(const_Plane&)");
        }

        {
            Ogre::Vector3 Norm(-1.0,1.0,-1.0);
            Norm.normalise();
            Ogre::Real Const = 15;
            Ogre::Plane Surface(Norm,Const);
            Plane Copy;
            Copy = Surface;
            TEST( Copy.Normal == Vector3(Norm) && Copy.Distance == -Const ,"operator=(const_Ogre::Plane&)");
        }

        {
            Plane Surface(Vector3(0.0,1.0,0.0),25);
            Plane SurfacePass(Vector3(0.0,1.0,0.0),25);
            Plane SurfaceFail(Vector3(-1.0,0.0,0.0),40);
            TEST( ( Surface == SurfacePass ) && !( Surface == SurfaceFail ) ,"operator==(const_Plane&)_const");
        }

        {
            Plane Surface(Vector3(0.0,1.0,0.0),25);
            Plane SurfacePass(Vector3(-1.0,0.0,0.0),40);
            Plane SurfaceFail(Vector3(0.0,1.0,0.0),25);
            TEST( ( Surface != SurfacePass ) && !( Surface != SurfaceFail ) ,"operator!=(const_Plane&)_const");
        }
    }

    /// @brief Since RunAutomaticTests is implemented so is this.
    /// @return returns true
    virtual bool HasAutomaticTests() const
        { return true; }
};//planetests

#endif
