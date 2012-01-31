//© Copyright 2010 - 2011 BlackTopp Studios Inc.
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
#include "cameramanager.h"
#include "crossplatform.h"
#include "gamewindow.h"
#include "viewport.h"

#include <SDL.h>
#include <Ogre.h>

#include <cstdlib>

namespace Mezzanine
{
    template<> GraphicsManager* Singleton<GraphicsManager>::SingletonPtr = 0;

    ///////////////////////////////////////////////////////////////////////////
    // Creation and Deletion functions
    ///////////////////////////////////
    GraphicsManager::GraphicsManager()
        : OgreBeenInitialized(false),
          CurrRenderSys(Mezzanine::RS_OpenGL2)
    {
        Construct( 800, 600, false );
    }

    GraphicsManager::GraphicsManager( const Whole &Width, const Whole &Height, const bool &FullScreen )
        : OgreBeenInitialized(false),
          CurrRenderSys(Mezzanine::RS_OpenGL2)
    {
        Construct( Width, Height, FullScreen );
    }

    GraphicsManager::~GraphicsManager()
    {
        DestroyAllGameWindows(false);
    }

    void GraphicsManager::Construct(const Whole &Width, const Whole &Height, const bool &FullScreen )
    {
        DefaultSettings.Fullscreen = FullScreen;
        DefaultSettings.RenderHeight = Height;
        DefaultSettings.RenderWidth = Width;

        InitSDL();
        SDL_DisplayMode DeskMode;
        SDL_GetDesktopDisplayMode(0,&DeskMode);
        DesktopSettings.RenderWidth = DeskMode.w;
        DesktopSettings.RenderHeight = DeskMode.h;
        DesktopSettings.RefreshRate = DeskMode.refresh_rate;

        this->Priority = 0;
        this->FrameDelay = 0;
    }

    String GraphicsManager::ConvertRenderSystem(const Mezzanine::RenderSystem& RS)
    {
        switch(RS)
        {
            case Mezzanine::RS_DirectX9: return "Direct3D9 Rendering Subsystem"; break;
            case Mezzanine::RS_DirectX10: return "Direct3D10 Rendering Subsystem"; break;
            case Mezzanine::RS_DirectX11: return "Direct3D11 Rendering Subsystem"; break;
            case Mezzanine::RS_OpenGL2: return "OpenGL Rendering Subsystem"; break;  /// @todo This will likely have to change when other OGL systems are implemented
            //case Mezzanine::RS_OpenGL3: return ""; break;  Not yet implemented
            //case Mezzanine::RS_OpenGL4: return ""; break;  Not yet implemented
            case Mezzanine::RS_OpenGLES1: return "OpenGL ES 1.x Rendering Subsystem"; break;
            //case Mezzanine::RS_OpenGLES2: return "OpenGL ES 2.x Rendering Subsystem"; break;  Not yet implemented
        }
        return "";
    }

    void GraphicsManager::InitSDL()
    {
        #ifdef MEZZDEBUG
        World::GetWorldPointer()->Log( String("Initializing SDL :")+SDL_GetError() );
        #endif

        if(!SDL_WasInit(SDL_INIT_VIDEO))
        {
            if( SDL_Init(SDL_INIT_VIDEO | SDL_INIT_NOPARACHUTE) == -1) //http://wiki.libsdl.org/moin.cgi/SDL_Init?highlight=%28\bCategoryAPI\b%29|%28SDLFunctionTemplate%29 // for more flags
            {
                World::GetWorldPointer()->LogAndThrow( String("Failed to Initialize SDL for User input, SDL Error:")+SDL_GetError() );
            }
            //atexit(SDL_Quit);
        }

        #ifdef MEZZDEBUG
        World::GetWorldPointer()->Log( String("Initialized SDL :")+SDL_GetError() );
        #endif
    }

