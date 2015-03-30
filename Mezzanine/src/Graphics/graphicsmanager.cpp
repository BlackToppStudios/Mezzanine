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
#ifndef _graphicsmanager_cpp
#define _graphicsmanager_cpp

#include "Graphics/graphicsmanager.h"
#include "Graphics/cameramanager.h"
#include "Graphics/texturemanager.h"
#include "Graphics/cameraproxy.h"
#include "Graphics/viewport.h"

#include "Resource/resourcemanager.h"

#include "UI/uimanager.h"

#include "eventmanager.h"
#include "crossplatform.h"
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
#include <algorithm>

namespace Mezzanine
{
    template<> Graphics::GraphicsManager* Singleton<Graphics::GraphicsManager>::SingletonPtr = NULL;

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

        //template<> GraphicsManager* Singleton<GraphicsManager>::SingletonPtr = NULL;
        const String GraphicsManager::ImplementationName = "DefaultGraphicsManager";
        const ManagerBase::ManagerType GraphicsManager::InterfaceType = ManagerBase::MT_GraphicsManager;

        GraphicsManager::GraphicsManager() :
            PrimaryGameWindow(NULL),
            RenderWork(NULL),
            ThreadResources(NULL),
            CurrRenderSys(Graphics::RS_OpenGL2),
            OgreBeenInitialized(false)
        {
            this->Construct();
            this->AutoGenFiles = false;
        }

        GraphicsManager::GraphicsManager(XML::Node& XMLNode) :
            PrimaryGameWindow(NULL),
            RenderWork(NULL),
            ThreadResources(NULL),
            CurrRenderSys(Graphics::RS_OpenGL2),
            OgreBeenInitialized(false)
        {
            this->Construct();

            this->ObjectSettingsHandler::ProtoDeSerialize(XMLNode);

            /// @todo This is currently necessary because a render window of some kind needs to exist for the loading
            /// of resources that occurs later in world construction (when this is constructed by the world, which this
            /// assumes.  If possible this should be removed, to keep construction more flexible.
            this->InitOgreRenderSystem();
        }


        GraphicsManager::~GraphicsManager()
        {
            this->Deinitialize();
            this->DestroyAllGameWindows(false);

            //UInt32 InitSDLSystems = SDL_WasInit(0);
            //if( SDL_INIT_VIDEO | InitSDLSystems ) {
            //    SDL_QuitSubSystem(SDL_INIT_VIDEO);
            //}

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
            DesktopSettings.WinRes.Width = DeskMode.w;
            DesktopSettings.WinRes.Height = DeskMode.h;
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
                    //Ogre::RenderSystem* temp = OgreCore->getRenderSystemByName( this->GetRenderSystemName( this->RenderSystemTypes[0] ) );
                    OgreCore->setRenderSystem( OgreCore->getRenderSystemByName( this->GetRenderSystemName( this->RenderSystemTypes[0] ) ) );
                }else{
                    //Ogre::RenderSystem* temp = OgreCore->getRenderSystemByName( this->GetRenderSystemName( this->CurrRenderSys ) );
                    OgreCore->setRenderSystem( OgreCore->getRenderSystemByName( this->GetRenderSystemName( this->CurrRenderSys ) ) );
                }

                OgreCore->initialise(false,"");
                this->OgreBeenInitialized = true;

