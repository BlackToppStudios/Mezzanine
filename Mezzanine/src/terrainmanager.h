// © Copyright 2010 - 2016 BlackTopp Studios Inc.
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
#ifndef terrainmanager_h
#define terrainmanager_h

#include "worldmanager.h"
#include "worldmanagerfactory.h"
#ifndef SWIG
    #include "Threading/workunit.h"
#endif

namespace Mezzanine
{
    class Terrain;
    class TerrainFactory;
    class TerrainManager;

    ///////////////////////////////////////////////////////////////////////////////
    /// @brief This is a Mezzanine::Threading::iWorkUnit for the updating of terrains.
    ///////////////////////////////////////
    class MEZZ_LIB TerrainUpdateWorkUnit : public Threading::DefaultWorkUnit
    {
    protected:
        /// @internal
        /// @brief A pointer to the manager this work unit is processing.
        TerrainManager* TargetManager;
        /// @internal
        /// @brief Protected copy constructor.  THIS IS NOT ALLOWED.
        /// @param Other The other work unit being copied from.  WHICH WILL NEVER HAPPEN.
        TerrainUpdateWorkUnit(const TerrainUpdateWorkUnit& Other);
        /// @internal
        /// @brief Protected assignment operator.  THIS IS NOT ALLOWED.
        /// @param Other The other work unit being copied from.  WHICH WILL NEVER HAPPEN.
        TerrainUpdateWorkUnit& operator=(const TerrainUpdateWorkUnit& Other);
    public:
        /// @brief Class constructor.
        /// @param Target The TerrainManager this work unit will process during the frame.
        TerrainUpdateWorkUnit(TerrainManager* Target);
        /// @brief Class destructor.
        virtual ~TerrainUpdateWorkUnit();

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        /// @brief This does any required update of the Terrains stored by it's manager.
        /// @param CurrentThreadStorage The storage class for all resources owned by this work unit during it's execution.
        virtual void DoWork(Threading::DefaultThreadSpecificStorage::Type& CurrentThreadStorage);
    };//TerrainUpdateWorkUnit

    ///////////////////////////////////////////////////////////////////////////////
    /// @brief A manager responsible for the storage and management of all terrains that exist in a world.
    /// @details More or less Management point for a container of terrains to help keep them sorted.
    ///////////////////////////////////////
    class MEZZ_LIB TerrainManager : public WorldManager
    {
    public:
        /// @brief Basic container type for TerrainFactory storage by this class.
        typedef std::map<String,TerrainFactory*>       FactoryContainer;
        /// @brief Iterator type for TerrainFactory instances stored by this class.
        typedef FactoryContainer::iterator             FactoryIterator;
        /// @brief Const Iterator type for TerrainFactory instances stored by this class.
        typedef FactoryContainer::const_iterator       ConstFactoryIterator;
        /// @brief Basic container type for @ref Terrain storage by this class.
        typedef std::vector<Terrain*>                  TerrainContainer;
        /// @brief Iterator type for @ref Terrain instances stored by this class.
        typedef TerrainContainer::iterator             TerrainIterator;
        /// @brief Const Iterator type for @ref Terrain instances stored by this class.
        typedef TerrainContainer::const_iterator       ConstTerrainIterator;

        /// @brief A String containing the name of this manager implementation.
        static const String ImplementationName;
        /// @brief A ManagerType enum value used to describe the type of interface/functionality this manager provides.
        static const ManagerBase::ManagerType InterfaceType;
    protected:
        friend class TerrainUpdateWorkUnit;

        /// @internal
        /// @brief A map containing all registered Terrain type factories.
        static FactoryContainer TerrainFactories;
        /// @internal
        /// @brief Container storing all Terrains belonging to this manager.
        TerrainContainer Terrains;

        /// @internal
        /// @brief The work unit that updates all the terrains stored by this manager.
        TerrainUpdateWorkUnit* TerrainUpdateWork;
        /// @internal
        /// @brief Can be used for thread safe logging and other thread specific resources.
        Threading::DefaultThreadSpecificStorage::Type* ThreadResources;
    public:
        /// @brief Class constructor.
        /// @param Creator The parent world that is creating the manager.
        TerrainManager(World* Creator);
        /// @brief XML constructor.
        /// @param Creator The parent world that is creating the manager.
        /// @param XMLNode The node of the xml document to construct from.
        TerrainManager(World* Creator, const XML::Node& XMLNode);
        /// @brief Class destructor.
        virtual ~TerrainManager();

        ///////////////////////////////////////////////////////////////////////////////
        // Prefab Terrain Type Creation

        ///////////////////////////////////////////////////////////////////////////////
        // Terrain Management

        /// @brief Creates a new Terrain.
        /// @param TypeName A string containing the name of the type of Terrain to be constructed.
        /// @param InstanceName A string containing the name to be given to the created Terrain.
        /// @param Params A container of additional parameters to be used for the construction of the new Terrain.
        /// @return Returns a pointer to the created Terrain.
        Terrain* CreateTerrain(const String& TypeName, const String& InstanceName, const NameValuePairMap& Params);
        /// @brief Creates a new Terrain class from an XML node.
        /// @remarks This is mostly useful for deserialization.
        /// @return Returns a pointer to the created Terrain.
        Terrain* CreateTerrain(const XML::Node& SelfRoot);

