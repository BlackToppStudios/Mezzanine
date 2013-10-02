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
#ifndef _graphicsgamewindow_h
#define _graphicsgamewindow_h

#include "Graphics/windowsettings.h"
namespace Ogre
{
    class RenderWindow;
}

struct SDL_Window;

namespace Mezzanine
{
    namespace Graphics
    {
        // Forward Declarations for Mezzanine::Graphics::
        class GraphicsManager;
        class Viewport;
        class GameWindowInternalData;
        class Camera;

        ///////////////////////////////////////////////////////////////////////////////
        /// @class GameWindow
        /// @headerfile gamewindow.h
        /// @brief This class is for creating and managing game windows.
        /// @details
        ///////////////////////////////////////
        class MEZZ_LIB GameWindow
        {
            public:
                typedef std::pair<Integer,Viewport*> ViewportEntry;
                typedef std::list<ViewportEntry> ViewportContainer;
                typedef ViewportContainer::iterator ViewportIterator;
                typedef ViewportContainer::const_iterator ConstViewportIterator;
                typedef ViewportContainer::reverse_iterator ReverseViewportIterator;
                typedef ViewportContainer::const_reverse_iterator ConstReverseViewportIterator;

                /// @enum WindowFlags
                /// @brief A listing of potential options for configuring a game window during construction.
                enum WindowFlags
                {
                    WF_Fullscreen = 1,    ///< This enables fullscreen on the window.
                    WF_Hidden = 2,        ///< This hides the window so that it isn't visible.
                    WF_VsyncEnabled = 4,  ///< This enables vsync for the window.
                    WF_FSAA_2 = 8,        ///< Enables Fullscreen Anti-Aliasing level 2 for the window.
                    WF_FSAA_4 = 16,       ///< Enables Fullscreen Anti-Aliasing level 4 for the window.
                    WF_FSAA_8 = 32,       ///< Enables Fullscreen Anti-Aliasing level 8 for the window.
                    WF_FSAA_16 = 64,      ///< Enables Fullscreen Anti-Aliasing level 16 for the window.
                    WF_Resizeable = 128,  ///< Creates a window with resizable borders, otherwise it is fixed size.
                    WF_Maximized = 256,   ///< Maximizes the window immediately after construction.
                    WF_Borderless = 512   ///< Removes all window decorations from the window(titlebar, borders, etc.).
                };
            protected:
                GraphicsManager* Manager;
                Ogre::RenderWindow* OgreWindow;
                SDL_Window* SDLWindow;
                GameWindowInternalData* GWID;
                WindowSettings Settings;
                ViewportContainer Viewports;

                void CreateGameWindow(const String& WindowCaption, const Whole& Width, const Whole& Height, const Whole& Flags);
                void UpdateViewportsAndCameras();
                int IsLargerThenDesktop(const Whole& Width, const Whole& Height);
            public:
                /// @brief Class constructor.
                /// @param WindowCaption The caption to be set in the window titlebar.
                /// @param Width The desired width in pixels.
                /// @param Height The desired height in pixels.
                /// @param Flags Additional misc parameters, see WindowFlags enum for more info.
                GameWindow(const String& WindowCaption, const Whole& Width, const Whole& Height, const Whole& Flags);
                /// @brief Class destructor.
                ~GameWindow();

                ///////////////////////////////////////////////////////////////////////////////
                // Viewport Management

                /// @brief Creates an additional Viewport within a created render window.
                /// @param VeiwportCamera The camera that is to be attached to this Viewport.
                /// @param ZOrder The render order of this viewport relative to other viewports in this window.
                /// 0 means it'll use the current viewport count to determine the ZOrder.
                Viewport* CreateViewport(Camera* ViewportCamera, const Integer ZOrder = 0);
                /// @brief Gets a viewport by index.
                /// @return Returns a pointer to the viewport requested.
                Viewport* GetViewport(const Whole Index) const;
                /// @brief Gets the number of viewports within this window.
                /// @return Returns a Whole representing the number of viewports within this window.
                Whole GetNumViewports() const;
                /// @brief Destroys a viewport within this window.
                /// @param ToBeDestroyed The viewport that will be destroyed.
                void DestroyViewport(Viewport* ToBeDestroyed);

                /// @brief Gets an iterator to the first viewport in this window.
                ViewportIterator BeginViewport();
                /// @brief Gets an iterator to one passed the last viewport in this window.
                ViewportIterator EndViewport();
                /// @brief Gets a const iterator to the first viewport in this window.
                ConstViewportIterator BeginViewport() const;
                /// @brief Gets a const iterator to one passed the last viewport in this window.
                ConstViewportIterator EndViewport() const;

                /// @brief Gets an iterator to the last viewport in this window.
                ReverseViewportIterator ReverseBeginViewport();
                /// @brief Gets an iterator to one before the first viewport in this window.
                ReverseViewportIterator ReverseEndViewport();
                /// @brief Gets a const iterator to the last viewport in this window.
                ConstReverseViewportIterator ReverseBeginViewport() const;
                /// @brief Gets a const iterator to one before the first viewport in this window.
                ConstReverseViewportIterator ReverseEndViewport() const;

