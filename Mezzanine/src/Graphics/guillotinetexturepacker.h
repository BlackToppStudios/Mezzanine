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

#ifndef _graphicsguillotinetexturepacker_h
#define _graphicsguillotinetexturepacker_h

#include "Graphics/texturepacker.h"

namespace Mezzanine
{
    namespace Graphics
    {
        /// @brief Specifies the different heuristic rules used by the GuillotineTexturePacker that can be used when deciding where to place a new rectangle.
        enum GuillotinePlacement
        {
            GP_BestAreaFit,       ///< -BAF
            GP_BestShortSideFit,  ///< -BSSF
            GP_BestLongSideFit,   ///< -BLSF
            GP_WorstAreaFit,      ///< -WAF
            GP_WorstShortSideFit, ///< -WSSF
            GP_WorstLongSideFit   ///< -WLSF
        };
        /// @brief This enum helps decide on which axis the free rect should be split to fit a new triangle.
        enum GuillotineSplit
        {
            GS_ShorterLeftoverAxis, ///< -SLAS
            GS_LongerLeftoverAxis,  ///< -LLAS
            GS_MinimizeArea,        ///< -MINAS, Try to make a single big rectangle at the expense of making the other small.
            GS_MaximizeArea,        ///< -MAXAS, Try to make both remaining rectangles as even-sized as possible.
            GS_ShorterAxis,         ///< -SAS
            GS_LongerAxis           ///< -LAS
        };

        ///////////////////////////////////////////////////////////////////////////////
        /// @brief This uses the Guillotine algorithm to subdivide free rects to fit placed rects efficiently.
        /// @details The Guillotine algorithm operates by finding rects that fit into an available corner and then
        /// subdivide the remaining 'L' shape on the horizontal or vertical and then repeating.
        ///////////////////////////////////////
        class MEZZ_LIB GuillotineTexturePacker : public TexturePacker
        {
        protected:
            /// @brief Stores a list of all the rectangles that we have packed so far.
            /// @remarks This is used only to compute the Occupancy ratio, so if you want to have the packer
            /// consume less memory, this can be removed.
            RectVector UsedRectangles;
            /// @brief Stores a list of rectangles that represents the free area of the bin.
            /// @remarks The rectangles in this list are disjoint.
            RectVector FreeRectangles;

            /// @brief Finds the best position for a new texture.
            /// @remarks Running time is Theta(|freeRectangles|).
            /// @param RectSize The size of the Rect to be placed.
            /// @param RectChoice Describes how to choose the ideal free Rect.
            /// @param NodeIndex The index of the free rectangle into which the new Rect was placed.
            /// @return Returns the placed Rect.
            PlacementResult FindPositionForNewNode(const RectSizeType& RectSize, const GuillotinePlacement RectChoice, Integer* NodeIndex) const;
            /// @brief Splits an L-shaped free rectangle into two new free rectangles after a Rect has been placed into it.
            /// @param FreeRect The intact free Rect prior to placement.
            /// @param PlacedRect The Rect being placed in the free Rect.
            /// @param Method Describes how the free Rect should be split into smaller free Rects.
            void SplitFreeRectByHeuristic(const RectType& FreeRect, const RectType& PlacedRect, const GuillotineSplit Method);
            /// @brief Splits an L-shaped free rectangle into two new free rectangles along the given fixed split axis.
            /// @param FreeRect The intact free Rect prior to placement.
            /// @param PlacedRect The Rect being placed in the free Rect.
            /// @param SplitHorizontal Whether or not the free Rect split should be performed along the horizontal axis when.
            void SplitFreeRectAlongAxis(const RectType& FreeRect, const RectType& PlacedRect, const Boole SplitHorizontal);

