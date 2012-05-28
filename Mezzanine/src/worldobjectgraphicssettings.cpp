//© Copyright 2010 - 2012 BlackTopp Studios Inc.
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
#ifndef _worldobjectgraphicssettings_cpp
#define _worldobjectgraphicssettings_cpp

#include "worldobjectgraphicssettings.h"
#include "actorbase.h"
#include "worldobject.h"
#include "objectreference.h"
#include "mesh.h"
#include "serialization.h"
#include "meshmanager.h"
#include "scenemanager.h"
#include "stringtool.h"

#include <map>
#include <memory>

#include <Ogre.h>
#include "btBulletDynamicsCommon.h"

namespace Mezzanine
{
    namespace Internal
    {
        /// @internal
        /// @brief Used to store internal data about an world objects.
        class InternalWorldObjectGraphicsSettings
        {
        public:

            /// @internal
            /// @brief Graphics Object of the world object.
            Ogre::Entity* WorldObjectEnt;

            /// @internal
            /// @brief Used for tracking ambient color on sub-meshes
            std::map<Whole, ColourValue> Ambient;

            /// @internal
            /// @brief Used for tracking Diffuse color on sub-meshes
            std::map<Whole, ColourValue> Diffuse;

            /// @internal
            /// @brief Used for tracking Specular color on sub-meshes
            std::map<Whole, ColourValue> Specular;

            InternalWorldObjectGraphicsSettings(Ogre::Entity* GraphicsObject)
              : WorldObjectEnt(GraphicsObject)
            {
            }

        };
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Construction

    WorldObjectGraphicsSettings::WorldObjectGraphicsSettings(WorldObject* WO, Ogre::Entity* GraphicsObject)
        : Parent(WO)
    {
        this->IWOGS = new Internal::InternalWorldObjectGraphicsSettings(GraphicsObject);
        if(IWOGS->WorldObjectEnt)
            WorldObjectMesh = MeshManager::GetSingletonPtr()->GetMesh(GraphicsObject->getMesh()->getName());
    }

