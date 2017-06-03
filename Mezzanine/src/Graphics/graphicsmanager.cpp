// © Copyright 2010 - 2017 BlackTopp Studios Inc.
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
#include "Graphics/texturemanager.h"
#include "Graphics/cameraproxy.h"
#include "Graphics/viewport.h"

#include "Resource/resourcemanager.h"

#include "UI/uimanager.h"

#include "crossplatform.h"
#include "stringtool.h"
#include "entresol.h"

#include <SDL.h>

#include <OgreRoot.h>
#include <OgreRenderSystem.h>
#include <OgreRenderSystemCapabilities.h>
#include <OgreRenderWindow.h>
#include <OgreWindowEventUtilities.h>

#ifdef MEZZ_BUILD_DIRECTX9_SUPPORT
#include "d3d9rendersyshelper.h.cpp"
#endif
#ifdef MEZZ_BUILD_DIRECTX11_SUPPORT
#include "d3d11rendersyshelper.h.cpp"
#endif
#ifdef MEZZ_BUILD_OPENGL_SUPPORT
#include "oglrendersyshelper.h.cpp"
#endif
#ifdef MEZZ_BUILD_OPENGL3PLUS_SUPPORT
#include "ogl3plusrendersyshelper.h.cpp"
#endif
#ifdef MEZZ_BUILD_OPENGLES_SUPPORT
#include "oglesrendersyshelper.h.cpp"
#endif
#ifdef MEZZ_BUILD_OPENGLES2_SUPPORT
#include "ogles2rendersyshelper.h.cpp"
#endif

#include <cstdlib>
#include <algorithm>

namespace
{
    using namespace Mezzanine;
    using namespace Mezzanine::Graphics;

    /// @brief Handles the "Shown" Window event from internal sources.
    /// @param EventWindow A pointer to the Window raising the event.
    /// @param WindowID The unique ID belonging to the Window raising the event.
    void HandleWindowEventShown(GameWindow* EventWindow, const UInt32 WindowID)
    {
        WindowEventPtr WinEv = std::make_shared<WindowEvent>(GameWindow::EventWindowShown,WindowID);
        EventWindow->DispatchEvent(WinEv);
    }

    /// @brief Handles the "Hidden" Window event from internal sources.
    /// @param EventWindow A pointer to the Window raising the event.
    /// @param WindowID The unique ID belonging to the Window raising the event.
    void HandleWindowEventHidden(GameWindow* EventWindow, const UInt32 WindowID)
    {
        WindowEventPtr WinEv = std::make_shared<WindowEvent>(GameWindow::EventWindowHidden,WindowID);
        EventWindow->DispatchEvent(WinEv);
    }

    /// @brief Handles the "Exposed" Window event from internal sources.
    /// @param EventWindow A pointer to the Window raising the event.
    /// @param WindowID The unique ID belonging to the Window raising the event.
    void HandleWindowEventExposed(GameWindow* EventWindow, const UInt32 WindowID)
    {
        WindowEventPtr WinEv = std::make_shared<WindowEvent>(GameWindow::EventWindowExposed,WindowID);
        EventWindow->DispatchEvent(WinEv);
    }

    /// @brief Handles the "Moved" Window event from internal sources.
    /// @param InternalEvent The internal Window event to handle.
    /// @param EventWindow A pointer to the Window raising the event.
    /// @param WindowID The unique ID belonging to the Window raising the event.
    void HandleWindowEventMoved(const SDL_WindowEvent& InternalEvent, GameWindow* EventWindow, const UInt32 WindowID)
    {
        Integer X = InternalEvent.data1;
        Integer Y = InternalEvent.data2;
        WindowTransformEventPtr WinEv = std::make_shared<WindowTransformEvent>(GameWindow::EventWindowMoved,WindowID,X,Y);
        EventWindow->DispatchEvent(WinEv);
    }

