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
#include "eventrendertime.h"
#include "actorcontainervector.h"
#include "graphicsmanager.h"
#include "cameramanager.h"
#include "resourcemanager.h"
#include "uimanager.h"
#include "camera.h"

#include <SDL.h>
#include <Ogre.h>

namespace phys
{

    ///////////////////////////////////////////////////////////////////////////
    // Creation and Deletion functions
    ///////////////////////////////////
    GraphicsManager::GraphicsManager()
    {
        Construct( 800, 600, false );
    }

    GraphicsManager::GraphicsManager( const Whole &Width_, const Whole &Height_, const bool &FullScreen_ )
    {
        Construct( Width_, Height_, FullScreen_ );
    }

    GraphicsManager::~GraphicsManager()
    {
        DestroyRenderWindow();
        ShutdownSDL();
    }

    void GraphicsManager::Construct(const Whole &Width_, const Whole &Height_, const bool &FullScreen_ )
    {
        Settings.Fullscreen = FullScreen_;
        Settings.RenderHeight = Height_;
        Settings.RenderWidth = Width_;
        this->Priority = 0;
        this->FrameDelay = 0;
    }

    void GraphicsManager::CreateRenderWindow()
    {
        //InitSDL();
        SDL_Init(SDL_INIT_VIDEO);
        size_t RC = 0;
        //http://wiki.libsdl.org/moin.cgi/SDL_Init?highlight=%28\bCategoryAPI\b%29|%28SDLFunctionTemplate%29 // for more flags

		try
		{
			//Setup the SDL render window
			SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
			if(Settings.Fullscreen)
			{
			    this->SDLwindow = SDL_CreateWindow( GameWorld->GetWindowName().c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, Settings.RenderWidth, Settings.RenderHeight, SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN | SDL_WINDOW_SHOWN );
			    //this->SDLscreen = SDL_SetVideoMode( RenderWidth, RenderHeight,0, SDL_OPENGL | SDL_FULLSCREEN );
			}else{
                this->SDLwindow = SDL_CreateWindow( GameWorld->GetWindowName().c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, Settings.RenderWidth, Settings.RenderHeight, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN );
                //this->SDLscreen = SDL_SetVideoMode( RenderWidth, RenderHeight,0, SDL_OPENGL );
			}
			RC = (size_t)SDL_GL_CreateContext(this->SDLwindow);
			//SDL_WM_SetCaption(GameWorld->GetWindowName().c_str(), NULL);
			#ifdef PHYSDEBUG
            GameWorld->Log("Successfully Setup SDL");
            #endif
		}catch (exception& e) {
		    GameWorld->Log("Failed to Setup SDL");
			GameWorld->LogAndThrow(e.what());
		}

        //Start Ogre Without a native render window
        try
        {
            //crossplatform::WaitMilliseconds(1000);
            OgreGameWindow = Ogre::Root::getSingleton().initialise(false, GameWorld->GetWindowName());
            #ifdef PHYSDEBUG
            GameWorld->Log("Setup Ogre Window");
            #endif
        }catch (exception& e) {
		    GameWorld->Log("Failed to Setup Ogre Window");
			GameWorld->LogAndThrow(e.what());
		}

        //Configure Ogre to render to the SDL window
        Ogre::NameValuePairList *misc;
        misc=(Ogre::NameValuePairList*) crossplatform::GetSDLOgreBinder(SDLwindow,RC);
        OgreGameWindow = Ogre::Root::getSingleton().createRenderWindow(GameWorld->GetWindowName(), Settings.RenderWidth, Settings.RenderHeight, Settings.Fullscreen, misc);
        #ifdef PHYSDEBUG
        GameWorld->Log("Bound Ogre to an SDL window");
        #endif

        InitViewportAndCamera();
    }

    void GraphicsManager::DestroyRenderWindow()
    {
        OgreGameWindow->destroy();
    }

    void GraphicsManager::InitSDL()
    {
        /// @todo TODO set multithreaded SDL so it will the run event manager in another thread
        //Get what is needed for SDL started
        if (SDL_Init(SDL_INIT_VIDEO) < 0) {
            GameWorld->Log("Error 2: Unable to init SDL, SDL Error Follows:");
            GameWorld->LogAndThrow(SDL_GetError());
        }
        SDLBeenInitialized = true;
        #ifdef PHYSDEBUG
        GameWorld->Log("Initialized SDL");
        #endif
    }

