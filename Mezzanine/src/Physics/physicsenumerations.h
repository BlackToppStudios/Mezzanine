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
#ifndef _physicsenumerations_h
#define _physicsenumerations_h
///////////////////////////////////////////////////////////////////////////////
//Any global enumerations shared between multiple classes in the Physics namespace is to be declared here.
///////////////////////////////////////

namespace Mezzanine
{
    namespace Physics
    {
        /// @brief This is used by physics proxies to monitor and set their physics activation.
        /// @details The physics subsystem can, and will, change the activation state of the objects depending on the state of the simulation.
        /// This is unless either "AS_Disable*" states are set, which will prevent any changes to the activation state from the subsystem.  Any
        /// change from either "AS_Disable*" states must be manual.
        enum ActivationState
        {
            AS_Active               = 1, ///< Object is processed by the simulation each frame.
            AS_Island_Sleeping      = 2, ///< Objects' simulation island is dormant, so the object isn't processed.
            AS_Wants_Deactivation   = 3, ///< Object thinks it can be deactivated but the system hasn't yet deactivated it.
            AS_DisableDeactivation  = 4, ///< Prevents the object from disabling, causing it to be active until this state is unset.
            AS_DisableSimulation    = 5  ///< Prevents the object from activating, causing it to be disabled until this state is unset.
        };

        /// @brief This is used by physics proxies to help determine the behavior of it's anistropic friction behavior.
        /// @details See the documentation on the PhysicsProxy class for more details.
        enum AnisotropicFrictionFlags
        {
            AFF_AnisotropicFrictionDisabled  = 0,
            AFF_AnisotropicFriction          = 1,
            AFF_AnisotropicRollingFriction   = 2
        };

        /// @brief These are the various filters that can be applied to all physics accelerated world objects to limit what they collide with.
        /// @details You can create your own groups on top of the ones that exist here, but they have to be powers of 2, and not overlap with
        /// any of the existing groups defined here.  Simply pass in the Whole where appropriate.
        enum CollisionFilter
        {
            // Standard base filters
            CF_GenericFilter    = 1,  ///< A catch-all filter used when other filters may not be appropriate.
            CF_StaticFilter     = 2,  ///< Filter used by static objects, most commonly terrain.
            CF_KinematicFilter  = 4,  ///< Filter used by objects with movement that is explicitly controlled by non-physics logic.
            CF_DebrisFilter     = 8,  ///< Filter used by dynamic objects with no special movement behavior, simply have collision response.
            CF_SensorFilter     = 16, ///< Filter used by non-solid objects.
            CF_CharacterFilter  = 32, ///< Filter used to express objects explicitly controlled by a player.
            CF_AllFilter        = -1, ///< All the filters!

            // Non-Standard Filters
            CF_UserFilter1      = 64, ///< Custom User Filter.
            CF_UserFilter2      = 128,///< Custom User Filter.
            CF_UserFilter3      = 256,///< Custom User Filter.
            CF_UserFilter4      = 512 ///< Custom User Filter.
        };

        /// @brief This is used by physics proxies to help determine collision response behavior of a proxy.
        enum CollisionFlags
        {
            CF_StaticObject                   = 1,
            CF_KinematicObject                = 2,
            CF_NoContactResponse              = 4,
            CF_CustomMaterialCallback         = 8,
            CF_CharacterObject                = 16,
            CF_DisableVisualizeObject         = 32,
            CF_DisableSPUCollisionProcessing  = 64
        };

        /// @brief Enum specifying the state change occuring in the collision.
        enum CollisionState
        {
            Col_Begin,
            Col_Contacts_Updated,
            Col_End
        };

        /// @brief Enum specifying what kind of collision this class is storing.
        enum CollisionType
        {
            Col_Actor_Actor,            ///< Specifies a collision between two Actors.
            Col_Actor_AreaEffect,       ///< Specifies a collision between an Actor and an AreaRffect.
            Col_Actor_Debris,           ///< Specifies a collision between an Actor and a Debris.
            Col_Actor_Terrain,          ///< Specifies a collision between an Actor and some Terrain.
            Col_AreaEffect_AreaEffect,  ///< Specifies a collision between two AreaEffects.
            Col_AreaEffect_Debris,      ///< Specifies a collision between an AreaEffect and a Debris.
            Col_AreaEffect_Terrain,     ///< Specifies a collision between an AreaEffect and some Terrain.
            Col_Debris_Debris,          ///< Specifies a collision between two Debris.
            Col_Debris_Terrain          ///< Specifies a collision between a Debris and some Terrain.
        };

        /// @brief This is a collection of flags designed to describe what to draw when the Debug Drawer is enabled.
        /// @details Most of these options work, with the exception of text-based modes and contact point modes.
        enum DebugDrawMode
        {
            DDM_NoDebug               = 0,
            DDM_DrawWireframe         = 1,
            DDM_DrawAABB              = 2,
            DDM_DrawFeaturesText      = 4,
            DDM_DrawContactPoints     = 8,
            DDM_NoDeactivation        = 16,
            DDM_NoHelpText            = 32,
            DDM_DrawText              = 64,
            DDM_ProfileTimings        = 128,
            DDM_EnableSatComparison   = 256,
            DDM_DisableBulletLCP      = 512,
            DDM_EnableCCD             = 1024,
            DDM_DrawConstraints       = 2048,
            DDM_DrawConstraintLimits  = 4096,
            DDM_FastWireframe         = 8192,
            DDM_DrawNormals           = 16384,

            DDM_All                   = -1
        };

        /// @brief This is used by the PhysicsProxy child classes to describe which proxy type they are.
        enum ProxyType
        {
            PT_Rigid  = 1,
            PT_Soft   = 2,
            PT_Ghost  = 3
        };
    }//physics
}//Mezzanine

#endif
