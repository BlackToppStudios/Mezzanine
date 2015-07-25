// © Copyright 2010 - 2015 BlackTopp Studios Inc.
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

#include "worldmanager.h"
#include "worldmanagerfactory.h"
#ifndef SWIG
    #include "Threading/workunit.h"
#endif

namespace Mezzanine
{
    class AreaEffect;
    class AreaEffectFactory;
    class AreaEffectManager;

    class FieldOfForce;
    class GravityField;
    class GravityWell;

    namespace Physics {
        class CollisionShape;
    }

    ///////////////////////////////////////////////////////////////////////////////
    /// @brief This is a Mezzanine::Threading::iWorkUnit for the updating of AreaEffects.
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
        /// @brief Basic container type for AreaEffectFactory storage by this class.
        typedef std::map<String,AreaEffectFactory*>   FactoryMap;
        /// @brief Iterator type for AreaEffectFactory instances stored by this class.
        typedef FactoryMap::iterator                  FactoryIterator;
        /// @brief Const Iterator type for AreaEffectFactory instances stored by this class.
        typedef FactoryMap::const_iterator            ConstFactoryIterator;
        /// @brief Basic container type for AreaEffect storage by this class.
        typedef std::vector<AreaEffect*>              AreaEffectContainer;
        /// @brief Iterator type for AreaEffect instances stored by this class.
        typedef AreaEffectContainer::iterator         AreaEffectIterator;
        /// @brief Const Iterator type for AreaEffect instances stored by this class.
        typedef AreaEffectContainer::const_iterator   ConstAreaEffectIterator;

        /// @brief A String containing the name of this manager implementation.
        static const String ImplementationName;
        /// @brief A ManagerType enum value used to describe the type of interface/functionality this manager provides.
        static const ManagerBase::ManagerType InterfaceType;
    protected:
        friend class AreaEffectUpdateWorkUnit;

        /// @internal
        /// @brief A map containing all registered AreaEffect type factories.
        FactoryMap AreaEffectFactories;
        /// @internal
        /// @brief Container storing all AreaEffects belonging to this manager.
        AreaEffectContainer AreaEffects;

        /// @internal
        /// @brief The work unit that updates all the actors stored by this manager.
        AreaEffectUpdateWorkUnit* AreaEffectUpdateWork;
        /// @internal
        /// @brief Can be used for thread safe logging and other thread specific resources.
        Threading::DefaultThreadSpecificStorage::Type* ThreadResources;
    public:
        /// @brief Class constructor.
        /// @param Creator The parent world that is creating the manager.
        AreaEffectManager(World* Creator);
        /// @brief XML constructor.
        /// @param Creator The parent world that is creating the manager.
        /// @param XMLNode The node of the xml document to construct from.
        AreaEffectManager(World* Creator, XML::Node& XMLNode);
        /// @brief Class destructor.
        virtual ~AreaEffectManager();

        ///////////////////////////////////////////////////////////////////////////////
        // Prefab AreaEffect Type Creation

