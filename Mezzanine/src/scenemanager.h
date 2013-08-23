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
#ifndef _scenemanager_h
#define _scenemanager_h

#include "colourvalue.h"
#include "worldmanager.h"
#include "managerfactory.h"
#include "singleton.h"
#include "quaternion.h"
#include "vector3.h"
#include "XML/xml.h"
#include "Threading/workunit.h"

namespace Ogre
{
    class SceneManager;
}

namespace Mezzanine
{
    class Entresol;
    class Light;
    class Plane;
    class ParticleEffect;
    class Entity;
    class WorldNode;
    class SceneManager;

    namespace Internal
    {
        class SceneManagerData;
    }

    ///////////////////////////////////////////////////////////////////////////////
    /// @brief This is a Mezzanine::Threading::iWorkUnit for the updating of tracking world nodes in the world.
    /// @details
    ///////////////////////////////////////
    class MEZZ_LIB TrackingNodeUpdateWorkUnit : public Threading::DefaultWorkUnit
    {
    protected:
        /// @internal
        /// @brief A pointer to the manager this work unit is processing.
        SceneManager* TargetManager;
        /// @internal
        /// @brief Protected copy constructor.  THIS IS NOT ALLOWED.
        /// @param Other The other work unit being copied from.  WHICH WILL NEVER HAPPEN.
        TrackingNodeUpdateWorkUnit(const TrackingNodeUpdateWorkUnit& Other);
        /// @internal
        /// @brief Protected assignment operator.  THIS IS NOT ALLOWED.
        /// @param Other The other work unit being copied from.  WHICH WILL NEVER HAPPEN.
        TrackingNodeUpdateWorkUnit& operator=(const TrackingNodeUpdateWorkUnit& Other);
    public:
        /// @brief Class constructor.
        /// @param Target The SceneManager this work unit will process during the frame.
        TrackingNodeUpdateWorkUnit(SceneManager* Target);
        /// @brief Class destructor.
        virtual ~TrackingNodeUpdateWorkUnit();

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        /// @brief This does any required update of the Tracking nodes in the world.
        /// @param CurrentThreadStorage The storage class for all resources owned by this work unit during it's execution.
        virtual void DoWork(Threading::DefaultThreadSpecificStorage::Type& CurrentThreadStorage);
    };//TrackingNodeUpdateWorkUnit

    ///////////////////////////////////////////////////////////////////////////////
    /// @class SceneManager
    /// @headerfile scenemanager.h
    /// @brief This class contains utilities and functions to allow the manipulation of the Graphical
    /// scene, rather then the physics inside, or the object inside.
    /// @details This class contains functions that allow the manipulation of lighting, skyboxes, internal
    /// scenemanager types, and more.
    ///////////////////////////////////////
    class MEZZ_LIB SceneManager : public WorldManager
    {
    public:
        /// @brief Basic container type for @ref WorldNode storage by this class.
        typedef std::vector< WorldNode* >                     WorldNodeContainer;
        /// @brief Iterator type for @ref WorldNode instances stored by this class.
        typedef WorldNodeContainer::iterator                  WorldNodeIterator;
        /// @brief Const Iterator type for @ref WorldNode instances stored by this class.
        typedef WorldNodeContainer::const_iterator            ConstWorldNodeIterator;
        /// @brief Basic container type for @ref Light storage by this class.
        typedef std::vector< Light* >                         LightContainer;
        /// @brief Iterator type for @ref Light instances stored by this class.
        typedef LightContainer::iterator                      LightIterator;
        /// @brief Const Iterator type for @ref Light instances stored by this class.
        typedef LightContainer::const_iterator                ConstLightIterator;
        /// @brief Basic container type for @ref ParticleEffect storage by this class.
        typedef std::vector< ParticleEffect* >                ParticleContainer;
        /// @brief Iterator type for @ref ParticleEffect instances stored by this class.
        typedef ParticleContainer::iterator                   ParticleIterator;
        /// @brief Const Iterator type for @ref ParticleEffect instances stored by this class.
        typedef ParticleContainer::const_iterator             ConstParticleIterator;
        /// @brief Basic container type for @ref Entity storage by this class.
        typedef std::vector< Entity* >                        EntityContainer;
        /// @brief Iterator type for @ref Entity instances stored by this class.
        typedef EntityContainer::iterator                     EntityIterator;
        /// @brief Const Iterator type for @ref Entity instances stored by this class.
        typedef EntityContainer::const_iterator               ConstEntityIterator;

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
        friend class TrackingNodeUpdateWorkUnit;

