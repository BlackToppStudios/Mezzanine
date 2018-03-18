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
#ifndef _eventstests_h
#define _eventstests_h

#include "mezztest.h"
#include "event.h"
#include "eventdispatcher.h"
#include "eventhelper.h"
#include "eventid.h"
#include "eventpublisher.h"
#include "eventsubscriber.h"
#include "eventsubscriberbinding.h"
#include "eventsubscriberid.h"
#include "eventsubscriptioncontainer.h"
#include "eventsubscriptionfactory.h"
#include "eventsubscriptiontable.h"
#include "eventsubscriptiontablebase.h"
#include "eventsubscriptiontabletraits.h"

#include <stdexcept> //only used to throw for TEST_THROW
#include <ostream>
#include <memory>

/// @file
/// @brief This file should be used as template/example for building future Unit Tests

using namespace Mezzanine;
using namespace Mezzanine::Testing;

class TestSubscriberBase
{
public:
    using IDType = EventSubscriberID;

    virtual void TestSet(const Whole Set) = 0;
    virtual Whole TestGet() const = 0;
    virtual Whole GetCallCount() const = 0;
    virtual IDType GetID() const = 0;
};

class TestSubscriberImpl : public TestSubscriberBase
{
protected:
    static Whole TotalCallCount;
    Whole Data = 0;
    Whole CallCount = 0;
    IDType ID;
public:
    TestSubscriberImpl() = delete;
    TestSubscriberImpl(IDType SubID) : ID(SubID) {  }
    TestSubscriberImpl(const TestSubscriberImpl& Other) = default;
    TestSubscriberImpl(TestSubscriberImpl&& Other) = default;
    TestSubscriberImpl& operator=(const TestSubscriberImpl& Other) = default;
    TestSubscriberImpl& operator=(TestSubscriberImpl&& Other) = default;

    void TestSet(const Whole Set)
    {
        this->Data = Set;
        ++CallCount;
        ++TotalCallCount;
    }
    Whole TestGet() const
        { return this->Data; }
    Whole GetCallCount() const
        { return this->CallCount; }
    IDType GetID() const
        { return this->ID; }

    static Whole GetTotalCallCount()
        { return TotalCallCount; }
};

Whole TestSubscriberImpl::TotalCallCount = 0;

class TestSubscriberSorter
{
public:
    TestSubscriberSorter() = default;
    TestSubscriberSorter(const TestSubscriberSorter& Other) = default;
    TestSubscriberSorter(TestSubscriberSorter&& Other) = default;

    TestSubscriberSorter& operator=(const TestSubscriberSorter& Other) = default;
    TestSubscriberSorter& operator=(TestSubscriberSorter&& Other) = default;

    Boole operator()(TestSubscriberBase* First, TestSubscriberBase* Second)
        { return First->GetID() < Second->GetID(); }
};

struct NUnETestConfig : public EventSubscriptionTableConfig<TestSubscriberBase*>
{
    // Leave the defaults provided by the base class/template param.
};

struct BUnETestConfig : public EventBindingTableConfig< FunctionSubscriber<EventSubscriberID,void,EventPtr> >
{
    // Leave the defaults provided by the base class/template param.
};

struct NUFSTestConfig : public EventSubscriptionTableConfig<TestSubscriberBase*>
{
    static const SubscriptionContainerType ContainerType = SubscriptionContainerType::SCT_Unsorted_Fixed;
    static const EventDispatcherType DispatcherType = EventDispatcherType::EDT_Silencable;
    static const size_t StorageCount = 3;
};

struct NSnETestConfig : public EventSubscriptionTableConfig<TestSubscriberBase*>
{
    static const SubscriptionContainerType ContainerType = SubscriptionContainerType::SCT_Single;
};

struct BSFQTestConfig : public EventBindingTableConfig< FunctionSubscriber<EventSubscriberID,void,EventPtr> >
{
    static const SubscriptionContainerType ContainerType = SubscriptionContainerType::SCT_Sorted_Fixed;
    static const EventDispatcherType DispatcherType = EventDispatcherType::EDT_Queued;
    static const size_t StorageCount = 3;
};

struct NSoQTestConfig : public EventSubscriptionTableConfig<TestSubscriberBase*>
{
    static const SubscriptionContainerType ContainerType = SubscriptionContainerType::SCT_Sorted;
    static const EventDispatcherType DispatcherType = EventDispatcherType::EDT_Queued;
    using StoragePredicate = TestSubscriberSorter;
};

/// @brief A series of tests for the Event subsystem.
class eventstests : public UnitTestGroup
{
public:
    /// @copydoc Mezzanine::Testing::UnitTestGroup::Name
    /// @return Returns a String containing "BoilerPlate"
    virtual String Name()
        { return String("Events"); }

