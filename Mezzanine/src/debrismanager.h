// © Copyright 2010 - 2014 BlackTopp Studios Inc.
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
#ifndef _debrismanager_h
#define _debrismanager_h

/// @file
/// @brief This file contains the declaration for the manager that manages debris objects in a world.

#include "worldmanager.h"
#include "managerfactory.h"
#ifndef SWIG
    #include "Threading/workunit.h"
#endif

namespace Mezzanine
{
    class Debris;
    class DebrisFactory;
    class DebrisManager;

    class RigidDebris;
    class SoftDebris;

    namespace Physics {
        class CollisionShape;
    }
    namespace Graphics {
        class Mesh;
    }
    ///////////////////////////////////////////////////////////////////////////////
    /// @brief This is a Mezzanine::Threading::iWorkUnit for the updating of Debris.
    /// @details
    ///////////////////////////////////////
    class MEZZ_LIB DebrisUpdateWorkUnit : public Threading::DefaultWorkUnit
    {
    protected:
        /// @internal
        /// @brief A pointer to the manager this work unit is processing.
        DebrisManager* TargetManager;
        /// @internal
        /// @brief Protected copy constructor.  THIS IS NOT ALLOWED.
        /// @param Other The other work unit being copied from.  WHICH WILL NEVER HAPPEN.
        DebrisUpdateWorkUnit(const DebrisUpdateWorkUnit& Other);
        /// @internal
        /// @brief Protected assignment operator.  THIS IS NOT ALLOWED.
        /// @param Other The other work unit being copied from.  WHICH WILL NEVER HAPPEN.
        DebrisUpdateWorkUnit& operator=(const DebrisUpdateWorkUnit& Other);
    public:
        /// @brief Class constructor.
        /// @param Target The DebrisManager this work unit will process during the frame.
        DebrisUpdateWorkUnit(DebrisManager* Target);
        /// @brief Class destructor.
        virtual ~DebrisUpdateWorkUnit();

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        /// @brief This does any required update of the Debris stored by it's manager.
        /// @param CurrentThreadStorage The storage class for all resources owned by this work unit during it's execution.
        virtual void DoWork(Threading::DefaultThreadSpecificStorage::Type& CurrentThreadStorage);
    };//DebrisUpdateWorkUnit

    ///////////////////////////////////////////////////////////////////////////////
    /// @brief A manager responsible for the storage and management of all Debris that exist in a world.
    /// @details More or less Management point for a container of Debris to help keep them sorted.
    ///////////////////////////////////////
    class MEZZ_LIB DebrisManager : public WorldManager
    {
    public:
        /// @brief Basic container type for DebrisFactory storage by this class.
        typedef std::map<String,DebrisFactory*>      FactoryMap;
        /// @brief Iterator type for DebrisFactory instances stored by this class.
        typedef FactoryMap::iterator                 FactoryIterator;
        /// @brief Const Iterator type for DebrisFactory instances stored by this class.
        typedef FactoryMap::const_iterator           ConstFactoryIterator;
        /// @brief Basic container type for Debris storage by this class.
        typedef std::vector< Debris* >               DebrisContainer;
        /// @brief Iterator type for Debris instances stored by this class.
        typedef DebrisContainer::iterator            DebrisIterator;
        /// @brief Const Iterator type for Debris instances stored by this class.
        typedef DebrisContainer::const_iterator      ConstDebrisIterator;
    protected:
        friend class DebrisUpdateWorkUnit;

        /// @internal
        /// @brief A map containing all registered Debris type factories.
        FactoryMap DebrisFactories;
        /// @internal
        /// @brief Container storing all Debris belonging to this manager.
        DebrisContainer Debriss;

        /// @internal
        /// @brief The work unit that updates all the Debris stored by this manager.
        DebrisUpdateWorkUnit* DebrisUpdateWork;
        /// @internal
        /// @brief Can be used for thread safe logging and other thread specific resources.
        Threading::DefaultThreadSpecificStorage::Type* ThreadResources;
    public:
        /// @brief Class constructor.
        DebrisManager(World * ParentWorld = NULL);
        /// @brief XML constructor.
        /// @param XMLNode The node of the xml document to construct from.
        DebrisManager(XML::Node& XMLNode);
        /// @brief Class destructor.
        virtual ~DebrisManager();

