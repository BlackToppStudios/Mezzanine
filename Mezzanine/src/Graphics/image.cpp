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

#ifndef _graphicsimage_cpp
#define _graphicsimage_cpp

#include "Graphics/image.h"
#include "Graphics/texture.h"
#include "Graphics/graphicsutilities.h"

#include "stringtool.h"

#include "Internal/iostreamwrapper.h.cpp"

#include <Ogre.h>

namespace Mezzanine
{
    namespace Graphics
    {
        ///////////////////////////////////////////////////////////////////////////////
        /// @class InternalImageData
        /// @brief This class is used to store the internal structures needed by the Image class.
        ///////////////////////////////////////
        class MEZZ_LIB InternalImageData
        {
        public:
            /// @internal
            /// @brief The internal representation of the Image.
            Ogre::Image GraphicsImage;

            /// @brief Blank Constructor.
            InternalImageData()
                {  }
            /// @brief Copy Constructor.
            /// @param Other The other Image to be copied.
            InternalImageData(const Image& Other) :
                GraphicsImage(Other._GetInternalImage())
                {  }
        };//InternalImageData

        /// @brief Converts an X-Y-Z pixel coordinate into an index.
        /// @param X The X position of the pixel.
        /// @param Y The Y position of the pixel.
        /// @param Z The Z position of the pixel.
        /// @param Width The width of the target image.
        /// @param Height The height of the target image.
        /// @return Returns the calculated pixel index.
        Whole GetPixelIndex(const Whole X, const Whole Y, const Whole Z, const Whole Width, const Whole Height)
        {
            return ( Z * Width * Height ) + ( Y * Width ) + X;
        }

