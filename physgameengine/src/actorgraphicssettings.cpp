//© Copyright 2010 - 2011 BlackTopp Studios Inc.
/* This file is part of The PhysGame Engine.

    The PhysGame Engine is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The PhysGame Engine is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with The PhysGame Engine.  If not, see <http://www.gnu.org/licenses/>.
*/
/* The original authors have included a copy of the license specified above in the
   'Docs' folder. See 'gpl.txt'
*/
/* We welcome the use of The PhysGame anyone, including companies who wish to
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
#include "datatypes.h"

#include <map>

#include <Ogre.h>

namespace phys
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
            /// @brief The Actor this belongs to.
            ActorBase* Parent;

            /// @internal
            /// @brief Used for tracking ambient color on sub-meshes
            std::map<Whole, ColourValue> Ambient;

            /// @internal
            /// @brief Used for tracking Diffuse color on sub-meshes
            std::map<Whole, ColourValue> Diffuse;

            /// @internal
            /// @brief Used for tracking Specular color on sub-meshes
            std::map<Whole, ColourValue> Specular;

            InternalActorGraphicsSettings(ActorBase* Actor, Ogre::Entity* GraphicsObject)
              : Parent(Actor),
                ActorEnt(GraphicsObject)
            {
            }

        };
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Construction

    ActorGraphicsSettings::ActorGraphicsSettings(ActorBase* Actor, Ogre::Entity* GraphicsObject)
    {
        this->IAGS = new internal::InternalActorGraphicsSettings(Actor, GraphicsObject);
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
}
#ifdef PHYSXML
std::ostream& operator << (std::ostream& stream, const phys::ActorGraphicsSettings& Ev)
{
    stream      << "<ActorGraphicsSettings Version=\"1\">";
        for(phys::Whole Counter = Ev.GetNumSubmeshes(); Counter!=0; --Counter)
        {
            if(Ev.GetMaterialAmbient()!=phys::ColourValue())
                stream << "<AmbientMaterial Submesh=\"" << Counter << "\">" << Ev.GetMaterialAmbient() << "</AmbientMaterial>";

            if(Ev.GetMaterialSpecular()!=phys::ColourValue())
                stream << "<SpecularMaterial Submesh=\"" << Counter << "\">" << Ev.GetMaterialSpecular() << "</SpecularMaterial>";

            if(Ev.GetMaterialDiffuse()!=phys::ColourValue())
                stream << "<DiffuseMaterial Submesh=\"" << Counter << "\">" << Ev.GetMaterialDiffuse() << "</DiffuseMaterial>";
        }

    stream      << "</ActorGraphicsSettings>";
    return stream;
}

std::istream& PHYS_LIB operator >> (std::istream& stream, phys::ActorGraphicsSettings& Ev)
{
    phys::String OneTag( phys::xml::GetOneTag(stream) );
    std::auto_ptr<phys::xml::Document> Doc( phys::xml::PreParseClassFromSingleTag("phys::", "ActorGraphicsSettings", OneTag) );

    Doc->GetFirstChild() >> Ev;

    return stream;
}

phys::xml::Node& operator >> (const phys::xml::Node& OneNode, phys::ActorGraphicsSettings& Ev)
{
    if ( phys::String(OneNode.Name())==phys::String("ActorGraphicsSettings") )
    {
        if(OneNode.GetAttribute("Version").AsInt() == 1)
        {
            phys::ColourValue TempColour(0,0,0,0);

            for(phys::xml::Node Child = OneNode.GetFirstChild(); Child!=0; Child = Child.GetNextSibling())
            {
                phys::String Name(Child.Name());
                switch(Name[0])
                {
                    case 'A':   //fDiffuseColour
                        if(Name==phys::String("AmbientMaterial"))
                        {
                            Child.GetFirstChild() >> TempColour;
                            Ev.SetMaterialAmbient(TempColour, Child.GetAttribute("Submesh").AsWhole());
                        }else{
                            throw( phys::Exception(phys::StringCat("Incompatible XML Version for ActorGraphicsSettings: Includes unknown Element A-\"",Name,"\"")) );
                        }
                        break;
                    case 'S':   //fDiffuseColour
                        if(Name==phys::String("SpecularMaterial"))
                        {
                            Child.GetFirstChild() >> TempColour;
                            Ev.SetMaterialSpecular(TempColour, Child.GetAttribute("Submesh").AsWhole());
                        }else{
                            throw( phys::Exception(phys::StringCat("Incompatible XML Version for ActorGraphicsSettings: Includes unknown Element S-\"",Name,"\"")) );
                        }
                        break;
                    case 'D':   //fDiffuseColour
                        if(Name==phys::String("DiffuseMaterial"))
                        {
                            Child.GetFirstChild() >> TempColour;
                            Ev.SetMaterialDiffuse(TempColour, Child.GetAttribute("Submesh").AsWhole());
                        }else{
                            throw( phys::Exception(phys::StringCat("Incompatible XML Version for ActorGraphicsSettings: Includes unknown Element D-\"",Name,"\"")) );
                        }
                        break;
                    default:
                        throw( phys::Exception(phys::StringCat("Incompatible XML Version for ActorGraphicsSettings: Includes unknown Element default-\"",Name,"\"")) );
                        break;
                }
            }

        }else{
            throw( phys::Exception("Incompatible XML Version for ActorGraphicsSettings: Not Version 1"));
        }
    }else{
        throw( phys::Exception(phys::StringCat("Attempting to deserialize a ActorGraphicsSettings, found a ", OneNode.Name())));
    }
}
#endif // \PHYSXML


#endif
