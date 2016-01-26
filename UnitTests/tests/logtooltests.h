// © Copyright 2010 - 2016 BlackTopp Studios Inc.
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
#ifndef _logtooltests_h
#define _logtooltests_h

#include "mezztest.h"

#include <sstream>

/// @file
/// @brief This has tests for logging dedicated components in the frame scheduler

using namespace Mezzanine;
using namespace Mezzanine::Testing;

/// @brief Tests the ScopedTimer to verify its format is what is expected to be in the log
class logtooltests : public UnitTestGroup
{
    public:
        /// @copydoc Mezzanine::Testing::UnitTestGroup::Name
        /// @return Returns a String containing "BoilerPlate"
        virtual String Name()
            { return String("LogTool"); }

        std::stringstream TestLog;

        void ScopedTimerTest()
        {
            SCOPEDTIMER(TestLog);
            Mezzanine::Threading::this_thread::sleep_for(10000);
        }

        /// @brief This is called when Automatic tests are run
        void RunAutomaticTests()
        {
            TestOutput << "Instrumenting a sample function and reading log: " << endl;
            TestLog << "<log>" << endl;
            ScopedTimerTest();
            TestLog << "</log>" << endl;

            pugi::xml_document Doc;
            Doc.load(TestLog);

            pugi::xpath_node_set Results = Doc.select_nodes("/log/ScopedTimerBegin/@TimeStamp");
            String ResultString(Results.first().attribute().as_string());
            TEST(ResultString.size()>0,"BeginTimeStamp");

            Results = Doc.select_nodes("/log/ScopedTimerBegin/@Line");
            Whole LineNumber = Results.first().attribute().as_int();
            TEST(LineNumber==66,"BeginLine");

            Results = Doc.select_nodes("/log/ScopedTimerBegin/@FileName");
            ResultString = String(Results.first().attribute().as_string());
            Whole Location = ResultString.find_last_of("\\/") + 1;
            if(Location <= ResultString.size() )
                { ResultString = ResultString.substr(Location); }
            TEST(ResultString == String("logtooltests.h"),"BeginFileName");

            Results = Doc.select_nodes("/log/ScopedTimerEnd/@TimeStamp");
            ResultString = String(Results.first().attribute().as_string());
            TEST(ResultString.size()>0,"EndTimeStamp");

            Results = Doc.select_nodes("/log/ScopedTimerEnd/@Duration");
            Whole Duration = Results.first().attribute().as_int();
            TEST(9900<Duration && Duration<12000,"EndDuration");

            TestOutput << TestLog.str() << endl;
        }

        /// @brief Since RunAutomaticTests is implemented so is this.
        /// @return returns true
        virtual bool HasAutomaticTests() const
            { return true; }


};

#endif

