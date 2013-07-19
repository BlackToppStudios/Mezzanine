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
#ifndef _logaggregatortests_h
#define _logaggregatortests_h

#include "mezztest.h"

#include "dagframescheduler.h"
#include "monopolytests.h"
#include "pugixml.h"

/// @file
/// @brief Basic tests of the logging system

using namespace std;
using namespace Mezzanine;
using namespace Mezzanine::Testing;
using namespace Mezzanine::Threading;


/// @brief Tests for the log aggregator
class logaggregatortests : public UnitTestGroup
{
    public:
        /// @copydoc Mezzanine::Testing::UnitTestGroup::Name
        /// @return Returns a String containing "LogAggregator"
        virtual String Name()
            { return String("LogAggregator"); }

        /// @copydoc Mezzanine::Testing::UnitTestGroup::RunTests
        /// @detail Test if the log aggregator aggregates
        virtual void RunAutomaticTests()
        {
            stringstream TempLog;
            TempLog << "<Log>" << endl;

            cout << endl << "Creating a FrameScheduler with a few PiMakerWorkUnits Testing the logger workunits to get a handle multiple logs, logging to cout: " << endl;

            FrameScheduler TestSchedulerMono(&TempLog,4);           // Create a frame scheduler with 4 threads
            TestSchedulerMono.AddWorkUnitMain(new PiMakerWorkUnit(500,"A",false));
            TestSchedulerMono.AddWorkUnitMain(new PiMakerWorkUnit(500,"A",false));
            TestSchedulerMono.AddWorkUnitMain(new PiMakerWorkUnit(500,"A",false)); // Add 4 workunit one for each
            TestSchedulerMono.AddWorkUnitMain(new PiMakerWorkUnit(500,"A",false)); // Hopefully there is enough work that each thread gets on each frame
            TestSchedulerMono.SetFrameLength(0);                    // run as fast as allowable

            LogAggregator Agg;
            LogBufferSwapper Swapper;                               //create an aggreator and swapper

            ThreadSpecificStorage SwapResource(&TestSchedulerMono); //This will only be used for its pointer to the Framescheduler
            for(Whole Counter=0; Counter<100; Counter++)
            {
                TestSchedulerMono.DoOneFrame(); // Framescheduler pass its own resources into each Pimaker each frame
                Swapper(SwapResource);          // Swap the active logs and bufferable logs
                Agg(SwapResource);              // Aggregate buferrable logs and send them to the Framescheduler Logger output TempLog
            }
            TempLog << "</Log>" << endl;


            cout << TempLog.str();
            cout << "Large log should have been emitted that showed PI being calculated 400 times and which thread it was calculated in. 100 iterations should have occurred each in one of four threads." << endl;

            pugi::xml_document Doc;
            pugi::xml_parse_result Results = Doc.load(TempLog);                  // if there is no race condition the output should be valid XML

            TEST(pugi::status_ok == Results.status, "LogAggregator");
        }

        /// @brief Since RunAutomaticTests is implemented so is this.
        /// @return returns true
        virtual bool HasAutomaticTests() const
            { return true; }

};

#endif

