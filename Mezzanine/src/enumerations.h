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
#ifndef _enumerations_h
#define _enumerations_h

#include "datatypes.h"

///////////////////////////////////////////////////////////////////////////////
/// @file
/// @brief Any global enumerations shared between multiple classes is to be declared here.
///////////////////////////////////////

namespace Mezzanine
{
    /// @brief Method to assist the conversion of enum class values to ints.
    /// @param Val The enumeration value to be converted.
    /// @return Returns an int of the underlying type of the enum.
    template<typename Enumeration>
    constexpr typename std::underlying_type<Enumeration>::type EnumClassToInt(const Enumeration Val)
    {
        static_assert( std::is_enum<Enumeration>::value, "Value received does not belong to an enum or enum class." );
        return static_cast< typename std::underlying_type<Enumeration>::type >(Val);
    }

    /// @brief Convenience method for getting the value of the n-th bit.  Useful for setting bit-fields.
    /// @param Bit The n-th bit to get the value of.  Range of 1-64.
    /// @return Returns the value at the specified bit position.
    constexpr UInt64 EnumBit(const Whole Bit)
    {
        return ( 1 << ( Bit - 1 ) );
    }

    /// @brief These values represent the kind of attenuation applied to the field strength over a distance.
    /// @details None is the default, where the force is constant in all area's of the field.  @n
    /// Linear is where the force applied drops by the attenuation value times the distance (strength - (attenuation amount * distance to AE center)).  @n
    /// Quadratic is where the force applied drops by the attenuation value times the distance squared (strength - (attenuation amount * distance to AE center * distance to AE center)).
    enum AttenuationStyle
    {
        Att_None,        ///< No Attenuation, Equal strength through.
        Att_Linear,      ///< Linear attenuation, Strength weaker farther from center.
        Att_Quadratic    ///< Quadratic/Exponential Attenuation, similar to real gravity, it tapers of more the further from the center you get.
    };

    /// @brief Used by all World proxies to describe what their derived types are.
    enum ComponentType
    {
        CT_ComponentLast = 999,  ///< Placeholder for sorting
        CT_ProxyFirst = 1000,    ///< Placeholder for sorting

        CT_Audio_First = 1100,
        CT_Audio_Listener,
        CT_Audio_SoundProxy,
        CT_Audio_Last,

        CT_Graphics_First = 1200,
        CT_Graphics_BillboardSetProxy,
        CT_Graphics_CameraProxy,
        CT_Graphics_ItemProxy,
        CT_Graphics_LightProxy,
        CT_Graphics_LineGroupProxy,
        CT_Graphics_ParticleSystemProxy,
        CT_Graphics_Last,

        CT_Physics_First = 1300,
        CT_Physics_GhostProxy,
        CT_Physics_RigidProxy,
        CT_Physics_SoftProxy,
        CT_Physics_Last
    };

    /// @brief Used by various classes to help identify what class an object is.
    /// @details This enum can be used to express any object which could be considered "insertable" into the game world.
    enum EntityType
    {
        ET_None                   = 0,          ///< Used to help report error conditions.
        // Generic Entities
        ET_Generic                = EnumBit(1),
        // Terrain Objects
        ET_TerrainFirst           = EnumBit(2),

        ET_MeshTerrain            = EnumBit(2),
        ET_HeightfieldTerrain     = EnumBit(3),
        ET_VectorFieldTerrain     = EnumBit(4),
        ET_VoxelTerrain           = EnumBit(5),
        ET_MarchingCubeTerrain    = EnumBit(6),
        ET_UnknownTerrain         = EnumBit(7),

        ET_TerrainLast            = EnumBit(7),

        ET_AllTerrains            = ET_MeshTerrain | ET_HeightfieldTerrain | ET_VectorFieldTerrain |
                                    ET_VoxelTerrain | ET_MarchingCubeTerrain | ET_UnknownTerrain,

        // Debris Objects
        ET_DebrisFirst            = EnumBit(8),