    void GraphicsManager::InitOgre()
    {
        if(!OgreBeenInitialized)
        {
            //if (!Ogre::Root::getSingleton().restoreConfig())
            //{
            //    GameWorld->LogAndThrow(Exception("Failed to load Ogre settings during OgreInit in GraphicsManager::InitOgre()."));
            //}
            Ogre::Root* OgreCore = Ogre::Root::getSingletonPtr();
            OgreCore->setRenderSystem(OgreCore->getRenderSystemByName(ConvertRenderSystem(CurrRenderSys)));
            OgreCore->initialise(false,"AppName");
            #ifdef MEZZDEBUG
            GameWorld->Log("Setup Ogre Window");
            #endif
            OgreBeenInitialized = true;
        }
    }

    void GraphicsManager::ShutdownSDL()
    {
        //SDL_DeleteContext();
        //SDL_DestroyWindow(SDLwindow);
        //SDL_FreeSurface(SDLscreen);
        SDL_Quit();               // replaced with atexist call
    }

    void GraphicsManager::InitViewportAndCamera(GameWindow* NewWindow)
    {
        //setup a default camera unless has been setup yet
        Camera* camera = NULL;
        if(GameWorld->GetCameraManager()==0)
        {
            GameWorld->AddManager(new Mezzanine::CameraManager(0));
        }
        camera = CameraManager::GetSingletonPtr()->GetDefaultCamera();
        #ifdef MEZZDEBUG
        GameWorld->Log("Created Default Camera");
        #endif

        //viewport connects camera and render window
        Viewport* PrimaryViewport = NewWindow->CreateViewport(camera);

        //setting the aspect ratio must be done after we setup the viewport
        camera->SetAspectRatio( (Real)(PrimaryViewport->GetActualWidth()) / (Real)(PrimaryViewport->GetActualHeight()) );
        #ifdef MEZZDEBUG
        GameWorld->Log("Configured Viewport and Aspect Ratio");
        #endif
    }

    GameWindow* GraphicsManager::CreateGameWindow(const String& WindowCaption, const Whole& Width, const Whole& Height, const Whole& Flags)
    {
        if(!OgreBeenInitialized) InitOgre();

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

    const GraphicsSettings& GraphicsManager::GetDefaultSettings()
    {
        return DefaultSettings;
    }

    bool GraphicsManager::HasSDLBeenInitialized()
    {
        return SDL_WasInit(SDL_INIT_VIDEO);
    }

    bool GraphicsManager::HasOgreBeenInitialized()
    {
        return OgreBeenInitialized;
    }

    void GraphicsManager::SetRenderSystem(const Mezzanine::RenderSystem& RenderSys)
    {
        if(!OgreBeenInitialized) CurrRenderSys = RenderSys;
        else GameWorld->LogAndThrow(Exception("Attempting to set RenderSystem after graphics has been initialized.  This is not supported."));
    }

    Mezzanine::RenderSystem GraphicsManager::GetCurrRenderSystem()
    {
        return CurrRenderSys;
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

    void GraphicsManager::RenderOneFrame()
    {
        World* TheWorld = World::GetWorldPointer();
        #ifdef MEZZDEBUG
        TheWorld->Log("Rendering the World.");
        #endif
        Ogre::Root::getSingleton().renderOneFrame();
        if( !GetPrimaryGameWindow()->GetOgreWindowPointer()->isVisible() )
            Ogre::Root::getSingleton().clearEventTimes();
        #ifdef MEZZDEBUG
        TheWorld->Log("Finished Rendering");
        #endif
    }

    void GraphicsManager::SwapAllBuffers(bool WaitForVsync)
    {
        for( Whole X = 0 ; X < GetNumGameWindows() ; X++ )
            GetGameWindow(X)->GetOgreWindowPointer()->swapBuffers(false);
    }

    //Inherited From ManagerBase
    void GraphicsManager::Initialize()
    {
        if(GameWindows.empty())
            CreateGameWindow("",DefaultSettings.RenderWidth,DefaultSettings.RenderHeight,DefaultSettings.Fullscreen?GameWindow::WF_Fullscreen:0);
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

        GraphicsInitialized = true;
    }

    void GraphicsManager::DoMainLoopItems()
    {
        Ogre::WindowEventUtilities::messagePump();
        //crossplatform::RenderMezzWorld();
        RenderOneFrame();

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
        EventManager::GetSingletonPtr()->AddEvent(new EventRenderTime(this->GameWorld->GetFrameTime()));
        return ManagerBase::PostMainLoopItems();
    }
}

#endif
