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
#ifndef _graphicsmanager_cpp
#define _graphicsmanager_cpp

#include "world.h"
#include "eventmanager.h"
#include "eventrendertime.h"
#include "actorcontainervector.h"
#include "graphicsmanager.h"
#include "cameramanager.h"
#include "resourcemanager.h"
#include "uimanager.h"
#include "camera.h"
#include "crossplatform.h"
#include "gamewindow.h"
#include "viewport.h"

#include <SDL.h>
#include <Ogre.h>

namespace phys
{

    ///////////////////////////////////////////////////////////////////////////
    // Creation and Deletion functions
    ///////////////////////////////////
    GraphicsManager::GraphicsManager()
        : SDLBeenInitialized(false),
          OgreBeenInitialized(false)
    {
        Construct( 800, 600, false );
    }

    GraphicsManager::GraphicsManager( const Whole &Width, const Whole &Height, const bool &FullScreen )
        : SDLBeenInitialized(false),
          OgreBeenInitialized(false)
    {
        Construct( Width, Height, FullScreen );
    }

    GraphicsManager::~GraphicsManager()
    {
        DestroyAllGameWindows(false);
        ShutdownSDL();
    }

    void GraphicsManager::Construct(const Whole &Width, const Whole &Height, const bool &FullScreen )
    {
        PrimarySettings.Fullscreen = FullScreen;
        PrimarySettings.RenderHeight = Height;
        PrimarySettings.RenderWidth = Width;
        this->Priority = 0;
        this->FrameDelay = 0;
    }

    void GraphicsManager::InitSDL()
    {
        /// @todo TODO set multithreaded SDL so it will the run event manager in another thread
        if(!SDLBeenInitialized)
        {
            SDL_Init(SDL_INIT_VIDEO);
            SDLBeenInitialized = true;
        }
        #ifdef PHYSDEBUG
        GameWorld->Log("Initialized SDL");
        #endif
    }

    void GraphicsManager::InitOgre()
    {
        if(!OgreBeenInitialized)
        {
            try
            {
                /*NewWindows.OgreWindow = */Ogre::Root::getSingleton().initialise(false,"AppName");
                #ifdef PHYSDEBUG
                GameWorld->Log("Setup Ogre Window");
                #endif
            }catch (exception& e) {
                GameWorld->Log("Failed to Setup Ogre Window");
                GameWorld->LogAndThrow(e.what());
            }
            OgreBeenInitialized = true;
        }
    }

    void GraphicsManager::ShutdownSDL()
    {
        //SDL_DeleteContext();
        //SDL_DestroyWindow(SDLwindow);
        //SDL_FreeSurface(SDLscreen);
        SDL_Quit();
    }

    void GraphicsManager::InitViewportAndCamera(GameWindow* NewWindow)
    {
        //setup a default camera unless has been setup yet
        Camera* camera = NULL;
        if(GameWorld->GetCameraManager()==0)
        {
            GameWorld->AddManager(new phys::CameraManager(0));
        }
        camera = GameWorld->GetCameraManager()->GetDefaultCamera();
        #ifdef PHYSDEBUG
        GameWorld->Log("Created Default Camera");
        #endif

        //viewport connects camera and render window
        Viewport* PrimaryViewport = NewWindow->CreateViewport(camera);

        //setting the aspect ratio must be done after we setup the viewport
        camera->SetAspectRatio( (Real)(PrimaryViewport->GetActualWidth()) / (Real)(PrimaryViewport->GetActualHeight()) );
        #ifdef PHYSDEBUG
        GameWorld->Log("Configured Viewport and Aspect Ratio");
        #endif
    }

    GameWindow* GraphicsManager::CreateGameWindow(const String& WindowCaption, const Whole& Width, const Whole& Height, const Whole& Flags)
    {
        if(!SDLBeenInitialized) InitSDL();
        if(!OgreBeenInitialized) InitOgre();
        //http://wiki.libsdl.org/moin.cgi/SDL_Init?highlight=%28\bCategoryAPI\b%29|%28SDLFunctionTemplate%29 // for more flags
        GameWindow* NewWindow = new GameWindow(WindowCaption,Width,Height,Flags);

        if(GameWindows.empty())
        {
            InitViewportAndCamera(NewWindow);
            PrimaryGameWindow = NewWindow;
            PrimarySettings = NewWindow->GetSettings();
        }
        GameWindows.push_back(NewWindow);
        return NewWindow;
    }

