//© Copyright 2010 - 2012 BlackTopp Studios Inc.
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
#ifndef _gamewindow_cpp
#define _gamewindow_cpp

#include "gamewindow.h"
#include "crossplatform.h"
#include "viewport.h"
#include "camera.h"
#include "graphicsmanager.h"
#include "world.h"

#include <SDL.h>
#include "../src/video/SDL_sysvideo.h"
#include <Ogre.h>

#ifdef WINDOWS
#include <windows.h>
#endif

#ifdef LINUX
#include <X11/X.h>      //x11proto-core-dev
#include <X11/Xlib.h>   //libx11-dev
#endif

namespace Mezzanine
{
    class GameWindowInternalData
    {
        public:
            bool Borderless;
            bool Resizeable;

            GameWindowInternalData() : Borderless(false), Resizeable(false) {};
    };

    GameWindow::GameWindow(const String& WindowCaption, const Whole& Width, const Whole& Height, const Whole& Flags)
        : OgreWindow(NULL),
          SDLWindow(NULL)
    {
        GWID = new GameWindowInternalData();
        CreateGameWindow(WindowCaption,Width,Height,Flags);
    }

    GameWindow::~GameWindow()
    {
        OgreWindow->destroy();
        SDL_DestroyWindow(SDLWindow);
        delete GWID;
    }

    void GameWindow::CreateGameWindow(const String& WindowCaption, const Whole& Width, const Whole& Height, const Whole& Flags)
    {
        Manager = GraphicsManager::GetSingletonPtr();
        Settings.RenderWidth = Width;
        Settings.RenderHeight = Height;

        Ogre::NameValuePairList Opts;
        if(WF_Fullscreen & Flags)
        {
            Settings.Fullscreen = true;
        }
        if(WF_Hidden & Flags)
        {
            Opts["hidden"] = "true";
        }
        if(WF_VsyncEnabled & Flags)
        {
            Opts["vsync"] = "true";
            Settings.VSync = true;
        }
        if(WF_FSAA_16 & Flags)
        {
            Opts["FSAA"] = "16";
        }
        else if(WF_FSAA_8 & Flags)
        {
            Opts["FSAA"] = "8";
        }
        else if(WF_FSAA_4 & Flags)
        {
            Opts["FSAA"] = "4";
        }
        else if(WF_FSAA_2 & Flags)
        {
            Opts["FSAA"] = "2";
        }
        if(WF_Resizeable & Flags)
        {
            Opts["border"] = "resize";
            GWID->Resizeable = true;
        }
        else if(WF_Borderless & Flags)
        {
            Opts["border"] = "none";
            GWID->Borderless = true;
        }
        else
        {
            Opts["border"] = "fixed";
        }
        if(WF_Maximized & Flags)
        {
        }

        //#ifdef LINUX
        //Ogre::ResourceGroupManager::getSingleton().addResourceLocation(ResourceManager::GetSingletonPtr()->GetEngineDataDirectory(),"FileSystem");
        //#endif
        OgreWindow = Ogre::Root::getSingleton().createRenderWindow(WindowCaption, Settings.RenderWidth, Settings.RenderHeight, Settings.Fullscreen, &Opts);//*/

        if( !(WF_Hidden & Flags) )
        {
            #ifdef WINDOWS
            HWND Data = 0;
            #endif
            #ifdef LINUX
            Window Data = 0;
            #endif
            #ifdef MACOS
            NSWindow* Data = 0;
            #endif
            OgreWindow->getCustomAttribute("WINDOW",&Data);
            SDLWindow = SDL_CreateWindowFrom((void*)Data);
            SDLWindow->data = new SDL_WindowUserData();
            SDLWindow->data->name = NULL;
            SDLWindow->data->data = this;
            SDLWindow->data->next = NULL;
        }
    }

    void GameWindow::UpdateViewportsAndCameras()
    {
        for( std::vector<Viewport*>::iterator ViewIt = Viewports.begin() ; ViewIt != Viewports.end() ; ++ViewIt )
        {
            Camera* Cam = (*ViewIt)->GetViewportCamera();
            if(Cam)
                Cam->SetAspectRatio((Real)((*ViewIt)->GetActualWidth()) / (Real)((*ViewIt)->GetActualHeight()));
        }
    }

