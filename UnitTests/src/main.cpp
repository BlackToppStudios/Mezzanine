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

#include "autodetect.h"
#include "datatypes.h"
#include "main.h"

#include <cstdlib> // For system
#include <fstream>
#include <vector>
#include <stdexcept>

static const Mezzanine::String MemSpaceArg("inthismemoryspacetheworkshallbedone");
Mezzanine::String CommandName;

// this does all the heavy lifting
class AllUnitTestGroups : public UnitTestGroup
{
    public:
        bool RunAll;
        bool RunAutomaticTests;
        bool RunInteractiveTests;
        bool ExecuteInThisMemorySpace;
        GlobalCoreTestGroup& TestGroups;

        AllUnitTestGroups(GlobalCoreTestGroup& GlobalTestGroups) :
            RunAll(true),
            RunAutomaticTests(false),
            RunInteractiveTests(false),
            ExecuteInThisMemorySpace(false),
            TestGroups(GlobalTestGroups)
        {}

        std::vector<Mezzanine::String> TestGroupsToRun;           //List of tests to run

        virtual void RunTests()
        {
            if (RunAutomaticTests==RunInteractiveTests && RunInteractiveTests==false)   // enforce running automatic tests if no type of test is specified
                { RunAutomaticTests=true;  }

            if (RunAll)
            {
                for(map<String,UnitTestGroup*>::iterator Iter=TestGroups.begin(); Iter!=TestGroups.end(); ++Iter)
                    { TestGroupsToRun.push_back(Iter->first); }
            }

            if(ExecuteInThisMemorySpace) // Should we be executing the test right now?
            {
                for(std::vector<Mezzanine::String>::iterator CurrentTestName=TestGroupsToRun.begin(); CurrentTestName!=TestGroupsToRun.end(); ++CurrentTestName ) // Actually run the tests
                {
                    try{
                        TestGroups[*CurrentTestName]->RunTests(RunAutomaticTests, RunInteractiveTests);
                    }catch (std::exception e){
                        // maybe we should log or somehting.
                    }

                    (*this) += *(TestGroups[*CurrentTestName]);
                }
            }else{ // No, We should be executing the test in a place that cannot possibly crash this program
                for(std::vector<Mezzanine::String>::iterator CurrentTestName=TestGroupsToRun.begin(); CurrentTestName!=TestGroupsToRun.end(); ++CurrentTestName )
                {
                    ClearTempFile();
                    //std::cout << String(CommandName + " " + *CurrentTestName + " " + MemSpaceArg) << std::endl;
                    if(system(String(CommandName + " " + *CurrentTestName + " " + MemSpaceArg + " " + Mezzanine::String(RunAutomaticTests?"automatic ":"") + Mezzanine::String(RunInteractiveTests?"interactive ":"")).c_str()))   // Run a single unit test as another process
                    {
                        this->AddTestResult(String("Process::" + *CurrentTestName), Failed);
                    }else {
                        this->AddTestResult(String("Process::" + *CurrentTestName), Success);
                    }

                    (*this) += GetResultsFromTempFile();

                }
                DeleteTempFile();
            } // \if(ExecuteInThisMemorySpace)
        } // \function

        virtual void DisplayResults(std::ostream& Output=std::cout, bool Summary = true, bool FullOutput = true, bool HeaderOutput = true)
        {
            if(ExecuteInThisMemorySpace) // we are running a test in a seperate process, so we need to control the output for communcation purposes
            {
                std::ofstream OutputFile(TempFile.c_str(),std::ios_base::out|std::ios_base::trunc);
                UnitTestGroup::DisplayResults(OutputFile,false,true,false);
                OutputFile.close();

            }else{
                UnitTestGroup::DisplayResults(Output, Summary, FullOutput, HeaderOutput);
            }
        }
};

/// @brief This
int main (int argc, char** argv)
{
    GlobalCoreTestGroup TestGroups;
    bool WriteFile = true;

    if( !system( NULL ) ) // If this is not being run from a shell somehow, then using system() to run this task in a new process is not really possible.
    {
        std::cerr << "system() call not supported, missing command processor." << std::endl;
        return EXIT_FAILURE;
    }

    // Display everything, or just a Summary or neither? Should this process do the work, or should we spawn a new process.
    bool FullDisplay = true, SummaryDisplay = true;

    if (argc > 0) //Not really sure how this would happen, but I would rather test for it than have it fail
        { CommandName=argv[0]; }
    else
        { return Usage("UnitTestGroups", TestGroups); }

    AllUnitTestGroups Runner(TestGroups);

    for (int c=1; c<argc; ++c) // Check Command line for keywords and get all the test names
    {
        String ThisArg(AllLower(argv[c]));
        if(ThisArg=="help")
            { return Usage(CommandName, TestGroups); }
        else if(ThisArg==MemSpaceArg)        // Check to see if we do the work now or later
            { Runner.ExecuteInThisMemorySpace = true; }
        else if(ThisArg=="testlist")
            { return PrintList(TestGroups); }
        else if(ThisArg=="interactive")
            { Runner.RunInteractiveTests=true; Runner.RunAll=false; }
        else if(ThisArg=="automatic")
            { Runner.RunAutomaticTests=true; Runner.RunAll=false; }
        else if(ThisArg=="all")
            { Runner.RunAll=true; }
        else if(ThisArg=="summary")
            { FullDisplay = false, SummaryDisplay = true; }
        else if(ThisArg=="skipfile")
            { WriteFile = false; }
        else  // Wasn't a command so it is either gibberish or a test group
        {
            try
            {
                TestGroups.at(ThisArg.c_str());
                Runner.RunAll=false;
                Runner.TestGroupsToRun.push_back(AllLower(argv[c]));
            } catch ( const std::out_of_range& e) {
                std::cerr << ThisArg << " is not a valid testgroup or parameter." << std::endl;
                Usage(CommandName, TestGroups);
                return ExitInvalidArguments;
            }
        }
    }

    Runner.RunTests();

    if(WriteFile)
    {
        String FileName("TestResults.txt");
        std::ofstream OutFile(FileName.c_str());
        Runner.DisplayResults(OutFile,SummaryDisplay,FullDisplay);
        OutFile.close();
    }
    Runner.DisplayResults(std::cout,SummaryDisplay,FullDisplay);

    return ExitSuccess;
 }
