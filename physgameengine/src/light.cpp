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
#ifndef _light_cpp
#define _light_cpp

#include "light.h"
#include "scenemanager.h"
#include "stringtool.h"
#include "world.h"
#include "worldnode.h"

#include <Ogre.h>

#include <memory>

/// @file light.cpp
/// @brief Implemenation of phys::Light

namespace phys
{
    ///////////////////////////////////////////////////////////////////////////////
    // Construction

    Light::Light(const String& Name, SceneManager* manager)
    {
        Manager = manager;
        OgreLight = Manager->GetGraphicsWorldPointer()->createLight(Name);
    }

    Light::Light(Ogre::Light* light, SceneManager* manager)
    {
        OgreLight = light;
        Manager = manager;
    }

    Light::~Light()
        { Manager->GetGraphicsWorldPointer()->destroyLight(OgreLight); }

    ///////////////////////////////////////////////////////////////////////////////
    // Meta

    ConstString& Light::GetName() const
        { return OgreLight->getName(); }

    void Light::SetType(Light::LightType Type)
    {
        Ogre::Light::LightTypes OgreType;
        switch (Type)
        {
            case Light::Point:
                OgreType = Ogre::Light::LT_POINT;
                break;
            case Light::Directional:
                OgreType = Ogre::Light::LT_DIRECTIONAL;
                break;
            case Light::Spotlight:
                OgreType = Ogre::Light::LT_SPOTLIGHT;
                break;
            default:
                return;
        }
        OgreLight->setType(OgreType);
    }


    Light::LightType Light::GetType() const
    {
        Ogre::Light::LightTypes OgreType = OgreLight->getType();
        Light::LightType PhysType;
        switch (OgreType)
        {
            case Ogre::Light::LT_POINT:
                PhysType = Light::Point;
                break;
            case Ogre::Light::LT_DIRECTIONAL:
                PhysType = Light::Directional;
                break;
            case Ogre::Light::LT_SPOTLIGHT:
                PhysType = Light::Spotlight;
                break;
            default:
                return Light::Point;
        }
        return PhysType;
    }

    ///////////////////////////////////////////////////////////////////////////////
    // 3d navigation

    void Light::SetLocation(const Vector3 &Location)
        { OgreLight->setPosition(Location.GetOgreVector3()); }

    void Light::SetOrientation(const Quaternion& Orientation)
        {}

    void Light::SetDirection(const Vector3& Direction)
        { OgreLight->setDirection(Direction.GetOgreVector3()); }

    Vector3 Light::GetLocation() const
        { return Vector3(OgreLight->getPosition()); }

    Quaternion Light::GetOrientation() const
        { return Quaternion(); }

    Vector3 Light::GetDirection() const
        { return Vector3(OgreLight->getDirection()); }

    ///////////////////////////////////////////////////////////////////////////////
    // Colour

    void Light::SetDiffuseColour(Real Red, Real Green, Real Blue)
        { OgreLight->setDiffuseColour(Red, Green, Blue); }

    void Light::SetDiffuseColour(const ColourValue &Colour)
        { OgreLight->setDiffuseColour(Colour.GetOgreColourValue()); }

    void Light::SetSpecularColour(Real Red, Real Green, Real Blue)
        { OgreLight->setSpecularColour(Red, Green, Blue); }

    void Light::SetSpecularColour(const ColourValue &Colour)
        { OgreLight->setSpecularColour(Colour.GetOgreColourValue()); }

    ColourValue Light::GetDiffuseColour() const
        { return ColourValue(OgreLight->getDiffuseColour()); }

    ColourValue Light::GetSpecularColour() const
        { return ColourValue(OgreLight->getSpecularColour()); }

    ///////////////////////////////////////////////////////////////////////////////
    // Attenuation

    void Light::SetAttenuation(Real Range, Real Constant, Real Linear, Real Quadratic)
        { OgreLight->setAttenuation(Range, Constant, Linear, Quadratic); }

    Real Light::GetAttenuationRange() const
        { return OgreLight->getAttenuationRange(); }

