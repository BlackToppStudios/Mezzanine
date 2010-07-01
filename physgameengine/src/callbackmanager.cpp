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
#ifndef CALLBACKMANAGER_CPP
#define CALLBACKMANAGER_CPP


//Includes
#include "callbackmanager.h"
#include "datatypes.h"
#include "world.h"

using namespace std;

namespace phys
{
    ///////////////////////////////////////////////////////////////////////////////
    // Important Functions
    ///////////////////////////////////////
    CallBackManager::CallBackManager(World* _Parent) //: ManagerBase(_Parent)
    {
        PreInputCallback = NULL;
        PostInputCallback = NULL;

        PrePhysicsCallback = NULL;
        PostPhysicsCallback = NULL;

        PreRenderCallback = NULL;
        PostRenderCallback = NULL;
    }

    CallBackManager::~CallBackManager()
        {}

    void CallBackManager::Initialize()
        {}

    //ManagerTypeName CallBackManager::GetType() const
    //    { return ManagerBase::CallBackManager; }

    ///////////////////////////////////////////////////////////////////////////////
    //Preinput callback will be called in the main loop first
    ///////////////////////////////////////
    bool CallBackManager::PreInput()
    {
        //If a call back has been set then we use it, otherwise we can
        //assume that we should just keep execute the main loop
        if(NULL!=PreInputCallback)
        {
            return (*PreInputCallback)();
        }
        return true;
    }

    void CallBackManager::ErasePreInput()
    {
        this->PreInputCallback = NULL;
    }

    void CallBackManager::SetPreInput(bool (*Callback)())
    {
        this->PreInputCallback = Callback;
    }

    bool CallBackManager::IsPreInputCallbackSet() const
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
    bool CallBackManager::PostInput()
    {
        //If a call back has been set then we use it, otherwise we can
        //assume that we should just keep execute the main loop
        if(NULL!=PostInputCallback)
        {
            return (*PostInputCallback)();
        }
        return true;
    }

    void CallBackManager::ErasePostInput()
    {
        this->PostInputCallback = NULL;
    }

    void CallBackManager::SetPostInput(bool (*Callback)())
    {
        this->PostInputCallback = Callback;
    }

    bool CallBackManager::IsPostInputCallbackSet() const
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
    bool CallBackManager::PrePhysics()
    {
        if(NULL!=PrePhysicsCallback)
        {
            return (*PrePhysicsCallback)();
        }
        return true;
    }

    void CallBackManager::ErasePrePhysics()
    {
        this->PrePhysicsCallback = NULL;
    }

    void CallBackManager::SetPrePhysics(bool (*Callback)())
    {
        this->PrePhysicsCallback = Callback;
    }

    bool CallBackManager::IsPrePhysicsCallbackSet() const
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
    bool CallBackManager::PostPhysics()
    {
        if(NULL!=PostPhysicsCallback)
        {
            return (*PostPhysicsCallback)();
        }
        return true;
    }

    void CallBackManager::ErasePostPhysics()
    {
        this->PostPhysicsCallback = NULL;
    }

    void CallBackManager::SetPostPhysics(bool (*Callback)())
    {
        this->PostPhysicsCallback = Callback;
    }

    bool CallBackManager::IsPostPhysicsCallbackSet() const
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
    bool CallBackManager::PreRender()
    {
        if(NULL!=PreRenderCallback)
        {
            return (*PreRenderCallback)();
        }
        return true;
    }

    void CallBackManager::ErasePreRender()
    {
        this->PreRenderCallback = NULL;
    }

    void CallBackManager::SetPreRender(bool (*Callback)())
    {
        this->PreRenderCallback = Callback;
    }

    bool CallBackManager::IsPreRenderCallbackSet() const
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
    bool CallBackManager::PostRender()
    {
        if(NULL!=PostRenderCallback)
        {
            return (*PostRenderCallback)();
        }
        return true;
    }

    void CallBackManager::ErasePostRender()
    {
        this->PostRenderCallback = NULL;
    }

    void CallBackManager::SetPostRender(bool (*Callback)())
    {
        this->PostRenderCallback = Callback;
    }

    bool CallBackManager::IsPostRenderCallbackSet() const
    {
        if(NULL==PostRenderCallback)
        {
            return false;
        }
        return true;
    }
}

#endif
