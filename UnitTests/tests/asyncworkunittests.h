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
#ifndef _asyncworkunittests_h
#define _asyncworkunittests_h

#include "mezztest.h"

#include "dagframescheduler.h"

#include <iomanip>

/// @file
/// @brief Basic tests of the asyncronous workunits

using namespace std;
using namespace Mezzanine;
using namespace Mezzanine::Testing;
using namespace Mezzanine::Threading;

/// @brief Create a string suitable for output that converts IO in bytes and time in  microseconds to easily human readable text
String GetPerfString(double IOSize, double Duration)
{
    stringstream Maker;
    Maker << fixed << showpoint << setprecision(2);
    vector<String> Terms;
    Terms.push_back(" Bytes/sec");
    Terms.push_back(" KB/sec");
    Terms.push_back(" MB/sec");
    Terms.push_back(" GB/sec");
    Terms.push_back(" TB/sec");
    Whole WhichTerm = 0;

    IOSize = IOSize/Duration * double(1000000); // convert to bytes/sec

    while(1024<IOSize && 4!=WhichTerm)
    {
        WhichTerm++;
        IOSize/=1024;
    }
    Maker << IOSize << Terms[WhichTerm];
    return Maker.str();
}


/// @brief Tests for the asyncronous workunits
class asyncworkunittests : public UnitTestGroup
{
    public:
        /// @copydoc Mezzanine::Testing::UnitTestGroup::Name
        /// @return Returns a String containing "AsyncWorkUnit"
        virtual String Name()
            { return String("AsyncWorkUnit"); }

        /// @copydoc Mezzanine::Testing::UnitTestGroup::RunTests
        /// @detail Test if the asyncronous workunits
        virtual void RunTests(bool RunAutomaticTests, bool RunInteractiveTests)
        {
            RunInteractiveTests = false; //prevent warnings

            if (RunAutomaticTests)
            {
                TestResult temp;

                cout << "Creating three files that might take whole seconds to write." << endl;
                MaxInt MaxTime = 10000000;
                Whole MaxFileWrites = 10000000;
                Whole CurrentCount = 0;
                MaxInt TimeStarted = GetTimeStamp();

                vector<String> Files;
                Files.push_back(String("File1.txt"));
                Files.push_back(String("File2.txt"));
                Files.push_back(String("File3.txt"));

                ofstream File1a(Files[0].c_str());
                ofstream File2a(Files[1].c_str());
                ofstream File3a(Files[2].c_str());
                while(GetTimeStamp()<TimeStarted+MaxTime && CurrentCount<MaxFileWrites)
                {
                    CurrentCount++;
                    File1a.write("Packets1Packets2Packets3", 24);
                    File2a.write("01", 2);
                    File3a.write("-", 1);
                }
                Whole Duration = GetTimeStamp()-TimeStarted;
                Whole WriteSize = 27*CurrentCount;
                String PerfString(GetPerfString(WriteSize,Duration));
                File1a.close();
                File2a.close();
                File3a.close();
                cout << fixed << showpoint << setprecision(2);
                cout << "Creating files took " << Duration << " microseconds " << endl;
                cout << "A total of " << WriteSize << " Bytes were written or " << PerfString << endl;

                cout << "Creating an AsynchronousFileLoadWorkUnit to load the contents of these files." << endl;
                AsynchronousFileLoadWorkUnit Testable;
                Testable.BeginLoading(Files);

                FrameScheduler Scheduler1(&cout,2);
                DefaultThreadSpecificStorage::Type AResource(&Scheduler1);
                temp = Testing::Success;
                TimeStarted = GetTimeStamp();
                while(Complete!=Testable.IsWorkDone())
                {
                    Testable.DoWork(AResource);
                    if(GetTimeStamp()>TimeStarted+MaxTime*20)
                    {
                        temp=Testing::Failed;
                        break;
                    }
                }
                AddTestResult("DAGFrameScheduler::Async::ReadWriteTimeParity", temp);

                Duration = GetTimeStamp()-TimeStarted;
                Whole ReadSize = Testable.GetFile(0)->Size+Testable.GetFile(1)->Size+Testable.GetFile(2)->Size;
                PerfString = GetPerfString(ReadSize,Duration);
                cout << "Reading file took " << Duration << " microseconds " << endl;
                cout << "A total of " << ReadSize << " Bytes were read or " << PerfString << endl;


                cout << "The files have been loaded. performing a basic consistency check." << endl;
                if(ReadSize == WriteSize )
                    { temp=Testing::Success; }
                else
                    { temp=Testing::Failed; }
                AddTestResult("DAGFrameScheduler::Async::Read1Valid", temp);
                if(Testable.GetFile(0)->Size>0)
                    { temp=Testing::Success; }
                else
                    { temp=Testing::Failed; }
                AddTestResult("DAGFrameScheduler::Async::Read1Present", temp);
                if(Testable.GetFile(0)->Data[0]=='P')
                    { temp=Testing::Success; }
                else
                    { temp=Testing::Failed; }
                AddTestResult("DAGFrameScheduler::Async::Read1Valid", temp);


                if(Testable.GetFile(1)->Size>0)
                    { temp=Testing::Success; }
                else
                    { temp=Testing::Failed; }
                AddTestResult("DAGFrameScheduler::Async::Read2Present", temp);
                if(Testable.GetFile(1)->Data[0]=='0')
                    { temp=Testing::Success; }
                else
                    { temp=Testing::Failed; }
                AddTestResult("DAGFrameScheduler::Async::Read2Valid", temp);


                if(Testable.GetFile(2)->Size>0)
                    { temp=Testing::Success; }
                else
                    { temp=Testing::Failed; }
                AddTestResult("DAGFrameScheduler::Async::Read3Present", temp);
                if(Testable.GetFile(2)->Data[0]=='-')
                    { temp=Testing::Success; }
                else
                    { temp=Testing::Failed; }
                AddTestResult("DAGFrameScheduler::Async::Read3Valid", temp);

                cout << "Files seem at least superficially consistent, trunctating files on disk to conserve space." << endl;

                ofstream File1b(Files[0].c_str());
                ofstream File2b(Files[1].c_str());
                ofstream File3b(Files[2].c_str());
                File1b.close();
                File2b.close();
                File3b.close();

                if(true)
                    { temp=Testing::Success; }
                else
                    { temp=Testing::Failed; }
                AddTestResult("DAGFrameScheduler::WorkUnit::true", temp);

            }else{
                AddTestResult("DAGFrameScheduler::WorkUnit::true", Testing::Skipped);
                AddTestResult("DAGFrameScheduler::Async::ReadWriteTimeParity", Testing::Skipped);
                AddTestResult("DAGFrameScheduler::Async::Read1Valid", Testing::Skipped);
                AddTestResult("DAGFrameScheduler::Async::Read1Present", Testing::Skipped);
                AddTestResult("DAGFrameScheduler::Async::Read1Valid", Testing::Skipped);
                AddTestResult("DAGFrameScheduler::Async::Read2Present", Testing::Skipped);
                AddTestResult("DAGFrameScheduler::Async::Read2Valid", Testing::Skipped);
                AddTestResult("DAGFrameScheduler::Async::Read3Present", Testing::Skipped);
                AddTestResult("DAGFrameScheduler::Async::Read3Valid", Testing::Skipped);
            }


        }
};

#endif

