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
#include "world.h"
#include "exception.h"
#include "stringtool.h"
#include "serialization.h"

namespace Mezzanine
{
    ///////////////////////////////////////////////////////////////////////////////
    // RigidDebris Methods

    SoftDebris::SoftDebris(World* TheWorld) :
        Debris(TheWorld)
        {  }

    SoftDebris::SoftDebris(const String& Name, const Real Mass, World* TheWorld) :
        Debris(Name,TheWorld)
        { this->CreateSoftDebris(Mass); }

    SoftDebris::SoftDebris(const XML::Node& SelfRoot, World* TheWorld) :
        Debris(TheWorld)
        { this->ProtoDeSerialize(SelfRoot); }

    SoftDebris::~SoftDebris()
        { this->DestroySoftDebris(); }

    void SoftDebris::CreateSoftDebris(const Real Mass)
    {
        Physics::SoftProxy* SofProx = NULL;
        Physics::PhysicsManager* PhysMan = static_cast<Physics::PhysicsManager*>( this->ParentWorld->GetManager(ManagerBase::MT_PhysicsManager) );
        if( PhysMan ) {
            SofProx = PhysMan->CreateSoftProxy(Mass);
            this->AddProxy( SofProx );
            this->SetPrimaryProxy( SofProx );
        }

        Graphics::EntityProxy* EntProx = NULL;
        Graphics::SceneManager* SceneMan = static_cast<Graphics::SceneManager*>( this->ParentWorld->GetManager(ManagerBase::MT_SceneManager) );
        if( SceneMan ) {
            EntProx = SceneMan->CreateEntityProxy();
            this->AddProxy( EntProx );
        }
    }

    void SoftDebris::DestroySoftDebris()
    {
        this->RemoveFromWorld();
        this->DestroyAllProxies();
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Utility and Configuration

    WorldObjectType SoftDebris::GetType() const
        { return Mezzanine::WO_SoftDebris; }

    Graphics::EntityProxy* SoftDebris::GetEntityProxy() const
        { return static_cast<Graphics::EntityProxy*>( this->GetProxy(Mezzanine::PT_Graphics_EntityProxy,0) ); }

    Physics::SoftProxy* SoftDebris::GetSoftProxy() const
        { return static_cast<Physics::SoftProxy*>( this->GetProxy(Mezzanine::PT_Physics_SoftProxy,0) ); }

    ///////////////////////////////////////////////////////////////////////////////
    // Serialization

    void SoftDebris::ProtoSerializeProperties(XML::Node& SelfRoot) const
    {
        this->Debris::ProtoSerializeProperties(SelfRoot);
    }

    void SoftDebris::ProtoDeSerializeProperties(const XML::Node& SelfRoot)
    {
        this->Debris::ProtoDeSerializeProperties(SelfRoot);
    }

    String SoftDebris::GetDerivedSerializableName() const
        { return SoftDebris::GetSerializableName(); }

    String SoftDebris::GetSerializableName()
        { return "SoftDebris"; }

    ///////////////////////////////////////////////////////////////////////////////
    // Internal Methods

    void SoftDebris::_Update(const Whole Delta)
    {
        // Do nothing
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

    Debris* SoftDebrisFactory::CreateDebris(const String& Name, World* TheWorld, const NameValuePairMap& Params)
    {
        Real Mass = 0;
        NameValuePairMap::const_iterator ParamIt = Params.find( "Mass" );
        if( ParamIt != Params.end() )
            Mass = StringTools::ConvertToReal( (*ParamIt).second );

        return new SoftDebris(Name,Mass,TheWorld);
    }

    Debris* SoftDebrisFactory::CreateDebris(const XML::Node& XMLNode, World* TheWorld)
        { return new SoftDebris(XMLNode,TheWorld); }

    void SoftDebrisFactory::DestroyDebris(Debris* ToBeDestroyed)
        { delete ToBeDestroyed; }
}//Mezzanine

#endif
