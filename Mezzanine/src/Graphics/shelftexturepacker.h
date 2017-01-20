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

#ifndef _graphicsshelftexturepacker_h
#define _graphicsshelftexturepacker_h

#include "Graphics/texturepacker.h"

namespace Mezzanine
{
    namespace Graphics
    {
        /// @brief Specifies the different heuristic rules used by the ShelfTexturePacker that can be used when deciding where to place a new rectangle.
        enum ShelfPlacement
        {
            SP_NextFit,       ///< -NF: We always put the new rectangle to the last open shelf.  Makes no attempt to rotate.
            SP_FirstFit,      ///< -FF: We test each rectangle against each shelf in turn and pack it to the first where it fits.  Makes no attempt to rotate.
            SP_BestAreaFit,   ///< -BAF: Choose the shelf with smallest remaining shelf area.
            SP_WorstAreaFit,  ///< -WAF: Choose the shelf with the largest remaining shelf area.
            SP_BestHeightFit, ///< -BHF: Choose the smallest shelf (height-wise) where the rectangle fits.
            SP_BestWidthFit,  ///< -BWF: Choose the shelf that has the least remaining horizontal shelf space available after packing.
            SP_WorstWidthFit  ///< -WWF: Choose the shelf that will have most remainining horizontal shelf space available after packing.
        };

        ///////////////////////////////////////////////////////////////////////////////
        /// @brief This is used by the ShelfTexturePacker algorithm to keep track of a row of Rects.
        ///////////////////////////////////////
        struct MEZZ_LIB TextureShelf
        {
            ///////////////////////////////////////////////////////////////////////////////
            // Public Types

            /// @brief Convenience typedef to use for Rects as TexturePacker.
            typedef TexturePacker::RectType RectType;
            /// @brief Convenience typedef to use for RectSizes as TexturePacker.
            typedef TexturePacker::RectSizeType RectSizeType;
            /// @brief Convenience typedef to use the same container type as TexturePacker.
            typedef TexturePacker::RectVector RectVector;

            ///////////////////////////////////////////////////////////////////////////////
            // Data Members

            /// @brief The current placing position on the X axis for the shelf.
            Integer XPos;
            /// @brief The current placing position on the Y axis for the shelf.
            Integer YPos;
            /// @brief The height of this shelf, which is the largest height of the Rects in the shelf.
            Integer ShelfHeight;
            /// @brief A container of the Rects that have been placed on the shelf.
            RectVector UsedRectangles;

            ///////////////////////////////////////////////////////////////////////////////
            // Construction and Destruction

            /// @brief Position constructor.
            /// @param YPosition The starting position for Rect placement in this shelf on the Y axis.
            TextureShelf(const Integer YPosition);
            /// @brief Descriptive constructor.
            /// @param YPosition The starting position for Rect placement in this shelf on the Y axis.
            /// @param Height The starting height of the shelf.
            TextureShelf(const Integer YPosition, const Integer Height);
            /// @brief Class destructor.
            ~TextureShelf() = default;

            ///////////////////////////////////////////////////////////////////////////////
            // Functionality

            /// @brief Checks to see if a Rect of a given size can fit on this shelf.
            /// @param RectSize The size of the Rect being tested.
            /// @param AtlasSize The size of the atlas being placed into.
            /// @param CanResize Whether or not the shelf itself is allowed to adjust it's height to accommodate the Rect.
            /// @return Returns true if the Rect can be placed on this shelf, false otherwise.
            Boole FitsOnShelf(const RectSizeType& RectSize, const RectSizeType& AtlasSize, const Boole CanResize) const;
            /// @brief Adds a Rect to the shelf.
            /// @remarks This trusts that checks were performed prior to it's calling.  Use with care.
            /// @param RectSize The size of the Rect being added.
            /// @return Returns a complete Rect that has been placed on this shelf.
            RectType AddToShelf(const RectSizeType& RectSize);
            /// @brief Determines if the Rect should be rotated for an optimal fit.
            /// @remarks This is a fairly simple method that determines rotation based on whether the width is equal to or
            /// less than the current shelf height while the width is greater than height.
            /// @param RectSize The size of the Rect being tested.
            /// @param AtlasSize The size of the atlas being placed into.
            /// @return Returns true if the Rect should be rotated for a better fit, false otherwise.
            Boole ShouldRotateToFit(const RectSizeType& RectSize, const RectSizeType& AtlasSize) const;
        };//TextureShelf

