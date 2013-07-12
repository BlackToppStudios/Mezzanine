//© Copyright 2010 - 2013 BlackTopp Studios Inc.
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
#ifndef _graphicsgamewindow_cpp
#define _graphicsgamewindow_cpp

#include "datatypes.h"
#include "Graphics/gamewindow.h"
#include "crossplatform.h"
#include "Graphics/viewport.h"
#include "camera.h"
#include "graphicsmanager.h"
#include "entresol.h"

#ifdef WINDOWS
#include <windows.h>
#endif

#ifdef LINUX
#include <X11/X.h>      //x11proto-core-dev
#include <X11/Xlib.h>   //libx11-dev
#endif

#ifdef MACOSX
#include <Cocoa/Cocoa.h>
#endif

#include <SDL.h>
#include "../src/video/SDL_sysvideo.h"
#include <Ogre.h>

namespace Mezzanine
{
    namespace Graphics
    {
        class GameWindowInternalData
        {
        public:
            bool Borderless;
            bool Resizeable;

            GameWindowInternalData() : Borderless(false), Resizeable(false) {  }
        };

        GameWindow::GameWindow(const String& WindowCaption, const Whole& Width, const Whole& Height, const Whole& Flags)
            : OgreWindow(NULL),
              SDLWindow(NULL)
        {
            this->GWID = new GameWindowInternalData();
            this->CreateGameWindow(WindowCaption,Width,Height,Flags);
        }

        GameWindow::~GameWindow()
        {
            // first lets clear out the user data manually, the alternative is using SDL's allocation methods to make it, which we can opt for later
            if( this->SDLWindow ) {
                SDL_WindowUserData* WindowData = this->SDLWindow->data;
                this->SDLWindow->data = NULL;
                delete WindowData;

                SDL_DestroyWindow(this->SDLWindow);
            }

            this->OgreWindow->destroy();
            Ogre::Root::getSingleton().destroyRenderTarget(OgreWindow);
            delete GWID;
        }