        /// @brief Converts an X-Y-Z pixel coordinate into an X-Y rotation index.
        /// @param X The original X position of the pixel to be rotated.
        /// @param Y The original Y position of the pixel to be rotated.
        /// @param Z The original Z position of the pixel to be rotated.
        /// @param Width The width of the target image.
        /// @param Height The height of the target image.
        /// @param Mode The rotation to be applied.
        /// @return Returns the rotated pixel index.
        Whole GetPixelIndexXY(const Whole X, const Whole Y, const Whole Z, const Whole Width, const Whole Height, const Graphics::OrientationMode Mode)
        {
            // We're rotating on the XY plane, so the Z component is the same regardless of the rotation.
            Whole ZResult = Z * Width * Height;
            switch( Mode )
            {
                case Graphics::OM_Degree_90:   return ZResult + ( X * Width ) + ( ( Height - 1 ) - Y );
                case Graphics::OM_Degree_180:  return ZResult + ( ( ( Height - 1 ) - Y ) * Width ) + ( ( Width - 1 ) - X );
                case Graphics::OM_Degree_270:  return ZResult + ( ( ( Height - 1 ) - X ) * Width ) + Y;
                case Graphics::OM_Degree_0:
                default:                       return ZResult + ( Y * Width ) + X;
            }
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Image Methods

        Image::Image()
        {
            this->IID = new InternalImageData();
        }

        Image::Image(const Image& Other)
        {
            this->IID = new InternalImageData(Other);
        }

        Image::Image(const Image&& Other)
        {
            this->IID->GraphicsImage = Other.IID->GraphicsImage;
            Other.IID->GraphicsImage.freeMemory();
        }

        Image::Image(const String& ResourceName, const String& ResourceGroup)
        {
            this->IID = new InternalImageData();
            this->Load(ResourceName,ResourceGroup);
        }

        Image::Image(UInt8* Data, const UInt32 Width, const UInt32 Height, const Graphics::PixelFormat Format, const Boole AutoDelete, const Whole NumFaces, const UInt8 NumMipMaps)
        {
            this->IID = new InternalImageData();
            this->Initialize(Data,Width,Height,Format,AutoDelete,NumFaces,NumMipMaps);
        }

        Image::Image(UInt8* Data, const UInt32 Width, const UInt32 Height, const UInt32 Depth, const Graphics::PixelFormat Format, const Boole AutoDelete, const Whole NumFaces, const UInt8 NumMipMaps)
        {
            this->IID = new InternalImageData();
            this->Initialize(Data,Width,Height,Depth,Format,AutoDelete,NumFaces,NumMipMaps);
        }

        Image::~Image()
            { delete this->IID; }

        ///////////////////////////////////////////////////////////////////////////////
        // Utility Methods

        UInt32 Image::GetWidth() const
            { return this->IID->GraphicsImage.getWidth(); }

        UInt32 Image::GetHeight() const
            { return this->IID->GraphicsImage.getHeight(); }

        UInt32 Image::GetDepth() const
            { return this->IID->GraphicsImage.getDepth(); }

        Graphics::PixelFormat Image::GetFormat() const
            { return static_cast<Graphics::PixelFormat>( this->IID->GraphicsImage.getFormat() ); }

        UInt8 Image::GetBitsPerPixel() const
            { return this->IID->GraphicsImage.getBPP(); }

        Whole Image::GetNumFaces() const
            { return this->IID->GraphicsImage.getNumFaces(); }

        Whole Image::GetNumMipMaps() const
            { return this->IID->GraphicsImage.getNumMipmaps(); }

        UInt8* Image::GetImageData()
            { return static_cast<UInt8*>( this->IID->GraphicsImage.getData() ); }

        const UInt8* Image::GetImageData() const
            { return static_cast<const UInt8*>( this->IID->GraphicsImage.getData() ); }

        Whole Image::GetSize() const
            { return this->IID->GraphicsImage.getSize(); }

        Whole Image::GetRowSpan() const
            { return this->IID->GraphicsImage.getRowSpan(); }

        Image& Image::ReverseXAxis()
        {
            this->IID->GraphicsImage.flipAroundX();
            return *this;
        }

        Image& Image::ReverseYAxis()
        {
            this->IID->GraphicsImage.flipAroundY();
            return *this;
        }

        Image& Image::RotateXY(const Graphics::OrientationMode Orientation)
        {
            if( Orientation == Graphics::OM_Degree_0 )
                return *this;

            Whole Width = this->GetWidth();
            Whole Height = this->GetHeight();
            Whole Depth = this->GetDepth();
            Whole TargetWidth = ( Orientation != Graphics::OM_Degree_180 ? Height : Width );
            Whole TargetHeight = ( Orientation != Graphics::OM_Degree_180 ? Width : Height );
            Ogre::PixelFormat Format = this->IID->GraphicsImage.getFormat();
            Whole BytesPerPixel = Ogre::PixelUtil::getNumElemBytes(Format);
            Whole NumMipMaps = this->GetNumMipMaps();
            Whole NumFaces = this->GetNumFaces();

            Ogre::Image& TheImage = this->IID->GraphicsImage;
            Ogre::ColourValue PixelColour;
            UInt8* OrigBuf = this->GetImageData();
            UInt8* RotatedBuf = new UInt8[ Width * Height * Depth * BytesPerPixel ];
            for( Whole ZPos = 0 ; ZPos < Depth ; ++ZPos )
            {
                for( Whole YPos = 0 ; YPos < Height ; ++YPos )
                {
                    for( Whole XPos = 0 ; XPos < Width ; ++XPos )
                    {
                        Whole PixelIndex = GetPixelIndex(XPos,YPos,ZPos,Width,Height) * BytesPerPixel;
                        Whole RotatedIndex = GetPixelIndexXY(XPos,YPos,ZPos,TargetWidth,TargetHeight,Orientation) * BytesPerPixel;
                        Ogre::PixelUtil::unpackColour(&PixelColour,Format,OrigBuf + PixelIndex);
                        Ogre::PixelUtil::packColour(PixelColour,Format,RotatedBuf + RotatedIndex);
                    }
                }
            }

            this->IID->GraphicsImage.loadDynamicImage(RotatedBuf,TargetWidth,TargetHeight,Depth,Format,true,NumFaces,NumMipMaps);
            return *this;
        }

        void Image::SetColourAt(const Whole X, const Whole Y, const Whole Z, const ColourValue& Colour)
            { this->IID->GraphicsImage.setColourAt(Colour.GetOgreColourValue(),X,Y,Z); }

        ColourValue Image::GetColourAt(const Whole X, const Whole Y, const Whole Z) const
            { return this->IID->GraphicsImage.getColourAt(X,Y,Z); }

        void Image::BlitTo(Image& Other, const Whole X, const Whole Y, const Whole Z) const
        {
            Ogre::PixelBox SrcBox = this->IID->GraphicsImage.getPixelBox();
            Ogre::Box DstBounds( X, Y, Z, X + this->GetWidth(), Y + this->GetHeight(), Z + this->GetDepth() );
            Ogre::PixelBox DstBox = Other.IID->GraphicsImage.getPixelBox().getSubVolume(DstBounds);
            Ogre::PixelUtil::bulkPixelConversion(SrcBox,DstBox);
        }

        void Image::BlitTo(Image& Other, const ImageBounds& Bounds, const Whole X, const Whole Y, const Whole Z) const
        {
            Ogre::Box SrcBounds( Bounds.Left, Bounds.Top, Bounds.Front, Bounds.Right, Bounds.Bottom, Bounds.Back );
            Ogre::PixelBox SrcBox = this->IID->GraphicsImage.getPixelBox().getSubVolume(SrcBounds);
            Ogre::Box DstBounds( X, Y, Z, X + Bounds.GetWidth(), Y + Bounds.GetHeight(), Z + Bounds.GetDepth() );
            Ogre::PixelBox DstBox = Other.IID->GraphicsImage.getPixelBox().getSubVolume(DstBounds);
            Ogre::PixelUtil::bulkPixelConversion(SrcBox,DstBox);
        }

        void Image::BlitTo(Texture* Other, const Whole X, const Whole Y, const Whole Z) const
        {
            Ogre::PixelBox SrcBox = this->IID->GraphicsImage.getPixelBox();
            Ogre::Box DstBounds( X, Y, Z, X + this->GetWidth(), Y + this->GetHeight(), Z + this->GetDepth() );
            Other->_GetInternalTexture()->getBuffer()->blitFromMemory(SrcBox,DstBounds);
        }

        void Image::BlitTo(Texture* Other, const ImageBounds& Bounds, const Whole X, const Whole Y, const Whole Z) const
        {
            Ogre::Box SrcBounds( Bounds.Left, Bounds.Top, Bounds.Front, Bounds.Right, Bounds.Bottom, Bounds.Back );
            Ogre::PixelBox SrcBox = this->IID->GraphicsImage.getPixelBox().getSubVolume(SrcBounds);
            Ogre::Box DstBounds( X, Y, Z, X + Bounds.GetWidth(), Y + Bounds.GetHeight(), Z + Bounds.GetDepth() );
            Other->_GetInternalTexture()->getBuffer()->blitFromMemory(SrcBox,DstBounds);
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Initialize Methods

        Image& Image::Initialize(UInt8* Data, const UInt32 Width, const UInt32 Height, const Graphics::PixelFormat Format, const Boole AutoDelete, const Whole NumFaces, const UInt8 NumMipMaps)
        {
            this->IID->GraphicsImage.loadDynamicImage(Data,Width,Height,1,static_cast<Ogre::PixelFormat>(Format),AutoDelete,NumFaces,NumMipMaps);
            return *this;
        }

        Image& Image::Initialize(UInt8* Data, const UInt32 Width, const UInt32 Height, const UInt32 Depth, const Graphics::PixelFormat Format, const Boole AutoDelete, const Whole NumFaces, const UInt8 NumMipMaps)
        {
            this->IID->GraphicsImage.loadDynamicImage(Data,Width,Height,Depth,static_cast<Ogre::PixelFormat>(Format),AutoDelete,NumFaces,NumMipMaps);
            return *this;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Loading Methods

        Image& Image::Load(const String& ResourceName, const String& ResourceGroup)
        {
            this->IID->GraphicsImage.load(ResourceName,ResourceGroup);
            return *this;
        }

        Image& Image::Load(const String& FilePathAndName)
        {
            std::ifstream ImageFileStream;
            ImageFileStream.open(FilePathAndName.c_str(),std::ifstream::in | std::ifstream::binary);
            if( ImageFileStream.fail() ) {
                MEZZ_EXCEPTION(ExceptionBase::IO_EXCEPTION,"Unable to open the file at: \"" + FilePathAndName + "\".");
            }
            return this->Load(&ImageFileStream,FilePathAndName.substr(FilePathAndName.find_last_of('.')+1));
        }

        Image& Image::Load(std::istream* Stream, const String& ExtensionHint)
        {
            Ogre::DataStreamPtr OgreStreamPtr(new Internal::IStreamWrapper(Stream,false));
            this->IID->GraphicsImage.load(OgreStreamPtr,ExtensionHint);
            return *this;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Saving Methods

        Image& Image::Save(const String& FileName, const String& GroupName)
        {
            MEZZ_EXCEPTION(ExceptionBase::NOT_IMPLEMENTED_EXCEPTION,"Saving Images via asset groups is not supported yet.");
            return *this;
        }

        Image& Image::Save(const String& FilePathAndName)
        {
            // Ogre does everything for us in a straightforward manner
            this->IID->GraphicsImage.save(FilePathAndName);
            return *this;
        }

        Image& Image::Save(const Graphics::ImageFileFormat Format, std::ostream* Stream)
        {
            // Be lazy
            return this->Save(Graphics::ConvertImageFileExtension(Format),Stream);
        }

        Image& Image::Save(const String& Extension, std::ostream* Stream)
        {
            Ogre::DataStreamPtr OgreStream = this->IID->GraphicsImage.encode(Extension);
            Ogre::MemoryDataStream* RawOgreStream = static_cast<Ogre::MemoryDataStream*>( OgreStream.getPointer() );
            Stream->write(reinterpret_cast<char*>(RawOgreStream->getPtr()),RawOgreStream->size());
            return *this;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Operators

        Image& Image::operator=(const Image& Other)
        {
            this->IID->GraphicsImage = Other.IID->GraphicsImage;
            return *this;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Internal Methods

        Ogre::Image& Image::_GetInternalImage() const
            { return this->IID->GraphicsImage; }
    }//Graphics
}//Mezzanine

#endif
