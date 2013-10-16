//© Copyright 2010 - 2013 BlackTopp Studios Inc.
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
#ifndef _graphicsscenemanager_cpp
#define _graphicsscenemanager_cpp

#include "Graphics/cameramanager.h"
#include "Graphics/scenemanager.h"
#include "areaeffectmanager.h"
#include "entresol.h"
#include "plane.h"
#include "UI/uimanager.h"
#include "Physics/physicsmanager.h"
#include "stringtool.h"

#include "Graphics/billboardsetproxy.h"
#include "Graphics/entityproxy.h"
#include "Graphics/lightproxy.h"
#include "Graphics/particlesystemproxy.h"

#include <memory>

#include <Ogre.h>

namespace Mezzanine
{
    namespace Graphics
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
                SkyMaterialName(""),
                SkyOrientation(0,0,0,0),
                SkyMaterialGroupName(""),
                SkyDrawnFirst(false),
                SkyThePlane(Vector3(0,0,0),0),
                ShadowTextureSize(512),
                OgreManager(0)
            {

            }


            /// @internal
            /// @brief Destructor
            ~SceneManagerData()
            {
                Ogre::Root::getSingleton().destroySceneManager(OgreManager);
            }
        };//SceneManagerData

        ///////////////////////////////////////////////////////////////////////////////
        // SceneManager Methods

        SceneManager::SceneManager(const String& InternalManagerTypeName) :
            ThreadResources(NULL)
        {
            this->SMD = new SceneManagerData(this);
            this->SMD->OgreManager = Ogre::Root::getSingleton().createSceneManager(InternalManagerTypeName);
            //this->SetAmbientLight(ColourValue(0.0,0.0,0.0));
            //const Ogre::ShadowCameraSetupPtr ShadowCam = Ogre::ShadowCameraSetupPtr(new Ogre::DefaultShadowCameraSetup());
            //OgreManager->setShadowCameraSetup(ShadowCam);
        }

        SceneManager::SceneManager(XML::Node& XMLNode) :
            ThreadResources(NULL)
        {
            this->SMD = new SceneManagerData(this);

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
            this->Deinitialize();

            delete this->SMD;
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
        // Creating Proxies

        BillboardSetProxy* SceneManager::CreateBillboardSetProxy(const UInt32 InitialPoolSize)
        {
            BillboardSetProxy* NewProxy = new BillboardSetProxy(InitialPoolSize,this);
            this->Proxies.push_back(NewProxy);
            return NewProxy;
        }

        BillboardSetProxy* SceneManager::CreateBillboardSetProxy(const XML::Node& SelfRoot)
        {
            BillboardSetProxy* NewProxy = new BillboardSetProxy(SelfRoot,this);
            this->Proxies.push_back(NewProxy);
            return NewProxy;
        }

        EntityProxy* SceneManager::CreateEntityProxy()
        {
            EntityProxy* NewProxy = new EntityProxy(this);
            this->Proxies.push_back(NewProxy);
            return NewProxy;
        }

        EntityProxy* SceneManager::CreateEntityProxy(Mesh* TheMesh)
        {
            EntityProxy* NewProxy = new EntityProxy(TheMesh,this);
            this->Proxies.push_back(NewProxy);
            return NewProxy;
        }

        EntityProxy* SceneManager::CreateEntityProxy(const String& MeshName, const String& GroupName)
        {
            EntityProxy* NewProxy = new EntityProxy(MeshName,GroupName,this);
            this->Proxies.push_back(NewProxy);
            return NewProxy;
        }

        EntityProxy* SceneManager::CreateEntityProxy(const XML::Node& SelfRoot)
        {
            EntityProxy* NewProxy = new EntityProxy(SelfRoot,this);
            this->Proxies.push_back(NewProxy);
            return NewProxy;
        }

        LightProxy* SceneManager::CreateLightProxy()
        {
            LightProxy* NewProxy = new LightProxy(this);
            this->Proxies.push_back(NewProxy);
            return NewProxy;
        }

        LightProxy* SceneManager::CreateLightProxy(const Graphics::LightType Type)
        {
            LightProxy* NewProxy = new LightProxy(Type,this);
            this->Proxies.push_back(NewProxy);
            return NewProxy;
        }

        LightProxy* SceneManager::CreateLightProxy(const XML::Node& SelfRoot)
        {
            LightProxy* NewProxy = new LightProxy(SelfRoot,this);
            this->Proxies.push_back(NewProxy);
            return NewProxy;
        }

        ParticleSystemProxy* SceneManager::CreateParticleSystemProxy(const String& Template)
        {
            ParticleSystemProxy* NewProxy = new ParticleSystemProxy(Template,this);
            this->Proxies.push_back(NewProxy);
            return NewProxy;
        }

        ParticleSystemProxy* SceneManager::CreateParticleSystemProxy(const XML::Node& SelfRoot)
        {
            ParticleSystemProxy* NewProxy = new ParticleSystemProxy(SelfRoot,this);
            this->Proxies.push_back(NewProxy);
            return NewProxy;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Proxy Management

        RenderableProxy* SceneManager::GetProxy(const UInt32 Index) const
            { return this->Proxies.at(Index); }

        UInt32 SceneManager::GetNumProxies() const
            { return this->Proxies.size(); }

        void SceneManager::DestroyProxy(RenderableProxy* ToBeDestroyed)
        {
            for( ProxyIterator ProxIt = this->Proxies.begin() ; ProxIt != this->Proxies.end() ; ++ProxIt )
            {
                if( ToBeDestroyed == (*ProxIt) ) {
                    this->Proxies.erase(ProxIt);
                    delete (*ProxIt);
                    return;
                }
            }
        }

        void SceneManager::DestroyAllProxies()
        {
            for( ProxyIterator ProxIt = this->Proxies.begin() ; ProxIt != this->Proxies.end() ; ++ProxIt )
            {
                delete (*ProxIt);
            }
            this->Proxies.clear();
        }

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

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        ConstString& SceneManager::GetName() const
            { return this->SMD->OgreManager->getName(); }

        void SceneManager::PauseAllParticles(bool Pause)
        {
            for( SceneManager::ProxyIterator it = Proxies.begin() ; it != Proxies.end() ; it++ )
                if( (*it)->GetProxyType() == Mezzanine::PT_Graphics_ParticleSystemProxy )
                    static_cast<ParticleSystemProxy*>( (*it) )->PauseParticleSystem(Pause);
        }

        void SceneManager::Pause(const UInt32 PL)
        {
            // Do nothing for now
        }

        void SceneManager::Initialize()
        {
            if( !this->Initialized )
            {
                // Manager Initializations
                //WorldManager::Initialize();

                CameraManager* CamMan = this->TheEntresol->GetCameraManager();
                if( CamMan ) {
                    CamMan->Initialize();
                }

                this->Initialized = true;
            }
        }

        void SceneManager::Deinitialize()
        {
            if( this->Initialized )
            {
                this->DestroyAllProxies();

                // Manager Initializations
                CameraManager* CamMan = this->TheEntresol->GetCameraManager();
                if( CamMan ) {
                    CamMan->Deinitialize();
                }

                this->Initialized = false;
            }
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Type Identifier Methods

        ManagerBase::ManagerType SceneManager::GetInterfaceType() const
            { return ManagerBase::MT_SceneManager; }

        String SceneManager::GetImplementationTypeName() const
            { return "DefaultSceneManager"; }

        ///////////////////////////////////////////////////////////////////////////////
        // Internal/Other

        Ogre::SceneManager* SceneManager::_GetGraphicsWorldPointer() const
            { return (this->SMD && this->SMD->OgreManager) ? this->SMD->OgreManager : 0; }

        SceneManagerData* SceneManager::_GetRawInternalDataPointer() const
            { return this->SMD; }

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

        ManagerBase* DefaultSceneManagerFactory::CreateManager(XML::Node& XMLNode)
        {
            return new SceneManager(XMLNode);
        }

        void DefaultSceneManagerFactory::DestroyManager(ManagerBase* ToBeDestroyed)
        {
            delete ToBeDestroyed;
        }
    }//Graphics
}//Mezzanine

///////////////////////////////////////////////////////////////////////////////
// Class External << Operators for streaming or assignment
std::ostream& operator << (std::ostream& stream, const Mezzanine::Graphics::SceneManager& Ev)
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

                Mezzanine::Graphics::SceneManagerData* SMD = Ev._GetRawInternalDataPointer();
                switch (Ev.WhichSky())
                {
                    case Mezzanine::Graphics::SceneManager::SkyNone:
                        // Nothing to do
                        break;
                    case Mezzanine::Graphics::SceneManager::SkyPlane:{
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
                    case Mezzanine::Graphics::SceneManager::SkyBox:{
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
                    case Mezzanine::Graphics::SceneManager::SkyDome:{
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

std::istream& MEZZ_LIB operator >> (std::istream& stream, Mezzanine::Graphics::SceneManager& Ev)
{
    Mezzanine::String OneTag( Mezzanine::XML::GetOneTag(stream) );
    Mezzanine::CountedPtr<Mezzanine::XML::Document> Doc( Mezzanine::XML::PreParseClassFromSingleTag("Mezzanine::", "SceneManager", OneTag) );

    Doc->GetFirstChild() >> Ev;

    return stream;
}

Mezzanine::XML::Node& operator >> (const Mezzanine::XML::Node& OneNode, Mezzanine::Graphics::SceneManager& Ev)
{
    if ( Mezzanine::String(OneNode.Name())==Mezzanine::String("SceneManager") )
    {
        if(OneNode.GetAttribute("Version").AsInt() == 1)
        {
            //Ev.SetType(static_cast<Mezzanine::SceneManager::SceneManagerType>(OneNode.GetAttribute("Type").AsInt()));
            Ev.SetSceneShadowTechnique((Mezzanine::Graphics::SceneManager::SceneShadowTechnique)(OneNode.GetAttribute("SceneShadowTechnique").AsInt()));
            Ev.SetShadowTextureCount(OneNode.GetAttribute("ShadowTextureCount").AsInt());
            Ev.SetShadowTextureSize(OneNode.GetAttribute("ShadowTextureSize").AsInt());
            Ev.SetShadowFarDistance(OneNode.GetAttribute("ShadowFarDistance").AsReal());
            Mezzanine::Graphics::SceneManager::SkyMethod DoubleCheck = (Mezzanine::Graphics::SceneManager::SkyMethod)(OneNode.GetAttribute("SkyMethod").AsInt());

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
