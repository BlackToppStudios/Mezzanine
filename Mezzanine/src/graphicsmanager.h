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
#ifndef _graphicsmanager_h
#define _graphicsmanager_h

#include "managerbase.h"
#include "managerfactory.h"
#include "singleton.h"
#include "Graphics/windowsettings.h"
#include "Graphics/graphicsenumerations.h"
#include "Graphics/gamewindow.h"
#include "objectsettings.h"

namespace Ogre
{
    class Timer;
    class RenderWindow;
    class Viewport;
    class Plugin;
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
    class MEZZ_LIB GraphicsManager: public ManagerBase, public ObjectSettingsHandler, public Singleton<GraphicsManager>
    {
        public:
            typedef std::vector<Graphics::GameWindow*> GameWindowContainer;
            typedef GameWindowContainer::iterator GameWindowIterator;
            typedef GameWindowContainer::const_iterator ConstGameWindowIterator;

        protected:
            /// @internal
            /// @brief Track all statically linked Ogre render systems, usually only one, but could be many.
            std::vector<Ogre::Plugin*> RenderSystems;

            /// @internal
            /// @brief A listing of the types of rendersystems Types that correspond to the entry in @ref RenderSystems .
            std::vector<Graphics::RenderSystem> RenderSystemTypes;


            Ogre::Timer* RenderTimer;
            Graphics::GameWindow* PrimaryGameWindow;
            bool OgreBeenInitialized;
            Whole FrameDelay;
            Graphics::RenderSystem CurrRenderSys;
            WindowSettings DesktopSettings;

            GameWindowContainer GameWindows;
            StringVector SupportedResolutions;
            StringVector SupportedDevices;

            /// @brief Construct the manager and set sane defaults.
            void Construct();
            void InitOgreRenderSystem();
            virtual String GetObjectRootNodeName() const;
            virtual void AppendCurrentSettings(XML::Node& SettingsRootNode);
            virtual void ApplySettingGroupImpl(ObjectSettingGroup* Group);
        public:

            /// @brief Basic constructor
            /// @details This creates a basic Graphics Settings with resolution 640x480 with fullscreen set to false
            GraphicsManager();
            /// @brief XML constructor.
            /// @param XMLNode The node of the xml document to construct from.
            GraphicsManager(XML::Node& XMLNode);
            /// @brief Class Destructor.
            ~GraphicsManager();

            ///////////////////////////////////////////////////////////////////////////////
            // Window Management

            /// @brief Creates a new game window to be rendered to.
            /// @param WindowCaption The caption to be set in the window titlebar.
            /// @param Width The desired width in pixels.
            /// @param Height The desired height in pixels.
            /// @param Flags Additional misc parameters, see Graphics::GameWindow class for more info.
            Graphics::GameWindow* CreateGameWindow(const String& WindowCaption, const Whole& Width, const Whole& Height, const Whole& Flags);
            /// @brief Gets a game window by index.
            /// @return Returns a pointer to the game window requested.
            Graphics::GameWindow* GetGameWindow(const Whole& Index);
            /// @brief Gets the number of game windows within this manager.
            /// @return Returns a Whole representing the number of game windows within this manager.
            Whole GetNumGameWindows();
            /// @brief Destroys a created game window by index.
            /// @param WindowIndex The index of the window to be destroyed.
            void DestroyGameWindow(Graphics::GameWindow* ToBeDestroyed);
            /// @brief Destroys every game window created.
            /// @param ExcludePrimary Whether or not you want to spare the primary window created.
            void DestroyAllGameWindows(bool ExcludePrimary = true);
            /// @brief Gets the primary(first) game window.
            /// @return Returns a pointer to the primary game window.
            Graphics::GameWindow* GetPrimaryGameWindow();

            /// @brief Gets an iterator to the first GameWindow stored in this manager.
            GameWindowIterator BeginGameWindow();
            /// @brief Gets an iterator to one passed the last GameWindow stored in this manager.
            GameWindowIterator EndGameWindow();
            /// @brief Gets a const iterator to the first GameWindow stored in this manager.
            ConstGameWindowIterator BeginGameWindow() const;
            /// @brief Gets a const iterator to one passed the last GameWindow stored in this manager.
            ConstGameWindowIterator EndGameWindow() const;

            ///////////////////////////////////////////////////////////////////////////////
            // RenderSystem Management

            /// @brief Sets the render system to be used.
            /// @remarks This will only work prior to a window being created/graphics manager being initialized.  The internal structures to be built need
            /// to know what rendersystem to build for.  Additionally this cannot be swapped/changed at runtime.  If called after a window has been made this will throw an exception.
            /// @param RenderSys The Render system to be used.
            /// @param InitializeRenderSystem Whether to immediately initialize the rendersystem afterwords.
            void SetRenderSystem(const Graphics::RenderSystem& RenderSys, bool InitializeRenderSystem = false);
            /// @brief Gets the current rendersystem being used.
            /// @remarks This does not return a pointer or any other kind of accessor to the actual rendersystem structure.  If you need that, then we're doing something wrong.
            /// @return Returns an enum value coresponding to the render system being used.
            Graphics::RenderSystem GetCurrRenderSystem();
            /// @brief Gets the name of the provided render system.
            /// @param RenderSys The rendersystem to get the name of.
            /// @return Returns a string containing the name of the provided render system.
            String GetRenderSystemName(const Graphics::RenderSystem& RenderSys);
            /// @brief Gets a short hand name of the provided render system.
            /// @param RenderSys The rendersystem to get the name of.
            /// @return Returns a string containing the shortened name of the provided render system.
            String GetShortenedRenderSystemName(const Graphics::RenderSystem& RenderSys);

            ///////////////////////////////////////////////////////////////////////////////
            // Query Methods

            /// @brief Gets a vector containing all the resolutions supported by this render system on the current hardware.
            /// @details This vector is populated when the manager gets initialized.  Calling on it before then will give you an empty vector.
            /// @return Returns a Const Pointer to the vector storing all the supported resolutions.
            const StringVector* GetSupportedResolutions();
            /// @brief Gets a vector containing all the devices supported by this render system on the current hardware.
            /// @details This vector is populated when the manager gets initialized.  Calling on it before then will give you an empty vector.
            /// @return Returns a Const Pointer to the vector storing all the supported devices.
            const StringVector* GetSupportedDevices();
            /// @brief Gets the desktop display settings.
            /// @param Returns a WindowSettings struct with the desktop display settings.
            const WindowSettings& GetDesktopSettings();

            ///////////////////////////////////////////////////////////////////////////////
            // Utility Methods

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
            // SubSystem Initialization

            /// @brief Gets whether or not SDL has been started.
            /// @return Returns a bool indicating whether or not SDL has been initialized yet.
            bool HasSDLBeenInitialized();
            /// @brief Gets whether or not Ogre has been started.
            /// @return Returns a bool indicating whether or not Ogre has been initialized yet.
            bool HasOgreBeenInitialized();

            ///////////////////////////////////////////////////////////////////////////////
            // Inherited from ManagerBase

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
            /// @copydoc ManagerFactory::CreateManager(XML::Node&)
            ManagerBase* CreateManager(XML::Node& XMLNode);
            /// @copydoc ManagerFactory::DestroyManager(ManagerBase*)
            void DestroyManager(ManagerBase* ToBeDestroyed);
    };//DefaultGraphicsManagerFactory
}//Mezzanine
#endif
