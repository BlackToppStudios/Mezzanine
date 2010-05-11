//© Copyright 2010 Joseph Toppi and John Blackwood
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
#ifndef WORLDCALLBACKMANAGER_CPP
#define WORLDCALLBACKMANAGER_CPP


//Includes
#include "worldcallbackmanager.h"
#include "datatypes.h"
#include "physworld.h"

using namespace std;

namespace phys
{
    ///////////////////////////////////////////////////////////////////////////////
    // PhysWorldCallBackManager Constructor
    ///////////////////////////////////////
    WorldCallBackManager::WorldCallBackManager(PhysWorld* _Parent): PreInputCallback(NULL)
    {
        TheWorldIListenTo = _Parent;

        PreInputCallback = NULL;
        PostInputCallback = NULL;

        PrePhysicsCallback = NULL;
        PostPhysicsCallback = NULL;

        PreRenderCallback = NULL;
        PostRenderCallback = NULL;
    }

    WorldCallBackManager::~WorldCallBackManager()
    {

    }

    ///////////////////////////////////////////////////////////////////////////////
    //Preinput callback will be called in the main loop first
    ///////////////////////////////////////
    bool WorldCallBackManager::PreInput()
    {
        //If a call back has been set then we use it, otherwise we can
        //assume that we should just keep execute the main loop
        if(NULL!=PreInputCallback)
        {
            return (*PreInputCallback)();
        }
        return true;
    }

    void WorldCallBackManager::ErasePreInput()
    {
        this->PreInputCallback = NULL;
    }

    void WorldCallBackManager::SetPreInput(bool (*Callback)())
    {
        this->PreInputCallback = Callback;
    }

    bool WorldCallBackManager::IsPreInputCallbackSet()
    {
        if(NULL==PreInputCallback)
        {
            return false;
        }
        return true;
    }

    ///////////////////////////////////////////////////////////////////////////////
    //Postinput callback will be called in the main loop right after input
    ///////////////////////////////////////
    bool WorldCallBackManager::PostInput()
    {
        //If a call back has been set then we use it, otherwise we can
        //assume that we should just keep execute the main loop
        if(NULL!=PostInputCallback)
        {
            return (*PostInputCallback)();
        }
        return true;
    }

    void WorldCallBackManager::ErasePostInput()
    {
        this->PostInputCallback = NULL;
    }

    void WorldCallBackManager::SetPostInput(bool (*Callback)())
    {
        this->PostInputCallback = Callback;
    }

    bool WorldCallBackManager::IsPostInputCallbackSet()
    {
        if(NULL==PostInputCallback)
        {
            return false;
        }
        return true;
    }

    ///////////////////////////////////////////////////////////////////////////////
    // These functions manage the pre physics functions
    ///////////////////////////////////////
    bool WorldCallBackManager::PrePhysics()
    {
        if(NULL!=PrePhysicsCallback)
        {
            return (*PrePhysicsCallback)();
        }
        return true;
    }

    void WorldCallBackManager::ErasePrePhysics()
    {
        this->PrePhysicsCallback = NULL;
    }

    void WorldCallBackManager::SetPrePhysics(bool (*Callback)())
    {
        this->PrePhysicsCallback = Callback;
    }

    bool WorldCallBackManager::IsPrePhysicsCallbackSet()
    {
        if(NULL==PrePhysicsCallback)
        {
            return false;
        }
        return true;
    }

    ///////////////////////////////////////////////////////////////////////////////
    // These functions manage the Post physics functions
    ///////////////////////////////////////
    bool WorldCallBackManager::PostPhysics()
    {
        if(NULL!=PostPhysicsCallback)
        {
            return (*PostPhysicsCallback)();
        }
        return true;
    }

    void WorldCallBackManager::ErasePostPhysics()
    {
        this->PostPhysicsCallback = NULL;
    }

    void WorldCallBackManager::SetPostPhysics(bool (*Callback)())
    {
        this->PostPhysicsCallback = Callback;
    }

    bool WorldCallBackManager::IsPostPhysicsCallbackSet()
    {
        if(NULL==PostPhysicsCallback)
        {
            return false;
        }
        return true;
    }

    ///////////////////////////////////////////////////////////////////////////////
    // These functions manage the pre render functions
    ///////////////////////////////////////
    bool WorldCallBackManager::PreRender()
    {
        if(NULL!=PreRenderCallback)
        {
            return (*PreRenderCallback)();
        }
        return true;
    }

    void WorldCallBackManager::ErasePreRender()
    {
        this->PreRenderCallback = NULL;
    }

    void WorldCallBackManager::SetPreRender(bool (*Callback)())
    {
        this->PreRenderCallback = Callback;
    }

    bool WorldCallBackManager::IsPreRenderCallbackSet()
    {
        if(NULL==PreRenderCallback)
        {
            return false;
        }
        return true;
    }


    ///////////////////////////////////////////////////////////////////////////////
    // These functions manage the posy render functions
    /////////////////////////////////////// PostRenderCallback
    bool WorldCallBackManager::PostRender()
    {
        if(NULL!=PostRenderCallback)
        {
            return (*PostRenderCallback)();
        }
        return true;
    }

    void WorldCallBackManager::ErasePostRender()
    {
        this->PostRenderCallback = NULL;
    }

    void WorldCallBackManager::SetPostRender(bool (*Callback)())
    {
        this->PostRenderCallback = Callback;
    }

    bool WorldCallBackManager::IsPostRenderCallbackSet()
    {
        if(NULL==PostRenderCallback)
        {
            return false;
        }
        return true;
    }
}

#endif
