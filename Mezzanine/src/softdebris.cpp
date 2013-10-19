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
#ifndef _softdebris_cpp
#define _softdebris_cpp

/// @file
/// @brief This file contains the implementation for the debris class that will compress and deform.

#include "softdebris.h"

#include "Graphics/scenemanager.h"
#include "Graphics/entityproxy.h"

#include "Physics/physicsmanager.h"
#include "Physics/softproxy.h"

#include "entresol.h"
#include "exception.h"
#include "stringtool.h"
#include "serialization.h"

namespace Mezzanine
{
    ///////////////////////////////////////////////////////////////////////////////
    // RigidDebris Methods

    SoftDebris::SoftDebris(World* TheWorld) :
        Debris(TheWorld),
        EntProx(NULL),
        SofProx(NULL)
        {  }

    SoftDebris::SoftDebris(const String& Name, const Real Mass, World* TheWorld) :
        Debris(Name,TheWorld),
        EntProx(NULL),
        SofProx(NULL)
        { this->CreateSoftDebris(Mass); }

    SoftDebris::SoftDebris(const XML::Node& SelfRoot, World* TheWorld) :
        Debris(TheWorld),
        EntProx(NULL),
        SofProx(NULL)
        { this->ProtoDeSerialize(SelfRoot); }

    SoftDebris::~SoftDebris()
        { this->DestroySoftDebris(); }

    void SoftDebris::CreateSoftDebris(const Real Mass)
    {
        Graphics::SceneManager* SceneMan = Entresol::GetSingletonPtr()->GetSceneManager();
        if( SceneMan ) {
            this->EntProx = SceneMan->CreateEntityProxy();
            this->EntProx->_Bind( this );
        }

        Physics::PhysicsManager* PhysMan = Entresol::GetSingletonPtr()->GetPhysicsManager();
        if( PhysMan ) {
            this->SofProx = PhysMan->CreateSoftProxy(Mass);
            this->SofProx->_Bind( this );
        }
    }

