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
#ifndef _graphicsmanager_cpp
#define _graphicsmanager_cpp

#include "graphicsmanager.h"
#include "eventmanager.h"
#include "cameramanager.h"
#include "resourcemanager.h"
#include "UI/uimanager.h"
#include "camera.h"
#include "cameramanager.h"
#include "crossplatform.h"
#include "Graphics/viewport.h"
#include "stringtool.h"
#include "entresol.h"

#include <SDL.h>
#include <Ogre.h>

#ifdef MEZZ_BUILD_DIRECTX9_SUPPORT
#include "OgreD3D9Plugin.h"
#endif
#ifdef MEZZ_BUILD_DIRECTX11_SUPPORT
#include "OgreD3D11Plugin.h"
#endif
#ifdef MEZZ_BUILD_OPENGLES2_SUPPORT
#include "OgreGLES2Plugin.h"
#endif
#ifdef MEZZ_BUILD_OPENGLES_SUPPORT
#include "OgreGLESPlugin.h"
#endif
#ifdef MEZZ_BUILD_OPENGL_SUPPORT
#include "OgreGLPlugin.h"
#endif

#include <cstdlib>

namespace Mezzanine
{
    namespace Graphics
    {
        ///////////////////////////////////////////////////////////////////////////
        // RenderWorkUnit Methods

        RenderWorkUnit::RenderWorkUnit(GraphicsManager* Target) :
            TargetManager(Target)
            {  }

        RenderWorkUnit::~RenderWorkUnit()
            {  }

        void RenderWorkUnit::UseThreads(const Whole& AmountToUse)
        {
            //TargetGraphicsManager->SomehowTellOgreHowManyThread(AmountToUse);
        }

        Whole RenderWorkUnit::UsingThreadCount()
        {
            //return TargetGraphicsManager->GetThreadCountFromOgreSomehow();
            return 1;
        }

        void RenderWorkUnit::DoWork(Threading::DefaultThreadSpecificStorage::Type& CurrentThreadStorage)
        {
            this->TargetManager->ThreadResources = &CurrentThreadStorage; // Allow this full access to any thread specific resources it needs
            Ogre::WindowEventUtilities::messagePump();
            this->TargetManager->RenderOneFrame();
            this->TargetManager->ThreadResources = NULL;                  // Take it all away.
        }

        ///////////////////////////////////////////////////////////////////////////
        // GraphicsManager Methods

        template<> GraphicsManager* Singleton<GraphicsManager>::SingletonPtr = NULL;

        GraphicsManager::GraphicsManager() :
            RenderWork(NULL),
            ThreadResources(NULL),
            OgreBeenInitialized(false),
            CurrRenderSys(Graphics::RS_OpenGL2)
        {
            this->Construct();
            this->AutoGenFiles = false;
        }

