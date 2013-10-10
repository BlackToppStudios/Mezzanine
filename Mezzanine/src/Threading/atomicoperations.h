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
#ifndef _atomic_black_ops_would_be_an_awesome_name_for_a_cheezy_game_h
#define _atomic_black_ops_would_be_an_awesome_name_for_a_cheezy_game_h

#include "datatypes.h"


/// @file
/// @brief Simple thread safe ways to check and change a specified variable atomically.

namespace Mezzanine
{
    namespace Threading
    {
#ifndef SWIG

        /// @brief Atomically Compares And Swaps a 32 bit value.
        /// @details In such a way that this cannot be interupted by another thread this performs a number of steps.
        /// First, it compares the dereferenced value VariableToChange to OldValue. Then if they match it writes NewValue
        /// to the memory location VariableToChange points to. If they did not match, no work is performed except the dereferencing.
        /// In either case since VariableToChange has been derefenced the value stored before any exchange that might have
        /// occured is returned.
        /// @param VariableToChange A pointer to the value to compare and if it matches OldValue Atomically change.
        /// @param OldValue what is expected to be at the other end of VariableToChange.
        /// @param NewValue The value to be written to VariableToChange if at the time the actual CPU instruction is executed OldValue Matches *VariableToChange.
        /// @note This very specific semantics of this function are useless in most scripting so it is not included in Lua and other scripting languages.
        /// @return This always returns the value that was pointed to by VariableToChange immediately before this call.
        Int32 MEZZ_LIB AtomicCompareAndSwap32(Int32* VariableToChange, const Int32& OldValue, const Int32& NewValue);
/*
        // @brief Atomically Compares And Swaps a 64 bit value.
        // @details In such a way that this cannot be interupted by another thread this performs a number of steps.
        // First, it compares the dereferenced value VariableToChange to OldValue. Then if they match it writes NewValue
        // to the memory location VariableToChange points to. If they did not match, no work is performed except the dereferencing.
        // In either case since VariableToChange has been derefenced the value stored before any exchange that might have
        // occured is returned.
        // @param VariableToChange A pointer to the value to compare and if it matches OldValue Atomically change.
        // @param OldValue what is expected to be at the other end of VariableToChange.
        // @param NewValue The value to be written to VariableToChange if at the time the actual CPU instruction is executed OldValue Matches *VariableToChange.
        // @return This always returns the value that was pointed to by VariableToChange immediately before this call.
        Int64 MEZZ_LIB AtomicCompareAndSwap64(Int64* VariableToChange, const Int64& OldValue, const Int64& NewValue);
*/
        /// @brief Increments a value in a way guaranteed to not lose any atomic increments.
        /// @param VariableToChange A pointer to the 32 bit integer to increment by the amount specified.
        /// @param Value The amount to increment the VariableToChange by.
        ////// @note This very specific semantics of this function are useless in most scripting so it is not included in Lua and other scripting languages.
        /// @return The newly incremented value. This is not always *VariableToChange+Value. If another thread attempted and atomic operation on this at the same time the result could be the new value pointed to by VariableToChange plus Value.
        Int32 MEZZ_LIB AtomicAdd(Int32* VariableToChange, Int32 Value);
#endif
    }//Threading
}//Mezzanine

#endif
