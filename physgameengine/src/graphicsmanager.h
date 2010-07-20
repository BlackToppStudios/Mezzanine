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

#include "managerbase.h"

namespace phys
{
    ///////////////////////////////////////////////////////////////////////////////
    /// @class GraphicsManager
    /// @headerfile graphicsmanager.h
    /// @brief This is intended to store basic graphics setting for the user.
    /// @details This stores x/y resolution, fullscreen and in the future other
    /// settings. This is intended to make it easy for developers to pass/move around
    /// complex graphics settings. We hope to eventually include other items like
    /// shader settings, rendering API, and maybe other settings too.
    class GraphicsManager: ManagerBase
    {
        private:

            /// @internal
            /// @brief This is the desired state of whether the window is fullscreen or not.
            bool Fullscreen;

            /// @internal
            /// @brief This stores the Height of the renderwindow
            Whole RenderHeight;

            /// @internal
            /// @brief This stores the Width of the renderwindow
            Whole RenderWidth;

        public:
            /// @brief Default constructor
            /// @details This creates a default Graphics Settings with resolution 640x480 with fullscreen set to false
            GraphicsManager();

            /// @brief Versatile Constructor
            /// @param Width_ The desired width.
            /// @param Height_ The desired height.
            /// @param FullScreen_ True if fullscreen, false if not.
            /// @details This creates a Graphics Settings with resolution and fullscreen passed into to it. Be careful that the
            /// settings selected are appropriate. Many mobile devices do not support windows, and many screens do not support
            /// arbitrary resolutions in fullscreen mode.
            GraphicsManager(const Whole &Width_, const Whole &Height_, const bool &FullScreen_);

            /// @brief Adjust all Settings
            /// @param Width_ The desired width.
            /// @param Height_ The desired height.
            /// @param FullScreen_ True if fullscreen, false if not.
            /// @details This adjusts most data in this Graphics Settings and accepts new resolution and fullscreen settings. Be
            /// careful that the settings selected are appropriate. Many mobile devices do not support windows, and many screens
            /// do not support arbitrary resolutions in fullscreen mode.
            void Construct(const Whole &Width_, const Whole &Height_, const bool &FullScreen_);

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
            /// @details This should be useful in situations where it is not possible to update the width and hright separately.
            /// @param Width_ The new desired Width for the rendering area as a whole number
            /// @param Height_ The new desired Width for the rendering area as a whole number
            void setRenderResolution(const Whole &Width_, const Whole &Height_);

        //Inherited from ManagerBase
            /// @brief Empty Initializor
            /// @details This specific initializor is unneeded, but we implement it for compatibility. It also exists
            /// in case a derived class wants to override it for some reason
            virtual void Initialize();

            /// @brief This returns the type of this manager.
            /// @return This returns ManagerTypeName::GraphicsManager
            virtual ManagerTypeName GetType() const;

    };
}
#endif