        ///////////////////////////////////////////////////////////////////////////////
        /// @brief This uses the Shelf algorithm to quickly place Rects at similar heights.
        /// @details The Shelf algorithm is pretty brain dead simple.  When a Rect is placed it is checked to see if
        /// it is taller than the other Rects in the same row (or "Shelf" as this algorithm likes to call it).  When
        /// possible Rects of similar heights are placed on the same row to reduce space wastage.  When the end of the
        /// row is reached the shelf is closed and a new shelf is made.  This repeats until vertical space is exhausted
        /// or all Rects are placed. @n @n
        /// Space wastage with this algorithm is fairly high.  It's only saving grace is it's speed.  So it is best used
        /// when packing similarly sized Rects.  The MaxRects or Guillotine texture packers are better suited if you
        /// have a collection of varying sized Rects.
        ///////////////////////////////////////
        class MEZZ_LIB ShelfTexturePacker : public TexturePacker
        {
        public:
            /// @brief Convenience type for a container of Shelves.
            typedef std::vector<TextureShelf>       ShelfContainer;
            /// @brief Iterator type for Shelves stored by this class.
            typedef ShelfContainer::iterator        ShelfIterator;
            /// @brief Const Iterator type for Shelves stored by this class.
            typedef ShelfContainer::const_iterator  ConstShelfIterator;
        protected:
            /// @brief Shelf storage for this texture packer.
            ShelfContainer Shelves;
            /// @brief The current Y position of the most recent Shelf.
            Integer CurrentY;

            /// Returns true if there is still room in the bin to start a new shelf of the given height.
            Boole CanStartNewShelf(const Integer Height) const;
            /// Creates a new shelf of the given starting height, which will become the topmost 'open' shelf.
            void StartNewShelf(const Integer InitialHeight);

            /// @brief Places a Rect at the end of the most recent shelf.
            /// @param RectSize The size of the Rect to be placed.
            /// @return Returns a PlacementResult that is the placed Rect and if it has been rotated.
            PlacementResult HandleNextFit(const RectSizeType& RectSize);
            /// @brief Places a Rect at the end of the first shelf it will fit on.
            /// @param RectSize The size of the Rect to be placed.
            /// @return Returns a PlacementResult that is the placed Rect and if it has been rotated.
            PlacementResult HandleFirstFit(const RectSizeType& RectSize);
            /// @brief Places a Rect on the shelf with the least remaining area that the Rect fits in.
            /// @param RectSize The size of the Rect to be placed.
            /// @return Returns a PlacementResult that is the placed Rect and if it has been rotated.
            PlacementResult HandleBestAreaFit(const RectSizeType& RectSize);
            /// @brief Places a Rect on the shelf with the most remaining area that the Rect fits in.
            /// @param RectSize The size of the Rect to be placed.
            /// @return Returns a PlacementResult that is the placed Rect and if it has been rotated.
            PlacementResult HandleWorstAreaFit(const RectSizeType& RectSize);
            /// @brief Places a Rect on the shelf with the closest matching height without going over.
            /// @param RectSize The size of the Rect to be placed.
            /// @return Returns a PlacementResult that is the placed Rect and if it has been rotated.
            PlacementResult HandleBestHeightFit(const RectSizeType& RectSize);
            /// @brief Places a Rect on the shelf with the least remaining width after packing.
            /// @param RectSize The size of the Rect to be placed.
            /// @return Returns a PlacementResult that is the placed Rect and if it has been rotated.
            PlacementResult HandleBestWidthFit(const RectSizeType& RectSize);
            /// @brief Places a Rect on the shelf with the most remaining width after packing.
            /// @param RectSize The size of the Rect to be placed.
            /// @return Returns a PlacementResult that is the placed Rect and if it has been rotated.
            PlacementResult HandleWorstWidthFit(const RectSizeType& RectSize);
        public:
            /// @brief No Init constructor.
            /// @remarks Initial texture size will be zero.  Must call Init after this constructor.
            ShelfTexturePacker();
            /// @brief Copy constructor.
            /// @param Other The other packer to be copied.
            ShelfTexturePacker(const ShelfTexturePacker& Other) = default;
            /// @brief Move constructor.
            /// @param Other The other packer to be moved.
            ShelfTexturePacker(ShelfTexturePacker&& Other) = default;
            /// @brief Initialization constructor.
            /// @param TexWidth The width of the texture to be packed.
            /// @param TexHeight The height of the texture to be packed.
            ShelfTexturePacker(const Whole TexWidth, const Whole TexHeight);
            /// @brief Class destructor.
            virtual ~ShelfTexturePacker() = default;

