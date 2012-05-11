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
#ifndef _particleeffect_h
#define _particleeffect_h

#include "attachable.h"
#include "datatypes.h"
#include "quaternion.h"
#include "xml.h"

/// @file particleeffect.cpp
/// @brief Contains the declaration of the ParticleEffect class

namespace Ogre
{
    class ParticleSystem;
    class SceneNode;
}

namespace Mezzanine
{
    namespace Internal
    {
        struct ParticleEffectInternalData;
    }

    class SceneManager;
    class ParticleAffector;
    class ParticleEmitter;
    ///////////////////////////////////////////////////////////////////////////////
    /// @class ParticleEffect
    /// @headerfile particleeffect.h
    /// @brief This class is responsible for creating visual particle effects, such as rain, smoke, sparks, and explosions.
    /// @details Particle effects are loaded from particle scripts which are contained in particle files(*.particle). @n
    /// All particle effects are created attached to a world node. The world Node provides the Nagivation functionality of for this
    ///////////////////////////////////////
    class MEZZ_LIB ParticleEffect : public AttachableChild
    {
        private:
            /// @internal
            /// @brief The internal data for the particle effect
            Internal::ParticleEffectInternalData* Pie;
            /// @internal
            /// @brief Vector of emitters in use by this particle effect.
            std::vector<ParticleEmitter*> Emitters;
            /// @internal
            /// @brief Vector of affectors in use by this particle effect.
            std::vector<ParticleAffector*> Affectors;
        public:
            ///////////////////////////////////////////////////////////////////////////////
            // Construction

            /// @brief Standard initialization constructor.
            /// @param Name The name of this particle effect.
            /// @param Template Name of the particle script to be used in creating this particle effect.
            /// @param manager Pointer to the manager that this particle effect is to be used in.
            ParticleEffect(const String& Name, const String& Template, SceneManager* manager);
            /// @internal
            /// @brief Internal constructor.
            /// @details This constructor should not be called on manually.
            /// @param System Pointer to the Ogre ParticleSystem this class is based on.
            /// @param Template Name of the particle script to be used in creating this particle effect.
            /// @param manager Pointer to the manager that this particle effect is to be used in.
            ParticleEffect(Ogre::ParticleSystem* System, const String& Template, SceneManager* manager);
            /// @brief Class destructor.
            virtual ~ParticleEffect();

            ///////////////////////////////////////////////////////////////////////////////
            // Inherited From AttachableChild

            /// @copydoc Mezzanine::AttachableBase::GetName()
            ConstString& GetName() const;
            /// @copydoc Mezzanine::AttachableBase::GetType()
            WorldAndSceneObjectType GetType() const;
            /// @copydoc Mezzanine::AttachableBase::SetLocation(const Vector3& Location)
            void SetLocation(const Vector3& Location);
            /// @copydoc Mezzanine::AttachableBase::GetLocation()
            Vector3 GetLocation() const;
            /// @copydoc Mezzanine::AttachableBase::SetOrientation(const Quaternion& Orientation)
            void SetOrientation(const Quaternion& Orientation);
            /// @copydoc Mezzanine::AttachableBase::GetOrientation()
            Quaternion GetOrientation() const;
            /// @copydoc Mezzanine::AttachableBase::SetScaling(Vector3&)
            void SetScaling(const Vector3& Scale);
            /// @copydoc Mezzanine::AttachableBase::GetScaling()
            Vector3 GetScaling() const;
            /// @copydoc Mezzanine::AttachableChild::SetLocalLocation(Vector3&)
            void SetLocalLocation(const Vector3& Location);
            /// @copydoc Mezzanine::AttachableChild::SetLocalOrientation(Quaternion&)
            void SetLocalOrientation(const Quaternion& Orientation);

            ///////////////////////////////////////////////////////////////////////////////
            // Emitters

            /// @brief Gets the Emitter at the specified index.
            /// @param Index The index of the Emitter to get.
            /// @return Returns a pointer to the Emitter at the specified index.
            ParticleEmitter* GetEmitter(const UInt16& Index) const;
            /// @brief Gets the number of Emitters in use by this particle effect.
            /// @return Returns a UInt16 representing the number of Emitters in this particle effect.
            UInt16 GetNumEmitters() const;
            /// @brief Destroy's an Emitter in use by this particle effect.
            /// @param Index The index to destroy.
            void DestroyEmitter(const UInt16& Index);
            /// @brief Destroy's all Emitters in use by this particle effect.
            void DestroyAllEmitters();

            ///////////////////////////////////////////////////////////////////////////////
            // Affectors

