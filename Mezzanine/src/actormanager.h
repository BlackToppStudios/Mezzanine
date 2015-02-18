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
#ifndef actormanager_h
#define actormanager_h

#include "worldmanager.h"
#include "managerfactory.h"
#ifndef SWIG
    #include "Threading/workunit.h"
#endif

namespace Mezzanine
{
    class Actor;
    class ActorFactory;
    class ActorManager;

    ///////////////////////////////////////////////////////////////////////////////
    /// @brief This is a Mezzanine::Threading::iWorkUnit for the updating of actors.
    /// @details
    ///////////////////////////////////////
    class MEZZ_LIB ActorUpdateWorkUnit : public Threading::DefaultWorkUnit
    {
    protected:
        /// @internal
        /// @brief A pointer to the manager this work unit is processing.
        ActorManager* TargetManager;
        /// @internal
        /// @brief Protected copy constructor.  THIS IS NOT ALLOWED.
        /// @param Other The other work unit being copied from.  WHICH WILL NEVER HAPPEN.
        ActorUpdateWorkUnit(const ActorUpdateWorkUnit& Other);
        /// @internal
        /// @brief Protected assignment operator.  THIS IS NOT ALLOWED.
        /// @param Other The other work unit being copied from.  WHICH WILL NEVER HAPPEN.
        ActorUpdateWorkUnit& operator=(const ActorUpdateWorkUnit& Other);
    public:
        /// @brief Class constructor.
        /// @param Target The ActorManager this work unit will process during the frame.
        ActorUpdateWorkUnit(ActorManager* Target);
        /// @brief Class destructor.
        virtual ~ActorUpdateWorkUnit();

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        /// @brief This does any required update of the Actors stored by it's manager.
        /// @param CurrentThreadStorage The storage class for all resources owned by this work unit during it's execution.
        virtual void DoWork(Threading::DefaultThreadSpecificStorage::Type& CurrentThreadStorage);
    };//ActorUpdateWorkUnit

    ///////////////////////////////////////////////////////////////////////////////
    /// @class ActorManager
    /// @headerfile actormanager.h
    /// @brief A manager responsible for the storage and management of all actors that exist in a world.
    /// @details More or less Management point for a container of actors to help keep them sorted.
    ///////////////////////////////////////
    class MEZZ_LIB ActorManager : public WorldManager
    {
    public:
        /// @brief Basic container type for ActorFactory storage by this class.
        typedef std::map<String,ActorFactory*>      FactoryMap;
        /// @brief Iterator type for ActorFactory instances stored by this class.
        typedef FactoryMap::iterator                 FactoryIterator;
        /// @brief Const Iterator type for ActorFactory instances stored by this class.
        typedef FactoryMap::const_iterator           ConstFactoryIterator;
        /// @brief Basic container type for @ref Actor storage by this class.
        typedef std::vector<Actor*>                  ActorContainer;
        /// @brief Iterator type for @ref Actor instances stored by this class.
        typedef ActorContainer::iterator             ActorIterator;
        /// @brief Const Iterator type for @ref Actor instances stored by this class.
        typedef ActorContainer::const_iterator       ConstActorIterator;
    protected:
        friend class ActorUpdateWorkUnit;

        /// @internal
        /// @brief A map containing all registered Actor type factories.
        FactoryMap ActorFactories;
        /// @internal
        /// @brief Container storing all Actors belonging to this manager.
        ActorContainer Actors;

        /// @internal
        /// @brief The work unit that updates all the actors stored by this manager.
        ActorUpdateWorkUnit* ActorUpdateWork;
        /// @internal
        /// @brief Can be used for thread safe logging and other thread specific resources.
        Threading::DefaultThreadSpecificStorage::Type* ThreadResources;
    public:
        /// @brief Class constructor.
        ActorManager(World * ParentWorld = NULL);
        /// @brief XML constructor.
        /// @param XMLNode The node of the xml document to construct from.
        ActorManager(XML::Node& XMLNode);
        /// @brief Class destructor.
        virtual ~ActorManager();

        ///////////////////////////////////////////////////////////////////////////////
        // Prefab Actor Type Creation

        ///////////////////////////////////////////////////////////////////////////////
        // Actor Management

        /// @brief Creates a new Actor.
        /// @param TypeName A string containing the name of the type of Actor to be constructed.
        /// @param InstanceName A string containing the name to be given to the created Actor.
        /// @param Params A container of additional parameters to be used for the construction of the new Actor.
        /// @return Returns a pointer to the created Actor.
        Actor* CreateActor(const String& TypeName, const String& InstanceName, const NameValuePairMap& Params);
        /// @brief Creates a new Actor class from an XML node.
        /// @remarks This is mostly useful for deserialization.
        /// @return Returns a pointer to the created Actor.
        Actor* CreateActor(const XML::Node& SelfRoot);

