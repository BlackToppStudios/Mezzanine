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
#ifndef _areaeffectmanager_h
#define _areaeffectmanager_h

#include "datatypes.h"
#include "worldmanager.h"
#include "managerfactory.h"
#include "singleton.h"
#include "Threading/workunit.h"

namespace Mezzanine
{
    class AreaEffect;
    class AreaEffectManager;

    ///////////////////////////////////////////////////////////////////////////////
    /// @brief This is a @ref iWorkUnit for the updating of AreaEffects.
    /// @details
    ///////////////////////////////////////
    class MEZZ_LIB AreaEffectUpdateWorkUnit : public Threading::DefaultWorkUnit
    {
    protected:
        /// @internal
        /// @brief A pointer to the manager this work unit is processing.
        AreaEffectManager* TargetManager;
        /// @internal
        /// @brief Protected copy constructor.  THIS IS NOT ALLOWED.
        /// @param Other The other work unit being copied from.  WHICH WILL NEVER HAPPEN.
        AreaEffectUpdateWorkUnit(const AreaEffectUpdateWorkUnit& Other);
        /// @internal
        /// @brief Protected assignment operator.  THIS IS NOT ALLOWED.
        /// @param Other The other work unit being copied from.  WHICH WILL NEVER HAPPEN.
        AreaEffectUpdateWorkUnit& operator=(const AreaEffectUpdateWorkUnit& Other);
    public:
        /// @brief Class constructor.
        /// @param Target The AreaEffectManager this work unit will process during the frame.
        AreaEffectUpdateWorkUnit(AreaEffectManager* Target);
        /// @brief Class destructor.
        virtual ~AreaEffectUpdateWorkUnit();

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        /// @brief This does any required update of the Graphical Scene graph and REnders one frame
        /// @param CurrentThreadStorage The storage class for all resources owned by this work unit during it's execution.
        virtual void DoWork(Threading::DefaultThreadSpecificStorage::Type& CurrentThreadStorage);
    };//AreaEffectUpdateWorkUnit

    ///////////////////////////////////////////////////////////////////////////////
    /// @class AreaEffectManager
    /// @headerfile areaeffectmanager.h
    /// @brief A manager responsible for the storage and management of all areaeffects in use.
    /// @details More or less Management point for a container of areaeffects to help keep them sorted.
    ///////////////////////////////////////
    class MEZZ_LIB AreaEffectManager : public WorldManager
    {
    public:
        /// @brief Basic container type for @ref AreaEffect storage by this class.
        typedef std::vector<AreaEffect*>              AreaEffectContainer;
        /// @brief Iterator type for @ref AreaEffect instances stored by this class.
        typedef AreaEffectContainer::iterator         AreaEffectIterator;
        /// @brief Const Iterator type for @ref AreaEffect instances stored by this class.
        typedef AreaEffectContainer::const_iterator   ConstAreaEffectIterator;
    protected:
        friend class AreaEffectUpdateWorkUnit;

        /// @brief The actual areaeffect container.
        AreaEffectContainer AreaEffects;

        /// @internal
        /// @brief The work unit that updates all the actors stored by this manager.
        AreaEffectUpdateWorkUnit* AreaEffectUpdateWork;
        /// @internal
        /// @brief Can be used for thread safe logging and other thread specific resources.
        Threading::DefaultThreadSpecificStorage::Type* ThreadResources;
    public:
        /// @brief Class constructor.
        AreaEffectManager();
        /// @brief XML constructor.
        /// @param XMLNode The node of the xml document to construct from.
        AreaEffectManager(XML::Node& XMLNode);
        /// @brief Class destructor.
        virtual ~AreaEffectManager();

        ///////////////////////////////////////////////////////////////////////////////
        // Managing all actors

