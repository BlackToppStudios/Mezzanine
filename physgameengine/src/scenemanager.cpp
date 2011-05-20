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
#include "world.h"
#include <Ogre.h>

namespace phys
{
    namespace internal
    {
        /// @brief Stores internal data for the SCeneManager to keep it from cluttering the Header file
        /// @internal
        class SceneManagerData
        {
            public:

            /// @internal
            /// @brief The currently active sky, if set to anything other than SkyNone, then the 5 other skycache variable may have meaning
            SceneManager::SkyMethod ActiveSky;

            // The SkyCache
            /// @internal
            /// @brief The Name of the Material the sky is made of
            String SkyMaterialName;
            /// @internal
            /// @brief The orientation of the sky, unless it's a Skyplane, this this is all 0s
            Quaternion SkyOrientation;
            /// @internal
            /// @brief The name of the group the sky material is in
            String SkyMaterialGroupName;
            /// @internal
            /// @brief When is the sky drawn, first or per Z-order
            bool SkyDrawnFirst;
            /// @internal
            /// @brief Used to describe a skyplane instead of orientation
            Plane SkyThePlane;

            /// @brief The size
            unsigned short ShadowTextureSize;

            /// @internal
            /// @brief Pointer for the Ogre Scenemanager, where this manager gets it's functionality.
            Ogre::SceneManager* OgreManager;

            void DisableSky(SceneManager* ScenePTR)
            {
                switch(ActiveSky)
                {
                    case SceneManager::SkyNone:
                        break;
                    case SceneManager::SkyPlane:
                        ScenePTR->DisableSkyPlane();
                        break;
                    case SceneManager::SkyBox:
                        ScenePTR->DisableSkyBox();
                        break;
                    case SceneManager::SkyDome:
                        ScenePTR->DisableSkyDome();
                        break;
                }
                ActiveSky = SceneManager::SkyNone;
            }

            /// @internal
            /// @brief update
            void UpdateSkyCache(
                            SceneManager::SkyMethod FreshSkyMethod = SceneManager::SkyNone,
                            String FreshSkyMaterialName = "",
                            Quaternion FreshSkyOrientation = Quaternion(0,0,0,0),
                            String FreshSkyMaterialGroupName = "",
                            bool FreshSkyDrawnFirst = false,
                            Plane FreshSkyThePlane = Plane(Vector3(0,0,0),0)
                        )
            {
                ActiveSky=FreshSkyMethod;
                SkyOrientation=FreshSkyOrientation;
                SkyMaterialName=FreshSkyMaterialName;
                SkyMaterialGroupName=FreshSkyMaterialGroupName;
                SkyDrawnFirst=FreshSkyDrawnFirst;
                SkyThePlane=FreshSkyThePlane;
            }

            SceneManagerData():
                ActiveSky(SceneManager::SkyNone),
                OgreManager(0),
                SkyDrawnFirst(false),
                SkyOrientation(0,0,0,0),
                SkyMaterialName(""),
                SkyMaterialGroupName(""),
                SkyThePlane(Vector3(0,0,0),0),
                ShadowTextureSize(512)
            {

            }

            ~SceneManagerData()
            {
                Ogre::Root::getSingleton().destroySceneManager(OgreManager);
                //delete OgreManager;
            }
        };
    }

    ///////////////////////////////////////////////////////////////////////////////
    /// Construction

    SceneManager::SceneManager(SceneManager::SceneManagerType ManagerType)
    {
        this->SMD = new internal::SceneManagerData();
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
        this->SMD->OgreManager = Ogre::Root::getSingleton().createSceneManager(Type);
        this->SetAmbientLight(ColourValue(0.0,0.0,0.0));
        //const Ogre::ShadowCameraSetupPtr ShadowCam = Ogre::ShadowCameraSetupPtr(new Ogre::DefaultShadowCameraSetup());
        //OgreManager->setShadowCameraSetup(ShadowCam);
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
        delete SMD;
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Shadow Management

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
        this->SMD->OgreManager->setShadowTechnique(Type);
    }