        /// @brief Gets an Actor by Index.
        /// @param Index The index of the Actor you wish to retrieve.
        /// @return Returns a pointer to the Actor at the specified index.
        virtual Actor* GetActor(const Whole Index) const;
        /// @brief Gets an Actor by Name.
        /// @param Name The name of the Actor you wish to retrieve.
        /// @return Returns a pointer to the Actor of the specified name.
        virtual Actor* GetActor(const String& Name) const;
        /// @brief Gets the number of Actors stored in this manager.
        /// @return Returns a whole representing the current Actor count.
        virtual Whole GetNumActors() const;
        /// @brief Destroys an Actor at the specified index.
        /// @param Index The index at which to destroy the Actor.
        virtual void DestroyActor(const Whole Index);
        /// @brief Destroys an Actor.
        /// @param ToBeDestroyed The Actor to be destroyed.
        virtual void DestroyActor(Actor* ToBeDestroyed);
        /// @brief Destroys all Actors currently within this manager.
        virtual void DestroyAllActors();

        #ifndef SWIG
        /// @brief Gets an iterator to the first Actor in this manager.
        /// @return Returns an iterator to the first Actor being stored by this manager.
        ActorIterator BeginActor();
        /// @brief Gets an iterator to one passed the last Actor in this manager.
        /// @return Returns an iterator to one passed the last Actor being stored by this manager.
        ActorIterator EndActor();
        /// @brief Gets a const iterator to the first Actor in this manager.
        /// @return Returns a const iterator to the first Actor being stored by this manager.
        ConstActorIterator BeginActor() const;
        /// @brief Gets a const iterator to one passed the last Actor in this manager.
        /// @return Returns a const iterator to one passed the last Actor being stored by this manager.
        ConstActorIterator EndActor() const;
        #endif

        ///////////////////////////////////////////////////////////////////////////////
        // ActorFactory Management

        /// @brief Adds/registers a Actor factory with this manager, allowing it to be constructed through this API.
        /// @param ToBeAdded The Actor factory to be added.
        virtual void AddActorFactory(ActorFactory* ToBeAdded);
        /// @brief Removes a Actor factory from this manager.
        /// @param ToBeRemoved A pointer to the Actor factory that is to be removed.
        virtual void RemoveActorFactory(ActorFactory* ToBeRemoved);
        /// @brief Removes a Actor factory from this manager.
        /// @param ImplName The name of the Actor implementation created by the factory to be removed.
        virtual void RemoveActorFactory(const String& ImplName);
        /// @brief Removes and destroys a Actor factory in this manager.
        /// @param ToBeDestroyed A pointer to the Actor factory that is to be removed and destroyed.
        virtual void DestroyActorFactory(ActorFactory* ToBeDestroyed);
        /// @brief Removes and destroys a Actor factory in this manager.
        /// @param ImplName The name of the Actor implementation created by the factory to be removed and destroyed.
        virtual void DestroyActorFactory(const String& ImplName);
        /// @brief Destroys all Actor factories in this manager.
        /// @warning The destruction of Actor factories should only be done after all the Actor have been destroyed, otherwise this will cause an exception.
        virtual void DestroyAllActorFactories();

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        /// @copydoc WorldManager::Pause(const UInt32)
        virtual void Pause(const UInt32 PL);

        /// @copydoc WorldManager::Initialize()
        virtual void Initialize();
        /// @copydoc ManagerBase::Deinitialize()
        virtual void Deinitialize();

        /// @brief Gets the work unit responsible for updating actors stored by this manager.
        /// @return Returns a pointer to the ActorUpdateWorkUnit used by this manager.
        ActorUpdateWorkUnit* GetActorUpdateWork();

        ///////////////////////////////////////////////////////////////////////////////
        // Type Identifier Methods

        /// @copydoc ManagerBase::GetInterfaceType()
        virtual ManagerType GetInterfaceType() const;
        /// @copydoc ManagerBase::GetImplementationTypeName()
        virtual String GetImplementationTypeName() const;
    };//ActorManager

    ///////////////////////////////////////////////////////////////////////////////
    /// @class DefaultActorManagerFactory
    /// @headerfile actormanager.h
    /// @brief A factory responsible for the creation and destruction of the default actormanager.
    ///////////////////////////////////////
    class DefaultActorManagerFactory : public ManagerFactory
    {
    public:
        /// @brief Class constructor.
        DefaultActorManagerFactory();
        /// @brief Class destructor.
        virtual ~DefaultActorManagerFactory();

        /// @copydoc ManagerFactory::GetManagerTypeName()
        String GetManagerTypeName() const;

        /// @copydoc ManagerFactory::CreateManager(NameValuePairList&)
        ManagerBase* CreateManager(NameValuePairList& Params);
        /// @copydoc ManagerFactory::CreateManager(XML::Node&)
        ManagerBase* CreateManager(XML::Node& XMLNode);
        /// @copydoc ManagerFactory::DestroyManager(ManagerBase*)
        void DestroyManager(ManagerBase* ToBeDestroyed);
    };//DefaultActorManagerFactory
}//Mezzanine

#endif
