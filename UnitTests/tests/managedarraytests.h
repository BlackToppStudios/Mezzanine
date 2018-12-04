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

#include <stdexcept> //only used to throw for TEST_THROW

/// @file
/// @brief This file tests the functionality of the ManagedArray class.

using namespace Mezzanine;
using namespace Mezzanine::Testing;

/// @brief A collection of tests for an array that acts like a vector as much as it can short of allocating memory.
class managedarraytests : public UnitTestGroup
{
public:
    /// @copydoc Mezzanine::Testing::UnitTestGroup::Name
    /// @return Returns a String containing "ManagedArray".
    virtual String Name()
        { return String("ManagedArray"); }

    /// @brief This is called when Automatic tests are run.
    void RunAutomaticTests()
    {
        {//Construction
            ManagedArray<int,5> DefaultTest;
            TEST(DefaultTest.size() == 0 &&
                 DefaultTest.begin() == DefaultTest.end(),
                 "ManagedArray::ManagedArray()");

            ManagedArray<int,10> InitializerTest = { 3, 14, 15, 92, 65, 35 };
            Boole ElementsInitPass = InitializerTest[0] == 3 && InitializerTest[1] == 14 &&
                                     InitializerTest[2] == 15 && InitializerTest[3] == 92 &&
                                     InitializerTest[4] == 65 && InitializerTest[5] == 35;
            TEST(InitializerTest.size() == 6 && ElementsInitPass,
                 "ManagedArray::ManagedArray(std::initializer_list<ElementType>");

            ManagedArray<int,10> CopyTest(InitializerTest);
            Boole ElementsCopyPass = CopyTest[0] == 3 && CopyTest[1] == 14 &&
                                     CopyTest[2] == 15 && CopyTest[3] == 92 &&
                                     CopyTest[4] == 65 && CopyTest[5] == 35;
            TEST(CopyTest.size() == 6 && ElementsCopyPass,
                 "ManagedArray::ManagedArray(const_ManagedArray&");

            ManagedArray<int,15> MoveSourceTest = { 1, 1, 2, 3, 5, 8, 13, 21, 34, 55 };
            ManagedArray<int,15> MoveDestTest( std::move( MoveSourceTest ) );
            Boole ElementsMovePass = MoveDestTest[0] == 1 && MoveDestTest[1] == 1 &&
                                     MoveDestTest[2] == 2 && MoveDestTest[3] == 3 &&
                                     MoveDestTest[4] == 5 && MoveDestTest[5] == 8 &&
                                     MoveDestTest[6] == 13 && MoveDestTest[7] == 21 &&
                                     MoveDestTest[8] == 34 && MoveDestTest[9] == 55;
            TEST(MoveDestTest.size() == 10 && ElementsMovePass,
                 "ManagedArray::ManagedArray(ManagedArray&&)");
        }//Construction

        {//Operators
            ManagedArray<int,12> CopyAssignSource = { 1, 3, 5, 7, 9 };
            ManagedArray<int,12> CopyAssignDest = { 9, 7, 5, 3, 1, 0 };
            CopyAssignDest = CopyAssignSource;
            Boole DestCopyPass = CopyAssignDest[0] == 1 && CopyAssignDest[1] == 3 &&
                                 CopyAssignDest[2] == 5 && CopyAssignDest[3] == 7 &&
                                 CopyAssignDest[4] == 9;
            Boole SourceCopyPass = CopyAssignSource[0] == 1 && CopyAssignSource[1] == 3 &&
                                   CopyAssignSource[2] == 5 && CopyAssignSource[3] == 7 &&
                                   CopyAssignSource[4] == 9;
            Boole CopySizeTest = CopyAssignSource.size() == 5 && CopyAssignDest.size() == 5;
            TEST(CopySizeTest && SourceCopyPass && DestCopyPass,"ManagedArray::operator=(const_ManagedArray&)");

            // Should the source array zero out after a move?
            ManagedArray<int,12> MoveAssignSource = { 9, 7, 5, 3, 1, 0 };
            ManagedArray<int,12> MoveAssignDest = { 1, 3, 5, 7, 9 };
            MoveAssignDest = std::move( MoveAssignSource );
            Boole DestMovePass = MoveAssignDest[0] == 9 && MoveAssignDest[1] == 7 &&
                                 MoveAssignDest[2] == 5 && MoveAssignDest[3] == 3 &&
                                 MoveAssignDest[4] == 1 && MoveAssignDest[5] == 0;
            Boole SourceMovePass = MoveAssignSource[0] == 9 && MoveAssignSource[1] == 7 &&
                                   MoveAssignSource[2] == 5 && MoveAssignSource[3] == 3 &&
                                   MoveAssignSource[4] == 1 && MoveAssignSource[5] == 0;
            Boole MoveSizeTest = MoveAssignSource.size() == 6 && MoveAssignDest.size() == 6;
            TEST(MoveSizeTest && SourceMovePass && DestMovePass,"ManagedArray::operator=(ManagedArray&&)");
        }//Operators

        {//Iterators
            using FirstArrayType = ManagedArray<int,10>;
            using SecondArrayType = ManagedArray<int,15>;
            FirstArrayType TestArray = { 3, 14, 15, 92, 65, 35, 89, 79, 32, 38 };
            const SecondArrayType ConstTestArray = { 1, 3, 6, 10, 15, 21, 28, 36, 45, 55, 66, 78, 91, 105, 120 };

            FirstArrayType::iterator BeginIter = TestArray.begin();
            FirstArrayType::const_iterator CBeginIter = TestArray.cbegin();
            SecondArrayType::const_iterator ConstBeginIter = ConstTestArray.begin();
            TEST(*BeginIter == 3,"ManagedArray::begin()");
            TEST(*CBeginIter == 3,"ManagedArray::cbegin()_const");
            TEST(*ConstBeginIter == 1,"ManagedArray::begin()_const");

            FirstArrayType::iterator EndIter = TestArray.end();
            FirstArrayType::const_iterator CEndIter = TestArray.cend();
            SecondArrayType::const_iterator ConstEndIter = ConstTestArray.end();
            TEST(EndIter - BeginIter == 10,"ManagedArray::end()");
            TEST(CEndIter - CBeginIter == 10,"ManagedArray::cend()_const");
            TEST(ConstEndIter - ConstBeginIter == 15,"ManagedArray::end()_const");

            FirstArrayType::reverse_iterator BeginRevIter = TestArray.rbegin();
            FirstArrayType::const_reverse_iterator CBeginRevIter = TestArray.crbegin();
            SecondArrayType::const_reverse_iterator ConstBeginRevIter = ConstTestArray.rbegin();
            TEST(*BeginRevIter == 38,"ManagedArray::rbegin()");
            TEST(*CBeginRevIter == 38,"ManagedArray::crbegin()_const");
            TEST(*ConstBeginRevIter == 120,"ManagedArray::rbegin()_const");

            FirstArrayType::reverse_iterator EndRevIter = TestArray.rend();
            FirstArrayType::const_reverse_iterator CEndRevIter = TestArray.crend();
            SecondArrayType::const_reverse_iterator ConstEndRevIter = ConstTestArray.rend();
            TEST(EndRevIter - BeginRevIter == 10,"ManagedArray::rend()");
            TEST(CEndRevIter - CBeginRevIter == 10,"ManagedArray::crend()_const");
            TEST(ConstEndRevIter - ConstBeginRevIter == 15,"ManagedArray::rend()_const");
        }//Iterators

        {//Query and Access
            ManagedArray<int,8> EmptyArray;
            ManagedArray<int,16> QueryArray = { 0, 1, 4, 9, 16, 25, 36, 49, 64, 81 };
            const ManagedArray<int,24> ConstQueryArray = { 1, 8, 27, 64, 125, 216, 343, 512 };

            TEST(EmptyArray.size() == 0 &&
                 QueryArray.size() == 10 &&
                 ConstQueryArray.size() == 8,
                 "ManagedArray::size()_const");

            TEST(EmptyArray.capacity() == 8 &&
                 QueryArray.capacity() == 16 &&
                 ConstQueryArray.capacity() == 24,
                 "ManagedArray::capacity()_const");

            TEST(EmptyArray.max_size() == 8 &&
                 QueryArray.max_size() == 16 &&
                 ConstQueryArray.max_size() == 24,
                 "ManagedArray::max_size()_const");

            TEST(EmptyArray.empty() == true &&
                 QueryArray.empty() == false &&
                 ConstQueryArray.empty() == false,
                 "ManagedArray::empty()_const");

            Boole ArrayAccessPass = QueryArray[0] == 0 && QueryArray[2] == 4 &&
                                    QueryArray[4] == 16 && QueryArray[6] == 36 &&
                                    QueryArray[8] == 64;
            TEST(ArrayAccessPass,"ManagedArray::operator[](size_t)");

            Boole ConstArrayAccessPass = ConstQueryArray[0] == 1 && ConstQueryArray[2] == 27 &&
                                         ConstQueryArray[4] == 125 && ConstQueryArray[6] == 343;
            TEST(ConstArrayAccessPass,"ManagedArray::operator[](size_t)_const");

            Boole AtAccessPass = QueryArray[1] == 1 && QueryArray[3] == 9 &&
                                 QueryArray[5] == 25 && QueryArray[7] == 49 &&
                                 QueryArray[9] == 81;
            TEST(AtAccessPass,"ManagedArray::at(size_t)");

            Boole ConstAtAccessPass = ConstQueryArray[1] == 8 && ConstQueryArray[3] == 64 &&
                                      ConstQueryArray[5] == 216 && ConstQueryArray[7] == 512;
            TEST(ConstAtAccessPass,"ManagedArray::at(size_t)_const");

            TEST_THROW(std::out_of_range,QueryArray.at(100),"ManagedArray::at(size_t)-Throw");
            TEST_THROW(std::out_of_range,ConstQueryArray.at(500),"ManagedArray::at(size_t)_const-Throw");

            Boole ContainsPass = QueryArray.contains(36);
            Boole ContainsFail = !QueryArray.contains(50);
            TEST(ContainsPass && ContainsFail,"ManagedArray::contains(const_value_type&)_const");
        }//Query and Access

        {//Sequence Modifiers
            ManagedArray<int,1> FirstPushBack;
            const int PushBackCopyValue = 66;
            FirstPushBack.push_back(PushBackCopyValue);
            TEST(FirstPushBack[0] == 66,"ManagedArray::push_back(const_value_type&)");
            TEST_THROW(std::out_of_range,
                       FirstPushBack.push_back(99),
                       "ManagedArray::push_back(const_value_type&)-Throw");

            ManagedArray<int,1> SecondPushBack;
            SecondPushBack.push_back( std::move(66) );
            TEST(SecondPushBack[0] == 66,"ManagedArray::push_back(value_type&&)");
            TEST_THROW(std::out_of_range,
                       SecondPushBack.push_back( std::move(99) ),
                       "ManagedArray::push_back(value_type&&)-Throw");

            ManagedArray<int,10> AppendArray = { 1, 3, 5 };
            std::vector<int> AppendSource = { 7, 9, 11};
            AppendArray.append(AppendSource.begin(),AppendSource.end());
            Boole AppendPass = AppendArray[0] == 1 && AppendArray[1] == 3 &&
                               AppendArray[2] == 5 && AppendArray[3] == 7 &&
                               AppendArray[4] == 9 && AppendArray[5] == 11;
            TEST(AppendPass,"ManagedArray::append(InputIterator,InputIterator)");
            std::vector<int> ThrowAppendSource = { 1, 1, 1, 1, 1, 1 };
            TEST_THROW(std::out_of_range,
                       AppendArray.append(ThrowAppendSource.begin(),ThrowAppendSource.end()),
                       "ManagedArray::append(InputIterator,InputIterator)-Throw");

            ManagedArray<int,4> EmplaceArray = { 1, 3, 7 };
            EmplaceArray.emplace(EmplaceArray.begin() + 1, 3 );
            Boole EmplacePass = EmplaceArray[0] == 1 && EmplaceArray[1] == 3 &&
                                EmplaceArray[2] == 3 && EmplaceArray[3] == 7;
            TEST(EmplacePass,"ManagedArray::emplace(const_iterator,ArgTypes&&...)");
            TEST_THROW(std::out_of_range,
                       EmplaceArray.emplace(EmplaceArray.begin(),1337),
                       "ManagedArray::emplace(const_iterator,ArgTypes&&...)-Throw");

            ManagedArray<String,4> EmplaceBackArray = { "This ", "is ", "a " };
            EmplaceBackArray.emplace_back("test.");
            Boole EmplaceBackPass = EmplaceBackArray[0] == "This " && EmplaceBackArray[1] == "is " &&
                                    EmplaceBackArray[2] == "a " && EmplaceBackArray[3] == "test.";
            TEST(EmplaceBackPass,"ManagedArray::emplace_back(ArgTypes&&...)");
            TEST_THROW(std::out_of_range,
                       EmplaceBackArray.emplace_back("NOT!"),
                       "ManagedArray::emplace_back(ArgTypes&&...)-Throw");

            ManagedArray<int,3> InsertCopyArray = { 4, 6 };
            const int InsertCopyValue = 5;
            InsertCopyArray.insert( InsertCopyArray.begin() + 1, InsertCopyValue );
            Boole InsertCopyPass = InsertCopyArray[0] == 4 && InsertCopyArray[1] == 5 &&
                                   InsertCopyArray[2] == 6;
            TEST(InsertCopyPass,"ManagedArray::insert(const_iterator,const_value_type&)");
            TEST_THROW(std::out_of_range,
                       InsertCopyArray.insert(InsertCopyArray.begin(),0xDEADBEEF),
                       "ManagedArray::insert(const_iterator,const_value_type&)-Throw");

            ManagedArray<int,3> InsertMoveArray = { 9, 11 };
            InsertMoveArray.insert( InsertMoveArray.begin() + 1, std::move(10) );
            Boole InsertMovePass = InsertMoveArray[0] == 9 && InsertMoveArray[1] == 10 &&
                                   InsertMoveArray[2] == 11;
            TEST(InsertMovePass,"ManagedArray::insert(const_iterator,value_type&&)");
            TEST_THROW(std::out_of_range,
                       InsertMoveArray.insert(InsertMoveArray.begin(),0xBAADC0DE),
                       "ManagedArray::insert(const_iterator,value_type&&)-Throw");

            ManagedArray<int,14> InsertRangeArray = { 0, 55, 89 };//{ 0, 1, 1, 2, 3, 5, 8, 13, 21, 34, 55, 89 };
            ManagedArray<int,4> FirstInsertSource = { 8, 13, 21, 34 };
            ManagedArray<int,5> SecondInsertSource = { 1, 1, 2, 3, 5 };
            InsertRangeArray.insert(InsertRangeArray.begin() + 1,FirstInsertSource.begin(),FirstInsertSource.end());
            Boole FirstInsertRangePass = InsertRangeArray[0] == 0 && InsertRangeArray[1] == 8 &&
                                         InsertRangeArray[2] == 13 && InsertRangeArray[3] == 21 &&
                                         InsertRangeArray[4] == 34 && InsertRangeArray[5] == 55 &&
                                         InsertRangeArray[6] == 89 && InsertRangeArray.size() == 7;
            InsertRangeArray.insert(InsertRangeArray.begin() + 1,SecondInsertSource.begin(),SecondInsertSource.end());
            Boole SecondInsertRangePass = InsertRangeArray[0] == 0 && InsertRangeArray[1] == 1 &&
                                          InsertRangeArray[2] == 1 && InsertRangeArray[3] == 2 &&
                                          InsertRangeArray[4] == 3 && InsertRangeArray[5] == 5 &&
                                          InsertRangeArray[6] == 8 && InsertRangeArray[7] == 13 &&
                                          InsertRangeArray[8] == 21 && InsertRangeArray[9] == 34 &&
                                          InsertRangeArray[10] == 55 && InsertRangeArray[11] == 89 &&
                                          InsertRangeArray.size() == 12;
            TEST(FirstInsertRangePass && SecondInsertRangePass,"ManagedArray::insert(const_iterator,InputIterator,InputIterator)");
            TEST_THROW(std::out_of_range,
                       InsertRangeArray.insert(InsertRangeArray.begin() + 6,FirstInsertSource.begin(),FirstInsertSource.end()),
                       "ManagedArray::insert(const_iterator,InputIterator,InputIterator)-Throw");

            ManagedArray<int,2> PopBackArray = { 5, 10 };
            PopBackArray.pop_back();
            TEST(PopBackArray.size() == 1 && PopBackArray[0] == 5,"ManagedArray::pop_back()");

            ManagedArray<int,3> EraseArray = { 333, 555, 777 };
            EraseArray.erase(EraseArray.begin() + 1);
            Boole ErasePass = EraseArray[0] == 333 && EraseArray[1] == 777 &&
                              EraseArray.size() == 2;
            TEST(ErasePass,"ManagedArray::erase(const_iterator)");
            TEST_THROW(std::out_of_range,
                       EraseArray.erase(EraseArray.begin() + 9),
                       "ManagedArray::erase(const_iterator)-Throw");

            ManagedArray<int,6> EraseRangeArray = { 1, 22, 333, 4444, 55555, 666666 };
            EraseRangeArray.erase(EraseRangeArray.begin() + 2, EraseRangeArray.begin() + 5);
            Boole EraseRangePass = EraseRangeArray[0] == 1 && EraseRangeArray[1] == 22 &&
                                   EraseRangeArray[2] == 666666 && EraseRangeArray.size() == 3;
            TEST(EraseRangePass,"ManagedArray::erase(const_iterator,const_iterator)");
            TEST_THROW(std::out_of_range,
                       EraseRangeArray.erase(EraseRangeArray.begin() + 7, EraseRangeArray.begin() + 9),
                       "ManagedArray::erase(const_iterator,const_iterator)-Throw");

            ManagedArray<int,3> FirstSwapArray = { 123, 234, 345 };
            ManagedArray<int,3> SecondSwapArray = { 567, 678 };
            FirstSwapArray.swap(SecondSwapArray);
            Boole FirstSwapPass = FirstSwapArray[0] == 567 && FirstSwapArray[1] == 678 &&
                                  FirstSwapArray.size() == 2;
            Boole SecondSwapPass = SecondSwapArray[0] == 123 && SecondSwapArray[1] == 234 &&
                                   SecondSwapArray[2] == 345 && SecondSwapArray.size() == 3;
            TEST(FirstSwapPass && SecondSwapPass,"ManagedArray::swap()");

            ManagedArray<int,6> ClearArray = { 1, 2, 3, 4, 5, 6 };
            ClearArray.clear();
            TEST(ClearArray.empty(),"ManagedArray::clear()");
            //TEST(,"ManagedArray::");
        }//Sequence Modifiers
    }

    /// @brief Since RunAutomaticTests is implemented so is this.
    /// @return Returns true.
    virtual bool HasAutomaticTests() const
        { return true; }
};//managedarraytests

#endif
