//© Copyright 2010 - 2011 BlackTopp Studios Inc.
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
#ifndef _actorgraphicssettings_cpp
#define _actorgraphicssettings_cpp

#include "actorgraphicssettings.h"
#include "actorbase.h"
#include "mesh.h"
#include "datatypes.h"
#include "serialization.h"
#include "world.h"
#include "meshmanager.h"
#include "stringtool.h"

#include <map>

#include <memory>

#include <Ogre.h>

namespace Mezzanine
{
    namespace internal
    {
        /// @internal
        /// @brief Used to store internal data about an actors.
        class InternalActorGraphicsSettings
        {
        public:

            /// @internal
            /// @brief Graphics Object of the actor.
            Ogre::Entity* ActorEnt;

            /// @internal
            /// @brief Used for tracking ambient color on sub-meshes
            std::map<Whole, ColourValue> Ambient;

            /// @internal
            /// @brief Used for tracking Diffuse color on sub-meshes
            std::map<Whole, ColourValue> Diffuse;

            /// @internal
            /// @brief Used for tracking Specular color on sub-meshes
            std::map<Whole, ColourValue> Specular;

            InternalActorGraphicsSettings(Ogre::Entity* GraphicsObject)
              : ActorEnt(GraphicsObject)
            {
            }

        };
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Construction

    ActorGraphicsSettings::ActorGraphicsSettings(ActorBase* Actor, Ogre::Entity* GraphicsObject)
        : Parent(Actor)
    {
        this->IAGS = new internal::InternalActorGraphicsSettings(GraphicsObject);
        ActorMesh = MeshManager::GetSingletonPtr()->GetMesh(GraphicsObject->getMesh()->getName());
    }

