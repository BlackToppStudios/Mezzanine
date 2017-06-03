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
#ifndef _countedptrtests_h
#define _countedptrtests_h

#include "mezztest.h"
#include "mezzanine.h"

#include "countedptr.h"

#include <memory>

using namespace std;
using namespace Mezzanine;
using namespace Mezzanine::Testing;

namespace Mezzanine
{
    namespace Testing
    {
        /// @brief A class to point at
        class FooExternal
        {
            public:
                /// @brief A value to use for testing Purposes
                Integer Value;

                /// @brief A pointer to the test results to set on destruction
                TestResult* Results;

                /// @brief A constructor that allows setting the value
                /// @param TargetResults Where to save destruction results
                /// @param Val A default value to use for testing.
                explicit FooExternal(TestResult* TargetResults, Integer Val = 0)
                    : Value(Val), Results(TargetResults)
                    {}

                /// @brief A destructor that sets Results to Success if it wasNotApplicable and leaves it untouched otherwise.
                ~FooExternal()
                {
                    if(NotApplicable==*Results)
                        { *Results = Success; }
                }
        };

        /// @brief A class to point at that uses its ownreferencing counting internal mechanism
        class FooInternal
        {
            private:
                /// @brief This is the Counter that stores how many references exist
                Whole RefCount;

            public:
                /// @brief A value to use for testing Purposes
                Integer Value;

                /// @brief A pointer to the test results to set on destruction
                TestResult* Results;

                /// @brief A constructor that allows setting the value
                /// @param TargetResults Where to save destruction results
                /// @param Val A default value to use for testing.
                explicit FooInternal(TestResult* TargetResults = 0, Integer Val = 0)
                    : RefCount(0), Value(Val), Results(TargetResults)
                    {}

                /// @brief A destructor that sets Results to Success if it wasNotApplicable and leaves it untouched otherwise.
                virtual ~FooInternal()
                {
                    if(Results && NotApplicable==*Results)
                        { *Results = Success; }
                }

                /// @brief Increase the reference count by one and return the updated count.
                /// @return The updated count;
                Whole IncrementReferenceCount()
                    { return ++RefCount; }

                /// @brief Decrease the reference count by one and return the updated count.
                /// @return The updated count;
                Whole DecrementReferenceCount()
                    { return --RefCount; }

                /// @brief Gets the actual pointer to the target.
                /// @return A Pointer of the targeted type to the object being managed.
                FooInternal* GetReferenceCountTargetAsPointer()
                    { return this; }

                /// @brief Get the current amount of references.
                /// @return A Whole with the current reference count
                Whole GetReferenceCount()
                    { return RefCount; }

                /// @brief Get a pointer to the most Derived type of this class
                /// @return A pointer cast to a void*, for use with CountedPtrCast
                virtual FooInternal* GetMostDerived()
                    { return this; }
        };

        class FooDerived1 : public virtual FooInternal
        {
            public:
                int Value1;

                /// @brief Get a pointer to the most Derived type of this class
                /// @return A pointer for use with CountedPtrCast
                virtual FooDerived1* GetMostDerived()
                    { return this; }
        };

        class FooDerived2 : public virtual FooInternal
        {
            public:
                int Value2;

                /// @brief Get a pointer to the most Derived type of this class
                /// @return A pointer for use with CountedPtrCast
                virtual FooDerived2* GetMostDerived()
                    { return this; }
        };

        class FooDiamond : public FooDerived1, public FooDerived2
        {
            public:
                int ValueDiamond;

                /// @brief Get a pointer to the most Derived type of this class
                /// @return A pointer for use with CountedPtrCast
                virtual FooDiamond* GetMostDerived()
                    { return this; }
        };

        class FooInternalBase
        {
        public:
            virtual ~FooInternalBase()
             {}

            /// @brief Increase the reference count by one and return the updated count.
            /// @return The updated count;
            virtual Whole IncrementReferenceCount() = 0;

            /// @brief Decrease the reference count by one and return the updated count.
            /// @return The updated count;
            virtual Whole DecrementReferenceCount() = 0;

            /// @brief Gets the actual pointer to the target.
            /// @return A Pointer of the targeted type to the object being managed.
            virtual FooInternalBase* GetReferenceCountTargetAsPointer() = 0;

