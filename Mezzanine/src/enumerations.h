//© Copyright 2010 - 2011 BlackTopp Studios Inc.
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
#ifndef _enumerations_h
#define _enumerations_h
///////////////////////////////////////////////////////////////////////////////
//Any global enumerations shared between multiple classes is to be declared here.
///////////////////////////////////////

namespace Mezzanine
{
    /// @enum AspectRatio
    /// @brief This is used primarily by the GraphicsManager to keep track of the supported
    /// and currently used aspect ratios.
    enum AspectRatio
    {
        AR_3_By_2,     ///< 1.5
        AR_4_By_3,     ///< 1.334
        AR_5_By_3,     ///< 1.667
        AR_5_By_4,     ///< 1.25
        AR_8_By_5,     ///< 1.6
        AR_16_By_9,    ///< 1.778
        AR_17_By_9,    ///< 1.889
        AR_1_85_By_1,  ///< 1.85
        AR_2_39_By_1   ///< 2.39
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

    /// @enum RenderSystem
    /// @brief Used primarily by the graphics manager class during initialization.
    /// @details This enum specifies which Rendersystem is to be used for rendering.
    enum RenderSystem
    {
        RS_DirectX9,
        RS_DirectX10,
        RS_DirectX11,
        RS_OpenGL2,
        RS_OpenGL3,
        RS_OpenGL4,
        RS_OpenGLES1,
        RS_OpenGLES2
    };

    /// @brief Used to identify different Axis in a 3d coordinate system.
    /// @note These are compatible with the linear Axis on many constraints, but not the rotational axis.
    enum StandardAxis
    {
        Axis_X         = 0,    ///< X axis
        Axis_Y         = 1,    ///< Y axis
        Axis_Z         = 2,    ///< Z axis
    };

    /// @enum WorldObjectActivationState
    /// @brief This is used by all the world objects to monitor and set their physics activation.
    enum WorldObjectActivationState
    {
        WOAS_Active = 1,
        WOAS_Island_Sleeping = 2,
        WOAS_Wants_Deactivation = 3,
        WOAS_DisableDeactivation = 4,
        WOAS_DisableSimulation = 5
    };

    /// @enum WorldObjectType
    /// @brief Used by various classes to help identify what class an object is.
    /// @details This is mostly used internally for casting void pointers.
    enum WorldObjectType
    {
        // Actors
        WOT_ActorFirst = 1,
        WOT_ActorRigid = 1,
        WOT_ActorSoft = 2,
        WOT_ActorCharacter = 4,
        WOT_ActorUnknown = 8,
        WOT_ActorLast = 8,

        // Terrains go here
        WOT_TerrainFirst = 16,
        WOT_MeshTerrain = 16,
        WOT_UnimplementedTerrain1 = 32, // should rename to Heightfield terrain
        WOT_UnimplementedTerrain2 = 64, // should rename to Vectorfield terrain
        WOT_UnimplementedTerrain3 = 128, // should rename to Voxel Terrain
        WOT_UnknownTerrain = 256,
        WOT_TerrainLast = 256,

        // AE Fields
        WOT_AEFirst = 512,
        WOT_AEGravityField = 512,
        WOT_AEGravityWell = 1024,
        WOT_AEFieldOfForce = 2048,
        WOT_AEPlaceHolder1 = 4096,
        WOT_AEPlaceHolder2 = 8192,
        WOT_AEUnknown = 16384,
        WOT_AELast = 16384,
    };
}

#endif
