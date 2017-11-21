// © Copyright 2010 - 2017 BlackTopp Studios Inc.
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
#ifndef _managedarraytests_h
#define _managedarraytests_h

#include "mezztest.h"

#include "managedarray.h"
#include "sortedmanagedarray.h"

#include <stdexcept> //only used to throw for TEST_THROW

/// @file
/// @brief This file tests the functionality of the SortedManagedArray class.

using namespace Mezzanine;
using namespace Mezzanine::Testing;

/// @brief A collection of tests for an array-like container that keeps its elements sorted.
class sortedmanagedarraytests : public UnitTestGroup
{
public:
    /// @copydoc Mezzanine::Testing::UnitTestGroup::Name
    /// @return Returns a String containing "SortedManagedArray".
    virtual String Name()
        { return String("SortedManagedArray"); }

    /// @brief This is called when Automatic tests are run.
    void RunAutomaticTests()
    {
        {
            ManagedArray<int,3> Sorted;
            Sorted.push_back(1);
            Sorted.push_back(2);
            Sorted.push_back(3);
            TEST( binary_find(Sorted.begin(),Sorted.end(),1,std::less<int>()) == Sorted.begin(),
                  "BinaryFind1Less");
            TEST( binary_find(Sorted.begin(),Sorted.end(),2,std::less<int>()) == Sorted.begin()+1,
                  "BinaryFind2Less");
            TEST( binary_find(Sorted.begin(),Sorted.end(),3,std::less<int>()) == Sorted.end()-1,
                  "BinaryFind3Less");


            TEST( binary_find(Sorted.begin(),Sorted.end(),1) == Sorted.begin(),    "BinaryFind1");
            TEST( binary_find(Sorted.begin(),Sorted.end(),2) == Sorted.begin()+1,  "BinaryFind2");
            TEST( binary_find(Sorted.begin(),Sorted.end(),3) == Sorted.end()-1,    "BinaryFind3");
        }

        {
            ManagedArray<int,3> Back;
            Back.push_back(3);
            Back.push_back(2);
            Back.push_back(1);
            //std::cout << *binary_find(Back.begin(),Back.end(),3,std::greater<int>()) << std::endl;
            //std::cout << *binary_find(Back.begin(),Back.end(),2,std::greater<int>()) << std::endl;
            //std::cout << *binary_find(Back.begin(),Back.end(),1,std::greater<int>()) << std::endl;
            TEST( binary_find(Back.begin(),Back.end(),3,std::greater<int>()) == Back.begin(),
                  "BinaryFind1Greater");
            TEST( binary_find(Back.begin(),Back.end(),2,std::greater<int>()) == Back.begin()+1,
                  "BinaryFind2Greater");
            TEST( binary_find(Back.begin(),Back.end(),1,std::greater<int>()) == Back.end()-1,
                  "BinaryFind3Greater");
        }

        {
            SortedManagedArray<Int32,1> tested;
            TEST(tested.size() == 0, "SortedManagedArray.Size1");
        }

        {
            SortedManagedArray<Int32,1> tested;
            tested.add(5);
            TEST(tested.size() == 1,            "SortedManagedArray.Size2");
            TEST( *(tested.begin()) == 5,       "SortedManagedArray.BeginDereferenced1");

            tested.clear();
            TEST( tested.size() == 0,           "SortedManagedArray.SizeIntClear");
        }

        {
            SortedManagedArray<Int32,3> tested;
            tested.add(13);
            tested.add(5);
            tested.add(8);
            TEST(tested.size() == 3,                        "SortedManagedArray.SizeInt");
            TEST( *(tested.begin()) == 5,                   "SortedManagedArray.SortedValue1Begin");
            TEST( *(tested.begin()+1) == 8,                 "SortedManagedArray.SortedValue2Begin");
            TEST( *(tested.begin()+2) == 13,                "SortedManagedArray.SortedValue3Begin");
            TEST( *(tested.end()-3) == 5,                   "SortedManagedArray.SortedValue1End");
            TEST( *(tested.end()-2) == 8,                   "SortedManagedArray.SortedValue2End");
            TEST( *(tested.end()-1) == 13,                  "SortedManagedArray.SortedValue3End");
            TEST( tested[0] == 5,                           "SortedManagedArray.SortedValue0Bracket");
            TEST( tested[1] == 8,                           "SortedManagedArray.SortedValue1Bracket");
            TEST( tested[2] == 13,                          "SortedManagedArray.SortedValue2Bracket");
            SortedManagedArray<Int32,3>::const_iterator constfind5 = tested.find(5);
            SortedManagedArray<Int32,3>::const_iterator constfind8 = tested.find(8);
            SortedManagedArray<Int32,3>::const_iterator constfind13 = tested.find(13);
            SortedManagedArray<Int32,3>::const_iterator constfind100 = tested.find(100);
            SortedManagedArray<Int32,3>::const_iterator constfindend = tested.end();
            TEST( *constfind5 == 5,                 "SortedManagedArray.find1");
            TEST( *constfind8 == 8,                 "SortedManagedArray.find2");
            TEST( *constfind13 == 13,               "SortedManagedArray.find3");
            TEST( constfind100 == constfindend,     "SortedManagedArray.findfail1");
            SortedManagedArray<Int32,3>::iterator find5 = tested.find(5);
            SortedManagedArray<Int32,3>::iterator find8 = tested.find(8);
            SortedManagedArray<Int32,3>::iterator find13 = tested.find(13);
            SortedManagedArray<Int32,3>::iterator find100 = tested.find(100);
            SortedManagedArray<Int32,3>::iterator findend = tested.end();
            TEST( *find5 == 5,                  "SortedManagedArray.find1");
            TEST( *find8 == 8,                  "SortedManagedArray.find2");
            TEST( *find13 == 13,                "SortedManagedArray.find3");
            TEST( find100 == findend,           "SortedManagedArray.findfail1");
            TEST( tested.contains(5) == true,       "SortedManagedArray.contains1");
            TEST( tested.contains(8) == true,       "SortedManagedArray.contains2");
            TEST( tested.contains(13) == true,      "SortedManagedArray.contains3");
            TEST( tested.contains(-1) == false,     "SortedManagedArray.containsfalse");
        }

        {
            SortedManagedArray<String,3> tested;
            tested.add("B");
            tested.add("A");
            tested.add("C");
            TEST( tested[0] == "A",                 "SortedManagedArray.SortedString0Bracket");
            TEST( tested[1] == "B",                 "SortedManagedArray.SortedString1Bracket");
            TEST( tested[2] == "C",                 "SortedManagedArray.SortedString2Bracket");

            TEST( tested.size() == 3,               "SortedManagedArray.SizeString");

            tested.clear()
            TEST( tested.size() == 0,               "SortedManagedArray.SizeStringClear");
        }

        {
            SortedManagedArray<Int32,3> tested;
            tested.add(9);
            tested.add(3);
            tested.add(6);

            TEST( *(tested.rbegin()) == 9,          "SortedManagedArray.SortedValue1RBegin");
            TEST( *(tested.rbegin()+1) == 6,        "SortedManagedArray.SortedValue2RBegin");
            TEST( *(tested.rbegin()+2) == 3,        "SortedManagedArray.SortedValue3RBegin");
            TEST( *(tested.rend()-3) == 9,          "SortedManagedArray.SortedValue1REnd");
            TEST( *(tested.rend()-2) == 6,          "SortedManagedArray.SortedValue2REnd");
            TEST( *(tested.rend()-1) == 3,          "SortedManagedArray.SortedValue3REnd");
        }

        {
            SortedManagedArray<String,6> tested;
            tested.add("B");
            tested.add("E");
            tested.add("C");
            tested.add("A");
            tested.add("F");
            tested.add("D");
            TEST( tested[0] == "A",                 "SortedManagedArray.ErasePrep");
            tested.erase(tested.begin());
            TEST( tested[0] == "B",                 "SortedManagedArray.EraseSingle");
            tested.erase(tested.begin(), tested.begin()+3);
            std::cout << tested[0] << std::endl;
            TEST( tested[0] == "E",                 "SortedManagedArray.EraseRange");
        }

        {
            std::vector<Whole> Unsorted;
            Unsorted.push_back(15);
            Unsorted.push_back(10);
            Unsorted.push_back(60);

            SortedManagedArray<Whole,3> Sorted;
            Sorted.add_range(Unsorted.begin(), Unsorted.end());
            TEST( *(Sorted.begin()) == 10,          "SortedManagedArray.RangeAdd1Begin");
            TEST( *(Sorted.begin()+1) == 15,        "SortedManagedArray.RangeAdd2Begin");
            TEST( *(Sorted.begin()+2) == 60,        "SortedManagedArray.RangeAdd3Begin");

        }
    }

    /// @brief Since RunAutomaticTests is implemented so is this.
    /// @return Returns true.
    virtual bool HasAutomaticTests() const
        { return true; }
};//sortedmanagedarraytests

#endif
