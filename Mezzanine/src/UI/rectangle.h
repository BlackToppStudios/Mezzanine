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
#ifndef _uirectangle_h
#define _uirectangle_h

#include "colourvalue.h"
#include "uienumerations.h"
#include "UI/renderablerect.h"
#include "UI/basicrenderable.h"

namespace Mezzanine
{
    class UIManager;
    namespace UI
    {
        class Layer;
        class ScreenVertexData;
        ///////////////////////////////////////////////////////////////////////////////
        /// @class Rectangle
        /// @headerfile uirectangle.h
        /// @brief This class is a helper class, specifically for use with background sprites and colours.
        /// @details Unlike a button, this class cannot be interacted with by clicking.
        /// It is important to understand what you want your space to do when selecting the class to use.
        ///////////////////////////////////////
        class MEZZ_LIB Rectangle : public BasicRenderable
        {
            protected:
                bool MouseHover;
                bool CustomCenter;
                Real BorderWidth;
                Real RotAngle;
                Vector2 ActPosition;
                Vector2 ActSize;
                Vector2 RelPosition;
                Vector2 RelSize;
                Vector2	RotCenter;
                Vector2 UVs[4];
                ColourValue BackgroundColours[4];
                ColourValue BorderColours[4];

                /// @brief Builds the rectangle.
                virtual void ConstructRectangle(const UI::RenderableRect& Rect);
                /// @brief Draws the border, if any is set.
                virtual void DrawBorder(const Vector2& TopLeft, const Vector2& TopRight, const Vector2& BottomLeft, const Vector2& BottomRight,
                                        const Vector2& OuterTopLeft, const Vector2& OuterTopRight, const Vector2& OuterBottomLeft, const Vector2& OuterBottomRight);
                /// @brief Draws the main rectangle.
                virtual void DrawFill(const Vector2& TopLeft, const Vector2& TopRight, const Vector2& BottomLeft, const Vector2& BottomRight);
                /// @brief Transforms coordinates to be used in rendering.
                virtual void RotationTransform(Vector2& TopLeft, Vector2& TopRight, Vector2& BottomLeft, Vector2& BottomRight);
                /// @brief Sets the hovered state of this renderable.
                virtual void SetHovered(bool Hovered);
                /// @brief Sets all the sprite meta-data of this class to render a specific sprite.
                virtual void SetSprite(Sprite* PSprite);
                /// @brief Inheritance constructor.
                /// @param name The name to be given to this renderable.
                /// @param Rect The Rect representing the position and size of the rectangle.
                /// @param Layer Pointer to the parent Layer that created this rectangle.
                Rectangle(const String& name, const RenderableRect& Rect, Layer* PLayer);
            public:
                /// @brief Class constructor.
                /// @param Rect The Rect representing the position and size of the rectangle.
                /// @param Layer Pointer to the parent Layer that created this rectangle.
                Rectangle(const RenderableRect& Rect, Layer* PLayer);
                /// @brief Class destructor.
                virtual ~Rectangle();

                /// @brief Determines whether the mouse is over this renderable.
                /// @return Returns a bool indicating whether the mouse is over this renderable.
                virtual bool CheckMouseHover();
                /// @brief Gets the stored value of whether or not the mouse is over the renderable.
                /// @details This function does not perform any checks.  If you want to do a manual check, call CheckMouseHover().
                /// @return Returns the stored value of whether or not the mouse is over the renderable.
                virtual bool GetMouseHover();

                /// @brief Sets the background colour of the renderable.
                /// @param Colour A colour value representing the colour to be set.
                virtual void SetBackgroundColour(const ColourValue& Colour);
                /// @brief Sets the background colour for one corner the renderable.
                /// @param Corner Which corner the colour should be applied to.
                /// @param Colour A colour value representing the colour to be set.
                virtual void SetBackgroundColour(const UI::QuadCorner& Corner, const ColourValue& Colour);
                /// @brief Sets the background image(if provided in the atlas) of the renderable.
                /// @param PSprite A pointer to the sprite to set as the background.
                virtual void SetBackgroundSprite(Sprite* PSprite);
                /// @brief Sets the background image(if provided in the atlas) of the renderable.
                /// @param SpriteName The name of the sprite to set as the background.
                virtual void SetBackgroundSprite(const String& SpriteName);
                /// @brief Sets the background image(if provided in the atlas) of the renderable.
                /// @param SpriteName The name of the sprite to set as the background.
                /// @param Atlas The Atlas to load the sprite from.
                virtual void SetBackgroundSprite(const String& SpriteName, const String& Atlas);
                /// @brief Sets a colour gradient to be applied to this renderable.
                /// @param Grad The direction/style of gradient to enable.
                /// @param ColourA The colour to apply to the first side of the gradient.
                /// @param ColourB The colour to apply to the second side of the gradient.
                virtual void SetBackgroundGradient(const UI::Gradient& Grad, const ColourValue& ColourA, const ColourValue& ColourB);
                /// @brief Gets the colour of a corner of this renderable.
                /// @param Corner The corner to get the colour for.
                /// @return Returns the colour of the specified corner.
                virtual ColourValue GetBackgroundColour(const UI::QuadCorner& Corner) const;

