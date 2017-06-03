// © Copyright 2010 - 2017 BlackTopp Studios Inc.
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
#ifndef _graphicsscenemanager_h
#define _graphicsscenemanager_h

#include "quaternion.h"
#include "vector3.h"
#include "colourvalue.h"
#include "singleton.h"

#include "uidgenerator.h"
#include "worldproxymanager.h"
#include "worldmanagerfactory.h"

#include "Graphics/graphicsenumerations.h"
#ifndef SWIG
    #include "XML/xml.h"
#endif
#include "Threading/workunit.h"

namespace Ogre
{
    class SceneManager;
}

namespace Mezzanine
{
    class Entresol;
    class Plane;
    namespace Graphics
    {
        class RenderableProxy;
        class BillboardSetProxy;
        class CameraProxy;
        class EntityProxy;
        class LightProxy;
        class LineGroupProxy;
        class ParticleSystemProxy;
        class SceneManager;
        class SceneManagerData;
        class Mesh;

        ///////////////////////////////////////////////////////////////////////////////
        /// @brief This class contains utilities and functions to allow the manipulation of the Graphical
        /// scene, rather then the physics inside, or the object inside.
        /// @details This class contains functions that allow the manipulation of lighting, skyboxes, internal
        /// scenemanager types, and more.
        ///////////////////////////////////////
        class MEZZ_LIB SceneManager : public WorldProxyManager
        {
        public:
            /// @brief Basic container type for RenderableProxy storage by this class.
            typedef std::vector< RenderableProxy* >               ProxyContainer;
            /// @brief Iterator type for RenderableProxy instances stored by this class.
            typedef ProxyContainer::iterator                      ProxyIterator;
            /// @brief Const Iterator type for RenderableProxy instances stored by this class.
            typedef ProxyContainer::const_iterator                ConstProxyIterator;

            /// @brief A String containing the name of this manager implementation.
            static const String ImplementationName;
            /// @brief A ManagerType enum value used to describe the type of interface/functionality this manager provides.
            static const ManagerBase::ManagerType InterfaceType;

            /// @brief needs to be documented
            enum SceneShadowTechnique
            {                                     //Shadow Docs from Ogre ShadowTechnique Documentation.
                SST_None                          = 0,      ///< No shadows.
                SST_Stencil_Modulative            = 1,      ///< Stencil shadow technique which renders all shadow volumes as a modulation after all the non-transparent areas have been rendered.
                SST_Stencil_Additive              = 2,      ///< Stencil shadow technique which renders each light as a separate additive pass to the scene.
                SST_Texture_Modulative            = 11,     ///< Texture-based shadow technique which involves a monochrome render-to-texture of the shadow caster and a projection of that texture onto the shadow receivers as a modulative pass.
                SST_Texture_Additive              = 12,     ///< Texture-based shadow technique which involves a render-to-texture of the shadow caster and a projection of that texture onto the shadow receivers, built up per light as additive passes.
                SST_Texture_Additive_Integrated   = 13,     ///< Texture-based shadow technique which involves a render-to-texture of the shadow caster and a projection of that texture on to the shadow receivers, with the usage of those shadow textures completely controlled by the materials of the receivers.
                SST_Texture_Modulative_Integrated = 14      ///< Texture-based shadow technique which involves a render-to-texture of the shadow caster and a projection of that texture on to the shadow receivers, with the usage of those shadow textures completely controlled by the materials of the receivers.
            };