    Real Light::GetAttenuationConstant() const
        { return OgreLight->getAttenuationConstant(); }

    Real Light::GetAttenuationLinear() const
        { return OgreLight->getAttenuationLinear(); }

    Real Light::GetAttenuationQuadric() const
        { return OgreLight->getAttenuationQuadric(); }

    ///////////////////////////////////////////////////////////////////////////////
    // Spotlight specific methods

    void Light::SetSpotlightRange(Real InnerAngle, Real OuterAngle, Real Falloff)
    {
        Ogre::Radian IAngle(InnerAngle);
        Ogre::Radian OAngle(OuterAngle);
        OgreLight->setSpotlightRange(IAngle, OAngle, Falloff);
    }

    void Light::SetSpotlightInnerAngle(Real InnerAngle)
    {
        Ogre::Radian IAngle(InnerAngle);
        OgreLight->setSpotlightInnerAngle(IAngle);
    }

    void Light::SetSpotlightOuterAngle(Real OuterAngle)
    {
        Ogre::Radian OAngle(OuterAngle);
        OgreLight->setSpotlightOuterAngle(OAngle);
    }

    void Light::SetSpotlightFalloff(Real Falloff)
        { OgreLight->setSpotlightFalloff(Falloff); }

    Real Light::GetSpotlightInnerAngle() const
        { return OgreLight->getSpotlightInnerAngle().valueRadians(); }

    Real Light::GetSpotlightOuterAngle() const
        { return OgreLight->getSpotlightOuterAngle().valueRadians(); }

    Real Light::GetSpotlightFalloff() const
        { return OgreLight->getSpotlightFalloff(); }

    ///////////////////////////////////////////////////////////////////////////////
    // Effects

    Real Light::GetPowerScale() const
        { return OgreLight->getPowerScale(); }

    void Light::SetPowerScale(Real Power)
        { OgreLight->setPowerScale(Power); }

    ///////////////////////////////////////////////////////////////////////////////
    // Inherited From Attachable

    Attachable::AttachableElement Light::GetAttachableType() const
        { return Attachable::Light; }

    ///////////////////////////////////////////////////////////////////////////////
    // Internal Functions

    AttachableData Light::GetAttachableData() const
    {
        AttachableData Data;
        Data.OgreMovable = OgreLight;
        Data.Type = Attachable::Light;
        return Data;
    }
}//phys

///////////////////////////////////////////////////////////////////////////////
// Class External << Operators for streaming or assignment
#ifdef PHYSXML
std::ostream& operator << (std::ostream& stream, const phys::Light& Ev)
{
    stream      << "<Light Version=\"1\" Name=\"" << Ev.GetName()
                    << "\" AttachedTo=\"" << ( Ev.GetAttachedTo() ? Ev.GetAttachedTo()->GetName() : "" )
                    << "\" Type=\"" << Ev.GetType()
                    << "\" PowerScale=\"" << Ev.GetPowerScale()
                    << "\" AttenuationRange=\"" << Ev.GetAttenuationRange()
                    << "\" AttenuationConstant=\"" << Ev.GetAttenuationConstant()
                    << "\" AttenuationQuadric=\"" << Ev.GetAttenuationQuadric()
                    << "\" AttenuationLinear=\"" << Ev.GetAttenuationLinear()
                    << "\" SpotlightInnerAngle=\"" << Ev.GetSpotlightInnerAngle()
                    << "\" SpotlightOuterAngle=\"" << Ev.GetSpotlightOuterAngle()
                    << "\" SpotlightFalloff=\"" << Ev.GetSpotlightFalloff()
                << "\">"
                << "<Direction>" << Ev.GetDirection() << "</Direction>"
                << "<Location>" << Ev.GetLocation() << "</Location>"
                << "<SpecularColour>" << Ev.GetSpecularColour() << "</SpecularColour>"
                << "<fDiffuseColour>" << Ev.GetDiffuseColour() << "</fDiffuseColour>"
                << "</Light>";
    return stream;
}

