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
            {
                TrackLinear<Vector3> TestDualPointTrack;
                TestDualPointTrack.push_back(Vector3(0,0,0));
                TestDualPointTrack.push_back(Vector3(10,10,10));
                TestOutput << "Testing interpolation on a Track with just two points:" << endl
                           << "\t0.1 should be 1,1,1 and is " << TestDualPointTrack.GetInterpolated(0.1) << endl
                           << "\t0.1 should be 5,5,5 and is " << TestDualPointTrack.GetInterpolated(0.5) << endl
                           << "\t0.1 should be 9.5,9.9,9.5 and is " << TestDualPointTrack.GetInterpolated(0.95) << endl;
                TEST_EQUAL_EPSILON(TestDualPointTrack.GetInterpolated(0.1), Vector3(1.0,1.0,1.0), "DualPointTrack1");
                TEST_EQUAL_EPSILON(TestDualPointTrack.GetInterpolated(0.5), Vector3(5.0,5.0,5.0), "DualPointTrack2");
                TEST_EQUAL_EPSILON(TestDualPointTrack.GetInterpolated(0.95), Vector3(9.5,9.5,9.5), "DualPointTrack3");
            }
            // The TEST macro will capture Line, function file Metadata while
            TEST(true,"AutomaticTest");
        }

        /// @brief Since RunAutomaticTests is implemented so is this.
        /// @return returns true
        virtual bool HasAutomaticTests() const
            { return true; }
};

#endif

