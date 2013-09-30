// © Copyright 2010 - 2013 BlackTopp Studios Inc.
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
#ifndef _graphicsparticlesystemproxy_h
#define _graphicsparticlesystemproxy_h

/// @file
/// @brief This file contains the declaration for the World proxy wrapping particle functionality.

#include "Graphics/renderableproxy.h"

namespace Ogre
{
    class ParticleSystem;
}

namespace Mezzanine
{
    namespace Graphics
    {
        class ParticleEmitter;
        class ParticleAffector;
        ///////////////////////////////////////////////////////////////////////////////
        /// @brief This is the proxy class for placing and manipulating particles in the scene.
        /// @details
        ///////////////////////////////////////
        class MEZZ_LIB ParticleSystemProxy : public RenderableProxy
        {
        public:
            /// @brief Basic container type for ParticleEmitter storage by this class.
            typedef std::vector<ParticleEmitter*>               EmitterContainer;
            /// @brief Iterator type for ParticleEmitter instances stored by this class.
            typedef EmitterContainer::iterator                  EmitterIterator;
            /// @brief Const Iterator type for ParticleEmitter instances stored by this class.
            typedef EmitterContainer::const_iterator            ConstEmitterIterator;
            /// @brief Basic container type for ParticleAffector storage by this class.
            typedef std::vector<ParticleAffector*>              AffectorContainer;
            /// @brief Iterator type for ParticleAffector instances stored by this class.
            typedef AffectorContainer::iterator                 AffectorIterator;
            /// @brief Const Iterator type for ParticleAffector instances stored by this class.
            typedef AffectorContainer::const_iterator           ConstAffectorIterator;
        protected:
            /// @internal
            /// @brief A cache containing all of the custom altered parameters of this particle system.
            /// @note Once we have a proper wrapper for particle systems (or our own implementation) this no longer needs to exist.
            NameValuePairMap CustomSystemParameters;
            /// @internal
            /// @brief Vector of emitters in use by this particle effect.
            EmitterContainer Emitters;
            /// @internal
            /// @brief Vector of affectors in use by this particle effect.
            AffectorContainer Affectors;
            /// @internal
            /// @brief Stores the template, primarily for serialization.
            String Template;
            /// @internal
            /// @brief Pointer to the ogre ParticleSystem from which this proxy gets it's functionality.
            Ogre::ParticleSystem* GraphicsParticleSystem;
            /// @internal
            /// @brief Stores the current speed factor of this particle system for when it gets paused.
            Real SpeedFactor;
            /// @internal
            /// @brief Stores whether or not updates to this particle system are paused.
            Bool Paused;
        public:
            /// @todo Create a non-template constructor so people can make particle effects entirely from c++.

            /// @brief Template constructor.
            /// @param Name The name of this particle effect.
            /// @param Template Name of the particle script to be used in creating this particle effect.
            /// @param Creator A pointer to the manager that created this proxy.
            ParticleSystemProxy(const String& Name, const String& Template, SceneManager* Creator);
            /// @brief Class destructor.
            virtual ~ParticleSystemProxy();

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            /// @copydoc WorldProxy::GetProxyType() const
            virtual Mezzanine::ProxyType GetProxyType() const;

            /// @brief Gets the name of this particle effect.
            /// @note This method will be removed in the Ogre 2.0 switch.
            /// @return Returns a const reference to a string containing the name of this particle system proxy.
            const String& GetName() const;
            /// @brief Gets the name of the template this particle effect is based on.
            /// @return Returns a const reference to a string containing the name of the template this particle system proxy is based on.
            const String& GetTemplate() const;

            /// @brief Pauses this proxy, preventing it from emitting, moving, or removing any particles.
            /// @param Pause Will pauae the proxy if running and Pause is true, otherwise it will resume the proxy if paused and Pause is false.
            void PauseParticleSystem(bool Pause);
            /// @brief Gets whether or not this particle system is currently paused.
            /// @return Returns true if this particle system is paused, false otherwise.
            Bool IsParticleSystemPaused() const;

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

            ///////////////////////////////////////////////////////////////////////////////
            // Emitters

