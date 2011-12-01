//© Copyright 2010 - 2011 BlackTopp Studios Inc.
/* This file is part of The PhysGame Engine.

    The PhysGame Engine is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The PhysGame Engine is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with The PhysGame Engine.  If not, see <http://www.gnu.org/licenses/>.
*/
/* The original authors have included a copy of the license specified above in the
   'Docs' folder. See 'gpl.txt'
*/
/* We welcome the use of The PhysGame anyone, including companies who wish to
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
#ifndef _objectpair_cpp
#define _objectpair_cpp

#include "objectpair.h"
#include "actorbase.h"
#include "areaeffect.h"
//#include "terrainbase.h"

namespace phys
{
    ///////////////////////////////////////////////////////////////////////////////
    // ObjectPair
    bool ObjectPair::PairsMatch(const ObjectPair& Pair) const
    {
        bool ContainsA = ( this->GetObjectA() == Pair.GetObjectA() ) || ( this->GetObjectA() == Pair.GetObjectB() );
        bool ContainsB = ( this->GetObjectB() == Pair.GetObjectA() ) || ( this->GetObjectB() == Pair.GetObjectB() );
        return (ContainsA && ContainsB);
    }

    bool ObjectPair::operator==(const ObjectPair& Pair) const
    {
        return PairsMatch(Pair);
    }

    bool ObjectPair::operator<(const ObjectPair& Pair) const
    {
        String PairAName = this->GetCombinedName();
        String PairBName = Pair.GetCombinedName();
        return PairAName.compare(PairBName) < 0;
    }

    bool ObjectPair::operator>(const ObjectPair& Pair) const
    {
        String PairAName = this->GetCombinedName();
        String PairBName = Pair.GetCombinedName();
        return PairAName.compare(PairBName) > 0;
    }

    ///////////////////////////////////////////////////////////////////////////////
    // ActorPair

    ActorPair::ActorPair() : ActorA(NULL), ActorB(NULL)
    {
    }

    ActorPair::ActorPair(ActorBase* A, ActorBase* B)
        : ActorA(A), ActorB(B)
    {
    }

    ActorPair::ActorPair(const ActorPair& OtherPair)
    {
        ActorA = OtherPair.ActorA;
        ActorB = OtherPair.ActorB;
    }

    void* ActorPair::GetObjectA() const
    {
        return ActorA;
    }

    void* ActorPair::GetObjectB() const
    {
        return ActorB;
    }

    String ActorPair::GetCombinedName() const
    {
        return String(ActorA->GetName()+ActorB->GetName());
    }

    bool ActorPair::PairsMatch(ActorBase* A, ActorBase* B) const
    {
        bool ContainsA = (A == ActorA) || (A == ActorB);
        bool ContainsB = (B == ActorA) || (B == ActorB);
        return (ContainsA && ContainsB);
    }

    ///////////////////////////////////////////////////////////////////////////////
    // ActorTerrainPair

    ActorTerrainPair::ActorTerrainPair() : Actor(NULL), Terrain(NULL)
    {
    }

    ActorTerrainPair::ActorTerrainPair(ActorBase* A, TerrainBase* B)
        : Actor(A), Terrain(B)
    {
    }

    ActorTerrainPair::ActorTerrainPair(const ActorTerrainPair& OtherPair)
    {
        Actor = OtherPair.Actor;
        Terrain = OtherPair.Terrain;
    }

    void* ActorTerrainPair::GetObjectA() const
    {
        return Actor;
    }

    void* ActorTerrainPair::GetObjectB() const
    {
        return Terrain;
    }

    String ActorTerrainPair::GetCombinedName() const
    {
        //return String(Actor->GetName()+Terrain->GetName());
        return "";
    }

    ///////////////////////////////////////////////////////////////////////////////
    // ActorAreaEffectPair

    ActorAreaEffectPair::ActorAreaEffectPair() : Actor(NULL), AE(NULL)
    {
    }

    ActorAreaEffectPair::ActorAreaEffectPair(ActorBase* A, AreaEffect* B)
        : Actor(A), AE(B)
    {
    }

    ActorAreaEffectPair::ActorAreaEffectPair(const ActorAreaEffectPair& OtherPair)
    {
        Actor = OtherPair.Actor;
        AE = OtherPair.AE;
    }

    void* ActorAreaEffectPair::GetObjectA() const
    {
        return Actor;
    }

    void* ActorAreaEffectPair::GetObjectB() const
    {
        return AE;
    }

    String ActorAreaEffectPair::GetCombinedName() const
    {
        return String(Actor->GetName()+AE->GetName());
    }

    ///////////////////////////////////////////////////////////////////////////////
    // AreaEffectTerrainPair

    AreaEffectTerrainPair::AreaEffectTerrainPair() : AE(NULL), Terrain(NULL)
    {
    }

    AreaEffectTerrainPair::AreaEffectTerrainPair(AreaEffect* A, TerrainBase* B)
        : AE(A), Terrain(B)
    {
    }

    AreaEffectTerrainPair::AreaEffectTerrainPair(const AreaEffectTerrainPair& OtherPair)
    {
        AE = OtherPair.AE;
        Terrain = OtherPair.Terrain;
    }

    void* AreaEffectTerrainPair::GetObjectA() const
    {
        return AE;
    }

    void* AreaEffectTerrainPair::GetObjectB() const
    {
        return Terrain;
    }

    String AreaEffectTerrainPair::GetCombinedName() const
    {
        //return String(AE->GetName()+Terrain->GetName());
        return "";
    }
}//phys

#endif