            /// @brief Gets the Affector at the specified index.
            /// @param Index The index of the Affector to get.
            /// @return Returns a pointer to the Affector at the specified index.
            ParticleAffector* GetAffector(const UInt16& Index) const;
            /// @brief Gets the number of Affectors in use by this particle effect.
            /// @return Returns a UInt16 representing the number of Affectors in this particle effect.
            UInt16 GetNumAffectors() const;
            /// @brief Destroy's an Affector in use by this particle effect.
            /// @param Index The index to destroy.
            void DestroyAffector(const UInt16& Index);
            /// @brief Destroy's all Affectors in use by this particle effect.
            void DestroyAllAffectors();

            ///////////////////////////////////////////////////////////////////////////////
            // Other Particle Functionality

            /// @brief Adds this particle effect to the world, allowing it to render.
            /// @details If the Effect is already in the world, this does nothing.
            void AddToWorld();

            /// @brief Removes this particle effect from the world, preventing it from rendering.
            /// @details If the Effect is not in the world, this does nothing
            void RemoveFromWorld();

            /// @brief Gets whether or not this effect is in the world.
            /// @return True if in the world, false otherwise.
            bool IsInWorld() const;

            /// @brief Sets the factor at which time progresses for this ParticleEffect.
            /// @param Factor The factor at which time progresses.  Valid values range from 0.0 to 1.0.
            /// @note If the ParticleEffect is paused, this will set the value to resume with when the ParticleEffect is unpaused.
            void SetSpeedFactor(const Real& Factor);

            /// @brief Gets the Factor at which this ParticleEffect is currently progressing.
            /// @return Returns a Real representing the factor at which the ParticleEffect is currently progressing.
            /// @note If the ParticleEffect is paused, this will return what the speed would be if it were unpaused.
            Real GetSpeedFactor();

            /// @brief Pauses this ParticleEffect, preventing it from emitting, moving, or removing any particles.
            /// @param Pause Will pauae the ParticleEffect if running and Pause is true, otherwise will resume particle effect if paused and Pause is false.
            void PauseParticleEffect(bool Pause);

            /// @brief The name of the template used to create this.
            /// @return A reaference to a ConstString that has the name.
            ConstString& GetTemplate() const;

            /// @brief Sets a custom parameter of a particle effect.
            /// @remarks The params available to change depends on the internal particle system used.
            /// You should check your particle script where possible to ensure particles you are
            /// changing are the type you expect.
            /// @param Name The name of the parameter to alter.
            /// @param Value The new value to set for the named parameter.
            void SetCustomParam(const String& Name, const String& Value);

            /// @brief Gets a custom parameter of a particle effect.
            /// @remarks The params available to change depends on the internal particle system used.
            /// You should check your particle script where possible to ensure the particle has the
            /// param you are looking for.
            /// @param Name The name of the parameter to fetch.
            String GetCustomParam(const String& Name) const;
    };//ParticleEffect
}//Mezzanine

///////////////////////////////////////////////////////////////////////////////
// Class External << Operators for streaming or assignment
#ifdef MEZZXML

/// @brief Serializes the passed Mezzanine::ParticleEffect to XML
/// @param stream The ostream to send the xml to.
/// @param Ev the Mezzanine::ParticleEffect to be serialized
/// @return this returns the ostream, now with the serialized data
std::ostream& MEZZ_LIB operator << (std::ostream& stream, const Mezzanine::ParticleEffect& Ev);

/// @brief Deserialize a Mezzanine::ParticleEffect
/// @param stream The istream to get the xml from to (re)make the Mezzanine::ParticleEffect.
/// @param Ev the Mezzanine::ParticleEffect to be deserialized.
/// @return this returns the ostream, advanced past the Mezzanine::ParticleEffect that was recreated onto Ev.
/// @warning This does not the pointer to the scene manager. When a scene manager is serialized, this data is implicitly stored by the ParticleEffects location in the xml hierarchy, this is used instead. The Name of the manager is stored for possible future use.
std::istream& MEZZ_LIB operator >> (std::istream& stream, Mezzanine::ParticleEffect& Ev);

/// @brief Set all values of a Mezzanine::ParticleEffect from parsed xml.
/// @param OneNode The istream to get the xml from to (re)make the Mezzanine::ParticleEffect.
/// @param Ev the Mezzanine::ParticleEffect to be reset.
/// @return This returns thexml::Node that was passed in.
/// @warning This does not attempt to de-serialize the name or template of the ParticleEffect. These is not currently changeable after the creation of a ParticleEffect. However, the ParticleEffectmanager will correctly create name ParticleEffect upon creation then deserialize the rest of the ParticleEffect.
/// @warning This does not throw an exception if the ParticleEffect could not be attached to the appropriate worldnode. It is assumed that the worldnode will be able to adjust the pointer on this if it is deserialized second.
Mezzanine::xml::Node& MEZZ_LIB operator >> (const Mezzanine::xml::Node& OneNode, Mezzanine::ParticleEffect& Ev);

#endif


#endif