    void SoftDebris::DestroySoftDebris()
    {
        this->RemoveFromWorld();
        if( this->EntProx ) {
            Graphics::SceneManager* SceneMan = Entresol::GetSingletonPtr()->GetSceneManager();
            if( SceneMan ) {
                SceneMan->DestroyProxy( this->EntProx );
                this->EntProx = NULL;
            }
        }

        if( this->SofProx ) {
            Physics::PhysicsManager* PhysMan = Entresol::GetSingletonPtr()->GetPhysicsManager();
            if( PhysMan ) {
                PhysMan->DestroyProxy( this->SofProx );
                this->SofProx = NULL;
            }
        }
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Utility and Configuration

    WorldObjectType SoftDebris::GetType() const
        { return Mezzanine::WO_DebrisSoft; }

    Graphics::EntityProxy* SoftDebris::GetEntityProxy() const
        { return this->EntProx; }

    Physics::SoftProxy* SoftDebris::GetSoftProxy() const
        { return this->SofProx; }

    Bool SoftDebris::IsInWorld() const
        { return this->SofProx->IsInWorld(); }

    Bool SoftDebris::IsStatic() const
        { return this->SofProx->IsStatic(); }

    Bool SoftDebris::IsKinematic() const
        { return this->SofProx->IsKinematic(); }

    void SoftDebris::GetProxies(ProxyContainer& Proxies)
    {
        Proxies.push_back( this->EntProx );
        Proxies.push_back( this->SofProx );
    }

    void SoftDebris::GetProxies(const UInt32 Types, ProxyContainer& Proxies)
    {
        if( Types & Mezzanine::PT_Graphics_EntityProxy ) {
            Proxies.push_back( this->EntProx );
        }
        if( Types & Mezzanine::PT_Physics_SoftProxy ) {
            Proxies.push_back( this->SofProx );
        }
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Working with the World

    void SoftDebris::AddToWorld()
    {
        if( this->EntProx )
            this->EntProx->AddToWorld();

        if( this->SofProx )
            this->SofProx->AddToWorld();
    }

    void SoftDebris::RemoveFromWorld()
    {
        if( this->EntProx )
            this->EntProx->RemoveFromWorld();

        if( this->SofProx )
            this->SofProx->RemoveFromWorld();
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Transform Methods

    void SoftDebris::SetLocation(const Vector3& Loc)
    {
        this->SofProx->SetLocation(Loc);
        this->EntProx->SetLocation(Loc);
    }

    void SoftDebris::SetLocation(const Real X, const Real Y, const Real Z)
    {
        this->SofProx->SetLocation(X,Y,Z);
        this->EntProx->SetLocation(X,Y,Z);
    }

    Vector3 SoftDebris::GetLocation() const
    {
        return this->SofProx->GetLocation();
    }

    void SoftDebris::SetOrientation(const Quaternion& Ori)
    {
        this->SofProx->SetOrientation(Ori);
        this->EntProx->SetOrientation(Ori);
    }

    void SoftDebris::SetOrientation(const Real X, const Real Y, const Real Z, const Real W)
    {
        this->SofProx->SetOrientation(X,Y,Z,W);
        this->EntProx->SetOrientation(X,Y,Z,W);
    }

    Quaternion SoftDebris::GetOrientation() const
    {
        return this->SofProx->GetOrientation();
    }

    void SoftDebris::SetScale(const Vector3& Sc)
    {
        this->SofProx->SetScale(Sc);
        this->EntProx->SetScale(Sc);
    }

    void SoftDebris::SetScale(const Real X, const Real Y, const Real Z)
    {
        this->SofProx->SetScale(X,Y,Z);
        this->EntProx->SetScale(X,Y,Z);
    }

    Vector3 SoftDebris::GetScale() const
    {
        return this->SofProx->GetScale();
    }

    void SoftDebris::Translate(const Vector3& Trans)
    {
        this->SofProx->Translate(Trans);
        this->EntProx->Translate(Trans);
    }

    void SoftDebris::Translate(const Real X, const Real Y, const Real Z)
    {
        this->SofProx->Translate(X,Y,Z);
        this->EntProx->Translate(X,Y,Z);
    }

    void SoftDebris::Yaw(const Real Angle)
    {
        this->SofProx->Yaw(Angle);
        this->EntProx->Yaw(Angle);
    }

    void SoftDebris::Pitch(const Real Angle)
    {
        this->SofProx->Pitch(Angle);
        this->EntProx->Pitch(Angle);
    }

    void SoftDebris::Roll(const Real Angle)
    {
        this->SofProx->Roll(Angle);
        this->EntProx->Roll(Angle);
    }

    void SoftDebris::Rotate(const Vector3& Axis, const Real Angle)
    {
        this->SofProx->Rotate(Axis,Angle);
        this->EntProx->Rotate(Axis,Angle);
    }

    void SoftDebris::Rotate(const Quaternion& Rotation)
    {
        this->SofProx->Rotate(Rotation);
        this->EntProx->Rotate(Rotation);
    }

    void SoftDebris::Scale(const Vector3& Scale)
    {
        this->SofProx->Scale(Scale);
        this->EntProx->Scale(Scale);
    }

    void SoftDebris::Scale(const Real X, const Real Y, const Real Z)
    {
        this->SofProx->Scale(X,Y,Z);
        this->EntProx->Scale(X,Y,Z);
    }


    ///////////////////////////////////////////////////////////////////////////////
    // Serialization

    void SoftDebris::ProtoSerializeProperties(XML::Node& SelfRoot) const
    {
        this->Debris::ProtoSerializeProperties(SelfRoot);
    }

    void SoftDebris::ProtoSerializeProxies(XML::Node& SelfRoot) const
    {
        // No base implementations to call
        XML::Node ProxiesNode = SelfRoot.AppendChild( SoftDebris::GetSerializableName() + "Proxies" );

        if( ProxiesNode.AppendAttribute("Version").SetValue("1") )
        {
            XML::Node EntProxNode = ProxiesNode.AppendChild("EntProx");
            this->EntProx->ProtoSerialize( EntProxNode );
            XML::Node SofProxNode = ProxiesNode.AppendChild("SofProx");
            this->SofProx->ProtoSerialize( SofProxNode );

            return;
        }else{
            SerializeError("Create XML Attribute Values",SoftDebris::GetSerializableName() + "Proxies",true);
        }
    }

    void SoftDebris::ProtoDeSerializeProperties(const XML::Node& SelfRoot)
    {
        this->Debris::ProtoDeSerializeProperties(SelfRoot);
    }

    void SoftDebris::ProtoDeSerializeProxies(const XML::Node& SelfRoot)
    {
        this->DestroySoftDebris();
        // No base implementations to call
        //XML::Attribute CurrAttrib;
        XML::Node ProxiesNode = SelfRoot.GetChild( SoftDebris::GetSerializableName() + "Proxies" );

        if( !ProxiesNode.Empty() ) {
            if(ProxiesNode.GetAttribute("Version").AsInt() == 1) {
                /// @todo I don't think an exception is appropriate for the failure of the worldmanager validity checks,
                /// however a warning should be written to the log if that happens.  This should be updated to do that once
                /// logging refactors are done.

                XML::Node EntProxNode = ProxiesNode.GetChild("EntProx").GetFirstChild();
                if( !EntProxNode.Empty() ) {
                    Graphics::SceneManager* SceneMan = Entresol::GetSingletonPtr()->GetSceneManager();
                    if( SceneMan ) {
                        this->EntProx = SceneMan->CreateEntityProxy( EntProxNode );
                        this->EntProx->_Bind( this );
                    }
                }

                XML::Node SofProxNode = ProxiesNode.GetChild("SofProx").GetFirstChild();
                if( !SofProxNode.Empty() ) {
                    Physics::PhysicsManager* PhysMan = Entresol::GetSingletonPtr()->GetPhysicsManager();
                    if( PhysMan ) {
                        this->SofProx = PhysMan->CreateSoftProxy(SofProxNode);
                        this->SofProx->_Bind( this );
                    }
                }
            }else{
                MEZZ_EXCEPTION(Exception::INVALID_VERSION_EXCEPTION,"Incompatible XML Version for " + (SoftDebris::GetSerializableName() + "Proxies" ) + ": Not Version 1.");
            }
        }else{
            MEZZ_EXCEPTION(Exception::II_IDENTITY_NOT_FOUND_EXCEPTION,SoftDebris::GetSerializableName() + "Proxies" + " was not found in the provided XML node, which was expected.");
        }
    }

    String SoftDebris::GetDerivedSerializableName() const
        { return SoftDebris::GetSerializableName(); }

    String SoftDebris::GetSerializableName()
        { return "SoftDebris"; }

    ///////////////////////////////////////////////////////////////////////////////
    // Internal Methods

    void SoftDebris::_Update()
    {
        // Do nothing
    }

    void SoftDebris::_NotifyProxyDestroyed(WorldProxy* ToBeDestroyed)
    {
        if( ToBeDestroyed == NULL )
            return;

        if( this->EntProx == ToBeDestroyed ) {
            this->EntProx = NULL;
        }

        if( this->SofProx == ToBeDestroyed ) {
            this->SofProx = NULL;
        }
    }

    ///////////////////////////////////////////////////////////////////////////////
    // RigidDebrisFactory Methods

    SoftDebrisFactory::SoftDebrisFactory()
        {  }

    SoftDebrisFactory::~SoftDebrisFactory()
        {  }

    String SoftDebrisFactory::GetTypeName() const
        { return SoftDebris::GetSerializableName(); }

    SoftDebris* SoftDebrisFactory::CreateSoftDebris(const String& Name, const Real Mass, World* TheWorld)
        { return new SoftDebris(Name,Mass,TheWorld); }

    SoftDebris* SoftDebrisFactory::CreateSoftDebris(const XML::Node& XMLNode, World* TheWorld)
        { return static_cast<SoftDebris*>( this->CreateDebris(XMLNode,TheWorld) ); }

    Debris* SoftDebrisFactory::CreateDebris(const String& Name, World* TheWorld, NameValuePairList& Params)
    {
        Real Mass = 0;
        for( NameValuePairList::iterator ValIt = Params.begin() ; ValIt != Params.end() ; ++ValIt )
        {
            if( (*ValIt).first == "Mass" )
                Mass = StringTools::ConvertToReal( (*ValIt).second );
        }
        return new SoftDebris(Name,Mass,TheWorld);
    }

    Debris* SoftDebrisFactory::CreateDebris(const XML::Node& XMLNode, World* TheWorld)
        { return new SoftDebris(XMLNode,TheWorld); }

    void SoftDebrisFactory::DestroyDebris(Debris* ToBeDestroyed)
        { delete ToBeDestroyed; }
}//Mezzanine

#endif
