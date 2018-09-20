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
#ifndef _flatmaptests_h
#define _flatmaptests_h

#include "mezztest.h"

#include "flatmap.h"

#include <stdexcept> //only used to throw for TEST_THROW

/// @file
/// @brief This file tests the functionality of the FlatMap class.

using namespace Mezzanine;
using namespace Mezzanine::Testing;

namespace std {
    // Alt key compare operators
    bool operator<(const std::vector<char>& AltKey, const std::string& Str)
        { return std::lexicographical_compare(AltKey.begin(),AltKey.end(),Str.begin(),Str.end()); }
    bool operator<(const std::string& Str, const std::vector<char>& AltKey)
        { return std::lexicographical_compare(Str.begin(),Str.end(),AltKey.begin(),AltKey.end()); }
}

/// @brief A collection of tests for an std::map like class that stores it's elements in contiguous memory.
class flatmaptests : public UnitTestGroup
{
public:
    /// @copydoc Mezzanine::Testing::UnitTestGroup::Name
    /// @return Returns a String containing "FlatMap".
    virtual String Name()
        { return String("FlatMap"); }

    /// @brief This is called when Automatic tests are run.
    void RunAutomaticTests()
    {
        {//Construction
            FlatMap<int,std::string> DefaultTest;
            TEST( DefaultTest.size() == 0, "FlatMap()-Count/Empty" );
            TEST( DefaultTest.begin() == DefaultTest.end(), "FlatMap()-BeginMatchesEnd" );

            FlatMap<int,std::string> InitializerTest = { {12,"Twelve"}, {5,"Five"}, {50,"Fifty"}, {22,"TwentyTwo"} };
            FlatMap<int,std::string>::iterator InitIter = InitializerTest.begin();
            TEST( (*InitIter).first == 5, "FlatMap(std::initializer_list<value_type>)-Element1-First" );
            TEST( (*InitIter).second == "Five", "FlatMap(std::initializer_list<value_type>)-Element1-Second" );
            std::advance(InitIter,1);
            TEST( (*InitIter).first == 12, "FlatMap(std::initializer_list<value_type>)-Element2-First" );
            TEST( (*InitIter).second == "Twelve", "FlatMap(std::initializer_list<value_type>)-Element2-Second" );
            std::advance(InitIter,1);
            TEST( (*InitIter).first == 22, "FlatMap(std::initializer_list<value_type>)-Element3-First" );
            TEST( (*InitIter).second == "TwentyTwo", "FlatMap(std::initializer_list<value_type>)-Element3-Second" );
            std::advance(InitIter,1);
            TEST( (*InitIter).first == 50, "FlatMap(std::initializer_list<value_type>)-Element4-First" );
            TEST( (*InitIter).second == "Fifty", "FlatMap(std::initializer_list<value_type>)-Element4-Second" );
            TEST( InitializerTest.size() == 4, "FlatMap(std::initializer_list<value_type>)-ElementCount" );

            FlatMap<int,std::string> CopyTest(InitializerTest);
            FlatMap<int,std::string>::iterator CopyIter = CopyTest.begin();
            TEST( (*CopyIter).first == 5, "FlatMap(const_SelfType&)-Element1-First" );
            TEST( (*CopyIter).second == "Five", "FlatMap(const_SelfType&)-Element1-Second" );
            std::advance(CopyIter,1);
            TEST( (*CopyIter).first == 12, "FlatMap(const_SelfType&)-Element2-First" );
            TEST( (*CopyIter).second == "Twelve", "FlatMap(const_SelfType&)-Element2-Second" );
            std::advance(CopyIter,1);
            TEST( (*CopyIter).first == 22, "FlatMap(const_SelfType&)-Element3-First" );
            TEST( (*CopyIter).second == "TwentyTwo", "FlatMap(const_SelfType&)-Element3-Second" );
            std::advance(CopyIter,1);
            TEST( (*CopyIter).first == 50, "FlatMap(const_SelfType&)-Element4-First" );
            TEST( (*CopyIter).second == "Fifty", "FlatMap(const_SelfType&)-Element4-Second" );
            TEST( CopyTest.size() == 4, "FlatMap(const_SelfType&)-ElementCount" );

            std::vector<std::pair<std::string,int>> RangeSourceTest = { {"Aardvark",5}, {"Zebra",21}, {"Lion",10}, {"Platypus",1} };
            FlatMap<std::string,int> RangeDestTest(RangeSourceTest.begin(),RangeSourceTest.end());
            FlatMap<std::string,int>::iterator RangeIter = RangeDestTest.begin();
            TEST( (*RangeIter).first == "Aardvark", "FlatMap(InputIterator,InputIterator)-Element1-First" );
            TEST( (*RangeIter).second == 5, "FlatMap(InputIterator,InputIterator)-Element1-Second" );
            std::advance(RangeIter,1);
            TEST( (*RangeIter).first == "Lion", "FlatMap(InputIterator,InputIterator)-Element2-First" );
            TEST( (*RangeIter).second == 10, "FlatMap(InputIterator,InputIterator)-Element2-Second" );
            std::advance(RangeIter,1);
            TEST( (*RangeIter).first == "Platypus", "FlatMap(InputIterator,InputIterator)-Element3-First" );
            TEST( (*RangeIter).second == 1, "FlatMap(InputIterator,InputIterator)-Element3-Second" );
            std::advance(RangeIter,1);
            TEST( (*RangeIter).first == "Zebra", "FlatMap(InputIterator,InputIterator)-Element4-First" );
            TEST( (*RangeIter).second == 21, "FlatMap(InputIterator,InputIterator)-Element4-Second" );
            TEST( RangeDestTest.size() == 4, "FlatMap(InputIterator,InputIterator)-ElementCount" );

            FlatMap<int,float> MoveSourceTest = { {27,13.5f}, {73,36.5f}, {15,7.5f}, {14,7.0f}, {99,49.5} };
            FlatMap<int,float> MoveDestTest( std::move(MoveSourceTest) );
            FlatMap<int,float>::iterator MoveIter = MoveDestTest.begin();
            TEST( (*MoveIter).first == 14, "FlatMap(SelfType&&)-Element1-First" );
            TEST( (*MoveIter).second == 7.0f, "FlatMap(SelfType&&)-Element1-Second" );
            std::advance(MoveIter,1);
            TEST( (*MoveIter).first == 15, "FlatMap(SelfType&&)-Element2-First" );
            TEST( (*MoveIter).second == 7.5f, "FlatMap(SelfType&&)-Element2-Second" );
            std::advance(MoveIter,1);
            TEST( (*MoveIter).first == 27, "FlatMap(SelfType&&)-Element3-First" );
            TEST( (*MoveIter).second == 13.5f, "FlatMap(SelfType&&)-Element3-Second" );
            std::advance(MoveIter,1);
            TEST( (*MoveIter).first == 73, "FlatMap(SelfType&&)-Element4-First" );
            TEST( (*MoveIter).second == 36.5f, "FlatMap(SelfType&&)-Element4-Second" );
            std::advance(MoveIter,1);
            TEST( (*MoveIter).first == 99, "FlatMap(SelfType&&)-Element5-First" );
            TEST( (*MoveIter).second == 49.5, "FlatMap(SelfType&&)-Element5-Second" );
            TEST( MoveDestTest.size() == 5, "FlatMap(SelfType&&)-ElementCount" );
        }//Construction

        {//Operators
            FlatMap<int,std::string> CopyAssignSource = { {39,"39"}, {8,"8"}, {17,"17"} };
            FlatMap<int,std::string> CopyAssignDest = { {123,"123"}, {234,"234"}, {345,"345"}, {456,"456"}, {567,"567"} };
            TEST( CopyAssignDest.size() == 5, "operator=(const_SelfType&)-Dest-InitialElementCount" );
            CopyAssignDest = CopyAssignSource;
            FlatMap<int,std::string>::iterator SourceCopyIter = CopyAssignSource.begin();
            TEST( (*SourceCopyIter).first == 8, "operator=(const_SelfType&)-Source-Element1-First" );
            TEST( (*SourceCopyIter).second == "8", "operator=(const_SelfType&)-Source-Element1-Second" );
            std::advance(SourceCopyIter,1);
            TEST( (*SourceCopyIter).first == 17, "operator=(const_SelfType&)-Source-Element2-First" );
            TEST( (*SourceCopyIter).second == "17", "operator=(const_SelfType&)-Source-Element2-Second" );
            std::advance(SourceCopyIter,1);
            TEST( (*SourceCopyIter).first == 39, "operator=(const_SelfType&)-Source-Element3-First" );
            TEST( (*SourceCopyIter).second == "39", "operator=(const_SelfType&)-Source-Element3-Second" );
            TEST( CopyAssignSource.size() == 3, "operator=(const_SelfType&)-Source-ElementCount" );
            FlatMap<int,std::string>::iterator DestCopyIter = CopyAssignDest.begin();
            TEST( (*DestCopyIter).first == 8, "operator=(const_SelfType&)-Dest-Element1-First" );
            TEST( (*DestCopyIter).second == "8", "operator=(const_SelfType&)-Dest-Element1-Second" );
            std::advance(DestCopyIter,1);
            TEST( (*DestCopyIter).first == 17, "operator=(const_SelfType&)-Dest-Element2-First" );
            TEST( (*DestCopyIter).second == "17", "operator=(const_SelfType&)-Dest-Element2-Second" );
            std::advance(DestCopyIter,1);
            TEST( (*DestCopyIter).first == 39, "operator=(const_SelfType&)-Dest-Element3-First" );
            TEST( (*DestCopyIter).second == "39", "operator=(const_SelfType&)-Dest-Element3-Second" );
            TEST( CopyAssignDest.size() == 3, "operator=(const_SelfType&)-Dest-ElementCount" );

            FlatMap<int,float> MoveAssignSource = { {78,19.5f}, {96,24.0f}, {14,3.5f}, {32,8.0f} };
            FlatMap<int,float> MoveAssignDest = { {12,3.0f}, {23,5.75f}, {34,8.5f} };
            TEST( MoveAssignDest.size() == 3, "operator=(SelfType&&)-Dest-InitialElementCount" );
            MoveAssignDest = std::move( MoveAssignSource );
            TEST( MoveAssignSource.empty(), "operator=(SelfType&&)-Source-ElementCount/Empty" );
            FlatMap<int,float>::iterator DestMoveIter = MoveAssignDest.begin();
            TEST( (*DestMoveIter).first == 14, "operator=(SelfType&&)-Dest-Element1-First" );
            TEST( (*DestMoveIter).second == 3.5f, "operator=(SelfType&&)-Dest-Element1-Second" );
            std::advance(DestMoveIter,1);
            TEST( (*DestMoveIter).first == 32, "operator=(SelfType&&)-Dest-Element2-First" );
            TEST( (*DestMoveIter).second == 8.0f, "operator=(SelfType&&)-Dest-Element2-Second" );
            std::advance(DestMoveIter,1);
            TEST( (*DestMoveIter).first == 78, "operator=(SelfType&&)-Dest-Element3-First" );
            TEST( (*DestMoveIter).second == 19.5f, "operator=(SelfType&&)-Dest-Element3-Second" );
            std::advance(DestMoveIter,1);
            TEST( (*DestMoveIter).first == 96, "operator=(SelfType&&)-Dest-Element3-First" );
            TEST( (*DestMoveIter).second == 24.0f, "operator=(SelfType&&)-Dest-Element3-Second" );
            TEST( MoveAssignDest.size() == 4, "operator=(SelfType&&)-Dest-ElementCount" );
        }//Operators

        {//Allocator
            // This is a little bit awkward because default allocators are stateless and compare equal to all instances
            // of the same type.  Really, this test exists solely to verify "get_allocator()" exists on the map.
            FlatMap<int,int> AllocTest;
            TEST( AllocTest.get_allocator() == std::allocator<decltype(AllocTest)::value_type>(),
                  "get_allocator()_const" );
        }//Allocator

        {//Iterators
            using FirstMapType = FlatMap<std::string,int>;
            using SecondMapType = FlatMap<std::string,float>;
            FirstMapType TestMap = { {"Nix",48694},
                                     {"Hydra",64738},
                                     {"Charon",17536},
                                     {"Styx",42656},
                                     {"Kerberos",57783} };
            const SecondMapType ConstTestMap = { {"Io",1.796f},
                                                 {"Europa",1.314f},
                                                 {"Callisto",1.235f},
                                                 {"Ganymede",1.428f} };

            FirstMapType::iterator BeginIter = TestMap.begin();
            FirstMapType::const_iterator CBeginIter = TestMap.cbegin();
            SecondMapType::const_iterator ConstBeginIter = ConstTestMap.begin();
            TEST( (*BeginIter).first == "Charon", "begin()-First" );
            TEST( (*BeginIter).second == 17536, "begin()-Second" );
            TEST( (*CBeginIter).first == "Charon", "cbegin()_const-First" );
            TEST( (*CBeginIter).second == 17536, "cbegin()_const-Second" );
            TEST( (*ConstBeginIter).first == "Callisto", "begin()_const-First" );
            TEST( (*ConstBeginIter).second == 1.235f, "begin()_const-Second" );

            FirstMapType::iterator EndIter = TestMap.end();
            FirstMapType::const_iterator CEndIter = TestMap.cend();
            SecondMapType::const_iterator ConstEndIter = ConstTestMap.end();
            TEST( EndIter - BeginIter == 5, "end()" );
            TEST( CEndIter - CBeginIter == 5, "cend()_const" );
            TEST( ConstEndIter - ConstBeginIter == 4, "end()_const" );

            FirstMapType::reverse_iterator BeginRevIter = TestMap.rbegin();
            FirstMapType::const_reverse_iterator CBeginRevIter = TestMap.crbegin();
            SecondMapType::const_reverse_iterator ConstBeginRevIter = ConstTestMap.rbegin();
            TEST( (*BeginRevIter).first == "Styx", "rbegin()-First" );
            TEST( (*BeginRevIter).second == 42656, "rbegin()-Second" );
            TEST( (*CBeginRevIter).first == "Styx", "crbegin()_const-First" );
            TEST( (*CBeginRevIter).second == 42656, "crbegin()_const-Second" );
            TEST( (*ConstBeginRevIter).first == "Io", "rbegin()_const-First" );
            TEST( (*ConstBeginRevIter).second == 1.796f, "rbegin()_const-Second" );

            FirstMapType::reverse_iterator EndRevIter = TestMap.rend();
            FirstMapType::const_reverse_iterator CEndRevIter = TestMap.crend();
            SecondMapType::const_reverse_iterator ConstEndRevIter = ConstTestMap.rend();
            TEST( EndRevIter - BeginRevIter == 5, "rend()" );
            TEST( CEndRevIter - CBeginRevIter == 5, "crend()_const" );
            TEST( ConstEndRevIter - ConstBeginRevIter == 4, "rend()_const" );
        }//Iterators

        {//Capacity
            // The max_size tests here are awkward as well, as the actual valid value is implementation defined.
            // So the tests here for max_size are solely meant to verify the method exists and doesn't give a
            // completely bogus value.
            using TestMapType = FlatMap<int,float>;
            TestMapType EmptyMap;
            TestMapType FullMap = { {1,2.5f}, {3,7.5f}, {5,12.5f}, {7,17.5f} };

            TEST( EmptyMap.size() == 0, "size()_const-Empty" );
            TEST( FullMap.size() == 4, "size()_const-Full" );
            TEST( EmptyMap.max_size() >= 1000000, "max_size()_const-Empty" );
            TEST( FullMap.max_size() >= 1000000, "max_size()_const-Full" );
            TEST( EmptyMap.empty() == true, "empty()_const-Empty" );
            TEST( FullMap.empty() == false, "empty()_const-Full" );
        }//Capacity

        {//Element Access
            // Tiny struct to ensure zero initialization in all contexts.
            struct SafeFloat
            {
                float Val{0.0f};

                SafeFloat() = default;
                SafeFloat(const SafeFloat& New) = default;
                SafeFloat(SafeFloat&& New) = default;
                SafeFloat(const float New) : Val(New) {  };
                ~SafeFloat() = default;

                SafeFloat& operator=(const SafeFloat& Other) = default;
                SafeFloat& operator=(SafeFloat&& Other) = default;
                operator float() const { return Val; }
            };

            using TestMapType = FlatMap<std::string,SafeFloat>;
            TestMapType AccessMap = { {"Dione",0.232f}, {"Enceladus",0.113f}, {"Iapetus",0.223f}, {"Mimas",0.064f} };
            const TestMapType ConstMap = { {"Rhea",0.264f}, {"Tethys",0.146f}, {"Titan",1.352f} };

            TEST( AccessMap[std::string("Mimas")] == 0.064f, "operator[](const_key_type&)-Check1" );
            TEST( AccessMap[std::string("Iapetus")] == 0.223f, "operator[](const_key_type&)-Check2" );
            TEST( AccessMap[std::string("Enceladus")] == 0.113f, "operator[](const_key_type&)-Check3" );
            TEST( AccessMap[std::string("Dione")] == 0.232f, "operator[](const_key_type&)-Check4" );

            TEST( AccessMap["Vacuum"] == 0.0f, "operator[](const_key_type&)-Insert" );

            TEST( AccessMap.at(std::string("Mimas")) == 0.064f, "at(const_key_type&)-Check1" );
            TEST( AccessMap.at(std::string("Iapetus")) == 0.223f, "at(const_key_type&)-Check2" );
            TEST( AccessMap.at(std::string("Enceladus")) == 0.113f, "at(const_key_type&)-Check3" );
            TEST( AccessMap.at(std::string("Dione")) == 0.232f, "at(const_key_type&)-Check4" );

            TEST( ConstMap.at("Titan") == 1.352f, "at(const_key_type&)_const-Check1" );
            TEST( ConstMap.at("Tethys") == 0.146f, "at(const_key_type&)_const-Check2" );
            TEST( ConstMap.at("Rhea") == 0.264f, "at(const_key_type&)_const-Check3" );

            TEST_THROW(std::out_of_range,
                       AccessMap.at("Mars"),
                       "at(const_key_type&)-Throw");
            TEST_THROW(std::out_of_range,
                       ConstMap.at("Venus"),
                       "at(const_key_type&)_const-Throw");
        }//Element Access

        {//Lookup
            using TestMapType = FlatMap<std::string,std::string,std::less<>>;
            using MapPairType = TestMapType::iterator_pair;
            using ConstMapPairType = TestMapType::const_iterator_pair;

            TestMapType LookupMap = { {"Earth","Sol III"},
                                      {"Jupiter","Sol V"},
                                      {"Venus","Sol II"},
                                      {"Mars","Sol IV"},
                                      {"Mercury","Sol I"} };
            TestMapType::iterator BeginIt = LookupMap.begin();
            TestMapType::iterator EndIt = LookupMap.end();
            const TestMapType ConstLookupMap = { {"Uranus","Sol VII"},
                                                 {"Saturn","Sol VI"},
                                                 {"Pluto","Sol IX"},
                                                 {"Neptune","Sol VIII"} };
            TestMapType::const_iterator ConstBeginIt = ConstLookupMap.begin();
            TestMapType::const_iterator ConstEndIt = ConstLookupMap.end();

            std::vector<char> MercuryAltKey = { {'M'}, {'e'}, {'r'}, {'c'}, {'u'}, {'r'}, {'y'} };
            std::vector<char> VenusAltKey = { {'V'}, {'e'}, {'n'}, {'u'}, {'s'} };
            std::vector<char> EarthAltKey = { {'E'}, {'a'}, {'r'}, {'t'}, {'h'} };
            std::vector<char> MarsAltKey = { {'M'}, {'a'}, {'r'}, {'s'} };
            std::vector<char> JupiterAltKey = { {'J'}, {'u'}, {'p'}, {'i'}, {'t'}, {'e'}, {'r'} };
            std::vector<char> SaturnAltKey = { {'S'}, {'a'}, {'t'}, {'u'}, {'r'}, {'n'} };
            std::vector<char> UranusAltKey = { {'U'}, {'r'}, {'a'}, {'n'}, {'u'}, {'s'} };
            std::vector<char> NeptuneAltKey = { {'N'}, {'e'}, {'p'}, {'t'}, {'u'}, {'n'}, {'e'} };
            std::vector<char> PlutoAltKey = { {'P'}, {'l'}, {'u'}, {'t'}, {'o'} };
            std::vector<char> AgniaAltKey = { {'A'}, {'g'}, {'n'}, {'i'}, {'a'} };
            std::vector<char> XeniaAltKey = { {'X'}, {'e'}, {'n'}, {'i'}, {'a'} };

            TEST( LookupMap.count("Earth") == 1, "count(const_key_type&)_const-Pass" );
            TEST( LookupMap.count("Pluto") == 0, "count(const_key_type&)_const-Fail" );
            TEST( LookupMap.count(EarthAltKey) == 1, "count(const_alt_key&)_const-Pass" );
            TEST( LookupMap.count(PlutoAltKey) == 0, "count(const_alt_key&)_const-Fail" );

            TEST( LookupMap.find("Venus") == BeginIt + 4, "find(const_key_type&)-Pass" );
            TEST( LookupMap.find("Uranus") == EndIt, "find(const_key_type&)-Fail" );
            TEST( ConstLookupMap.find("Neptune") == ConstBeginIt, "find(const_key_type&)_const-Pass" );
            TEST( ConstLookupMap.find("Mars") == ConstEndIt, "find(const_key_type&)_const-Fail" );
            TEST( LookupMap.find(JupiterAltKey) == BeginIt + 1, "find(const_alt_key&)-Pass" );
            TEST( LookupMap.find(PlutoAltKey) == EndIt, "find(const_alt_key&)-Fail" );
            TEST( ConstLookupMap.find(SaturnAltKey) == ConstBeginIt + 2, "find(const_alt_key&)_const-Pass" );
            TEST( ConstLookupMap.find(MercuryAltKey) == ConstEndIt, "find(const_alt_key&)_const-Fail" );

            TEST( ConstLookupMap.contains("Pluto") == true, "contains(const_key_type&)_const-Pass" );
            TEST( ConstLookupMap.contains("Mars") == false, "contains(const_key_type&)_const-Fail" );
            TEST( ConstLookupMap.contains(PlutoAltKey) == true, "contains(const_alt_key&)_const-Pass" );
            TEST( ConstLookupMap.contains(MarsAltKey) == false, "contains(const_alt_key&)_const-Fail" );

            TEST( LookupMap.equal_range("Mars") == MapPairType(BeginIt + 2,BeginIt + 3),
                  "equal_range(const_key_type&)-Pass" );
            TEST( LookupMap.equal_range("Xenia") == MapPairType(EndIt,EndIt),
                  "equal_range(const_key_type&)-Fail" );
            TEST( ConstLookupMap.equal_range("Saturn") == ConstMapPairType(ConstBeginIt + 2,ConstBeginIt + 3),
                  "equal_range(const_key_type&)_const-Pass" );
            TEST( ConstLookupMap.equal_range("Mercury") == ConstMapPairType(ConstBeginIt,ConstBeginIt),
                  "equal_range(const_key_type&)_const-Fail" );
            TEST( LookupMap.equal_range(MarsAltKey) == MapPairType(BeginIt + 2,BeginIt + 3),
                  "equal_range(const_alt_key&)-Pass" );
            TEST( LookupMap.equal_range(AgniaAltKey) == MapPairType(BeginIt,BeginIt),
                  "equal_range(const_alt_key&)-Fail" );
            TEST( ConstLookupMap.equal_range(NeptuneAltKey) == ConstMapPairType(ConstBeginIt,ConstBeginIt + 1),
                  "equal_range(const_alt_key&)_const-Pass" );
            TEST( ConstLookupMap.equal_range(XeniaAltKey) == ConstMapPairType(ConstEndIt,ConstEndIt),
                  "equal_range(const_alt_key&)_const-Fail" );

            TEST( LookupMap.lower_bound("Agnia") == BeginIt,
                  "lower_bound(const_key_type&)-Start" );
            TEST( LookupMap.lower_bound("Earth") == BeginIt,
                  "lower_bound(const_key_type&)-Middle" );
            TEST( ConstLookupMap.lower_bound("Agnia") == ConstBeginIt,
                  "lower_bound(const_key_type&)_const-Start" );
            TEST( ConstLookupMap.lower_bound("Pluto") == ConstBeginIt + 1,
                  "lower_bound(const_key_type&)_const-Middle" );
            TEST( LookupMap.lower_bound(AgniaAltKey) == BeginIt,
                  "lower_bound(const_alt_key&)-Start" );
            TEST( LookupMap.lower_bound(MarsAltKey) == BeginIt + 2,
                  "lower_bound(const_alt_key&)-Middle" );
            TEST( ConstLookupMap.lower_bound(AgniaAltKey) == ConstBeginIt,
                  "lower_bound(const_alt_key&)_const-Start" );
            TEST( ConstLookupMap.lower_bound(UranusAltKey) == ConstBeginIt + 3,
                  "lower_bound(const_alt_key&)_const-Middle" );

            TEST( LookupMap.upper_bound("Mercury") == BeginIt + 4,
                  "upper_bound(const_key_type&)-Middle" );
            TEST( LookupMap.upper_bound("Xenia") == EndIt,
                  "upper_bound(const_key_type&)-End" );
            TEST( ConstLookupMap.upper_bound("Uranus") == ConstBeginIt + 4,
                  "upper_bound(const_key_type&)_const-Middle" );
            TEST( ConstLookupMap.upper_bound("Xenia") == ConstEndIt,
                  "upper_bound(const_key_type&)_const-End" );
            TEST( LookupMap.upper_bound(JupiterAltKey) == BeginIt + 2,
                  "upper_bound(const_alt_key&)-Middle" );
            TEST( LookupMap.upper_bound(XeniaAltKey) == EndIt,
                  "upper_bound(const_alt_key&)-End" );
            TEST( ConstLookupMap.upper_bound(PlutoAltKey) == ConstBeginIt + 2,
                  "upper_bound(const_alt_key&)_const-Middle" );
            TEST( ConstLookupMap.upper_bound(XeniaAltKey) == ConstEndIt,
                  "upper_bound(const_alt_key&)_const-End" );
        }//Lookup

        {//Sequence Modifiers
            using TestMapType = FlatMap<int,std::string>;
            using TestMapValue = TestMapType::value_type;
            using TestListType = std::list<TestMapValue>;
            using TestInitListType = std::initializer_list<TestMapValue>;
            using InsertPairResult = std::pair<TestMapType::iterator,bool>;
            TestMapType EmptyMap;

            TestMapType CopyInsertMap = { {1,"One"}, {3,"Three"} };
            TestMapValue NewCopyInsertValue(2,"Two");
            TestMapValue OldCopyInsertValue(3,"Twelve");
            TEST( CopyInsertMap.size() == 2, "insert(const_value_type&)-BeforeCount" );
            InsertPairResult NewCopyInsertResult = CopyInsertMap.insert(NewCopyInsertValue);
            InsertPairResult OldCopyInsertResult = CopyInsertMap.insert(OldCopyInsertValue);
            TEST( CopyInsertMap.size() == 3, "insert(const_value_type&)-AfterCount" );
            TEST( NewCopyInsertResult.first == CopyInsertMap.begin() + 1, "insert(const_value_type&)-NewResult-First" );
            TEST( NewCopyInsertResult.second == true, "insert(const_value_type&)-NewResult-Second" );
            TEST( OldCopyInsertResult.first == CopyInsertMap.begin() + 2, "insert(const_value_type&)-OldResult-First" );
            TEST( OldCopyInsertResult.second == false, "insert(const_value_type&)-OldResult-Second" );
            TEST( CopyInsertMap.begin()->first == 1, "insert(const_value_type&)-Element1-First" );
            TEST( CopyInsertMap.begin()->second == "One", "insert(const_value_type&)-Element1-Second" );
            TEST( ( CopyInsertMap.begin() + 1 )->first == 2, "insert(const_value_type&)-Element2-First" );
            TEST( ( CopyInsertMap.begin() + 1 )->second == "Two", "insert(const_value_type&)-Element2-Second" );
            TEST( ( CopyInsertMap.begin() + 2 )->first == 3, "insert(const_value_type&)-Element3-First" );
            TEST( ( CopyInsertMap.begin() + 2 )->second == "Three", "insert(const_value_type&)-Element3-Second" );

            TestMapType MoveInsertMap = { {4,"Four"}, {6,"Six"} };
            TestMapValue NewMoveInsertValue(5,"Five");
            TestMapValue OldMoveInsertValue(4,"Sixteen");
            TEST( MoveInsertMap.size() == 2, "insert(value_type&&)-BeforeCount" );
            InsertPairResult NewMoveInsertResult = MoveInsertMap.insert( std::move(NewMoveInsertValue) );
            InsertPairResult OldMoveInsertResult = MoveInsertMap.insert( std::move(OldMoveInsertValue) );
            TEST( MoveInsertMap.size() == 3, "insert(value_type&&)-AfterCount" );
            TEST( NewMoveInsertResult.first == MoveInsertMap.begin() + 1, "insert(value_type&&)-NewResult-First" );
            TEST( NewMoveInsertResult.second == true, "insert(value_type&&)-NewResult-Second" );
            TEST( OldMoveInsertResult.first == MoveInsertMap.begin(), "insert(value_type&&)-OldResult-First" );
            TEST( OldMoveInsertResult.second == false, "insert(value_type&&)-OldResult-Second" );
            TEST( MoveInsertMap.begin()->first == 4, "insert(value_type&&)-Element1-First" );
            TEST( MoveInsertMap.begin()->second == "Four", "insert(value_type&&)-Element1-Second" );
            TEST( ( MoveInsertMap.begin() + 1 )->first == 5, "insert(value_type&&)-Element2-First" );
            TEST( ( MoveInsertMap.begin() + 1 )->second == "Five", "insert(value_type&&)-Element2-Second" );
            TEST( ( MoveInsertMap.begin() + 2 )->first == 6, "insert(value_type&&)-Element3-First" );
            TEST( ( MoveInsertMap.begin() + 2 )->second == "Six", "insert(value_type&&)-Element3-Second" );
            TEST( NewMoveInsertValue.second.empty(), "insert(value_type&&)-VerifyMove-New" );
            TEST( !OldMoveInsertValue.second.empty(), "insert(value_type&&)-VerifyMove-Old" );

            TestMapType HintCopyInsertMap;
            TestMapValue FirstCopyHintValue(1,"Mercury");
            TestMapValue SecondCopyHintValue(2,"Venus");
            TestMapValue ThirdCopyHintValue(3,"Earth");
            TestMapValue FourthCopyHintValue(4,"Mars");
            TestMapValue FifthCopyHintValue(5,"Asteroids");
            TestMapValue FailCopyHintValue(3,"Earf");
            TestMapType::iterator CopyHint = HintCopyInsertMap.begin();
            TestMapType::iterator CopyHintResult = HintCopyInsertMap.insert(CopyHint,SecondCopyHintValue);
            TEST( CopyHintResult == HintCopyInsertMap.begin(),
                  "insert(const_iterator,const_value_type&)-EmptyInsert-Position" );
            TEST( CopyHintResult->first == 2,
                  "insert(const_iterator,const_value_type&)-EmptyInsert-First" );
            TEST( CopyHintResult->second == "Venus",
                  "insert(const_iterator,const_value_type&)-EmptyInsert-Second" );
            CopyHint = HintCopyInsertMap.begin();
            CopyHintResult = HintCopyInsertMap.insert(CopyHint,FirstCopyHintValue);
            TEST( CopyHintResult == HintCopyInsertMap.begin(),
                  "insert(const_iterator,const_value_type&)-BeginInsert-Position" );
            TEST( CopyHintResult->first == 1,
                  "insert(const_iterator,const_value_type&)-BeginInsert-First" );
            TEST( CopyHintResult->second == "Mercury",
                  "insert(const_iterator,const_value_type&)-BeginInsert-Second" );
            CopyHint = HintCopyInsertMap.end();
            CopyHintResult = HintCopyInsertMap.insert(CopyHint,FifthCopyHintValue);
            TEST( CopyHintResult == HintCopyInsertMap.begin() + 2,
                  "insert(const_iterator,const_value_type&)-EndInsert-Position" );
            TEST( CopyHintResult->first == 5,
                  "insert(const_iterator,const_value_type&)-EndInsert-First" );
            TEST( CopyHintResult->second == "Asteroids",
                  "insert(const_iterator,const_value_type&)-EndInsert-Second" );
            CopyHint = HintCopyInsertMap.begin() + 2;
            CopyHintResult = HintCopyInsertMap.insert(CopyHint,ThirdCopyHintValue);
            TEST( CopyHintResult == HintCopyInsertMap.begin() + 2,
                  "insert(const_iterator,const_value_type&)-MiddleInsert-Position" );
            TEST( CopyHintResult->first == 3,
                  "insert(const_iterator,const_value_type&)-MiddleInsert-First" );
            TEST( CopyHintResult->second == "Earth",
                  "insert(const_iterator,const_value_type&)-MiddleInsert-Second" );
            CopyHint = HintCopyInsertMap.begin();
            CopyHintResult = HintCopyInsertMap.insert(CopyHint,FourthCopyHintValue);
            TEST( CopyHintResult == HintCopyInsertMap.begin() + 3,
                  "insert(const_iterator,const_value_type&)-FallBackInsert-Position" );
            TEST( CopyHintResult->first == 4,
                  "insert(const_iterator,const_value_type&)-FallBackInsert-First" );
            TEST( CopyHintResult->second == "Mars",
                  "insert(const_iterator,const_value_type&)-FallBackInsert-Second" );
            CopyHint = HintCopyInsertMap.begin() + 2;
            CopyHintResult = HintCopyInsertMap.insert(CopyHint,FailCopyHintValue);
            TEST( CopyHintResult == HintCopyInsertMap.begin() + 2,
                  "insert(const_iterator,const_value_type&)-FailInsert-Position" );
            TEST( CopyHintResult->first == 3,
                  "insert(const_iterator,const_value_type&)-FailInsert-First" );
            TEST( CopyHintResult->second == "Earth",
                  "insert(const_iterator,const_value_type&)-FailInsert-Second" );
            // Sequence check the entire container
            TEST( HintCopyInsertMap.size() == 5,
                  "insert(const_iterator,const_value_type&)-FinalSize" );
            TEST( HintCopyInsertMap.begin()->first == 1,
                  "insert(const_iterator,const_value_type&)-Element1-First" );
            TEST( HintCopyInsertMap.begin()->second == "Mercury",
                  "insert(const_iterator,const_value_type&)-Element1-Second" );
            TEST( ( HintCopyInsertMap.begin() + 1 )->first == 2,
                  "insert(const_iterator,const_value_type&)-Element2-First" );
            TEST( ( HintCopyInsertMap.begin() + 1 )->second == "Venus",
                  "insert(const_iterator,const_value_type&)-Element2-Second" );
            TEST( ( HintCopyInsertMap.begin() + 2 )->first == 3,
                  "insert(const_iterator,const_value_type&)-Element3-First" );
            TEST( ( HintCopyInsertMap.begin() + 2 )->second == "Earth",
                  "insert(const_iterator,const_value_type&)-Element3-Second" );
            TEST( ( HintCopyInsertMap.begin() + 3 )->first == 4,
                  "insert(const_iterator,const_value_type&)-Element4-First" );
            TEST( ( HintCopyInsertMap.begin() + 3 )->second == "Mars",
                  "insert(const_iterator,const_value_type&)-Element4-Second" );
            TEST( ( HintCopyInsertMap.begin() + 4 )->first == 5,
                  "insert(const_iterator,const_value_type&)-Element5-First" );
            TEST( ( HintCopyInsertMap.begin() + 4 )->second == "Asteroids",
                  "insert(const_iterator,const_value_type&)-Element5-Second" );

            TestMapType HintMoveInsertMap;
            TestMapValue FirstMoveHintValue(5,"Jupiter");
            TestMapValue SecondMoveHintValue(6,"Saturn");
            TestMapValue ThirdMoveHintValue(7,"Uranus");
            TestMapValue FourthMoveHintValue(8,"Neptune");
            TestMapValue FifthMoveHintValue(9,"Pluto");
            TestMapValue FailMoveHintValue(9,"Planet X");
            TestMapType::iterator MoveHint = HintMoveInsertMap.begin();
            TestMapType::iterator MoveHintResult = HintMoveInsertMap.insert(MoveHint,std::move(ThirdMoveHintValue));
            TEST( MoveHintResult == HintMoveInsertMap.begin(),
                  "insert(const_iterator,value_type&&)-EmptyInsert-Position" );
            TEST( MoveHintResult->first == 7,
                  "insert(const_iterator,value_type&&)-EmptyInsert-First" );
            TEST( MoveHintResult->second == "Uranus",
                  "insert(const_iterator,value_type&&)-EmptyInsert-Second" );
            MoveHint = HintMoveInsertMap.begin();
            MoveHintResult = HintMoveInsertMap.insert(MoveHint,std::move(FirstMoveHintValue));
            TEST( MoveHintResult == HintMoveInsertMap.begin(),
                  "insert(const_iterator,value_type&&)-BeginInsert-Position" );
            TEST( MoveHintResult->first == 5,
                  "insert(const_iterator,value_type&&)-BeginInsert-First" );
            TEST( MoveHintResult->second == "Jupiter",
                  "insert(const_iterator,value_type&&)-BeginInsert-Second" );
            MoveHint = HintMoveInsertMap.end();
            MoveHintResult = HintMoveInsertMap.insert(MoveHint,std::move(FourthMoveHintValue));
            TEST( MoveHintResult == HintMoveInsertMap.begin() + 2,
                  "insert(const_iterator,value_type&&)-EndInsert-Position" );
            TEST( MoveHintResult->first == 8,
                  "insert(const_iterator,value_type&&)-EndInsert-First" );
            TEST( MoveHintResult->second == "Neptune",
                  "insert(const_iterator,value_type&&)-EndInsert-Second" );
            MoveHint = HintMoveInsertMap.begin() + 1;
            MoveHintResult = HintMoveInsertMap.insert(MoveHint,std::move(SecondMoveHintValue));
            TEST( MoveHintResult == HintMoveInsertMap.begin() + 1,
                  "insert(const_iterator,value_type&&)-MiddleInsert-Position" );
            TEST( MoveHintResult->first == 6,
                  "insert(const_iterator,value_type&&)-MiddleInsert-First" );
            TEST( MoveHintResult->second == "Saturn",
                  "insert(const_iterator,value_type&&)-MiddleInsert-Second" );
            MoveHint = HintMoveInsertMap.begin();
            MoveHintResult = HintMoveInsertMap.insert(MoveHint,std::move(FifthMoveHintValue));
            TEST( MoveHintResult == HintMoveInsertMap.begin() + 4,
                  "insert(const_iterator,value_type&&)-FallBackInsert-Position" );
            TEST( MoveHintResult->first == 9,
                  "insert(const_iterator,value_type&&)-FallBackInsert-First" );
            TEST( MoveHintResult->second == "Pluto",
                  "insert(const_iterator,value_type&&)-FallBackInsert-Second" );
            MoveHint = HintMoveInsertMap.begin() + 4;
            MoveHintResult = HintMoveInsertMap.insert(MoveHint,std::move(FailMoveHintValue));
            TEST( MoveHintResult == HintMoveInsertMap.begin() + 4,
                  "insert(const_iterator,value_type&&)-FailInsert-Position" );
            TEST( MoveHintResult->first == 9,
                  "insert(const_iterator,value_type&&)-FailInsert-First" );
            TEST( MoveHintResult->second == "Pluto",
                  "insert(const_iterator,value_type&&)-FailInsert-Second" );
            // Sequence check the entire container
            TEST( HintMoveInsertMap.size() == 5,
                  "insert(const_iterator,value_type&&)-FinalSize" );
            TEST( HintMoveInsertMap.begin()->first == 5,
                  "insert(const_iterator,value_type&&)-Element1-First" );
            TEST( HintMoveInsertMap.begin()->second == "Jupiter",
                  "insert(const_iterator,value_type&&)-Element1-Second" );
            TEST( ( HintMoveInsertMap.begin() + 1 )->first == 6,
                  "insert(const_iterator,value_type&&)-Element2-First" );
            TEST( ( HintMoveInsertMap.begin() + 1 )->second == "Saturn",
                  "insert(const_iterator,value_type&&)-Element2-Second" );
            TEST( ( HintMoveInsertMap.begin() + 2 )->first == 7,
                  "insert(const_iterator,value_type&&)-Element3-First" );
            TEST( ( HintMoveInsertMap.begin() + 2 )->second == "Uranus",
                  "insert(const_iterator,value_type&&)-Element3-Second" );
            TEST( ( HintMoveInsertMap.begin() + 3 )->first == 8,
                  "insert(const_iterator,value_type&&)-Element4-First" );
            TEST( ( HintMoveInsertMap.begin() + 3 )->second == "Neptune",
                  "insert(const_iterator,value_type&&)-Element4-Second" );
            TEST( ( HintMoveInsertMap.begin() + 4 )->first == 9,
                  "insert(const_iterator,value_type&&)-Element5-First" );
            TEST( ( HintMoveInsertMap.begin() + 4 )->second == "Pluto",
                  "insert(const_iterator,value_type&&)-Element5-Second" );
            TEST( FirstMoveHintValue.second.empty(),
                  "insert(const_iterator,value_type&&)-VerifyMove-First" );
            TEST( SecondMoveHintValue.second.empty(),
                  "insert(const_iterator,value_type&&)-VerifyMove-Second" );
            TEST( ThirdMoveHintValue.second.empty(),
                  "insert(const_iterator,value_type&&)-VerifyMove-Third" );
            TEST( FourthMoveHintValue.second.empty(),
                  "insert(const_iterator,value_type&&)-VerifyMove-Fourth" );
            TEST( FifthMoveHintValue.second.empty(),
                  "insert(const_iterator,value_type&&)-VerifyMove-Fifth" );
            TEST( !FailMoveHintValue.second.empty(),
                  "insert(const_iterator,value_type&&)-VerifyMove-Fail" );

            TestListType RangeInsertList = { {2,"Zwei"}, {6,"Sechs"}, {7,"Seiben"} };
            TestMapType RangeInsertMap = { {1,"Eins"}, {5,"Funf"}, {9,"Neun"} };
            TEST( RangeInsertMap.size() == 3,
                  "insert(ItType,ItType)-BeforeCount" );
            RangeInsertMap.insert(RangeInsertList.begin(),RangeInsertList.end());
            TEST( RangeInsertMap.size() == 6,
                  "insert(ItType,ItType)-AfterCount" );
            TEST( RangeInsertMap.begin()->first == 1,
                  "insert(ItType,ItType)-Element1-First" );
            TEST( RangeInsertMap.begin()->second == "Eins",
                  "insert(ItType,ItType)-Element1-Second" );
            TEST( ( RangeInsertMap.begin() + 1 )->first == 2,
                  "insert(ItType,ItType)-Element2-First" );
            TEST( ( RangeInsertMap.begin() + 1 )->second == "Zwei",
                  "insert(ItType,ItType)-Element2-Second" );
            TEST( ( RangeInsertMap.begin() + 2 )->first == 5,
                  "insert(ItType,ItType)-Element3-First" );
            TEST( ( RangeInsertMap.begin() + 2 )->second == "Funf",
                  "insert(ItType,ItType)-Element3-Second" );
            TEST( ( RangeInsertMap.begin() + 3 )->first == 6,
                  "insert(ItType,ItType)-Element4-First" );
            TEST( ( RangeInsertMap.begin() + 3 )->second == "Sechs",
                  "insert(ItType,ItType)-Element4-Second" );
            TEST( ( RangeInsertMap.begin() + 4 )->first == 7,
                  "insert(ItType,ItType)-Element5-First" );
            TEST( ( RangeInsertMap.begin() + 4 )->second == "Seiben",
                  "insert(ItType,ItType)-Element5-Second" );
            TEST( ( RangeInsertMap.begin() + 5 )->first == 9,
                  "insert(ItType,ItType)-Element6-First" );
            TEST( ( RangeInsertMap.begin() + 5 )->second == "Neun",
                  "insert(ItType,ItType)-Element6-Second" );

            TestMapType InitListInsertMap = { {50074,"Thalassa"}, {117646,"Proteus"} };
            TestInitListType InsertInitList = { {73548,"Larissa"}, {5513818,"Nereid"}, {61953,"Galatea"} };
            TEST( InitListInsertMap.size() == 2, "insert(std::initializer_list<value_type>)-ConstructCount" );
            InitListInsertMap.insert( InsertInitList );
            TEST( InitListInsertMap.size() == 5, "insert(std::initializer_list<value_type>)-ExplicitInsertCount" );
            InitListInsertMap.insert( { {52526,"Despina"}, {48227,"Naiad"}, {354759,"Triton"} } );
            TEST( InitListInsertMap.size() == 8, "insert(std::initializer_list<value_type>)-ImplicitInsertCount" );
            TEST( InitListInsertMap.begin()->first == 48227,
                  "insert(std::initializer_list<value_type>)-Element1-First" );
            TEST( InitListInsertMap.begin()->second == "Naiad",
                  "insert(std::initializer_list<value_type>)-Element1-Second" );
            TEST( ( InitListInsertMap.begin() + 1 )->first == 50074,
                  "insert(std::initializer_list<value_type>)-Element2-First" );
            TEST( ( InitListInsertMap.begin() + 1 )->second == "Thalassa",
                  "insert(std::initializer_list<value_type>)-Element2-Second" );
            TEST( ( InitListInsertMap.begin() + 2 )->first == 52526,
                  "insert(std::initializer_list<value_type>)-Element3-First" );
            TEST( ( InitListInsertMap.begin() + 2 )->second == "Despina",
                  "insert(std::initializer_list<value_type>)-Element3-Second" );
            TEST( ( InitListInsertMap.begin() + 3 )->first == 61953,
                  "insert(std::initializer_list<value_type>)-Element4-First" );
            TEST( ( InitListInsertMap.begin() + 3 )->second == "Galatea",
                  "insert(std::initializer_list<value_type>)-Element4-Second" );
            TEST( ( InitListInsertMap.begin() + 4 )->first == 73548,
                  "insert(std::initializer_list<value_type>)-Element5-First" );
            TEST( ( InitListInsertMap.begin() + 4 )->second == "Larissa",
                  "insert(std::initializer_list<value_type>)-Element5-Second" );
            TEST( ( InitListInsertMap.begin() + 5 )->first == 117646,
                  "insert(std::initializer_list<value_type>)-Element6-First" );
            TEST( ( InitListInsertMap.begin() + 5 )->second == "Proteus",
                  "insert(std::initializer_list<value_type>)-Element6-Second" );
            TEST( ( InitListInsertMap.begin() + 6 )->first == 354759,
                  "insert(std::initializer_list<value_type>)-Element7-First" );
            TEST( ( InitListInsertMap.begin() + 6 )->second == "Triton",
                  "insert(std::initializer_list<value_type>)-Element7-Second" );
            TEST( ( InitListInsertMap.begin() + 7 )->first == 5513818,
                  "insert(std::initializer_list<value_type>)-Element8-First" );
            TEST( ( InitListInsertMap.begin() + 7 )->second == "Nereid",
                  "insert(std::initializer_list<value_type>)-Element8-Second" );

            TestMapType EmplaceMap = { {7,"Seven"}, {9,"Nine"} };
            TEST( EmplaceMap.size() == 2, "emplace(ArgTypes&&...)-BeforeCount" );
            InsertPairResult NewEmplaceResult = EmplaceMap.emplace(8,"Eight");
            InsertPairResult OldEmplaceResult = EmplaceMap.emplace(7,"Fourteen");
            TEST( EmplaceMap.size() == 3, "emplace(ArgTypes&&...)-AfterCount" );
            TEST( NewEmplaceResult.first == EmplaceMap.begin() + 1, "emplace(ArgTypes&&...)-NewResult-First" );
            TEST( NewEmplaceResult.second == true, "emplace(ArgTypes&&...)-NewResult-Second" );
            TEST( OldEmplaceResult.first == EmplaceMap.begin(), "emplace(ArgTypes&&...)-OldResult-First" );
            TEST( OldEmplaceResult.second == false, "emplace(ArgTypes&&...)-OldResult-Second" );
            TEST( EmplaceMap.begin()->first == 7, "emplace(ArgTypes&&...)-Element1-First" );
            TEST( EmplaceMap.begin()->second == "Seven", "emplace(ArgTypes&&...)-Element1-Second" );
            TEST( ( EmplaceMap.begin() + 1 )->first == 8, "emplace(ArgTypes&&...)-Element2-First" );
            TEST( ( EmplaceMap.begin() + 1 )->second == "Eight", "emplace(ArgTypes&&...)-Element2-Second" );
            TEST( ( EmplaceMap.begin() + 2 )->first == 9, "emplace(ArgTypes&&...)-Element3-First" );
            TEST( ( EmplaceMap.begin() + 2 )->second == "Nine", "emplace(ArgTypes&&...)-Element3-Second" );

            TestMapType HintEmplaceMap;
            //TestMapValue FirstEmplaceHintValue(129390,"Miranda");
            //TestMapValue SecondEmplaceHintValue(191020,"Ariel");
            //TestMapValue ThirdEmplaceHintValue(266300,"Umbriel");
            //TestMapValue FourthEmplaceHintValue(435910,"Titania");
            //TestMapValue FifthEmplaceHintValue(583520,"Oberon");
            //TestMapValue FailEmplaceHintValue(129390,"Earf");
            TestMapType::iterator EmplaceHint = HintEmplaceMap.begin();
            TestMapType::iterator EmplaceHintResult = HintEmplaceMap.emplace_hint(EmplaceHint,191020,"Ariel");
            TEST( EmplaceHintResult == HintEmplaceMap.begin(),
                  "emplace_hint(const_iterator,ArgTypes&&...)-EmptyInsert-Position" );
            TEST( EmplaceHintResult->first == 191020,
                  "emplace_hint(const_iterator,ArgTypes&&...)-EmptyInsert-First" );
            TEST( EmplaceHintResult->second == "Ariel",
                  "emplace_hint(const_iterator,ArgTypes&&...)-EmptyInsert-Second" );
            EmplaceHint = HintEmplaceMap.begin();
            EmplaceHintResult = HintEmplaceMap.emplace_hint(EmplaceHint,129390,"Miranda");
            TEST( EmplaceHintResult == HintEmplaceMap.begin(),
                  "emplace_hint(const_iterator,ArgTypes&&...)-BeginInsert-Position" );
            TEST( EmplaceHintResult->first == 129390,
                  "emplace_hint(const_iterator,ArgTypes&&...)-BeginInsert-First" );
            TEST( EmplaceHintResult->second == "Miranda",
                  "emplace_hint(const_iterator,ArgTypes&&...)-BeginInsert-Second" );
            EmplaceHint = HintEmplaceMap.end();
            EmplaceHintResult = HintEmplaceMap.emplace_hint(EmplaceHint,583520,"Oberon");
            TEST( EmplaceHintResult == HintEmplaceMap.begin() + 2,
                  "emplace_hint(const_iterator,ArgTypes&&...)-EndInsert-Position" );
            TEST( EmplaceHintResult->first == 583520,
                  "emplace_hint(const_iterator,ArgTypes&&...)-EndInsert-First" );
            TEST( EmplaceHintResult->second == "Oberon",
                  "emplace_hint(const_iterator,ArgTypes&&...)-EndInsert-Second" );
            EmplaceHint = HintEmplaceMap.begin() + 2;
            EmplaceHintResult = HintEmplaceMap.emplace_hint(EmplaceHint,266300,"Umbriel");
            TEST( EmplaceHintResult == HintEmplaceMap.begin() + 2,
                  "emplace_hint(const_iterator,ArgTypes&&...)-MiddleInsert-Position" );
            TEST( EmplaceHintResult->first == 266300,
                  "emplace_hint(const_iterator,ArgTypes&&...)-MiddleInsert-First" );
            TEST( EmplaceHintResult->second == "Umbriel",
                  "emplace_hint(const_iterator,ArgTypes&&...)-MiddleInsert-Second" );
            EmplaceHint = HintEmplaceMap.begin();
            EmplaceHintResult = HintEmplaceMap.emplace_hint(EmplaceHint,435910,"Titania");
            TEST( EmplaceHintResult == HintEmplaceMap.begin() + 3,
                  "emplace_hint(const_iterator,ArgTypes&&...)-FallBackInsert-Position" );
            TEST( EmplaceHintResult->first == 435910,
                  "emplace_hint(const_iterator,ArgTypes&&...)-FallBackInsert-First" );
            TEST( EmplaceHintResult->second == "Titania",
                  "emplace_hint(const_iterator,ArgTypes&&...)-FallBackInsert-Second" );
            EmplaceHint = HintEmplaceMap.begin() + 2;
            EmplaceHintResult = HintEmplaceMap.emplace_hint(EmplaceHint,129390,"Earf");
            TEST( EmplaceHintResult == HintEmplaceMap.begin(),
                  "emplace_hint(const_iterator,ArgTypes&&...)-FailInsert-Position" );
            TEST( EmplaceHintResult->first == 129390,
                  "emplace_hint(const_iterator,ArgTypes&&...)-FailInsert-First" );
            TEST( EmplaceHintResult->second == "Miranda",
                  "emplace_hint(const_iterator,ArgTypes&&...)-FailInsert-Second" );
            // Sequence check the entire container
            TEST( HintEmplaceMap.size() == 5,
                  "emplace_hint(const_iterator,ArgTypes&&...)-FinalSize" );
            TEST( HintEmplaceMap.begin()->first == 129390,
                  "emplace_hint(const_iterator,ArgTypes&&...)-Element1-First" );
            TEST( HintEmplaceMap.begin()->second == "Miranda",
                  "emplace_hint(const_iterator,ArgTypes&&...)-Element1-Second" );
            TEST( ( HintEmplaceMap.begin() + 1 )->first == 191020,
                  "emplace_hint(const_iterator,ArgTypes&&...)-Element2-First" );
            TEST( ( HintEmplaceMap.begin() + 1 )->second == "Ariel",
                  "emplace_hint(const_iterator,ArgTypes&&...)-Element2-Second" );
            TEST( ( HintEmplaceMap.begin() + 2 )->first == 266300,
                  "emplace_hint(const_iterator,ArgTypes&&...)-Element3-First" );
            TEST( ( HintEmplaceMap.begin() + 2 )->second == "Umbriel",
                  "emplace_hint(const_iterator,ArgTypes&&...)-Element3-Second" );
            TEST( ( HintEmplaceMap.begin() + 3 )->first == 435910,
                  "emplace_hint(const_iterator,ArgTypes&&...)-Element4-First" );
            TEST( ( HintEmplaceMap.begin() + 3 )->second == "Titania",
                  "emplace_hint(const_iterator,ArgTypes&&...)-Element4-Second" );
            TEST( ( HintEmplaceMap.begin() + 4 )->first == 583520,
                  "emplace_hint(const_iterator,ArgTypes&&...)-Element5-First" );
            TEST( ( HintEmplaceMap.begin() + 4 )->second == "Oberon",
                  "emplace_hint(const_iterator,ArgTypes&&...)-Element5-Second" );

            TestMapType SwapSrcMap = { {10,"Ten"}, {20,"Twenty"}, {30,"Thirty"} };
            TestMapType SwapDestMap = { {40,"Forty"}, {50,"Fifty"} };
            SwapDestMap.swap(SwapSrcMap);
            TEST( SwapSrcMap.size() == 2, "swap(SelfType&)-SrcCount" );
            TEST( SwapDestMap.size() == 3, "swap(SelfType&)-DestCount" );
            TEST( SwapSrcMap.begin()->first == 40, "swap(SelfType&)-SrcMap-Element1-First" );
            TEST( SwapSrcMap.begin()->second == "Forty", "swap(SelfType&)-SrcMap-Element1-Second" );
            TEST( ( SwapSrcMap.begin() + 1 )->first == 50, "swap(SelfType&)-SrcMap-Element2-First" );
            TEST( ( SwapSrcMap.begin() + 1 )->second == "Fifty", "swap(SelfType&)-SrcMap-Element2-Second" );
            TEST( SwapDestMap.begin()->first == 10, "swap(SelfType&)-DestMap-Element1-First" );
            TEST( SwapDestMap.begin()->second == "Ten", "swap(SelfType&)-DestMap-Element1-Second" );
            TEST( ( SwapDestMap.begin() + 1 )->first == 20, "swap(SelfType&)-DestMap-Element2-First" );
            TEST( ( SwapDestMap.begin() + 1 )->second == "Twenty", "swap(SelfType&)-DestMap-Element2-Second" );
            TEST( ( SwapDestMap.begin() + 2 )->first == 30, "swap(SelfType&)-DestMap-Element3-First" );
            TEST( ( SwapDestMap.begin() + 2 )->second == "Thirty", "swap(SelfType&)-DestMap-Element3-Second" );

            TestMapType EraseMap = { {4,"Cuatro"}, {3,"Tres"}, {2,"Dos"}, {1,"Uno"} };
            TEST( EraseMap.size() == 4, "erase((const_)iterator)-BeforeSize" );
            TestMapType::iterator FirstErase = EraseMap.begin() + 1;
            TestMapType::iterator FirstEraseResult = EraseMap.erase(FirstErase);
            TEST( EraseMap.size() == 3, "erase((const_)iterator)-AfterFirstEraseSize" );
            TEST( FirstEraseResult == EraseMap.begin() + 1 , "erase((const_)iterator)-FirstEraseResultPosition" );
            TestMapType::const_iterator SecondErase = EraseMap.begin() + 2;
            TestMapType::const_iterator SecondEraseResult = EraseMap.erase(SecondErase);
            TEST( EraseMap.size() == 2, "erase((const_)iterator)-AfterSecondEraseSize" );
            TEST( SecondEraseResult == EraseMap.begin() + 2 , "erase((const_)iterator)-SecondEraseResultPosition" );
            TEST( EraseMap.begin()->first == 1 , "erase((const_)iterator)-Element1-First" );
            TEST( EraseMap.begin()->second == "Uno" , "erase((const_)iterator)-Element1-Second" );
            TEST( ( EraseMap.begin() + 1 )->first == 3 , "erase((const_)iterator)-Element2-First" );
            TEST( ( EraseMap.begin() + 1 )->second == "Tres" , "erase((const_)iterator)-Element2-Second" );

            TestMapType RangeEraseMap = { {15,"30"}, {30,"60"}, {45,"90"}, {60,"120"}, {75,"150"}, {90,"180"} };
            TEST( RangeEraseMap.size() == 6, "erase(const_iterator,const_iterator)-BeforeCount" );
            TestMapType::iterator RangeRetIt = RangeEraseMap.erase(RangeEraseMap.begin() + 2,RangeEraseMap.begin() + 5);
            TEST( RangeEraseMap.size() == 3, "erase(const_iterator,const_iterator)-AfterCount" );
            TEST( RangeRetIt == RangeEraseMap.begin() + 2, "erase(const_iterator,const_iterator)-ReturnPosition" );
            TEST( RangeEraseMap.begin()->first == 15, "erase(const_iterator,const_iterator)-Element1-First" );
            TEST( RangeEraseMap.begin()->second == "30", "erase(const_iterator,const_iterator)-Element1-Second" );
            TEST( ( RangeEraseMap.begin() + 1 )->first == 30, "erase(const_iterator,const_iterator)-Element2-First" );
            TEST( ( RangeEraseMap.begin() + 1 )->second == "60", "erase(const_iterator,const_iterator)-Element2-Second" );
            TEST( ( RangeEraseMap.begin() + 2 )->first == 90, "erase(const_iterator,const_iterator)-Element3-First" );
            TEST( ( RangeEraseMap.begin() + 2 )->second == "180", "erase(const_iterator,const_iterator)-Element3-Second" );

            TestMapType KeyEraseMap = { {7,"Sept"}, {6,"Six"}, {5,"Cinq"} };
            TEST( KeyEraseMap.size() == 3, "erase(const_key_type&)-BeforeCount" );
            TEST( KeyEraseMap.erase(6) == 1, "erase(const_key_type&)-Pass" );
            TEST( KeyEraseMap.size() == 2, "erase(const_key_type&)-AfterCount" );
            TEST( KeyEraseMap.erase(9) == 0, "erase(const_key_type&)-Fail" );
            TEST( KeyEraseMap.begin()->first == 5, "erase(const_key_type&)-Element1-First" );
            TEST( KeyEraseMap.begin()->second == "Cinq", "erase(const_key_type&)-Element1-Second" );
            TEST( ( KeyEraseMap.begin() + 1 )->first == 7, "erase(const_key_type&)-Element2-First" );
            TEST( ( KeyEraseMap.begin() + 1 )->second == "Sept", "erase(const_key_type&)-Element2-Second" );

            TestMapType ClearMap = { {4,"Quatre"}, {3,"Trois"}, {2,"Deux"}, {1,"Un"} };
            TEST( ClearMap.size() == 4, "clear()-BeforeCount" );
            ClearMap.clear();
            TEST( ClearMap.empty(), "clear()-Empty" );
        }//Sequence Modifiers
    }

    /// @brief Since RunAutomaticTests is implemented so is this.
    /// @return Returns true.
    virtual bool HasAutomaticTests() const
        { return true; }
};//flatmaptests

#endif
