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

using namespace Mezzanine;

namespace Mezzanine
{
    namespace Testing
    {
        int PrintList(CoreTestGroup& TestGroups)
        {
            for(CoreTestGroup::iterator Iter=TestGroups.begin(); Iter!=TestGroups.end(); ++Iter)
                { std::cout << Iter->first << std::endl; }
            return ExitSuccess;
        }

        TestData StringToTestData(Mezzanine::String Line)
        {
            TestData Results;
            size_t LastSpace=Line.rfind(' ');
            Results.second=StringToTestResult(Line.substr(LastSpace+1,100)); // No testdata should be longer than 100
            Results.first=rtrim(Line.substr(0,LastSpace));
            return Results;
        }

        UnitTestGroup::UnitTestGroup() :
            LongestNameLength(0)
        {}

        void UnitTestGroup::AddTestResult(TestData FreshMeat, OverWriteResults Behavior)
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

        void UnitTestGroup::AddTestResult(const Mezzanine::String Fresh, TestResult Meat, OverWriteResults Behavior)
        {
            std::cout << "Noting result of " << Fresh << " as " << TestResultToString(Meat) << std::endl;
            AddTestResult(TestData(Fresh,Meat),Behavior);
        }

        const UnitTestGroup& UnitTestGroup::operator+=(const UnitTestGroup& rhs)
        {
            if(rhs.LongestNameLength>LongestNameLength)
                { LongestNameLength=rhs.LongestNameLength; }

            insert(rhs.begin(),rhs.end());
            return *this;
        }

        void UnitTestGroup::DisplayResults(std::ostream& Output, bool Summary, bool FullOutput, bool HeaderOutput)
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

        bool UnitTestGroup::AddSuccessFromBool(Mezzanine::String TestName, bool Condition)
        {
            if(Condition)
            {
                AddTestResult(TestName, Success, OverWrite);
            }else{
                AddTestResult(TestName, Failed, OverWrite);
            }
            return Condition;
        }


    }// Testing
}// Mezzanine

#endif
_testdata_cpp
