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
#ifndef _timeddagworkunitkey_h
#define _timeddagworkunitkey_h

#include "datatypes.h"

/// @file
/// @brief This file defines the metadata used to sort workunits.

namespace Mezzanine
{
    namespace Threading
    {
        class iWorkUnit;

        /// @brief Stores data about a single work unit so that it can easily be sorted.
        class MEZZ_LIB WorkUnitKey
        {
            public:
				/// @brief The WorkUnit this carries metadata for.
				iWorkUnit* Unit;
                
				/// @brief This stores how many things must run after the target workunit.
                /// @details This is most important sorting factor. In the Default algorithm.
                /// All of the Workunit that the most workunits depend on are run before the
                /// any others. Since this number is usually low (less than 10ish) in large
                /// groups of workunits, then there must be a tie-breaker. This should be
                /// calculated including the dependers of all the dependers (I don't think
                /// counting a workunit twice hurts, and it may even help if something
                /// actually depend on multiple workunits that depend on this.)
                /// @note I chose the word depender instead of dependent, to make the distinction between this field and dependencies more clear when communicated verbally. In some english dialects dependencies is pronounced Dee-pen-dent-sees only one syllable off from Dee-pen-dent.
                Whole Dependers;

                /// @brief Some representation of the time the target workunit takes to execute.
                /// @details In the default algorithm this is used to determine what executes
                /// first after dependents are counted. It is important to know what takes the longest time
                /// and start it first so that it might finish coterminously with shorter
                /// workunits. This is used second because length of execution of one workunit
                /// means nothing if many other workunits run serialized in one thread rather than
                /// in parrellel.
                Whole Time;

                /// @brief Default Constructor
                /// @details This creates an empty and quite useless Key, needs to be filled with data before being useful.
                WorkUnitKey();

                /// @brief Constructor
                /// @param Dependers_ How many items depend on this. This needs to be calculated the same for all WorkUnitKeys.
                /// @param Time_ How long is this workunit expected to execute for. This needs to be calculated the same for all WorkUnitKeys.
                /// @param WorkUnit_ A pointer to the workunit in question.
                WorkUnitKey(const Whole& Dependers_, const Whole& Time_, iWorkUnit* WorkUnit_);

                /// @brief Destructor
                virtual ~WorkUnitKey();

                /// @brief The function that does the comparison in most containers.
                /// @param rhs The right hand WorkUnitKey when using <.
                /// @details This enforces the algorithm that sorts workunits. The workunit with the most
                /// other work units depending on it is priotized first. Because the longest delays in
                /// execution time can be introduced if WorkUnits are waiting for a dependency and it is
                /// poorly scheduled, this can results in single threaded execution when more could have
                /// been possible. If there is a tie in dependent count, then the work unit that takes
                /// longer to execute is prioritized (this needs to be tested for correctness). This
                /// starts WorkUnits that take longer earlier in the frame, helping to minimize the
                /// length of the critical path. In the event that those are both tied, then the WorkUnit
                /// with the lowest pointer value is priotized to prevent any of WorkUnitKey from comparing
                /// identically.
                /// @return True if the left hand WorkUnit should be prioritized ahead of the right hand work unit.
                virtual bool operator< (const WorkUnitKey& rhs ) const;

                /// @brief Does this key point to the Same work unit as another?
                /// @param rhs The other key.
                /// @return True if the both keys are for the same unit, other parameters are ignored.
                virtual bool operator== (const WorkUnitKey& rhs ) const;
        };//WorkUnitKey
    }//Threading
}//Mezzanine
#endif
