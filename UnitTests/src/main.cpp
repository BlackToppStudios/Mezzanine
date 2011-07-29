//© Copyright 2010 - 2011 BlackTopp Studios Inc.
/* This file is part of The PhysGame Engine.

    The PhysGame Engine is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The PhysGame Engine is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with The PhysGame Engine.  If not, see <http://www.gnu.org/licenses/>.
*/
/* The original authors have included a copy of the license specified above in the
   'Docs' folder. See 'gpl.txt'
*/
/* We welcome the use of The PhysGame anyone, including companies who wish to
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

#include "main.h"

#include "actortests.h"
#include "compilerflagtests.h"
#include "eventmanagertests.h"
#include "particleeffecttests.h"
#include "scenemanagertests.h"
#include "vector2tests.h"
#include "vector3tests.h"
#include "worldnodetests.h"

#include <cstdlib>

#include <vector>

// this does all the heavy lifting
class AllUnitTests : public UnitTest
{
    public:
        AllUnitTests():RunAll(false)                    //constructor, just to set a bool ;)
        {};

        vector<phys::String> TestGroupsToRun;           //List of tests to run
        bool RunAll;

        virtual TestResult RunTests(bool RunAutomaticTests, bool RunInteractiveTests)
        {
            TestResult LowResults = Unknown;
            if(RunAll)
            {                                   //Run every test in the big list of tests
                for(map<String,UnitTest*>::iterator Iter=TestGroups.begin(); Iter!=TestGroups.end(); ++Iter)
                {
                    TestResult temp = Iter->second->RunTests(RunAutomaticTests, RunInteractiveTests);
                    if (temp < LowResults)
                        { LowResults = temp; }
                    (*this) += *(Iter->second);
                }
            }else{                              // run the tests one at a time in the list of things to run by finding them in the big list
                for(vector<phys::String>::iterator CurrentTestName=TestGroupsToRun.begin(); CurrentTestName!=TestGroupsToRun.end(); ++CurrentTestName )
                {
                    TestResult temp = TestGroups[*CurrentTestName]->RunTests(RunAutomaticTests, RunInteractiveTests);
                    if (temp < LowResults)
                        { LowResults = temp; }
                    (*this) += *(TestGroups[*CurrentTestName]);
                }
            }
            return LowResults;
        }
};

void DeleteTests()
{
    for(map<String,UnitTest*>::iterator Iter=TestGroups.begin(); Iter!=TestGroups.end(); ++Iter)
    {
        //cout << Iter->first << endl;
        delete Iter->second;
    }
}

int main (int argc, char** argv)
{
    atexit(&DeleteTests);
    // This is the complete group of all Unit tests, when adding the header for a unit test it should be added here
    TestGroups["actor"] = new ActorTests;
    TestGroups["compilerflag"] = new CompilerFlagTests;
    TestGroups["eventmanager"] = new EventManagerTests;
    TestGroups["particleeffect"] = new ParticleEffectTests;
    TestGroups["scenemanager"] = new SceneManagerTests;
    TestGroups["vector2"] = new Vector2Tests;
    TestGroups["vector3"] = new Vector3Tests;
    TestGroups["worldnode"] = new WorldNodeTests;

    bool RunAutomaticTests = false, RunInteractiveTests = false;    //Set them both to false now, if they are both false later, then we will pass true
    bool FullDisplay = true, SummaryDisplay = true;
    phys::String CommandName;

    if (argc > 0)                                                   //Not really sure how this would happen, but I would rather test for it than have it fail
        { CommandName=argv[0]; }
    else
        { return Usage("UnitTests"); }

    if (argc == 1)
        { return Usage(CommandName); }

    AllUnitTests Runner;

    for (int c=1; c<argc; ++c)                                                  // Check Command line for keywords and get all the test names
    {
        String ThisArg(AllLower(argv[c]));
        if(ThisArg=="help")
            { Usage(argv[0]); return ExitSuccess; }
        else if(ThisArg=="interactive")
            { RunInteractiveTests=true; }
        else if(ThisArg=="automatic")
            { RunAutomaticTests=true; }
        else if(ThisArg=="all")
            { Runner.RunAll=true; }
        else if(ThisArg=="summary")
            { FullDisplay = false, SummaryDisplay = true; }
        else                                                                    // Wasn't a command so it is either gibberish or a test group
        {
            if(TestGroups[ThisArg.c_str()])                                     //pointer will be null if gibberish
            {
                Runner.TestGroupsToRun.push_back(AllLower(argv[c]));
            }else{
                cerr << ThisArg << " is not a valid testgroup or parameter." << endl;
                return ExitInvalidArguments;
            }
        }
    }

    if (RunAutomaticTests==RunInteractiveTests && RunInteractiveTests==false)   // enforce running all tests if no type of test is specified
        { RunAutomaticTests=true; RunInteractiveTests=true; }

    Runner.RunTests(RunAutomaticTests,RunInteractiveTests);
    Runner.DisplayResults(SummaryDisplay, FullDisplay);

    return ExitSuccess;
 }


