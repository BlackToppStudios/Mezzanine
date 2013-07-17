//© Copyright 2010 - 2013 BlackTopp Studios Inc.
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

#include "macros.h"

///////////////////////////////////////////////////////////////////////////////
/// @file
/// @brief Any global enumerations shared between multiple classes is to be declared here.
///////////////////////////////////////

namespace Mezzanine
{
    /// @brief Used to indicate what kind of resources the Entrosol should look for
    enum ArchiveType
    {
        AT_FileSystem  = 0,    ///< Look for raw files
        AT_Zip         = 1,    ///< Look for stuff in zip files even if the extension is not '.zip'.
        AT_Invalid     = 32768 ///< Indicates this valid was messed up unrecoverably, most likely by a bug.
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

    /// @enum OrientationMode
    /// @brief Simple enum for communicating the orientation the UI and Camera have relative to the world it is rendering.
    /// @details This enum is used by the UI and graphics sub-systems for when a change in orientation is detected.  Under normal circumstances
    /// this kind of thing should only occur in mobile devices such as SmartPhones and Tablets.  However this can be forced on other devices/platforms.
    enum OrientationMode
    {
        OM_Degree_0 = 0,
        OM_Degree_90 = 1,
        OM_Degree_180 = 2,
        OM_Degree_270 = 3,

        OM_Portrait = OM_Degree_0,
        OM_LandscapeRight = OM_Degree_90,
        OM_LandscapeLeft = OM_Degree_270
    };

    /// @enum PauseLevel
    /// @brief Used by the world class to describe the extent of pausing a world.
    enum PauseLevel
    {
        PL_Unpaused          = MEZZ_BITMASK(0),

        PL_PausePhysics      = MEZZ_BITMASK(1),
        PL_PauseParticles    = MEZZ_BITMASK(2),
        PL_PauseAnimations   = MEZZ_BITMASK(3),

        PL_PauseAll          = MEZZ_BITMASK32MAX
    };

    /// @enum StandardAxis
    /// @brief Used to identify different Axis in a 3d coordinate system.
    /// @note These are compatible with the linear Axis on many constraints, but not the rotational axis.
    enum StandardAxis
    {
        Axis_Invalid   = -1,   ///< Not an axis, Don't pass this into functions or operator[] functions, it is intended as a
        Axis_X         = 0,    ///< X axis
        Axis_Y         = 1,    ///< Y axis
        Axis_Z         = 2     ///< Z axis
    };

    /// @enum TrackType
    /// @brief Used by Track classes to define the type of curves the track has.
    enum TrackType
    {
        TT_Simple = 1,
        TT_Spline = 2,
        TT_Bezier = 3
    };

    /// @enum TransformSpace
    /// @brief Used to define what frame of reference is to be used when positioning or rotating objects.
    enum TransformSpace
    {
        TS_Local  = 0,    ///< Local space, aka the object in questions world position is used as origin.
        TS_Parent = 1,    ///< Mostly reserved for rotations, means a rotation to occur around the parent instead of self.
        TS_World  = 2     ///< World space
    };

    /// @enum WorldAndSceneObjectType
    /// @brief Used by various classes to help identify what class an object is.
    /// @details This enum can be used to express any object which could be considered "insertable" into the game world.
    enum WorldAndSceneObjectType
    {
        // Actors
        WSO_ActorFirst             = MEZZ_BITMASK(1),//1,
        WSO_ActorRigid             = MEZZ_BITMASK(1),//1,
        WSO_ActorSoft              = MEZZ_BITMASK(2),//2,
        WSO_ActorCharacter         = MEZZ_BITMASK(3),//4,
        WSO_ActorUnknown           = MEZZ_BITMASK(4),//8,
        WSO_ActorLast              = MEZZ_BITMASK(4),//8,

        // Terrains go here
        WSO_TerrainFirst           = MEZZ_BITMASK(5),//16,
        WSO_MeshTerrain            = MEZZ_BITMASK(5),//16,
        WSO_UnimplementedTerrain1  = MEZZ_BITMASK(6),//32, // should rename to Heightfield terrain
        WSO_UnimplementedTerrain2  = MEZZ_BITMASK(7),//64, // should rename to Vectorfield terrain
        WSO_UnimplementedTerrain3  = MEZZ_BITMASK(8),//128, // should rename to Voxel Terrain
        WSO_UnimplementedTerrain4  = MEZZ_BITMASK(9),//256, // should rename to MarchingCube Terrain
        WSO_UnknownTerrain         = MEZZ_BITMASK(10),//512,
        WSO_TerrainLast            = MEZZ_BITMASK(10),//512,

        // AE Fields
        WSO_AEFirst                = MEZZ_BITMASK(11),//1024,
        WSO_AEGravityField         = MEZZ_BITMASK(11),//1024,
        WSO_AEGravityWell          = MEZZ_BITMASK(12),//2048,
        WSO_AEFieldOfForce         = MEZZ_BITMASK(13),//4096,
        WSO_AEPlaceHolder1         = MEZZ_BITMASK(14),//8192,
        WSO_AEPlaceHolder2         = MEZZ_BITMASK(15),//16384,
        WSO_AEUnknown              = MEZZ_BITMASK(16),//32768,
        WSO_AELast                 = MEZZ_BITMASK(16),//32768,

        // Scene Objects
        WSO_Camera                 = MEZZ_BITMASK(17),//65536,
        WSO_Entity                 = MEZZ_BITMASK(18),//131072,
        WSO_Light                  = MEZZ_BITMASK(19),//262144,
        WSO_ParticleEffect         = MEZZ_BITMASK(20),//524288,

        // Other Objects
        WSO_WorldNode              = MEZZ_BITMASK(21)//1048576
    };
}

#endif