        /// @internal
        /// @brief Vector storing all the nodes in use by this class.
        WorldNodeContainer WorldNodes;
        /// @internal
        /// @brief Vector storing all the lights in use by this class.
        LightContainer Lights;
        /// @internal
        /// @brief Vector storing all the particle effects in use by this class.
        ParticleContainer Particles;
        /// @internal
        /// @brief Vector storing all the entities in use by this class.
        EntityContainer Entities;

        /// @internal
        /// @brief Container of nodes currently tracking other objects.
        std::set< WorldNode* > TrackingNodes;

        /// @internal
        /// @brief Pointer to a class storing sensative internal data for the scene.
        Internal::SceneManagerData* SMD;

        /// @internal
        /// @brief The work unit that updates the tracking nodes so they are directed at their targets.
        TrackingNodeUpdateWorkUnit* TrackingNodeUpdateWork;
        /// @internal
        /// @brief Can be used for thread safe logging and other thread specific resources.
        Threading::DefaultThreadSpecificStorage::Type* ThreadResources;

        /// @brief Updates all nodes tracking other objects.
        void UpdateTrackingNodes();
    public:
        /// @brief Class Constructor.
        /// @details Standard class initialization constructor.
        /// @param InternalManagerTypeName The name of the scenemanager type to be constructed.
        SceneManager(const String& InternalManagerTypeName = "DefaultSceneManager");
        /// @brief XML constructor.
        /// @param XMLNode The node of the xml document to construct from.
        SceneManager(XML::Node& XMLNode);
        /// @brief Class Destructor.
        /// @details The class destructor.
        ~SceneManager();

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
                            bool DrawFirst=true, Real Bow=0, int XSegments=1, int YSegments=1);
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
        void CreateSkyBox(const String& Material, const String& Group, Real Distance, bool DrawFirst=true, Quaternion Orientation=Quaternion());
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
        void CreateSkyDome(const String& Material, const String& Group, Real Distance, Real Curvature=10.0, Real Tiling=8.0, bool DrawFirst=true,
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

        /// @brief Creates a dynamic light.
        /// @param Name The name to be given to this light.
        /// @return Returns a pointer to the light class which was created by this function.
        Light* CreateLight(const String& Name);
        /// @brief Gets an already created light by name.
        /// @return Returns a pointer to the light of the specified name.
        Light* GetLight(const String& Name) const;
        /// @brief Gets an already created light by index.
        /// @return Returns a pointer to the light at the specified index.
        Light* GetLight(const Whole& Index) const;
        /// @brief Gets the number of lights created and stored in this manager.
        /// @return Returns the number of lights this manager is storing.
        Whole GetNumLights() const;
        /// @brief Deletes a light and removes all trace of it from the manager.
        /// @param ToBeDestroyed The light to be destroyed.
        void DestroyLight(Light* ToBeDestroyed);
        /// @brief Destroys all lights currently in the manager.
        void DestroyAllLights();

        /// @brief Get a LightIterator to the first Light*
        /// @return A LightIterator to the first Light*
        LightIterator BeginLight();
        /// @brief Get a LightIterator to one past the last Light*
        /// @return A LightIterator to one past the last Light*
        LightIterator EndLight();
        /// @brief Get a ConstLightIterator to the first Light*
        /// @return A ConstLightIterator to the first Light*
        ConstLightIterator BeginLight() const;
        /// @brief Get a ConstLightIterator to one past the last Light*
        /// @return A ConstLightIterator to one past the last Light*
        ConstLightIterator EndLight() const;

        ///////////////////////////////////////////////////////////////////////////////
        // Particle Effect Management

        /// @brief Creates a particle effect.
        /// @details Particle effects are useful when trying to create visual effects for rain, smoke, explosions, fireworks, etc..
        /// @param Name The name to be given to this particle effect.
        /// @param Template The particle script (from a .particle file) to base this particle effect on.
        /// @return Returns a pointer to the particle effect class which was created by this function.
        ParticleEffect* CreateParticleEffect(const String& Name, const String& Template);
        /// @brief Gets an already created particle effect by name.
        /// @return Returns a pointer to the particle effect of the specified name.
        ParticleEffect* GetParticleEffect(const String& Name) const;
        /// @brief Gets an already created particle effect by index.
        /// @return Returns a pointer to the particle effect at the specified index.
        ParticleEffect* GetParticleEffect(const Whole& Index) const;
        /// @brief Gets the number of particle effects created and stored in this manager.
        /// @return Returns the number of particle effects this manager is storing.
        Whole GetNumParticleEffects() const;
        /// @brief Deletes a particle effect and removes all trace of it from the manager.
        /// @param ToBeDestroyed The particle effect to be destroyed.
        void DestroyParticleEffect(ParticleEffect* ToBeDestroyed);
        /// @brief Destroys all particle effects currently in the manager.
        void DestroyAllParticleEffects();

        /// @brief Pauses(or unpauses) all particles stored in this manager.
        /// @param Pause Will pause all Particles if true, unpause if false.
        void PauseAllParticles(bool Pause);

        /// @brief Get a ParticleEffectIterator to the first ParticleEffect*
        /// @return A ParticleEffectIterator to the first ParticleEffect*
        ParticleIterator BeginParticleEffect();
        /// @brief Get a ParticleEffectIterator to one past the last ParticleEffect*
        /// @return A ParticleEffectIterator to one past the last ParticleEffect*
        ParticleIterator EndParticleEffect();
        /// @brief Get a ConstParticleEffectIterator to the first ParticleEffect*
        /// @return A ConstParticleEffectIterator to the first ParticleEffect*
        ConstParticleIterator BeginParticleEffect() const;
        /// @brief Get a ConstParticleEffectIterator to one past the last ParticleEffect*
        /// @return A ConstParticleEffectIterator to one past the last ParticleEffect*
        ConstParticleIterator EndParticleEffect() const;

        ///////////////////////////////////////////////////////////////////////////////
        // Entity Management

        /// @brief Creates an entity.
        /// @details Entities are objects that have zero physical representation, they are essentially meshes in world space.
        /// @param EntName The name to be given to this entity.
        /// @param MeshName The name of the mesh to construct this entity from.
        /// @param Group The resource group to find the mesh.
        /// @return Returns a pointer to the entity class which was created by this function.
        Entity* CreateEntity(const String& EntName, const String& MeshName, const String& Group);
        /// @brief Gets an already created entity by name.
        /// @return Returns a pointer to the entity of the specified name.
        Entity* GetEntity(const String& Name) const;
        /// @brief Gets an already created entity by index.
        /// @return Returns a pointer to the entity at the specified index.
        Entity* GetEntity(const Whole& Index) const;
        /// @brief Gets the number of entities created and stored in this manager.
        /// @return Returns the number of entities this manager is storing.
        Whole GetNumEntities() const;
        /// @brief Deletes an entity and removes all trace of it from the manager.
        /// @param ToBeDestroyed The entity to be destroyed.
        void DestroyEntity(Entity* ToBeDestroyed);
        /// @brief Destroys all entities currently in the manager.
        void DestroyAllEntities();

        /// @brief Get an EntityIterator to the first Entity*
        /// @return An EntityIterator to the first Entity*
        EntityIterator BeginEntity();
        /// @brief Get a EntityIterator to one past the last Entity*
        /// @return A EntityIterator to one past the last Entity*
        EntityIterator EndEntity();
        /// @brief Get a ConstEntityIterator to the first Entity*
        /// @return A ConstEntityIterator to the first Entity*
        ConstEntityIterator BeginEntity() const;
        /// @brief Get a ConstEntityIterator to one past the last Entity*
        /// @return A ConstEntityIterator to one past the last Entity*
        ConstEntityIterator EndEntity() const;

        ///////////////////////////////////////////////////////////////////////////////
        // WorldNode Management

        /// @brief Creates a world node that can be manipulated.
        /// @param Name The name to be given to the node.
        WorldNode* CreateWorldNode(const String& Name);
        /// @brief Gets an already created node by name.
        /// @return Returns a pointer to the node of the specified name, or 0 if no matching WorldNode could be Found.
        /// @details This runs in Linear time
        WorldNode* GetNode(const String& Name) const;
        /// @brief Gets an already created node by index.
        /// @return Returns a pointer to the node at the specified index.
        /// @details This runs in constant time.
        WorldNode* GetNode(const Whole& Index) const;
        /// @brief Gets the number of nodes created and stored in this manager.
        /// @return Returns the number of nodes this manager is storing.
        /// @details This runs in constant time, this data is cached constantly.
        Whole GetNumNodes() const;
        /// @brief Deletes a node and removes all trace of it from the manager.
        /// @param ToBeDestroyed The node to be destroyed.
        void DestroyNode(WorldNode* ToBeDestroyed);
        /// @brief Destroys all world nodes currently in the manager.
        void DestroyAllWorldNodes();

        /// @brief Get a WorldNodeIterator to the first WorldNode*
        /// @return A WorldNodeIterator to the first WorldNode*
        WorldNodeIterator BeginWorldNode();
        /// @brief Get a WorldNodeIterator to one past the last WorldNode*
        /// @return A WorldNodeIterator to one past the last WorldNode*
        WorldNodeIterator EndWorldNode();
        /// @brief Get a ConstWorldNodeIterator to the first WorldNode*
        /// @return A ConstWorldNodeIterator to the first WorldNode*
        ConstWorldNodeIterator BeginWorldNode() const;
        /// @brief Get a ConstWorldNodeIterator to one past the last WorldNode*
        /// @return A ConstWorldNodeIterator to one past the last WorldNode*
        ConstWorldNodeIterator EndWorldNode() const;

        /// @brief Informs this manager that a node needs periodic updates for tracking.
        /// @param Tracker The node that is getting tracking enabled.
        void _RegisterTrackingNode(WorldNode* Tracker);
        /// @brief Informs this manager a node is no longer tracking another object.
        /// @param Tracker The node that is getting tracking disabled.
        void _UnRegisterTrackingNode(WorldNode* Tracker);

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        /// @brief Gets the name of this manager.
        /// @return Returns the name of this manager.
        ConstString& GetName() const;

        /// @copydoc WorldManager::Pause(const UInt32)
        virtual void Pause(const UInt32 PL);

        /// @copydoc WorldManager::Initialize()
        virtual void Initialize();
        /// @copydoc ManagerBase::Deinitialize()
        virtual void Deinitialize();

        /// @brief Gets the work unit responsible for updating the tracking nodes in this manager.
        /// @return Returns a pointer to the TrackingNodeUpdateWorkUnit used by this manager.
        TrackingNodeUpdateWorkUnit* GetTrackingNodeUpdateWork();

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
        Ogre::SceneManager* GetGraphicsWorldPointer() const;
        /// @internal
        /// @brief Gets the raw internal internal data.
        /// @return Returns a to the raw internal data.
        Internal::SceneManagerData* GetRawInternalDataPointer() const;
    };//SceneManager

