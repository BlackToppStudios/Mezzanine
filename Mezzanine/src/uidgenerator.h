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

#ifndef _uidgenerator_h
#define _uidgenerator_h

#include "datatypes.h"
#include "interval.h"

#ifndef SWIG
#include <cassert>
#endif

namespace Mezzanine
{
    ///////////////////////////////////////////////////////////////////////////////
    /// @brief This class will generate keep track of a pool of unique 32-bit ID's that can be used for distinct object instances.
    ///////////////////////////////////////
    template<typename IntType = UInt32>
    class MEZZ_LIB UIDGenerator
    {
    public:
        /// @brief Assert to enforce using an Integral type with the UIDGenerator.
        static_assert(std::is_integral<IntType>::value,"UIDGenerator requires the template type to be integral.");

        /// @brief Convenience enum to describe any and all constant values used by the Generator.
        enum GeneratorConstants
        {
            InvalidID = 0,   ///< An invalid/unusable value.
        };

        /// @brief Convenience type for describing the defined type in use by a given instance.
        using SelfType = UIDGenerator<IntType>;
        /// @brief Convenience type for the ID to be used.  Should be some flavor of int.
        using IDType = IntType;
        /// @brief Convenience type for free ID ranges used by this class.
        using RangeType = Interval<IDType>;
        /// @brief Basic container type for IDs generated and stored by this class.
        using IDContainer = std::vector<RangeType>;
    protected:
        /// @brief The container storing all IDs generated and in use by this generator.
        IDContainer FreeIDs;

        /// @brief Splits a currently stored Range into two Ranges at the specified ID.
        /// @param ID The ID to split the Range at.
        /// @param RangeIt An iterator to the Range that needs to be split.
        void SplitAtID(const IDType ID, typename IDContainer::iterator RangeIt)
        {
            IDContainer Temp;
            if( RangeIt != this->FreeIDs.begin() ) {
                Temp.insert(Temp.end(),this->FreeIDs.begin(),RangeIt);
            }
            Temp.push_back( RangeType((*RangeIt).GetLowerBound(),ID - 1) ); // Previous
            Temp.push_back( RangeType(ID + 1,(*RangeIt).GetUpperBound()) ); // Next
            typename IDContainer::iterator AfterIterator = RangeIt + 1;
            if( AfterIterator != this->FreeIDs.end() ) {
                Temp.insert(Temp.end(),AfterIterator,this->FreeIDs.end());
            }
            this->FreeIDs.swap(Temp);
        }
        /// @brief Merges two neighboring ranges into one.
        /// @param First The first range to be merged.
        /// @param Second The second range to be merged.
        void MergeRanges(typename IDContainer::iterator First, typename IDContainer::iterator Second)
        {
            assert( std::distance(First,Second) == 1 && "Attempting to merge non-neighboring ranges." );
            IDContainer Temp;
            Temp.insert(Temp.end(),this->FreeIDs.begin(),First);
            Temp.push_back( RangeType((*First).GetLowerBound(),(*Second).GetUpperBound()) );
            if( Second + 1 != this->FreeIDs.end() ) {
                Temp.insert(Temp.end(),Second + 1,this->FreeIDs.end());
            }
            this->FreeIDs.swap(Temp);
        }
        /// @brief Adds a single ID back into the pool of free/generate-able IDs.
        /// @param ID The ID to be re-added to the pool of free IDs.
        /// @param PrevFreeRange An iterator to the previous range of free IDs.
        /// @param NextFreeRange An iterator to the next range of free IDs.
        void AddIDToPool(const IDType ID, typename IDContainer::iterator PrevFreeRange, typename IDContainer::iterator NextFreeRange)
        {
            Boole BorderCurr = (*PrevFreeRange).GetUpperBound() + 1 == ID;
            Boole BorderNext = (*NextFreeRange).GetLowerBound() - 1 == ID;
            if( BorderCurr && BorderNext ) {
                this->MergeRanges(PrevFreeRange,NextFreeRange);
            }else if( BorderCurr ) {
                (*PrevFreeRange).UpperBound++;
            }else if( BorderNext ) {
                (*NextFreeRange).LowerBound--;
            }else{
                this->FreeIDs.insert(this->FreeIDs.begin(),RangeType(ID,ID));
            }
        }
    public:
        /// @brief Class constructor.
        UIDGenerator()
            { this->FreeIDs.push_back( RangeType(1,std::numeric_limits<IDType>::max()) ); }
        /// @brief Class destructor.
        ~UIDGenerator() = default;

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        /// @brief Sorts the free IDs stored in this generator.
        void Sort()
            { std::sort(this->FreeIDs.begin(),this->FreeIDs.end()); }