            /// @brief Used to help identify which method is used to draw the sky, if any
            enum SkyMethod
            {
                SkyNone     = 0,        ///< No Sky rendering at all.
                SkyPlane    = 1,        ///< A flat plane use to draw the sky.
                SkyBox      = 2,        ///< A box using 5 Rectangles to draw the sky.
                SkyDome     = 3         ///< A multifaceted hemispherical dome, the most sophisticated sky background.
            };
        protected:
            /// @internal
            /// @brief Generator responsible for creating unique IDs for CollidableProxy instances.
            UIDGenerator ProxyIDGen;
            /// @internal
            /// @brief Container storing all of the RenderableProxy instances created by this manager.
            ProxyContainer Proxies;
            /// @internal
            /// @brief Pointer to a class storing sensative internal data for the scene.
            SceneManagerData* SMD;
            /// @internal
            /// @brief Can be used for thread safe logging and other thread specific resources.
            Threading::DefaultThreadSpecificStorage::Type* ThreadResources;
        public:
            /// @brief Class Constructor.
            /// @param Creator The parent world that is creating the manager.
            /// @param InternalManagerTypeName The name of the scenemanager type to be constructed.
            SceneManager(World* Creator, const String& InternalManagerTypeName = "DefaultSceneManager");
            /// @brief XML constructor.
            /// @param Creator The parent world that is creating the manager.
            /// @param XMLNode The node of the xml document to construct from.
            SceneManager(World* Creator, const XML::Node& XMLNode);
            /// @brief Class destructor.
            virtual ~SceneManager();

            ///////////////////////////////////////////////////////////////////////////////
            // Shadow Management

            /// @brief Sets the type of shadows to be used when rendering the scene.
            /// @details The scene manager defaults to no shadows.
            /// @param Shadows The technique to be applied, see SceneShadowTechnique enum for more info.
            void SetSceneShadowTechnique(SceneShadowTechnique Shadows);
            /// @brief Gets the currently set shadow technique.
            /// @return Returns a SceneShadowTechnique enum value representing the currently set shadow technique.
            SceneShadowTechnique GetSceneShadowTechnique() const;
            /// @brief Sets the number of textures to be alloted for creating shadows.
            /// @details Defaults to 1.
            /// @param Count The amount of textures to be used for creating texture-based shadows.
            void SetShadowTextureCount(const Whole& Count);
            /// @brief Gets the currently set number of textures being used to make texture shadows.
            /// @return Returns a Whole indicating the number of textures used to make texture shadows.
            Whole GetShadowTextureCount() const;
            /// @brief Sets the size of all texture based shadows.
            /// @details This defaults to 512.  Sizes must be a power of 2.
            /// @param Size The size of all textures to be used with shadows, in KB(?).
            void SetShadowTextureSize(unsigned short Size);
            /// @brief Retrieve the size of textures.
            /// @return An unsigned short which is the size of the textures.
            unsigned short GetShadowTextureSize() const;
            /// @brief Sets the maximum distance from the camera that shadows will be visible.
            /// @param FarDist The maximum distance from the camera shadows will be rendered.
            void SetShadowFarDistance(const Real& FarDist);
            /// @brief Gets the maximum distance from the camera that shadows will be visible.
            /// @return Returns a Real representing the maximum distance from the camera shadows will be rendered.
            Real GetShadowFarDistance() const;
            /// @brief Sets the colour to be used when casting shadows.
            /// @param ShadowColour The colour desired to be used when rendering shadows.
            void SetShadowColour(const ColourValue& ShadowColour);
            /// @brief Gets the colour being used when casting shadows.
            /// @return Returns a ColourValue representing the colour used when casting shadows.
            ColourValue GetShadowColour() const;

            ///////////////////////////////////////////////////////////////////////////////
            // Sky Surface Management

