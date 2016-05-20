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

#ifndef _uimultiimagelayer_h
#define _uimultiimagelayer_h

#include "UI/imagelayer.h"
#include "UI/unifieddim.h"

namespace Mezzanine
{
    namespace UI
    {
        ///////////////////////////////////////////////////////////////////////////////
        /// @brief This is an image layer that supports rendering of multiple images within it's space.
        /// @details
        ///////////////////////////////////////
        class MEZZ_LIB MultiImageData
        {
        public:
            ///////////////////////////////////////////////////////////////////////////////
            // Public Data Members

            /// @brief An array of ColourValues containing the colours to be applied to each corner of the sprite quad.
            ColourValue FillColours[4];
            /// @brief The rect describing the size and position for the sprite to be rendered.
            UnifiedRect FillRect;
            /// @brief A pointer to the sprite to be rendered.
            Sprite* LayerSprite;

            ///////////////////////////////////////////////////////////////////////////////
            // Construction and Destruction

            /// @brief Class constructor.
            MultiImageData();
            /// @brief Class destructor.
            ~MultiImageData();

            /*///////////////////////////////////////////////////////////////////////////////
            // Serialization

            /// @brief Convert this class to an XML::Node ready for serialization.
            /// @param ParentNode The point in the XML hierarchy that all this MultiImageData should be appended to.
            void ProtoSerialize(XML::Node& ParentNode) const;
            /// @brief Take the data stored in an XML Node and overwrite this object with it.
            /// @param SelfRoot An XML::Node containing the data to populate the new instance with.
            void ProtoDeSerialize(const XML::Node& SelfRoot);
            /// @brief Get the name of the the XML tag the class will leave behind as its instances are serialized.
            /// @return A string containing the name of this class.
            static String GetSerializableName();// */
        };//MultiImageData

        ///////////////////////////////////////////////////////////////////////////////
        /// @brief This is an image layer that supports rendering of multiple images within it's space.
        /// @details
        ///////////////////////////////////////
        class MEZZ_LIB MultiImageLayer : public ImageLayer
        {
        public:
            /// @brief Container type for the image metadata storage in this class.
            typedef std::vector< MultiImageData >               ImageDataContainer;
            /// @brief Iterator type for image metadata being stored by this class.
            typedef ImageDataContainer::iterator                ImageDataIterator;
            /// @brief ConstIterator type for image metadata being stored by this class.
            typedef ImageDataContainer::const_iterator          ConstImageDataIterator;
        protected:
            /// @internal
            /// @brief Container storing all of the images/sprites that will be rendered by this layer.
            ImageDataContainer LayerImages;

            /// @copydoc SimpleRenderer::RedrawImpl(Boole Force)
            virtual void RedrawImpl(Boole Force);
        public:
            /// @brief Class constructor.
            /// @param ParentRenderable The renderable that created this layer.
            MultiImageLayer(QuadRenderable* ParentRenderable);
            /// @brief Class destructor.
            virtual ~MultiImageLayer();

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            /// @copydoc RenderLayer::GetLayerType() const
            virtual UI::RenderLayerType GetLayerType() const;

            ///////////////////////////////////////////////////////////////////////////////
            // MultiImageData Configuration

            /// @brief Creates space for the specified number of MultiImageData instances to be populated.
            /// @param NumImages The number of images to reserve space in this layer for.
            virtual void ReserveMultiImageData(const Whole NumImages);
            /// @brief Gets a MultiImageData in this layer by index.
            /// @param Index The index of the MultiImageData to retrieve.
            /// @return Returns a reference to the MultiImageData at the specified index.
            virtual MultiImageData& GetMultiImageData(const Whole Index);
            /// @brief Gets a MultiImageData in this layer by index.
            /// @param Index The index of the MultiImageData to retrieve.
            /// @return Returns a const reference to the MultiImageData at the specified index.
            virtual const MultiImageData& GetMultiImageData(const Whole Index) const;
            /// @brief Gets the number of MultiImageData instances currently in this layer.
            /// @return Returns the number of images this layer is currently configured to support.
            virtual Whole GetNumMultiImageData() const;

            ///////////////////////////////////////////////////////////////////////////////
            // Fill Methods

