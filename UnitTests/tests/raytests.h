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
#ifndef _raytests_h
#define _raytests_h

#include "mezztest.h"

#include "axisalignedbox.h"
#include "plane.h"
#include "ray.h"
#include "sphere.h"
#include "XML/xml.h"

#include <stdexcept> //only used to throw for TEST_THROW

#include <Ogre.h>

// Needed because windows sucks and uses macros improperly
// Sqeaky this crap might have been an intentionally introduced incompatibility issue aspart of an ms EEE
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
class raytests : public UnitTestGroup
{
public:
    /// @copydoc Mezzanine::Testing::UnitTestGroup::Name
    /// @return Returns a String containing "Ray"
    virtual String Name()
        { return String("Ray"); }

    /// @brief Test most features on the Ray.
    void RunAutomaticTests()
    {
        {
            Ray Blank;
            TEST( Blank.Origin == Vector3(0.0,0.0,0.0) && Blank == Vector3(0.0,1.0,0.0) ,"Ray()");
        }

        {
            Vector3 From(45.0,20.0,-15.0);
            Vector3 To(1.0,0.0,0.0);
            Ray Cast(From,To);
            Ray Copy(Cast);
            TEST( Copy.Origin == From && Copy.Normal == To ,"Ray(const_Ray&)");
        }

        {
            Vector3 To(0.0,-1.0,0.0);
            Ray Single(To);
            TEST( Single.Origin == Vector3(0.0,0.0,0.0) && Single.Normal == Vector3(0.0,-1.0,0.0) ,"Ray(const_Vector3&)");
        }

        {
            Vector3 From(45.0,20.0,-15.0);
            Vector3 To(0.0,0.0,1.0);
            Ray Cast(From,To);
            TEST( Cast.Origin == Vector3(45.0,20.0,-15.0) && Cast.Normal == Vector3(0.0,0.0,1.0) ,"Ray(const_Vector3&,_const_Vector3&)");
        }

        {
            Ogre::Vector3 From(-40.0,0.0,0.0);
            Ogre::Vector3 To(0.0,1.0,0.0);
            Ogre::Ray Cast(From,To);
            Ray Copy(Cast);
            TEST( Copy.Origin == Vector3(-40.0,0.0,0.0) && Copy.Normal == Vector3(0.0,1.0,0.0) ,"Ray(const_Ogre::Ray&)");
        }

        {
            Vector3 From(-40.0,0.0,0.0);
            Vector3 To(30.0,0.0,0.0);
            Ray Cast(From,To);
            TEST( Cast.GetNormal() == Vector3(1.0,0.0,0.0) ,"GetNormal()_const");
        }

        {
            Vector3 From(-40.0,0.0,0.0);
            Vector3 To(1.0,0.0,0.0);
            Ray Cast(From,To);
            Plane PassSurface(Vector3(-1.0,0.0,0.0),5);
            Plane FailSurface(Vector3(0.0,0.0,-1.0),10);
            Ray::PlaneRayTestResult PassResult = Cast.Intersects(PassSurface);
            Ray::PlaneRayTestResult FailResult = Cast.Intersects(FailSurface);
            TEST(PassResult.first && PassResult.second == Vector3(-5.0,0.0,0.0), "Intersects(const_Plane&)_const_positive");
            TEST(!FailResult.first && FailResult.second == Vector3(0.0,0.0,0.0),"Intersects(const_Plane&)_const_negative");
        }

        {
            Vector3 From(-40.0,0.0,0.0);
            Vector3 To(1.0,0.0,0.0);
            Ray Cast(From,To);
            Sphere PassBall(Vector3(20.0,0.0,0.0),10);
            Sphere FailBall(Vector3(-20.0,-30.0,0.0),10);
            Ray::GeometryRayTestResult PassResult = Cast.Intersects(PassBall);
            Ray::GeometryRayTestResult FailResult = Cast.Intersects(FailBall);
            TEST(PassResult.first && PassResult.second == Ray(Vector3(10.0,0.0,0.0),Vector3(30.0,0.0,0.0)), "Intersects(const_Sphere&)_const_positive");
            TEST(!FailResult.first && FailResult.second == Ray(Vector3(0.0,0.0,0.0),Vector3(0.0,1.0,0.0)), "Intersects(const_Sphere&)_const_negative");
        }

        {
            Vector3 From(-40.0,0.0,0.0);
            Vector3 To(1.0,0.0,0.0);
            Ray Cast(From,To);
            AxisAlignedBox PassBox(Vector3(-5.0,-5.0,-5.0),Vector3(5.0,5.0,5.0));
            AxisAlignedBox FailBox(Vector3(10.0,-30.0,5.0),Vector3(15.0,-20.0,15.0));
            Ray::GeometryRayTestResult PassResult = Cast.Intersects(PassBox);
            Ray::GeometryRayTestResult FailResult = Cast.Intersects(FailBox);
            TEST(PassResult.first && PassResult.second == Ray(Vector3(-5.0,0.0,0.0),Vector3(5.0,0.0,0.0)), "Intersects(const_AxisAlignedBox&)_const_positive");
            TEST(!FailResult.first && FailResult.second == Ray(Vector3(0.0,0.0,0.0),Vector3(0.0,1.0,0.0)), "Intersects(const_AxisAlignedBox&)_const_negative");
        }

        {
            Ogre::Vector3 From(-40.0,0.0,0.0);
            Ogre::Vector3 To(0.0,1.0,0.0);
            Ogre::Ray Cast(From,To);
            Ray Copy;
            Copy.ExtractOgreRay(Cast);
            TEST( Copy.Origin == Vector3(-40.0,0.0,0.0) && Copy.Normal == Vector3(0.0,1.0,0.0) ,"ExtractOgreRay(const_Ogre::Ray&)");
        }

        {
            Vector3 From(45.0,20.0,-15.0);
            Vector3 To(0.0,1.0,0.0);
            Ray Cast(From,To);
            Ogre::Ray Converted = Cast.GetOgreRay();
            TEST( Converted.getOrigin() == Ogre::Vector3(45.0,20.0,-15.0) && Converted.getDirection() == Cast.GetNormal().GetOgreVector3() ,"GetOgreRay()_const");
        }

        {
            String Expected( "<?xml version=\"1.0\"?><Ray Version=\"1\"><Origin><Vector3 Version=\"1\" X=\"45.5\" Y=\"20.5\" Z=\"-15.5\" /></Origin><Normal><Vector3 Version=\"1\" X=\"0\" Y=\"-1\" Z=\"0\" /></Normal></Ray>" );

            XML::Document Doc;
            Ray Cast(Vector3(45.5,20.5,-15.5),Vector3(0.0,-1.0,0.0));
            Cast.ProtoSerialize(Doc);
            StringStream Buffer;
            Doc.Save(Buffer);

            TEST( Expected == String( Buffer.str() ) ,"ProtoSerialize(XML::Node&)_const");
        }

        {
            String Source( "<?xml version=\"1.0\"?><Ray Version=\"1\"><Origin><Vector3 Version=\"1\" X=\"45.5\" Y=\"20.5\" Z=\"-15.5\" /></Origin><Normal><Vector3 Version=\"1\" X=\"0\" Y=\"-1\" Z=\"0\" /></Normal></Ray>" );

            XML::Document Doc;
            StringStream Buffer;
            Buffer.str( Source );
            Doc.Load(Buffer);

            Ray Cast;
            Cast.ProtoDeSerialize(Doc.GetFirstChild());

            TEST( Cast == Ray(Vector3(45.5,20.5,-15.5),Vector3(0.0,-1.0,0.0)) ,"ProtoDeSerialize(const_XML::Node&)");
        }

        {
            String ClassName("Ray");
            TEST( ClassName == Ray::GetSerializableName() ,"GetSerializableName()");
        }

        {
            Vector3 From(45.0,20.0,-15.0);
            Vector3 To(0.0,1.0,0.0);
            Ray Cast(From,To);
            Ray Copy;
            Copy = Cast;
            TEST( Copy.Origin == From && Copy.Normal == To ,"operator=(const_Ray&)");
        }

        {
            Ray Cast(Vector3(45.0,20.0,-15.0),Vector3(0.0,0.0,-1.0));
            Ray CastPass(Vector3(45.0,20.0,-15.0),Vector3(0.0,0.0,-1.0));
            Ray CastFail(Vector3(-40.0,0.0,0.0),Vector3(0.0,0.0,1.0));
            TEST( ( Cast == CastPass ) && !( Cast == CastFail ) ,"operator==(const_Ray&)_const");
        }

        {
            Ray Cast(Vector3(45.0,20.0,-15.0),Vector3(0.0,0.0,-1.0));
            Ray CastPass(Vector3(-40.0,0.0,0.0),Vector3(0.0,0.0,1.0));
            Ray CastFail(Vector3(45.0,20.0,-15.0),Vector3(0.0,0.0,-1.0));
            TEST( ( Cast != CastPass ) && !( Cast != CastFail ) ,"operator!=(const_Ray&)_const");
        }
    }

    /// @brief Since RunAutomaticTests is implemented so is this.
    /// @return returns true
    virtual bool HasAutomaticTests() const
        { return true; }
};//raytests

#endif