        ///////////////////////////////////////////////////////////////////////////////
        // Prefab Debris Type Creation

        /// @brief Creates a new RigidDebris.
        /// @param Name The name to be given to the new RigidDebris.
        /// @param Mass The mass of the debris object.
        /// @param AddToWorld Wether or not the new Debris should be added to the world after it has been created.
        /// @return Returns a pointer to the created Debris.
        RigidDebris* CreateRigidDebris(const String& Name, const Real Mass, const Boole AddToWorld = true);
        /// @brief Creates a new RigidDebris.
        /// @param Name The name to be given to the new RigidDebris.
        /// @param Mass The mass of the debris object.
        /// @param DebMesh A pointer to the Graphics mesh that will be applied to the new RigidDebris.
        /// @param DebShape A pointer to the Collision shape that will be applied to the new RigidDebris.
        /// @param AddToWorld Wether or not the new Debris should be added to the world after it has been created.
        /// @return Returns a pointer to the created Debris.
        RigidDebris* CreateRigidDebris(const String& Name, const Real Mass, Graphics::Mesh* DebMesh, Physics::CollisionShape* DebShape, const Boole AddToWorld = true);
        /// @brief Creates a new RigidDebris.
        /// @param SelfRoot An XML::Node containing the data to populate this class with.
        /// @return Returns a pointer to the created Debris.
        RigidDebris* CreateRigidDebris(const XML::Node& SelfRoot);
        /// @brief Creates a new SoftDebris.
        /// @param Name The name to be given to the new SoftDebris.
        /// @param Mass The mass of the debris object.
        /// @param AddToWorld Wether or not the new Debris should be added to the world after it has been created.
        /// @return Returns a pointer to the created Debris.
        SoftDebris* CreateSoftDebris(const String& Name, const Real Mass, const Boole AddToWorld = true);
        /// @brief Creates a new SoftDebris.
        /// @param SelfRoot An XML::Node containing the data to populate this class with.
        /// @return Returns a pointer to the created Debris.
        SoftDebris* CreateSoftDebris(const XML::Node& SelfRoot);

        ///////////////////////////////////////////////////////////////////////////////
        // Debris Management

        /// @brief Creates a new Debris.
        /// @param TypeName A string containing the name of the type of Debris to be constructed.
        /// @param InstanceName A string containing the name to be given to the created Debris.
        /// @param Params A container of additional parameters to be used for the construction of the new Debris.
        /// @param AddToWorld Wether or not the new Debris should be added to the world after it has been created.
        /// @return Returns a pointer to the created Debris.
        Debris* CreateDebris(const String& TypeName, const String& InstanceName, const NameValuePairMap& Params, const Boole AddToWorld = true);
        /// @brief Creates a new Debris class from an XML node.
        /// @remarks This is mostly useful for deserialization.
        /// @return Returns a pointer to the created Debris.
        Debris* CreateDebris(const XML::Node& SelfRoot);

        /// @brief Gets an Debris by Index.
        /// @param Index The index of the Debris you wish to retrieve.
        /// @return Returns a pointer to the Debris at the specified index.
        virtual Debris* GetDebris(const Whole Index) const;
        /// @brief Gets an Debris by Name.
        /// @param Name The name of the Debris you wish to retrieve.
        /// @return Returns a pointer to the Debris of the specified name.
        virtual Debris* GetDebris(const String& Name) const;
        /// @brief Gets the number of Debriss stored in this manager.
        /// @return Returns a whole representing the current Debris count.
        virtual Whole GetNumDebris() const;
        /// @brief Destroys an Debris at the specified index.
        /// @param Index The index at which to destroy the Debris.
        virtual void DestroyDebris(const Whole Index);
        /// @brief Destroys an Debris.
        /// @param ToBeDestroyed The Debris to be destroyed.
        virtual void DestroyDebris(Debris* ToBeDestroyed);
        /// @brief Destroys all Debriss currently within this manager.
        virtual void DestroyAllDebris();

