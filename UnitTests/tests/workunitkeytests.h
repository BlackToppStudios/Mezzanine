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
#ifndef _workunitkeytests_h
#define _workunitkeytests_h

#include "mezztest.h"

#include "dagframescheduler.h"
#include "workunittests.h"

/// @file
/// @brief Basic tests of the workunitkeys class

using namespace std;
using namespace Mezzanine;
using namespace Mezzanine::Testing;
using namespace Mezzanine::Threading;

/// @brief Tests for the WorkUnitket class, the sort of workunits
class workunitkeytests : public UnitTestGroup
{
    public:
        /// @copydoc Mezzanine::Testing::UnitTestGroup::Name
        /// @return Returns a String containing "WorkUnitKey"
        virtual String Name()
            { return String("WorkUnitKey"); }

        /// @brief Test if the WorkUnit
        void RunAutomaticTests()
        {
            cout << "Creating a number of WorkUnitKeys and tesing their ability to sort: " << endl;
            cout << "\t" << "                   Deps, Time, Ptr" << endl;
            cout << "\t" << "WorkUnitKey First(   10, 500,  0 ); //Units with the most dependants get priority" << endl;
            cout << "\t" << "WorkUnitKey Second(   5, 600,  0 ); //Then Units that take more time" << endl;
            cout << "\t" << "WorkUnitKey Third(    5, 500,  0 ); " << endl;
            cout << "\t" << "WorkUnitKey Fourth(   3, 500,  1 ); //In case of a tie use higher pointer" << endl;
            cout << "\t" << "WorkUnitKey Fifth(    3, 500,  0 );" << endl;
            WorkUnitKey First(10,500,0);
            WorkUnitKey Second(5,600,0);
            WorkUnitKey Third(5,500,0);
            WorkUnitKey Fourth(3,500,(iWorkUnit*)1);
            WorkUnitKey Fifth(3,500,0);
            cout << "Second<First: " << (Second < First) << "\t Third<First: " << (Third < First) << "\t Fourth<First: " << (Fourth < First) << "\t Fifth<First: " << (Fifth < First) << endl;
            TEST(Second < First,"Second<First");
            TEST(Third < First,"Third<First");
            TEST(Fourth < First,"Fourth<First");
            TEST(Fifth < First,"Fifth<First");

            cout << "Third<Second: " << (Third < Second) << "\t Fourth<Second: " << (Fourth < Second) << "\t Fifth<Second: " << (Third < Second) << endl;
            TEST(Third < Second,"Third<Second");
            TEST(Fourth < Second,"Fourth<Second");
            TEST(Fifth < Second,"Fifth<Second");

            cout << "Fourth<Third: " << (Fourth < Third) << "\t Fifth<Third: " << (Fifth < Third) << endl;
            TEST(Fourth < Third,"Fourth<Third");
            TEST(Fifth < Third,"Fifth<Third");
            TEST(Fifth < Fourth,"Fifth<Fourth");

            cout << "First<Second: " << (First < Second) << "\t First<Third: " << (First < Third) << "\t First<Fourth: " << (First < Fourth) << "\t First<Fifth: " << (First < Fifth) << endl;
            TEST(!(First < Second),"!(First<Second)");
            TEST(!(First < Third),"!(First<Third)");
            TEST(!(First < Fourth),"!(First<Fourth)");
            TEST(!(First < Fifth),"!(First<Fifth)");

            cout << "Second<Third: " << (Second < Third) << "\t Second<Fourth: " << (Second < Fourth) << "\t Second<Fifth: " << (Second < Fifth) << endl;
            TEST(!(Second < Third),"!(Second<Third)");
            TEST(!(Second < Fourth),"!(Second<Fourth)");
            TEST(!(Second < Fifth),"!(Second<Fifth)");

            cout << "Third<Fourth: " << (Third < Fourth) << "\t Third<Fifth: " << (Third < Fifth) << endl;
            TEST(!(Third < Fourth),"!(Third<Fourth)");
            TEST(!(Third < Fifth),"!(Third<Fifth)");

            cout << "Fourth<Fifth: " << (Fourth<Fifth) << endl;
            TEST(!(Fourth < Fifth),"!(Fourth<Fifth)");

            cout << "Creating 4 WorkUnits for a sorting test with an set (be the only differrence between fourth and fifth was the address of the workunit, and we cannot control that.):" << endl;
            PiMakerWorkUnit *WorkUnitK1 = new PiMakerWorkUnit(500,"First",false);
            PiMakerWorkUnit *WorkUnitK2 = new PiMakerWorkUnit(500,"Second",false);
            PiMakerWorkUnit *WorkUnitK3 = new PiMakerWorkUnit(500,"Third",false);
            PiMakerWorkUnit *WorkUnitK4 = new PiMakerWorkUnit(500,"Fourth",false);
            First.Unit=WorkUnitK1;
            Second.Unit=WorkUnitK2;
            Third.Unit=WorkUnitK3;
            Fourth.Unit=WorkUnitK4;
            set<WorkUnitKey> WorkUnitKeyTest;
            WorkUnitKeyTest.insert(Second);
            WorkUnitKeyTest.insert(Fourth);
            WorkUnitKeyTest.insert(Third);
            WorkUnitKeyTest.insert(First);
            set<WorkUnitKey>::reverse_iterator Iter=WorkUnitKeyTest.rbegin();

            cout << ((PiMakerWorkUnit*)(Iter->Unit))->Name << " ";
            TEST(((PiMakerWorkUnit*)(Iter->Unit))->Name == String("First"), "First");
            Iter++;

            cout << ((PiMakerWorkUnit*)(Iter->Unit))->Name << " ";
            TEST(((PiMakerWorkUnit*)(Iter->Unit))->Name == String("Second"), "Second");

            Iter++;
            cout << ((PiMakerWorkUnit*)(Iter->Unit))->Name << " ";
            TEST(((PiMakerWorkUnit*)(Iter->Unit))->Name == String("Third"), "Third");
            Iter++;

            cout << ((PiMakerWorkUnit*)(Iter->Unit))->Name << " ";
            TEST(((PiMakerWorkUnit*)(Iter->Unit))->Name == String("Fourth"), "Fourth");
            Iter++;
            cout << endl;

            delete WorkUnitK1; delete WorkUnitK2; delete WorkUnitK3; delete WorkUnitK4;
        }

        /// @brief Since RunAutomaticTests is implemented so is this.
        /// @return returns true
        virtual bool HasAutomaticTests() const
            { return true; }
};

#endif

