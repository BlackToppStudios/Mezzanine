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
/*
 -----------------------------------------------------------------------------
 This source file is part of ogre-procedural

 For the latest info, see http://code.google.com/p/ogre-procedural/

 Copyright (c) 2010-2013 Michael Broutin

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 -----------------------------------------------------------------------------
 */
#ifndef _graphicsproceduraltexturebuffer_h
#define _graphicsproceduraltexturebuffer_h

#include "colourvalue.h"
#include "Graphics/graphicsenumerations.h"

namespace Mezzanine
{
    namespace Graphics
    {
        class Image;
        class Texture;
        namespace Procedural
        {
            ///////////////////////////////////////////////////////////////////////////////
            /// @brief A convenience buffer that stores pixel colour values of a texture to be generated.
            /// @details
            /// @todo Verify byte order and endian-ness isn't an issue on all supported platforms.
            ///////////////////////////////////////
            class MEZZ_LIB TextureBuffer
            {
            public:
                /// @brief Convenience typedef for the datatype containing the value for each colour channel.
                typedef UInt8 ColourChannelType;
            protected:
                /// @internal
                /// @brief A pointer to the buffer storing all the pixels of this texture buffer.
                ColourChannelType* Pixels;
                /// @internal
                /// @brief The size of the texture to be generated on the X axis.
                Whole Width;
                /// @internal
                /// @brief The size of the texture to be generated on the Y axis.
                Whole Height;

                /// @internal
                /// @brief Gets access to the pixel at the specified position in this buffer.
                /// @exception If the X or Y location go beyond the set size of this texture buffer, then a MM_OUT_OF_BOUNDS_EXCEPTION will be thrown.
                /// @param X The X position of the pixel to access.
                /// @param Y The Y position of the pixel to access.
                /// @param Component The colour component to access.
                /// @return Returns a ColourChannelType reference to the requested component of the requested pixel.
                ColourChannelType& GetPixel(const Whole X, const Whole Y, const UInt16 Component);
                /// @internal
                /// @brief Gets access to the pixel at the specified position in this buffer.
                /// @exception If the X or Y location go beyond the set size of this texture buffer, then a MM_OUT_OF_BOUNDS_EXCEPTION will be thrown.
                /// @param X The X position of the pixel to access.
                /// @param Y The Y position of the pixel to access.
                /// @param Component The colour component to access.
                /// @return Returns a const ColourChannelType reference to the requested component of the requested pixel.
                const ColourChannelType& GetPixel(const Whole X, const Whole Y, const UInt16 Component) const;
            public:
                /// @brief Square size constructor.
                /// @param SquareSize The width and height of the texture to create.  This value will be clamped to be at least 8.
                TextureBuffer(const Whole SquareSize);
                /// @brief Size dimensions constructor.
                /// @param TexWidth The pixel width of the texture to create.  This value will be clamped to be at least 8.
                /// @param TexHeight The pixel height of the texture to create.  This value will be clamped to be at least 8.
                TextureBuffer(const Whole TexWidth, const Whole TexHeight);
                /// @brief Copy constructor.
                /// @param Other The other TextureBuffer to be copied.
                TextureBuffer(const TextureBuffer& Other);
                /// @brief Class destructor.
                ~TextureBuffer();

                ///////////////////////////////////////////////////////////////////////////////
                // Utility

                /// @brief Copies image data from another buffer into this buffer.
                /// @param Other The other TextureBuffer that will having it's contents copied to this.
                void SetData(const TextureBuffer& Other);

                /// @brief Get the pixel width of this texture.
                /// @return Returns a Whole representing the width of this TextureBuffer in pixels.
                Whole GetWidth() const;
                /// @brief Get the pixel height of this texture.
                /// @return Returns a Whole representing the height of this TextureBuffer in pixels.
                Whole GetHeight() const;

                /// @brief Gets the number of pixels in this texture.
                /// @return Returns the total number of pixels in this texture (width * height).
                Whole GetPixelCount() const;
                /// @brief Gets the number of colour channels among all the pixels in this texture.
                /// @return Returns the total number of colour channels in this texture (PixelCount * 4).
                Whole GetSubChannelCount() const;
                /// @brief Gets the size of the internal buffer in bytes.
                /// @return Returns the size of the buffer containing the pixels of the image in bytes.
                Whole GetByteSize() const;

