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
#ifndef _graphicsmanager_h
#define _graphicsmanager_h

#include "managerbase.h"
#include "managerfactory.h"
#include "singleton.h"
#include "graphicssettings.h"
#include "enumerations.h"
#include "gamewindow.h"

namespace Ogre
{
    class Timer;
    class RenderWindow;
    class Viewport;
}

struct SDL_Surface;
struct SDL_Window;

namespace Mezzanine
{
    // Used by the scripting language binder to help create bindgings for this class. SWIG does know to creation template instances
    #ifdef SWIG
    %template(SingletonGraphicsManager) Singleton<GraphicsManager>;
    #endif

    ///////////////////////////////////////////////////////////////////////////////
    /// @class GraphicsManager
    /// @headerfile graphicsmanager.h
    /// @brief This is intended to store basic graphics setting for the user.
    /// @details This stores x/y resolution, fullscreen and in the future other
    /// settings. This is intended to make it easy for developers to pass/move around
    /// complex graphics settings. We hope to eventually include other items like
    /// shader settings, rendering API, and maybe other settings too.
    ///////////////////////////////////////
    class MEZZ_LIB GraphicsManager: public ManagerBase, public Singleton<GraphicsManager>
    {
        private:
            GraphicsSettings PrimarySettings;
            GraphicsSettings DefaultSettings;
            GraphicsSettings DesktopSettings;

            std::vector< GameWindow* > GameWindows;

            std::vector< String > SupportedResolutions;
            std::vector< String > SupportedDevices;

            /// @brief Adjust all Settings
            /// @param Width The desired width.
            /// @param Height The desired height.
            /// @param FullScreen True if fullscreen, false if not.
            /// @details This adjusts most data in this Graphics Settings and accepts new resolution and fullscreen settings. Be
            /// careful that the settings selected are appropriate. Many mobile devices do not support windows, and many screens
            /// do not support arbitrary resolutions in fullscreen mode.
            void Construct( const Whole& Width, const Whole& Height, const bool& FullScreen);
            String ConvertRenderSystem(const Mezzanine::RenderSystem& RS);

            void InitOgreRenderSystem();

            Ogre::Timer* RenderTimer;
            GameWindow* PrimaryGameWindow;

            Whole FrameDelay;
            Mezzanine::RenderSystem CurrRenderSys;

            bool OgreBeenInitialized;
        public:

            /// @brief Basic constructor
            /// @details This creates a basic Graphics Settings with resolution 640x480 with fullscreen set to false
            GraphicsManager();
#ifdef MEZZXML
            /// @brief XML constructor.
            /// @param XMLNode The node of the xml document to construct from.
            GraphicsManager(xml::Node& XMLNode);
#endif
            /// @brief Versatile Constructor
            /// @param Width The desired width.
            /// @param Height The desired height.
            /// @param FullScreen True if fullscreen, false if not.
            /// @details This creates a Graphics Settings with resolution and fullscreen passed into to it that will be used
            /// when creating the primary window if one isn't already created when this manager in initialized. Be careful that the
            /// settings selected are appropriate. Many mobile devices do not support windows, and many screens do not support
            /// arbitrary resolutions in fullscreen mode.
            GraphicsManager(const Whole &Width, const Whole &Height, const bool &FullScreen);

            /// @brief Class Destructor.
            ~GraphicsManager();

            /// @brief Creates a new game window to be rendered to.
            /// @param WindowCaption The caption to be set in the window titlebar.
            /// @param Width The desired width in pixels.
            /// @param Height The desired height in pixels.
            /// @param Flags Additional misc parameters, see GameWindow class for more info.
            /// @param ViewportConf The configuration/layout of the viewports on this window.
            GameWindow* CreateGameWindow(const String& WindowCaption, const Whole& Width, const Whole& Height, const Whole& Flags, const GameWindow::ViewportLayout& ViewportConf = GameWindow::VL_Custom);

            /// @brief Gets a game window by index.
            /// @return Returns a pointer to the game window requested.
            GameWindow* GetGameWindow(const Whole& Index);

            /// @brief Gets the number of game windows within this manager.
            /// @return Returns a Whole representing the number of game windows within this manager.
            Whole GetNumGameWindows();

            /// @brief Destroys a created game window by index.
            /// @param WindowIndex The index of the window to be destroyed.
            void DestroyGameWindow(GameWindow* ToBeDestroyed);

            /// @brief Destroys every game window created.
            /// @param ExcludePrimary Whether or not you want to spare the primary window created.
            void DestroyAllGameWindows(bool ExcludePrimary = true);

            /// @brief Gets the primary(first) game window.
            /// @return Returns a pointer to the primary game window.
            GameWindow* GetPrimaryGameWindow();