            /// @brief Get the current amount of references.
            /// @return A Whole with the current reference count
            virtual Whole GetReferenceCount() = 0;

            /// @brief Get a pointer to the most Derived type of this class
            /// @return A pointer cast to a void*, for use with CountedPtrCast
            virtual FooInternalBase* GetMostDerived() = 0;
        };

        /// @brief A class to point at that uses its ownreferencing counting internal mechanism
        class FooInternalInherits : public FooInternalBase
        {
            private:
                /// @brief This is the Counter that stores how many references exist
                Whole RefCount;

            public:
                /// @brief A value to use for testing Purposes
                Integer Value;

                /// @brief A constructor that allows setting the value
                /// @param TargetResults Where to save destruction results
                /// @param Val A default value to use for testing.
                explicit FooInternalInherits(Integer Val = 0)
                    : RefCount(0), Value(Val)
                    {}

                /// @brief A destructor that sets Results to Success if it wasNotApplicable and leaves it untouched otherwise.
                virtual ~FooInternalInherits()
                 {}

                /// @brief Increase the reference count by one and return the updated count.
                /// @return The updated count;
                virtual Whole IncrementReferenceCount()
                    { return ++RefCount; }

                /// @brief Decrease the reference count by one and return the updated count.
                /// @return The updated count;
                virtual Whole DecrementReferenceCount()
                    { return --RefCount; }

                /// @brief Gets the actual pointer to the target.
                /// @return A Pointer of the targeted type to the object being managed.
                virtual FooInternalInherits* GetReferenceCountTargetAsPointer()
                    { return this; }

                /// @brief Get the current amount of references.
                /// @return A Whole with the current reference count
                virtual Whole GetReferenceCount()
                    { return RefCount; }

                /// @brief Get a pointer to the most Derived type of this class
                /// @return A pointer cast to a void*, for use with CountedPtrCast
                virtual FooInternalInherits* GetMostDerived()
                    { return this; }
        };

        /// @brief A class to point at that uses its ownreferencing counting internal mechanism and is suitable from some simple static casting experiments
        class VehicleTest
        {
            private:
                /// @brief This is the Counter that stores how many references exist
                Whole RefCount;

            public:
                /// @brief Increase the reference count by one and return the updated count.
                /// @return The updated count;
                Whole IncrementReferenceCount()
                    { return ++RefCount; }

                /// @brief Decrease the reference count by one and return the updated count.
                /// @return The updated count;
                Whole DecrementReferenceCount()
                    { return --RefCount; }

                /// @brief Gets the actual pointer to the target.
                /// @return A Pointer of the targeted type to the object being managed.
                VehicleTest* GetReferenceCountTargetAsPointer()
                    { return this; }

                /// @brief Get the current amount of references.
                /// @return A Whole with the current reference count
                Whole GetReferenceCount()
                    { return RefCount; }

                /// @brief Get a pointer to the most Derived type of this class
                /// @return A pointer for use with CountedPtrCast
                virtual VehicleTest* GetMostDerived()
                    { return this; }

                String StartEngine()
                    { return "Unknown Engine"; }

                virtual ~VehicleTest()
                    {}
        };

        class CarTest : public VehicleTest
        {
            public:
                String StartEngine()
                    { return "Starting V6"; }

                /// @brief Get a pointer to the most Derived type of this class
                /// @return A pointer for use with CountedPtrCast
                virtual CarTest* GetMostDerived()
                    { return this; }

                virtual ~CarTest()
                    {}
        };

    } // Testing Namespace

    template <>
    class ReferenceCountTraits <FooInternal>
    {
        public:
            typedef FooInternal RefCountType;

            static RefCountType* ConstructionPointer(RefCountType* Target)
                { return Target; }

            enum { IsCastable = CastDynamic };
    };

    template <>
    class ReferenceCountTraits <FooDerived1>
    {
        public:
            typedef FooDerived1 RefCountType;

            static RefCountType* ConstructionPointer(RefCountType* Target)
                { return Target; }

            enum { IsCastable = CastDynamic };
    };

    template <>
    class ReferenceCountTraits <FooDerived2>
    {
        public:
            typedef FooDerived2 RefCountType;

            static RefCountType* ConstructionPointer(RefCountType* Target)
                { return Target; }

