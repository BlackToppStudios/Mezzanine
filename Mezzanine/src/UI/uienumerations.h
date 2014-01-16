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
#ifndef _uienumerations_h
#define _uienumerations_h
///////////////////////////////////////////////////////////////////////////////
//Any global enumerations shared between multiple classes in the UI namespace is to be declared here.
///////////////////////////////////////

namespace Mezzanine
{
    namespace UI
    {
        /// @enum AspectRatioLock
        /// @brief Used by sizing behavior classes to determine how resizes that preserve aspect ratio should behave.
        enum AspectRatioLock
        {
            ARL_Ratio_Unlocked = 0,          ///< The aspect ratio is not locked and both dimensions can resize freely.
            ARL_Ratio_Locked_Expanding = 1,  ///< The aspect ratio is locked.  When multiple axes are resized the axis with more growth is used to determine the size of the other axis.
            ARL_Ratio_Locked_Shrinking = 2   ///< The aspect ratio is locked.  When multiple axes are resized the axis with less growth is used to determine the size of the other axis.
        };

        /// @enum Border
        /// @brief Used by various basic renderables for border effects.
        /// @details Internally border information is stored in small arrays, and this facilitates the proper index being called every time.
        enum Border
        {
            Border_North = 0,
            Border_South = 1,
            Border_East  = 2,
            Border_West  = 3
        };

        /// @enum Gradient
        /// @brief Used by various basic renderables for applying a gradient effect to a colour or texture on a quad.
        /// @details Details regarding the colour or texture depends on the individual settings of the renderable the gradient is being applied to.
        /// Gradients simply allow one colour on one portion of a quad to fade into another.
        enum Gradient
        {
            Gradient_NorthSouth = 0,
            Gradient_WestEast   = 1,
            Gradient_Diagonal_1 = 2,
            Gradient_Diagonal_2 = 3
        };

        /// @enum LinearAlignment
        /// @brief Used by various UI classes to determine the alignment of their child objects, such as text in text lines, or quads in a layout container.
        /// @details This enum is only ever used to describe one axis at a time.  Thus the values do not represent
        /// corners, but rather the appropriate side on the axis in question.
        enum LinearAlignment
        {
            LA_TopLeft,
            LA_Center,
            LA_BottomRight
        };

        /// @enum OrderingPriority
        /// @brief Used by container widgets to determine which axis is considered primary for children/pages.
        enum OrderingPriority
        {
            OP_Horizontal_Vertical = 1,
            OP_Vertical_Horizontal = 2
        };

        /// @enum RepositionFlags
        /// @brief Used by UI container classes to determine their movement behavior when the container is resized.
        /// @details These flags are not used if the container object is simply repositioned.  Only when a new position
        /// is needed after the container is resized.  If rules for one axis isn't covered, the Unified position is used.
        enum PositioningFlags
        {
            PF_Unified_Pos       = 0,                             ///< Does not anchor to any side, using only the provided unified dimensions.
            PF_Top               = 1,                             ///< Anchors to the top side of the quad.
            PF_Bottom            = 2,                             ///< Anchors to the bottom side of the quad.
            PF_Left              = 4,                             ///< Anchors to the left side of the quad.
            PF_Right             = 8,                             ///< Anchors to the right side of the quad.

            PF_TopLeft           = (PF_Top | PF_Left),            ///< Anchors to the top and left sides of the quad.
            PF_TopRight          = (PF_Top | PF_Right),           ///< Anchors to the top and right sides of the quad.
            PF_BottomLeft        = (PF_Bottom | PF_Left),         ///< Anchors to the bottom and left sides of the quad.
            PF_BottomRight       = (PF_Bottom | PF_Right),        ///< Anchors to the bottom and right sides of the quad.

            PF_VerticalCenter    = (PF_Top | PF_Bottom),          ///< Centers along the Y axis of the quad.
            PF_HorizontalCenter  = (PF_Left | PF_Right),          ///< Centers along the X axis of the quad.

            PF_Center            = (PF_TopLeft | PF_BottomRight)  ///< Centers within the quad.
        };//*/

        /// @enum QuadCorner
        /// @brief Used by Sprites and Glyphs for tracking their placement on a TextureAtlas.
        /// @details This helps to ensure the proper indexs are accessed at all times when getting UV information.
        enum QuadCorner
        {
            QC_TopLeft     = 0,
            QC_TopRight    = 1,
            QC_BottomRight = 2,
            QC_BottomLeft  = 3
        };

        /// @enum RenderPriority
        /// @brief Used by UI elements created by layers to determine z-ordering within a layer.
        /// @details This essentially means we're adding layers to our layers without the messy
        /// and un-intuitive re-coding necessary to actually accomplish that.
        enum RenderPriority
        {
            RP_Low,
            RP_Medium,
            RP_High
        };

        /// @enum ScrollbarStyle
        /// @brief Used by the scrollbar class to determine what styling should be used for the scrollbar.
        /// @details These values are mostly self explanitory, the scrollbar class documentation has more details.
        enum ScrollbarStyle
        {
            SB_NoButtons,
            SB_Separate,
            SB_TogetherUpLeft,
            SB_TogetherDownRight
        };

        /// @enum SizingRules
        /// @brief Used by UI container classes to determine their resize behavior on a given axis when the container is resized.
        enum SizingRules
        {
            SR_Unified_Dims               = 0, ///< Resizing will use the provided unified dimensions with no further alterations.  This is the default for most QuadRenderables.
            SR_Match_Other_Axis           = 1, ///< The calculated value for the perpendicular axis will be used as the final value for this axis.
            SR_Fill_Available             = 2, ///< Unified dimensions are ignored and will instead us all available space.
            SR_Size_For_Text              = 3, ///< Sizes the container so all text in layers will be visible.  Only valid for Vertical sizing(exception will be thrown).
            SR_Fixed_Size                 = 4  ///< No resizing of any kind will take place.
        };

        /// @enum SpinnerStyle
        /// @brief Used by the spinner class to determine what styling should be used for the spinner.
        /// @details In the case of the buttons being together, the increment button will be above the decrement button.
        enum SpinnerStyle
        {
            Spn_Separate,
            Spn_Together_Left,
            Spn_Together_Right
        };

        /// @enum TextOrdering
        /// @brief This enum describes the direction of advancing characters or lines along the X axis.
        enum TextOrdering
        {
            TO_Left_To_Right = 1, ///< Characters/New Lines originate from the left, and advance to the right.
            TO_Right_To_Left = 2  ///< Characters/New Lines originate from the right, and advance to the left.
        };
    }//UI
}//Mezzanine

#endif
