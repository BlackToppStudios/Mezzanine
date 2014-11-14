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


// For performance comparison
namespace
{
    /// @typedef SegmentPosPair
    /// @brief This is an std::pair that stores the near and far segement points for ray-aabb intersections.
    typedef std::pair<Mezzanine::Real,Mezzanine::Real> SegmentPosPair;
    typedef std::pair<Mezzanine::Boole,Mezzanine::Ray> OldGeometryRayTestResult;

    /// @brief Helper fucntion for calculating ray-aabb intersections.
    /// @param Axis The axis to be calculated.
    /// @param Cast The ray being casted for the intersection.
    /// @param Box The AABB to check for intersection.
    /// @param PosPair A pair for the near and far distances in the check so far.
    /// @return Returns false if the check has succeeded in ruling out an intersection, true otherwise.
    Mezzanine::Boole OldCalculateAxis(const Mezzanine::Whole Axis, const Mezzanine::Ray& Cast, const Mezzanine::AxisAlignedBox& Box, SegmentPosPair& PosPair)
    {
        Mezzanine::Vector3 RayDir = Cast.GetNormal();
        Mezzanine::Real Denom = 1 / RayDir[Axis];
        Mezzanine::Real NewStart = ( Box.MinExt[Axis] - Cast.GetOrigin()[Axis] ) * Denom;
        Mezzanine::Real NewEnd = ( Box.MaxExt[Axis] - Cast.GetOrigin()[Axis] ) * Denom;

        if( NewStart > NewEnd )
            std::swap(NewStart,NewEnd);

        if( NewStart > PosPair.second || NewEnd < PosPair.first )
            return false;

        if( NewStart > PosPair.first )
            PosPair.first = NewStart;

        if( NewEnd < PosPair.second )
            PosPair.second = NewEnd;

        return true;
    }

    OldGeometryRayTestResult OldIntersects(const Mezzanine::AxisAlignedBox& Box, const Mezzanine::Ray& Cast)
    {
        // Code in this function is based on the equivalent in Ogre
        Mezzanine::Vector3 CastDir = Cast.GetNormal();
        Mezzanine::Vector3 AbsoluteDir = CastDir;
        AbsoluteDir.X = Mezzanine::MathTools::Abs( AbsoluteDir.X );
        AbsoluteDir.Y = Mezzanine::MathTools::Abs( AbsoluteDir.Y );
        AbsoluteDir.Z = Mezzanine::MathTools::Abs( AbsoluteDir.Z );

        // A small fixed sized constant time sorting algorithm for sorting the length of each axis.
        Mezzanine::Whole MaxAxis = 0, MidAxis = 1, MinAxis = 2;
        if( AbsoluteDir[0] < AbsoluteDir[2] ) {
            MaxAxis = 2;
            MinAxis = 1;
        }else if( AbsoluteDir[1] < AbsoluteDir[MinAxis] ) {
            MidAxis = MinAxis;
            MinAxis = 1;
        }else if( AbsoluteDir[1] > AbsoluteDir[MaxAxis] ) {
            MidAxis = MaxAxis;
            MaxAxis = 1;
        }

        SegmentPosPair Distances(0,std::numeric_limits<Mezzanine::Real>::infinity());

        OldCalculateAxis(MaxAxis,Cast,Box,Distances);
        if( AbsoluteDir[MidAxis] < std::numeric_limits<Mezzanine::Real>::epsilon() ) {
            if( Cast.GetOrigin()[MidAxis] < Box.MinExt[MidAxis] || Cast.GetOrigin()[MidAxis] > Box.MaxExt[MidAxis] ||
                Cast.GetOrigin()[MinAxis] < Box.MinExt[MinAxis] || Cast.GetOrigin()[MinAxis] > Box.MaxExt[MinAxis] )
            {
                return OldGeometryRayTestResult(false,Mezzanine::Ray());
            }
        }else{
            ::OldCalculateAxis(MidAxis,Cast,Box,Distances);
            if( AbsoluteDir[MinAxis] < std::numeric_limits<Mezzanine::Real>::epsilon() ) {
                if( Cast.GetOrigin()[MinAxis] < Box.MinExt[MinAxis] || Cast.GetOrigin()[MinAxis] > Box.MaxExt[MinAxis] ) {
                    return OldGeometryRayTestResult(false,Mezzanine::Ray());
                }
            }else{
                ::OldCalculateAxis(MinAxis,Cast,Box,Distances);
            }
        }

        Mezzanine::Ray Ret( Cast.GetOrigin() + (CastDir * Distances.first), Cast.GetOrigin() + (CastDir * Distances.second) );
        return OldGeometryRayTestResult(true,Ret);
    }

}

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

        { //Timing test for old collision algorithm
            TestOutput << std::endl << "Preparing to test old vs new Ray/AABB collision algorithm performance" << std::endl;
            Ray Cast(Vector3(-40.0,0.0,0.0), Vector3(1.0,0.0,0.0));
            AxisAlignedBox Box(Vector3(-5.0,-5.0,-5.0),Vector3(5.0,5.0,5.0));

            const Real StepSize = 0.0001;
            const Real XStart = -100;
            const Real XEnd = 100;
            const Whole StepCount =  (XEnd - XStart) / StepSize + 1; // Plus 1 to accomodate rounding errors

            MaxInt OldTime = 0;
            MaxInt NewTime = 0;

            TestOutput << "Starting OLD test, Taking Steps to walk a ray by change its normal by " << StepSize << " starting at " << XStart << " and going to " << XEnd << std::endl;
            {
                TimedTest OldTestTimer;
                std::vector<OldGeometryRayTestResult> OldResults;
                OldResults.reserve(StepCount);                  // Allocation is intentionally part of this test. Data structure size relates to cache misses and therefore performance
                for(Real Current = XStart; Current < XEnd; Current += StepSize)
                {
                    Cast.Normal.X = Current;
                    Cast.Normal.Normalize();
                    OldResults.push_back(OldIntersects(Box, Cast));
                }

                OldTime = OldTestTimer.GetLength();
                int ResultID = rand() % StepCount;
                TestOutput << "Finished OLD performance test in " << OldTime << " microseconds. Dumping a random entry to stymie optimizers: " << OldResults.at(ResultID).first << ", " << OldResults.at(ResultID).second << std::endl;
            }

            TestOutput << "Starting NEW test, Taking Steps to walk a ray by change its normal by " << StepSize << " starting at " << XStart << " and going to " << XEnd << std::endl;
            {
                TimedTest NewTestTimer;
                std::vector<Ray::GeometryRayTestResult> NewResults;
                NewResults.reserve(StepCount);                  // Allocation is intentionally part of this test. Data structure size relates to cache misses and therefore performance
                for(Real Current = XStart; Current < XEnd; Current += StepSize)
                {
                    Cast.Normal.X = Current;
                    Cast.Normal.Normalize();
                    NewResults.push_back(MathTools::Intersects(Box, Cast));
                }

                NewTime = NewTestTimer.GetLength();
                int ResultID = rand() % StepCount;
                TestOutput << "Finished NEW performance test in " << NewTime << " microseconds. Dumping a random entry to stymie optimizers: " << NewResults.at(ResultID).first << ", " << NewResults.at(ResultID).second << std::endl;
            }
            TestOutput << "The OLD RayGeometry result class is " << sizeof(OldGeometryRayTestResult) << " bytes in size." << std::endl;
            TestOutput << "The NEW RayGeometry result class is " << sizeof(Ray::GeometryRayTestResult) << " bytes in size." << std::endl;

            TEST_WARN( NewTime<OldTime, "Intersects(const_AxisAlignedBox&)_performance");

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