        GraphicsManager::GraphicsManager(XML::Node& XMLNode) :
            RenderWork(NULL),
            ThreadResources(NULL),
            OgreBeenInitialized(false),
            CurrRenderSys(Graphics::RS_OpenGL2)
        {
            this->Construct();

            XML::Attribute CurrAttrib;
            String PathPreset;
            // Get whether or not to autogen the directory path and settings file.
            XML::Node AutoGenNode = XMLNode.GetChild("AutoCreateSettings");
            if(!AutoGenNode.Empty())
            {
                CurrAttrib = AutoGenNode.GetAttribute("Auto");
                if(!CurrAttrib.Empty())
                    this->AutoGenPath = this->AutoGenFiles = StringTools::ConvertToBool( CurrAttrib.AsString() );
            }
            // Get preset path to default to if a path is not provided.
            XML::Node PathNode = XMLNode.GetChild("SettingsPath");
            if(!PathNode.Empty())
            {
                CurrAttrib = PathNode.GetAttribute("Path");
                if(!CurrAttrib.Empty())
                    PathPreset = CurrAttrib.AsString();

                if(!PathPreset.empty())
                    this->SetSettingsFilePath(PathPreset);
            }
            // Get the files to be loaded, and load them.
            XML::Node FilesNode = XMLNode.GetChild("SettingsFiles");
            if(!FilesNode.Empty())
            {
                for( XML::NodeIterator SetFileIt = FilesNode.begin() ; SetFileIt != FilesNode.end() ; ++SetFileIt )
                {
                    String FileName, FilePath, FileGroup;
                    // Get the filename to load
                    CurrAttrib = (*SetFileIt).GetAttribute("FileName");
                    if(!CurrAttrib.Empty())
                        FileName = CurrAttrib.AsString();
                    // Get the path
                    CurrAttrib = (*SetFileIt).GetAttribute("Path");
                    if(!CurrAttrib.Empty())
                        FilePath = CurrAttrib.AsString();
                    else
                    {
                        CurrAttrib = (*SetFileIt).GetAttribute("Group");
                        if(!CurrAttrib.Empty())
                            FileGroup = CurrAttrib.AsString();
                    }

                    if(FilePath.empty())
                    {
                        if(FileGroup.empty()) this->LoadSettings(FileName);
                        else this->LoadSettingsFromGroup(FileName,FileGroup);
                    }
                    else this->LoadSettings(FileName,FilePath);
                }
            }
            /// @todo This is currently necessary because a render window of some kind needs to exist for the loading
            /// of resources that occurs later in world construction (when this is constructed by the world, which this
            /// assumes.  If possible this should be removed, to keep construction more flexible.
            this->InitOgreRenderSystem();
        }


        GraphicsManager::~GraphicsManager()
        {
            this->Deinitialize();
            this->DestroyAllGameWindows(false);

            UInt32 InitSDLSystems = SDL_WasInit(0);
            if( SDL_INIT_VIDEO | InitSDLSystems )
            {
                SDL_QuitSubSystem(SDL_INIT_VIDEO);
            }

            delete RenderWork;
        }

        void GraphicsManager::Construct()
        {
            this->PrimaryGameWindow=0;
            this->RenderWork = new RenderWorkUnit(this);

            UInt32 InitSDLSystems = SDL_WasInit(0);
            if( (SDL_INIT_VIDEO & InitSDLSystems) == 0 )
            {
                if( SDL_Init(SDL_INIT_VIDEO | SDL_INIT_NOPARACHUTE) < 0 )
                    { MEZZ_EXCEPTION(Exception::INTERNAL_EXCEPTION,String("Failed to Initialize SDL for Video/Windowing, SDL Error: ") + SDL_GetError()); }
            }

            SDL_DisplayMode DeskMode;
            SDL_GetDesktopDisplayMode(0,&DeskMode);
            DesktopSettings.RenderWidth = DeskMode.w;
            DesktopSettings.RenderHeight = DeskMode.h;
            DesktopSettings.RefreshRate = DeskMode.refresh_rate;

        }

