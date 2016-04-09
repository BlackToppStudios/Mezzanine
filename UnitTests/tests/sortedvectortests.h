// © Copyright 2010 - 2016 BlackTopp Studios Inc.
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
#ifndef sortedvectortests_h
#define sortedvectortests_h

#include "mezztest.h"

#include "sortedvector.h"

/// @file
/// @brief Test the sorted vector works

using namespace Mezzanine;
using namespace Mezzanine::Testing;

/// @brief Test
class sortedvectortests : public UnitTestGroup
{
public:
    /// @copydoc Mezzanine::Testing::UnitTestGroup::Name
    /// @return Returns a String containing "SortedVector"
    virtual String Name()
        { return String("SortedVector"); }

    /// @brief Test most features on the SortedVector.
    void RunAutomaticTests()
    {
        {
            std::vector<int> Sorted;
            Sorted.push_back(1);
            Sorted.push_back(2);
            Sorted.push_back(3);
            TEST( binary_find(Sorted.begin(),Sorted.end(),1) == Sorted.begin(),    "BinaryFind1");
            TEST( binary_find(Sorted.begin(),Sorted.end(),2) == Sorted.begin()+1,  "BinaryFind2");
            TEST( binary_find(Sorted.begin(),Sorted.end(),3) == Sorted.end()-1,    "BinaryFind3");
        }

        {
            SortedVector<Int32> sample;
            TEST(sample.size() == 0, "SortedVector.Size1");
        }

        {
            SortedVector<Int32> sample;
            sample.add(5);
            TEST(sample.size() == 1, "SortedVector.Size2");
            TEST( *(sample.begin()) == 5, "SortedVector.BeginDerefenced1");
        }

        {
            SortedVector<Int32> sample;
            sample.add(13);
            sample.add(5);
            sample.add(8);
            TEST(sample.size() == 3,                        "SortedVector.Size3");
            TEST( *(sample.begin()) == 5,                   "SortedVector.SortedValue1Begin");
            TEST( *(sample.begin()+1) == 8,                 "SortedVector.SortedValue2Begin");
            TEST( *(sample.begin()+2) == 13,                "SortedVector.SortedValue3Begin");
            TEST( *(sample.end()-3) == 5,                   "SortedVector.SortedValue1End");
            TEST( *(sample.end()-2) == 8,                   "SortedVector.SortedValue2End");
            TEST( *(sample.end()-1) == 13,                  "SortedVector.SortedValue3End");
            TEST( sample[0] == 5,                           "SortedVector.SortedValue0Bracket");
            TEST( sample[1] == 8,                           "SortedVector.SortedValue1Bracket");
            TEST( sample[2] == 13,                          "SortedVector.SortedValue2Bracket");
            SortedVector<Int32>::const_iterator constfind5 = sample.find(5);
            SortedVector<Int32>::const_iterator constfind8 = sample.find(8);
            SortedVector<Int32>::const_iterator constfind13 = sample.find(13);
            SortedVector<Int32>::const_iterator constfind100 = sample.find(100);
            SortedVector<Int32>::const_iterator constfindend = sample.end();
            TEST( *constfind5 == 5,                      "SortedVector.find1");
            TEST( *constfind8 == 8,                   "SortedVector.find2");
            TEST( *constfind13 == 13,                 "SortedVector.find3");
            TEST( constfind100 == constfindend,         "SortedVector.findfail1");
            SortedVector<Int32>::iterator find5 = sample.find(5);
            SortedVector<Int32>::iterator find8 = sample.find(8);
            SortedVector<Int32>::iterator find13 = sample.find(13);
            SortedVector<Int32>::iterator find100 = sample.find(100);
            SortedVector<Int32>::iterator findend = sample.end();
            TEST( *find5 == 5,                      "SortedVector.find1");
            TEST( *find8 == 8,                   "SortedVector.find2");
            TEST( *find13 == 13,                 "SortedVector.find3");
            TEST( find100 == findend,         "SortedVector.findfail1");
            TEST( sample.contains(5) == true,               "SortedVector.contains1");
            TEST( sample.contains(8) == true,               "SortedVector.contains2");
            TEST( sample.contains(13) == true,              "SortedVector.contains3");
            TEST( sample.contains(-1) == false,             "SortedVector.containsfalse");


        }


    }

    /// @brief Since RunAutomaticTests is implemented so is this.
    /// @return returns true
    virtual bool HasAutomaticTests() const
        { return true; }
};//spheretests

#endif
