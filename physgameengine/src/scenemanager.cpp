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
#ifndef _scenemanager_cpp
#define _scenemanager_cpp

#include "scenemanager.h"
#include "light.h"
#include "plane.h"
#include "particleeffect.h"
#include "worldnode.h"
#include <Ogre.h>

namespace phys
{
    SceneManager::SceneManager(SceneManager::SceneManagerType ManagerType)
    {
        Ogre::SceneType Type;
        switch (ManagerType)
        {
            case Generic:
                Type = Ogre::ST_GENERIC;
                break;
            case Exterior:
                Type = Ogre::ST_EXTERIOR_FAR;
                break;
            case ExteriorRealFar:
                Type = Ogre::ST_EXTERIOR_REAL_FAR;
                break;
            case Interior:
                Type = Ogre::ST_INTERIOR;
                break;
            default:
                Type = Ogre::ST_GENERIC;
        }
        OgreManager = Ogre::Root::getSingleton().createSceneManager(Type);
    }

    SceneManager::~SceneManager()
    {
        for( unsigned int x=0 ; x < Lights.size() ; x++ )
        {
            delete Lights[x];
        }
        Lights.clear();
        for( unsigned int x=0 ; x < Particles.size() ; x++ )
        {
            delete Particles[x];
        }
        Particles.clear();
        for( unsigned int x=0 ; x < WorldNodes.size() ; x++ )
        {
            delete WorldNodes[x];
        }
        WorldNodes.clear();
        Ogre::Root::getSingleton().destroySceneManager(OgreManager);
    }

    void SceneManager::Initialize()
    {
    }

    void SceneManager::DoMainLoopItems()
    {
    }

    void SceneManager::SetSceneShadowTechnique(SceneShadowTechnique Shadows)
    {
        Ogre::ShadowTechnique Type;
        switch (Shadows)
        {
            case SST_Stencil_Modulative:
                Type = Ogre::SHADOWTYPE_STENCIL_MODULATIVE;
                break;
            case SST_Stencil_Additive:
                Type = Ogre::SHADOWTYPE_STENCIL_ADDITIVE;
                break;
            case SST_Texture_Modulative:
                Type = Ogre::SHADOWTYPE_TEXTURE_MODULATIVE;
                break;
            case SST_Texture_Additive:
                Type = Ogre::SHADOWTYPE_TEXTURE_ADDITIVE;
                break;
            case SST_Texture_Additive_Integrated:
                Type = Ogre::SHADOWTYPE_TEXTURE_ADDITIVE_INTEGRATED;
                break;
            case SST_Texture_Modulative_Integrated:
                Type = Ogre::SHADOWTYPE_TEXTURE_MODULATIVE_INTEGRATED;
                break;
            default:
                Type = Ogre::SHADOWTYPE_NONE;
        }
        OgreManager->setShadowTechnique(Type);
    }

    SceneManager::SceneShadowTechnique SceneManager::GetSceneShadowTechnique()
    {
        Ogre::ShadowTechnique ShadowType = OgreManager->getShadowTechnique();
        switch (ShadowType)
        {
            case Ogre::SHADOWTYPE_STENCIL_MODULATIVE:
                return SST_Stencil_Modulative;
                break;
            case Ogre::SHADOWTYPE_STENCIL_ADDITIVE:
                return SST_Stencil_Additive;
                break;
            case Ogre::SHADOWTYPE_TEXTURE_MODULATIVE:
                return SST_Texture_Modulative;
                break;
            case Ogre::SHADOWTYPE_TEXTURE_ADDITIVE:
                return SST_Texture_Additive;
                break;
            case Ogre::SHADOWTYPE_TEXTURE_ADDITIVE_INTEGRATED:
                return SST_Texture_Additive_Integrated;
                break;
            case Ogre::SHADOWTYPE_TEXTURE_MODULATIVE_INTEGRATED:
                return SST_Texture_Modulative_Integrated;
                break;
            default:
                return SST_None;
        }
    }

    void SceneManager::SetShadowColour(const ColourValue& ShadowColour)
    {
        this->OgreManager->setShadowColour(ShadowColour.GetOgreColourValue());
    }

