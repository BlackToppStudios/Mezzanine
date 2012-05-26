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
/// @brief Implemenation of Mezzanine::Light

namespace Mezzanine
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

    void Light::SetLightType(Light::LightType Type)
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


    Light::LightType Light::GetLightType() const
    {
        Ogre::Light::LightTypes OgreType = OgreLight->getType();
        Light::LightType MezzType;
        switch (OgreType)
        {
            case Ogre::Light::LT_POINT:
                MezzType = Light::Point;
                break;
            case Ogre::Light::LT_DIRECTIONAL:
                MezzType = Light::Directional;
                break;
            case Ogre::Light::LT_SPOTLIGHT:
                MezzType = Light::Spotlight;
                break;
            default:
                return Light::Point;
        }
        return MezzType;
    }

    ///////////////////////////////////////////////////////////////////////////////
    // 3d navigation

    void Light::SetLocation(const Vector3 &Location)
    {
        OgreLight->setPosition(Location.GetOgreVector3());
        LocalTransformDirty = !(GetUpdating(Parent));

        _RecalculateLocalTransform();
    }

    Vector3 Light::GetLocation() const
        { return Vector3(OgreLight->getPosition()); }

    void Light::SetOrientation(const Quaternion& Orientation)
        { /*Nothing to do*/ }

    Quaternion Light::GetOrientation() const
        { return Quaternion(); }

    void Light::SetDirection(const Vector3& Direction)
        { OgreLight->setDirection(Direction.GetOgreVector3()); }

    Vector3 Light::GetDirection() const
        { return Vector3(OgreLight->getDirection()); }

    ///////////////////////////////////////////////////////////////////////////////
    // Colour

    void Light::SetDiffuseColour(const Real& Red, const Real& Green, const Real& Blue)
        { OgreLight->setDiffuseColour(Red, Green, Blue); }

    void Light::SetDiffuseColour(const ColourValue &Colour)
        { OgreLight->setDiffuseColour(Colour.GetOgreColourValue()); }

    ColourValue Light::GetDiffuseColour() const
        { return ColourValue(OgreLight->getDiffuseColour()); }

    void Light::SetSpecularColour(const Real& Red, const Real& Green, const Real& Blue)
        { OgreLight->setSpecularColour(Red, Green, Blue); }

    void Light::SetSpecularColour(const ColourValue &Colour)
        { OgreLight->setSpecularColour(Colour.GetOgreColourValue()); }

    ColourValue Light::GetSpecularColour() const
        { return ColourValue(OgreLight->getSpecularColour()); }

    ///////////////////////////////////////////////////////////////////////////////
    // Attenuation

    void Light::SetAttenuation(const Real& Range, const Real& Constant, const Real& Linear, const Real& Quadratic)
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

    void Light::SetSpotlightRange(const Real& InnerAngle, const Real& OuterAngle, const Real& Falloff)
    {
        Ogre::Radian IAngle(InnerAngle);
        Ogre::Radian OAngle(OuterAngle);
        OgreLight->setSpotlightRange(IAngle, OAngle, Falloff);
    }

    void Light::SetSpotlightInnerAngle(const Real& InnerAngle)
    {
        Ogre::Radian IAngle(InnerAngle);
        OgreLight->setSpotlightInnerAngle(IAngle);
    }

    Real Light::GetSpotlightInnerAngle() const
        { return OgreLight->getSpotlightInnerAngle().valueRadians(); }

    void Light::SetSpotlightOuterAngle(const Real& OuterAngle)
    {
        Ogre::Radian OAngle(OuterAngle);
        OgreLight->setSpotlightOuterAngle(OAngle);
    }

    Real Light::GetSpotlightOuterAngle() const
        { return OgreLight->getSpotlightOuterAngle().valueRadians(); }

    void Light::SetSpotlightFalloff(const Real& Falloff)
        { OgreLight->setSpotlightFalloff(Falloff); }

    Real Light::GetSpotlightFalloff() const
        { return OgreLight->getSpotlightFalloff(); }

    ///////////////////////////////////////////////////////////////////////////////
    // Effects

    void Light::SetPowerScale(const Real& Power)
        { OgreLight->setPowerScale(Power); }

    Real Light::GetPowerScale() const
        { return OgreLight->getPowerScale(); }

    ///////////////////////////////////////////////////////////////////////////////
    // Inherited From AttachableChild

    WorldAndSceneObjectType Light::GetType() const
        { return Mezzanine::WSO_Light; }

    void Light::SetScaling(const Vector3& Scale)
        { /* Can't scale a light */ }

    Vector3 Light::GetScaling() const
        { return Vector3(1,1,1); }

    void Light::SetLocalLocation(const Vector3& Location)
    {
        LocalXform.Location = Location;
        GlobalTransformDirty = true;

        _RecalculateGlobalTransform();
    }

    void Light::SetLocalOrientation(const Quaternion& Orientation)
    {
        LocalXform.Rotation = Orientation;
        GlobalTransformDirty = true;

        _RecalculateGlobalTransform();
    }
}//Mezzanine