                /// @brief Creates an Image from this buffer.
                /// @remarks The generated image is not owned by this buffer or anything.  It is up to the user/caller of this method to clean it up.
                /// @param Format The pixel format for the generated Image.  Defaults to: Graphics::PF_R8G8B8A8 (Ideal for PNG).
                /// @return Returns a pointer to the converted Image.
                Image* GenerateImage(const Graphics::PixelFormat Format = Graphics::PF_R8G8B8A8) const;
                /// @brief Creates a Texture from this buffer.
                /// @param TexName The name of the Texture to be generated.
                /// @param TexGroup The name of the resource group to create the Texture in.
                /// @param Format The pixel format for the generated Texture.  Defaults to: Graphics::PF_R8G8B8A8 (Ideal for PNG).
                /// @return Returns a pointer to the converted Texture.
                Texture* GenerateTexture(const String& TexName, const String& TexGroup, const Graphics::PixelFormat Format = Graphics::PF_R8G8B8A8) const;

                ///////////////////////////////////////////////////////////////////////////////
                // Pixel Configuration

                /// @brief Set colour of a specified pixel using a ColourValue.
                /// @exception If the X or Y location go beyond the set size of this texture buffer, then a MM_OUT_OF_BOUNDS_EXCEPTION will be thrown.
                /// @param X The X position of pixel to change the colour of.
                /// @param Y The Y position of pixel to change the colour of.
                /// @param Colour The colour to be applied to the specified pixel.
                void SetPixel(const Whole X, const Whole Y, const ColourValue& Colour);
                /// @brief Set colour of a specified pixel using a 0-255 range.
                /// @exception If the X or Y location go beyond the set size of this texture buffer, then a MM_OUT_OF_BOUNDS_EXCEPTION will be thrown.
                /// @param X The X position of pixel to change the colour of.
                /// @param Y The Y position of pixel to change the colour of.
                /// @param Red The amount of Red colour to be applied to the specified pixel.  Range: [0, 255].
                /// @param Green The amount of Green colour to be applied to the specified pixel.  Range: [0, 255].
                /// @param Blue The amount of Blue colour to be applied to the specified pixel.  Range: [0, 255].
                /// @param Alpha The amount of Alpha colour to be applied to the specified pixel.  Range: [0, 255].
                void SetPixelByte(const Whole X, const Whole Y, const ColourChannelType Red, const ColourChannelType Green, const ColourChannelType Blue, const ColourChannelType Alpha = 255);
                /// @brief Set colour of a specified pixel using a 0.0-1.0 range.
                /// @exception If the X or Y location go beyond the set size of this texture buffer, then a MM_OUT_OF_BOUNDS_EXCEPTION will be thrown.
                /// @exception If any of the colour values passed in are outside of the range of 0.0-1.0, then a PARAMETERS_RANGE_EXCEPTION will be thrown.
                /// @param X The X position of pixel to change the colour of.
                /// @param Y The Y position of pixel to change the colour of.
                /// @param Red The amount of Red colour to be applied to the specified pixel.  Range: [0.0, 1.0].
                /// @param Green The amount of Green colour to be applied to the specified pixel.  Range: [0.0, 1.0].
                /// @param Blue The amount of Blue colour to be applied to the specified pixel.  Range: [0.0, 1.0].
                /// @param Alpha The amount of Alpha colour to be applied to the specified pixel.  Range: [0.0, 1.0].
                void SetPixelReal(const Whole X, const Whole Y, const Real Red, const Real Green, const Real Blue, const Real Alpha = 1.0);
                /// @brief Get the colour of a pixel at the specified point.
                /// @exception If the X or Y location go beyond the set size of this texture buffer, then a MM_OUT_OF_BOUNDS_EXCEPTION will be thrown.
                /// @param X The X position of pixel to retrieve.
                /// @param Y The Y position of pixel to retrieve.
                /// @return Returns a ColourValue containing the colour of the specified pixel.
                ColourValue GetPixel(const Whole X, const Whole Y) const;

