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
#ifndef _tracktests_h
#define _tracktests_h

#include "mezztest.h"

#include "track.h"
#include "trackiterator.h"
#include "exception.h"
#include "vector3.h"


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
            TestOutput.precision(10);
            TestOutput << std::fixed << "Epsilon of the Vector3: " << numeric_limits<Vector3>::epsilon();

            {
                std::vector<Vector3> TestDataPoints;
                TestDataPoints.push_back(Vector3(0,0,0));
                Track<LinearInterpolator<Vector3> > TestSinglePointTrack(TestDataPoints);
                TestOutput << "Testing interpolation on a Track with just one point, should throw and exception (SingleTrackPointThrows):" << endl;
                TEST_THROW(Mezzanine::ParametersRangeException,
                           TestSinglePointTrack.GetInterpolated(0.5),
                           "SingleTrackPointThrows");
            }

            {
                std::vector<Vector3> TestDataPoints;
                TestDataPoints.push_back(Vector3(0,0,0));
                TestDataPoints.push_back(Vector3(10,10,10));

                Track<LinearInterpolator<Vector3> > TestDualPointTrack(TestDataPoints);

                TestOutput << endl << "Testing interpolation on a Track with just two points(DualPointTrack):" << endl
                           << "\t0.1 should be 1,1,1 and is " << TestDualPointTrack.GetInterpolated(0.1) << endl
                           << "\t0.5 should be 5,5,5 and is " << TestDualPointTrack.GetInterpolated(0.5) << endl
                           << "\t0.95 should be 9.5,9.9,9.5 and is " << TestDualPointTrack.GetInterpolated(0.95) << endl;
                TEST_EQUAL_EPSILON(TestDualPointTrack.GetInterpolated(0.1), Vector3(1.0,1.0,1.0), "DualPointTrack1");
                TEST_EQUAL_EPSILON(TestDualPointTrack.GetInterpolated(0.5), Vector3(5.0,5.0,5.0), "DualPointTrack2");
                TEST_EQUAL_EPSILON(TestDualPointTrack.GetInterpolated(0.95), Vector3(9.5,9.5,9.5), "DualPointTrack3");
            }

            {
                std::vector<Vector3> TestDataPoints;
                //TestDataPoints.push_back(Vector3(0,0,0));
                //TestDataPoints.push_back(Vector3(10,10,10));

                TrackLooped<LinearInterpolator<Vector3> > TestDualPointTrack(TestDataPoints);
                TestDualPointTrack.push_back(Vector3(0,0,0));
                TestDualPointTrack.push_back(Vector3(10,10,10));
                TestOutput << endl << "Testing interpolation on a Track with just two points as a loop(DualPointTrackLoop):" << endl
                           << "\t0.0 should be 0,0,0 and is " << TestDualPointTrack.GetInterpolated(0.0) << endl
                           << "\t0.1 should be 2,2,2 and is " << TestDualPointTrack.GetInterpolated(0.1) << endl
                           << "\t0.5 should be 10,10,10 and is " << TestDualPointTrack.GetInterpolated(0.5) << endl
                           << "\t0.95 should be 1,1,1 and is " << TestDualPointTrack.GetInterpolated(0.95) << endl
                           << "\t1.0 should be 0,0,0 and is " << TestDualPointTrack.GetInterpolated(1.0) << endl;
                TEST_EQUAL_MULTI_EPSILON(TestDualPointTrack.GetInterpolated(0.0), Vector3(0.0,0.0,0.0), "DualPointTrackLoop1",5);
                TEST_EQUAL_MULTI_EPSILON(TestDualPointTrack.GetInterpolated(0.1), Vector3(2.0,2.0,2.0), "DualPointTrackLoop2",5);
                TEST_EQUAL_MULTI_EPSILON(TestDualPointTrack.GetInterpolated(0.5), Vector3(10.0,10.0,10.0), "DualPointTrackLoop3",5);
                TEST_EQUAL_MULTI_EPSILON(TestDualPointTrack.GetInterpolated(0.95), Vector3(1.0,1.0,1.0), "DualPointTrackLoop4",5);
                TEST_EQUAL_MULTI_EPSILON(TestDualPointTrack.GetInterpolated(1.0), Vector3(0.0,0.0,0.0), "DualPointTrackLoop5",5);
            }

            {
                Track<LinearInterpolator<Vector3> > TestTriplePointTrack;
                TestTriplePointTrack.push_back(Vector3(0,0,0));
                TestTriplePointTrack.push_back(Vector3(0,0,10));
                TestTriplePointTrack.push_back(Vector3(0,10,10));
                TestOutput << endl << "Testing interpolation on a Track with three points(TriplePointTrack):" << endl
                           << "\t0.1 should be 20% along the first line segment at 0,0,2 and is " << TestTriplePointTrack.GetInterpolated(0.1) << endl
                           << "\t0.4 should be 80% along the first line segment at 0,0,8 and is " << TestTriplePointTrack.GetInterpolated(0.4) << endl
                           << "\t0.5 should be the middle point at 0,0,10 and is " << TestTriplePointTrack.GetInterpolated(0.5) << endl
                           << "\t0.6 should be 20% along the second line segment at 0,2,10 and is " << TestTriplePointTrack.GetInterpolated(0.6) << endl
                           << "\t0.9 should be 80% along the second line segment at 0,8,10 and is " << TestTriplePointTrack.GetInterpolated(0.9) << endl
                           << "\t1.0 should be the second point at 0,10,10 and is " << TestTriplePointTrack.GetInterpolated(1.0) << endl;
                TEST_EQUAL_EPSILON(TestTriplePointTrack.GetInterpolated(0.1), Vector3(0.0,0.0,2.0), "TriplePointTrack1");
                TEST_EQUAL_EPSILON(TestTriplePointTrack.GetInterpolated(0.4), Vector3(0.0,0.0,8.0), "TriplePointTrack2");
                TEST_EQUAL_EPSILON(TestTriplePointTrack.GetInterpolated(0.5), Vector3(0.0,0.0,10.0), "TriplePointTrack3");
                TEST_EQUAL_MULTI_EPSILON(TestTriplePointTrack.GetInterpolated(0.6), Vector3(0.0,2.0,10.0), "TriplePointTrack4",5);
                TEST_EQUAL_MULTI_EPSILON(TestTriplePointTrack.GetInterpolated(0.9), Vector3(0.0,8.0,10.0), "TriplePointTrack5",5);
                TEST_EQUAL_EPSILON(TestTriplePointTrack.GetInterpolated(1.0), Vector3(0.0,10.0,10.0), "TriplePointTrack6");
            }

            {
                TrackLooped<LinearInterpolator<Vector3> > TestTriplePointTrack;
                TestTriplePointTrack.push_back(Vector3(0,0,0));
                TestTriplePointTrack.push_back(Vector3(0,0,10));
                TestTriplePointTrack.push_back(Vector3(0,10,10));
                TestOutput << endl << "Testing interpolation on a Track with three points as a loop(TriplePointTrackLoop):" << endl
                           << "\t0.0 should be 0,0,0 and is " << TestTriplePointTrack.GetInterpolated(0.0) << endl
                           << "\t1.0/3.0 should be 0,0,10, and is " << TestTriplePointTrack.GetInterpolated(1.0/3.0) << endl
                           << "\t2.0/3.0 should be 0,10,10, and is " << TestTriplePointTrack.GetInterpolated(2.0/3.0) << endl
                           << "\t5.0/6.0 should be 0,5,5, and is " << TestTriplePointTrack.GetInterpolated(5.0/6.0) << endl
                           << "\t1.0 should be 0,0,0 and is " << TestTriplePointTrack.GetInterpolated(1.0) << endl;
                TEST_EQUAL_MULTI_EPSILON(TestTriplePointTrack.GetInterpolated(0.0), Vector3(0.0,0.0,0.0), "TriplePointTrackLoop1",5);
                TEST_EQUAL_MULTI_EPSILON(TestTriplePointTrack.GetInterpolated(1.0/3.0), Vector3(0.0,0.0,10.0), "TriplePointTrackLoop2",5);
                TEST_EQUAL_MULTI_EPSILON(TestTriplePointTrack.GetInterpolated(2.0/3.0), Vector3(0.0,10.0,10.0), "TriplePointTrackLoop3",5);
                TEST_EQUAL_MULTI_EPSILON(TestTriplePointTrack.GetInterpolated(5.0/6.0), Vector3(0.0,5.0,5.0), "TriplePointTrackLoop4",5);
                TEST_EQUAL_MULTI_EPSILON(TestTriplePointTrack.GetInterpolated(1.0), Vector3(0.0,0.0,0.0), "TriplePointTrackLoop5",5);

            }

            {
                Track<LinearInterpolator<Vector3> > TestQuadPointTrack;
                TestQuadPointTrack.push_back(Vector3(0,0,0));
                TestQuadPointTrack.push_back(Vector3(10,0,0));
                TestQuadPointTrack.push_back(Vector3(10,10,0));
                TestQuadPointTrack.push_back(Vector3(0,10,0));
                TestOutput << endl << "Testing interpolation on a Track with four points(QuadPointTrack):" << endl
                           << "\t0.0 should be 0% along the first line segment at 0,0,0 and is " << TestQuadPointTrack.GetInterpolated(0.0) << endl
                           << "\t1.0/3.0 should be the second point, at 10,0,0, and is " << TestQuadPointTrack.GetInterpolated(1.0/3.0) << endl
                           << "\t2.0/3.0 should be the third point, at 10,10,0, and is " << TestQuadPointTrack.GetInterpolated(2.0/3.0) << endl
                           << "\t1.0 should be the fourth point, at 0,10,0, and is " << TestQuadPointTrack.GetInterpolated(1.0) << endl
                           << "\t0.5 should be the middle point at 10,5,0 and is " << TestQuadPointTrack.GetInterpolated(0.5) << endl;
                TEST_EQUAL_MULTI_EPSILON(TestQuadPointTrack.GetInterpolated(0.0), Vector3(0.0,0.0,0.0), "QuadPointTrack1",5);
                TEST_EQUAL_MULTI_EPSILON(TestQuadPointTrack.GetInterpolated(1.0/3.0), Vector3(10.0,0.0,0.0), "QuadPointTrack2",5);
                TEST_EQUAL_MULTI_EPSILON(TestQuadPointTrack.GetInterpolated(2.0/3.0), Vector3(10.0,10.0,0.0), "QuadPointTrack3",5);
                TEST_EQUAL_MULTI_EPSILON(TestQuadPointTrack.GetInterpolated(1.0), Vector3(0.0,10.0,0.0), "QuadPointTrack4",5);
                TEST_EQUAL_MULTI_EPSILON(TestQuadPointTrack.GetInterpolated(0.5), Vector3(10.0,5.0,0.0), "QuadPointTrack5",5);

                TestOutput << endl << "Testing interpolation on a track with four points for performance:" << endl;
                {
                    SCOPEDTIMER(TestOutput);
                    Vector3 DefeatCompilerOptimization(0,0,0);
                    for(Whole Counter = 0; Counter<1000000; Counter++)
                    {
                        DefeatCompilerOptimization = TestQuadPointTrack.GetInterpolated(rand()/RAND_MAX);
                    }
                    TestOutput << "A random point on the track to defeat optimizations: " << DefeatCompilerOptimization << endl << endl;
                }
            }

            {
                TrackLooped<LinearInterpolator<Vector3> > TestQuadPointTrack;
                TestQuadPointTrack.push_back(Vector3(0,0,0));
                TestQuadPointTrack.push_back(Vector3(10,0,0));
                TestQuadPointTrack.push_back(Vector3(10,10,0));
                TestQuadPointTrack.push_back(Vector3(0,10,0));
                TestOutput << endl << "Testing interpolation on a Track with a loop with four points as a loop(QuadPointTrackLoop):" << endl
                           << "\t0.0 should be 0,0,0 and is " << TestQuadPointTrack.GetInterpolated(0.0) << endl
                           << "\t0.5 should be 10,10,0, and is " << TestQuadPointTrack.GetInterpolated(0.5) << endl
                           << "\t1.0 should be 0,0,0 and is " << TestQuadPointTrack.GetInterpolated(1.0) << endl;
                TEST_EQUAL_EPSILON(TestQuadPointTrack.GetInterpolated(0.0), Vector3(0.0,0.0,0.0), "QuadPointTrackLoop1");
                TEST_EQUAL_EPSILON(TestQuadPointTrack.GetInterpolated(0.5), Vector3(10.0,10.0,0.0), "QuadPointTrackLoop2");
                TEST_EQUAL_EPSILON(TestQuadPointTrack.GetInterpolated(1.0), Vector3(0.0,0.0,0.0), "QuadPointTrackLoop3");

                TestOutput << endl << "Testing interpolation on a track with four points for performance:" << endl;
                {
                    SCOPEDTIMER(TestOutput);
                    Vector3 DefeatCompilerOptimization(0,0,0);
                    for(Whole Counter = 0; Counter<1000000; Counter++)
                    {
                        DefeatCompilerOptimization = TestQuadPointTrack.GetInterpolated(rand()/RAND_MAX);
                    }
                    TestOutput << "A random point on the track to defeat optimizations: " << DefeatCompilerOptimization << endl << endl;
                }
            }

            {
                Track<LinearInterpolator<Vector3> > TestIteratorTrack;

                TestIteratorTrack.push_back(Vector3(0,0,0));
                TestIteratorTrack.push_back(Vector3(10,0,0));
                TestIteratorTrack.push_back(Vector3(10,10,0));
                TestIteratorTrack.push_back(Vector3(0,10,0));

                SmoothTrackIterator<LinearInterpolator<Vector3> > Iter(&TestIteratorTrack,0.0,1.0/30.0);

                TestOutput << "Performing some basic tests to see if SmoothTrackIterators basib functionality work at all." << endl
                           << "A dereferenced X component from an iterator refering to 0.0 on a Linear track (expecting 0.0): " << Iter->X << endl
                           << "A dereferenced Vector3 from an iterator (expecting \"<Vector3 Version=\"1\" X=\"0\" Y=\"0\" Z=\"0\" />\")" << *Iter << endl << endl;
                TEST_EQUAL_EPSILON(Iter->X, Real(0.0), "SmoothTrackIterator->1");
                TEST_EQUAL_EPSILON(*Iter, Vector3(0,0,0), "SmoothTrackIterator*1");


                TestOutput << "Incrementing the iterator via postfix++, this should move it 1/30th of the track: " << endl;
                Iter++;
                TestOutput << "A Dereferenced X component from an iterator refering to 1/30 on a Linear track (expecting 1.0): " << Iter->X << endl
                           << "A dereferenced Vector3 from an iterator (expecting \"<Vector3 Version=\"1\" X=\"1.0\" Y=\"0\" Z=\"0\" />\")" << *Iter << endl << endl;
                TEST_EQUAL_EPSILON(Iter->X, Real(1.0), "SmoothTrackIterator->2");
                TEST_EQUAL_EPSILON(*Iter, Vector3(1.0,0.0,0.0), "SmoothTrackIterator*2");

                TestOutput << "Incrementing the iterator via prefix:" << endl;
                ++Iter;
                TestOutput << "A Dereferenced X component from an iterator refering to 2/30 (using ++) on a Linear track: " << Iter->X << endl
                           << "A dereferenced Vector3 from an iterator " << *Iter << endl;
                TEST_EQUAL_MULTI_EPSILON(Iter->X, Real(2.0), "SmoothTrackIterator->++",5);
                TEST_EQUAL_MULTI_EPSILON(*Iter, Vector3(2.0,0.0,0.0), "SmoothTrackIterator*++",5);

                TestOutput << "Decrementing the iterator via prefix:" << endl;
                --Iter;
                TestOutput << "A Dereferenced X component from an iterator refering to 1/30 (using --)on a Linear track: " << Iter->X << endl
                           << "A dereferenced Vector3 from an iterator " << *Iter << endl;
                TEST_EQUAL_MULTI_EPSILON(Iter->X, Real(1.0), "SmoothTrackIterator->--",5);
                TEST_EQUAL_MULTI_EPSILON(*Iter, Vector3(1.0,0.0,0.0), "SmoothTrackIterator*--",5);

                Iter = Iter + 3;
                TestOutput << "A Dereferenced X component from an iterator refering to 1/30 (using +3)on a Linear track: " << *Iter << endl;
                TEST_EQUAL_MULTI_EPSILON(*Iter, Vector3(4.0,0.0,0.0), "SmoothTrackIterator+",5);
                Iter = Iter - 3;
                TestOutput << "A Dereferenced X component from an iterator refering to 1/30 (using -3)on a Linear track: " << *Iter << endl;
                TEST_EQUAL_MULTI_EPSILON(*Iter, Vector3(1.0,0.0,0.0), "SmoothTrackIterator-",5);

                Iter += 3;
                TestOutput << "A Dereferenced X component from an iterator refering to 1/30 (using +=3)on a Linear track: " << *Iter << endl;
                TEST_EQUAL_MULTI_EPSILON(*Iter, Vector3(4.0,0.0,0.0), "SmoothTrackIterator+=",5);
                Iter -= 3;
                TestOutput << "A Dereferenced X component from an iterator refering to 1/30 (using -=3)on a Linear track: " << *Iter << endl;
                TEST_EQUAL_MULTI_EPSILON(*Iter, Vector3(1.0,0.0,0.0), "SmoothTrackIterator-=",5);

                SmoothTrackIterator<LinearInterpolator<Vector3> > Iter2(Iter);
                TEST_EQUAL_EPSILON(*Iter, *Iter2, "SmoothTrackIterator=");
                TEST(Iter==Iter2, "SmoothTrackIterator==");
                TEST(!(Iter!=Iter2), "SmoothTrackIterator!=");


                TestIteratorTrack.push_back(Vector3(0,0,0)); // need to do this better.
                TestOutput << "200 steps with a looped iterator on at a 100 step resolution on a four point track:" << endl;

                TestOutput << "Validating iterator bounds checking:" << endl;
                TEST(SmoothTrackIterator<LinearInterpolator<Vector3> >(&TestIteratorTrack,0.0,1.0/100.0).BoundsCheck()==0, "SmoothTrackIterator::BoundsCheckIn");
                TEST(SmoothTrackIterator<LinearInterpolator<Vector3> >(&TestIteratorTrack,-0.1,1.0/100.0).BoundsCheck()==-1, "SmoothTrackIterator::BoundsCheckLow");
                TEST(SmoothTrackIterator<LinearInterpolator<Vector3> >(&TestIteratorTrack,1.1,1.0/100.0).BoundsCheck()==1, "SmoothTrackIterator::BoundsCheckHigh");

                // Some sample loops for academic purposes
                // Idiomatic loops
                TestOutput << "Sample track iteration with custom end and nearly idiomatic for loop:" << endl;
                for(SmoothTrackIterator<LinearInterpolator<Vector3> > LoopedIter(&TestIteratorTrack,0.0,1.0/10.0);
                    LoopedIter!=SmoothTrackIterator<LinearInterpolator<Vector3> > (&TestIteratorTrack,1.0,1.0/10.0);
                    LoopedIter++)
                    { TestOutput << *LoopedIter << endl; }

                TestOutput << "Sample track iteration with idiomatic for loop:" << endl;
                for(SmoothTrackIterator<LinearInterpolator<Vector3> > LoopedIter(TestIteratorTrack.begin(10));
                    LoopedIter!=TestIteratorTrack.end();
                    LoopedIter++)
                    { TestOutput << *LoopedIter << endl; }

                // Just by executing this line we can tell that these work to some degree.
                TEST(true, "SmoothTrackIterator::begin1");
                TEST(true, "SmoothTrackIterator::end1");
                TEST((TestIteratorTrack.begin(10)==SmoothTrackIterator<LinearInterpolator<Vector3> >(&TestIteratorTrack,0.0,1.0/10.0) ), "SmoothTrackIterator::begin2");
                TEST((TestIteratorTrack.end()==SmoothTrackIterator<LinearInterpolator<Vector3> >(&TestIteratorTrack,1.0,0.0)), "SmoothTrackIterator::end2");

                // Almost Idiomatic loop
                TestOutput << "Sample track iteration with for loop:" << endl;
                for(SmoothTrackIterator<LinearInterpolator<Vector3> > LoopedIter(&TestIteratorTrack,0.0,1.0/10.0);
                    0==LoopedIter.BoundsCheck();
                    LoopedIter++)
                    { TestOutput << *LoopedIter << endl; }

                // non-idiomatic way to iterate over a track
                SmoothTrackIterator<LinearInterpolator<Vector3> > LoopedIter(&TestIteratorTrack,0.0,1.0/10.0);
                for(Whole Counter=0; Counter<10; Counter++, LoopedIter++)
                    { TestOutput << *LoopedIter << endl; }

                TestOutput << "Validating iterator comparison operators." << endl;
                SmoothTrackIterator<LinearInterpolator<Vector3> > HighIter(&TestIteratorTrack,.8,1.0/100.0);
                SmoothTrackIterator<LinearInterpolator<Vector3> > MidIter(&TestIteratorTrack,.5,1.0/100.0);
                SmoothTrackIterator<LinearInterpolator<Vector3> > LowIter(&TestIteratorTrack,.2,1.0/100.0);
                TEST((HighIter < LowIter) == false, "SmoothTrackIterator<1");
                TEST((LowIter < HighIter) == true, "SmoothTrackIterator<2");
                TEST((MidIter < MidIter) == false, "SmoothTrackIterator<3");
                TEST((HighIter > LowIter) == true, "SmoothTrackIterator>1");
                TEST((LowIter > HighIter) == false, "SmoothTrackIterator>2");
                TEST((MidIter > MidIter) == false, "SmoothTrackIterator>3");
                TEST((HighIter <= LowIter) == false, "SmoothTrackIterator<=1");
                TEST((LowIter <= HighIter) == true, "SmoothTrackIterator<=2");
                TEST((MidIter <= MidIter) == true, "SmoothTrackIterator<=3");
                TEST((HighIter >= LowIter) == true, "SmoothTrackIterator>=1");
                TEST((LowIter >= HighIter) == false, "SmoothTrackIterator>=2");
                TEST((MidIter >= MidIter) == true, "SmoothTrackIterator>=3");

                TestOutput << "Validating iterator indexing [] operator." << endl;
                SmoothTrackIterator<LinearInterpolator<Vector3> > IndexIter1(&TestIteratorTrack,.8,1.0/100.0);
                SmoothTrackIterator<LinearInterpolator<Vector3> > IndexIter2(&TestIteratorTrack,.9,1.0/100.0);
                TEST(IndexIter1[10] == IndexIter2, "SmoothTrackIterator[]");
            }

            {
                //SlowSplineInterpolator<Vector2> CSV;

                //Track<SlowSplineInterpolator<Vector2> > CubicSplineTrack;
                //CubicSplineTrack.push_back(Vector2(0,0));
                //TestIteratorTrack.push_back(Vector2(10,10));
                //TestIteratorTrack.push_back(Vector2(20,0));
            }

            {

            }




        }

        /// @brief Since RunAutomaticTests is implemented so is this.
        /// @return returns true
        virtual bool HasAutomaticTests() const
            { return true; }
};

#endif

