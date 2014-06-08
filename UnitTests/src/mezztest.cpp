// © Copyright 2010 - 2014 BlackTopp Studios Inc.
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
#include "mezztest.h"

#include <cstdlib> // For system
#include <fstream>
#include <vector>
#include <stdexcept>

/// @file
/// @brief This file is the entry point for the unit test framework.
/// @details If you need to change the nature of the executable this is the
/// file to change. This is where the simple (but robust) sub process
/// mechanism is implemented. Very little of the rest of the code in the
/// unit test frame work makes calls to this file, and everything that does
/// does so through the UnitTestGroup class via polymorphism.

/// @internal
/// @brief ArgC as it was passed into Main.
/// @details This cannot be set statically, it must wait for main(int, char**) to
/// be initialized
int ArgC;
int GetMainArgumentCount()
    { return ArgC; }

/// @internal
/// @brief ArgC as it was passed into Main.
/// @details This cannot be set statically, it must wait for main(int, char**) to
/// be initialized
char** ArgV;
char** GetMainArgumentVector()
    { return ArgV; }

/// @internal
/// @brief This will store the name of the command that launched this executable at run time
Mezzanine::String CommandName;
Mezzanine::String GetExecutableName()
    { return CommandName; }

/// @internal
/// @brief The current process depth as interpretted by Main
ProcessDepth Depth;
ProcessDepth GetCurrentProcessDepth()
    { return Depth; }

/// @internal
/// @brief A string intended for use by any subsubprocess test
Mezzanine::String SubSubProcessArgument;
Mezzanine::String GetSubSubProcessArgument()
    { return SubSubProcessArgument; }


/// @brief Write the passed UnitTestGroup to an XML temp file
/// @param TestsToWrite Teh group of tests to write.
void WriteTempFile(const UnitTestGroup &TestsToWrite)
{
    std::ofstream File(TempFile.c_str());
    File << TestsToWrite.GetAsXML();
    File.close();
}

/// @internal
/// @brief This will open then parse the contents of the file specified by TempFile and interpret any test results located
/// @throw This can throw any exception that the C++ filestream classes can throw.
/// @return This "reads" the temp file and interprets it. It tries to extract the name of the test as the whole of a line minus
/// the last word. The last word is then converted into a @ref TestResult using @ref StringToTestResult . Any Whitespace between
/// between the end of the last word and the end of the test name is dropped. All lines are interpretted this way and returned
/// as a single @ref UnitTestGroup.
UnitTestGroup GetResultsFromTempFile()
{
    UnitTestGroup Results;

    pugi::xml_document Doc;

    std::ifstream InputFile(TempFile.c_str());
    pugi::xml_parse_result LoadResults = Doc.load(InputFile);

    if(LoadResults)
    {
        Results.AddTestsFromXML(Doc.first_child());
    }else{
        std::stringstream FailStream;
        FailStream << "Failure loading tempfile from SubProcess: "
                   << LoadResults.description() << std::endl
                   << "At " << LoadResults.offset << " bytes into the file.";
        throw runtime_error(FailStream.str());
    }

    return Results;
}

/// @internal
/// @brief Empty the file specified by TempFile
/// @warning This doesn't ask for permission and can't easily be cancelled or recovered
/// from. This will open, then erase the contents of the file.
/// @throw This can throw any exception that the C++ filestream classes can throw.
void ClearTempFile()
{
    std::ofstream FileToClear;
    FileToClear.open(TempFile.c_str(),std::ios_base::out|std::ios_base::trunc); // Clear the work file.
    FileToClear.close();
}

/// @brief Attempts to delete TempFile. Silently fails if not possible.
void DeleteTempFile()
    { std::remove(TempFile.c_str()); }


/// @internal
/// @brief This aggregates the results of all the other test groups.
class AllUnitTestGroups : public UnitTestGroup
{
    private:
        /// @internal
        /// @brief So no one uses it
        void operator=(AllUnitTestGroups&)
            {}

        /// @internal
        /// @brief Should all tests be run.
        bool RunAll;

    public:

        /// @internal
        /// @brief This indicates we should run automatic tests rather than than launching a subprocess for that
        virtual void ShouldRunAutomaticTests()
        {
            RunAll = false;
            UnitTestGroup::ShouldRunAutomaticTests();
        }

        /// @internal
        /// @brief This indicates we should run interactive tests rather than than launching a subprocess for that
        virtual void ShouldRunInteractiveTests()
        {
            RunAll = false;
            UnitTestGroup::ShouldRunInteractiveTests();
        }

        /// @internal
        /// @brief Tells this whether or not it should be used to run any tests that child tests might want to spwan in a subprocess
        virtual void ShouldRunSubProcessTests()
        {
            RunAll = false;
            UnitTestGroup::ShouldRunSubProcessTests();
        }