    /// @brief Handles the "Resizing" Window event from internal sources.
    /// @param InternalEvent The internal Window event to handle.
    /// @param EventWindow A pointer to the Window raising the event.
    /// @param WindowID The unique ID belonging to the Window raising the event.
    void HandleWindowEventResizing(const SDL_WindowEvent& InternalEvent, GameWindow* EventWindow, const UInt32 WindowID)
    {
        Integer X = InternalEvent.data1;
        Integer Y = InternalEvent.data2;
        WindowTransformEventPtr WinEv = std::make_shared<WindowTransformEvent>(GameWindow::EventWindowResizing,WindowID,X,Y);
        EventWindow->DispatchEvent(WinEv);
    }

    /// @brief Handles the "Resized" Window event from internal sources.
    /// @param InternalEvent The internal Window event to handle.
    /// @param EventWindow A pointer to the Window raising the event.
    /// @param WindowID The unique ID belonging to the Window raising the event.
    void HandleWindowEventResized(const SDL_WindowEvent& InternalEvent, GameWindow* EventWindow, const UInt32 WindowID)
    {
        Integer X = InternalEvent.data1;
        Integer Y = InternalEvent.data2;
        WindowTransformEventPtr WinEv = std::make_shared<WindowTransformEvent>(GameWindow::EventWindowResized,WindowID,X,Y);
        EventWindow->DispatchEvent(WinEv);
    }

    /// @brief Handles the "Minimized" Window event from internal sources.
    /// @param EventWindow A pointer to the Window raising the event.
    /// @param WindowID The unique ID belonging to the Window raising the event.
    void HandleWindowEventMinimized(GameWindow* EventWindow, const UInt32 WindowID)
    {
        WindowEventPtr WinEv = std::make_shared<WindowEvent>(GameWindow::EventWindowMinimized,WindowID);
        EventWindow->DispatchEvent(WinEv);
    }

    /// @brief Handles the "Maximized" Window event from internal sources.
    /// @param EventWindow A pointer to the Window raising the event.
    /// @param WindowID The unique ID belonging to the Window raising the event.
    void HandleWindowEventMaximized(GameWindow* EventWindow, const UInt32 WindowID)
    {
        WindowEventPtr WinEv = std::make_shared<WindowEvent>(GameWindow::EventWindowMaximized,WindowID);
        EventWindow->DispatchEvent(WinEv);
    }

    /// @brief Handles the "Restored" Window event from internal sources.
    /// @param EventWindow A pointer to the Window raising the event.
    /// @param WindowID The unique ID belonging to the Window raising the event.
    void HandleWindowEventRestored(GameWindow* EventWindow, const UInt32 WindowID)
    {
        WindowEventPtr WinEv = std::make_shared<WindowEvent>(GameWindow::EventWindowRestored,WindowID);
        EventWindow->DispatchEvent(WinEv);
    }

    /// @brief Handles the "Enter" Window event from internal sources.
    /// @param EventWindow A pointer to the Window raising the event.
    /// @param WindowID The unique ID belonging to the Window raising the event.
    void HandleWindowEventEnter(GameWindow* EventWindow, const UInt32 WindowID)
    {
        WindowEventPtr WinEv = std::make_shared<WindowEvent>(GameWindow::EventWindowEnter,WindowID);
        EventWindow->DispatchEvent(WinEv);
    }

    /// @brief Handles the "Leave" Window event from internal sources.
    /// @param EventWindow A pointer to the Window raising the event.
    /// @param WindowID The unique ID belonging to the Window raising the event.
    void HandleWindowEventLeave(GameWindow* EventWindow, const UInt32 WindowID)
    {
        WindowEventPtr WinEv = std::make_shared<WindowEvent>(GameWindow::EventWindowLeave,WindowID);
        EventWindow->DispatchEvent(WinEv);
    }

    /// @brief Handles the "Focus Gained" Window event from internal sources.
    /// @param EventWindow A pointer to the Window raising the event.
    /// @param WindowID The unique ID belonging to the Window raising the event.
    void HandleWindowEventFocusGained(GameWindow* EventWindow, const UInt32 WindowID)
    {
        WindowEventPtr WinEv = std::make_shared<WindowEvent>(GameWindow::EventWindowFocusGained,WindowID);
        EventWindow->DispatchEvent(WinEv);
    }