                this->PrimaryGameWindow = new GameWindow("Primary",1,1,GameWindow::WF_Hidden);
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
                { (*WinIt)->ProtoSerialize(CurrentSettings); }
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
                        Entresol::GetSingletonPtr()->_Log("WARNING: Attempting to apply new RenderSystem settings after the GraphicsManager has been initialized.  "
                                                      "These Settings will be applied the next time settings are loaded during manager construction if current settings are saved.");
                    }
                }
                else if( "GameWindow" == (*SubSetIt)->GetName() )
                {
                    GameWindow* CurrWindow = NULL;
                    String WinCaption("Mezzanine Window");
                    Whole WinWidth = 800;
                    Whole WinHeight = 600;
                    Whole WinFlags = 0;

                    ObjectSettingSet* PropertiesSet = (*SubSetIt)->GetChildObjectSettingSet("GameWindowProperties");
                    if( PropertiesSet != NULL ) {
                        // Get the caption.
                        CurrSettingValue = PropertiesSet->GetSettingValue("Caption");
                        if(!CurrSettingValue.empty())
                            WinCaption = CurrSettingValue;
                        // Get the width.
                        CurrSettingValue = PropertiesSet->GetSettingValue("Width");
                        if(!CurrSettingValue.empty())
                            WinWidth = StringTools::ConvertToUInt32(CurrSettingValue);
                        // Get the height.
                        CurrSettingValue = PropertiesSet->GetSettingValue("Height");
                        if(!CurrSettingValue.empty())
                            WinHeight = StringTools::ConvertToUInt32(CurrSettingValue);
                        // Get fullscreen.
                        CurrSettingValue = PropertiesSet->GetSettingValue("Fullscreen");
                        if(!CurrSettingValue.empty())
                        {
                            if(StringTools::ConvertToBool(CurrSettingValue))
                                WinFlags = (WinFlags | GameWindow::WF_Fullscreen);
                        }
                        // Get hidden.
                        CurrSettingValue = PropertiesSet->GetSettingValue("Hidden");
                        if(!CurrSettingValue.empty())
                        {
                            if(StringTools::ConvertToBool(CurrSettingValue))
                                WinFlags = (WinFlags | GameWindow::WF_Hidden);
                        }
                        // Get vsync.
                        CurrSettingValue = PropertiesSet->GetSettingValue("Vsync");
                        if(!CurrSettingValue.empty())
                        {
                            if(StringTools::ConvertToBool(CurrSettingValue))
                                WinFlags = (WinFlags | GameWindow::WF_VsyncEnabled);
                        }
                        // Get resizable.
                        CurrSettingValue = PropertiesSet->GetSettingValue("Resizeable");
                        if(!CurrSettingValue.empty())
                        {
                            if(StringTools::ConvertToBool(CurrSettingValue))
                                WinFlags = (WinFlags | GameWindow::WF_Resizeable);
                        }
                        // Get maximized.
                        CurrSettingValue = PropertiesSet->GetSettingValue("Maximized");
                        if(!CurrSettingValue.empty())
                        {
                            if(StringTools::ConvertToBool(CurrSettingValue))
                                WinFlags = (WinFlags | GameWindow::WF_Maximized);
                        }
                        // Get borderless.
                        CurrSettingValue = PropertiesSet->GetSettingValue("Borderless");
                        if(!CurrSettingValue.empty())
                        {
                            if(StringTools::ConvertToBool(CurrSettingValue))
                                WinFlags = (WinFlags | GameWindow::WF_Borderless);
                        }
                        // Get the FSAA level
                        CurrSettingValue = PropertiesSet->GetSettingValue("FSAA");
                        if(!CurrSettingValue.empty())
                        {
                            switch( StringTools::ConvertToUInt32(CurrSettingValue) )
                            {
                                case 2:
                                    WinFlags = (WinFlags | GameWindow::WF_FSAA_2);
                                    break;
                                case 4:
                                    WinFlags = (WinFlags | GameWindow::WF_FSAA_4);
                                    break;
                                case 8:
                                    WinFlags = (WinFlags | GameWindow::WF_FSAA_8);
                                    break;
                                case 16:
                                    WinFlags = (WinFlags | GameWindow::WF_FSAA_16);
                                    break;
                            }
                        }
                        // Finally, construct the window.
                        CurrWindow = this->CreateGameWindow(WinCaption,WinWidth,WinHeight,WinFlags);
                    }
                    // Set up the viewports
                    ObjectSettingSet* ViewportsSet = (*SubSetIt)->GetChildObjectSettingSet("Viewports");
                    if( ViewportsSet != NULL && CurrWindow != NULL ) {
                        for( ObjectSettingSetContainer::SubSetIterator VPIt = ViewportsSet->SubSetBegin() ; VPIt != ViewportsSet->SubSetEnd() ; ++VPIt )
                        {
                            if( "Viewport" == (*VPIt)->GetName() ) {
                                Integer ZO = 0;
                                Vector2 Position(0,0);
                                Vector2 Size(1,1);

                                CurrSettingValue = (*VPIt)->GetSettingValue("ZOrder");
                                if( !CurrSettingValue.empty() ) {
                                    ZO = StringTools::ConvertToInteger( CurrSettingValue );
                                }

                                ObjectSettingSet* PositionSet = (*VPIt)->GetChildObjectSettingSet("Position");
                                if( PositionSet != NULL ) {
                                    ObjectSettingSet* PositionVector = PositionSet->GetChildObjectSettingSet("Vector2");

                                    CurrSettingValue = PositionVector->GetSettingValue("X");
                                    if( !CurrSettingValue.empty() ) {
                                        Position.X = StringTools::ConvertToReal( CurrSettingValue );
                                    }
                                    CurrSettingValue = PositionVector->GetSettingValue("Y");
                                    if( !CurrSettingValue.empty() ) {
                                        Position.Y = StringTools::ConvertToReal( CurrSettingValue );
                                    }
                                }

                                ObjectSettingSet* SizeSet = (*VPIt)->GetChildObjectSettingSet("Size");
                                if( SizeSet != NULL ) {
                                    ObjectSettingSet* SizeVector = SizeSet->GetChildObjectSettingSet("Vector2");

                                    CurrSettingValue = SizeVector->GetSettingValue("X");
                                    if( !CurrSettingValue.empty() ) {
                                        Size.X = StringTools::ConvertToReal( CurrSettingValue );
                                    }
                                    CurrSettingValue = SizeVector->GetSettingValue("Y");
                                    if( !CurrSettingValue.empty() ) {
                                        Size.Y = StringTools::ConvertToReal( CurrSettingValue );
                                    }
                                }

                                Viewport* CurrViewport = CurrWindow->CreateViewport(NULL,ZO);
                                CurrViewport->SetDimensions(Position,Size);
                            }// if - Viewport
                        }// for - Viewports
                    }// if - ViewportsSet
                }// if - RS || GameWindow
            }// for - SubSets
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Window Management

        GameWindow* GraphicsManager::CreateGameWindow(const String& WindowCaption, const Whole& Width, const Whole& Height, const Whole& Flags)
        {
            if( !this->OgreBeenInitialized )
                this->InitOgreRenderSystem();

            GameWindow* NewWindow = new GameWindow(WindowCaption,Width,Height,Flags);
            this->GameWindows.push_back(NewWindow);
            return NewWindow;
        }

        GameWindow* GraphicsManager::GetGameWindow(const Whole& Index) const
        {
            return this->GameWindows.at(Index);
        }

        GameWindow* GraphicsManager::GetGameWindow(const String& Caption) const
        {
            for( ConstGameWindowIterator WinIt = this->GameWindows.begin() ; WinIt != this->GameWindows.end() ; ++WinIt )
            {
                if( (*WinIt)->GetWindowCaption() == Caption ) {
                    return (*WinIt);
                }
            }
            return NULL;
        }

        Whole GraphicsManager::GetNumGameWindows() const
        {
            return this->GameWindows.size();
        }

        void GraphicsManager::DestroyGameWindow(GameWindow* ToBeDestroyed)
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

        void GraphicsManager::DestroyAllGameWindows(Boole ExcludePrimary)
        {
            for( GameWindowIterator Iter = this->GameWindows.begin() ; Iter != this->GameWindows.end() ; ++Iter )
                { delete *Iter; }
            this->GameWindows.clear();

            if(!ExcludePrimary)
            {
                delete this->PrimaryGameWindow;
                this->PrimaryGameWindow = NULL;
            }
        }

        GameWindow* GraphicsManager::GetPrimaryGameWindow()
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

        void GraphicsManager::SetRenderSystem(const Graphics::RenderSystem& RenderSys, Boole InitializeRenderSystem)
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
                case Graphics::RS_DirectX9:   return "Direct3D9 Rendering Subsystem";      break;
                case Graphics::RS_DirectX11:  return "Direct3D11 Rendering Subsystem";     break;
                case Graphics::RS_OpenGL2:    return "OpenGL Rendering Subsystem";         break;  /// @todo This will likely have to change when other OGL systems are implemented
                // Case Graphics::RS_OpenGL3:  return "";                                   break;  Not yet implemented
                // Case Graphics::RS_OpenGL4:  return "";                                   break;  Not yet implemented
                case Graphics::RS_OpenGLES1:  return "OpenGL ES 1.x Rendering Subsystem";  break;
                case Graphics::RS_OpenGLES2:  return "OpenGL ES 2.x Rendering Subsystem";  break;
                default:                      return "";                                   break;
            }
            return "";
        }

        String GraphicsManager::GetShortenedRenderSystemName(const Graphics::RenderSystem& RenderSys)
        {
            switch(RenderSys)
            {
                case Graphics::RS_DirectX9:   return "Direct3D9";    break;
                case Graphics::RS_DirectX11:  return "Direct3D11";   break;
                case Graphics::RS_OpenGL2:    return "OpenGL";       break;  /// @todo This will likely have to change when other OGL systems are implemented
                // Case Graphics::RS_OpenGL3:  return "";             break;  Not yet implemented
                // Case Graphics::RS_OpenGL4:  return "";             break;  Not yet implemented
                case Graphics::RS_OpenGLES1:  return "OpenGLES1.x";  break;
                case Graphics::RS_OpenGLES2:  return "OpenGLES2.x";  break;
                default:                      return "";             break;
            }
            return "";
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Query Methods

        const GraphicsManager::ResolutionContainer& GraphicsManager::GetSupportedResolutions() const
        {
            return this->SupportedResolutions;
        }

        const StringVector& GraphicsManager::GetSupportedDevices() const
        {
            return this->SupportedDevices;
        }

        const WindowSettings& GraphicsManager::GetDesktopSettings() const
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

        void GraphicsManager::SwapAllBuffers(Boole WaitForVsync)
        {
            for( Whole X = 0 ; X < this->GetNumGameWindows() ; X++ )
                this->GetGameWindow(X)->_GetOgreWindowPointer()->swapBuffers(false);
        }

        void GraphicsManager::Initialize()
        {
            if( !this->Initialized )
            {
                this->TheEntresol->GetScheduler().AddWorkUnitMonopoly(this->RenderWork, "RenderWork");

                if( !this->OgreBeenInitialized ) {
                    this->InitOgreRenderSystem();
                }

                Ogre::ConfigOptionMap& CurrentRendererOptions = Ogre::Root::getSingleton().getRenderSystem()->getConfigOptions();
                for( Ogre::ConfigOptionMap::iterator configItr = CurrentRendererOptions.begin() ; configItr != CurrentRendererOptions.end() ; configItr++ )
                {
                    if( (configItr)->first == "Video Mode" ) {
                        for( Whole X = 0 ; X < (configItr)->second.possibleValues.size() ; X++ )
                        {
                            String NewRes = (configItr)->second.possibleValues[X];
                            String ResWidth = NewRes.substr(0,NewRes.find_first_of('x'));
                            String ResHeight = NewRes.substr(NewRes.find_first_of('x')+1);
                            StringTools::Trim( ResWidth );
                            StringTools::Trim( ResHeight );
                            this->SupportedResolutions.push_back( Resolution( StringTools::ConvertToWhole( ResWidth ) , StringTools::ConvertToWhole( ResHeight ) ) );
                        }
                        std::sort(this->SupportedResolutions.begin(),this->SupportedResolutions.end());
                    }else if( (configItr)->first == "Rendering Device" ) {
                        for( Whole Y = 0 ; Y < (configItr)->second.possibleValues.size() ; Y++ )
                            { this->SupportedDevices.push_back( (configItr)->second.possibleValues[Y] ); }
                    }
                }

                if( this->AutoGenFiles ) {
                    this->SaveAllSettings();
                }

                this->Initialized = true;
            }
        }

        void GraphicsManager::Deinitialize()
        {
            if( this->Initialized )
            {
                this->TheEntresol->GetScheduler().RemoveWorkUnitMonopoly( this->RenderWork );

                // Textures are loaded into video memory, which we can only do with a valid and initialized rendering context.
                // If the context goes away, such as is the case when the rendersystem is shut down then we can't have textures loaded.
                // So deinitialize the TextureManager.
                this->TheEntresol->GetTextureManager()->Deinitialize();

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

        Boole GraphicsManager::HasSDLBeenInitialized()
        {
            return SDL_WasInit(SDL_INIT_VIDEO);
        }

        Boole GraphicsManager::HasOgreBeenInitialized()
        {
            return this->OgreBeenInitialized;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Type Identifier Methods

        ManagerBase::ManagerType GraphicsManager::GetInterfaceType() const
            { return GraphicsManager::InterfaceType; }

        String GraphicsManager::GetImplementationTypeName() const
            { return GraphicsManager::ImplementationName; }

        ///////////////////////////////////////////////////////////////////////////////
        // DefaultGraphicsManagerFactory Methods

        DefaultGraphicsManagerFactory::DefaultGraphicsManagerFactory()
            {  }

        DefaultGraphicsManagerFactory::~DefaultGraphicsManagerFactory()
            {  }

        String DefaultGraphicsManagerFactory::GetManagerImplName() const
            { return GraphicsManager::ImplementationName; }

        ManagerBase::ManagerType DefaultGraphicsManagerFactory::GetManagerType() const
            { return GraphicsManager::InterfaceType; }

        EntresolManager* DefaultGraphicsManagerFactory::CreateManager(NameValuePairList& Params)
        {
            if( GraphicsManager::SingletonValid() ) {
                /// @todo Add something to log a warning that the manager exists and was requested to be constructed when we have a logging manager set up.
                return GraphicsManager::GetSingletonPtr();
            }else{
                return new GraphicsManager();
            }
        }

        EntresolManager* DefaultGraphicsManagerFactory::CreateManager(XML::Node& XMLNode)
        {
            if( GraphicsManager::SingletonValid() ) {
                /// @todo Add something to log a warning that the manager exists and was requested to be constructed when we have a logging manager set up.
                return GraphicsManager::GetSingletonPtr();
            }else{
                return new GraphicsManager(XMLNode);
            }
        }

        void DefaultGraphicsManagerFactory::DestroyManager(EntresolManager* ToBeDestroyed)
            { delete ToBeDestroyed; }
    } // Graphics Namespace
} //Mezzanine

#endif
