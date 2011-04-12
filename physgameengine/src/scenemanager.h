//� Copyright 2010 - 2011 BlackTopp Studios Inc.
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
#ifndef _scenemanager_h
#define _scenemanager_h
#include "crossplatformexport.h"

#include "colourvalue.h"
#include "managerbase.h"
#include "quaternion.h"
#include "vector3.h"
#include "xml.h"

namespace Ogre
{
    class SceneManager;
}

namespace phys
{
    class World;
    class Light;
    class Plane;
    class ParticleEffect;
    class WorldNode;
    ///////////////////////////////////////////////////////////////////////////////
    /// @class SceneManager
    /// @headerfile scenemanager.h
    /// @brief This class contains utilities and functions to allow the manipulation of the Graphical
    /// scene, rather then the physics inside, or the object inside.
    /// @details This class contains functions that allow the manipulation of lighting, skyboxes, internal
    /// scenemanager types, and more.
    ///////////////////////////////////////
    class PHYS_LIB SceneManager : public ManagerBase
    {
        public:
            /// @brief Needs to be documented.
            enum SceneManagerType
            {
                Generic,            ///< Documatation Required
                Exterior,           ///< Documatation Required
                ExteriorRealFar,    ///< Documatation Required
                Interior            ///< Documatation Required
            };
            enum SceneShadowTechnique
            {                                     //Shadow Docs from Ogre ShadowTechnique Documentation.
                SST_None,                         ///< No shadows.
                SST_Stencil_Modulative,           ///< Stencil shadow technique which renders all shadow volumes as a modulation after all the non-transparent areas have been rendered.
                SST_Stencil_Additive,             ///< Stencil shadow technique which renders each light as a separate additive pass to the scene.
                SST_Texture_Modulative,           ///< Texture-based shadow technique which involves a monochrome render-to-texture of the shadow caster and a projection of that texture onto the shadow receivers as a modulative pass.
                SST_Texture_Additive,             ///< Texture-based shadow technique which involves a render-to-texture of the shadow caster and a projection of that texture onto the shadow receivers, built up per light as additive passes.
                SST_Texture_Additive_Integrated,  ///< Texture-based shadow technique which involves a render-to-texture of the shadow caster and a projection of that texture on to the shadow receivers, with the usage of those shadow textures completely controlled by the materials of the receivers.
                SST_Texture_Modulative_Integrated ///< Texture-based shadow technique which involves a render-to-texture of the shadow caster and a projection of that texture on to the shadow receivers, with the usage of those shadow textures completely controlled by the materials of the receivers.
            };
        protected:
            /// @brief Pointer for the Ogre Scenemanager, where this manager gets it's functionality.
            Ogre::SceneManager* OgreManager;
            /// @brief Vector storing all the nodes in use by this class.
            std::vector< WorldNode* > WorldNodes;
            /// @brief Vector storing all the lights in use by this class.
            std::vector< Light* > Lights;
            /// @brief Vector storing all the particle effects in use by this class.
            std::vector< ParticleEffect* > Particles;
        public:
            /// @brief Class Constructor.
            /// @details Standard class initialization constructor.
            /// @param ManagerType Type of Scene Manager to be created.
            SceneManager(SceneManager::SceneManagerType ManagerType);
            /// @brief Class Destructor.
            /// @details The class destructor.
            ~SceneManager();

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
            /// @brief Sets the maximum distance from the camera that shadows will be visible.
            /// @param FarDist The maximum distance from the camera shadows will be rendered.
            void SetShadowFarDistance(const Real& FarDist);
            /// @brief Gets the maximum distance from the camera that shadows will be visible.
            /// @return Returns a Real representing the maximum distance from the camera shadows will be rendered.
            Real GetShadowFarDistance();
            /// @brief Sets the colour to be used when casting shadows.
            /// @param ShadowColour The colour desired to be used when rendering shadows.
            void SetShadowColour(const ColourValue& ShadowColour);
            /// @brief Gets the colour being used when casting shadows.
            /// @return Returns a ColourValue representing the colour used when casting shadows.
            ColourValue GetShadowColour();

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
            void CreateSkyPlane(Plane& SkyPlane, String& Material, String& Group, Real Scale=1000.0, Real Tiling=10.0,
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
            void CreateSkyBox(String& Material, String& Group, Real Distance, bool DrawFirst=true, Quaternion Orientation=Quaternion());
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
            void CreateSkyDome(String& Material, String& Group, Real Distance, Real Curvature=10.0, Real Tiling=8.0, bool DrawFirst=true,
                               Quaternion Orientation=Quaternion(), int XSegments=16, int YSegments=16);
            /// @brief Disables the currently active skydome.
            /// @details Using this function effectively deletes the skydome, so you will have to provide a new set of parameters
            /// if you wish to re-create the skydome.
            void DisableSkyDome();

            /// @brief Sets the ambient light for the scene.
            /// @details Not all scene's will need ambient light.  Ambient light is light that hits all objects from
            /// all directions.
            /// @param Red The value representing the amount of red color in the ambient light.
            /// @param Green The value representing the amount of green color in the ambient light.
            /// @param Blue The value representing the amount of blue color in the ambient light.
            /// @param Alpha The value representing the transparency of the color in the ambient light.
            void SetAmbientLight(Real Red=1.0, Real Green=1.0, Real Blue=1.0, Real Alpha=1.0);

            /// @todo Add GetAmbientLight()

            /// @brief Creates a dynamic light.
            /// @param Name The name to be given to this light.
            /// @return Returns a pointer to the light class which was created by this function.
            Light* CreateLight(const String& Name);
            /// @brief Gets an already created light by name.
            /// @return Returns a pointer to the light of the specified name.
            Light* GetLight(const String& Name) const;
            /// @brief Gets an already created light by index.
            /// @return Returns a pointer to the light at the specified index.
            Light* GetLight(Whole Index) const;
            /// @brief Gets the number of lights created and stored in this manager.
            /// @return Returns the number of lights this manager is storing.
            Whole GetNumLights() const;
            /// @brief Deletes a light and removes all trace of it from the manager.
            /// @param light The light to be destroyed.
            void DestroyLight(Light* light);

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
            ParticleEffect* GetParticleEffect(Whole Index) const;
            /// @brief Gets the number of particle effects created and stored in this manager.
            /// @return Returns the number of particle effects this manager is storing.
            Whole GetNumParticleEffects() const;
            /// @brief Deletes a particle effect and removes all trace of it from the manager.
            /// @param particleeffect The particle effect to be destroyed.
            void DestroyParticleEffect(ParticleEffect* particleeffect);

            /// @brief Creates a node that will orbit around a point.
            /// @details This will create 2 nodes in the scene, the first being the point in the world you want to orbit
            /// the second node around.  The second being the node that does the orbiting.  You can then attach a light,
            /// particle effect, or ribbon trail to the orbiting node .
            /// @param Target The location of the first node which you will be orbiting around.
            /// @param RelativeLoc The location of the node that will be in orbit relative to the first node.  Assume the
            /// first node is at Origin (0,0,0).
            WorldNode* CreateOrbitingNode(const String& Name, Vector3 Target, Vector3 RelativeLoc, bool AutoTrack);
            /// @brief Creates a stationary node that will look at a location.
            /// @details This will create a node that doesn't move, and will look at one location that you specify.  This
            /// node can then have lights, particle effects, or ribbon trails attached to it.
            /// @param LookAt The location you want the node to look at.  Automatically handles orientation.
            /// @param Location The location of the node itself.
            WorldNode* CreateStandNode(const String& Name, Vector3 LookAt, Vector3 Location);
            /// @brief Creates a freely moveable node that will look at a location.
            /// @details This will create a node that can be freely moved. When created it will look at one location that you specify.  This
            /// node can then have lights, particle effects or other attachables attached to it.
            /// @param LookAt The location you want the node to look at.  Automatically handles orientation.
            /// @param Location The location of the node itself.
            WorldNode* CreateFreeNode(const String& Name, Vector3 LookAt, Vector3 Location);

            /// @brief Gets an already created node by name.
            /// @return Returns a pointer to the node of the specified name, or 0 if no matching WorldNode could be Found.
            WorldNode* GetNode(const String& Name) const;
            /// @brief Gets an already created node by index.
            /// @return Returns a pointer to the node at the specified index.
            WorldNode* GetNode(Whole Index) const;
            /// @brief Gets the number of nodes created and stored in this manager.
            /// @return Returns the number of nodes this manager is storing.
            Whole GetNumNodes() const;
            /// @brief Gets the number of stand type nodes created and stored in this manager.
            /// @return Returns the number of stand type nodes this manager is storing.
            Whole GetNumStandNodes() const;
            /// @brief Gets the number of orbit type nodes created and stored in this manager.
            /// @return Returns the number of orbit type nodes this manager is storing.
            Whole GetNumOrbitNodes() const;
            /// @brief Deletes a node and removes all trace of it from the manager.
            /// @param node The node to be destroyed.
            void DestroyNode(WorldNode* node);

            /// @brief Gets the name of this manager.
            /// @return Returns the name of this manager.
            ConstString& GetName() const;
            /// @brief Inherited from ManagerBase.
            void Initialize();
            /// @brief Inherited from ManagerBase.
            void DoMainLoopItems();
            /// @brief Gets the type of manager that this manager is.
            /// @return Returns an enum value representing the type of manager that this manager is.
            ManagerBase::ManagerTypeName GetType() const;
            /// @internal
            /// @brief Gets the internal Ogre Scene Manager pointer.
            /// @return Returns a pointer to the ogre Scene Manager.
            Ogre::SceneManager* GetGraphicsWorldPointer() const;
    };//scenemanager
}//phys

///////////////////////////////////////////////////////////////////////////////
// Class External << Operators for streaming or assignment
#ifdef PHYSXML

/// @brief Serializes the passed phys::SceneManager to XML
/// @param stream The ostream to send the xml to.
/// @param Ev the phys::SceneManager to be serialized
/// @return this returns the ostream, now with the serialized data
std::ostream& PHYS_LIB operator << (std::ostream& stream, const phys::SceneManager& Ev);

/// @brief Deserialize a phys::SceneManager
/// @param stream The istream to get the xml from to (re)make the phys::SceneManager.
/// @param Ev the phys::SceneManager to be deserialized.
/// @return this returns the ostream, advanced past the phys::SceneManager that was recreated onto Ev.
std::istream& PHYS_LIB operator >> (std::istream& stream, phys::SceneManager& Ev);

/// @brief Set all values of a phys::SceneManager from parsed xml.
/// @param OneNode The istream to get the xml from to (re)make the phys::SceneManager.
/// @param Ev the phys::SceneManager to be reset.
/// @return This returns thexml::Node that was passed in.
phys::xml::Node& PHYS_LIB operator >> (const phys::xml::Node& OneNode, phys::SceneManager& Ev);

#endif // \PHYSXML


#endif
