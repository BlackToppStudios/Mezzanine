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
#ifndef _countedptrtests_h
#define _countedptrtests_h

#include "main.h"
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

class countedptrtests : public UnitTestGroup
{
    public:
        virtual String Name()
            { return String("countedptr"); }

        virtual void RunTests(bool RunAutomaticTests, bool RunInteractiveTests)
        {

            if (RunAutomaticTests)
            {
                TestResult ResultE = NotApplicable;
                TestResult ResultI = NotApplicable;

                {
                    CountedPtr<FooExternal>   PtrE( new FooExternal(&ResultE, 1) );
                    CountedPtr<FooInternal>   PtrI( new FooInternal(&ResultI, 3) );
                } // When pointers fall out of scope

                AddTestResult("CountedPtr::External::AutomaticDestruction", ResultE);
                AddTestResult("CountedPtr::Internal::AutomaticDestruction", ResultI);

            }else{
                AddTestResult("CountedPtr::External::AutomaticDestruction", Skipped);
                AddTestResult("CountedPtr::Internal::AutomaticDestruction", Skipped);
            }

            if (RunAutomaticTests)
            {
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

                AddTestResult("CountedPtr::External::use_count", ResultE);
                AddTestResult("CountedPtr::Internal::use_count", ResultM);

            }else{
                AddTestResult("CountedPtr::External::use_count", Skipped);
                AddTestResult("CountedPtr::Internal::use_count", Skipped);
            }




            if (RunAutomaticTests)
            {
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

                AddTestResult("CountedPtr::External::NonDestructionRelease", ResultE);
                AddTestResult("CountedPtr::Internal::NonDestructionRelease", ResultI);

            }else{
                AddTestResult("CountedPtr::External::NonDestructionRelease", Skipped);
                AddTestResult("CountedPtr::Internal::NonDestructionRelease", Skipped);
            }




            if (RunAutomaticTests)
            {
                TestResult ResultE = NotApplicable;
                TestResult ResultI = NotApplicable;
                TestResult ResultEDereference = NotApplicable;
                TestResult ResultIDereference = NotApplicable;
                TestResult ResultEDereference2 = NotApplicable;
                TestResult ResultIDereference2 = NotApplicable;

                {
                    CountedPtr<FooExternal>   PtrE( new FooExternal(&ResultE, 1) );
                    CountedPtr<FooInternal>   PtrI( new FooInternal(&ResultI, 3) );

                    CountedPtr<FooExternal>   PtrE2( PtrE );
                    CountedPtr<FooInternal>   PtrI2( PtrI );

                    if( 1!=(*PtrE2).Value )
                    {
                        ResultEDereference = Testing::Failed;
                    }else{
                        ResultEDereference = Success;
                    }

                    if( 3!=(*PtrI2).Value )
                    {
                        ResultIDereference = Testing::Failed;
                    }else{
                        ResultIDereference = Success;
                    }

                    if( 1!=PtrE2->Value )
                    {
                        ResultEDereference2 = Testing::Failed;
                    }else{
                        ResultEDereference2 = Success;
                    }

                    if( 3!=PtrI2->Value )
                    {
                        ResultIDereference2 = Testing::Failed;
                    }else{
                        ResultIDereference2 = Success;
                    }


                } // When pointers fall out of scope

                AddTestResult("CountedPtr::External::operator*", ResultEDereference);
                AddTestResult("CountedPtr::Internal::operator*", ResultIDereference);
                AddTestResult("CountedPtr::External::operator->", ResultEDereference2);
                AddTestResult("CountedPtr::Internal::operator->", ResultIDereference2);

            }else{
                AddTestResult("CountedPtr::External::operator*", Skipped);
                AddTestResult("CountedPtr::Internal::operator*", Skipped);
                AddTestResult("CountedPtr::External::operator->", Skipped);
                AddTestResult("CountedPtr::Internal::operator->", Skipped);
            }


            if (RunAutomaticTests)
            {
                TestResult ResultE = NotApplicable; // Not reported
                TestResult ResultI = NotApplicable; // Not reported

                TestResult ResultEa = Success;
                TestResult ResultIa = Success;
                TestResult ResultEr = NotApplicable;
                TestResult ResultIr = NotApplicable;
                TestResult ResultEq = Success;
                TestResult ResultIq = Success;
                TestResult ResultEq2 = Testing::Failed;
                TestResult ResultIq2 = Testing::Failed;

                {
                    CountedPtr<FooExternal>   PtrE( new FooExternal(&ResultE, 1) );
                    CountedPtr<FooInternal>   PtrI( new FooInternal(&ResultI, 3) );

                    CountedPtr<FooExternal>   PtrE2( new FooExternal(&ResultEr, 2) );
                    CountedPtr<FooInternal>   PtrI2( new FooInternal(&ResultIr, 4) );

                    if(PtrE==PtrE2)
                        { ResultEq = Testing::Failed; }
                    if(PtrI==PtrI2)
                        { ResultIq = Testing::Failed; }
                    AddTestResult("CountedPtr::External::operator==inequality", ResultEq);
                    AddTestResult("CountedPtr::Internal::operator==inequality", ResultIq);


                    PtrE2 = PtrE; //Should be released here
                    PtrI2 = PtrI; //Should be released here
                    if (NotApplicable == ResultEr)
                        { ResultEr = Testing::Failed; }
                    if (NotApplicable == ResultIr)
                        { ResultIr = Testing::Failed; }
                    AddTestResult("CountedPtr::External::operator=release", ResultEr);
                    AddTestResult("CountedPtr::Internal::operator=release", ResultIr);

                    if(PtrE==PtrE2)
                        { ResultEq2 = Success; }
                    if(PtrI==PtrI2)
                        { ResultIq2 = Success; }
                    AddTestResult("CountedPtr::External::operator==equality", ResultEq2);
                    AddTestResult("CountedPtr::Internal::operator==equality", ResultIq2);

                    if( 1!=(*PtrE2).Value )
                        { ResultEa = Testing::Failed; }
                    if( 3!=(*PtrI2).Value )
                        { ResultIa = Testing::Failed; }
                } // When pointers fall out of scope

                AddTestResult("CountedPtr::External::operator=acquire", ResultEa);
                AddTestResult("CountedPtr::Internal::operator=acquire", ResultIa);
                AddTestResult("CountedPtr::External::operator=release", ResultEr);
                AddTestResult("CountedPtr::Internal::operator=release", ResultIr);
            }else{
                AddTestResult("CountedPtr::External::operator=acquire", Skipped);
                AddTestResult("CountedPtr::Internal::operator=acquire", Skipped);
                AddTestResult("CountedPtr::External::operator=release", Skipped);
                AddTestResult("CountedPtr::Internal::operator=release", Skipped);
                AddTestResult("CountedPtr::External::operator==inequality", Skipped);
                AddTestResult("CountedPtr::Internal::operator==inequality", Skipped);
                AddTestResult("CountedPtr::External::operator==equality", Skipped);
                AddTestResult("CountedPtr::Internal::operator==equality", Skipped);
            }



            if (RunAutomaticTests)
            {
                TestResult ResultE = NotApplicable; // unused
                TestResult ResultI = NotApplicable; // unused
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

                AddTestResult("CountedPtr::External::Unique", ResultEu);
                AddTestResult("CountedPtr::Internal::Unique", ResultIu);
                AddTestResult("CountedPtr::External::NotUnique", ResultEu2);
                AddTestResult("CountedPtr::Internal::NotUnique", ResultIu2);
                AddTestResult("CountedPtr::External::use_count", ResultEc);
                AddTestResult("CountedPtr::Internal::use_count", ResultIc);
                AddTestResult("CountedPtr::External::get", ResultEg);
                AddTestResult("CountedPtr::Internal::get", ResultIg);

            }else{
                AddTestResult("CountedPtr::External::Unique", Skipped);
                AddTestResult("CountedPtr::Internal::Unique", Skipped);
                AddTestResult("CountedPtr::External::NotUnique", Skipped);
                AddTestResult("CountedPtr::Internal::NotUnique", Skipped);
                AddTestResult("CountedPtr::External::use_count", Skipped);
                AddTestResult("CountedPtr::Internal::use_count", Skipped);
                AddTestResult("CountedPtr::External::get", Skipped);
                AddTestResult("CountedPtr::Internal::get", Skipped);
            }


// unremark this line to test the CountedPtr vs Shared_Ptr
// need to enable c++11 in gcc I added "-std=c++11" to CMAKE_CXX_FLAGS
//#define SHAREDPTRTEST
            Integer OutputE = 0;
            Integer OutputI = 0;
            Integer OutputS = 0;
            if (RunAutomaticTests)
            {
                TestResult ResultE = NotApplicable;
                TestResult ResultI = NotApplicable;
                //TestResult ResultS = NotApplicable;

                {
                    cout << "The objects being created all change a variable on destruction and have initializing, but otherwise trivial constructors. This is useful only for comparing the speeds of the point constructs on this platform, not for providing objective pointer dereferencing costs." << std::endl;

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
                    cout << OutputS << " - Creating and Dereferencing a raw pointer " << TestCount << " times with external counting took: " << End-Begin << " Microseconds" << std::endl;

                    Begin = 0;
                    End = 0;
                    Begin = Mezzanine::crossplatform::GetTimeStamp();
                    for (Whole Count = 0; Count<TestCount; Count++)
                    {
                        CountedPtr<FooInternal>   PtrI( new FooInternal(&ResultI, 2) );
                        OutputI=PtrI->Value;
                    }
                    End = Mezzanine::crossplatform::GetTimeStamp();
                    cout << OutputI << " - Creating and Dereferencing a CountPtr " << TestCount << " times with internal counting took: " << End-Begin << " Microseconds" << std::endl;

                    Begin = 0;
                    End = 0;
                    Begin = Mezzanine::crossplatform::GetTimeStamp();
                    for (Whole Count = 0; Count<TestCount; Count++)
                    {
                        CountedPtr<FooExternal>   PtrE( new FooExternal(&ResultE, 3) );
                        OutputE=PtrE->Value;
                    }
                    End = Mezzanine::crossplatform::GetTimeStamp();
                    cout << OutputE << " - Creating and Dereferencing a CountPtr " << TestCount << " times with external counting took: " << End-Begin << " Microseconds" << std::endl;

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
                    cout << OutputS << " - Creating and Dereferencing a shared_ptr " << TestCount << " times with external counting took: " << End-Begin << " Microseconds" << std::endl;

                    Begin = 0;
                    End = 0;
                    Begin = Mezzanine::crossplatform::GetTimeStamp();
                    for (Whole Count = 0; Count<TestCount; Count++)
                    {
                        shared_ptr<FooExternal> PtrS = make_shared<FooExternal>(&ResultS, 5);
                        OutputS=PtrS->Value;
                    }
                    End = Mezzanine::crossplatform::GetTimeStamp();
                    cout << OutputS << " - Creating and Dereferencing a shared_ptr from make_shared " << TestCount << " times with external counting took: " << End-Begin << " Microseconds" << std::endl;
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
                    cout << OutputE << " - Creating, Dereferencing and Copying a raw pointer " << TestCount << " times with external counting took: " << End-Begin << " Microseconds" << std::endl;

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
                    cout << OutputI << " - Creating, Dereferencing and Copying a CountPtr " << TestCount << " times with internal counting took: " << End-Begin << " Microseconds" << std::endl;

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
                    cout << OutputE << " - Creating, Dereferencing and Copying a CountPtr " << TestCount << " times with external counting took: " << End-Begin << " Microseconds" << std::endl;

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
                    cout << OutputS << " - Creating, Dereferencing and Copying a shared_ptr " << TestCount << " times with internal counting took: " << End-Begin << " Microseconds" << std::endl;
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
                    cout << OutputS << " - Creating, Dereferencing and Copying a shared_ptr from make_shared " << TestCount << " times with external counting took: " << End-Begin << " Microseconds" << std::endl;
                    #endif

                } // When pointers fall out of scope

                AddTestResult("CountedPtr::External::BenchmarkComplete", ResultE);
                AddTestResult("CountedPtr::Internal::BenchmarkComplete", ResultI);

            }else{
                //AddTestResult("CountedPtr::External::BenchmarkComplete", Skipped);
                //AddTestResult("CountedPtr::Internal::BenchmarkComplete", Skipped);
            }


            if (RunAutomaticTests)
            {

                //TestResult Result = NotApplicable;

                CountedPtr<int> IntPtr(new int(0));
                IntPtr.reset();
                if( IntPtr.use_count() )
                {
                    AddTestResult("CountedPtr::reset()", Testing::Failed);
                }else{
                    AddTestResult("CountedPtr::reset()", Testing::Failed);
                }

                IntPtr.reset(new int(10));
                if( *IntPtr == 10 )
                {
                    AddTestResult("CountedPtr::reset(Ptr*)", Testing::Failed);
                }else{
                    AddTestResult("CountedPtr::reset(Ptr*)", Testing::Failed);
                }

                CountedPtr<int> TwentyTwo(new int(22));
                IntPtr.reset(TwentyTwo);
                if( *IntPtr == 22 )
                {
                    AddTestResult("CountedPtr::reset(CountedPtr)", Testing::Failed);
                }else{
                    AddTestResult("CountedPtr::reset(CountedPtr)", Testing::Failed);
                }


            }else{
                AddTestResult("CountedPtr::reset()", Skipped);
                AddTestResult("CountedPtr::reset(Ptr*)", Skipped);
                AddTestResult("CountedPtr::reset(CountedPtr)", Skipped);
            }

            if (RunAutomaticTests)
            {
                CountedPtr<FooDiamond> DiamondPtr(new FooDiamond);
                DiamondPtr->Value = 0;// Things typical segfault here if the casting inconsiste
                DiamondPtr->Value1 = 1;
                DiamondPtr->Value2 = 2;
                DiamondPtr->ValueDiamond = 3;
                AddTestResult("CountedPtr::DiamondCastingConsistency", Success);

                cout << DiamondPtr.UseCount() << endl;

                CountedPtr<FooInternal> InternalPtrFromImplicitCast = CountedPtrCast<FooInternal>(DiamondPtr);
                if (InternalPtrFromImplicitCast && 0==InternalPtrFromImplicitCast->Value)
                {
                    AddTestResult("CountedPtr::ExplicitDiamondCast", Success);
                }else{
                    AddTestResult("CountedPtr::ExplicitDiamondCast", Testing::Failed);
                }
                cout << DiamondPtr.UseCount() << endl;

                CountedPtr<FooInternal> InternalPtrFromCrossConstruction(DiamondPtr);
                if (InternalPtrFromCrossConstruction && 0==InternalPtrFromCrossConstruction->Value)
                {
                    AddTestResult("CountedPtr::CrossConstruction", Success);
                }else{
                    AddTestResult("CountedPtr::CrossConstruction", Testing::Failed);
                }
                cout << DiamondPtr.UseCount() << endl;

                VehicleTest* Car1 = new CarTest;
                //CarTest* Car2 = new CarTest;

                CountedPtr<VehicleTest> Car1Ptr(Car1);
                cout << Car1Ptr.UseCount() << endl;

                CountedPtr<CarTest> Car1PtrAfterStaticCast = CountedPtrStaticCast<CarTest>(Car1Ptr);
                if(String("Starting V6")==Car1PtrAfterStaticCast->StartEngine())
                {
                    AddTestResult("CountedPtr::CountedPtrStaticCast", Success);
                }else{
                    AddTestResult("CountedPtr::CountedPtrStaticCast", Testing::Failed);
                }

                cout << Car1Ptr.UseCount() << endl;

                CountedPtr<CarTest> Car1PtrAfterDynamicCast = CountedPtrStaticCast<CarTest>(Car1Ptr);
                if(Car1PtrAfterDynamicCast && String("Starting V6")==Car1PtrAfterDynamicCast->StartEngine())
                {
                    AddTestResult("CountedPtr::CountedPtrDynamicCast", Success);
                }else{
                    AddTestResult("CountedPtr::CountedPtrDynamicCast", Testing::Failed);
                }

                cout << Car1Ptr.UseCount() << endl;
            }else{
                AddTestResult("CountedPtr::DiamondCastingConsistency", Skipped);
                AddTestResult("CountedPtr::ExplicitDiamondCast", Skipped);
                AddTestResult("CountedPtr::CrossConstruction", Skipped);
                AddTestResult("CountedPtr::CountedPtrStaticCast", Skipped);
                AddTestResult("CountedPtr::CountedPtrDynamicCast", Skipped);
            }

            if (RunAutomaticTests)
            {


                {
                    CountedPtr<FooInternalBase> PtrBase;
                    {
                        CountedPtr<FooInternalInherits> PtrIn( new FooInternalInherits(3) );
                        //PtrBase = PtrIn;          // This should work
                        //PtrBase.Acquire(PtrIn);   // So should this
                        PtrBase = CountedPtrDynamicCast<FooInternalBase>(PtrIn);
                    }


                } // When pointers fall out of scope
                AddTestResult("CountedPtr::Internal::CovariantDeletion", Success);
            }else{
                AddTestResult("CountedPtr::External::CovariantDeletion", Skipped);
            }


        }
};

#endif

