//© Copyright 2010 - 2013 BlackTopp Studios Inc.
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
#ifndef _fieldofforce_cpp
#define _fieldofforce_cpp

#include "fieldofforce.h"

#include "Physics/rigidproxy.h"

#include "serialization.h"
#include "exception.h"

namespace Mezzanine
{
    ///////////////////////////////////////////////////////////////////////////////
    // FieldOfForce Methods

    FieldOfForce::FieldOfForce(World* TheWorld) :
        AreaEffect(TheWorld),
        AttenSource(Vector3(0,0,0)),
        Direction(Vector3(0,1,0)),
        AttenAmount(0),
        Strength(0),
        AttenStyle(Mezzanine::Att_None)
        {  }

    FieldOfForce::FieldOfForce(const String& Name, World* TheWorld) :
        AreaEffect(Name,TheWorld),
        AttenSource(Vector3(0,0,0)),
        Direction(Vector3(0,1,0)),
        AttenAmount(0),
        Strength(0),
        AttenStyle(Mezzanine::Att_None)
        {  }

    FieldOfForce::FieldOfForce(const XML::Node& SelfRoot, World* TheWorld) :
        AreaEffect(TheWorld),
        AttenSource(Vector3(0,0,0)),
        Direction(Vector3(0,1,0)),
        AttenAmount(0),
        Strength(0),
        AttenStyle(Mezzanine::Att_None)
        { this->ProtoDeSerialize(SelfRoot); }

    FieldOfForce::~FieldOfForce()
        {  }

    ///////////////////////////////////////////////////////////////////////////////
    // Utility

    Mezzanine::WorldObjectType FieldOfForce::GetType() const
        { return Mezzanine::WO_AreaEffectFieldOfForce; }

    void FieldOfForce::ApplyEffect()
    {
        /// @todo This currently will apply this fields force uniformly to all rigid proxies contained in a WorldObject.
        /// Instead this should perhaps apply only to the ones in the field, or perhaps apply force based on the proxy position
        /// rather than the WorldObject position to get more interesting results.
        /// @todo Update to allow the application of force to soft proxies.

        if(0 == this->Strength)
            return;
        if( !this->OverlappingObjects.empty() )
        {
            Vector3 ObjectLoc;
            //Real Distance, AppliedStrength, InvMass;
            Real Distance, AppliedStrength;
            for( ObjectIterator ObjIt = this->OverlappingObjects.begin() ; ObjIt != this->OverlappingObjects.end() ; ObjIt++ )
            {
                ProxyContainer RigidProxies;
                (*ObjIt)->GetProxies(Mezzanine::PT_Physics_RigidProxy,RigidProxies);
                if( RigidProxies.empty() )
                    continue;

                ObjectLoc = (*ObjIt)->GetLocation();
                switch(this->AttenStyle)
                {
                    case Mezzanine::Att_Linear:
                    {
                        Distance = ObjectLoc.Distance(this->AttenSource);
                        AppliedStrength = this->Strength - (this->AttenAmount * Distance);
                        break;
                    }
                    case Mezzanine::Att_Quadratic:
                    {
                        Distance = ObjectLoc.Distance(this->AttenSource);
                        AppliedStrength = this->Strength - (this->AttenAmount * (Distance * Distance));
                        break;
                    }
                    case Mezzanine::Att_None:
                    {
                        AppliedStrength = this->Strength;
                        break;
                    }
                }

                //Collect necessary data
                //InvMass = (*ObjIt)->GetBulletObject()->getInvMass();
                //if(0 != InvMass)
                //    AppliedStrength *= (1 / (*ObjIt)->GetBulletObject()->getInvMass());
                //else
                //    AppliedStrength = 0;
                if(0 > AppliedStrength)
                    AppliedStrength = 0;

                //Apply the Force
                for( ProxyIterator ProxIt = RigidProxies.begin() ; ProxIt != RigidProxies.end() ; ++ProxIt )
                {
                    Physics::RigidProxy* RigProx = static_cast<Physics::RigidProxy*>( *ProxIt );
                    RigProx->ApplyForce( Direction * AppliedStrength );
                }
            }
        }
    }

    ///////////////////////////////////////////////////////////////////////////////
    // FieldOfForce Properties

    void FieldOfForce::SetFieldStrength(const Real FieldStrength)
        { this->Strength = FieldStrength; }

    Real FieldOfForce::GetFieldStrength() const
        { return this->Strength; }

    void FieldOfForce::SetDirectionOfForce(const Vector3& ForceDirection)
        { this->Direction = ForceDirection; }

    Vector3 FieldOfForce::GetDirectionOfForce() const
        { return this->Direction; }

    void FieldOfForce::SetAttenuation(const Real Amount, const Mezzanine::AttenuationStyle Style, const Vector3& Source)
    {
        this->AttenAmount = Amount;
        this->AttenStyle = Style;
        this->AttenSource = Source;
    }

    void FieldOfForce::SetAttenuationStyle(const Mezzanine::AttenuationStyle Style)
        { this->AttenStyle = Style; }

    Mezzanine::AttenuationStyle FieldOfForce::GetAttenuationStyle() const
        { return this->AttenStyle; }

    void FieldOfForce::SetAttenuationAmount(const Real Amount)
        { this->AttenAmount = Amount; }

