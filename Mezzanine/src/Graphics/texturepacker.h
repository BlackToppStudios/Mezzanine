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

#ifndef _graphicstexturepacker_h
#define _graphicstexturepacker_h

#include "rectbase.h"
#include "Graphics/graphicsenumerations.h"
#include "Graphics/image.h"

#include <limits>

namespace Mezzanine
{
    namespace Graphics
    {
        ///////////////////////////////////////////////////////////////////////////////
        /// @brief A simple struct used to maintain a unique identity of an Image as it is packed.
        ///////////////////////////////////////
        struct MEZZ_LIB ImageIDPair
        {
            /// @brief The image being packed.
            Image* Img;
            /// @brief The identity of the image being packed.
            String ImgName;

            /// @brief Struct constructor.
            /// @param TheImage The image being packed.
            /// @param TheName The identity of the image being packed.
            ImageIDPair(Image* TheImage, const String& TheName) :
                Img(TheImage),
                ImgName(TheName)
                {  }
        };//ImageIDPair

        ///////////////////////////////////////////////////////////////////////////////
        /// @brief This is an interface class for handling the results of large collections of images being packed into an atlas.
        ///////////////////////////////////////
        class MEZZ_LIB PackResultHandler
        {
        public:
            /// @brief Convenience type for returning a placed Rect and whether or not if was rotated.
            typedef std::pair<IntegerRect,Boole> PlacementResult;

            /// @brief Performs callback logic when a texture is added to the atlas.
            /// @param ImgPair The image instance and name of the image that was packed.
            /// @param PlacedRect The Rect where the image was placed in the atlas.
            virtual void ProcessTexturePlacement(const ImageIDPair& ImgPair, const PlacementResult& PlacedRect) = 0;
            /*/// @brief Performs callback logic when a texture is added to the atlas.
            /// @param ImgName The name of the image that was packed.
            /// @param Img The image that was packed.
            /// @param PlacedRect The Rect where the image was placed in the atlas.
            virtual void ProcessTexturePlacement(const String& ImgName, const Image& Img, const PlacementResult& PlacedRect) = 0;//*/
        };//PackResultHandler

        ///////////////////////////////////////////////////////////////////////////////
        /// @brief A simple struct used by some texture packers for returning a resulting Rect and it's positioning score.
        ///////////////////////////////////////
        struct MEZZ_LIB RectScoreResult
        {
            /// @brief Type of RectBase to use for texture packing.
            typedef IntegerRect RectType;

            /// @brief The new Rect being placed.
            RectType NewRect;
            /// @brief The first scoring value for the given heuristic.
            Integer Score1;
            /// @brief The second scoring value for the given heuristic.
            Integer Score2;
            /// @brief The index of the skyline the Rect was placed on.
            Integer Index;
            /// @brief Whether or not the new Rect was flipped.
            Boole Rotated;

            /// @brief No init constructor.
            RectScoreResult() :
                Index(-1),
                Rotated(false)
                { this->ResetScores(); }

            /// @brief Resets the scores back to max.
            void ResetScores()
            {
                this->Score1 = std::numeric_limits<RectType::MemberType>::max();
                this->Score2 = std::numeric_limits<RectType::MemberType>::max();
            }

            /// @brief Assignment operator.
            /// @param Other The other RectScoreResult to copy from.
            /// @return Returns a reference to this.
            RectScoreResult& operator=(const RectScoreResult& Other)
            {
                this->NewRect = Other.NewRect;
                this->Score1 = Other.Score1;
                this->Score2 = Other.Score2;
                this->Index = Other.Index;
                this->Rotated = Other.Rotated;
                return *this;
            }
            /// @brief Less-than operator.
            /// @param Other The other RectScoreResult to compare to.
            /// @return Returns true if this RectScoreResult should be placed before Other when sorting.
            Boole operator<(const RectScoreResult& Other)
            {
                if( this->Score1 < Other.Score1 ) {
                    return true;
                }else if( this->Score1 == Other.Score1 && this->Score2 < Other.Score2 ) {
                    return true;
                }
                return false;
            }
        };//RectScoreResult

        ///////////////////////////////////////////////////////////////////////////////
        /// @brief This is the base class for algorithms that pack textures into an atlas.
        /// @details The TexturePacker and all associated classes are derived from a paper written by Jukka Jylänki
        /// with code available in the public domain.  The repo can be found here: @n
        /// https://github.com/juj/RectangleBinPack
        ///////////////////////////////////////
        class MEZZ_LIB TexturePacker
        {
        public:
            /// @brief Type of RectBase to use for texture packing.
            typedef IntegerRect                     RectType;
            /// @brief Type of RectSize to use for texture packing.
            typedef RectType::SizeType              RectSizeType;
            /// @brief Convenience type for returning a placed Rect and whether or not if was rotated.
            typedef std::pair<RectType,Boole>       PlacementResult;
            /// @brief Convenience type for a container of Rects.
            typedef std::vector<RectType>           RectVector;
            /// @brief Iterator type for Rects stored by this class.
            typedef RectVector::iterator            RectIterator;
            /// @brief Const Iterator type for Rects stored by this class.
            typedef RectVector::const_iterator      ConstRectIterator;
            /// @brief Convenience type for the container storing Images to be processed.
            typedef std::vector<ImageIDPair>        ImageContainer;
            /// @brief Iterator type for images to be packed into the atlas.
            typedef ImageContainer::iterator        ImageIterator;
            /// @brief Const Iterator type for images to be packed into the atlas.
            typedef ImageContainer::const_iterator  ConstImageIterator;
        protected:
            /// @brief The image to blit all the parameter images to.
            Image AtlasImage;
            /// @brief The padding around each image to apply during packing.
            Whole Padding;
            /// @brief Whether or not to allow images to be Rotated when placing into the atlas.
            Boole AllowRotated;

