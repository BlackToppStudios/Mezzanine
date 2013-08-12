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
#ifndef _testdata_cpp
#define _testdata_cpp

/// @file
/// @brief The definition of the string manipulation functions the unit tests use

#include "testdata.h"
#include "consolestringmanipulation.h"

#include <vector>
#include <stdexcept>
#include <sstream>

using namespace Mezzanine;
using namespace std;

namespace Mezzanine
{
    namespace Testing
    {

        TestData::TestData(const String& Name,
                 TestResult Result,
                 const String& FuncName,
                 const String& File,
                 Mezzanine::Whole Line)
            : TestName(Name), Results(Result), FunctionName(FuncName), FileName(File), LineNumber(Line)
        {}

        TestData::TestData(pugi::xml_node Node)
            : TestName( Node.attribute("TestName").as_string() ),
              Results( StringToTestResult(Node.attribute ("Results").as_string())),
              FunctionName( Node.attribute("FunctionName").as_string() ),
              FileName( Node.attribute("FileName").as_string() ),
              LineNumber( Node.attribute("LineNumber").as_uint() )
        {}

        bool TestData::operator<(const TestData& Rhs) const
            { return this->TestName < Rhs.TestName; }

        String TestData::GetAsXML() const
        {
            std::stringstream Snippet;
            Snippet << "<TestData "
                    << "TestName=\"" << TestName << "\" "
                    << "Results=\"" << TestResultToString(Results) << "\" "
                    << "FunctionName=\"" << FunctionName << "\" "
                    << "FileName=\"" << FileName << "\" "
                    << "LineNumber=\"" << LineNumber << "\" "
                    << "/>";
            return Snippet.str();
        }

        int PrintList(CoreTestGroup& TestGroups)
        {
            for(CoreTestGroup::iterator Iter=TestGroups.begin(); Iter!=TestGroups.end(); ++Iter)
                { cout << Iter->first << std::endl; }
            return ExitSuccess;
        }

        TestData StringToTestData(Mezzanine::String Line)
        {
            TestData Results;
            size_t LastSpace=Line.rfind(' ');
            Results.Results = StringToTestResult(Line.substr(LastSpace+1));
            Results.TestName=rtrim(Line.substr(0,LastSpace));
            return Results;
        }

        UnitTestGroup::UnitTestGroup() :
            LongestNameLength(0)
        {}

        void UnitTestGroup::RunTests(bool RunAuto, bool RunInteractive)
        {
            try
            {
                if(RunAuto)
                    { RunAutomaticTests(); }
                else if(HasAutomaticTests())
                    { AddTestResult( TestData("AutomaticTests",Testing::Skipped, "RunTests") );}
            }catch(exception& e){
                cerr << "Caught an unhandled exception while doing RunAutomaticTests()." << endl
                     << "Message: " << e.what() << endl;
                AddTestResult( TestData("UnhandledException", Testing::Failed) );
            }

            try
            {
                if(RunInteractive)
                    { RunInteractiveTests(); }
                else if(HasInteractiveTests())
                    { AddTestResult( TestData("InteractiveTests",Testing::Skipped, "RunTests") );}
            }catch(exception& e){
                cerr << "Caught an unhandled exception while doing RunInteractiveTests()." << endl
                     << "Message: " << e.what() << endl;
                AddTestResult( TestData("UnhandledException", Testing::Failed) );
            }
        }

        void UnitTestGroup::RunAutomaticTests()
        {

        }

        bool UnitTestGroup::HasAutomaticTests() const
            { return false; }

        void UnitTestGroup::RunInteractiveTests()
        {

        }

        bool UnitTestGroup::HasInteractiveTests() const
            { return false; }

        Mezzanine::String UnitTestGroup::Name()
            { return ""; }


        void UnitTestGroup::AddTestResult(TestData CurrentTest, OverWriteResults Behavior)
        {
            bool Added=false;

            if(CurrentTest.TestName.npos != CurrentTest.TestName.find(" "))
                { throw std::invalid_argument("Invalid Test Name, contains one or more space character ( ), TestName: \"" + CurrentTest.TestName + "\""); }
            if(CurrentTest.TestName.npos != CurrentTest.TestName.find("\""))
                { throw std::invalid_argument("Invalid Test Name, contains one or more double quote (\") character(s), TestName: \"" + CurrentTest.TestName + "\""); }

            if(this->Name().length())
            {
                if(this->Name().npos != this->Name().find(" "))
                    { throw std::invalid_argument("Invalid UnitTestGroup Name, contains one or more space character ( ), name: \"" + this->Name() + "\""); }
                if(this->Name().npos != this->Name().find("\""))
                    { throw std::invalid_argument("Invalid UnitTestGroup Name, contains one or more double quote (\"), name: \"" + this->Name() + "\""); }
                CurrentTest.TestName = this->Name() + "::" + CurrentTest.TestName;
            }


            TestDataStorage::iterator PreExisting = this->find(CurrentTest.TestName);
            if(this->end()!=PreExisting)
            {
                switch(Behavior)
                {
                    case OverWriteIfLessSuccessful:
                        if (PreExisting->Results <= CurrentTest.Results)
                        {
                            this->erase(PreExisting);
                            this->insert(CurrentTest);
                            Added=true;
                        }
                        break;
                    case OverWrite:
                        this->erase(PreExisting);
                        this->insert(CurrentTest);
                        Added=true;
                        break;
                    case OverWriteIfMoreSuccessful:
                        if (PreExisting->Results >= CurrentTest.Results)
                        {
                            this->erase(PreExisting);
                            this->insert(CurrentTest);
                            Added=true;
                        }
                        break;
                    case DoNotOverWrite:
                        break;
                }
            }else{
                this->insert(CurrentTest);
                Added=true;
            }

            if (Added)
            {
                if(CurrentTest.TestName.length()>LongestNameLength)
                    { LongestNameLength=CurrentTest.TestName.length(); }
            }
        }