        void GraphicsManager::InitOgreRenderSystem()
        {
            if( !this->OgreBeenInitialized )
            {
                Ogre::Root* OgreCore = Ogre::Root::getSingletonPtr();
                Ogre::Plugin* CurrentRenderSystem = 0;

                #ifdef MEZZ_BUILD_DIRECTX9_SUPPORT
                CurrentRenderSystem = new Ogre::D3D9Plugin();
                this->RenderSystems.push_back( CurrentRenderSystem );
                OgreCore->installPlugin( CurrentRenderSystem );
                this->RenderSystemTypes.push_back( Graphics::RS_DirectX9 );
                #endif
                #ifdef MEZZ_BUILD_DIRECTX11_SUPPORT
                CurrentRenderSystem = new Ogre::D3D11Plugin();
                this->RenderSystems.push_back( CurrentRenderSystem );
                OgreCore->installPlugin( CurrentRenderSystem );
                this->RenderSystemTypes.push_back( Graphics::RS_DirectX11 );
                #endif
                #ifdef MEZZ_BUILD_OPENGLES2_SUPPORT
                CurrentRenderSystem = new Ogre::GLES2Plugin();
                this->RenderSystems.push_back( CurrentRenderSystem );
                OgreCore->installPlugin( CurrentRenderSystem );
                this->RenderSystemTypes.push_back( Graphics::RS_OpenGLES2 );
                #endif
                #ifdef MEZZ_BUILD_OPENGLES_SUPPORT
                CurrentRenderSystem = new Ogre::GLESPlugin();
                this->RenderSystems.push_back( CurrentRenderSystem );
                OgreCore->installPlugin( CurrentRenderSystem );
                this->RenderSystemTypes.push_back( Graphics::RS_OpenGLES1 );
                #endif
                #ifdef MEZZ_BUILD_OPENGL_SUPPORT
                CurrentRenderSystem = new Ogre::GLPlugin();
                this->RenderSystems.push_back( CurrentRenderSystem );
                OgreCore->installPlugin( CurrentRenderSystem );
                this->RenderSystemTypes.push_back( Graphics::RS_OpenGL2 );
                #endif

                if( this->RenderSystems.size() == 1 )
                {
                    Ogre::RenderSystem* temp = OgreCore->getRenderSystemByName( this->GetRenderSystemName( this->RenderSystemTypes[0] ) );
                    OgreCore->setRenderSystem( OgreCore->getRenderSystemByName( this->GetRenderSystemName( this->RenderSystemTypes[0] ) ) );
                }else{
                    Ogre::RenderSystem* temp = OgreCore->getRenderSystemByName( this->GetRenderSystemName( this->CurrRenderSys ) );
                    OgreCore->setRenderSystem( OgreCore->getRenderSystemByName( this->GetRenderSystemName( this->CurrRenderSys ) ) );
                }

                OgreCore->initialise(false,"");
                this->OgreBeenInitialized = true;

                this->PrimaryGameWindow = new Graphics::GameWindow("Primary",1,1,Graphics::GameWindow::WF_Hidden);
            }
        }


        String GraphicsManager::GetObjectRootNodeName() const
        {
            return "DefaultGraphicsManagerSettings";
        }

        void GraphicsManager::AppendCurrentSettings(XML::Node& SettingsRootNode)
        {
            // Create the Group node to be returned
            XML::Node CurrentSettings = SettingsRootNode.AppendChild("Current");
            // Create and initialize the rendersystem settings
            XML::Node RenderSystemNode = CurrentSettings.AppendChild("RenderSystem");
            RenderSystemNode.AppendAttribute("Name").SetValue( this->GetShortenedRenderSystemName(CurrRenderSys) );
            // Create and initialize the window configuration
            for( GameWindowIterator WinIt = this->BeginGameWindow() ; WinIt != this->EndGameWindow() ; ++WinIt )
            {
                XML::Node WindowConfigNode = CurrentSettings.AppendChild("GameWindow");
                WindowConfigNode.AppendAttribute("Caption").SetValue( (*WinIt)->GetWindowCaption() );
                WindowConfigNode.AppendAttribute("Width").SetValue( StringTools::ConvertToString( (*WinIt)->GetRenderWidth() ) );
                WindowConfigNode.AppendAttribute("Height").SetValue( StringTools::ConvertToString( (*WinIt)->GetRenderHeight() ) );
                WindowConfigNode.AppendAttribute("Fullscreen").SetValue( StringTools::ConvertToString( (*WinIt)->GetFullscreen() ) );
                WindowConfigNode.AppendAttribute("Hidden").SetValue( StringTools::ConvertToString( (*WinIt)->IsHidden() ) );
                WindowConfigNode.AppendAttribute("Vsync").SetValue( StringTools::ConvertToString( (*WinIt)->VsyncEnabled() ) );
                WindowConfigNode.AppendAttribute("Resizeable").SetValue( StringTools::ConvertToString( (*WinIt)->BorderIsResizeable() ) );
                WindowConfigNode.AppendAttribute("Borderless").SetValue( StringTools::ConvertToString( (*WinIt)->IsBorderless() ) );
                WindowConfigNode.AppendAttribute("FSAA").SetValue( StringTools::ConvertToString( (*WinIt)->GetFSAALevel() ) );
                /// @todo Currently the maximized setting does nothing in the gamewindow.  If it gets implemented, so does this.
                //WindowConfigNode.AppendAttribute("Maximized").SetValue( (*WinIt)-> );//
                for( Graphics::GameWindow::ViewportIterator VPIt = (*WinIt)->BeginViewport() ; VPIt != (*WinIt)->EndViewport() ; ++VPIt )
                {
                    XML::Node ViewportConfigNode = WindowConfigNode.AppendChild("Viewport");
                    ViewportConfigNode.AppendAttribute("ZOrder").SetValue( (*VPIt).second->GetZOrder() );
                    ViewportConfigNode.AppendAttribute("Position").SetValue( StringTools::ConvertToString( Vector2((*VPIt).second->GetLeft(),(*VPIt).second->GetTop()) ) );
                    ViewportConfigNode.AppendAttribute("Size").SetValue( StringTools::ConvertToString( Vector2((*VPIt).second->GetWidth(),(*VPIt).second->GetHeight()) ) );
                }
            }
        }

