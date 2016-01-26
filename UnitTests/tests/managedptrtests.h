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
#ifndef _managedptrtests_h
#define _managedptrtests_h

#include "mezztest.h"
#include "mezzanine.h"

#include "managedptr.h"

#include <stdexcept> //only used to throw for TEST_THROW

/// @file
/// @brief Test the features of ManagedPtr

using namespace Mezzanine;
using namespace Mezzanine::Testing;

class TrackedHandleInt
{
    public:
        /// @brief This is type ManagedPtr will use to work with a handle
        typedef int* TargetPtrType;
        typedef int TargetType;
        /// @brief The ManagedPtr never access objects directly, so this is just an implementation detail
        TargetPtrType Pointer;

        /// @brief On Destruction of this handle store the ConstructCount count in this if it not null
        int* ExternalConstructCount;
        /// @brief On Destruction of this handle store the DeconstructCount count in this if it not null
        int* ExternalDeconstructCount;
        /// @brief On Destruction of this handle store the ExternalSetCount count in this if it not null
        int* ExternalSetCount;
        /// @brief On Destruction of this handle store the ExternalGetCount count in this if it not null
        int* ExternalGetCount;

        /// @brief Initialize variables for tracking data during tests.
        TrackedHandleInt() :
            ExternalConstructCount(NULL),
            ExternalDeconstructCount(NULL),
            ExternalSetCount(NULL),
            ExternalGetCount(NULL)
        {}

        /// @brief This is expected to leave Pointer in some valid state for dereferencing.
        void Construct()
        {
            Pointer = new int(10); // Set pointee to arbitrary nonzero value for testing
            if(ExternalConstructCount)
                { (*ExternalConstructCount)++; }
        }
        /// @brief This can perform any cleanup, but should not fail if Pointer is NULL
        void Deconstruct()
        {
            if(Pointer&&ExternalDeconstructCount)
                { (*ExternalDeconstructCount)++; }
            delete Pointer; //deleting NULL is a no-op, otherwise a check would be needed.
        }

        /// @brief This is what ManagedPtr will use in copy and assignment operations as well as invaliding handles.
        /// @param Value The new value for the pointer. If NULL the only thing that the ManagedPtr will do to the handle is call its deconstruct method.
        void SetPointer(TargetPtrType Value)
        {
            Pointer = Value;
            if(ExternalSetCount)
                { (*ExternalSetCount)++; }
        }
        /// @brief This is what the ManagedPtr with use for dereferencing.
        /// @return The pointer to the managed data. This is expected to return a value that resolves to false when used as a condition when invalid.
        TargetPtrType GetPointer()
        {
            if(ExternalGetCount)
                { (*ExternalGetCount)++; }
            return Pointer;
        }
};

class TrackedHandleVector3
{
    public:
        /// @brief This is type ManagedPtr will use to work with a handle
        typedef Vector3* TargetPtrType;
        typedef Vector3 TargetType;
        /// @brief The ManagedPtr never access objects directly, so this is just an implementation detail
        TargetPtrType Pointer;

        /// @brief On Destruction of this handle store the ConstructCount count in this if it not null
        int* ExternalConstructCount;
        /// @brief On Destruction of this handle store the DeconstructCount count in this if it not null
        int* ExternalDeconstructCount;
        /// @brief On Destruction of this handle store the ExternalSetCount count in this if it not null
        int* ExternalSetCount;
        /// @brief On Destruction of this handle store the ExternalGetCount count in this if it not null
        int* ExternalGetCount;

        /// @brief Initialize variables for tracking data during tests.
        TrackedHandleVector3() :
            ExternalConstructCount(NULL),
            ExternalDeconstructCount(NULL),
            ExternalSetCount(NULL),
            ExternalGetCount(NULL)
        {}

        /// @brief This is expected to leave Pointer in some valid state for dereferencing.
        void Construct()
        {
            Pointer = new Vector3(1,2,3); // Set pointee to arbitrary nonzero value for testing
            if(ExternalConstructCount)
                { (*ExternalConstructCount)++; }
        }
        /// @brief This can perform any cleanup, but should not fail if Pointer is NULL
        void Deconstruct()
        {
            if(Pointer&&ExternalDeconstructCount)
                { (*ExternalDeconstructCount)++; }
            delete Pointer; //deleting NULL is a no-op, otherwise a check would be needed.
        }