            /// @brief Gets the desktop display settings.
            /// @param Returns a GraphicsSettings struct with the desktop display settings.
            const GraphicsSettings& GetDesktopSettings();

            /// @brief Gets the default settings for windows this manager initialized with.
            /// @param Returns a GraphicsSettings struct with the default display settings.
            const GraphicsSettings& GetDefaultSettings();

            /// @brief Gets whether or not SDL has been started.
            /// @return Returns a bool indicating whether or not SDL has been initialized yet.
            bool HasSDLBeenInitialized();

            /// @brief Gets whether or not Ogre has been started.
            /// @return Returns a bool indicating whether or not Ogre has been initialized yet.
            bool HasOgreBeenInitialized();

            /// @brief Sets the render system to be used.
            /// @remarks This will only work prior to a window being created/graphics manager being initialized.  The internal structures to be built need
            /// to know what rendersystem to build for.  Additionally this cannot be swapped/changed at runtime.  If called after a window has been made this will throw an exception.
            /// @param RenderSys The Render system to be used.
            /// @param InitializeRenderSystem Whether to immediately initialize the rendersystem afterwords.
            void SetRenderSystem(const Mezzanine::RenderSystem& RenderSys, bool InitializeRenderSystem = false);

            /// @brief Gets the current rendersystem being used.
            /// @remarks This does not return a pointer or any other kind of accessor to the actual rendersystem structure.  If you need that, then we're doing something wrong.
            /// @return Returns an enum value coresponding to the render system being used.
            Mezzanine::RenderSystem GetCurrRenderSystem();

            /// @brief Gets the name of the render system in current use.
            /// @return Returns a string containing the name of the current render system.
            String GetRenderSystemName();

            /// @brief Gets a vector containing all the resolutions supported by this render system on the current hardware.
            /// @details This vector is populated when the manager gets initialized.  Calling on it before then will give you an empty vector.
            /// @return Returns a Const Pointer to the vector storing all the supported resolutions.
            const std::vector<String>* GetSupportedResolutions();

            /// @brief Gets a vector containing all the devices supported by this render system on the current hardware.
            /// @details This vector is populated when the manager gets initialized.  Calling on it before then will give you an empty vector.
            /// @return Returns a Const Pointer to the vector storing all the supported devices.
            const std::vector<String>* GetSupportedDevices();

            /// @brief Resets the Render timer.
            /// @details This function should not be called unless you know exactly what you are doing.
            /// This will reset the timer keeping track of game frame times, and thus can disrupt functionality of the mainloop.
            virtual void ResetRenderTimer();

            /// @brief Renders one frame of the scene.
            virtual void RenderOneFrame();

            /// @brief Swaps all the buffers of all GameWindows.
            /// @param WaitForVsync Whether or not the buffer should swap after the vsync interval is completed.
            virtual void SwapAllBuffers(bool WaitForVsync);

            ///////////////////////////////////////////////////////////////////////////////
            //Inherited from ManagerBase

            /// @copydoc ManagerBase::Initialize()
            virtual void Initialize();
            /// @copydoc ManagerBase::DoMainLoopItems()
            virtual void DoMainLoopItems();
            /// @copydoc ManagerBase::PostMainLoopItems()
            virtual bool PostMainLoopItems();
            /// @copydoc ManagerBase::GetInterfaceType()
            virtual ManagerType GetInterfaceType() const;
            /// @copydoc ManagerBase::GetImplementationTypeName()
            virtual String GetImplementationTypeName() const;
    };//GraphicsManager

    ///////////////////////////////////////////////////////////////////////////////
    /// @class DefaultGraphicsManagerFactory
    /// @headerfile graphicsmanager.h
    /// @brief A factory responsible for the creation and destruction of the default graphicsmanager.
    ///////////////////////////////////////
    class MEZZ_LIB DefaultGraphicsManagerFactory : public ManagerFactory
    {
        public:
            /// @brief Class constructor.
            DefaultGraphicsManagerFactory();
            /// @brief Class destructor.
            virtual ~DefaultGraphicsManagerFactory();

            /// @copydoc ManagerFactory::GetManagerTypeName()
            String GetManagerTypeName() const;
            /// @copydoc ManagerFactory::CreateManager(NameValuePairList&)
            ManagerBase* CreateManager(NameValuePairList& Params);
#ifdef MEZZXML
            /// @copydoc ManagerFactory::CreateManager(xml::Node&)
            ManagerBase* CreateManager(xml::Node& XMLNode);
#endif
            /// @copydoc ManagerFactory::DestroyManager(ManagerBase*)
            void DestroyManager(ManagerBase* ToBeDestroyed);
    };//DefaultGraphicsManagerFactory
}//Mezzanine
#endif
