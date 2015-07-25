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
#ifndef _rigiddebris_cpp
#define _rigiddebris_cpp

/// @file
/// @brief This file contains the implementation for the debris class that does not deform.

#include "rigiddebris.h"

#include "Graphics/scenemanager.h"
#include "Graphics/entityproxy.h"

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
        Debris(TheWorld),
        EntProx(NULL),
        RigProx(NULL)
        {  }

    RigidDebris::RigidDebris(const String& Name, const Real Mass, World* TheWorld) :
        Debris(Name,TheWorld),
        EntProx(NULL),
        RigProx(NULL)
        { this->CreateRigidDebris(Mass); }

    RigidDebris::RigidDebris(const String& Name, const Real Mass, Graphics::Mesh* DebMesh, Physics::CollisionShape* DebShape, World* TheWorld) :
        Debris(Name,TheWorld),
        EntProx(NULL),
        RigProx(NULL)
        { this->CreateRigidDebris(Mass,DebMesh,DebShape); }

    RigidDebris::RigidDebris(const XML::Node& SelfRoot, World* TheWorld) :
        Debris(TheWorld),
        EntProx(NULL),
        RigProx(NULL)
        { this->ProtoDeSerialize(SelfRoot); }

    RigidDebris::~RigidDebris()
        { this->DestroyRigidDebris(); }

    void RigidDebris::CreateRigidDebris(const Real Mass)
    {
        Graphics::SceneManager* SceneMan = this->ParentWorld->GetSceneManager();
        if( SceneMan ) {
            this->EntProx = SceneMan->CreateEntityProxy(false);
            this->EntProx->_Bind( this );
        }

        Physics::PhysicsManager* PhysMan = this->ParentWorld->GetPhysicsManager();
        if( PhysMan ) {
            this->RigProx = PhysMan->CreateRigidProxy(Mass);
            this->RigProx->_Bind( this );
        }

        if( this->EntProx && this->RigProx ) {
            this->RigProx->AddSyncObject( this->EntProx );
        }
    }

    void RigidDebris::CreateRigidDebris(const Real Mass, Graphics::Mesh* DebMesh, Physics::CollisionShape* DebShape)
    {
        Graphics::SceneManager* SceneMan = this->ParentWorld->GetSceneManager();
        if( SceneMan ) {
            this->EntProx = SceneMan->CreateEntityProxy(DebMesh,false);
            this->EntProx->_Bind( this );
        }

        Physics::PhysicsManager* PhysMan = this->ParentWorld->GetPhysicsManager();
        if( PhysMan ) {
            this->RigProx = PhysMan->CreateRigidProxy(Mass,DebShape,false);
            this->RigProx->_Bind( this );
        }

        if( this->EntProx && this->RigProx ) {
            this->RigProx->AddSyncObject( this->EntProx );
        }
    }

    void RigidDebris::DestroyRigidDebris()
    {
        this->RemoveFromWorld();
        if( this->EntProx ) {
            Graphics::SceneManager* SceneMan = ParentWorld->GetSceneManager();
            if( SceneMan ) {
                SceneMan->DestroyProxy( this->EntProx );
                this->EntProx = NULL;
            }
        }

        if( this->RigProx ) {
            Physics::PhysicsManager* PhysMan = this->ParentWorld->GetPhysicsManager();
            if( PhysMan ) {
                PhysMan->DestroyProxy( this->RigProx );
                this->RigProx = NULL;
            }
        }
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Utility and Configuration

    WorldObjectType RigidDebris::GetType() const
        { return Mezzanine::WO_DebrisRigid; }

    Graphics::EntityProxy* RigidDebris::GetEntityProxy() const
        { return this->EntProx; }

    Physics::RigidProxy* RigidDebris::GetRigidProxy() const
        { return this->RigProx; }

    Boole RigidDebris::IsInWorld() const
        { return this->RigProx->IsInWorld(); }

    Boole RigidDebris::IsStatic() const
        { return this->RigProx->IsStatic(); }

    Boole RigidDebris::IsKinematic() const
        { return this->RigProx->IsKinematic(); }

    void RigidDebris::GetProxies(ProxyContainer& Proxies)
    {
        Proxies.push_back( this->EntProx );
        Proxies.push_back( this->RigProx );
    }

    void RigidDebris::GetProxies(const UInt32 Types, ProxyContainer& Proxies)
    {
        if( Types & Mezzanine::PT_Graphics_EntityProxy ) {
            Proxies.push_back( this->EntProx );
        }
        if( Types & Mezzanine::PT_Physics_RigidProxy ) {
            Proxies.push_back( this->RigProx );
        }
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Working with the World

    void RigidDebris::AddToWorld()
    {
        if( this->EntProx )
            this->EntProx->AddToWorld();

        if( this->RigProx )
            this->RigProx->AddToWorld();
    }

    void RigidDebris::RemoveFromWorld()
    {
        if( this->EntProx )
            this->EntProx->RemoveFromWorld();

        if( this->RigProx )
            this->RigProx->RemoveFromWorld();
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Transform Methods

    void RigidDebris::SetLocation(const Vector3& Loc)
    {
        this->RigProx->SetLocation(Loc);
        this->EntProx->SetLocation(Loc);
    }

    void RigidDebris::SetLocation(const Real X, const Real Y, const Real Z)
    {
        this->RigProx->SetLocation(X,Y,Z);
        this->EntProx->SetLocation(X,Y,Z);
    }

    Vector3 RigidDebris::GetLocation() const
    {
        return this->RigProx->GetLocation();
    }

    void RigidDebris::SetOrientation(const Quaternion& Ori)
    {
        this->RigProx->SetOrientation(Ori);
        this->EntProx->SetOrientation(Ori);
    }

    void RigidDebris::SetOrientation(const Real X, const Real Y, const Real Z, const Real W)
    {
        this->RigProx->SetOrientation(X,Y,Z,W);
        this->EntProx->SetOrientation(X,Y,Z,W);
    }

    Quaternion RigidDebris::GetOrientation() const
    {
        return this->RigProx->GetOrientation();
    }

    void RigidDebris::SetScale(const Vector3& Sc)
    {
        this->RigProx->SetScale(Sc);
        this->EntProx->SetScale(Sc);
    }

    void RigidDebris::SetScale(const Real X, const Real Y, const Real Z)
    {
        this->RigProx->SetScale(X,Y,Z);
        this->EntProx->SetScale(X,Y,Z);
    }

    Vector3 RigidDebris::GetScale() const
    {
        return this->RigProx->GetScale();
    }

    void RigidDebris::Translate(const Vector3& Trans)
    {
        this->RigProx->Translate(Trans);
        this->EntProx->Translate(Trans);
    }

    void RigidDebris::Translate(const Real X, const Real Y, const Real Z)
    {
        this->RigProx->Translate(X,Y,Z);
        this->EntProx->Translate(X,Y,Z);
    }

    void RigidDebris::Yaw(const Real Angle)
    {
        this->RigProx->Yaw(Angle);
        this->EntProx->Yaw(Angle);
    }

    void RigidDebris::Pitch(const Real Angle)
    {
        this->RigProx->Pitch(Angle);
        this->EntProx->Pitch(Angle);
    }

    void RigidDebris::Roll(const Real Angle)
    {
        this->RigProx->Roll(Angle);
        this->EntProx->Roll(Angle);
    }

    void RigidDebris::Rotate(const Vector3& Axis, const Real Angle)
    {
        this->RigProx->Rotate(Axis,Angle);
        this->EntProx->Rotate(Axis,Angle);
    }

    void RigidDebris::Rotate(const Quaternion& Rotation)
    {
        this->RigProx->Rotate(Rotation);
        this->EntProx->Rotate(Rotation);
    }

    void RigidDebris::Scale(const Vector3& Scale)
    {
        this->RigProx->Scale(Scale);
        this->EntProx->Scale(Scale);
    }

    void RigidDebris::Scale(const Real X, const Real Y, const Real Z)
    {
        this->RigProx->Scale(X,Y,Z);
        this->EntProx->Scale(X,Y,Z);
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Serialization

    void RigidDebris::ProtoSerializeProperties(XML::Node& SelfRoot) const
    {
        this->Debris::ProtoSerializeProperties(SelfRoot);
    }

    void RigidDebris::ProtoSerializeProxies(XML::Node& SelfRoot) const
    {
        // No base implementations to call
        XML::Node ProxiesNode = SelfRoot.AppendChild( RigidDebris::GetSerializableName() + "Proxies" );

        if( ProxiesNode.AppendAttribute("Version").SetValue("1") )
        {
            XML::Node EntProxNode = ProxiesNode.AppendChild("EntProx");
            this->EntProx->ProtoSerialize( EntProxNode );
            XML::Node RigProxNode = ProxiesNode.AppendChild("RigProx");
            this->RigProx->ProtoSerialize( RigProxNode );

            return;
        }else{
            SerializeError("Create XML Attribute Values",RigidDebris::GetSerializableName() + "Proxies",true);
        }
    }

    void RigidDebris::ProtoDeSerializeProperties(const XML::Node& SelfRoot)
    {
        this->Debris::ProtoDeSerializeProperties(SelfRoot);
    }

    void RigidDebris::ProtoDeSerializeProxies(const XML::Node& SelfRoot)
    {
        this->DestroyRigidDebris();
        // No base implementations to call
        //XML::Attribute CurrAttrib;
        XML::Node ProxiesNode = SelfRoot.GetChild( RigidDebris::GetSerializableName() + "Proxies" );

        if( !ProxiesNode.Empty() ) {
            if(ProxiesNode.GetAttribute("Version").AsInt() == 1) {
                /// @todo I don't think an exception is appropriate for the failure of the worldmanager validity checks,
                /// however a warning should be written to the log if that happens.  This should be updated to do that once
                /// logging refactors are done.

                XML::Node EntProxNode = ProxiesNode.GetChild("EntProx").GetFirstChild();
                if( !EntProxNode.Empty() ) {
                    Graphics::SceneManager* SceneMan = this->ParentWorld->GetSceneManager();
                    if( SceneMan ) {
                        this->EntProx = SceneMan->CreateEntityProxy( EntProxNode );
                        this->EntProx->_Bind( this );
                    }
                }

                XML::Node RigProxNode = ProxiesNode.GetChild("RigProx").GetFirstChild();
                if( !RigProxNode.Empty() ) {
                    Physics::PhysicsManager* PhysMan = this->ParentWorld->GetPhysicsManager();
                    if( PhysMan ) {
                        this->RigProx = PhysMan->CreateRigidProxy(RigProxNode);
                        this->RigProx->_Bind( this );
                    }
                }

                if( this->EntProx && this->RigProx ) {
                    this->RigProx->AddSyncObject( this->EntProx );
                }
            }else{
                MEZZ_EXCEPTION(Exception::INVALID_VERSION_EXCEPTION,"Incompatible XML Version for " + (RigidDebris::GetSerializableName() + "Proxies" ) + ": Not Version 1.");
            }
        }else{
            MEZZ_EXCEPTION(Exception::II_IDENTITY_NOT_FOUND_EXCEPTION,RigidDebris::GetSerializableName() + "Proxies" + " was not found in the provided XML node, which was expected.");
        }
    }

    String RigidDebris::GetDerivedSerializableName() const
        { return RigidDebris::GetSerializableName(); }

    String RigidDebris::GetSerializableName()
        { return "RigidDebris"; }

    ///////////////////////////////////////////////////////////////////////////////
    // Internal Methods

    void RigidDebris::_Update()
    {
        // Do nothing
    }

    void RigidDebris::_NotifyProxyDestroyed(WorldProxy* ToBeDestroyed)
    {
        if( ToBeDestroyed == NULL )
            return;

        if( this->EntProx == ToBeDestroyed ) {
            if( this->RigProx ) {
                this->RigProx->RemoveSyncObject( this->EntProx );
            }

            this->EntProx = NULL;
        }

        if( this->RigProx == ToBeDestroyed ) {
            this->RigProx = NULL;
        }
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
