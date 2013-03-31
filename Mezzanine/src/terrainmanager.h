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

#ifndef _terrainmanager_h
#define _terrainmanager_h

#include "datatypes.h"
#include "vector3.h"
#include "managerbase.h"
#include "managerfactory.h"
#include "singleton.h"

namespace Mezzanine
{
    class MeshTerrain;
    class TerrainBase;

    // Used by the scripting language binder to help create bindgings for this class. SWIG does know to creation template instances
    #ifdef SWIG
    %template(SingletonTerrainManager) Mezzanine::Singleton<TerrainManager>;
    #endif

    ///////////////////////////////////////////////////////////////////////////////
    /// @class TerrainManager
    /// @headerfile terrainmanager.h
    /// @brief This is manager for terrains and their functions.
    /// @details
    ///////////////////////////////////////
    class MEZZ_LIB TerrainManager : public ManagerBase, public Singleton<TerrainManager>
    {
        protected:
            /// @brief A container of all terrain instances.
            std::vector<TerrainBase*> Terrains;
            /// @brief A container of created MeshTerrain objects.
            std::vector<MeshTerrain*> MeshTerrains;
        public:
            /// @brief Class constructor.
            TerrainManager();

            /// @brief XML constructor.
            /// @param XMLNode The node of the xml document to construct from.
            TerrainManager(XML::Node& XMLNode);

            /// @brief Class destructor.
            virtual ~TerrainManager();

            ///////////////////////////////////////////////////////////////////////////////
            // Managing all terrains

            /// @brief Retrieves a MeshTerrain from the list of terrains.
            /// @param index Index of desired terrain in MeshTerrains.
            /// @return Returns a pointer to the MeshTerrain at the given index.
            virtual TerrainBase* GetTerrainByIndex(const Whole& Index);
            /// @brief Retrieves a Meshterrain from the list of terrains.
            /// @param name The name of the terrain.
            /// @return Returns a pointer to the named MeshTerrain, or NULL if no MeshTerrain exists with given name.
            virtual TerrainBase* GetTerrainByName(const String& Name);
            /// @brief Gets the number of terrains being stored in this manager.
            /// @return Returns a whole representing the number of terrains in this manager.
            virtual Whole GetNumTerrains() const;
            /// @brief Adds a pre-made terrain to the world and the manager.
            /// @param Terrain The terrain to be added.
            virtual void AddTerrain(TerrainBase* Terrain);
            /// @brief Removes a terrain from the world and this manager by index.
            /// @param Index The index at which to remove the terrain.
            virtual void RemoveTerrain(const Whole& Index);
            /// @brief Removes a terrain from the world and this manager.
            /// @param ToBeRemoved The terrain to be removed.
            virtual void RemoveTerrain(TerrainBase* ToBeRemoved);
            /// @brief Removes all terrains currently in this manager from the world and the manager.
            virtual void RemoveAllTerrains();
            /// @brief Destroys a terrain and removes it from world.
            /// @param Index Index of desired terrain in MeshTerrains.
            virtual void DestroyTerrain(const Whole& Index);
            /// @brief Destroys a terrain and removes it from world.
            /// @param Name name of desired terrain in MeshTerrains.
            virtual void DestroyTerrain(TerrainBase* ToBeDestroyed);
            /// @brief Removes and deletes all terrains currently in this manager from the world and the manager.
            virtual void DestroyAllTerrains();

            ///////////////////////////////////////////////////////////////////////////////
            // MeshTerrain Management

            /// @brief Creates a terrain based on a given mesh.
            /// @details This method creates a terrain object and handles adding it to the world.
            /// @param InitPosition The location of the terrain.
            /// @param name The name of the terrain.
            /// @param file The 3d mesh file that contains the 3d model the actor will use.
            /// @param group The resource group where the 3d mesh and other related files can be found.
            /// @return Returns a pointer to the created MeshTerrain object.
            virtual MeshTerrain* CreateMeshTerrain(const Vector3& InitPosition, const String& name, const String& file, const String& group);

            ///////////////////////////////////////////////////////////////////////////////
            // HeightfieldTerrain Management

            ///////////////////////////////////////////////////////////////////////////////
            // VectorfieldTerrain Management

            ///////////////////////////////////////////////////////////////////////////////
            // VoxelTerrain Management

            ///////////////////////////////////////////////////////////////////////////////
            // Inherited from Managerbase

            /// @copydoc Mezzanine::ManagerBase::Initialize()
            virtual void Initialize();
            /// @copydoc Mezzanine::ManagerBase::DoMainLoopItems()
            virtual void DoMainLoopItems();
            /// @copydoc ManagerBase::GetInterfaceType()
            virtual ManagerType GetInterfaceType() const;
            /// @copydoc ManagerBase::GetImplementationTypeName()
            virtual String GetImplementationTypeName() const;
    };//TerrainManager

    ///////////////////////////////////////////////////////////////////////////////
    /// @class DefaultTerrainManagerFactory
    /// @headerfile terrainmanager.h
    /// @brief A factory responsible for the creation and destruction of the default terrainmanager.
    ///////////////////////////////////////
    class MEZZ_LIB DefaultTerrainManagerFactory : public ManagerFactory
    {
        public:
            /// @brief Class constructor.
            DefaultTerrainManagerFactory();
            /// @brief Class destructor.
            virtual ~DefaultTerrainManagerFactory();

            /// @copydoc ManagerFactory::GetManagerTypeName()
            String GetManagerTypeName() const;
            /// @copydoc ManagerFactory::CreateManager(NameValuePairList&)
            ManagerBase* CreateManager(NameValuePairList& Params);

            /// @copydoc ManagerFactory::CreateManager(XML::Node&)
            ManagerBase* CreateManager(XML::Node& XMLNode);

            /// @copydoc ManagerFactory::DestroyManager(ManagerBase*)
            void DestroyManager(ManagerBase* ToBeDestroyed);
    };//DefaultTerrainManagerFactory
}//Mezzanine

#endif // _terrainmanager_h