    WorldObjectGraphicsSettings::~WorldObjectGraphicsSettings()
    {
        delete this->IWOGS;
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Mesh Management

    void WorldObjectGraphicsSettings::SetMesh(Mesh* ObjectMesh)
    {
        Ogre::SceneManager* OgreManager = SceneManager::GetSingletonPtr()->GetGraphicsWorldPointer();
        if(IWOGS->WorldObjectEnt)
        {
            OgreManager->destroyEntity(IWOGS->WorldObjectEnt);
            IWOGS->WorldObjectEnt = NULL;
        }
        this->WorldObjectMesh = ObjectMesh;

        if(!ObjectMesh)
            return;

        IWOGS->WorldObjectEnt = OgreManager->createEntity(Parent->Name,ObjectMesh->GetName(),ObjectMesh->GetGroup());
        Parent->GraphicsNode->setPosition((Parent->GetLocation()).GetOgreVector3());
        //Parent->GraphicsNode->attachObject(IWOGS->WorldObjectEnt);

        ObjectReference* AERef = (ObjectReference*)Parent->PhysicsObject->getUserPointer();
        Ogre::Any OgreRef(AERef);
        IWOGS->WorldObjectEnt->setUserAny(OgreRef);

        Parent->GraphicsObject = IWOGS->WorldObjectEnt;
    }

    void WorldObjectGraphicsSettings::SetMesh(const String& MeshName, const String& Group)
    {
        Mesh* TheMesh = MeshManager::GetSingletonPtr()->LoadMesh(MeshName,Group);
        this->SetMesh(TheMesh);
    }

    Mesh* WorldObjectGraphicsSettings::GetMesh() const
    {
        return WorldObjectMesh;
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Material Management

    Ogre::MaterialPtr WorldObjectGraphicsSettings::GetMaterial(const Whole& Submesh)
    {
        return static_cast<Ogre::MaterialPtr>(Ogre::MaterialManager::getSingleton().getByName(GetMaterialName(Submesh)));
    }

    void WorldObjectGraphicsSettings::SetMaterial(const String& MatName, const Whole& Submesh)
    {
        if(!IWOGS->WorldObjectEnt)
            return;
        this->IWOGS->WorldObjectEnt->getMesh()->getSubMesh(Submesh)->setMaterialName(MatName);
    }

    void WorldObjectGraphicsSettings::CloneMaterial(const String& NewName)
    {
        if(!IWOGS->WorldObjectEnt)
            return;
        this->GetMaterial()->clone(NewName);
    }

    ConstString WorldObjectGraphicsSettings::GetMaterialName(const Whole& Submesh) const
    {
        if(!IWOGS->WorldObjectEnt)
            return "";
        return this->IWOGS->WorldObjectEnt->getMesh()->getSubMesh(Submesh)->getMaterialName();
    }

    bool WorldObjectGraphicsSettings::HasMaterialSet(const Whole& Submesh)
    {
        if(!IWOGS->WorldObjectEnt)
            return false;
        return this->IWOGS->WorldObjectEnt->getMesh()->getSubMesh(Submesh)->isMatInitialised();
    }

    Whole WorldObjectGraphicsSettings::GetNumSubmeshes() const
    {
        if(!IWOGS->WorldObjectEnt)
            return 0;
        return this->IWOGS->WorldObjectEnt->getMesh()->getNumSubMeshes();
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Material Colors

    void WorldObjectGraphicsSettings::SetMaterialAmbient(const ColourValue& Ambient, const Whole& Submesh)
    {
        if(!IWOGS->WorldObjectEnt)
            return;
        this->IWOGS->Ambient[Submesh] = Ambient;
        GetMaterial(Submesh)->setAmbient(Ambient.GetOgreColourValue());
    }

    void WorldObjectGraphicsSettings::SetMaterialSpecular(const ColourValue& Specular, const Whole& Submesh)
    {
        if(!IWOGS->WorldObjectEnt)
            return;
        this->IWOGS->Specular[Submesh] = Specular;
        GetMaterial(Submesh)->setSpecular(Specular.GetOgreColourValue());
    }

    void WorldObjectGraphicsSettings::SetMaterialDiffuse(const ColourValue& Diffuse, const Whole& Submesh)
    {
        if(!IWOGS->WorldObjectEnt)
            return;
        this->IWOGS->Diffuse[Submesh] = Diffuse;
        GetMaterial(Submesh)->setDiffuse(Diffuse.GetOgreColourValue());
    }

    ColourValue WorldObjectGraphicsSettings::GetMaterialAmbient(const Whole& Submesh) const
    {
        if(!IWOGS->WorldObjectEnt)
            return ColourValue::Transparent();
        return this->IWOGS->Ambient[Submesh];
    }

    ColourValue WorldObjectGraphicsSettings::GetMaterialSpecular(const Whole& Submesh) const
    {
        if(!IWOGS->WorldObjectEnt)
            return ColourValue::Transparent();
        return this->IWOGS->Specular[Submesh];
    }

    ColourValue WorldObjectGraphicsSettings::GetMaterialDiffuse(const Whole& Submesh) const
    {
        if(!IWOGS->WorldObjectEnt)
            return ColourValue::Transparent();
        return this->IWOGS->Diffuse[Submesh];
    }

#ifdef MEZZXML
        // Serializable
        void WorldObjectGraphicsSettings::ProtoSerialize(XML::Node& CurrentRoot) const
        {
            XML::Node BaseNode = CurrentRoot.AppendChild(this->WorldObjectGraphicsSettings::SerializableName());
            if (!BaseNode)
                { SerializeError("Create BaseNode", SerializableName()); }

            Mezzanine::XML::Attribute Version = BaseNode.AppendAttribute("Version");                            // Version
            if (!Version)
                { SerializeError("Create Version Attribute", SerializableName()); }
            Version.SetValue(1);

            for(Mezzanine::Whole Counter = 0; this->GetNumSubmeshes()>Counter; ++Counter)
            {
                if(this->GetMaterialAmbient(Counter)!=Mezzanine::ColourValue())
                {
                    XML::Node AmbientMaterial = BaseNode.AppendChild("AmbientMaterial");
                    if (!AmbientMaterial)
                        { SerializeError("Create AmbientMaterial Node", SerializableName()); }
                    this->GetMaterialAmbient(Counter).ProtoSerialize(AmbientMaterial);
                    Mezzanine::XML::Attribute AmbientMaterialSubMesh = AmbientMaterial.AppendAttribute("Submesh");
                    if (!AmbientMaterialSubMesh)
                        { SerializeError("Create AmbientMaterialSubMesh Attribute", SerializableName()); }
                    AmbientMaterialSubMesh.SetValue(Counter);
                }

                if(this->GetMaterialSpecular(Counter)!=Mezzanine::ColourValue())
                {
                    XML::Node SpecularMaterial = BaseNode.AppendChild("SpecularMaterial");
                    if (!SpecularMaterial)
                        { SerializeError("Create SpecularMaterial Node", SerializableName()); }
                    this->GetMaterialSpecular(Counter).ProtoSerialize(SpecularMaterial);
                    Mezzanine::XML::Attribute SpecularMaterialSubMesh = SpecularMaterial.AppendAttribute("Submesh");
                    if (!SpecularMaterialSubMesh)
                        { SerializeError("Create SpecularMaterialSubMesh Attribute", SerializableName()); }
                    SpecularMaterialSubMesh.SetValue(Counter);
                }

                if(this->GetMaterialDiffuse(Counter)!=Mezzanine::ColourValue())
                {
                    XML::Node DiffuseMaterial = BaseNode.AppendChild("DiffuseMaterial");
                    if (!DiffuseMaterial)
                        { SerializeError("Create DiffuseMaterial Node", SerializableName()); }
                    this->GetMaterialDiffuse(Counter).ProtoSerialize(DiffuseMaterial);
                    Mezzanine::XML::Attribute DiffuseMaterialSubMesh = DiffuseMaterial.AppendAttribute("Submesh");
                    if (!DiffuseMaterialSubMesh)
                        { SerializeError("Create DiffuseMaterialSubMesh Attribute", SerializableName()); }
                    DiffuseMaterialSubMesh.SetValue(Counter);
                }
/*
                if(this->GetMaterialAmbient(Counter)!=Mezzanine::ColourValue())
                    stream << "<AmbientMaterial Submesh=\"" << Counter << "\">" << this->GetMaterialAmbient(Counter) << "</AmbientMaterial>";

                if(this->GetMaterialSpecular(Counter)!=Mezzanine::ColourValue())
                    stream << "<SpecularMaterial Submesh=\"" << Counter << "\">" << this->GetMaterialAmbient(Counter) << "</SpecularMaterial>";

                if(this->GetMaterialDiffuse(Counter)!=Mezzanine::ColourValue())
                    stream << "<DiffuseMaterial Submesh=\"" << Counter << "\">" << this->GetMaterialDiffuse(Counter) << "</DiffuseMaterial>";
*/
            }
        }

        // DeSerializable
        void WorldObjectGraphicsSettings::ProtoDeSerialize(const XML::Node& OneNode)
        {
            if ( Mezzanine::String(OneNode.Name())==this->WorldObjectGraphicsSettings::SerializableName() )
            {
                if(OneNode.GetAttribute("Version").AsInt() == 1)
                {
                    Mezzanine::ColourValue TempColour;

                    for(Mezzanine::Whole Counter = 0; this->GetNumSubmeshes()>Counter; ++Counter)
                    {
                        this->SetMaterialAmbient(TempColour, Counter);
                        this->SetMaterialSpecular(TempColour, Counter);
                        this->SetMaterialDiffuse(TempColour, Counter);
                    }

                    for(Mezzanine::XML::Node Child = OneNode.GetFirstChild(); Child!=0; Child = Child.GetNextSibling())
                    {
                        Mezzanine::String Name(Child.Name());
                        switch(Name[0])
                        {
                            case 'A':   //fDiffuseColour
                                if(Name==Mezzanine::String("AmbientMaterial"))
                                {
                                    Child.GetFirstChild() >> TempColour;
                                    this->SetMaterialAmbient(TempColour, Child.GetAttribute("Submesh").AsWhole());
                                }else{
                                    MEZZ_EXCEPTION(Mezzanine::Exception::INVALID_VERSION_EXCEPTION,"Incompatible XML Version for WorldObjectGraphicsSettings: Includes unknown Element A-\"" + Name + "\".");
                                }
                                break;
                            case 'S':   //fDiffuseColour
                                if(Name==Mezzanine::String("SpecularMaterial"))
                                {
                                    Child.GetFirstChild() >> TempColour;
                                    this->SetMaterialSpecular(TempColour, Child.GetAttribute("Submesh").AsWhole());
                                }else{
                                    MEZZ_EXCEPTION(Mezzanine::Exception::INVALID_VERSION_EXCEPTION,"Incompatible XML Version for WorldObjectGraphicsSettings: Includes unknown Element S-\"" + Name + "\".");
                                }
                                break;
                            case 'D':   //fDiffuseColour
                                if(Name==Mezzanine::String("DiffuseMaterial"))
                                {
                                    Child.GetFirstChild() >> TempColour;
                                    this->SetMaterialDiffuse(TempColour, Child.GetAttribute("Submesh").AsWhole());
                                }else{
                                    MEZZ_EXCEPTION(Mezzanine::Exception::INVALID_VERSION_EXCEPTION,"Incompatible XML Version for WorldObjectGraphicsSettings: Includes unknown Element D-\"" + Name + "\".");
                                }
                                break;
                            default:
                                { MEZZ_EXCEPTION(Mezzanine::Exception::INVALID_VERSION_EXCEPTION,"Incompatible XML Version for WorldObjectGraphicsSettings: Includes unknown Element default-\"" + Name + "\"."); }
                                break;
                        }
                    }
                }else{
                    MEZZ_EXCEPTION(Mezzanine::Exception::INVALID_VERSION_EXCEPTION,"Incompatible XML Version for" + WorldObjectGraphicsSettings::SerializableName() + ": Not Version 1.");
                }
            }else{
                MEZZ_EXCEPTION(Mezzanine::Exception::II_IDENTITY_INVALID_EXCEPTION,"Attempting to deserialize a " + WorldObjectGraphicsSettings::SerializableName() + ", found a " + Mezzanine::String(OneNode.Name()) + ".");
            }
        }

        String WorldObjectGraphicsSettings::SerializableName()
            { return String("WorldObjectGraphicsSettings"); }
#endif
}


#ifdef MEZZXML
std::ostream& MEZZ_LIB operator << (std::ostream& stream, const Mezzanine::WorldObjectGraphicsSettings& Ev)
    { return Serialize(stream, Ev); }

std::istream& MEZZ_LIB operator >> (std::istream& stream, Mezzanine::WorldObjectGraphicsSettings& Ev)
    { return DeSerialize(stream,Ev); }

Mezzanine::XML::Node& MEZZ_LIB operator >> (const Mezzanine::XML::Node& OneNode, Mezzanine::WorldObjectGraphicsSettings& Ev)
    { Ev.ProtoDeSerialize(OneNode); }


#endif // \MEZZXML


#endif