            /// @brief Creates a skyplane for use in making a sky.
            /// @details Only one skyplane can exist in a scene.  Making a new one will remove the old one.  Skyplanes are
            /// flat planes that face in one direction.  They are ideal for levels with surrounding mountains or anything
            /// where the horizon is not visable.
            /// @param SkyPlane The plane that will become the sky.
            /// @param Material The name of the material to be applied to the skyplane.  Note: This is not referring to the
            /// filename, but the specific material script within the file.
            /// @param Group The resource group where the material can be found.
            /// @param Scale The scaling to be applied to the skyplane.  This may need to be tweaked based on how high you
            /// set the plane off the ground.
            /// @param Tiling The number of times to tile the texture or textures listed in the material script across the skyplane.
            /// @param DrawFirst Whether or not the skyplane should be the first thing rendered in the scene.  Usually you will
            /// want this to be true as it'll ensure all other objects are rendered on top of it.
            /// @param Bow This will add curvature to the skyplane if set above zero.  Note: Use small numbers.  A bow of 1.5
            /// should be noticable.
            /// @param XSegments The number of segments, or boxes, the skyplane consists of on the planes X axis.  This is usful
            /// when giving the skyplane a bow.  By default the skyplane is just one massive box.
            /// @param YSegments The number of segments, or boxes, the skyplane consists of on the planes Y axis.  This is usful
            /// when giving the skyplane a bow.  By default the skyplane is just one massive box.
            void CreateSkyPlane(const Plane& SkyPlane_, const String& Material, const String& Group, Real Scale=1000.0, Real Tiling=10.0,
                                Boole DrawFirst=true, Real Bow=0, int XSegments=1, int YSegments=1);
            /// @brief Disables the currently active skyplane.
            /// @details Using this function effectively deletes the skyplane, so you will have to provide a new set of parameters
            /// if you wish to re-create the skyplane.
            void DisableSkyPlane();

            /// @brief Creates a skybox for use in making a sky.
            /// @details Like skyplanes, only one can exist per scene.  Unlike skyplanes, skyboxes will be applied individually to
            /// each camera in the scene.  The skybox will move with the camera, so as a result the camera will never be able to
            /// "touch" the sky.  Skyboxes are more performance intensive then skyplanes.
            /// @param Material The name of the material to be applied to the skybox.  Note: This is not referring to the
            /// filename, but the specific material script within the file.
            /// @param Group The resource group where the material can be found.
            /// @param Distance The distance from the camera where the skybox is found.  This is in world units.
            /// @param DrawFirst Whether or not the skybox should be the first thing rendered in the scene.  Usually you will
            /// want this to be true as it'll ensure all other objects are rendered on top of it.
            /// @param Orientation Optional quaternion to rotate the orientation of the skybox.
            void CreateSkyBox(const String& Material, const String& Group, Real Distance, Boole DrawFirst=true, Quaternion Orientation=Quaternion());
            /// @brief Disables the currently active skybox.
            /// @details Using this function effectively deletes the skybox, so you will have to provide a new set of parameters
            /// if you wish to re-create the skybox.
            void DisableSkyBox();

            /// @brief Creates a skydome for use in making a sky.
            /// @details Like the other two types of sky's, their can be only one skydome per scene.  Skydomes much like skyboxes, except
            /// they have 5 sides(the bottom side is missing), and they bow each of the sides to make the dome.  In all other respects they
            /// are the same.
            /// @param Material The name of the material to be applied to the skydome.  Note: This is not referring to the
            /// filename, but the specific material script within the file.
            /// @param Group The resource group where the material can be found.
            /// @param Distance The distance from the camera where the skydome is found.  This is in world units.
            /// @param Curvature Curvature of the dome.  Usually you want this value to be between 2 and 65.
            /// @param Tiling The number of times to tile the texture or textures listed in the material script across the skydome.
            /// @param DrawFirst Whether or not the skybox should be the first thing rendered in the scene.  Usually you will
            /// want this to be true as it'll ensure all other objects are rendered on top of it.
            /// @param Orientation Optional quaternion to rotate the orientation of the skydome.
            /// @param XSegments The number of segments, or boxes, the skydome consists of on the dome's X axis.
            /// @param YSegments The number of segments, or boxes, the skydome consists of on the dome's Y axis.
            void CreateSkyDome(const String& Material, const String& Group, Real Distance, Real Curvature=10.0, Real Tiling=8.0, Boole DrawFirst=true,
                               Quaternion Orientation=Quaternion(), int XSegments=16, int YSegments=16);
            /// @brief Disables the currently active skydome.
            /// @details Using this function effectively deletes the skydome, so you will have to provide a new set of parameters
            /// if you wish to re-create the skydome.
            void DisableSkyDome();

