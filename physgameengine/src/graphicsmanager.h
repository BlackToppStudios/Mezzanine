//© Copyright 2010 - 2011 BlackTopp Studios Inc.
/* This file is part of The PhysGame Engine.

    The PhysGame Engine is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The PhysGame Engine is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with The PhysGame Engine.  If not, see <http://www.gnu.org/licenses/>.
*/
/* The original authors have included a copy of the license specified above in the
   'Docs' folder. See 'gpl.txt'
*/
/* We welcome the use of The PhysGame anyone, including companies who wish to
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
#include "graphicssettings.h"
#include "enumerations.h"

namespace Ogre
{
    class Timer;
    class RenderWindow;
    class Viewport;
}

struct SDL_Surface;
struct SDL_Window;

namespace phys
{
    class GameWindow;
    ///////////////////////////////////////////////////////////////////////////////
    /// @class GraphicsManager
    /// @headerfile graphicsmanager.h
    /// @brief This is intended to store basic graphics setting for the user.
    /// @details This stores x/y resolution, fullscreen and in the future other
    /// settings. This is intended to make it easy for developers to pass/move around
    /// complex graphics settings. We hope to eventually include other items like
    /// shader settings, rendering API, and maybe other settings too.
    ///////////////////////////////////////
    class PHYS_LIB GraphicsManager: public ManagerBase
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
            void Construct( const Whole &Width, const Whole &Height, const bool &FullScreen);
            String ConvertRenderSystem(const phys::RenderSystem& RS);

            void InitOgre();
            void ShutdownSDL();
            void InitViewportAndCamera(GameWindow* NewWindow);

            Ogre::Timer* RenderTimer;
            GameWindow* PrimaryGameWindow;

            Whole FrameDelay;
            phys::RenderSystem CurrRenderSys;

            bool OgreBeenInitialized;
            bool GraphicsInitialized;

        public:
            /// @internal
            /// @brief SDL is used for use input, and must be initialized prior to use.
            static void InitSDL();

            /// @brief Basic constructor
            /// @details This creates a basic Graphics Settings with resolution 640x480 with fullscreen set to false
            GraphicsManager();

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
            GameWindow* CreateGameWindow(const String& WindowCaption, const Whole& Width, const Whole& Height, const Whole& Flags);

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
            void SetRenderSystem(const phys::RenderSystem& RenderSys);

            /// @brief Gets the current rendersystem being used.
            /// @remarks This does not return a pointer or any other kind of accessor to the actual rendersystem structure.  If you need that, then we're doing something wrong.
            /// @return Returns an enum value coresponding to the render system being used.
            phys::RenderSystem GetCurrRenderSystem();

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

        //Inherited from ManagerBase
            /// @brief Empty Initializor
            /// @details This specific initializor is unneeded, but we implement it for compatibility. It also exists
            /// in case a derived class wants to override it for some reason
            virtual void Initialize();

            /// @brief This is where the rendering takes place.
            /// @details This does the rendering for the game using all the actors in the actormanager.
            virtual void DoMainLoopItems();

            /// @brief This returns the type of this manager.
            /// @return This returns ManagerTypeName::GraphicsManager
            virtual ManagerTypeName GetType() const;

            /// @brief This is derived from and uses the ManagerBase to perform the the post main loop callbacks
            /// @return This returns a true or false depending on what the callback returns
            virtual bool PostMainLoopItems();
    };
}
#endif
