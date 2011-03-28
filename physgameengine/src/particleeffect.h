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
#ifndef _particleeffect_h
#define _particleeffect_h

#include "attachable.h"
#include "crossplatformexport.h"
#include "datatypes.h"

/// @file particleeffect.cpp
/// @brief Contains the declaration of the ParticleEffect class

namespace Ogre
{
    class ParticleSystem;
    class SceneNode;
}

namespace phys
{
    namespace internal
    {
        struct ParticleEffectInternalData;
    }

    class SceneManager;
    ///////////////////////////////////////////////////////////////////////////////
    /// @class ParticleEffect
    /// @headerfile particleeffect.h
    /// @brief This class is responsible for creating visual particle effects, such as rain, smoke, sparks, and explosions.
    /// @details Particle effects are loaded from particle scripts which are contained in particle files(*.particle). @n
    /// All particle effects are created attached to a world node. The world Node provides the Nagivation functionality of for this
    ///////////////////////////////////////
    class PHYS_LIB ParticleEffect : public Attachable
    {
        private:
            friend class WorldNode;

            /// @internal
            /// The internal data for the particle effect
            internal::ParticleEffectInternalData *Pie;
        public:

            ///////////////////////////////////////////////////////////////////////////////
            /// Construction

            /// @brief Standard initialization constructor.
            /// @param Name The name of this particle effect.
            /// @param Template Name of the particle script to be used in creating this particle effect.
            /// @param manager Pointer to the manager that this particle effect is to be used in.
            ParticleEffect(const String& Name, const String& Template, SceneManager* manager);

            /// @brief Internal constructor.
            /// @details This constructor should not be called on manually.
            /// @param System Pointer to the Ogre ParticleSystem this class is based on.
            /// @param manager Pointer to the manager that this particle effect is to be used in.
            ParticleEffect(Ogre::ParticleSystem* System, SceneManager* manager);

            /// @brief Class destructor.
            ~ParticleEffect();

            ///////////////////////////////////////////////////////////////////////////////
            /// Inherited From Attachable

            /// @brief Gets the name of this particle effect.
            /// @return Returns a string containing the name given to this particle effect.
            ConstString& GetName() const;

            /// @brief What kind of Attachable is this.
            /// @return An Attachable::GetAttachableType containing Attachable::Attachable.
            virtual Attachable::AttachableElement GetAttachableType() const;

            virtual void AttachToFinal(Ogre::SceneNode* RawTarget, phys::WorldNode* Target);
            virtual void DetachFromFinal(Ogre::SceneNode* RawTarget);

            ///////////////////////////////////////////////////////////////////////////////
            /// Particle Functionality

            /// @brief Enables the particle effect, allowing it to render.
            void EnableParticleEffect();

            /// @brief Disables the particle effect, preventing it from rendering.
            void DisableParticleEffect();

            /// @brief Set the location of this Particle effect relative to the object it is attached to.
            /// @param vec The location.
            virtual void SetLocation(const Vector3& Vec);

            /// @brief Get the location of the Particle Effect, relative to the Object is is attached to.
            /// @return A Vector3 with the location.
            virtual Vector3 GetLocation() const;
    };
}

#endif
