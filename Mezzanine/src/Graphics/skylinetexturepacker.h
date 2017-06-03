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

#ifndef _graphicsskylinetexturepacker_h
#define _graphicsskylinetexturepacker_h

#include "Graphics/texturepacker.h"

namespace Mezzanine
{
    namespace Graphics
    {
        /// @brief Specifies the different heuristic rules used by the SkylineTexturePacker that can be used when deciding where to place a new rectangle.
        enum SkylinePlacement
        {
            SLP_TopLeft,       ///< Places the Rect in the most top-left position available in the atlas.
            SLP_BestAreaFit    ///< Places the Rect in the position with the minimal wasted space.
        };

        ///////////////////////////////////////////////////////////////////////////////
        /// @brief This is used by the SkylineTexturePacker algorithm to keep track of the skyline separating used and unused space.
        ///////////////////////////////////////
        struct MEZZ_LIB TextureSkylineNode
        {
            ///////////////////////////////////////////////////////////////////////////////
            // Data Members

            /// @brief The X axis start point of the skyline.
            Integer XPos;
            /// @brief The Y axis edge of the skyline.
            Integer YPos;
            /// @brief The width of this skyline.
            Integer LineWidth;

            ///////////////////////////////////////////////////////////////////////////////
            // Construction and Destruction

            /// @brief Position constructor.
            /// @param Width The width of this skyline.
            TextureSkylineNode(const Integer Width);
            /// @brief Descriptive constructor.
            /// @param XPosition The X axis start point of the skyline.
            /// @param YPosition The Y axis edge of the skyline.
            /// @param Width The width of this skyline.
            TextureSkylineNode(const Integer XPosition, const Integer YPosition, const Integer Width);
            /// @brief Class destructor.
            ~TextureSkylineNode() = default;

            ///////////////////////////////////////////////////////////////////////////////
            // Functionality

            /// @brief Gets the end point of the line segment for this skyline node.
            /// @return Returns this nodes XPos + LineWidth.
            Integer GetEndPoint() const;
            /// @brief Checks to see if a point on the X axis is within the width of this Skyline node.
            /// @return Returns true if the specified point is between the start and end points of this Skyline node.
            Boole PointIsWithin(const Integer Point) const;
            /// @brief Checks to see if this Skyline node overlaps with another.
            /// @return Returns true if this Skyline node occupies the same X-axis space as the other Skyline node, false otherwise.
            Boole Overlaps(const TextureSkylineNode& Other) const;
        };//TextureSkylineNode

        ///////////////////////////////////////////////////////////////////////////////
        /// @brief This uses the Skyline algorithm to easily track used and unused space in the atlas.
        /// @details The Skyline algorithm is somewhat simple, in that it attempts to track exclusively used and unused areas
        /// of the texture.  Placing Rects forms raises the skyline to envelope the placed Rect and any space between the
        /// Rect and the former skyline on that X-axis position.  The end result is a jagged line stretching across the
        /// atlas that resembles of a simple city skyline, if we assume the origin point is the bottom left of the atlas.
        /// This is not the case for us, the origin is in the top left with our addressing so the actual skyline would appear
        /// to shrink as Rects were added, but the algorithm survives this inversion just fine fortunately.  @n @n
        /// Space wastage with the Skyline algorithm is not as bad as it is with the Shelf algorithm, however it's not as good
        /// as with the Guillotine or MaxRects algorithms.  It does however consume the least amount of memory and in general is
        /// a decent middle ground between Shelf and the other algorithms.  Like with the Shelf algorithm, having many Rects of
        /// varying sizes will cause more space wastage as it doesn't compensate well for that.  If you do have many varying sized
        /// Rects to place than you should use the Guillotine or MaxRects algorithm instead.  @n @n
        /// The UsedRectangles data member is not strictly necessary in this class, as the skyline itself is all that is needed
        /// to place new Rects.  However it exists both for convenience/reference, and to help calculate the used space of the
        /// atlas.
        ///////////////////////////////////////
        class MEZZ_LIB SkylineTexturePacker : public TexturePacker
        {
        public:
            /// @brief Convenience type for a container of TextureSkylineNodes.
            typedef std::vector<TextureSkylineNode>   SkylineContainer;
            /// @brief Iterator type for TextureSkylineNodes stored by this class.
            typedef SkylineContainer::iterator        SkylineIterator;
            /// @brief Const Iterator type for TextureSkylineNodes stored by this class.
            typedef SkylineContainer::const_iterator  ConstSkylineIterator;
        protected:
            /// @brief Stores a list of all the rectangles that we have packed so far.
            RectVector UsedRectangles;
            /// @brief Stores each segment(node) of the skyline separating the used and unused space.
            SkylineContainer Skyline;