            enum { IsCastable = CastDynamic };
    };

    template <>
    class ReferenceCountTraits <FooDiamond>
    {
        public:
            typedef FooDiamond RefCountType;

            static RefCountType* ConstructionPointer(RefCountType* Target)
                { return Target; }

            enum { IsCastable = CastDynamic };
    };

    template <>
    class ReferenceCountTraits <FooInternalInherits>
    {
        public:
            typedef FooInternalInherits RefCountType;

            static RefCountType* ConstructionPointer(RefCountType* Target)
                { return Target; }

            enum { IsCastable = CastDynamic };
    };

    template <>
    class ReferenceCountTraits <FooInternalBase>
    {
        public:
            typedef FooInternalBase RefCountType;

            static RefCountType* ConstructionPointer(RefCountType* Target)
                { return Target; }

            enum { IsCastable = CastDynamic };
    };

    template <>
    class ReferenceCountTraits <VehicleTest>
    {
        public:
            typedef VehicleTest RefCountType;

            static RefCountType* ConstructionPointer(RefCountType* Target)
                { return Target; }

            enum { IsCastable = CastStatic };
    };

    template <>
    class ReferenceCountTraits <CarTest>
    {
        public:
            typedef CarTest RefCountType;

            static RefCountType* ConstructionPointer(RefCountType* Target)
                { return Target; }

            enum { IsCastable = CastStatic };
    };

}

template <typename T>
CountedPtr<T> RecurseImpl(CountedPtr<T> Ptr, Whole Count)
{
    if(0==Count)
    {
        return Ptr;
    }else{
        CountedPtr<T> TempToForceExtraCopies = RecurseImpl(Ptr, Count-1);
        return TempToForceExtraCopies;
    }

}

template <typename T>
CountedPtr<T> RecurseTest(T Datum, Whole Count)
{
    return RecurseImpl(CountedPtr<T>(new T(Datum)), Count);
}

class countedptrtests : public UnitTestGroup
{
    public:
        virtual String Name()
            { return String("CountedPtr"); }


