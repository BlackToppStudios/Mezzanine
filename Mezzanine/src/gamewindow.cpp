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

namespace Mezzanine
{
    GameWindow::GameWindow(const String& WindowCaption, const Whole& Width, const Whole& Height, const Whole& Flags, const ViewportLayout& ViewportConf)
        : OgreWindow(NULL),
          SDLWindow(NULL)
    {
        CreateGameWindow(WindowCaption,Width,Height,Flags,ViewportConf);
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

    void GameWindow::CreateGameWindow(const String& WindowCaption, const Whole& Width, const Whole& Height, const Whole& Flags, const ViewportLayout& ViewportConf)
    {
        Manager = GraphicsManager::GetSingletonPtr();
        Settings.RenderWidth = Width;
        Settings.RenderHeight = Height;
        ViewLayout = ViewportConf;

        Ogre::NameValuePairList Opts;
        if(WF_Fullscreen & Flags)
        {
            Settings.Fullscreen = true;
        }
        if(WF_Hidden & Flags)
        {
            Opts["hidden"] = true;
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
        }
        else if(WF_Borderless & Flags)
        {
            Opts["border"] = "none";
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

        if(VL_Custom != ViewLayout)
        {
            Vector2 ViewportSize, ViewportPosition;
            switch (ViewLayout)
            {
                case VL_1_FullWindow:
                {
                    Viewport* Viewport1 = CreateViewport(NULL,0);
                    Viewport1->SetDimensions(0,0,1,1);
                    break;
                }
                case VL_2_HorizontalSplit:
                {
                    Viewport* Viewport1 = CreateViewport(NULL,0);
                    Viewport1->SetDimensions(0,0,1,0.5);
                    Viewport* Viewport2 = CreateViewport(NULL,1);
                    Viewport2->SetDimensions(0,0.5,1,0.5);
                    break;
                }
                case VL_2_VerticalSplit:
                {
                    Viewport* Viewport1 = CreateViewport(NULL,0);
                    Viewport1->SetDimensions(0,0,0.5,1);
                    Viewport* Viewport2 = CreateViewport(NULL,1);
                    Viewport2->SetDimensions(0.5,0,0.5,1);
                    break;
                }
                case VL_3_TopLarge:
                {
                    Viewport* Viewport1 = CreateViewport(NULL,0);
                    Viewport1->SetDimensions(0,0,1,0.5);
                    Viewport* Viewport2 = CreateViewport(NULL,1);
                    Viewport2->SetDimensions(0,0.5,0.5,0.5);
                    Viewport* Viewport3 = CreateViewport(NULL,2);
                    Viewport3->SetDimensions(0.5,0.5,0.5,0.5);
                    break;
                }
                case VL_4_EvenlySpaced:
                {
                    Viewport* Viewport1 = CreateViewport(NULL,0);
                    Viewport1->SetDimensions(0,0,0.5,0.5);
                    Viewport* Viewport2 = CreateViewport(NULL,1);
                    Viewport2->SetDimensions(0.5,0,0.5,0.5);
                    Viewport* Viewport3 = CreateViewport(NULL,2);
                    Viewport3->SetDimensions(0,0.5,0.5,0.5);
                    Viewport* Viewport4 = CreateViewport(NULL,3);
                    Viewport4->SetDimensions(0.5,0.5,0.5,0.5);
                    break;
                }
            }
        }
    }

    void GameWindow::UpdateViewportsAndCameras()
    {
        switch (ViewLayout)
        {
            case VL_1_FullWindow:
            {
                GetViewport(0)->SetDimensions(0,0,1,1);
                break;
            }
            case VL_2_HorizontalSplit:
            {
                GetViewport(0)->SetDimensions(0,0,1,0.5);
                GetViewport(1)->SetDimensions(0,0.5,1,0.5);
                break;
            }
            case VL_2_VerticalSplit:
            {
                GetViewport(0)->SetDimensions(0,0,0.5,1);
                GetViewport(1)->SetDimensions(0.5,0,0.5,1);
                break;
            }
            case VL_3_TopLarge:
            {
                GetViewport(0)->SetDimensions(0,0,1,0.5);
                GetViewport(1)->SetDimensions(0,0.5,0.5,0.5);
                GetViewport(2)->SetDimensions(0.5,0.5,0.5,0.5);
                break;
            }
            case VL_4_EvenlySpaced:
            {
                GetViewport(0)->SetDimensions(0,0,0.5,0.5);
                GetViewport(1)->SetDimensions(0.5,0,0.5,0.5);
                GetViewport(2)->SetDimensions(0,0.5,0.5,0.5);
                GetViewport(3)->SetDimensions(0.5,0.5,0.5,0.5);
                break;
            }
        }
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
    bool GameWindow::GetFullscreen() const
    {
        return Settings.Fullscreen;
    }

    //returns: false if changes could not be made
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

    ///////////////////////////////////////////////////////////////////////////
    // Resolution functions
    ///////////////////////////////////
    Whole GameWindow::GetRenderHeight() const
    {
        return Settings.RenderHeight;
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

    void GameWindow::SetRenderWidth(const Whole &Width)
    {
        if(Settings.RenderWidth == Width)
            return;
        SetRenderResolution(Width,Settings.RenderHeight);
        Settings.RenderWidth = Width;
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

    void GameWindow::SetRenderOptions(const GraphicsSettings& NewSettings)
    {
        SetFullscreen(NewSettings.Fullscreen);
        SetRenderResolution(NewSettings.RenderWidth,NewSettings.RenderHeight);
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
