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
#ifndef _interpolatortests_h
#define _interpolatortests_h

#include "mezztest.h"

#include "vector3.h"
#include "vector2.h"
#include "transform.h"
#include "interpolator.h"

/// @file
/// @brief Test interpolation functions not specific to a single interpolatable class

using namespace Mezzanine;
using namespace Mezzanine::Testing;

/// @brief A small series of sample tests, which can be used as a boilerplate so creating new test groups
class interpolatortests : public UnitTestGroup
{
    public:
        /// @copydoc Mezzanine::Testing::UnitTestGroup::Name
        /// @return Returns a String containing "BoilerPlate"
        virtual String Name()
            { return String("Interpolator"); }

        /// @brief This is called when Automatic tests are run
        void RunAutomaticTests()
        {
            TestOutput.precision(10);
            TestOutput << std::fixed;

            {
                LinearInterpolator<Vector3> li;
                Vector3 a(0.0,0.0,0.0);
                Vector3 b(1.0,1.0,1.0);
                std::vector<Vector3> Points;
                Points.push_back(a);
                Points.push_back(b);

                TestOutput << "Lets try some simple interpolations between 0,0,0 and 1,1,1 at 0,.1,.5, and 1:" << endl
                           << li.Interpolate(Points.begin(),Points.end(),0.0) << endl
                           << li.Interpolate(Points.begin(),Points.end(),0.1) << endl
                           << li.Interpolate(Points.begin(),Points.end(),0.5) << endl
                           << li.Interpolate(Points.begin(),Points.end(),1.0) << endl << endl;

                TEST_EQUAL_EPSILON(li.Interpolate(Points.begin(),Points.end(),0.0),a,"Vector3LinearInterpolationBegin");
                TEST_EQUAL_EPSILON(li.Interpolate(Points.begin(),Points.end(),0.1),Vector3(0.1,0.1,0.1),"Vector3LinearInterpolationTenth");
                TEST_EQUAL_EPSILON(li.Interpolate(Points.begin(),Points.end(),0.5),Vector3(0.5,0.5,0.5),"Vector3LinearInterpolationHalfway");
                TEST_EQUAL_EPSILON(li.Interpolate(Points.begin(),Points.end(),1.0),b,"Vector3LinearInterpolationEnd");
             }

            {
                LinearInterpolator<Integer> li;
                Integer a = 0;
                Integer b = 10;
                std::vector<Integer> DataPoints;
                DataPoints.push_back(a);
                DataPoints.push_back(b);

                TestOutput << "Lets try some simple Integer interpolations between 0 and 10 at 0,.1,.5, and 1:" << endl
                           << li.Interpolate(DataPoints.begin(),DataPoints.end(),0.0) << endl
                           << li.Interpolate(DataPoints.begin(),DataPoints.end(),0.1) << endl
                           << li.Interpolate(DataPoints.begin(),DataPoints.end(),0.5) << endl
                           << li.Interpolate(DataPoints.begin(),DataPoints.end(),1.0) << endl << endl;

                TEST_EQUAL_EPSILON(li.Interpolate(DataPoints.begin(),DataPoints.end(),0.0),a,"IntegerLinearInterpolationBegin");
                TEST_EQUAL_EPSILON(li.Interpolate(DataPoints.begin(),DataPoints.end(),0.1),1,"IntegerLinearInterpolationTenth");
                TEST_EQUAL_EPSILON(li.Interpolate(DataPoints.begin(),DataPoints.end(),0.5),5,"IntegerLinearInterpolationHalfway");
                TEST_EQUAL_EPSILON(li.Interpolate(DataPoints.begin(),DataPoints.end(),1.0),b,"IntegerLinearInterpolationEnd");
             }

            {
                LinearInterpolator<Real> li;
                Real a = 0;
                Real b = 5;
                Real c = 10;
                Real d = 20;

                std::vector<Real> DataPoints;
                DataPoints.push_back(a);
                DataPoints.push_back(b);

                TestOutput << "Lets try some simple Floating point interpolations between 0 and 5 at the locations 0,.1,.5, and 1:" << endl
                           << li.Interpolate(DataPoints.begin(),DataPoints.end(),0.0) << endl
                           << li.Interpolate(DataPoints.begin(),DataPoints.end(),0.1) << endl
                           << li.Interpolate(DataPoints.begin(),DataPoints.end(),0.5) << endl
                           << li.Interpolate(DataPoints.begin(),DataPoints.end(),1.0) << endl
                           << endl;
                TEST_EQUAL_EPSILON(li.Interpolate(DataPoints.begin(),DataPoints.end(),0.0),a,"RealLinearInterpolationBegin");
                TEST_EQUAL_EPSILON(li.Interpolate(DataPoints.begin(),DataPoints.end(),0.1),Real(0.5),"RealLinearInterpolationTenth");
                TEST_EQUAL_EPSILON(li.Interpolate(DataPoints.begin(),DataPoints.end(),0.5),Real(2.5),"RealLinearInterpolationHalfway");
                TEST_EQUAL_EPSILON(li.Interpolate(DataPoints.begin(),DataPoints.end(),1.0),b,"RealLinearInterpolationEnd");

                DataPoints.push_back(c);

                Whole DataPointCount = std::distance(DataPoints.begin(),DataPoints.end());
                Whole LineSegmentCount = DataPointCount-1;
                TestOutput << "Lets try some 3 point Floating point interpolations between 0,5 and 10 at the locations 0,.1,.5, and 1:" << endl;
                TestOutput << " 0 @ 0.0 = " << li.Interpolate(DataPoints.begin(),DataPoints.end(),0.0)
                           << "\tLineSegment: " << Whole((0.0) * Real(LineSegmentCount)) << " LocalPercentage: " << std::fmod(PreciseReal(0.0),PreciseReal(1.0/PreciseReal(LineSegmentCount)))*LineSegmentCount << endl;
                TestOutput << " 1 @ 0.1 = " << li.Interpolate(DataPoints.begin(),DataPoints.end(),0.1)
                           << "\tLineSegment: " << Whole((0.1) * Real(LineSegmentCount)) << " LocalPercentage: " << std::fmod(PreciseReal(0.1),PreciseReal(1.0/PreciseReal(LineSegmentCount)))*LineSegmentCount << endl;
                TestOutput << " 5 @ 0.5 = " << li.Interpolate(DataPoints.begin(),DataPoints.end(),0.5)
                           << "\tLineSegment: " << Whole((0.5) * Real(LineSegmentCount)) << " LocalPercentage: " << std::fmod(PreciseReal(0.5),PreciseReal(1.0/PreciseReal(LineSegmentCount)))*LineSegmentCount << endl;
                TestOutput << "10 @ 1.0 = " << li.Interpolate(DataPoints.begin(),DataPoints.end(),1.0)
                           << "\tLineSegment: " << Whole((1.0) * Real(LineSegmentCount)) << " LocalPercentage: " << std::fmod(PreciseReal(1.0),PreciseReal(1.0/PreciseReal(LineSegmentCount)))*LineSegmentCount << endl;
                TestOutput << endl;
                TEST_EQUAL_EPSILON(li.Interpolate(DataPoints.begin(),DataPoints.end(),0.0),a,"Real3PointLinearInterpolationBegin");
                TEST_EQUAL_EPSILON(li.Interpolate(DataPoints.begin(),DataPoints.end(),0.1),Real(1.0),"Real3PointLinearInterpolationTenth");
                TEST_EQUAL_EPSILON(li.Interpolate(DataPoints.begin(),DataPoints.end(),0.5),b,"Real3PointLinearInterpolationHalfway");
                TEST_EQUAL_EPSILON(li.Interpolate(DataPoints.begin(),DataPoints.end(),1.0),c,"Real3PointLinearInterpolationEnd");

                DataPoints.push_back(d);
                TestOutput << "Lets try some 4 point Floating point interpolations between 0, 5, 10 and 20 at the locations 0, .1, .3333333333, .5, .6666666666, .8 and 1:" << endl;
                TestOutput << " 0.*  @ 0.0  = " << li.Interpolate(DataPoints.begin(),DataPoints.end(),0.0)
                           << "\tLineSegment: " << Whole((0.0) * Real(LineSegmentCount)) << " LocalPercentage: " << std::fmod(PreciseReal(0.0),PreciseReal(1.0/PreciseReal(LineSegmentCount)))*LineSegmentCount << endl;
                TestOutput << " 1.5  @ 0.1  = " << li.Interpolate(DataPoints.begin(),DataPoints.end(),0.1)
                           << "\tLineSegment: " << Whole((0.1) * Real(LineSegmentCount)) << " LocalPercentage: " << std::fmod(PreciseReal(0.1),PreciseReal(1.0/PreciseReal(LineSegmentCount)))*LineSegmentCount << endl;
                TestOutput << " 5    @ 0.33 = " << li.Interpolate(DataPoints.begin(),DataPoints.end(),0.333333333)
                           << "\tLineSegment: " << Whole((0.3333333333) * Real(LineSegmentCount)) << " LocalPercentage: " << std::fmod(PreciseReal(0.3333333333),PreciseReal(1.0/PreciseReal(LineSegmentCount)))*LineSegmentCount << endl;
                TestOutput << " 7.5  @ 0.5  = " << li.Interpolate(DataPoints.begin(),DataPoints.end(),0.5)
                           << "\tLineSegment: " << Whole((0.5) * Real(LineSegmentCount)) << " LocalPercentage: " << std::fmod(PreciseReal(0.5),PreciseReal(1.0/PreciseReal(LineSegmentCount)))*LineSegmentCount << endl;
                TestOutput << "10.0  @ 0.66 = " << li.Interpolate(DataPoints.begin(),DataPoints.end(),0.6666666666)
                           << "\tLineSegment: " << Whole((0.6666666666) * Real(LineSegmentCount)) << " LocalPercentage: " << std::fmod(PreciseReal(0.6666666666),PreciseReal(1.0/PreciseReal(LineSegmentCount)))*LineSegmentCount << endl;
                TestOutput << "14.0  @ 0.8  = " << li.Interpolate(DataPoints.begin(),DataPoints.end(),0.8)
                           << "\tLineSegment: " << Whole((0.8) * Real(LineSegmentCount)) << " LocalPercentage: " << std::fmod(PreciseReal(0.8),PreciseReal(1.0/PreciseReal(LineSegmentCount)))*LineSegmentCount << endl;
                TestOutput << "20.0  @ 1.0  = " << li.Interpolate(DataPoints.begin(),DataPoints.end(),1.0)
                           << "\tLineSegment: " << Whole((1.0) * Real(LineSegmentCount)) << " LocalPercentage: " << std::fmod(PreciseReal(1.0),PreciseReal(1.0/PreciseReal(LineSegmentCount)))*LineSegmentCount << endl;
                TestOutput << endl;
                TEST_EQUAL_EPSILON(li.Interpolate(DataPoints.begin(),DataPoints.end(),0.0),a,"Real4PointLinearInterpolationBegin");
                TEST_EQUAL_EPSILON(li.Interpolate(DataPoints.begin(),DataPoints.end(),0.1),Real(1.5),"Real4PointLinearInterpolationTenth");
                TEST_EQUAL_MULTI_EPSILON(li.Interpolate(DataPoints.begin(),DataPoints.end(),0.3333333333),b,"Real4PointLinearInterpolationOneThird",8);
                TEST_EQUAL_EPSILON(li.Interpolate(DataPoints.begin(),DataPoints.end(),0.5),Real(7.5),"Real4PointLinearInterpolationHalfway");
                TEST_EQUAL_MULTI_EPSILON(li.Interpolate(DataPoints.begin(),DataPoints.end(),0.6666666666),c,"Real4PointLinearInterpolationTwoThirds",8);
                TEST_EQUAL_EPSILON(li.Interpolate(DataPoints.begin(),DataPoints.end(),0.8),Real(14.0),"Real4PointLinearInterpolationEighty");
                TEST_EQUAL_EPSILON(li.Interpolate(DataPoints.begin(),DataPoints.end(),1.0),d,"Real4PointLinearInterpolationEnd");
            }

            {
                LinearInterpolator<Transform> li;
                Transform a(Vector3(0,0,0),Quaternion(0,0,0,0));
                Transform z(Vector3(1,1,1),Quaternion(1,1,1,1));
                std::vector<Transform> DataPoints;
                DataPoints.push_back(a);
                DataPoints.push_back(z);

                TestOutput << "Lets try some simple Transform interpolations between 0 and 5 at 0,.1,.5, and 1:" << endl
                           << li.Interpolate(DataPoints.begin(),DataPoints.end(),0.0) << endl
                           << li.Interpolate(DataPoints.begin(),DataPoints.end(),0.1) << endl
                           << li.Interpolate(DataPoints.begin(),DataPoints.end(),0.5) << endl
                           << li.Interpolate(DataPoints.begin(),DataPoints.end(),1.0) << endl << endl;

                TEST_EQUAL_EPSILON(li.Interpolate(DataPoints.begin(),DataPoints.end(),0.0),a,"TransformLinearInterpolationBegin");
                TEST_EQUAL_EPSILON(li.Interpolate(DataPoints.begin(),DataPoints.end(),0.1),Transform(Vector3(0.1,0.1,0.1),Quaternion(0.1,0.1,0.1,0.1)),"TransformLinearInterpolationTenth");
                TEST_EQUAL_EPSILON(li.Interpolate(DataPoints.begin(),DataPoints.end(),0.5),Transform(Vector3(0.5,0.5,0.5),Quaternion(0.5,0.5,0.5,0.5)),"TransformLinearInterpolationHalfway");
                TEST_EQUAL_EPSILON(li.Interpolate(DataPoints.begin(),DataPoints.end(),1.0),z,"TransformLinearInterpolationEnd");
            }

            {
                BezierInterpolator<Real> Bi;
                Real A(0), B(1);

                std::vector<Real> DataPoints;
                DataPoints.push_back(A);
                DataPoints.push_back(B);

                TestOutput << "Lets try some simple Bezier interpolation of reals between 0, 1 and 2 with only 2 points (so that is really a linear interpolation):" << endl
                           << Bi.Interpolate(DataPoints.begin(),DataPoints.end(),0.0) << endl
                           << Bi.Interpolate(DataPoints.begin(),DataPoints.end(),0.1) << endl
                           << Bi.Interpolate(DataPoints.begin(),DataPoints.end(),0.5) << endl
                           << Bi.Interpolate(DataPoints.begin(),DataPoints.end(),1.0) << endl  << endl;

                TEST_THROW(Mezzanine::ParametersRangeException,
                           Bi.Interpolate(DataPoints.begin(),DataPoints.begin(),0.0),
                           "BezierNoArgThrows");
                TEST_THROW(Mezzanine::ParametersRangeException,
                           Bi.Interpolate(DataPoints.begin(),DataPoints.begin()+1,0.0),
                           "BezierSingleArgThrows");

                TEST_EQUAL_EPSILON(Bi.Interpolate(DataPoints.begin(), DataPoints.end(), 0.0),A,"BezierDualArg1");
                TEST_EQUAL_EPSILON(Bi.Interpolate(DataPoints.begin(), DataPoints.end(), 0.1),Real(0.1),"BezierDualArg2");
                TEST_EQUAL_EPSILON(Bi.Interpolate(DataPoints.begin(), DataPoints.end(), 0.5),Real(0.5),"BezierDualArg3");
                TEST_EQUAL_EPSILON(Bi.Interpolate(DataPoints.begin(), DataPoints.end(), 1.0),B,"BezierDualArg4");

            }

            {
                BezierInterpolator<Vector2> Bi;
                Vector2 A(0,0);
                Vector2 B(1,1);
                Vector2 C(0,2);

                std::vector<Vector2> DataPoints;
                DataPoints.push_back(A);
                DataPoints.push_back(B);
                DataPoints.push_back(C);

                TestOutput << "Lets try some 3 point Bezier interpolation of some Vector2s along the track (0,0) (1,1) (0,2):" << endl;
                TestOutput << Bi.Interpolate(DataPoints.begin(),DataPoints.end(),0.0) << endl;
                TestOutput << Bi.Interpolate(DataPoints.begin(),DataPoints.begin()+3,0.1) << endl;
                TestOutput << Bi.Interpolate(DataPoints.begin(),DataPoints.begin()+3,0.5) << endl;
                TestOutput << Bi.Interpolate(DataPoints.begin(),DataPoints.begin()+3,0.9) << endl;
                TestOutput << Bi.Interpolate(DataPoints.begin(),DataPoints.begin()+3,1.0) << endl;
                TestOutput << endl;
                TEST_EQUAL_EPSILON(Bi.Interpolate(DataPoints.begin(), DataPoints.begin()+3, 0.0),A,"BezierTriArg1");
                TEST_EQUAL_EPSILON(Bi.Interpolate(DataPoints.begin(), DataPoints.begin()+3, 0.1),Vector2(0.18,0.2),"BezierTriArg2");
                TEST_EQUAL_EPSILON(Bi.Interpolate(DataPoints.begin(), DataPoints.begin()+3, 0.5),Vector2(0.5,1),"BezierTriArg3");
                TEST_EQUAL_EPSILON(Bi.Interpolate(DataPoints.begin(), DataPoints.begin()+3, 0.9),Vector2(0.18,1.8),"BezierTriArg4");
                TEST_EQUAL_EPSILON(Bi.Interpolate(DataPoints.begin(), DataPoints.begin()+3, 1.0),C,"BezierTriArg5");
            }

            {
                Vector2 A(0,0);
                Vector2 B(1,1);
                Vector2 C(0,2);
                std::vector<Vector2> DataPoints;
                DataPoints.push_back(A);
                DataPoints.push_back(B);
                DataPoints.push_back(C);

                std::vector<Real> Weights;
                Weights.push_back(0.0);
                Weights.push_back(0.5);
                Weights.push_back(1.0);

                Vector2 DesiredAtZero(0,0);
                Vector2 DesiredAtPointTwoFive(0.6875,0.5);
                Vector2 DesiredAtPointFive(1,1);
                Vector2 DesiredAtPointSevenFive(0.6875,1.5);
                Vector2 DesiredAtOne(0,2);



                CubicSpline<Real,Vector2> Sp(Weights,DataPoints);

                TestOutput << "Lets try some 3 point Cubic Spline interpolation of some Vector2s along a raw spline with (0,0) (1,1) (0,2):" << endl;
                TestOutput << Sp.interpolate(0) << endl;
                TestOutput << Sp.interpolate(.25) << endl;
                TestOutput << Sp.interpolate(.5) << endl;
                TestOutput << Sp.interpolate(.75) << endl;
                TestOutput << Sp.interpolate(1) << endl << endl;
                TEST_EQUAL_EPSILON(Sp.interpolate(0.0),DesiredAtZero,"RawCubicSpline1");
                TEST_EQUAL_EPSILON(Sp.interpolate(0.25),DesiredAtPointTwoFive,"RawCubicSpline2");
                TEST_EQUAL_EPSILON(Sp.interpolate(0.5),DesiredAtPointFive,"RawCubicSpline3");
                TEST_EQUAL_EPSILON(Sp.interpolate(0.75),DesiredAtPointSevenFive,"RawCubicSpline4");
                TEST_EQUAL_EPSILON(Sp.interpolate(1.0),DesiredAtOne,"RawCubicSpline5");


                TestOutput << "Lets try some 3 point Cubic Spline interpolation of some Vector2s with and interpolator along (0,0) (1,1) (0,2):" << endl;
                TestOutput << SlowSplineInterpolator<Vector2>::Interpolate(DataPoints.begin(),DataPoints.end(),0) << endl;
                TestOutput << SlowSplineInterpolator<Vector2>::Interpolate(DataPoints.begin(),DataPoints.end(),0.25) << endl;
                TestOutput << SlowSplineInterpolator<Vector2>::Interpolate(DataPoints.begin(),DataPoints.end(),0.5) << endl;
                TestOutput << SlowSplineInterpolator<Vector2>::Interpolate(DataPoints.begin(),DataPoints.end(),0.75) << endl;
                TestOutput << SlowSplineInterpolator<Vector2>::Interpolate(DataPoints.begin(),DataPoints.end(),1) << endl << endl;
                TEST_EQUAL_EPSILON(SlowSplineInterpolator<Vector2>::Interpolate(DataPoints.begin(),DataPoints.end(),0)
                                   ,DesiredAtZero,"SlowSplineInterpolator1");
                TEST_EQUAL_EPSILON(SlowSplineInterpolator<Vector2>::Interpolate(DataPoints.begin(),DataPoints.end(),0.25)
                                   ,DesiredAtPointTwoFive,"SlowSplineInterpolator2");
                TEST_EQUAL_EPSILON(SlowSplineInterpolator<Vector2>::Interpolate(DataPoints.begin(),DataPoints.end(),0.5)
                                   ,DesiredAtPointFive,"SlowSplineInterpolator3");
                TEST_EQUAL_EPSILON(SlowSplineInterpolator<Vector2>::Interpolate(DataPoints.begin(),DataPoints.end(),0.75)
                                   ,DesiredAtPointSevenFive,"SlowSplineInterpolator4");
                TEST_EQUAL_EPSILON(SlowSplineInterpolator<Vector2>::Interpolate(DataPoints.begin(),DataPoints.end(),1)
                                   ,DesiredAtOne,"SlowSplineInterpolator5");

                SlowSplineInterpolator<Vector2>::Interpolate(DataPoints.begin(),DataPoints.end(),0.5);

                /*TestOutput << Sp.Interpolate(DataPoints.begin(),DataPoints.end(),0.0) << endl;
                TestOutput << Sp.Interpolate(DataPoints.begin(),DataPoints.begin()+3,0.1) << endl;
                TestOutput << Sp.Interpolate(DataPoints.begin(),DataPoints.begin()+3,0.5) << endl;
                TestOutput << Sp.Interpolate(DataPoints.begin(),DataPoints.begin()+3,0.9) << endl;
                TestOutput << Sp.Interpolate(DataPoints.begin(),DataPoints.begin()+3,1.0) << endl;
                TestOutput << endl;
                TEST_EQUAL_EPSILON(Sp.Interpolate(DataPoints.begin(), DataPoints.begin()+3, 0.0),A,"BezierTriArg1");
                TEST_EQUAL_EPSILON(Sp.Interpolate(DataPoints.begin(), DataPoints.begin()+3, 0.1),Vector2(0.18,0.2),"BezierTriArg2");
                TEST_EQUAL_EPSILON(Sp.Interpolate(DataPoints.begin(), DataPoints.begin()+3, 0.5),Vector2(0.5,1),"BezierTriArg3");
                TEST_EQUAL_EPSILON(Sp.Interpolate(DataPoints.begin(), DataPoints.begin()+3, 0.9),Vector2(0.18,1.8),"BezierTriArg4");
                TEST_EQUAL_EPSILON(Sp.Interpolate(DataPoints.begin(), DataPoints.begin()+3, 1.0),C,"BezierTriArg5");*/
            }
            /*{
                std::vector<Vector3> PreTrack;
                PreTrack.push_back(Vector3( 0, 0, 0));
                PreTrack.push_back(Vector3(10, 0, 5));
                PreTrack.push_back(Vector3(10,10,10));
                PreTrack.push_back(Vector3( 0,10,15));
                PreTrack.push_back(Vector3( 0, 0,20));
                for(Real Counter=0.0;
                    Counter<1.0;
                    Counter+=0.01)
                {
                    cout << InterpolatableTraits<Vector3>::BezierInterpolator::Interpolate(PreTrack.begin(),
                                                                                           PreTrack.end(),
                                                                                           Counter)
                         << endl;
                }
            }// */

        }

        /// @brief Since RunAutomaticTests is implemented so is this.
        /// @return returns true
        virtual bool HasAutomaticTests() const
            { return true; }

};

#endif

