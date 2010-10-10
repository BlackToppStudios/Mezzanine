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
#ifndef _uimanager_cpp
#define _uimanager_cpp

#include "uimanager.h"
#include "internalGorilla.h.cpp"
#include "world.h"
#include "cameramanager.h"
#include "uiscreen.h"
#include "uibutton.h"

#include <Ogre.h>

namespace phys
{
    UIManager::UIManager(World* World_) : ManagerBase(World_)
    {
        Silver = new Gorilla::Silverback();
    }

    UIManager::~UIManager()
    {
        DestroyAllScreens();
        delete Silver;
    }

    void UIManager::Initialize()
    {
    }

    void UIManager::DoMainLoopItems()
    {
    }

    void UIManager::LoadGorilla(const String& Name)
    {
        Silver->loadAtlas(Name);
    }

    UIScreen* UIManager::CreateScreen(const String& Screen, const String& Atlas, const String& Viewport)
    {
        Ogre::Viewport* OgrePort = GameWorld->GetCameraManager()->GetOgreViewport(Viewport);
        Gorilla::Screen* guiscreen = Silver->createScreen(OgrePort, Atlas);
        UIScreen* physscreen = new UIScreen(Screen, guiscreen, this);
        Screens.push_back(physscreen);
        return physscreen;
    }

    UIScreen* UIManager::GetScreen(const String& Name)
    {
        std::vector<UIScreen*>::iterator it = Screens.begin();
        for ( std::vector<UIScreen*>::iterator it = Screens.begin() ; it != Screens.end() ; it++ )
        {
            if ( Name == (*it)->GetName() )
            {
                UIScreen* Screen = (*it);
                return Screen;
            }
        }
        return 0;
    }

    UIScreen* UIManager::GetScreen(Whole Index)
    {
        return Screens[Index];
    }

    Whole UIManager::GetNumScreens()
    {
        return Screens.size();
    }

    void UIManager::DestroyScreen(UIScreen* Screen)
    {
        if(Screens.empty())
            return;
        for( std::vector<UIScreen*>::iterator it = Screens.begin() ; it != Screens.end() ; it++ )
        {
            if( Screen == (*it) )
            {
                delete (*it);
                Screens.erase(it);
                return;
            }
        }
    }

    void UIManager::DestroyAllScreens()
    {
        if(Screens.empty())
            return;
        for( std::vector<UIScreen*>::iterator it = Screens.begin() ; it != Screens.end() ; it++ )
        {
            delete (*it);
            Screens.erase(it);
        }
        return;
    }

    ManagerBase::ManagerTypeName UIManager::GetType() const
        { return ManagerBase::UIManager; }

    Gorilla::Silverback* UIManager::GetSilverbackPointer()
    {
        return Silver;
    }
}//phys

#endif
