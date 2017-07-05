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
#ifndef _rigiddebris_cpp
#define _rigiddebris_cpp

/// @file
/// @brief This file contains the implementation for the debris class that does not deform.

#include "rigiddebris.h"

#include "Graphics/scenemanager.h"
#include "Graphics/itemproxy.h"

#include "Physics/physicsmanager.h"
#include "Physics/rigidproxy.h"

#include "entresol.h"
#include "world.h"
#include "exception.h"
#include "stringtool.h"
#include "serialization.h"

namespace Mezzanine
{
    ///////////////////////////////////////////////////////////////////////////////
    // RigidDebris Methods

    RigidDebris::RigidDebris(World* TheWorld) :
        Debris(TheWorld)
        {  }

    RigidDebris::RigidDebris(const String& Name, const Real Mass, World* TheWorld) :
        Debris(Name,TheWorld)
        { this->CreateRigidDebris(Mass); }

    RigidDebris::RigidDebris(const String& Name, const Real Mass, Graphics::Mesh* DebMesh, Physics::CollisionShape* DebShape, World* TheWorld) :
        Debris(Name,TheWorld)
        { this->CreateRigidDebris(Mass,DebMesh,DebShape); }

    RigidDebris::RigidDebris(const XML::Node& SelfRoot, World* TheWorld) :
        Debris(TheWorld)
        { this->ProtoDeSerialize(SelfRoot); }

    RigidDebris::~RigidDebris()
        { this->DestroyRigidDebris(); }

    void RigidDebris::CreateRigidDebris(const Real Mass)
    {
        Physics::RigidProxy* RigProx = NULL;
        Physics::PhysicsManager* PhysMan = static_cast<Physics::PhysicsManager*>( this->ParentWorld->GetManager(ManagerBase::MT_PhysicsManager) );
        if( PhysMan ) {
            RigProx = PhysMan->CreateRigidProxy(Mass);
            this->AddComponent( RigProx );
            this->SetPrimaryProxy( RigProx );
        }

        Graphics::ItemProxy* ItemProx = NULL;
        Graphics::SceneManager* SceneMan = static_cast<Graphics::SceneManager*>( this->ParentWorld->GetManager(ManagerBase::MT_SceneManager) );
        if( SceneMan ) {
            ItemProx = SceneMan->CreateItemProxy();
            this->AddComponent( ItemProx );
        }

        //if( RigProx && EntProx ) {
        //    RigProx->AddSyncObject( EntProx );
        //}
    }

    void RigidDebris::CreateRigidDebris(const Real Mass, Graphics::Mesh* DebMesh, Physics::CollisionShape* DebShape)
    {
        Physics::RigidProxy* RigProx = NULL;
        Physics::PhysicsManager* PhysMan = static_cast<Physics::PhysicsManager*>( this->ParentWorld->GetManager(ManagerBase::MT_PhysicsManager) );
        if( PhysMan ) {
            RigProx = PhysMan->CreateRigidProxy(Mass,DebShape);
            this->AddComponent( RigProx );
            this->SetPrimaryProxy( RigProx );
        }

        Graphics::ItemProxy* ItemProx = NULL;
        Graphics::SceneManager* SceneMan = static_cast<Graphics::SceneManager*>( this->ParentWorld->GetManager(ManagerBase::MT_SceneManager) );
        if( SceneMan ) {
            ItemProx = SceneMan->CreateItemProxy(DebMesh);
            this->AddComponent( ItemProx );
        }

        //if( RigProx && EntProx ) {
        //    RigProx->AddSyncObject( EntProx );
        //}
    }

    void RigidDebris::DestroyRigidDebris()
    {
        this->RemoveFromWorld();
        this->DestroyAllComponents();
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Utility and Configuration

    EntityType RigidDebris::GetType() const
        { return Mezzanine::ET_RigidDebris; }

    Graphics::ItemProxy* RigidDebris::GetItemProxy() const
        { return static_cast<Graphics::ItemProxy*>( this->GetComponent(Mezzanine::CT_Graphics_ItemProxy,0) ); }

    Physics::RigidProxy* RigidDebris::GetRigidProxy() const
        { return static_cast<Physics::RigidProxy*>( this->GetComponent(Mezzanine::CT_Physics_RigidProxy,0) ); }

    ///////////////////////////////////////////////////////////////////////////////
    // Serialization

    void RigidDebris::ProtoSerializeProperties(XML::Node& SelfRoot) const
    {
        this->Debris::ProtoSerializeProperties(SelfRoot);
    }

    void RigidDebris::ProtoDeSerializeProperties(const XML::Node& SelfRoot)
    {
        this->Debris::ProtoDeSerializeProperties(SelfRoot);
    }

    String RigidDebris::GetDerivedSerializableName() const
        { return RigidDebris::GetSerializableName(); }

    String RigidDebris::GetSerializableName()
        { return "RigidDebris"; }

    ///////////////////////////////////////////////////////////////////////////////
    // Internal Methods

    void RigidDebris::_Update(const Whole Delta)
    {
        // Do nothing
    }

    ///////////////////////////////////////////////////////////////////////////////
    // RigidDebrisFactory Methods

    RigidDebrisFactory::RigidDebrisFactory()
        {  }

    RigidDebrisFactory::~RigidDebrisFactory()
        {  }

    String RigidDebrisFactory::GetTypeName() const
        { return RigidDebris::GetSerializableName(); }

    RigidDebris* RigidDebrisFactory::CreateRigidDebris(const String& Name, const Real Mass, World* TheWorld)
        { return new RigidDebris(Name,Mass,TheWorld); }

    RigidDebris* RigidDebrisFactory::CreateRigidDebris(const String& Name, const Real Mass, Graphics::Mesh* DebMesh, Physics::CollisionShape* DebShape, World* TheWorld)
        { return new RigidDebris(Name,Mass,DebMesh,DebShape,TheWorld); }

    RigidDebris* RigidDebrisFactory::CreateRigidDebris(const XML::Node& XMLNode, World* TheWorld)
        { return static_cast<RigidDebris*>( this->CreateDebris(XMLNode,TheWorld) ); }

    Debris* RigidDebrisFactory::CreateDebris(const String& Name, World* TheWorld, const NameValuePairMap& Params)
    {
        Real Mass = 0;
        NameValuePairMap::const_iterator ParamIt = Params.find( "Mass" );
        if( ParamIt != Params.end() )
            Mass = StringTools::ConvertToReal( (*ParamIt).second );

        return new RigidDebris(Name,Mass,TheWorld);
    }

    Debris* RigidDebrisFactory::CreateDebris(const XML::Node& XMLNode, World* TheWorld)
        { return new RigidDebris(XMLNode,TheWorld); }

    void RigidDebrisFactory::DestroyDebris(Debris* ToBeDestroyed)
        { delete ToBeDestroyed; }
}//Mezzanine

#endif