    int GameWindow::IsLargerThenDesktop(const Whole& Width, const Whole& Height)
    {
        SDL_DisplayMode DesktopDisplay;
        SDL_GetDesktopDisplayMode(0,&DesktopDisplay);
        if(Width > DesktopDisplay.w || Height > DesktopDisplay.h)
            return 1;
        else if(Width == DesktopDisplay.w || Height == DesktopDisplay.h)
            return 0;
        else
            return -1;
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Viewport Management

    Viewport* GameWindow::CreateViewport(Camera* ViewportCamera, const Whole& ZOrder)
    {
        Whole Order = ZOrder == 0 ? Viewports.size() : ZOrder;
        Viewport* NewViewport = new Viewport(ViewportCamera,Order,this);
        Viewports.push_back(NewViewport);
        return NewViewport;
    }

    Viewport* GameWindow::GetViewport(const Whole& Index) const
    {
        return Viewports.at(Index);
    }

    Whole GameWindow::GetNumViewports() const
    {
        return Viewports.size();
    }

    void GameWindow::DestroyViewport(Viewport* ToBeDestroyed)
    {
        for ( std::vector<Viewport*>::iterator it = Viewports.begin() ; it != Viewports.end() ; it++ )
        {
            if ( ToBeDestroyed == (*it) )
            {
                delete ToBeDestroyed;
                Viewports.erase(it);
                return;
            }
        }
    }

    GameWindow::ViewportIterator GameWindow::BeginViewport()
    {
        return Viewports.begin();
    }

    GameWindow::ViewportIterator GameWindow::EndViewport()
    {
        return Viewports.end();
    }

    GameWindow::ConstViewportIterator GameWindow::BeginViewport() const
    {
        return Viewports.begin();
    }

    GameWindow::ConstViewportIterator GameWindow::EndViewport() const
    {
        return Viewports.end();
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Window Metrics Management

    void GameWindow::SetRenderWidth(const Whole &Width)
    {
        if(Settings.RenderWidth == Width)
            return;
        SetRenderResolution(Width,Settings.RenderHeight);
        Settings.RenderWidth = Width;
    }

    Whole GameWindow::GetRenderWidth() const
    {
        return Settings.RenderWidth;
    }

    void GameWindow::SetRenderHeight(const Whole &Height)
    {
        if(Settings.RenderHeight == Height)
            return;
        SetRenderResolution(Settings.RenderWidth,Height);
        Settings.RenderHeight = Height;
    }

    Whole GameWindow::GetRenderHeight() const
    {
        return Settings.RenderHeight;
    }

    void GameWindow::SetRenderResolution(const Whole &Width, const Whole &Height)
    {
        if(Settings.RenderWidth == Width && Settings.RenderHeight == Height)
            return;
        if(Settings.Fullscreen)
        {
            SDL_DisplayMode CurrentDisplay;
            SDL_GetWindowDisplayMode(SDLWindow,&CurrentDisplay);
            CurrentDisplay.w = Width;
            CurrentDisplay.h = Height;
            //CurrentDisplay.refresh_rate = 60;
            if(SDL_SetWindowDisplayMode(SDLWindow,&CurrentDisplay) == 0)
            {
                OgreWindow->setFullscreen(true,Width,Height);
                UpdateViewportsAndCameras();
                Settings.RenderWidth = Width;
                Settings.RenderHeight = Height;
                return;
            }
        }else{
            int Result = IsLargerThenDesktop(Width,Height);
            if(Result == 0)
            {
                Whole ResultWidth, ResultHeight;
                crossplatform::SanitizeWindowedRes(Width,Height,ResultWidth,ResultHeight);
                SDL_SetWindowSize(SDLWindow,ResultWidth,ResultHeight);
                OgreWindow->setFullscreen(false,ResultWidth,ResultHeight);
            }else if(Result == 1){
                World::GetWorldPointer()->Log("Cannot create a window larger then the desktop resolution.");
                return;
            }else{
                SDL_SetWindowSize(SDLWindow,Width,Height);
                OgreWindow->setFullscreen(false,Width,Height);
            }
            UpdateViewportsAndCameras();
            Settings.RenderWidth = Width;
            Settings.RenderHeight = Height;
        }
    }

    void GameWindow::SetFullscreen(const bool &Fullscreen)
    {
        static SDL_DisplayMode FSDisplayMode;

        if(Fullscreen == Settings.Fullscreen)
            return;

        if(!Fullscreen && Settings.Fullscreen)
        {
            const GraphicsSettings& DeskSet = Manager->GetDesktopSettings();
            if( Settings.RenderWidth > DeskSet.RenderWidth || Settings.RenderHeight > DeskSet.RenderHeight )
            {
                Settings.RenderWidth = DeskSet.RenderWidth;
                Settings.RenderHeight = DeskSet.RenderHeight;
            }
            if( Settings.RenderWidth == DeskSet.RenderWidth || Settings.RenderHeight == DeskSet.RenderHeight )
            {
                Whole ResultWidth, ResultHeight;
                crossplatform::SanitizeWindowedRes(Settings.RenderWidth,Settings.RenderHeight,ResultWidth,ResultHeight);
                SetRenderResolution(ResultWidth,ResultHeight);
                Settings.RenderWidth = DeskSet.RenderWidth;
                Settings.RenderHeight = DeskSet.RenderHeight;
            }
        }
        else if(Fullscreen && !Settings.Fullscreen)
        {
            FSDisplayMode.w = Settings.RenderWidth;
            FSDisplayMode.h = Settings.RenderHeight;
            FSDisplayMode.refresh_rate = Settings.RefreshRate;
            SDL_SetWindowDisplayMode(SDLWindow,&FSDisplayMode);
        }

        if(SDL_SetWindowFullscreen(SDLWindow, Fullscreen?SDL_TRUE:SDL_FALSE ) == 0)
        {
            OgreWindow->setFullscreen(Fullscreen,Settings.RenderWidth,Settings.RenderHeight);
            UpdateViewportsAndCameras();
            Settings.Fullscreen = Fullscreen;
        }
    }

    bool GameWindow::GetFullscreen() const
    {
        return Settings.Fullscreen;
    }

    void GameWindow::SetRenderOptions(const GraphicsSettings& NewSettings)
    {
        SetFullscreen(NewSettings.Fullscreen);
        SetRenderResolution(NewSettings.RenderWidth,NewSettings.RenderHeight);
    }

    const GraphicsSettings& GameWindow::GetSettings()
    {
        return Settings;
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Window Settings Methods

    const String& GameWindow::GetWindowCaption()
    {
        return OgreWindow->getName();
    }

    Whole GameWindow::GetFSAALevel() const
    {
        return OgreWindow->getFSAA();
    }

    void GameWindow::EnableVsync(bool Enable)
    {
        OgreWindow->setVSyncEnabled(Enable);
    }

    bool GameWindow::VsyncEnabled() const
    {
        return OgreWindow->isVSyncEnabled();
    }

    void GameWindow::SetHidden(bool Hidden)
    {
        OgreWindow->setHidden(Hidden);
    }

    bool GameWindow::IsHidden() const
    {
        return OgreWindow->isHidden();
    }

    bool GameWindow::BorderIsResizeable() const
    {
        return GWID->Resizeable;
    }

    bool GameWindow::IsBorderless() const
    {
        return GWID->Borderless;
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Window Stats Methods

    Real GameWindow::GetLastFPS() const
    {
        return OgreWindow->getLastFPS();
    }

    Real GameWindow::GetAverageFPS() const
    {
        return OgreWindow->getAverageFPS();
    }

    Real GameWindow::GetBestFPS() const
    {
        return OgreWindow->getBestFPS();
    }

    Real GameWindow::GetWorstFPS() const
    {
        return OgreWindow->getWorstFPS();
    }

    Real GameWindow::GetBestFrameTime() const
    {
        return OgreWindow->getBestFrameTime();
    }

    Real GameWindow::GetWorstFrameTime() const
    {
        return OgreWindow->getWorstFrameTime();
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Internal Methods

    Ogre::RenderWindow* GameWindow::_GetOgreWindowPointer()
    {
        return OgreWindow;
    }

    SDL_Window* GameWindow::_GetSDLWindowPointer()
    {
        return SDLWindow;
    }
}

#endif
