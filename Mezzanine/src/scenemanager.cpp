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
#ifndef _scenemanager_cpp
#define _scenemanager_cpp

#include "cameramanager.h"
#include "scenemanager.h"
#include "light.h"
#include "entity.h"
#include "entresol.h"
#include "plane.h"
#include "particleeffect.h"
#include "uimanager.h"
#include "stringtool.h"
#include "worldnode.h"

#include <memory>

#include <Ogre.h>

namespace Mezzanine
{
    namespace Internal
    {
        /// @brief Stores internal data for the SCeneManager to keep it from cluttering the Header file
        /// @internal
        class SceneManagerData
        {
            public:

            /// @internal
            /// @brief A Pointer to the scenemanager this works with
            SceneManager* SM;

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

            /// @internal
            /// @brief Disable the sky ina given scenemanager
            /// @param ScenePTR A pointer to the scenmanager.
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
            /// @brief update the information that is cached about the sky
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

            /// @internal
            /// @brief Create One of these, the data every scenemanager needs.
            /// @param _SM A pointer to the scenemanager this with work with in general
            SceneManagerData(SceneManager* _SM):
                SM(_SM),
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


            /// @internal
            /// @brief Destructor
            ~SceneManagerData()
            {
                CameraManager* CamMan = Entresol::GetSingletonPtr()->GetCameraManager();
                if(CamMan)
                {
                    CamMan->DestroyAllCameraControllers();
                    CamMan->DestroyAllCameras();
                    CamMan->SceneMan = 0;
                }
                Ogre::Root::getSingleton().destroySceneManager(OgreManager);
            }
        };
    }

    template<> SceneManager* Singleton<SceneManager>::SingletonPtr = 0;

    ///////////////////////////////////////////////////////////////////////////////
    /// Construction

    SceneManager::SceneManager(const String& InternalManagerTypeName)
    {
        this->SMD = new Internal::SceneManagerData(this);
        this->SMD->OgreManager = Ogre::Root::getSingleton().createSceneManager(InternalManagerTypeName);
        this->Priority = 30;
        //this->SetAmbientLight(ColourValue(0.0,0.0,0.0));
        //const Ogre::ShadowCameraSetupPtr ShadowCam = Ogre::ShadowCameraSetupPtr(new Ogre::DefaultShadowCameraSetup());
        //OgreManager->setShadowCameraSetup(ShadowCam);
    }

