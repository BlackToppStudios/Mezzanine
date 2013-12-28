// The DAGFrameScheduler is a Multi-Threaded lock free and wait free scheduling library.
// © Copyright 2010 - 2014 BlackTopp Studios Inc.
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
#ifndef _timeddagworkunitkey_cpp
#define _timeddagworkunitkey_cpp

#include "workunitkey.h"


/// @file
/// @brief This file declares the functionality of the metadata used to sort the @ref Mezzanine::Threading::iWorkUnit "iWorkUnit"s.

namespace Mezzanine
{
    namespace Threading
    {
        WorkUnitKey::WorkUnitKey()
            : Unit(0), Dependers(0), Time(0)
            {}

        WorkUnitKey::WorkUnitKey(const Whole& Dependers_, const Whole& Time_, iWorkUnit* WorkUnit_)
            : Unit(WorkUnit_), Dependers(Dependers_), Time(Time_)
            {}

        WorkUnitKey::~WorkUnitKey()
            {}

        bool WorkUnitKey::operator< (const WorkUnitKey& rhs ) const
        {
            if (this->Dependers < rhs.Dependers) // reduce priority of Items with fewer dependents
                { return true; }
            if (this->Dependers == rhs.Dependers)
            {
                if (this->Time < rhs.Time)       // reduce priority of faster workunits
                    { return true; }
                if (this->Time == rhs.Time)
                    { return this->Unit < rhs.Unit; }
            }
            return false;
        }

        bool WorkUnitKey::operator ==(const WorkUnitKey &rhs) const
            { return Unit == rhs.Unit; }
    }//Threading
}//Mezzanine
#endif
