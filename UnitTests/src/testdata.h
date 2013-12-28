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
#ifndef _testdata_h
#define _testdata_h

/// @file
/// @brief TestData, TestDataStorage and UnitTestGroup class definitions.

#include "datatypes.h"

#include "testenumerations.h"
#include "pugixml.h"

#include <set>
#include <map>
#include <iostream>
#include <sstream>

namespace Mezzanine
{
    namespace Testing
    {
        ///////////////////////////////////////////////////////////////////////////////////////////
        /// @brief The information about a test and how to easily find it in the filesystem
        struct TestData
        {
            /// @brief The name of a given test
            Mezzanine::String TestName;
            /// @brief How did the test turn out
            TestResult Results;
            /// @brief The function the test was called from
            Mezzanine::String FunctionName;
            /// @brief The File The test happened in
            Mezzanine::String FileName;
            /// @brief What line in the file this test occurred when the test was compiled
            Mezzanine::Whole LineNumber;

            /// @brief Create a TestData
            /// @param Name the name of the test, defaults to "".
            /// @param Result A TestResult, defaults to Testing::Success.
            /// @param FuncName The name of the function this test was called from, Defaults to "".
            /// @param File The name of the file in which the test exists, Defaults to "".
            /// @param Line The line in the file in which the test exists, Defaults to 0.
            TestData(const String& Name = "",
                     TestResult Result = Testing::Success,
                     const String& FuncName = "",
                     const String& File = "",
                     Mezzanine::Whole Line = 0);

            /// @brief Create Test data from xml.
            /// @param Node The XMl node to create this from
            TestData(pugi::xml_node Node);

            /// @brief Used to sort TestData in std::std and other sorted containers, by TestName.
            /// @param Rhs the right hand operand when using the less than comparison operator.
            /// @return A bool with the same value as this->TestName < Rhs.TestName.
            bool operator<(const TestData& Rhs) const;

            /// @brief Return a snippet of xml describing this TestData
            /// @return A String with a single XML element with an attribute for each of the TestName, Results, FunctionName, FileName, LineNumber
            String GetAsXML() const;
        };

        /// @brief Just a map to store the content of TestData, incidentally it will lexographically sort the list of tests.
        typedef std::set<TestData> TestDataStorage;

        // Forward declaration.
        class UnitTestGroup;

        /// @brief A group of testnames and the Actual test that implement those test(s).
        typedef std::map<Mezzanine::String, UnitTestGroup*> CoreTestGroup;

        /// @brief Print all the groups that exist in a given CoreTestGroup
        /// @param TestGroups The group whose constents names with be printed
        /// @return ExitSuccess on success.
        int PrintList(CoreTestGroup &TestGroups);

        /// @brief Trim the whitespace from a line of text and try to interpret the remains as TestResults and a testname
        /// @param Line A line of Test that starts with whitespace, then a TestResult String, then has a whitesapce delimiter and a ends with the name of test.
        /// @return A parsed TestData.
        TestData StringToTestData(Mezzanine::String Line);

        ///////////////////////////////////////////////////////////////////////////////////////////
        /// @brief A single group of tests, suitable for being all the tests of a small subsystem or single class.
        class UnitTestGroup : public TestDataStorage
        {
            protected:
                /// @brief A destination for all normal ouput in the tests.
                std::stringstream TestOutput;

                /// @brief A destination for errors
                std::stringstream TestError;

                /// @brief Some basic variable for tracking simple statistics
                unsigned int LongestNameLength;

            public:
                /// @brief Default constructor
                UnitTestGroup();

                /// @brief Copy constructor
                /// @param OtherGroup A UnitTestGroup to copy into this one. The contents of any log streams are copied and the streams themselves.
                UnitTestGroup(const UnitTestGroup& OtherGroup);

                /// @brief This will call RunAutomaticTests based on the values passed.
                /// @details All test results should be inserted using AddTestResult to allow the returning of results.
                /// @n @n This can be overloaded to enable better detection of skipped tests. This niavely reports only
                /// "TestName::Interactive" and "TestName::Automatic" as skipped, and even then only if HasAutomaticTests
                /// or HasInteractiveTests return true.
                /// @param RunAutomaticTests True if the automatic tests should be run false if they should
                /// @param RunInteractiveTests True if the interactive tests should run false otherwise/.RunInteractiveTests
                virtual void RunTests(bool RunAuto, bool RunInteractive);