            /// @brief If any sky is active, disable it
            void DisableSky();

            /// @brief get the kind of sy in use
            /// @return The kind of sky in use
            SkyMethod WhichSky() const;

            ///////////////////////////////////////////////////////////////////////////////
            // Creating Proxies

            /// @brief Creates a new BillboardSetProxy.
            /// @param InitialPoolSize The number of billboards to reserve space for.  20 is a sane default.
            /// @return Returns a pointer to the created proxy.
            BillboardSetProxy* CreateBillboardSetProxy(const UInt32 InitialPoolSize = 20);
            /// @brief Creates a new BillboardSetProxy.
            /// @param SelfRoot An XML::Node containing the data to populate the new instance with.
            /// @return Returns a pointer to the created proxy.
            BillboardSetProxy* CreateBillboardSetProxy(const XML::Node& SelfRoot);

            /// @brief Creates a new camera.
            /// @return Returns a pointer to the created camera.
            CameraProxy* CreateCamera();
            /// @brief Creates a new camera.
            /// @param SelfRoot An XML::Node containing the data to populate the new instance with.
            /// @return Returns a pointer to the created camera.
            CameraProxy* CreateCamera(const XML::Node& SelfRoot);

            /// @brief Creates a new EntityProxy.
            /// @return Returns a pointer to the created proxy.
            EntityProxy* CreateEntityProxy();
            /// @brief Creates a new EntityProxy.
            /// @param TheMesh A pointer to the mesh to be applied to this proxy.
            /// @return Returns a pointer to the created proxy.
            EntityProxy* CreateEntityProxy(Mesh* TheMesh);
            /// @brief Creates a new EntityProxy.
            /// @param MeshName The name of the mesh to be loaded and applied to this proxy.
            /// @param GroupName The resource group name where the mesh can be found.
            /// @return Returns a pointer to the created proxy.
            EntityProxy* CreateEntityProxy(const String& MeshName, const String& GroupName);
            /// @brief Creates a new EntityProxy.
            /// @param SelfRoot An XML::Node containing the data to populate the new instance with.
            /// @return Returns a pointer to the created proxy.
            EntityProxy* CreateEntityProxy(const XML::Node& SelfRoot);

            /// @brief Creates a new LightProxy.
            /// @return Returns a pointer to the created proxy.
            LightProxy* CreateLightProxy();
            /// @brief Creates a new LightProxy.
            /// @param Type The type of light this light is to be constructed as.
            /// @return Returns a pointer to the created proxy.
            LightProxy* CreateLightProxy(const Graphics::LightType Type);
            /// @brief Creates a new LightProxy.
            /// @param SelfRoot An XML::Node containing the data to populate the new instance with.
            /// @return Returns a pointer to the created proxy.
            LightProxy* CreateLightProxy(const XML::Node& SelfRoot);

            /// @brief Creates a new LineGroupProxy.
            /// @return Returns a pointer to the created LineGroupProxy.
            LineGroupProxy* CreateLineGroupProxy();
            /// @brief Creates a new LineGroupProxy.
            /// @param SelfRoot An XML::Node containing the data to populate the new instance with.
            /// @return Returns a pointer to the created LineGroupProxy.
            LineGroupProxy* CreateLineGroupProxy(const XML::Node& SelfRoot);

            /// @brief Creates a new ParticleSystemProxy.
            /// @param Template Name of the particle script to be used in creating this particle effect.
            /// @return Returns a pointer to the created proxy.
            ParticleSystemProxy* CreateParticleSystemProxy(const String& Template);
            /// @brief Creates a new ParticleSystemProxy.
            /// @param SelfRoot An XML::Node containing the data to populate the new instance with.
            /// @return Returns a pointer to the created proxy.
            ParticleSystemProxy* CreateParticleSystemProxy(const XML::Node& SelfRoot);

