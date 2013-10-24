#ifndef _startarea_cpp
#define _startarea_cpp

#include "startarea.h"
#include "catchapp.h"

#include <algorithm>

///////////////////////////////////////////////////////////////////////////////
// StartArea Methods

StartArea::StartArea(World* TheWorld) :
    LevelZone(TheWorld)
    { CatchApp::GetCatchAppPointer()->RegisterStartArea(this); }

StartArea::StartArea(const String& Name, World* TheWorld, const Vector3& HalfAreaSize) :
    LevelZone(Name,TheWorld,HalfAreaSize)
    { this->CreateStartArea(HalfAreaSize); }

StartArea::StartArea(const XML::Node& SelfRoot, World* TheWorld) :
    LevelZone(TheWorld)
    { this->ProtoDeSerialize(SelfRoot); }

StartArea::~StartArea()
    { this->DestroyStartArea(); }

void StartArea::CreateStartArea(const Vector3& HalfAreaSize)
{
    CatchApp::GetCatchAppPointer()->RegisterStartArea(this);
    Graphics::SceneManager* SceneMan = Entresol::GetSingletonPtr()->GetSceneManager();
    //Graphics::MeshManager* MeshMan = Entresol::GetSingletonPtr()->GetMeshManager();
    //Physics::PhysicsManager* PhysMan = Entresol::GetSingletonPtr()->GetPhysicsManager();
    //Physics::CollisionShapeMananger* CSMan = Entresol::GetSingletonPtr()->GetCollisionShapeManager();

    if( SceneMan != NULL ) {
        this->ParSysProx = SceneMan->CreateParticleSystemProxy("StartVortex");
        this->ParSysProx->_Bind(this);

        Graphics::ParticleEmitter* ParSysEmit = this->ParSysProx->GetEmitter(0);
        ParSysEmit->SetCustomParam( "width",StringTools::ConvertToString( HalfAreaSize.X * (Real)1.8 ) );
        ParSysEmit->SetCustomParam( "height",StringTools::ConvertToString( HalfAreaSize.Y * (Real)0.6 ) );
        ParSysEmit->SetCustomParam( "depth",StringTools::ConvertToString( HalfAreaSize.Z * (Real)1.8 ) );
        this->ParSysProx->SetLocation(this->GetParticleSystemOffset());
    }

    /*if( this->EntProx != NULL && MeshMan != NULL ) {
        Graphics::Mesh* StartMesh = MeshMan->CreateBoxCornerMesh("StartAreaMesh",ColourValue(0.1,0.8,0.1,0.8),HalfAreaSize,4.0);
        this->EntProx->SetMesh( StartMesh );
    }

    if( this->Ghost != NULL && CSMan != NULL ) {
        Physics::CollisionShape* StartCS = new Physics::BoxCollisionShape("StartAreaShape",HalfAreaSize);
        this->Ghost->SetCollisionShape( StartCS );
    }//*/
}

void StartArea::DestroyStartArea()
{
    this->RemoveFromWorld();
    Graphics::SceneManager* SceneMan = Entresol::GetSingletonPtr()->GetSceneManager();
    if( this->ParSysProx != NULL && SceneMan != NULL ) {
        SceneMan->DestroyProxy( this->ParSysProx );
        this->ParSysProx = NULL;
    }
}

Vector3 StartArea::GetParticleSystemOffset() const
{
    if( this->ParSysProx ) {
        Real ParSysHeight = StringTools::ConvertToReal( this->ParSysProx->GetEmitter(0)->GetCustomParam( "height" ) );
        return Vector3( 0, ( -this->ZoneSize.Y ) + ( ParSysHeight * 0.5 ), 0 );
    }else{
        return Vector3();
    }
}

///////////////////////////////////////////////////////////////////////////////
// Utility

Graphics::ParticleSystemProxy* StartArea::GetParticleSystemProxy() const
    { return this->ParSysProx; }

void StartArea::SetParticleMinimumTimeToLive(const Real MinTTL)
{
    if( this->ParSysProx )
        this->ParSysProx->GetEmitter(0)->SetCustomParam( "time_to_live_min",StringTools::ConvertToString( MinTTL ) );
}

