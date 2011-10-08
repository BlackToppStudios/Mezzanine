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
#ifndef _gamewindow_cpp
#define _gamewindow_cpp

#include "gamewindow.h"
#include "crossplatform.h"
#include "viewport.h"
#include "camera.h"
#include "graphicsmanager.h"
#include "world.h"

#include <SDL.h>
#include <Ogre.h>

#ifdef WINDOWS
#include <windows.h>
#endif

#ifdef LINUX
#include <X11/X.h>      //x11proto-core-dev
#include <X11/Xlib.h>   //libx11-dev
#endif

namespace phys
{
    GameWindow::GameWindow(const String& WindowCaption, const Whole& Width, const Whole& Height, const Whole& Flags)
        : OgreWindow(NULL),
          SDLWindow(NULL)
    {
        CreateGameWindow(WindowCaption,Width,Height,Flags);
    }

    namespace
    {
        // Stores a handle for the SDL rendering context
        void* RC = 0;
    }

    GameWindow::~GameWindow()
    {
        OgreWindow->destroy();
        SDL_DestroyWindow(SDLWindow);
        if(this == Manager->GetPrimaryGameWindow())
            RC = 0;
    }

    void GameWindow::CreateGameWindow(const String& WindowCaption, const Whole& Width, const Whole& Height, const Whole& Flags)
    {
        Manager = World::GetWorldPointer()->GetGraphicsManager();
        Settings.RenderWidth = Width;
        Settings.RenderHeight = Height;
        //RenderContext = 0;
        int SDLFlags = 0;
        Ogre::NameValuePairList Opts;
        if(WF_Fullscreen & Flags)
        {
            SDLFlags|=SDL_WINDOW_FULLSCREEN;
            Settings.Fullscreen = true;
        }
        if(WF_Hidden & Flags)
        {
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
            SDLFlags|=SDL_WINDOW_RESIZABLE;
        }
        if(WF_Maximized & Flags)
        {
            SDLFlags|=SDL_WINDOW_MAXIMIZED;
        }
        else if(WF_Minimized & Flags)
        {
            SDLFlags|=SDL_WINDOW_MINIMIZED;
        }
        if(WF_Borderless & Flags)
        {
            SDLFlags|=SDL_WINDOW_BORDERLESS;
            Opts["border"] = "none";
        }
        /*SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
        SDLFlags|=SDL_WINDOW_OPENGL;
        SDLFlags|=SDL_WINDOW_SHOWN;
        SDLWindow = SDL_CreateWindow( WindowCaption.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, Settings.RenderWidth, Settings.RenderHeight, SDLFlags );

        if(!RC)
            RC = SDL_GL_CreateContext(this->SDLWindow);
        RenderContext = RC;

        Ogre::NameValuePairList* Binder;

        Binder = (Ogre::NameValuePairList*) crossplatform::GetSDLOgreBinder(SDLWindow,(size_t)RenderContext);
        Opts.insert(Binder->begin(),Binder->end());// */

        #ifdef LINUX
        Ogre::ResourceGroupManager::getSingleton().addResourceLocation(crossplatform::GetDataDirectory(),"FileSystem");
        #endif
        OgreWindow = Ogre::Root::getSingleton().createRenderWindow(WindowCaption, Settings.RenderWidth, Settings.RenderHeight, Settings.Fullscreen, &Opts);//*/

        #ifdef WINDOWS
        HWND Data = 0;
        OgreWindow->getCustomAttribute("WINDOW",&Data);
        SDLWindow = SDL_CreateWindowFrom(Data);
        #endif
        #ifdef LINUX
        Window Data=0;
        World::GetWorldPointer()->CommitLog();
        OgreWindow->getCustomAttribute("WINDOW",&Data);
        SDLWindow = SDL_CreateWindowFrom((void*)Data); // o rly?!
        #endif
        #ifdef MACOS
        NSWindow* Data = 0;
        OgreWindow->getCustomAttribute("WINDOW",Data);
        SDLWindow = SDL_CreateWindowFrom(Data);
        #endif// */
        SDL_SetWindowGrab(SDLWindow,SDL_TRUE);
        //delete Binder;
    }