        ET_RigidDebris            = EnumBit(8),
        ET_SoftDebris             = EnumBit(9),
        ET_UnknownDebris          = EnumBit(10),

        ET_DebrisLast             = EnumBit(10),

        ET_AllDebris              = ET_RigidDebris | ET_SoftDebris | ET_UnknownDebris,

        // AreaEffect Objects
        ET_AreaEffectFirst        = EnumBit(11),

        ET_AreaEffectGravityField = EnumBit(11),
        ET_AreaEffectGravityWell  = EnumBit(12),
        ET_AreaEffectFieldOfForce = EnumBit(13),
        ET_AreaEffectPlaceHolder1 = EnumBit(14),
        ET_AreaEffectPlaceHolder2 = EnumBit(15),
        ET_AreaEffectUnknown      = EnumBit(16),

        ET_AreaEffectLast         = EnumBit(16),

        ET_AllAreaEffects         = ET_AreaEffectGravityField | ET_AreaEffectGravityWell | ET_AreaEffectFieldOfForce |
                                    ET_AreaEffectPlaceHolder1 | ET_AreaEffectPlaceHolder2 | ET_AreaEffectUnknown,

        // Actor Objects
        ET_ActorFirst             = EnumBit(17),

        ET_ActorPlaceHolder1      = EnumBit(17),
        ET_ActorPlaceHolder2      = EnumBit(18),
        ET_ActorPlaceHolder3      = EnumBit(19),

        ET_ActorLast              = EnumBit(19),

        ET_AllActors              = ET_ActorPlaceHolder1 | ET_ActorPlaceHolder2 | ET_ActorPlaceHolder3,

        // Vehicle Objects
        ET_VehicleFirst           = EnumBit(20),

        ET_MultiBodyVehicle       = EnumBit(20),
        ET_RaycastVehicle         = EnumBit(21),
        ET_SimpleVehicle          = EnumBit(22),
        ET_VehiclePlaceHolder1    = EnumBit(23),
        ET_VehiclePlaceHolder2    = EnumBit(24),

        ET_VehicleLast            = EnumBit(24),

        ET_AllVehicles            = ET_MultiBodyVehicle | ET_RaycastVehicle | ET_SimpleVehicle |
                                    ET_VehiclePlaceHolder1 | ET_VehiclePlaceHolder2
    };

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

    /// @brief Used by the world class to describe the extent of pausing a world.
    enum PauseLevel
    {
        PL_Unpaused          = 0,

        PL_PausePhysics      = EnumBit(1),
        PL_PauseParticles    = EnumBit(2),
        PL_PauseAnimations   = EnumBit(3),

        PL_PauseAll          = PL_PausePhysics | PL_PauseParticles | PL_PauseAnimations
    };

    /// @brief Used to identify different Axis in a 3d coordinate system.
    /// @note These are compatible with the linear Axis on many constraints, but not the rotational axis.
    enum StandardAxis
    {
        Axis_Invalid   = -1,        ///< Not an axis, Don't pass this into functions or operator[] functions, it is intended as an error value.
        Axis_X         = 0,         ///< X axis
        Axis_Min       = Axis_X,     ///< For mathematical shortcuts this can be used as the lower limit for valid axis
        Axis_Y         = 1,         ///< Y axis
        Axis_Z         = 2,         ///< Z axis
        Axis_Max       = Axis_Y     ///< For mathematical shortcuts this can be used as the upper limit
    };

    /// @brief Used by Track classes to define the type of curves the track has.
    enum TrackType
    {
        TT_Simple = 1,
        TT_Spline = 2,
        TT_Bezier = 3
    };

    /// @brief Used to define what frame of reference is to be used when positioning or rotating objects.
    enum TransformSpace
    {
        TS_Local  = 0,    ///< Local space, aka the object in questions world position is used as origin.
        TS_Parent = 1,    ///< Mostly reserved for rotations, means a rotation to occur around the parent instead of self.
        TS_World  = 2     ///< World space
    };
}//Mezzanine

#endif
