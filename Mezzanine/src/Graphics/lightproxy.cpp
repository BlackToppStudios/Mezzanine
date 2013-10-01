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
#ifndef _graphicslightproxy_cpp
#define _graphicslightproxy_cpp

/// @file
/// @brief This file contains the implementation for the World proxy wrapping light functionality.

#include "Graphics/lightproxy.h"
#include "Graphics/scenemanager.h"

#include "exception.h"
#include "serialization.h"
#include "stringtool.h"

#include <Ogre.h>

namespace
{
    /// @internal
    /// @brief Converts an Ogre LightType enum value to it's corresponding Mezzanine type.
    /// @param Type The Ogre type to be converted.
    /// @return Returns the Mezzanine LightType corresponding to the provided Ogre type.
    Mezzanine::Graphics::LightType ConvertLightType(const Ogre::Light::LightTypes Type)
    {
        switch(Type)
        {
            case Ogre::Light::LT_DIRECTIONAL:  return Mezzanine::Graphics::LT_Directional;  break;
            case Ogre::Light::LT_POINT:        return Mezzanine::Graphics::LT_Point;        break;
            case Ogre::Light::LT_SPOTLIGHT:    return Mezzanine::Graphics::LT_Spotlight;    break;
        }
        return Mezzanine::Graphics::LT_Point;
    }

    /// @internal
    /// @brief Converts a Mezzanine LightType enum value to it's corresponding Ogre type.
    /// @param Type The Mezzanine type to be converted.
    /// @return Returns the Ogre LightType corresponding to the provided Mezzanine type.
    Ogre::Light::LightTypes ConvertLightType(const Mezzanine::Graphics::LightType Type)
    {
        switch(Type)
        {
            case Mezzanine::Graphics::LT_Directional:  return Ogre::Light::LT_DIRECTIONAL;  break;
            case Mezzanine::Graphics::LT_Point:        return Ogre::Light::LT_POINT;        break;
            case Mezzanine::Graphics::LT_Spotlight:    return Ogre::Light::LT_SPOTLIGHT;    break;
        }
        return Ogre::Light::LT_POINT;
    }
}

namespace Mezzanine
{
    namespace Graphics
    {
        LightProxy::LightProxy(SceneManager* Creator) :
            RenderableProxy(Creator),
            GraphicsLight(NULL)
            { this->CreateLight(); }

        LightProxy::~LightProxy()
            { this->DestroyLight(); }

        void LightProxy::CreateLight()
        {
            this->GraphicsLight = this->Manager->_GetGraphicsWorldPointer()->createLight();
            this->GraphicsNode->attachObject( this->GraphicsLight );
            this->GraphicsLight->setUserAny( Ogre::Any( static_cast<RenderableProxy*>( this ) ) );
            this->GraphicsLight->setVisibilityFlags(0);
            this->GraphicsLight->setQueryFlags(0);
        }

