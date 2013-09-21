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
#ifndef actormanager_h
#define actormanager_h

#include "datatypes.h"
#include "worldmanager.h"
#include "managerfactory.h"
#include "singleton.h"
#include "Threading/workunit.h"

namespace Mezzanine
{
    class ActorBase;
    class ActorRigid;
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
        /// @brief Basic container type for @ref ActorBase storage by this class.
        typedef std::vector<ActorBase*>              ActorContainer;
        /// @brief Iterator type for @ref ActorBase instances stored by this class.
        typedef ActorContainer::iterator             ActorIterator;
        /// @brief Const Iterator type for @ref ActorBase instances stored by this class.
        typedef ActorContainer::const_iterator       ConstActorIterator;
    protected:
        friend class ActorUpdateWorkUnit;

        /// @brief The actual actor container
        ActorContainer Actors;

        /// @internal
        /// @brief The work unit that updates all the actors stored by this manager.
        ActorUpdateWorkUnit* ActorUpdateWork;
        /// @internal
        /// @brief Can be used for thread safe logging and other thread specific resources.
        Threading::DefaultThreadSpecificStorage::Type* ThreadResources;
    public:
        /// @brief Class constructor.
        ActorManager();
        /// @brief XML constructor.
        /// @param XMLNode The node of the xml document to construct from.
        ActorManager(XML::Node& XMLNode);
        /// @brief Class destructor.
        virtual ~ActorManager();

        ///////////////////////////////////////////////////////////////////////////////
        // Managing all actors

        /// @brief Adds a pre-created actor to the manager.
        /// @details In some cases you may want to add and remove an actor from the world without destroying it and do some special
        /// manipulations to it to achieve some special/unique affects.  This function along with the "RemoveActor()"
        /// function facilitates this. @n
        /// This function is also necessary for anyone inheriting from our actors to add their actors to the world.
        /// @param Actor The actor to be added to the manager.
        virtual void AddActor(ActorBase* Actor);
        /// @brief Gets an Actor by Index.
        /// @param Index The index of the actor you wish to retrieve.
        /// @return Returns a pointer to the actor at the specified index.
        virtual ActorBase* GetActor(const Whole& Index) const;
        /// @brief Gets an Actor by Name.
        /// @param Name The name of the actor you wish to retrieve.
        /// @return Returns a pointer to the actor of the specified name.
        virtual ActorBase* GetActor(const String& Name) const;
        /// @brief Gets the number of actors stored in this manager.
        /// @return Returns a whole representing the current actor count.
        virtual Whole GetNumActors() const;
        /// @brief Removes an actor from this manager without destroying it.
        /// @details In some cases you may want to add and remove an actor from the world without destroying it and do some special
        /// manipulations to it to achieve some special/unique affects.  This function along with the "RemoveActor()"
        /// function facilitates this. @n
        /// This function is also necessary for anyone inheriting from our actors to remove their actors from the world.
        /// @param Index The index at which to remove the actor.
        virtual void RemoveActor(const Whole& Index);
        /// @brief Removes an actor from this manager without destroying it.
        /// @details In some cases you may want to add and remove an actor from the world without destroying it and do some special
        /// manipulations to it to achieve some special/unique affects.  This function along with the "RemoveActor()"
        /// function facilitates this. @n
        /// This function is also necessary for anyone inheriting from our actors to remove their actors from the world.
        /// @param ToBeRemoved The actor to be removed from the manager.
        virtual void RemoveActor(ActorBase* ToBeRemoved);
        /// @brief Removes all actors from this manager without destroying them.
        virtual void RemoveAllActors();
        /// @brief Destroys an actor at the specified index.
        /// @param Index The index at which to destroy the actor.
        virtual void DestroyActor(const Whole& Index);
        /// @brief Destroys an actor.
        /// @param ToBeDestroyed The actor to be destroyed.
        virtual void DestroyActor(ActorBase* ToBeDestroyed);
        /// @brief Destroys all actors currently within this manager.
        virtual void DestroyAllActors();

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