Real StartArea::GetParticleMinimumTimeToLive() const
{
    if( this->ParSysProx )
        return StringTools::ConvertToReal( this->ParSysProx->GetEmitter(0)->GetCustomParam( "time_to_live_min" ) );
    else
        return 0;
}

void StartArea::SetParticleMaximumTimeToLive(const Real MaxTTL)
{
    if( this->ParSysProx )
        this->ParSysProx->GetEmitter(0)->SetCustomParam( "time_to_live_max",StringTools::ConvertToString( MaxTTL ) );
}

Real StartArea::GetParticleMaximumTimeToLive() const
{
    if( this->ParSysProx )
        return StringTools::ConvertToReal( this->ParSysProx->GetEmitter(0)->GetCustomParam( "time_to_live_max" ) );
    else
        return 0;
}

void StartArea::ApplyEffect()
{
    static const Vector3 ZeroGrav(0,0,0);
    if( !this->AddedObjects.empty() )
    {
        for( ObjectIterator ObjIt = this->AddedObjects.begin() ; ObjIt != this->AddedObjects.end() ; ObjIt++ )
        {
            ProxyContainer ColProxies;
            (*ObjIt)->GetProxies(Mezzanine::PT_Physics_RigidProxy | Mezzanine::PT_Physics_SoftProxy,ColProxies);
            for( ProxyIterator ProxIt = ColProxies.begin() ; ProxIt != ColProxies.end() ; ++ProxIt )
            {
                static_cast<Physics::CollidableProxy*>( *ProxIt )->SetCollisionResponse(false);
                if( (*ProxIt)->GetProxyType() == Mezzanine::PT_Physics_RigidProxy ) {
                    static_cast<Physics::RigidProxy*>( *ProxIt )->SetGravity( ZeroGrav );
                }
            }
        }
    }

    if( !this->RemovedObjects.empty() )
    {
        const Vector3 WorldGrav = Entresol::GetSingletonPtr()->GetPhysicsManager()->GetWorldGravity();
        for( ObjectIterator ObjIt = this->RemovedObjects.begin() ; ObjIt != this->RemovedObjects.end() ; ObjIt++ )
        {
            ProxyContainer ColProxies;
            (*ObjIt)->GetProxies(Mezzanine::PT_Physics_RigidProxy | Mezzanine::PT_Physics_SoftProxy,ColProxies);
            for( ProxyIterator ProxIt = ColProxies.begin() ; ProxIt != ColProxies.end() ; ++ProxIt )
            {
                static_cast<Physics::CollidableProxy*>( *ProxIt )->SetCollisionResponse(true);
                if( (*ProxIt)->GetProxyType() == Mezzanine::PT_Physics_RigidProxy ) {
                    static_cast<Physics::RigidProxy*>( *ProxIt )->SetGravity( WorldGrav );
                }
            }
        }
    }
}

void StartArea::GetProxies(ProxyContainer& Proxies)
{
    LevelZone::GetProxies(Proxies);
    Proxies.push_back( this->ParSysProx );
}

void StartArea::GetProxies(const UInt32 Types, ProxyContainer& Proxies)
{
    LevelZone::GetProxies(Types,Proxies);
    if( Types & Mezzanine::PT_Graphics_ParticleSystemProxy ) {
        Proxies.push_back( this->ParSysProx );
    }
}

///////////////////////////////////////////////////////////////////////////////
// Working with the World

void StartArea::AddToWorld()
{
    LevelZone::AddToWorld();
    if( this->ParSysProx )
        this->ParSysProx->AddToWorld();
}

void StartArea::RemoveFromWorld()
{
    LevelZone::RemoveFromWorld();
    if( this->ParSysProx )
        this->ParSysProx->RemoveFromWorld();
}

///////////////////////////////////////////////////////////////////////////////
// LevelZone Properties

///////////////////////////////////////////////////////////////////////////////
// Transform Methods

void StartArea::SetLocation(const Vector3& Loc)
{
    LevelZone::SetLocation(Loc);
    this->ParSysProx->SetLocation( this->ConvertLocalToGlobal( Loc + this->GetParticleSystemOffset() ) );
}

void StartArea::SetLocation(const Real X, const Real Y, const Real Z)
{
    LevelZone::SetLocation(X,Y,Z);
    this->ParSysProx->SetLocation( this->ConvertLocalToGlobal( Vector3(X,Y,Z) + this->GetParticleSystemOffset() ) );
}

