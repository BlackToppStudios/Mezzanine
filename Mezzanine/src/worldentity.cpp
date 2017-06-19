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
#ifndef _worldentity_cpp
#define _worldentity_cpp

#include "worldentity.h"
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
    WorldEntity::WorldEntity(World* TheWorld) :
        PrimaryProxy(nullptr),
        ParentWorld(TheWorld)
        {  }

    WorldEntity::WorldEntity(const String& Name, World* TheWorld) :
        ObjectName(Name),
        PrimaryProxy(nullptr),
        ParentWorld(TheWorld)
        {  }

    WorldEntity::~WorldEntity()
        {  }

    void WorldEntity::DestroyAllComponents()
    {
        ProxyIterator ProxIt = this->Proxies.begin();
        while( ProxIt != this->Proxies.end() )
        {
            WorldProxyManager* ProxMan = (*ProxIt)->GetCreator();
            (*ProxIt)->_Bind(nullptr);
            ProxMan->DestroyProxy(*ProxIt);
            ++ProxIt;
        }
        this->Proxies.clear();
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Utility and Configuration

    const String& WorldEntity::GetName() const
        { return this->ObjectName; }

    World* WorldEntity::GetWorld() const
        { return this->ParentWorld; }

    Boole WorldEntity::IsInWorld() const
    {
        assert(this->PrimaryProxy != nullptr && "Valid proxy is needed to query if WorldEntity is in world.");
        return this->PrimaryProxy->IsInWorld();
    }

    Boole WorldEntity::IsStatic() const
    {
        assert(this->PrimaryProxy != nullptr && "Valid proxy is needed to query if WorldEntity is static.");
        return this->PrimaryProxy->IsStatic();
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Component Management

    void WorldEntity::AddComponent(WorldProxy* ToAdd)
    {
        ToAdd->_Bind( this );
        this->Proxies.push_back(ToAdd);
    }

    WorldProxy* WorldEntity::RemoveComponent(WorldProxy* ToRemove)
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
        return nullptr;
    }

    WorldEntity::ProxyContainer WorldEntity::RemoveAllComponentsOfType(const ComponentType Type)
    {
        ProxyContainer Ret;
        ProxyIterator ProxIt = this->Proxies.begin();
        while( ProxIt != this->Proxies.end() )
        {
            if( (*ProxIt)->GetComponentType() == Type ) {
                (*ProxIt)->_Bind( nullptr );
                Ret.push_back( (*ProxIt) );
                ProxIt = this->Proxies.erase(ProxIt);
            }else{
                ++ProxIt;
            }
        }
        return Ret;
    }

    WorldEntity::ProxyContainer WorldEntity::RemoveAllComponentsOfTypes(const ComponentType TypeFirst, const ComponentType TypeLast)
    {
        ProxyContainer Ret;
        ProxyIterator ProxIt = this->Proxies.begin();
        while( ProxIt != this->Proxies.end() )
        {
            if( (*ProxIt)->GetComponentType() >= TypeFirst && (*ProxIt)->GetComponentType() <= TypeLast ) {
                (*ProxIt)->_Bind( nullptr );
                Ret.push_back( (*ProxIt) );
                ProxIt = this->Proxies.erase(ProxIt);
            }else{
                ++ProxIt;
            }
        }
        return Ret;
    }

    WorldEntity::ProxyContainer WorldEntity::RemoveAllComponents()
    {
        ProxyContainer Ret;
        ProxyIterator ProxIt = this->Proxies.begin();
        while( ProxIt != this->Proxies.end() )
            { (*ProxIt)->_Bind( nullptr ); }
        Ret.swap(this->Proxies);
        return Ret;
    }

    Whole WorldEntity::GetNumComponents() const
    {
        return this->Proxies.size();
    }

    WorldProxy* WorldEntity::GetComponent(const Whole Index) const
    {
        return this->Proxies.at(Index);
    }

    WorldProxy* WorldEntity::GetComponent(const ComponentType Type, Whole TypeIndex) const
    {
        ConstProxyIterator ProxIt = this->Proxies.begin();
        while( ProxIt != this->Proxies.end() )
        {
            if( (*ProxIt)->GetComponentType() == Type ) {
                if( TypeIndex == 0 ) return (*ProxIt);
                else --TypeIndex;
            }
            ++ProxIt;
        }
        return nullptr;
    }

    WorldProxy* WorldEntity::GetComponent(const ComponentType TypeFirst, const ComponentType TypeLast, Whole TypeIndex) const
    {
        ConstProxyIterator ProxIt = this->Proxies.begin();
        while( ProxIt != this->Proxies.end() )
        {
            if( (*ProxIt)->GetComponentType() >= TypeFirst && (*ProxIt)->GetComponentType() <= TypeLast ) {
                if( TypeIndex == 0 ) return (*ProxIt);
                else --TypeIndex;
            }
            ++ProxIt;
        }
        return nullptr;
    }

    const WorldEntity::ProxyContainer& WorldEntity::GetComponents() const
    {
        return this->Proxies;
    }

    WorldEntity::ProxyContainer WorldEntity::GetComponents(const ComponentType Type) const
    {
        ProxyContainer Ret;
        ConstProxyIterator ProxIt = this->Proxies.begin();
        while( ProxIt != this->Proxies.end() )
        {
            if( (*ProxIt)->GetComponentType() == Type ) {
                Ret.push_back(*ProxIt);
            }
            ++ProxIt;
        }
        return Ret;
    }

    WorldEntity::ProxyContainer WorldEntity::GetComponents(const ComponentType TypeFirst, const ComponentType TypeLast) const
    {
        ProxyContainer Ret;
        ConstProxyIterator ProxIt = this->Proxies.begin();
        while( ProxIt != this->Proxies.end() )
        {
            if( (*ProxIt)->GetComponentType() >= TypeFirst && (*ProxIt)->GetComponentType() <= TypeLast ) {
                Ret.push_back(*ProxIt);
            }
            ++ProxIt;
        }
        return Ret;
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Proxy Management

    void WorldEntity::SetPrimaryProxy(WorldProxy* Primary)
    {
        assert(this == Primary->GetParentEntity() && "WorldProxies can only be made PrimaryProxy of a WorldEntity they are bound to.");
        this->PrimaryProxy = Primary;
    }

    WorldProxy* WorldEntity::GetPrimaryProxy() const
    {
        return this->PrimaryProxy;
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Working with the World

    void WorldEntity::AddToWorld()
    {
        for( ProxyIterator ProxIt = this->Proxies.begin() ; ProxIt != this->Proxies.end() ; ++ProxIt )
            { (*ProxIt)->AddToWorld(); }
    }

    void WorldEntity::RemoveFromWorld()
    {
        for( ProxyIterator ProxIt = this->Proxies.begin() ; ProxIt != this->Proxies.end() ; ++ProxIt )
            { (*ProxIt)->RemoveFromWorld(); }
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Transform Methods

    void WorldEntity::SetTransform(const Transform& Trans)
    {
        for( ProxyIterator ProxIt = this->Proxies.begin() ; ProxIt != this->Proxies.end() ; ++ProxIt )
            { (*ProxIt)->SetTransform(Trans); }
    }

    void WorldEntity::SetTransform(const Vector3& Loc, const Quaternion& Ori)
    {
        for( ProxyIterator ProxIt = this->Proxies.begin() ; ProxIt != this->Proxies.end() ; ++ProxIt )
            { (*ProxIt)->SetTransform(Loc,Ori); }
    }

    Transform WorldEntity::GetTransform() const
    {
        assert(this->PrimaryProxy != nullptr && "Valid proxy is needed to query WorldEntity transform.");
        return this->PrimaryProxy->GetTransform();
    }

    void WorldEntity::SetLocation(const Vector3& Loc)
    {
        for( ProxyIterator ProxIt = this->Proxies.begin() ; ProxIt != this->Proxies.end() ; ++ProxIt )
            { (*ProxIt)->SetLocation(Loc); }
    }

    void WorldEntity::SetLocation(const Real X, const Real Y, const Real Z)
    {
        for( ProxyIterator ProxIt = this->Proxies.begin() ; ProxIt != this->Proxies.end() ; ++ProxIt )
            { (*ProxIt)->SetLocation(X,Y,Z); }
    }

    Vector3 WorldEntity::GetLocation() const
    {
        assert(this->PrimaryProxy != nullptr && "Valid proxy is needed to query WorldEntity transform.");
        return this->PrimaryProxy->GetLocation();
    }

    void WorldEntity::SetOrientation(const Quaternion& Ori)
    {
        for( ProxyIterator ProxIt = this->Proxies.begin() ; ProxIt != this->Proxies.end() ; ++ProxIt )
            { (*ProxIt)->SetOrientation(Ori); }
    }

    void WorldEntity::SetOrientation(const Real X, const Real Y, const Real Z, const Real W)
    {
        for( ProxyIterator ProxIt = this->Proxies.begin() ; ProxIt != this->Proxies.end() ; ++ProxIt )
            { (*ProxIt)->SetOrientation(X,Y,Z,W); }
    }

    Quaternion WorldEntity::GetOrientation() const
    {
        assert(this->PrimaryProxy != nullptr && "Valid proxy is needed to query WorldEntity transform.");
        return this->PrimaryProxy->GetOrientation();
    }

    void WorldEntity::SetScale(const Vector3& Sc)
    {
        for( ProxyIterator ProxIt = this->Proxies.begin() ; ProxIt != this->Proxies.end() ; ++ProxIt )
            { (*ProxIt)->SetScale(Sc); }
    }

    void WorldEntity::SetScale(const Real X, const Real Y, const Real Z)
    {
        for( ProxyIterator ProxIt = this->Proxies.begin() ; ProxIt != this->Proxies.end() ; ++ProxIt )
            { (*ProxIt)->SetScale(X,Y,Z); }
    }

    Vector3 WorldEntity::GetScale() const
    {
        assert(this->PrimaryProxy != nullptr && "Valid proxy is needed to query WorldEntity transform.");
        return this->PrimaryProxy->GetScale();
    }

    void WorldEntity::Translate(const Vector3& Trans)
    {
        for( ProxyIterator ProxIt = this->Proxies.begin() ; ProxIt != this->Proxies.end() ; ++ProxIt )
            { (*ProxIt)->Translate(Trans); }
    }

    void WorldEntity::Translate(const Real X, const Real Y, const Real Z)
    {
        for( ProxyIterator ProxIt = this->Proxies.begin() ; ProxIt != this->Proxies.end() ; ++ProxIt )
            { (*ProxIt)->Translate(X,Y,Z); }
    }

    void WorldEntity::Yaw(const Real Angle)
    {
        for( ProxyIterator ProxIt = this->Proxies.begin() ; ProxIt != this->Proxies.end() ; ++ProxIt )
            { (*ProxIt)->Yaw(Angle); }
    }

    void WorldEntity::Pitch(const Real Angle)
    {
        for( ProxyIterator ProxIt = this->Proxies.begin() ; ProxIt != this->Proxies.end() ; ++ProxIt )
            { (*ProxIt)->Pitch(Angle); }
    }

    void WorldEntity::Roll(const Real Angle)
    {
        for( ProxyIterator ProxIt = this->Proxies.begin() ; ProxIt != this->Proxies.end() ; ++ProxIt )
            { (*ProxIt)->Roll(Angle); }
    }

    void WorldEntity::Rotate(const Vector3& Axis, const Real Angle)
    {
        for( ProxyIterator ProxIt = this->Proxies.begin() ; ProxIt != this->Proxies.end() ; ++ProxIt )
            { (*ProxIt)->Rotate(Axis,Angle); }
    }

    void WorldEntity::Rotate(const Quaternion& Rotation)
    {
        for( ProxyIterator ProxIt = this->Proxies.begin() ; ProxIt != this->Proxies.end() ; ++ProxIt )
            { (*ProxIt)->Rotate(Rotation); }
    }

    void WorldEntity::Scale(const Vector3& Sc)
    {
        for( ProxyIterator ProxIt = this->Proxies.begin() ; ProxIt != this->Proxies.end() ; ++ProxIt )
            { (*ProxIt)->Scale(Sc); }
    }

    void WorldEntity::Scale(const Real X, const Real Y, const Real Z)
    {
        for( ProxyIterator ProxIt = this->Proxies.begin() ; ProxIt != this->Proxies.end() ; ++ProxIt )
            { (*ProxIt)->Scale(X,Y,Z); }
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Serialization

    void WorldEntity::ProtoSerialize(XML::Node& ParentNode) const
    {
        XML::Node SelfRoot = ParentNode.AppendChild(this->GetDerivedSerializableName());

        this->ProtoSerializeProperties(SelfRoot);
        this->ProtoSerializeProxies(SelfRoot);
    }

    void WorldEntity::ProtoSerializeProperties(XML::Node& SelfRoot) const
    {
        XML::Node PropertiesNode = SelfRoot.AppendChild( WorldEntity::GetSerializableName() + "Properties" );

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
            SerializeError("Create XML Attribute Values",WorldEntity::GetSerializableName() + "Properties",true);
        }
    }

    void WorldEntity::ProtoSerializeProxies(XML::Node& SelfRoot) const
    {
        XML::Node ProxiesNode = SelfRoot.AppendChild( WorldEntity::GetSerializableName() + "Proxies" );

        if( ProxiesNode.AppendAttribute("Version").SetValue("1") &&
            ProxiesNode.AppendAttribute("ProxyCount").SetValue( this->Proxies.size() ) )
        {
            const ConstProxyIterator BegProx = this->Proxies.begin();
            for( ConstProxyIterator ProxIt = BegProx ; ProxIt != this->Proxies.end() ; ++ProxIt )
            {
                XML::Node ProxyNode = ProxiesNode.AppendChild( (*ProxIt)->GetDerivedSerializableName() );
                if( ProxyNode.AppendAttribute("ComponentID").SetValue( (*ProxIt)->GetComponentID() ) &&
                    ProxyNode.AppendAttribute("ManagerType").SetValue( (*ProxIt)->GetCreator()->GetInterfaceType() ) )
                {
                    continue;
                }else{
                    SerializeError("Create XML Attribute Values",WorldEntity::GetSerializableName() + "Proxies",true);
                }
            }

            return;
        }else{
            SerializeError("Create XML Attribute Values",WorldEntity::GetSerializableName() + "Proxies",true);
        }
    }

    void WorldEntity::ProtoDeSerialize(const XML::Node& SelfRoot)
    {
        this->ProtoDeSerializeProxies(SelfRoot);
        this->ProtoDeSerializeProperties(SelfRoot);
    }

    void WorldEntity::ProtoDeSerializeProperties(const XML::Node& SelfRoot)
    {
        XML::Attribute CurrAttrib;
        XML::Node PropertiesNode = SelfRoot.GetChild( WorldEntity::GetSerializableName() + "Properties" );

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
                MEZZ_EXCEPTION(ExceptionBase::INVALID_VERSION_EXCEPTION,"Incompatible XML Version for " + (WorldEntity::GetSerializableName() + "Properties" ) + ": Not Version 1.");
            }
        }else{
            MEZZ_EXCEPTION(ExceptionBase::II_IDENTITY_NOT_FOUND_EXCEPTION,WorldEntity::GetSerializableName() + "Properties" + " was not found in the provided XML node, which was expected.");
        }
    }

    void WorldEntity::ProtoDeSerializeProxies(const XML::Node& SelfRoot)
    {
        this->DestroyAllComponents();

        XML::Attribute CurrAttrib;
        XML::Node ProxiesNode = SelfRoot.GetChild( WorldEntity::GetSerializableName() + "Proxies" );

        if( !ProxiesNode.Empty() ) {
            if(ProxiesNode.GetAttribute("Version").AsInt() == 1) {
                //CurrAttrib = ProxiesNode.GetAttribute("ProxyCount");
                //if( !CurrAttrib.Empty() )
                //    this->Proxies.resize( CurrAttrib.AsUint() );

                for( XML::NodeIterator ProxyNodeIt = ProxiesNode.begin() ; ProxyNodeIt != ProxiesNode.end() ; ++ProxyNodeIt )
                {
                    WorldProxyManager* ProxMan = nullptr;
                    UInt32 ComponentID = 0;

                    CurrAttrib = (*ProxyNodeIt).GetAttribute("ComponentID");
                    if( !CurrAttrib.Empty() )
                        ComponentID = CurrAttrib.AsUint();

                    CurrAttrib = (*ProxyNodeIt).GetAttribute("ManagerType");
                    if( !CurrAttrib.Empty() )
                        ProxMan = this->ParentWorld->GetProxyManager( static_cast<ManagerBase::ManagerType>( CurrAttrib.AsUint() ) );

                    if( ComponentID != 0 && ProxMan != nullptr ) {
                        WorldProxy* ToAdd = ProxMan->GetProxyByID(ComponentID);
                        if( ToAdd != nullptr ) {
                            this->AddComponent( ToAdd );
                        }else{
                            MEZZ_EXCEPTION(ExceptionBase::II_IDENTITY_NOT_FOUND_EXCEPTION,"WorldProxy of the specified ID does not exist.");
                        }
                    }
                }

                return;
            }else{
                MEZZ_EXCEPTION(ExceptionBase::INVALID_VERSION_EXCEPTION,"Incompatible XML Version for " + (WorldEntity::GetSerializableName() + "Proxies" ) + ": Not Version 1.");
            }
        }else{
            MEZZ_EXCEPTION(ExceptionBase::II_IDENTITY_NOT_FOUND_EXCEPTION,WorldEntity::GetSerializableName() + "Proxies" + " was not found in the provided XML node, which was expected.");
        }
    }

    String WorldEntity::GetDerivedSerializableName() const
        { return WorldEntity::GetSerializableName(); }

    String WorldEntity::GetSerializableName()
        { return "WorldEntity"; }

    ///////////////////////////////////////////////////////////////////////////////
    // Internal Methods

    void WorldEntity::_SyncTransforms(WorldProxy* Exclude, const Transform& NewTrans)
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
