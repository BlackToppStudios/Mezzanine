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
#ifndef _enumerations_h
#define _enumerations_h

#include "macros.h"

///////////////////////////////////////////////////////////////////////////////
/// @file
/// @brief Any global enumerations shared between multiple classes is to be declared here.
///////////////////////////////////////

namespace Mezzanine
{
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
        PT_Audio_First                  = 1,//1
        PT_Audio_Listener               = 1,//1
        PT_Audio_SoundProxy             = 2,//2
        PT_Audio_Last                   = 2,//2

        PT_Graphics_First               = 4,//3
        PT_Graphics_BillboardSetProxy   = 4,//3
        PT_Graphics_CameraProxy         = 8,//4
        PT_Graphics_EntityProxy         = 16,//5
        PT_Graphics_LightProxy          = 32,//6
        PT_Graphics_ParticleSystemProxy = 64,//7
        PT_Graphics_Last                = 64,//7

        PT_Physics_First                = 128,//8
        PT_Physics_GhostProxy           = 128,//8
        PT_Physics_RigidProxy           = 256,//9
        PT_Physics_SoftProxy            = 512,//10
        PT_Physics_Last                 = 512,//10

        PT_Audio_All_Proxies            = ( PT_Audio_Listener | PT_Audio_SoundProxy ),
        PT_Graphics_All_Proxies         = ( PT_Graphics_BillboardSetProxy | PT_Graphics_CameraProxy |PT_Graphics_EntityProxy |PT_Graphics_LightProxy |PT_Graphics_ParticleSystemProxy ),
        PT_Physics_All_Proxies          = ( PT_Physics_GhostProxy | PT_Physics_RigidProxy | PT_Physics_SoftProxy )
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
    enum WorldObjectType
    {
        // Terrain Objects
        WO_TerrainFirst           = ( 1u << 0 ),//1,

        WO_MeshTerrain            = ( 1u << 0 ),//1,
        WO_HeightfieldTerrain     = ( 1u << 1 ),//2,
        WO_VectorFieldTerrain     = ( 1u << 2 ),//4,
        WO_VoxelTerrain           = ( 1u << 3 ),//8,
        WO_MarchingCubeTerrain    = ( 1u << 4 ),//16,
        WO_UnknownTerrain         = ( 1u << 5 ),//32,

        WO_TerrainLast            = ( 1u << 5 ),//32,
        // Debris Objects
        WO_DebrisFirst            = ( 1u << 6 ),//64,

        WO_RigidDebris            = ( 1u << 6 ),//64,
        WO_SoftDebris             = ( 1u << 7 ),//128,
        WO_UnknownDebris          = ( 1u << 8 ),//256,

        WO_DebrisLast             = ( 1u << 8 ),//256,
        // AreaEffect Objects
        WO_AreaEffectFirst        = ( 1u << 9 ),//512,

        WO_AreaEffectGravityField = ( 1u << 9 ),//512,
        WO_AreaEffectGravityWell  = ( 1u << 10 ),//1024,
        WO_AreaEffectFieldOfForce = ( 1u << 11 ),//2048,
        WO_AreaEffectPlaceHolder1 = ( 1u << 12 ),//4096,
        WO_AreaEffectPlaceHolder2 = ( 1u << 13 ),//8192,
        WO_AreaEffectUnknown      = ( 1u << 14 ),//16384,

        WO_AreaEffectLast         = ( 1u << 14 ),//16384,
        // Actor Objects
        WO_ActorFirst             = ( 1u << 15 ),//32768,

        WO_ActorPlaceHolder1      = ( 1u << 15 ),//32768,
        WO_ActorPlaceHolder2      = ( 1u << 16 ),//65536,
        WO_ActorPlaceHolder3      = ( 1u << 17 ),//131072,

        WO_ActorLast              = ( 1u << 17 ),//131072,
        // Vehicle Objects
        WO_VehicleFirst           = ( 1u << 18 ),//262144,

        WO_VehiclePlaceHolder1    = ( 1u << 18 ),//262144,
        WO_VehiclePlaceHolder2    = ( 1u << 19 ),//524288,
        WO_VehiclePlaceHolder3    = ( 1u << 20 ),//1048576,
        WO_VehiclePlaceHolder4    = ( 1u << 21 ),//2097152,
        WO_VehiclePlaceHolder5    = ( 1u << 22 ),//4194304,

        WO_VehicleLast            = ( 1u << 22 ) //4194304
    };
}//Mezzanine

#endif
