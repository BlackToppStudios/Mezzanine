// © Copyright 2010 - 2015 BlackTopp Studios Inc.
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

#ifndef _graphicsparticleemitter_cpp
#define _graphicsparticleemitter_cpp

#include "Graphics/particleemitter.h"

#include "exception.h"
#include "serialization.h"
#include "stringtool.h"

#include <Ogre.h>

namespace Mezzanine
{
    namespace Graphics
    {
        ParticleEmitter::ParticleEmitter(Ogre::ParticleEmitter* InternalEmitter, ParticleSystemProxy* Creator) :
            GraphicsEmitter(InternalEmitter),
            ParentSystem(Creator)
            {  }

        ParticleEmitter::~ParticleEmitter()
            {  }

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        void ParticleEmitter::SetCustomParam(const String& Name, const String& Value)
        {
            this->GraphicsEmitter->setParameter(Name,Value);
            this->CustomEmitterParameters[Name] = Value;
        }

        String ParticleEmitter::GetCustomParam(const String& Name) const
        {
            return this->GraphicsEmitter->getParameter(Name);
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Serialization

        void ParticleEmitter::ProtoSerialize(XML::Node& ParentNode) const
        {
            XML::Node SelfRoot = ParentNode.AppendChild(this->GetDerivedSerializableName());

            this->ProtoSerializeCustomParameters(SelfRoot);
        }

        /*void ParticleEmitter::ProtoSerializeProperties(XML::Node& SelfRoot) const
        {

        }*/

        void ParticleEmitter::ProtoSerializeCustomParameters(XML::Node& SelfRoot) const
        {
            XML::Node CustomParametersNode = SelfRoot.AppendChild( ParticleEmitter::GetSerializableName() + "CustomParameters" );

            if( CustomParametersNode.AppendAttribute("Version").SetValue("1") )
            {
                for( NameValuePairMap::const_iterator ParamIt = this->CustomEmitterParameters.begin() ; ParamIt != this->CustomEmitterParameters.end() ; ++ParamIt )
                {
                    XML::Node CustomParamNode = CustomParametersNode.AppendChild( "CustomParam" );
                    if( CustomParamNode.AppendAttribute("Version").SetValue("1") &&
                        CustomParamNode.AppendAttribute("ParamName").SetValue( (*ParamIt).first ) &&
                        CustomParamNode.AppendAttribute("ParamValue").SetValue( (*ParamIt).second ) )
                    {
                        return;
                    }else{
                        SerializeError("Create XML Attribute Values",ParticleEmitter::GetSerializableName() + "CustomParameters",true);
                    }
                }
            }else{
                SerializeError("Create XML Attribute Values",ParticleEmitter::GetSerializableName() + "CustomParameters",true);
            }
        }

        void ParticleEmitter::ProtoDeSerialize(const XML::Node& SelfRoot)
        {
            this->ProtoDeSerializeCustomParameters(SelfRoot);
        }

        /*void ParticleEmitter::ProtoDeSerializeProperties(const XML::Node& SelfRoot)
        {

        }*/

        void ParticleEmitter::ProtoDeSerializeCustomParameters(const XML::Node& SelfRoot)
        {
            XML::Attribute CurrAttrib;
            XML::Node CustomParametersNode = SelfRoot.GetChild( ParticleEmitter::GetSerializableName() + "CustomParameters" );

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
                    MEZZ_EXCEPTION(Exception::INVALID_VERSION_EXCEPTION,"Incompatible XML Version for " + (ParticleEmitter::GetSerializableName() + "CustomParameters" ) + ": Not Version 1.");
                }
            }else{
                MEZZ_EXCEPTION(Exception::II_IDENTITY_NOT_FOUND_EXCEPTION,ParticleEmitter::GetSerializableName() + "CustomParameters" + " was not found in the provided XML node, which was expected.");
            }
        }

        String ParticleEmitter::GetDerivedSerializableName() const
            { return ParticleEmitter::GetSerializableName(); }

        String ParticleEmitter::GetSerializableName()
            { return "ParticleEmitter"; }

        ///////////////////////////////////////////////////////////////////////////////
        // Internal Methods

        Ogre::ParticleEmitter* ParticleEmitter::_GetGraphicsEmitter() const
            { return this->GraphicsEmitter; }
    }//Graphics
}//Mezzanine

#endif
