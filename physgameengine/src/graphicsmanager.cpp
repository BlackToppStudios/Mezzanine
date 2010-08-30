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
#ifndef _graphicsmanager_cpp
#define _graphicsmanager_cpp

#include "world.h"
#include "eventmanager.h"
#include "graphicsmanager.h"

#include "Ogre.h"

namespace phys
{

    ///////////////////////////////////////////////////////////////////////////
    // Creation and Deletion functions
    ///////////////////////////////////
    GraphicsManager::GraphicsManager( World* GameWorld_ )
    {
        Construct( GameWorld_, 640, 480, false );
    }

    GraphicsManager::GraphicsManager( World* GameWorld_, const Whole &Width_, const Whole &Height_, const bool &FullScreen_ )
    {
        Construct( GameWorld_, Width_, Height_, FullScreen_ );
    }

    void GraphicsManager::Construct( World* GameWorld_, const Whole &Width_, const Whole &Height_, const bool &FullScreen_ )
    {
        this->FrameDelay = 0;
        this->GameWorld = GameWorld_;
        this->Fullscreen = FullScreen_;
        this->RenderHeight = Height_;
        this->RenderWidth = Width_;
        this->Priority = 0;
    }

    ///////////////////////////////////////////////////////////////////////////
    // Fullscreen functions
    ///////////////////////////////////
    bool GraphicsManager::getFullscreen() const
    {
        return this->Fullscreen;
    }

    //returns: false if changes could not be made
    void GraphicsManager::setFullscreen(const bool &Fullscreen_)
    {
        /// @todo TODO: Need to attempt to switch to fullscreen here
        /// @todo TODO: We really should double check that going into fullscreen worked the way we wanted, this fails in too many games
        this->Fullscreen = Fullscreen_;
    }

    ///////////////////////////////////////////////////////////////////////////
    // Resolution functions
    ///////////////////////////////////
    Whole GraphicsManager::getRenderHeight() const
    {
        return this->RenderHeight;
    }

    Whole GraphicsManager::getRenderWidth() const
    {
        return this->RenderWidth;
    }

    void GraphicsManager::setRenderHeight(const Whole &Height_)
    {
        /// @todo TODO: Need to attempt to update resolution here
        this->RenderHeight = Height_;
    }

    void GraphicsManager::setRenderWidth(const Whole &Width_)
    {
        /// @todo TODO: Need to attempt to update resolution here
        this->RenderWidth = Width_;
    }

    void GraphicsManager::setRenderResolution(const Whole &Width_, const Whole &Height_)
    {
        /// @todo TODO: Need to attempt to update resolution here
        this->RenderWidth = Width_;
        this->RenderHeight = Height_;
    }

    ///////////////////////////////////////////////////////////////////////////////
    //Shows the ogre settings Dialog, and allows it to save settings to ogres
    //preset save location
    bool GraphicsManager::ShowGraphicsSettingDialog()
    {
        try
        {
            return this->GameWorld->OgreRoot->showConfigDialog();
        } catch (exception& e) {
            this->GameWorld->Log("Ogre settings windows from main UI or mandatory setting failure");
            this->GameWorld->Log(e.what());
            return false;
        }
    }

    //Inherited From ManagerBase
    void GraphicsManager::Initialize()
    {
        this->RenderTimer = new Ogre::Timer();
    }

    void GraphicsManager::DoMainLoopItems()
    {
        //Create a the RenderTimer, which will be used to measure the time

        crossplatform::RenderPhysWorld(this->GameWorld);

        //Do Time Calculations to Determine Rendering Time
        this->GameWorld->SetFrameTime( this->RenderTimer->getMilliseconds() );
        this->RenderTimer->reset();
        if(this->GameWorld->GetTargetFrameTime() > this->GameWorld->GetFrameTime()){
            FrameDelay++;
        }else if(this->GameWorld->GetTargetFrameTime() == this->GameWorld->GetFrameTime()){
        }else{
            if (0<FrameDelay){
                FrameDelay--;
            }else{
                FrameDelay=0;
            }
        }
        crossplatform::WaitMilliseconds( FrameDelay );
    }

    ManagerBase::ManagerTypeName GraphicsManager::GetType() const
        { return ManagerBase::GraphicsManager; }

    bool GraphicsManager::PostMainLoopItems()
    {
        this->GameWorld->GetEventManager()->AddEvent(new EventRenderTime(this->GameWorld->GetFrameTime()));
        return ManagerBase::PostMainLoopItems();
    }


}

#endif