        #ifndef SWIG
        /// @brief Gets an iterator to the first Debris in this manager.
        /// @return Returns an iterator to the first Debris being stored by this manager.
        DebrisIterator BeginDebris();
        /// @brief Gets an iterator to one passed the last Debris in this manager.
        /// @return Returns an iterator to one passed the last Debris being stored by this manager.
        DebrisIterator EndDebris();
        /// @brief Gets a const iterator to the first Debris in this manager.
        /// @return Returns a const iterator to the first Debris being stored by this manager.
        ConstDebrisIterator BeginDebris() const;
        /// @brief Gets a const iterator to one passed the last Debris in this manager.
        /// @return Returns a const iterator to one passed the last Debris being stored by this manager.
        ConstDebrisIterator EndDebris() const;
        #endif

        ///////////////////////////////////////////////////////////////////////////////
        // DebrisFactory Management

        /// @brief Adds/registers a Debris factory with this manager, allowing it to be constructed through this API.
        /// @param ToBeAdded The Debris factory to be added.
        virtual void AddDebrisFactory(DebrisFactory* ToBeAdded);
        /// @brief Removes a Debris factory from this manager.
        /// @param ToBeRemoved A pointer to the Debris factory that is to be removed.
        virtual void RemoveDebrisFactory(DebrisFactory* ToBeRemoved);
        /// @brief Removes a Debris factory from this manager.
        /// @param ImplName The name of the Debris implementation created by the factory to be removed.
        virtual void RemoveDebrisFactory(const String& ImplName);
        /// @brief Removes and destroys a Debris factory in this manager.
        /// @param ToBeDestroyed A pointer to the Debris factory that is to be removed and destroyed.
        virtual void DestroyDebrisFactory(DebrisFactory* ToBeDestroyed);
        /// @brief Removes and destroys a Debris factory in this manager.
        /// @param ImplName The name of the Debris implementation created by the factory to be removed and destroyed.
        virtual void DestroyDebrisFactory(const String& ImplName);
        /// @brief Destroys all Debris factories in this manager.
        /// @warning The destruction of Debris factories should only be done after all the Debris have been destroyed, otherwise this will cause an exception.
        virtual void DestroyAllDebrisFactories();

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        /// @copydoc WorldManager::Pause(const UInt32)
        virtual void Pause(const UInt32 PL);

        /// @copydoc WorldManager::Initialize()
        virtual void Initialize();
        /// @copydoc ManagerBase::Deinitialize()
        virtual void Deinitialize();

        /// @brief Gets the work unit responsible for updating Debriss stored by this manager.
        /// @return Returns a pointer to the ActorUpdateWorkUnit used by this manager.
        DebrisUpdateWorkUnit* GetDebrisUpdateWork();

        ///////////////////////////////////////////////////////////////////////////////
        // Type Identifier Methods

        /// @copydoc ManagerBase::GetInterfaceType()
        virtual ManagerType GetInterfaceType() const;
        /// @copydoc ManagerBase::GetImplementationTypeName()
        virtual String GetImplementationTypeName() const;
    };//DebrisManager

    ///////////////////////////////////////////////////////////////////////////////
    /// @brief A factory responsible for the creation and destruction of the default DebrisManager.
    ///////////////////////////////////////
    class DefaultDebrisManagerFactory : public ManagerFactory
    {
    public:
        /// @brief Class constructor.
        DefaultDebrisManagerFactory();
        /// @brief Class destructor.
        virtual ~DefaultDebrisManagerFactory();

        /// @copydoc ManagerFactory::GetManagerTypeName()
        String GetManagerTypeName() const;

        /// @copydoc ManagerFactory::CreateManager(NameValuePairList&)
        ManagerBase* CreateManager(NameValuePairList& Params);
        /// @copydoc ManagerFactory::CreateManager(XML::Node&)
        ManagerBase* CreateManager(XML::Node& XMLNode);
        /// @copydoc ManagerFactory::DestroyManager(ManagerBase*)
        void DestroyManager(ManagerBase* ToBeDestroyed);
    };//DefaultDebrisManagerFactory
}//Mezzanine

#endif
