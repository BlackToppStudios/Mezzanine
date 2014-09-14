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
    class TexturePtr;
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
            Texture();
            /// @brief Class Destructor.
            ~Texture();

            ///////////////////////////////////////////////////////////////////////////////
            // Utility Methods

            /// @brief Gets the width of the source texture in pixels.
            /// @remarks The actual size of the texture in hardware is subject to change based on a number of configurations in the rendersystem.
            /// This method ignores those settings and reports only the original unaltered size.
            /// @return Returns a UInt32 representing the size of this texture on the X axis.
            UInt32 GetOriginalWidth() const;
            /// @brief Gets the height of the source texture in pixels.
            /// @remarks The actual size of the texture in hardware is subject to change based on a number of configurations in the rendersystem.
            /// This method ignores those settings and reports only the original unaltered size.
            /// @return Returns a UInt32 representing the size of this texture on the Y axis.
            UInt32 GetOriginalHeight() const;
            /// @brief Gets the depth of the source texture in pixels.
            /// @remarks The actual size of the texture in hardware is subject to change based on a number of configurations in the rendersystem.
            /// This method ignores those settings and reports only the original unaltered size.
            /// @return Returns a UInt32 representing the size of this texture on the Z axis.
            UInt32 GetOriginalDepth() const;
            /// @brief Gets the pixel format of this image.
            /// @return Returns a PixelFormat enum value representing how the bits of this image are layed out.
            Graphics::PixelFormat GetFormat() const;
            /// @brief Gets the number of MipMaps that exist for this image.
            /// @return Returns the number of MipMaps this image has.
            Whole GetNumMipMaps() const;

            /// @brief Gets the size of this image.
            /// @return Returns a Whole representing the total size of this image in bytes.
            Whole GetSize() const;

            ///////////////////////////////////////////////////////////////////////////////
            // AssetMethods

            ///////////////////////////////////////////////////////////////////////////////
            // Internal Methods
        };//Texture
    }//Graphics
}//Mezzanine

#endif