            /// @copydoc WorldProxyManager::CreateProxy(const XML::Node&)
            WorldProxy* CreateProxy(const XML::Node& SelfRoot);

            ///////////////////////////////////////////////////////////////////////////////
            // Proxy Management

            /// @brief Gets a RenderableProxy instance by index.
            /// @param Index The index of the RenderableProxy to be retrieved.
            /// @return Returns a pointer to the RenderableProxy at the specified index.
            RenderableProxy* GetProxy(const UInt32 Index) const;

            /// @copydoc WorldProxyManager::GetProxyByID(const UInt32) const
            WorldProxy* GetProxyByID(const UInt32 ID) const;

            /// @copydoc WorldProxyManager::GetNumProxies() const
            UInt32 GetNumProxies() const;
            /// @copydoc WorldProxyManager::GetNumProxies(const UInt32) const
            UInt32 GetNumProxies(const UInt32 Types) const;
            /// @copydoc WorldProxyManager::GetProxies() const
            WorldProxyManager::WorldProxyVec GetProxies() const;

            /// @copydoc WorldProxyManager::DestroyProxy(WorldProxy*)
            void DestroyProxy(WorldProxy* ToBeDestroyed);
            /// @copydoc WorldProxyManager::DestroyAllProxies(const UInt32)
            void DestroyAllProxies(const UInt32 Types);
            /// @copydoc WorldProxyManager::DestroyAllProxies()
            void DestroyAllProxies();

            #ifndef SWIG
            /// @brief Gets an iterator to the first Renderable Proxy in this manager.
            /// @return Returns an iterator to the first Renderable Proxy being stored by this manager.
            ProxyIterator BeginRenderableProxy();
            /// @brief Gets an iterator to one passed the last Renderable Proxy in this manager.
            /// @return Returns an iterator to one passed the last Renderable Proxy being stored by this manager.
            ProxyIterator EndRenderableProxy();
            /// @brief Gets a const iterator to the first Renderable Proxy in this manager.
            /// @return Returns a const iterator to the first Renderable Proxy being stored by this manager.
            ConstProxyIterator BeginRenderableProxy() const;
            /// @brief Gets a const iterator to one passed the last Renderable Proxy in this manager.
            /// @return Returns a const iterator to one passed the last Renderable Proxy being stored by this manager.
            ConstProxyIterator EndRenderableProxy() const;
            #endif

            ///////////////////////////////////////////////////////////////////////////////
            // Light Management

            /// @brief Sets the ambient light for the scene.
            /// @details Not all scene's will need ambient light.  Ambient light is light that hits all objects from
            /// all directions.
            /// @param Red The value representing the amount of red color in the ambient light.
            /// @param Green The value representing the amount of green color in the ambient light.
            /// @param Blue The value representing the amount of blue color in the ambient light.
            /// @param Alpha The value representing the transparency of the color in the ambient light.
            void SetAmbientLight(Real Red=1.0, Real Green=1.0, Real Blue=1.0, Real Alpha=1.0);
            /// @brief Sets the ambient light for the scene, in a single value.
            /// @param Red The value representing the amount of red color in the ambient light.
            void SetAmbientLight(const ColourValue &LightColor);
            /// @brief Retrieve the level of the ambient light
            /// @return A ColourValue with the ambient light levels
            ColourValue GetAmbientLight() const;

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            /// @brief Gets the name of this manager.
            /// @return Returns the name of this manager.
            ConstString& GetName() const;

            /// @brief Pauses(or unpauses) all particles stored in this manager.
            /// @param Pause Will pause all Particles if true, unpause if false.
            void PauseAllParticles(Boole Pause);

