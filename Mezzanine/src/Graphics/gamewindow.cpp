// © Copyright 2010 - 2014 BlackTopp Studios Inc.
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

#include "Graphics/gamewindow.h"
#include "Graphics/viewport.h"
#include "Graphics/cameraproxy.h"
#include "Graphics/graphicsmanager.h"

#include "crossplatform.h"
#include "entresol.h"

#include "serialization.h"
#include "exception.h"

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
        GameWindow::GameWindow(const String& WindowCaption, const Whole Width, const Whole Height, const Whole Flags) :
            OgreWindow(NULL),
            SDLWindow(NULL),
            RequestedFSAA(0),
            CreationFlags(0)
            { this->CreateGameWindow(WindowCaption,Width,Height,Flags); }

        GameWindow::GameWindow(const XML::Node& XMLNode) :
            OgreWindow(NULL),
            SDLWindow(NULL),
            RequestedFSAA(0),
            CreationFlags(0)
            { this->ProtoDeSerialize(XMLNode); }

        GameWindow::~GameWindow()
        {
            this->DestroyAllViewports();
            // first lets clear out the user data manually, the alternative is using SDL's allocation methods to make it, which we can opt for later
            if( this->SDLWindow ) {
                SDL_WindowUserData* WindowData = this->SDLWindow->data;
                this->SDLWindow->data = NULL;
                delete WindowData;

                SDL_DestroyWindow(this->SDLWindow);
            }

            //this->OgreWindow->destroy();
            Ogre::Root::getSingleton().destroyRenderTarget(this->OgreWindow);
        }

        void GameWindow::CreateGameWindow(const String& WindowCaption, const Whole Width, const Whole Height, const Whole Flags)
        {
            this->Manager = Graphics::GraphicsManager::GetSingletonPtr();
            this->CreationFlags = Flags;
            this->Settings.WinRes.SetResolution(Width,Height);

            Ogre::NameValuePairList Opts;
            if(WF_Fullscreen & Flags) {
                this->Settings.Fullscreen = true;
            }

            if(WF_Hidden & Flags) {
                Opts["hidden"] = "true";
            }

            if(WF_VsyncEnabled & Flags) {
                Opts["vsync"] = "true";
                this->Settings.VSync = true;
            }

            if(WF_FSAA_16 & Flags) {
                Opts["FSAA"] = "16";
            }else if(WF_FSAA_8 & Flags) {
                Opts["FSAA"] = "8";
            }else if(WF_FSAA_4 & Flags) {
                Opts["FSAA"] = "4";
            }else if(WF_FSAA_2 & Flags) {
                Opts["FSAA"] = "2";
            }

            if(WF_Resizeable & Flags) {
                Opts["border"] = "resize";
            }else if(WF_Borderless & Flags) {
                Opts["border"] = "none";
            }else{
                Opts["border"] = "fixed";
            }

            if(WF_Maximized & Flags) {

            }

            #ifdef MACOSX
			Opts["macAPI"] = "cocoa";
            #endif

            //#ifdef LINUX
            //Ogre::ResourceGroupManager::getSingleton().addResourceLocation(ResourceManager::GetSingletonPtr()->GetEngineDataDirectory(),"FileSystem");
            //#endif
            this->OgreWindow = Ogre::Root::getSingleton().createRenderWindow(WindowCaption, this->Settings.WinRes.Width, this->Settings.WinRes.Height, this->Settings.Fullscreen, &Opts);//*/
            this->RequestedFSAA = this->GetActualFSAALevel();

            if( !(WF_Hidden & Flags) ) {
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

        void GameWindow::AddViewport(Viewport* NewVP)
        {
            for( ViewportIterator ViewIt = this->Viewports.begin() ; ViewIt != this->Viewports.end() ; ++ViewIt )
            {
                if( (*ViewIt)->GetZOrder() > NewVP->GetZOrder() ) {
                    this->Viewports.insert( ViewIt, NewVP );
                    return;
                }
            }
            this->Viewports.push_back( NewVP );
        }

        void GameWindow::UpdateViewportsAndCameras()
        {
            for( ViewportIterator ViewIt = this->Viewports.begin() ; ViewIt != this->Viewports.end() ; ++ViewIt )
            {
                CameraProxy* Cam = (*ViewIt)->GetCamera();
                if(Cam)
                    Cam->SetAspectRatio((Real)((*ViewIt)->GetActualWidth()) / (Real)((*ViewIt)->GetActualHeight()));
            }
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Viewport Management

        Viewport* GameWindow::CreateViewport(CameraProxy* ViewportCamera, const Integer ZOrder)
        {
            Viewport* NewViewport = new Viewport(ViewportCamera,ZOrder,this);
            this->AddViewport(NewViewport);
            return NewViewport;
        }

        Viewport* GameWindow::GetViewport(const Whole Index) const
        {
            Whole Count = Index;
            ConstViewportIterator ViewIt = this->Viewports.begin();
            while( Count-- )
                ++ViewIt;

            return (*ViewIt);
        }

        Viewport* GameWindow::GetViewportByZOrder(const Integer ZOrder) const
        {
            for( ConstViewportIterator ViewIt = this->Viewports.begin() ; ViewIt != this->Viewports.end() ; ++ViewIt )
            {
                if( (*ViewIt)->GetZOrder() == ZOrder ) {
                    return (*ViewIt);
                }
            }
            return NULL;
        }

        Whole GameWindow::GetNumViewports() const
        {
            return this->Viewports.size();
        }

        void GameWindow::DestroyViewport(Viewport* ToBeDestroyed)
        {
            for( ViewportIterator ViewIt = this->Viewports.begin() ; ViewIt != this->Viewports.end() ; ++ViewIt )
            {
                if ( ToBeDestroyed == (*ViewIt) ) {
                    delete ToBeDestroyed;
                    this->Viewports.erase(ViewIt);
                    return;
                }
            }
        }

        void GameWindow::DestroyAllViewports()
        {
            for( ViewportIterator ViewIt = this->Viewports.begin() ; ViewIt != this->Viewports.end() ; ++ViewIt )
            {
                delete (*ViewIt);
            }
            this->Viewports.clear();
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

        void GameWindow::SetWidth(const Whole& Width)
            { this->SetResolution(Width,this->Settings.WinRes.Height); }

        Whole GameWindow::GetWidth() const
            { return this->Settings.WinRes.Width; }

        void GameWindow::SetHeight(const Whole& Height)
            { this->SetResolution(this->Settings.WinRes.Width,Height); }

        Whole GameWindow::GetHeight() const
            { return this->Settings.WinRes.Height; }

        void GameWindow::SetResolution(const Resolution& WinRes)
            { this->SetResolution(WinRes.Width,WinRes.Height); }

        void GameWindow::SetResolution(const Whole& Width, const Whole& Height)
        {
            if( this->Settings.WinRes.Width != Width || this->Settings.WinRes.Height != Height ) {
                if( this->Settings.Fullscreen ) {
                    SDL_DisplayMode CurrentDisplay;
                    SDL_GetWindowDisplayMode(SDLWindow,&CurrentDisplay);
                    CurrentDisplay.w = Width;
                    CurrentDisplay.h = Height;
                    // CurrentDisplay.refresh_rate = 60;
                    if(SDL_SetWindowDisplayMode(SDLWindow,&CurrentDisplay) == 0) {
                        this->OgreWindow->setFullscreen(true,Width,Height);
                        //this->OgreWindow->resize(Width,Height);
                    }
                    //this->OgreWindow->resize(Width,Height);
                }else{
                    SDL_SetWindowSize(SDLWindow,Width,Height);
                    //this->OgreWindow->setFullscreen(false,Width,Height);
                    this->OgreWindow->resize(Width,Height);
                }
                this->UpdateViewportsAndCameras();
                this->Settings.WinRes.Width = Width;
                this->Settings.WinRes.Height = Height;
            }
        }

        const Resolution& GameWindow::GetResolution() const
        {
            return this->Settings.WinRes;
        }

        void GameWindow::SetFullscreen(const Boole Fullscreen)
        {
            //static SDL_DisplayMode FSDisplayMode;

            if( Fullscreen != this->Settings.Fullscreen ) {
                /*if( !Fullscreen && this->Settings.Fullscreen ) {
                    const WindowSettings& DeskSet = this->Manager->GetDesktopSettings();
                    if( this->Settings.WinRes.Width > DeskSet.WinRes.Width || this->Settings.WinRes.Height > DeskSet.WinRes.Height ) {
                        this->Settings.WinRes.Width = DeskSet.WinRes.Width;
                        this->Settings.WinRes.Height = DeskSet.WinRes.Height;
                    }
                    if( this->Settings.WinRes.Width == DeskSet.WinRes.Width || this->Settings.WinRes.Height == DeskSet.WinRes.Height ) {
                        Whole ResultWidth, ResultHeight;
                        crossplatform::SanitizeWindowedRes(Settings.WinRes.Width,Settings.WinRes.Height,ResultWidth,ResultHeight);
                        this->SetResolution(ResultWidth,ResultHeight);
                        this->Settings.WinRes.Width = DeskSet.WinRes.Width;
                        this->Settings.WinRes.Height = DeskSet.WinRes.Height;
                    }
                }else if(Fullscreen && !Settings.Fullscreen) {
                    FSDisplayMode.w = this->Settings.WinRes.Width;
                    FSDisplayMode.h = this->Settings.WinRes.Height;
                    FSDisplayMode.refresh_rate = Settings.RefreshRate;
                    SDL_SetWindowDisplayMode(SDLWindow,&FSDisplayMode);
                }//*/

                if(SDL_SetWindowFullscreen(SDLWindow, Fullscreen?SDL_TRUE:SDL_FALSE ) == 0) {
                    this->OgreWindow->setFullscreen(Fullscreen,this->Settings.WinRes.Width,this->Settings.WinRes.Height);
                    this->UpdateViewportsAndCameras();
                    this->Settings.Fullscreen = Fullscreen;
                }
            }
        }

        Boole GameWindow::GetFullscreen() const
        {
            return this->Settings.Fullscreen;
        }

        void GameWindow::SetRenderOptions(const WindowSettings& NewSettings)
        {
            this->SetFullscreen( NewSettings.Fullscreen );
            this->SetResolution( NewSettings.WinRes.Width, NewSettings.WinRes.Height );
        }

        const WindowSettings& GameWindow::GetSettings()
        {
            return this->Settings;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Window Settings Methods

        const String& GameWindow::GetWindowCaption() const
            { return this->OgreWindow->getName(); }

        void GameWindow::SetFSAALevel(const Whole FSAA)
            { this->RequestedFSAA = FSAA; }

        Whole GameWindow::GetFSAALevel() const
            { return this->RequestedFSAA; }

        Whole GameWindow::GetActualFSAALevel() const
            { return this->OgreWindow->getFSAA(); }

        void GameWindow::EnableVsync(Boole Enable)
            { this->OgreWindow->setVSyncEnabled(Enable); }

        Boole GameWindow::VsyncEnabled() const
            { return this->OgreWindow->isVSyncEnabled(); }

        void GameWindow::SetHidden(Boole Hidden)
            { this->OgreWindow->setHidden(Hidden); }

        Boole GameWindow::IsHidden() const
            { return this->OgreWindow->isHidden(); }

        Boole GameWindow::BorderIsResizeable() const
            { return (this->CreationFlags & GameWindow::WF_Resizeable); }

        Boole GameWindow::IsBorderless() const
            { return (this->CreationFlags & GameWindow::WF_Borderless); }

        ///////////////////////////////////////////////////////////////////////////////
        // Window Stats Methods

        Real GameWindow::GetLastFPS() const
            { return this->OgreWindow->getLastFPS(); }

        Real GameWindow::GetAverageFPS() const
            { return this->OgreWindow->getAverageFPS(); }

        Real GameWindow::GetBestFPS() const
            { return this->OgreWindow->getBestFPS(); }

        Real GameWindow::GetWorstFPS() const
            { return this->OgreWindow->getWorstFPS(); }

        Real GameWindow::GetBestFrameTime() const
            { return this->OgreWindow->getBestFrameTime(); }

        Real GameWindow::GetWorstFrameTime() const
            { return this->OgreWindow->getWorstFrameTime(); }

        ///////////////////////////////////////////////////////////////////////////////
        // Serialization

        void GameWindow::ProtoSerialize(XML::Node& ParentNode) const
        {
            XML::Node SelfRoot = ParentNode.AppendChild( GameWindow::GetSerializableName() );

            this->ProtoSerializeProperties(SelfRoot);
            this->ProtoSerializeViewports(SelfRoot);
        }

        void GameWindow::ProtoSerializeProperties(XML::Node& SelfRoot) const
        {
            XML::Node PropertiesNode = SelfRoot.AppendChild( GameWindow::GetSerializableName() + "Properties" );

            if( PropertiesNode.AppendAttribute("Version").SetValue("1") &&
                PropertiesNode.AppendAttribute("Caption").SetValue( this->GetWindowCaption() ) &&
                PropertiesNode.AppendAttribute("Width").SetValue( this->GetWidth() ) &&
                PropertiesNode.AppendAttribute("Height").SetValue( this->GetHeight() ) &&
                PropertiesNode.AppendAttribute("Fullscreen").SetValue( this->GetFullscreen() ) &&
                PropertiesNode.AppendAttribute("Hidden").SetValue( this->IsHidden() ) &&
                PropertiesNode.AppendAttribute("Vsync").SetValue( this->VsyncEnabled() ) &&
                PropertiesNode.AppendAttribute("Resizeable").SetValue( this->BorderIsResizeable() ) &&
                PropertiesNode.AppendAttribute("Borderless").SetValue( this->IsBorderless() ) &&
                PropertiesNode.AppendAttribute("FSAA").SetValue( this->GetFSAALevel() ) )
                /// @todo Currently the maximized setting does nothing in the gamewindow.  If it gets implemented, so does this.
                //PropertiesNode.AppendAttribute("Maximized").SetValue( (*WinIt)-> );//*/ )
            {
                return;
            }else{
                SerializeError("Create XML Attribute Values",GameWindow::GetSerializableName() + "Properties",true);
            }
        }

        void GameWindow::ProtoSerializeViewports(XML::Node& SelfRoot) const
        {
            XML::Node ViewportsNode = SelfRoot.AppendChild( "Viewports" );
            if( ViewportsNode.AppendAttribute("Version").SetValue("1") == false ) {
                SerializeError("Create XML Version Attribute","Viewports",true);
            }

            for( ConstViewportIterator ViewIt = this->Viewports.begin() ; ViewIt != this->Viewports.end() ; ++ViewIt )
            {
                (*ViewIt)->ProtoSerialize(ViewportsNode);
            }
        }

        void GameWindow::ProtoDeSerialize(const XML::Node& SelfRoot)
        {
            this->ProtoDeSerializeProperties(SelfRoot);
            this->ProtoDeSerializeViewports(SelfRoot);
        }

        void GameWindow::ProtoDeSerializeProperties(const XML::Node& SelfRoot)
        {
            this->DestroyAllViewports();
            if( this->OgreWindow != NULL ) {
                Ogre::Root::getSingleton().destroyRenderTarget(this->OgreWindow);
                this->OgreWindow = NULL;
            }

            XML::Attribute CurrAttrib;
            XML::Node PropertiesNode = SelfRoot.GetChild( GameWindow::GetSerializableName() + "Properties" );

            if( !PropertiesNode.Empty() ) {
                if(SelfRoot.GetAttribute("Version").AsInt() == 1) {
                    String WinCaption;
                    Whole WinWidth = 0, WinHeight = 0, WinFlags = 0;

                    CurrAttrib = SelfRoot.GetAttribute("Caption");
                    if( !CurrAttrib.Empty() )
                        WinCaption = CurrAttrib.AsString();

                    CurrAttrib = SelfRoot.GetAttribute("Width");
                    if( !CurrAttrib.Empty() )
                        WinWidth = CurrAttrib.AsWhole();

                    CurrAttrib = SelfRoot.GetAttribute("Height");
                    if( !CurrAttrib.Empty() )
                        WinHeight = CurrAttrib.AsWhole();

                    CurrAttrib = SelfRoot.GetAttribute("Fullscreen");
                    if( !CurrAttrib.Empty() && CurrAttrib.AsBool() )
                        WinFlags |= WF_Fullscreen;

                    CurrAttrib = SelfRoot.GetAttribute("Hidden");
                    if( !CurrAttrib.Empty() && CurrAttrib.AsBool() )
                        WinFlags |= WF_Hidden;

                    CurrAttrib = SelfRoot.GetAttribute("Vsync");
                    if( !CurrAttrib.Empty() && CurrAttrib.AsBool() )
                        WinFlags |= WF_VsyncEnabled;

                    CurrAttrib = SelfRoot.GetAttribute("Resizeable");
                    if( !CurrAttrib.Empty() && CurrAttrib.AsBool() )
                        WinFlags |= WF_Resizeable;

                    CurrAttrib = SelfRoot.GetAttribute("Borderless");
                    if( !CurrAttrib.Empty() && CurrAttrib.AsBool() )
                        WinFlags |= WF_Borderless;

                    CurrAttrib = SelfRoot.GetAttribute("FSAA");
                    if( !CurrAttrib.Empty() ) {
                        Whole FSAALevel = CurrAttrib.AsWhole();
                        if( FSAALevel == 1 ) {
                            WinFlags |= WF_FSAA_2;
                        }else if( FSAALevel == 2 ) {
                            WinFlags |= WF_FSAA_4;
                        }else if( FSAALevel == 3 ) {
                            WinFlags |= WF_FSAA_8;
                        }else if( FSAALevel == 4 ) {
                            WinFlags |= WF_FSAA_16;
                        }
                    }

                    this->CreateGameWindow(WinCaption,WinWidth,WinHeight,WinFlags);
                }else{
                    MEZZ_EXCEPTION(Exception::INVALID_VERSION_EXCEPTION,"Incompatible XML Version for " + GameWindow::GetSerializableName() + ": Not Version 1.");
                }
            }else{
                MEZZ_EXCEPTION(Exception::II_IDENTITY_NOT_FOUND_EXCEPTION,GameWindow::GetSerializableName() + " was not found in the provided XML node, which was expected.");
            }
        }

        void GameWindow::ProtoDeSerializeViewports(const XML::Node& SelfRoot)
        {
            this->DestroyAllViewports();

            XML::Attribute CurrAttrib;
            XML::Node ViewportsNode = SelfRoot.GetChild( "Viewports" );

            if( !ViewportsNode.Empty() ) {
                if(ViewportsNode.GetAttribute("Version").AsInt() == 1) {
                    for( XML::NodeIterator ViewNodeIt = ViewportsNode.begin() ; ViewNodeIt != ViewportsNode.end() ; ++ViewNodeIt )
                    {
                        Viewport* NewViewport = new Viewport( (*ViewNodeIt), this );
                        this->AddViewport(NewViewport);
                    }
                }else{
                    MEZZ_EXCEPTION(Exception::INVALID_VERSION_EXCEPTION,"Incompatible XML Version for RenderLayers: Not Version 1.");
                }
            }
        }

        String GameWindow::GetSerializableName()
            { return "GameWindow"; }

        ///////////////////////////////////////////////////////////////////////////////
        // Internal Methods

        Ogre::RenderWindow* GameWindow::_GetOgreWindowPointer()
            { return this->OgreWindow; }

        SDL_Window* GameWindow::_GetSDLWindowPointer()
            { return this->SDLWindow; }
    }//Graphics
}//Mezzanine

#endif