std::istream& PHYS_LIB operator >> (std::istream& stream, phys::Light& Ev)
{
    phys::String OneTag( phys::xml::GetOneTag(stream) );
    std::auto_ptr<phys::xml::Document> Doc( phys::xml::PreParseClassFromSingleTag("phys::", "Light", OneTag) );

    Doc->GetFirstChild() >> Ev;

    return stream;
}

phys::xml::Node& operator >> (const phys::xml::Node& OneNode, phys::Light& Ev)
{
    if ( phys::String(OneNode.Name())==phys::String("Light") )
    {
        if(OneNode.GetAttribute("Version").AsInt() == 1)
        {
            Ev.SetType(static_cast<phys::Light::LightType>(OneNode.GetAttribute("Type").AsInt()));
            Ev.SetPowerScale(OneNode.GetAttribute("PowerScale").AsReal());
            Ev.SetAttenuation(OneNode.GetAttribute("AttenuationRange").AsReal(), OneNode.GetAttribute("AttenuationConstant").AsReal(), OneNode.GetAttribute("AttenuationLinear").AsReal(), OneNode.GetAttribute("AttenuationQuadric").AsReal());
            Ev.SetSpotlightInnerAngle(OneNode.GetAttribute("SpotlightInnerAngle").AsReal());
            Ev.SetSpotlightOuterAngle(OneNode.GetAttribute("SpotlightOuterAngle").AsReal());
            Ev.SetSpotlightFalloff(OneNode.GetAttribute("SpotlightFalloff").AsReal());
            phys::WorldNode * AttachPtr = phys::World::GetWorldPointer()->GetSceneManager()->GetNode( OneNode.GetAttribute("AttachedTo").AsString() );
            if (AttachPtr)
                { AttachPtr->AttachObject(&Ev); }

            phys::ColourValue TempColour(0,0,0,0);
            phys::Vector3 TempVec(0,0,0);

            for(phys::xml::Node Child = OneNode.GetFirstChild(); Child!=0; Child = Child.GetNextSibling())
            {
                phys::String Name(Child.Name());
                switch(Name[0])
                {
                    case 'f':   //fDiffuseColour
                        if(Name==phys::String("fDiffuseColour"))
                        {
                            Child.GetFirstChild() >> TempColour;
                            Ev.SetDiffuseColour(TempColour);
                        }else{
                            throw( phys::Exception(phys::StringTool::StringCat("Incompatible XML Version for Light: Includes unknown Element f-\"",Name,"\"")) );
                        }
                        break;
                    case 'S':   //SpecularColour
                        if(Name==phys::String("SpecularColour"))
                        {
                            Child.GetFirstChild() >> TempColour;
                            Ev.SetSpecularColour(TempColour);
                        }else{
                            throw( phys::Exception(phys::StringTool::StringCat("Incompatible XML Version for Light: Includes unknown Element S-\"",Name,"\"")) );
                        }
                        break;
                    case 'D':   //Direction
                        if(Name==phys::String("Direction"))
                        {
                            Child.GetFirstChild() >> TempVec;
                            Ev.SetDirection(TempVec);
                        }else{
                            throw( phys::Exception(phys::StringTool::StringCat("Incompatible XML Version for Light: Includes unknown Element D-\"",Name,"\"")) );
                        }
                        break;
                    case 'L':   //Location
                        if(Name==phys::String("Location"))
                        {
                            Child.GetFirstChild() >> TempVec;
                            Ev.SetLocation(TempVec);
                        }else{
                            throw( phys::Exception(phys::StringTool::StringCat("Incompatible XML Version for Light: Includes unknown Element L-\"",Name,"\"")) );
                        }
                        break;
                    default:
                        throw( phys::Exception(phys::StringTool::StringCat("Incompatible XML Version for Light: Includes unknown Element default-\"",Name,"\"")) );
                        break;
                }
            }

        }else{
            throw( phys::Exception("Incompatible XML Version for Light: Not Version 1"));
        }
    }else{
        throw( phys::Exception(phys::StringTool::StringCat("Attempting to deserialize a Light, found a ", OneNode.Name())));
    }
}
#endif // \PHYSXML


#endif