    SceneManager::SceneManager(XML::Node& XMLNode)
    {
        this->SMD = new Internal::SceneManagerData(this);
        this->Priority = 30;

        XML::Attribute CurrAttrib;
        // Get the name of the manager to construct.
        String ManagerName;
        XML::Node ManagerType = XMLNode.GetChild("ManagerType");
        if(!ManagerType.Empty())
        {
            CurrAttrib = ManagerType.GetAttribute("TypeName");
            if(!CurrAttrib.Empty())
                ManagerName = CurrAttrib.AsString();
        }
        if(ManagerName.empty())
            ManagerName = "Default";
        this->SMD->OgreManager = Ogre::Root::getSingleton().createSceneManager(ManagerName+"SceneManager");

        // Setup the shadow configuration
        bool TextureShadows = false;
        XML::Node ShadowSettings = XMLNode.GetChild("ShadowSettings");
        if(!ShadowSettings.Empty())
        {
            String TechniqueName;
            CurrAttrib = ShadowSettings.GetAttribute("Technique");
            if(!CurrAttrib.Empty())
            {
                TechniqueName = CurrAttrib.AsString();
                if( "StencilModulative" == TechniqueName )
                    SetSceneShadowTechnique(SST_Stencil_Modulative);
                else if( "StencilAdditive" == TechniqueName )
                    SetSceneShadowTechnique(SST_Stencil_Additive);
                else if( "TextureModulative" == TechniqueName )
                    SetSceneShadowTechnique(SST_Texture_Modulative);
                else if( "TextureAdditive" == TechniqueName )
                    SetSceneShadowTechnique(SST_Texture_Additive);
                else if( "TextureAdditiveIntegrated" == TechniqueName )
                    SetSceneShadowTechnique(SST_Texture_Additive_Integrated);
                else if( "TextureModulativeIntegrated" == TechniqueName )
                    SetSceneShadowTechnique(SST_Texture_Modulative_Integrated);
            }

            if(!TechniqueName.empty())
            {
                if(String::npos != TechniqueName.find("Texture"))
                    TextureShadows = true;

                CurrAttrib = ShadowSettings.GetAttribute("ShadowColour");
                if(!CurrAttrib)
                    SetShadowColour(StringTools::ConvertToColourValue(CurrAttrib.AsString()));

                CurrAttrib = ShadowSettings.GetAttribute("ShadowFarDistance");
                if(!CurrAttrib)
                    SetShadowFarDistance(CurrAttrib.AsReal());
            }
        }

        // Setup texture shadow settings if any are set.
        XML::Node TextureShadowSettings = XMLNode.GetChild("TextureShadowSettings");
        if(!TextureShadowSettings.Empty() && TextureShadows)
        {
            CurrAttrib = TextureShadowSettings.GetAttribute("ShadowTextureCount");
            if(!CurrAttrib.Empty())
                SetShadowTextureCount(CurrAttrib.AsWhole());

            CurrAttrib = TextureShadowSettings.GetAttribute("ShadowTextureSize");
            if(!CurrAttrib.Empty())
                SetShadowTextureSize(static_cast<unsigned short>(CurrAttrib.AsWhole()));
        }
    }

    SceneManager::~SceneManager()
    {
        DestroyAllLights();
        DestroyAllParticleEffects();
        DestroyAllWorldNodes();
        delete SMD;
    }