        /// @brief Generates a new ID unique to the pool made by this generator.
        /// @return Returns a new unique ID, or zero if there was a failure.
        IDType GenerateID()
        {
            IDType RetID = SelfType::InvalidID;
            if( !this->FreeIDs.empty() ) {
                RangeType& FirstRange = this->FreeIDs.front();
                RetID = FirstRange.GetLowerBound();

                if( FirstRange.GetSize() > 0 ) {
                    FirstRange.LowerBound++;
                }else{
                    this->FreeIDs.erase(this->FreeIDs.begin());
                }
            }
            return RetID;
        }
        /// @brief Checks to see if an ID is in use.
        /// @param ID The ID to check whether or not it is used.
        /// @return Returns true if the specified ID has already been generated and is in use.
        Boole IsIDUsed(const IDType ID) const
        {
            for( const RangeType CurrRange : this->FreeIDs )
            {
                if( CurrRange.IsWithinBounds(ID) ) {
                    return false;
                }
            }
            return true;
        }
        /// @brief Adds a specific ID to the pool of used IDs.
        /// @param ID The ID to be reserved.
        /// @return Returns true if the ID was successfully reserved, false if it is already generated.
        Boole ReserveID(const IDType ID)
        {
            typename IDContainer::iterator IDRangeIt = this->FreeIDs.begin();
            while( IDRangeIt != this->FreeIDs.end() )
            {
                if( (*IDRangeIt).IsWithinBounds(ID) ) {
                    if( (*IDRangeIt).GetLowerBound() == ID ) {
                        (*IDRangeIt).LowerBound++;
                        return true;
                    }
                    if( (*IDRangeIt).GetUpperBound() == ID ) {
                        (*IDRangeIt).UpperBound--;
                        return true;
                    }
                    this->SplitAtID(ID,IDRangeIt);
                    return true;
                }
                ++IDRangeIt;
            }
            return false;
        }
        /// @brief Frees up an ID so that it can be reused.
        /// @param ID The ID to be freed up for reuse.
        /// @return Returns true of the ID was successfully released, false if it was already free.
        Boole ReleaseID(const IDType ID)
        {
            if( ID == SelfType::InvalidID ) {
                MEZZ_EXCEPTION(ExceptionBase::PARAMETERS_RANGE_EXCEPTION,"Attempting to release an invalid ID.")
            }

            const typename IDContainer::iterator IDBegin = this->FreeIDs.begin();
            if( (*IDBegin).GetLowerBound() > ID ) {
                if( (*IDBegin).GetLowerBound() - 1 == ID ) {
                    (*IDBegin).LowerBound--;
                }else{
                    this->FreeIDs.insert(IDBegin,RangeType(ID,ID));
                }
                return true;
            }else{
                typename IDContainer::iterator IDRangeIt = IDBegin;
                while( IDRangeIt != this->FreeIDs.end() )
                {
                    // Check if we have an invalid/already issued ID
                    if( (*IDRangeIt).IsWithinBounds(ID) ) {
                        return false;
                    }
                    // Check the lowest value of the next range
                    typename IDContainer::iterator NextRange = IDRangeIt + 1;
                    if( (*NextRange).GetLowerBound() > ID ) {
                        this->AddIDToPool(ID,IDRangeIt,NextRange);
                        return true;
                    }
                    ++IDRangeIt;
                }
            }
            return false;
        }
    };//UIDGenerator
}//Mezzanine

#endif