    void GraphicsManager::ShutdownSDL()
    {
        //SDL_DeleteContext();
        SDL_DestroyWindow(SDLwindow);
        //SDL_FreeSurface(SDLscreen);
        SDL_Quit();
    }

    void GraphicsManager::InitViewportAndCamera()
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
        Ogre::Viewport* OgreViewport = NULL;
        OgreViewport = OgreGameWindow->addViewport(camera->GetOgreCamera());
        GameWorld->GetCameraManager()->Viewports["Viewport1"] = OgreViewport;

        //setting the aspect ratio must be done after we setup the viewport
        camera->SetAspectRatio( (Real)(OgreViewport->getActualWidth()) / (Real)(OgreViewport->getActualHeight()) );
        #ifdef PHYSDEBUG
        GameWorld->Log("Configured Viewport and Aspect Ratio");
        #endif
    }

    void GraphicsManager::UpdateWindowStats()
    {
        GameWorld->Log("Updating Screen Mode. ");
        //OgreGameWindow->destroy();
        if(Settings.Fullscreen)
        {
            GameWorld->Log("Setting SDL. ");
            this->SDLscreen = SDL_SetVideoMode( Settings.RenderWidth, Settings.RenderHeight,0, SDL_OPENGL | SDL_FULLSCREEN);
            GameWorld->Log("Setting Ogre. ");
            //OgreGameWindow->setFullscreen(true,RenderWidth,RenderHeight);
        }else{
            GameWorld->Log("Setting SDL. ");
            this->SDLscreen = SDL_SetVideoMode( Settings.RenderWidth, Settings.RenderHeight,0, SDL_OPENGL );
            GameWorld->Log("Setting Ogre. ");
            //OgreGameWindow->setFullscreen(false,RenderWidth,RenderHeight);
        }
        Ogre::NameValuePairList *misc;
        //misc=(Ogre::NameValuePairList*) crossplatform::GetSDLOgreBinder();
        //OgreGameWindow->create(GameWorld->GetWindowName(), RenderHeight, RenderWidth, Fullscreen, misc);

        GameWorld->Log("Updating Viewport. ");
        Ogre::Viewport* OgreViewport = GameWorld->GetCameraManager()->GetOgreViewport("Viewport1");
        OgreViewport->setDimensions(0,0,1,1);
        OgreViewport->getCamera()->setAspectRatio((Real)(OgreViewport->getActualWidth()) / (Real)(OgreViewport->getActualHeight()));

        ActorContainerVector* ActorMan = static_cast<ActorContainerVector*>(GameWorld->GetActorManager());
        //for( std::vector<ActorBase*>::iterator cur = ActorMan->begin() ; cur != ActorMan->end() ; cur++ )
        //    (*cur)->InitEntity(true);

        Ogre::CompositorManager::getSingleton().reloadAll();
        //Ogre::GpuProgramManager::getSingleton().reloadAll();
        //Ogre::HighLevelGpuProgramManager::getSingleton().reloadAll();
        //Ogre::FontManager::getSingleton().reloadAll();
        //Ogre::MaterialManager::getSingleton().reloadAll();
        Ogre::TextureManager::getSingleton().reloadAll();

        //GameWorld->GetResourceManager()->ParseMaterialScripts();

        Ogre::MeshManager::getSingleton().reloadAll();
        //Ogre::SkeletonManager::getSingleton().reloadAll();

        for( std::vector<ActorBase*>::iterator cur = ActorMan->begin() ; cur != ActorMan->end() ; cur++ )
            (*cur)->InitEntity(true);
        //GameWorld->GetUIManager()->RedrawAll(true);
        GameWorld->Log("Done Updating Screen Mode. ");
    }

    ///////////////////////////////////////////////////////////////////////////
    // Fullscreen functions
    ///////////////////////////////////
    bool GraphicsManager::getFullscreen() const
    {
        return Settings.Fullscreen;
    }

    //returns: false if changes could not be made
    void GraphicsManager::setFullscreen(const bool &Fullscreen_)
    {
        if(!GraphicsInitialized)
        {
            Settings.Fullscreen = Fullscreen_;
            return;
        }
        /// @todo TODO: Need to attempt to switch to fullscreen here
        /// @todo TODO: We really should double check that going into fullscreen worked the way we wanted, this fails in too many games

        if(SDL_SetWindowFullscreen(SDLwindow, Fullscreen_?SDL_TRUE:SDL_FALSE ) > 0)
            Settings.Fullscreen = Fullscreen_;
    }

    ///////////////////////////////////////////////////////////////////////////
    // Resolution functions
    ///////////////////////////////////
    Whole GraphicsManager::getRenderHeight() const
    {
        return Settings.RenderHeight;
    }

    Whole GraphicsManager::getRenderWidth() const
    {
        return Settings.RenderWidth;
    }

    void GraphicsManager::setRenderHeight(const Whole &Height_)
    {
        if(!GraphicsInitialized)
        {
            Settings.RenderHeight = Height_;
            return;
        }
        /// @todo TODO: Need to attempt to update resolution here
        setRenderResolution(Settings.RenderWidth,Height_);
    }

    void GraphicsManager::setRenderWidth(const Whole &Width_)
    {
        if(!GraphicsInitialized)
        {
            Settings.RenderWidth = Width_;
            return;
        }
        /// @todo TODO: Need to attempt to update resolution here
        setRenderResolution(Width_,Settings.RenderHeight);
    }

    void GraphicsManager::setRenderResolution(const Whole &Width_, const Whole &Height_)
    {
        if(!GraphicsInitialized)
        {
            Settings.RenderWidth = Width_;
            Settings.RenderHeight = Height_;
            return;
        }
        /// @todo TODO: Need to attempt to update resolution here
        if(Settings.Fullscreen)
        {
            SDL_DisplayMode CurrentDisplay;
            SDL_GetWindowDisplayMode(SDLwindow,&CurrentDisplay);
            CurrentDisplay.w = Width_;
            CurrentDisplay.h = Height_;
            if(SDL_SetWindowDisplayMode(SDLwindow,&CurrentDisplay) > 0)
            {
                Settings.RenderWidth = Width_;
                Settings.RenderHeight = Height_;
                return;
            }
        }else{
            SDL_SetWindowSize(SDLwindow,Settings.RenderWidth,Settings.RenderHeight);
        }
    }

    void GraphicsManager::setRenderOptions(const GraphicsSettings& NewSettings)
    {
        if(!GraphicsInitialized)
        {
            Settings = NewSettings;
            return;
        }
        setFullscreen(NewSettings.Fullscreen);
        setRenderResolution(NewSettings.RenderWidth,NewSettings.RenderHeight);
    }

    bool GraphicsManager::HasSDLBeenInitialized()
    {
        return SDLBeenInitialized;
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

    ///////////////////////////////////////////////////////////////////////////////
    //Stats functions
    Real GraphicsManager::GetLastFPS()
    {
        return OgreGameWindow->getLastFPS();
    }

    Real GraphicsManager::GetAverageFPS()
    {
        return OgreGameWindow->getAverageFPS();
    }

    Real GraphicsManager::GetBestFPS()
    {
        return OgreGameWindow->getBestFPS();
    }

    Real GraphicsManager::GetWorstFPS()
    {
        return OgreGameWindow->getWorstFPS();
    }

    Real GraphicsManager::GetBestFrameTime()
    {
        return OgreGameWindow->getBestFrameTime();
    }

    Real GraphicsManager::GetWorstFrameTime()
    {
        return OgreGameWindow->getWorstFrameTime();
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
        CreateRenderWindow();
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
        crossplatform::RenderPhysWorld(this->OgreGameWindow, this->SDLwindow);

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

    void GraphicsManager::SetOgreWindowPointer(Ogre::RenderWindow* OgreWindow)
    {
        OgreGameWindow = OgreWindow;
    }

    Ogre::RenderWindow* GraphicsManager::GetOgreWindowPointer()
    {
        return OgreGameWindow;
    }
}

#endif
