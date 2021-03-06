#ifndef _levelzone_cpp
#define _levelzone_cpp

#include "levelzone.h"
#include "catchapp.h"

#include <mezzanine.h>

#include <algorithm>

///////////////////////////////////////////////////////////////////////////////
// LevelZone Methods

LevelZone::LevelZone(World* TheWorld) :
    AreaEffect(TheWorld)
    {  }

LevelZone::LevelZone(const EntityID& EntID, World* TheWorld, const Vector3& HalfZoneSize) :
    AreaEffect(EntID,TheWorld),
    ZoneSize(HalfZoneSize)
    { this->CreateLevelZone(); }

LevelZone::~LevelZone()
    { this->DestroyLevelZone(); }

void LevelZone::CreateLevelZone()
{
    Graphics::ItemProxy* ItemProx = NULL;
    Graphics::SceneManager* SceneMan = static_cast<Graphics::SceneManager*>( this->ParentWorld->GetManager(ManagerBase::MT_SceneManager) );
    if( SceneMan != NULL ) {
        ItemProx = SceneMan->CreateItemProxy();
        this->AddComponent(ItemProx);
    }
}

void LevelZone::DestroyLevelZone()
{

}

///////////////////////////////////////////////////////////////////////////////
// Utility

Mezzanine::EntityType LevelZone::GetEntityType() const
    { return Mezzanine::ET_AreaEffectUnknown; }

Graphics::ItemProxy* LevelZone::GetItemProxy() const
    { return static_cast<Graphics::ItemProxy*>( this->GetComponent(Mezzanine::CT_Graphics_ItemProxy,0) ); }

Boole LevelZone::IsInside(Entity* Object)
    { return ( std::find( this->OverlappingObjects.begin(), this->OverlappingObjects.end(), Object ) != this->OverlappingObjects.end() ); }

Boole LevelZone::IsEmpty()
    { return this->OverlappingObjects.empty(); }

///////////////////////////////////////////////////////////////////////////////
// Working with the World

///////////////////////////////////////////////////////////////////////////////
// LevelZone Properties

Vector3 LevelZone::GetZoneSize() const
    { return this->ZoneSize; }

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
            MEZZ_EXCEPTION(ExceptionBase::INVALID_VERSION_EXCEPTION,"Incompatible XML Version for " + (LevelZone::GetSerializableName() + "Properties" ) + ": Not Version 1.");
        }
    }else{
        MEZZ_EXCEPTION(ExceptionBase::II_IDENTITY_NOT_FOUND_EXCEPTION,LevelZone::GetSerializableName() + "Properties" + " was not found in the provided XML node, which was expected.");
    }
}

String LevelZone::GetDerivedSerializableName() const
    { return LevelZone::GetSerializableName(); }

String LevelZone::GetSerializableName()
    { return "LevelZone"; }

#endif
