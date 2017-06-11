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
        Att_None,        ///< No Attentuation, Equal strength through.
        Att_Linear,      ///< Linear attentuation, Strength weaker farther from center.
        Att_Quadratic    ///< Quadratic/Exponential Attentuation, similar to real gravity, it tapers of more the further from the center you get.
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

        PL_PausePhysics      = ( 1u << 0 ),
        PL_PauseParticles    = ( 1u << 1 ),
        PL_PauseAnimations   = ( 1u << 2 ),

        PL_PauseAll          = PL_PausePhysics | PL_PauseParticles | PL_PauseAnimations
    };

    /// @brief Used by all World proxies to describe what their derived types are.
    enum ProxyType
    {
        PT_Audio_First                  = EnumBit(1),

        PT_Audio_Listener               = EnumBit(1),
        PT_Audio_SoundProxy             = EnumBit(2),

        PT_Audio_Last                   = EnumBit(2),

        PT_Audio_All_Proxies            = ( PT_Audio_Listener | PT_Audio_SoundProxy ),

        PT_Graphics_First               = EnumBit(3),

        PT_Graphics_BillboardSetProxy   = EnumBit(3),
        PT_Graphics_CameraProxy         = EnumBit(4),
        PT_Graphics_EntityProxy         = EnumBit(5),
        PT_Graphics_LightProxy          = EnumBit(6),
        PT_Graphics_LineGroupProxy      = EnumBit(7),
        PT_Graphics_ParticleSystemProxy = EnumBit(8),

        PT_Graphics_Last                = EnumBit(8),

        PT_Graphics_All_Proxies         = ( PT_Graphics_BillboardSetProxy | PT_Graphics_CameraProxy | PT_Graphics_EntityProxy |
                                            PT_Graphics_LightProxy | PT_Graphics_LineGroupProxy | PT_Graphics_ParticleSystemProxy ),

        PT_Physics_First                = EnumBit(9),

        PT_Physics_GhostProxy           = EnumBit(9),
        PT_Physics_RigidProxy           = EnumBit(10),
        PT_Physics_SoftProxy            = EnumBit(11),

        PT_Physics_Last                 = EnumBit(11),

        PT_Physics_All_Proxies          = ( PT_Physics_GhostProxy | PT_Physics_RigidProxy | PT_Physics_SoftProxy ),

        PT_All_Proxies                  = ( PT_Audio_All_Proxies | PT_Graphics_All_Proxies | PT_Physics_All_Proxies )
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

    /// @brief Used by various classes to help identify what class an object is.
    /// @details This enum can be used to express any object which could be considered "insertable" into the game world.
    enum WorldEntityType
    {
        WO_None                   = 0,          ///< Used to help report error conditions.
        // Terrain Objects
        WO_TerrainFirst           = EnumBit(1),

        WO_MeshTerrain            = EnumBit(1),
        WO_HeightfieldTerrain     = EnumBit(2),
        WO_VectorFieldTerrain     = EnumBit(3),
        WO_VoxelTerrain           = EnumBit(4),
        WO_MarchingCubeTerrain    = EnumBit(5),
        WO_UnknownTerrain         = EnumBit(6),

        WO_TerrainLast            = EnumBit(6),

        WO_AllTerrains            = WO_MeshTerrain | WO_HeightfieldTerrain | WO_VectorFieldTerrain |
                                    WO_VoxelTerrain | WO_MarchingCubeTerrain | WO_UnknownTerrain,

        // Debris Objects
        WO_DebrisFirst            = EnumBit(7),

        WO_RigidDebris            = EnumBit(7),
        WO_SoftDebris             = EnumBit(8),
        WO_UnknownDebris          = EnumBit(9),

        WO_DebrisLast             = EnumBit(9),

        WO_AllDebris              = WO_RigidDebris | WO_SoftDebris | WO_UnknownDebris,

        // AreaEffect Objects
        WO_AreaEffectFirst        = EnumBit(10),

        WO_AreaEffectGravityField = EnumBit(10),
        WO_AreaEffectGravityWell  = EnumBit(11),
        WO_AreaEffectFieldOfForce = EnumBit(12),
        WO_AreaEffectPlaceHolder1 = EnumBit(13),
        WO_AreaEffectPlaceHolder2 = EnumBit(14),
        WO_AreaEffectUnknown      = EnumBit(15),

        WO_AreaEffectLast         = EnumBit(15),

        WO_AllAreaEffects         = WO_AreaEffectGravityField | WO_AreaEffectGravityWell | WO_AreaEffectFieldOfForce |
                                    WO_AreaEffectPlaceHolder1 | WO_AreaEffectPlaceHolder2 | WO_AreaEffectUnknown,

        // Actor Objects
        WO_ActorFirst             = EnumBit(16),

        WO_ActorPlaceHolder1      = EnumBit(16),
        WO_ActorPlaceHolder2      = EnumBit(17),
        WO_ActorPlaceHolder3      = EnumBit(18),

        WO_ActorLast              = EnumBit(18),

        WO_AllActors              = WO_ActorPlaceHolder1 | WO_ActorPlaceHolder2 | WO_ActorPlaceHolder3,

        // Vehicle Objects
        WO_VehicleFirst           = EnumBit(19),

        WO_MultiBodyVehicle       = EnumBit(19),
        WO_RaycastVehicle         = EnumBit(20),
        WO_SimpleVehicle          = EnumBit(21),
        WO_VehiclePlaceHolder1    = EnumBit(22),
        WO_VehiclePlaceHolder2    = EnumBit(23),

        WO_VehicleLast            = EnumBit(23),

        WO_AllVehicles            = WO_MultiBodyVehicle | WO_RaycastVehicle | WO_SimpleVehicle |
                                    WO_VehiclePlaceHolder1 | WO_VehiclePlaceHolder2
    };
}//Mezzanine

#endif
