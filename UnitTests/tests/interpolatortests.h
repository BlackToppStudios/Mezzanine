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
            {
                InterpolatableTraits<Vector3>::LinearInterpolator li;
                Vector3 a(0.0,0.0,0.0);
                Vector3 b(1.0,1.0,1.0);

                TestOutput << "Lets try some simple interpolations between 0,0,0 and 1,1,1 at 0,.1,.5, and 1:" << endl
                           << li.Interpolate(a,b,0.0) << endl
                           << li.Interpolate(a,b,0.1) << endl
                           << li.Interpolate(a,b,0.5) << endl
                           << li.Interpolate(a,b,1.0) << endl << endl;

                TEST_EQUAL_EPSILON(li.Interpolate(a,b,0.0),a,"Vector3LinearInterpolationBegin");
                TEST_EQUAL_EPSILON(li.Interpolate(a,b,0.1),Vector3(0.1,0.1,0.1),"Vector3LinearInterpolationTenth");
                TEST_EQUAL_EPSILON(li.Interpolate(a,b,0.5),Vector3(0.5,0.5,0.5),"Vector3LinearInterpolationHalfway");
                TEST_EQUAL_EPSILON(li.Interpolate(a,b,1.0),b,"Vector3LinearInterpolationEnd");
             }

            {
                InterpolatableTraits<Integer>::LinearInterpolator li;
                Integer a = 0;
                Integer b = 10;

                TestOutput << "Lets try some simple Integer interpolations between 0 and 10 at 0,.1,.5, and 1:" << endl
                           << li.Interpolate(a,b,0.0) << endl
                           << li.Interpolate(a,b,0.1) << endl
                           << li.Interpolate(a,b,0.5) << endl
                           << li.Interpolate(a,b,1.0) << endl << endl;

                TEST_EQUAL_EPSILON(li.Interpolate(a,b,0.0),a,"IntegerLinearInterpolationBegin");
                TEST_EQUAL_EPSILON(li.Interpolate(a,b,0.1),1,"IntegerLinearInterpolationTenth");
                TEST_EQUAL_EPSILON(li.Interpolate(a,b,0.5),5,"IntegerLinearInterpolationHalfway");
                TEST_EQUAL_EPSILON(li.Interpolate(a,b,1.0),b,"IntegerLinearInterpolationEnd");
             }

            {
                InterpolatableTraits<Real>::LinearInterpolator li;
                Real a = 0;
                Real b = 5;

                TestOutput << "Lets try some simple Integer interpolations between 0 and 5 at 0,.1,.5, and 1:" << endl
                           << li.Interpolate(a,b,0.0) << endl
                           << li.Interpolate(a,b,0.1) << endl
                           << li.Interpolate(a,b,0.5) << endl
                           << li.Interpolate(a,b,1.0) << endl << endl;

                TEST_EQUAL_EPSILON(li.Interpolate(a,b,0.0),a,"RealLinearInterpolationBegin");
                TEST_EQUAL_EPSILON(li.Interpolate(a,b,0.1),Real(0.5),"RealLinearInterpolationTenth");
                TEST_EQUAL_EPSILON(li.Interpolate(a,b,0.5),Real(2.5),"RealLinearInterpolationHalfway");
                TEST_EQUAL_EPSILON(li.Interpolate(a,b,1.0),b,"RealLinearInterpolationEnd");
             }
        }

        /// @brief Since RunAutomaticTests is implemented so is this.
        /// @return returns true
        virtual bool HasAutomaticTests() const
            { return true; }

};

#endif

