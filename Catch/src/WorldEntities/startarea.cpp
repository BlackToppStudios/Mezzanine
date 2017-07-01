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
    Graphics::ParticleSystemProxy* ParSysProx = NULL;
    Graphics::SceneManager* SceneMan = static_cast<Graphics::SceneManager*>( this->ParentWorld->GetManager(ManagerBase::MT_SceneManager) );

    if( SceneMan != NULL ) {
        ParSysProx = SceneMan->CreateParticleSystemProxy("StartVortex");
        this->AddComponent(ParSysProx);

        Graphics::ParticleEmitter* ParSysEmit = ParSysProx->GetEmitter(0);
        ParSysEmit->SetCustomParam( "width",StringTools::ConvertToString( HalfAreaSize.X * Real(1.8) ) );
        ParSysEmit->SetCustomParam( "height",StringTools::ConvertToString( HalfAreaSize.Y * Real(0.6) ) );
        ParSysEmit->SetCustomParam( "depth",StringTools::ConvertToString( HalfAreaSize.Z * Real(1.8) ) );
        ParSysProx->SetLocation(this->GetParticleSystemOffset());
    }
}

void StartArea::DestroyStartArea()
{

}

Vector3 StartArea::GetParticleSystemOffset() const
{
    Graphics::ParticleSystemProxy* ParSysProx = this->GetParticleSystemProxy();
    if( ParSysProx != NULL ) {
        Real ParSysHeight = StringTools::ConvertToReal( ParSysProx->GetEmitter(0)->GetCustomParam( "height" ) );
        return Vector3( 0, ( -this->ZoneSize.Y ) + ( ParSysHeight * 0.5 ), 0 );
    }else{
        return Vector3();
    }
}

///////////////////////////////////////////////////////////////////////////////
// Utility

Graphics::ParticleSystemProxy* StartArea::GetParticleSystemProxy() const
    { return static_cast<Graphics::ParticleSystemProxy*>( this->GetComponent(Mezzanine::CT_Graphics_ParticleSystemProxy,0) ); }

void StartArea::SetParticleMinimumTimeToLive(const Real MinTTL)
{
    Graphics::ParticleSystemProxy* ParSysProx = this->GetParticleSystemProxy();
    if( ParSysProx != NULL )
        ParSysProx->GetEmitter(0)->SetCustomParam( "time_to_live_min",StringTools::ConvertToString( MinTTL ) );
}

Real StartArea::GetParticleMinimumTimeToLive() const
{
    Graphics::ParticleSystemProxy* ParSysProx = this->GetParticleSystemProxy();
    if( ParSysProx != NULL )
        return StringTools::ConvertToReal( ParSysProx->GetEmitter(0)->GetCustomParam( "time_to_live_min" ) );
    else
        return 0;
}

void StartArea::SetParticleMaximumTimeToLive(const Real MaxTTL)
{
    Graphics::ParticleSystemProxy* ParSysProx = this->GetParticleSystemProxy();
    if( ParSysProx != NULL )
        ParSysProx->GetEmitter(0)->SetCustomParam( "time_to_live_max",StringTools::ConvertToString( MaxTTL ) );
}

Real StartArea::GetParticleMaximumTimeToLive() const
{
    Graphics::ParticleSystemProxy* ParSysProx = this->GetParticleSystemProxy();
    if( ParSysProx != NULL )
        return StringTools::ConvertToReal( ParSysProx->GetEmitter(0)->GetCustomParam( "time_to_live_max" ) );
    else
        return 0;
}

void StartArea::ApplyEffect()
{
    static const Vector3 ZeroGrav(0,0,0);
    AreaEffect::ApplyEffect();
    if( !this->AddedObjects.empty() ) {
        for( ObjectIterator ObjIt = this->AddedObjects.begin() ; ObjIt != this->AddedObjects.end() ; ObjIt++ )
        {
            ComponentContainer ColProxies = (*ObjIt)->GetComponents(Mezzanine::CT_Physics_RigidProxy,Mezzanine::CT_Physics_SoftProxy);
            for( ComponentIterator ProxIt = ColProxies.begin() ; ProxIt != ColProxies.end() ; ++ProxIt )
            {
                static_cast<Physics::CollidableProxy*>( *ProxIt )->SetCollisionResponse(false);
                if( (*ProxIt)->GetComponentType() == Mezzanine::CT_Physics_RigidProxy ) {
                    static_cast<Physics::RigidProxy*>( *ProxIt )->SetGravity( ZeroGrav );
                }
            }
        }
    }

    if( !this->RemovedObjects.empty() ) {
        const Vector3 WorldGrav = static_cast<Physics::PhysicsManager*>( this->ParentWorld->GetManager(ManagerBase::MT_PhysicsManager) )->GetWorldGravity();
        for( ObjectIterator ObjIt = this->RemovedObjects.begin() ; ObjIt != this->RemovedObjects.end() ; ObjIt++ )
        {
            ComponentContainer ColProxies = (*ObjIt)->GetComponents(Mezzanine::CT_Physics_RigidProxy,Mezzanine::CT_Physics_SoftProxy);
            for( ComponentIterator ProxIt = ColProxies.begin() ; ProxIt != ColProxies.end() ; ++ProxIt )
            {
                static_cast<Physics::CollidableProxy*>( *ProxIt )->SetCollisionResponse(true);
                if( (*ProxIt)->GetComponentType() == Mezzanine::CT_Physics_RigidProxy ) {
                    static_cast<Physics::RigidProxy*>( *ProxIt )->SetGravity( WorldGrav );
                }
            }
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
// Working with the World

///////////////////////////////////////////////////////////////////////////////
// LevelZone Properties

///////////////////////////////////////////////////////////////////////////////
// Serialization

void StartArea::ProtoSerializeProperties(XML::Node& SelfRoot) const
{
    this->LevelZone::ProtoSerializeProperties(SelfRoot);
}

void StartArea::ProtoDeSerializeProperties(const XML::Node& SelfRoot)
{
    this->LevelZone::ProtoDeSerializeProperties(SelfRoot);
}

String StartArea::GetDerivedSerializableName() const
    { return StartArea::GetSerializableName(); }

String StartArea::GetSerializableName()
    { return "StartArea"; }

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