                /// @brief Sets the red colour value of a specified pixel.
                /// @exception If the X or Y location go beyond the set size of this texture buffer, then a MM_OUT_OF_BOUNDS_EXCEPTION will be thrown.
                /// @param X The X position of pixel to change the colour of.
                /// @param Y The Y position of pixel to change the colour of.
                /// @param Red The amount of red colour to be applied to the specified pixel.  Range: [0, 255].
                void SetRedByte(const Whole X, const Whole Y, const ColourChannelType Red);
                /// @brief Sets the red colour value of a specified pixel.
                /// @exception If the X or Y location go beyond the set size of this texture buffer, then a MM_OUT_OF_BOUNDS_EXCEPTION will be thrown.
                /// @exception If any of the colour values passed in are outside of the range of 0.0-1.0, then a PARAMETERS_RANGE_EXCEPTION will be thrown.
                /// @param X The X position of pixel to change the colour of.
                /// @param Y The Y position of pixel to change the colour of.
                /// @param Red The amount of red colour to be applied to the specified pixel.  Range: [0.0, 1.0].
                void SetRedReal(const Whole X, const Whole Y, const Real Red);
                /// @brief Gets the red colour value of a specified pixel.
                /// @exception If the X or Y location go beyond the set size of this texture buffer, then a MM_OUT_OF_BOUNDS_EXCEPTION will be thrown.
                /// @param X The X position of pixel to retrieve.
                /// @param Y The Y position of pixel to retrieve.
                /// @return Returns a ColourChannelType representing the red component of the colour in the specified pixel.  Range: [0, 255].
                ColourChannelType GetRedByte(const Whole X, const Whole Y) const;
                /// @brief Gets the red colour value of a specified pixel.
                /// @exception If the X or Y location go beyond the set size of this texture buffer, then a MM_OUT_OF_BOUNDS_EXCEPTION will be thrown.
                /// @param X The X position of pixel to retrieve.
                /// @param Y The Y position of pixel to retrieve.
                /// @return Returns a Real representing the red component of the colour in the specified pixel.  Range: [0.0, 1.0].
                Real GetRedReal(const Whole X, const Whole Y) const;

                /// @brief Sets the green colour value of a specified pixel.
                /// @exception If the X or Y location go beyond the set size of this texture buffer, then a MM_OUT_OF_BOUNDS_EXCEPTION will be thrown.
                /// @param X The X position of pixel to change the colour of.
                /// @param Y The Y position of pixel to change the colour of.
                /// @param Green The amount of green colour to be applied to the specified pixel.  Range: [0, 255].
                void SetGreenByte(const Whole X, const Whole Y, const ColourChannelType Green);
                /// @brief Sets the green colour value of a specified pixel.
                /// @exception If the X or Y location go beyond the set size of this texture buffer, then a MM_OUT_OF_BOUNDS_EXCEPTION will be thrown.
                /// @exception If any of the colour values passed in are outside of the range of 0.0-1.0, then a PARAMETERS_RANGE_EXCEPTION will be thrown.
                /// @param X The X position of pixel to change the colour of.
                /// @param Y The Y position of pixel to change the colour of.
                /// @param Green The amount of green colour to be applied to the specified pixel.  Range: [0.0, 1.0].
                void SetGreenReal(const Whole X, const Whole Y, const Real Green);
                /// @brief Gets the green colour value of a specified pixel.
                /// @exception If the X or Y location go beyond the set size of this texture buffer, then a MM_OUT_OF_BOUNDS_EXCEPTION will be thrown.
                /// @param X The X position of pixel to retrieve.
                /// @param Y The Y position of pixel to retrieve.
                /// @return Returns a ColourChannelType representing the green component of the colour in the specified pixel.  Range: [0, 255].
                ColourChannelType GetGreenByte(const Whole X, const Whole Y) const;
                /// @brief Gets the green colour value of a specified pixel.
                /// @exception If the X or Y location go beyond the set size of this texture buffer, then a MM_OUT_OF_BOUNDS_EXCEPTION will be thrown.
                /// @param X The X position of pixel to retrieve.
                /// @param Y The Y position of pixel to retrieve.
                /// @return Returns a Real representing the green component of the colour in the specified pixel.  Range: [0.0, 1.0].
                Real GetGreenReal(const Whole X, const Whole Y) const;

