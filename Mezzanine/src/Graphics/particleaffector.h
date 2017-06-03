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

#ifndef _graphicsparticleaffector_h
#define _graphicsparticleaffector_h

#include "datatypes.h"
#include "XML/xml.h"

namespace Ogre
{
    class ParticleAffector;
}

namespace Mezzanine
{
    namespace Graphics
    {
        class ParticleSystemProxy;
        ///////////////////////////////////////////////////////////////////////////////
        /// @brief This class defines how particles of a given particle effect behave during their lifetime.
        /// @details
        ///////////////////////////////////////
        class MEZZ_LIB ParticleAffector
        {
        protected:
            /// @internal
            /// @brief A cache containing all of the custom altered parameters of this particle affector.
            /// @note Once we have a proper wrapper for particle systems (or our own implementation) this no longer needs to exist.
            NameValuePairMap CustomAffectorParameters;
            /// @internal
            /// @brief The internal affector class this class get's it functionality from.
            Ogre::ParticleAffector* GraphicsAffector;
            /// @internal
            /// @brief A pointer to the system that this affector is affecting.
            ParticleSystemProxy* ParentSystem;
        public:
            /// @brief Internal Class constructor.
            /// @param InternalAffector A pointer to the internal affector providing this class' functionality.
            /// @param Creator A pointer to the particle system that created this affector.
            ParticleAffector(Ogre::ParticleAffector* InternalAffector, ParticleSystemProxy* Creator);
            /// @brief Class destructor.
            virtual ~ParticleAffector();

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            /// @brief Sets a custom parameter of a particle affector.
            /// @remarks The params available to change depends on the internal particle affector used.
            /// You should check your particle script where possible to ensure particles you are
            /// changing are the type you expect.
            /// @param Name The name of the parameter to alter.
            /// @param Value The new value to set for the named parameter.
            void SetCustomParam(const String& Name, const String& Value);
            /// @brief Gets a custom parameter of a particle affector.
            /// @remarks The params available to change depends on the internal particle affector used.
            /// You should check your particle script where possible to ensure the particle has the
            /// param you are looking for.
            /// @param Name The name of the parameter to fetch.
            String GetCustomParam(const String& Name) const;

            ///////////////////////////////////////////////////////////////////////////////
            // Serialization

            /// @copydoc WorldProxy::ProtoSerialize(XML::Node&) const
            virtual void ProtoSerialize(XML::Node& ParentNode) const;
            /*/// @copydoc WorldProxy::ProtoSerializeProperties(XML::Node& SelfRoot) const
            virtual void ProtoSerializeProperties(XML::Node& SelfRoot) const;*/
            /// @brief Convert the custom altered parameters of this class to an XML::Node ready for serialization.
            /// @param SelfRoot The root node containing all the serialized data for this instance.
            virtual void ProtoSerializeCustomParameters(XML::Node& SelfRoot) const;

            /// @copydoc WorldProxy::ProtoDeSerialize(const XML::Node&)
            virtual void ProtoDeSerialize(const XML::Node& SelfRoot);
            /*/// @copydoc WorldProxy::ProtoDeSerializeProperties(const XML::Node& SelfRoot)
            virtual void ProtoDeSerializeProperties(const XML::Node& SelfRoot);*/
            /// @brief Take the data stored in an XML Node and overwrite the custom altered parameters of this object with it.
            /// @param SelfRoot An XML::Node containing the data to populate the new instance with.
            virtual void ProtoDeSerializeCustomParameters(const XML::Node& SelfRoot);

            /// @copydoc WorldProxy::GetDerivedSerializableName() const
            virtual String GetDerivedSerializableName() const;
            /// @copydoc WorldProxy::GetSerializableName()
            static String GetSerializableName();

            ///////////////////////////////////////////////////////////////////////////////
            // Internal Methods

            /// @internal
            /// @brief Accessor for the internal particle affector.
            /// @return Returns a pointer to the internal particle affector this proxy is based on.
            virtual Ogre::ParticleAffector* _GetGraphicsAffector() const;
        };//ParticleAffector
    }//Graphics
}//Mezzanine

#endif