    /// @brief This is called when Automatic tests are run
    void RunAutomaticTests()
    {
        // Throughout these tests arbitrary numbers are used for testing.  They are arbitrary.
        // They can be replaced with any other value and just work.
        {//EventID
            EventID IntegerID(144);
            TEST(IntegerID.ID == 144,"EventID(const_IDType)");

            HashedString32 IDHashed("TestEventHash");
            EventID HashEventID(IDHashed);
            TEST(HashEventID.ID == IDHashed.GetHash(),"EventID(const_HashedString32&)");

            EventID CopyID(IntegerID);
            TEST(CopyID.ID == 144,"EventID(const_EventID&)");

            EventID MoveSourceID(288);
            EventID MoveDestID(std::move(MoveSourceID));
            TEST(MoveDestID.ID == 288,"EventID(EventID&&)");

            EventID AssignSourceID(576);
            EventID AssignDestID(0);
            AssignDestID = AssignSourceID;
            TEST(AssignDestID.ID == 576,"EventID::operator=(const_EventID&)");

            EventID AssignMoveSourceID(1152);
            EventID AssignMoveDestID(0);
            AssignMoveDestID = std::move(AssignMoveSourceID);
            TEST(AssignMoveDestID.ID == 1152,"EventID::operator=(EventID&&)");

            TEST(EventID(50) == EventID(50),"EventID::operator==(const_EventID&)");
            TEST(EventID(100) != EventID(242),"EventID::operator!=(const_EventID&)");

            StringStream ResultStream;
            EventID StreamID(150);
            ResultStream << StreamID;
            TEST(ResultStream.str() == "150","operator<<(std::ostream&,const_EventID&)");
        }//EventID

        {//EventSubscriberID
            EventSubscriberID IntegerID(144);
            TEST(IntegerID.ID == 144,"EventSubscriberID(const_IDType)");

            EventSubscriberID* SubPtr = &IntegerID;
            EventSubscriberID PointerID(SubPtr);
            TEST(PointerID.ID == reinterpret_cast<EventSubscriberID::IDType>(SubPtr),"EventSubscriberID(PtrType*)");

            EventSubscriberID CopyID(IntegerID);
            TEST(CopyID.ID == 144,"EventSubscriberID(const_EventSubscriberID&)");

            EventSubscriberID MoveSourceID(288);
            EventSubscriberID MoveDestID(std::move(MoveSourceID));
            TEST(MoveDestID.ID == 288,"EventSubscriberID(EventSubscriberID&&)");

            EventSubscriberID AssignSourceID(576);
            EventSubscriberID AssignDestID(0);
            AssignDestID = AssignSourceID;
            TEST(AssignDestID.ID == 576,"EventSubscriberID::operator=(const_EventSubscriberID&)");

            EventSubscriberID AssignMoveSourceID(1152);
            EventSubscriberID AssignMoveDestID(0);
            AssignMoveDestID = std::move(AssignMoveSourceID);
            TEST(AssignMoveDestID.ID == 1152,"EventSubscriberID::operator=(EventSubscriberID&&)");

            TEST(EventSubscriberID(50) == EventSubscriberID(50),"EventSubscriberID::operator==(const_EventSubscriberID&)");
            TEST(EventSubscriberID(100) != EventSubscriberID(242),"EventSubscriberID::operator!=(const_EventSubscriberID&)");

            StringStream ResultStream;
            EventSubscriberID StreamID(150);
            ResultStream << StreamID;
            TEST(ResultStream.str() == "150","operator<<(std::ostream&,const_EventSubscriberID&)");
        }//EventSubscriberID

        {//FunctionSubscriber
            {//SingleArg
                using TestSubscriberType = FunctionSubscriber<EventSubscriberID,void,EventPtr>;

                Whole DispatchCount = 0;
                EventPtr TestArg = std::make_shared<Event>("TestEvent");
                TestSubscriberType::FunctionType TestDelegate([&](EventPtr Args){ ++DispatchCount; });

                TestSubscriberType IntegerSub(144,TestDelegate);
                TEST(IntegerSub.GetID().ID == 144,"FunctionSubscriber(const_IDType,const_FunctionType&)-SingleArg");

                TestSubscriberType CopySub(IntegerSub);
                TEST(CopySub.GetID().ID == 144,"FunctionSubscriber(const_FunctionSubscriber&)-SingleArg");

                TestSubscriberType MoveSourceSub(288,TestDelegate);
                TestSubscriberType MoveDestSub(std::move(MoveSourceSub));
                TEST(MoveDestSub.GetID().ID == 288,"FunctionSubscriber(FunctionSubscriber&&)-SingleArg");

                TestSubscriberType AssignSourceSub(576,TestDelegate);
                TestSubscriberType AssignDestSub(0,TestDelegate);
                AssignDestSub = AssignSourceSub;
                TEST(AssignDestSub.GetID().ID == 576,"FunctionSubscriber::operator=(const_FunctionSubscriber&)-SingleArg");

                TestSubscriberType AssignMoveSourceSub(1152,TestDelegate);
                TestSubscriberType AssignMoveDestSub(0,TestDelegate);
                AssignMoveDestSub = std::move(AssignMoveSourceSub);
                TEST(AssignMoveDestSub.GetID().ID == 1152,"FunctionSubscriber::operator=(FunctionSubscriber&&)-SingleArg");

                TestSubscriberType DispatchSub(2304,TestDelegate);
                DispatchSub(TestArg);
                TEST(DispatchCount == 1,"FunctionSubscriber::DispatchEvent(Args...)_const-SingleArg");
            }//SingleArg

            {//MixedArg
                using TestSubscriberType = FunctionSubscriber<EventSubscriberID,void,const Real&, Whole>;

                Whole DispatchCount = 0;
                Real TestArg1 = 3.1415926;
                Whole TestArg2 = 8675309;
                TestSubscriberType::FunctionType TestDelegate([&](const Real& fn, Whole ln){ ++DispatchCount; });

                TestSubscriberType IntegerSub(144,TestDelegate);
                TEST(IntegerSub.GetID().ID == 144,"FunctionSubscriber(const_IDType,const_FunctionType&)-MixedArg");

                TestSubscriberType CopySub(IntegerSub);
                TEST(CopySub.GetID().ID == 144,"FunctionSubscriber(const_FunctionSubscriber&)-MixedArg");

                TestSubscriberType MoveSourceSub(288,TestDelegate);
                TestSubscriberType MoveDestSub(std::move(MoveSourceSub));
                TEST(MoveDestSub.GetID().ID == 288,"FunctionSubscriber(FunctionSubscriber&&)-MixedArg");

                TestSubscriberType AssignSourceSub(576,TestDelegate);
                TestSubscriberType AssignDestSub(0,TestDelegate);
                AssignDestSub = AssignSourceSub;
                TEST(AssignDestSub.GetID().ID == 576,"FunctionSubscriber::operator=(const_FunctionSubscriber&)-MixedArg");

                TestSubscriberType AssignMoveSourceSub(1152,TestDelegate);
                TestSubscriberType AssignMoveDestSub(0,TestDelegate);
                AssignMoveDestSub = std::move(AssignMoveSourceSub);
                TEST(AssignMoveDestSub.GetID().ID == 1152,"FunctionSubscriber::operator=(FunctionSubscriber&&)-MixedArg");

                TestSubscriberType DispatchSub(2304,TestDelegate);
                DispatchSub(TestArg1,TestArg2);
                TEST(DispatchCount == 1,"FunctionSubscriber::DispatchEvent(Args...)_const-MixedArg");
            }//MixedArg
        }//FunctionSubscriber

        {//EventSubscriberBindingImpl
            using TestTableType = EventSubscriptionTable<EventBindingTableConfig<FunctionSubscriber<EventSubscriberID,void,EventPtr>>>;
            using TestSubscriberType = TestTableType::SubscriberType;
            //using TestBindingType = TestTableType::BindingType;
            //using TestBindingPtrType = TestTableType::BindingPtrType;
            using TestBindingImplType = TestTableType::TraitsType::ActualFactoryType::BindingImplType;
            using TestBindingImplPtrType = TestTableType::TraitsType::ActualFactoryType::BindingImplPtrType;

            {//NoTable
                Whole DispatchCount = 0;
                TestSubscriberType::FunctionType TestDelegate([&](EventPtr Args){ ++DispatchCount; });

                TestBindingImplType IntegerBinding(TestSubscriberType(144,TestDelegate),nullptr);
                TEST(IntegerBinding.GetID().ID == 144 &&
                     IntegerBinding.GetTable() == nullptr,
                     "EventSubscriberBindingImpl::ConstructionAndGetters-NoTable");

                TestBindingImplType MoveBinding(std::move(IntegerBinding));
                TEST(MoveBinding.GetID().ID == 144 &&
                     MoveBinding.GetTable() == nullptr,
                     "EventSubscriberBindingImpl::(EventSubscriberBindingImpl&&)-NoTable");

                TestBindingImplType AssignMoveSourceBind(TestSubscriberType(288,TestDelegate),nullptr);
                TestBindingImplType AssignMoveDestBind(TestSubscriberType(0,TestDelegate),nullptr);
                AssignMoveDestBind = std::move(AssignMoveSourceBind);
                TEST(AssignMoveDestBind.GetID().ID == 288 &&
                     AssignMoveDestBind.GetTable() == nullptr,
                     "EventSubscriberBindingImpl::operator=(EventSubscriberBindingImpl&&)-NoTable");

                TestBindingImplType UtilityTestBinding(TestSubscriberType(576,TestDelegate),nullptr);
                TEST( !UtilityTestBinding.IsSubscribed() ,"EventSubscriberBindingImpl::IsSubscribed()_const-NoTable");
            }//NoTable

            {//Table
                Whole DispatchCount = 0;
                HashedString32 FirstTestTableName("FirstTestTable");
                TestTableType FirstTestTable(FirstTestTableName);
                HashedString32 SecondTestTableName("SecondTestTable");
                TestTableType SecondTestTable(SecondTestTableName);
                TestSubscriberType::FunctionType TestDelegate([&](EventPtr Args){ ++DispatchCount; });

                TestBindingImplPtrType IntegerBinding = std::static_pointer_cast<TestBindingImplType>(FirstTestTable.Subscribe(TestSubscriberType(144,TestDelegate)));
                TEST(IntegerBinding->GetID().ID == 144 &&
                     IntegerBinding->GetTable() == &FirstTestTable,
                     "EventSubscriberBindingImpl::ConstructionAndGetters-Table");

                TestBindingImplPtrType MoveBinding(IntegerBinding);
                TEST(MoveBinding->GetID().ID == 144 &&
                     MoveBinding->GetTable() == &FirstTestTable,
                     "EventSubscriberBindingImpl::(EventSubscriberBindingImpl&&)-Table");

                TestBindingImplPtrType AssignMoveSourceBind = std::static_pointer_cast<TestBindingImplType>(FirstTestTable.Subscribe(TestSubscriberType(288,TestDelegate)));
                TestBindingImplPtrType AssignMoveDestBind = std::static_pointer_cast<TestBindingImplType>(SecondTestTable.Subscribe(TestSubscriberType(578,TestDelegate)));
                *AssignMoveDestBind = std::move(*AssignMoveSourceBind);
                TEST(AssignMoveDestBind->GetID().ID == 288 &&
                     AssignMoveDestBind->GetTable() == &FirstTestTable,
                     "EventSubscriberBindingImpl::operator=(EventSubscriberBindingImpl&&)-Table");

                TestBindingImplPtrType UtilityTestBinding = std::static_pointer_cast<TestBindingImplType>(FirstTestTable.Subscribe(TestSubscriberType(1156,TestDelegate)));
                Boole SubscribedPassTest = UtilityTestBinding->IsSubscribed();

                UtilityTestBinding->UpdateTable(&SecondTestTable);
                TEST(UtilityTestBinding->GetTable() == &SecondTestTable,
                     "EventSubscriberBindingImpl::UpdateTable(TableType*)-Table");

                UtilityTestBinding->Unbind();
                TEST(UtilityTestBinding->GetTable() == nullptr,
                     "EventSubscriberBindingImpl::Unbind()-Table");

                Boole SubscribedFailTest = !UtilityTestBinding->IsSubscribed();
                TEST(SubscribedPassTest && SubscribedFailTest,
                     "EventSubscriberBindingImpl::IsSubscribed()_const-Table");

                UtilityTestBinding->UpdateTable(&FirstTestTable);
                UtilityTestBinding->Unsubscribe();
                TEST(UtilityTestBinding->IsSubscribed() == false,
                     "EventSubscriberBindingImpl::Unsubscribe()-Subscription");
                TEST_THROW(ExceptionFactory<ExceptionBase::II_IDENTITY_NOT_FOUND_EXCEPTION>::Type,
                           FirstTestTable.GetSubscription(1156),
                           "EventSubscriberBindingImpl::Unsubscribe()-TableThrow");
            }//Table
        }//EventSubscriberBindingImpl

        {//EventSubscriptionTable
            // Test name suffix key:
            // { B or N } { Sn or Un or Sr or UF or SF } { E or S or Q }
            //
            // B - Binding Factory
            // N - Non-binding Factory
            //
            // Sn - Single Container
            // Un - Unsorted Container
            // Sr - Sorted Container
            // UF - Unsorted Fixed-size Container
            // SF - Sorted Fixed-size Container
            //
            // E - Empty Dispatcher
            // S - Silenceable Dispatcher
            // Q - Queued Dispatcher

            {//BUnE (Default config settings)
                using TestTableType = EventSubscriptionTable<BUnETestConfig>;
                using TestSubscriberType = TestTableType::SubscriberType;
                using TestSubscriberIDType = TestSubscriberType::IDType;
                using TestBindingType = TestTableType::SubscriptionGet;
                using TestBindingImplType = TestTableType::TraitsType::ActualFactoryType::BindingImplType;
                using TestBindingImplPtrType = TestTableType::TraitsType::ActualFactoryType::BindingImplPtrType;

                Whole TestSubCallCount = 0;
                Whole TestSubTwoCallCount = 0;
                TestTableType TestTable(1337);
                TestSubscriberType TestSubscriberOne(0xDEADBEEF,[&](EventPtr TheEvent){ ++TestSubCallCount; });
                TestSubscriberType TestSubscriberTwo(0xBAADC0DE,[&](EventPtr TheEvent){ ++TestSubCallCount; ++TestSubTwoCallCount; });
                TestTable.Subscribe(TestSubscriberOne);

                TEST(TestTable.GetID() == 1337 &&
                     TestTable.GetNumSubscriptions() == 1 &&
                     TestTable.GetSubscription(0xDEADBEEF) != nullptr,
                     "EventSubscriptionTable::ConstructionAndGetters-BUnE");

                TestTableType MoveTable( std::move(TestTable) );
                TEST(MoveTable.GetID() == 1337 &&
                     MoveTable.GetNumSubscriptions() == 1 &&
                     MoveTable.GetSubscription(0xDEADBEEF) != nullptr,
                     "EventSubscriptionTable::(EventSubscriptionTable&&)-BUnE");

                TestTableType AssignMoveSourceTable( 12345 );
                TestTableType AssignMoveDestTable( 90210 );
                AssignMoveDestTable = std::move(AssignMoveSourceTable);
                TEST(AssignMoveDestTable.GetID() == 12345,
                     "EventSubscriptionTable::operator=(EventSubscriptionTable&&)-BUnE");

                TestTableType TestTableTwo(54321);
                TestBindingType TestBindingOne = TestTableTwo.Subscribe(TestSubscriberOne);
                TestBindingImplPtrType TestBindingTwo = std::static_pointer_cast<TestBindingImplType>( TestTableTwo.Subscribe(TestSubscriberTwo) );
                TEST(TestBindingTwo->GetTable() == &TestTableTwo,
                     "EventSubscriptionTable::Subscribe(SubscribeArg)-BUnE");

                TEST(TestTableTwo.GetNumSubscriptions() == 2,
                     "EventSubscriptionTable::GetNumSubscriptions()_const-BUnE");

                TEST(TestTableTwo.GetSubscription(0xDEADBEEF) != nullptr &&
                     TestTableTwo.GetSubscription(0xBAADC0DE) != nullptr,
                     "EventSubscriptionTable::GetSubscription(const_SubscriberIDType)_const-BUnE");
                TEST_THROW(ExceptionFactory<ExceptionBase::II_IDENTITY_NOT_FOUND_EXCEPTION>::Type,
                           TestTableTwo.GetSubscription(12345),
                           "EventSubscriptionTable::GetSubscription(const_SubscriberIDType)_const-Throw-BUnE");

                TEST(TestTableTwo.HasSubscription(0xDEADBEEF) &&
                     TestTableTwo.HasSubscription(0xBAADC0DE),
                     "EventSubscriptionTable::HasSubscription(const_SubscriberIDType)_const-BUnE");
                TEST(TestTableTwo.HasSubscription(12345) == false,
                     "EventSubscriptionTable::HasSubscription(const_SubscriberIDType)_const-Failure-BUnE");

                EventPtr ToDispatch = std::make_shared<Event>("Test");
                TestTableTwo.DispatchEvent(&TestSubscriberType::operator(),ToDispatch);
                TEST(TestSubCallCount == 2,
                     "EventSubscriptionTable::DispatchEvent(Funct,Args...)_const-BUnE");

                TestTableTwo.DispatchEventSingle(0xBAADC0DE,&TestSubscriberType::operator(),ToDispatch);
                TEST(TestSubCallCount == 3 &&
                     TestSubTwoCallCount == 2,
                     "EventSubscriptionTable::DispatchEventSingle(const_SubIDType,Funct,Args...)_const-BUnE");

                // These aren't stellar tests, but the default subscriber wasn't built with queries in mind
                std::vector<TestSubscriberIDType> QueryVecOne = TestTableTwo.DispatchQuery(&TestSubscriberType::GetID);
                TEST(QueryVecOne.size() == 2 &&
                     QueryVecOne[0] == TestSubscriberIDType(0xDEADBEEF) &&
                     QueryVecOne[1] == TestSubscriberIDType(0xBAADC0DE),
                     "EventSubscriptionTable::DispatchQuery(Funct,Args...)_const-BUnE");

                std::vector<TestSubscriberIDType> QueryVecTwo = TestTableTwo.DispatchQuerySingle(0xBAADC0DE,&TestSubscriberType::GetID);
                TEST(QueryVecTwo.size() == 1 &&
                     QueryVecTwo[0] == TestSubscriberIDType(0xBAADC0DE),
                     "EventSubscriptionTable::DispatchQuerySingle(const_SubIDType,Funct,Args...)_const-BUnE");

                TestTableTwo.Unsubscribe(0xBAADC0DE);
                TEST(TestTableTwo.GetNumSubscriptions() == 1 &&
                     TestBindingTwo->IsSubscribed() == false,
                     "EventSubscriptionTable::Unsubscribe(const_SubscriberIDType)-BUnE");
                TEST_THROW(ExceptionFactory<ExceptionBase::II_IDENTITY_NOT_FOUND_EXCEPTION>::Type,
                           TestTableTwo.GetSubscription(0xBAADC0DE),
                           "EventSubscriptionTable::Unsubscribe(const_SubscriberIDType)-Verify-BUnE");

                TEST(TestTableTwo.UnsubscribeAll() == 1 &&
                     TestBindingOne->IsSubscribed() == false,
                     "EventSubscriptionTable::UnsubscribeAll()-BUnE");
                TEST_THROW(ExceptionFactory<ExceptionBase::II_IDENTITY_NOT_FOUND_EXCEPTION>::Type,
                           TestTableTwo.GetSubscription(0xDEADBEEF),
                           "EventSubscriptionTable::UnsubscribeAll()-Verify-BUnE");
            }//BUnE (Default config settings)

            {//NUFS
                using TestTableType = EventSubscriptionTable<NUFSTestConfig>;
                using TestSubscriberType = TestSubscriberImpl;

                TestTableType TestTable(1337);
                TestSubscriberType TestSubscriberOne(0xDEADBEEF);
                TestSubscriberType TestSubscriberTwo(0xBAADC0DE);
                TestTable.Subscribe(&TestSubscriberOne);

                TEST(TestTable.GetID() == 1337 &&
                     TestTable.GetNumSubscriptions() == 1 &&
                     TestTable.GetSubscription(0xDEADBEEF) != nullptr,
                     "EventSubscriptionTable::ConstructionAndGetters-NUFS");

                TestTableType MoveTable( std::move(TestTable) );
                TEST(MoveTable.GetID() == 1337 &&
                     MoveTable.GetNumSubscriptions() == 1 &&
                     MoveTable.GetSubscription(0xDEADBEEF) != nullptr,
                     "EventSubscriptionTable::(EventSubscriptionTable&&)-NUFS");

                TestTableType AssignMoveSourceTable( 12345 );
                TestTableType AssignMoveDestTable( 90210 );
                AssignMoveDestTable = std::move(AssignMoveSourceTable);
                TEST(AssignMoveDestTable.GetID() == 12345,
                     "EventSubscriptionTable::operator=(EventSubscriptionTable&&)-NUFS");

                TestTableType TestTableTwo(54321);
                TestSubscriberType* TestSubOnePtr = static_cast<TestSubscriberType*>( TestTableTwo.Subscribe(&TestSubscriberOne) );
                TestSubscriberType* TestSubTwoPtr = static_cast<TestSubscriberType*>( TestTableTwo.Subscribe(&TestSubscriberTwo) );
                TEST(TestSubOnePtr == &TestSubscriberOne &&
                     TestSubTwoPtr == &TestSubscriberTwo,
                     "EventSubscriptionTable::Subscribe(SubscribeArg)-NUFS");

                TEST(TestTableTwo.GetNumSubscriptions() == 2,
                     "EventSubscriptionTable::GetNumSubscriptions()_const-NUFS");

                TEST(TestTableTwo.GetMaxSubscriptions() == 3,
                     "EventSubscriptionTable::GetMaxSubscriptions()_const-NUFS");

                TEST(TestTableTwo.GetSubscription(0xDEADBEEF) != nullptr &&
                     TestTableTwo.GetSubscription(0xBAADC0DE) != nullptr,
                     "EventSubscriptionTable::GetSubscription(const_SubscriberIDType)_const-NUFS");
                TEST_THROW(ExceptionFactory<ExceptionBase::II_IDENTITY_NOT_FOUND_EXCEPTION>::Type,
                           TestTableTwo.GetSubscription(12345),
                           "EventSubscriptionTable::GetSubscription(const_SubscriberIDType)_const-Throw-NUFS");

                TEST(TestTableTwo.HasSubscription(0xDEADBEEF) &&
                     TestTableTwo.HasSubscription(0xBAADC0DE),
                     "EventSubscriptionTable::HasSubscription(const_SubscriberIDType)_const-NUFS");
                TEST(TestTableTwo.HasSubscription(12345) == false,
                     "EventSubscriptionTable::HasSubscription(const_SubscriberIDType)_const-Failure-NUFS");

                TestTableTwo.DispatchEvent(&TestSubscriberBase::TestSet,20);
                TEST(TestSubscriberOne.GetCallCount() == 1 &&
                     TestSubscriberTwo.GetCallCount() == 1 &&
                     TestSubscriberOne.TestGet() == 20 &&
                     TestSubscriberTwo.TestGet() == 20,
                     "EventSubscriptionTable::DispatchEvent(Funct,Args...)_const-NUFS");

                TestTableTwo.DispatchEventSingle(0xBAADC0DE,&TestSubscriberBase::TestSet,80);
                TEST(TestSubscriberOne.GetCallCount() == 1 &&
                     TestSubscriberTwo.GetCallCount() == 2 &&
                     TestSubscriberOne.TestGet() == 20 &&
                     TestSubscriberTwo.TestGet() == 80,
                     "EventSubscriptionTable::DispatchEventSingle(const_SubIDType,Funct,Args...)_const-NUFS");

                std::vector<Whole> QueryVecOne = TestTableTwo.DispatchQuery(&TestSubscriberBase::TestGet);
                TEST(QueryVecOne.size() == 2 &&
                     QueryVecOne[0] == 20 &&
                     QueryVecOne[1] == 80,
                     "EventSubscriptionTable::DispatchQuery(Funct,Args...)_const-NUFS");

                std::vector<Whole> QueryVecTwo = TestTableTwo.DispatchQuerySingle(0xBAADC0DE,&TestSubscriberBase::TestGet);
                TEST(QueryVecTwo.size() == 1 &&
                     QueryVecTwo[0] == 80,
                     "EventSubscriptionTable::DispatchQuerySingle(const_SubIDType,Funct,Args...)_const-NUFS");

                TestTableTwo.SetMuteEvents(true);
                TestTableTwo.DispatchEvent(&TestSubscriberBase::TestSet,10);
                TestTableTwo.DispatchEventSingle(0xBAADC0DE,&TestSubscriberBase::TestSet,90);
                std::vector<Whole> QueryVecThree = TestTableTwo.DispatchQuery(&TestSubscriberBase::TestGet);
                std::vector<Whole> QueryVecFour = TestTableTwo.DispatchQuerySingle(0xBAADC0DE,&TestSubscriberBase::TestGet);
                TEST(TestSubscriberOne.GetCallCount() == 1 &&
                     TestSubscriberTwo.GetCallCount() == 2 &&
                     TestSubscriberOne.TestGet() == 20 &&
                     TestSubscriberTwo.TestGet() == 80 &&
                     QueryVecThree.empty() &&
                     QueryVecFour.empty() &&
                     TestTableTwo.GetMuteEvents() == true,
                     "EventSubscriptionTable::SetAndGetMuteEvents-NUFS");

                TestTableTwo.Unsubscribe(0xBAADC0DE);
                TEST(TestTableTwo.GetNumSubscriptions() == 1,
                     "EventSubscriptionTable::Unsubscribe(const_SubscriberIDType)-NUFS");
                TEST_THROW(ExceptionFactory<ExceptionBase::II_IDENTITY_NOT_FOUND_EXCEPTION>::Type,
                           TestTableTwo.GetSubscription(0xBAADC0DE),
                           "EventSubscriptionTable::Unsubscribe(const_SubscriberIDType)-Verify-NUFS");

                TEST(TestTableTwo.UnsubscribeAll() == 1,
                     "EventSubscriptionTable::UnsubscribeAll()-NUFS");
                TEST_THROW(ExceptionFactory<ExceptionBase::II_IDENTITY_NOT_FOUND_EXCEPTION>::Type,
                           TestTableTwo.GetSubscription(0xDEADBEEF),
                           "EventSubscriptionTable::UnsubscribeAll()-Verify-NUFS");
            }//NUFS

            {//NSnE
                using TestTableType = EventSubscriptionTable<NSnETestConfig>;
                using TestSubscriberType = TestSubscriberImpl;

                TestTableType TestTable(1337);
                TestSubscriberType TestSubscriberOne(0xDEADBEEF);
                TestSubscriberType TestSubscriberTwo(0xBAADC0DE);
                TestTable.Subscribe(&TestSubscriberOne);

                TEST(TestTable.GetID() == 1337 &&
                     TestTable.GetSubscription(0xDEADBEEF) != nullptr,
                     "EventSubscriptionTable::ConstructionAndGetters-NSnE");

                TestTableType MoveTable( std::move(TestTable) );
                TEST(MoveTable.GetID() == 1337 &&
                     MoveTable.GetSubscription(0xDEADBEEF) != nullptr,
                     "EventSubscriptionTable::(EventSubscriptionTable&&)-NSnE");

                TestTableType AssignMoveSourceTable( 12345 );
                TestTableType AssignMoveDestTable( 90210 );
                AssignMoveDestTable = std::move(AssignMoveSourceTable);
                TEST(AssignMoveDestTable.GetID() == 12345,
                     "EventSubscriptionTable::operator=(EventSubscriptionTable&&)-NSnE");

                TestTableType TestTableTwo(54321);
                TestSubscriberType* TestSubOnePtr = static_cast<TestSubscriberType*>( TestTableTwo.Subscribe(&TestSubscriberOne) );
                TEST(TestSubOnePtr == &TestSubscriberOne,
                     "EventSubscriptionTable::Subscribe(SubscribeArg)-NSnE");
                TEST_THROW(ExceptionFactory<ExceptionBase::INVALID_STATE_EXCEPTION>::Type,
                           TestTableTwo.Subscribe(&TestSubscriberTwo),
                           "EventSubscriptionTable::Subscribe(SubscribeArg)-Failure-NSnE");

                TEST(TestTableTwo.GetSubscription() == &TestSubscriberOne,
                     "EventSubscriptionTable::GetSubscription()_const-NSnE");
                TEST(TestTableTwo.GetSubscription(0xDEADBEEF) != nullptr,
                     "EventSubscriptionTable::GetSubscription(const_SubscriberIDType)_const-NSnE");
                TEST_THROW(ExceptionFactory<ExceptionBase::II_IDENTITY_NOT_FOUND_EXCEPTION>::Type,
                           TestTableTwo.GetSubscription(0xBAADC0DE),
                           "EventSubscriptionTable::GetSubscription(const_SubscriberIDType)_const-Throw-NSnE");

                TEST(TestTableTwo.HasSubscription(),
                     "EventSubscriptionTable::HasSubscription()_const-NSnE");
                TEST(TestTableTwo.HasSubscription(0xDEADBEEF),
                     "EventSubscriptionTable::HasSubscription(const_SubscriberIDType)_const-NSnE");
                TEST(!TestTableTwo.HasSubscription(0xBAADC0DE),
                     "EventSubscriptionTable::HasSubscription(const_SubscriberIDType)_const-Failure-NSnE");

                TestTableTwo.DispatchEvent(&TestSubscriberBase::TestSet,20);
                TEST(TestSubscriberOne.GetCallCount() == 1 &&
                     TestSubscriberOne.TestGet() == 20,
                     "EventSubscriptionTable::DispatchEvent(Funct,Args...)_const-NSnE");

                TestTableTwo.DispatchEventSingle(0xDEADBEEF,&TestSubscriberBase::TestSet,80);
                TEST(TestSubscriberOne.GetCallCount() == 2 &&
                     TestSubscriberOne.TestGet() == 80,
                     "EventSubscriptionTable::DispatchEventSingle(const_SubIDType,Funct,Args...)_const-NSnE");

                std::vector<Whole> QueryVecOne = TestTableTwo.DispatchQuery(&TestSubscriberBase::TestGet);
                TEST(QueryVecOne.size() == 1 &&
                     QueryVecOne[0] == 80,
                     "EventSubscriptionTable::DispatchQuery(Funct,Args...)_const-NSnE");

                TestSubscriberOne.TestSet(50);
                std::vector<Whole> QueryVecTwo = TestTableTwo.DispatchQuerySingle(0xDEADBEEF,&TestSubscriberBase::TestGet);
                TEST(QueryVecTwo.size() == 1 &&
                     QueryVecTwo[0] == 50,
                     "EventSubscriptionTable::DispatchQuerySingle(const_SubIDType,Funct,Args...)_const-NSnE");

                TestTableTwo.Unsubscribe(0xDEADBEEF);
                TEST(!TestTableTwo.HasSubscription(),
                     "EventSubscriptionTable::Unsubscribe(const_SubscriberIDType)-NSnE");
                TEST_THROW(ExceptionFactory<ExceptionBase::II_IDENTITY_NOT_FOUND_EXCEPTION>::Type,
                           TestTableTwo.GetSubscription(0xDEADBEEF),
                           "EventSubscriptionTable::Unsubscribe(const_SubscriberIDType)-Verify-NSnE");

                TestTableTwo.Subscribe(&TestSubscriberTwo);
                TEST(TestTableTwo.Unsubscribe() == true,
                     "EventSubscriptionTable::Unsubscribe(const_SubscriberIDType)-NSnE");
                TEST_THROW(ExceptionFactory<ExceptionBase::II_IDENTITY_NOT_FOUND_EXCEPTION>::Type,
                           TestTableTwo.GetSubscription(0xBAADC0DE),
                           "EventSubscriptionTable::Unsubscribe(const_SubscriberIDType)-Verify-NSnE");
            }//NSnE

            {//BSFQ
                using TestTableType = EventSubscriptionTable<BSFQTestConfig>;
                using TestSubscriberType = TestTableType::SubscriberType;
                using TestSubscriberIDType = TestSubscriberType::IDType;
                using TestBindingType = TestTableType::SubscriptionGet;
                using TestBindingImplType = TestTableType::TraitsType::ActualFactoryType::BindingImplType;
                using TestBindingImplPtrType = TestTableType::TraitsType::ActualFactoryType::BindingImplPtrType;

                Whole TestSubCallCount = 0;
                Whole TestSubTwoCallCount = 0;
                TestTableType TestTable(1337);
                TestSubscriberType TestSubscriberOne(0xDEADBEEF,[&](EventPtr TheEvent){ ++TestSubCallCount; });
                TestSubscriberType TestSubscriberTwo(0xBAADC0DE,[&](EventPtr TheEvent){ ++TestSubCallCount; ++TestSubTwoCallCount; });
                TestTable.Subscribe(TestSubscriberOne);

                TEST(TestTable.GetID() == 1337 &&
                     TestTable.GetNumSubscriptions() == 1 &&
                     TestTable.GetSubscription(0xDEADBEEF) != nullptr,
                     "EventSubscriptionTable::ConstructionAndGetters-BSFQ");

                TestTableType MoveTable( std::move(TestTable) );
                TEST(MoveTable.GetID() == 1337 &&
                     MoveTable.GetNumSubscriptions() == 1 &&
                     MoveTable.GetSubscription(0xDEADBEEF) != nullptr,
                     "EventSubscriptionTable::(EventSubscriptionTable&&)-BSFQ");

                TestTableType AssignMoveSourceTable( 12345 );
                TestTableType AssignMoveDestTable( 90210 );
                AssignMoveDestTable = std::move(AssignMoveSourceTable);
                TEST(AssignMoveDestTable.GetID() == 12345,
                     "EventSubscriptionTable::operator=(EventSubscriptionTable&&)-BSFQ");

                TestTableType TestTableTwo(54321);
                TestBindingType TestBindingOne = TestTableTwo.Subscribe(TestSubscriberOne);
                TestBindingImplPtrType TestBindingTwo = std::static_pointer_cast<TestBindingImplType>( TestTableTwo.Subscribe(TestSubscriberTwo) );
                TEST(TestBindingTwo->GetTable() == &TestTableTwo,
                     "EventSubscriptionTable::Subscribe(SubscribeArg)-BSFQ");

                TEST(TestTableTwo.GetNumSubscriptions() == 2,
                     "EventSubscriptionTable::GetNumSubscriptions()_const-BSFQ");

                TEST(TestTableTwo.GetMaxSubscriptions() == 3,
                     "EventSubscriptionTable::GetMaxSubscriptions()_const-BSFQ");

                TEST(TestTableTwo.GetSubscription(0xDEADBEEF) != nullptr &&
                     TestTableTwo.GetSubscription(0xBAADC0DE) != nullptr,
                     "EventSubscriptionTable::GetSubscription(const_SubscriberIDType)_const-BSFQ");
                TEST_THROW(ExceptionFactory<ExceptionBase::II_IDENTITY_NOT_FOUND_EXCEPTION>::Type,
                           TestTableTwo.GetSubscription(12345),
                           "EventSubscriptionTable::GetSubscription(const_SubscriberIDType)_const-Throw-BSFQ");

                TEST(TestTableTwo.HasSubscription(0xDEADBEEF) &&
                     TestTableTwo.HasSubscription(0xBAADC0DE),
                     "EventSubscriptionTable::HasSubscription(const_SubscriberIDType)_const-BSFQ");
                TEST(TestTableTwo.HasSubscription(12345) == false,
                     "EventSubscriptionTable::HasSubscription(const_SubscriberIDType)_const-Failure-BSFQ");

                EventPtr ToDispatch = std::make_shared<Event>("Test");
                TestTableTwo.DispatchEvent(&TestSubscriberType::operator(),ToDispatch);
                TEST(TestSubCallCount == 0 &&
                     TestSubTwoCallCount == 0,
                     "EventSubscriptionTable::DispatchEvent(Funct,Args...)_const-PreFlush-BSFQ");
                TestTableTwo.FlushAllEvents();
                TEST(TestSubCallCount == 2 &&
                     TestSubTwoCallCount == 1,
                     "EventSubscriptionTable::DispatchEvent(Funct,Args...)_const-PostFlush-BSFQ");

                TestTableTwo.DispatchEventSingle(0xBAADC0DE,&TestSubscriberType::operator(),ToDispatch);
                TEST(TestSubCallCount == 2 &&
                     TestSubTwoCallCount == 1,
                     "EventSubscriptionTable::DispatchEventSingle(const_SubIDType,Funct,Args...)_const-PreFlush-BSFQ");
                TestTableTwo.FlushAllEvents();
                TEST(TestSubCallCount == 3 &&
                     TestSubTwoCallCount == 2,
                     "EventSubscriptionTable::DispatchEventSingle(const_SubIDType,Funct,Args...)_const-PostFlush-BSFQ");

                QueuedQueryResultsPtr<TestSubscriberIDType> ResultsOne = TestTableTwo.DispatchQuery(&TestSubscriberType::GetID);
                TEST(ResultsOne->IsReady() == false,
                     "EventSubscriptionTable::DispatchQuery(Funct,Args...)_const-PreFlush-BSFQ");
                TestTableTwo.FlushAllQueries();
                TEST(ResultsOne->IsReady() == true &&
                     ResultsOne->Results.size() == 2 &&
                     ResultsOne->Results[0] == TestSubscriberIDType(0xBAADC0DE) &&
                     ResultsOne->Results[1] == TestSubscriberIDType(0xDEADBEEF),
                     "EventSubscriptionTable::DispatchQuery(Funct,Args...)_const-PostFlush-BSFQ");

                QueuedQueryResultsPtr<TestSubscriberIDType> ResultsTwo = TestTableTwo.DispatchQuerySingle(0xBAADC0DE,&TestSubscriberType::GetID);
                TEST(ResultsTwo->IsReady() == false,
                     "EventSubscriptionTable::DispatchQuerySingle(const_SubIDType,Funct,Args...)_const-PreFlush-BSFQ");
                TestTableTwo.FlushAllQueries();
                TEST(ResultsTwo->IsReady() == true &&
                     ResultsTwo->Results.size() == 1 &&
                     ResultsTwo->Results[0] == TestSubscriberIDType(0xBAADC0DE),
                     "EventSubscriptionTable::DispatchQuerySingle(const_SubIDType,Funct,Args...)_const-PostFlush-BSFQ");

                TestTableTwo.Unsubscribe(0xBAADC0DE);
                TEST(TestTableTwo.GetNumSubscriptions() == 1 &&
                     TestBindingTwo->IsSubscribed() == false,
                     "EventSubscriptionTable::Unsubscribe(const_SubscriberIDType)-BSFQ");
                TEST_THROW(ExceptionFactory<ExceptionBase::II_IDENTITY_NOT_FOUND_EXCEPTION>::Type,
                           TestTableTwo.GetSubscription(0xBAADC0DE),
                           "EventSubscriptionTable::Unsubscribe(const_SubscriberIDType)-Verify-BSFQ");

                TEST(TestTableTwo.UnsubscribeAll() == 1 &&
                     TestBindingOne->IsSubscribed() == false,
                     "EventSubscriptionTable::UnsubscribeAll()-BSFQ");
                TEST_THROW(ExceptionFactory<ExceptionBase::II_IDENTITY_NOT_FOUND_EXCEPTION>::Type,
                           TestTableTwo.GetSubscription(0xDEADBEEF),
                           "EventSubscriptionTable::UnsubscribeAll()-Verify-BSFQ");
            }//BSFQ

            {//NSoQ
                using TestTableType = EventSubscriptionTable<NSoQTestConfig>;
                using TestSubscriberType = TestSubscriberImpl;

                TestTableType TestTable(1337);
                TestSubscriberType TestSubscriberOne(0xDEADBEEF);
                TestSubscriberType TestSubscriberTwo(0xBAADC0DE);
                TestTable.Subscribe(&TestSubscriberOne);

                TEST(TestTable.GetID() == 1337 &&
                     TestTable.GetNumSubscriptions() == 1 &&
                     TestTable.GetSubscription(0xDEADBEEF) != nullptr,
                     "EventSubscriptionTable::ConstructionAndGetters-NSoQ");

                TestTableType MoveTable( std::move(TestTable) );
                TEST(MoveTable.GetID() == 1337 &&
                     MoveTable.GetNumSubscriptions() == 1 &&
                     MoveTable.GetSubscription(0xDEADBEEF) != nullptr,
                     "EventSubscriptionTable::(EventSubscriptionTable&&)-NSoQ");

                TestTableType AssignMoveSourceTable( 12345 );
                TestTableType AssignMoveDestTable( 90210 );
                AssignMoveDestTable = std::move(AssignMoveSourceTable);
                TEST(AssignMoveDestTable.GetID() == 12345,
                     "EventSubscriptionTable::operator=(EventSubscriptionTable&&)-NSoQ");

                TestTableType TestTableTwo(54321);
                TestSubscriberType* TestSubOnePtr = static_cast<TestSubscriberType*>( TestTableTwo.Subscribe(&TestSubscriberOne) );
                TestSubscriberType* TestSubTwoPtr = static_cast<TestSubscriberType*>( TestTableTwo.Subscribe(&TestSubscriberTwo) );
                TEST(TestSubOnePtr == &TestSubscriberOne &&
                     TestSubTwoPtr == &TestSubscriberTwo,
                     "EventSubscriptionTable::Subscribe(SubscribeArg)-NSoQ");

                TEST(TestTableTwo.GetNumSubscriptions() == 2,
                     "EventSubscriptionTable::GetNumSubscriptions()_const-NSoQ");

                TEST(TestTableTwo.GetSubscription(0xDEADBEEF) != nullptr &&
                     TestTableTwo.GetSubscription(0xBAADC0DE) != nullptr,
                     "EventSubscriptionTable::GetSubscription(const_SubscriberIDType)_const-NSoQ");
                TEST_THROW(ExceptionFactory<ExceptionBase::II_IDENTITY_NOT_FOUND_EXCEPTION>::Type,
                           TestTableTwo.GetSubscription(12345),
                           "EventSubscriptionTable::GetSubscription(const_SubscriberIDType)_const-Throw-NSoQ");

                TEST(TestTableTwo.HasSubscription(0xDEADBEEF) &&
                     TestTableTwo.HasSubscription(0xBAADC0DE),
                     "EventSubscriptionTable::HasSubscription(const_SubscriberIDType)_const-NSoQ");
                TEST(TestTableTwo.HasSubscription(12345) == false,
                     "EventSubscriptionTable::HasSubscription(const_SubscriberIDType)_const-Failure-NSoQ");

                TestTableTwo.DispatchEvent(&TestSubscriberBase::TestSet,20);
                TEST(TestSubscriberOne.GetCallCount() == 0 &&
                     TestSubscriberTwo.GetCallCount() == 0 &&
                     TestSubscriberOne.TestGet() == 0 &&
                     TestSubscriberTwo.TestGet() == 0,
                     "EventSubscriptionTable::DispatchEvent(Funct,Args...)_const-PreFlush-NSoQ");
                TestTableTwo.FlushAllEvents();
                TEST(TestSubscriberOne.GetCallCount() == 1 &&
                     TestSubscriberTwo.GetCallCount() == 1 &&
                     TestSubscriberOne.TestGet() == 20 &&
                     TestSubscriberTwo.TestGet() == 20,
                     "EventSubscriptionTable::DispatchEvent(Funct,Args...)_const-PostFlush-NSoQ");

                TestTableTwo.DispatchEventSingle(0xBAADC0DE,&TestSubscriberBase::TestSet,80);
                TEST(TestSubscriberOne.GetCallCount() == 1 &&
                     TestSubscriberTwo.GetCallCount() == 1 &&
                     TestSubscriberOne.TestGet() == 20 &&
                     TestSubscriberTwo.TestGet() == 20,
                     "EventSubscriptionTable::DispatchEventSingle(const_SubIDType,Funct,Args...)_const-PreFlush-NSoQ");
                TestTableTwo.FlushAllEvents();
                TEST(TestSubscriberOne.GetCallCount() == 1 &&
                     TestSubscriberTwo.GetCallCount() == 2 &&
                     TestSubscriberOne.TestGet() == 20 &&
                     TestSubscriberTwo.TestGet() == 80,
                     "EventSubscriptionTable::DispatchEventSingle(const_SubIDType,Funct,Args...)_const-PostFlush-NSoQ");

                QueuedQueryResultsPtr<Whole> ResultsOne = TestTableTwo.DispatchQuery(&TestSubscriberBase::TestGet);
                TEST(ResultsOne->IsReady() == false,
                     "EventSubscriptionTable::DispatchQuery(Funct,Args...)_const-PreFlush-NSoQ");
                TestTableTwo.FlushAllQueries();
                TEST(ResultsOne->IsReady() == true &&
                     ResultsOne->Results.size() == 2 &&
                     ResultsOne->Results[0] == 80 &&
                     ResultsOne->Results[1] == 20,
                     "EventSubscriptionTable::DispatchQuery(Funct,Args...)_const-PostFlush-NSoQ");

                QueuedQueryResultsPtr<Whole> ResultsTwo = TestTableTwo.DispatchQuerySingle(0xBAADC0DE,&TestSubscriberBase::TestGet);
                TEST(ResultsTwo->IsReady() == false,
                     "EventSubscriptionTable::DispatchQuerySingle(const_SubIDType,Funct,Args...)_const-PreFlush-NSoQ");
                TestTableTwo.FlushAllQueries();
                TEST(ResultsTwo->IsReady() == true &&
                     ResultsTwo->Results.size() == 1 &&
                     ResultsTwo->Results[0] == 80,
                     "EventSubscriptionTable::DispatchQuerySingle(const_SubIDType,Funct,Args...)_const-PostFlush-NSoQ");

                TestTableTwo.Unsubscribe(0xBAADC0DE);
                TEST(TestTableTwo.GetNumSubscriptions() == 1,
                     "EventSubscriptionTable::Unsubscribe(const_SubscriberIDType)-NSoQ");
                TEST_THROW(ExceptionFactory<ExceptionBase::II_IDENTITY_NOT_FOUND_EXCEPTION>::Type,
                           TestTableTwo.GetSubscription(0xBAADC0DE),
                           "EventSubscriptionTable::Unsubscribe(const_SubscriberIDType)-Verify-NSoQ");

                TEST(TestTableTwo.UnsubscribeAll() == 1,
                     "EventSubscriptionTable::UnsubscribeAll()-NSoQ");
                TEST_THROW(ExceptionFactory<ExceptionBase::II_IDENTITY_NOT_FOUND_EXCEPTION>::Type,
                           TestTableTwo.GetSubscription(0xDEADBEEF),
                           "EventSubscriptionTable::UnsubscribeAll()-Verify-NSoQ");
            }//NSoQ
        }//EventSubscriptionTable

        {//EventPublisher
            {//EventPublisher
                using TestPublisherType = EventPublisher<NUnETestConfig>;
                //using TestTableIterator = TestPublisherType::TableIterator;
                using TestSubscriberType = TestSubscriberImpl;

                TestSubscriberType TestSubscriberOne(0xDEADBEEF);
                TestSubscriberType TestSubscriberTwo(0xBAADC0DE);
                TestSubscriberType TestSubscriberThree(0xBABECAFE);

                TestPublisherType EmptyTestPublisher;
                TEST(EmptyTestPublisher.GetNumSubscriptionTables() == 0,
                     "EventPublisher::EventPublisher()");

                TestPublisherType SourceMoveConstruct;
                SourceMoveConstruct.AddSubscriptionTable(12345);
                TestPublisherType DestMoveConstruct( std::move(SourceMoveConstruct) );
                TEST(SourceMoveConstruct.GetNumSubscriptionTables() == 0,
                     "EventPublisher::EventPublisher(EventPublisher&&)-SourceTables");
                TEST(DestMoveConstruct.GetNumSubscriptionTables() == 1,
                     "EventPublisher::EventPublisher(EventPublisher&&)-DestTables");

                TestPublisherType SourceMoveAssign;
                TestPublisherType DestMoveAssign;
                SourceMoveAssign.AddSubscriptionTable(12345);
                DestMoveAssign = std::move(SourceMoveAssign);
                TEST(SourceMoveConstruct.GetNumSubscriptionTables() == 0,
                     "EventPublisher::operator=(EventPublisher&&)-SourceTables");
                TEST(DestMoveConstruct.GetNumSubscriptionTables() == 1,
                     "EventPublisher::operator=(EventPublisher&&)-DestTables");

                TestPublisherType TestPublisher(3);
                const TestPublisherType& ConstTestPublisher = TestPublisher;
                TEST(TestPublisher.GetNumSubscriptionTables() == 0,
                     "EventPublisher::EventPublisher(const_Whole)");

                TEST(TestPublisher.AddSubscriptionTable(5)->GetID() == 5,
                     "EventPublisher::AddSubscriptionTable(const_DispatchIDType)-Table1");
                TEST(TestPublisher.AddSubscriptionTable(25)->GetID() == 25,
                     "EventPublisher::AddSubscriptionTable(const_DispatchIDType)-Table2");
                TEST(TestPublisher.AddSubscriptionTable(125)->GetID() == 125,
                     "EventPublisher::AddSubscriptionTable(const_DispatchIDType)-Table3");

                TEST(TestPublisher.GetNumSubscriptionTables() == 3,
                     "EventPublisher::GetNumSubscriptionTables()");

                TEST(TestPublisher.begin()->GetID() == 5,
                     "EventPublisher::begin()");
                TEST(ConstTestPublisher.begin()->GetID() == 5,
                     "EventPublisher::begin()_const");
                TEST(std::distance(TestPublisher.begin(),TestPublisher.end()) == 3,
                     "EventPublisher::end()");
                TEST(std::distance(ConstTestPublisher.begin(),ConstTestPublisher.end()) == 3,
                     "EventPublisher::end()_const");

                TEST(TestPublisher.HasSubscriptionTable(5),
                     "EventPublisher::HasSubscriptionTable(const_DispatchIDType)_const-Table1");
                TEST(TestPublisher.HasSubscriptionTable(25),
                     "EventPublisher::HasSubscriptionTable(const_DispatchIDType)_const-Table2");
                TEST(TestPublisher.HasSubscriptionTable(125),
                     "EventPublisher::HasSubscriptionTable(const_DispatchIDType)_const-Table3");

                TEST(TestPublisher.GetSubscriptionTable(5) == TestPublisher.begin(),
                     "EventPublisher::GetSubscriptionTable(const_DispatchIDType)-Table1");
                TEST(TestPublisher.GetSubscriptionTable(25) == TestPublisher.begin() + 1,
                     "EventPublisher::GetSubscriptionTable(const_DispatchIDType)-Table2");
                TEST(TestPublisher.GetSubscriptionTable(125) == TestPublisher.begin() + 2,
                     "EventPublisher::GetSubscriptionTable(const_DispatchIDType)-Table3");

                TEST(ConstTestPublisher.GetSubscriptionTable(5) == ConstTestPublisher.begin(),
                     "EventPublisher::GetSubscriptionTable(const_DispatchIDType)_const-Table1");
                TEST(ConstTestPublisher.GetSubscriptionTable(25) == ConstTestPublisher.begin() + 1,
                     "EventPublisher::GetSubscriptionTable(const_DispatchIDType)_const-Table2");
                TEST(ConstTestPublisher.GetSubscriptionTable(125) == ConstTestPublisher.begin() + 2,
                     "EventPublisher::GetSubscriptionTable(const_DispatchIDType)_const-Table3");

                TEST(TestPublisher.Subscribe(5,&TestSubscriberOne) == &TestSubscriberOne,
                     "EventPublisher::Subscribe(const_DispatchIDType,SubscriberArg)-Table1-Sub1");
                TEST(TestPublisher.Subscribe(5,&TestSubscriberTwo) == &TestSubscriberTwo,
                     "EventPublisher::Subscribe(const_DispatchIDType,SubscriberArg)-Table1-Sub2");
                TEST(TestPublisher.Subscribe(5,&TestSubscriberThree) == &TestSubscriberThree,
                     "EventPublisher::Subscribe(const_DispatchIDType,SubscriberArg)-Table1-Sub3");

                TEST(TestPublisher.Subscribe(25,&TestSubscriberOne) == &TestSubscriberOne,
                     "EventPublisher::Subscribe(const_DispatchIDType,SubscriberArg)-Table2-Sub1");
                TEST(TestPublisher.Subscribe(25,&TestSubscriberTwo) == &TestSubscriberTwo,
                     "EventPublisher::Subscribe(const_DispatchIDType,SubscriberArg)-Table2-Sub2");
                TEST(TestPublisher.Subscribe(25,&TestSubscriberThree) == &TestSubscriberThree,
                     "EventPublisher::Subscribe(const_DispatchIDType,SubscriberArg)-Table2-Sub3");

                TEST(TestPublisher.Subscribe(125,&TestSubscriberOne) == &TestSubscriberOne,
                     "EventPublisher::Subscribe(const_DispatchIDType,SubscriberArg)-Table3-Sub1");

                const Whole SavedCount = TestSubscriberImpl::GetTotalCallCount();
                TestPublisher.DispatchEvent(25,&TestSubscriberBase::TestSet,1);
                TEST(TestSubscriberImpl::GetTotalCallCount() == SavedCount + 3,
                     "EventPublisher::DispatchEvent(const_DispatchIDType,MemberFunct,Args...)-Table2");
                TestPublisher.DispatchEvent(125,&TestSubscriberBase::TestSet,3);
                TEST(TestSubscriberImpl::GetTotalCallCount() == SavedCount + 4,
                     "EventPublisher::DispatchEvent(const_DispatchIDType,MemberFunct,Args...)-Table3");
                TestPublisher.DispatchEvent(5,&TestSubscriberBase::TestSet,7);
                TEST(TestSubscriberImpl::GetTotalCallCount() == SavedCount + 7,
                     "EventPublisher::DispatchEvent(const_DispatchIDType,MemberFunct,Args...)-Table1");

                TestPublisher.DispatchEventSingle(25,0xDEADBEEF,&TestSubscriberBase::TestSet,9);
                TEST(TestSubscriberImpl::GetTotalCallCount() == SavedCount + 8,
                     "EventPublisher::DispatchEventSingle(const_DispatchIDType,const_SubscriberIDType,MemberFunct,Args...)-Table2-Sub1");
                TestPublisher.DispatchEventSingle(5,0xBAADC0DE,&TestSubscriberBase::TestSet,3);
                TEST(TestSubscriberImpl::GetTotalCallCount() == SavedCount + 9,
                     "EventPublisher::DispatchEventSingle(const_DispatchIDType,const_SubscriberIDType,MemberFunct,Args...)-Table1-Sub2");

                std::vector<Whole> Results = TestPublisher.DispatchQuery(25,&TestSubscriberBase::TestGet);
                TEST(Results.size() == 3,
                     "EventPublisher::DispatchQuery(const_DispatchIDType,MemberFunct,Args...)-Table2-ResultCount");
                TEST(Results[0] == 9,
                     "EventPublisher::DispatchQuery(const_DispatchIDType,MemberFunct,Args...)-Table2-Result1");
                TEST(Results[1] == 3,
                     "EventPublisher::DispatchQuery(const_DispatchIDType,MemberFunct,Args...)-Table2-Result2");
                TEST(Results[2] == 7,
                     "EventPublisher::DispatchQuery(const_DispatchIDType,MemberFunct,Args...)-Table2-Result3");

                std::vector<Whole> ResultsSingle = TestPublisher.DispatchQuerySingle(5,0xBAADC0DE,&TestSubscriberBase::TestGet);
                TEST(ResultsSingle.size() == 1,
                     "EventPublisher::DispatchQuerySingle(const_DispatchIDType,const_SubscriberIDType,MemberFunct,Args...)-Table1-Sub2-ResultCount");
                TEST(ResultsSingle[0] == 3,
                     "EventPublisher::DispatchQuerySingle(const_DispatchIDType,const_SubscriberIDType,MemberFunct,Args...)-Table1-Sub2-Result1");

                TestPublisher.Unsubscribe(25,0xBAADC0DE);
                TEST(!TestPublisher.GetSubscriptionTable(25)->HasSubscription(0xBAADC0DE),
                     "EventPublisher::Unsubscribe(const_DispatchIDType,const_SubscriberIDType)");

                TestPublisher.Unsubscribe(0xBABECAFE);
                TEST(!TestPublisher.GetSubscriptionTable(5)->HasSubscription(0xBABECAFE),
                     "EventPublisher::Unsubscribe(const_SubscriberIDType)-Table1");
                TEST(!TestPublisher.GetSubscriptionTable(25)->HasSubscription(0xBABECAFE),
                     "EventPublisher::Unsubscribe(const_SubscriberIDType)-Table2");

                TEST(TestPublisher.UnsubscribeAll(5) == 2,
                     "EventPublisher::UnsubscribeAll(const_DispatchIDType)");

                TEST(TestPublisher.UnsubscribeAll() == 2,
                     "EventPublisher::UnsubscribeAll()");

                TestPublisher.RemoveSubscriptionTable(125);
                TEST(!TestPublisher.HasSubscriptionTable(125),
                     "EventPublisher::RemoveSubscriptionTable(const_DispatchIDType)");

                TEST(TestPublisher.RemoveAllSubscriptionTables() == 2,
                     "EventPublisher::RemoveAllSubscriptionTables()");
            }//EventPublisher

            {//AnyEventPublisher

            }//AnyEventPublisher
        }//EventPublisher
    }

    /// @brief Since RunAutomaticTests is implemented so is this.
    /// @return returns true
    virtual bool HasAutomaticTests() const
        { return true; }
};//eventstests

#endif

