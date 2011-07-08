//© Copyright 2010 - 2011 BlackTopp Studios Inc.
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
    /// @enum ActorActivationState
    /// @brief This is used by all the actors to monitor and set their physics activation.
    enum ActorActivationState
    {
        AAS_Active = 1,
        AAS_Island_Sleeping = 2,
        AAS_Wants_Deactivation = 3,
        AAS_DisableDeactivation = 4,
        AAS_DisableSimulation = 5
    };

    /// @enum AttenuationStyle
    /// @brief These values represent the kind of attenuation applied to the field strength
    /// over a distance.
    /// @details None is the default, where the force is constant in all area's of the
    /// field.  Linear is where the force applied drops by the attenuation value times the distance
    /// (strength - (attenuation amount * distance to AE center)).  Quadratic is where the force
    /// applied drops by the attenuation value times the distance squared (strength -
    /// (attenuation amount * distance to AE center * distance to AE center)).
    enum AttenuationStyle
    {
        Att_None,        ///< No Attentuation, Equal strength through.
        Att_Linear,      ///< Linear attentuation, Strength weaker farther from center.
        Att_Quadratic    ///< Quadratic/Exponential Attentuation, similar to real gravity, it tapers of more the further from the center you get.
    };

    /// @enum CollisionFilter
    /// @brief These are the various filters that can be applied to all physics accelerated world objects to limit what they collide with.
    /// @details You can create your own groups on top of the ones that exist here, but they have to be powers of 2, and not overlap with
    /// any of the existing groups defined here.  Simply pass in the Whole where appropriate.
    enum CollisionFilter
    {
        // Standard base filters
        CF_GenericFilter = 1,
        CF_StaticFilter = 2,
        CF_KinematicFilter = 4,
        CF_DebrisFilter = 8,
        CF_SensorFilter = 16,
        CF_CharacterFilter = 32,
        CF_AllFilter = -1,

        // Non-Standard Filters
        CF_UserFilter1 = 64,
        CF_UserFilter2 = 128
    };

    /// @enum ConstraintParams
    /// @brief Used by constraints for setting some parameters.
    /// @details See the constraint class documentation for more details.
    enum ConstraintParams
    {
        Con_ERP = 1,
        Con_Stop_ERP,
        Con_CFM,
        Con_Stop_CFM
    };

    /// @enum WorldObjectType
    /// @brief Used by various classes to help identify what class an object is.
    /// @details This is mostly used internally for casting void pointers.
    enum WorldObjectType
    {
        // Actors
        WOT_ActorBase = 1,
        WOT_ActorRigid = 2,
        WOT_ActorSoft = 4,
        WOT_ActorCharacter = 8,
        WOT_ActorTerrain = 16,

        // AE Fields
        WOT_AreaEffect = 32,
        WOT_CustomAE  = 64,
        WOT_GravityField = 128,
        WOT_GravityWell = 256,

        // Scene Objects
        WOT_Light = 512,
        WOT_ParticleEffect = 1024,
        WOT_Camera = 2048
    };

    namespace UI
    {
        /// @enum ActivationCondition
        /// @brief Used primarily by UI buttons to decide how/when a button is activated.
        /// @details Essentially used to decide if a button should activate when it's activation button
        /// is pressed or lifted.
        enum ActivationCondition
        {
            AC_OnPress,
            AC_OnLift
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
        /// @enum SpinnerStyle
        /// @brief Used by the spinner class to determine what styling should be used for the spinner.
        /// @details In the case of the buttons being together, the increment button will be above the decrement button.
        enum SpinnerStyle
        {
            Spn_Separate,
            Spn_Together_Left,
            Spn_Together_Right
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