            /// @copydoc WorldManager::Pause(const UInt32)
            virtual void Pause(const UInt32 PL);

            /// @copydoc WorldManager::Initialize()
            virtual void Initialize();
            /// @copydoc ManagerBase::Deinitialize()
            virtual void Deinitialize();

            ///////////////////////////////////////////////////////////////////////////////
            // Type Identifier Methods

            /// @copydoc ManagerBase::GetInterfaceType()
            virtual ManagerType GetInterfaceType() const;
            /// @copydoc ManagerBase::GetImplementationTypeName()
            virtual String GetImplementationTypeName() const;

            ///////////////////////////////////////////////////////////////////////////////
            // Internal/Other

            /// @internal
            /// @brief Gets the internal Ogre Scene Manager pointer.
            /// @return Returns a pointer to the ogre Scene Manager.
            Ogre::SceneManager* _GetGraphicsWorldPointer() const;
            /// @internal
            /// @brief Gets the raw internal internal data.
            /// @return Returns a to the raw internal data.
            SceneManagerData* _GetRawInternalDataPointer() const;
        };//SceneManager

        ///////////////////////////////////////////////////////////////////////////////
        /// @class DefaultSceneManagerFactory
        /// @brief A factory responsible for the creation and destruction of the default scenemanager.
        ///////////////////////////////////////
        class MEZZ_LIB DefaultSceneManagerFactory : public WorldManagerFactory
        {
        public:
            /// @brief Class constructor.
            DefaultSceneManagerFactory();
            /// @brief Class destructor.
            virtual ~DefaultSceneManagerFactory();

            /// @copydoc ManagerFactory::GetManagerImplName()
            String GetManagerImplName() const;
            /// @copydoc ManagerFactory::GetManagerType() const
            ManagerBase::ManagerType GetManagerType() const;

            /// @copydoc WorldManagerFactory::CreateManager(World*, const NameValuePairList&)
            WorldManager* CreateManager(World* Creator, const NameValuePairList& Params);
            /// @copydoc WorldManagerFactory::CreateManager(World*, const XML::Node&)
            WorldManager* CreateManager(World* Creator, const XML::Node& XMLNode);
            /// @copydoc WorldManagerFactory::DestroyManager(WorldManager*)
            void DestroyManager(WorldManager* ToBeDestroyed);
        };//DefaultSceneManagerFactory
    }//Graphics
}//Mezzanine

///////////////////////////////////////////////////////////////////////////////
// Class External << Operators for streaming or assignment
/// @brief Serializes the passed Mezzanine::Graphics::SceneManager to XML
/// @param stream The ostream to send the xml to.
/// @param Ev the Mezzanine::Graphics::SceneManager to be serialized
/// @return this returns the ostream, now with the serialized data
std::ostream& MEZZ_LIB operator << (std::ostream& stream, const Mezzanine::Graphics::SceneManager& Ev);

/// @brief Deserialize a Mezzanine::Graphics::SceneManager
/// @param stream The istream to get the xml from to (re)make the Mezzanine::Graphics::SceneManager.
/// @param Ev the Mezzanine::Graphics::SceneManager to be deserialized.
/// @return this returns the ostream, advanced past the Mezzanine::Graphics::SceneManager that was recreated onto Ev.
std::istream& MEZZ_LIB operator >> (std::istream& stream, Mezzanine::Graphics::SceneManager& Ev);

/// @brief Set all values of a Mezzanine::Graphics::SceneManager from parsed xml.
/// @param OneNode The istream to get the xml from to (re)make the Mezzanine::Graphics::SceneManager.
/// @param Ev the Mezzanine::Graphics::SceneManager to be reset.
/// @return This returns the XML::Node that was passed in.
Mezzanine::XML::Node& MEZZ_LIB operator >> (const Mezzanine::XML::Node& OneNode, Mezzanine::Graphics::SceneManager& Ev);


#endif