        /// @internal
        /// @brief Used to signal
        virtual void ShouldRunAllTests()
            { RunAll = true; }
        /// @internal
        /// @brief Used to signal that none or only a subset of tests should run
        virtual void DontRunAllTests()
            { RunAll = false; }

        /// @internal
        /// @brief A collection of all the test groups
        GlobalCoreTestGroup& TestGroups;

        /// @internal
        /// @brief Constructor
        /// @param GlobalTestGroups The collection of tests that could be run
        AllUnitTestGroups(GlobalCoreTestGroup& GlobalTestGroups)
            : RunAll(true),
              TestGroups(GlobalTestGroups)
        {}

        /// @internal
        /// @brief When determining what tests to run the name are aggregated here
        std::vector<Mezzanine::String> TestGroupsToRun;           //List of tests to run

    private:
        /// @internal
        /// @brief This is used when the passed flags at the command prompt or when a master process is executing a single tests.
        void ExecuteSubTest()
        {
            for(std::vector<Mezzanine::String>::iterator CurrentTestName=TestGroupsToRun.begin(); CurrentTestName!=TestGroupsToRun.end(); ++CurrentTestName ) // Actually run the tests
            {
                try{
                    if(DoInteractiveTest)
                        { TestGroups[*CurrentTestName]->ShouldRunInteractiveTests(); }
                    if(DoAutomaticTest)
                        { TestGroups[*CurrentTestName]->ShouldRunAutomaticTests(); }
                    TestGroups[*CurrentTestName]->RunTests();
                } catch (std::exception e) {
                    TestError << std::endl << e.what() << std::endl;
                    // maybe we should log or somehting.
                }

                (*this) += *(TestGroups[*CurrentTestName]);
            }
        }

        /// @internal
        /// @brief this is used when there are test to execute and we need to loop over them and run each in a child process.
        void IterateSubtests()
        {
            setvbuf (stdout, NULL, _IONBF, 0);
            for(std::vector<Mezzanine::String>::iterator CurrentTestName=TestGroupsToRun.begin(); CurrentTestName!=TestGroupsToRun.end(); ++CurrentTestName )
            {
                ClearTempFile();
                String SubprocessInvocation(CommandName + " " + *CurrentTestName + " " + MemSpaceArg + " " + Mezzanine::String(DoAutomaticTest?"automatic ":"") + Mezzanine::String(DoInteractiveTest?"interactive ":""));
                if(system(SubprocessInvocation.c_str()))   // Run a single unit test as another process
                {
                    printf("%s", (SubprocessInvocation+String(" - Failure")).c_str() );
                    this->AddTestResult(String("Process::" + *CurrentTestName), Testing::Failed);
                }else {
                    printf("%s", (SubprocessInvocation+String(" - Success")).c_str() );
                    this->AddTestResult(String("Process::" + *CurrentTestName), Success);
                }

                try
                {
                    (*this) += GetResultsFromTempFile();
                } catch (std::exception& e) {
                    TestError << e.what() << endl;
                }

            }
            DeleteTempFile();
        }
    public:

        /// @internal
        /// @brief Determine which tests need to be run and run them
        virtual void RunTests()
        {
            if (DoAutomaticTest==DoInteractiveTest && DoInteractiveTest==false)   // enforce running automatic tests if no type of test is specified
                { DoAutomaticTest=true; }
            if(RunAll)
            {
                // if Runall is set it is presumed that no tests have been added to list of tests to run yet.
                for(map<String,UnitTestGroup*>::iterator Iter=TestGroups.begin(); Iter!=TestGroups.end(); ++Iter)
                    { TestGroupsToRun.push_back(Iter->first); }
            }

            if(MainProcess == GetCurrentProcessDepth())
            {
                IterateSubtests();
            }else{
                ExecuteSubTest();
            }
        } // \function

        /// @internal
        /// @brief Display the results either to the console or to the temp file for the main process to pick up.
        /// @param Output Where to present the output, this only works for the main process. Defaults to std::cout.
        /// @param Error A stream to send all errors to, defailts to std::cerr
        /// @param Summary Passed onto the UnitTests UnitTestGroup::DisplayResults if run from the main process.
        /// @param FullOutput Passed onto the UnitTests UnitTestGroup::DisplayResults if run from the main process.
        /// @param HeaderOutput Passed onto the UnitTests UnitTestGroup::DisplayResults if run from the main process.
        virtual void DisplayResults(std::ostream& Output=std::cout,
                                    std::ostream& Error=std::cerr,
                                    bool Summary = true,
                                    bool FullOutput = true,
                                    bool HeaderOutput = true)
        {
            //sleep_for(500000); // A half second sleep to allow subprocesses to finish output and not clobber stdout
            if(DoSubProcessTest) // we are running a test in a seperate process, so we need to control the output for communcation purposes
            {
                WriteTempFile(*this);
                UnitTestGroup::DisplayResults(Output, Error, Summary, FullOutput, HeaderOutput);
                //std::ofstream OutputFile(TempFile.c_str(),std::ios_base::out|std::ios_base::trunc);
                //UnitTestGroup::DisplayResults(OutputFile,false,true,false);
                //OutputFile.close();
            }else{
                UnitTestGroup::DisplayResults(Output, Error, Summary, FullOutput, HeaderOutput);
            }
        }
};

