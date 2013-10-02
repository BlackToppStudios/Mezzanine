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
#ifndef _graphicsviewport_h
#define _graphicsviewport_h

#include "vector2.h"

namespace Ogre
{
    class Viewport;
}

namespace Mezzanine
{
    namespace Graphics
    {
        class GameWindow;
        class Camera;
        ///////////////////////////////////////////////////////////////////////////////
        /// @class Viewport
        /// @headerfile viewport.h
        /// @brief This class is for creating and managing viewports within a game window.
        /// @details A game window can contain several veiwports, and veiwports can overlap.  This is
        /// useful for editors mostly, but can be of use in games as well.  Be careful about positioning
        /// Veiwports in a game window.
        ///////////////////////////////////////
        class MEZZ_LIB Viewport
        {
            protected:
                Ogre::Viewport* OgreViewport;
                GameWindow* Parent;
                Camera* ViewportCam;
            public:
                /// @brief Class constructor.
                /// @param ViewportCamera The camera that is to be attached to this veiwport.
                /// @param ZOrder The render order of this viewport relative to other viewports in the game window.
                /// @param ParentWindow The game window this viewport belongs to.
                Viewport(Camera* ViewportCamera, const Integer& ZOrder, GameWindow* ParentWindow);
                /// @brief Class destructor.
                ~Viewport();

                ///////////////////////////////////////////////////////////////////////////////
                // Camera and parent Management

                /// @brief Sets which camera is bound to this viewport.
                /// @param ViewportCamera Pointer to the camera to be bount to this viewport, or NULL to simply unbind a camera.
                void SetCamera(Camera* ViewportCamera);
                /// @brief Gets the camera associated with this viewport.
                /// @return Returns a pointer to the camera using this viewport.
                Camera* GetViewportCamera();
                /// @brief Gets the game window this viewport belongs to.
                /// @return Returns a pointer to the game window that created this viewport.
                GameWindow* GetParentWindow();

                ///////////////////////////////////////////////////////////////////////////////
                // Viewport Metrics Management

                /// @brief Gets the Zorder assigned to this viewport.
                /// @return Returns an integer that represents this viewports rendering order in it's parent GameWindow.
                Integer GetZOrder() const;
                /// @brief Sets the position and size of this viewport within the game window.
                /// @param Position Vector2 of relative values(range: 0-1) representing the top left corner of the viewport.
                /// Values are relative to the game window this viewport belongs to.
                /// @param Size Vector2 of relative values(range: 0-1) representing the width and height of the viewport.
                /// Values are relative to the game window this viewport belongs to.
                void SetDimensions(const Vector2& Position, const Vector2& Size);
                /// @brief Sets the position and size of this viewport within the game window.
                /// @param Left A relative value(range: 0-1) representing the leftmost position of the viewport.
                /// @param Top A relative value(range: 0-1) representing the topmost position of the viewport.
                /// @param Width A relative value(range: 0-1) representing the width of the viewport.
                /// @param Height A relative value(range: 0-1) representing the height of the viewport.
                void SetDimensions(const Real& Left, const Real& Top, const Real& Width, const Real& Height);
                /// @brief Gets the relative left position of the viewport.
                /// @return Returns a real representing the relative left position of this veiwport.
                Real GetLeft() const;
                /// @brief Gets the relative top position of the viewport.
                /// @return Returns a real representing the relative top position of this veiwport.
                Real GetTop() const;
                /// @brief Gets the relative width of the viewport.
                /// @return Returns a real representing the relative width of this veiwport.
                Real GetWidth() const;
                /// @brief Gets the relative height of the viewport.
                /// @return Returns a real representing the relative height of this veiwport.
                Real GetHeight() const;
                /// @brief Gets the left position of the viewport in pixels.
                /// @return Returns a whole representing the left position of this veiwport in pixels.
                Whole GetActualLeft() const;
                /// @brief Gets the top position of the viewport in pixels.
                /// @return Returns a whole representing the top position of this veiwport in pixels.
                Whole GetActualTop() const;
                /// @brief Gets the width of the viewport in pixels.
                /// @return Returns a whole representing the width of this veiwport in pixels.
                Whole GetActualWidth() const;
                /// @brief Gets the height of the viewport in pixels.
                /// @return Returns a whole representing the height of this veiwport in pixels.
                Whole GetActualHeight() const;

                ///////////////////////////////////////////////////////////////////////////////
                // Internal Methods

                /// @internal
                /// @brief Gets the internal Ogre Viewport.
                /// @return Returns a pointer to the Ogre Viewport this class is based on.
                Ogre::Viewport* GetOgreViewport() const;
        };//Viewport
    }//Graphics
}//Mezzanine

#endif
