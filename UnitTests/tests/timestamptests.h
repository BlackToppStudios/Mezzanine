// © Copyright 2010 - 2017 BlackTopp Studios Inc.
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
#ifndef _timestamptests_h
#define _timestamptests_h

#include "mezztest.h"

#include "dagframescheduler.h"

/// @file
/// @brief Basic tests of the timestamp tools in the framescheduler

using namespace std;
using namespace Mezzanine;
using namespace Mezzanine::Testing;
using namespace Mezzanine::Threading;

/// @brief Tests for the timestamp tools
class timestamptests : public UnitTestGroup
{
    public:
        /// @copydoc Mezzanine::Testing::UnitTestGroup::Name
        /// @return Returns a String containing "TimeStamp"
        virtual String Name()
            { return String("TimeStamp"); }

        /// @brief Test if the timestamp tools works correctly were possible
        void RunAutomaticTests()
        {
            TestOutput << "Starting timekeeping tests." << endl;
            TestOutput << "Getting Timestamp1" << endl;
            Mezzanine::MaxInt Timestamp1 = Mezzanine::GetTimeStamp();

            TestOutput << "Sleeping main thread for 300ms." << endl;
            Mezzanine::Threading::this_thread::sleep_for(300000);

            TestOutput << "Getting Timestamp2" << endl;
            Mezzanine::MaxInt Timestamp2 = Mezzanine::GetTimeStamp();

            TestOutput << "Timestamp1: " << Timestamp1 << endl;
            TestOutput << "Timestamp2: " << Timestamp2 << endl;
            TestOutput << "Timestamp2 - Timestamp1 = " << Timestamp2-Timestamp1 << endl;
            TestOutput << "Is Timestamp1 <= Timestamp2: " << (Timestamp1<=Timestamp2) << endl;
            TestOutput << "Timer Resolution: " << GetTimeStampResolution() << " microsecond(s)" << endl;
            TEST(Timestamp1<=Timestamp2,"TimeStampChronology")

            TestOutput << "Is Timestamp1+300000-(2*TimerResolution) <= Timestamp2 = " << Timestamp1+300000-(2*GetTimeStampResolution()) << "<=" << Timestamp2 << endl;
            TestOutput << "Is Timestamp1+300000-(2*TimerResolution) <= Timestamp2: " << (MaxInt(Timestamp1+300000-(2*GetTimeStampResolution()))<=Timestamp2) << endl;
            TEST(MaxInt(Timestamp1+300000-(2*GetTimeStampResolution()))<=Timestamp2,"TimeStampResolution")
        }

        /// @brief Since RunAutomaticTests is implemented so is this.
        /// @return returns true
        virtual bool HasAutomaticTests() const
            { return true; }

};

#endif