Vector3 StartArea::GetLocation() const
{
    return AreaEffect::GetLocation();
}

void StartArea::SetOrientation(const Quaternion& Ori)
{
    LevelZone::SetOrientation(Ori);
    this->ParSysProx->SetOrientation(Ori);
    this->ParSysProx->SetLocation( this->ConvertLocalToGlobal( this->GetLocation() + this->GetParticleSystemOffset() ) );
}

void StartArea::SetOrientation(const Real X, const Real Y, const Real Z, const Real W)
{
    LevelZone::SetOrientation(X,Y,Z,W);
    this->ParSysProx->SetOrientation(X,Y,Z,W);
    this->ParSysProx->SetLocation( this->ConvertLocalToGlobal( this->GetLocation() + this->GetParticleSystemOffset() ) );
}

Quaternion StartArea::GetOrientation() const
{
    return AreaEffect::GetOrientation();
}

void StartArea::SetScale(const Vector3& Sc)
{
    LevelZone::SetScale(Sc);
    this->ParSysProx->SetScale(Sc);
    this->ParSysProx->SetLocation( this->ConvertLocalToGlobal( this->GetLocation() + this->GetParticleSystemOffset() ) );
}

void StartArea::SetScale(const Real X, const Real Y, const Real Z)
{
    LevelZone::SetScale(X,Y,Z);
    this->ParSysProx->SetScale(X,Y,Z);
    this->ParSysProx->SetLocation( this->ConvertLocalToGlobal( this->GetLocation() + this->GetParticleSystemOffset() ) );
}

Vector3 StartArea::GetScale() const
{
    return AreaEffect::GetScale();
}

void StartArea::Translate(const Vector3& Trans)
{
    LevelZone::Translate(Trans);
    this->ParSysProx->Translate(Trans);
}

void StartArea::Translate(const Real X, const Real Y, const Real Z)
{
    LevelZone::Translate(X,Y,Z);
    this->ParSysProx->Translate(X,Y,Z);
}

void StartArea::Yaw(const Real Angle)
{
    LevelZone::Yaw(Angle);
    this->ParSysProx->Yaw(Angle);
}

void StartArea::Pitch(const Real Angle)
{
    LevelZone::Pitch(Angle);
    this->ParSysProx->Pitch(Angle);
}

void StartArea::Roll(const Real Angle)
{
    LevelZone::Roll(Angle);
    this->ParSysProx->Roll(Angle);
}

void StartArea::Rotate(const Vector3& Axis, const Real Angle)
{
    LevelZone::Rotate(Axis,Angle);
    this->ParSysProx->Rotate(Axis,Angle);
}

void StartArea::Rotate(const Quaternion& Rotation)
{
    LevelZone::Rotate(Rotation);
    this->ParSysProx->Rotate(Rotation);
}

void StartArea::Scale(const Vector3& Scale)
{
    LevelZone::Scale(Scale);
    this->ParSysProx->Scale(Scale);
}

void StartArea::Scale(const Real X, const Real Y, const Real Z)
{
    LevelZone::Scale(X,Y,Z);
    this->ParSysProx->Scale(X,Y,Z);
}

///////////////////////////////////////////////////////////////////////////////
// Serialization

void StartArea::ProtoSerializeProperties(XML::Node& SelfRoot) const
{
    this->LevelZone::ProtoSerializeProperties(SelfRoot);
}

void StartArea::ProtoSerializeProxies(XML::Node& SelfRoot) const
{
    this->LevelZone::ProtoSerializeProxies(SelfRoot);
    XML::Node ProxiesNode = SelfRoot.AppendChild( StartArea::GetSerializableName() + "Proxies" );

    if( ProxiesNode.AppendAttribute("Version").SetValue("1") )
    {
        XML::Node ParSysProxNode = ProxiesNode.AppendChild("ParSysProx");
        this->ParSysProx->ProtoSerialize( ParSysProxNode );

        return;
    }else{
        SerializeError("Create XML Attribute Values",StartArea::GetSerializableName() + "Proxies",true);
    }
}

