// © Copyright 2010 - 2014 BlackTopp Studios Inc.
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

#ifndef _world_cpp
#define _world_cpp

#include "world.h"
#include "Physics/physicsmanager.h"
#include "Graphics/scenemanager.h"
//#include "pagingmanager.h"
#include "actormanager.h"
#include "areaeffectmanager.h"
#include "Graphics/cameramanager.h"
#include "Audio/soundscapemanager.h"
#include "terrainmanager.h"

namespace Mezzanine
{
    World::World(const String& WorldName) :
        Name(WorldName)
    {
        Physics::ManagerConstructionInfo PhysicsInfo;
        std::vector <WorldManager*> temp;

        this->Construct(PhysicsInfo,"DefaultSceneManager",temp);
    }

    World::World(const String& WorldName, const WorldManagerContainer& Managers) :
        Name(WorldName)
    {
        Physics::ManagerConstructionInfo PhysicsInfo;

        this->Construct(PhysicsInfo,"DefaultSceneManager",Managers);
    }

    World::World(const String& WorldName, const Physics::ManagerConstructionInfo& PhysicsInfo, const String& SceneType) :
        Name(WorldName)
    {
        std::vector <WorldManager*> temp;

        this->Construct(PhysicsInfo, SceneType, temp);
    }

    World::World(const String& WorldName, const WorldManagerContainer& Managers, const Physics::ManagerConstructionInfo& PhysicsInfo, const String& SceneType) :
        Name(WorldName)
    {
        this->Construct(PhysicsInfo, SceneType, Managers);
    }

    World::World(const XML::Node& SelfNode)
    {

    }

    World::~World()
    {
        this->Deinitialize();
    }

    void World::Construct(const Physics::ManagerConstructionInfo& PhysicsInfo,
            const String& SceneType, const std::vector <WorldManager*>& ManagerToBeAdded  )
    {
        //add each manager that was passed in to the manager list
        for(std::vector<WorldManager*>::const_iterator iter = ManagerToBeAdded.begin(); iter!= ManagerToBeAdded.end(); iter++)
            { this->AddManager(*iter); }

        if(this->GetActorManager()==0)
            { this->AddManager(new ActorManager()); }
        if(this->GetAreaEffectManager()==0)
            { this->AddManager(new AreaEffectManager()); }
        if(this->GetCameraManager()==0)
            { this->AddManager(new Graphics::CameraManager()); }
        if(this->GetPhysicsManager()==0)
            { this->AddManager(new Physics::PhysicsManager(PhysicsInfo)); }
        if(this->GetSceneManager()==0)
            { this->AddManager(new Graphics::SceneManager(SceneType)); }
    }

    const String& World::GetName() const
    {
        return this->Name;
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Initialization

    void World::Initialize()
    {
        for( WorldManagerIterator ManIter = this->WorldManagers.begin() ; ManIter != this->WorldManagers.end() ; ++ManIter )
        {
            (*ManIter)->Initialize();
        }
    }

    void World::Deinitialize()
    {
        for( WorldManagerIterator ManIter = this->WorldManagers.begin() ; ManIter != this->WorldManagers.end() ; ++ManIter )
        {
            (*ManIter)->Deinitialize();
        }
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Upper Management

    Boole World::AddManager(WorldManager* ManagerToAdd)
    {
        ManagerBase::ManagerType IType = ManagerToAdd->GetInterfaceType();
        for( WorldManagerIterator ManIter = this->WorldManagers.begin() ; ManIter != this->WorldManagers.end() ; ++ManIter )
        {
            if( (*ManIter)->GetInterfaceType() == IType )
                return false;
        }
        this->WorldManagers.push_back(ManagerToAdd);
        return true;
    }

    WorldManager* World::GetManager(const ManagerBase::ManagerType ManagerToGet)
    {
        for( WorldManagerIterator ManIter = this->WorldManagers.begin() ; ManIter != this->WorldManagers.end() ; ++ManIter )
        {
            if( (*ManIter)->GetInterfaceType() == ManagerToGet )
                return *ManIter;
        }
        return NULL;
    }

    void World::RemoveManager(WorldManager* ToBeRemoved)
    {
        for( WorldManagerIterator ManIter = this->WorldManagers.begin() ; ManIter != this->WorldManagers.end() ; ++ManIter )
        {
            if( (*ManIter) == ToBeRemoved )
            {
                this->WorldManagers.erase(ManIter);
                return;
            }
        }
    }

    void World::RemoveManager(const ManagerBase::ManagerType ToBeRemoved)
    {
        for( WorldManagerIterator ManIter = this->WorldManagers.begin() ; ManIter != this->WorldManagers.end() ; ++ManIter )
        {
            if( (*ManIter)->GetInterfaceType() == ToBeRemoved )
            {
                this->WorldManagers.erase(ManIter);
                return;
            }
        }
    }

    void World::RemoveAllManagers()
    {
        WorldManagers.clear();
    }

    std::vector<WorldManager*> World::GetWorldManagers()
    {
        return WorldManagers;
    }

    ActorManager* World::GetActorManager()
    {
        return static_cast<ActorManager*>( this->GetManager(ManagerBase::MT_ActorManager) );
        //return NULL;
    }

    AreaEffectManager* World::GetAreaEffectManager()
    {
        return static_cast<AreaEffectManager*>( this->GetManager(ManagerBase::MT_AreaEffectManager) );
        //return NULL;
    }

    Graphics::CameraManager* World::GetCameraManager()
    {
        return static_cast<Graphics::CameraManager*>( this->GetManager(ManagerBase::MT_CameraManager) );
        //return NULL;
    }

    Physics::PhysicsManager* World::GetPhysicsManager()
    {
        return static_cast<Physics::PhysicsManager*>( this->GetManager(ManagerBase::MT_PhysicsManager) );
        //return NULL;
    }

    Graphics::SceneManager* World::GetSceneManager()
    {
        return static_cast<Graphics::SceneManager*>( this->GetManager(ManagerBase::MT_SceneManager) );
        //return NULL;
    }

    Audio::SoundScapeManager* World::GetSoundScapeManager()
    {
        return static_cast<Audio::SoundScapeManager*>( this->GetManager(ManagerBase::MT_SoundScapeManager) );
        //return NULL;
    }

    TerrainManager* World::GetTerrainManager()
    {
        return static_cast<TerrainManager*>( this->GetManager(ManagerBase::MT_TerrainManager) );
        //return NULL;
    }
}//Mezzanine

#endif
