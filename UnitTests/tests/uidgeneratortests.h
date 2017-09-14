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
#ifndef _uidgeneratortests_h
#define _uidgeneratortests_h

#include "mezztest.h"

#include <stdexcept> //only used to throw for TEST_THROW

/// @file
/// @brief This file contains the group of tests used to verify the UIDGenerator class performs correctly.

using namespace Mezzanine;
using namespace Mezzanine::Testing;

/// @brief A small series of tests to verify basic functionality of generating unique integer IDs with the Mezzanine.
class uidgeneratortests : public UnitTestGroup
{
public:
    /// @copydoc Mezzanine::Testing::UnitTestGroup::Name
    /// @return Returns a String containing "UIDGenerator"
    virtual String Name()
        { return String("UIDGenerator"); }

    /// @brief This is called when Automatic tests are run
    void RunAutomaticTests()
    {
        UIDGenerator<UInt32> IDGen;
        // [1,int_max]
        Boole ReserveSuccess1 = IDGen.ReserveID(3);
        // [1,2] [4,int_max]
        Boole ReserveSuccess2 = IDGen.ReserveID(6);
        // [1,2] [4,5] [7,int_max]
        Boole ReserveSuccess3 = IDGen.ReserveID(9);
        // [1,2] [4,5] [7,8] [10,int_max]
        Boole ReserveFail1 = IDGen.ReserveID(6);// Should fail
        // [1,2] [4,5] [7,8] [10,int_max]

        TEST(ReserveSuccess1 && ReserveSuccess2 && ReserveSuccess3 && !ReserveFail1,"UIDGenerator::ReserveID(const_IDType&)");

        UInt32 Generated1 = IDGen.GenerateID();
        // [2,2] [4,5] [7,8] [10,int_max]
        UInt32 Generated2 = IDGen.GenerateID();
        // [4,5] [7,8] [10,int_max]
        UInt32 Generated3 = IDGen.GenerateID();
        // [5,5] [7,8] [10,int_max]

        TEST(Generated1 == 1 && Generated2 == 2 && Generated3 == 4,"UIDGenerator::GenerateID()");

        Boole ReleaseSuccess1 = IDGen.ReleaseID(2);
        // [2,2] [5,5] [7,8] [10,int_max]
        Boole ReleaseSuccess2 = IDGen.ReleaseID(3);
        // [2,3] [5,5] [7,8] [10,int_max]
        Boole ReleaseSuccess3 = IDGen.ReleaseID(4);
        // [2,5] [7,8] [10,int_max]
        Boole ReleaseFail1 = IDGen.ReleaseID(12);// Should fail
        // [2,5] [7,8] [10,int_max]

        TEST(ReleaseSuccess1 && ReleaseSuccess2 && ReleaseSuccess3 && !ReleaseFail1,"UIDGenerator::ReleaseID(const_IDType&)");

        Boole CheckSuccess = IDGen.IsIDUsed(9);
        Boole CheckFail = IDGen.IsIDUsed(50);

        TEST(CheckSuccess && !CheckFail,"UIDGenerator::IsIDUsed(const_IDType&)");
    }

    /// @brief Since RunAutomaticTests is implemented so is this.
    /// @return returns true
    virtual bool HasAutomaticTests() const
        { return true; }
};//uidgeneratortests

#endif
