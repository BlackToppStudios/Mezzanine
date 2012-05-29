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
#ifndef _graphicsmanager_cpp
#define _graphicsmanager_cpp

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
#include "viewport.h"
#include "stringtool.h"

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

#ifdef MEZZXML
    GraphicsManager::GraphicsManager(XML::Node& XMLNode)
        : OgreBeenInitialized(false),
          CurrRenderSys(Mezzanine::RS_OpenGL2)
    {
        Construct( 800, 600, false );

        XML::Attribute CurrAttrib;
        for( XML::NodeIterator SecIt = XMLNode.begin() ; SecIt != XMLNode.end() ; ++SecIt )
        {
            String SecName = (*SecIt).Name();
            if( "RenderSystem" == SecName && !OgreBeenInitialized )
            {
                CurrAttrib = (*SecIt).GetAttribute("Name");
                String RenderSystemName = CurrAttrib.AsString();
                if( "Direct3D9" == RenderSystemName )
                    SetRenderSystem(Mezzanine::RS_DirectX9,true);
                else if( "Direct3D11" == RenderSystemName )
                    SetRenderSystem(Mezzanine::RS_DirectX11,true);
                else if( "OpenGL" == RenderSystemName )
                    SetRenderSystem(Mezzanine::RS_OpenGL2,true);
                else if( "OpenGLES1.x" == RenderSystemName )
                    SetRenderSystem(Mezzanine::RS_OpenGLES1,true);
                else if( "OpenGLES2.x" == RenderSystemName )
                    SetRenderSystem(Mezzanine::RS_OpenGLES2,true);
            }
            else if( "GameWindow" == SecName )
            {
                String WinCaption("Mezzanine Window");
                Whole WinWidth = 800;
                Whole WinHeight = 600;
                Whole WinFlags = 0;
                //GameWindow::WindowFlags WinFlags;
                GameWindow::ViewportLayout VPLayout = GameWindow::VL_Custom;

                // Get the caption.
                CurrAttrib = (*SecIt).GetAttribute("Caption");
                if(!CurrAttrib.Empty())
                    WinCaption = CurrAttrib.AsString();
                // Get the width.
                CurrAttrib = (*SecIt).GetAttribute("Width");
                if(!CurrAttrib.Empty())
                    WinWidth = CurrAttrib.AsWhole();
                // Get the height.
                CurrAttrib = (*SecIt).GetAttribute("Height");
                if(!CurrAttrib.Empty())
                    WinHeight = CurrAttrib.AsWhole();
                // Get fullscreen.
                CurrAttrib = (*SecIt).GetAttribute("Fullscreen");
                if(!CurrAttrib.Empty())
                {
                    if(StringTool::ConvertToBool(CurrAttrib.AsString()))
                        WinFlags = (WinFlags | GameWindow::WF_Fullscreen);
                }
                // Get hidden.
                CurrAttrib = (*SecIt).GetAttribute("Hidden");
                if(!CurrAttrib.Empty())
                {
                    if(StringTool::ConvertToBool(CurrAttrib.AsString()))
                        WinFlags = (WinFlags | GameWindow::WF_Hidden);
                }
                // Get vsync.
                CurrAttrib = (*SecIt).GetAttribute("Vsync");
                if(!CurrAttrib.Empty())
                {
                    if(StringTool::ConvertToBool(CurrAttrib.AsString()))
                        WinFlags = (WinFlags | GameWindow::WF_VsyncEnabled);
                }
                // Get resizable.
                CurrAttrib = (*SecIt).GetAttribute("Resizeable");
                if(!CurrAttrib.Empty())
                {
                    if(StringTool::ConvertToBool(CurrAttrib.AsString()))
                        WinFlags = (WinFlags | GameWindow::WF_Resizeable);
                }
                // Get maximized.
                CurrAttrib = (*SecIt).GetAttribute("Maximized");
                if(!CurrAttrib.Empty())
                {
                    if(StringTool::ConvertToBool(CurrAttrib.AsString()))
                        WinFlags = (WinFlags | GameWindow::WF_Maximized);
                }
                // Get borderless.
                CurrAttrib = (*SecIt).GetAttribute("Borderless");
                if(!CurrAttrib.Empty())
                {
                    if(StringTool::ConvertToBool(CurrAttrib.AsString()))
                        WinFlags = (WinFlags | GameWindow::WF_Borderless);
                }
                // Get the FSAA level
                CurrAttrib = (*SecIt).GetAttribute("FSAA");
                if(!CurrAttrib.Empty())
                {
                    switch (CurrAttrib.AsWhole())
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
                // Get the viewport layout
                CurrAttrib = (*SecIt).GetAttribute("ViewportConf");
                if(!CurrAttrib.Empty())
                {
                    String VPConf = CurrAttrib.AsString();
                    if( "1-FullWindow" == VPConf )
                        VPLayout = GameWindow::VL_1_FullWindow;
                    else if( "2-HorizontalSplit" == VPConf )
                        VPLayout = GameWindow::VL_2_HorizontalSplit;
                    else if( "2-VerticalSplit" == VPConf )
                        VPLayout = GameWindow::VL_2_VerticalSplit;
                    else if( "3-TopLarge" == VPConf )
                        VPLayout = GameWindow::VL_3_TopLarge;
                    else if( "4-EvenlySplit" == VPConf )
                        VPLayout = GameWindow::VL_4_EvenlySpaced;
                }
                // Finally, construct the window.
                CreateGameWindow(WinCaption,WinWidth,WinHeight,WinFlags,VPLayout);
            }
        }
    }
#endif

    GraphicsManager::~GraphicsManager()
    {
        DestroyAllGameWindows(false);
    }

    void GraphicsManager::Construct(const Whole& Width, const Whole& Height, const bool& FullScreen )
    {
        DefaultSettings.Fullscreen = FullScreen;
        DefaultSettings.RenderHeight = Height;
        DefaultSettings.RenderWidth = Width;

        SDL_DisplayMode DeskMode;
        SDL_GetDesktopDisplayMode(0,&DeskMode);
        DesktopSettings.RenderWidth = DeskMode.w;
        DesktopSettings.RenderHeight = DeskMode.h;
        DesktopSettings.RefreshRate = DeskMode.refresh_rate;

        this->Priority = 65;
        this->FrameDelay = 0;
    }

    String GraphicsManager::ConvertRenderSystem(const Mezzanine::RenderSystem& RS)
    {
        switch(RS)
        {
            case Mezzanine::RS_DirectX9: return "Direct3D9 Rendering Subsystem"; break;
            case Mezzanine::RS_DirectX11: return "Direct3D11 Rendering Subsystem"; break;
            case Mezzanine::RS_OpenGL2: return "OpenGL Rendering Subsystem"; break;  /// @todo This will likely have to change when other OGL systems are implemented
            //case Mezzanine::RS_OpenGL3: return ""; break;  Not yet implemented
            //case Mezzanine::RS_OpenGL4: return ""; break;  Not yet implemented
            case Mezzanine::RS_OpenGLES1: return "OpenGL ES 1.x Rendering Subsystem"; break;
            case Mezzanine::RS_OpenGLES2: return "OpenGL ES 2.x Rendering Subsystem"; break;
        }
        return "";
    }

    void GraphicsManager::InitOgreRenderSystem()
    {
        if(!OgreBeenInitialized)
        {
            Ogre::Root* OgreCore = Ogre::Root::getSingletonPtr();
            OgreCore->setRenderSystem(OgreCore->getRenderSystemByName(ConvertRenderSystem(CurrRenderSys)));
            OgreCore->initialise(false,"");
            OgreBeenInitialized = true;
        }
    }

    GameWindow* GraphicsManager::CreateGameWindow(const String& WindowCaption, const Whole& Width, const Whole& Height, const Whole& Flags, const GameWindow::ViewportLayout& ViewportConf)
    {
        if(!OgreBeenInitialized) InitOgreRenderSystem();

        GameWindow* NewWindow = new GameWindow(WindowCaption,Width,Height,Flags,ViewportConf);

        if(GameWindows.empty())
        {
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

    void GraphicsManager::SetRenderSystem(const Mezzanine::RenderSystem& RenderSys, bool InitializeRenderSystem)
    {
        if(!OgreBeenInitialized) CurrRenderSys = RenderSys;
        else { MEZZ_EXCEPTION(Exception::INVALID_STATE_EXCEPTION,"Attempting to set RenderSystem after graphics has been initialized.  This is not supported."); }

        if(InitializeRenderSystem)
            InitOgreRenderSystem();
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
            CreateGameWindow("",DefaultSettings.RenderWidth,DefaultSettings.RenderHeight,DefaultSettings.Fullscreen?GameWindow::WF_Fullscreen:0,GameWindow::VL_1_FullWindow);
        this->RenderTimer = new Ogre::Timer();

        Ogre::ConfigOptionMap& CurrentRendererOptions = Ogre::Root::getSingleton().getRenderSystem()->getConfigOptions();
        for( Ogre::ConfigOptionMap::iterator configItr = CurrentRendererOptions.begin() ;
            configItr != CurrentRendererOptions.end() ; configItr++)
        {
            if( (configItr)->first == "Video Mode" )
            {
                for( Whole X = 0 ; X < (configItr)->second.possibleValues.size() ; X++ )
                {
                    String NewRes = (configItr)->second.possibleValues[X];
                    StringTool::RemoveDuplicateWhitespaces(NewRes);
                    StringTool::Trim(NewRes);
                    SupportedResolutions.push_back(NewRes);
                }
                continue;
            }
            if( (configItr)->first == "Rendering Device" )
            {
                for( Whole Y = 0 ; Y < (configItr)->second.possibleValues.size() ; Y++ )
                    SupportedDevices.push_back((configItr)->second.possibleValues[Y]);
                continue;
            }
        }

        Initialized = true;
    }

    void GraphicsManager::DoMainLoopItems()
    {
        Ogre::WindowEventUtilities::messagePump();
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

    bool GraphicsManager::PostMainLoopItems()
    {
        EventManager::GetSingletonPtr()->AddEvent(new EventRenderTime(this->GameWorld->GetFrameTime()));
        return ManagerBase::PostMainLoopItems();
    }

    ManagerBase::ManagerType GraphicsManager::GetInterfaceType() const
        { return ManagerBase::GraphicsManager; }

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
                    StringTool::ToLowerCase(Lower);
                    if( "width" == Lower )
                    {
                        Width = StringTool::ConvertToUInt32( (*ParIt).second );
                    }
                    else if( "height" == Lower )
                    {
                        Height = StringTool::ConvertToUInt32( (*ParIt).second );
                    }
                    else if( "fullscreen" == Lower )
                    {
                        FullScreen = StringTool::ConvertToBool( (*ParIt).second );
                    }
                }
                return new GraphicsManager(Width,Height,FullScreen);
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
}//Mezzanine

#endif
