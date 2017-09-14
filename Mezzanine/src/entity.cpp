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
#ifndef _entity_cpp
#define _entity_cpp

#include "entity.h"
#include "world.h"
#include "entityproxy.h"
#include "entitycomponentmanager.h"
#include "serialization.h"
#include "exception.h"
#include "entresol.h"

#include "Physics/collision.h"

#include <assert.h>

namespace Mezzanine
{
    Boole ComponentCompare(EntityComponent* CompFirst, EntityComponent* CompSecond)
    {
        /// @todo This is placeholder for now.
        return CompFirst->GetComponentType() < CompSecond->GetComponentType();
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Entity

    Entity::Entity(World* TheWorld) :
        PrimaryProxy(nullptr),
        ParentWorld(TheWorld),
        ProxyStartIndex(0)
        {  }

    Entity::Entity(const EntityID& EntID, World* TheWorld) :
        ID(EntID),
        PrimaryProxy(nullptr),
        ParentWorld(TheWorld),
        ProxyStartIndex(0)
        {  }

    Entity::Entity(const XML::Node& SelfRoot, World* TheWorld) :
        PrimaryProxy(nullptr),
        ParentWorld(TheWorld),
        ProxyStartIndex(0)
        { this->ProtoDeSerialize(SelfRoot); }

    void Entity::DestroyAllComponents()
    {
        for( EntityComponent* Comp : this->Components )
        {
            EntityComponentManager* ComponentMan = Comp->GetCreator();
            Comp->_Bind(nullptr);
            ComponentMan->DestroyComponent(Comp);
        }
        this->Components.clear();
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Utility and Configuration

    Mezzanine::EntityType Entity::GetEntityType() const
        { return Mezzanine::ET_Generic; }

    EntityID Entity::GetEntityID() const
        { return this->ID; }

    World* Entity::GetWorld() const
        { return this->ParentWorld; }

    void Entity::SetName(const String& Name)
        { this->EntityName = Name; }

    const String& Entity::GetName() const
        { return this->EntityName; }

    Boole Entity::IsInWorld() const
    {
        assert(this->PrimaryProxy != nullptr && "Valid proxy is needed to query if Entity is in world.");
        return this->PrimaryProxy->IsActivated();
    }

    Boole Entity::IsStatic() const
    {
        assert(this->PrimaryProxy != nullptr && "Valid proxy is needed to query if Entity is static.");
        return this->PrimaryProxy->IsStatic();
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Component Management

    void Entity::AddComponent(EntityComponent* ToAdd)
    {
        ToAdd->_Bind( this );
        ComponentIterator CompIt = std::lower_bound(this->Components.begin(),this->Components.end(),ToAdd,ComponentCompare);
        this->Components.insert(CompIt,ToAdd);
        if( ToAdd->GetComponentType() < Mezzanine::CT_ProxyFirst ) {
            ++(this->ProxyStartIndex);
        }
    }

    EntityComponent* Entity::RemoveComponent(EntityComponent* ToRemove)
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

    Entity::ComponentContainer Entity::RemoveAllComponentsOfType(const ComponentType Type)
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

    Entity::ComponentContainer Entity::RemoveAllComponentsOfTypes(const ComponentType TypeFirst, const ComponentType TypeLast)
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

    Entity::ComponentContainer Entity::RemoveAllComponents()
    {
        ComponentContainer Ret;
        for( EntityComponent* Comp : this->Components )
            { Comp->_Bind( nullptr ); }
        Ret.swap(this->Components);
        this->ProxyStartIndex = 0;
        return Ret;
    }

    Whole Entity::GetNumComponents() const
    {
        return this->Components.size();
    }

    EntityComponent* Entity::GetComponent(const Whole Index) const
    {
        return this->Components.at(Index);
    }

    EntityComponent* Entity::GetComponent(const ComponentType Type, Whole TypeIndex) const
    {
        for( EntityComponent* Comp : this->Components )
        {
            if( Comp->GetComponentType() == Type ) {
                if( TypeIndex == 0 ) return Comp;
                else --TypeIndex;
            }
        }
        return nullptr;
    }

    EntityComponent* Entity::GetComponent(const ComponentType TypeFirst, const ComponentType TypeLast, Whole TypeIndex) const
    {
        for( EntityComponent* Comp : this->Components )
        {
            if( Comp->GetComponentType() >= TypeFirst && Comp->GetComponentType() <= TypeLast ) {
                if( TypeIndex == 0 ) return Comp;
                else --TypeIndex;
            }
        }
        return nullptr;
    }

    Entity::ComponentRange Entity::GetNonProxyRange()
        { return ComponentRange(this->Components.begin(),this->Components.begin() + this->ProxyStartIndex); }

    Entity::ConstComponentRange Entity::GetNonProxyRange() const
        { return ConstComponentRange(this->Components.begin(),this->Components.begin() + this->ProxyStartIndex); }

    Entity::ComponentRange Entity::GetProxyRange()
        { return ComponentRange(this->Components.begin() + this->ProxyStartIndex,this->Components.end()); }

    Entity::ConstComponentRange Entity::GetProxyRange() const
        { return ConstComponentRange(this->Components.begin() + this->ProxyStartIndex,this->Components.end()); }

    const Entity::ComponentContainer& Entity::GetComponents() const
        { return this->Components; }

    Entity::ComponentContainer Entity::GetComponents(const ComponentType Type) const
    {
        ComponentContainer Ret;
        for( EntityComponent* Comp : this->Components )
        {
            if( Comp->GetComponentType() == Type ) {
                Ret.push_back(Comp);
            }
        }
        return Ret;
    }

    Entity::ComponentContainer Entity::GetComponents(const ComponentType TypeFirst, const ComponentType TypeLast) const
    {
        ComponentContainer Ret;
        for( EntityComponent* Comp : this->Components )
        {
            if( Comp->GetComponentType() >= TypeFirst && Comp->GetComponentType() <= TypeLast ) {
                Ret.push_back(Comp);
            }
        }
        return Ret;
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Proxy Management

    void Entity::SetPrimaryProxy(EntityProxy* Primary)
    {
        assert(this == Primary->GetParentEntity() && "WorldProxies can only be made PrimaryProxy of a Entity they are bound to.");
        this->PrimaryProxy = Primary;
    }

    EntityProxy* Entity::GetPrimaryProxy() const
    {
        return this->PrimaryProxy;
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Working with the World

    void Entity::AddToWorld()
    {
        ComponentRange Range = this->GetProxyRange();
        for( EntityComponent* Prox : Range )
            { static_cast<EntityProxy*>(Prox)->Activate(); }
    }

    void Entity::RemoveFromWorld()
    {
        ComponentRange Range = this->GetProxyRange();
        for( EntityComponent* Prox : Range )
            { static_cast<EntityProxy*>(Prox)->Deactivate(); }
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Transform Methods

    void Entity::SetTransform(const Transform& Trans)
    {
        ComponentRange Range = this->GetProxyRange();
        for( EntityComponent* Prox : Range )
            { static_cast<EntityProxy*>(Prox)->SetTransform(Trans); }
    }

    void Entity::SetTransform(const Vector3& Loc, const Quaternion& Ori)
    {
        ComponentRange Range = this->GetProxyRange();
        for( EntityComponent* Prox : Range )
            { static_cast<EntityProxy*>(Prox)->SetTransform(Loc,Ori); }
    }

    Transform Entity::GetTransform() const
    {
        assert(this->PrimaryProxy != nullptr && "Valid proxy is needed to query Entity transform.");
        return this->PrimaryProxy->GetTransform();
    }

    void Entity::SetLocation(const Vector3& Loc)
    {
        ComponentRange Range = this->GetProxyRange();
        for( EntityComponent* Prox : Range )
            { static_cast<EntityProxy*>(Prox)->SetLocation(Loc); }
    }

    void Entity::SetLocation(const Real X, const Real Y, const Real Z)
    {
        ComponentRange Range = this->GetProxyRange();
        for( EntityComponent* Prox : Range )
            { static_cast<EntityProxy*>(Prox)->SetLocation(X,Y,Z); }
    }

    Vector3 Entity::GetLocation() const
    {
        assert(this->PrimaryProxy != nullptr && "Valid proxy is needed to query Entity transform.");
        return this->PrimaryProxy->GetLocation();
    }

    void Entity::SetOrientation(const Quaternion& Ori)
    {
        ComponentRange Range = this->GetProxyRange();
        for( EntityComponent* Prox : Range )
            { static_cast<EntityProxy*>(Prox)->SetOrientation(Ori); }
    }

    void Entity::SetOrientation(const Real X, const Real Y, const Real Z, const Real W)
    {
        ComponentRange Range = this->GetProxyRange();
        for( EntityComponent* Prox : Range )
            { static_cast<EntityProxy*>(Prox)->SetOrientation(X,Y,Z,W); }
    }

    Quaternion Entity::GetOrientation() const
    {
        assert(this->PrimaryProxy != nullptr && "Valid proxy is needed to query Entity transform.");
        return this->PrimaryProxy->GetOrientation();
    }

    void Entity::SetScale(const Vector3& Sc)
    {
        ComponentRange Range = this->GetProxyRange();
        for( EntityComponent* Prox : Range )
            { static_cast<EntityProxy*>(Prox)->SetScale(Sc); }
    }

    void Entity::SetScale(const Real X, const Real Y, const Real Z)
    {
        ComponentRange Range = this->GetProxyRange();
        for( EntityComponent* Prox : Range )
            { static_cast<EntityProxy*>(Prox)->SetScale(X,Y,Z); }
    }

    Vector3 Entity::GetScale() const
    {
        assert(this->PrimaryProxy != nullptr && "Valid proxy is needed to query Entity transform.");
        return this->PrimaryProxy->GetScale();
    }

    void Entity::Translate(const Vector3& Trans)
    {
        ComponentRange Range = this->GetProxyRange();
        for( EntityComponent* Prox : Range )
            { static_cast<EntityProxy*>(Prox)->Translate(Trans); }
    }

    void Entity::Translate(const Real X, const Real Y, const Real Z)
    {
        ComponentRange Range = this->GetProxyRange();
        for( EntityComponent* Prox : Range )
            { static_cast<EntityProxy*>(Prox)->Translate(X,Y,Z); }
    }

    void Entity::Yaw(const Real Angle)
    {
        ComponentRange Range = this->GetProxyRange();
        for( EntityComponent* Prox : Range )
            { static_cast<EntityProxy*>(Prox)->Yaw(Angle); }
    }

    void Entity::Pitch(const Real Angle)
    {
        ComponentRange Range = this->GetProxyRange();
        for( EntityComponent* Prox : Range )
            { static_cast<EntityProxy*>(Prox)->Pitch(Angle); }
    }

    void Entity::Roll(const Real Angle)
    {
        ComponentRange Range = this->GetProxyRange();
        for( EntityComponent* Prox : Range )
            { static_cast<EntityProxy*>(Prox)->Roll(Angle); }
    }

    void Entity::Rotate(const Vector3& Axis, const Real Angle)
    {
        ComponentRange Range = this->GetProxyRange();
        for( EntityComponent* Prox : Range )
            { static_cast<EntityProxy*>(Prox)->Rotate(Axis,Angle); }
    }

    void Entity::Rotate(const Quaternion& Rotation)
    {
        ComponentRange Range = this->GetProxyRange();
        for( EntityComponent* Prox : Range )
            { static_cast<EntityProxy*>(Prox)->Rotate(Rotation); }
    }

    void Entity::Scale(const Vector3& Sc)
    {
        ComponentRange Range = this->GetProxyRange();
        for( EntityComponent* Prox : Range )
            { static_cast<EntityProxy*>(Prox)->Scale(Sc); }
    }

    void Entity::Scale(const Real X, const Real Y, const Real Z)
    {
        ComponentRange Range = this->GetProxyRange();
        for( EntityComponent* Prox : Range )
            { static_cast<EntityProxy*>(Prox)->Scale(X,Y,Z); }
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Serialization

    void Entity::ProtoSerialize(XML::Node& ParentNode) const
    {
        XML::Node SelfRoot = ParentNode.AppendChild(this->GetDerivedSerializableName());

        this->ProtoSerializeProperties(SelfRoot);
        this->ProtoSerializeComponents(SelfRoot);
    }

    void Entity::ProtoSerializeProperties(XML::Node& SelfRoot) const
    {
        XML::Node PropertiesNode = SelfRoot.AppendChild( Entity::GetSerializableName() + "Properties" );

        if( PropertiesNode.AppendAttribute("Version").SetValue("1") &&
            PropertiesNode.AppendAttribute("Name").SetValue( this->GetName() ) )
        {
            XML::Node IDNode = PropertiesNode.AppendChild("ID");
            this->ID.ProtoSerialize( IDNode );
            /// @todo Is serializing transform data necessary?  Proxies have their own copy.
            XML::Node LocationNode = PropertiesNode.AppendChild("Location");
            this->GetLocation().ProtoSerialize( LocationNode );
            XML::Node OrientationNode = PropertiesNode.AppendChild("Orientation");
            this->GetOrientation().ProtoSerialize( OrientationNode );
            XML::Node ScaleNode = PropertiesNode.AppendChild("Scale");
            this->GetScale().ProtoSerialize( ScaleNode );

            return;
        }else{
            SerializeError("Create XML Attribute Values",Entity::GetSerializableName() + "Properties",true);
        }
    }

    void Entity::ProtoSerializeComponents(XML::Node& SelfRoot) const
    {
        XML::Node ComponentsNode = SelfRoot.AppendChild( Entity::GetSerializableName() + "Components" );

        if( ComponentsNode.AppendAttribute("Version").SetValue("1") &&
            ComponentsNode.AppendAttribute("ProxyCount").SetValue( this->Components.size() ) )
        {
            const ConstComponentIterator BegProx = this->Components.begin();
            for( ConstComponentIterator CompIt = BegProx ; CompIt != this->Components.end() ; ++CompIt )
            {
                XML::Node ProxyNode = ComponentsNode.AppendChild( (*CompIt)->GetDerivedSerializableName() );
                if( ProxyNode.AppendAttribute("ComponentID").SetValue( (*CompIt)->GetComponentID().ID ) &&
                    ProxyNode.AppendAttribute("ManagerType").SetValue( (*CompIt)->GetCreator()->GetInterfaceType() ) )
                {
                    continue;
                }else{
                    SerializeError("Create XML Attribute Values",Entity::GetSerializableName() + "Components",true);
                }
            }

            return;
        }else{
            SerializeError("Create XML Attribute Values",Entity::GetSerializableName() + "Components",true);
        }
    }

    void Entity::ProtoDeSerialize(const XML::Node& SelfRoot)
    {
        this->ProtoDeSerializeComponents(SelfRoot);
        this->ProtoDeSerializeProperties(SelfRoot);
    }

    void Entity::ProtoDeSerializeProperties(const XML::Node& SelfRoot)
    {
        XML::Attribute CurrAttrib;
        XML::Node PropertiesNode = SelfRoot.GetChild( Entity::GetSerializableName() + "Properties" );

        if( !PropertiesNode.Empty() ) {
            if( PropertiesNode.GetAttribute("Version").AsInt() == 1 ) {
                CurrAttrib = PropertiesNode.GetAttribute("Name");
                if( !CurrAttrib.Empty() ) {
                    this->SetName( CurrAttrib.AsString() );
                }

                XML::Node IDNode = PropertiesNode.GetChild("ID").GetFirstChild();
                if( !IDNode.Empty() ) {
                    this->ID.ProtoDeSerialize(IDNode);
                }

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
                MEZZ_EXCEPTION(ExceptionBase::INVALID_VERSION_EXCEPTION,"Incompatible XML Version for " + (Entity::GetSerializableName() + "Properties" ) + ": Not Version 1.");
            }
        }else{
            MEZZ_EXCEPTION(ExceptionBase::II_IDENTITY_NOT_FOUND_EXCEPTION,Entity::GetSerializableName() + "Properties" + " was not found in the provided XML node, which was expected.");
        }
    }

    void Entity::ProtoDeSerializeComponents(const XML::Node& SelfRoot)
    {
        this->DestroyAllComponents();

        XML::Attribute CurrAttrib;
        XML::Node ComponentsNode = SelfRoot.GetChild( Entity::GetSerializableName() + "Components" );

        if( !ComponentsNode.Empty() ) {
            if( ComponentsNode.GetAttribute("Version").AsInt() == 1 ) {
                CurrAttrib = ComponentsNode.GetAttribute("ProxyCount");
                if( !CurrAttrib.Empty() ) {
                    this->Components.resize( CurrAttrib.AsUint() );
                }

                for( XML::NodeIterator ProxyNodeIt = ComponentsNode.begin() ; ProxyNodeIt != ComponentsNode.end() ; ++ProxyNodeIt )
                {
                    EntityComponentManager* CompMan = nullptr;
                    UInt32 CompID = 0;

                    CurrAttrib = (*ProxyNodeIt).GetAttribute("ComponentID");
                    if( !CurrAttrib.Empty() ) {
                        CompID = CurrAttrib.AsUint();
                    }

                    CurrAttrib = (*ProxyNodeIt).GetAttribute("ManagerType");
                    if( !CurrAttrib.Empty() ) {
                        CompMan = this->ParentWorld->GetComponentManager( static_cast<ManagerBase::ManagerType>( CurrAttrib.AsUint() ) );
                    }

                    if( CompID != 0 && CompMan != nullptr ) {
                        EntityComponent* ToAdd = CompMan->GetComponentByID(CompID);
                        if( ToAdd != nullptr ) {
                            this->AddComponent( ToAdd );
                        }else{
                            MEZZ_EXCEPTION(ExceptionBase::II_IDENTITY_NOT_FOUND_EXCEPTION,"EntityProxy of the specified ID does not exist.");
                        }
                    }
                }

                return;
            }else{
                MEZZ_EXCEPTION(ExceptionBase::INVALID_VERSION_EXCEPTION,"Incompatible XML Version for " + (Entity::GetSerializableName() + "Components" ) + ": Not Version 1.");
            }
        }else{
            MEZZ_EXCEPTION(ExceptionBase::II_IDENTITY_NOT_FOUND_EXCEPTION,Entity::GetSerializableName() + "Components" + " was not found in the provided XML node, which was expected.");
        }
    }

    String Entity::GetDerivedSerializableName() const
        { return Entity::GetSerializableName(); }

    String Entity::GetSerializableName()
        { return "Entity"; }

    ///////////////////////////////////////////////////////////////////////////////
    // Internal Methods

    void Entity::_Update(const Whole Delta)
        {  }

    void Entity::_SyncTransforms(EntityProxy* Exclude, const Transform& NewTrans)
    {
        ComponentRange Range = this->GetProxyRange();
        for( EntityComponent* Prox : Range )
        {
            if( Prox != Exclude ) {
                static_cast<EntityProxy*>(Prox)->SetTransform(NewTrans);
            }
        }
    }

    ///////////////////////////////////////////////////////////////////////////////
    // EntityFactory Methods

    String GenericEntityFactory::GetTypeName() const
        { return Entity::GetSerializableName(); }

    Entity* GenericEntityFactory::CreateEntity(const EntityID& EntID, World* TheWorld, const NameValuePairMap& Params)
        { return new Entity(EntID,TheWorld); }

    Entity* GenericEntityFactory::CreateEntity(const XML::Node& XMLNode, World* TheWorld)
        { return new Entity(XMLNode,TheWorld); }

    void GenericEntityFactory::DestroyEntity(Entity* ToBeDestroyed)
        { delete ToBeDestroyed; }
}//Mezzanine

#endif