            /// @brief Gets the Emitter at the specified index.
            /// @param Index The index of the Emitter to get.
            /// @return Returns a pointer to the Emitter at the specified index.
            ParticleEmitter* GetEmitter(const UInt16 Index) const;
            /// @brief Gets the number of Emitters in use by this particle effect.
            /// @return Returns a UInt16 representing the number of Emitters in this particle effect.
            UInt16 GetNumEmitters() const;
            /// @brief Destroy's an Emitter in use by this particle effect.
            /// @param Index The index to destroy.
            void DestroyEmitter(const UInt16 Index);
            /// @brief Destroy's all Emitters in use by this particle effect.
            void DestroyAllEmitters();

            ///////////////////////////////////////////////////////////////////////////////
            // Affectors

            /// @brief Gets the Affector at the specified index.
            /// @param Index The index of the Affector to get.
            /// @return Returns a pointer to the Affector at the specified index.
            ParticleAffector* GetAffector(const UInt16 Index) const;
            /// @brief Gets the number of Affectors in use by this particle effect.
            /// @return Returns a UInt16 representing the number of Affectors in this particle effect.
            UInt16 GetNumAffectors() const;
            /// @brief Destroy's an Affector in use by this particle effect.
            /// @param Index The index to destroy.
            void DestroyAffector(const UInt16 Index);
            /// @brief Destroy's all Affectors in use by this particle effect.
            void DestroyAllAffectors();

            ///////////////////////////////////////////////////////////////////////////////
            // ParticleSystem Properties

            /// @brief Sets the factor at which time progresses for this ParticleEffect.
            /// @param Factor The factor at which time progresses.  Valid values range from 0.0 to 1.0.
            /// @note If the ParticleEffect is paused, this will set the value to resume with when the ParticleEffect is unpaused.
            void SetSpeedFactor(const Real Factor);
            /// @brief Gets the Factor at which this ParticleEffect is currently progressing.
            /// @return Returns a Real representing the factor at which the ParticleEffect is currently progressing.
            /// @note If the ParticleEffect is paused, this will return what the speed would be if it were unpaused.
            Real GetSpeedFactor();

            ///////////////////////////////////////////////////////////////////////////////
            // Serialization

            /// @copydoc WorldProxy::ProtoSerialize(XML::Node&) const
            virtual void ProtoSerialize(XML::Node& ParentNode) const;
            /// @brief Convert the template name of this class to an XML::Node ready for serialization.
            /// @param SelfRoot The root node containing all the serialized data for this instance.
            virtual void ProtoSerializeTemplate(XML::Node& SelfRoot) const;
            /// @brief Convert the custom altered parameters of this class to an XML::Node ready for serialization.
            /// @param SelfRoot The root node containing all the serialized data for this instance.
            virtual void ProtoSerializeCustomParameters(XML::Node& SelfRoot) const;
            /// @copydoc WorldProxy::ProtoSerializeProperties(XML::Node& SelfRoot) const
            virtual void ProtoSerializeProperties(XML::Node& SelfRoot) const;

            /// @copydoc WorldProxy::ProtoDeSerialize(const XML::Node&)
            virtual void ProtoDeSerialize(const XML::Node& SelfRoot);
            /// @brief Take the data stored in an XML Node and overwrite the template name of this object with it.
            /// @param SelfRoot An XML::Node containing the data to populate this class with.
            virtual void ProtoDeSerializeTemplate(const XML::Node& SelfRoot);
            /// @brief Take the data stored in an XML Node and overwrite the custom altered parameters of this object with it.
            /// @param SelfRoot An XML::Node containing the data to populate this class with.
            virtual void ProtoDeSerializeCustomParameters(const XML::Node& SelfRoot);
            /// @copydoc WorldProxy::ProtoDeSerializeProperties(const XML::Node& SelfRoot)
            virtual void ProtoDeSerializeProperties(const XML::Node& SelfRoot);

            /// @copydoc WorldProxy::GetDerivedSerializableName() const
            virtual String GetDerivedSerializableName() const;
            /// @copydoc WorldProxy::GetSerializableName()
            static String GetSerializableName();

            ///////////////////////////////////////////////////////////////////////////////
            // Internal Methods

            /// @internal
            /// @brief Accessor for the internal particle system.
            /// @return Returns a pointer to the internal particle system this proxy is based on.
            virtual Ogre::ParticleSystem* _GetGraphicsObject() const;
            /// @copydoc RenderableProxy::_GetBaseGraphicsObject() const
            virtual Ogre::MovableObject* _GetBaseGraphicsObject() const;
        };//ParticleSystemProxy
    }//Graphics
}//Mezzanine

#endif