        /// @brief Creates a new FieldOfForce.
        /// @param Name The name to be given to the new FieldOfForce.
        /// @param AddToWorld Wether or not the new AreaEffect should be added to the world after it has been created.
        /// @return Returns a pointer to the created AreaEffect.
        FieldOfForce* CreateFieldOfForce(const String& Name, const Boole AddToWorld = true);
        /// @brief Creates a new FieldOfForce.
        /// @param Name The name to be given to the new FieldOfForce.
        /// @param AEShape A pointer to the Collision shape that will be applied to the new FieldOfForce.
        /// @param AddToWorld Wether or not the new AreaEffect should be added to the world after it has been created.
        /// @return Returns a pointer to the created AreaEffect.
        FieldOfForce* CreateFieldOfForce(const String& Name, Physics::CollisionShape* AEShape, const Boole AddToWorld = true);
        /// @brief Creates a new FieldOfForce.
        /// @param SelfRoot An XML::Node containing the data to populate this class with.
        /// @return Returns a pointer to the created AreaEffect.
        FieldOfForce* CreateFieldOfForce(const XML::Node& SelfRoot);
        /// @brief Creates a new GravityField.
        /// @param Name The name to be given to the new GravityField.
        /// @param AddToWorld Wether or not the new AreaEffect should be added to the world after it has been created.
        /// @return Returns a pointer to the created AreaEffect.
        GravityField* CreateGravityField(const String& Name, const Boole AddToWorld = true);
        /// @brief Creates a new GravityField.
        /// @param Name The name to be given to the new GravityField.
        /// @param AEShape A pointer to the Collision shape that will be applied to the new GravityField.
        /// @param AddToWorld Wether or not the new AreaEffect should be added to the world after it has been created.
        /// @return Returns a pointer to the created AreaEffect.
        GravityField* CreateGravityField(const String& Name, Physics::CollisionShape* AEShape, const Boole AddToWorld = true);
        /// @brief Creates a new GravityField.
        /// @param SelfRoot An XML::Node containing the data to populate this class with.
        /// @return Returns a pointer to the created AreaEffect.
        GravityField* CreateGravityField(const XML::Node& SelfRoot);
        /// @brief Creates a new GravityWell.
        /// @param Name The name to be given to the new GravityWell.
        /// @param AddToWorld Wether or not the new AreaEffect should be added to the world after it has been created.
        /// @return Returns a pointer to the created AreaEffect.
        GravityWell* CreateGravityWell(const String& Name, const Boole AddToWorld = true);
        /// @brief Creates a new GravityWell.
        /// @param Name The name to be given to the new GravityWell.
        /// @param AEShape A pointer to the Collision shape that will be applied to the new GravityWell.
        /// @param AddToWorld Wether or not the new AreaEffect should be added to the world after it has been created.
        /// @return Returns a pointer to the created AreaEffect.
        GravityWell* CreateGravityWell(const String& Name, Physics::CollisionShape* AEShape, const Boole AddToWorld = true);
        /// @brief Creates a new GravityWell.
        /// @param SelfRoot An XML::Node containing the data to populate this class with.
        /// @return Returns a pointer to the created AreaEffect.
        GravityWell* CreateGravityWell(const XML::Node& SelfRoot);

        ///////////////////////////////////////////////////////////////////////////////
        // AreaEffect Management

        /// @brief Creates a new AreaEffect.
        /// @param TypeName A string containing the name of the type of AreaEffect to be constructed.
        /// @param InstanceName A string containing the name to be given to the created AreaEffect.
        /// @param Params A container of additional parameters to be used for the construction of the new AreaEffect.
        /// @param AddToWorld Wether or not the new AreaEffect should be added to the world after it has been created.
        /// @return Returns a pointer to the created AreaEffect.
        AreaEffect* CreateAreaEffect(const String& TypeName, const String& InstanceName, const NameValuePairMap& Params, const Boole AddToWorld = true);
        /// @brief Creates a new AreaEffect class from an XML node.
        /// @remarks This is mostly useful for deserialization.
        /// @return Returns a pointer to the created AreaEffect.
        AreaEffect* CreateAreaEffect(const XML::Node& SelfRoot);

        /// @brief Gets an AreaEffect by Index.
        /// @param Index The index of the areaeffect you wish to retrieve.
        /// @return Returns a pointer to the areaeffect at the specified index.
        virtual AreaEffect* GetAreaEffect(const Whole Index) const;
        /// @brief Gets an AreaEffect by Name.
        /// @param Name The name of the areaeffect you wish to retrieve.
        /// @return Returns a pointer to the areaeffect of the specified name.
        virtual AreaEffect* GetAreaEffect(const String& Name) const;
        /// @brief Gets the number of actors stored in this manager.
        /// @return Returns a whole representing the current areaeffect count.
        virtual Whole GetNumAreaEffects() const;
        /// @brief Destroys an areaeffect at the specified index.
        /// @param Index The index at which to destroy the areaeffect.
        virtual void DestroyAreaEffect(const Whole Index);
        /// @brief Destroys an areaeffect.
        /// @param ToBeDestroyed The areaeffect to be destroyed.
        virtual void DestroyAreaEffect(AreaEffect* ToBeDestroyed);
        /// @brief Destroys all actors currently within this manager.
        virtual void DestroyAllAreaEffects();