        void UnitTestGroup::AddTestResult(const Mezzanine::String Fresh, TestResult Meat, OverWriteResults Behavior)
        {
            std::cout << "Noting result of " << this->Name() + "::" + Fresh << " as " << TestResultToString(Meat) << std::endl;
            AddTestResult(TestData(Fresh,Meat),Behavior);
        }

        const UnitTestGroup& UnitTestGroup::operator+=(const UnitTestGroup& rhs)
        {
            if(rhs.LongestNameLength>LongestNameLength)
                { LongestNameLength=rhs.LongestNameLength; }

            insert(rhs.begin(),rhs.end());
            return *this;
        }

        void UnitTestGroup::AddTestsFromXML(pugi::xml_node Node)
        {
            if(!Node)
            {
                throw std::invalid_argument(
                        String("UnitTestGroup::AddTestsFromXML can only handle XML but was passed an empty file. Expected results from ")
                        + Node.name()
                    );
            }
            if(String("UnitTestGroup")!=Node.name())
            {
                throw std::invalid_argument(
                        String("UnitTestGroup::AddTestsFromXML can only handle XML with \"UnitTestGroup\" as passed element.  Expected results from ")
                        + Node.name()
                    );
            }
            for(pugi::xml_node::iterator Iter = Node.begin(); Iter!=Node.end(); Iter++)
                { this->AddTestResult(TestData(*Iter)); }
        }

        String UnitTestGroup::GetAsXML() const
        {
            String Results("<UnitTestGroup>");
            for (iterator Iter=this->begin(); Iter!=this->end(); Iter++)
                { Results += "\n  " + Iter->GetAsXML(); }
            Results += "\n</UnitTestGroup>";
            return Results;
        }

        void UnitTestGroup::DisplayResults(std::ostream& Output, std::ostream& Error, bool Summary, bool FullOutput, bool HeaderOutput)
        {
            std::vector<unsigned int> TestCounts; // This will store the counts of the Sucesses, failures, etc...
            TestCounts.insert(TestCounts.end(),1+(unsigned int)NotApplicable, 0); //Fill with the exact amount of 0s

            if(FullOutput && HeaderOutput) // No point in displaying the header without the other content.
            {
                Mezzanine::String TestName("Test Name");
                Output << std::endl << " " << TestName << MakePadding(TestName, LongestNameLength) << "Result" << std::endl;
            }

            for (TestDataStorage::iterator Iter=this->begin(); Iter!=this->end(); Iter++)
            {
                if(FullOutput)
                {
                    Output << Iter->TestName << MakePadding(Iter->TestName, LongestNameLength+1) << TestResultToString(Iter->Results);
                    if(Iter->Results) // Not Testing::Success
                    {
                        Output << "\t";
                        if(Iter->FileName.length())
                            { Output << " File: " << Iter->FileName; }
                        if(Iter->FunctionName.length())
                            { Output << " Function: " << Iter->FunctionName; }
                        if(Iter->LineNumber)
                            { Output << " Line: " << Iter->LineNumber; }
                        if(Iter->FileName.length()==0 && Iter->FunctionName.length() == 0 && Iter->LineNumber==0)
                            { Output << " No Metadata available able issue, use TEST to capture"; }
                    }
                    Output << endl;
                }

                if (Iter->Results && Iter->FileName.length() && Iter->FunctionName.length() && Iter->LineNumber)
                {
                    Error << Iter->FileName << ":" << Iter->LineNumber
                          << " Test " << TestResultToString(Iter->Results)
                          << " in function " << Iter->FunctionName << endl;
                }
                TestCounts.at((unsigned int)Iter->Results)++; // Count this test result
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

        void UnitTestGroup::Test(bool TestCondition, const String& TestName, TestResult IfFalse, TestResult IfTrue, const String& FuncName, const String& File, Whole Line )
        {
            try
            {
                if(TestCondition)
                {
                    AddTestResult( TestData(TestName, IfTrue, FuncName, File, Line) );
                }else{
                    AddTestResult( TestData(TestName, IfFalse, FuncName, File, Line) );
                }
            }catch(exception& e){
                cerr << "Caught an unhandled exception while adding results for " << TestName << endl
                     << "Message: " << e.what() << endl;
                AddTestResult( TestData("UnhandledException", Testing::Failed, FuncName, File, Line) );
            }
        }


    }// Testing
}// Mezzanine

#endif // \ _testdata_cpp
