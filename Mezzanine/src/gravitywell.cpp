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
#ifndef _gravitywell_cpp
#define _gravitywell_cpp

#include "gravitywell.h"

#include "Physics/rigidproxy.h"
#include "Physics/ghostproxy.h"
#include "Physics/physicsmanager.h"

#include "entresol.h"
#include "world.h"
#include "serialization.h"
#include "exception.h"
#include "stringtool.h"

namespace Mezzanine
{
    GravityWell::GravityWell(World* TheWorld) :
        AreaEffect(TheWorld),
        AttenAmount(0),
        Strength(0),
        AttenStyle(Mezzanine::Att_None),
        AllowWorldGrav(true)
        {  }

    GravityWell::GravityWell(const String& Name, World* TheWorld) :
        AreaEffect(Name,TheWorld),
        AttenAmount(0),
        Strength(0),
        AttenStyle(Mezzanine::Att_None),
        AllowWorldGrav(true)
        {  }

    GravityWell::GravityWell(const String& Name, Physics::CollisionShape* Shape, World* TheWorld) :
        AreaEffect(Name,Shape,TheWorld),
        AttenAmount(0),
        Strength(0),
        AttenStyle(Mezzanine::Att_None),
        AllowWorldGrav(true)
        {  }

    GravityWell::GravityWell(const XML::Node& SelfRoot, World* TheWorld) :
        AreaEffect(TheWorld),
        AttenAmount(0),
        Strength(0),
        AttenStyle(Mezzanine::Att_None),
        AllowWorldGrav(true)
        { this->ProtoDeSerialize(SelfRoot); }

    GravityWell::~GravityWell()
        {  }

    ///////////////////////////////////////////////////////////////////////////////
    // Utility

    Mezzanine::WorldObjectType GravityWell::GetType() const
        { return Mezzanine::WO_AreaEffectGravityWell; }

    void GravityWell::ApplyEffect()
    {
        /// @todo This currently will apply this fields force uniformly to all rigid proxies contained in a WorldObject.
        /// Instead this should perhaps apply only to the ones in the field, or perhaps apply force based on the proxy position
        /// rather than the WorldObject position to get more interesting results.
        /// @todo Update to allow the application of force to soft proxies.

        if( 0 == this->Strength )
            return;

        if( !this->AllowWorldGrav && !this->AddedObjects.empty() )
        {
            for( ObjectIterator AddedIt = this->AddedObjects.begin() ; AddedIt != this->AddedObjects.end() ; ++AddedIt )
            {
                ProxyContainer RigidProxies;
                (*AddedIt)->GetProxies(Mezzanine::PT_Physics_RigidProxy,RigidProxies);
                for( ProxyIterator ProxIt = RigidProxies.begin() ; ProxIt != RigidProxies.end() ; ++ProxIt )
                {
                    Physics::RigidProxy* RigProx = static_cast<Physics::RigidProxy*>( *ProxIt );
                    RigProx->SetGravity( Vector3(0,0,0) );
                }
            }
        }

        if( !this->OverlappingObjects.empty() ) {
            const Vector3 SelfLoc = this->Ghost->GetLocation();
            Vector3 ObjectLoc, Direction;
            Real Distance = 0,AppliedStrength = 0;

            for( ObjectIterator ObjIt = this->OverlappingObjects.begin() ; ObjIt != this->OverlappingObjects.end() ; ObjIt++ )
            {
                ProxyContainer RigidProxies;
                (*ObjIt)->GetProxies(Mezzanine::PT_Physics_RigidProxy,RigidProxies);
                if( RigidProxies.empty() )
                    continue;

                Distance = ObjectLoc.Distance(SelfLoc);
                Direction = (SelfLoc - ObjectLoc) / Distance;
                switch(this->AttenStyle)
                {
                    case Mezzanine::Att_Linear:
                        AppliedStrength = Strength - (AttenAmount * Distance);
                        break;
                    case Mezzanine::Att_Quadratic:
                        AppliedStrength = Strength - (AttenAmount * (Distance * Distance));
                        break;
                    default:
                        AppliedStrength = Strength;
                        break;
                }

                //Apply the Force
                for( ProxyIterator ProxIt = RigidProxies.begin() ; ProxIt != RigidProxies.end() ; ++ProxIt )
                {
                    Physics::RigidProxy* RigProx = static_cast<Physics::RigidProxy*>( *ProxIt );

                    Real Mass = RigProx->GetMass();
                    if( 0 > AppliedStrength ) {
                        AppliedStrength = 0;
                    }

                    RigProx->ApplyForce( Direction * (AppliedStrength * Mass ) );
                }
            }
        }

        if( !this->AllowWorldGrav && !this->RemovedObjects.empty() )
        {
            const Vector3 WorldGravity = static_cast<Physics::PhysicsManager*>( this->ParentWorld->GetManager(ManagerBase::MT_PhysicsManager) )->GetWorldGravity();
            for( ObjectIterator RemovedIt = this->RemovedObjects.begin() ; RemovedIt != this->RemovedObjects.end() ; ++RemovedIt )
            {
                ProxyContainer RigidProxies;
                (*RemovedIt)->GetProxies(Mezzanine::PT_Physics_RigidProxy,RigidProxies);
                for( ProxyIterator ProxIt = RigidProxies.begin() ; ProxIt != RigidProxies.end() ; ++ProxIt )
                {
                    Physics::RigidProxy* RigProx = static_cast<Physics::RigidProxy*>( *ProxIt );
                    RigProx->SetGravity( WorldGravity );
                }
            }
        }
    }

