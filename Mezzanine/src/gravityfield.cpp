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
#ifndef _gravityfield_cpp
#define _gravityfield_cpp

#include "gravityfield.h"

#include "Physics/rigidproxy.h"
#include "Physics/physicsmanager.h"

#include "entresol.h"
#include "serialization.h"
#include "exception.h"

namespace Mezzanine
{
    GravityField::GravityField(World* TheWorld) :
        AreaEffect(TheWorld)
        {  }

    GravityField::GravityField(const String& Name, World* TheWorld) :
        AreaEffect(Name,TheWorld)
        {  }

    GravityField::GravityField(const XML::Node& SelfRoot, World* TheWorld) :
        AreaEffect(TheWorld)
        { this->ProtoDeSerialize(SelfRoot); }

    GravityField::~GravityField()
        {  }

    ///////////////////////////////////////////////////////////////////////////////
    // Utility

    Mezzanine::WorldObjectType GravityField::GetType() const
        { return Mezzanine::WO_AreaEffectGravityField; }

    void GravityField::ApplyEffect()
    {
        for( ObjectIterator AddedIt = this->AddedObjects.begin() ; AddedIt != this->AddedObjects.end() ; ++AddedIt )
        {
            ProxyContainer RigidProxies;
            (*AddedIt)->GetProxies(Mezzanine::PT_Physics_RigidProxy,RigidProxies);
            for( ProxyIterator ProxIt = RigidProxies.begin() ; ProxIt != RigidProxies.end() ; ++ProxIt )
            {
                Physics::RigidProxy* RigProx = static_cast<Physics::RigidProxy*>( *ProxIt );
                RigProx->SetGravity(this->Grav);
            }
        }

        if( !RemovedObjects.empty() )
        {
            const Vector3 WorldGravity = Entresol::GetSingletonPtr()->GetPhysicsManager()->GetWorldGravity();

            for( ObjectIterator RemovedIt = this->RemovedObjects.begin() ; RemovedIt != this->RemovedObjects.end() ; ++RemovedIt )
            {
                ProxyContainer RigidProxies;
                (*RemovedIt)->GetProxies(Mezzanine::PT_Physics_RigidProxy,RigidProxies);
                for( ProxyIterator ProxIt = RigidProxies.begin() ; ProxIt != RigidProxies.end() ; ++ProxIt )
                {
                    Physics::RigidProxy* RigProx = static_cast<Physics::RigidProxy*>( *ProxIt );
                    RigProx->SetGravity(WorldGravity);
                }
            }
        }
    }

    ///////////////////////////////////////////////////////////////////////////////
    // GravityField Properties

    void GravityField::SetFieldGravity(const Vector3& Gravity)
        { this->Grav = Gravity; }

    Vector3 GravityField::GetFieldGravity() const
        { return this->Grav; }

    ///////////////////////////////////////////////////////////////////////////////
    // Serialization

    void GravityField::ProtoSerializeProperties(XML::Node& SelfRoot) const
    {
        this->AreaEffect::ProtoSerializeProperties(SelfRoot);

        XML::Node PropertiesNode = SelfRoot.AppendChild( WorldObject::GetSerializableName() + "Properties" );

        if( PropertiesNode.AppendAttribute("Version").SetValue("1") )
        {
            XML::Node GravityNode = PropertiesNode.AppendChild("Gravity");
            this->GetFieldGravity().ProtoSerialize( GravityNode );

            return;
        }else{
            SerializeError("Create XML Attribute Values",WorldObject::GetSerializableName() + "Properties",true);
        }
    }

    void GravityField::ProtoDeSerializeProperties(const XML::Node& SelfRoot)
    {
        this->AreaEffect::ProtoDeSerializeProperties(SelfRoot);

        XML::Node PropertiesNode = SelfRoot.GetChild( WorldObject::GetSerializableName() + "Properties" );

        if( !PropertiesNode.Empty() ) {
            if(PropertiesNode.GetAttribute("Version").AsInt() == 1) {
                XML::Node GravityNode = PropertiesNode.GetChild("Gravity").GetFirstChild();
                if( !GravityNode.Empty() ) {
                    Vector3 Gravity(GravityNode);
                    this->SetFieldGravity(Gravity);
                }
            }else{
                MEZZ_EXCEPTION(Exception::INVALID_VERSION_EXCEPTION,"Incompatible XML Version for " + (WorldObject::GetSerializableName() + "Properties" ) + ": Not Version 1.");
            }
        }else{
            MEZZ_EXCEPTION(Exception::II_IDENTITY_NOT_FOUND_EXCEPTION,WorldObject::GetSerializableName() + "Properties" + " was not found in the provided XML node, which was expected.");
        }
    }

    String GravityField::GetDerivedSerializableName() const
        { return GravityField::GetSerializableName(); }

    String GravityField::GetSerializableName()
        { return "GravityField"; }

    ///////////////////////////////////////////////////////////////////////////////
    // GravityFieldFactory Methods

    GravityFieldFactory::GravityFieldFactory()
        {  }

    GravityFieldFactory::~GravityFieldFactory()
        {  }

    String GravityFieldFactory::GetTypeName() const
        { return GravityField::GetSerializableName(); }

    GravityField* GravityFieldFactory::CreateGravityField(const String& Name, World* TheWorld)
        { return new GravityField(Name,TheWorld); }

    GravityField* GravityFieldFactory::CreateGravityField(const XML::Node& XMLNode, World* TheWorld)
        { return static_cast<GravityField*>( this->CreateAreaEffect(XMLNode,TheWorld) ); }

    AreaEffect* GravityFieldFactory::CreateAreaEffect(const String& Name, World* TheWorld, const NameValuePairMap& Params)
        { return new GravityField(Name,TheWorld); }

    AreaEffect* GravityFieldFactory::CreateAreaEffect(const XML::Node& XMLNode, World* TheWorld)
        { return new GravityField(XMLNode,TheWorld); }

    void GravityFieldFactory::DestroyAreaEffect(AreaEffect* ToBeDestroyed)
        { delete ToBeDestroyed; }
}//Mezzanine

#endif
