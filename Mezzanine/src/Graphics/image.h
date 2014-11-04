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

#ifndef _graphicsimage_h
#define _graphicsimage_h

#include "datatypes.h"
#include "colourvalue.h"
#include "Graphics/graphicsenumerations.h"


namespace Ogre
{
    class Image;
}//Ogre

namespace Mezzanine
{
    namespace Graphics
    {
        class InternalImageData;
        ///////////////////////////////////////////////////////////////////////////////
        /// @brief This class represents an image loaded into system memory.
        /// @details
        ///////////////////////////////////////
        class MEZZ_LIB Image
        {
        protected:
            /// @internal
            /// @brief A pointer to the internal implementation of the Image.
            InternalImageData* IID;
        public:
            /// @brief Resource constructor.
            /// @param ResourceName The name of the resource to be loaded.
            /// @param ResourceGroup The name of the group the resource is located in.
            Image(const String& ResourceName, const String& ResourceGroup);
            /// @brief 2D image buffer constructor.
            /// @param Data The buffer to load the image from.
            /// @param Width The width of the image to be loaded.
            /// @param Height The height of the image to be loaded.
            /// @param Format The pixel format of the buffer being loaded.
            /// @param AutoDelete Whether or not this image owns the buffer, allowing it to delete the buffer when it is done.
            /// @param NumFaces The number of faces that exist for the image.
            /// @param NumMipMaps The number of mipmaps that exist for the image.
            Image(UInt8* Data, const UInt32 Width, const UInt32 Height, const Graphics::PixelFormat Format, const Boole AutoDelete = false, const Whole NumFaces = 1, const UInt8 NumMipMaps = 0);
            /// @brief 3D image buffer constructor.
            /// @param Data The buffer to load the image from.
            /// @param Width The width of the image to be loaded.
            /// @param Height The height of the image to be loaded.
            /// @param Depth The depth of the image to be loaded.
            /// @param Format The pixel format of the buffer being loaded.
            /// @param AutoDelete Whether or not this image owns the buffer, allowing it to delete the buffer when it is done.
            /// @param NumFaces The number of faces that exist for the image.
            /// @param NumMipMaps The number of mipmaps that exist for the image.
            Image(UInt8* Data, const UInt32 Width, const UInt32 Height, const UInt32 Depth, const Graphics::PixelFormat Format, const Boole AutoDelete = false, const Whole NumFaces = 1, const UInt8 NumMipMaps = 0);
            /// @brief Class Destructor.
            ~Image();

            ///////////////////////////////////////////////////////////////////////////////
            // Utility Methods

            /// @brief Gets the width of this image in pixels.
            /// @return Returns a UInt32 representing the size of this image on the X axis.
            UInt32 GetWidth() const;
            /// @brief Gets the height of this image in pixels.
            /// @return Returns a UInt32 representing the size of this image on the Y axis.
            UInt32 GetHeight() const;
            /// @brief Gets the depth of this image in pixels.
            /// @return Returns a UInt32 representing the size of this image on the Z axis.
            UInt32 GetDepth() const;
            /// @brief Gets the pixel format of this image.
            /// @return Returns a PixelFormat enum value representing how the bits of this image are layed out.
            Graphics::PixelFormat GetFormat() const;
            /// @brief Gets the number of bits allocated for each pixel in this image.
            /// @return Returns a UInt8 containing the size of each pixel in bits.
            UInt8 GetBitsPerPixel() const;
            /// @brief Gets the number of faces for thie image.
            /// @remarks Generally this will either be 1 for a normal image, or 6 for a cubemap.
            /// @return Returns a Whole representing the number of faces this image has.
            Whole GetNumFaces() const;
            /// @brief Gets the number of MipMaps that exist for this image.
            /// @return Returns the number of MipMaps this image has.
            Whole GetNumMipMaps() const;

            /// @brief Gets the raw image data.
            /// @return Returns a pointer to the array storing the raw data for this image.
            UInt8* GetImageData();
            /// @brief Gets the raw image data.
            /// @return Returns a const pointer to the array storing the raw data for this image.
            const UInt8* GetImageData() const;
            /// @brief Gets the size of this image.
            /// @return Returns a Whole representing the total size of this image in bytes.
            Whole GetSize() const;
            /// @brief Gets the size of a row of pixels in this image.
            /// @return Returns a Whole representing the byte size of a row of pixels in this image.
            Whole GetRowSpan() const;

            /// @brief Reverses the order of each pixel on the X axis.
            /// @return Returns a reference to this.
            Image& FlipAroundXAxis();
            /// @brief Reverses the order of each pixel on the Y axis.
            /// @return Returns a reference to this.
            Image& FlipAroundYAxis();

            /// @brief Sets the colour of an individual pixel in this image.
            /// @param X The X position of the pixel to alter.
            /// @param Y The Y position of the pixel to alter.
            /// @param Z The Z position of the pixel to alter.
            /// @param Colour The colour to set for the pixel at the specified position.
            void SetColourAt(const Whole X, const Whole Y, const Whole Z, const ColourValue& Colour);
            /// @brief Gets the colour of an individual pixel in this image.
            /// @param X The X position of the pixel to retrieve.
            /// @param Y The Y position of the pixel to retrieve.
            /// @param Z The Z position of the pixel to retrieve.
            /// @return Returns a ColourValue containing the colour of the pixel at the specified position.
            ColourValue GetColourAt(const Whole X, const Whole Y, const Whole Z) const;

            ///////////////////////////////////////////////////////////////////////////////
            // Loading Methods

            /// @brief Loads an image.
            /// @param ResourceName The name of the resource to be loaded.
            /// @param ResourceGroup The name of the group the resource is located in.
            /// @return Returns a reference to this.
            Image& LoadImage(const String& ResourceName, const String& ResourceGroup);
            /// @brief Loads an image from a buffer.
            /// @param Data The buffer to load the image from.
            /// @param Width The width of the image to be loaded.
            /// @param Height The height of the image to be loaded.
            /// @param Format The pixel format of the buffer being loaded.
            /// @param AutoDelete Whether or not this image owns the buffer, allowing it to delete the buffer when it is done.
            /// @param NumFaces The number of faces that exist for the image.
            /// @param NumMipMaps The number of mipmaps that exist for the image.
            /// @return Returns a reference to this.
            Image& LoadImage(UInt8* Data, const UInt32 Width, const UInt32 Height, const Graphics::PixelFormat Format, const Boole AutoDelete = false, const Whole NumFaces = 1, const UInt8 NumMipMaps = 0);
            /// @brief Loads an image from a buffer.
            /// @param Data The buffer to load the image from.
            /// @param Width The width of the image to be loaded.
            /// @param Height The height of the image to be loaded.
            /// @param Depth The depth of the image to be loaded.
            /// @param Format The pixel format of the buffer being loaded.
            /// @param AutoDelete Whether or not this image owns the buffer, allowing it to delete the buffer when it is done.
            /// @param NumFaces The number of faces that exist for the image.
            /// @param NumMipMaps The number of mipmaps that exist for the image.
            /// @return Returns a reference to this.
            Image& LoadImage(UInt8* Data, const UInt32 Width, const UInt32 Height, const UInt32 Depth, const Graphics::PixelFormat Format, const Boole AutoDelete = false, const Whole NumFaces = 1, const UInt8 NumMipMaps = 0);

            ///////////////////////////////////////////////////////////////////////////////
            // Internal Methods

            /// @internal
            /// @brief Gets the internal Image this class is based on.
            /// @return Returns a reference to the internal image class.
            Ogre::Image& _GetInternalImage() const;
        };//Image
    }//Graphics
}//Mezzanine

#endif
