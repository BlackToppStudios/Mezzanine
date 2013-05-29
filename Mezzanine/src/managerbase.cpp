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
#ifndef _managerbase_cpp
#define _managerbase_cpp

#include "entresol.h"
#include "managerbase.h"
#include "stringtool.h"

namespace Mezzanine
{
    ManagerBase::ManagerBase()
        : PreMainLoop(NULL),
          PostMainLoop(NULL),
          Initialized(false),
          Priority(0)
    {
        this->TheEntresol = Entresol::GetSingletonPtr();
    }

    ManagerBase::~ManagerBase()
        {}

    bool ManagerBase::IsInitialized()
        { return Initialized; }

    String ManagerBase::GetInterfaceTypeAsString() const
    {
        return ManagerBase::GetTypeNameAsString(GetInterfaceType());
    }

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

    String ManagerBase::GetTypeNameAsString(const ManagerBase::ManagerType& ManagerType)
    {
        switch (ManagerType)
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
            case ManagerBase::InputManager:
                return "InputManager";
                break;
            case ManagerBase::LogManager:
                return "LogManager";
                break;
            case ManagerBase::MeshManager:
                return "MeshManager";
                break;
            case ManagerBase::NetworkManager:
                return "NetworkManager";
                break;
            case ManagerBase::PagingManager:
                return "PagingManager";
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
    }

    ManagerBase::ManagerType ManagerBase::GetTypeNameFromString(const String& ManagerName)
    {
        String Lower = ManagerName;
        StringTools::ToLowerCase(Lower);
        switch ( Lower.at(0) )
        {
            case 'a':
            {
                if( 'c' == Lower.at(1) ) return ManagerBase::ActorManager;
                else if( 'u' == Lower.at(1) ) return ManagerBase::AudioManager;
                break;
            }
            case 'c':
            {
                if( 'a' == Lower.at(1) ) return ManagerBase::CameraManager;
                else if( 'o' == Lower.at(1) ) return ManagerBase::CollisionShapeManager;
                break;
            }
            case 'e':
            {
                return ManagerBase::EventManager;
                break;
            }
            case 'g':
            {
                return ManagerBase::GraphicsManager;
                break;
            }
            case 'i':
            {
                return ManagerBase::InputManager;
                break;
            }
            case 'l':
            {
                return ManagerBase::LogManager;
                break;
            }
            case 'm':
            {
                return ManagerBase::MeshManager;
                break;
            }
            case 'n':
            {
                return ManagerBase::NetworkManager;
                break;
            }
            case 'p':
            {
                if( 'a' == Lower.at(1) ) return ManagerBase::PagingManager;
                else if( 'h' == Lower.at(1) ) return ManagerBase::PhysicsManager;
                break;
            }
            case 'r':
            {
                return ManagerBase::ResourceManager;
                break;
            }
            case 's':
            {
                // Both of these start with the same two letters, so skip to the third.
                if( 'e' == Lower.at(2) ) return ManagerBase::SceneManager;
                else if( 'r' == Lower.at(2) ) return ManagerBase::ScriptingManager;
                break;
            }
            case 't':
            {
                if( 'e' == Lower.at(1) ) return ManagerBase::TerrainManager;
                else if( 'i' == Lower.at(1) ) return ManagerBase::TimerManager;
                break;
            }
            case 'u':
            {
                if( 'i' == Lower.at(1) ) return ManagerBase::UIManager;
                else if( 's' == Lower.at(1) ) return ManagerBase::UserCreated;
                break;
            }
        }

        //If we got this far, there was a problem with the string provided.
        MEZZ_EXCEPTION(Exception::PARAMETERS_EXCEPTION,"Attempting to get ManagerType from string, but no match was found.  Is the string valid?");
    }

    short int ManagerBase::GetPriority()
    {
        return this->Priority;
    }

    void ManagerBase::SetPriority(short int Priority_)
    {
        this->Priority = Priority_;
    }

}// /Mezz

#endif