            /// @brief Initializes the atlas image so it is ready for packing.
            /// @note For maximum hardware compatibility, the dimensions should be the same and powers of 2.
            /// @param Width The pixel width the atlas image should be.
            /// @param Height The pixel height the atlas image should be.
            void InitImage(const Whole Width, const Whole Height);
            /// @brief Blits an unrotated image to the Atlas.
            /// @param Img The image that was packed.
            /// @param PlacedRect The Rect where the image is to be placed in the atlas.
            void AddUnrotatedToAtlas(const Image* Img, const IntegerRect& PlacedRect);
            /// @brief Blits a rotated image to the Atlas.
            /// @param Img The image that was packed.
            /// @param PlacedRect The Rect where the image is to be placed in the atlas.
            void AddRotatedToAtlas(const Image* Img, const IntegerRect& PlacedRect);
            /// @brief Gets a padded version of a RectSize representing an image to be inserted.
            /// @param Img The image to get the padded RectSize for.
            /// @return Returns a RectSize containing the image size plus padding.
            RectSizeType GetPaddedRectSize(const Image* Img);
        public:
            /// @brief Class constructor.
            TexturePacker();
            /// @brief Copy constructor.
            /// @param Other The other packer to be copied.
            TexturePacker(const TexturePacker& Other) = default;
            /// @brief Move constructor.
            /// @param Other The other packer to be moved.
            TexturePacker(TexturePacker&& Other) = default;
            /// @brief Class destructor.
            virtual ~TexturePacker() = default;

            ///////////////////////////////////////////////////////////////////////////////
            // Initialization

            /// @brief Sets the packer into a fresh state reading for packing.
            /// @param TexWidth The width of the texture atlas to generate.
            /// @param TexHeight The height of the texture atlas to generate.
            virtual void Initialize(const Whole TexWidth, const Whole TexHeight) = 0;

            ///////////////////////////////////////////////////////////////////////////////
            // Configuration

            /// @brief Sets whether or not rotated images is allowed for placement.
            /// @param Allow True to allow image rotating, false otherwise.  Default: false.
            void SetAllowRotated(const Boole Allow);
            /// @brief Gets whether or not rotated images is allowed for placement.
            /// @return Returns true if rotating an image is allowed during placement, false otherwise.
            Boole GetAllowRotated() const;
            /// @brief Sets the amount of padding around each placed image there should be when packing.
            /// @param Pad The amount of extra space (in pixels) on each edge Rects will get when being placed.
            void SetPadding(const Whole Pad);
            /// @brief Gets the amount of padding around each placed image there will be when packing.
            /// @return Returns the amount of extra pixel space each Rect will get on every edge when being placed.
            Whole GetPadding() const;

            ///////////////////////////////////////////////////////////////////////////////
            // Insertion

            /// @brief Inserts a single image into the atlas.
            /// @param ToInsert The image to be inserted.
            /// @return Returns a PlacementResult containing the location of the image as placed in the atlas and whether it was rotated.
            virtual PlacementResult Insert(const Image* ToInsert) = 0;
            /// @brief Inserts a series of images into the atlas.
            /// @param ToPack The collection of Images to be packed.
            /// @param Handler A pointer to the handler that will process the packing result.  Must be valid.
            /// @return Returns a container of the unpacked images.
            virtual ImageContainer Insert(const ImageContainer& ToPack, PackResultHandler* Handler) = 0;

            ///////////////////////////////////////////////////////////////////////////////
            // Results

            /// @brief Generates the Texture.
            /// @param TexName The name to give to the generated Texture.
            /// @param TexGroup The asset group to place the Texture in.
            /// @param Format The pixel format for the generated Texture.
            /// @return Returns a pointer to the newly created Texture.
            Texture* GenerateTexture(const String& TexName, const String& TexGroup, const PixelFormat Format = Graphics::PF_R8G8B8A8) const;
            /// @brief Generates the Image.
            /// @remarks The generated image is not owned by this generator or anything.  It is up to the user/caller of this method to clean it up.
            /// @param Format The pixel format for the generated Image.
            /// @return Returns a pointer to the newly created Image.
            Image* GenerateImage(const PixelFormat Format = Graphics::PF_R8G8B8A8) const;

            ///////////////////////////////////////////////////////////////////////////////
            // Query

            /// @brief Gets the width of the texture atlas being generated.
            /// @return Returns the pixel width of the texture atlas.
            Whole GetWidth() const;
            /// @brief Gets the height of the texture atlas being generated.
            /// @return Returns the pixel height of the texture atlas.
            Whole GetHeight() const;

            /// @brief Gets the number of pixels in the atlas texture.
            /// @return Returns the number of pixels that exist in this atlas.
            Whole GetPixelCount() const;
            /// @brief Gets the size of the atlas in memory.
            /// @return Returns the amount of bytes allocated for the atlas buffer.
            Whole GetByteSize() const;

            /// @brief Gets the internal image used to assemble the atlas.
            /// @return Returns a reference to the image being packed into.
            Image& GetAtlasImage();
            /// @brief Gets the internal image used to assemble the atlas.
            /// @return Returns a const reference to the image being packed into.
            const Image& GetAtlasImage() const;

            /// @brief Gets the percentage of the atlas that has been covered with placed images.
            /// @return Returns a Real containing the percentage of used pixels in the atlas.
            virtual Real GetCoverage() const = 0;
        };//TexturePacker
    }//Graphics
}//Mezzanine

#endif
