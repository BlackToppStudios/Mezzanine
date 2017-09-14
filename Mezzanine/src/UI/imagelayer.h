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

#ifndef _uiimagelayer_h
#define _uiimagelayer_h

#include "UI/renderlayer.h"
#include "UI/sprite.h"

namespace Mezzanine
{
    namespace UI
    {
        ///////////////////////////////////////////////////////////////////////////////
        /// @brief This layer is for placing images and basic colours in it's parents' quad space.
        /// @details
        ///////////////////////////////////////
        class MEZZ_LIB ImageLayer : public RenderLayer
        {
        protected:
            /// @internal
            /// @brief An array of colours to be used for each corner section of the border being rendered with this layer.
            ColourValue BorderColours[4];
            /// @internal
            /// @brief The width of the border to render around this layer in pixels.
            Real BorderWidth;

            /// @internal
            /// @brief Draws the border, if any is set.
            /// @param InnerRect An array of 4 Vector2s storing the 4 points of the inner rect for the border.
            /// @param OuterRect An array of 4 Vector2s storing the 4 points of the outer rect for the border.
            void DrawBorder(const Vector2* InnerRect, const Vector2* OuterRect);
            /// @internal
            /// @brief Draws the main rectangle.
            /// @param FillRect An array of 4 Vector2s storing the 4 points of the rect to render.
            /// @param FillSprite A pointer to the Sprite that will be rendered within the provided rect.
            /// @param FillColours An array of 4 ColourValues storing the colour to render at each corner.
            void DrawFill(const Vector2* FillRect, const Sprite* FillSprite, const ColourValue* FillColours);
        public:
            /// @brief Class constructor.
            /// @param ParentRenderable The renderable that created this layer.
            ImageLayer(QuadRenderable* ParentRenderable);
            /// @brief Class destructor.
            virtual ~ImageLayer();

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