///////////////////////////////////////////////////////////////////////////////
// Class External << Operators for streaming or assignment
#ifdef MEZZXML
std::ostream& operator << (std::ostream& stream, const Mezzanine::Light& Ev)
{
    stream      << "<Light Version=\"1\" Name=\"" << Ev.GetName()
                    << "\" AttachedTo=\"" << ( Ev.GetParent() ? Ev.GetParent()->GetName() : "" )
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

std::istream& MEZZ_LIB operator >> (std::istream& stream, Mezzanine::Light& Ev)
{
    Mezzanine::String OneTag( Mezzanine::XML::GetOneTag(stream) );
    std::auto_ptr<Mezzanine::XML::Document> Doc( Mezzanine::XML::PreParseClassFromSingleTag("Mezzanine::", "Light", OneTag) );

    Doc->GetFirstChild() >> Ev;

    return stream;
}

Mezzanine::XML::Node& operator >> (const Mezzanine::XML::Node& OneNode, Mezzanine::Light& Ev)
{
    if ( Mezzanine::String(OneNode.Name())==Mezzanine::String("Light") )
    {
        if(OneNode.GetAttribute("Version").AsInt() == 1)
        {
            Ev.SetLightType(static_cast<Mezzanine::Light::LightType>(OneNode.GetAttribute("Type").AsInt()));
            Ev.SetPowerScale(OneNode.GetAttribute("PowerScale").AsReal());
            Ev.SetAttenuation(OneNode.GetAttribute("AttenuationRange").AsReal(), OneNode.GetAttribute("AttenuationConstant").AsReal(), OneNode.GetAttribute("AttenuationLinear").AsReal(), OneNode.GetAttribute("AttenuationQuadric").AsReal());
            Ev.SetSpotlightInnerAngle(OneNode.GetAttribute("SpotlightInnerAngle").AsReal());
            Ev.SetSpotlightOuterAngle(OneNode.GetAttribute("SpotlightOuterAngle").AsReal());
            Ev.SetSpotlightFalloff(OneNode.GetAttribute("SpotlightFalloff").AsReal());
            Mezzanine::WorldNode * AttachPtr = Mezzanine::SceneManager::GetSingletonPtr()->GetNode( OneNode.GetAttribute("AttachedTo").AsString() );
            if (AttachPtr)
                { AttachPtr->AttachObject(&Ev); }

            Mezzanine::ColourValue TempColour(0,0,0,0);
            Mezzanine::Vector3 TempVec(0,0,0);

            for(Mezzanine::XML::Node Child = OneNode.GetFirstChild(); Child!=0; Child = Child.GetNextSibling())
            {
                Mezzanine::String Name(Child.Name());
                switch(Name[0])
                {
                    case 'f':   //fDiffuseColour
                        if(Name==Mezzanine::String("fDiffuseColour"))
                        {
                            Child.GetFirstChild() >> TempColour;
                            Ev.SetDiffuseColour(TempColour);
                        }else{
                            throw( Mezzanine::Exception(Mezzanine::StringTool::StringCat("Incompatible XML Version for Light: Includes unknown Element f-\"",Name,"\"")) );
                        }
                        break;
                    case 'S':   //SpecularColour
                        if(Name==Mezzanine::String("SpecularColour"))
                        {
                            Child.GetFirstChild() >> TempColour;
                            Ev.SetSpecularColour(TempColour);
                        }else{
                            throw( Mezzanine::Exception(Mezzanine::StringTool::StringCat("Incompatible XML Version for Light: Includes unknown Element S-\"",Name,"\"")) );
                        }
                        break;
                    case 'D':   //Direction
                        if(Name==Mezzanine::String("Direction"))
                        {
                            Child.GetFirstChild() >> TempVec;
                            Ev.SetDirection(TempVec);
                        }else{
                            throw( Mezzanine::Exception(Mezzanine::StringTool::StringCat("Incompatible XML Version for Light: Includes unknown Element D-\"",Name,"\"")) );
                        }
                        break;
                    case 'L':   //Location
                        if(Name==Mezzanine::String("Location"))
                        {
                            Child.GetFirstChild() >> TempVec;
                            Ev.SetLocation(TempVec);
                        }else{
                            throw( Mezzanine::Exception(Mezzanine::StringTool::StringCat("Incompatible XML Version for Light: Includes unknown Element L-\"",Name,"\"")) );
                        }
                        break;
                    default:
                        throw( Mezzanine::Exception(Mezzanine::StringTool::StringCat("Incompatible XML Version for Light: Includes unknown Element default-\"",Name,"\"")) );
                        break;
                }
            }

        }else{
            throw( Mezzanine::Exception("Incompatible XML Version for Light: Not Version 1"));
        }
    }else{
        throw( Mezzanine::Exception(Mezzanine::StringTool::StringCat("Attempting to deserialize a Light, found a ", OneNode.Name())));
    }
}
#endif // \MEZZXML


#endif