            /// @brief Alters the skyline to accommodate the placement of a new Rect.
            /// @param SkylineIndex The index of the skyline node the Rect is being placed on.
            /// @param Placed The Rect being placed on the skyline.
            void AddSkylineLevel(const Whole SkylineIndex, const RectType& Placed);
            /// @brief Checks to see if a Rectangle can fit being placed on top of a skyline node.
            /// @param SkylineIndex The index of the skyline node the Rect is being placed on.
            /// @param Width The width of the Rect to be placed.
            /// @param Height The height of the Rect to be placed.
            /// @return Returns the Y position of the top edge the Rect is to be placed, or -1 if the Rect does not fit.
            Integer RectangleFits(const Whole SkylineIndex, const Integer Width, const Integer Height) const;
            /// @brief Calculates the amount of wasted area if a Rect were placed.
            /// @param SkylineIndex The index of the TextureSkylineNode the Rect would be placed at.
            /// @param YPos The adjusted YPosition the top edge of the Rect should be placed, due to other Skyline nodes it would collide with.
            /// @param RectSize The size of the Rect to be placed.
            /// @return Returns the total area that will go to waste if the Rect is placed at the specified location, or -1 if an invalid placement is detected.
            Integer ComputeWastedArea(const Whole SkylineIndex, const Integer YPos, const RectSizeType& RectSize) const;

            /// @brief Finds the most ideal top-left position to place a Rect.
            /// @param RectSize The size of the Rect to be placed.
            /// @return Returns an std::pair with the index of the ideal skyline node to place on and the actual placed Rect.
            RectScoreResult FindPositionForTopLeft(const RectSizeType& RectSize) const;
            /// @brief Finds the best position with the least amount of wasted space.
            /// @param RectSize The size of the Rect to be placed.
            /// @return Returns an std::pair with the index of the ideal skyline node to place on and the actual placed Rect.
            RectScoreResult FindPositionForBestArea(const RectSizeType& RectSize) const;
        public:
            /// @brief No Init constructor.
            /// @remarks Initial texture size will be zero.  Must call Init after this constructor.
            SkylineTexturePacker() = default;
            /// @brief Copy constructor.
            /// @param Other The other packer to be copied.
            SkylineTexturePacker(const SkylineTexturePacker& Other) = default;
            /// @brief Move constructor.
            /// @param Other The other packer to be moved.
            SkylineTexturePacker(SkylineTexturePacker&& Other) = default;
            /// @brief Initialization constructor.
            /// @param TexWidth The width of the texture to be packed.
            /// @param TexHeight The height of the texture to be packed.
            SkylineTexturePacker(const Whole TexWidth, const Whole TexHeight);
            /// @brief Class destructor.
            virtual ~SkylineTexturePacker() = default;

            /// @brief Merges all skyline nodes that are at the same Y position and next to each other.
            void MergeSkylines();

            ///////////////////////////////////////////////////////////////////////////////
            // Initialization

            /// @copydoc TexturePacker::Initialize(const Whole, const Whole)
            virtual void Initialize(const Whole TexWidth, const Whole TexHeight) override;

            ///////////////////////////////////////////////////////////////////////////////
            // Insertion

            /// @copydoc TexturePacker::Insert(const Image*)
            /// @remarks This method calls the more descriptive Insert method with (SLP_BestAreaFit)
            /// as sane defaults.  Call the other Insert method directly for more control.
            virtual PlacementResult Insert(const Image* ToInsert) override;
            /// @copydoc TexturePacker::Insert(const ImageContainer&, PackResultHandler*)
            /// @remarks This method calls the more descriptive Insert method with (SLP_BestAreaFit)
            /// as sane defaults.  Call the other Insert method directly for more control.
            virtual ImageContainer Insert(const ImageContainer& ToPack, PackResultHandler* Handler) override;

            /// @brief Inserts a single image into the atlas.
            /// @remarks The packer might rotate the rectangle, in which case the returned struct will have the width and height values swapped.
            /// @n Merging will defragment the free rectangles and may improve performance when finding a free rect, but will cause this method
            /// call to take longer.  It may improve performance, it may not.
            /// @param ToInsert The image to be inserted.
            /// @param RectChoice The rect placement heuristic rule to use.
            /// @return Returns a Rect of where the image was placed.  The Rect may have invalid dimensions if the image wasn't placed.
            virtual PlacementResult Insert(const Image* ToInsert, const SkylinePlacement RectChoice);
            /// @brief Inserts a series of images into the atlas.
            /// @remarks The packer might rotate the rectangle, in which case the returned struct will have the width and height values swapped.
            /// @n Merging will defragment the free rectangles and may improve performance when finding a free rect, but will cause this method
            /// call to take longer.  It may improve performance, it may not.
            /// @param ToPack The collection of Images to be packed.
            /// @param Handler A pointer to the handler that will process the packing result.
            /// @param RectChoice The rect placement heuristic rule to use.
            /// @return Returns a container of the unpacked images.
            virtual ImageContainer Insert(const ImageContainer& ToPack, PackResultHandler* Handler, const SkylinePlacement RectChoice);

            ///////////////////////////////////////////////////////////////////////////////
            // Query

            /// @brief Gets all of the currently packed rectangles.
            /// @return Returns a const reference to a container of Rects representing the used space.
            const RectVector& GetUsedRectangles() const;
            /// @copydoc TexturePacker::GetCoverage() const
            virtual Real GetCoverage() const override;
        };//SkylineTexturePacker
    }//Graphics
}//Mezzanine

#endif