    GameWindow* GraphicsManager::GetGameWindow(const Whole& Index)
    {
        return GameWindows[Index];
    }

    Whole GraphicsManager::GetNumGameWindows()
    {
        return GameWindows.size();
    }

    void GraphicsManager::DestroyGameWindow(GameWindow* ToBeDestroyed)
    {
        for ( std::vector<GameWindow*>::iterator it = GameWindows.begin() ; it != GameWindows.end() ; it++ )
        {
            if ( ToBeDestroyed == (*it) )
            {
                delete ToBeDestroyed;
                GameWindows.erase(it);
                return;
            }
        }
    }

    void GraphicsManager::DestroyAllGameWindows(bool ExcludePrimary)
    {
        Whole X = 0;
        if(ExcludePrimary) X++;
        while( X < GameWindows.size() )
        {
            delete GameWindows[X];
            X++;
        }
        GameWindows.clear();
        if(ExcludePrimary) GameWindows.push_back(PrimaryGameWindow);
    }

    GameWindow* GraphicsManager::GetPrimaryGameWindow()
    {
        return PrimaryGameWindow;
    }

    const GraphicsSettings& GraphicsManager::GetDesktopSettings()
    {
        return DesktopSettings;
    }

    bool GraphicsManager::HasSDLBeenInitialized()
    {
        return SDLBeenInitialized;
    }

    bool GraphicsManager::HasOgreBeenInitialized()
    {
        return OgreBeenInitialized;
    }

    ///////////////////////////////////////////////////////////////////////////////
    //Shows the ogre settings Dialog, and allows it to save settings to ogres
    //preset save location
    bool GraphicsManager::ShowGraphicsSettingDialog()
    {
        try
        {
            return Ogre::Root::getSingleton().showConfigDialog();
        } catch (exception& e) {
            this->GameWorld->Log("Ogre settings windows from main UI or mandatory setting failure");
            this->GameWorld->Log(e.what());
            return false;
        }
    }

    String GraphicsManager::GetRenderSystemName()
    {
        String Renderer = Ogre::Root::getSingleton().getRenderSystem()->getName();
        Whole EditPos = Renderer.find("Rendering");
        Renderer.erase(EditPos - 1);
        return Renderer;
    }

    const std::vector<String>* GraphicsManager::GetSupportedResolutions()
    {
        return &SupportedResolutions;
    }

    const std::vector<String>* GraphicsManager::GetSupportedDevices()
    {
        return &SupportedDevices;
    }

    void GraphicsManager::ResetRenderTimer()
    {
        this->RenderTimer->reset();
    }

    //Inherited From ManagerBase
    void GraphicsManager::Initialize()
    {
        if(GameWindows.empty())
            CreateGameWindow("",PrimarySettings.RenderWidth,PrimarySettings.RenderHeight,PrimarySettings.Fullscreen?GameWindow::WF_Fullscreen:0);
        this->RenderTimer = new Ogre::Timer();

        Ogre::ConfigOptionMap& CurrentRendererOptions = Ogre::Root::getSingleton().getRenderSystem()->getConfigOptions();
        for( Ogre::ConfigOptionMap::iterator configItr = CurrentRendererOptions.begin() ;
            configItr != CurrentRendererOptions.end() ; configItr++)
        {
            if( (configItr)->first == "Video Mode" )
            {
                for( Whole X = 0 ; X < (configItr)->second.possibleValues.size() ; X++ )
                    SupportedResolutions.push_back((configItr)->second.possibleValues[X]);
                continue;
            }
            if( (configItr)->first == "Rendering Device" )
            {
                for( Whole Y = 0 ; Y < (configItr)->second.possibleValues.size() ; Y++ )
                    SupportedDevices.push_back((configItr)->second.possibleValues[Y]);
                continue;
            }
        }

        SDL_DisplayMode DeskMode;
        SDL_GetDesktopDisplayMode(0,&DeskMode);
        DesktopSettings.RenderWidth = DeskMode.w;
        DesktopSettings.RenderHeight = DeskMode.h;
        DesktopSettings.RefreshRate = DeskMode.refresh_rate;

        GraphicsInitialized = true;
    }

    void GraphicsManager::DoMainLoopItems()
    {
        //Ogre::WindowEventUtilities::messagePump();
        crossplatform::RenderPhysWorld();

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
