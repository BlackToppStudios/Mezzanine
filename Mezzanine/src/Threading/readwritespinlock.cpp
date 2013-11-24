// The DAGFrameScheduler is a Multi-Threaded lock free and wait free scheduling library.
// © Copyright 2010 - 2013 BlackTopp Studios Inc.
/* This file is part of The DAGFrameScheduler.

    The DAGFrameScheduler is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The DAGFrameScheduler is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with The DAGFrameScheduler.  If not, see <http://www.gnu.org/licenses/>.
*/
/* The original authors have included a copy of the license specified above in the
   'doc' folder. See 'gpl.txt'
*/
/* We welcome the use of the DAGFrameScheduler to anyone, including companies who wish to
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
#ifndef _readwritespinlock_cpp
#define _readwritespinlock_cpp

#include "atomicoperations.h"
#include "readwritespinlock.h"
#include "crossplatformincludes.h"

/// @file
/// @brief Contains the implementation for the @ref Mezzanine::Threading::Mutex Mutex synchronization object.

namespace Mezzanine
{
    namespace Threading
    {
        ReadWriteSpinLock::ReadWriteSpinLock() : Locked(0)
        {}

        ReadWriteSpinLock::~ReadWriteSpinLock()
        {}


        void ReadWriteSpinLock::LockForRead()
        {
            bool locked = false;
            Int32 Expected = Locked;
            /*while(!locked)
            {
                if (0<AtomicCompareAndSwap32(&Locked,0,0))
                {

                }
            }*/

            if(0>Expected)
                { return; }
            //while(AtomicCompareAndSwap32(&Locked,0,1))
                {}
        }

        bool ReadWriteSpinLock::TryLockForRead()
        {
            Int32 Expected = Locked;
            if(0>=Expected) // Can spuriously fail but shouldn't spuriosly succeed
            {
                return Expected==AtomicCompareAndSwap32(&Locked,Expected,Expected+1);
            }else{
                return false;
            }
        }

        void ReadWriteSpinLock::UnlockRead()
        {
            Int32 Expected = Locked;
            if(0>Expected)
                { return; }
            //while(Expected==AtomicCompareAndSwap32(&Locked,Expected,Expected-1))
                { Expected = Locked; }
        }
        //{ AtomicCompareAndSwap32(&Locked,1,0); }


        void ReadWriteSpinLock::LockForWrite()
        {
            //while(AtomicCompareAndSwap32(&Locked,0,1))
                {}
        }

        bool ReadWriteSpinLock::TryLockForWrite()
            { return false; }//{ return !AtomicCompareAndSwap32(&Locked,0,1); }

        void ReadWriteSpinLock::UnlockWrite()
            {}//{ AtomicCompareAndSwap32(&Locked,1,0); }



    } // \Threading namespace
} // \Mezzanine namespace
#endif