                ///////////////////////////////////////////////////////////////////////////////
                // Window Metrics Management

                /// @brief Sets the Width.
                /// @details Set the Render Width inside the window in windowed mode, set the resolution of the screen in fullscreen
                /// @param Width This accepts a Whole.
                void SetRenderWidth(const Whole& Width);
                /// @brief Gets the Width of the Rendering Area
                /// @details Gets the Width of the Rendering Area
                /// @return This returns the Width of the Rendering Area
                Whole GetRenderWidth() const;
                /// @brief Sets the Height.
                /// @details Set the Render Height inside the window in windowed mode, set the resolution of the screen in fullscreen
                /// @param Height This accepts a Whole.
                void SetRenderHeight(const Whole& Height);
                /// @brief Gets the Height of the Rendering Area
                /// @details Gets the Height of the Rendering Area
                /// @return This returns the Height of the Rendering Area
                Whole GetRenderHeight() const;
                /// @brief Changes the X and Y Resolution at the same time
                /// @details This should be useful in situations where it is not possible to update the width and height separately.
                /// @param Width The new desired Width for the rendering area as a whole number
                /// @param Height The new desired Width for the rendering area as a whole number
                void SetRenderResolution(const Whole& Width, const Whole& Height);
                 /// @brief Set the Fullscreen Setting
                /// @details Set the Fullscreen Setting
                /// @param Fullscreen This accepts a bool. True for fullscreen, false for windowed
                void SetFullscreen(const bool Fullscreen);
                /// @brief Gets the Fullscreen Setting
                /// @details Gets the Fullscreen Setting
                /// @return This returns a bool, true if fullscreen is set, false otherwise
                bool GetFullscreen() const;
                /// @brief Changes the X Resolution, Y Resolution, and fullscreen at the same time
                /// @details This should be useful in situations where it is not possible to update all of the options separately.
                void SetRenderOptions(const WindowSettings& NewSettings);
                /// @brief Gets the current window settings.
                /// @param Returns a WindowSettings struct with the current window settings.
                const WindowSettings& GetSettings();

                ///////////////////////////////////////////////////////////////////////////////
                // Window Settings Methods

                /// @brief Gets the the text in the titlebar.
                /// @return Returns a const string ref of the text in the titlebar.
                const String& GetWindowCaption();
                /// @brief Gets the current level of Anti-Aliasing enabled on this Window.
                /// @return Returns a Whole indicating which level of AA is enabled on this window, or 0 if AA is disabled.
                Whole GetFSAALevel() const;
                /// @brief Enables (or disables) vsync on this window.
                /// @param Enable Whether or not to enable vsync.
                void EnableVsync(bool Enable);
                /// @brief Gets whether or not vsync is currently enabled on this window.
                /// @return Returns true if vsync is currently enabled, false otherwise.
                bool VsyncEnabled() const;
                /// @brief Hides (shows) the window.
                /// @param Hidden Whether or not to hide the window.
                void SetHidden(bool Hidden);
                /// @brief Gets whether this window is hidden.
                /// @return Returns true if this window is being hidden, false otherwise.
                bool IsHidden() const;
                /// @brief Gets whether or not this window has a resizeable border.
                /// @return Returns true if this window has a border when not fullscreen and it can be resized, false otherwise.
                bool BorderIsResizeable() const;
                /// @brief Gets whether this window is borderless.
                /// @return Returns true if this window has no border, false otherwise.
                bool IsBorderless() const;

                ///////////////////////////////////////////////////////////////////////////////
                // Window Stats Methods

                /// @brief Gets the FPS based on the last frame rendered.
                /// @details This essentially determines how many frames could be rendered if all
                /// frames within a second rendered at the same speed.
                /// @return Returns a Real representing the framerate.
                Real GetLastFPS() const;
                /// @brief Gets the Average FPS.
                /// @return Returns a Real representing the average framerate.
                Real GetAverageFPS() const;
                /// @brief Gets the Best FPS.
                /// @return Returns a Real representing the best framerate.
                Real GetBestFPS() const;
                /// @brief Gets the Worst FPS.
                /// @return Returns a Real representing the worst framerate.
                Real GetWorstFPS() const;
                /// @brief Gets the shortest amount of time it's taken to render a frame.
                /// @return Returns a Real representing the best time for rendering a frame.
                Real GetBestFrameTime() const;
                /// @brief Gets the longest amount of time it's taken to render a frame.
                /// @return Returns a Real representing the worst time for rendering a frame.
                Real GetWorstFrameTime() const;

                ///////////////////////////////////////////////////////////////////////////////
                // Internal Methods

                /// @internal
                /// @brief This will get a pointer to the Ogre RenderWindow.
                /// @return Returns a pointer to the Ogre RenderWindow class in use.
                Ogre::RenderWindow* _GetOgreWindowPointer();
                /// @internal
                /// @brief This will get a pointer to the SDL Window.
                /// @return Returns a pointer to the SDL Window class in use.
                SDL_Window* _GetSDLWindowPointer();
        };//GameWindow
    }//Graphics
}//Mezzanine

#endif