        /// @brief This is what ManagedPtr will use in copy and assignment operations as well as invaliding handles.
        /// @param Value The new value for the pointer. If NULL the only thing that the ManagedPtr will do to the handle is call its deconstruct method.
        void SetPointer(TargetPtrType Value)
        {
            Pointer = Value;
            if(ExternalSetCount)
                { (*ExternalSetCount)++; }
        }
        /// @brief This is what the ManagedPtr with use for dereferencing.
        /// @return The pointer to the managed data. This is expected to return a value that resolves to false when used as a condition when invalid.
        TargetPtrType GetPointer()
        {
            if(ExternalGetCount)
                { (*ExternalGetCount)++; }
            return Pointer;
        }
};



/// @brief A small series of tests for ManagedPtr
class managedptrtests : public UnitTestGroup
{
    public:
        /// @copydoc Mezzanine::Testing::UnitTestGroup::Name
        /// @return Returns a String containing "BoilerPlate"
        virtual String Name()
            { return String("ManagedPtr"); }

        /// @brief This is called when Automatic tests are run
        void RunAutomaticTests()
        {
            int TestConstructCount = 0;
            int TestDeconstructCount = 0;
            int TestSetCount = 0;
            int TestGetCount = 0;
            TestOutput << "Testing Basic consistency of ManagedPtr, by creating one and counting its calls in a simple situation." << endl;
            {
                TrackedHandleInt AuditableHandle;
                AuditableHandle.ExternalConstructCount = &TestConstructCount;
                AuditableHandle.ExternalDeconstructCount = &TestDeconstructCount;
                AuditableHandle.ExternalSetCount = &TestSetCount;
                AuditableHandle.ExternalGetCount = &TestGetCount;
                ManagedPtr<TrackedHandleInt> BasicConsistencyTest(AuditableHandle);
                TEST(10==*BasicConsistencyTest,"BaseConsistency::Dereference");
            };
            TestOutput << endl
                 << " Constructed count: " << TestConstructCount << endl
                 << " Deconstructed count: " << TestDeconstructCount << endl
                 << " Set count: " << TestSetCount << endl
                 << " Get count: " << TestGetCount << endl << endl;
            TEST(TestConstructCount==1,"BaseConsistency::Construct");
            TEST(TestDeconstructCount==1,"BaseConsistency::Deconstruct");


            TestConstructCount = 0;
            TestDeconstructCount = 0;
            TestSetCount = 0;
            TestGetCount = 0;
            TestOutput << "Testing assignment of ManagedPtr, by creating one and counting its calls in a simple situation requiring assigment." << endl;
            {
                TrackedHandleInt AuditableHandle;
                AuditableHandle.ExternalConstructCount = &TestConstructCount;
                AuditableHandle.ExternalDeconstructCount = &TestDeconstructCount;
                AuditableHandle.ExternalSetCount = &TestSetCount;
                AuditableHandle.ExternalGetCount = &TestGetCount;
                ManagedPtr<TrackedHandleInt> Assigner(AuditableHandle);
                ManagedPtr<TrackedHandleInt> Assignee;
                Assignee=Assigner;
                TEST_THROW(Mezzanine::MemoryManagementException&,*Assigner;,"AssignmentConsistency::Invalidate");
                TEST(!Assigner,"AssignmentConsistency::BoolInvalidity");
                TEST(Assignee,"AssignmentConsistency::BoolValidity");
                TEST(10==*Assignee,"AssignmentConsistency::Dereference");
            };
            TestOutput << endl
                 << " Constructed count: " << TestConstructCount << endl
                 << " Deconstructed count: " << TestDeconstructCount << endl
                 << " Set count: " << TestSetCount << endl
                 << " Get count: " << TestGetCount << endl << endl;
            TEST(TestConstructCount==1,"AssignmentConsistency::Construct");
            TEST(TestDeconstructCount==1,"AssignmentConsistency::Deconstruct");


            TestConstructCount = 0;
            TestDeconstructCount = 0;
            TestSetCount = 0;
            TestGetCount = 0;
            TestOutput << "Testing copy construction of ManagedPtr, by creating one and counting its calls in a simple situation requiring copying." << endl;
            {
                TrackedHandleInt AuditableHandle;
                AuditableHandle.ExternalConstructCount = &TestConstructCount;
                AuditableHandle.ExternalDeconstructCount = &TestDeconstructCount;
                AuditableHandle.ExternalSetCount = &TestSetCount;
                AuditableHandle.ExternalGetCount = &TestGetCount;
                ManagedPtr<TrackedHandleInt> Assigner(AuditableHandle);
                ManagedPtr<TrackedHandleInt> Assignee(Assigner);
                TEST_THROW(Mezzanine::MemoryManagementException&,*Assigner;,"CopyConsistency::InvalidateThrow");
                TEST(!Assigner,"CopyConsistency::BoolInvalidity");
                TEST(Assignee,"CopyConsistency::BoolValidity");
                TEST(10==*Assignee,"CopyConsistency::Dereference");
            };
            TestOutput << endl
                 << " Constructed count: " << TestConstructCount << endl
                 << " Deconstructed count: " << TestDeconstructCount << endl
                 << " Set count: " << TestSetCount << endl
                 << " Get count: " << TestGetCount << endl << endl;
            TEST(TestConstructCount==1,"CopyConsistency::Construct");
            TEST(TestDeconstructCount==1,"CopyConsistency::Deconstruct");


            TestConstructCount = 0;
            TestDeconstructCount = 0;
            TestSetCount = 0;
            TestGetCount = 0;
            TestOutput << "Testing copy construction of ManagedPtr, by creating one and counting its calls in a simple situation requiring copying." << endl;
            {
                TrackedHandleInt AuditableHandle;
                AuditableHandle.ExternalConstructCount = &TestConstructCount;
                AuditableHandle.ExternalDeconstructCount = &TestDeconstructCount;
                AuditableHandle.ExternalSetCount = &TestSetCount;
                AuditableHandle.ExternalGetCount = &TestGetCount;
                ManagedPtr<TrackedHandleInt> Assigner(AuditableHandle);
                ManagedPtr<TrackedHandleInt> Assignee(Assigner);
                TEST_THROW(Mezzanine::MemoryManagementException&,*Assigner;,"CopyConsistency::InvalidateThrows");
                TEST(10==*Assignee,"CopyConsistency::Dereference");
            };
            TestOutput << endl
                 << " Constructed count: " << TestConstructCount << endl
                 << " Deconstructed count: " << TestDeconstructCount << endl
                 << " Set count: " << TestSetCount << endl
                 << " Get count: " << TestGetCount << endl << endl;
            TEST(TestConstructCount==1,"CopyConsistency::Construct");
            TEST(TestDeconstructCount==1,"CopyConsistency::Deconstruct");

            TestConstructCount = 0;
            TestDeconstructCount = 0;
            TestSetCount = 0;
            TestGetCount = 0;
            TestOutput << "Testing copy construction of ManagedPtr, by creating one and counting its calls in a simple situation requiring copying." << endl;
            {
                TrackedHandleVector3 AuditableHandle;
                AuditableHandle.ExternalConstructCount = &TestConstructCount;
                AuditableHandle.ExternalDeconstructCount = &TestDeconstructCount;
                AuditableHandle.ExternalSetCount = &TestSetCount;
                AuditableHandle.ExternalGetCount = &TestGetCount;
                ManagedPtr<TrackedHandleVector3> Deffer(AuditableHandle);
                TEST(1==Deffer->X && 2==Deffer->Y && 3==Deffer->Z,"ArrowConsistency::Dereference");
                ManagedPtr<TrackedHandleVector3> Deffer2(Deffer);
                TEST_THROW(Mezzanine::MemoryManagementException&,Deffer->Z;,"ArrowConsistency::InvalidateThrows");
                TEST(1==Deffer2->X && 2==Deffer2->Y && 3==Deffer2->Z,"ArrowConsistency::CopyDereference");
            };
            TestOutput << endl
                 << " Constructed count: " << TestConstructCount << endl
                 << " Deconstructed count: " << TestDeconstructCount << endl
                 << " Set count: " << TestSetCount << endl
                 << " Get count: " << TestGetCount << endl << endl;
            TEST(TestConstructCount==1,"ArrowConsistency::Construct");
            TEST(TestDeconstructCount==1,"ArrowConsistency::Deconstruct");

        }

        /// @brief Since RunAutomaticTests is implemented so is this.
        /// @return returns true
        virtual bool HasAutomaticTests() const
            { return true; }

};

#endif
