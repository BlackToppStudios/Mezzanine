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
#ifndef _graphicsparticlesystemproxy_cpp
#define _graphicsparticlesystemproxy_cpp

/// @file
/// @brief This file contains the implementation for the World proxy wrapping particle functionality.

#include "Graphics/particlesystemproxy.h"

#include "Graphics/particleaffector.h"
#include "Graphics/particleemitter.h"

#include <Ogre.h>

namespace Mezzanine
{
    namespace Graphics
    {
        ParticleSystemProxy::ParticleSystemProxy(const String& Name, const String& Template, SceneManager* Creator) :
            RenderableProxy(Creator),
            GraphicsParticleSystem(NULL),
            SpeedFactor(1.0),
            Paused(false)
        {
            // Wrap our emitters
            UInt16 NumEmitters = this->GraphicsParticleSystem->getNumEmitters();
            for( UInt16 X = 0 ; X < NumEmitters ; ++X )
            {
                Graphics::ParticleEmitter* NewEmitter = new Graphics::ParticleEmitter( this->GraphicsParticleSystem->getEmitter(X), this );
                this->Emitters.push_back(NewEmitter);
            }
            // Wrap our affectors
            UInt16 NumAffectors = this->GraphicsParticleSystem->getNumAffectors();
            for( UInt16 X = 0 ; X < NumAffectors ; ++X )
            {
                Graphics::ParticleAffector* NewAffector = new Graphics::ParticleAffector( this->GraphicsParticleSystem->getAffector(X), this );
                this->Affectors.push_back(NewAffector);
            }
        }

        ParticleSystemProxy::~ParticleSystemProxy()
        {
            this->DestroyAllEmitters();
            this->DestroyAllAffectors();
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        Mezzanine::ProxyType ParticleSystemProxy::GetProxyType() const
        {
            return PT_Graphics_ParticleSystemProxy;
        }

        const String& ParticleSystemProxy::GetName() const
        {
            return this->GraphicsParticleSystem->getName();
        }

        const String& ParticleSystemProxy::GetTemplate() const
        {
            return this->Template;
        }

        void ParticleSystemProxy::PauseParticleSystem(bool Pause)
        {
            if( this->Paused != Pause ) {
                this->Paused = Pause;
                this->GraphicsParticleSystem->setSpeedFactor( this->Paused ? 0.0 : this->SpeedFactor );
            }
        }

        Bool ParticleSystemProxy::IsParticleSystemPaused() const
        {
            return this->Paused;
        }

        void ParticleSystemProxy::SetCustomParam(const String& Name, const String& Value)
        {
            this->GraphicsParticleSystem->setParameter(Name,Value);
            this->CustomSystemParameters[Name] = Value;
        }

        String ParticleSystemProxy::GetCustomParam(const String& Name) const
        {
            return this->GraphicsParticleSystem->getParameter(Name);
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Emitters

        ParticleEmitter* ParticleSystemProxy::GetEmitter(const UInt16 Index) const
            { return this->Emitters.at(Index); }

        UInt16 ParticleSystemProxy::GetNumEmitters() const
            { return this->Emitters.size(); }

        void ParticleSystemProxy::DestroyEmitter(const UInt16 Index)
        {
            ParticleEmitter* ToBeDestroyed = this->GetEmitter(Index);
            this->Emitters.erase( this->Emitters.begin() + Index );
            delete ToBeDestroyed;
            this->GraphicsParticleSystem->removeEmitter(Index);
        }

        void ParticleSystemProxy::DestroyAllEmitters()
        {
            for( EmitterIterator EmmitIt = this->Emitters.begin() ; EmmitIt != this->Emitters.end() ; ++EmmitIt )
            {
                delete (*EmmitIt);
            }
            this->Emitters.clear();
            this->GraphicsParticleSystem->removeAllEmitters();
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Affectors

        ParticleAffector* ParticleSystemProxy::GetAffector(const UInt16 Index) const
            { return this->Affectors.at(Index); }

        UInt16 ParticleSystemProxy::GetNumAffectors() const
            { return this->Affectors.size(); }

        void ParticleSystemProxy::DestroyAffector(const UInt16 Index)
        {
            ParticleAffector* ToBeDestroyed = this->GetAffector(Index);
            this->Affectors.erase( this->Affectors.begin() + Index );
            delete ToBeDestroyed;
            this->GraphicsParticleSystem->removeAffector(Index);
        }

        void ParticleSystemProxy::DestroyAllAffectors()
        {
            for( AffectorIterator AffectIt = this->Affectors.begin() ; AffectIt != this->Affectors.end() ; ++AffectIt )
            {
                delete (*AffectIt);
            }
            this->Affectors.clear();
            this->GraphicsParticleSystem->removeAllAffectors();
        }

        ///////////////////////////////////////////////////////////////////////////////
        // ParticleSystem Properties

        void ParticleSystemProxy::SetSpeedFactor(const Real Factor)
        {
            this->SpeedFactor = Factor;
            if( !this->Paused ) {
                this->GraphicsParticleSystem->setSpeedFactor( this->SpeedFactor );
            }
        }

        Real ParticleSystemProxy::GetSpeedFactor()
        {
            return this->SpeedFactor;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Serialization

        void ParticleSystemProxy::ProtoSerialize(XML::Node& ParentNode) const
        {
            XML::Node SelfRoot = ParentNode.AppendChild(this->GetDerivedSerializableName());

            this->ProtoSerializeTemplate(SelfRoot);
            this->ProtoSerializeCustomParameters(SelfRoot);
            this->ProtoSerializeProperties(SelfRoot);
        }

        void ParticleSystemProxy::ProtoSerializeTemplate(XML::Node& SelfRoot) const
        {

        }

        void ParticleSystemProxy::ProtoSerializeCustomParameters(XML::Node& SelfRoot) const
        {

        }

        void ParticleSystemProxy::ProtoSerializeProperties(XML::Node& SelfRoot) const
        {
            this->RenderableProxy::ProtoSerializeProperties(SelfRoot);
        }

        void ParticleSystemProxy::ProtoDeSerialize(const XML::Node& SelfRoot)
        {
            this->ProtoDeSerializeTemplate(SelfRoot);
            this->ProtoDeSerializeCustomParameters(SelfRoot);
            this->ProtoDeSerializeProperties(SelfRoot);
        }

        void ParticleSystemProxy::ProtoDeSerializeTemplate(const XML::Node& SelfRoot)
        {

        }

        void ParticleSystemProxy::ProtoDeSerializeCustomParameters(const XML::Node& SelfRoot)
        {

        }

        void ParticleSystemProxy::ProtoDeSerializeProperties(const XML::Node& SelfRoot)
        {
            this->RenderableProxy::ProtoDeSerializeProperties(SelfRoot);
        }

        String ParticleSystemProxy::GetDerivedSerializableName() const
            { return ParticleSystemProxy::GetSerializableName(); }

        String ParticleSystemProxy::GetSerializableName()
            { return "ParticleSystemProxy"; }

        ///////////////////////////////////////////////////////////////////////////////
        // Internal Methods

        Ogre::ParticleSystem* ParticleSystemProxy::_GetGraphicsObject() const
            { return this->GraphicsParticleSystem; }

        Ogre::MovableObject* ParticleSystemProxy::_GetBaseGraphicsObject() const
            { return this->GraphicsParticleSystem; }
    }//Graphics
}//Mezzanine

#endif