    ColourValue SceneManager::GetShadowColour()
    {
        ColourValue Shadow(this->OgreManager->getShadowColour());
        return Shadow;
    }

    void SceneManager::CreateSkyPlane(Plane& SkyPlane, String& Material, String& Group, Real Scale, Real Tiling, bool DrawFirst, Real Bow, int XSegments, int YSegments)
    {
        this->OgreManager->setSkyPlane(true, SkyPlane.GetOgrePlane(), Material, Scale, Tiling, DrawFirst, Bow, XSegments, YSegments, Group);
    }

    void SceneManager::DisableSkyPlane()
    {
        this->OgreManager->setSkyPlane(false, Ogre::Plane(), "");
    }

    void SceneManager::CreateSkyBox(String& Material, String& Group, Real Distance, bool DrawFirst, Quaternion Orientation)
    {
        this->OgreManager->setSkyBox(true, Material, Distance, DrawFirst, Orientation.GetOgreQuaternion(), Group);
    }

    void SceneManager::DisableSkyBox()
    {
        this->OgreManager->setSkyBox(false, "");
    }

    void SceneManager::CreateSkyDome(String& Material, String& Group, Real Distance, Real Curvature, Real Tiling, bool DrawFirst,
                                    Quaternion Orientation, int XSegments, int YSegments)
    {
        this->OgreManager->setSkyDome(true, Material, Curvature, Tiling, Distance, DrawFirst, Orientation.GetOgreQuaternion(), XSegments, YSegments, -1, Group);
    }

    void SceneManager::DisableSkyDome()
    {
        this->OgreManager->setSkyDome(false, "");
    }

    void SceneManager::SetAmbientLight(Real Red, Real Green, Real Blue, Real Alpha)
    {
        this->OgreManager->setAmbientLight(Ogre::ColourValue(Red, Green, Blue, Alpha));
    }

    Light* SceneManager::CreateLight(const String& Name)
    {
        Light* light = new Light(this->OgreManager->createLight(Name), this);
        Lights.push_back(light);
        return light;
    }

    Light* SceneManager::GetLight(const String& Name) const
    {
        if(Lights.empty())
            return 0;
        for( std::vector<Light*>::const_iterator it = Lights.begin() ; it != Lights.end() ; it++ )
        {
            if( Name == (*it)->GetName() )
            {
                return (*it);
            }
        }
        return 0;
    }

    Light* SceneManager::GetLight(Whole Index) const
    {
        return Lights[Index];
    }

    Whole SceneManager::GetNumLights() const
    {
        return Lights.size();
    }

    void SceneManager::DestroyLight(Light* light)
    {
        if(Lights.empty())
            return;
        for( std::vector<Light*>::iterator it = Lights.begin() ; it != Lights.end() ; it++ )
        {
            if( light == (*it) )
            {
                delete (*it);
                Lights.erase(it);
                return;
            }
        }
    }

    ParticleEffect* SceneManager::CreateParticleEffect(const String& Name, const String& Template)
    {
        ParticleEffect* Particle = new ParticleEffect(this->OgreManager->createParticleSystem(Name, Template), this);
        Particles.push_back(Particle);
        return Particle;
    }

    ParticleEffect* SceneManager::GetParticleEffect(const String& Name) const
    {
        if(Particles.empty())
            return 0;
        for( std::vector<ParticleEffect*>::const_iterator it = Particles.begin() ; it != Particles.end() ; it++ )
        {
            if( Name == (*it)->GetName() )
            {
                return (*it);
            }
        }
        return 0;
    }

    ParticleEffect* SceneManager::GetParticleEffect(Whole Index) const
    {
        return Particles[Index];
    }

    Whole SceneManager::GetNumParticleEffects() const
    {
        return Particles.size();
    }

    void SceneManager::DestroyParticleEffect(ParticleEffect* particleeffect)
    {
        if(Particles.empty())
            return;
        for( std::vector<ParticleEffect*>::iterator it = Particles.begin() ; it != Particles.end() ; it++ )
        {
            if( particleeffect == (*it) )
            {
                delete (*it);
                Particles.erase(it);
                return;
            }
        }
    }

    ConstString& SceneManager::GetName() const
    {
        return this->OgreManager->getName();
    }

