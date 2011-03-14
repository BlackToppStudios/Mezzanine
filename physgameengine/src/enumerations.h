//© Copyright 2010 BlackTopp Studios Inc.
/* This file is part of The PhysGame Engine.

    The PhysGame Engine is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The PhysGame Engine is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with The PhysGame Engine.  If not, see <http://www.gnu.org/licenses/>.
*/
/* The original authors have included a copy of the license specified above in the
   'Docs' folder. See 'gpl.txt'
*/
/* We welcome the use of The PhysGame anyone, including companies who wish to
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
#ifndef _enumerations_h
#define _enumerations_h
///////////////////////////////////////////////////////////////////////////////
//Any global enumerations shared between multiple classes is to be declared here.
///////////////////////////////////////

namespace phys
{
    /// @enum WorldObjectType
    /// @brief Used by various classes to help identify what class an object is.
    /// @details This is mostly used internally for casting void pointers.
    enum WorldObjectType
    {
        // Actors
        WOT_ActorBase,
        WOT_ActorRigid,
        WOT_ActorSoft,
        WOT_ActorRagdoll,
        WOT_ActorTerrain,

        // AE Fields
        WOT_AreaEffect,
        WOT_GravityField,
        WOT_GravityWell,

        // Scene Objects
        WOT_Light,
        WOT_ParticleEffect,
        WOT_Camera
    };

    namespace UI
    {
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
        /// @enum ResizeableAnchor
        /// @brief Used by UI container classes to determine their movement behavior when the container is resized.
        /// @details RA_AnchorTop means it stays in the same position if given a resize event relative to the top.
        /// RA_AnchorBottom means the same as RA_AnchorTop but for the bottom of the container.  RA_AnchorMiddle means
        /// it'll change position by half of the pixels that the container was resized.
        enum ResizeableAnchor
        {
            RA_AnchorTopLeft,
            RA_AnchorTop,
            RA_AnchorTopRight,
            RA_AnchorLeft,
            RA_AnchorMiddle,
            RA_AnchorRight,
            RA_AnchorBottomLeft,
            RA_AnchorBottom,
            RA_AnchorBottomRight
        };
        /// @enum ResizeableTether
        /// @brief Used by UI container classes to determine their resize behavior when the container is resized.
        /// @details RT_TetherHorizontal means it will resize itself equally during any Horizontal resizing of the container.
        /// RT_TetherVertical means the same as RT_TetherHorizontal, but for Vertical resizes.  RT_TetherBoth means it'll do both.
        enum ResizeableTether
        {
            RT_TetherBoth,
            RT_TetherNone,
            RT_TetherHorizontal,
            RT_TetherVertical
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
        /// @enum TextHorizontalAlign
        /// @brief Used by UI classes to determine the placement of text within a given area.
        /// @details This enum determines the Horizontal alignment of text.
        /// This enum is used by these classes in the UI namespace: Button and Caption.
        enum TextHorizontalAlign
        {
            Txt_Left,
            Txt_Right,
            Txt_Middle
        };
        /// @enum TextVerticalAlign
        /// @brief Used by UI classes to determine the placement of text within a given area.
        /// @details This enum determines the Vertical alignment of text.
        /// This enum is used by these classes in the UI namespace: Button and Caption.
        enum TextVerticalAlign
        {
            Txt_Top,
            Txt_Bottom,
            Txt_Center
        };
    }
}

#endif
