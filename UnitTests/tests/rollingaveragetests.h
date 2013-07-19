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
#ifndef _rollingaveragetests_h
#define _rollingaveragetests_h

#include "mezztest.h"

#include "dagframescheduler.h"

#include <typeinfo>

/// @file
/// @brief Basic tests of the rolling average class

using namespace std;
using namespace Mezzanine;
using namespace Mezzanine::Testing;
using namespace Mezzanine::Threading;


/// @brief Tests for the rolling average classes
class rollingaveragetests : public UnitTestGroup
{
    public:
        /// @copydoc Mezzanine::Testing::UnitTestGroup::Name
        /// @return Returns a String containing "RollingAverage"
        virtual String Name()
            { return String("RollingAverage"); }

        /// @brief Keeping track of times without temporary spikes in time is critical to the DAG Scheduling Algorithm.
        virtual void RunAutomaticTests()
        {
            cout << "Starting Rolling Average Tests" << endl;
            cout << "Is the Default Rolling Average the BufferedRollingAverage: " << (typeid(Mezzanine::DefaultRollingAverage<Mezzanine::Whole>::Type)==typeid(Mezzanine::BufferedRollingAverage<Mezzanine::Whole>)) << endl;
            cout << "Is the Default Rolling Average the BufferedRollingAverage: " << (typeid(Mezzanine::DefaultRollingAverage<Mezzanine::Whole>::Type)==typeid(Mezzanine::WeightedRollingAverage<Mezzanine::Whole,float>)) << endl;
            cout << "Creating a BufferedRollingAverage, WeightedRollingAverage and DefaultRollingAverage with Mezzanine::Whole (and sometimes float for math)" << endl;
            Mezzanine::BufferedRollingAverage<Mezzanine::Whole> RollingB(10);
            Mezzanine::WeightedRollingAverage<Mezzanine::Whole,float> RollingW(10);
            Mezzanine::DefaultRollingAverage<Mezzanine::Whole>::Type RollingD(10);

            cout << "Inserting into each: ";
            for(Mezzanine::Whole Counter=1; Counter<=20; Counter++)
            {
                cout << Counter << (20==Counter?".":", ");
                RollingB.Insert(Counter);
                RollingW.Insert(Counter);
                RollingD.Insert(Counter);
            }
            cout << endl;

            cout << "BufferedRollingAverage Result, should be 15: " << RollingB.GetAverage() << endl;
            TEST(RollingB.GetAverage()==15,"Buffered1")
            cout << "WeightedRollingAverage Result, should be about 10: " << RollingW.GetAverage() << endl;
            TEST(RollingW.GetAverage()>9||RollingW.GetAverage()<16,"Weighted1")
            cout << "DefaultRollingAverage Result, should match its underlying type : " << RollingD.GetAverage() << endl;

            cout << "Creating a BufferedRollingAverage, WeightedRollingAverage and DefaultRollingAverage with floats" << endl;
            Mezzanine::BufferedRollingAverage<float> RollingB2(10);
            Mezzanine::WeightedRollingAverage<float,float> RollingW2(10);
            Mezzanine::DefaultRollingAverage<float>::Type RollingD2(10);

            cout << "Inserting into each: ";
            for(float Counter=1.0; Counter<=20.0; Counter++)
            {
                cout << Counter << (20.0<=Counter?".":", ");
                RollingB2.Insert(Counter);
                RollingW2.Insert(Counter);
                RollingD2.Insert(Counter);
            }
            cout << endl;

            cout << "BufferedRollingAverage Result, should be ~15.5: " << RollingB2.GetAverage() << endl;
            TEST(RollingB2.GetAverage()>15.4 && RollingB2.GetAverage()<15.6,"Buffered2")
            cout << "WeightedRollingAverage Result, should be ~12.2158: " << RollingW2.GetAverage() << endl;
            TEST(RollingW2.GetAverage()>12.1 && RollingW2.GetAverage()<15.6,"Weighted2")

            cout << "DefaultRollingAverage Result, should match its underlying type : " << RollingD2.GetAverage() << endl;
        }

        /// @brief Since RunAutomaticTests is implemented so is this.
        /// @return returns true
        virtual bool HasAutomaticTests() const
            { return true; }
};

#endif

