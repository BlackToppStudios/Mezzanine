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

#ifndef _graphicstexture_h
#define _graphicstexture_h

#include "datatypes.h"
#include "Graphics/graphicsenumerations.h"

namespace Ogre
{
    class Texture;
    template<typename T> class SharedPtr;
    typedef SharedPtr<Texture> TexturePtr;
}//Ogre

namespace Mezzanine
{
    namespace Graphics
    {
        class InternalTextureData;
        ///////////////////////////////////////////////////////////////////////////////
        /// @brief This class represents a texture loaded into video memory.
        /// @details
        ///////////////////////////////////////
        class MEZZ_LIB Texture
        {
        protected:
            /// @internal
            /// @brief A pointer to the internal implementation of the Texture.
            InternalTextureData* ITD;
        public:
            /// @brief Class Constructor.
            Texture(Ogre::TexturePtr InternalTexture);
            /// @brief Class Destructor.
            ~Texture();

            ///////////////////////////////////////////////////////////////////////////////
            // Utility Methods

            /// @brief Gets the width of the source Texture in pixels.
            /// @remarks The actual size of the Texture in hardware is subject to change based on a number of configurations in the rendersystem.
            /// This method ignores those settings and reports only the original unaltered size.
            /// @return Returns a UInt32 representing the size of this Texture on the X axis.
            UInt32 GetOriginalWidth() const;
            /// @brief Gets the height of the source Texture in pixels.
            /// @remarks The actual size of the Texture in hardware is subject to change based on a number of configurations in the rendersystem.
            /// This method ignores those settings and reports only the original unaltered size.
            /// @return Returns a UInt32 representing the size of this Texture on the Y axis.
            UInt32 GetOriginalHeight() const;
            /// @brief Gets the depth of the source Texture in pixels.
            /// @remarks The actual size of the Texture in hardware is subject to change based on a number of configurations in the rendersystem.
            /// This method ignores those settings and reports only the original unaltered size.
            /// @return Returns a UInt32 representing the size of this Texture on the Z axis.
            UInt32 GetOriginalDepth() const;
            /// @brief Gets the pixel format of this Texture.
            /// @return Returns a PixelFormat enum value representing how the bits of this Texture are layed out.
            Graphics::PixelFormat GetFormat() const;
            /// @brief Gets the number of MipMaps that exist for this Texture.
            /// @return Returns the number of MipMaps this Texture has.
            Whole GetNumMipMaps() const;

            /// @brief Gets the size of this Texture.
            /// @return Returns a Whole representing the total size of this Texture in bytes.
            Whole GetSize() const;

            ///////////////////////////////////////////////////////////////////////////////
            // Asset Methods

            /// @brief Gets the Name of this Texture.
            /// @note If this Texture originated from a file, usually the name of the Texture will be the file name.
            /// @return Returns a const string reference containing the name of this Texture.
            const String& GetName() const;
            /// @brief Gets the resource group this Texture belongs to.
            /// @return Returns a const string reference containing the group this Texture belongs to.
            const String& GetGroup() const;

            ///////////////////////////////////////////////////////////////////////////////
            // Internal Buffer Manipulation Methods

            /// @brief Reads from the internal buffer and populates the provided buffer with the texture data.
            /// @exception If the size of the provided buffer is smaller than this texture, a PARAMETERS_EXCEPTION will be thrown.
            /// @param DestBuffer The buffer to be written to.
            /// @param BufferSize The size of the buffer provided.
            /// @return Returns the number of bytes read from the texture.  Usually this will be the size of the texture itself.
            Whole _ReadFromBuffer(UInt8* DestBuffer, const Whole BufferSize);
            /// @brief Writes to the textures internal buffer.
            /// @exception If the size of this texture is smaller than the buffer being written, a PARAMETERS_EXCEPTION will be thrown.
            /// @param SrcBuffer The buffer containing the pixel data to be written.
            /// @param BufferSize The size of the buffer being written.
            /// @param SrcFormat The pixel format of the texture in the source buffer.  Pixel format will automatically be converted to the set format of this texture.
            void _WriteToBuffer(UInt8* SrcBuffer, const Whole BufferSize, const Graphics::PixelFormat SrcFormat);

            ///////////////////////////////////////////////////////////////////////////////
            // Internal Methods

            /// @internal
            /// @brief Gets the internal Texture pointer.
            /// @return Returns a shared pointer pointing to the internal Texture.
            Ogre::TexturePtr _GetInternalTexture() const;
        };//Texture
    }//Graphics
}//Mezzanine

#endif
