#ifndef _levelzone_cpp
#define _levelzone_cpp

#include "levelzone.h"
#include "catchapp.h"

#include <mezzanine.h>

#include <algorithm>

///////////////////////////////////////////////////////////////////////////////
// LevelZone Methods

LevelZone::LevelZone(World* TheWorld) :
    AreaEffect(TheWorld),
    EntProx(NULL)
    {  }

LevelZone::LevelZone(const String& Name, World* TheWorld, const Vector3& HalfZoneSize) :
    AreaEffect(Name,TheWorld),
    ZoneSize(HalfZoneSize),
    EntProx(NULL)
    { this->CreateLevelZone(); }

LevelZone::~LevelZone()
    { this->DestroyLevelZone(); }

void LevelZone::CreateLevelZone()
{
    Graphics::SceneManager* SceneMan = this->ParentWorld->GetSceneManager();
    if( SceneMan != NULL ) {
        this->EntProx = SceneMan->CreateEntityProxy(false);
        this->EntProx->_Bind(this);
    }
}

void LevelZone::DestroyLevelZone()
{
    this->RemoveFromWorld();
    Graphics::SceneManager* SceneMan = this->ParentWorld->GetSceneManager();
    if( this->EntProx != NULL && SceneMan != NULL ) {
        SceneMan->DestroyProxy( this->EntProx );
        this->EntProx = NULL;
    }
}

///////////////////////////////////////////////////////////////////////////////
// Utility

Mezzanine::WorldObjectType LevelZone::GetType() const
    { return Mezzanine::WO_AreaEffectUnknown; }

Graphics::EntityProxy* LevelZone::GetEntityProxy() const
    { return this->EntProx; }

bool LevelZone::IsInside(WorldObject* Object)
    { return ( std::find( this->OverlappingObjects.begin(), this->OverlappingObjects.end(), Object ) != this->OverlappingObjects.end() ); }

bool LevelZone::IsEmpty()
    { return OverlappingObjects.empty(); }

void LevelZone::GetProxies(ProxyContainer& Proxies)
{
    AreaEffect::GetProxies(Proxies);
    Proxies.push_back( this->EntProx );
}

void LevelZone::GetProxies(const UInt32 Types, ProxyContainer& Proxies)
{
    AreaEffect::GetProxies(Types,Proxies);
    if( Types & Mezzanine::PT_Graphics_EntityProxy ) {
        Proxies.push_back( this->EntProx );
    }
}

///////////////////////////////////////////////////////////////////////////////
// Working with the World

void LevelZone::AddToWorld()
{
    AreaEffect::AddToWorld();
    if( this->EntProx )
        this->EntProx->AddToWorld();
}

void LevelZone::RemoveFromWorld()
{
    AreaEffect::RemoveFromWorld();
    if( this->EntProx )
        this->EntProx->RemoveFromWorld();
}

///////////////////////////////////////////////////////////////////////////////
// LevelZone Properties

///////////////////////////////////////////////////////////////////////////////
// Transform Methods

void LevelZone::SetLocation(const Vector3& Loc)
{
    AreaEffect::SetLocation(Loc);
    this->EntProx->SetLocation(Loc);
}

void LevelZone::SetLocation(const Real X, const Real Y, const Real Z)
{
    AreaEffect::SetLocation(X,Y,Z);
    this->EntProx->SetLocation(X,Y,Z);
}

Vector3 LevelZone::GetLocation() const
{
    return AreaEffect::GetLocation();
}

void LevelZone::SetOrientation(const Quaternion& Ori)
{
    AreaEffect::SetOrientation(Ori);
    this->EntProx->SetOrientation(Ori);
}

void LevelZone::SetOrientation(const Real X, const Real Y, const Real Z, const Real W)
{
    AreaEffect::SetOrientation(X,Y,Z,W);
    this->EntProx->SetOrientation(X,Y,Z,W);
}

Quaternion LevelZone::GetOrientation() const
{
    return AreaEffect::GetOrientation();
}

void LevelZone::SetScale(const Vector3& Sc)
{
    AreaEffect::SetScale(Sc);
    this->EntProx->SetScale(Sc);
}

void LevelZone::SetScale(const Real X, const Real Y, const Real Z)
{
    AreaEffect::SetScale(X,Y,Z);
    this->EntProx->SetScale(X,Y,Z);
}

Vector3 LevelZone::GetScale() const
{
    return AreaEffect::GetScale();
}

void LevelZone::Translate(const Vector3& Trans)
{
    AreaEffect::Translate(Trans);
    this->EntProx->Translate(Trans);
}

void LevelZone::Translate(const Real X, const Real Y, const Real Z)
{
    AreaEffect::Translate(X,Y,Z);
    this->EntProx->Translate(X,Y,Z);
}

void LevelZone::Yaw(const Real Angle)
{
    AreaEffect::Yaw(Angle);
    this->EntProx->Yaw(Angle);
}

void LevelZone::Pitch(const Real Angle)
{
    AreaEffect::Pitch(Angle);
    this->EntProx->Pitch(Angle);
}

void LevelZone::Roll(const Real Angle)
{
    AreaEffect::Roll(Angle);
    this->EntProx->Roll(Angle);
}

void LevelZone::Rotate(const Vector3& Axis, const Real Angle)
{
    AreaEffect::Rotate(Axis,Angle);
    this->EntProx->Rotate(Axis,Angle);
}

