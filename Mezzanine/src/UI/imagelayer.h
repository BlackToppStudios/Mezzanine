// © Copyright 2010 - 2014 BlackTopp Studios Inc.
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

#ifndef _uiimagelayer_h
#define _uiimagelayer_h

#include "UI/renderlayer.h"
#include "UI/sprite.h"

namespace Mezzanine
{
    namespace UI
    {
        ///////////////////////////////////////////////////////////////////////////////
        /// @class ImageLayer
        /// @headerfile imagelayer.h
        /// @brief This layer is for placing images and basic colours in it's parents' quad space.
        /// @details
        ///////////////////////////////////////
        class MEZZ_LIB ImageLayer : public RenderLayer
        {
        protected:
            /// @internal
            /// @brief An array of colours to be used for each corner section of this layer.
            ColourValue FillColours[4];
            /// @internal
            /// @brief An array of colours to be used for each corner section of the border being rendered with this layer.
            ColourValue BorderColours[4];
            /// @internal
            /// @brief A pointer to the sprite that will be rendered by this layer.
            Sprite* LayerSprite;
            /// @internal
            /// @brief The width of the border to render around this layer in pixels.
            Real BorderWidth;

            /// @copydoc SimpleRenderer::RedrawImpl(bool Force)
            virtual void RedrawImpl(bool Force);
            /// @internal
            /// @brief Draws the border, if any is set.
            void DrawBorder(const Vector2& TopLeft, const Vector2& TopRight, const Vector2& BottomLeft, const Vector2& BottomRight,
                            const Vector2& OuterTopLeft, const Vector2& OuterTopRight, const Vector2& OuterBottomLeft, const Vector2& OuterBottomRight);
            /// @internal
            /// @brief Draws the main rectangle.
            void DrawFill(const Vector2& TopLeft, const Vector2& TopRight, const Vector2& BottomLeft, const Vector2& BottomRight);
        public:
            /// @brief Class constructor.
            /// @param ParentRenderable The renderable that created this layer.
            ImageLayer(QuadRenderable* ParentRenderable);
            /// @brief Class destructor.
            virtual ~ImageLayer();

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            /// @copydoc RenderLayer::GetLayerType() const
            virtual UI::RenderLayerType GetLayerType() const;

            ///////////////////////////////////////////////////////////////////////////////
            // Fill Methods

            /// @brief Sets the colour of the layer.
            /// @param Colour A colour value representing the colour to be set.
            virtual void SetColour(const ColourValue& Colour);
            /// @brief Sets the colour for one corner the layer.
            /// @param Corner Which corner the colour should be applied to.
            /// @param Colour A colour value representing the colour to be set.
            virtual void SetColour(const UI::QuadCorner Corner, const ColourValue& Colour);
            /// @brief Sets the fill image(if provided in the atlas) of the layer.
            /// @param PSprite A pointer to the sprite to set as the fill.
            virtual void SetSprite(Sprite* PSprite);
            /// @brief Sets the fill image(if provided in the atlas) of the layer.
            /// @param SpriteName The name of the sprite to set as the fill.
            virtual void SetSprite(const String& SpriteName);
            /// @brief Sets the fill image(if provided in the atlas) of the layer.
            /// @param SpriteName The name of the sprite to set as the fill.
            /// @param Atlas The Atlas to load the sprite from.
            virtual void SetSprite(const String& SpriteName, const String& Atlas);
            /// @brief Sets a colour gradient to be applied to this layer.
            /// @param Grad The direction/style of gradient to enable.
            /// @param ColourA The colour to apply to the first side of the gradient.
            /// @param ColourB The colour to apply to the second side of the gradient.
            virtual void SetGradient(const UI::Gradient Grad, const ColourValue& ColourA, const ColourValue& ColourB);
            /// @brief Gets the colour of a corner of this layer.
            /// @param Corner The corner to get the colour for.
            /// @return Returns the colour of the specified corner.
            virtual const ColourValue& GetColour(const UI::QuadCorner Corner) const;
            /// @brief Gets whether or not this layer is entirely transparent.
            /// @return Returns true if all quad corners of this layer have their value set to zero, false otherwise.
            virtual bool IsCompletelyTransparent() const;

            ///////////////////////////////////////////////////////////////////////////////
            // Border Methods

            /// @brief Sets the width of the border for this renderable.
            /// @param Width The width to set for the border.
            virtual void SetBorderWidth(const Real Width);
            /// @brief Sets the colour of the border for this renderable.
            /// @param Colour The colour to set for the border.
            virtual void SetBorderColour(const ColourValue& Colour);
            /// @brief Sets the colour of one side of the border for this renderable.
            /// @param Side The side to have the colour applied to.
            /// @param Colour The colour to be applied.
            virtual void SetBorderColour(const UI::Border Side, const ColourValue& Colour);
            /// @brief Sets all parameters for enabling a border.
            /// @param Width The width to set for the border.
            /// @param Colour The colour to be applied to all sides of the border.
            virtual void SetBorder(const Real Width, const ColourValue& Colour);
            /// @brief Sets all parameters for enabling a border.
            /// @param Width The width to set for the border.
            /// @param North The colour to apply to the north side of the border.
            /// @param South The colour to apply to the south side of the border.
            /// @param East The colour to apply to the east side of the border.
            /// @param West The colour to apply to the west side of the border.
            virtual void SetBorder(const Real Width, const ColourValue& North, const ColourValue& South, const ColourValue& East, const ColourValue& West);
            /// @brief Disables any border set on this renderable if one is currently set.
            virtual void NoBorder();
            /// @brief Gets the border width of this renderable.
            /// @return Returns a Real with the currently set width of the border.
            virtual Real GetBorderWidth() const;
            /// @brief Gets the colour of a border on this renderable.
            /// @param Side The side to retrieve the colour for.
            /// @return Returns the colour of the border on the specified side.
            virtual const ColourValue& GetBorderColour(const UI::Border Side) const;

            ///////////////////////////////////////////////////////////////////////////////
            // Serialization

            /// @copydoc SimpleRenderer::ProtoSerializeProperties(XML::Node&) const
            virtual void ProtoSerializeProperties(XML::Node& SelfRoot) const;
            /// @copydoc SimpleRenderer::ProtoDeSerializeProperties(const XML::Node&)
            virtual void ProtoDeSerializeProperties(const XML::Node& SelfRoot);

            /// @copydoc SimpleRenderer::GetDerivedSerializableName() const
            virtual String GetDerivedSerializableName() const;
            /// @copydoc SimpleRenderer::GetSerializableName()
            static String GetSerializableName();
        };//ImageLayer
    }//UI
}//Mezzanine

#endif