                /// @brief Sets the blue colour value of a specified pixel.
                /// @exception If the X or Y location go beyond the set size of this texture buffer, then a MM_OUT_OF_BOUNDS_EXCEPTION will be thrown.
                /// @param X The X position of pixel to change the colour of.
                /// @param Y The Y position of pixel to change the colour of.
                /// @param Blue The amount of blue colour to be applied to the specified pixel.  Range: [0, 255].
                void SetBlueByte(const Whole X, const Whole Y, const ColourChannelType Blue);
                /// @brief Sets the blue colour value of a specified pixel.
                /// @exception If the X or Y location go beyond the set size of this texture buffer, then a MM_OUT_OF_BOUNDS_EXCEPTION will be thrown.
                /// @exception If any of the colour values passed in are outside of the range of 0.0-1.0, then a PARAMETERS_RANGE_EXCEPTION will be thrown.
                /// @param X The X position of pixel to change the colour of.
                /// @param Y The Y position of pixel to change the colour of.
                /// @param Blue The amount of blue colour to be applied to the specified pixel.  Range: [0.0, 1.0].
                void SetBlueReal(const Whole X, const Whole Y, const Real Blue);
                /// @brief Gets the blue colour value of a specified pixel.
                /// @exception If the X or Y location go beyond the set size of this texture buffer, then a MM_OUT_OF_BOUNDS_EXCEPTION will be thrown.
                /// @param X The X position of pixel to retrieve.
                /// @param Y The Y position of pixel to retrieve.
                /// @return Returns a ColourChannelType representing the blue component of the colour in the specified pixel.  Range: [0, 255].
                ColourChannelType GetBlueByte(const Whole X, const Whole Y) const;
                /// @brief Gets the blue colour value of a specified pixel.
                /// @exception If the X or Y location go beyond the set size of this texture buffer, then a MM_OUT_OF_BOUNDS_EXCEPTION will be thrown.
                /// @param X The X position of pixel to retrieve.
                /// @param Y The Y position of pixel to retrieve.
                /// @return Returns a Real representing the blue component of the colour in the specified pixel.  Range: [0.0, 1.0].
                Real GetBlueReal(const Whole X, const Whole Y) const;

                /// @brief Sets the alpha colour value of a specified pixel.
                /// @exception If the X or Y location go beyond the set size of this texture buffer, then a MM_OUT_OF_BOUNDS_EXCEPTION will be thrown.
                /// @param X The X position of pixel to change the colour of.
                /// @param Y The Y position of pixel to change the colour of.
                /// @param Alpha The amount of alpha colour to be applied to the specified pixel.  Range: [0, 255].
                void SetAlphaByte(const Whole X, const Whole Y, const ColourChannelType Alpha);
                /// @brief Sets the alpha colour value of a specified pixel.
                /// @exception If the X or Y location go beyond the set size of this texture buffer, then a MM_OUT_OF_BOUNDS_EXCEPTION will be thrown.
                /// @exception If any of the colour values passed in are outside of the range of 0.0-1.0, then a PARAMETERS_RANGE_EXCEPTION will be thrown.
                /// @param X The X position of pixel to change the colour of.
                /// @param Y The Y position of pixel to change the colour of.
                /// @param Alpha The amount of alpha colour to be applied to the specified pixel.  Range: [0.0, 1.0].
                void SetAlphaReal(const Whole X, const Whole Y, const Real Alpha);
                /// @brief Gets the alpha colour value of a specified pixel.
                /// @exception If the X or Y location go beyond the set size of this texture buffer, then a MM_OUT_OF_BOUNDS_EXCEPTION will be thrown.
                /// @param X The X position of pixel to retrieve.
                /// @param Y The Y position of pixel to retrieve.
                /// @return Returns a ColourChannelType representing the alpha component of the colour in the specified pixel.  Range: [0, 255].
                ColourChannelType GetAlphaByte(const Whole X, const Whole Y) const;
                /// @brief Gets the alpha colour value of a specified pixel.
                /// @exception If the X or Y location go beyond the set size of this texture buffer, then a MM_OUT_OF_BOUNDS_EXCEPTION will be thrown.
                /// @param X The X position of pixel to retrieve.
                /// @param Y The Y position of pixel to retrieve.
                /// @return Returns a Real representing the alpha component of the colour in the specified pixel.  Range: [0.0, 1.0].
                Real GetAlphaReal(const Whole X, const Whole Y) const;
            };//TextureBuffer

            ///////////////////////////////////////////////////////////////////////////////
            /// @brief Convenience class for pixel processing.
            /// @details
            ///////////////////////////////////////
            struct IntVector2
            {
                /// @brief Position on the X axis.
                Whole X;
                /// @brief Position on the Y axis.
                Whole Y;
            };//IntVector2
        }//Procedural
    }//Graphics
}//Mezzanine

#endif
