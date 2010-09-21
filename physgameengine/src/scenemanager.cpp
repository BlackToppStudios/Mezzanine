//© Copyright 2010 BlackTopp Studios Inc.
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
#ifndef _scenemanager_cpp
#define _scenemanager_cpp

#include "scenemanager.h"
#include "light.h"
#include <Ogre.h>

namespace phys
{
    SceneManager::SceneManager(const String& Name, SceneManager::SceneManagerType ManagerType, World* world) : ManagerBase(world)
    {
        Ogre::SceneType Type;
        switch (ManagerType)
        {
            case Generic:
                Type = Ogre::ST_GENERIC;
                break;
            case Exterior:
                Type = Ogre::ST_EXTERIOR_FAR;
                break;
            case ExteriorRealFar:
                Type = Ogre::ST_EXTERIOR_REAL_FAR;
                break;
            case Interior:
                Type = Ogre::ST_INTERIOR;
                break;
            default:
                Type = Ogre::ST_GENERIC;
        }
        OgreManager = Ogre::Root::getSingleton().createSceneManager(Type, Name);
    }

    SceneManager::~SceneManager()
    {
        Ogre::Root::getSingleton().destroySceneManager(OgreManager);
    }

    void SceneManager::Initialize()
    {
    }

    void SceneManager::DoMainLoopItems()
    {
    }

    void SceneManager::SetAmbientLight(Real Red, Real Green, Real Blue, Real Alpha)
    {
        this->OgreManager->setAmbientLight(Ogre::ColourValue(Red, Green, Blue, Alpha));
    }

    Light* SceneManager::CreateLight(const String& Name)
    {
        Light* light = new Light(this->OgreManager->createLight(Name), this);
        Lights.push_back(light);
        return light;
    }

    Light* SceneManager::GetLight(const String& Name)
    {
        if(Lights.empty())
            return 0;
        for( std::vector<Light*>::iterator it = Lights.begin() ; it != Lights.end() ; it++ )
        {
            if( Name == (*it)->GetName() )
            {
                return (*it);
            }
        }
        return 0;
    }

    void SceneManager::DestroyLight(Light* light)
    {
        if(Lights.empty())
            return;
        for( std::vector<Light*>::iterator it = Lights.begin() ; it != Lights.end() ; it++ )
        {
            if( light == (*it) )
            {
                delete (*it);
                Lights.erase(it);
                return;
            }
        }
    }

    String& SceneManager::GetName()
    {
        return this->OgreManager->getName();
    }

    ManagerBase::ManagerTypeName SceneManager::GetType() const
        { return ManagerBase::SceneManager; }

    Ogre::SceneManager* SceneManager::GetGraphicsWorldPointer()
    {
        return OgreManager;
    }
}

#endif
