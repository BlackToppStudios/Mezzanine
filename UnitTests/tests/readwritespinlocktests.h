// © Copyright 2010 - 2014 BlackTopp Studios Inc.
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
#ifndef _mutexreadwritespinlocktests_h
#define _mutexreadwritespinlocktests_h

#include "mezztest.h"

#include "dagframescheduler.h"

/// @file
/// @brief Basic tests of the mutex class

using namespace std;
using namespace Mezzanine;
using namespace Mezzanine::Testing;
using namespace Mezzanine::Threading;

/// @brief Used in ReadWriteSpinLock try_lock tests
static Mezzanine::Threading::ReadWriteSpinLock TryReadWriteSpinlock;
/// @brief Used in ReadWriteSpinlock TryWritelock tests
/// @param Value This is the a value passed into the thread to confirm that it works
void WriteInThreadRWSpin(void* Value)
{
    Int32* Out = (Int32*)Value;
    //std::cout << "Thread trying to lock for write mutex TryReadWriteSpinlock, thread has id: " << Mezzanine::Threading::this_thread::get_id() << endl;
    TryReadWriteSpinlock.LockForWrite();
    //std::cout << "Thread " << Mezzanine::Threading::this_thread::get_id() << " locked mutex for read and value is " << (*Out) << endl;
    (*Out)++;
    //std::cout << "Thread " << Mezzanine::Threading::this_thread::get_id() << " adjusted value to " << (*Out) << endl;
    TryReadWriteSpinlock.UnlockWrite();
    //std::cout << "Thread " << Mezzanine::Threading::this_thread::get_id() << " mutex UnlockWrite called." << endl;

}

/// @brief Used in ReadWriteSpinlock TryReadlock tests
/// @param Value This is the a value passed into the thread to confirm that it works
void ReadInThreadRWSpin(void* Value)
{
    //std::cout << "Thread trying to lock for read mutex TryReadWriteSpinlock, thread has id: " << Mezzanine::Threading::this_thread::get_id() << endl;
    TryReadWriteSpinlock.LockForRead();
    //Mezzanine::Threading::this_thread::sleep_for(10000);
    //std::cout << "Thread " << Mezzanine::Threading::this_thread::get_id() << " locked mutex for read and value is " << (*(Int32*)Value) << endl;
    TryReadWriteSpinlock.UnlockRead();
    //std::cout << "Thread " << Mezzanine::Threading::this_thread::get_id() << " mutex UnlockRead called." << endl;
}


/// @brief Tests for the mutex class
class readwritespinlocktests : public UnitTestGroup
{
    public:
        /// @copydoc Mezzanine::Testing::UnitTestGroup::Name
        /// @return Returns a String containing "Mutex"
        virtual String Name()
            { return String("ReadWriteSpinLock"); }

