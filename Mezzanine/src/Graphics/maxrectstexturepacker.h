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

#ifndef _graphicsmaxrectstexturepacker_h
#define _graphicsmaxrectstexturepacker_h

#include "Graphics/texturepacker.h"

namespace Mezzanine
{
    namespace Graphics
    {
        /// @brief Specifies the different heuristic rules used by the MaxRectsTexturePacker that can be used when deciding where to place a new rectangle.
        enum MaxRectsPlacement
        {
            MRP_BestShortSideFit, ///< -BSSF: Positions the rectangle against the short side of a free rectangle into which it fits the best.
            MRP_BestLongSideFit,  ///< -BLSF: Positions the rectangle against the long side of a free rectangle into which it fits the best.
            MRP_BestAreaFit,      ///< -BAF: Positions the rectangle into the smallest free rect into which it fits.
            MRP_TopLeftRule,      ///< -BL: Maximizes placement towards the top left of the atlas.
            MRP_ContactPointRule  ///< -CP: Choose the placement where the rectangle touches other rects as much as possible.
        };

        ///////////////////////////////////////////////////////////////////////////////
        /// @brief This uses the MaxRects algorithm to subdivide free rects to fit placed rects efficiently.
        /// @details The MaxRects algorithm operates by finding rects that fit into an available corner and then
        /// subdivide the remaining 'L' shape into two overlapping free rectangles that can be placed into.  This
        /// incurs additional checks to keep the free rectangle cache accurate, but ensures placement of a rect
        /// won't be missed due to a poorly chosen split axis.
        ///////////////////////////////////////
        class MEZZ_LIB MaxRectsTexturePacker : public TexturePacker
        {
        protected:
            /// @brief Stores a list of all the rectangles that we have packed so far.
            RectVector UsedRectangles;
            /// @brief Stores a list of rectangles that represents the free area of the bin.
            RectVector FreeRectangles;

            /// @brief Computes the placement score for placing the given rectangle with the given method.
            /// @param RectSize The size of the Rect to be placed.
            /// @param Method The placement heuristic to use when scoring.
            /// @return Returns a struct containing the ideal position for the method and it's score for that placement.
            RectScoreResult ScoreRect(const RectSizeType& RectSize, const MaxRectsPlacement Method) const;
            /// @brief Places the given rectangle into the bin.
            /// @param Node The rect being placed.
            void PlaceRect(const RectType& Node);

            /// @brief Computes the placement score for the -CP variant.
            /// @param XPos The X axis position of the rect being placed.
            /// @param YPos The Y axis position of the rect being placed.
            /// @param RectSize The size of the Rect to be placed.
            /// @return Returns the score for the rect placement, lower is better.
            Integer ContactPointScoreNode(const Integer XPos, const Integer YPos, const RectSizeType& RectSize) const;
            /// @brief Splits a free node based on a newly placed rect.
            /// @param FreeNode The free node being placed into.
            /// @param UsedNode The new node being placed.
            /// @return Returns true if there was a successful split, false otherwise.
            Boole SplitFreeNode(const RectType& FreeNode, const RectType& UsedNode);

            /// @brief Finds the ideal Rect position based on top-left placement.
            /// @param RectSize The size of the Rect to be placed.
            /// @return Returns a struct containing the ideal position for the method and it's score for that placement.
            RectScoreResult FindPositionForTopLeft(const RectSizeType& RectSize) const;
            /// @brief Finds the ideal Rect position based on the smallest remaining side.
            /// @param RectSize The size of the Rect to be placed.
            /// @return Returns a struct containing the ideal position for the method and it's score for that placement.
            RectScoreResult FindPositionForBestShortSideFit(const RectSizeType& RectSize) const;
            /// @brief Finds the ideal Rect position based on the largest remaining side.
            /// @param RectSize The size of the Rect to be placed.
            /// @return Returns a struct containing the ideal position for the method and it's score for that placement.
            RectScoreResult FindPositionForBestLongSideFit(const RectSizeType& RectSize) const;
            /// @brief Finds the ideal Rect position based on the most overall area consumed.
            /// @param RectSize The size of the Rect to be placed.
            /// @return Returns a struct containing the ideal position for the method and it's score for that placement.
            RectScoreResult FindPositionForBestAreaFit(const RectSizeType& RectSize) const;
            /// @brief Finds the ideal Rect position based on touching edges.
            /// @param RectSize The size of the Rect to be placed.
            /// @return Returns a struct containing the ideal position for the method and it's score for that placement.
            RectScoreResult FindPositionForContactPoint(const RectSizeType& RectSize) const;
        public:
            /// @brief No Init constructor.
            /// @remarks Initial texture size will be zero.  Must call Init after this constructor.
            MaxRectsTexturePacker();
            /// @brief Initialization constructor.
            /// @param TexWidth The width of the texture to be packed.
            /// @param TexHeight The height of the texture to be packed.
            MaxRectsTexturePacker(const Whole TexWidth, const Whole TexHeight);
            /// @brief Class destructor.
            ~MaxRectsTexturePacker();