    void SceneManager::UpdateTrackingNodes()
    {
        for( std::set<WorldNode*>::iterator it = TrackingNodes.begin() ; it != TrackingNodes.end() ; ++it )
        {
            (*it)->_UpdateTracking();
        }
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
        this->SMD->DisableSky(this);
        this->SMD->UpdateSkyCache();
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

    Light* SceneManager::GetLight(const Whole& Index) const
    {
        return Lights[Index];
    }

    Whole SceneManager::GetNumLights() const
    {
        return Lights.size();
    }

    void SceneManager::DestroyLight(Light* ToBeDestroyed)
    {
        if(Lights.empty())
            return;
        for( std::vector<Light*>::iterator it = Lights.begin() ; it != Lights.end() ; it++ )
        {
            if( ToBeDestroyed == (*it) )
            {
                delete (*it);
                Lights.erase(it);
                return;
            }
        }
    }

    void SceneManager::DestroyAllLights()
    {
        for( Whole X = 0 ; X < Lights.size() ; X++ )
            delete Lights[X];
        Lights.clear();
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
        ParticleEffect* Particle = new ParticleEffect(Name,Template,this);
        Particles.push_back(Particle);
        return Particle;
    }

    ParticleEffect* SceneManager::GetParticleEffect(const String& Name) const
    {
        if(Particles.empty())
            return 0;
        for( SceneManager::ConstParticleEffectIterator it = Particles.begin() ; it != Particles.end() ; it++ )
        {
            if( Name == (*it)->GetName() )
            {
                return (*it);
            }
        }
        return 0;
    }

    ParticleEffect* SceneManager::GetParticleEffect(const Whole& Index) const
    {
        return Particles[Index];
    }

    Whole SceneManager::GetNumParticleEffects() const
    {
        return Particles.size();
    }

    void SceneManager::DestroyParticleEffect(ParticleEffect* ToBeDestroyed)
    {
        if(Particles.empty())
            return;
        for( SceneManager::ParticleEffectIterator it = Particles.begin() ; it != Particles.end() ; it++ )
        {
            if( ToBeDestroyed == (*it) )
            {
                delete (*it);
                Particles.erase(it);
                return;
            }
        }
    }

    void SceneManager::DestroyAllParticleEffects()
    {
        for( Whole X = 0 ; X < Particles.size() ; X++ )
            delete Particles[X];
        Particles.clear();
    }

    void SceneManager::PauseAllParticles(bool Pause)
    {
        for( SceneManager::ParticleEffectIterator it = Particles.begin() ; it != Particles.end() ; it++ )
            (*it)->PauseParticleEffect(Pause);
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
    // Entity Management

    Entity* SceneManager::CreateEntity(const String& EntName, const String& MeshName, const String& Group)
    {
        Entity* Ent = new Entity(EntName,MeshName,Group,this);
        Entities.push_back(Ent);
        return Ent;
    }

    Entity* SceneManager::GetEntity(const String& Name) const
    {
        if(Entities.empty())
            return 0;
        for( std::vector<Entity*>::const_iterator it = Entities.begin() ; it != Entities.end() ; it++ )
        {
            if( Name == (*it)->GetName() )
            {
                return (*it);
            }
        }
        return 0;
    }

    Entity* SceneManager::GetEntity(const Whole& Index) const
    {
        return Entities[Index];
    }

    Whole SceneManager::GetNumEntities() const
    {
        return Entities.size();
    }

    void SceneManager::DestroyEntity(Entity* ToBeDestroyed)
    {
        if(Entities.empty())
            return;
        for( std::vector<Entity*>::iterator it = Entities.begin() ; it != Entities.end() ; it++ )
        {
            if( ToBeDestroyed == (*it) )
            {
                delete (*it);
                Entities.erase(it);
                return;
            }
        }
    }

    void SceneManager::DestroyAllEntities()
    {
        for( Whole X = 0 ; X < Entities.size() ; X++ )
            delete Entities[X];
        Entities.clear();
    }

    SceneManager::EntityIterator SceneManager::BeginEntity()
        { return this->Entities.begin(); }

    SceneManager::EntityIterator SceneManager::EndEntity()
        { return this->Entities.end(); }

    SceneManager::ConstEntityIterator SceneManager::BeginEntity() const
        { return this->Entities.begin(); }

    SceneManager::ConstEntityIterator SceneManager::EndEntity() const
        { return this->Entities.end(); }

    ///////////////////////////////////////////////////////////////////////////////
    // WorldNode Management

    WorldNode* SceneManager::CreateWorldNode(const String& Name)
    {
        WorldNode* MezzNode = new WorldNode(Name,this);
        WorldNodes.push_back(MezzNode);
        return MezzNode;
    }

    WorldNode* SceneManager::GetNode(const String& Name) const
    {
        if(WorldNodes.empty())
            return 0;
        for( ConstWorldNodeIterator it = WorldNodes.begin() ; it != WorldNodes.end() ; it++ )
        {
            if( Name == (*it)->GetName() )
            {
                WorldNode* node = (*it);
                return node;
            }
        }
        return 0;
    }

    WorldNode* SceneManager::GetNode(const Whole& Index) const
    {
        return WorldNodes[Index];
    }

    Whole SceneManager::GetNumNodes() const
    {
        return WorldNodes.size();
    }

    void SceneManager::DestroyNode(WorldNode* ToBeDestroyed)
    {
        if(WorldNodes.empty())
            return;
        for( WorldNodeIterator it = WorldNodes.begin() ; it != WorldNodes.end() ; it++ )
        {
            if( ToBeDestroyed == (*it) )
            {
                delete (*it);
                WorldNodes.erase(it);
                return;
            }
        }
    }

    void SceneManager::DestroyAllWorldNodes()
    {
        for( Whole X = 0 ; X < WorldNodes.size() ; X++ )
            delete WorldNodes[X];
        WorldNodes.clear();
    }

    SceneManager::WorldNodeIterator SceneManager::BeginWorldNode()
        { return this->WorldNodes.begin(); }

    SceneManager::WorldNodeIterator SceneManager::EndWorldNode()
        { return this->WorldNodes.end(); }

    SceneManager::ConstWorldNodeIterator SceneManager::BeginWorldNode() const
        { return this->WorldNodes.begin(); }

    SceneManager::ConstWorldNodeIterator SceneManager::EndWorldNode() const
        { return this->WorldNodes.end(); }

    void SceneManager::_RegisterTrackingNode(WorldNode* Tracker)
        { TrackingNodes.insert(Tracker); }

    void SceneManager::_UnRegisterTrackingNode(WorldNode* Tracker)
        { TrackingNodes.erase(TrackingNodes.find(Tracker)); }

    ///////////////////////////////////////////////////////////////////////////////
    // Basic Functionality

    ConstString& SceneManager::GetName() const
        { return this->SMD->OgreManager->getName(); }

    Ogre::SceneManager* SceneManager::GetGraphicsWorldPointer() const
        { return (this->SMD && this->SMD->OgreManager) ? this->SMD->OgreManager : 0; }

    Internal::SceneManagerData* SceneManager::GetRawInternalDataPointer() const
        { return this->SMD; }

    ///////////////////////////////////////////////////////////////////////////////
    // Inherited From ManagerBase

    void SceneManager::Initialize()
        { this->Initialized = true; }

    void SceneManager::DoMainLoopItems()
        { UpdateTrackingNodes(); }

    ManagerBase::ManagerType SceneManager::GetInterfaceType() const
        { return ManagerBase::SceneManager; }

    String SceneManager::GetImplementationTypeName() const
        { return "DefaultSceneManager"; }

    ///////////////////////////////////////////////////////////////////////////////
    // DefaultSceneManagerFactory Methods

    DefaultSceneManagerFactory::DefaultSceneManagerFactory()
    {
    }

    DefaultSceneManagerFactory::~DefaultSceneManagerFactory()
    {
    }

    String DefaultSceneManagerFactory::GetManagerTypeName() const
    {
        return "DefaultSceneManager";
    }

    ManagerBase* DefaultSceneManagerFactory::CreateManager(NameValuePairList& Params)
    {
        if(SceneManager::SingletonValid())
        {
            /// @todo Add something to log a warning that the manager exists and was requested to be constructed when we have a logging manager set up.
            return SceneManager::GetSingletonPtr();
        }else{
            if(Params.empty()) return new SceneManager();
            else
            {
                String InternalManagerTypeName;
                for( NameValuePairList::iterator ParIt = Params.begin() ; ParIt != Params.end() ; ++ParIt )
                {
                    String Lower = (*ParIt).first;
                    StringTools::ToLowerCase(Lower);
                    if( "internalmanagertypename" == Lower )
                    {
                        InternalManagerTypeName = (*ParIt).second;
                    }
                }
                return new SceneManager(InternalManagerTypeName);
            }
        }
    }

    ManagerBase* DefaultSceneManagerFactory::CreateManager(XML::Node& XMLNode)
    {
        if(SceneManager::SingletonValid())
        {
            /// @todo Add something to log a warning that the manager exists and was requested to be constructed when we have a logging manager set up.
            return SceneManager::GetSingletonPtr();
        }else return new SceneManager(XMLNode);
    }

    void DefaultSceneManagerFactory::DestroyManager(ManagerBase* ToBeDestroyed)
    {
        delete ToBeDestroyed;
    }
}//Mezzanine

///////////////////////////////////////////////////////////////////////////////
// Class External << Operators for streaming or assignment
std::ostream& operator << (std::ostream& stream, const Mezzanine::SceneManager& Ev)
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

                Mezzanine::Internal::SceneManagerData* SMD = Ev.GetRawInternalDataPointer();
                switch (Ev.WhichSky())
                {
                    case Mezzanine::SceneManager::SkyNone:
                        // Nothing to do
                        break;
                    case Mezzanine::SceneManager::SkyPlane:{
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
                    case Mezzanine::SceneManager::SkyBox:{
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
                    case Mezzanine::SceneManager::SkyDome:{
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

                for (Mezzanine::SceneManager::ConstLightIterator Iter = Mezzanine::SceneManager::GetSingletonPtr()->BeginLight();
                        Mezzanine::SceneManager::GetSingletonPtr()->EndLight()!=Iter;
                        ++Iter)
                    { stream << **Iter; }
                for (Mezzanine::SceneManager::ConstParticleEffectIterator Iter = Mezzanine::SceneManager::GetSingletonPtr()->BeginParticleEffect();
                        Mezzanine::SceneManager::GetSingletonPtr()->EndParticleEffect()!=Iter;
                        ++Iter)
                    { stream << **Iter; }
                for (Mezzanine::SceneManager::ConstWorldNodeIterator Iter = Mezzanine::SceneManager::GetSingletonPtr()->BeginWorldNode();
                        Mezzanine::SceneManager::GetSingletonPtr()->EndWorldNode()!=Iter;
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

std::istream& MEZZ_LIB operator >> (std::istream& stream, Mezzanine::SceneManager& Ev)
{
    Mezzanine::String OneTag( Mezzanine::XML::GetOneTag(stream) );
    std::auto_ptr<Mezzanine::XML::Document> Doc( Mezzanine::XML::PreParseClassFromSingleTag("Mezzanine::", "SceneManager", OneTag) );

    Doc->GetFirstChild() >> Ev;

    return stream;
}

Mezzanine::XML::Node& operator >> (const Mezzanine::XML::Node& OneNode, Mezzanine::SceneManager& Ev)
{
    if ( Mezzanine::String(OneNode.Name())==Mezzanine::String("SceneManager") )
    {
        if(OneNode.GetAttribute("Version").AsInt() == 1)
        {
            //Ev.SetType(static_cast<Mezzanine::SceneManager::SceneManagerType>(OneNode.GetAttribute("Type").AsInt()));
            Ev.SetSceneShadowTechnique((Mezzanine::SceneManager::SceneShadowTechnique)(OneNode.GetAttribute("SceneShadowTechnique").AsInt()));
            Ev.SetShadowTextureCount(OneNode.GetAttribute("ShadowTextureCount").AsInt());
            Ev.SetShadowTextureSize(OneNode.GetAttribute("ShadowTextureSize").AsInt());
            Ev.SetShadowFarDistance(OneNode.GetAttribute("ShadowFarDistance").AsReal());
            Mezzanine::SceneManager::SkyMethod DoubleCheck = (Mezzanine::SceneManager::SkyMethod)(OneNode.GetAttribute("SkyMethod").AsInt());

            for(Mezzanine::XML::Node Child = OneNode.GetFirstChild(); Child!=0; Child = Child.GetNextSibling())
            {
                Mezzanine::String Name(Child.Name());
                switch(Name[0])
                {
                    case 'A': // AmbientLight
                        if(Name==Mezzanine::String("AmbientLight"))
                        {
                            if(Mezzanine::String(Child.GetFirstChild().Name()) == "ColourValue")
                            {
                                Mezzanine::ColourValue AllAroundUs;
                                Child.GetFirstChild() >> AllAroundUs;
                                Ev.SetAmbientLight(AllAroundUs);
                            }else{
                                MEZZ_EXCEPTION(Mezzanine::Exception::INVALID_VERSION_EXCEPTION,"Incompatible XML for SceneManager: Includes unknown Element AmbientLight-\"" + Name + "\".");
                            }
                        }else{
                            MEZZ_EXCEPTION(Mezzanine::Exception::INVALID_VERSION_EXCEPTION,"Incompatible XML for SceneManager: Includes unknown Element Sd-\"" + Name + "\".");
                        }
                        break;
                    case 'L': // Light
                        if(Name==Mezzanine::String("Light"))
                        {
                            Mezzanine::String ChildName(Child.GetAttribute("Name").AsString());
                            if(0!=ChildName.length())
                            {
                                Mezzanine::Light* ChildLight = Ev.CreateLight(ChildName);
                                Child >> *ChildLight;
                            }else{
                                MEZZ_EXCEPTION(Mezzanine::Exception::INVALID_PARAMETERS_EXCEPTION,"Attemping to deserialize nameless light during deserialization of SceneManager but lights must have a name.");
                            }
                        }else{
                            MEZZ_EXCEPTION(Mezzanine::Exception::INVALID_VERSION_EXCEPTION,"Incompatible XML for SceneManager: Includes unknown Element L-\"" + Name + "\".");
                        }
                        break;
                    case 'P': // Particle Effect
                        if(Name==Mezzanine::String("ParticleEffect"))
                        {
                            Mezzanine::String ChildName(Child.GetAttribute("Name").AsString());
                            if(0!=ChildName.length())
                            {
                                Mezzanine::String ChildTemplate(Child.GetAttribute("Template").AsString());
                                if(0!=ChildTemplate.length())
                                {
                                    Mezzanine::ParticleEffect* ChildPE = Ev.CreateParticleEffect(ChildName,ChildTemplate);
                                    Child >> *ChildPE;
                                }else{
                                    MEZZ_EXCEPTION(Mezzanine::Exception::INVALID_PARAMETERS_EXCEPTION,"Attemping to deserialize Templateless ParticleEffect during deserialization of SceneManager but ParticleEffects must have a Template.");
                                }
                            }else{
                                MEZZ_EXCEPTION(Mezzanine::Exception::INVALID_PARAMETERS_EXCEPTION,"Attemping to deserialize nameless ParticleEffect during deserialization of SceneManager but ParticleEffects must have a name.");
                            }
                        }else{
                            MEZZ_EXCEPTION(Mezzanine::Exception::INVALID_VERSION_EXCEPTION,"Incompatible XML for SceneManager: Includes unknown Element P-\"" + Name + "\".");
                        }
                        break;

                    case 'W': // WorldNode
                        if(Name==Mezzanine::String("WorldNode"))
                        {

                            Mezzanine::String ChildName(Child.GetAttribute("Name").AsString());
                            if(0!=ChildName.length())
                            {
                                Mezzanine::WorldNode* ChildNode = Ev.CreateWorldNode(ChildName);
                            }else{
                                MEZZ_EXCEPTION(Mezzanine::Exception::INVALID_PARAMETERS_EXCEPTION,"Attemping to deserialize nameless WorldNode during deserialization of SceneManager but WorldNodes must have a name.");
                            }
                        }else{
                            MEZZ_EXCEPTION(Mezzanine::Exception::INVALID_VERSION_EXCEPTION,"Incompatible XML for SceneManager: Includes unknown Element W-\"" + Name + "\".");
                        }
                        break;
                    case 'S': // Sky of some kind or "ShadowColor"
                        switch(Name[3])
                        {
                            case 'd': // ShadowColour
                                if(Name==Mezzanine::String("ShadowColour"))
                                {
                                    if(Child.GetFirstChild())
                                    {
                                        if(Mezzanine::String(Child.GetFirstChild().Name()) == "ColourValue")
                                        {
                                            Mezzanine::ColourValue InTheShade;
                                            Child.GetFirstChild() >> InTheShade;
                                            Ev.SetShadowColour(InTheShade);
                                        }else{
                                            MEZZ_EXCEPTION(Mezzanine::Exception::INVALID_VERSION_EXCEPTION,"Incompatible XML for SceneManager: Includes unknown Element ShadowColour-\"" + Mezzanine::String(Child.GetFirstChild().Name()) + "\".");
                                        }
                                    }else{
                                        MEZZ_EXCEPTION(Mezzanine::Exception::INVALID_VERSION_EXCEPTION,"Incompatible XML for SceneManager: ShadowColour has no child");
                                    }
                                }else{
                                    MEZZ_EXCEPTION(Mezzanine::Exception::INVALID_VERSION_EXCEPTION,"Incompatible XML for SceneManager: Includes unknown Element Sd-\"" + Name + "\".");
                                }
                                break;
                            case 'B': // SkyBox
                                if(Name==Mezzanine::String("SkyBox"))
                                {
                                    if(Child.GetAttribute("Version").AsInt() == 1)
                                    {
                                        Mezzanine::Quaternion Orientation;
                                        if(Mezzanine::String(Child.GetFirstChild().Name()) == "Orientation")
                                        {
                                            Child.GetFirstChild().GetFirstChild() >> Orientation;
                                        }else{
                                            MEZZ_EXCEPTION(Mezzanine::Exception::INVALID_VERSION_EXCEPTION,"Incompatible XML for SceneManager: Includes unknown Element Orientation-\"" + Name + "\".");
                                        }
                                        Ev.CreateSkyBox( Child.GetAttribute("MaterialName").AsString(),
                                                         Child.GetAttribute("MaterialGroupName").AsString(),
                                                         Child.GetAttribute("Distance").AsReal(),
                                                         Child.GetAttribute("DrawFirst").AsBool(),
                                                         Orientation);
                                    }else{
                                        MEZZ_EXCEPTION(Mezzanine::Exception::INVALID_VERSION_EXCEPTION,"Incompatible XML for SkyBox: Not Version 1.");
                                    }
                                }else{
                                    MEZZ_EXCEPTION(Mezzanine::Exception::INVALID_VERSION_EXCEPTION,"Incompatible XML for SceneManager: Includes unknown Element SB-\"" + Name + "\".");
                                }
                                break;
                            case 'D': // SkyDome
                                if(Name==Mezzanine::String("SkyDome"))
                                {
                                    if(Child.GetAttribute("Version").AsInt() == 1)
                                    {
                                        Mezzanine::Quaternion Orientation;
                                        if(Mezzanine::String(Child.GetFirstChild().Name()) == "Orientation")
                                        {
                                            Child.GetFirstChild().GetFirstChild() >> Orientation;
                                        }else{
                                            MEZZ_EXCEPTION(Mezzanine::Exception::INVALID_VERSION_EXCEPTION,"Incompatible XML for SceneManager: Includes unknown Element Orientation-\"" + Name + "\".");
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
                                        MEZZ_EXCEPTION(Mezzanine::Exception::INVALID_VERSION_EXCEPTION,"Incompatible XML for SkyDome: Not Version 1.");
                                    }
                                }else{
                                    MEZZ_EXCEPTION(Mezzanine::Exception::INVALID_VERSION_EXCEPTION,"Incompatible XML for SceneManager: Includes unknown Element SD-\"" + Name + "\".");
                                }
                                break;
                            case 'P': // SkyPlane
                                if(Name==Mezzanine::String("SkyPlane"))
                                {
                                    Mezzanine::Plane SkyOrientation;
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
                                        MEZZ_EXCEPTION(Mezzanine::Exception::INVALID_VERSION_EXCEPTION,"Incompatible XML for SkyPlane: Not Version 1.");
                                    }
                                }else{
                                    MEZZ_EXCEPTION(Mezzanine::Exception::INVALID_VERSION_EXCEPTION,"Incompatible XML for SceneManager: Includes unknown Element SP-\"" + Name + "\".");
                                }
                                break;
                        }
                        break;
                        default:
                            { MEZZ_EXCEPTION(Mezzanine::Exception::INVALID_VERSION_EXCEPTION,"Incompatible XML for SceneManager: Includes unknown Element def-\"" + Name + "\"."); }
                }
            }
        }else{
            MEZZ_EXCEPTION(Mezzanine::Exception::INVALID_VERSION_EXCEPTION,"Incompatible XML Version for SceneManager: Not Version 1.");
        }
    }else{
        MEZZ_EXCEPTION(Mezzanine::Exception::II_IDENTITY_INVALID_EXCEPTION,"Attempting to deserialize a SceneManager, found a " + Mezzanine::String(OneNode.Name()));
    }
}



#endif
