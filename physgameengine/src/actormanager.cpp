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
#ifndef actormanager_cpp
#define actormanager_cpp

#include "actormanager.h"
#include "actorcontainervector.h"

#include <sstream>

namespace phys
{
    ActorManager::ActorManager()
    {
        Priority = -25;
        Actors = new ActorContainerVector();
    }

    ActorManager::~ActorManager()
    {
        delete Actors;
    }

    ActorBase* ActorManager::GetActor(const Whole& Index)
    {
        return 0;
    }

    ActorBase* ActorManager::GetActor(const String& Name)
    {
        return Actors->FindActor(Name);
    }

    Whole ActorManager::GetNumActors()
    {
        return Actors->GetActorCount();
    }

    void ActorManager::AddActor(ActorBase* Actor)
    {
        Actors->AddActor(Actor);
        Actor->AddObjectToWorld();
    }

    void ActorManager::RemoveActor(ActorBase* Actor)
    {
        Actors->RemoveActor(Actor);
        Actor->RemoveObjectFromWorld();
    }

    void ActorManager::RemoveAllActors()
    {
        if( 0 == Actors->GetActorCount() )
            return;
        ActorBase* Act = NULL;
        Actors->CursorToFirst();
        for( Whole ActCur = 0 ; ActCur < Actors->GetActorCount() ; ActCur++ )
        {
            Act = Actors->GetAtCursor();
            Act->RemoveObjectFromWorld();
            Actors->CursorToNext();
        }
        Actors->RemoveAllActors();
        RigidActors.clear();
        SoftActors.clear();
        CharacterActors.clear();
    }

    void ActorManager::DestroyActor(ActorBase* Actor)
    {
        RemoveActor(Actor);
        delete Actor;
    }

    void ActorManager::DestroyAllActors()
    {
        if( 0 == Actors->GetActorCount() )
            return;
        ActorBase* Act = NULL;
        Actors->CursorToFirst();
        for( Whole ActCur = 0 ; ActCur < Actors->GetActorCount() ; ActCur++ )
        {
            Act = Actors->GetAtCursor();
            Act->RemoveObjectFromWorld();
            delete Act;
            Actors->CursorToNext();
        }
        Actors->RemoveAllActors();
        RigidActors.clear();
        SoftActors.clear();
        CharacterActors.clear();
    }

    ActorContainerBase* ActorManager::GetActorContainer()
    {
        return Actors;
    }

    //Inherited From ManagerBase
    void ActorManager::Initialize()
    {
    }

    void ActorManager::DoMainLoopItems()
    {
    }

    ManagerBase::ManagerTypeName ActorManager::GetType() const
        { return ManagerBase::ActorManager; }
}

#endif