    void GameWindow::CorrectViewportAndCamera(const Whole& Index)
    {
        Viewport* viewport = Viewports[Index];
        if(1 == Viewports.size()) viewport->SetDimensions(0,0,1,1);
        viewport->GetViewportCamera()->SetAspectRatio((Real)(viewport->GetActualWidth()) / (Real)(viewport->GetActualHeight()));
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

    void GameWindow::SetWindowCaption(const String &NewCaption)
    {
        SDL_SetWindowTitle(SDLWindow,NewCaption.c_str());
    }

    Viewport* GameWindow::CreateViewport(Camera* ViewportCamera, const Whole& ZOrder)
    {
        Whole Order = ZOrder == 0 ? Viewports.size() : ZOrder;
        Viewport* NewViewport = new Viewport(ViewportCamera,Order,this);
        Viewports.push_back(NewViewport);
        return NewViewport;
    }

    Viewport* GameWindow::GetViewport(const Whole& Index)
    {
        return Viewports[Index];
    }

    Whole GameWindow::GetNumViewports()
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

    const GraphicsSettings& GameWindow::GetSettings()
    {
        return Settings;
    }

    ///////////////////////////////////////////////////////////////////////////
    // Fullscreen functions
    ///////////////////////////////////
    bool GameWindow::getFullscreen() const
    {
        return Settings.Fullscreen;
    }

    //returns: false if changes could not be made
    void GameWindow::setFullscreen(const bool &Fullscreen)
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
                setRenderResolution(ResultWidth,ResultHeight);
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
            for(Whole X=0;X<Viewports.size();X++)
                CorrectViewportAndCamera(X);
            Settings.Fullscreen = Fullscreen;
        }
    }

    ///////////////////////////////////////////////////////////////////////////
    // Resolution functions
    ///////////////////////////////////
    Whole GameWindow::getRenderHeight() const
    {
        return Settings.RenderHeight;
    }

    Whole GameWindow::getRenderWidth() const
    {
        return Settings.RenderWidth;
    }

    void GameWindow::setRenderHeight(const Whole &Height)
    {
        if(Settings.RenderHeight == Height)
            return;
        setRenderResolution(Settings.RenderWidth,Height);
        Settings.RenderHeight = Height;
    }

    void GameWindow::setRenderWidth(const Whole &Width)
    {
        if(Settings.RenderWidth == Width)
            return;
        setRenderResolution(Width,Settings.RenderHeight);
        Settings.RenderWidth = Width;
    }

    void GameWindow::setRenderResolution(const Whole &Width, const Whole &Height)
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
                for(Whole X=0;X<Viewports.size();X++)
                    CorrectViewportAndCamera(X);
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
            for(Whole X=0;X<Viewports.size();X++)
                CorrectViewportAndCamera(X);
            Settings.RenderWidth = Width;
            Settings.RenderHeight = Height;
        }
    }

    void GameWindow::setRenderOptions(const GraphicsSettings& NewSettings)
    {
        setFullscreen(NewSettings.Fullscreen);
        setRenderResolution(NewSettings.RenderWidth,NewSettings.RenderHeight);
    }

    Whole GameWindow::GetFSAALevel() const
    {
        return OgreWindow->getFSAA();
    }

    ///////////////////////////////////////////////////////////////////////////////
    //Stats functions
    Real GameWindow::GetLastFPS()
    {
        return OgreWindow->getLastFPS();
    }

    Real GameWindow::GetAverageFPS()
    {
        return OgreWindow->getAverageFPS();
    }

    Real GameWindow::GetBestFPS()
    {
        return OgreWindow->getBestFPS();
    }

    Real GameWindow::GetWorstFPS()
    {
        return OgreWindow->getWorstFPS();
    }

    Real GameWindow::GetBestFrameTime()
    {
        return OgreWindow->getBestFrameTime();
    }

    Real GameWindow::GetWorstFrameTime()
    {
        return OgreWindow->getWorstFrameTime();
    }

    void* GameWindow::GetRenderContext()
    {
        return 0;//(void*)RenderContext;
    }

    Ogre::RenderWindow* GameWindow::GetOgreWindowPointer()
    {
        return OgreWindow;
    }

    SDL_Window* GameWindow::GetSDLWindowPointer()
    {
        return SDLWindow;
    }
}

#endif
