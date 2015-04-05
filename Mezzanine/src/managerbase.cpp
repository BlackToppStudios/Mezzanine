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
#ifndef _managerbase_cpp
#define _managerbase_cpp

#include "entresol.h"
#include "managerbase.h"
#include "stringtool.h"

namespace Mezzanine
{
    ManagerBase::ManagerBase() :
        TheEntresol(NULL),
        Initialized(false)
    {
        this->TheEntresol = Entresol::GetSingletonPtr();
    }

    ManagerBase::~ManagerBase()
        {}

    ///////////////////////////////////////////////////////////////////////////////
    // Utility

    ///////////////////////////////////////////////////////////////////////////////
    // Initialization Methods

    Boole ManagerBase::IsInitialized() const
        { return Initialized; }

    ///////////////////////////////////////////////////////////////////////////////
    // Type Identifier Methods

    String ManagerBase::GetInterfaceTypeAsString() const
    {
        return ManagerBase::GetTypeAsString( this->GetInterfaceType() );
    }

    String ManagerBase::GetTypeAsString(const ManagerBase::ManagerType& ManagerType)
    {
        switch( ManagerType )
        {
            case ManagerBase::MT_ActorManager:
                return "ActorManager";
                break;
            case ManagerBase::MT_AnimationManager:
                return "AnimationManager";
                break;
            case ManagerBase::MT_AreaEffectManager:
                return "AreaEffectManager";
                break;
            case ManagerBase::MT_AudioManager:
                return "AudioManager";
                break;
            case ManagerBase::MT_CollisionShapeManager:
                return "CollisionShapeManager";
                break;
            case ManagerBase::MT_CompositorManager:
                return "CompositorManager";
                break;
            case ManagerBase::MT_DebrisManager:
                return "DebrisManager";
                break;
            case ManagerBase::MT_EventManager:
                return "EventManager";
                break;
            case ManagerBase::MT_GraphicsManager:
                return "GraphicsManager";
                break;
            case ManagerBase::MT_InputManager:
                return "InputManager";
                break;
            case ManagerBase::MT_LogManager:
                return "LogManager";
                break;
            case ManagerBase::MT_MaterialManager:
                return "MaterialManager";
                break;
            case ManagerBase::MT_MeshManager:
                return "MeshManager";
                break;
            case ManagerBase::MT_NetworkManager:
                return "NetworkManager";
                break;
            case ManagerBase::MT_PagingManager:
                return "PagingManager";
                break;
            case ManagerBase::MT_PhysicsManager:
                return "PhysicsManager";
                break;
            case ManagerBase::MT_ResourceManager:
                return "ResourceManager";
                break;
            case ManagerBase::MT_SceneManager:
                return "SceneManager";
                break;
            case ManagerBase::MT_ScriptingManager:
                return "ScriptingManager";
                break;
            case ManagerBase::MT_SoundScapeManager:
                return "SoundScapeManager";
                break;
            case ManagerBase::MT_TerrainManager:
                return "TerrainManager";
                break;
            case ManagerBase::MT_TextureManager:
                return "TextureManager";
                break;
            case ManagerBase::MT_UIManager:
                return "UIManager";
                break;
            case ManagerBase::MT_VehicleManager:
                return "VehicleManager";
                break;
            case ManagerBase::MT_UserCreated:
                return "UserCreated";
                break;
            default:
                return "Unknown";
                break;
        }
    }

    ManagerBase::ManagerType ManagerBase::GetTypeFromString(const String& ManagerName)
    {
        String Lower = ManagerName;
        StringTools::ToLowerCase(Lower);
        switch ( Lower.at(0) )
        {
            case 'a':
            {
                if( 'c' == Lower.at(1) ) return ManagerBase::MT_ActorManager;
                else if( 'n' == Lower.at(1) ) return ManagerBase::MT_AnimationManager;
                else if( 'r' == Lower.at(1) ) return ManagerBase::MT_AreaEffectManager;
                else if( 'u' == Lower.at(1) ) return ManagerBase::MT_AudioManager;
                break;
            }
            case 'c':
            {
                if( 'o' == Lower.at(1) ) {
                    if( 'l' == Lower.at(2) ) return ManagerBase::MT_CollisionShapeManager;
                    else if( 'm' == Lower.at(2) ) return ManagerBase::MT_CompositorManager;
                }
                break;
            }
            case 'd':
            {
                return ManagerBase::MT_DebrisManager;
                break;
            }
            case 'e':
            {
                return ManagerBase::MT_EventManager;
                break;
            }
            case 'g':
            {
                return ManagerBase::MT_GraphicsManager;
                break;
            }
            case 'i':
            {
                return ManagerBase::MT_InputManager;
                break;
            }
            case 'l':
            {
                return ManagerBase::MT_LogManager;
                break;
            }
            case 'm':
            {
                if( 'a' == Lower.at(1) ) return ManagerBase::MT_MaterialManager;
                else if( 'e' == Lower.at(1) )return ManagerBase::MT_MeshManager;
                break;
            }
            case 'n':
            {
                return ManagerBase::MT_NetworkManager;
                break;
            }
            case 'p':
            {
                if( 'a' == Lower.at(1) ) return ManagerBase::MT_PagingManager;
                else if( 'h' == Lower.at(1) ) return ManagerBase::MT_PhysicsManager;
                break;
            }
            case 'r':
            {
                return ManagerBase::MT_ResourceManager;
                break;
            }
            case 's':
            {
                if( 'o' == Lower.at(1) ) {
                    return ManagerBase::MT_SoundScapeManager;
                }else{
                    // Both of these start with the same two letters, so skip to the third.
                    if( 'e' == Lower.at(2) ) return ManagerBase::MT_SceneManager;
                    else if( 'r' == Lower.at(2) ) return ManagerBase::MT_ScriptingManager;
                }
                break;
            }
            case 't':
            {
                if( 'r' == Lower.at(2) ) return ManagerBase::MT_TerrainManager;
                else if( 'x' == Lower.at(2) ) return ManagerBase::MT_TextureManager;
                break;
            }
            case 'u':
            {
                if( 'i' == Lower.at(1) ) return ManagerBase::MT_UIManager;
                else if( 's' == Lower.at(1) ) return ManagerBase::MT_UserCreated;
                break;
            }
            case 'v':
            {
                return ManagerBase::MT_VehicleManager;
                break;
            }
        }

        //If we got this far, there was a problem with the string provided.
        MEZZ_EXCEPTION(Exception::PARAMETERS_EXCEPTION,"Attempting to get ManagerType from string, but no match was found.  Is the string valid?");
    }
}//Mezzanine

#endif
