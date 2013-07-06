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

#include "datatypes.h"

#include "testenumerations.h"

#include <map>
#include <iostream>

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

        /// @brief Print all the groups that exist in a given CoreTestGroup
        /// @param TestGroups The group whose constents names with be printed
        /// @return ExitSuccess on success.
        int PrintList(CoreTestGroup &TestGroups);

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
                /// @param RunAutomaticTests True if the automatic tests should be run false if they should
                /// @param RunInteractiveTests True if the interactive tests should run false otherwise/.RunInteractiveTests
                /// @note One of two methods that must be implmented on a UnitTestGroup
                virtual void RunTests(bool RunAutomaticTests, bool RunInteractiveTests)
                    {}

                /// @brief Get Name of this UnitTestGroup
                /// @return The string that must be type at the command line to run this testgroup, should be all lowercase.
                /// @note One of two methods that must be implmented on a UnitTestGroup
                virtual Mezzanine::String Name()
                    { return String(); }

                /// @brief Its expected that tests will be inserted using this
                /// @details This will automate tracking of the most and least successful tests
                /// @param FreshMeat The New test results and name
                /// @param Behavior An OverWriteResults that defines the overwirte behavior of this function, defaults to OverWriteIfLessSuccessful
                void AddTestResult(TestData FreshMeat, OverWriteResults Behavior=OverWriteIfLessSuccessful);

                /// @brief Add a test results without having to to construct a TestData first
                /// @details It is expected that every member of a class in Mezzanine will be tested and its full name, include scoping operators, namespace,
                /// class and function name will here (include argnames if required). Functions outside of classes should use their namespace, functionname
                /// and arguments if required as the testname.
                /// Example TestNames (The Fresh parameter)
                ///      "Mezzanine::Vector2::Vector2(Real,Real)"     //Test of the Vector2 Constructor that accepts 2 reals
                ///      "Mezzanine::Vector2::operator+"              //Test of only operator+ on Vector2
                ///      "operator<<(ostream,Vector2)"           //Test of streaming operator for vector2 in root namespace
                /// @param Fresh The name of the Test
                /// @param Meat The actual TestResult
                /// @param Behavior An OverWriteResults that defines the overwirte behavior of this function, defaults to OverWriteIfLessSuccessful
                void AddTestResult(const Mezzanine::String Fresh, TestResult Meat, OverWriteResults Behavior=OverWriteIfLessSuccessful);

                /// @brief Add all the items in another UnitTestGroup to this one
                /// @param rhs The item on the right hand side of the +=.
                /// @return A reference to this is returned allowiong operator chaining.
                const UnitTestGroup& operator+=(const UnitTestGroup& rhs);

                /// @brief Print the results or save them to a file.
                /// @param Output the stream to send the results to.
                /// @param Summary Print Statistics at the end, not needed when sending results between processes. Defaults to true/enabled.
                /// @param FullOutput Sometimes the user does not want to see each test results and just wants a little blurb. Defaults to true/enabled.
                /// @param HeaderOutput makes the output a little more understandif it is short or needs to be copied into a spreadsheet. Defaults to true/enabled.
                virtual void DisplayResults(std::ostream& Output=std::cout, bool Summary = true, bool FullOutput = true, bool HeaderOutput = true);

                /// @brief Convert a Bool to an added test.
                /// @param TestName The name of the Test.
                /// @param Condition if false converted to Failed and if true converted Success.
                /// @return The value passed in as the condition.
                virtual bool AddSuccessFromBool(Mezzanine::String TestName, bool Condition);
        };



    }// Testing
}// Mezzanine

#endif
