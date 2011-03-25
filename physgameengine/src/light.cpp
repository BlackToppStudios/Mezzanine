//© Copyright 2010 BlackTopp Studios Inc.
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
#include <Ogre.h>

namespace phys
{
    Light::Light(const String& Name, SceneManager* manager)
    {
        Manager = manager;
        OgreLight = Manager->GetGraphicsWorldPointer()->createLight(Name);
        SetElementType(Attachable::Light);
    }

    Light::Light(Ogre::Light* light, SceneManager* manager)
    {
        OgreLight = light;
        Manager = manager;
        SetElementType(Attachable::Light);
    }

    Light::~Light()
    {
        Manager->GetGraphicsWorldPointer()->destroyLight(OgreLight);
    }

    ConstString& Light::GetName() const
    {
        return OgreLight->getName();
    }

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

    void Light::SetPosition(Vector3 Position)
    {
        OgreLight->setPosition(Position.GetOgreVector3());
    }

    void Light::SetDirection(Vector3 Direction)
    {
        OgreLight->setDirection(Direction.GetOgreVector3());
    }

    void Light::SetDiffuseColour(Real Red, Real Green, Real Blue)
    {
        OgreLight->setDiffuseColour(Red, Green, Blue);
    }

    void Light::SetDiffuseColour(ColourValue &Colour)
    {
        OgreLight->setDiffuseColour(Colour.GetOgreColourValue());
    }

    void Light::SetSpecularColour(Real Red, Real Green, Real Blue)
    {
        OgreLight->setSpecularColour(Red, Green, Blue);
    }

    void Light::SetSpecularColour(ColourValue &Colour)
    {
        OgreLight->setSpecularColour(Colour.GetOgreColourValue());
    }

    void Light::SetAttenuation(Real Range, Real Constant, Real Linear, Real Quadratic)
    {
        OgreLight->setAttenuation(Range, Constant, Linear, Quadratic);
    }

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
    {
        OgreLight->setSpotlightFalloff(Falloff);
    }

    void Light::SetPowerScale(Real Power)
    {
        OgreLight->setPowerScale(Power);
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

    Vector3 Light::GetPosition() const
    {
        Vector3 Pos(OgreLight->getPosition());
        return Pos;
    }

    Vector3 Light::GetDirection() const
    {
        Vector3 Dir(OgreLight->getDirection());
        return Dir;
    }

    ColourValue Light::GetDiffuseColour() const
    {
        ColourValue DiffCol(OgreLight->getDiffuseColour());
        return DiffCol;
    }

    ColourValue Light::GetSpecularColour() const
    {
        ColourValue SpecCol(OgreLight->getSpecularColour());
        return SpecCol;
    }

    Real Light::GetAttenuationRange() const
    {
        return OgreLight->getAttenuationRange();
    }

    Real Light::GetAttenuationConstant() const
    {
        return OgreLight->getAttenuationConstant();
    }

    Real Light::GetAttenuationLinear() const
    {
        return OgreLight->getAttenuationLinear();
    }

    Real Light::GetAttenuationQuadric() const
    {
        return OgreLight->getAttenuationQuadric();
    }

    Real Light::GetSpotlightInnerAngle() const
    {
        return OgreLight->getSpotlightInnerAngle().valueRadians();
    }

    Real Light::GetSpotlightOuterAngle() const
    {
        return OgreLight->getSpotlightOuterAngle().valueRadians();
    }

    Real Light::GetSpotlightFalloff() const
    {
        return OgreLight->getSpotlightFalloff();
    }

    Real Light::GetPowerScale() const
    {
        return OgreLight->getPowerScale();
    }
}//phys

#endif
