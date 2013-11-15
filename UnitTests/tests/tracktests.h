//© Copyright 2010 - 2013 BlackTopp Studios Inc.
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
#ifndef _tracktests_h
#define _tracketests_h

#include "mezztest.h"

#include "track.h"
#include "exception.h"


/// @file
/// @brief Test the track classes produce the expected interpolated values.

using namespace Mezzanine;
using namespace Mezzanine::Testing;

/// @brief A small series of sample tests, which can be used as a boilerplate so creating new test groups
class tracktests : public UnitTestGroup
{
    public:
        /// @copydoc Mezzanine::Testing::UnitTestGroup::Name
        /// @return Returns a String containing "BoilerPlate"
        virtual String Name()
            { return String("Track"); }

        /// @brief This is called when Automatic tests are run
        void RunAutomaticTests()
        {
            //TestOutput.precision(10);
            //TestOutput << std::fixed;

            {
                TrackLinear<Vector3> TestSinglePointTrack;
                TestSinglePointTrack.push_back(Vector3(0,0,0));
                TestOutput << "Testing interpolation on a Track with just one point, should throw and exception:" << endl;
                TEST_THROW(Mezzanine::ParametersRangeException,
                           TestSinglePointTrack.GetInterpolated(0.5),
                           "SingleTrackPointThrows");
                TEST_THROW(Mezzanine::ParametersRangeException,
                           TestSinglePointTrack.GetInterpolatedAsLoop(0.5),
                           "SingleTrackPointLoopThrows");
            }

            {
                TrackLinear<Vector3> TestDualPointTrack;
                TestDualPointTrack.push_back(Vector3(0,0,0));
                TestDualPointTrack.push_back(Vector3(10,10,10));
                TestOutput << endl << "Testing interpolation on a Track with just two points:" << endl
                           << "\t0.1 should be 1,1,1 and is " << TestDualPointTrack.GetInterpolated(0.1) << endl
                           << "\t0.5 should be 5,5,5 and is " << TestDualPointTrack.GetInterpolated(0.5) << endl
                           << "\t0.95 should be 9.5,9.9,9.5 and is " << TestDualPointTrack.GetInterpolated(0.95) << endl;
                TEST_EQUAL_EPSILON(TestDualPointTrack.GetInterpolated(0.1), Vector3(1.0,1.0,1.0), "DualPointTrack1");
                TEST_EQUAL_EPSILON(TestDualPointTrack.GetInterpolated(0.5), Vector3(5.0,5.0,5.0), "DualPointTrack2");
                TEST_EQUAL_EPSILON(TestDualPointTrack.GetInterpolated(0.95), Vector3(9.5,9.5,9.5), "DualPointTrack3");
                TestOutput << endl << "Testing line segment selection on previous track:" << endl
                           << "\t0.1 should be on segement 0 and is on segment " << TestDualPointTrack.GetLineSegmentFor(0.1) << endl
                           << "\t0.5 should be on segement 0 and is on segment " << TestDualPointTrack.GetLineSegmentFor(0.5) << endl
                           << "\t0.95 should be on segement 0 and is on segment " << TestDualPointTrack.GetLineSegmentFor(0.95) << endl;
                TEST(TestDualPointTrack.GetLineSegmentFor(0.1)==0, "DualPointTrackSegment1");
                TEST(TestDualPointTrack.GetLineSegmentFor(0.5)==0, "DualPointTrackSegment2");
                TEST(TestDualPointTrack.GetLineSegmentFor(0.95)==0, "DualPointTrackSegment3");
                TestOutput << endl << "Testing line segment location on previous track:" << endl
                           << "\t0.1 should be on segement 0 at .1 and is " << TestDualPointTrack.GetPercentageThroughSegment(0.1) << endl
                           << "\t0.5 should be on segement 0 at .5 and is " << TestDualPointTrack.GetPercentageThroughSegment(0.5) << endl
                           << "\t0.95 should be on segement 0 at .95 and is " << TestDualPointTrack.GetPercentageThroughSegment(0.95) << endl;
                TEST_EQUAL_EPSILON(TestDualPointTrack.GetPercentageThroughSegment(0.1), Real(0.1), "DualPointTrackSegmentLocation1");
                TEST_EQUAL_EPSILON(TestDualPointTrack.GetPercentageThroughSegment(0.5), Real(0.5), "DualPointTrackSegmentLocation2");
                TEST_EQUAL_EPSILON(TestDualPointTrack.GetPercentageThroughSegment(0.95), Real(0.95), "DualPointTrackSegmentLocation3");
                TestOutput << endl << "Testing interpolation on a Track with just two points as a loop:" << endl
                           << "\t0.0 should be 0,0,0 and is " << TestDualPointTrack.GetInterpolatedAsLoop(0.0) << endl
                           << "\t0.1 should be 2,2,2 and is " << TestDualPointTrack.GetInterpolatedAsLoop(0.1) << endl
                           << "\t0.5 should be 10,10,10 and is " << TestDualPointTrack.GetInterpolatedAsLoop(0.5) << endl
                           << "\t0.95 should be 1,1,1 and is " << TestDualPointTrack.GetInterpolatedAsLoop(0.95) << endl
                           << "\t1.0 should be 0,0,0 and is " << TestDualPointTrack.GetInterpolatedAsLoop(1.0) << endl;
                TEST_EQUAL_EPSILON(TestDualPointTrack.GetInterpolatedAsLoop(0.0), Vector3(0.0,0.0,0.0), "DualPointTrackLoop1");
                TEST_EQUAL_EPSILON(TestDualPointTrack.GetInterpolatedAsLoop(0.1), Vector3(2.0,2.0,2.0), "DualPointTrackLoop2");
                TEST_EQUAL_EPSILON(TestDualPointTrack.GetInterpolatedAsLoop(0.5), Vector3(10.0,10.0,10.0), "DualPointTrackLoop3");
                TEST_EQUAL_EPSILON(TestDualPointTrack.GetInterpolatedAsLoop(0.95), Vector3(1.0,1.0,1.0), "DualPointTrackLoop4");
                TEST_EQUAL_EPSILON(TestDualPointTrack.GetInterpolatedAsLoop(1.0), Vector3(0.0,0.0,0.0), "DualPointTrackLoop5");
            }

            {
                TrackLinear<Vector3> TestTriplePointTrack;
                TestTriplePointTrack.push_back(Vector3(0,0,0));
                TestTriplePointTrack.push_back(Vector3(0,0,10));
                TestTriplePointTrack.push_back(Vector3(0,10,10));
                TestOutput << endl << "Testing interpolation on a Track with three points:" << endl
                           << "\t0.1 should be 20% along the first line segment at 0,0,2 and is " << TestTriplePointTrack.GetInterpolated(0.1) << endl
                           << "\t0.4 should be 80% along the first line segment at 0,0,8 and is " << TestTriplePointTrack.GetInterpolated(0.4) << endl
                           << "\t0.5 should be the middle point at 0,0,10 and is " << TestTriplePointTrack.GetInterpolated(0.5) << endl
                           << "\t0.6 should be 20% along the second line segment at 0,2,10 and is " << TestTriplePointTrack.GetInterpolated(0.6) << endl
                           << "\t0.9 should be 80% along the second line segment at 0,8,10 and is " << TestTriplePointTrack.GetInterpolated(0.9) << endl
                           << "\t1.0 should be the second point at 0,10,10 and is " << TestTriplePointTrack.GetInterpolated(1.0) << endl;
                TEST_EQUAL_EPSILON(TestTriplePointTrack.GetInterpolated(0.1), Vector3(0.0,0.0,2.0), "TriplePointTrack1");
                TEST_EQUAL_EPSILON(TestTriplePointTrack.GetInterpolated(0.4), Vector3(0.0,0.0,8.0), "TriplePointTrack2");
                TEST_EQUAL_EPSILON(TestTriplePointTrack.GetInterpolated(0.5), Vector3(0.0,0.0,10.0), "TriplePointTrack3");
                TEST_EQUAL_MULTI_EPSILON(TestTriplePointTrack.GetInterpolated(0.6), Vector3(0.0,2.0,10.0), "TriplePointTrack4",4);
                TEST_EQUAL_MULTI_EPSILON(TestTriplePointTrack.GetInterpolated(0.9), Vector3(0.0,8.0,10.0), "TriplePointTrack5",4);
                TEST_EQUAL_EPSILON(TestTriplePointTrack.GetInterpolated(1.0), Vector3(0.0,10.0,10.0), "TriplePointTrack6");
                TestOutput << endl << "Testing line segment selection on previous track:" << endl
                           << "\t0.1 should be line segment 0 and is on " << TestTriplePointTrack.GetLineSegmentFor(0.1) << endl
                           << "\t0.4 should be line segment 0 and is on " << TestTriplePointTrack.GetLineSegmentFor(0.4) << endl
                           << "\t0.5 should be line segment 0 or 1 and is on " << TestTriplePointTrack.GetLineSegmentFor(0.5) << endl
                           << "\t0.6 should be line segment 1 and is on " << TestTriplePointTrack.GetLineSegmentFor(0.6) << endl
                           << "\t0.9 should be line segment 1 and is on " << TestTriplePointTrack.GetLineSegmentFor(0.9) << endl
                           << "\t1.0 should be line segment 1 or 2 and is on " << TestTriplePointTrack.GetLineSegmentFor(1.0) << endl;
                TEST(TestTriplePointTrack.GetLineSegmentFor(0.1)==0, "TriplePointTrackSegment1");
                TEST(TestTriplePointTrack.GetLineSegmentFor(0.4)==0, "TriplePointTrackSegment2");
                TEST(TestTriplePointTrack.GetLineSegmentFor(0.5)==0||TestTriplePointTrack.GetLineSegmentFor(0.5)==1, "TriplePointTrackSegment3");
                TEST(TestTriplePointTrack.GetLineSegmentFor(0.6)==1, "TriplePointTrackSegment4");
                TEST(TestTriplePointTrack.GetLineSegmentFor(0.9)==1, "TriplePointTrackSegment5");
                TEST(TestTriplePointTrack.GetLineSegmentFor(1.0)==1||TestTriplePointTrack.GetLineSegmentFor(1.0)==2, "TriplePointTrackSegment6");
                TestOutput << endl << "Testing line segment selection on previous track:" << endl
                           << "\t0.1 should be 0.2 on line segment 1 and is " << TestTriplePointTrack.GetPercentageThroughSegment(0.1) << endl
                           << "\t0.4 should be 0.8 on line segment 0 and is " << TestTriplePointTrack.GetPercentageThroughSegment(0.4) << endl
                           << "\t0.5 should be 1.0 or 0.0 line segment 0 or 1 and is " << TestTriplePointTrack.GetPercentageThroughSegment(0.5) << endl
                           << "\t0.6 should be .2 line segment 1 and is " << TestTriplePointTrack.GetPercentageThroughSegment(0.6) << endl
                           << "\t0.9 should be .8 line segment 1 and is " << TestTriplePointTrack.GetPercentageThroughSegment(0.9) << endl
                           << "\t1.0 should be 1.0 or 0.0 line segment 1 or 2 and is " << TestTriplePointTrack.GetPercentageThroughSegment(1.0) << endl;
                TEST_EQUAL_EPSILON(TestTriplePointTrack.GetPercentageThroughSegment(0.1), Real(0.2), "TriplePointTrackSegmentLocation1");
                TEST_EQUAL_EPSILON(TestTriplePointTrack.GetPercentageThroughSegment(0.4), Real(0.8), "TriplePointTrackSegmentLocation2");
                if(0==TestTriplePointTrack.GetLineSegmentFor(0.5)) // I really don't care if this is the first point on the next segment or the last point on this one
                {
                    TEST_EQUAL_EPSILON(TestTriplePointTrack.GetPercentageThroughSegment(0.5), Real(1.0), "TriplePointTrackSegmentLocation3");
                }else{
                    TEST_EQUAL_EPSILON(TestTriplePointTrack.GetPercentageThroughSegment(0.5), Real(0.0), "TriplePointTrackSegmentLocation3");
                }
                TEST_EQUAL_EPSILON(TestTriplePointTrack.GetPercentageThroughSegment(0.6), Real(0.2), "TriplePointTrackSegmentLocation4");
                TEST_EQUAL_EPSILON(TestTriplePointTrack.GetPercentageThroughSegment(0.9), Real(0.8), "TriplePointTrackSegmentLocation5");
                if(1==TestTriplePointTrack.GetLineSegmentFor(1.0))
                {
                    TEST_EQUAL_EPSILON(TestTriplePointTrack.GetPercentageThroughSegment(0.5), Real(1.0), "TriplePointTrackSegmentLocation6");
                }else{
                    TEST_EQUAL_EPSILON(TestTriplePointTrack.GetPercentageThroughSegment(0.5), Real(0.0), "TriplePointTrackSegmentLocation6");
                }
                TestOutput << endl << "Testing interpolation on a Track with three points as a loop:" << endl
                           << "\t0.0 should be 0,0,0 and is " << TestTriplePointTrack.GetInterpolatedAsLoop(0.0) << endl
                           << "\t1.0/3.0 should be 0,0,10, and is " << TestTriplePointTrack.GetInterpolatedAsLoop(1.0/3.0) << endl
                           << "\t2.0/3.0 should be 0,10,10, and is " << TestTriplePointTrack.GetInterpolatedAsLoop(2.0/3.0) << endl
                           << "\t1.0 should be 0,0,0 and is " << TestTriplePointTrack.GetInterpolatedAsLoop(1.0) << endl
                           << "\t5.0/6.0 should be 0,5,5, and is " << TestTriplePointTrack.GetInterpolatedAsLoop(5.0/6.0) << endl;
                TEST_EQUAL_EPSILON(TestTriplePointTrack.GetInterpolatedAsLoop(0.0), Vector3(0.0,0.0,0.0), "TriplePointTrackLoop1");
                TEST_EQUAL_EPSILON(TestTriplePointTrack.GetInterpolatedAsLoop(1.0/3.0), Vector3(0.0,0.0,10.0), "TriplePointTrackLoop2");
                TEST_EQUAL_EPSILON(TestTriplePointTrack.GetInterpolatedAsLoop(2.0/3.0), Vector3(0.0,10.0,10.0), "TriplePointTrackLoop3");
                TEST_EQUAL_EPSILON(TestTriplePointTrack.GetInterpolatedAsLoop(5.0/6.0), Vector3(0.0,5.0,5.0), "TriplePointTrackLoop4");
                TEST_EQUAL_EPSILON(TestTriplePointTrack.GetInterpolatedAsLoop(1.0), Vector3(0.0,0.0,0.0), "TriplePointTrackLoop5");

            }
            
            {
                TrackLinear<Vector3> TestQuadPointTrack;
                TestQuadPointTrack.push_back(Vector3(0,0,0));
                TestQuadPointTrack.push_back(Vector3(10,0,0));
                TestQuadPointTrack.push_back(Vector3(10,10,0));
                TestQuadPointTrack.push_back(Vector3(0,10,0));
                TestOutput << endl << "Testing interpolation on a Track with four points:" << endl
                           << "\t0.0 should be 0% along the first line segment at 0,0,0 and is " << TestQuadPointTrack.GetInterpolated(0.0) << endl
                           << "\t1.0/3.0 should be the second point, at 10,0,0, and is " << TestQuadPointTrack.GetInterpolated(1.0/3.0) << endl
                           << "\t2.0/3.0 should be the third point, at 10,10,0, and is " << TestQuadPointTrack.GetInterpolated(2.0/3.0) << endl
                           << "\t1.0 should be the fourth point, at 0,10,0, and is " << TestQuadPointTrack.GetInterpolated(1.0) << endl
                           << "\t0.5 should be the middle point at 10,5,0 and is " << TestQuadPointTrack.GetInterpolated(0.5) << endl;
                TEST_EQUAL_EPSILON(TestQuadPointTrack.GetInterpolated(0.0), Vector3(0.0,0.0,0.0), "QuadPointTrack1");
                TEST_EQUAL_EPSILON(TestQuadPointTrack.GetInterpolated(1.0/3.0), Vector3(10.0,0.0,0.0), "QuadPointTrack2");
                TEST_EQUAL_MULTI_EPSILON(TestQuadPointTrack.GetInterpolated(2.0/3.0), Vector3(10.0,10.0,0.0), "QuadPointTrack3",4);
                TEST_EQUAL_MULTI_EPSILON(TestQuadPointTrack.GetInterpolated(1.0), Vector3(0.0,10.0,0.0), "QuadPointTrack4",4);
                TEST_EQUAL_EPSILON(TestQuadPointTrack.GetInterpolated(0.5), Vector3(10.0,5.0,0.0), "QuadPointTrack5");
                TestOutput << endl << "Testing interpolation on a Track with four points as a loop:" << endl
                           << "\t0.0 should be 0,0,0 and is " << TestQuadPointTrack.GetInterpolatedAsLoop(0.0) << endl
                           << "\t0.5 should be 10,5,0, and is " << TestQuadPointTrack.GetInterpolatedAsLoop(0.5) << endl
                           << "\t1.0 should be 0,0,0 and is " << TestQuadPointTrack.GetInterpolatedAsLoop(1.0) << endl;
                TEST_EQUAL_EPSILON(TestQuadPointTrack.GetInterpolatedAsLoop(0.0), Vector3(0.0,0.0,0.0), "QuadPointTrackLoop1");
                TEST_EQUAL_EPSILON(TestQuadPointTrack.GetInterpolatedAsLoop(2.0/3.0), Vector3(10.0,5.0,0.0), "QuadPointTrackLoop2");
                TEST_EQUAL_EPSILON(TestQuadPointTrack.GetInterpolatedAsLoop(1.0), Vector3(0.0,0.0,0.0), "QuadPointTrackLoop3");
            }
            
        }

        /// @brief Since RunAutomaticTests is implemented so is this.
        /// @return returns true
        virtual bool HasAutomaticTests() const
            { return true; }
};

#endif