void LevelZone::Rotate(const Quaternion& Rotation)
{
    AreaEffect::Rotate(Rotation);
    this->EntProx->Rotate(Rotation);
}

void LevelZone::Scale(const Vector3& Scale)
{
    AreaEffect::Scale(Scale);
    this->EntProx->Scale(Scale);
}

void LevelZone::Scale(const Real X, const Real Y, const Real Z)
{
    AreaEffect::Scale(X,Y,Z);
    this->EntProx->Scale(X,Y,Z);
}

///////////////////////////////////////////////////////////////////////////////
// Serialization

void LevelZone::ProtoSerializeProperties(XML::Node& SelfRoot) const
{
    this->AreaEffect::ProtoSerializeProperties(SelfRoot);
    XML::Node PropertiesNode = SelfRoot.AppendChild( LevelZone::GetSerializableName() + "Properties" );

    if( PropertiesNode.AppendAttribute("Version").SetValue("1") )
    {
        XML::Node ZoneSizeNode = PropertiesNode.AppendChild("ZoneSize");
        this->ZoneSize.ProtoSerialize( ZoneSizeNode );

        return;
    }else{
        SerializeError("Create XML Attribute Values",LevelZone::GetSerializableName() + "Properties",true);
    }
}

void LevelZone::ProtoSerializeProxies(XML::Node& SelfRoot) const
{
    this->AreaEffect::ProtoSerializeProxies(SelfRoot);
    XML::Node ProxiesNode = SelfRoot.AppendChild( LevelZone::GetSerializableName() + "Proxies" );

    if( ProxiesNode.AppendAttribute("Version").SetValue("1") )
    {
        XML::Node EntProxNode = ProxiesNode.AppendChild("EntProx");
        this->EntProx->ProtoSerialize( EntProxNode );

        return;
    }else{
        SerializeError("Create XML Attribute Values",LevelZone::GetSerializableName() + "Proxies",true);
    }
}

void LevelZone::ProtoDeSerializeProperties(const XML::Node& SelfRoot)
{
    this->AreaEffect::ProtoDeSerializeProperties(SelfRoot);
    //XML::Attribute CurrAttrib;
    XML::Node PropertiesNode = SelfRoot.GetChild( LevelZone::GetSerializableName() + "Properties" );

    if( !PropertiesNode.Empty() ) {
        if(PropertiesNode.GetAttribute("Version").AsInt() == 1) {
            XML::Node ZoneSizeNode = PropertiesNode.GetChild("ZoneSize").GetFirstChild();
            if( !ZoneSizeNode.Empty() ) {
                this->ZoneSize.ProtoDeSerialize( ZoneSizeNode );
            }
        }else{
            MEZZ_EXCEPTION(Exception::INVALID_VERSION_EXCEPTION,"Incompatible XML Version for " + (LevelZone::GetSerializableName() + "Properties" ) + ": Not Version 1.");
        }
    }else{
        MEZZ_EXCEPTION(Exception::II_IDENTITY_NOT_FOUND_EXCEPTION,LevelZone::GetSerializableName() + "Properties" + " was not found in the provided XML node, which was expected.");
    }
}

void LevelZone::ProtoDeSerializeProxies(const XML::Node& SelfRoot)
{
    this->DestroyLevelZone();
    this->AreaEffect::ProtoDeSerializeProxies(SelfRoot);
    //XML::Attribute CurrAttrib;
    XML::Node ProxiesNode = SelfRoot.GetChild( LevelZone::GetSerializableName() + "Proxies" );

    if( !ProxiesNode.Empty() ) {
        if(ProxiesNode.GetAttribute("Version").AsInt() == 1) {
            /// @todo I don't think an exception is appropriate for the failure of the worldmanager validity checks,
            /// however a warning should be written to the log if that happens.  This should be updated to do that once
            /// logging refactors are done.

            XML::Node EntProxNode = ProxiesNode.GetChild("EntProx").GetFirstChild();
            if( !EntProxNode.Empty() ) {
                Graphics::SceneManager* SceneMan = this->ParentWorld->GetSceneManager();
                if( SceneMan ) {
                    this->EntProx = SceneMan->CreateEntityProxy(EntProxNode);
                    this->EntProx->_Bind( this );
                }
            }
        }else{
            MEZZ_EXCEPTION(Exception::INVALID_VERSION_EXCEPTION,"Incompatible XML Version for " + (LevelZone::GetSerializableName() + "Proxies" ) + ": Not Version 1.");
        }
    }else{
        MEZZ_EXCEPTION(Exception::II_IDENTITY_NOT_FOUND_EXCEPTION,LevelZone::GetSerializableName() + "Proxies" + " was not found in the provided XML node, which was expected.");
    }
}

String LevelZone::GetDerivedSerializableName() const
    { return LevelZone::GetSerializableName(); }

String LevelZone::GetSerializableName()
    { return "LevelZone"; }

///////////////////////////////////////////////////////////////////////////////
// Internal Methods

void LevelZone::_NotifyProxyDestroyed(WorldProxy* ToBeDestroyed)
{
    AreaEffect::_NotifyProxyDestroyed(ToBeDestroyed);
    if( ToBeDestroyed == NULL )
        return;

    if( this->EntProx == ToBeDestroyed ) {
        this->EntProx = NULL;
    }
}

#endif
