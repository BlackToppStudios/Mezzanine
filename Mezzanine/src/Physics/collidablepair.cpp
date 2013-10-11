//© Copyright 2010 - 2013 BlackTopp Studios Inc.
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
#ifndef _collidablepair_cpp
#define _collidablepair_cpp

#include "collidablepair.h"
#include "collidableproxy.h"

namespace Mezzanine
{
    namespace Physics
    {
        ///////////////////////////////////////////////////////////////////////////////
        // Construction and Destruction

        CollidablePair::CollidablePair(CollidableProxy* A, CollidableProxy* B) :
            ProxyA(A), ProxyB(B)
            {  }

        CollidablePair::~CollidablePair()
            {  }

        UInt64 CollidablePair::GetPairID() const
        {
            UInt64 AID = static_cast<UInt64>( this->ProxyA->_GetBroadphaseUniqueID() );
            UInt64 BID = static_cast<UInt64>( this->ProxyB->_GetBroadphaseUniqueID() );
            return UInt64( ( AID << 32 ) | ( BID ) );
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        bool CollidablePair::PairsMatch(const CollidablePair& Pair) const
        {
            bool ContainsA = ( this->ProxyA == Pair.ProxyA ) || ( this->ProxyA == Pair.ProxyB );
            bool ContainsB = ( this->ProxyB == Pair.ProxyA ) || ( this->ProxyB == Pair.ProxyB );
            return (ContainsA && ContainsB);
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Operators

        bool CollidablePair::operator==(const CollidablePair& Pair) const
            { return this->PairsMatch(Pair); }

        bool CollidablePair::operator<(const CollidablePair& Pair) const
            { return this->GetPairID() < Pair.GetPairID(); }

        bool CollidablePair::operator>(const CollidablePair& Pair) const
            { return this->GetPairID() > Pair.GetPairID(); }
    }//Physics
}//Mezzanine

#endif
