//© Copyright 2010 BlackTopp Studios Inc.
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
#ifndef _main_h
#define _main_h

#include <utility> //For pair
#include <map>
#include <iostream>

#include <cctype> //for tolower

#include <physgame.h> // For String and all of physgame

// Define some Results of tests
enum TestResult
{
    Success         = 0,
    Skipped         = 1,
    Canceled        = 2,        // Was canceled by user, so success is unknown, but user doesn't cared
    Inconclusive    = 3,        // if a user answers that they don't know what happened in a test that involved interaction, it likely worked, but we can't be sure
    Failed          = 4,        // Known failure
    Unknown         = 5         // Since we don't know what happed this is the worst kind of failure
};

// the return type of tests
typedef std::pair<phys::String,TestResult> TestData;

// The classes for Tests themselves
// inherits from std::map to make storage location of of the TestData obvious
typedef std::map<phys::String,TestResult> TestDataStorage;
class UnitTest : public TestDataStorage
{
    protected:

        // Some basic variable for tracking simple statistics
        TestData MostSuccessful;
        TestData LeastSuccessful;

    public:
        UnitTest()
        {
            MostSuccessful=TestData("",Unknown);
            LeastSuccessful=TestData("",Success);
        }

        // This is expected to run all the tests that meet either of the criteria passed in
        // This should return the LeastSuccessful TestResult this will make it easier for the main to find and report errors
        virtual TestResult RunTests(bool RunAutomaticTests, bool RunInteractiveTests) = 0;

        // It is expected that tests will be inserted using these, this will automate tracking of the most and least successful tests
        void AddTestResult(TestData FreshMeat)
        {
            if(FreshMeat.second<MostSuccessful.second)
                { MostSuccessful=FreshMeat; }
            if(FreshMeat.second>LeastSuccessful.second)
                { LeastSuccessful=FreshMeat; }
            this->insert(FreshMeat);
        }

        // It is expected that every member of a class in physgame will be tested and its full name, include scoping operators, namespace,
        // class and function name will here (include argnames if required). Functions outside of classes should use their namespace, functionname
        // and arguments if required as the testname.
        // Example TestNames (The Fresh parameter)
        //      "phys::Vector2::Vector2(Real,Real)"     //Test of the Vector2 Constructor that accepts 2 reals
        //      "phys::Vector2::operator+"              //Test of only operator+ on Vector2
        //      "operator<<(ostream,Vector2)"           //Test of streaming operator for vector2 in root namespace
        void AddTestResult(const phys::String Fresh, TestResult Meat)
            { AddTestResult(TestData(Fresh,Meat)); }

        // make it a little easier to aggregate answers in one place
        const UnitTest& operator+=(const UnitTest& rhs)
        {
            if(rhs.MostSuccessful.second<MostSuccessful.second)
                { MostSuccessful=rhs.MostSuccessful; }
            if(rhs.MostSuccessful.second>LeastSuccessful.second)
                { LeastSuccessful=rhs.MostSuccessful; }
            insert(rhs.begin(),rhs.end());
        }

};

///Possible way to exit the UnitTest Program
enum ExitCodes
{
    ExitSuccess             = 0,
    ExitInvalidArguments    = 1
};

int Usage(phys::String ThisName)
{
    std::cout   << endl << "Usage: " << ThisName << " [[interactive][automatic]] | [all] Test Group Names ..." << endl << endl
                << "All:         All tests in all groups will be run." << endl
                << "Interactive: Only interactive tests will be performed on specified test groups." << endl
                << "Automatic:   Only automated tests will be performed on specified test groups." << endl << endl
                << "Interactive and Automatic: All tests will be run on specificied test groups." << endl
                << "If only test group names are entered, then all tests in those groups are run." << endl
                << "This command is not case sensitive." << endl << endl
                << "Current Test Groups: " << endl
                ;//<< \tVector2" <<endl;
    return ExitInvalidArguments;
}

char* AllLower(char* CString)
{
    locale loc;
    for(int c=0; CString[c]!='\0'; ++c)
    {
        CString[c]=tolower(CString[c],loc);
    }
    return CString;
}




#endif