    /// @brief Handles the "Focus Lost" Window event from internal sources.
    /// @param EventWindow A pointer to the Window raising the event.
    /// @param WindowID The unique ID belonging to the Window raising the event.
    void HandleWindowEventFocusLost(GameWindow* EventWindow, const UInt32 WindowID)
    {
        WindowEventPtr WinEv = std::make_shared<WindowEvent>(GameWindow::EventWindowFocusLost,WindowID);
        EventWindow->DispatchEvent(WinEv);
    }
}

namespace Mezzanine
{
    template<> Graphics::GraphicsManager* Singleton<Graphics::GraphicsManager>::SingletonPtr = nullptr;

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
            this->TargetManager->ThreadResources = nullptr;                  // Take it all away.
        }

        ///////////////////////////////////////////////////////////////////////////
        // GraphicsManager Methods

        //template<> GraphicsManager* Singleton<GraphicsManager>::SingletonPtr = nullptr;
        const String GraphicsManager::ImplementationName = "DefaultGraphicsManager";
        const ManagerBase::ManagerType GraphicsManager::InterfaceType = ManagerBase::MT_GraphicsManager;

        const EventNameType GraphicsManager::EventWindowCreated = "WindowCreated";
        const EventNameType GraphicsManager::EventWindowDestroyed = "WindowDestroyed";

        GraphicsManager::GraphicsManager() :
            PrimaryWindow(nullptr),
            RenderWork(nullptr),
            ThreadResources(nullptr),
            CurrRenderSys(Graphics::RS_OpenGL2),
            OgreBeenInitialized(false)
        {
            this->Construct();
            this->AutoGenFiles = false;
        }

        GraphicsManager::GraphicsManager(const XML::Node& XMLNode) :
            PrimaryWindow(nullptr),
            RenderWork(nullptr),
            ThreadResources(nullptr),
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

        #ifdef MEZZ_BUILD_DIRECTX9_SUPPORT
            DestroyD3D9RenderSystem();
        #endif
        #ifdef MEZZ_BUILD_DIRECTX11_SUPPORT
            DestroyD3D11RenderSystem();
        #endif
        #ifdef MEZZ_BUILD_OPENGL_SUPPORT
            DestroyGLRenderSystem();
        #endif
        #ifdef MEZZ_BUILD_OPENGL3PLUS_SUPPORT
            DestroyGL3PlusRenderSystem();
        #endif
        #ifdef MEZZ_BUILD_OPENGLES_SUPPORT
            DestroyGLESRenderSystem();
        #endif
        #ifdef MEZZ_BUILD_OPENGLES2_SUPPORT
            DestroyGLES2RenderSystem();
        #endif

            //UInt32 InitSDLSystems = SDL_WasInit(0);
            //if( SDL_INIT_VIDEO | InitSDLSystems ) {
            //    SDL_QuitSubSystem(SDL_INIT_VIDEO);
            //}

            delete RenderWork;
        }

        void GraphicsManager::Construct()
        {
            this->PrimaryWindow = nullptr;
            this->RenderWork = new RenderWorkUnit(this);

            UInt32 InitSDLSystems = SDL_WasInit(0);
            if( (SDL_INIT_VIDEO & InitSDLSystems) == 0 ) {
                if( SDL_Init(SDL_INIT_VIDEO | SDL_INIT_NOPARACHUTE) < 0 ) {
                    MEZZ_EXCEPTION(ExceptionBase::INTERNAL_EXCEPTION,String("Failed to Initialize SDL for Video/Windowing, SDL Error: ") + SDL_GetError());
                }
            }

            this->GraphicsPublisher.AddSubscriptionTable(EventWindowCreated);
            this->GraphicsPublisher.AddSubscriptionTable(EventWindowDestroyed);

            // Ogre handles this stuff for us
            SDL_EventState(SDL_RENDER_TARGETS_RESET,SDL_DISABLE);
            SDL_EventState(SDL_RENDER_DEVICE_RESET,SDL_DISABLE);

            SDL_DisplayMode DeskMode;
            SDL_GetDesktopDisplayMode(0,&DeskMode);
            DesktopSettings.WinRes.Width = DeskMode.w;
            DesktopSettings.WinRes.Height = DeskMode.h;
            DesktopSettings.RefreshRate = DeskMode.refresh_rate;

        #ifdef MEZZ_BUILD_DIRECTX9_SUPPORT
            CreateD3D9RenderSystem();
        #endif
        #ifdef MEZZ_BUILD_DIRECTX11_SUPPORT
            CreateD3D11RenderSystem();
        #endif
        #ifdef MEZZ_BUILD_OPENGL_SUPPORT
            CreateGLRenderSystem();
        #endif
        #ifdef MEZZ_BUILD_OPENGL3PLUS_SUPPORT
            CreateGL3PlusRenderSystem();
        #endif
        #ifdef MEZZ_BUILD_OPENGLES_SUPPORT
            CreateGLESRenderSystem();
        #endif
        #ifdef MEZZ_BUILD_OPENGLES2_SUPPORT
            CreateGLES2RenderSystem();
        #endif
        }

        void GraphicsManager::PumpInternalEvents()
        {
            /* A listing of all the SDL_Events we care about in the Graphics System.
            // A * next to the Event denotes working support for that event does not yet exist.
            // A - next to the Event denotes that the event is disabled or ignored by Mezzanine.
            // Window events
            SDL_WINDOWEVENT    = 0x200, // Window state change
            SDL_SYSWMEVENT,             // System specific event
            // Render events
            -SDL_RENDER_TARGETS_RESET = 0x2000, // The render targets have been reset and their contents need to be updated
            -SDL_RENDER_DEVICE_RESET,           // The device has been reset and all textures need to be recreated
            // This is a listing similar to the one above, but are Window specific sub-events.
            -SDL_WINDOWEVENT_NONE,         // Never used
            SDL_WINDOWEVENT_SHOWN,         // Window has been shown
            SDL_WINDOWEVENT_HIDDEN,        // Window has been hidden
            SDL_WINDOWEVENT_EXPOSED,       // Window has been exposed and should be redrawn
            SDL_WINDOWEVENT_MOVED,         // Window has been moved to data1, data2
            SDL_WINDOWEVENT_RESIZED,       // Window has been resized to data1xdata2
            SDL_WINDOWEVENT_SIZE_CHANGED,  // The window size has changed, either as a result of an API call or through the system or user changing the window size.
            SDL_WINDOWEVENT_MINIMIZED,     // Window has been minimized
            SDL_WINDOWEVENT_MAXIMIZED,     // Window has been maximized
            SDL_WINDOWEVENT_RESTORED,      // Window has been restored to normal size and position
            SDL_WINDOWEVENT_ENTER,         // Window has gained mouse focus
            SDL_WINDOWEVENT_LEAVE,         // Window has lost mouse focus
            SDL_WINDOWEVENT_FOCUS_GAINED,  // Window has gained keyboard focus
            SDL_WINDOWEVENT_FOCUS_LOST,    // Window has lost keyboard focus
            SDL_WINDOWEVENT_CLOSE,         // The window manager requests that the window be closed
            -SDL_WINDOWEVENT_TAKE_FOCUS,   // Window is being offered a focus (should SetWindowInputFocus() on itself or a subwindow, or ignore)
            -SDL_WINDOWEVENT_HIT_TEST      // Window had a hit test that wasn't SDL_HITTEST_NORMAL.
            */

            // Make a fixed size array and grab our events.
            // Internally the event queue is capped at 128 events.  16 seems sane for this, but may need modifying.
            // SDL_KEYDOWN to SDL_MULTIGESTURE are all input events.  Everything in between those values, as well as those values, will be pulled.
            std::array<SDL_Event,16> InternalEvents;
            Integer NumEvents = SDL_PeepEvents(InternalEvents.data(),InternalEvents.size(),SDL_GETEVENT,SDL_WINDOWEVENT,SDL_SYSWMEVENT);
            assert( NumEvents >= 0 && SDL_GetError() );

            for( Integer CurrEv = 0 ; CurrEv < NumEvents ; ++CurrEv )
            {
                UInt32 EventType = InternalEvents[CurrEv].type;
                if( EventType != SDL_WINDOWEVENT ) {
                    continue;
                }

                SDL_WindowEvent InternalEvent = InternalEvents[CurrEv].window;

                UInt32 WindowID = InternalEvent.windowID;
                GameWindow* EventWindow = this->GetGameWindowByID( WindowID );
                if( EventWindow == nullptr ) {
                    continue;
                }

                switch( InternalEvent.event )
                {
                    case SDL_WINDOWEVENT_SHOWN:         HandleWindowEventShown(EventWindow,WindowID);       break;
                    case SDL_WINDOWEVENT_HIDDEN:        HandleWindowEventHidden(EventWindow,WindowID);      break;
                    case SDL_WINDOWEVENT_EXPOSED:       HandleWindowEventExposed(EventWindow,WindowID);     break;
                    case SDL_WINDOWEVENT_MOVED:         HandleWindowEventMoved(InternalEvent,EventWindow,WindowID);    break;
                    case SDL_WINDOWEVENT_RESIZED:       HandleWindowEventResizing(InternalEvent,EventWindow,WindowID); break;
                    case SDL_WINDOWEVENT_SIZE_CHANGED:  HandleWindowEventResized(InternalEvent,EventWindow,WindowID);  break;
                    case SDL_WINDOWEVENT_MINIMIZED:     HandleWindowEventMinimized(EventWindow,WindowID);   break;
                    case SDL_WINDOWEVENT_MAXIMIZED:     HandleWindowEventMaximized(EventWindow,WindowID);   break;
                    case SDL_WINDOWEVENT_RESTORED:      HandleWindowEventRestored(EventWindow,WindowID);    break;
                    case SDL_WINDOWEVENT_ENTER:         HandleWindowEventEnter(EventWindow,WindowID);       break;
                    case SDL_WINDOWEVENT_LEAVE:         HandleWindowEventLeave(EventWindow,WindowID);       break;
                    case SDL_WINDOWEVENT_FOCUS_GAINED:  HandleWindowEventFocusGained(EventWindow,WindowID); break;
                    case SDL_WINDOWEVENT_FOCUS_LOST:    HandleWindowEventFocusLost(EventWindow,WindowID);   break;
                    case SDL_WINDOWEVENT_CLOSE:         this->DestroyGameWindow(EventWindow);               break;
                    default:                            break; // Ignore the event.
                }// switch window event type

                // We don't care about events other than Window Events.
                // We could add another if or throw an error here if necessary in the future.
            }// for each event
        }

        void GraphicsManager::InitOgreRenderSystem()
        {
            if( !this->OgreBeenInitialized ) {
                Ogre::Root* OgreCore = Ogre::Root::getSingletonPtr();
                const Ogre::RenderSystemList& RSList = OgreCore->getAvailableRenderers();
                if( RSList.size() == 1 ) {
                    //Ogre::RenderSystem* temp = OgreCore->getRenderSystemByName( this->GetRenderSystemName( this->RenderSystemTypes[0] ) );
                    OgreCore->setRenderSystem( RSList[0] );
                }else{
                    //Ogre::RenderSystem* temp = OgreCore->getRenderSystemByName( this->GetRenderSystemName( this->CurrRenderSys ) );
                    OgreCore->setRenderSystem( OgreCore->getRenderSystemByName( this->GetRenderSystemName( this->CurrRenderSys ) ) );
                }

                OgreCore->initialise(false,"");
                this->OgreBeenInitialized = true;

                this->PrimaryWindow = new GameWindow("Primary",1,1,GameWindow::WF_Hidden);
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
                if( "RenderSystem" == (*SubSetIt)->GetName() ) {
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

                    if( !this->OgreBeenInitialized ) {
                        this->SetRenderSystem(this->CurrRenderSys,true);
                    }else{
                        /// @todo May want to make some other data member so that people can accurately get what is set now, instead of what will be set.
                        Entresol::GetSingletonPtr()->_Log("WARNING: Attempting to apply new RenderSystem settings after the GraphicsManager has been initialized.  "
                                                      "These Settings will be applied the next time settings are loaded during manager construction if current settings are saved.");
                    }
                }
                else if( "GameWindow" == (*SubSetIt)->GetName() )
                {
                    GameWindow* CurrWindow = nullptr;
                    String WinCaption("Mezzanine Window");
                    Whole WinWidth = 800;
                    Whole WinHeight = 600;
                    Whole WinFlags = 0;

                    ObjectSettingSet* PropertiesSet = (*SubSetIt)->GetChildObjectSettingSet("GameWindowProperties");
                    if( PropertiesSet != nullptr ) {
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
                        if(!CurrSettingValue.empty()) {
                            if(StringTools::ConvertToBool(CurrSettingValue))
                                WinFlags = (WinFlags | GameWindow::WF_Fullscreen);
                        }
                        // Get hidden.
                        CurrSettingValue = PropertiesSet->GetSettingValue("Hidden");
                        if(!CurrSettingValue.empty()) {
                            if(StringTools::ConvertToBool(CurrSettingValue))
                                WinFlags = (WinFlags | GameWindow::WF_Hidden);
                        }
                        // Get vsync.
                        CurrSettingValue = PropertiesSet->GetSettingValue("Vsync");
                        if(!CurrSettingValue.empty()) {
                            if(StringTools::ConvertToBool(CurrSettingValue))
                                WinFlags = (WinFlags | GameWindow::WF_VsyncEnabled);
                        }
                        // Get resizable.
                        CurrSettingValue = PropertiesSet->GetSettingValue("Resizeable");
                        if(!CurrSettingValue.empty()) {
                            if(StringTools::ConvertToBool(CurrSettingValue))
                                WinFlags = (WinFlags | GameWindow::WF_Resizeable);
                        }
                        // Get maximized.
                        CurrSettingValue = PropertiesSet->GetSettingValue("Maximized");
                        if(!CurrSettingValue.empty()) {
                            if(StringTools::ConvertToBool(CurrSettingValue))
                                WinFlags = (WinFlags | GameWindow::WF_Maximized);
                        }
                        // Get borderless.
                        CurrSettingValue = PropertiesSet->GetSettingValue("Borderless");
                        if(!CurrSettingValue.empty()) {
                            if(StringTools::ConvertToBool(CurrSettingValue))
                                WinFlags = (WinFlags | GameWindow::WF_Borderless);
                        }
                        // Get the FSAA level
                        CurrSettingValue = PropertiesSet->GetSettingValue("FSAA");
                        if(!CurrSettingValue.empty()) {
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
                    if( ViewportsSet != nullptr && CurrWindow != nullptr ) {
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
                                if( PositionSet != nullptr ) {
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
                                if( SizeSet != nullptr ) {
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

                                Viewport* CurrViewport = CurrWindow->CreateViewport(nullptr,ZO);
                                CurrViewport->SetDimensions(Position,Size);
                            }// if - Viewport
                        }// for - Viewports
                    }// if - ViewportsSet
                }// if - RS || GameWindow
            }// for - SubSets
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Window Management

        GameWindow* GraphicsManager::CreateGameWindow(const String& WindowCaption, const Whole Width, const Whole Height, const Whole Flags)
        {
            if( !this->OgreBeenInitialized ) {
                this->InitOgreRenderSystem();
            }

            GameWindow* NewWindow = new GameWindow(WindowCaption,Width,Height,Flags);
            this->GameWindows.push_back(NewWindow);

            WindowEventPtr WinEv = std::make_shared<WindowEvent>(GraphicsManager::EventWindowCreated,NewWindow->GetID());
            this->GraphicsPublisher.DispatchEvent(WinEv);

            return NewWindow;
        }

        GameWindow* GraphicsManager::GetGameWindow(const Whole Index) const
        {
            return this->GameWindows.at(Index);
        }

        GameWindow* GraphicsManager::GetGameWindowByID(const UInt32 WinID) const
        {
            for( GameWindow* CurrWin : this->GameWindows )
            {
                if( CurrWin->GetID() == WinID ) {
                    return CurrWin;
                }
            }
            return nullptr;
        }

        GameWindow* GraphicsManager::GetGameWindowByCaption(const String& Caption) const
        {
            for( GameWindow* CurrWin : this->GameWindows )
            {
                if( CurrWin->GetCaption() == Caption ) {
                    return CurrWin;
                }
            }
            return nullptr;
        }

        Whole GraphicsManager::GetNumGameWindows() const
        {
            return this->GameWindows.size();
        }

        void GraphicsManager::DestroyGameWindow(GameWindow* ToBeDestroyed)
        {
            for( GameWindowIterator WinIt = this->GameWindows.begin() ; WinIt != this->GameWindows.end() ; ++WinIt )
            {
                if( ToBeDestroyed == (*WinIt) ) {
                    WindowEventPtr WinEv = std::make_shared<WindowEvent>(EventWindowDestroyed,(*WinIt)->GetID());
                    this->GraphicsPublisher.DispatchEvent(WinEv);

                    delete ToBeDestroyed;
                    this->GameWindows.erase(WinIt);
                    return;
                }
            }
        }

        void GraphicsManager::DestroyAllGameWindows(Boole ExcludePrimary)
        {
            for( GameWindow* CurrWin : this->GameWindows )
            {
                WindowEventPtr WinEv = std::make_shared<WindowEvent>(EventWindowDestroyed,CurrWin->GetID());
                this->GraphicsPublisher.DispatchEvent(WinEv);

                delete CurrWin;
            }
            this->GameWindows.clear();

            if( !ExcludePrimary ) {
                delete this->PrimaryWindow;
                this->PrimaryWindow = nullptr;
            }
        }

        GameWindow* GraphicsManager::GetPrimaryWindow()
        {
            return this->PrimaryWindow;
        }

        GraphicsManager::GameWindowIterator GraphicsManager::BeginGameWindow()
            { return this->GameWindows.begin(); }

        GraphicsManager::GameWindowIterator GraphicsManager::EndGameWindow()
            { return this->GameWindows.end(); }

        GraphicsManager::ConstGameWindowIterator GraphicsManager::BeginGameWindow() const
            { return this->GameWindows.begin(); }

        GraphicsManager::ConstGameWindowIterator GraphicsManager::EndGameWindow() const
            { return this->GameWindows.end(); }

        ///////////////////////////////////////////////////////////////////////////////
        // RenderSystem Management

        void GraphicsManager::SetRenderSystem(const Graphics::RenderSystem RenderSys, Boole InitializeRenderSystem)
        {
            if( !this->OgreBeenInitialized ) {
                this->CurrRenderSys = RenderSys;
            }else{
                MEZZ_EXCEPTION(ExceptionBase::INVALID_STATE_EXCEPTION,"Attempting to set RenderSystem after graphics has been initialized.  This is not supported.");
            }

            if( InitializeRenderSystem ) {
                this->InitOgreRenderSystem();
            }
        }

        Graphics::RenderSystem GraphicsManager::GetCurrRenderSystem()
        {
            return this->CurrRenderSys;
        }

        String GraphicsManager::GetRenderSystemName(const Graphics::RenderSystem RenderSys)
        {
            switch(RenderSys)
            {
                case Graphics::RS_DirectX9:     return "Direct3D9 Rendering Subsystem";          break;
                case Graphics::RS_DirectX11:    return "Direct3D11 Rendering Subsystem";         break;
                case Graphics::RS_OpenGL2:      return "OpenGL Rendering Subsystem";             break;
                case Graphics::RS_OpenGL3Plus:  return "OpenGL 3+ Rendering Subsystem (ALPHA)";  break;
                case Graphics::RS_OpenGLES1:    return "OpenGL ES 1.x Rendering Subsystem";      break;
                case Graphics::RS_OpenGLES2:    return "OpenGL ES 2.x Rendering Subsystem";      break;
                default:                        return "";                                       break;
            }
            return "";
        }

        String GraphicsManager::GetShortenedRenderSystemName(const Graphics::RenderSystem RenderSys)
        {
            switch(RenderSys)
            {
                case Graphics::RS_DirectX9:     return "Direct3D9";    break;
                case Graphics::RS_DirectX11:    return "Direct3D11";   break;
                case Graphics::RS_OpenGL2:      return "OpenGL";       break;
                case Graphics::RS_OpenGL3Plus:  return "OpenGL3+";     break;
                case Graphics::RS_OpenGLES1:    return "OpenGLES1.x";  break;
                case Graphics::RS_OpenGLES2:    return "OpenGLES2.x";  break;
                default:                        return "";             break;
            }
            return "";
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Query Methods

        const GraphicsManager::ResolutionContainer& GraphicsManager::GetSupportedResolutions() const
            { return this->SupportedResolutions; }

        const StringVector& GraphicsManager::GetSupportedDevices() const
            { return this->SupportedDevices; }

        const WindowSettings& GraphicsManager::GetDesktopSettings() const
            { return this->DesktopSettings; }

        EventPublisher& GraphicsManager::GetGraphicsPublisher()
            { return this->GraphicsPublisher; }

        const EventPublisher& GraphicsManager::GetGraphicsPublisher() const
            { return this->GraphicsPublisher; }

        ///////////////////////////////////////////////////////////////////////////////
        // Utility Methods

        void GraphicsManager::RenderOneFrame()
        {
            this->PumpInternalEvents();
            // Do the actual frame
            Ogre::Root::getSingleton().renderOneFrame();
            // This fixes an undocumented error of some kind
            if( !this->GetPrimaryWindow()->_GetOgreWindowPointer()->isVisible() ) {
                Ogre::Root::getSingleton().clearEventTimes();
            }
            // Do the Logging aggregation
            /*Ogre::Log* DefaultLog = Ogre::LogManager::getSingleton().getDefaultLog();
            if( DefaultLog != nullptr ) {
                if( this->ThreadResources != nullptr ) {
                    //this->ThreadResources->GetUsableLogger() << DefaultLog->stream();
                }else{
                    //Entresol::GetSingletonPtr()->_GetLogStream() << DefaultLog->stream();
                }
                DefaultLog->stream().flush();
            }// */
        }

        void GraphicsManager::SwapAllBuffers(Boole WaitForVsync)
        {
            for( Whole X = 0 ; X < this->GetNumGameWindows() ; X++ )
                this->GetGameWindow(X)->_GetOgreWindowPointer()->swapBuffers();
        }

        void GraphicsManager::Initialize()
        {
            if( !this->Initialized ) {
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
            if( this->Initialized ) {
                this->TheEntresol->GetScheduler().RemoveWorkUnitMonopoly( this->RenderWork );

                // Textures are loaded into video memory, which we can only do with a valid and initialized rendering context.
                // If the context goes away, such as is the case when the rendersystem is shut down then we can't have textures loaded.
                // So deinitialize the TextureManager.
                this->TheEntresol->GetManager(ManagerBase::MT_TextureManager)->Deinitialize();

                /// @todo Here is where we should be shutting down the rendersystem, but Ogre in it's poorly coded ways wasn't
                /// designed for that unless you are also deleting Ogre::Root.  Take it up with them if you don't like it.
                /// (tell me where the thread is when you do, I'll join in)
                /// Related: http://www.ogre3d.org/forums/viewtopic.php?f=4&t=77036&sid=a7ce7512032eb851d50da083795198f3

                if( this->AutoGenFiles )
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

        EntresolManager* DefaultGraphicsManagerFactory::CreateManager(const NameValuePairList& Params)
        {
            if( GraphicsManager::SingletonValid() ) {
                /// @todo Add something to log a warning that the manager exists and was requested to be constructed when we have a logging manager set up.
                return GraphicsManager::GetSingletonPtr();
            }else{
                return new GraphicsManager();
            }
        }

        EntresolManager* DefaultGraphicsManagerFactory::CreateManager(const XML::Node& XMLNode)
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
