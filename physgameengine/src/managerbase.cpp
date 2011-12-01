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
#ifndef _managerbase_cpp
#define _managerbase_cpp

#include "managerbase.h"
#include "world.h"

namespace phys
{
    ManagerBase::ManagerBase()
    {
        this->GameWorld = World::GetWorldPointer();
        this->PreMainLoop = NULL;
        this->PostMainLoop = NULL;
        this->Priority = 0;
    }

    /*ManagerBase::ManagerBase(World* GameWorld_)
    {
        this->GameWorld = GameWorld_;
        this->PreMainLoop = NULL;
        this->PostMainLoop = NULL;
        this->Priority = 0;
    }*/

    World* ManagerBase::GetGameWorld() const
        { return this->GameWorld; }

    void ManagerBase::SetGameWorld( World* GameWorld_ )
        { this->GameWorld = GameWorld_; }

    ManagerBase::~ManagerBase()
        {}

    void ManagerBase::SetPreMainLoopItems( ManagerBase::Callback PreMainCallback )
        { this->PreMainLoop=PreMainCallback; }

    bool ManagerBase::PreMainLoopItems()
    {
        if(NULL != this->PreMainLoop)
        {
            return (*( this->PreMainLoop))();
        }
        return true;
    }

    ManagerBase::Callback ManagerBase::GetPreMainLoopItems() const
        { return this->PreMainLoop; }

    void ManagerBase::ErasePreMainLoopItems()
        { this->SetPreMainLoopItems(0); }


    void ManagerBase::SetPostMainLoopItems( ManagerBase::Callback PostMainCallback )
        { this->PostMainLoop=PostMainCallback; }

    bool ManagerBase::PostMainLoopItems()
    {
        if(NULL != this->PostMainLoop)
        {
            return (*( this->PostMainLoop))();
        }
        return true;
    }

    ManagerBase::Callback ManagerBase::GetPostMainLoopItems() const
        { return this->PostMainLoop; }

    void ManagerBase::ErasePostMainLoopItems()
        { this->SetPostMainLoopItems(0); }

    std::string ManagerBase::GetTypeName()
    {
        switch (this->GetType())
        {
            case ManagerBase::ActorManager:
                return "ActorManager";
                break;
            case ManagerBase::AudioManager:
                return "AudioManager";
                break;
            case ManagerBase::CameraManager:
                return "CameraManager";
                break;
            case ManagerBase::CollisionShapeManager:
                return "CollisionShapeManager";
                break;
            case ManagerBase::EventManager:
                return "EventManager";
                break;
            case ManagerBase::GraphicsManager:
                return "GraphicsManager";
                break;
            case ManagerBase::MeshManager:
                return "MeshManager";
                break;
            case ManagerBase::PhysicsManager:
                return "PhysicsManager";
                break;
            case ManagerBase::ResourceManager:
                return "ResourceManager";
                break;
            case ManagerBase::SceneManager:
                return "SceneManager";
                break;
            case ManagerBase::ScriptingManager:
                return "ScriptingManager";
                break;
            case ManagerBase::TerrainManager:
                return "TerrainManager";
                break;
            case ManagerBase::TimerManager:
                return "TimerManager";
                break;
            case ManagerBase::UIManager:
                return "UIManager";
                break;
            case ManagerBase::UserCreated:
                return "UserCreated";
                break;
            default:
                return "Unknown";
                break;
        }
    }// */

    short int ManagerBase::GetPriority()
    {
        return this->Priority;
    }

    void ManagerBase::SetPriority(short int Priority_)
    {
        this->Priority = Priority_;
    }

}// /phys

#endif