    ActorGraphicsSettings::~ActorGraphicsSettings()
    {
        delete this->IAGS;
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Material Management

    Ogre::MaterialPtr ActorGraphicsSettings::GetMaterial(Whole Submesh)
    {
        return static_cast<Ogre::MaterialPtr>(Ogre::MaterialManager::getSingleton().getByName(GetMaterialName(Submesh)));
    }

    void ActorGraphicsSettings::SetMaterial(String MatName, Whole Submesh)
    {
        this->IAGS->ActorEnt->getMesh()->getSubMesh(Submesh)->setMaterialName(MatName);
    }

    void ActorGraphicsSettings::CloneMaterial(const String& newName)
    {
         this->GetMaterial()->clone(newName);
    }

    ConstString& ActorGraphicsSettings::GetMaterialName(Whole Submesh) const
    {
        return this->IAGS->ActorEnt->getMesh()->getSubMesh(Submesh)->getMaterialName();
    }

    bool ActorGraphicsSettings::HasMaterialSet(Whole Submesh)
    {
        return this->IAGS->ActorEnt->getMesh()->getSubMesh(Submesh)->isMatInitialised();
    }

    Whole ActorGraphicsSettings::GetNumSubmeshes() const
    {
        return this->IAGS->ActorEnt->getMesh()->getNumSubMeshes();
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Material Colors

    void ActorGraphicsSettings::SetMaterialAmbient(const ColourValue& Ambient, Whole Submesh)
    {
        this->IAGS->Ambient[Submesh] = Ambient;
        GetMaterial(Submesh)->setAmbient(Ambient.GetOgreColourValue());
    }

    void ActorGraphicsSettings::SetMaterialSpecular(const ColourValue& Specular, Whole Submesh)
    {
        this->IAGS->Specular[Submesh] = Specular;
        GetMaterial(Submesh)->setSpecular(Specular.GetOgreColourValue());
    }

    void ActorGraphicsSettings::SetMaterialDiffuse(const ColourValue& Diffuse, Whole Submesh)
    {
        this->IAGS->Diffuse[Submesh] = Diffuse;
        GetMaterial(Submesh)->setDiffuse(Diffuse.GetOgreColourValue());
    }

    ColourValue ActorGraphicsSettings::GetMaterialAmbient(Whole Submesh) const
        { return this->IAGS->Ambient[Submesh]; }

    ColourValue ActorGraphicsSettings::GetMaterialSpecular(Whole Submesh) const
        { return this->IAGS->Specular[Submesh]; }

    ColourValue ActorGraphicsSettings::GetMaterialDiffuse(Whole Submesh) const
        { return this->IAGS->Diffuse[Submesh]; }

#ifdef MEZZXML
        // Serializable
        void ActorGraphicsSettings::ProtoSerialize(xml::Node& CurrentRoot) const
        {
            xml::Node BaseNode = CurrentRoot.AppendChild(this->ActorGraphicsSettings::SerializableName());
            if (!BaseNode)
                { SerializeError("Create BaseNode", SerializableName()); }

            Mezzanine::xml::Attribute Version = BaseNode.AppendAttribute("Version");                            // Version
            if (!Version)
                { SerializeError("Create Version Attribute", SerializableName()); }
            Version.SetValue(1);

            for(Mezzanine::Whole Counter = 0; this->GetNumSubmeshes()>Counter; ++Counter)
            {
                if(this->GetMaterialAmbient(Counter)!=Mezzanine::ColourValue())
                {
                    xml::Node AmbientMaterial = BaseNode.AppendChild("AmbientMaterial");
                    if (!AmbientMaterial)
                        { SerializeError("Create AmbientMaterial Node", SerializableName()); }
                    this->GetMaterialAmbient(Counter).ProtoSerialize(AmbientMaterial);
                    Mezzanine::xml::Attribute AmbientMaterialSubMesh = AmbientMaterial.AppendAttribute("Submesh");
                    if (!AmbientMaterialSubMesh)
                        { SerializeError("Create AmbientMaterialSubMesh Attribute", SerializableName()); }
                    AmbientMaterialSubMesh.SetValue(Counter);
                }

                if(this->GetMaterialSpecular(Counter)!=Mezzanine::ColourValue())
                {
                    xml::Node SpecularMaterial = BaseNode.AppendChild("SpecularMaterial");
                    if (!SpecularMaterial)
                        { SerializeError("Create SpecularMaterial Node", SerializableName()); }
                    this->GetMaterialSpecular(Counter).ProtoSerialize(SpecularMaterial);
                    Mezzanine::xml::Attribute SpecularMaterialSubMesh = SpecularMaterial.AppendAttribute("Submesh");
                    if (!SpecularMaterialSubMesh)
                        { SerializeError("Create SpecularMaterialSubMesh Attribute", SerializableName()); }
                    SpecularMaterialSubMesh.SetValue(Counter);
                }

                if(this->GetMaterialDiffuse(Counter)!=Mezzanine::ColourValue())
                {
                    xml::Node DiffuseMaterial = BaseNode.AppendChild("DiffuseMaterial");
                    if (!DiffuseMaterial)
                        { SerializeError("Create DiffuseMaterial Node", SerializableName()); }
                    this->GetMaterialDiffuse(Counter).ProtoSerialize(DiffuseMaterial);
                    Mezzanine::xml::Attribute DiffuseMaterialSubMesh = DiffuseMaterial.AppendAttribute("Submesh");
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
        void ActorGraphicsSettings::ProtoDeSerialize(const xml::Node& OneNode)
        {
            if ( Mezzanine::String(OneNode.Name())==this->ActorGraphicsSettings::SerializableName() )
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

                    for(Mezzanine::xml::Node Child = OneNode.GetFirstChild(); Child!=0; Child = Child.GetNextSibling())
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
                                    throw( Mezzanine::Exception(StringTool::StringCat("Incompatible XML Version for ActorGraphicsSettings: Includes unknown Element A-\"",Name,"\"")) );
                                }
                                break;
                            case 'S':   //fDiffuseColour
                                if(Name==Mezzanine::String("SpecularMaterial"))
                                {
                                    Child.GetFirstChild() >> TempColour;
                                    this->SetMaterialSpecular(TempColour, Child.GetAttribute("Submesh").AsWhole());
                                }else{
                                    throw( Mezzanine::Exception(StringTool::StringCat("Incompatible XML Version for ActorGraphicsSettings: Includes unknown Element S-\"",Name,"\"")) );
                                }
                                break;
                            case 'D':   //fDiffuseColour
                                if(Name==Mezzanine::String("DiffuseMaterial"))
                                {
                                    Child.GetFirstChild() >> TempColour;
                                    this->SetMaterialDiffuse(TempColour, Child.GetAttribute("Submesh").AsWhole());
                                }else{
                                    throw( Mezzanine::Exception(StringTool::StringCat("Incompatible XML Version for ActorGraphicsSettings: Includes unknown Element D-\"",Name,"\"")) );
                                }
                                break;
                            default:
                                throw( Mezzanine::Exception(StringTool::StringCat("Incompatible XML Version for ActorGraphicsSettings: Includes unknown Element default-\"",Name,"\"")) );
                                break;
                        }
                    }
                }else{
                    throw( Mezzanine::Exception(String("Incompatible XML Version for")+ this->ActorGraphicsSettings::SerializableName() + ": Not Version 1"));
                }
            }else{
                throw( Mezzanine::Exception(StringTool::StringCat("Attempting to deserialize a ", this->ActorGraphicsSettings::SerializableName(),", found a ", OneNode.Name())));
            }
        }

        String ActorGraphicsSettings::SerializableName()
            { return String("ActorGraphicsSettings"); }
#endif
}


#ifdef MEZZXML
std::ostream& MEZZ_LIB operator << (std::ostream& stream, const Mezzanine::ActorGraphicsSettings& Ev)
    { return Serialize(stream, Ev); }

std::istream& MEZZ_LIB operator >> (std::istream& stream, Mezzanine::ActorGraphicsSettings& Ev)
    { return DeSerialize(stream,Ev); }

Mezzanine::xml::Node& MEZZ_LIB operator >> (const Mezzanine::xml::Node& OneNode, Mezzanine::ActorGraphicsSettings& Ev)
    { Ev.ProtoDeSerialize(OneNode); }


#endif // \MEZZXML


#endif
