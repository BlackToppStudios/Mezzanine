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

#ifndef _graphicstexturepacker_cpp
#define _graphicstexturepacker_cpp

#include "Graphics/texturepacker.h"
#include "Graphics/image.h"
#include "Graphics/texture.h"
#include "Graphics/texturemanager.h"

#include <Ogre.h>

namespace Mezzanine
{
    namespace Graphics
    {
        TexturePacker::TexturePacker() :
            Padding(0),
            AllowRotated(false)
            {  }

        void TexturePacker::InitImage(const Whole Width, const Whole Height)
        {
            Whole BufferSize = Width * Height * 4;
            UInt8* ImageBuf = new UInt8[BufferSize];
            memset(ImageBuf,255,BufferSize);
            this->AtlasImage.Initialize(ImageBuf,Width,Height,Graphics::PF_R8G8B8A8,true);
        }

        void TexturePacker::AddUnrotatedToAtlas(const Image* Img, const IntegerRect& PlacedRect)
        {
            Img->BlitTo(this->AtlasImage,PlacedRect.Left + this->Padding,PlacedRect.Top + this->Padding,0);
        }

        void TexturePacker::AddRotatedToAtlas(const Image* Img, const IntegerRect& PlacedRect)
        {
            Image Rotated(*Img);
            Rotated.RotateXY(Graphics::OM_Degree_270);
            Rotated.BlitTo(this->AtlasImage,PlacedRect.Left + this->Padding,PlacedRect.Top + this->Padding,0);
        }

        TexturePacker::RectSizeType TexturePacker::GetPaddedRectSize(const Image* Img)
            { return RectSizeType(Img->GetWidth() + (this->Padding * 2),Img->GetHeight() + (this->Padding * 2)); }

        ///////////////////////////////////////////////////////////////////////////////
        // Initialization

        ///////////////////////////////////////////////////////////////////////////////
        // Configuration

        void TexturePacker::SetAllowRotated(const Boole Allow)
            { this->AllowRotated = Allow; }

        Boole TexturePacker::GetAllowRotated() const
            { return this->AllowRotated; }

        void TexturePacker::SetPadding(const Whole Pad)
            { this->Padding = Pad; }

        Whole TexturePacker::GetPadding() const
            { return this->Padding; }

        ///////////////////////////////////////////////////////////////////////////////
        // Insertion

        ///////////////////////////////////////////////////////////////////////////////
        // Results

        Texture* TexturePacker::GenerateTexture(const String& TexName, const String& TexGroup, const PixelFormat Format) const
        {
            Texture* Ret = TextureManager::GetSingletonPtr()->CreateManualTexture(TexName,TexGroup,Graphics::TT_2D,this->GetWidth(),this->GetHeight(),0,Format);
            Ret->_WriteToBuffer(this->AtlasImage.GetImageData(),this->AtlasImage.GetSize(),this->AtlasImage.GetFormat());
            return Ret;
        }

        Image* TexturePacker::GenerateImage(const PixelFormat Format) const
        {
            UInt8* NewBuff = new UInt8[ Ogre::PixelUtil::getMemorySize(this->GetWidth(),this->GetHeight(),1,static_cast<Ogre::PixelFormat>(Format)) ];
            UInt8* AtlasData = const_cast<UInt8*>( this->AtlasImage.GetImageData() );// Contents of the buffer aren't actually altered, just bad API.
            Ogre::PixelUtil::bulkPixelConversion(AtlasData,static_cast<Ogre::PixelFormat>(Graphics::PF_R8G8B8A8),NewBuff,static_cast<Ogre::PixelFormat>(Format),this->GetPixelCount());
            Image* NewImage = new Image(NewBuff,this->GetWidth(),this->GetHeight(),Format,true);
            return NewImage;//*/
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Query

        Whole TexturePacker::GetWidth() const
            { return this->AtlasImage.GetWidth(); }

        Whole TexturePacker::GetHeight() const
            { return this->AtlasImage.GetHeight(); }

        Image& TexturePacker::GetAtlasImage()
            { return this->AtlasImage; }

        const Image& TexturePacker::GetAtlasImage() const
            { return this->AtlasImage; }

        Whole TexturePacker::GetPixelCount() const
            { return this->GetWidth() * this->GetHeight(); }

        Whole TexturePacker::GetByteSize() const
            { return this->AtlasImage.GetSize(); }
    }//Graphics
}//Mezzanine

#endif
