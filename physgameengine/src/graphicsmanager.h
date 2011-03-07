//© Copyright 2010 BlackTopp Studios Inc.
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

#include "crossplatformexport.h"
#include "managerbase.h"

namespace Ogre
{
    class Timer;
    class RenderWindow;
}

class SDL_Surface;
class SDL_Window;

namespace phys
{
    ///////////////////////////////////////////////////////////////////////////////
    /// @class GraphicsSettings
    /// @headerfile graphicsmanager.h
    /// @brief This stores all the possible configuration options the graphics manager supports.
    /// @details The graphics manager stores one of these for all of it's configuration options,
    /// additionally one can be created and passed into the manager to set all the configuration
    /// options at once.
    ///////////////////////////////////////
    struct GraphicsSettings
    {
        /// @brief This stores the Height of the renderwindow
        Whole RenderHeight;
        /// @brief This stores the Width of the renderwindow
        Whole RenderWidth;
        /// @brief This is the desired state of whether the window is fullscreen or not.
        bool Fullscreen;
        /// @brief This is the desired state of whether to enable VSync or not.
        bool VSync;
        /// @brief Struct Constructor
        GraphicsSettings() : RenderHeight(0),RenderWidth(0),Fullscreen(false),VSync(false) {}
        GraphicsSettings& operator= (const GraphicsSettings &GS)
        {
            this->RenderHeight = GS.RenderHeight;
            this->RenderWidth = GS.RenderWidth;
            this->Fullscreen = GS.Fullscreen;
            this->VSync = GS.VSync;
            return *this;
        }
    };

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
            GraphicsSettings Settings;

            std::vector<String> SupportedResolutions;
            std::vector<String> SupportedDevices;

            /// @brief Adjust all Settings
            /// @param Width_ The desired width.
            /// @param Height_ The desired height.
            /// @param FullScreen_ True if fullscreen, false if not.
            /// @details This adjusts most data in this Graphics Settings and accepts new resolution and fullscreen settings. Be
            /// careful that the settings selected are appropriate. Many mobile devices do not support windows, and many screens
            /// do not support arbitrary resolutions in fullscreen mode.
            void Construct( const Whole &Width_, const Whole &Height_, const bool &FullScreen_);

            void CreateRenderWindow();
            void DestroyRenderWindow();
            void InitSDL();
            void ShutdownSDL();
            void InitViewportAndCamera();
            void UpdateWindowStats();

            Ogre::Timer *RenderTimer;
            Ogre::RenderWindow* OgreGameWindow;
            SDL_Surface* SDLscreen;
            SDL_Window* SDLwindow;

            Whole FrameDelay;
            bool SDLBeenInitialized;
            bool GraphicsInitialized;

        public:
            /// @brief Basic constructor
            /// @details This creates a basic Graphics Settings with resolution 640x480 with fullscreen set to false
            GraphicsManager();

            /// @brief Versatile Constructor
            /// @param Width_ The desired width.
            /// @param Height_ The desired height.
            /// @param FullScreen_ True if fullscreen, false if not.
            /// @details This creates a Graphics Settings with resolution and fullscreen passed into to it. Be careful that the
            /// settings selected are appropriate. Many mobile devices do not support windows, and many screens do not support
            /// arbitrary resolutions in fullscreen mode.
            GraphicsManager(const Whole &Width_, const Whole &Height_, const bool &FullScreen_);

            /// @brief Class Destructor.
            ~GraphicsManager();

            /// @brief Gets the Fullscreen Setting
            /// @details Gets the Fullscreen Setting
            /// @return This returns a bool, true if fullscreen is set, false otherwise
            bool getFullscreen() const;

            /// @brief Set the Fullscreen Setting
            /// @details Set the Fullscreen Setting
            /// @param Fullscreen_ This accepts a bool. True for fullscreen, false for windowed
            void setFullscreen(const bool &Fullscreen_);

            /// @brief Gets the Height of the Rendering Area
            /// @details Gets the Height of the Rendering Area
            /// @return This returns the Height of the Rendering Area
            Whole getRenderHeight() const;

            /// @brief Sets the Height.
            /// @details Set the Render Height inside the window in windowed mode, set the resolution of the screen in fullscreen
            /// @param Height_ This accepts a Whole.
            void setRenderHeight(const Whole &Height_);

            /// @brief Gets the Width of the Rendering Area
            /// @details Gets the Width of the Rendering Area
            /// @return This returns the Width of the Rendering Area
            Whole getRenderWidth() const;

            /// @brief Sets the Width.
            /// @details Set the Render Width inside the window in windowed mode, set the resolution of the screen in fullscreen
            /// @param Width_ This accepts a Whole.
            void setRenderWidth(const Whole &Width_);

            /// @brief Changes the X and Y Resolution at the same time
            /// @details This should be useful in situations where it is not possible to update the width and height separately.
            /// @param Width_ The new desired Width for the rendering area as a whole number
            /// @param Height_ The new desired Width for the rendering area as a whole number
            void setRenderResolution(const Whole &Width_, const Whole &Height_);

            /// @brief Changes the X Resolution, Y Resolution, and fullscreen at the same time
            /// @details This should be useful in situations where it is not possible to update all of the options separately.
            void setRenderOptions(const GraphicsSettings& NewSettings);

            /// @brief Gets whether or not SDL has been started.
            /// @return Returns a bool indicating whether or not SDL has been initialized yet.
            bool HasSDLBeenInitialized();

            /// @brief This Shows an Engine Generated Configuration Screen
            /// @details This could look like and could offer just about any option to the user. It is loosely expected to show Graphical Configuration
            /// options, like Vsync and Resolution, But it might ask some really silly stuff. I think this would be fine for smaller simpler apps
            /// Which have no other way to configure such things, but any sizable project should develop their own way to expose and manage
            /// user settings.
            bool ShowGraphicsSettingDialog();

            /// @brief Gets the FPS based on the last frame rendered.
            /// @details This essentially determines how many frames could be rendered if all
            /// frames within a second rendered at the same speed.
            /// @return Returns a Real representing the framerate.
            Real GetLastFPS();

            /// @brief Gets the Average FPS.
            /// @return Returns a Real representing the average framerate.
            Real GetAverageFPS();

            /// @brief Gets the Best FPS.
            /// @return Returns a Real representing the best framerate.
            Real GetBestFPS();

            /// @brief Gets the Worst FPS.
            /// @return Returns a Real representing the worst framerate.
            Real GetWorstFPS();

            /// @brief Gets the shortest amount of time it's taken to render a frame.
            /// @return Returns a Real representing the best time for rendering a frame.
            Real GetBestFrameTime();

            /// @brief Gets the longest amount of time it's taken to render a frame.
            /// @return Returns a Real representing the worst time for rendering a frame.
            Real GetWorstFrameTime();

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
        //Internal access functions
            /// @internal
            /// @brief This will set the stored pointer to the ogre render window.
            /// @details This function is called on once when constructing the world, should not be called otherwise.
            /// @param OgreWindow A pointer to the created Ogre RenderWindow class.
            void SetOgreWindowPointer(Ogre::RenderWindow* OgreWindow);
            /// @internal
            /// @brief This will get a pointer to the RenderWindow class in use by the world.
            /// @return Returns a pointer to the Ogre RenderWindow class in use.
            Ogre::RenderWindow* GetOgreWindowPointer();
    };
}
#endif