void StartArea::ProtoDeSerializeProperties(const XML::Node& SelfRoot)
{
    this->LevelZone::ProtoDeSerializeProperties(SelfRoot);
}

void StartArea::ProtoDeSerializeProxies(const XML::Node& SelfRoot)
{
    this->DestroyStartArea();
    this->LevelZone::ProtoDeSerializeProxies(SelfRoot);
    //XML::Attribute CurrAttrib;
    XML::Node ProxiesNode = SelfRoot.GetChild( StartArea::GetSerializableName() + "Proxies" );

    if( !ProxiesNode.Empty() ) {
        if(ProxiesNode.GetAttribute("Version").AsInt() == 1) {
            /// @todo I don't think an exception is appropriate for the failure of the worldmanager validity checks,
            /// however a warning should be written to the log if that happens.  This should be updated to do that once
            /// logging refactors are done.

            XML::Node ParSysProxNode = ProxiesNode.GetChild("ParSysProx").GetFirstChild();
            if( !ParSysProxNode.Empty() ) {
                Graphics::SceneManager* SceneMan = Entresol::GetSingletonPtr()->GetSceneManager();
                if( SceneMan ) {
                    this->ParSysProx = SceneMan->CreateParticleSystemProxy(ParSysProxNode);
                    this->ParSysProx->_Bind( this );
                }
            }
        }else{
            MEZZ_EXCEPTION(Exception::INVALID_VERSION_EXCEPTION,"Incompatible XML Version for " + (StartArea::GetSerializableName() + "Proxies" ) + ": Not Version 1.");
        }
    }else{
        MEZZ_EXCEPTION(Exception::II_IDENTITY_NOT_FOUND_EXCEPTION,StartArea::GetSerializableName() + "Proxies" + " was not found in the provided XML node, which was expected.");
    }
}

String StartArea::GetDerivedSerializableName() const
    { return StartArea::GetSerializableName(); }

String StartArea::GetSerializableName()
    { return "StartArea"; }

///////////////////////////////////////////////////////////////////////////////
// Internal Methods

void StartArea::_NotifyProxyDestroyed(WorldProxy* ToBeDestroyed)
{
    LevelZone::_NotifyProxyDestroyed(ToBeDestroyed);
    if( ToBeDestroyed == NULL )
        return;

    if( this->ParSysProx == ToBeDestroyed ) {
        this->ParSysProx = NULL;
    }
}

///////////////////////////////////////////////////////////////////////////////
// StartAreaFactory Methods

StartAreaFactory::StartAreaFactory()
    {  }

StartAreaFactory::~StartAreaFactory()
    {  }

String StartAreaFactory::GetTypeName() const
    { return StartArea::GetSerializableName(); }

StartArea* StartAreaFactory::CreateStartArea(const String& Name, World* TheWorld, const Vector3& HalfAreaSize)
    { return new StartArea(Name,TheWorld,HalfAreaSize); }

StartArea* StartAreaFactory::CreateStartArea(const XML::Node& XMLNode, World* TheWorld)
    { return static_cast<StartArea*>( this->CreateAreaEffect(XMLNode,TheWorld) ); }

AreaEffect* StartAreaFactory::CreateAreaEffect(const String& Name, World* TheWorld, const NameValuePairMap& Params)
{
    Vector3 HalfSize;
    NameValuePairMap::const_iterator ParamIt;
    ParamIt = Params.find( "SizeX" );
    if( ParamIt != Params.end() )
        HalfSize.X = StringTools::ConvertToReal( (*ParamIt).second );

    ParamIt = Params.find( "SizeY" );
    if( ParamIt != Params.end() )
        HalfSize.Y = StringTools::ConvertToReal( (*ParamIt).second );

    ParamIt = Params.find( "SizeZ" );
    if( ParamIt != Params.end() )
        HalfSize.Z = StringTools::ConvertToReal( (*ParamIt).second );

    return new StartArea(Name,TheWorld,HalfSize);
}

AreaEffect* StartAreaFactory::CreateAreaEffect(const XML::Node& XMLNode, World* TheWorld)
    { return new StartArea(XMLNode,TheWorld); }

void StartAreaFactory::DestroyAreaEffect(AreaEffect* ToBeDestroyed)
    { delete ToBeDestroyed; }

#endif
