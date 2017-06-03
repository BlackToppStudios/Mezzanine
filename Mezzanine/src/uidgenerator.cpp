// © Copyright 2010 - 2017 BlackTopp Studios Inc.
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

#ifndef _uidgenerator_cpp
#define _uidgenerator_cpp

#include "uidgenerator.h"

#include <limits>

namespace Mezzanine
{
    UIDGenerator::UIDGenerator()
    {
        // Create the full range of valid ID's
        this->FreeIDs.push_back( IntervalType(1,std::numeric_limits<IDType>::max()) );
    }

    UIDGenerator::~UIDGenerator()
        {  }

    ///////////////////////////////////////////////////////////////////////////////
    // Utility

    UIDGenerator::IDType UIDGenerator::GenerateID()
    {
        IDType RetID = 0;
        if( !this->FreeIDs.empty() ) {
            IntervalType& FirstInterval = this->FreeIDs.front();
            RetID = FirstInterval.GetLowerBound();

            if( FirstInterval.GetIntervalSize() > 0 ) {
                FirstInterval.LowerBound++;
            }else{
                this->FreeIDs.erase(this->FreeIDs.begin());
            }
        }
        return RetID;
    }

    Boole UIDGenerator::IsIDUsed(const UIDGenerator::IDType ID) const
    {
        for( ConstIDIterator IDRangeIt = this->FreeIDs.begin() ; IDRangeIt != this->FreeIDs.end() ; ++IDRangeIt )
        {
            if( (*IDRangeIt).IsWithinBounds(ID) ) {
                return false;
            }
        }
        return true;
    }

    Boole UIDGenerator::ReserveID(const UIDGenerator::IDType ID)
    {
        for( IDIterator IDRangeIt = this->FreeIDs.begin() ; IDRangeIt != this->FreeIDs.end() ; ++IDRangeIt )
        {
            // Find the range we're in
            if( (*IDRangeIt).IsWithinBounds(ID) ) {
                // Rule out being on the edges
                if( (*IDRangeIt).GetLowerBound() == ID ) {
                    // Just increment the bounds
                    (*IDRangeIt).LowerBound++;
                    return true;
                }
                if( (*IDRangeIt).GetUpperBound() == ID ) {
                    // Just decrement the bounds
                    (*IDRangeIt).UpperBound--;
                    return true;
                }

                // Gonna have to make a split
                IDContainer Temp;

                // Get all the ranges before the split into our new container
                if( IDRangeIt != this->FreeIDs.begin() ) {
                    Temp.insert(Temp.end(),this->FreeIDs.begin(),IDRangeIt);
                }

                // Perform the split
                Temp.push_back( IntervalType((*IDRangeIt).GetLowerBound(),ID - 1) ); // Previous
                Temp.push_back( IntervalType(ID + 1,(*IDRangeIt).GetUpperBound()) ); // Next

                // Get all the ranges after the split into our container
                IDIterator AfterIterator = IDRangeIt + 1;
                if( AfterIterator != this->FreeIDs.end() ) {
                    Temp.insert(Temp.end(),AfterIterator,this->FreeIDs.end());
                }

                // Cleanup
                this->FreeIDs.swap(Temp);
                return true;
            }
        }
        return false;
    }

    Boole UIDGenerator::ReleaseID(const UIDGenerator::IDType ID)
    {
        // Only proceed if we are working with a valid ID
        if( ID >= 1 ) {
            // Check the simple case
            IDIterator IDBegin = this->FreeIDs.begin();
            if( (*IDBegin).GetLowerBound() > ID ) {
                if( (*IDBegin).GetLowerBound() - 1 == ID ) {
                    (*IDBegin).LowerBound--;
                }else{
                    this->FreeIDs.insert(IDBegin,IntervalType(ID,ID));
                }
                return true;
            }else{
                // Ok, the ID being released isn't before the first free ID interval
                for( IDIterator IDRangeIt = IDBegin ; IDRangeIt != this->FreeIDs.end() ; ++IDRangeIt )
                {
                    // Check if we have an invalid/already issued ID
                    if( (*IDRangeIt).IsWithinBounds(ID) ) {
                        return false;
                    }
                    // Check the lowest value of the next range
                    IDIterator NextRange = IDRangeIt + 1;
                    if( NextRange == this->FreeIDs.end() ) {
                        MEZZ_EXCEPTION(ExceptionBase::INVALID_STATE_EXCEPTION,"Internal state error or all possible IDs have been generated/issued.");
                    }
                    // Check if we're in the right spot or increment and try again
                    if( (*NextRange).GetLowerBound() > ID ) {
                        Boole BorderPrev = (*IDRangeIt).GetUpperBound() + 1 == ID;
                        Boole BorderNext = (*NextRange).GetLowerBound() - 1 == ID;
                        if( BorderPrev && BorderNext ) {
                            // This is the last ID separating two intervals, gotta merge
                            IDContainer Temp;
                            Temp.insert(Temp.end(),this->FreeIDs.begin(),IDRangeIt);
                            Temp.push_back( IntervalType((*IDRangeIt).GetLowerBound(),(*NextRange).GetUpperBound()) );
                            if( NextRange + 1 != this->FreeIDs.end() ) {
                                Temp.insert(Temp.end(),NextRange + 1,this->FreeIDs.end());
                            }
                            this->FreeIDs.swap(Temp);
                        }else if( BorderPrev ) {
                            (*IDRangeIt).UpperBound++;
                        }else if( BorderNext ) {
                            (*NextRange).LowerBound--;
                        }else{
                            this->FreeIDs.insert(IDBegin,IntervalType(ID,ID));
                        }
                        return true;
                    }
                }
            }
        }
        return false;
    }

    void UIDGenerator::Sort()
        { std::sort(this->FreeIDs.begin(),this->FreeIDs.end()); }
}//Mezzanine

#endif
