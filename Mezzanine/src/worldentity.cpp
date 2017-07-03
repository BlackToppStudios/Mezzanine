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
#include "entitycomponentmanager.h"
#include "serialization.h"
#include "exception.h"
#include "entresol.h"

#include "Physics/collision.h"

#include <assert.h>

namespace Mezzanine
{
    Boole ComponentCompare(WorldEntityComponent* CompFirst, WorldEntityComponent* CompSecond)
    {
        /// @todo This is placeholder for now.
        return CompFirst->GetComponentType() < CompSecond->GetComponentType();
    }

    ///////////////////////////////////////////////////////////////////////////////
    // WorldEntity

    WorldEntity::WorldEntity(World* TheWorld) :
        PrimaryProxy(nullptr),
        ParentWorld(TheWorld),
        ProxyStartIndex(0)
        {  }

    WorldEntity::WorldEntity(const String& Name, World* TheWorld) :
        ObjectName(Name),
        PrimaryProxy(nullptr),
        ParentWorld(TheWorld),
        ProxyStartIndex(0)
        {  }

    void WorldEntity::DestroyAllComponents()
    {
        ComponentIterator CompIt = this->Components.begin();
        while( CompIt != this->Components.end() )
        {
            EntityComponentManager* ComponentMan = (*CompIt)->GetCreator();
            (*CompIt)->_Bind(nullptr);
            ComponentMan->DestroyComponent(*CompIt);
            ++CompIt;
        }
        this->Components.clear();
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Utility and Configuration

    Mezzanine::WorldEntityType WorldEntity::GetType() const
        { return Mezzanine::WE_Generic; }

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

    void WorldEntity::AddComponent(WorldEntityComponent* ToAdd)
    {
        ToAdd->_Bind( this );
        ComponentIterator CompIt = std::lower_bound(this->Components.begin(),this->Components.end(),ToAdd,ComponentCompare);
        this->Components.insert(CompIt,ToAdd);
        if( ToAdd->GetComponentType() < Mezzanine::CT_ProxyFirst ) {
            ++(this->ProxyStartIndex);
        }
    }

    WorldEntityComponent* WorldEntity::RemoveComponent(WorldEntityComponent* ToRemove)
    {
        ComponentIterator CompIt = this->Components.begin();
        while( CompIt != this->Components.end() )
        {
            if( (*CompIt) == ToRemove ) {
                this->Components.erase(CompIt);
                if( ToRemove->GetComponentType() < Mezzanine::CT_ProxyFirst ) {
                    --(this->ProxyStartIndex);
                }
                return ToRemove;
            }
            ++CompIt;
        }
        return nullptr;
    }

    WorldEntity::ComponentContainer WorldEntity::RemoveAllComponentsOfType(const ComponentType Type)
    {
        ComponentContainer Ret;
        ComponentIterator CompIt = this->Components.begin();
        Whole RemovedNonProxyCount = 0;
        while( CompIt != this->Components.end() )
        {
            if( (*CompIt)->GetComponentType() == Type ) {
                (*CompIt)->_Bind( nullptr );
                Ret.push_back( (*CompIt) );
                if( (*CompIt)->GetComponentType() < Mezzanine::CT_ProxyFirst ) {
                    ++RemovedNonProxyCount;
                }
                CompIt = this->Components.erase(CompIt);
            }else{
                ++CompIt;
            }
        }
        this->ProxyStartIndex -= RemovedNonProxyCount;
        return Ret;
    }

    WorldEntity::ComponentContainer WorldEntity::RemoveAllComponentsOfTypes(const ComponentType TypeFirst, const ComponentType TypeLast)
    {
        ComponentContainer Ret;
        ComponentIterator CompIt = this->Components.begin();
        Whole RemovedNonProxyCount = 0;
        while( CompIt != this->Components.end() )
        {
            if( (*CompIt)->GetComponentType() >= TypeFirst && (*CompIt)->GetComponentType() <= TypeLast ) {
                (*CompIt)->_Bind( nullptr );
                Ret.push_back( (*CompIt) );
                if( (*CompIt)->GetComponentType() < Mezzanine::CT_ProxyFirst ) {
                    ++RemovedNonProxyCount;
                }
                CompIt = this->Components.erase(CompIt);
            }else{
                ++CompIt;
            }
        }
        this->ProxyStartIndex -= RemovedNonProxyCount;
        return Ret;
    }

    WorldEntity::ComponentContainer WorldEntity::RemoveAllComponents()
    {
        ComponentContainer Ret;
        ComponentIterator CompIt = this->Components.begin();
        while( CompIt != this->Components.end() )
            { (*CompIt)->_Bind( nullptr ); }
        Ret.swap(this->Components);
        this->ProxyStartIndex = 0;
        return Ret;
    }

    Whole WorldEntity::GetNumComponents() const
    {
        return this->Components.size();
    }

    WorldEntityComponent* WorldEntity::GetComponent(const Whole Index) const
    {
        return this->Components.at(Index);
    }

    WorldEntityComponent* WorldEntity::GetComponent(const ComponentType Type, Whole TypeIndex) const
    {
        ConstComponentIterator CompIt = this->Components.begin();
        while( CompIt != this->Components.end() )
        {
            if( (*CompIt)->GetComponentType() == Type ) {
                if( TypeIndex == 0 ) return (*CompIt);
                else --TypeIndex;
            }
            ++CompIt;
        }
        return nullptr;
    }

    WorldEntityComponent* WorldEntity::GetComponent(const ComponentType TypeFirst, const ComponentType TypeLast, Whole TypeIndex) const
    {
        ConstComponentIterator CompIt = this->Components.begin();
        while( CompIt != this->Components.end() )
        {
            if( (*CompIt)->GetComponentType() >= TypeFirst && (*CompIt)->GetComponentType() <= TypeLast ) {
                if( TypeIndex == 0 ) return (*CompIt);
                else --TypeIndex;
            }
            ++CompIt;
        }
        return nullptr;
    }

    WorldEntity::ComponentRange WorldEntity::GetNonProxyRange()
        { return ComponentRange(this->Components.begin(),this->Components.begin() + this->ProxyStartIndex); }

    WorldEntity::ConstComponentRange WorldEntity::GetNonProxyRange() const
        { return ConstComponentRange(this->Components.begin(),this->Components.begin() + this->ProxyStartIndex); }

    WorldEntity::ComponentRange WorldEntity::GetProxyRange()
        { return ComponentRange(this->Components.begin() + this->ProxyStartIndex,this->Components.end()); }

    WorldEntity::ConstComponentRange WorldEntity::GetProxyRange() const
        { return ConstComponentRange(this->Components.begin() + this->ProxyStartIndex,this->Components.end()); }

    const WorldEntity::ComponentContainer& WorldEntity::GetComponents() const
        { return this->Components; }

    WorldEntity::ComponentContainer WorldEntity::GetComponents(const ComponentType Type) const
    {
        ComponentContainer Ret;
        ConstComponentIterator CompIt = this->Components.begin();
        while( CompIt != this->Components.end() )
        {
            if( (*CompIt)->GetComponentType() == Type ) {
                Ret.push_back(*CompIt);
            }
            ++CompIt;
        }
        return Ret;
    }

    WorldEntity::ComponentContainer WorldEntity::GetComponents(const ComponentType TypeFirst, const ComponentType TypeLast) const
    {
        ComponentContainer Ret;
        ConstComponentIterator CompIt = this->Components.begin();
        while( CompIt != this->Components.end() )
        {
            if( (*CompIt)->GetComponentType() >= TypeFirst && (*CompIt)->GetComponentType() <= TypeLast ) {
                Ret.push_back(*CompIt);
            }
            ++CompIt;
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
        ComponentRange Range = this->GetProxyRange();
        for( WorldEntityComponent* Prox : Range )
            { static_cast<WorldProxy*>(Prox)->AddToWorld(); }
    }

    void WorldEntity::RemoveFromWorld()
    {
        ComponentRange Range = this->GetProxyRange();
        for( WorldEntityComponent* Prox : Range )
            { static_cast<WorldProxy*>(Prox)->RemoveFromWorld(); }
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Transform Methods

    void WorldEntity::SetTransform(const Transform& Trans)
    {
        ComponentRange Range = this->GetProxyRange();
        for( WorldEntityComponent* Prox : Range )
            { static_cast<WorldProxy*>(Prox)->SetTransform(Trans); }
    }

    void WorldEntity::SetTransform(const Vector3& Loc, const Quaternion& Ori)
    {
        ComponentRange Range = this->GetProxyRange();
        for( WorldEntityComponent* Prox : Range )
            { static_cast<WorldProxy*>(Prox)->SetTransform(Loc,Ori); }
    }

    Transform WorldEntity::GetTransform() const
    {
        assert(this->PrimaryProxy != nullptr && "Valid proxy is needed to query WorldEntity transform.");
        return this->PrimaryProxy->GetTransform();
    }

    void WorldEntity::SetLocation(const Vector3& Loc)
    {
        ComponentRange Range = this->GetProxyRange();
        for( WorldEntityComponent* Prox : Range )
            { static_cast<WorldProxy*>(Prox)->SetLocation(Loc); }
    }

    void WorldEntity::SetLocation(const Real X, const Real Y, const Real Z)
    {
        ComponentRange Range = this->GetProxyRange();
        for( WorldEntityComponent* Prox : Range )
            { static_cast<WorldProxy*>(Prox)->SetLocation(X,Y,Z); }
    }

    Vector3 WorldEntity::GetLocation() const
    {
        assert(this->PrimaryProxy != nullptr && "Valid proxy is needed to query WorldEntity transform.");
        return this->PrimaryProxy->GetLocation();
    }

    void WorldEntity::SetOrientation(const Quaternion& Ori)
    {
        ComponentRange Range = this->GetProxyRange();
        for( WorldEntityComponent* Prox : Range )
            { static_cast<WorldProxy*>(Prox)->SetOrientation(Ori); }
    }

    void WorldEntity::SetOrientation(const Real X, const Real Y, const Real Z, const Real W)
    {
        ComponentRange Range = this->GetProxyRange();
        for( WorldEntityComponent* Prox : Range )
            { static_cast<WorldProxy*>(Prox)->SetOrientation(X,Y,Z,W); }
    }

    Quaternion WorldEntity::GetOrientation() const
    {
        assert(this->PrimaryProxy != nullptr && "Valid proxy is needed to query WorldEntity transform.");
        return this->PrimaryProxy->GetOrientation();
    }

    void WorldEntity::SetScale(const Vector3& Sc)
    {
        ComponentRange Range = this->GetProxyRange();
        for( WorldEntityComponent* Prox : Range )
            { static_cast<WorldProxy*>(Prox)->SetScale(Sc); }
    }

    void WorldEntity::SetScale(const Real X, const Real Y, const Real Z)
    {
        ComponentRange Range = this->GetProxyRange();
        for( WorldEntityComponent* Prox : Range )
            { static_cast<WorldProxy*>(Prox)->SetScale(X,Y,Z); }
    }

    Vector3 WorldEntity::GetScale() const
    {
        assert(this->PrimaryProxy != nullptr && "Valid proxy is needed to query WorldEntity transform.");
        return this->PrimaryProxy->GetScale();
    }

    void WorldEntity::Translate(const Vector3& Trans)
    {
        ComponentRange Range = this->GetProxyRange();
        for( WorldEntityComponent* Prox : Range )
            { static_cast<WorldProxy*>(Prox)->Translate(Trans); }
    }

    void WorldEntity::Translate(const Real X, const Real Y, const Real Z)
    {
        ComponentRange Range = this->GetProxyRange();
        for( WorldEntityComponent* Prox : Range )
            { static_cast<WorldProxy*>(Prox)->Translate(X,Y,Z); }
    }

    void WorldEntity::Yaw(const Real Angle)
    {
        ComponentRange Range = this->GetProxyRange();
        for( WorldEntityComponent* Prox : Range )
            { static_cast<WorldProxy*>(Prox)->Yaw(Angle); }
    }

    void WorldEntity::Pitch(const Real Angle)
    {
        ComponentRange Range = this->GetProxyRange();
        for( WorldEntityComponent* Prox : Range )
            { static_cast<WorldProxy*>(Prox)->Pitch(Angle); }
    }

    void WorldEntity::Roll(const Real Angle)
    {
        ComponentRange Range = this->GetProxyRange();
        for( WorldEntityComponent* Prox : Range )
            { static_cast<WorldProxy*>(Prox)->Roll(Angle); }
    }

    void WorldEntity::Rotate(const Vector3& Axis, const Real Angle)
    {
        ComponentRange Range = this->GetProxyRange();
        for( WorldEntityComponent* Prox : Range )
            { static_cast<WorldProxy*>(Prox)->Rotate(Axis,Angle); }
    }

    void WorldEntity::Rotate(const Quaternion& Rotation)
    {
        ComponentRange Range = this->GetProxyRange();
        for( WorldEntityComponent* Prox : Range )
            { static_cast<WorldProxy*>(Prox)->Rotate(Rotation); }
    }

    void WorldEntity::Scale(const Vector3& Sc)
    {
        ComponentRange Range = this->GetProxyRange();
        for( WorldEntityComponent* Prox : Range )
            { static_cast<WorldProxy*>(Prox)->Scale(Sc); }
    }

    void WorldEntity::Scale(const Real X, const Real Y, const Real Z)
    {
        ComponentRange Range = this->GetProxyRange();
        for( WorldEntityComponent* Prox : Range )
            { static_cast<WorldProxy*>(Prox)->Scale(X,Y,Z); }
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Serialization

    void WorldEntity::ProtoSerialize(XML::Node& ParentNode) const
    {
        XML::Node SelfRoot = ParentNode.AppendChild(this->GetDerivedSerializableName());

        this->ProtoSerializeProperties(SelfRoot);
        this->ProtoSerializeComponents(SelfRoot);
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

    void WorldEntity::ProtoSerializeComponents(XML::Node& SelfRoot) const
    {
        XML::Node ComponentsNode = SelfRoot.AppendChild( WorldEntity::GetSerializableName() + "Components" );

        if( ComponentsNode.AppendAttribute("Version").SetValue("1") &&
            ComponentsNode.AppendAttribute("ProxyCount").SetValue( this->Components.size() ) )
        {
            const ConstComponentIterator BegProx = this->Components.begin();
            for( ConstComponentIterator CompIt = BegProx ; CompIt != this->Components.end() ; ++CompIt )
            {
                XML::Node ProxyNode = ComponentsNode.AppendChild( (*CompIt)->GetDerivedSerializableName() );
                if( ProxyNode.AppendAttribute("ComponentID").SetValue( (*CompIt)->GetComponentID() ) &&
                    ProxyNode.AppendAttribute("ManagerType").SetValue( (*CompIt)->GetCreator()->GetInterfaceType() ) )
                {
                    continue;
                }else{
                    SerializeError("Create XML Attribute Values",WorldEntity::GetSerializableName() + "Components",true);
                }
            }

            return;
        }else{
            SerializeError("Create XML Attribute Values",WorldEntity::GetSerializableName() + "Components",true);
        }
    }

    void WorldEntity::ProtoDeSerialize(const XML::Node& SelfRoot)
    {
        this->ProtoDeSerializeComponents(SelfRoot);
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

    void WorldEntity::ProtoDeSerializeComponents(const XML::Node& SelfRoot)
    {
        this->DestroyAllComponents();

        XML::Attribute CurrAttrib;
        XML::Node ComponentsNode = SelfRoot.GetChild( WorldEntity::GetSerializableName() + "Components" );

        if( !ComponentsNode.Empty() ) {
            if(ComponentsNode.GetAttribute("Version").AsInt() == 1) {
                //CurrAttrib = ComponentsNode.GetAttribute("ProxyCount");
                //if( !CurrAttrib.Empty() )
                //    this->Components.resize( CurrAttrib.AsUint() );

                for( XML::NodeIterator ProxyNodeIt = ComponentsNode.begin() ; ProxyNodeIt != ComponentsNode.end() ; ++ProxyNodeIt )
                {
                    EntityComponentManager* CompMan = nullptr;
                    UInt32 ComponentID = 0;

                    CurrAttrib = (*ProxyNodeIt).GetAttribute("ComponentID");
                    if( !CurrAttrib.Empty() )
                        ComponentID = CurrAttrib.AsUint();

                    CurrAttrib = (*ProxyNodeIt).GetAttribute("ManagerType");
                    if( !CurrAttrib.Empty() )
                        CompMan = this->ParentWorld->GetComponentManager( static_cast<ManagerBase::ManagerType>( CurrAttrib.AsUint() ) );

                    if( ComponentID != 0 && CompMan != nullptr ) {
                        WorldEntityComponent* ToAdd = CompMan->GetComponentByID(ComponentID);
                        if( ToAdd != nullptr ) {
                            this->AddComponent( ToAdd );
                        }else{
                            MEZZ_EXCEPTION(ExceptionBase::II_IDENTITY_NOT_FOUND_EXCEPTION,"WorldProxy of the specified ID does not exist.");
                        }
                    }
                }

                return;
            }else{
                MEZZ_EXCEPTION(ExceptionBase::INVALID_VERSION_EXCEPTION,"Incompatible XML Version for " + (WorldEntity::GetSerializableName() + "Components" ) + ": Not Version 1.");
            }
        }else{
            MEZZ_EXCEPTION(ExceptionBase::II_IDENTITY_NOT_FOUND_EXCEPTION,WorldEntity::GetSerializableName() + "Components" + " was not found in the provided XML node, which was expected.");
        }
    }

    String WorldEntity::GetDerivedSerializableName() const
        { return WorldEntity::GetSerializableName(); }

    String WorldEntity::GetSerializableName()
        { return "WorldEntity"; }

    ///////////////////////////////////////////////////////////////////////////////
    // Internal Methods

    void WorldEntity::_Update(const Whole Delta)
        {  }

    void WorldEntity::_SyncTransforms(WorldProxy* Exclude, const Transform& NewTrans)
    {
        ComponentRange Range = this->GetProxyRange();
        for( WorldEntityComponent* Prox : Range )
        {
            if( Prox != Exclude ) {
                static_cast<WorldProxy*>(Prox)->SetTransform(NewTrans);
            }
        }
    }
}//Mezzanine

#endif