        #ifndef SWIG
        /// @brief Gets an iterator to the first AreaEffect in this manager.
        /// @return Returns an iterator to the first AreaEffect being stored by this manager.
        AreaEffectIterator BeginAreaEffect();
        /// @brief Gets an iterator to one passed the last AreaEffect in this manager.
        /// @return Returns an iterator to one passed the last AreaEffect being stored by this manager.
        AreaEffectIterator EndAreaEffect();
        /// @brief Gets a const iterator to the first AreaEffect in this manager.
        /// @return Returns a const iterator to the first AreaEffect being stored by this manager.
        ConstAreaEffectIterator BeginAreaEffect() const;
        /// @brief Gets a const iterator to one passed the last AreaEffect in this manager.
        /// @return Returns a const iterator to one passed the last AreaEffect being stored by this manager.
        ConstAreaEffectIterator EndAreaEffect() const;
        #endif

        ///////////////////////////////////////////////////////////////////////////////
        // AreaEffectFactory Management

        /// @brief Adds/registers a AreaEffect factory with this manager, allowing it to be constructed through this API.
        /// @param ToBeAdded The AreaEffect factory to be added.
        virtual void AddAreaEffectFactory(AreaEffectFactory* ToBeAdded);
        /// @brief Removes a AreaEffect factory from this manager.
        /// @param ToBeRemoved A pointer to the AreaEffect factory that is to be removed.
        virtual void RemoveAreaEffectFactory(AreaEffectFactory* ToBeRemoved);
        /// @brief Removes a AreaEffect factory from this manager.
        /// @param ImplName The name of the AreaEffect implementation created by the factory to be removed.
        virtual void RemoveAreaEffectFactory(const String& ImplName);
        /// @brief Removes and destroys a AreaEffect factory in this manager.
        /// @param ToBeDestroyed A pointer to the AreaEffect factory that is to be removed and destroyed.
        virtual void DestroyAreaEffectFactory(AreaEffectFactory* ToBeDestroyed);
        /// @brief Removes and destroys a AreaEffect factory in this manager.
        /// @param ImplName The name of the AreaEffect implementation created by the factory to be removed and destroyed.
        virtual void DestroyAreaEffectFactory(const String& ImplName);
        /// @brief Destroys all AreaEffect factories in this manager.
        /// @warning The destruction of AreaEffect factories should only be done after all the AreaEffects have been destroyed, otherwise this will cause an exception.
        virtual void DestroyAllAreaEffectFactories();

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        /// @copydoc WorldManager::Pause(const UInt32)
        virtual void Pause(const UInt32 PL);

        /// @brief Does all of the necessary configuration to prepare for the start of the main loop.
        virtual void MainLoopInitialize();
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
    /// @brief A factory responsible for the creation and destruction of the default areaeffectmanager.
    ///////////////////////////////////////
    class DefaultAreaEffectManagerFactory : public WorldManagerFactory
    {
    public:
        /// @brief Class constructor.
        DefaultAreaEffectManagerFactory();
        /// @brief Class destructor.
        virtual ~DefaultAreaEffectManagerFactory();

        /// @copydoc ManagerFactory::GetManagerImplName()
        String GetManagerImplName() const;
        /// @copydoc ManagerFactory::GetManagerType() const
        ManagerBase::ManagerType GetManagerType() const;

        /// @copydoc WorldManagerFactory::CreateManager(World*, NameValuePairList&)
        WorldManager* CreateManager(World* Creator, NameValuePairList& Params);
        /// @copydoc WorldManagerFactory::CreateManager(World*, XML::Node&)
        WorldManager* CreateManager(World* Creator, XML::Node& XMLNode);
        /// @copydoc WorldManagerFactory::DestroyManager(WorldManager*)
        void DestroyManager(WorldManager* ToBeDestroyed);
    };//DefaultAreaEffectManagerFactory
}//Mezzanine

#endif