        void RunAutomaticTests()
        {
            { // Simple automated desctruction
                TestResult ResultE = NotApplicable;
                TestResult ResultI = NotApplicable;

                {
                    CountedPtr<FooExternal>   PtrE( new FooExternal(&ResultE, 1) );
                    CountedPtr<FooInternal>   PtrI( new FooInternal(&ResultI, 3) );
                } // When pointers fall out of scope

                TEST_RESULT(ResultE, "External::AutomaticDestruction");
                TEST_RESULT(ResultI, "Internal::AutomaticDestruction");
            }

            { // Use count and dual handle  delete
                TestResult ResultE = NotApplicable;
                TestResult ResultM = NotApplicable;

                {
                    CountedPtr<FooExternal>   PtrE( new FooExternal(&ResultE, 1) );
                    CountedPtr<FooExternal>   PtrE2( PtrE );
                    if( 2!=PtrE.use_count() )
                        { ResultE = Testing::Failed; }

                    CountedPtr<FooInternal>   PtrI( new FooInternal(&ResultM, 3) );
                    CountedPtr<FooInternal>   PtrI2( PtrI );
                    if( 2!=PtrI.use_count() )
                        { ResultM = Testing::Failed; }
                } // When pointers fall out of scope

                TEST_RESULT(ResultE, "External::use_count");
                TEST_RESULT(ResultM, "Internal::use_count");
            }

            { // release and dual handles
                TestResult ResultE = NotApplicable;
                TestResult ResultI = NotApplicable;

                {
                    CountedPtr<FooExternal>   PtrE( new FooExternal(&ResultE, 1) );
                    CountedPtr<FooExternal>   PtrE2( PtrE ); // why is this incrementing as though it were internal
                    if( 1!=(*PtrE2).Value )
                        { ResultE = Testing::Failed; }
                    PtrE.Reset();
                    PtrE2.Reset();

                    CountedPtr<FooInternal>   PtrI( new FooInternal(&ResultI, 3) );
                    CountedPtr<FooInternal>   PtrI2( PtrI );
                    if( 3!=(*PtrI2).Value )
                        { ResultI = Testing::Failed; }
                    PtrI.Reset();
                    PtrI2.Reset();
                }

                TEST_RESULT(ResultE, "External::NonDestructionRelease");
                TEST_RESULT(ResultI, "Internal::NonDestructionRelease");
            }

            { // Dereferencing
                TestResult ResultE = NotApplicable;
                TestResult ResultI = NotApplicable;

                {
                    CountedPtr<FooExternal>   PtrE( new FooExternal(&ResultE, 1) );
                    CountedPtr<FooInternal>   PtrI( new FooInternal(&ResultI, 3) );

                    CountedPtr<FooExternal>   PtrE2( PtrE );
                    CountedPtr<FooInternal>   PtrI2( PtrI );

                    TEST( 1==(*PtrE2).Value, "External::operator*" );
                    TEST( 3==(*PtrI2).Value, "Internal::operator*" );

                    TEST( 1==PtrE2->Value, "External::operator->" );
                    TEST( 3==PtrI2->Value, "Internal::operator->" );
                }
            }

            { // reseating pointers
                TestResult ResultE = NotApplicable; // Not reported
                TestResult ResultI = NotApplicable; // Not reported

                TestResult ResultEa = Success;
                TestResult ResultIa = Success;
                TestResult ResultEr = NotApplicable;
                TestResult ResultIr = NotApplicable;

                {
                    CountedPtr<FooExternal>   PtrE( new FooExternal(&ResultE, 1) );
                    CountedPtr<FooInternal>   PtrI( new FooInternal(&ResultI, 3) );

                    CountedPtr<FooExternal>   PtrE2( new FooExternal(&ResultEr, 2) );
                    CountedPtr<FooInternal>   PtrI2( new FooInternal(&ResultIr, 4) );

                    TEST(!(PtrE!=PtrE2), "External::operator!=inequality");
                    TEST(!(PtrI!=PtrI2), "Internal::operator!=inequality");

                    PtrE2 = PtrE; //Should be released here calling destructor which changes results
                    PtrI2 = PtrI; //Should be released here, and destructor yadda yadda....
                    TEST(NotApplicable != ResultEr,"External::operator=release");
                    TEST(NotApplicable != ResultIr, "Internal::operator=release");
                    TEST(PtrE==PtrE2, "External::operator==equality");
                    TEST(PtrI==PtrI2, "Internal::operator==equality");

                    if( 1!=(*PtrE2).Value )
                        { ResultEa = Testing::Failed; }
                    if( 3!=(*PtrI2).Value )
                        { ResultIa = Testing::Failed; }
                } // When pointers fall out of scope

                TEST_RESULT(ResultEa, "CountedPtr::External::operator=acquire");
                TEST_RESULT(ResultIa, "CountedPtr::Internal::operator=acquire");
                TEST_RESULT(ResultEr, "CountedPtr::External::operator=release");
                TEST_RESULT(ResultIr, "CountedPtr::Internal::operator=release");
            }

            {
                TestResult ResultE = NotApplicable; // not used for tests
                TestResult ResultI = NotApplicable; // not used for tests
                TestResult ResultEu = Success;
                TestResult ResultIu = Success;
                TestResult ResultEu2 = Success;
                TestResult ResultIu2 = Success;
                TestResult ResultEc = Success;
                TestResult ResultIc = Success;
                TestResult ResultEg = Success;
                TestResult ResultIg = Success;
                {
                    FooExternal* FooE = new FooExternal(&ResultE, 1);
                    FooInternal* FooI = new FooInternal(&ResultI, 3);

                    CountedPtr<FooExternal>   PtrE( FooE );
                    CountedPtr<FooInternal>   PtrI( FooI );

                    if(PtrE.get()!=FooE)
                        { ResultEg = Testing::Failed; }
                    if(PtrI.get()!=FooI)
                        { ResultIg = Testing::Failed; }

                    if(PtrE.unique()!=true)
                        { ResultEu = Testing::Failed; }
                    if(PtrI.unique()!=true)
                        { ResultIu = Testing::Failed; }

                    if(PtrE.use_count()!=1)
                        { ResultEc = Testing::Failed; }
                    if(PtrI.use_count()!=1)
                        { ResultIc = Testing::Failed; }

                    CountedPtr<FooExternal>   PtrE2( PtrE );
                    CountedPtr<FooInternal>   PtrI2( PtrI );

                    if(PtrE2.unique()==true)
                        { ResultEu2 = Testing::Failed; }
                    if(PtrI2.unique()==true)
                        { ResultIu2 = Testing::Failed; }

                    if(PtrE.use_count()!=2)
                        { ResultEc = Testing::Failed; }
                    if(PtrI.use_count()!=2)
                        { ResultIc = Testing::Failed; }
                    if(PtrE2.use_count()!=2)
                        { ResultEc = Testing::Failed; }
                    if(PtrI2.use_count()!=2)
                        { ResultIc = Testing::Failed; }

                    if(PtrE2.get()!=FooE)
                        { ResultEg = Testing::Failed; }
                    if(PtrI2.get()!=FooI)
                        { ResultIg = Testing::Failed; }

                } // When pointers fall out of scope
                TEST_RESULT(ResultEu, "External::Unique");
                TEST_RESULT(ResultIu, "Internal::Unique");
                TEST_RESULT(ResultEu2, "External::NotUnique");
                TEST_RESULT(ResultIu2, "Internal::NotUnique");
                TEST_RESULT(ResultEc, "External::use_count");
                TEST_RESULT(ResultIc, "Internal::use_count");
                TEST_RESULT(ResultEg, "External::get");
                TEST_RESULT(ResultIg, "Internal::get");
            }

            { // Inheritance diamonds
                CountedPtr<FooDiamond> DiamondPtr(new FooDiamond);
                DiamondPtr->Value = 0;// Things typical segfault here if the casting inconsiste
                DiamondPtr->Value1 = 1;
                DiamondPtr->Value2 = 2;
                DiamondPtr->ValueDiamond = 3;
                //TEST_RESULT(Success, "DiamondCastingConsistency"); // ©ompiling this BS is sort of a test

                //TestOutput << DiamondPtr.UseCount() << endl;

                CountedPtr<FooInternal> InternalPtrFromImplicitCast = CountedPtrCast<FooInternal>(DiamondPtr);
                TEST(InternalPtrFromImplicitCast && 0==InternalPtrFromImplicitCast->Value, "ExplicitDiamondCast")

                CountedPtr<FooInternal> InternalPtrFromCrossConstruction(DiamondPtr);
                TEST (InternalPtrFromCrossConstruction && 0==InternalPtrFromCrossConstruction->Value, "CrossConstruction")

                VehicleTest* Car1 = new CarTest;
                // ©arTest* Car2 = new CarTest;

                CountedPtr<VehicleTest> Car1Ptr(Car1);
                //TestOutput << Car1Ptr.UseCount() << endl;
                CountedPtr<CarTest> Car1PtrAfterStaticCast = CountedPtrStaticCast<CarTest>(Car1Ptr);
                TEST(String("Starting V6")==Car1PtrAfterStaticCast->StartEngine(), "CountedPtrStaticCast")

                CountedPtr<CarTest> Car1PtrAfterDynamicCast = CountedPtrStaticCast<CarTest>(Car1Ptr);
                TEST(Car1PtrAfterDynamicCast && String("Starting V6")==Car1PtrAfterDynamicCast->StartEngine(), "CountedPtrDynamicCast")
            }

            {// Reset
                CountedPtr<int> IntPtr(new int(0));
                IntPtr.reset();
                TEST( IntPtr.use_count()==0, "reset()" )

                IntPtr.reset(new int(10));
                TEST( *IntPtr == 10, "reset(Ptr*)" )

                CountedPtr<int> TwentyTwo(new int(22));
                IntPtr.reset(TwentyTwo);
                TEST( *IntPtr == 22, "reset(CountedPtr)" )
            }


        }