        /// @brief Even though the framescheduler does not use Mutexes, any library providing multithreading capabilites without them would be lacking, so we must test them.
        virtual void RunAutomaticTests()
        {

            { // ReadWriteSpinLock
                TestOutput << endl << "ReadWriteSpinLock tests (true values are all good): " << endl;
                bool TestValue = true==TryReadWriteSpinlock.TryLockForRead();
                cout << "Just tried to TryLockForRead expecting to get the lock: " << TestValue << endl;
                TEST( TestValue, "RWSpinLock::TryLockRead");

                TestValue = false==TryReadWriteSpinlock.TryLockForWrite();
                cout << "Just tried to TryLockForWrite expecting to *not* get the lock: " << TestValue << endl;
                TEST(TestValue,"RWSpinLock::TryLockReadExcludesWrite");

                TestValue = true==TryReadWriteSpinlock.TryLockForRead();
                cout << "Just tried to TryLockForRead expecting to  get the lock: " << TestValue << endl;
                TEST(TestValue,"RWSpinLock::TryLockReadMultiple");
                cout << "Trying to UnlockRead twice expecting to release the lock twice, no test for this." << endl;
                TryReadWriteSpinlock.UnlockRead();
                TryReadWriteSpinlock.UnlockRead();

                TestValue = true==TryReadWriteSpinlock.TryLockForWrite();
                cout << "Just tried to TryLockForWrite expecting to get the lock: " << TestValue << endl;
                TEST(TestValue,"RWSpinLock::TryLockWrite");

                TestValue = false==TryReadWriteSpinlock.TryLockForRead();
                cout << "Just tried to TryLockForRead expecting to *not* get the lock: " << TestValue << endl;
                TEST(TestValue,"RWSpinLock::TryLockWriteExcludesRead");
                TestValue = false==TryReadWriteSpinlock.TryLockForWrite();
                cout << "Just tried to TryLockForWrite expecting to *not* get the lock: " << TestValue << endl;
                TEST(TestValue,"RWSpinLock::TryLockWriteExcludesWrite");
                cout << "Trying to UnlockWrite twice expecting to release the lock twice, no test for this." << endl;
                TryReadWriteSpinlock.UnlockWrite();

                {
                    Whole ThreadCount = 30000;
                    vector<Mezzanine::Threading::Thread*> Threads;
                    Threads.reserve(ThreadCount);
                    Int32 Value = 10;
                    //LogForMutexes.str("");
                    cout << endl << "Creating " << ThreadCount << " threads to read and write into a value proected by a ReadWriteSpinLock" << endl;
                    Boole WriteTest = true;
                    MaxInt Start = GetTimeStamp();
                    for(Whole Counter=0; Counter<ThreadCount; Counter++)
                    {
                        WriteTest = !WriteTest;
                        if(WriteTest)
                            { Threads.push_back(new Mezzanine::Threading::Thread(WriteInThreadRWSpin, &Value)); }
                        else
                            { Threads.push_back(new Mezzanine::Threading::Thread(ReadInThreadRWSpin, &Value)); }
                    }

                    //TestOutput << "Waiting briefly for most threaded work to complete." << endl;
                    //Mezzanine::Threading::this_thread::sleep_for(5000*ThreadCount);
                    //Mezzanine::Threading::this_thread::sleep_for(2*ThreadCount);

                    TestOutput << "Joining and then cleaning up all threads." << endl;
                    for(vector<Mezzanine::Threading::Thread*>::iterator Iter = Threads.begin();
                        Iter!=Threads.end();
                        Iter++)
                    {
                        (*Iter)->join();
                        delete *Iter;
                    }
                    MaxInt End = GetTimeStamp();

                    Int32 Expected = (ThreadCount+1)/2+10;
                    TestOutput << "Expected result is " << Expected << " actually got " << Value << "." << endl;
                    TEST(Expected==Value,"RWSpinLock::StressTest");
                    //TestOutput << "ThreadLog" << endl << LogForMutexes.str() << endl << "/ThreadLog" << endl;
                    TestOutput << "It took " << End-Start << " microseconds to create, run, join and then delete "
                               << ThreadCount << " threads. Half of which change the a single piece of data. "
                               << "This is " << PreciseReal(ThreadCount)/PreciseReal(End-Start) * PreciseReal(1000000)
                               << " threads per second." << endl;
                }

            } // ReadWriteSpinLock

            { // ReadWriteSpinLock and lock guards
                {
                    lock_guard<ReadWriteSpinLock> g(TryReadWriteSpinlock);
                }
                TEST(true,"RWSpinLock::lock_gaurd");
                {
                    ReadOnlyLockGuard<ReadWriteSpinLock> g(TryReadWriteSpinlock);
                }
                TEST(true,"RWSpinLock::ReadOnlyLockGuard");
                {
                    ReadWriteLockGuard<ReadWriteSpinLock> g(TryReadWriteSpinlock);
                }
                TEST(true,"RWSpinLock::ReadWriteLockGuard");
            } // ReadWriteSpinLock and lock guards
        }

        /// @brief Since RunAutomaticTests is implemented so is this.
        /// @return returns true
        virtual bool HasAutomaticTests() const
            { return true; }

};

#endif

