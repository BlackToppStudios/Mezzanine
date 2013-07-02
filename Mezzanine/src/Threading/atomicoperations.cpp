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
#ifndef _atomic_black_ops_would_be_an_awesome_name_for_a_cheezy_game_cpp
#define _atomic_black_ops_would_be_an_awesome_name_for_a_cheezy_game_cpp

#include "datatypes.h"
#include "crossplatformincludes.h"

/// @file
/// @brief The implementation of simple thread safe ways to check and change a specified variable atomically.

namespace Mezzanine
{
    namespace Threading
    {
        Int32 AtomicCompareAndSwap32(Int32* VariableToChange, const Int32& OldValue, const Int32& NewValue)
        {
            #ifdef _MEZZ_THREAD_WIN32_
                return InterlockedCompareExchange((long*)VariableToChange,NewValue,OldValue);
            #else
                return __sync_val_compare_and_swap(VariableToChange,OldValue,NewValue);
            #endif
        }
/*
        // This does not appear to be supported on winxp
        Int64 AtomicCompareAndSwap64(Int64* VariableToChange, const Int64& OldValue, const Int64& NewValue)
        {
            #ifdef _MEZZ_THREAD_WIN32_
                return InterlockedCompareExchange64((long long*)VariableToChange,NewValue,OldValue);
            #else
                return __sync_val_compare_and_swap(VariableToChange,OldValue,NewValue);
            #endif
        }
*/
        Int32 AtomicAdd(Int32* VariableToChange, Int32 Value)
        {
            //while (*VariableToChange!=AtomicCompareAndSwap(VariableToChange,*VariableToChange,*VariableToChange+Value));
            //return *VariableToChange;
            #ifdef _MEZZ_THREAD_WIN32_
                return InterlockedExchangeAdd((long*)VariableToChange, Value);
            #else
                return __sync_add_and_fetch(VariableToChange,Value);
            #endif
        }
    }//Threading
}//Mezzanine

#endif