                /// @brief This should be overloaded to run all tests that do require not user interaction
                virtual void RunAutomaticTests();

                /// @brief Used only to report skipped tests.
                /// @return Defaults to returning false, but should be overloaded to return true if RunAutomaticTests() implements any tests.
                virtual bool HasAutomaticTests() const;

                /// @brief This should be overloaded to run all tests require user interaction
                virtual void RunInteractiveTests();

                /// @brief Used only to report skipped tests.
                /// @return Defaults to returning false, but should be overloaded to return true if RunInteractiveTests() implements any tests.
                virtual bool HasInteractiveTests() const;

                /// @brief Get Name of this UnitTestGroup
                /// @return The string that must be type at the command line to run this testgroup, should be all lowercase.
                /// @note One of two methods that must be implmented on a UnitTestGroup
                virtual Mezzanine::String Name();

                /// @brief Its expected that tests will be inserted using this
                /// @details This will automate tracking of the most and least successful tests
                /// @param FreshMeat The New test results and name
                /// @param Behavior An OverWriteResults that defines the overwirte behavior of this function, defaults to OverWriteIfLessSuccessful
                void AddTestResult(TestData CurrentTest, OverWriteResults Behavior=OverWriteIfLessSuccessful);

                /// @brief Add a test results without having to to construct a TestData first
                /// @details This prepends the name of this UnitTestGroup and "::" to the
                /// @warning The name of the test can have no spaces in it. An exception will be thrown if found.
                /// @param TestName The name of the Test
                /// @param TResult The actual TestResult
                /// @param Behavior An OverWriteResults that defines the overwirte behavior of this function, defaults to OverWriteIfLessSuccessful
                void AddTestResult(const Mezzanine::String TestName, TestResult TResult, OverWriteResults Behavior=OverWriteIfLessSuccessful);

                /// @brief Add all the items in another UnitTestGroup to this one
                /// @param rhs The item on the right hand side of the +=.
                /// @return A reference to this is returned allowiong operator chaining.
                const UnitTestGroup& operator+=(const UnitTestGroup& rhs);

                /// @brief Create and add all the tests in a given piece of parsed xml
                /// @param Node A pugi::xml_node referencing a UnitTestGroup
                void AddTestsFromXML(pugi::xml_node Node);

                /// @brief Get the Whole UnitTestGroup as a valid XML document
                /// @details The root element of the XMl document is named UnitTestGroup
                /// and it will contain the XML from each TestData this contains
                /// @return A String containing some XML
                String GetAsXML() const;

                /// @brief Print the results or save them to a file.
                /// @param Output the stream to send the results to.
                /// @param Error A stream to send errors to.
                /// @param Summary Print Statistics at the end, not needed when sending results between processes. Defaults to true/enabled.
                /// @param FullOutput Sometimes the user does not want to see each test results and just wants a little blurb. Defaults to true/enabled.
                /// @param HeaderOutput Makes the output a little more understandable it is short or needs to be copied into a spreadsheet. Defaults to true/enabled.
                virtual void DisplayResults(std::ostream& Output=std::cout, std::ostream& Error=std::cerr, bool Summary = true, bool FullOutput = true, bool HeaderOutput = true);

                /// @brief Interpret Boolean value as a test result. Also Prepends the name of the current test, as returned by Name() + "::", to ease test scoping.
                /// @warning IfFalse comes first in the argument list, This is because the common test cases have IfTrue = Testing::Success while IfFalse makes sense as other things
                /// @param TestCondition The test itself or the results of it.
                /// @param TestName The comple name of the test, used to track results.
                /// @param IfFalse Defaults to Testing::Failed but can be whatever Testing::TestResult you want if a false passed as the TestCondition.
                /// @param IfTrue Defaults to Testing::Success but can be whatever Testing::TestResult you want if a true passed as the TestCondition.
                /// @param FuncName The function the test was called from, if blank
                virtual void Test(bool TestCondition, const String& TestName, TestResult IfFalse = Testing::Failed, TestResult IfTrue = Testing::Success,
                                  const String& FuncName = "", const String& File = "", Mezzanine::Whole Line = 0);
        };



    }// Testing
}// Mezzanine

#endif