        /// @brief Since RunAutomaticTests is implemented so is this.
        /// @return returns true
        virtual bool HasAutomaticTests() const
            { return true; }

        virtual void RunTests(bool RunAutoTests, bool RunInteractiveTests)
        {
            RunAutomaticTests();

// unremark this line to test the CountedPtr vs Shared_Ptr
// need to enable c++11 in gcc I added "-std=c++11" to CMAKE_CXX_FLAGS
//#define SHAREDPTRTEST
            Integer OutputE = 0;
            Integer OutputI = 0;
            Integer OutputS = 0;
            {
                TestResult ResultE = NotApplicable;
                TestResult ResultI = NotApplicable;

                Whole RawPtrTime = 0;
                Whole CountPtrInternalTime = 0;
                Whole CountPtrExternalTime = 0;

                Whole RawPtrCopyTime = 0;
                Whole CountPtrCopyInternalTime = 0;
                Whole CountPtrCopyExternalTime = 0;

                #ifdef SHAREDPTRTEST
                  TestResult ResultS = NotApplicable;
                  Whole SharedPtrTime = 0;
                  Whole MakeSharedTime = 0;
                  Whole SharedPtrCopyTime = 0;
                  Whole MakeSharedCopyTime = 0;
                #endif

                {
                    TestOutput << "The objects being created all change a variable on destruction and have initializing, but otherwise trivial constructors. This is useful only for comparing the speeds of the point constructs on this platform, not for providing objective pointer dereferencing costs." << std::endl;

                    MaxInt Begin;
                    MaxInt End;

                    const Whole TestCount=10000000;

                    /////////////////////////////////////
                    // Create tests
                    Begin = 0;
                    End = 0;
                    Begin = Mezzanine::crossplatform::GetTimeStamp();
                    for (Whole Count = 0; Count<TestCount; Count++)
                    {
                        FooExternal*   PtrR = new FooExternal(&ResultE, 1);
                        OutputS=PtrR->Value;
                        delete PtrR;
                    }
                    End = Mezzanine::crossplatform::GetTimeStamp();
                    RawPtrTime = End-Begin;
                    TestOutput << OutputS << " - Creating and Dereferencing a raw pointer " << TestCount << " times without reference counting took: " << RawPtrTime << " Microseconds" << std::endl;

                    Begin = 0;
                    End = 0;
                    Begin = Mezzanine::crossplatform::GetTimeStamp();
                    for (Whole Count = 0; Count<TestCount; Count++)
                    {
                        CountedPtr<FooInternal>   PtrI( new FooInternal(&ResultI, 2) );
                        OutputI=PtrI->Value;
                    }
                    End = Mezzanine::crossplatform::GetTimeStamp();
                    CountPtrInternalTime = End-Begin;
                    TestOutput << OutputI << " - Creating and Dereferencing a CountPtr " << TestCount << " times with internal counting took: " << CountPtrInternalTime << " Microseconds" << std::endl;

                    Begin = 0;
                    End = 0;
                    Begin = Mezzanine::crossplatform::GetTimeStamp();
                    for (Whole Count = 0; Count<TestCount; Count++)
                    {
                        CountedPtr<FooExternal>   PtrE( new FooExternal(&ResultE, 3) );
                        OutputE=PtrE->Value;
                    }
                    End = Mezzanine::crossplatform::GetTimeStamp();
                    CountPtrExternalTime = End-Begin;
                    TestOutput << OutputE << " - Creating and Dereferencing a CountPtr " << TestCount << " times with external counting took: " << CountPtrExternalTime << " Microseconds" << std::endl;

                    #ifdef SHAREDPTRTEST
                    Begin = 0;
                    End = 0;
                    Begin = Mezzanine::crossplatform::GetTimeStamp();
                    for (Whole Count = 0; Count<TestCount; Count++)
                    {
                        shared_ptr<FooExternal> PtrS(new FooExternal(&ResultS, 4));
                        OutputS=PtrS->Value;
                    }
                    End = Mezzanine::crossplatform::GetTimeStamp();
                    SharedPtrTime = End-Begin;
                    TestOutput << OutputS << " - Creating and Dereferencing a shared_ptr " << TestCount << " times with external counting took: " << SharedPtrTime << " Microseconds" << std::endl;

                    Begin = 0;
                    End = 0;
                    Begin = Mezzanine::crossplatform::GetTimeStamp();
                    for (Whole Count = 0; Count<TestCount; Count++)
                    {
                        shared_ptr<FooExternal> PtrS = make_shared<FooExternal>(&ResultS, 5);
                        OutputS=PtrS->Value;
                    }
                    End = Mezzanine::crossplatform::GetTimeStamp();
                    MakeSharedTime = End-Begin;
                    TestOutput << OutputS << " - Creating and Dereferencing a shared_ptr from make_shared " << TestCount << " times with external counting took: " << MakeSharedTime << " Microseconds" << std::endl;
                    #endif

                    /////////////////////////////////////
                    // Copy tests
                    Begin = 0;
                    End = 0;
                    Begin = Mezzanine::crossplatform::GetTimeStamp();
                    for (Whole Count = 0; Count<TestCount; Count++)
                    {
                        FooExternal*   PtrR = new FooExternal(&ResultE, 6);
                        FooExternal*   PtrR2 = PtrR;
                        OutputE=PtrR2->Value;
                        delete PtrR2;
                    }
                    End = Mezzanine::crossplatform::GetTimeStamp();
                    RawPtrCopyTime = End-Begin;
                    TestOutput << OutputE << " - Creating, Dereferencing and Copying a raw pointer " << TestCount << " times without reference counting took: " << RawPtrCopyTime << " Microseconds" << std::endl;

                    Begin = 0;
                    End = 0;
                    Begin = Mezzanine::crossplatform::GetTimeStamp();
                    for (Whole Count = 0; Count<TestCount; Count++)
                    {
                        CountedPtr<FooInternal>   PtrI( new FooInternal(&ResultI, 7) );
                        CountedPtr<FooInternal>   PtrI2( PtrI );
                        OutputI=PtrI2->Value;
                    }
                    End = Mezzanine::crossplatform::GetTimeStamp();
                    CountPtrCopyInternalTime = End-Begin;
                    TestOutput << OutputI << " - Creating, Dereferencing and Copying a CountPtr " << TestCount << " times with internal counting took: " << CountPtrCopyInternalTime << " Microseconds" << std::endl;

                    Begin = 0;
                    End = 0;
                    Begin = Mezzanine::crossplatform::GetTimeStamp();
                    for (Whole Count = 0; Count<TestCount; Count++)
                    {
                        CountedPtr<FooExternal>   PtrE( new FooExternal(&ResultE, 8) );
                        CountedPtr<FooExternal>   PtrE2(PtrE);
                        OutputE=PtrE2->Value;
                    }
                    End = Mezzanine::crossplatform::GetTimeStamp();
                    CountPtrCopyExternalTime = End-Begin;
                    TestOutput << OutputE << " - Creating, Dereferencing and Copying a CountPtr " << TestCount << " times with external counting took: " << CountPtrCopyExternalTime << " Microseconds" << std::endl;

                    #ifdef SHAREDPTRTEST
                    Begin = 0;
                    End = 0;
                    Begin = Mezzanine::crossplatform::GetTimeStamp();
                    for (Whole Count = 0; Count<TestCount; Count++)
                    {
                        shared_ptr<FooExternal> PtrS(new FooExternal(&ResultE, 9));
                        shared_ptr<FooExternal> PtrS2(PtrS);
                        OutputS=PtrS2->Value;
                    }
                    End = Mezzanine::crossplatform::GetTimeStamp();
                    SharedPtrCopyTime = End-Begin;
                    TestOutput << OutputS << " - Creating, Dereferencing and Copying a shared_ptr " << TestCount << " times with internal counting took: " << SharedPtrCopyTime << " Microseconds" << std::endl;

                    Begin = 0;
                    End = 0;
                    Begin = Mezzanine::crossplatform::GetTimeStamp();
                    for (Whole Count = 0; Count<TestCount; Count++)
                    {
                        shared_ptr<FooExternal> PtrS = make_shared<FooExternal>(&ResultS, 10);
                        shared_ptr<FooExternal> PtrS2(PtrS);
                        OutputS=PtrS2->Value;
                    }
                    End = Mezzanine::crossplatform::GetTimeStamp();
                    MakeSharedCopyTime = End-Begin;
                    TestOutput << OutputS << " - Creating, Dereferencing and Copying a shared_ptr from make_shared " << TestCount << " times with external counting took: " << MakeSharedCopyTime << " Microseconds" << std::endl;
                    #endif

                    TestOutput << "Checking that raw pointers are fastest for sanity: "
                         << (RawPtrTime<CountPtrInternalTime &&
                            RawPtrTime<CountPtrExternalTime
                            #ifdef SHAREDPTRTEST
                            && RawPtrTime<SharedPtrTime &&
                            RawPtrTime<MakeSharedTime
                            #endif
                            )
                         << " and "
                         << (RawPtrCopyTime<CountPtrCopyInternalTime &&
                            RawPtrCopyTime<CountPtrCopyExternalTime
                            #ifdef SHAREDPTRTEST
                            && RawPtrCopyTime<SharedPtrCopyTime &&
                            RawPtrCopyTime<MakeSharedCopyTime
                            #endif
                            ) << endl;
                    TEST_WARN(RawPtrTime<CountPtrInternalTime &&
                              RawPtrTime<CountPtrExternalTime, "SanityRawVsCountedPtr"); // SanityTests
                    TEST_WARN(RawPtrCopyTime<CountPtrCopyInternalTime &&
                              RawPtrCopyTime<CountPtrCopyExternalTime, "SanityRawVsCountedCopy"); // SanityTests
                    #ifdef SHAREDPTRTEST
                        TEST_WARN(RawPtrTime<SharedPtrTime &&
                                  RawPtrTime<MakeSharedTime, "SanityRawVsShared"); // SanityTests
                        TEST_WARN(RawPtrCopyTime<SharedPtrCopyTime &&
                                  RawPtrCopyTime<MakeSharedCopyTime, "SanityRawVsSharedCopy"); // SanityTests
                    #endif

                    //TEST_WARN(MakeSharedTime<SharedPtrTime, "SanitySharedWoutMake"); // not very good sanity tests
                    //TEST_WARN(MakeSharedCopyTime<SharedPtrCopyTime, "SanitySharedWoutMakeCopy"); // The assumption this tests is onyl very loosely associated with the CountedPtr

                    TestOutput << "Checking CountedPtr internal is faster than CountedPtr external: "
                         << (CountPtrInternalTime<CountPtrExternalTime)
                         << " and "
                         << (CountPtrCopyInternalTime<CountPtrCopyExternalTime) << endl;
                    TEST_WARN(CountPtrInternalTime<CountPtrExternalTime, "InternalvsExternalTime");
                    TEST_WARN(CountPtrCopyInternalTime<CountPtrCopyExternalTime, "InternalvsExternalTimeCopy");

                    #ifdef SHAREDPTRTEST
                        TestOutput << "Checking CountedPtr internal is faster than shared_ptr: "
                             << (CountPtrInternalTime<SharedPtrTime)
                             << " and "
                             << (CountPtrCopyInternalTime<SharedPtrCopyTime)
                             << " and "
                             << (CountPtrInternalTime<MakeSharedTime)
                             << " and "
                             << (CountPtrCopyInternalTime<MakeSharedCopyTime) << endl;
                        TEST_WARN(CountPtrInternalTime<SharedPtrTime, "InternalvsShared");
                        TEST_WARN(CountPtrCopyInternalTime<SharedPtrCopyTime, "InternalvsSharedCopy");
                        TEST_WARN(CountPtrInternalTime<MakeSharedTime, "InternalvsMakeShared");
                        TEST_WARN(CountPtrCopyInternalTime<MakeSharedCopyTime, "InternalvsMakeSharedCopy");

                        TestOutput << "Checking CountedPtr External is faster than shared_ptr: "
                             << (CountPtrExternalTime<SharedPtrTime)
                             << " and "
                             << (CountPtrCopyExternalTime<SharedPtrCopyTime)
                             << " and "
                             << (CountPtrExternalTime<MakeSharedTime)
                             << " and "
                             << (CountPtrCopyExternalTime<MakeSharedCopyTime)
                             << endl;
                        TEST_WARN(CountPtrExternalTime<SharedPtrTime, "ExternalvsShared");
                        TEST_WARN(CountPtrCopyExternalTime<SharedPtrCopyTime, "ExternalvsSharedCopy");
                        TEST_WARN(CountPtrExternalTime<MakeSharedTime, "ExternalvsMakeShared");
                        TEST_WARN(CountPtrCopyExternalTime<MakeSharedCopyTime, "ExternalvsMakeSharedCopy");
                    #endif
                } // When pointers fall out of scope

                {
                    // A test to Try to make the CountedPtr segfault. No need for test macros
                    // The process will fail if this causes a segfault.
                    #define SEGFAULT_TESTS
                    #ifdef SEGFAULT_TESTS
                        CountedPtr<int> ThingThatMightCrash = RecurseTest(10,5);
                        RecurseTest(10,5);
                    #endif
                }
            }


         }
};

#endif