            /// @brief Determines the score of a free Rect for placing a new texture.
            /// @param RectSize The size of the Rect to be placed.
            /// @param FreeRect The free Rect to be scored.
            /// @param RectChoice The heuristic to use for scoring which free Rect to use for new texture placement.
            /// @return Returns the score of placing the new texture in the free Rect.  Lower is better.
            static Integer ScoreByHeuristic(const RectSizeType& RectSize, const RectType& FreeRect, const GuillotinePlacement RectChoice);
            /// @brief Determines the score of a free Rect based on the best overall area fit.
            /// @param RectSize The size of the Rect to be placed.
            /// @param FreeRect The free Rect to be scored.
            /// @return Returns the score of placing the new texture in the free Rect.  Lower is better.
            static Integer ScoreBestAreaFit(const RectSizeType& RectSize, const RectType& FreeRect);
            /// @brief Determines the score of a free Rect based on closely matching the short side.
            /// @param RectSize The size of the Rect to be placed.
            /// @param FreeRect The free Rect to be scored.
            /// @return Returns the score of placing the new texture in the free Rect.  Lower is better.
            static Integer ScoreBestShortSideFit(const RectSizeType& RectSize, const RectType& FreeRect);
            /// @brief Determines the score of a free Rect based on closely matching the long side.
            /// @param RectSize The size of the Rect to be placed.
            /// @param FreeRect The free Rect to be scored.
            /// @return Returns the score of placing the new texture in the free Rect.  Lower is better.
            static Integer ScoreBestLongSideFit(const RectSizeType& RectSize, const RectType& FreeRect);
            /// @brief Determines the score of a free Rect based on the worst overall area fit.
            /// @param RectSize The size of the Rect to be placed.
            /// @param FreeRect The free Rect to be scored.
            /// @return Returns the score of placing the new texture in the free Rect.  Lower is better.
            static Integer ScoreWorstAreaFit(const RectSizeType& RectSize, const RectType& FreeRect);
            /// @brief Determines the score of a free Rect based on the most leftover space for the long side.
            /// @param RectSize The size of the Rect to be placed.
            /// @param FreeRect The free Rect to be scored.
            /// @return Returns the score of placing the new texture in the free Rect.  Lower is better.
            static Integer ScoreWorstShortSideFit(const RectSizeType& RectSize, const RectType& FreeRect);
            /// @brief Determines the score of a free Rect based on the most leftover space for the short side.
            /// @param RectSize The size of the Rect to be placed.
            /// @param FreeRect The free Rect to be scored.
            /// @return Returns the score of placing the new texture in the free Rect.  Lower is better.
            static Integer ScoreWorstLongSideFit(const RectSizeType& RectSize, const RectType& FreeRect);
        public:
            /// @brief No Init constructor.
            /// @remarks Initial texture size will be zero.  Must call Init after this constructor.
            GuillotineTexturePacker();
            /// @brief Initialization constructor.
            /// @param TexWidth The width of the texture to be packed.
            /// @param TexHeight The height of the texture to be packed.
            GuillotineTexturePacker(const Whole TexWidth, const Whole TexHeight);
            /// @brief Class destructor.
            ~GuillotineTexturePacker();

            /// @brief Combines bordering free rectangles that can be merged.
            /// @remarks This procedure looks for adjacent free rectangles and merges them if they can be represented with
            /// a single rectangle. Takes up Theta(|freeRectangles|^2) time.
            void MergeFreeList();

            ///////////////////////////////////////////////////////////////////////////////
            // Initialization

            /// @copydoc TexturePacker::Initialize(const Whole, const Whole)
            void Initialize(const Whole TexWidth, const Whole TexHeight);

            ///////////////////////////////////////////////////////////////////////////////
            // Insertion

            /// @copydoc TexturePacker::Insert(const Image*)
            /// @remarks This method calls the more descriptive Insert method with (true,GP_BestAreaFit,GS_MinimizeArea)
            /// as sane defaults.  Call the other Insert method directly for more control.
            PlacementResult Insert(const Image* ToInsert);
            /// @copydoc TexturePacker::Insert(const ImageContainer&, PackResultHandler*)
            /// @remarks This method calls the more descriptive Insert method with (true,GP_BestAreaFit,GS_MinimizeArea)
            /// as sane defaults.  Call the other Insert method directly for more control.
            ImageContainer Insert(const ImageContainer& ToPack, PackResultHandler* Handler);

            /// @brief Inserts a single image into the atlas.
            /// @remarks The packer might rotate the rectangle, in which case the returned struct will have the width and height values swapped.
            /// @n Merging will defragment the free rectangles and may improve performance when finding a free rect, but will cause this method
            /// call to take longer.  It may improve performance, it may not.
            /// @param ToInsert The image to be inserted.
            /// @param Merge Whether or not to perform free Rectangle Merge procedure after packing the new rectangle.
            /// @param RectChoice The rect placement heuristic rule to use.
            /// @param SplitMethod The free rectangle split heuristic rule to use.
            /// @return Returns a Rect of where the image was placed.  The Rect may have invalid dimensions if the image wasn't placed.
            PlacementResult Insert(const Image* ToInsert, const Boole Merge, const GuillotinePlacement RectChoice, const GuillotineSplit SplitMethod);
            /// @brief Inserts a series of images into the atlas.
            /// @remarks The packer might rotate the rectangle, in which case the returned struct will have the width and height values swapped.
            /// @n Merging will defragment the free rectangles and may improve performance when finding a free rect, but will cause this method
            /// call to take longer.  It may improve performance, it may not.
            /// @param ToPack The collection of Images to be packed.
            /// @param Handler A pointer to the handler that will process the packing result.
            /// @param Merge Whether or not to perform free Rectangle Merge procedure after packing the new rectangle.
            /// @param RectChoice The rect placement heuristic rule to use.
            /// @param SplitMethod The free rectangle split heuristic rule to use.
            /// @return Returns a container of the unpacked images.
            ImageContainer Insert(const ImageContainer& ToPack, PackResultHandler* Handler, const Boole Merge, const GuillotinePlacement RectChoice, const GuillotineSplit SplitMethod);

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
        };//GuillotineTexturePacker
    }//Graphics
}//Mezzanine

#endif