    SceneManager::SceneShadowTechnique SceneManager::GetSceneShadowTechnique() const
    {
        Ogre::ShadowTechnique ShadowType = this->SMD->OgreManager->getShadowTechnique();
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

    void SceneManager::SetShadowTextureCount(const Whole& Count)
        { this->SMD->OgreManager->setShadowTextureCount(Count); }

    Whole SceneManager::GetShadowTextureCount() const
        { return this->SMD->OgreManager->getShadowTextureCount(); }

    void SceneManager::SetShadowTextureSize(unsigned short Size)
    {
        this->SMD->OgreManager->setShadowTextureSize(Size);
        this->SMD->ShadowTextureSize=Size;
    }

    unsigned short SceneManager::GetShadowTextureSize() const
        { return this->SMD->ShadowTextureSize; }

    void SceneManager::SetShadowFarDistance(const Real& FarDist)
        { this->SMD->OgreManager->setShadowFarDistance(FarDist); }

    Real SceneManager::GetShadowFarDistance() const
        { return this->SMD->OgreManager->getShadowFarDistance(); }

    void SceneManager::SetShadowColour(const ColourValue& ShadowColour)
        { this->SMD->OgreManager->setShadowColour(ShadowColour.GetOgreColourValue()); }

    ColourValue SceneManager::GetShadowColour() const
    {
        ColourValue Shadow(this->SMD->OgreManager->getShadowColour());
        return Shadow;
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Sky Surface Management

    void SceneManager::CreateSkyPlane(const Plane& SkyPlane_, const String& Material, const String& Group, Real Scale, Real Tiling, bool DrawFirst, Real Bow, int XSegments, int YSegments)
    {
        this->SMD->DisableSky(this);
        this->SMD->UpdateSkyCache(SkyPlane, Material, Quaternion(0,0,0,0), Group, DrawFirst, SkyPlane_);
        this->SMD->OgreManager->setSkyPlane(true, SkyPlane_.GetOgrePlane(), Material, Scale, Tiling, DrawFirst, Bow, XSegments, YSegments, Group);
    }

    void SceneManager::DisableSkyPlane()
    {
        this->SMD->OgreManager->setSkyPlane(false, Ogre::Plane(), "");
    }

    void SceneManager::CreateSkyBox(const String& Material, const String& Group, Real Distance, bool DrawFirst, Quaternion Orientation)
    {
        this->SMD->DisableSky(this);
        this->SMD->UpdateSkyCache(SkyBox, Material, Orientation, Group, DrawFirst);
        this->SMD->OgreManager->setSkyBox(true, Material, Distance, DrawFirst, Orientation.GetOgreQuaternion(), Group);
    }

    void SceneManager::DisableSkyBox()
    {
        this->SMD->OgreManager->setSkyBox(false, "");
    }

    void SceneManager::CreateSkyDome(const String& Material, const String& Group, Real Distance, Real Curvature, Real Tiling, bool DrawFirst,
                                    Quaternion Orientation, int XSegments, int YSegments)
    {
        this->SMD->DisableSky(this);
        this->SMD->UpdateSkyCache(SkyDome, Material, Orientation, Group, DrawFirst);
        this->SMD->OgreManager->setSkyDome(true, Material, Curvature, Tiling, Distance, DrawFirst, Orientation.GetOgreQuaternion(), XSegments, YSegments, -1, Group);
    }

    void SceneManager::DisableSkyDome()
    {
        this->SMD->OgreManager->setSkyDome(false, "");
    }

    void SceneManager::DisableSky()
    {
        this->SMD->UpdateSkyCache();
        this->SMD->DisableSky(this);
    }

    SceneManager::SkyMethod SceneManager::WhichSky() const
        { return this->SMD->ActiveSky; }

    ///////////////////////////////////////////////////////////////////////////////
    // Light Management

    void SceneManager::SetAmbientLight(Real Red, Real Green, Real Blue, Real Alpha)
    {
        this->SMD->OgreManager->setAmbientLight(Ogre::ColourValue(Red, Green, Blue, Alpha));
    }

    void SceneManager::SetAmbientLight(const ColourValue &LightColor)
    {
        this->SMD->OgreManager->setAmbientLight(LightColor.GetOgreColourValue());
    }

    ColourValue SceneManager::GetAmbientLight() const
    {

        return ColourValue(this->SMD->OgreManager->getAmbientLight());
    }

    Light* SceneManager::CreateLight(const String& Name)
    {
        Light* light = new Light(this->SMD->OgreManager->createLight(Name), this);
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

    SceneManager::LightIterator SceneManager::BeginLight()
        { return this->Lights.begin(); }

    SceneManager::LightIterator SceneManager::EndLight()
        { return this->Lights.end(); }

    SceneManager::ConstLightIterator SceneManager::BeginLight() const
        { return this->Lights.begin(); }

    SceneManager::ConstLightIterator SceneManager::EndLight() const
        { return this->Lights.end(); }

    ///////////////////////////////////////////////////////////////////////////////
    // Particle Effect Management

    ParticleEffect* SceneManager::CreateParticleEffect(const String& Name, const String& Template)
    {
        ParticleEffect* Particle = new ParticleEffect(this->SMD->OgreManager->createParticleSystem(Name, Template), Template, this);
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

    SceneManager::ParticleEffectIterator SceneManager::BeginParticleEffect()
        { return this->Particles.begin(); }

    SceneManager::ParticleEffectIterator SceneManager::EndParticleEffect()
        { return this->Particles.end(); }

    SceneManager::ConstParticleEffectIterator SceneManager::BeginParticleEffect() const
        { return this->Particles.begin(); }

    SceneManager::ConstParticleEffectIterator SceneManager::EndParticleEffect() const
        { return this->Particles.end(); }

    ///////////////////////////////////////////////////////////////////////////////
    // WorldNode Management

    WorldNode* SceneManager::CreateOrbitingNode(const String& Name, Vector3 Target, Vector3 RelativeLoc, bool AutoTrack)
    {
        Ogre::SceneNode* OgreCNode = this->SMD->OgreManager->createSceneNode(Name + "C");
        this->SMD->OgreManager->getRootSceneNode()->addChild(OgreCNode);
        OgreCNode->setPosition(Target.GetOgreVector3());
        Ogre::SceneNode* OgreONode = this->SMD->OgreManager->createSceneNode(Name);
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
        Ogre::SceneNode* OgreNode = this->SMD->OgreManager->createSceneNode(Name);
        this->SMD->OgreManager->getRootSceneNode()->addChild(OgreNode);
        OgreNode->setPosition(Location.GetOgreVector3());
        OgreNode->lookAt(LookAt.GetOgreVector3(), Ogre::Node::TS_WORLD);
        WorldNode* PhysNode = new WorldNode(OgreNode, this);
        PhysNode->SetType(WorldNode::Stand);
        WorldNodes.push_back(PhysNode);
        return PhysNode;
    }

    WorldNode* SceneManager::CreateFreeNode(const String& Name, Vector3 LookAt, Vector3 Location)
    {
        Ogre::SceneNode* OgreNode = this->SMD->OgreManager->createSceneNode(Name);
        this->SMD->OgreManager->getRootSceneNode()->addChild(OgreNode);
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

    SceneManager::WorldNodeIterator SceneManager::BeginWorldNode()
        { return this->WorldNodes.begin(); }

    SceneManager::WorldNodeIterator SceneManager::EndWorldNode()
        { return this->WorldNodes.end(); }

    SceneManager::ConstWorldNodeIterator SceneManager::BeginWorldNode() const
        { return this->WorldNodes.begin(); }

    SceneManager::ConstWorldNodeIterator SceneManager::EndWorldNode() const
        { return this->WorldNodes.end(); }


    ///////////////////////////////////////////////////////////////////////////////
    // Basic Functionality

    ConstString& SceneManager::GetName() const
        { return this->SMD->OgreManager->getName(); }

    void SceneManager::Initialize()
        { }

    void SceneManager::DoMainLoopItems()
        { }

    ManagerBase::ManagerTypeName SceneManager::GetType() const
        { return ManagerBase::SceneManager; }

    Ogre::SceneManager* SceneManager::GetGraphicsWorldPointer() const
        { return this->SMD->OgreManager; }

    internal::SceneManagerData* SceneManager::GetRawInternalDataPointer() const
        { return this->SMD; }
}

///////////////////////////////////////////////////////////////////////////////
// Class External << Operators for streaming or assignment
#ifdef PHYSXML
std::ostream& operator << (std::ostream& stream, const phys::SceneManager& Ev)
{
    stream      << "<SceneManager Version=\"1\" Name=\"" << Ev.GetName()
                //<< "\" Type=\"" << Ev.GetType()
                << "\" SceneShadowTechnique=\"" << Ev.GetSceneShadowTechnique()
                << "\" ShadowTextureCount=\"" << Ev.GetShadowTextureCount()
                << "\" ShadowTextureSize=\"" << Ev.GetShadowTextureSize()
                << "\" ShadowFarDistance=\"" << Ev.GetShadowFarDistance()
                << "\" SkyMethod=\"" << Ev.WhichSky()
                << "\">"
                << "<ShadowColour>" << Ev.GetShadowColour() << "</ShadowColour>"
                << "<AmbientLight>" << Ev.GetAmbientLight() << "</AmbientLight>";

                phys::internal::SceneManagerData* SMD = Ev.GetRawInternalDataPointer();
                switch (Ev.WhichSky())
                {
                    case phys::SceneManager::SkyNone:
                        // Nothing to do
                        break;
                    case phys::SceneManager::SkyPlane:{
                        Ogre::SceneManager::SkyPlaneGenParameters Values=SMD->OgreManager->getSkyPlaneGenParameters();
                        stream  << "<SkyPlane Version=\"1"
                                << "\" MaterialName=\"" << SMD->SkyMaterialName
                                << "\" MaterialGroupName=\"" << SMD->SkyMaterialGroupName
                                << "\" DrawFirst=\"" << SMD->SkyDrawnFirst
                                << "\" Scale=\"" << Values.skyPlaneScale
                                << "\" Tiling=\"" << Values.skyPlaneTiling
                                << "\" Bow=\"" << Values.skyPlaneBow
                                << "\" XSegments=\"" << Values.skyPlaneXSegments
                                << "\" YSegments=\"" << Values.skyPlaneYSegments
                                << "\" >"
                                << SMD->SkyThePlane
                                << "</SkyPlane>";
                            //const Plane& SkyPlane_, const String& Material, const String& Group, Real Scale, Real Tiling, bool DrawFirst, Real Bow, int XSegments, int YSegments
                        }
                        break;
                    case phys::SceneManager::SkyBox:{
                        Ogre::SceneManager::SkyBoxGenParameters Values = SMD->OgreManager->getSkyBoxGenParameters();
                        stream  << "<SkyBox Version=\"1"
                                << "\" MaterialName=\"" << SMD->SkyMaterialName
                                << "\" MaterialGroupName=\"" << SMD->SkyMaterialGroupName
                                << "\" DrawFirst=\"" << SMD->SkyDrawnFirst
                                << "\" Distance=\"" << Values.skyBoxDistance
                                << "\" >"
                                << "<Orientation>" << SMD->SkyOrientation << "</Orientation>"
                                << "</SkyBox>";
                            //const String& Material, const String& Group, Real Distance, bool DrawFirst=true, Quaternion Orientation=Quaternion()
                        }
                        break;
                    case phys::SceneManager::SkyDome:{
                        Ogre::SceneManager::SkyDomeGenParameters Values=SMD->OgreManager->getSkyDomeGenParameters();
                        stream  << "<SkyDome Version=\"1"
                                << "\" MaterialName=\"" << SMD->SkyMaterialName
                                << "\" MaterialGroupName=\"" << SMD->SkyMaterialGroupName
                                << "\" DrawFirst=\"" << SMD->SkyDrawnFirst
                                << "\" Distance=\"" << Values.skyDomeDistance
                                << "\" Curvature=\"" << Values.skyDomeCurvature
                                << "\" Tiling=\"" << Values.skyDomeTiling
                                << "\" XSegments=\"" << Values.skyDomeXSegments
                                << "\" YSegments=\"" << Values.skyDomeYSegments
                                << "\" YSegments_keep=\"" << Values.skyDomeYSegments_keep
                                << "\" >"
                                << "<Orientation>" << SMD->SkyOrientation << "</Orientation>"
                                << "</SkyDome>";
                            //const String& Material, const String& Group, Real Distance, Real Curvature=10.0, Real Tiling=8.0, bool DrawFirst=true, Quaternion Orientation=Quaternion(), int XSegments=16, int YSegments=16
                        }
                        break;
                }
                /*  Sky Cache Member - String SkyMaterialName; Quaternion SkyOrientation; String SkyMaterialGroupName; bool SkyDrawnFirst; Plane SkyThePlane; */

                for (phys::SceneManager::ConstLightIterator Iter = phys::World::GetWorldPointer()->GetSceneManager()->BeginLight();
                        phys::World::GetWorldPointer()->GetSceneManager()->EndLight()!=Iter;
                        ++Iter)
                    { stream << **Iter; }
                for (phys::SceneManager::ConstParticleEffectIterator Iter = phys::World::GetWorldPointer()->GetSceneManager()->BeginParticleEffect();
                        phys::World::GetWorldPointer()->GetSceneManager()->EndParticleEffect()!=Iter;
                        ++Iter)
                    { stream << **Iter; }
                for (phys::SceneManager::ConstWorldNodeIterator Iter = phys::World::GetWorldPointer()->GetSceneManager()->BeginWorldNode();
                        phys::World::GetWorldPointer()->GetSceneManager()->EndWorldNode()!=Iter;
                        ++Iter)
                    { stream << **Iter; }
    stream      << "</SceneManager>";


// --name
// --shadow type
// --shadow Texture count
// --shadpw texture size
// --shadow distance
// --shadow colour
// --Sky
// --     Disable
// --     Plane& SkyPlane, String& Material, String& Group, Real Scale=1000.0, Real Tiling=10.0, bool DrawFirst=true, Real Bow=0, int XSegments=1, int YSegments=1
// --     SkyBox(String& Material, String& Group, Real Distance, bool DrawFirst=true, Quaternion Orientation=Quaternion());
// --     SkyDome(String& Material, String& Group, Real Distance, Real Curvature=10.0, Real Tiling=8.0, bool DrawFirst=true, Quaternion Orientation=Quaternion(), int XSegments=16, int YSegments=16);
// -- Ambient light
// -- iterate over
// --     lights
// --     particles
// --     Worldnodes

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
        if(OneNode.GetAttribute("Version").AsInt() == 1)
        {
            //Ev.SetType(static_cast<phys::SceneManager::SceneManagerType>(OneNode.GetAttribute("Type").AsInt()));
            Ev.SetSceneShadowTechnique((phys::SceneManager::SceneShadowTechnique)(OneNode.GetAttribute("SceneShadowTechnique").AsInt()));
            Ev.SetShadowTextureCount(OneNode.GetAttribute("ShadowTextureCount").AsInt());
            Ev.SetShadowTextureSize(OneNode.GetAttribute("ShadowTextureSize").AsInt());
            Ev.SetShadowFarDistance(OneNode.GetAttribute("ShadowFarDistance").AsReal());
            phys::SceneManager::SkyMethod DoubleCheck = (phys::SceneManager::SkyMethod)(OneNode.GetAttribute("SkyMethod").AsInt());

            for(phys::xml::Node Child = OneNode.GetFirstChild(); Child!=0; Child = Child.GetNextSibling())
            {
                phys::String Name(Child.Name());
                switch(Name[0])
                {
                    case 'A': // AmbientLight
                        if(Name==phys::String("AmbientLight"))
                        {
                            if(phys::String(Child.GetFirstChild().Name()) == "ColourValue")
                            {
                                phys::ColourValue AllAroundUs;
                                Child.GetFirstChild() >> AllAroundUs;
                                Ev.SetAmbientLight(AllAroundUs);
                            }else{
                                throw( phys::Exception(phys::StringCat("Incompatible XML for SceneManager: Includes unknown Element AmbientLight-\"",Name,"\"")) );
                            }
                        }else{
                            throw( phys::Exception(phys::StringCat("Incompatible XML for SceneManager: Includes unknown Element Sd-\"",Name,"\"")) );
                        }
                        break;
                    case 'L': // Light
                        if(Name==phys::String("Light"))
                        {
                            phys::String ChildName(Child.GetAttribute("Name").AsString());
                            if(0!=ChildName.length())
                            {
                                phys::Light* ChildLight = Ev.CreateLight(ChildName);
                                Child >> *ChildLight;
                            }else{
                                throw( phys::Exception("Attemping to deserialize nameless light during deserialization of SceneManager but lights must have a name.") );
                            }
                        }else{
                            throw( phys::Exception(phys::StringCat("Incompatible XML for SceneManager: Includes unknown Element L-\"",Name,"\"")) );
                        }
                        break;
                    case 'P': // Particle Effect
                        if(Name==phys::String("ParticleEffect"))
                        {
                            phys::String ChildName(Child.GetAttribute("Name").AsString());
                            if(0!=ChildName.length())
                            {
                                phys::String ChildTemplate(Child.GetAttribute("Template").AsString());
                                if(0!=ChildTemplate.length())
                                {
                                    phys::ParticleEffect* ChildPE = Ev.CreateParticleEffect(ChildName,ChildTemplate);
                                    Child >> *ChildPE;
                                }else{
                                    throw( phys::Exception("Attemping to deserialize Templateless ParticleEffect during deserialization of SceneManager but ParticleEffects must have a Template.") );
                                }
                            }else{
                                throw( phys::Exception("Attemping to deserialize nameless ParticleEffect during deserialization of SceneManager but ParticleEffects must have a name.") );
                            }
                        }else{
                            throw( phys::Exception(phys::StringCat("Incompatible XML for SceneManager: Includes unknown Element P-\"",Name,"\"")) );
                        }
                        break;

                    case 'W': // WorldNode
                        if(Name==phys::String("WorldNode"))
                        {

                            phys::String ChildName(Child.GetAttribute("Name").AsString());
                            if(0!=ChildName.length())
                            {
                                phys::WorldNode* ChildNode = 0;
                                switch(phys::WorldNode::NodeType(Child.GetAttribute("Template").AsWhole()))
                                {
                                    case phys::WorldNode::Free :
                                        ChildNode = Ev.CreateFreeNode(ChildName,phys::Vector3(),phys::Vector3());
                                        Child >> *ChildNode;
                                        break;
                                    case phys::WorldNode::Center :   //The orbitting node will handle this
                                        break;
                                    case phys::WorldNode::Orbit :
                                        ChildNode = Ev.CreateOrbitingNode(ChildName,phys::Vector3(),phys::Vector3(),true);
                                        Child >> *ChildNode;
                                        break;
                                    case phys::WorldNode::Stand :
                                        ChildNode = Ev.CreateStandNode(ChildName,phys::Vector3(),phys::Vector3());
                                        Child >> *ChildNode;
                                        break;
                                    default:
                                        throw( phys::Exception(phys::StringCat("Incompatible XML for SceneManager: Includes unknown Element W-\"",Name,"\"")) );
                                        break;
                                }
                            }else{
                                throw( phys::Exception("Attemping to deserialize nameless WorldNode during deserialization of SceneManager but WorldNodes must have a name.") );
                            }
                        }else{
                            throw( phys::Exception(phys::StringCat("Incompatible XML for SceneManager: Includes unknown Element W-\"",Name,"\"")) );
                        }
                        break;
                    case 'S': // Sky of some kind or "ShadowColor"
                        switch(Name[3])
                        {
                            case 'd': // ShadowColour
                                if(Name==phys::String("ShadowColour"))
                                {
                                    if(Child.GetFirstChild())
                                    {
                                        if(phys::String(Child.GetFirstChild().Name()) == "ColourValue")
                                        {
                                            phys::ColourValue InTheShade;
                                            Child.GetFirstChild() >> InTheShade;
                                            Ev.SetShadowColour(InTheShade);
                                        }else{
                                            throw( phys::Exception(phys::StringCat("Incompatible XML for SceneManager: Includes unknown Element ShadowColour-\"",Child.GetFirstChild().Name(),"\"")) );
                                        }
                                    }else{
                                        throw( phys::Exception("Incompatible XML for SceneManager: ShadowColour has no child"));
                                    }
                                }else{
                                    throw( phys::Exception(phys::StringCat("Incompatible XML for SceneManager: Includes unknown Element Sd-\"",Name,"\"")) );
                                }
                                break;
                            case 'B': // SkyBox
                                if(Name==phys::String("SkyBox"))
                                {
                                    if(Child.GetAttribute("Version").AsInt() == 1)
                                    {
                                        phys::Quaternion Orientation;
                                        if(phys::String(Child.GetFirstChild().Name()) == "Orientation")
                                        {
                                            Child.GetFirstChild().GetFirstChild() >> Orientation;
                                        }else{
                                            throw( phys::Exception(phys::StringCat("Incompatible XML for SceneManager: Includes unknown Element Orientation-\"",Name,"\"")) );
                                        }
                                        Ev.CreateSkyBox( Child.GetAttribute("MaterialName").AsString(),
                                                         Child.GetAttribute("MaterialGroupName").AsString(),
                                                         Child.GetAttribute("Distance").AsReal(),
                                                         Child.GetAttribute("DrawFirst").AsBool(),
                                                         Orientation);
                                    }else{
                                        throw( phys::Exception("Incompatible XML for SkyBox: Not Version 1"));
                                    }
                                }else{
                                    throw( phys::Exception(phys::StringCat("Incompatible XML for SceneManager: Includes unknown Element SB-\"",Name,"\"")) );
                                }
                                break;
                            case 'D': // SkyDome
                                if(Name==phys::String("SkyDome"))
                                {
                                    if(Child.GetAttribute("Version").AsInt() == 1)
                                    {
                                        phys::Quaternion Orientation;
                                        if(phys::String(Child.GetFirstChild().Name()) == "Orientation")
                                        {
                                            Child.GetFirstChild().GetFirstChild() >> Orientation;
                                        }else{
                                            throw( phys::Exception(phys::StringCat("Incompatible XML for SceneManager: Includes unknown Element Orientation-\"",Name,"\"")) );
                                        }
                                        Ev.CreateSkyDome( Child.GetAttribute("MaterialName").AsString(),
                                                          Child.GetAttribute("MaterialGroupName").AsString(),
                                                          Child.GetAttribute("Distance").AsReal(),
                                                          Child.GetAttribute("Curvature").AsReal(),
                                                          Child.GetAttribute("Tiling").AsReal(),
                                                          Child.GetAttribute("DrawFirst").AsBool(),
                                                          Orientation,
                                                          Child.GetAttribute("XSegments").AsInt(),
                                                          Child.GetAttribute("YSegments").AsInt());

                                    }else{
                                        throw( phys::Exception("Incompatible XML for SkyDome: Not Version 1"));
                                    }
                                }else{
                                    throw( phys::Exception(phys::StringCat("Incompatible XML for SceneManager: Includes unknown Element SD-\"",Name,"\"")) );
                                }
                                break;
                            case 'P': // SkyPlane
                                if(Name==phys::String("SkyPlane"))
                                {
                                    phys::Plane SkyOrientation;
                                    Child.GetFirstChild() >> SkyOrientation;

                                    if(Child.GetAttribute("Version").AsInt() == 1)
                                    {
                                        Ev.CreateSkyPlane(  SkyOrientation,
                                                            Child.GetAttribute("MaterialName").AsString(),
                                                            Child.GetAttribute("MaterialGroupName").AsString(),
                                                            Child.GetAttribute("Scale").AsReal(),
                                                            Child.GetAttribute("Tiling").AsReal(),
                                                            Child.GetAttribute("DrawFirst").AsBool(),
                                                            Child.GetAttribute("Bow").AsBool(),
                                                            Child.GetAttribute("XSegments").AsInt(),
                                                            Child.GetAttribute("YSegments").AsInt());
                                    }else{
                                        throw( phys::Exception("Incompatible XML for SkyPlane: Not Version 1"));
                                    }
                                }else{
                                    throw( phys::Exception(phys::StringCat("Incompatible XML for SceneManager: Includes unknown Element SP-\"",Name,"\"")) );
                                }
                                break;
                        }
                        break;
                        default:
                            throw( phys::Exception(phys::StringCat("Incompatible XML for SceneManager: Includes unknown Element def-\"",Name,"\"")) );
                }
            }
        }else{
            throw( phys::Exception("Incompatible XML Version for SceneManager: Not Version 1"));
        }
    }else{
        throw( phys::Exception(phys::StringCat("Attempting to deserialize a SceneManager, found a ", OneNode.Name())));
    }
}
#endif



#endif