            /// @brief Sets the Unified position and size of the image at the specified index.
            /// @param Index The index of the MultiImageData to update.
            /// @param Trans The UnifiedRect to be applied to the Image at the specified index.
            virtual void SetImageRect(const Whole Index, const UnifiedRect& Trans);
            /// @brief Gets the Unified position and size of the image at the specified index.
            /// @param Index The index of the MultiImageData to update.
            /// @return Returns a const Reference to the UnifiedRect to be applied to the image at the specified index.
            virtual const UnifiedRect& GetImageRect(const Whole Index) const;
            /// @brief Sets the Unified position of the image at the specified index.
            /// @param Index The index of the MultiImageData to update.
            /// @param Position The UnifiedVec2 describing the position to be applied to the image at the specified index.
            virtual void SetImagePosition(const Whole Index, const UnifiedVec2& Position);
            /// @brief Gets the Unified position of the image at the specified index.
            /// @param Index The index of the MultiImageData to update.
            /// @return Returns a const Reference to the Unified position to be applied to the image at the specified index.
            virtual const UnifiedVec2& GetImagePosition(const Whole Index) const;
            /// @brief Sets the Unified size of the image at the specified index.
            /// @param Index The index of the MultiImageData to update.
            /// @param Size The UnifiedVec2 describing the size to be applied to the image at the specified index.
            virtual void SetImageSize(const Whole Index, const UnifiedVec2& Size);
            /// @brief Gets the Unified size of the image at the specified index.
            /// @param Index The index of the MultiImageData to update.
            /// @return Returns a const Reference to the Unified size to be applied to the image at the specified index.
            virtual const UnifiedVec2& GetImageSize(const Whole Index) const;

            /// @brief Sets the colour of the layer.
            /// @param Index The index of the MultiImageData to update.
            /// @param Colour A colour value representing the colour to be set.
            virtual void SetColour(const Whole Index, const ColourValue& Colour);
            /// @brief Sets the colour for one corner the layer.
            /// @param Index The index of the MultiImageData to update.
            /// @param Corner Which corner the colour should be applied to.
            /// @param Colour A colour value representing the colour to be set.
            virtual void SetColour(const Whole Index, const UI::QuadCorner Corner, const ColourValue& Colour);
            /// @brief Gets the colour of a corner of this layer.
            /// @param Index The index of the MultiImageData to update.
            /// @param Corner The corner to get the colour for.
            /// @return Returns the colour of the specified corner.
            virtual const ColourValue& GetColour(const Whole Index, const UI::QuadCorner Corner) const;
            /// @brief Gets whether or not this layer is entirely transparent.
            /// @param Index The index of the MultiImageData to update.
            /// @return Returns true if all quad corners of this layer have their value set to zero, false otherwise.
            virtual Boole IsCompletelyTransparent(const Whole Index) const;

            /// @brief Sets the fill image(if provided in the atlas) of the layer.
            /// @param Index The index of the MultiImageData to update.
            /// @param NewSprite A pointer to the sprite to set as the fill.
            virtual void SetSprite(const Whole Index, Sprite* NewSprite);
            /// @brief Sets the fill image(if provided in the atlas) of the layer.
            /// @param Index The index of the MultiImageData to update.
            /// @param SpriteName The name of the sprite to set as the fill.
            virtual void SetSprite(const Whole Index, const String& SpriteName);
            /// @brief Sets the fill image(if provided in the atlas) of the layer.
            /// @param Index The index of the MultiImageData to update.
            /// @param SpriteName The name of the sprite to set as the fill.
            /// @param Atlas The Atlas to load the sprite from.
            virtual void SetSprite(const Whole Index, const String& SpriteName, const String& Atlas);

            /// @brief Sets a colour gradient to be applied to this layer.
            /// @param Index The index of the MultiImageData to update.
            /// @param Grad The direction/style of gradient to enable.
            /// @param ColourA The colour to apply to the first side of the gradient.
            /// @param ColourB The colour to apply to the second side of the gradient.
            virtual void SetGradient(const Whole Index, const UI::Gradient Grad, const ColourValue& ColourA, const ColourValue& ColourB);

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
        };//MultiImageLayer
    }//UI
}//Mezzanine

#endif