    ///////////////////////////////////////////////////////////////////////////////
    /// @class DefaultSceneManagerFactory
    /// @headerfile scenemanager.h
    /// @brief A factory responsible for the creation and destruction of the default scenemanager.
    ///////////////////////////////////////
    class MEZZ_LIB DefaultSceneManagerFactory : public ManagerFactory
    {
    public:
        /// @brief Class constructor.
        DefaultSceneManagerFactory();
        /// @brief Class destructor.
        virtual ~DefaultSceneManagerFactory();

        /// @copydoc ManagerFactory::GetManagerTypeName()
        String GetManagerTypeName() const;

        /// @copydoc ManagerFactory::CreateManager(NameValuePairList&)
        ManagerBase* CreateManager(NameValuePairList& Params);
        /// @copydoc ManagerFactory::CreateManager(XML::Node&)
        ManagerBase* CreateManager(XML::Node& XMLNode);
        /// @copydoc ManagerFactory::DestroyManager(ManagerBase*)
        void DestroyManager(ManagerBase* ToBeDestroyed);
    };//DefaultSceneManagerFactory
}//Mezzanine

///////////////////////////////////////////////////////////////////////////////
// Class External << Operators for streaming or assignment
/// @brief Serializes the passed Mezzanine::SceneManager to XML
/// @param stream The ostream to send the xml to.
/// @param Ev the Mezzanine::SceneManager to be serialized
/// @return this returns the ostream, now with the serialized data
std::ostream& MEZZ_LIB operator << (std::ostream& stream, const Mezzanine::SceneManager& Ev);

/// @brief Deserialize a Mezzanine::SceneManager
/// @param stream The istream to get the xml from to (re)make the Mezzanine::SceneManager.
/// @param Ev the Mezzanine::SceneManager to be deserialized.
/// @return this returns the ostream, advanced past the Mezzanine::SceneManager that was recreated onto Ev.
std::istream& MEZZ_LIB operator >> (std::istream& stream, Mezzanine::SceneManager& Ev);

/// @brief Set all values of a Mezzanine::SceneManager from parsed xml.
/// @param OneNode The istream to get the xml from to (re)make the Mezzanine::SceneManager.
/// @param Ev the Mezzanine::SceneManager to be reset.
/// @return This returns theXML::Node that was passed in.
Mezzanine::XML::Node& MEZZ_LIB operator >> (const Mezzanine::XML::Node& OneNode, Mezzanine::SceneManager& Ev);


#endif