    ///////////////////////////////////////////////////////////////////////////////
    // GravityWell Properties

    void GravityWell::SetFieldStrength(const Real FieldStrength)
        { this->Strength = FieldStrength; }

    Real GravityWell::GetFieldStrength() const
        { return this->Strength; }

    void GravityWell::SetAllowWorldGravity(Boole WorldGravity)
        { this->AllowWorldGrav = WorldGravity; }

    Boole GravityWell::GetAllowWorldGravity() const
        { return this->AllowWorldGrav; }

    void GravityWell::SetAttenuation(const Real Amount, const Mezzanine::AttenuationStyle Style)
    {
        this->AttenAmount = Amount;
        this->AttenStyle = Style;
    }

    void GravityWell::SetAttenuationStyle(const Mezzanine::AttenuationStyle Style)
        { this->AttenStyle = Style; }

    Mezzanine::AttenuationStyle GravityWell::GetAttenuationStyle() const
        { return this->AttenStyle; }

    void GravityWell::SetAttenuationAmount(const Real Amount)
        { this->AttenAmount = Amount; }

    Real GravityWell::GetAttenuationAmount() const
        { return this->AttenAmount; }

    ///////////////////////////////////////////////////////////////////////////////
    // Serialization

    void GravityWell::ProtoSerializeProperties(XML::Node& SelfRoot) const
    {
        this->AreaEffect::ProtoSerializeProperties(SelfRoot);

        XML::Node PropertiesNode = SelfRoot.AppendChild( GravityWell::GetSerializableName() + "Properties" );

        if( PropertiesNode.AppendAttribute("Version").SetValue("1") &&
            PropertiesNode.AppendAttribute("AttenAmount").SetValue( this->GetAttenuationAmount() ) &&
            PropertiesNode.AppendAttribute("AttenStyle").SetValue( this->GetAttenuationStyle() ) &&
            PropertiesNode.AppendAttribute("Strength").SetValue( this->GetFieldStrength() ) &&
            PropertiesNode.AppendAttribute("AllowWorldGravity").SetValue( this->GetAllowWorldGravity() ? "true" : "false" ) )
        {
            return;
        }else{
            SerializeError("Create XML Attribute Values",GravityWell::GetSerializableName() + "Properties",true);
        }
    }

    void GravityWell::ProtoDeSerializeProperties(const XML::Node& SelfRoot)
    {
        this->AreaEffect::ProtoDeSerializeProperties(SelfRoot);

        XML::Attribute CurrAttrib;
        XML::Node PropertiesNode = SelfRoot.GetChild( GravityWell::GetSerializableName() + "Properties" );

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

                CurrAttrib = PropertiesNode.GetAttribute("AllowWorldGravity");
                if( !CurrAttrib.Empty() )
                    this->SetAllowWorldGravity( StringTools::ConvertToBool( CurrAttrib.AsString() ) );
            }else{
                MEZZ_EXCEPTION(ExceptionBase::INVALID_VERSION_EXCEPTION,"Incompatible XML Version for " + (GravityWell::GetSerializableName() + "Properties" ) + ": Not Version 1.");
            }
        }else{
            MEZZ_EXCEPTION(ExceptionBase::II_IDENTITY_NOT_FOUND_EXCEPTION,GravityWell::GetSerializableName() + "Properties" + " was not found in the provided XML node, which was expected.");
        }
    }

    String GravityWell::GetDerivedSerializableName() const
        { return GravityWell::GetSerializableName(); }

    String GravityWell::GetSerializableName()
        { return "GravityWell"; }

    ///////////////////////////////////////////////////////////////////////////////
    // GravityWellFactory Methods

    GravityWellFactory::GravityWellFactory()
        {  }

    GravityWellFactory::~GravityWellFactory()
        {  }

    String GravityWellFactory::GetTypeName() const
        { return GravityWell::GetSerializableName(); }

    GravityWell* GravityWellFactory::CreateGravityWell(const String& Name, World* TheWorld)
        { return new GravityWell(Name,TheWorld); }

    GravityWell* GravityWellFactory::CreateGravityWell(const String& Name, Physics::CollisionShape* AEShape, World* TheWorld)
        { return new GravityWell(Name,AEShape,TheWorld); }

    GravityWell* GravityWellFactory::CreateGravityWell(const XML::Node& XMLNode, World* TheWorld)
        { return static_cast<GravityWell*>( this->CreateAreaEffect(XMLNode,TheWorld) ); }

    AreaEffect* GravityWellFactory::CreateAreaEffect(const String& Name, World* TheWorld, const NameValuePairMap& Params)
        { return new GravityWell(Name,TheWorld); }

    AreaEffect* GravityWellFactory::CreateAreaEffect(const XML::Node& XMLNode, World* TheWorld)
        { return new GravityWell(XMLNode,TheWorld); }

    void GravityWellFactory::DestroyAreaEffect(AreaEffect* ToBeDestroyed)
        { delete ToBeDestroyed; }
}//Mezzanine

#endif