    Real FieldOfForce::GetAttenuationAmount() const
        { return this->AttenAmount; }

    void FieldOfForce::SetAttenuationSource(const Vector3& Source)
        { this->AttenSource = Source; }

    Vector3 FieldOfForce::GetAttenuationSource() const
        { return this->AttenSource; }

    ///////////////////////////////////////////////////////////////////////////////
    // Serialization

    void FieldOfForce::ProtoSerializeProperties(XML::Node& SelfRoot) const
    {
        this->AreaEffect::ProtoSerializeProperties(SelfRoot);

        XML::Node PropertiesNode = SelfRoot.AppendChild( FieldOfForce::GetSerializableName() + "Properties" );

        if( PropertiesNode.AppendAttribute("Version").SetValue("1") &&
            PropertiesNode.AppendAttribute("AttenAmount").SetValue( this->GetAttenuationAmount() ) &&
            PropertiesNode.AppendAttribute("AttenStyle").SetValue( this->GetAttenuationStyle() ) &&
            PropertiesNode.AppendAttribute("Strength").SetValue( this->GetFieldStrength() ) )
        {
            XML::Node AttenSourceNode = PropertiesNode.AppendChild("AttenSource");
            this->GetAttenuationSource().ProtoSerialize( AttenSourceNode );
            XML::Node DirectionNode = PropertiesNode.AppendChild("Direction");
            this->GetDirectionOfForce().ProtoSerialize( DirectionNode );

            return;
        }else{
            SerializeError("Create XML Attribute Values",FieldOfForce::GetSerializableName() + "Properties",true);
        }
    }

    void FieldOfForce::ProtoDeSerializeProperties(const XML::Node& SelfRoot)
    {
        this->AreaEffect::ProtoDeSerializeProperties(SelfRoot);

        XML::Attribute CurrAttrib;
        XML::Node PropertiesNode = SelfRoot.GetChild( FieldOfForce::GetSerializableName() + "Properties" );

        if( !PropertiesNode.Empty() ) {
            if(PropertiesNode.GetAttribute("Version").AsInt() == 1) {
                CurrAttrib = PropertiesNode.GetAttribute("AttenAmount");
                if( !CurrAttrib.Empty() )
                    this->SetAttenuationAmount( CurrAttrib.AsReal() );

                CurrAttrib = PropertiesNode.GetAttribute("AttenStyle");
                if( !CurrAttrib.Empty() )
                    this->SetAttenuationStyle( static_cast<Mezzanine::AttenuationStyle>( CurrAttrib.AsWhole() ) );

                CurrAttrib = PropertiesNode.GetAttribute("Strength");
                if( !CurrAttrib.Empty() )
                    this->SetFieldStrength( CurrAttrib.AsReal() );

                XML::Node AttenSourceNode = PropertiesNode.GetChild("AttenSource").GetFirstChild();
                if( !AttenSourceNode.Empty() ) {
                    Vector3 Source(AttenSourceNode);
                    this->SetAttenuationSource(Source);
                }

                XML::Node DirectionNode = PropertiesNode.GetChild("Direction").GetFirstChild();
                if( !DirectionNode.Empty() ) {
                    Vector3 Dir(DirectionNode);
                    this->SetDirectionOfForce(Dir);
                }
            }else{
                MEZZ_EXCEPTION(Exception::INVALID_VERSION_EXCEPTION,"Incompatible XML Version for " + (FieldOfForce::GetSerializableName() + "Properties" ) + ": Not Version 1.");
            }
        }else{
            MEZZ_EXCEPTION(Exception::II_IDENTITY_NOT_FOUND_EXCEPTION,FieldOfForce::GetSerializableName() + "Properties" + " was not found in the provided XML node, which was expected.");
        }
    }

    String FieldOfForce::GetDerivedSerializableName() const
        { return FieldOfForce::GetSerializableName(); }

    String FieldOfForce::GetSerializableName()
        { return "FieldOfForce"; }

    ///////////////////////////////////////////////////////////////////////////////
    // FieldOfForceFactory Methods

    FieldOfForceFactory::FieldOfForceFactory()
        {  }

    FieldOfForceFactory::~FieldOfForceFactory()
        {  }

    String FieldOfForceFactory::GetTypeName() const
        { return FieldOfForce::GetSerializableName(); }

    FieldOfForce* FieldOfForceFactory::CreateFieldOfForce(const String& Name, World* TheWorld)
        { return new FieldOfForce(Name,TheWorld); }

    FieldOfForce* FieldOfForceFactory::CreateFieldOfForce(const XML::Node& XMLNode, World* TheWorld)
        { return static_cast<FieldOfForce*>( this->CreateAreaEffect(XMLNode,TheWorld) ); }

    AreaEffect* FieldOfForceFactory::CreateAreaEffect(const String& Name, World* TheWorld, NameValuePairList& Params)
        { return new FieldOfForce(Name,TheWorld); }

    AreaEffect* FieldOfForceFactory::CreateAreaEffect(const XML::Node& XMLNode, World* TheWorld)
        { return new FieldOfForce(XMLNode,TheWorld); }

    void FieldOfForceFactory::DestroyAreaEffect(AreaEffect* ToBeDestroyed)
        { delete ToBeDestroyed; }
}//Mezzanine

#endif