    WorldNode* SceneManager::CreateOrbitingNode(const String& Name, Vector3 Target, Vector3 RelativeLoc, bool AutoTrack)
    {
        Ogre::SceneNode* OgreCNode = OgreManager->createSceneNode(Name + "C");
        OgreManager->getRootSceneNode()->addChild(OgreCNode);
        OgreCNode->setPosition(Target.GetOgreVector3());
        Ogre::SceneNode* OgreONode = OgreManager->createSceneNode(Name);
        OgreCNode->addChild(OgreONode);
        OgreONode->setPosition(RelativeLoc.GetOgreVector3());
        if(AutoTrack)
        {
            OgreONode->setAutoTracking(true, OgreCNode);
        }
        WorldNode* PhysONode = new WorldNode(OgreONode, this);
        PhysONode->SetType(WorldNode::Orbit);
        WorldNodes.push_back(PhysONode);
        return PhysONode;
    }

    WorldNode* SceneManager::CreateStandNode(const String& Name, Vector3 LookAt, Vector3 Location)
    {
        Ogre::SceneNode* OgreNode = OgreManager->createSceneNode(Name);
        OgreManager->getRootSceneNode()->addChild(OgreNode);
        OgreNode->setPosition(Location.GetOgreVector3());
        OgreNode->lookAt(LookAt.GetOgreVector3(), Ogre::Node::TS_WORLD);
        WorldNode* PhysNode = new WorldNode(OgreNode, this);
        PhysNode->SetType(WorldNode::Stand);
        WorldNodes.push_back(PhysNode);
        return PhysNode;
    }

    WorldNode* SceneManager::CreateFreeNode(const String& Name, Vector3 LookAt, Vector3 Location)
    {
        Ogre::SceneNode* OgreNode = OgreManager->createSceneNode(Name);
        OgreManager->getRootSceneNode()->addChild(OgreNode);
        OgreNode->setPosition(Location.GetOgreVector3());
        OgreNode->lookAt(LookAt.GetOgreVector3(), Ogre::Node::TS_WORLD);
        WorldNode* PhysNode = new WorldNode(OgreNode, this);
        PhysNode->SetType(WorldNode::Free);
        WorldNodes.push_back(PhysNode);
        return PhysNode;
    }

    WorldNode* SceneManager::GetNode(const String& Name) const
    {
        if(WorldNodes.empty())
            return 0;
        for( std::vector<WorldNode*>::const_iterator it = WorldNodes.begin() ; it != WorldNodes.end() ; it++ )
        {
            if( Name == (*it)->GetName() )
            {
                WorldNode* node = (*it);
                return node;
            }
        }
        return 0;
    }

    WorldNode* SceneManager::GetNode(Whole Index) const
    {
        return WorldNodes[Index];
    }

    Whole SceneManager::GetNumNodes() const
    {
        return WorldNodes.size();
    }

    Whole SceneManager::GetNumStandNodes() const
    {
        if(WorldNodes.empty())
            return 0;
        Whole Num = 0;
        for( std::vector<WorldNode*>::const_iterator it = WorldNodes.begin() ; it != WorldNodes.end() ; it++ )
        {
            if( WorldNode::Stand == (*it)->GetType() )
            {
                Num++;
            }
        }
        return Num;
    }

    Whole SceneManager::GetNumOrbitNodes() const
    {
        if(WorldNodes.empty())
            return 0;
        Whole Num = 0;
        for( std::vector<WorldNode*>::const_iterator it = WorldNodes.begin() ; it != WorldNodes.end() ; it++ )
        {
            if( WorldNode::Orbit == (*it)->GetType() )
            {
                Num++;
            }
        }
        return Num;
    }

    void SceneManager::DestroyNode(WorldNode* node)
    {
        if(WorldNodes.empty())
            return;
        for( std::vector<WorldNode*>::iterator it = WorldNodes.begin() ; it != WorldNodes.end() ; it++ )
        {
            if( node == (*it) )
            {
                delete (*it);
                WorldNodes.erase(it);
                return;
            }
        }
    }

    ManagerBase::ManagerTypeName SceneManager::GetType() const
        { return ManagerBase::SceneManager; }