        void GraphicsManager::ApplySettingGroupImpl(ObjectSettingGroup* Group)
        {
            for( ObjectSettingSetContainer::SubSetIterator SubSetIt = Group->SubSetBegin() ; SubSetIt != Group->SubSetEnd() ; ++SubSetIt )
            {
                String CurrSettingValue;
                if( "RenderSystem" == (*SubSetIt)->GetName() )
                {
                    Graphics::RenderSystem RenderSys = Graphics::RS_OpenGL2;
                    CurrSettingValue = (*SubSetIt)->GetSettingValue("Name");
                    if( GetShortenedRenderSystemName(Graphics::RS_DirectX9) == CurrSettingValue )
                        RenderSys = Graphics::RS_DirectX9;
                    else if( GetShortenedRenderSystemName(Graphics::RS_DirectX11) == CurrSettingValue )
                        RenderSys = Graphics::RS_DirectX11;
                    else if( GetShortenedRenderSystemName(Graphics::RS_OpenGL2) == CurrSettingValue )
                        RenderSys = Graphics::RS_OpenGL2;
                    else if( GetShortenedRenderSystemName(Graphics::RS_OpenGLES1) == CurrSettingValue )
                        RenderSys = Graphics::RS_OpenGLES1;
                    else if( GetShortenedRenderSystemName(Graphics::RS_OpenGLES2) == CurrSettingValue )
                        RenderSys = Graphics::RS_OpenGLES2;

                    this->CurrRenderSys = RenderSys;

                    if( !this->OgreBeenInitialized )
                    {
                        this->SetRenderSystem(this->CurrRenderSys,true);
                    }else{
                        /// @todo May want to make some other data member so that people can accurately get what is set now, instead of what will be set.
                        Entresol::GetSingletonPtr()->Log("WARNING: Attempting to apply new RenderSystem settings after the GraphicsManager has been initialized.  "
                                                      "These Settings will be applied the next time settings are loaded during manager construction if current settings are saved.");
                    }
                }
                else if( "GameWindow" == (*SubSetIt)->GetName() )
                {
                    String WinCaption("Mezzanine Window");
                    Whole WinWidth = 800;
                    Whole WinHeight = 600;
                    Whole WinFlags = 0;

                    // Get the caption.
                    CurrSettingValue = (*SubSetIt)->GetSettingValue("Caption");
                    if(!CurrSettingValue.empty())
                        WinCaption = CurrSettingValue;
                    // Get the width.
                    CurrSettingValue = (*SubSetIt)->GetSettingValue("Width");
                    if(!CurrSettingValue.empty())
                        WinWidth = StringTools::ConvertToUInt32(CurrSettingValue);
                    // Get the height.
                    CurrSettingValue = (*SubSetIt)->GetSettingValue("Height");
                    if(!CurrSettingValue.empty())
                        WinHeight = StringTools::ConvertToUInt32(CurrSettingValue);
                    // Get fullscreen.
                    CurrSettingValue = (*SubSetIt)->GetSettingValue("Fullscreen");
                    if(!CurrSettingValue.empty())
                    {
                        if(StringTools::ConvertToBool(CurrSettingValue))
                            WinFlags = (WinFlags | Graphics::GameWindow::WF_Fullscreen);
                    }
                    // Get hidden.
                    CurrSettingValue = (*SubSetIt)->GetSettingValue("Hidden");
                    if(!CurrSettingValue.empty())
                    {
                        if(StringTools::ConvertToBool(CurrSettingValue))
                            WinFlags = (WinFlags | Graphics::GameWindow::WF_Hidden);
                    }
                    // Get vsync.
                    CurrSettingValue = (*SubSetIt)->GetSettingValue("Vsync");
                    if(!CurrSettingValue.empty())
                    {
                        if(StringTools::ConvertToBool(CurrSettingValue))
                            WinFlags = (WinFlags | Graphics::GameWindow::WF_VsyncEnabled);
                    }
                    // Get resizable.
                    CurrSettingValue = (*SubSetIt)->GetSettingValue("Resizeable");
                    if(!CurrSettingValue.empty())
                    {
                        if(StringTools::ConvertToBool(CurrSettingValue))
                            WinFlags = (WinFlags | Graphics::GameWindow::WF_Resizeable);
                    }
                    // Get maximized.
                    CurrSettingValue = (*SubSetIt)->GetSettingValue("Maximized");
                    if(!CurrSettingValue.empty())
                    {
                        if(StringTools::ConvertToBool(CurrSettingValue))
                            WinFlags = (WinFlags | Graphics::GameWindow::WF_Maximized);
                    }
                    // Get borderless.
                    CurrSettingValue = (*SubSetIt)->GetSettingValue("Borderless");
                    if(!CurrSettingValue.empty())
                    {
                        if(StringTools::ConvertToBool(CurrSettingValue))
                            WinFlags = (WinFlags | Graphics::GameWindow::WF_Borderless);
                    }
                    // Get the FSAA level
                    CurrSettingValue = (*SubSetIt)->GetSettingValue("FSAA");
                    if(!CurrSettingValue.empty())
                    {
                        switch (StringTools::ConvertToUInt32(CurrSettingValue))
                        {
                            case 2:
                                WinFlags = (WinFlags | Graphics::GameWindow::WF_FSAA_2);
                                break;
                            case 4:
                                WinFlags = (WinFlags | Graphics::GameWindow::WF_FSAA_4);
                                break;
                            case 8:
                                WinFlags = (WinFlags | Graphics::GameWindow::WF_FSAA_8);
                                break;
                            case 16:
                                WinFlags = (WinFlags | Graphics::GameWindow::WF_FSAA_16);
                                break;
                        }
                    }
                    // Finally, construct the window.
                    Graphics::GameWindow* CurrWindow = this->CreateGameWindow(WinCaption,WinWidth,WinHeight,WinFlags);
                    // Set up the viewports
                    for( ObjectSettingSetContainer::SubSetIterator VPIt = (*SubSetIt)->SubSetBegin() ; VPIt != (*SubSetIt)->SubSetEnd() ; ++VPIt )
                    {
                        if( "Viewport" == (*VPIt)->GetName() )
                        {
                            Integer ZO = 0;
                            Vector2 Position(1,1);
                            Vector2 Size(0,0);

                            CurrSettingValue = (*VPIt)->GetSettingValue("ZOrder");
                            if(!CurrSettingValue.empty())
                                ZO = StringTools::ConvertToInteger( CurrSettingValue );
                            CurrSettingValue = (*VPIt)->GetSettingValue("Position");
                            if(!CurrSettingValue.empty())
                                Position = StringTools::ConvertToVector2( CurrSettingValue );
                            CurrSettingValue = (*VPIt)->GetSettingValue("Size");
                            if(!CurrSettingValue.empty())
                                Size = StringTools::ConvertToVector2( CurrSettingValue );

                            Graphics::Viewport* CurrViewport = CurrWindow->CreateViewport(NULL,ZO);
                            CurrViewport->SetDimensions(Position.X,Position.Y,Size.X,Size.Y);
                        }// if - Viewport
                    }// for - Viewports
                }// if - RS || GameWindow
            }// for - SubSets
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Window Management

        Graphics::GameWindow* GraphicsManager::CreateGameWindow(const String& WindowCaption, const Whole& Width, const Whole& Height, const Whole& Flags)
        {
            if( !this->OgreBeenInitialized )
                this->InitOgreRenderSystem();

            Graphics::GameWindow* NewWindow = new Graphics::GameWindow(WindowCaption,Width,Height,Flags);
            this->GameWindows.push_back(NewWindow);
            return NewWindow;
        }

        Graphics::GameWindow* GraphicsManager::GetGameWindow(const Whole& Index)
        {
            return this->GameWindows.at(Index);
        }

        Whole GraphicsManager::GetNumGameWindows()
        {
            return this->GameWindows.size();
        }

        void GraphicsManager::DestroyGameWindow(Graphics::GameWindow* ToBeDestroyed)
        {
            for( GameWindowIterator it = this->GameWindows.begin() ; it != this->GameWindows.end() ; it++ )
            {
                if( ToBeDestroyed == (*it) )
                {
                    delete ToBeDestroyed;
                    this->GameWindows.erase(it);
                    return;
                }
            }
        }

        void GraphicsManager::DestroyAllGameWindows(bool ExcludePrimary)
        {
            for( GameWindowIterator Iter = GameWindows.begin() ; Iter != GameWindows.end() ; ++Iter )
                { delete *Iter; }
            this->GameWindows.clear();

            if(!ExcludePrimary)
            {
                delete PrimaryGameWindow;
                this->PrimaryGameWindow = 0;
            }
        }

        Graphics::GameWindow* GraphicsManager::GetPrimaryGameWindow()
        {
            return this->PrimaryGameWindow;
        }

        GraphicsManager::GameWindowIterator GraphicsManager::BeginGameWindow()
        {
            return this->GameWindows.begin();
        }

        GraphicsManager::GameWindowIterator GraphicsManager::EndGameWindow()
        {
            return this->GameWindows.end();
        }

        GraphicsManager::ConstGameWindowIterator GraphicsManager::BeginGameWindow() const
        {
            return this->GameWindows.begin();
        }

        GraphicsManager::ConstGameWindowIterator GraphicsManager::EndGameWindow() const
        {
            return this->GameWindows.end();
        }

        ///////////////////////////////////////////////////////////////////////////////
        // RenderSystem Management

        void GraphicsManager::SetRenderSystem(const Graphics::RenderSystem& RenderSys, bool InitializeRenderSystem)
        {
            if(!this->OgreBeenInitialized) this->CurrRenderSys = RenderSys;
            else { MEZZ_EXCEPTION(Exception::INVALID_STATE_EXCEPTION,"Attempting to set RenderSystem after graphics has been initialized.  This is not supported."); }

            if( InitializeRenderSystem )
                this->InitOgreRenderSystem();
        }

        Graphics::RenderSystem GraphicsManager::GetCurrRenderSystem()
        {
            return this->CurrRenderSys;
        }

        String GraphicsManager::GetRenderSystemName(const Graphics::RenderSystem& RenderSys)
        {
            switch(RenderSys)
            {
                case Graphics::RS_DirectX9: return "Direct3D9 Rendering Subsystem"; break;
                case Graphics::RS_DirectX11: return "Direct3D11 Rendering Subsystem"; break;
                case Graphics::RS_OpenGL2: return "OpenGL Rendering Subsystem"; break;  /// @todo This will likely have to change when other OGL systems are implemented
                //case Graphics::RS_OpenGL3: return ""; break;  Not yet implemented
                //case Graphics::RS_OpenGL4: return ""; break;  Not yet implemented
                case Graphics::RS_OpenGLES1: return "OpenGL ES 1.x Rendering Subsystem"; break;
                case Graphics::RS_OpenGLES2: return "OpenGL ES 2.x Rendering Subsystem"; break;
            }
            return "";
        }

        String GraphicsManager::GetShortenedRenderSystemName(const Graphics::RenderSystem& RenderSys)
        {
            switch(RenderSys)
            {
                case Graphics::RS_DirectX9: return "Direct3D9"; break;
                case Graphics::RS_DirectX11: return "Direct3D11"; break;
                case Graphics::RS_OpenGL2: return "OpenGL"; break;  /// @todo This will likely have to change when other OGL systems are implemented
                //case Graphics::RS_OpenGL3: return ""; break;  Not yet implemented
                //case Graphics::RS_OpenGL4: return ""; break;  Not yet implemented
                case Graphics::RS_OpenGLES1: return "OpenGLES1.x"; break;
                case Graphics::RS_OpenGLES2: return "OpenGLES2.x"; break;
            }
            return "";
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Query Methods

        const StringVector* GraphicsManager::GetSupportedResolutions()
        {
            return &(this->SupportedResolutions);
        }

        const StringVector* GraphicsManager::GetSupportedDevices()
        {
            return &(this->SupportedDevices);
        }

        const WindowSettings& GraphicsManager::GetDesktopSettings()
        {
            return this->DesktopSettings;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Utility Methods

        void GraphicsManager::RenderOneFrame()
        {
            Ogre::Root::getSingleton().renderOneFrame();
            if( !this->GetPrimaryGameWindow()->_GetOgreWindowPointer()->isVisible() )
                Ogre::Root::getSingleton().clearEventTimes();
        }

        void GraphicsManager::SwapAllBuffers(bool WaitForVsync)
        {
            for( Whole X = 0 ; X < this->GetNumGameWindows() ; X++ )
                this->GetGameWindow(X)->_GetOgreWindowPointer()->swapBuffers(false);
        }

        void GraphicsManager::Initialize()
        {
            if( !this->Initialized )
            {
                this->TheEntresol->GetScheduler().AddWorkUnitMonopoly(RenderWork);

                if( !this->OgreBeenInitialized )
                    this->InitOgreRenderSystem();

                Ogre::ConfigOptionMap& CurrentRendererOptions = Ogre::Root::getSingleton().getRenderSystem()->getConfigOptions();
                for( Ogre::ConfigOptionMap::iterator configItr = CurrentRendererOptions.begin() ;
                    configItr != CurrentRendererOptions.end() ; configItr++)
                {
                    if( (configItr)->first == "Video Mode" )
                    {
                        for( Whole X = 0 ; X < (configItr)->second.possibleValues.size() ; X++ )
                        {
                            String NewRes = (configItr)->second.possibleValues[X];
                            StringTools::RemoveDuplicateWhitespaces(NewRes);
                            StringTools::Trim(NewRes);
                            this->SupportedResolutions.push_back(NewRes);
                        }
                        continue;
                    }
                    if( (configItr)->first == "Rendering Device" )
                    {
                        for( Whole Y = 0 ; Y < (configItr)->second.possibleValues.size() ; Y++ )
                            this->SupportedDevices.push_back((configItr)->second.possibleValues[Y]);
                        continue;
                    }
                }

                if(this->AutoGenFiles)
                    this->SaveAllSettings();

                this->Initialized = true;
            }
        }

        void GraphicsManager::Deinitialize()
        {
            if( this->Initialized )
            {
                this->TheEntresol->GetScheduler().RemoveWorkUnitMonopoly( this->RenderWork );

                /// @todo Here is where we should be shutting down the rendersystem, but Ogre in it's poorly coded ways wasn't
                /// designed for that unless you are also deleting Ogre::Root.  Take it up with them if you don't like it.
                /// (tell me where the thread is when you do, I'll join in)
                /// Related: http://www.ogre3d.org/forums/viewtopic.php?f=4&t=77036&sid=a7ce7512032eb851d50da083795198f3

                if(this->AutoGenFiles)
                    this->SaveAllSettings();

                this->Initialized = false;
            }
        }

        RenderWorkUnit* GraphicsManager::GetRenderWork()
            { return this->RenderWork; }

        ///////////////////////////////////////////////////////////////////////////////
        // SubSystem Initialization

        bool GraphicsManager::HasSDLBeenInitialized()
        {
            return SDL_WasInit(SDL_INIT_VIDEO);
        }

        bool GraphicsManager::HasOgreBeenInitialized()
        {
            return this->OgreBeenInitialized;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Type Identifier Methods

        ManagerBase::ManagerType GraphicsManager::GetInterfaceType() const
            { return ManagerBase::MT_GraphicsManager; }

        String GraphicsManager::GetImplementationTypeName() const
            { return "DefaultGraphicsManager"; }

        ///////////////////////////////////////////////////////////////////////////////
        // DefaultGraphicsManagerFactory Methods

        DefaultGraphicsManagerFactory::DefaultGraphicsManagerFactory()
        {
        }

        DefaultGraphicsManagerFactory::~DefaultGraphicsManagerFactory()
        {
        }

        String DefaultGraphicsManagerFactory::GetManagerTypeName() const
        {
            return "DefaultGraphicsManager";
        }

        ManagerBase* DefaultGraphicsManagerFactory::CreateManager(NameValuePairList& Params)
        {
            if(GraphicsManager::SingletonValid())
            {
                /// @todo Add something to log a warning that the manager exists and was requested to be constructed when we have a logging manager set up.
                return GraphicsManager::GetSingletonPtr();
            }else{
                if(Params.empty()) return new GraphicsManager();
                else
                {
                    Whole Width = 800;
                    Whole Height = 600;
                    bool FullScreen = false;
                    for( NameValuePairList::iterator ParIt = Params.begin() ; ParIt != Params.end() ; ++ParIt )
                    {
                        String Lower = (*ParIt).first;
                        StringTools::ToLowerCase(Lower);
                        if( "width" == Lower )
                        {
                            Width = StringTools::ConvertToUInt32( (*ParIt).second );
                        }
                        else if( "height" == Lower )
                        {
                            Height = StringTools::ConvertToUInt32( (*ParIt).second );
                        }
                        else if( "fullscreen" == Lower )
                        {
                            FullScreen = StringTools::ConvertToBool( (*ParIt).second );
                        }
                    }
                    return new GraphicsManager();
                }
            }
        }

        ManagerBase* DefaultGraphicsManagerFactory::CreateManager(XML::Node& XMLNode)
        {
            if(GraphicsManager::SingletonValid())
            {
                /// @todo Add something to log a warning that the manager exists and was requested to be constructed when we have a logging manager set up.
                return GraphicsManager::GetSingletonPtr();
            }else return new GraphicsManager(XMLNode);
        }

        void DefaultGraphicsManagerFactory::DestroyManager(ManagerBase* ToBeDestroyed)
        {
            delete ToBeDestroyed;
        }

    } // Graphics Namespace
} //Mezzanine

#endif
