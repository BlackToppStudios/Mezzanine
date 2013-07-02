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
#ifndef _rollingaverage_h
#define _rollingaverage_h

/// @file
/// @brief This stores the implementation and the declaration of the RollingAverage, BufferedRollingAverage, WeightedRollingAverage and the DefaultRollingAverage

#include <vector>

#include "datatypes.h"

namespace Mezzanine
{
    /// @brief The interface for rolling averages used in the Mezzanine, and threading library.
    template <typename RecordType> class RollingAverage
    {
        public:
            /// @brief Used for accessing the derived type when it may not be directly known.
            typedef RecordType Type;

            /// @brief How many records does this use to calculate its wolling average.
            /// @return A Whole containing the capacity.
            virtual Whole RecordCapacity() const = 0;

            /// @brief Add another record.
            /// @param Datum The record to be added. No gaurantee is provided that this record will be preserved, it could be use just once then discarded.
            virtual void Insert(RecordType Datum) = 0;

            /// @brief What is the current rolling average.
            /// @return The current rolling average.
            /// @details The is no gaurantee on the time this runs in. It run in linear time relative to the amount of records or it could run in constant time
            virtual RecordType GetAverage() const = 0;

            /// @brief Empty virtual Destructor.
            virtual ~RollingAverage(){}
    };//RollingAverage

    // Possible optimizations
    // once the average is calculated store it until another insert is done, costs memory, saves cpu time
    // cache the 1/count costs memory saves cpu time on float starved machines
    //
    // The Rolling Average is key to the way this algorithm works
    // RecordType Needs to implement a constructor which accepts a single 0, +=, and / with size_t
    
    /// @brief A RollingAverage that stores a copy of each record. and does the math when queried
    template <typename RecordType> class BufferedRollingAverage : public RollingAverage<RecordType>
    {
        protected:
            /// @brief The collection of all the records that are being have been added going back as far as the capacity will allow.
            /// @note All writes to this are performed through the iterator @ref Current
            std::vector<RecordType> Records;

            /// @brief The iterator used to treat an std::vector as a circular buffer.
            typename std::vector<RecordType>::iterator Current;

            /// @brief Move Current to the next place to be written
            virtual void IncrementIterator()
            {
                if(Records.end() == ++Current)
                    { Current = Records.begin(); }
            }

        public:
            /// @brief Used for accessing the derived type when it may not be directly known.
            typedef RecordType Type;

            /// @brief Constructor
            /// @param RecordCount The capacity of the Rolling average. This defaults to 10.
            BufferedRollingAverage(const Whole& RecordCount = MEZZ_FRAMESTOTRACK)
            {
                Records.assign(RecordCount,RecordType(0));
                Current = Records.begin();
            }

            /// @brief Gets the capacity of this rolling average. How many records will this store.
            virtual Whole RecordCapacity() const
                { return Records.size(); }

            /// @brief Inserts a new record into the rolling average and if needed drops the oldest one out.
            /// @param Datum The record to add.
            virtual void Insert(RecordType Datum)
            {
                IncrementIterator();
                *Current = Datum;
            }

            /// @brief Add up all the records and divide by the count. A simple arithmetic mean.
            /// @return This returns the current average of all the records as a RecordType. If it is an Integer type floating parts are truncated.
            virtual RecordType GetAverage() const
            {
                RecordType Results(0);
                for(typename std::vector<RecordType>::const_iterator Iter = Records.begin(); Iter!=Records.end(); ++Iter)
                    {Results += *Iter;}
                return Results/Records.size();
            }

            /// @brief Copy constructor, performs deep copy.
            /// @param Rhs The Rolling average to copy.
            BufferedRollingAverage(const BufferedRollingAverage& Rhs)
            {
                this->Current=this->Records.end() - (Rhs->Records.end()-Rhs->Current);
                this->Records(Rhs->Records);
            }

            /// @brief Assignment operator, performs deep copy.
            /// @param Rhs The Rolling average to copy.
            /// @return A reference to this rolling average after assignment has occurred, to allow for operator chaining.
            BufferedRollingAverage& operator=(const BufferedRollingAverage& Rhs)
            {
                this->Current=this->Records.end() - (Rhs->Records.end()-Rhs->Current);
                this->Records(Rhs->Records);
            }

            /// @brief Deconstructor.
            virtual ~BufferedRollingAverage(){}
    };//BufferedRollingAverage

    /// @brief A weighted average that does math with every insertion and stores nothing.
    /// @details This is possibly an optimization over the BufferedRollingAverage. It has not yet been shown whether the amount of memory for
    /// each task will significantly affect load times (From RAM to CPU cache) when starting a rolling average, or whether any
    /// innaccuracies in sorting using this would outweigh that.
    template <typename RecordType, typename MathType> class WeightedRollingAverage : public RollingAverage<RecordType>
    {
        protected:
            /// @brief What the math says the current avergage is.
            RecordType CurrentAverage;

            /// @brief Replaces the capacity of normal rolling averages.
            Whole WeightCount;

        public:
            /// @brief Used for accessing the derived type when it may not be directly known.
            typedef RecordType Type;

            /// @brief Class Constructor.
            /// @param RecordCount How many records should this emulate.
            WeightedRollingAverage(const Whole& RecordCount = MEZZ_FRAMESTOTRACK) :
                CurrentAverage(0),
				WeightCount(RecordCount)
                {}

            /// @brief Returns how many records this is emulating.
            /// @return A Whole that stores the count of record emulation.
            virtual Whole RecordCapacity() const
                { return WeightCount; }

            /// @brief Update the currently stored Rolling average with a new data point/record.
            /// @param Datum Update the Current Average according to the following formula CurrentAverage = CurrentAverage * ((RecordCount-1)/RecordCount) + Datum/RecordCount.
            virtual void Insert(RecordType Datum)
            {
                CurrentAverage = ( MathType(this->CurrentAverage) ? MathType(this->CurrentAverage) : MathType(1)) // A zero really screws with averages that tend to move away from zero
                        * ((MathType(WeightCount)-MathType(1))/MathType(WeightCount))                             //Get weight of all the older members
                        + MathType(Datum)/MathType(WeightCount);                                                  //Get the Weight of the Current Member
            }

            /// @brief Get the current rolling average.
            /// @return Get the Current Rolling average as a RecordType.
            virtual RecordType GetAverage() const
                { return CurrentAverage; }

            /// @brief Class Destructor.
            virtual ~WeightedRollingAverage(){}
    };//WeightedRollingAverage

    /// @brief Use this to get the default rolling average for a given type.
    /// @details use "DefaultRollingAverage<Whole>::Type" as Type to instantiate an instance of whatever type this is. This allows
    /// newer code to benefit from testing that has been done.
    template<class RecordType>
    struct DefaultRollingAverage
    {
        /// @brief The Default rolling average for all types is currently BufferedRollingAverage, this could change with future testing.
        //typedef BufferedRollingAverage<RecordType> Type;
        typedef WeightedRollingAverage<RecordType,RecordType> Type;
    };//DefaultRollingAverage
}//Mezzanine
#endif