        /// @brief Gets an Terrain by Index.
        /// @param Index The index of the Terrain you wish to retrieve.
        /// @return Returns a pointer to the Terrain at the specified index.
        virtual Terrain* GetTerrain(const Whole Index) const;
        /// @brief Gets an Terrain by Name.
        /// @param Name The name of the Terrain you wish to retrieve.
        /// @return Returns a pointer to the Terrain of the specified name.
        virtual Terrain* GetTerrain(const String& Name) const;
        /// @brief Gets the number of Terrains stored in this manager.
        /// @return Returns a whole representing the current Terrain count.
        virtual Whole GetNumTerrains() const;
        /// @brief Destroys an Terrain at the specified index.
        /// @param Index The index at which to destroy the Terrain.
        virtual void DestroyTerrain(const Whole Index);
        /// @brief Destroys an Terrain.
        /// @param ToBeDestroyed The Terrain to be destroyed.
        virtual void DestroyTerrain(Terrain* ToBeDestroyed);
        /// @brief Destroys all Terrains currently within this manager.
        virtual void DestroyAllTerrains();

        #ifndef SWIG
        /// @brief Gets an iterator to the first Terrain in this manager.
        /// @return Returns an iterator to the first Terrain being stored by this manager.
        TerrainIterator BeginTerrain();
        /// @brief Gets an iterator to one passed the last Terrain in this manager.
        /// @return Returns an iterator to one passed the last Terrain being stored by this manager.
        TerrainIterator EndTerrain();
        /// @brief Gets a const iterator to the first Terrain in this manager.
        /// @return Returns a const iterator to the first Terrain being stored by this manager.
        ConstTerrainIterator BeginTerrain() const;
        /// @brief Gets a const iterator to one passed the last Terrain in this manager.
        /// @return Returns a const iterator to one passed the last Terrain being stored by this manager.
        ConstTerrainIterator EndTerrain() const;
        #endif

        ///////////////////////////////////////////////////////////////////////////////
        // TerrainFactory Management

        /// @brief Adds/registers a Terrain factory with this manager, allowing it to be constructed through this API.
        /// @param ToBeAdded The Terrain factory to be added.
        static void AddTerrainFactory(TerrainFactory* ToBeAdded);
        /// @brief Removes a Terrain factory from this manager.
        /// @param ToBeRemoved A pointer to the Terrain factory that is to be removed.
        static void RemoveTerrainFactory(TerrainFactory* ToBeRemoved);
        /// @brief Removes a Terrain factory from this manager.
        /// @param ImplName The name of the Terrain implementation created by the factory to be removed.
        static void RemoveTerrainFactory(const String& ImplName);
        /// @brief Removes and destroys a Terrain factory in this manager.
        /// @param ToBeDestroyed A pointer to the Terrain factory that is to be removed and destroyed.
        static void DestroyTerrainFactory(TerrainFactory* ToBeDestroyed);
        /// @brief Removes and destroys a Terrain factory in this manager.
        /// @param ImplName The name of the Terrain implementation created by the factory to be removed and destroyed.
        static void DestroyTerrainFactory(const String& ImplName);
        /// @brief Destroys all Terrain factories in this manager.
        /// @warning The destruction of Terrain factories should only be done after all the Terrain have been destroyed, otherwise this will cause an exception.
        static void DestroyAllTerrainFactories();

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        /// @copydoc WorldManager::Pause(const UInt32)
        virtual void Pause(const UInt32 PL);

        /// @copydoc WorldManager::Initialize()
        virtual void Initialize();
        /// @copydoc ManagerBase::Deinitialize()
        virtual void Deinitialize();

        /// @brief Gets the work unit responsible for updating terrains stored by this manager.
        /// @return Returns a pointer to the TerrainUpdateWorkUnit used by this manager.
        TerrainUpdateWorkUnit* GetTerrainUpdateWork();

        ///////////////////////////////////////////////////////////////////////////////
        // Type Identifier Methods

        /// @copydoc ManagerBase::GetInterfaceType()
        virtual ManagerType GetInterfaceType() const;
        /// @copydoc ManagerBase::GetImplementationTypeName()
        virtual String GetImplementationTypeName() const;
    };//TerrainManager

    ///////////////////////////////////////////////////////////////////////////////
    /// @brief A factory responsible for the creation and destruction of the default TerrainManager.
    ///////////////////////////////////////
    class DefaultTerrainManagerFactory : public WorldManagerFactory
    {
    public:
        /// @brief Class constructor.
        DefaultTerrainManagerFactory();
        /// @brief Class destructor.
        virtual ~DefaultTerrainManagerFactory();

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
    };//DefaultTerrainManagerFactory
}//Mezzanine

#endif