    Ogre::SceneManager* SceneManager::GetGraphicsWorldPointer() const
    {
        return OgreManager;
    }
}

///////////////////////////////////////////////////////////////////////////////
// Class External << Operators for streaming or assignment
#ifdef PHYSXML
std::ostream& operator << (std::ostream& stream, const phys::SceneManager& Ev)
{
    stream      << "<SceneManager Version=\"1\" Name=\"" << Ev.GetName()
                    //<< "\" AttachedTo=\"" << ( Ev.GetAttachedTo() ? Ev.GetAttachedTo()->GetName() : "" )
                    << "\" Type=\"" << Ev.GetType()
// shadow type
//
//                << "\">"
//                << "<Direction>" << Ev.GetDirection() << "</Direction>"


                << "</SceneManager>";
    return stream;
}

std::istream& PHYS_LIB operator >> (std::istream& stream, phys::SceneManager& Ev)
{
    phys::String OneTag( phys::xml::GetOneTag(stream) );
    std::auto_ptr<phys::xml::Document> Doc( phys::xml::PreParseClassFromSingleTag("phys::", "SceneManager", OneTag) );

    Doc->GetFirstChild() >> Ev;

    return stream;
}

phys::xml::Node& operator >> (const phys::xml::Node& OneNode, phys::SceneManager& Ev)
{
    if ( phys::String(OneNode.Name())==phys::String("SceneManager") )
    {
      /*  if(OneNode.GetAttribute("Version").AsInt() == 1)
        {
            Ev.SetType(static_cast<phys::SceneManager::SceneManagerType>(OneNode.GetAttribute("Type").AsInt()));
            Ev.SetPowerScale(OneNode.GetAttribute("PowerScale").AsReal());
            Ev.SetAttenuation(OneNode.GetAttribute("AttenuationRange").AsReal(), OneNode.GetAttribute("AttenuationConstant").AsReal(), OneNode.GetAttribute("AttenuationLinear").AsReal(), OneNode.GetAttribute("AttenuationQuadric").AsReal());
            Ev.SetSpotSceneManagerInnerAngle(OneNode.GetAttribute("SpotSceneManagerInnerAngle").AsReal());
            Ev.SetSpotSceneManagerOuterAngle(OneNode.GetAttribute("SpotSceneManagerOuterAngle").AsReal());
            Ev.SetSpotSceneManagerFalloff(OneNode.GetAttribute("SpotSceneManagerFalloff").AsReal());
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
                            throw( phys::Exception(phys::StringCat("Incompatible XML Version for SceneManager: Includes unknown Element f-\"",Name,"\"")) );
                        }
                        break;
                    case 'S':   //SpecularColour
                        if(Name==phys::String("SpecularColour"))
                        {
                            Child.GetFirstChild() >> TempColour;
                            Ev.SetSpecularColour(TempColour);
                        }else{
                            throw( phys::Exception(phys::StringCat("Incompatible XML Version for SceneManager: Includes unknown Element S-\"",Name,"\"")) );
                        }
                        break;
                    case 'D':   //Direction
                        if(Name==phys::String("Direction"))
                        {
                            Child.GetFirstChild() >> TempVec;
                            Ev.SetDirection(TempVec);
                        }else{
                            throw( phys::Exception(phys::StringCat("Incompatible XML Version for SceneManager: Includes unknown Element D-\"",Name,"\"")) );
                        }
                        break;
                    case 'L':   //Location
                        if(Name==phys::String("Location"))
                        {
                            Child.GetFirstChild() >> TempVec;
                            Ev.SetLocation(TempVec);
                        }else{
                            throw( phys::Exception(phys::StringCat("Incompatible XML Version for SceneManager: Includes unknown Element L-\"",Name,"\"")) );
                        }
                        break;
                    default:
                        throw( phys::Exception(phys::StringCat("Incompatible XML Version for SceneManager: Includes unknown Element default-\"",Name,"\"")) );
                        break;
                }
            }

        }else{
            throw( phys::Exception("Incompatible XML Version for SceneManager: Not Version 1"));
        }*/
    }else{
        throw( phys::Exception(phys::StringCat("Attempting to deserialize a SceneManager, found a ", OneNode.Name())));
    }
}
#endif



#endif
