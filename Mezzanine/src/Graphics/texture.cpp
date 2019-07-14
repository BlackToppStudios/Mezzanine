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

#ifndef _graphicstexture_cpp
#define _graphicstexture_cpp

#include "Graphics/texture.h"

#include "exception.h"

#include <OgreTexture.h>
#include <OgrePixelFormat.h>
#include <OgreHardwarePixelBuffer.h>

namespace Mezzanine
{
    namespace Graphics
    {
        ///////////////////////////////////////////////////////////////////////////////
        // Texture Methods

        Texture::Texture(Ogre::TexturePtr ToWrap, ManualTextureLoader* Loader) :
            InternalTexture(ToWrap),
            InternalLoader(Loader)
        {

        }

        Texture::~Texture()
        {
            if( this->InternalLoader ) {
                delete this->InternalLoader;
            }
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Utility Methods

        UInt32 Texture::GetOriginalWidth() const
            { return this->InternalTexture->getSrcWidth(); }

        UInt32 Texture::GetOriginalHeight() const
            { return this->InternalTexture->getSrcHeight(); }

        UInt32 Texture::GetOriginalDepth() const
            { return this->InternalTexture->getSrcDepth(); }

        Graphics::PixelFormat Texture::GetFormat() const
            { return static_cast<Graphics::PixelFormat>( this->InternalTexture->getFormat() ); }

        Whole Texture::GetNumMipMaps() const
            { return this->InternalTexture->getNumMipmaps(); }

        Whole Texture::GetSize() const
            { return this->InternalTexture->getSize(); }

        ///////////////////////////////////////////////////////////////////////////////
        // AssetMethods

        const String& Texture::GetName() const
            { return this->InternalTexture->getName(); }

        const String& Texture::GetGroup() const
            { return this->InternalTexture->getGroup(); }

        ///////////////////////////////////////////////////////////////////////////////
        // Internal Buffer Manipulation Methods

        Whole Texture::_ReadFromBuffer(UInt8* DestBuffer, const Whole BufferSize)
        {
            Whole TexSize = this->GetSize();
            if( BufferSize < TexSize ) {
                MEZZ_EXCEPTION(ExceptionBase::PARAMETERS_EXCEPTION,"Destination buffer is too small to fit all texture pixel data.");
            }

            Ogre::HardwarePixelBufferSharedPtr PixelBuffer = this->InternalTexture->getBuffer();

            // Lock the pixel buffer and get a pixel box
            PixelBuffer->lock(Ogre::HardwareBuffer::HBL_NORMAL);
            const Ogre::PixelBox& Box = PixelBuffer->getCurrentLock();

            UInt8* SrcBuf = static_cast<UInt8*>(Box.data);

            Ogre::PixelUtil::bulkPixelConversion(SrcBuf,this->InternalTexture->getFormat(),DestBuffer,this->InternalTexture->getFormat(),TexSize);
            //memcpy(DestBuffer,SrcBuf,TexSize);

            PixelBuffer->unlock();

            return TexSize;
        }

        void Texture::_WriteToBuffer(const UInt8* SrcBuffer, const Whole BufferSize, const Graphics::PixelFormat SrcFormat)
        {
            if( this->GetSize() != BufferSize ) {
                MEZZ_EXCEPTION(ExceptionBase::PARAMETERS_EXCEPTION,"Texture and write buffer are different sizes.  Sizes must match.");
            }

            Ogre::HardwarePixelBufferSharedPtr PixelBuffer = this->InternalTexture->getBuffer();

            // Lock the pixel buffer and get a pixel box
            PixelBuffer->lock(Ogre::HardwareBuffer::HBL_NORMAL);
            const Ogre::PixelBox& Box = PixelBuffer->getCurrentLock();

            UInt8* DestBuf = static_cast<UInt8*>(Box.data);
            Whole PixelCount = BufferSize / Ogre::PixelUtil::getNumElemBytes(static_cast<Ogre::PixelFormat>(SrcFormat));
            Ogre::PixelUtil::bulkPixelConversion(const_cast<UInt8*>(SrcBuffer),static_cast<Ogre::PixelFormat>(SrcFormat),DestBuf,this->InternalTexture->getFormat(),PixelCount);

            // Unlock the pixel buffer
            PixelBuffer->unlock();
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Internal Methods

        Ogre::TexturePtr Texture::_GetInternalTexture() const
            { return this->InternalTexture; }

        Ogre::TexturePtr Texture::_Upcast(Ogre::ResourcePtr ToCast)
            { return std::static_pointer_cast<Ogre::Texture>(ToCast); }
    }//Graphics
}//Mezzanine

#endif