            /// @brief Removed redundant free rectangle entries.
            void PruneFreeList();

            ///////////////////////////////////////////////////////////////////////////////
            // Initialization

            /// @copydoc TexturePacker::Initialize(const Whole, const Whole)
            void Initialize(const Whole TexWidth, const Whole TexHeight);

            ///////////////////////////////////////////////////////////////////////////////
            // Insertion

            /// @copydoc TexturePacker::Insert(const Image*)
            /// @remarks This method calls the more descriptive Insert method with (true,MRP_TopLeftRule)
            /// as sane defaults.  Call the other Insert method directly for more control.
            PlacementResult Insert(const Image* ToInsert);
            /// @copydoc TexturePacker::Insert(const ImageContainer&, PackResultHandler*)
            /// @remarks This method calls the more descriptive Insert method with (true,MRP_TopLeftRule)
            /// as sane defaults.  Call the other Insert method directly for more control.
            ImageContainer Insert(const ImageContainer& ToPack, PackResultHandler* Handler);

            /// @brief Inserts a single image into the atlas.
            /// @remarks The packer might rotate the rectangle, in which case the returned struct will have the width and height values swapped.
            /// @n Merging will defragment the free rectangles and may improve performance when finding a free rect, but will cause this method
            /// call to take longer.  It may improve performance, it may not.
            /// @param ToInsert The image to be inserted.
            /// @param Merge Whether or not to perform free Rectangle Merge procedure after packing the new rectangle.
            /// @param RectChoice The rect placement heuristic rule to use.
            /// @return Returns a Rect of where the image was placed.  The Rect may have invalid dimensions if the image wasn't placed.
            PlacementResult Insert(const Image* ToInsert, const Boole Merge, const MaxRectsPlacement RectChoice);
            /// @brief Inserts a series of images into the atlas.
            /// @remarks The packer might rotate the rectangle, in which case the returned struct will have the width and height values swapped.
            /// @n Merging will defragment the free rectangles and may improve performance when finding a free rect, but will cause this method
            /// call to take longer.  It may improve performance, it may not.
            /// @param ToPack The collection of Images to be packed.
            /// @param Handler A pointer to the handler that will process the packing result.
            /// @param Merge Whether or not to perform free Rectangle Merge procedure after packing the new rectangle.
            /// @param RectChoice The rect placement heuristic rule to use.
            /// @return Returns a container of the unpacked images.
            ImageContainer Insert(const ImageContainer& ToPack, PackResultHandler* Handler, const Boole Merge, const MaxRectsPlacement RectChoice);

            ///////////////////////////////////////////////////////////////////////////////
            // Query

            /// @brief Gets a container of disjoint rectangles that track the free area of the texture.
            /// @return Returns a const reference to a container of Rects representing the free space available.
            const RectVector& GetFreeRectangles() const;
            /// @brief Gets all of the currently packed rectangles.
            /// @return Returns a const reference to a container of Rects representing the used space.
            const RectVector& GetUsedRectangles() const;
            /// @copydoc TexturePacker::GetCoverage() const
            Real GetCoverage() const;
        };//MaxRectsTexturePacker
    }//Graphics
}//Mezzanine

#endif
