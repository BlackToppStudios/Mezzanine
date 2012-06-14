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
#include "world.h"

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
        Construct();
    }
#ifdef MEZZXML
    GraphicsManager::GraphicsManager(XML::Node& XMLNode)
        : OgreBeenInitialized(false),
          CurrRenderSys(Mezzanine::RS_OpenGL2)
    {
        Construct();

        XML::Attribute CurrAttrib;
        String PathPreset;
        // Get whether or not to autogen the directory path and settings file.
        XML::Node AutoGenNode = XMLNode.GetChild("AutoCreateSettings");
        if(!AutoGenNode.Empty())
        {
            CurrAttrib = AutoGenNode.GetAttribute("Auto");
            if(!CurrAttrib.Empty())
                AutoGenPath = AutoGenFiles = StringTool::ConvertToBool( CurrAttrib.AsString() );
        }
        // Get preset path to default to if a path is not provided.
        XML::Node PathNode = XMLNode.GetChild("SettingsPath");
        if(!PathNode.Empty())
        {
            CurrAttrib = PathNode.GetAttribute("Path");
            if(!CurrAttrib.Empty())
                PathPreset = CurrAttrib.AsString();

            if(!PathPreset.empty())
                SetSettingsFilePath(PathPreset);
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
                    if(FileGroup.empty()) LoadSettings(FileName);
                    else LoadSettingsFromGroup(FileName,FileGroup);
                }
                else LoadSettings(FileName,FilePath);
            }
        }
        /// @todo This is currently necessary because a render window of some kind needs to exist for the loading
        /// of resources that occurs later in world construction (when this is constructed by the world, which this
        /// assumes.  If possible this should be removed, to keep construction more flexible.
        InitOgreRenderSystem();
    }
#endif

    GraphicsManager::~GraphicsManager()
    {
        DestroyAllGameWindows(false);
    }

    void GraphicsManager::Construct()
    {
        SDL_DisplayMode DeskMode;
        SDL_GetDesktopDisplayMode(0,&DeskMode);
        DesktopSettings.RenderWidth = DeskMode.w;
        DesktopSettings.RenderHeight = DeskMode.h;
        DesktopSettings.RefreshRate = DeskMode.refresh_rate;

        this->Priority = 65;
        this->FrameDelay = 0;
    }

    void GraphicsManager::InitOgreRenderSystem()
    {
        if(!OgreBeenInitialized)
        {
            Ogre::Root* OgreCore = Ogre::Root::getSingletonPtr();
            String RenderSysName = GetRenderSystemName(CurrRenderSys);
            OgreCore->setRenderSystem(OgreCore->getRenderSystemByName(RenderSysName));
            OgreCore->initialise(false,"");
            OgreBeenInitialized = true;

            PrimaryGameWindow = new GameWindow("Primary",1,1,GameWindow::WF_Hidden);
        }
    }

#ifdef MEZZXML
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
        RenderSystemNode.AppendAttribute("Name").SetValue( GetShortenedRenderSystemName(CurrRenderSys) );
        // Create and initialize the window configuration
        for( GameWindowIterator WinIt = GameWindows.begin() ; WinIt != GameWindows.end() ; ++WinIt )
        {
            XML::Node WindowConfigNode = CurrentSettings.AppendChild("GameWindow");
            WindowConfigNode.AppendAttribute("Caption").SetValue( (*WinIt)->GetWindowCaption() );
            WindowConfigNode.AppendAttribute("Width").SetValue( StringTool::ConvertToString( (*WinIt)->GetRenderWidth() ) );
            WindowConfigNode.AppendAttribute("Height").SetValue( StringTool::ConvertToString( (*WinIt)->GetRenderHeight() ) );
            WindowConfigNode.AppendAttribute("Fullscreen").SetValue( StringTool::ConvertToString( (*WinIt)->GetFullscreen() ) );
            WindowConfigNode.AppendAttribute("Hidden").SetValue( StringTool::ConvertToString( (*WinIt)->IsHidden() ) );
            WindowConfigNode.AppendAttribute("Vsync").SetValue( StringTool::ConvertToString( (*WinIt)->VsyncEnabled() ) );
            WindowConfigNode.AppendAttribute("Resizeable").SetValue( StringTool::ConvertToString( (*WinIt)->BorderIsResizeable() ) );
            WindowConfigNode.AppendAttribute("Borderless").SetValue( StringTool::ConvertToString( (*WinIt)->IsBorderless() ) );
            WindowConfigNode.AppendAttribute("FSAA").SetValue( StringTool::ConvertToString( (*WinIt)->GetFSAALevel() ) );
            WindowConfigNode.AppendAttribute("ViewportConf").SetValue( (*WinIt)->GetViewportLayoutName( (*WinIt)->GetViewportLayout() ) );
            /// @todo Currently the maximized setting does nothing in the gamewindow.  If it gets implemented, so does this.
            //WindowConfigNode.AppendAttribute("Maximized").SetValue( (*WinIt)-> );//
        }
    }
