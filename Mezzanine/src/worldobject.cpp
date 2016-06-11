// © Copyright 2010 - 2016 BlackTopp Studios Inc.
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
#ifndef _worldobject_cpp
#define _worldobject_cpp

#include "worldobject.h"
#include "world.h"
#include "worldproxy.h"
#include "worldproxymanager.h"
#include "serialization.h"
#include "exception.h"
#include "entresol.h"

#include "Physics/collision.h"

#include <assert.h>

namespace Mezzanine
{
    WorldObject::WorldObject(World* TheWorld) :
        PrimaryProxy(NULL),
        ParentWorld(TheWorld)
        {  }

    WorldObject::WorldObject(const String& Name, World* TheWorld) :
        ObjectName(Name),
        PrimaryProxy(NULL),
        ParentWorld(TheWorld)
        {  }

    WorldObject::~WorldObject()
        {  }

    void WorldObject::DestroyAllProxies()
    {
        ProxyIterator ProxIt = this->Proxies.begin();
        while( ProxIt != this->Proxies.end() )
        {
            WorldProxyManager* ProxMan = (*ProxIt)->GetCreator();
            (*ProxIt)->_Bind(NULL);
            ProxMan->DestroyProxy(*ProxIt);
            ++ProxIt;
        }
        this->Proxies.clear();
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Utility and Configuration

    const String& WorldObject::GetName() const
        { return this->ObjectName; }

    World* WorldObject::GetWorld() const
        { return this->ParentWorld; }

    Boole WorldObject::IsInWorld() const
    {
        assert(this->PrimaryProxy != NULL && "Valid proxy is needed to query if WorldObject is in world.");
        return this->PrimaryProxy->IsInWorld();
    }

    Boole WorldObject::IsStatic() const
    {
        assert(this->PrimaryProxy != NULL && "Valid proxy is needed to query if WorldObject is static.");
        return this->PrimaryProxy->IsStatic();
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Proxy Management

    void WorldObject::AddProxy(WorldProxy* ToAdd)
    {
        ToAdd->_Bind( this );
        this->Proxies.push_back(ToAdd);
    }

    WorldProxy* WorldObject::RemoveProxy(WorldProxy* ToRemove)
    {
        ProxyIterator ProxIt = this->Proxies.begin();
        while( ProxIt != this->Proxies.end() )
        {
            if( (*ProxIt) == ToRemove ) {
                this->Proxies.erase(ProxIt);
                return ToRemove;
            }
            ++ProxIt;
        }
        return NULL;
    }

    WorldObject::ProxyContainer WorldObject::RemoveAllProxiesOfType(const UInt32 Types)
    {
        ProxyContainer Ret;
        ProxyIterator ProxIt = this->Proxies.begin();
        while( ProxIt != this->Proxies.end() )
        {
            if( (*ProxIt)->GetProxyType() & Types ) {
                (*ProxIt)->_Bind( NULL );
                Ret.push_back( (*ProxIt) );
                ProxIt = this->Proxies.erase(ProxIt);
            }else{
                ++ProxIt;
            }
        }
        return Ret;
    }

    WorldObject::ProxyContainer WorldObject::RemoveAllProxies()
    {
        ProxyContainer Ret;
        ProxyIterator ProxIt = this->Proxies.begin();
        while( ProxIt != this->Proxies.end() )
            { (*ProxIt)->_Bind( NULL ); }
        Ret.swap(this->Proxies);
        return Ret;
    }

    Whole WorldObject::GetNumProxies() const
    {
        return this->Proxies.size();
    }

    WorldProxy* WorldObject::GetProxy(const Whole Index) const
    {
        return this->Proxies.at(Index);
    }

    WorldProxy* WorldObject::GetProxy(const UInt32 Types, Whole TypeIndex) const
    {
        ConstProxyIterator ProxIt = this->Proxies.begin();
        while( ProxIt != this->Proxies.end() )
        {
            if( (*ProxIt)->GetProxyType() & Types ) {
                if( TypeIndex == 0 ) return (*ProxIt);
                else --TypeIndex;
            }
            ++ProxIt;
        }
        return NULL;
    }

    const WorldObject::ProxyContainer& WorldObject::GetProxies() const
    {
        return this->Proxies;
    }

    WorldObject::ProxyContainer WorldObject::GetProxies(const UInt32 Types) const
    {
        ProxyContainer Ret;
        ConstProxyIterator ProxIt = this->Proxies.begin();
        while( ProxIt != this->Proxies.end() )
        {
            if( (*ProxIt)->GetProxyType() & Types ) {
                Ret.push_back(*ProxIt);
            }
            ++ProxIt;
        }
        return Ret;
    }

    void WorldObject::SetPrimaryProxy(WorldProxy* Primary)
    {
        assert(this == Primary->GetParentObject() && "WorldProxies can only be made PrimaryProxy of a WorldObject they are bound to.");
        this->PrimaryProxy = Primary;
    }

    WorldProxy* WorldObject::GetPrimaryProxy() const
    {
        return this->PrimaryProxy;
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Working with the World

    void WorldObject::AddToWorld()
    {
        for( ProxyIterator ProxIt = this->Proxies.begin() ; ProxIt != this->Proxies.end() ; ++ProxIt )
            { (*ProxIt)->AddToWorld(); }
    }

    void WorldObject::RemoveFromWorld()
    {
        for( ProxyIterator ProxIt = this->Proxies.begin() ; ProxIt != this->Proxies.end() ; ++ProxIt )
            { (*ProxIt)->RemoveFromWorld(); }
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Transform Methods

    void WorldObject::SetTransform(const Transform& Trans)
    {
        for( ProxyIterator ProxIt = this->Proxies.begin() ; ProxIt != this->Proxies.end() ; ++ProxIt )
            { (*ProxIt)->SetTransform(Trans); }
    }

    void WorldObject::SetTransform(const Vector3& Loc, const Quaternion& Ori)
    {
        for( ProxyIterator ProxIt = this->Proxies.begin() ; ProxIt != this->Proxies.end() ; ++ProxIt )
            { (*ProxIt)->SetTransform(Loc,Ori); }
    }

    Transform WorldObject::GetTransform() const
    {
        assert(this->PrimaryProxy != NULL && "Valid proxy is needed to query WorldObject transform.");
        return this->PrimaryProxy->GetTransform();
    }

    void WorldObject::SetLocation(const Vector3& Loc)
    {
        for( ProxyIterator ProxIt = this->Proxies.begin() ; ProxIt != this->Proxies.end() ; ++ProxIt )
            { (*ProxIt)->SetLocation(Loc); }
    }

    void WorldObject::SetLocation(const Real X, const Real Y, const Real Z)
    {
        for( ProxyIterator ProxIt = this->Proxies.begin() ; ProxIt != this->Proxies.end() ; ++ProxIt )
            { (*ProxIt)->SetLocation(X,Y,Z); }
    }

    Vector3 WorldObject::GetLocation() const
    {
        assert(this->PrimaryProxy != NULL && "Valid proxy is needed to query WorldObject transform.");
        return this->PrimaryProxy->GetLocation();
    }

    void WorldObject::SetOrientation(const Quaternion& Ori)
    {
        for( ProxyIterator ProxIt = this->Proxies.begin() ; ProxIt != this->Proxies.end() ; ++ProxIt )
            { (*ProxIt)->SetOrientation(Ori); }
    }

    void WorldObject::SetOrientation(const Real X, const Real Y, const Real Z, const Real W)
    {
        for( ProxyIterator ProxIt = this->Proxies.begin() ; ProxIt != this->Proxies.end() ; ++ProxIt )
            { (*ProxIt)->SetOrientation(X,Y,Z,W); }
    }

    Quaternion WorldObject::GetOrientation() const
    {
        assert(this->PrimaryProxy != NULL && "Valid proxy is needed to query WorldObject transform.");
        return this->PrimaryProxy->GetOrientation();
    }

    void WorldObject::SetScale(const Vector3& Sc)
    {
        for( ProxyIterator ProxIt = this->Proxies.begin() ; ProxIt != this->Proxies.end() ; ++ProxIt )
            { (*ProxIt)->SetScale(Sc); }
    }

    void WorldObject::SetScale(const Real X, const Real Y, const Real Z)
    {
        for( ProxyIterator ProxIt = this->Proxies.begin() ; ProxIt != this->Proxies.end() ; ++ProxIt )
            { (*ProxIt)->SetScale(X,Y,Z); }
    }

    Vector3 WorldObject::GetScale() const
    {
        assert(this->PrimaryProxy != NULL && "Valid proxy is needed to query WorldObject transform.");
        return this->PrimaryProxy->GetScale();
    }

    void WorldObject::Translate(const Vector3& Trans)
    {
        for( ProxyIterator ProxIt = this->Proxies.begin() ; ProxIt != this->Proxies.end() ; ++ProxIt )
            { (*ProxIt)->Translate(Trans); }
    }

    void WorldObject::Translate(const Real X, const Real Y, const Real Z)
    {
        for( ProxyIterator ProxIt = this->Proxies.begin() ; ProxIt != this->Proxies.end() ; ++ProxIt )
            { (*ProxIt)->Translate(X,Y,Z); }
    }

    void WorldObject::Yaw(const Real Angle)
    {
        for( ProxyIterator ProxIt = this->Proxies.begin() ; ProxIt != this->Proxies.end() ; ++ProxIt )
            { (*ProxIt)->Yaw(Angle); }
    }

    void WorldObject::Pitch(const Real Angle)
    {
        for( ProxyIterator ProxIt = this->Proxies.begin() ; ProxIt != this->Proxies.end() ; ++ProxIt )
            { (*ProxIt)->Pitch(Angle); }
    }

    void WorldObject::Roll(const Real Angle)
    {
        for( ProxyIterator ProxIt = this->Proxies.begin() ; ProxIt != this->Proxies.end() ; ++ProxIt )
            { (*ProxIt)->Roll(Angle); }
    }

    void WorldObject::Rotate(const Vector3& Axis, const Real Angle)
    {
        for( ProxyIterator ProxIt = this->Proxies.begin() ; ProxIt != this->Proxies.end() ; ++ProxIt )
            { (*ProxIt)->Rotate(Axis,Angle); }
    }

    void WorldObject::Rotate(const Quaternion& Rotation)
    {
        for( ProxyIterator ProxIt = this->Proxies.begin() ; ProxIt != this->Proxies.end() ; ++ProxIt )
            { (*ProxIt)->Rotate(Rotation); }
    }

    void WorldObject::Scale(const Vector3& Sc)
    {
        for( ProxyIterator ProxIt = this->Proxies.begin() ; ProxIt != this->Proxies.end() ; ++ProxIt )
            { (*ProxIt)->Scale(Sc); }
    }

    void WorldObject::Scale(const Real X, const Real Y, const Real Z)
    {
        for( ProxyIterator ProxIt = this->Proxies.begin() ; ProxIt != this->Proxies.end() ; ++ProxIt )
            { (*ProxIt)->Scale(X,Y,Z); }
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Serialization

    void WorldObject::ProtoSerialize(XML::Node& ParentNode) const
    {
        XML::Node SelfRoot = ParentNode.AppendChild(this->GetDerivedSerializableName());

        this->ProtoSerializeProperties(SelfRoot);
        this->ProtoSerializeProxies(SelfRoot);
    }

    void WorldObject::ProtoSerializeProperties(XML::Node& SelfRoot) const
    {
        XML::Node PropertiesNode = SelfRoot.AppendChild( WorldObject::GetSerializableName() + "Properties" );

        if( PropertiesNode.AppendAttribute("Version").SetValue("1") &&
            PropertiesNode.AppendAttribute("Name").SetValue( this->GetName() ) )
        {
            /// @todo Is serializing transform data necessary?  Proxies have their own copy.
            XML::Node LocationNode = PropertiesNode.AppendChild("Location");
            this->GetLocation().ProtoSerialize( LocationNode );
            XML::Node OrientationNode = PropertiesNode.AppendChild("Orientation");
            this->GetOrientation().ProtoSerialize( OrientationNode );
            XML::Node ScaleNode = PropertiesNode.AppendChild("Scale");
            this->GetScale().ProtoSerialize( ScaleNode );

            return;
        }else{
            SerializeError("Create XML Attribute Values",WorldObject::GetSerializableName() + "Properties",true);
        }
    }

    void WorldObject::ProtoSerializeProxies(XML::Node& SelfRoot) const
    {
        XML::Node ProxiesNode = SelfRoot.AppendChild( WorldObject::GetSerializableName() + "Proxies" );

        if( ProxiesNode.AppendAttribute("Version").SetValue("1") &&
            ProxiesNode.AppendAttribute("ProxyCount").SetValue( this->Proxies.size() ) )
        {
            const ConstProxyIterator BegProx = this->Proxies.begin();
            for( ConstProxyIterator ProxIt = BegProx ; ProxIt != this->Proxies.end() ; ++ProxIt )
            {
                XML::Node ProxyNode = ProxiesNode.AppendChild( (*ProxIt)->GetDerivedSerializableName() );
                if( ProxyNode.AppendAttribute("ProxyID").SetValue( (*ProxIt)->GetProxyID() ) &&
                    ProxyNode.AppendAttribute("ManagerType").SetValue( (*ProxIt)->GetCreator()->GetInterfaceType() ) )
                {
                    continue;
                }else{
                    SerializeError("Create XML Attribute Values",WorldObject::GetSerializableName() + "Proxies",true);
                }
            }

            return;
        }else{
            SerializeError("Create XML Attribute Values",WorldObject::GetSerializableName() + "Proxies",true);
        }
    }

    void WorldObject::ProtoDeSerialize(const XML::Node& SelfRoot)
    {
        this->ProtoDeSerializeProxies(SelfRoot);
        this->ProtoDeSerializeProperties(SelfRoot);
    }

    void WorldObject::ProtoDeSerializeProperties(const XML::Node& SelfRoot)
    {
        XML::Attribute CurrAttrib;
        XML::Node PropertiesNode = SelfRoot.GetChild( WorldObject::GetSerializableName() + "Properties" );

        if( !PropertiesNode.Empty() ) {
            if(PropertiesNode.GetAttribute("Version").AsInt() == 1) {
                CurrAttrib = PropertiesNode.GetAttribute("Name");
                if( !CurrAttrib.Empty() )
                    this->ObjectName = CurrAttrib.AsString();

                XML::Node PositionNode = PropertiesNode.GetChild("Location").GetFirstChild();
                if( !PositionNode.Empty() ) {
                    Vector3 Loc(PositionNode);
                    this->SetLocation(Loc);
                }

                XML::Node OrientationNode = PropertiesNode.GetChild("Orientation").GetFirstChild();
                if( !OrientationNode.Empty() ) {
                    Quaternion Rot(OrientationNode);
                    this->SetOrientation(Rot);
                }

                XML::Node ScaleNode = PropertiesNode.GetChild("Scale").GetFirstChild();
                if( !ScaleNode.Empty() ) {
                    Vector3 Scale(ScaleNode);
                    this->SetScale(Scale);
                }
            }else{
                MEZZ_EXCEPTION(ExceptionBase::INVALID_VERSION_EXCEPTION,"Incompatible XML Version for " + (WorldObject::GetSerializableName() + "Properties" ) + ": Not Version 1.");
            }
        }else{
            MEZZ_EXCEPTION(ExceptionBase::II_IDENTITY_NOT_FOUND_EXCEPTION,WorldObject::GetSerializableName() + "Properties" + " was not found in the provided XML node, which was expected.");
        }
    }

    void WorldObject::ProtoDeSerializeProxies(const XML::Node& SelfRoot)
    {
        this->DestroyAllProxies();

        XML::Attribute CurrAttrib;
        XML::Node ProxiesNode = SelfRoot.GetChild( WorldObject::GetSerializableName() + "Proxies" );

        if( !ProxiesNode.Empty() ) {
            if(ProxiesNode.GetAttribute("Version").AsInt() == 1) {
                //CurrAttrib = ProxiesNode.GetAttribute("ProxyCount");
                //if( !CurrAttrib.Empty() )
                //    this->Proxies.resize( CurrAttrib.AsUint() );

                for( XML::NodeIterator ProxyNodeIt = ProxiesNode.begin() ; ProxyNodeIt != ProxiesNode.end() ; ++ProxyNodeIt )
                {
                    WorldProxyManager* ProxMan = NULL;
                    UInt32 ProxyID = 0;

                    CurrAttrib = (*ProxyNodeIt).GetAttribute("ProxyID");
                    if( !CurrAttrib.Empty() )
                        ProxyID = CurrAttrib.AsUint();

                    CurrAttrib = (*ProxyNodeIt).GetAttribute("ManagerType");
                    if( !CurrAttrib.Empty() )
                        ProxMan = this->ParentWorld->GetProxyManager( static_cast<ManagerBase::ManagerType>( CurrAttrib.AsUint() ) );

                    if( ProxyID != 0 && ProxMan != NULL ) {
                        WorldProxy* ToAdd = ProxMan->GetProxyByID(ProxyID);
                        if( ToAdd != NULL ) {
                            this->AddProxy( ToAdd );
                        }else{
                            MEZZ_EXCEPTION(ExceptionBase::II_IDENTITY_NOT_FOUND_EXCEPTION,"WorldProxy of the specified ID does not exist.");
                        }
                    }
                }

                return;
            }else{
                MEZZ_EXCEPTION(ExceptionBase::INVALID_VERSION_EXCEPTION,"Incompatible XML Version for " + (WorldObject::GetSerializableName() + "Proxies" ) + ": Not Version 1.");
            }
        }else{
            MEZZ_EXCEPTION(ExceptionBase::II_IDENTITY_NOT_FOUND_EXCEPTION,WorldObject::GetSerializableName() + "Proxies" + " was not found in the provided XML node, which was expected.");
        }
    }

    String WorldObject::GetDerivedSerializableName() const
        { return WorldObject::GetSerializableName(); }

    String WorldObject::GetSerializableName()
        { return "WorldObject"; }

    ///////////////////////////////////////////////////////////////////////////////
    // Internal Methods

    void WorldObject::_SyncTransforms(WorldProxy* Exclude, const Transform& NewTrans)
    {
        for( ProxyIterator ProxIt = this->Proxies.begin() ; ProxIt != this->Proxies.end() ; ++ProxIt )
        {
            if( (*ProxIt) != Exclude ) {
                (*ProxIt)->SetTransform(NewTrans);
            }
        }
    }
}//Mezzanine

#endif
