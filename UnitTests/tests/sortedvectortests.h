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
#include <hashedstring.h>


/// @file
/// @brief Test the sorted vector works

using namespace Mezzanine;
using namespace Mezzanine::Testing;

/// @brief Makes a random string for testing purposes.
/// @param Length The length of the String.
/// @return An alphanumeric string of the desired length.
/// @details Using code from
/// http://stackoverflow.com/questions/440133/how-do-i-create-a-random-alpha-numeric-string-in-c
/// with written by the Creative Commons SA license.
String GenerateRandomString(const int Length = (rand()%15+5))
{
    static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";

    String Results;
    Results.reserve(Length);
    for (int i = 0; i < Length; ++i)
        { Results.push_back(alphanum[rand() % (sizeof(alphanum) - 1)]); }

    return Results;
}

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
            std::vector<int> Back;
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

        {
            std::vector<Whole> Unsorted;
            Unsorted.push_back(15);
            Unsorted.push_back(10);
            Unsorted.push_back(60);

            SortedVector<Whole> Sorted;
            Sorted.add_range(Unsorted.begin(), Unsorted.end());
            TEST( *(Sorted.begin()) == 10,          "SortedVector.RangeAdd1Begin");
            TEST( *(Sorted.begin()+1) == 15,        "SortedVector.RangeAdd2Begin");
            TEST( *(Sorted.begin()+2) == 60,        "SortedVector.RangeAdd3Begin");

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


        {
            const Integer AmountToSort = 100000; // What is the lowest this start failing?
            TestOutput << endl << "Benchmarking creation of " << AmountToSort
                       << " Strings and Hashed Strings and sorting them in an std::set and "
                       << "a SortedVector." << endl;
            std::vector<String> Gibberish;
            std::vector<HashedString32> GibberishHashed;
            Gibberish.reserve(AmountToSort);
            GibberishHashed.reserve(AmountToSort);
            String Scratch;
            TestOutput << "Stuffing into set" << endl;
            for(Integer I = 0; I < AmountToSort; I++)
            {
                Scratch = GenerateRandomString();
                Gibberish.push_back(Scratch);
                GibberishHashed.push_back(HashedString32(Scratch));
            }
            std::vector<String> SortDump;
            std::vector<HashedString32> SortDumpHash;


            SortDumpHash.clear();
            SortDump.clear();
            TestOutput << "Stuffing String into set." << endl;
            MaxInt PreSetString = GetTimeStamp();
            std::set<String> SetString;
            for(std::vector<String>::iterator Iter = Gibberish.begin();
                Iter != Gibberish.end();
                Iter++)
            { SetString.insert(*Iter); }
            MaxInt PostSetString = GetTimeStamp();
            SortDump.reserve(AmountToSort);
            for(std::vector<String>::iterator Iter = Gibberish.begin();
                Iter != Gibberish.end();
                Iter++)
            { SortDump.push_back(*SetString.find(*Iter)); }
            MaxInt FinalSetString = GetTimeStamp();
            MaxInt SetStringSeek = FinalSetString - PostSetString;
            MaxInt SetStringDuration = PostSetString - PreSetString;
            TestOutput << "To prevent optimization: "
                       << *(SetString.begin()) << SortDump.at(rand()%AmountToSort) << endl;

            SortDumpHash.clear();
            SortDump.clear();
            TestOutput << "Stuffing Hashed String into set." << endl;
            MaxInt PreSetHash = GetTimeStamp();
            std::set<HashedString32> SetHashString;
            for(std::vector<HashedString32>::iterator Iter = GibberishHashed.begin();
                Iter != GibberishHashed.end();
                Iter++)
            { SetHashString.insert(*Iter); }
            MaxInt PostSetHash = GetTimeStamp();
            SortDumpHash.reserve(AmountToSort);
            for(std::vector<HashedString32>::iterator Iter = GibberishHashed.begin();
                Iter != GibberishHashed.end();
                Iter++)
            { SortDumpHash.push_back(*SetHashString.find(*Iter)); }
            MaxInt FinalSetHash = GetTimeStamp();
            MaxInt SetHashSeek = FinalSetHash - PostSetHash;
            MaxInt SetHashDuration = PostSetHash - PreSetHash;
            TestOutput << "To prevent optimization: "
                       << *(SetHashString.begin()) << SortDumpHash.at(rand()%AmountToSort) << endl;

/*
            SortDumpHash.clear();
            SortDump.clear();
            TestOutput << "Stuffing String into SortedVector." << endl;
            MaxInt PreVecString = GetTimeStamp();
            SortedVector<String> VecString;
            for(std::vector<String>::iterator Iter = Gibberish.begin();
                Iter != Gibberish.end();
                Iter++)
            { VecString.add(*Iter); }
            MaxInt PostVecString = GetTimeStamp();
            SortDump.reserve(AmountToSort);
            for(std::vector<String>::iterator Iter = Gibberish.begin();
                Iter != Gibberish.end();
                Iter++)
            { SortDump.push_back(*VecString.find(*Iter)); }
            MaxInt FinalVecString = GetTimeStamp();
            MaxInt VecStringSeek = FinalVecString - PostVecString;
            MaxInt VecStringDuration = PostVecString - PreVecString;
            TestOutput << "To prevent optimization: "
                       << *(VecString.begin()) << SortDump.at(rand()%AmountToSort) << endl;

            SortDumpHash.clear();
            SortDump.clear();
            TestOutput << "Stuffing Hashed String into SortedVector." << endl;
            MaxInt PreVecHash = GetTimeStamp();
            SortedVector<HashedString32> VecHash;
            for(std::vector<HashedString32>::iterator Iter = GibberishHashed.begin();
                Iter != GibberishHashed.end();
                Iter++)
            { VecHash.add(*Iter); }
            MaxInt PostVecHash = GetTimeStamp();
            SortDumpHash.reserve(AmountToSort);
            for(std::vector<HashedString32>::iterator Iter = GibberishHashed.begin();
                Iter != GibberishHashed.end();
                Iter++)
            { SortDumpHash.push_back(*VecHash.find(*Iter)); }
            MaxInt FinalVecHash = GetTimeStamp();
            MaxInt VecHashSeek = FinalVecHash - PostVecHash;
            MaxInt VecHashDuration = PostVecHash - PreVecHash;
            TestOutput << "To prevent optimization: "
                       << *(VecHash.begin()) << SortDumpHash.at(rand()%AmountToSort) << endl;


            SortDumpHash.clear();
            SortDump.clear();
            TestOutput << "Stuffing String into SortedVector with reserve." << endl;
            MaxInt PreCapString = GetTimeStamp();
            SortedVector<String> CapString;
            CapString.reserve(AmountToSort);
            for(std::vector<String>::iterator Iter = Gibberish.begin();
                Iter != Gibberish.end();
                Iter++)
            { CapString.add(*Iter); }
            MaxInt PostCapString = GetTimeStamp();
            SortDump.reserve(AmountToSort);
            for(std::vector<String>::iterator Iter = Gibberish.begin();
                Iter != Gibberish.end();
                Iter++)
            { SortDump.push_back(*CapString.find(*Iter)); }
            MaxInt FinalCapString = GetTimeStamp();
            MaxInt CapStringSeek = FinalCapString - PostCapString;
            MaxInt CapStringDuration = PostCapString - PreCapString;
            TestOutput << "To prevent optimization: "
                       << *(CapString.begin()) << SortDump.at(rand()%AmountToSort) << endl;

            SortDumpHash.clear();
            SortDump.clear();
            TestOutput << "Stuffing Hashed String into SortedVector with reserve." << endl;
            MaxInt PreCapHash = GetTimeStamp();
            SortedVector<HashedString32> CapHash;
            CapHash.reserve(AmountToSort);
            for(std::vector<HashedString32>::iterator Iter = GibberishHashed.begin();
                Iter != GibberishHashed.end();
                Iter++)
            { CapHash.add(*Iter); }
            MaxInt PostCapHash = GetTimeStamp();
            SortDumpHash.reserve(AmountToSort);
            for(std::vector<HashedString32>::iterator Iter = GibberishHashed.begin();
                Iter != GibberishHashed.end();
                Iter++)
            { SortDumpHash.push_back(*CapHash.find(*Iter)); }
            MaxInt FinalCapHash = GetTimeStamp();
            MaxInt CapHashSeek = FinalCapHash - PostCapHash;
            MaxInt CapHashDuration = PostCapHash - PreCapHash;
            TestOutput << "To prevent optimization: "
                       << *(CapHash.begin()) << SortDumpHash.at(rand()%AmountToSort) << endl;
*/

            SortDumpHash.clear();
            SortDump.clear();
            TestOutput << "RangeAdding String into SortedVector." << endl;
            MaxInt PreRangeString = GetTimeStamp();
            SortedVector<String> RangeString;
            RangeString.reserve(AmountToSort);
            RangeString.add_range(Gibberish.begin(), Gibberish.end());
            MaxInt PostRangeString = GetTimeStamp();
            SortDump.reserve(AmountToSort);
            for(std::vector<String>::iterator Iter = Gibberish.begin();
                Iter != Gibberish.end();
                Iter++)
            { SortDump.push_back(*RangeString.find(*Iter)); }
            MaxInt FinalRangeString = GetTimeStamp();
            MaxInt RangeStringSeek = FinalRangeString - PostRangeString;
            MaxInt RangeStringDuration = PostRangeString - PreRangeString;
            TestOutput << "To prevent optimization: "
                       << *(RangeString.begin()) << SortDump.at(rand()%AmountToSort) << endl;

            SortDumpHash.clear();
            SortDump.clear();
            TestOutput << "Stuffing Hashed String into SortedVector." << endl;
            MaxInt PreRangeHash = GetTimeStamp();
            SortedVector<HashedString32> RangeHash;
            RangeHash.reserve(AmountToSort);
            RangeHash.add_range(GibberishHashed.begin(), GibberishHashed.end());
            MaxInt PostRangeHash = GetTimeStamp();
            SortDumpHash.reserve(AmountToSort);
            for(std::vector<HashedString32>::iterator Iter = GibberishHashed.begin();
                Iter != GibberishHashed.end();
                Iter++)
            { SortDumpHash.push_back(*RangeHash.find(*Iter)); }
            MaxInt FinalRangeHash = GetTimeStamp();
            MaxInt RangeHashSeek = FinalRangeHash - PostRangeHash;
            MaxInt RangeHashDuration = PostRangeHash - PreRangeHash;
            TestOutput << "To prevent optimization: "
                       << *(RangeHash.begin()) << SortDumpHash.at(rand()%AmountToSort) << endl;

            TestOutput
                << "Set with String                               : " << SetStringDuration << endl
                << "seek:                                         : " << SetStringSeek << endl
                << endl
                << "Set with HashedString                         : " << SetHashDuration << endl
                << "seek:                                         : " << SetHashSeek << endl
                << endl
//                << "SortedVector with String                      : " << VecStringDuration << endl
//                << "seek:                                         : " << VecStringSeek << endl
//                << endl
//                << "SortedVector with HashedString                : " << VecHashDuration << endl
//                << "seek:                                         : " << VecHashSeek << endl
//                << endl
//                << "SortedVector with Capacity/String             : " << CapStringDuration << endl
//                << "seek:                                         : " << CapStringSeek << endl
//                << endl
//                << "SortedVector with Capacity/HashedString       : " << CapHashDuration << endl
//                << "seek:                                         : " << CapHashSeek << endl
//                << endl
                << "SortedVector with Range/Capacity/String       : " << RangeStringDuration << endl
                << "seek:                                         : " << RangeStringSeek << endl
                << endl
                << "SortedVector with Range/Capacity/HashedString : " << RangeHashDuration << endl
                << "seek:                                         : " << RangeHashSeek << endl
                << endl
                << endl;

            TEST_WARN(SetStringDuration > RangeStringDuration, "SortedVectorInsertsFast_String");
            TEST_WARN(SetStringSeek > RangeStringSeek, "SortedVectorSeekFast_String");
            TEST_WARN(SetHashDuration > RangeHashDuration, "SortedVectorInsertsFast_Hashed");
            TEST_WARN(SetHashSeek > RangeHashSeek, "SortedVectorSeekFast_Hashed");
        }
    }

    /// @brief Since RunAutomaticTests is implemented so is this.
    /// @return returns true
    virtual bool HasAutomaticTests() const
        { return true; }
};//spheretests

#endif