        void GameWindow::CreateGameWindow(const String& WindowCaption, const Whole& Width, const Whole& Height, const Whole& Flags)
        {
            this->Manager = Graphics::GraphicsManager::GetSingletonPtr();
            this->Settings.RenderWidth = Width;
            this->Settings.RenderHeight = Height;

            Ogre::NameValuePairList Opts;
            if(WF_Fullscreen & Flags)
            {
                this->Settings.Fullscreen = true;
            }
            if(WF_Hidden & Flags)
            {
                Opts["hidden"] = "true";
            }
            if(WF_VsyncEnabled & Flags)
            {
                Opts["vsync"] = "true";
                this->Settings.VSync = true;
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
                this->GWID->Resizeable = true;
            }
            else if(WF_Borderless & Flags)
            {
                Opts["border"] = "none";
                this->GWID->Borderless = true;
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
            this->OgreWindow = Ogre::Root::getSingleton().createRenderWindow(WindowCaption, this->Settings.RenderWidth, this->Settings.RenderHeight, this->Settings.Fullscreen, &Opts);//*/

            if( !(WF_Hidden & Flags) )
            {
                #ifdef WINDOWS
                HWND Data = 0;
                #endif
                #ifdef LINUX
                Window Data = 0;
                #endif
                #ifdef MACOSX
                NSWindow* Data = 0;
                #endif
                this->OgreWindow->getCustomAttribute("WINDOW",&Data);
                this->SDLWindow = SDL_CreateWindowFrom((void*)Data);
                this->SDLWindow->data = new SDL_WindowUserData();
                this->SDLWindow->data->name = NULL;
                this->SDLWindow->data->data = this;
                this->SDLWindow->data->next = NULL;
            }
        }

        void GameWindow::UpdateViewportsAndCameras()
        {
            for( ViewportIterator ViewIt = this->Viewports.begin() ; ViewIt != this->Viewports.end() ; ++ViewIt )
            {
                Camera* Cam = (*ViewIt).second->GetViewportCamera();
                if(Cam)
                    Cam->SetAspectRatio((Real)((*ViewIt).second->GetActualWidth()) / (Real)((*ViewIt).second->GetActualHeight()));
            }
        }

        int GameWindow::IsLargerThenDesktop(const Whole& Width, const Whole& Height)
        {
            SDL_DisplayMode DesktopDisplay;
            SDL_GetDesktopDisplayMode(0,&DesktopDisplay);
            if(Width > Whole(DesktopDisplay.w) || Height > Whole(DesktopDisplay.h))
                return 1;
            else if(Width == Whole(DesktopDisplay.w) || Height == Whole(DesktopDisplay.h))
                return 0;
            else
                return -1;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Viewport Management

        Viewport* GameWindow::CreateViewport(Camera* ViewportCamera, const Integer ZOrder)
        {
            Whole Order = ( ZOrder == 0 ? this->Viewports.size() : ZOrder );
            Viewport* NewViewport = new Viewport(ViewportCamera,Order,this);

            for( ViewportIterator ViewIt = this->Viewports.begin() ; ViewIt != this->Viewports.end() ; ++ViewIt )
            {
                if( (*ViewIt).second->GetZOrder() > ZOrder ) {
                    this->Viewports.insert( ViewIt, ViewportEntry( ZOrder, NewViewport ) );
                    return NewViewport;
                }
            }

            this->Viewports.push_back( ViewportEntry(ZOrder,NewViewport) );
            return NewViewport;
        }

        Viewport* GameWindow::GetViewport(const Whole Index) const
        {
            Whole Count = Index;
            ConstViewportIterator ViewIt = this->Viewports.begin();
            while( Count-- )
                ++ViewIt;

            return (*ViewIt).second;
        }

        Whole GameWindow::GetNumViewports() const
        {
            return this->Viewports.size();
        }

        void GameWindow::DestroyViewport(Viewport* ToBeDestroyed)
        {
            for ( ViewportIterator it = this->Viewports.begin() ; it != this->Viewports.end() ; it++ )
            {
                if ( ToBeDestroyed == (*it).second )
                {
                    delete ToBeDestroyed;
                    this->Viewports.erase(it);
                    return;
                }
            }
        }

        GameWindow::ViewportIterator GameWindow::BeginViewport()
            { return this->Viewports.begin(); }

        GameWindow::ViewportIterator GameWindow::EndViewport()
            { return this->Viewports.end(); }

        GameWindow::ConstViewportIterator GameWindow::BeginViewport() const
            { return this->Viewports.begin(); }

        GameWindow::ConstViewportIterator GameWindow::EndViewport() const
            { return this->Viewports.end(); }

        GameWindow::ReverseViewportIterator GameWindow::ReverseBeginViewport()
            { return this->Viewports.rbegin(); }

        GameWindow::ReverseViewportIterator GameWindow::ReverseEndViewport()
            { return this->Viewports.rend(); }

        GameWindow::ConstReverseViewportIterator GameWindow::ReverseBeginViewport() const
            { return this->Viewports.rbegin(); }

        GameWindow::ConstReverseViewportIterator GameWindow::ReverseEndViewport() const
            { return this->Viewports.rend(); }

        ///////////////////////////////////////////////////////////////////////////////
        // Window Metrics Management

        void GameWindow::SetRenderWidth(const Whole& Width)
        {
            if( this->Settings.RenderWidth == Width )
                return;
            this->SetRenderResolution(Width,this->Settings.RenderHeight);
            this->Settings.RenderWidth = Width;
        }

        Whole GameWindow::GetRenderWidth() const
        {
            return this->Settings.RenderWidth;
        }

        void GameWindow::SetRenderHeight(const Whole& Height)
        {
            if( this->Settings.RenderHeight == Height )
                return;
            this->SetRenderResolution(this->Settings.RenderWidth,Height);
            this->Settings.RenderHeight = Height;
        }

        Whole GameWindow::GetRenderHeight() const
        {
            return this->Settings.RenderHeight;
        }

        void GameWindow::SetRenderResolution(const Whole& Width, const Whole& Height)
        {
            if( this->Settings.RenderWidth == Width && this->Settings.RenderHeight == Height )
                return;
            if( this->Settings.Fullscreen )
            {
                SDL_DisplayMode CurrentDisplay;
                SDL_GetWindowDisplayMode(SDLWindow,&CurrentDisplay);
                CurrentDisplay.w = Width;
                CurrentDisplay.h = Height;
                //CurrentDisplay.refresh_rate = 60;
                if(SDL_SetWindowDisplayMode(SDLWindow,&CurrentDisplay) == 0)
                {
                    this->OgreWindow->setFullscreen(true,Width,Height);
                    this->UpdateViewportsAndCameras();
                    this->Settings.RenderWidth = Width;
                    this->Settings.RenderHeight = Height;
                    return;
                }
            }else{
                int Result = this->IsLargerThenDesktop(Width,Height);
                if(Result == 0)
                {
                    Whole ResultWidth, ResultHeight;
                    crossplatform::SanitizeWindowedRes(Width,Height,ResultWidth,ResultHeight);
                    SDL_SetWindowSize(SDLWindow,ResultWidth,ResultHeight);
                    this->OgreWindow->setFullscreen(false,ResultWidth,ResultHeight);
                }else if(Result == 1){
                    Entresol::GetSingletonPtr()->Log("Cannot create a window larger then the desktop resolution.");
                    return;
                }else{
                    SDL_SetWindowSize(SDLWindow,Width,Height);
                    this->OgreWindow->setFullscreen(false,Width,Height);
                }
                this->UpdateViewportsAndCameras();
                this->Settings.RenderWidth = Width;
                this->Settings.RenderHeight = Height;
            }
        }

        void GameWindow::SetFullscreen(const bool Fullscreen)
        {
            static SDL_DisplayMode FSDisplayMode;

            if( Fullscreen == this->Settings.Fullscreen )
                return;

            if( !Fullscreen && this->Settings.Fullscreen )
            {
                const WindowSettings& DeskSet = this->Manager->GetDesktopSettings();
                if( this->Settings.RenderWidth > DeskSet.RenderWidth || this->Settings.RenderHeight > DeskSet.RenderHeight )
                {
                    this->Settings.RenderWidth = DeskSet.RenderWidth;
                    this->Settings.RenderHeight = DeskSet.RenderHeight;
                }
                if( this->Settings.RenderWidth == DeskSet.RenderWidth || this->Settings.RenderHeight == DeskSet.RenderHeight )
                {
                    Whole ResultWidth, ResultHeight;
                    crossplatform::SanitizeWindowedRes(Settings.RenderWidth,Settings.RenderHeight,ResultWidth,ResultHeight);
                    this->SetRenderResolution(ResultWidth,ResultHeight);
                    this->Settings.RenderWidth = DeskSet.RenderWidth;
                    this->Settings.RenderHeight = DeskSet.RenderHeight;
                }
            }
            else if(Fullscreen && !Settings.Fullscreen)
            {
                FSDisplayMode.w = this->Settings.RenderWidth;
                FSDisplayMode.h = this->Settings.RenderHeight;
                FSDisplayMode.refresh_rate = Settings.RefreshRate;
                SDL_SetWindowDisplayMode(SDLWindow,&FSDisplayMode);
            }

            if(SDL_SetWindowFullscreen(SDLWindow, Fullscreen?SDL_TRUE:SDL_FALSE ) == 0)
            {
                this->OgreWindow->setFullscreen(Fullscreen,this->Settings.RenderWidth,this->Settings.RenderHeight);
                this->UpdateViewportsAndCameras();
                this->Settings.Fullscreen = Fullscreen;
            }
        }

        bool GameWindow::GetFullscreen() const
        {
            return this->Settings.Fullscreen;
        }

        void GameWindow::SetRenderOptions(const WindowSettings& NewSettings)
        {
            this->SetFullscreen( NewSettings.Fullscreen );
            this->SetRenderResolution( NewSettings.RenderWidth, NewSettings.RenderHeight );
        }

        const WindowSettings& GameWindow::GetSettings()
        {
            return this->Settings;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Window Settings Methods

        const String& GameWindow::GetWindowCaption()
        {
            return this->OgreWindow->getName();
        }

        Whole GameWindow::GetFSAALevel() const
        {
            return this->OgreWindow->getFSAA();
        }

        void GameWindow::EnableVsync(bool Enable)
        {
            this->OgreWindow->setVSyncEnabled(Enable);
        }

        bool GameWindow::VsyncEnabled() const
        {
            return this->OgreWindow->isVSyncEnabled();
        }

        void GameWindow::SetHidden(bool Hidden)
        {
            this->OgreWindow->setHidden(Hidden);
        }

        bool GameWindow::IsHidden() const
        {
            return this->OgreWindow->isHidden();
        }

        bool GameWindow::BorderIsResizeable() const
        {
            return this->GWID->Resizeable;
        }

        bool GameWindow::IsBorderless() const
        {
            return this->GWID->Borderless;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Window Stats Methods

        Real GameWindow::GetLastFPS() const
        {
            return this->OgreWindow->getLastFPS();
        }

        Real GameWindow::GetAverageFPS() const
        {
            return this->OgreWindow->getAverageFPS();
        }

        Real GameWindow::GetBestFPS() const
        {
            return this->OgreWindow->getBestFPS();
        }

        Real GameWindow::GetWorstFPS() const
        {
            return this->OgreWindow->getWorstFPS();
        }

        Real GameWindow::GetBestFrameTime() const
        {
            return this->OgreWindow->getBestFrameTime();
        }

        Real GameWindow::GetWorstFrameTime() const
        {
            return this->OgreWindow->getWorstFrameTime();
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Internal Methods

        Ogre::RenderWindow* GameWindow::_GetOgreWindowPointer()
        {
            return this->OgreWindow;
        }

        SDL_Window* GameWindow::_GetSDLWindowPointer()
        {
            return this->SDLWindow;
        }
    }//Graphics
}//Mezzanine

#endif
