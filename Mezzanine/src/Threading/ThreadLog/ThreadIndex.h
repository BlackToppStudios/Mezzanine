// The DAGFrameScheduler is a Multi-Threaded lock free and wait free scheduling library.
// © Copyright 2010 - 2016 BlackTopp Studios Inc.
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
#ifndef RaceLog_ThreadIndex_H
#define RaceLog_ThreadIndex_H

#include <ostream>
#include <vector>

namespace Mezzanine { namespace Threading {

    /// @brief A unique Id for threads suitable for use as the index into simple containers for quick lookups.
    /// @details A thread index is a sequentially coounted number from 0 to N where N is the number of threads. This
    /// supports only the most minimal set of operations that can be
    class ThreadIndex
    {
    public:
        /// @brief This is not really public but should be accessible to all RaceLog internals.
        using InternalType = std::int32_t;

        friend bool operator== (const ThreadIndex& LeftHand, const ThreadIndex& RightHand);
        friend std::ostream& operator<<(std::ostream& OutputStream, const ThreadIndex& OneThreadIndex);
        friend bool operator< (const ThreadIndex& LeftHand, const ThreadIndex& RightHand);

    private:
        /// @brief The actual thread index storage.
        InternalType Index;

    public:
        /// @brief Initializing Coinstructor.
        /// @param StartingIndex The starting value, defaults to -1
        constexpr ThreadIndex(const InternalType& StartingIndex = -1) : Index{StartingIndex} {}

        /// @brief Increment This Index and get a local reference.
        /// @return A reference the *this ThreadIndex after incrementing.
        ThreadIndex& operator++();

        /// @brief When getting indexes for other Threads it can be useful to use simple math
        /// @param Increment How many to move
        /// @return The thread index of the next Nth thread after this one, where N is the passed Increment.
        ThreadIndex operator+(const InternalType& Increment) const;

        /// @todo Add operator+, operator+= and operator-= to the ThreadIndex.

        /// @brief Used by other RaceLog internal to get at however this is represented.
        InternalType AsRaw() const;
    };

    /// @brief Compare two thread Index for equality.
    /// @param RightHand The item on the Right hand side of the equality comparison operator.
    /// @param LeftHand The item on the Left hand side of the equality comparison operator.
    /// @return True if they represent the same value (same index for the thread), and false otherwise.
    bool operator== (const ThreadIndex& LeftHand, const ThreadIndex& RightHand);

    /// @brief Serialize thispassed thread index for presentation in the output stream.
    /// @param OutputStream The stream to insert the Thread index into.
    /// @param OneThreadIndex The Thread Index to insert into the stream.
    /// @return A reference to the stream passed as OutputStream.
    std::ostream& operator<< (std::ostream& OutputStream, const ThreadIndex& OneThreadIndex);

    /// @brief Compare two thread indexes numerically to see if the one on the left is less than the one on the right.
    /// @param LeftHand A ThreadIndex to check if it is less than RightHand.
    /// @param RightHand Compareed to LeftHand to see if this is greater than it.
    /// @return True if the numberical value in the Left hand index is less that the numerical index inside the Right
    /// index.
    bool operator< (const ThreadIndex& LeftHand, const ThreadIndex& RightHand);

} }

#endif