/// @brief Handles one argument at a time for main
/// @details This should be the only place that configures global state in the test suite
/// everywhere else should access global state through the accessors in mezztest.h
/// @param Arg The argument
void HandleSingleArgument(Mezzanine::String Arg)
{


}

/// @brief This is the entry point for the unit test executable.
/// @details This will contruct an AllUnitTestGroups with the listing of unit tests
/// available from autodetect.h. It will then interpret any command line arguments
/// and direct the created AllUnitTestGroups about which tests to run and how to run
/// them. In addition to sending the results to the standard output a copy of the
/// test results will be written to TestResults.txt, if not configure not to.
/// @n @n
/// If no arguments are passed this will add all the tests to the AllUnitTestGroups
/// and execute all tests that are not interactive. Print out a default report of them.
/// @return This will return EXIT_SUCCESS if the tests ran, even if some or all failed,
/// even if a child process segfaulted, but will return other statuses only if the main
/// process fails. If the main process cannot create child processes it will return EXIT_FAILURE.
/// @param argc Is interpretted as the amount of passed arguments
/// @param argv Is interpretted as the arguments passed in from the launching shell.
int main(int argc, char** argv)
{
    ArgC = argc;
    ArgV = argv;
    Depth = MainProcess;

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
        {
            Runner.ShouldRunSubProcessTests();
            Depth = TestSubSubProcess;
        }
        else if(ThisArg=="testlist")
            { return PrintList(TestGroups); }
        else if(ThisArg=="interactive")
            { Runner.ShouldRunInteractiveTests(); }
        else if(ThisArg=="automatic")
            { Runner.ShouldRunAutomaticTests(); }
        else if(ThisArg=="all")
            { Runner.ShouldRunAllTests(); }
        else if(ThisArg=="summary")
            { FullDisplay = false, SummaryDisplay = true; }
        else if(ThisArg=="skipfile")
            { WriteFile = false; }
        else  // Wasn't a command so it is either gibberish or a test group or debug test group
        {
            if(ThisArg.size() > SubTestPrefix.size())
            {
                if( ThisArg.substr(0,SubTestPrefix.size()) == SubTestPrefix)
                {
                    Depth = TestSubSubProcess;
                    ThisArg = ThisArg.substr(SubTestPrefix.size(), ThisArg.size()-SubTestPrefix.size());
                    GlobalCoreTestGroup::iterator SearchResult = TestGroups.find(ThisArg);
                    if(TestGroups.end()==SearchResult)
                    {
                        std::cerr << ThisArg << " appears to be a request to debug a sub-process that does not exist." << std::endl;
                        Usage(CommandName, TestGroups);
                        return ExitInvalidArguments;
                    }
                    if(!SearchResult->second->HasSubprocessTest())
                    {
                        std::cerr << ThisArg << " appears to be a request to debug a sub-process that does not have a sub process." << std::endl;
                        Usage(CommandName, TestGroups);
                        return ExitInvalidArguments;
                    }
                    SearchResult->second->ShouldRunSubProcessTests();
                }
            }
            GlobalCoreTestGroup::iterator Found = TestGroups.find(String(ThisArg.c_str()));
            if(Found != TestGroups.end())
            {
                Runner.DontRunAllTests();
                Runner.TestGroupsToRun.push_back(ThisArg.c_str());
            }else{
                if(GetCurrentProcessDepth() == TestSubSubProcess)
                {
                    SubSubProcessArgument = String(ThisArg);
                }else{
                    std::cerr << ThisArg << " is not a valid testgroup or parameter." << std::endl;
                    Usage(CommandName, TestGroups);
                    return ExitInvalidArguments;
                }
            }
        }
    }

    Runner.RunTests();

    if(WriteFile)
    {
        String FileName("TestResults.txt");
        std::ofstream OutFile(FileName.c_str());
        Runner.DisplayResults(OutFile, OutFile, SummaryDisplay, FullDisplay);
        OutFile.close();
    }
    if(MainProcess == GetCurrentProcessDepth())
        { Runner.DisplayResults(cout, cerr, SummaryDisplay, FullDisplay); }

    for(AllUnitTestGroups::iterator Iter = Runner.begin(); Iter!=Runner.end(); Iter++)
    {
        if(Iter->Results>Skipped)
            { return ExitFailure; }
    }
    return ExitSuccess;
 }