            ///////////////////////////////////////////////////////////////////////////////
            // Initialization

            /// @copydoc TexturePacker::Initialize(const Whole, const Whole)
            virtual void Initialize(const Whole TexWidth, const Whole TexHeight) override;

            ///////////////////////////////////////////////////////////////////////////////
            // Insertion

            /// @copydoc TexturePacker::Insert(const Image*)
            /// @remarks This method calls the more descriptive Insert method with (SP_BestAreaFit)
            /// as sane defaults.  Call the other Insert method directly for more control.
            virtual PlacementResult Insert(const Image* ToInsert) override;
            /// @copydoc TexturePacker::Insert(const ImageContainer&, PackResultHandler*)
            /// @remarks This method calls the more descriptive Insert method with (SP_BestAreaFit)
            /// as sane defaults.  Call the other Insert method directly for more control.
            virtual ImageContainer Insert(const ImageContainer& ToPack, PackResultHandler* Handler) override;

            /// @brief Inserts a single image into the atlas.
            /// @remarks The packer might rotate the rectangle, in which case the returned struct will have the width and height values swapped.
            /// @n Merging will defragment the free rectangles and may improve performance when finding a free rect, but will cause this method
            /// call to take longer.  It may improve performance, it may not.
            /// @param ToInsert The image to be inserted.
            /// @param RectChoice The rect placement heuristic rule to use.
            /// @return Returns a Rect of where the image was placed.  The Rect may have invalid dimensions if the image wasn't placed.
            virtual PlacementResult Insert(const Image* ToInsert, const ShelfPlacement RectChoice);
            /// @brief Inserts a series of images into the atlas.
            /// @remarks The packer might rotate the rectangle, in which case the returned struct will have the width and height values swapped.
            /// @n Merging will defragment the free rectangles and may improve performance when finding a free rect, but will cause this method
            /// call to take longer.  It may improve performance, it may not.
            /// @param ToPack The collection of Images to be packed.
            /// @param Handler A pointer to the handler that will process the packing result.
            /// @param RectChoice The rect placement heuristic rule to use.
            /// @return Returns a container of the unpacked images.
            virtual ImageContainer Insert(const ImageContainer& ToPack, PackResultHandler* Handler, const ShelfPlacement RectChoice);

            ///////////////////////////////////////////////////////////////////////////////
            // Query

            /// @brief Gets the container of shelves being used to place Rects.
            /// @return Returns a const reference to the shelf container used by this texture packer.
            const ShelfContainer& GetShelves() const;
            /// @copydoc TexturePacker::GetCoverage() const
            virtual Real GetCoverage() const override;
        };//ShelfTexturePacker
    }//Graphics
}//Mezzanine

#endif
