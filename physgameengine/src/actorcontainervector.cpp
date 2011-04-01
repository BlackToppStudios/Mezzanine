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
#ifndef actorcontainervector_cpp
#define actorcontainervector_cpp

#include "actorcontainervector.h"


namespace phys
{
    ActorContainerVector::ActorContainerVector ()
        { this->Priority = -40; }

    ActorContainerVector::~ActorContainerVector ()
    {
        for( std::vector<ActorBase*>::iterator it = this->begin() ; it != this->end() ; it++ )
            delete (*it);
        this->clear();
    }

    void ActorContainerVector::AddActor(ActorBase* ActorToAdd)
    {
        this->RecentlyAdded = ActorToAdd;
        this->push_back(ActorToAdd);
        this->ActorContainerBase::AddActor(ActorToAdd);
    }

    ActorBase* ActorContainerVector::LastActorAdded()
        { return this->RecentlyAdded; }

    void ActorContainerVector::RemoveActor(ActorBase* ActorToRemove)
    {
        this->ActorContainerBase::RemoveActor(ActorToRemove);
        //we need to iterate through and remove all items of that match the actor to match the description in the container base
        for( vector<ActorBase*>::iterator c=this->begin(); c!=this->end(); c++)
        {
            if ( ActorToRemove == *c )
            {
                c=this->erase(c);
            }
        }
    }

    void ActorContainerVector::RemoveActorAtCursor()
        { this->cursor=this->erase(this->cursor); }

    Whole ActorContainerVector::GetActorCount() const
        { return this->size(); }

    void ActorContainerVector::CursorToFirst()
    {
        if(this->empty())
            throw("Cannot move Cursor in an empty container");

        this->cursor=this->begin();
    }

    void ActorContainerVector::CursorToPrevious()
    {
        if(this->empty())
            throw("Cannot move Cursor in an empty container");

        if(this->cursor != this->begin())
            this->cursor--;
    }

    void ActorContainerVector::CursorToNext()
    {
        if(this->empty())
            throw("Cannot move Cursor in an empty container");

        if(this->cursor != this->end())
            this->cursor++;
    }

    void ActorContainerVector::CursorToLast()
    {
        if(this->empty())
            throw("Cannot move Cursor in an empty container");

        this->cursor=this->end();
    }

    ActorBase* ActorContainerVector::GetAtCursor() const
    {
        if(this->empty())
            throw("Cannot get from in an empty container");

        return *(this->cursor);
    }

    ActorBase* ActorContainerVector::GetFirst() const
        { return *(this->begin()); }

    ActorBase* ActorContainerVector::GetLast() const
        { return *(this->end()); }

    String ActorContainerVector::GetContainerType() const
        { return String("phys::ActorContainerVector"); }

    ActorBase* ActorContainerVector::FindActor(btCollisionObject* PhysicsObject)
    {
        //we need to iterate through and remove all items of that match the actor to match the description in the container base
        for( vector<ActorBase*>::iterator c=this->begin(); c!=this->end(); c++)
        {
            if ( PhysicsObject == GetCollisionObject(*c) )
            { return *c; }
        }
        return NULL;
    }

    ActorBase* ActorContainerVector::FindActor(Ogre::SceneNode* GraphicsNode)
    {
        //we need to iterate through and remove all items of that match the actor to match the description in the container base
        for( vector<ActorBase*>::iterator c=this->begin(); c!=this->end(); c++)
        {
            if ( GraphicsNode == GetNode(*c) )
            { return *c; }
        }
        return NULL;
    }

    ActorBase* ActorContainerVector::FindActor(String Name)
    {
        //we need to iterate through and remove all items of that match the actor to match the description in the container base
        for( vector<ActorBase*>::iterator c=this->begin(); c!=this->end(); c++)
        {
            if ( Name == (*c)->GetName() )
            { return *c; }
        }
        return NULL;
    }

    ///////////////////////////////////////////////////////////////////////////////////
    // WorldGetSet/ManagerBase Members
    ///////////////////////////////////
    World* ActorContainerVector::GetGameWorld() const
        { return this->GameWorld; }

    void ActorContainerVector::SetGameWorld( World* GameWorld_ )
        { this->SetGameWorld( GameWorld_, true, true ); }

    void ActorContainerVector::SetGameWorld( World* GameWorld_, bool AddToWorld, bool RemoveFromWorld)
    {
        if (RemoveFromWorld && World::GetWorldPointer() != NULL)
        {
            for( vector<ActorBase*>::iterator c=this->begin(); c!=this->end(); c++)
                { (*c)->RemoveObjectFromWorld(World::GetWorldPointer()); }
        }

        this->GameWorld = GameWorld_;

        if (AddToWorld && this->GameWorld != NULL)
        {
            for( vector<ActorBase*>::iterator c=this->begin(); c!=this->end(); c++)
                { (*c)->AddObjectToWorld(this->GameWorld); }
        }
    }

    void ActorContainerVector::Initialize()
        {}

    void ActorContainerVector::DoMainLoopItems()
        {}
}


#endif
