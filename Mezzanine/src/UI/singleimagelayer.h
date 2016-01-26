// © Copyright 2010 - 2016 BlackTopp Studios Inc.
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

#ifndef _uisingleimagelayer_h
#define _uisingleimagelayer_h

#include "UI/imagelayer.h"

namespace Mezzanine
{
    namespace UI
    {
        ///////////////////////////////////////////////////////////////////////////////
        /// @brief This is an image layer that supports rendering only a single image/sprite.
        /// @details
        ///////////////////////////////////////
        class MEZZ_LIB SingleImageLayer : public ImageLayer
        {
        protected:
            /// @internal
            /// @brief An array of colours to be used for each corner section of this layer.
            ColourValue FillColours[4];
            /// @internal
            /// @brief A pointer to the sprite that will be rendered by this layer.
            Sprite* LayerSprite;

            /// @copydoc SimpleRenderer::RedrawImpl(Boole Force)
            virtual void RedrawImpl(Boole Force);
        public:
            /// @brief Class constructor.
            /// @param ParentRenderable The renderable that created this layer.
            SingleImageLayer(QuadRenderable* ParentRenderable);
            /// @brief Class destructor.
            virtual ~SingleImageLayer();

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
            /// @brief Gets the colour of a corner of this layer.
            /// @param Corner The corner to get the colour for.
            /// @return Returns the colour of the specified corner.
            virtual const ColourValue& GetColour(const UI::QuadCorner Corner) const;
            /// @brief Gets whether or not this layer is entirely transparent.
            /// @return Returns true if all quad corners of this layer have their value set to zero, false otherwise.
            virtual Boole IsCompletelyTransparent() const;

            /// @brief Sets the fill image(if provided in the atlas) of the layer.
            /// @param NewSprite A pointer to the sprite to set as the fill.
            virtual void SetSprite(Sprite* NewSprite);
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
        };//SingleImageLayer
    }//UI
}//Mezzanine

#endif
