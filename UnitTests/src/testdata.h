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
#ifndef _testdata_h
#define _testdata_h

/// @file
/// @brief TestData, TestDataStorage and UnitTestGroup class definitions.

#include "mezzanine.h"

#include "testenumerations.h"

namespace Mezzanine
{
    namespace Testing
    {
        ///////////////////////////////////////////////////////////////////////////////////////////
        /// @brief The name of a test paired with the Results of a test.
        typedef std::pair<Mezzanine::String,TestResult> TestData;

        /// @brief Just a map to store the content of TestData, incidentally it will lexographically sort the list of tests.
        typedef std::map<Mezzanine::String,TestResult> TestDataStorage;

        // Forward declaration.
        class UnitTestGroup;
        /// @brief A group of testnames and the Actual test that implement those test(s).
        typedef std::map<Mezzanine::String, UnitTestGroup*> CoreTestGroup;

        /// @brief Trim the whitespace from a line of text and try to interpret the remains as TestResults and a testname
        /// @param Line A line of Test that starts with whitespace, then a TestResult String, then has a whitesapce delimiter and a ends witht eh name of test.
        /// @return A parsed TestData.
        TestData StringToTestData(Mezzanine::String Line);

        ///////////////////////////////////////////////////////////////////////////////////////////
        /// @brief A single group of tests, suitable for being all the tests of a small subsystem or single class.
        class UnitTestGroup : public TestDataStorage
        {
            protected:
                /// @brief Some basic variable for tracking simple statistics
                unsigned int LongestNameLength;

            public:
                /// @brief Default constructor
                UnitTestGroup();

        // make iterating over tests that could be possible
        // use to set skips and run

                /// @brief This is expected to run all the tests that meet the criteria passed in.
                /// @details All test results should be inserted using AddTestResult to allow the returning of results.
                virtual void RunTests(bool RunAutomaticTests, bool RunInteractiveTests) = 0;

                // It is expected that tests will be inserted using these, this will automate tracking of the most and least successful tests
                void AddTestResult(TestData FreshMeat, OverWriteResults Behavior=OverWriteIfLessSuccessful)
                {
                    bool Added=false;

                    TestDataStorage::iterator PreExisting = this->find(FreshMeat.first);
                    if(this->end()!=PreExisting)
                    {
                        switch(Behavior)
                        {
                            case OverWriteIfLessSuccessful:
                                if (PreExisting->second <= FreshMeat.second)
                                    { PreExisting->second = FreshMeat.second; Added=true; }
                                break;
                            case OverWrite:
                                PreExisting->second = FreshMeat.second;
                                Added=true;
                                break;
                            case OverWriteIfMoreSuccessful:
                                if (PreExisting->second >= FreshMeat.second)
                                    { PreExisting->second = FreshMeat.second; Added=true; }
                                break;
                            case DoNotOverWrite:
                                break;
                        }
                    }else{
                        this->insert(FreshMeat);
                        Added=true;
                    }

                    if (Added)
                    {
                        if(FreshMeat.first.length()>LongestNameLength)
                            { LongestNameLength=FreshMeat.first.length(); }
                    }
                }

                // It is expected that every member of a class in Mezzanine will be tested and its full name, include scoping operators, namespace,
                // class and function name will here (include argnames if required). Functions outside of classes should use their namespace, functionname
                // and arguments if required as the testname.
                // Example TestNames (The Fresh parameter)
                //      "Mezzanine::Vector2::Vector2(Real,Real)"     //Test of the Vector2 Constructor that accepts 2 reals
                //      "Mezzanine::Vector2::operator+"              //Test of only operator+ on Vector2
                //      "operator<<(ostream,Vector2)"           //Test of streaming operator for vector2 in root namespace
                void AddTestResult(const Mezzanine::String Fresh, TestResult Meat, OverWriteResults Behavior=OverWriteIfLessSuccessful)
                    { AddTestResult(TestData(Fresh,Meat),Behavior); }

                // make it a little easier to aggregate answers in one place
                const UnitTestGroup& operator+=(const UnitTestGroup& rhs)
                {
                    if(rhs.LongestNameLength>LongestNameLength)
                        { LongestNameLength=rhs.LongestNameLength; }

                    insert(rhs.begin(),rhs.end());
                }

                virtual void DisplayResults(std::ostream& Output=std::cout, bool Summary = true, bool FullOutput = true, bool HeaderOutput = true)
                {
                    std::vector<unsigned int> TestCounts; // This will store the counts of the Sucesses, failures, etc...
                    TestCounts.insert(TestCounts.end(),1+(unsigned int)Unknown, 0); //Fill with the exact amount of 0s

                    if(FullOutput && HeaderOutput) // No point in displaying the header without the other content.
                    {
                        Mezzanine::String TestName("Test Name");
                        Output << std::endl << " " << TestName << MakePadding(TestName, LongestNameLength) << "Result" << std::endl;
                    }

                    for (TestDataStorage::iterator Iter=this->begin(); Iter!=this->end(); Iter++)
                    {
                        if(FullOutput)
                        {
                            Output << Iter->first << MakePadding(Iter->first, LongestNameLength+1) << TestResultToString(Iter->second) << std::endl;
                        }
                        TestCounts.at((unsigned int)Iter->second)++; // Count this test result
                    }

                    if(Summary)
                    {
                        Output << std::endl << " Results Summary:" << std::endl;
                        for(unsigned int c=0; c<TestCounts.size();++c)
                        {
                            Mezzanine::String ResultName(TestResultToString((TestResult)c));
                            Output << "  " << ResultName << MakePadding(ResultName,16) << TestCounts.at(c) << std::endl;
                        }
                        Output << std::endl;
                    }
                }


                virtual bool AddSuccessFromBool(Mezzanine::String TestName, bool Condition)
                {
                    if(Condition)
                    {
                        AddTestResult(TestName, Success, UnitTestGroup::OverWrite);
                    }else{
                        AddTestResult(TestName, Failed, UnitTestGroup::OverWrite);
                    }
                    return Condition;
                }


        };



    }// Testing
}// Mezzanine

#endif