#endif
    void GraphicsManager::ApplySettingGroupImpl(ObjectSettingGroup* Group)
    {
        for( ObjectSettingSetContainer::SubSetIterator SubSetIt = Group->SubSetBegin() ; SubSetIt != Group->SubSetEnd() ; ++SubSetIt )
        {
            String CurrSettingValue;
            if( "RenderSystem" == (*SubSetIt)->GetName() )
            {
                Mezzanine::RenderSystem RenderSys = Mezzanine::RS_OpenGL2;
                CurrSettingValue = (*SubSetIt)->GetSettingValue("Name");
                if( "Direct3D9" == CurrSettingValue )
                    RenderSys = Mezzanine::RS_DirectX9;
                else if( "Direct3D11" == CurrSettingValue )
                    RenderSys = Mezzanine::RS_DirectX11;
                else if( "OpenGL" == CurrSettingValue )
                    RenderSys = Mezzanine::RS_OpenGL2;
                else if( "OpenGLES1.x" == CurrSettingValue )
                    RenderSys = Mezzanine::RS_OpenGLES1;
                else if( "OpenGLES2.x" == CurrSettingValue )
                    RenderSys = Mezzanine::RS_OpenGLES2;

                if(!OgreBeenInitialized)
                {
                    SetRenderSystem(CurrRenderSys,true);
                }else{
                    /// @todo May want to make some other data member so that people can accurately get what is set now, instead of what will be set.
                    World::GetWorldPointer()->Log("WARNING: Attempting to apply new RenderSystem settings after the GraphicsManager has been initialized.  "
                                                  "These Settings will be applied the next time settings are loaded during manager construction if current settings are saved.");
                    CurrRenderSys = RenderSys;
                }
            }
            else if( "GameWindow" == (*SubSetIt)->GetName() )
            {
                String WinCaption("Mezzanine Window");
                Whole WinWidth = 800;
                Whole WinHeight = 600;
                Whole WinFlags = 0;
                //GameWindow::WindowFlags WinFlags;
                GameWindow::ViewportLayout VPLayout = GameWindow::VL_Custom;

                // Get the caption.
                CurrSettingValue = (*SubSetIt)->GetSettingValue("Caption");
                if(!CurrSettingValue.empty())
                    WinCaption = CurrSettingValue;
                // Get the width.
                CurrSettingValue = (*SubSetIt)->GetSettingValue("Width");
                if(!CurrSettingValue.empty())
                    WinWidth = StringTool::ConvertToUInt32(CurrSettingValue);
                // Get the height.
                CurrSettingValue = (*SubSetIt)->GetSettingValue("Height");
                if(!CurrSettingValue.empty())
                    WinHeight = StringTool::ConvertToUInt32(CurrSettingValue);
                // Get fullscreen.
                CurrSettingValue = (*SubSetIt)->GetSettingValue("Fullscreen");
                if(!CurrSettingValue.empty())
                {
                    if(StringTool::ConvertToBool(CurrSettingValue))
                        WinFlags = (WinFlags | GameWindow::WF_Fullscreen);
                }
                // Get hidden.
                CurrSettingValue = (*SubSetIt)->GetSettingValue("Hidden");
                if(!CurrSettingValue.empty())
                {
                    if(StringTool::ConvertToBool(CurrSettingValue))
                        WinFlags = (WinFlags | GameWindow::WF_Hidden);
                }
                // Get vsync.
                CurrSettingValue = (*SubSetIt)->GetSettingValue("Vsync");
                if(!CurrSettingValue.empty())
                {
                    if(StringTool::ConvertToBool(CurrSettingValue))
                        WinFlags = (WinFlags | GameWindow::WF_VsyncEnabled);
                }
                // Get resizable.
                CurrSettingValue = (*SubSetIt)->GetSettingValue("Resizeable");
                if(!CurrSettingValue.empty())
                {
                    if(StringTool::ConvertToBool(CurrSettingValue))
                        WinFlags = (WinFlags | GameWindow::WF_Resizeable);
                }
                // Get maximized.
                CurrSettingValue = (*SubSetIt)->GetSettingValue("Maximized");
                if(!CurrSettingValue.empty())
                {
                    if(StringTool::ConvertToBool(CurrSettingValue))
                        WinFlags = (WinFlags | GameWindow::WF_Maximized);
                }
                // Get borderless.
                CurrSettingValue = (*SubSetIt)->GetSettingValue("Borderless");
                if(!CurrSettingValue.empty())
                {
                    if(StringTool::ConvertToBool(CurrSettingValue))
                        WinFlags = (WinFlags | GameWindow::WF_Borderless);
                }
                // Get the FSAA level
                CurrSettingValue = (*SubSetIt)->GetSettingValue("FSAA");
                if(!CurrSettingValue.empty())
                {
                    switch (StringTool::ConvertToUInt32(CurrSettingValue))
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
                CurrSettingValue = (*SubSetIt)->GetSettingValue("ViewportConf");
                if(!CurrSettingValue.empty())
                {
                    if( "1-FullWindow" == CurrSettingValue )
                        VPLayout = GameWindow::VL_1_FullWindow;
                    else if( "2-HorizontalSplit" == CurrSettingValue )
                        VPLayout = GameWindow::VL_2_HorizontalSplit;
                    else if( "2-VerticalSplit" == CurrSettingValue )
                        VPLayout = GameWindow::VL_2_VerticalSplit;
                    else if( "3-TopLarge" == CurrSettingValue )
                        VPLayout = GameWindow::VL_3_TopLarge;
                    else if( "4-EvenlySplit" == CurrSettingValue )
                        VPLayout = GameWindow::VL_4_EvenlySpaced;
                }
                // Finally, construct the window.
                CreateGameWindow(WinCaption,WinWidth,WinHeight,WinFlags,VPLayout);
            }
        }
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Window Management

    GameWindow* GraphicsManager::CreateGameWindow(const String& WindowCaption, const Whole& Width, const Whole& Height, const Whole& Flags, const GameWindow::ViewportLayout& ViewportConf)
    {
        if(!OgreBeenInitialized) InitOgreRenderSystem();

        GameWindow* NewWindow = new GameWindow(WindowCaption,Width,Height,Flags,ViewportConf);
        GameWindows.push_back(NewWindow);
        return NewWindow;
    }

    GameWindow* GraphicsManager::GetGameWindow(const Whole& Index)
    {
        return GameWindows.at(Index);
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

    ///////////////////////////////////////////////////////////////////////////////
    // RenderSystem Management

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

    String GraphicsManager::GetRenderSystemName(const Mezzanine::RenderSystem& RenderSys)
    {
        switch(RenderSys)
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

    String GraphicsManager::GetShortenedRenderSystemName(const Mezzanine::RenderSystem& RenderSys)
    {
        switch(RenderSys)
        {
            case Mezzanine::RS_DirectX9: return "Direct3D9"; break;
            case Mezzanine::RS_DirectX11: return "Direct3D11"; break;
            case Mezzanine::RS_OpenGL2: return "OpenGL"; break;  /// @todo This will likely have to change when other OGL systems are implemented
            //case Mezzanine::RS_OpenGL3: return ""; break;  Not yet implemented
            //case Mezzanine::RS_OpenGL4: return ""; break;  Not yet implemented
            case Mezzanine::RS_OpenGLES1: return "OpenGLES1.x"; break;
            case Mezzanine::RS_OpenGLES2: return "OpenGLES2.x"; break;
        }
        return "";
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Query Methods

    const StringVector* GraphicsManager::GetSupportedResolutions()
    {
        return &SupportedResolutions;
    }

    const StringVector* GraphicsManager::GetSupportedDevices()
    {
        return &SupportedDevices;
    }

    const GraphicsSettings& GraphicsManager::GetDesktopSettings()
    {
        return DesktopSettings;
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Utility Methods

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
        if( !GetPrimaryGameWindow()->_GetOgreWindowPointer()->isVisible() )
            Ogre::Root::getSingleton().clearEventTimes();
        #ifdef MEZZDEBUG
        TheWorld->Log("Finished Rendering");
        #endif
    }

    void GraphicsManager::SwapAllBuffers(bool WaitForVsync)
    {
        for( Whole X = 0 ; X < GetNumGameWindows() ; X++ )
            GetGameWindow(X)->_GetOgreWindowPointer()->swapBuffers(false);
    }

    ///////////////////////////////////////////////////////////////////////////////
    // SubSystem Initialization

    bool GraphicsManager::HasSDLBeenInitialized()
    {
        return SDL_WasInit(SDL_INIT_VIDEO);
    }

    bool GraphicsManager::HasOgreBeenInitialized()
    {
        return OgreBeenInitialized;
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Inherited from ManagerBase

    void GraphicsManager::Initialize()
    {
        if(!OgreBeenInitialized)
            InitOgreRenderSystem();
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

        if(AutoGenFiles)
            SaveAllSettings();

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
}//Mezzanine

#endif
