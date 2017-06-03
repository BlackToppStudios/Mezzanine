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
#ifndef _collidablepair_h
#define _collidablepair_h

#include "datatypes.h"

namespace Mezzanine
{
    namespace Physics
    {
        class CollidableProxy;
        ///////////////////////////////////////////////////////////////////////////////
        /// @brief This is a helper class for storing pairs of collidable proxies in associative containers.
        /// @details Most commonly this class is used in collision tracking.
        ///////////////////////////////////////
        class MEZZ_LIB CollidablePair
        {
        protected:
            /// @internal
            /// @brief Gets the combined ID of the objects in this pair.
            virtual UInt64 GetPairID() const;
        public:
            ///////////////////////////////////////////////////////////////////////////////
            // Public Data Members

            /// @brief The first world object in the pair.
            CollidableProxy* ProxyA;
            /// @brief The second world object in the pair.
            CollidableProxy* ProxyB;

            ///////////////////////////////////////////////////////////////////////////////
            // Construction and Destruction

            /// @brief Class constructor.
            /// @param A The first proxy in the collision pair.
            /// @param B The second proxy in the collision pair.
            CollidablePair(CollidableProxy* A, CollidableProxy* B);
            /// @brief Class destructor.
            virtual ~CollidablePair();

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            /// @brief Checks to see if this pair is the same as another pair.
            /// @param Pair The other pair to compare against.
            /// @return Returns true if the pairs match regardless of ordering, false otherwise.
            virtual Boole PairsMatch(const CollidablePair& Pair) const;

            ///////////////////////////////////////////////////////////////////////////////
            // Operators

            /// @brief Checks to see if this pair is the same as another pair.
            /// @param Pair The other pair to compare against.
            /// @return Returns true if the pairs match regardless of ordering, false otherwise.
            virtual Boole operator==(const CollidablePair& Pair) const;
            /// @brief Checks to see if this pair is less than another pair.
            /// @remarks This check is based on combining the names of the objects and performing a std::string::compare on them.
            /// @param Pair The other pair to compare against.
            /// @return Returns true if this pair is found to be less then the passed in pair, false otherwise.
            virtual Boole operator<(const CollidablePair& Pair) const;
            /// @brief Checks to see if this pair is greater than another pair.
            /// @remarks This check is based on combining the names of the objects and performing a std::string::compare on them.
            /// @param Pair The other pair to compare against.
            /// @return Returns true if this pair is found to be greater then the passed in pair, false otherwise.
            virtual Boole operator>(const CollidablePair& Pair) const;
        };// CollidablePair
    }//Physics
}//Mezzanine

#endif
