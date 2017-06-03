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
#ifndef _graphicsviewport_h
#define _graphicsviewport_h

#include "vector2.h"
#include "enumerations.h"

namespace Ogre
{
    class Viewport;
}

namespace Mezzanine
{
    namespace Graphics
    {
        class GameWindow;
        class CameraProxy;
        ///////////////////////////////////////////////////////////////////////////////
        /// @brief This class is for creating and managing Viewports within a game window.
        /// @details A game window can contain several Veiwports, and Veiwports can overlap.  This is
        /// useful for editors mostly, but can be of use in games as well.  Be careful about positioning
        /// Veiwports in a game window.
        ///////////////////////////////////////
        class MEZZ_LIB Viewport
        {
        protected:
            /// @brief A pointer to the internal Viewport providing this classes functionality.
            Ogre::Viewport* OgreViewport;
            /// @brief A pointer to the window that created this Viewport.
            GameWindow* Parent;
            /// @brief A pointer to the camera being used to render this Viewport, or NULL if one isn't set.
            CameraProxy* ViewportCam;
        public:
            /// @brief Class constructor.
            /// @param ViewportCamera The CameraProxy that is to be attached to this Veiwport.
            /// @param ZOrder The render order of this Viewport relative to other Viewports in the game window.
            /// @param ParentWindow The game window this Viewport belongs to.
            Viewport(CameraProxy* ViewportCamera, const Integer& ZOrder, GameWindow* ParentWindow);
            /// @brief XML constructor.
            /// @param XMLNode The node of the xml document to construct from.
            /// @param ParentWindow The game window this Viewport belongs to.
            Viewport(const XML::Node& XMLNode, GameWindow* ParentWindow);
            /// @brief Class destructor.
            ~Viewport();

            ///////////////////////////////////////////////////////////////////////////////
            // Camera and parent Management

            /// @brief Sets which CameraProxy is bound to this Viewport.
            /// @param ViewportCamera Pointer to the CameraProxy to be bound to this Viewport, or NULL to simply unbind a CameraProxy.
            void SetCamera(CameraProxy* ViewportCamera);
            /// @brief Gets the CameraProxy associated with this Viewport.
            /// @return Returns a pointer to the CameraProxy using this Viewport.
            CameraProxy* GetCamera() const;
            /// @brief Gets the game window this Viewport belongs to.
            /// @return Returns a pointer to the game window that created this Viewport.
            GameWindow* GetParentWindow() const;

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            /// @brief Gets the Zorder assigned to this Viewport.
            /// @return Returns an integer that represents this Viewports rendering order in it's parent GameWindow.
            Integer GetZOrder() const;
            /// @brief Gets the current Orientation of the Viewport.
            /// @return Returns an enum representing the current orientation of the Viewport.
            Mezzanine::OrientationMode GetOrientationMode() const;

            /// @brief Checks whether or not a point is within the bounds of this Viewport.
            /// @param XPos The point on the X axis of the window to check.
            /// @param YPos The point on the Y axis of the window to check.
            /// @return Returns true if the point is within this Viewport, false otherwise.
            Boole IsWithinBounds(const Whole XPos, const Whole YPos) const;

            ///////////////////////////////////////////////////////////////////////////////
            // Viewport Metrics Management

            /// @brief Sets the position and size of this Viewport within the game window.
            /// @param Position Vector2 of relative values(range: 0-1) representing the top left corner of the Viewport.
            /// Values are relative to the game window this Viewport belongs to.
            /// @param Size Vector2 of relative values(range: 0-1) representing the width and height of the Viewport.
            /// Values are relative to the game window this Viewport belongs to.
            void SetDimensions(const Vector2& Position, const Vector2& Size);
            /// @brief Sets the position and size of this Viewport within the game window.
            /// @param Left A relative value(range: 0-1) representing the leftmost position of the Viewport.
            /// @param Top A relative value(range: 0-1) representing the topmost position of the Viewport.
            /// @param Width A relative value(range: 0-1) representing the width of the Viewport.
            /// @param Height A relative value(range: 0-1) representing the height of the Viewport.
            void SetDimensions(const Real& Left, const Real& Top, const Real& Width, const Real& Height);

            /// @brief Gets the position of this Viewport in it's parent GameWindow in relative units.
            /// @return Returns a Vector2 containing the relative position of this Viewport within the GameWindow.
            Vector2 GetPosition() const;
            /// @brief Gets the size of this Viewport relative to it's parent GameWindow.
            /// @return Returns a Vector2 containing the relative size of this Viewport.
            Vector2 GetSize() const;

            /// @brief Gets the relative left position of the Viewport.
            /// @return Returns a real representing the relative left position of this Viewport.
            Real GetLeft() const;
            /// @brief Gets the relative top position of the Viewport.
            /// @return Returns a real representing the relative top position of this Viewport.
            Real GetTop() const;
            /// @brief Gets the relative width of the Viewport.
            /// @return Returns a real representing the relative width of this Viewport.
            Real GetWidth() const;
            /// @brief Gets the relative height of the Viewport.
            /// @return Returns a real representing the relative height of this Viewport.
            Real GetHeight() const;

            /// @brief Gets the position of this Viewport in it's parent GameWindow in pixels.
            /// @return Returns a Vector2 containing the pixel position of this Viewport within the GameWindow.
            Vector2 GetActualPosition() const;
            /// @brief Gets the size of this Viewport in pixels.
            /// @return Returns a Vector2 containing the pixel size of this Viewport.
            Vector2 GetActualSize() const;

            /// @brief Gets the left position of the Viewport in pixels.
            /// @return Returns a whole representing the left position of this Viewport in pixels.
            Whole GetActualLeft() const;
            /// @brief Gets the top position of the Viewport in pixels.
            /// @return Returns a whole representing the top position of this Viewport in pixels.
            Whole GetActualTop() const;
            /// @brief Gets the width of the Viewport in pixels.
            /// @return Returns a whole representing the width of this Viewport in pixels.
            Whole GetActualWidth() const;
            /// @brief Gets the height of the Viewport in pixels.
            /// @return Returns a whole representing the height of this Viewport in pixels.
            Whole GetActualHeight() const;

            ///////////////////////////////////////////////////////////////////////////////
            // Serialization

            /// @brief Convert this class to an XML::Node ready for serialization.
            /// @param ParentNode The point in the XML hierarchy that all this Viewport should be appended to.
            void ProtoSerialize(XML::Node& ParentNode) const;
            /// @brief Take the data stored in an XML Node and overwrite this object with it.
            /// @param SelfRoot An XML::Node containing the data to populate the new instance with.
            void ProtoDeSerialize(const XML::Node& SelfRoot);
            /// @brief Get the name of the the XML tag the Renderable class will leave behind as its instances are serialized.
            /// @return A string containing the name of this class.
            static String GetSerializableName();

            ///////////////////////////////////////////////////////////////////////////////
            // Internal Methods

            /// @internal
            /// @brief Gets the internal Ogre Viewport.
            /// @return Returns a pointer to the Ogre Viewport this class is based on.
            Ogre::Viewport* _GetOgreViewport() const;
        };//Viewport
    }//Graphics
}//Mezzanine

#endif
