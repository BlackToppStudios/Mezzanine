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

#include "compilerflagtests.h"
#include "eventmanagertests.h"
#include "vector2tests.h"
#include "vector3tests.h"


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
            vector<phys::String>::iterator CurrentTestName;

            // If we want to run all the tests then we don't really need to iterate do we? if somehow all those
            // pesky breaks could be skipped we could just jump into the switch case and just run all the tests
            if(RunAll)
                { goto TheGauntlet; }        // yup, I used a goto

            for(CurrentTestName=TestGroupsToRun.begin(); CurrentTestName!=TestGroupsToRun.end(); ++CurrentTestName )
            {
                //add to the big switch case whenever you add a new UnitTest please.
                switch ((*CurrentTestName)[0])
                {
                    TheGauntlet:        //can you think of a better name for something that means all the tests will be run
                    case 'c':
                        if(RunAll || (*CurrentTestName)=="compilerflag")
                        {
                            CompilerFlagTests CompilerFlags_;
                            CompilerFlags_.RunTests(RunAutomaticTests, RunInteractiveTests);
                            (*this)+=CompilerFlags_;
                        }
                        if(!RunAll) break;

                    case 'e':
                        if(RunAll || (*CurrentTestName)=="eventmanager")
                        {
                            EventManagerTests EventManagerTests_;
                            EventManagerTests_.RunTests(RunAutomaticTests, RunInteractiveTests);
                            (*this)+=EventManagerTests_;
                        }
                        if(!RunAll) break;

                    case 'v':
                        if(RunAll || (*CurrentTestName)=="vector2")
                        {
                            Vector2Tests Vector2_;
                            Vector2_.RunTests(RunAutomaticTests, RunInteractiveTests);
                            (*this)+=Vector2_;
                        }
                        if(RunAll || (*CurrentTestName)=="vector3")
                        {
                            Vector3Tests Vector3_;
                            Vector3_.RunTests(RunAutomaticTests, RunInteractiveTests);
                            (*this)+=Vector3_;
                        }
                        if(!RunAll) break;

                    case 'y':
                        if(!RunAll) break;

                    case 'z':
                        break;
                }

                if(RunAll) // We Finished the Gauntlet We can Quit now
                    break;
            }

            return LeastSuccessful.second;
        }
};

int main (int argc, char** argv)
{
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
        if(phys::String(AllLower(argv[c]))=="help")
            { Usage(argv[0]); return ExitSuccess; }
        else if(phys::String(AllLower(argv[c]))=="interactive")
            { RunInteractiveTests=true; }
        else if(phys::String(AllLower(argv[c]))=="automatic")
            { RunAutomaticTests=true; }
        else if(phys::String(AllLower(argv[c]))=="all")
            { Runner.RunAll=true; }
        else if(phys::String(AllLower(argv[c]))=="summary")
            { FullDisplay = false, SummaryDisplay = true; }
        else
            { Runner.TestGroupsToRun.push_back(AllLower(argv[c])); }            // Testing group is filled here
    }

    if (RunAutomaticTests==RunInteractiveTests && RunInteractiveTests==false)   // enforce running all all test if no type of test is specified
        { RunAutomaticTests=true; RunInteractiveTests=true; }

    Runner.RunTests(RunAutomaticTests,RunInteractiveTests);
    Runner.DisplayResults(SummaryDisplay, FullDisplay);
}