                /// @brief Sets the width of the border for this renderable.
                /// @param Width The width to set for the border.
                virtual void SetBorderWidth(const Real& Width);
                /// @brief Sets the colour of the border for this renderable.
                /// @param Colour The colour to set for the border.
                virtual void SetBorderColour(const ColourValue& Colour);
                /// @brief Sets the colour of one side of the border for this renderable.
                /// @param Side The side to have the colour applied to.
                /// @param Colour The colour to be applied.
                virtual void SetBorderColour(const UI::Border& Side, const ColourValue& Colour);
                /// @brief Sets all parameters for enabling a border.
                /// @param Width The width to set for the border.
                /// @param Colour The colour to be applied to all sides of the border.
                virtual void SetBorder(const Real& Width, const ColourValue& Colour);
                /// @brief Sets all parameters for enabling a border.
                /// @param Width The width to set for the border.
                /// @param North The colour to apply to the north side of the border.
                /// @param South The colour to apply to the south side of the border.
                /// @param East The colour to apply to the east side of the border.
                /// @param West The colour to apply to the west side of the border.
                virtual void SetBorder(const Real& Width, const ColourValue& North, const ColourValue& South, const ColourValue& East, const ColourValue& West);
                /// @brief Disables any border set on this renderable if one is currently set.
                virtual void NoBorder();
                /// @brief Gets the border width of this renderable.
                /// @return Returns a Real with the currently set width of the border.
                virtual Real GetBorderWidth() const;
                /// @brief Gets the colour of a border on this renderable.
                /// @param Side The side to retrieve the colour for.
                /// @return Returns the colour of the border on the specified side.
                virtual ColourValue GetBorderColour(const UI::Border& Side) const;

                /// @brief Sets the rotation to be applied to this renderable.
                /// @param Degrees The angle of rotation in degrees.
                virtual void SetRotationDegrees(const Real& Degrees);
                /// @brief Sets the rotation to be applied to this renderable.
                /// @param Radians The angle of rotation in radians.
                virtual void SetRotationRadians(const Real& Radians);
                /// @brief Gets the current rotation applied to this renderable in degrees.
                /// @return Returns a Real with the current rotation.
                virtual Real GetRotationDegrees() const;
                /// @brief Gets the current rotation applied to this renderable in radians.
                /// @return Returns a Real with the current rotation.
                virtual Real GetRotationRadians() const;
                /// @brief Sets the point around which this renderable should rotate.
                /// @remarks By default, when rotating a renderable it will rotate around the center of it's quad.
                /// Using this however you can rotate it around any point in screen space.  When disabling this feature,
                /// you can simple pass in "false" as the only arguement.
                /// @param Custom Whether to enable or disable a custom rotation point.
                /// @param Center The point in screen space where this renderable should rotate around.
                virtual void SetRotationCenter(bool Custom, const Vector2& Center = Vector2());
                /// @brief Gets the current set rotation point.
                /// @return Returns a vector 2 with the point around which this renderable rotates in screen space.
                virtual Vector2 GetRotationCenter() const;

                /// @brief Sets the relative top left position of this renderable.
                /// @param Position A Vector2 representing the location of this renderable.
                virtual void SetPosition(const Vector2& Position);
                /// @brief Gets the relative top left position of this renderable.
                /// @return Returns a Vector2 representing the location of this renderable.
                virtual Vector2 GetPosition() const;
                /// @brief Sets the top left position of this renderable in pixels.
                /// @param Position A Vector2 representing the location of this renderable.
                virtual void SetActualPosition(const Vector2& Position);
                /// @brief Gets the top left position of this renderable in pixels.
                /// @return Returns a Vector2 representing the location of this renderable.
                virtual Vector2 GetActualPosition() const;
                /// @brief Sets the relative size of this renderable.
                /// @param Size A vector2 representing the size of this renderable.
                virtual void SetSize(const Vector2& Size);
                /// @brief Gets the relative size of this renderable.
                /// @return Returns a vector2 representing the size of this renderable.
                virtual Vector2 GetSize() const;
                /// @brief Sets the size of this renderable in pixels.
                /// @param Size A vector2 representing the size of this renderable.
                virtual void SetActualSize(const Vector2& Size);
                /// @brief Gets the size of this renderable in pixels.
                /// @return Returns a vector2 representing the size of this renderable.
                virtual Vector2 GetActualSize() const;

                /// @brief Updates the dimensions of this renderable to match those of the new screen size.
                /// @details This function is called automatically when a viewport changes in size, and shouldn't need to be called manually.
                virtual void UpdateDimensions();
                ///////////////////////////////////////////////////////////////////////////////
                // Internal Functions
                ///////////////////////////////////////
                /// @copydoc UI::BasicRenderable::_Redraw()
                virtual void _Redraw();
                /// @copydoc UI::BasicRenderable::_AppendVertices()
                virtual void _AppendVertices(ScreenVertexData& Vertices);
        };//rectangle
    }//UI
}//Mezzanine

#endif