        /// @brief Adds a pre-created areaeffect to the manager.
        /// @details In some cases you may want to add and remove an areaeffect from the world without destroying it and do some special
        /// manipulations to it to achieve some special/unique affects.  This function along with the "RemoveActor()"
        /// function facilitates this. @n
        /// This function is also necessary for anyone inheriting from our actors to add their actors to the world.
        /// @param ToBeAdded The areaeffects to be added to the manager.
        virtual void AddAreaEffect(AreaEffect* ToBeAdded);
        /// @brief Gets an AreaEffect by Index.
        /// @param Index The index of the areaeffect you wish to retrieve.
        /// @return Returns a pointer to the areaeffect at the specified index.
        virtual AreaEffect* GetAreaEffect(const Whole& Index) const;
        /// @brief Gets an AreaEffect by Name.
        /// @param Name The name of the areaeffect you wish to retrieve.
        /// @return Returns a pointer to the areaeffect of the specified name.
        virtual AreaEffect* GetAreaEffect(const String& Name) const;
        /// @brief Gets the number of actors stored in this manager.
        /// @return Returns a whole representing the current areaeffect count.
        virtual Whole GetNumAreaEffects() const;
        /// @brief Removes an areaeffect from this manager without destroying it.
        /// @details In some cases you may want to add and remove an areaeffect from the world without destroying it and do some special
        /// manipulations to it to achieve some special/unique affects.  This function along with the "RemoveActor()"
        /// function facilitates this. @n
        /// This function is also necessary for anyone inheriting from our actors to remove their actors from the world.
        /// @param Index The index at which to remove the areaeffect.
        virtual void RemoveAreaEffect(const Whole& Index);
        /// @brief Removes an areaeffect from this manager without destroying it.
        /// @details In some cases you may want to add and remove an areaeffect from the world without destroying it and do some special
        /// manipulations to it to achieve some special/unique affects.  This function along with the "RemoveActor()"
        /// function facilitates this. @n
        /// This function is also necessary for anyone inheriting from our actors to remove their actors from the world.
        /// @param ToBeRemoved The areaeffect to be removed from the manager.
        virtual void RemoveAreaEffect(AreaEffect* ToBeRemoved);
        /// @brief Removes all actors from this manager without destroying them.
        virtual void RemoveAllAreaEffects();
        /// @brief Destroys an areaeffect at the specified index.
        /// @param Index The index at which to destroy the areaeffect.
        virtual void DestroyAreaEffect(const Whole& Index);
        /// @brief Destroys an areaeffect.
        /// @param ToBeDestroyed The areaeffect to be destroyed.
        virtual void DestroyAreaEffect(AreaEffect* ToBeDestroyed);
        /// @brief Destroys all actors currently within this manager.
        virtual void DestroyAllAreaEffects();

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        /// @copydoc WorldManager::Pause(const UInt32)
        virtual void Pause(const UInt32 PL);

        /// @brief Does all of the necessary configuration to prepare for the start of the main loop.
        void MainLoopInitialize();
        /// @copydoc WorldManager::Initialize()
        virtual void Initialize();
        /// @copydoc ManagerBase::Deinitialize()
        virtual void Deinitialize();

        /// @brief Gets the work unit responsible for updating area effects stored by this manager.
        /// @return Returns a pointer to the AreaEffectUpdateWorkUnit used by this manager.
        AreaEffectUpdateWorkUnit* GetAreaEffectUpdateWork();

        ///////////////////////////////////////////////////////////////////////////////
        // Type Identifier Methods

        /// @copydoc ManagerBase::GetInterfaceType()
        virtual ManagerType GetInterfaceType() const;
        /// @copydoc ManagerBase::GetImplementationTypeName()
        virtual String GetImplementationTypeName() const;
    };//AreaEffectManager

    ///////////////////////////////////////////////////////////////////////////////
    /// @class DefaultAreaEffectManagerFactory
    /// @headerfile areaeffectmanager.h
    /// @brief A factory responsible for the creation and destruction of the default areaeffectmanager.
    ///////////////////////////////////////
    class DefaultAreaEffectManagerFactory : public ManagerFactory
    {
    public:
        /// @brief Class constructor.
        DefaultAreaEffectManagerFactory();
        /// @brief Class destructor.
        virtual ~DefaultAreaEffectManagerFactory();

        /// @copydoc ManagerFactory::GetManagerTypeName()
        String GetManagerTypeName() const;

        /// @copydoc ManagerFactory::CreateManager(NameValuePairList&)
        ManagerBase* CreateManager(NameValuePairList& Params);
        /// @copydoc ManagerFactory::CreateManager(XML::Node&)
        ManagerBase* CreateManager(XML::Node& XMLNode);
        /// @copydoc ManagerFactory::DestroyManager(ManagerBase*)
        void DestroyManager(ManagerBase* ToBeDestroyed);
    };//DefaultAreaEffectManagerFactory
}//Mezzanine

#endif