        void LightProxy::DestroyLight()
        {
            if( this->GraphicsLight ) {
                this->GraphicsNode->detachObject( this->GraphicsLight );
                this->Manager->_GetGraphicsWorldPointer()->destroyLight( this->GraphicsLight );
            }
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        Mezzanine::ProxyType LightProxy::GetProxyType() const
            { return Mezzanine::PT_Graphics_LightProxy; }

        void LightProxy::SetDirection(const Vector3& Dir)
            { this->SetOrientation( Vector3::Unit_Z().GetRotationToAxis( Dir ) ); }

        Vector3 LightProxy::GetDirection() const
            { return ( this->GetOrientation() * Vector3::Unit_Z() ); }

        ///////////////////////////////////////////////////////////////////////////////
        // Light Properties

        void LightProxy::SetDiffuseColour(const ColourValue& Diffuse)
            { this->GraphicsLight->setDiffuseColour( Diffuse.GetOgreColourValue() ); }

        ColourValue LightProxy::GetDiffuseColour() const
            { return ColourValue( this->GraphicsLight->getDiffuseColour() ); }

        void LightProxy::SetSpecularColour(const ColourValue& Specular)
            { this->GraphicsLight->setSpecularColour( Specular.GetOgreColourValue() ); }

        ColourValue LightProxy::GetSpecularColour() const
            { return ColourValue( this->GraphicsLight->getSpecularColour() ); }

        void LightProxy::SetType(const Graphics::LightType Type)
            { this->GraphicsLight->setType( ConvertLightType(Type) ); }

        Graphics::LightType LightProxy::GetType() const
            { return ConvertLightType( this->GraphicsLight->getType() ); }

        void LightProxy::SetAttenuation(const Real Range, const Real Constant, const Real Linear, const Real Quadratic)
            { this->GraphicsLight->setAttenuation(Range,Constant,Linear,Quadratic); }

        Real LightProxy::GetAttenuationRange() const
            { return this->GraphicsLight->getAttenuationRange(); }

        Real LightProxy::GetAttenuationConstant() const
            { return this->GraphicsLight->getAttenuationConstant(); }

        Real LightProxy::GetAttenuationLinear() const
            { return this->GraphicsLight->getAttenuationLinear(); }

        Real LightProxy::GetAttenuationQuadratic() const
            { return this->GraphicsLight->getAttenuationQuadric(); }

        void LightProxy::SetPowerScale(const Real Scale)
            { this->GraphicsLight->setPowerScale(Scale); }

        Real LightProxy::GetPowerScale() const
            { return this->GraphicsLight->getPowerScale(); }

        void LightProxy::SetSpotlightRange(const Real InnerAngle, const Real OuterAngle, const Real Falloff)
            { this->GraphicsLight->setSpotlightRange(Ogre::Radian(InnerAngle),Ogre::Radian(OuterAngle),Falloff); }

        void LightProxy::SetSpotlightInnerAngle(const Real Angle)
            { this->GraphicsLight->setSpotlightInnerAngle(Ogre::Radian(Angle)); }

        Real LightProxy::GetSpotlightInnerAngle() const
            { return this->GraphicsLight->getSpotlightInnerAngle().valueRadians(); }

        void LightProxy::SetSpotlightOuterAngle(const Real Angle)
            { this->GraphicsLight->setSpotlightOuterAngle(Ogre::Radian(Angle)); }

        Real LightProxy::GetSpotlightOuterAngle() const
            { return this->GraphicsLight->getSpotlightOuterAngle().valueRadians(); }

        void LightProxy::SetSpotlightFalloff(const Real Falloff)
            { this->GraphicsLight->setSpotlightFalloff(Falloff); }

        Real LightProxy::GetSpotlightFalloff() const
            { return this->GraphicsLight->getSpotlightFalloff(); }

        void LightProxy::SetSpotlightNearClipDistance(const Real NearClip)
            { this->GraphicsLight->setSpotlightNearClipDistance(NearClip); }

        Real LightProxy::GetSpotlightNearClipDistance() const
            { return this->GraphicsLight->getSpotlightNearClipDistance(); }

        ///////////////////////////////////////////////////////////////////////////////
        // Serialization

        void LightProxy::ProtoSerializeProperties(XML::Node& SelfRoot) const
        {
            this->RenderableProxy::ProtoSerializeProperties(SelfRoot);

            XML::Node PropertiesNode = SelfRoot.AppendChild( LightProxy::GetSerializableName() + "Properties" );

            if( PropertiesNode.AppendAttribute("Version").SetValue("1") &&
                PropertiesNode.AppendAttribute("LightType").SetValue( this->GetType() ) &&
                PropertiesNode.AppendAttribute("AttenRange").SetValue( this->GetAttenuationRange() ) &&
                PropertiesNode.AppendAttribute("AttenConstant").SetValue( this->GetAttenuationConstant() ) &&
                PropertiesNode.AppendAttribute("AttenLinear").SetValue( this->GetAttenuationLinear() ) &&
                PropertiesNode.AppendAttribute("AttenQuadratic").SetValue( this->GetAttenuationQuadratic() ) &&
                PropertiesNode.AppendAttribute("PowerScale").SetValue( this->GetPowerScale() ) &&
                PropertiesNode.AppendAttribute("SpotlightInnerAngle").SetValue( this->GetSpotlightInnerAngle() ) &&
                PropertiesNode.AppendAttribute("SpotlightOuterAngle").SetValue( this->GetSpotlightOuterAngle() ) &&
                PropertiesNode.AppendAttribute("SpotlightFalloff").SetValue( this->GetSpotlightFalloff() ) &&
                PropertiesNode.AppendAttribute("SpotlightNearClipDistance").SetValue( this->GetSpotlightNearClipDistance() ) )
            {
                XML::Node DiffuseColourNode = PropertiesNode.AppendChild("DiffuseColour");
                this->GetDiffuseColour().ProtoSerialize( DiffuseColourNode );
                XML::Node SpecularColourNode = PropertiesNode.AppendChild("SpecularColour");
                this->GetSpecularColour().ProtoSerialize( SpecularColourNode );

                return;
            }else{
                SerializeError("Create XML Attribute Values",LightProxy::GetSerializableName() + "Properties",true);
            }
        }

        void LightProxy::ProtoDeSerializeProperties(const XML::Node& SelfRoot)
        {
            this->RenderableProxy::ProtoDeSerializeProperties(SelfRoot);

            XML::Attribute CurrAttrib;
            XML::Node PropertiesNode = SelfRoot.GetChild( LightProxy::GetSerializableName() + "Properties" );

            if( !PropertiesNode.Empty() ) {
                if(PropertiesNode.GetAttribute("Version").AsInt() == 1) {
                    Real AttenRange = 100000.0, AttenConstant = 1.0, AttenLinear = 0.0, AttenQuadratic = 0.0;

                    CurrAttrib = PropertiesNode.GetAttribute("LightType");
                    if( !CurrAttrib.Empty() )
                        this->SetType( static_cast<Graphics::LightType>( CurrAttrib.AsWhole() ) );

                    CurrAttrib = PropertiesNode.GetAttribute("AttenRange");
                    if( !CurrAttrib.Empty() )
                        AttenRange = CurrAttrib.AsReal();

                    CurrAttrib = PropertiesNode.GetAttribute("AttenConstant");
                    if( !CurrAttrib.Empty() )
                        AttenConstant = CurrAttrib.AsReal();

                    CurrAttrib = PropertiesNode.GetAttribute("AttenLinear");
                    if( !CurrAttrib.Empty() )
                        AttenLinear = CurrAttrib.AsReal();

                    CurrAttrib = PropertiesNode.GetAttribute("AttenQuadratic");
                    if( !CurrAttrib.Empty() )
                        AttenQuadratic = CurrAttrib.AsReal();

                    CurrAttrib = PropertiesNode.GetAttribute("PowerScale");
                    if( !CurrAttrib.Empty() )
                        this->SetPowerScale( CurrAttrib.AsReal() );

                    CurrAttrib = PropertiesNode.GetAttribute("SpotlightInnerAngle");
                    if( !CurrAttrib.Empty() )
                        this->SetSpotlightInnerAngle( CurrAttrib.AsReal() );

                    CurrAttrib = PropertiesNode.GetAttribute("SpotlightOuterAngle");
                    if( !CurrAttrib.Empty() )
                        this->SetSpotlightOuterAngle( CurrAttrib.AsReal() );

                    CurrAttrib = PropertiesNode.GetAttribute("SpotlightFalloff");
                    if( !CurrAttrib.Empty() )
                        this->SetSpotlightFalloff( CurrAttrib.AsReal() );

                    CurrAttrib = PropertiesNode.GetAttribute("SpotlightNearClipDistance");
                    if( !CurrAttrib.Empty() )
                        this->SetSpotlightNearClipDistance( CurrAttrib.AsReal() );

                    this->SetAttenuation(AttenRange,AttenConstant,AttenLinear,AttenQuadratic);

                    XML::Node DiffuseColourNode = PropertiesNode.GetChild("DiffuseColour").GetFirstChild();
                    if( !DiffuseColourNode.Empty() ) {
                        ColourValue Loc(DiffuseColourNode);
                        this->SetDiffuseColour(Loc);
                    }

                    XML::Node SpecularColourNode = PropertiesNode.GetChild("SpecularColour").GetFirstChild();
                    if( !SpecularColourNode.Empty() ) {
                        ColourValue Loc(SpecularColourNode);
                        this->SetSpecularColour(Loc);
                    }
                }else{
                    MEZZ_EXCEPTION(Exception::INVALID_VERSION_EXCEPTION,"Incompatible XML Version for " + (LightProxy::GetSerializableName() + "Properties" ) + ": Not Version 1.");
                }
            }else{
                MEZZ_EXCEPTION(Exception::II_IDENTITY_NOT_FOUND_EXCEPTION,LightProxy::GetSerializableName() + "Properties" + " was not found in the provided XML node, which was expected.");
            }
        }

        String LightProxy::GetDerivedSerializableName() const
            { return LightProxy::GetSerializableName(); }

        String LightProxy::GetSerializableName()
            { return "LightProxy"; }

        ///////////////////////////////////////////////////////////////////////////////
        // Internal Methods

        Ogre::Light* LightProxy::_GetGraphicsObject() const
            { return this->GraphicsLight; }

        Ogre::MovableObject* LightProxy::_GetBaseGraphicsObject() const
            { return this->GraphicsLight; }
    }//Graphics
}//Mezzanine

#endif
