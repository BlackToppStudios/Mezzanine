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
#include "Graphics/scenemanager.h"

#include "exception.h"
#include "serialization.h"
#include "stringtool.h"

#include <Ogre.h>

namespace Mezzanine
{
    namespace Graphics
    {
        ParticleSystemProxy::ParticleSystemProxy(const String& Template, SceneManager* Creator) :
            RenderableProxy(Creator),
            GraphicsParticleSystem(NULL),
            SpeedFactor(1.0),
            Paused(false)
            { this->CreateParticleSystem(Template); }

        ParticleSystemProxy::~ParticleSystemProxy()
            { this->DestroyParticleSystem(); }

        void ParticleSystemProxy::CreateParticleSystem(const String& Template)
        {
            this->GraphicsParticleSystem = this->Manager->_GetGraphicsWorldPointer()->createParticleSystem(ParticleSystemProxy::GenerateName(),Template);
            this->GraphicsNode->attachObject( this->GraphicsParticleSystem );
            this->GraphicsParticleSystem->setUserAny( Ogre::Any( static_cast<RenderableProxy*>( this ) ) );
            this->GraphicsParticleSystem->setVisibilityFlags(0);
            this->GraphicsParticleSystem->setQueryFlags(0);

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

        void ParticleSystemProxy::DestroyParticleSystem()
        {
            this->DestroyAllEmitters();
            this->DestroyAllAffectors();

            if( this->GraphicsParticleSystem ) {
                this->GraphicsNode->detachObject( this->GraphicsParticleSystem );
                this->Manager->_GetGraphicsWorldPointer()->destroyParticleSystem( this->GraphicsParticleSystem );
            }
            this->CustomSystemParameters.clear();
        }

        String ParticleSystemProxy::GenerateName()
        {
            static UInt32 NameCounter = 0;
            StringStream NameStream;
            NameStream << "ParticleSystem" << ++NameCounter;
            return NameStream.str();
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
            for( EmitterIterator EmitIt = this->Emitters.begin() ; EmitIt != this->Emitters.end() ; ++EmitIt )
            {
                delete (*EmitIt);
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

        Real ParticleSystemProxy::GetSpeedFactor() const
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
            this->ProtoSerializeEmitters(SelfRoot);
            this->ProtoSerializeAffectors(SelfRoot);
        }

        void ParticleSystemProxy::ProtoSerializeTemplate(XML::Node& SelfRoot) const
        {
            XML::Node ConstructionParamNode = SelfRoot.AppendChild( ParticleSystemProxy::GetSerializableName() + "Template" );

            if( ConstructionParamNode.AppendAttribute("Version").SetValue("1") &&
                ConstructionParamNode.AppendAttribute("TemplateName").SetValue( this->GetTemplate() ) )
            {
                return;
            }else{
                SerializeError("Create XML Attribute Values",ParticleSystemProxy::GetSerializableName() + "TemplateAndName",true);
            }
        }

        void ParticleSystemProxy::ProtoSerializeCustomParameters(XML::Node& SelfRoot) const
        {
            XML::Node CustomParametersNode = SelfRoot.AppendChild( ParticleSystemProxy::GetSerializableName() + "CustomParameters" );

            if( CustomParametersNode.AppendAttribute("Version").SetValue("1") )
            {
                for( NameValuePairMap::const_iterator ParamIt = this->CustomSystemParameters.begin() ; ParamIt != this->CustomSystemParameters.end() ; ++ParamIt )
                {
                    XML::Node CustomParamNode = CustomParametersNode.AppendChild( "CustomParam" );
                    if( CustomParamNode.AppendAttribute("Version").SetValue("1") &&
                        CustomParamNode.AppendAttribute("ParamName").SetValue( (*ParamIt).first ) &&
                        CustomParamNode.AppendAttribute("ParamValue").SetValue( (*ParamIt).second ) )
                    {
                        return;
                    }else{
                        SerializeError("Create XML Attribute Values",ParticleSystemProxy::GetSerializableName() + "CustomParameters",true);
                    }
                }
            }else{
                SerializeError("Create XML Attribute Values",ParticleSystemProxy::GetSerializableName() + "CustomParameters",true);
            }
        }

        void ParticleSystemProxy::ProtoSerializeEmitters(XML::Node& SelfRoot) const
        {
            XML::Node EmittersNode = SelfRoot.AppendChild( ParticleSystemProxy::GetSerializableName() + "Emitters" );

            if( EmittersNode.AppendAttribute("Version").SetValue("1") )
            {
                const UInt16 NumEmitters = this->GetNumEmitters();
                for( UInt16 Index = 0 ; Index < NumEmitters ; ++Index )
                {
                    XML::Node EmitterNode = EmittersNode.AppendChild( "Emitter" );
                    if( EmitterNode.AppendAttribute("Version").SetValue("1") &&
                        EmitterNode.AppendAttribute("Index").SetValue( Index ) )
                    {
                        this->GetEmitter(Index)->ProtoSerialize(EmitterNode);
                    }else{
                        SerializeError("Create XML Attribute Values","Emitter",true);
                    }
                }
            }else{
                SerializeError("Create XML Attribute Values",ParticleSystemProxy::GetSerializableName() + "Emitters",true);
            }
        }

        void ParticleSystemProxy::ProtoSerializeAffectors(XML::Node& SelfRoot) const
        {
            XML::Node AffectorsNode = SelfRoot.AppendChild( ParticleSystemProxy::GetSerializableName() + "Affectors" );

            if( AffectorsNode.AppendAttribute("Version").SetValue("1") )
            {
                const UInt16 NumAffectors = this->GetNumAffectors();
                for( UInt16 Index = 0 ; Index < NumAffectors ; ++Index )
                {
                    XML::Node AffectorNode = AffectorsNode.AppendChild( "Affector" );
                    if( AffectorNode.AppendAttribute("Version").SetValue("1") &&
                        AffectorNode.AppendAttribute("Index").SetValue( Index ) )
                    {
                        this->GetAffector(Index)->ProtoSerialize(AffectorNode);
                    }else{
                        SerializeError("Create XML Attribute Values","Affector",true);
                    }
                }
            }else{
                SerializeError("Create XML Attribute Values",ParticleSystemProxy::GetSerializableName() + "Affectors",true);
            }
        }

        void ParticleSystemProxy::ProtoSerializeProperties(XML::Node& SelfRoot) const
        {
            this->RenderableProxy::ProtoSerializeProperties(SelfRoot);

            XML::Node PropertiesNode = SelfRoot.AppendChild( ParticleSystemProxy::GetSerializableName() + "Properties" );

            if( PropertiesNode.AppendAttribute("Version").SetValue("1") &&
                PropertiesNode.AppendAttribute("Paused").SetValue( this->IsParticleSystemPaused() ) &&
                PropertiesNode.AppendAttribute("SpeedFactor").SetValue( this->GetSpeedFactor() ) )
            {
                return;
            }else{
                SerializeError("Create XML Attribute Values",ParticleSystemProxy::GetSerializableName() + "Properties",true);
            }
        }

        void ParticleSystemProxy::ProtoDeSerialize(const XML::Node& SelfRoot)
        {
            this->DestroyParticleSystem();
            this->ProtoDeSerializeTemplate(SelfRoot);
            this->ProtoDeSerializeCustomParameters(SelfRoot);
            this->ProtoDeSerializeProperties(SelfRoot);
            this->ProtoDeSerializeEmitters(SelfRoot);
            this->ProtoDeSerializeAffectors(SelfRoot);
        }

        void ParticleSystemProxy::ProtoDeSerializeTemplate(const XML::Node& SelfRoot)
        {
            XML::Attribute CurrAttrib;
            XML::Node ConstructionParamNode = SelfRoot.GetChild( ParticleSystemProxy::GetSerializableName() + "Mesh" );

            if( !ConstructionParamNode.Empty() ) {
                if(ConstructionParamNode.GetAttribute("Version").AsInt() == 1) {
                    String TemplateName;

                    CurrAttrib = ConstructionParamNode.GetAttribute("TemplateName");
                    if( !CurrAttrib.Empty() )
                        TemplateName = CurrAttrib.AsString();

                    if( !TemplateName.empty() ) {
                        this->CreateParticleSystem(TemplateName);
                    }else{
                        MEZZ_EXCEPTION(Exception::PARAMETERS_EXCEPTION,"Attempting to DeSerialize a ParticleSystemProxy with an empty name or template.");
                    }
                }
            }
        }

        void ParticleSystemProxy::ProtoDeSerializeCustomParameters(const XML::Node& SelfRoot)
        {
            XML::Attribute CurrAttrib;
            XML::Node CustomParametersNode = SelfRoot.GetChild( ParticleSystemProxy::GetSerializableName() + "CustomParameters" );

            if( !CustomParametersNode.Empty() ) {
                if(CustomParametersNode.GetAttribute("Version").AsInt() == 1) {
                    String ParamName, ParamValue;

                    for( XML::NodeIterator ParamIt = CustomParametersNode.begin() ; ParamIt != CustomParametersNode.end() ; ++ParamIt )
                    {
                        if( !(*ParamIt).Empty() ) {
                            if((*ParamIt).GetAttribute("Version").AsInt() == 1) {
                                CurrAttrib = (*ParamIt).GetAttribute("ParamName");
                                if( !CurrAttrib.Empty() )
                                    ParamName = CurrAttrib.AsString();

                                CurrAttrib = (*ParamIt).GetAttribute("ParamValue");
                                if( !CurrAttrib.Empty() )
                                    ParamValue = CurrAttrib.AsString();

                                if( !ParamName.empty() && !ParamValue.empty() ) {
                                    this->SetCustomParam(ParamName,ParamValue);
                                }
                            }
                        }
                    }
                }else{
                    MEZZ_EXCEPTION(Exception::INVALID_VERSION_EXCEPTION,"Incompatible XML Version for " + (ParticleSystemProxy::GetSerializableName() + "CustomParameters" ) + ": Not Version 1.");
                }
            }else{
                MEZZ_EXCEPTION(Exception::II_IDENTITY_NOT_FOUND_EXCEPTION,ParticleSystemProxy::GetSerializableName() + "CustomParameters" + " was not found in the provided XML node, which was expected.");
            }
        }

        void ParticleSystemProxy::ProtoDeSerializeEmitters(const XML::Node& SelfRoot)
        {
            // Currently this assume the emitters were already created when deserializing the template.
            // So just use what we have and call DeSerialize on them.
            XML::Attribute CurrAttrib;
            XML::Node EmittersNode = SelfRoot.GetChild( ParticleSystemProxy::GetSerializableName() + "Emitters" );

            if( !EmittersNode.Empty() ) {
                if(EmittersNode.GetAttribute("Version").AsInt() == 1) {
                    UInt16 Index = 0;

                    for( XML::NodeIterator EmitIt = EmittersNode.begin() ; EmitIt != EmittersNode.end() ; ++EmitIt )
                    {
                        CurrAttrib = (*EmitIt).GetAttribute("Index");
                        if( !CurrAttrib.Empty() ) {
                            Index = static_cast<UInt16>( CurrAttrib.AsWhole() );
                        }else{
                            MEZZ_EXCEPTION(Exception::PARAMETERS_EXCEPTION,"Attempting to DeSerialize a ParticleEmitter without a valid Index.");
                        }

                        XML::Node EmitterNode = (*EmitIt).GetFirstChild();
                        this->GetEmitter(Index)->ProtoDeSerialize(EmitterNode);
                    }
                }
            }
        }

        void ParticleSystemProxy::ProtoDeSerializeAffectors(const XML::Node& SelfRoot)
        {
            // Currently this assume the emitters were already created when deserializing the template.
            // So just use what we have and call DeSerialize on them.
            XML::Attribute CurrAttrib;
            XML::Node AffectorsNode = SelfRoot.GetChild( ParticleSystemProxy::GetSerializableName() + "Affectors" );

            if( !AffectorsNode.Empty() ) {
                if(AffectorsNode.GetAttribute("Version").AsInt() == 1) {
                    UInt16 Index = 0;

                    for( XML::NodeIterator AffectIt = AffectorsNode.begin() ; AffectIt != AffectorsNode.end() ; ++AffectIt )
                    {
                        CurrAttrib = (*AffectIt).GetAttribute("Index");
                        if( !CurrAttrib.Empty() ) {
                            Index = static_cast<UInt16>( CurrAttrib.AsWhole() );
                        }else{
                            MEZZ_EXCEPTION(Exception::PARAMETERS_EXCEPTION,"Attempting to DeSerialize a ParticleAffector without a valid Index.");
                        }

                        XML::Node AffectorNode = (*AffectIt).GetFirstChild();
                        this->GetAffector(Index)->ProtoDeSerialize(AffectorNode);
                    }
                }
            }
        }

        void ParticleSystemProxy::ProtoDeSerializeProperties(const XML::Node& SelfRoot)
        {
            this->RenderableProxy::ProtoDeSerializeProperties(SelfRoot);

            XML::Attribute CurrAttrib;
            XML::Node PropertiesNode = SelfRoot.GetChild( ParticleSystemProxy::GetSerializableName() + "Properties" );

            if( !PropertiesNode.Empty() ) {
                if(PropertiesNode.GetAttribute("Version").AsInt() == 1) {
                    CurrAttrib = PropertiesNode.GetAttribute("Paused");
                    if( !CurrAttrib.Empty() )
                        this->PauseParticleSystem( StringTools::ConvertToBool( CurrAttrib.AsString() ) );

                    CurrAttrib = PropertiesNode.GetAttribute("SpeedFactor");
                    if( !CurrAttrib.Empty() )
                        this->SetSpeedFactor( CurrAttrib.AsReal() );
                }else{
                    MEZZ_EXCEPTION(Exception::INVALID_VERSION_EXCEPTION,"Incompatible XML Version for " + (ParticleSystemProxy::GetSerializableName() + "Properties" ) + ": Not Version 1.");
                }
            }else{
                MEZZ_EXCEPTION(Exception::II_IDENTITY_NOT_FOUND_EXCEPTION,ParticleSystemProxy::GetSerializableName() + "Properties" + " was not found in the provided XML node, which was expected.");
            }
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
