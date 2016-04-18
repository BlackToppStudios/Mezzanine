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
            SortedVector<Int32> tested;
            TEST(tested.size() == 0, "SortedVector.Size1");
        }

        {
            SortedVector<Int32> tested;
            tested.add(5);
            TEST(tested.size() == 1,            "SortedVector.Size2");
            TEST( *(tested.begin()) == 5,       "SortedVector.BeginDerefenced1");

            tested.clear();
            TEST( tested.size() == 0,           "SortedVector.SizeIntClear");
        }

        {
            SortedVector<Int32> tested;
            tested.add(13);
            tested.add(5);
            tested.add(8);
            TEST(tested.size() == 3,                        "SortedVector.SizeInt");
            TEST( *(tested.begin()) == 5,                   "SortedVector.SortedValue1Begin");
            TEST( *(tested.begin()+1) == 8,                 "SortedVector.SortedValue2Begin");
            TEST( *(tested.begin()+2) == 13,                "SortedVector.SortedValue3Begin");
            TEST( *(tested.end()-3) == 5,                   "SortedVector.SortedValue1End");
            TEST( *(tested.end()-2) == 8,                   "SortedVector.SortedValue2End");
            TEST( *(tested.end()-1) == 13,                  "SortedVector.SortedValue3End");
            TEST( tested[0] == 5,                           "SortedVector.SortedValue0Bracket");
            TEST( tested[1] == 8,                           "SortedVector.SortedValue1Bracket");
            TEST( tested[2] == 13,                          "SortedVector.SortedValue2Bracket");
            SortedVector<Int32>::const_iterator constfind5 = tested.find(5);
            SortedVector<Int32>::const_iterator constfind8 = tested.find(8);
            SortedVector<Int32>::const_iterator constfind13 = tested.find(13);
            SortedVector<Int32>::const_iterator constfind100 = tested.find(100);
            SortedVector<Int32>::const_iterator constfindend = tested.end();
            TEST( *constfind5 == 5,                 "SortedVector.find1");
            TEST( *constfind8 == 8,                 "SortedVector.find2");
            TEST( *constfind13 == 13,               "SortedVector.find3");
            TEST( constfind100 == constfindend,     "SortedVector.findfail1");
            SortedVector<Int32>::iterator find5 = tested.find(5);
            SortedVector<Int32>::iterator find8 = tested.find(8);
            SortedVector<Int32>::iterator find13 = tested.find(13);
            SortedVector<Int32>::iterator find100 = tested.find(100);
            SortedVector<Int32>::iterator findend = tested.end();
            TEST( *find5 == 5,                  "SortedVector.find1");
            TEST( *find8 == 8,                  "SortedVector.find2");
            TEST( *find13 == 13,                "SortedVector.find3");
            TEST( find100 == findend,           "SortedVector.findfail1");
            TEST( tested.contains(5) == true,       "SortedVector.contains1");
            TEST( tested.contains(8) == true,       "SortedVector.contains2");
            TEST( tested.contains(13) == true,      "SortedVector.contains3");
            TEST( tested.contains(-1) == false,     "SortedVector.containsfalse");
        }

        {
            SortedVector<String> tested;
            tested.add("B");
            tested.add("A");
            tested.add("C");
            TEST( tested[0] == "A",                 "SortedVector.SortedString0Bracket");
            TEST( tested[1] == "B",                 "SortedVector.SortedString1Bracket");
            TEST( tested[2] == "C",                 "SortedVector.SortedString2Bracket");

            TEST( tested.size() == 3,               "SortedVector.SizeString");

            tested.clear()
            TEST( tested.size() == 0,               "SortedVector.SizeStringClear");
        }

        {
            SortedVector<Int32> tested;
            tested.add(3);
            tested.add(6);
            tested.add(9);
            TEST(tested.capacity() >= 3, "SimpleCapacity");
            tested.reserve(10);
            TEST(tested.capacity() >= 9, "CapacityAfterReserve");
        }

        {
            SortedVector<Int32> tested;
            tested.add(9);
            tested.add(3);
            tested.add(6);

            TEST( *(tested.rbegin()) == 9,          "SortedVector.SortedValue1RBegin");
            TEST( *(tested.rbegin()+1) == 6,        "SortedVector.SortedValue2RBegin");
            TEST( *(tested.rbegin()+2) == 3,        "SortedVector.SortedValue3RBegin");
            TEST( *(tested.rend()-3) == 9,          "SortedVector.SortedValue1REnd");
            TEST( *(tested.rend()-2) == 6,          "SortedVector.SortedValue2REnd");
            TEST( *(tested.rend()-1) == 3,          "SortedVector.SortedValue3REnd");
        }

        {
            SortedVector<String> tested;
            tested.add("B");
            tested.add("E");
            tested.add("C");
            tested.add("A");
            tested.add("F");
            tested.add("D");
            TEST( tested[0] == "A",                 "SortedVector.ErasePrep");
            tested.erase(tested.begin());
            TEST( tested[0] == "B",                 "SortedVector.EraseSingle");
            tested.erase(tested.begin(), tested.begin()+3);
            std::cout << tested[0] << std::endl;
            TEST( tested[0] == "E",                 "SortedVector.EraseRange");
        }

        // need proper C++11 support for the good resize implementation
//        {
//            SortedVector<Int32> tested;
//            tested.add(1);
//            tested.add(9);
//            tested.resize(3,6);
//            TEST( tested.size() == 3,               "SortedVector.ResizeSizeInt");
//            TEST( *(tested.begin()) == 1,           "SortedVector.SortedValue1Resize");
//            TEST( *(tested.begin()+1) == 6,         "SortedVector.SortedValue2Resize");
//            TEST( *(tested.begin()+2) == 9,         "SortedVector.SortedValue3Resize");
//        }

        // Super busted without C++11
//        {
//            SortedVector<String> tested;
//            tested.add("C");
//            tested.add("A");
//            tested.resize(3);
//            TEST( tested.size() == 3,                "SortedVector.ResizeSizeString");
//            TEST( *(tested.begin()) == "",           "SortedVector.SortedValueString1Resize");
//            TEST( *(tested.begin()+1) == "A",        "SortedVector.SortedValueString2Resize");
//            TEST( *(tested.begin()+2) == "C",        "SortedVector.SortedValueString3Resize");
//        }



    }

    /// @brief Since RunAutomaticTests is implemented so is this.
    /// @return returns true
    virtual bool HasAutomaticTests() const
        { return true; }
};//spheretests

#endif
