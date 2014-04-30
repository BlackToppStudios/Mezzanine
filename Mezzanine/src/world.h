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

#ifndef _world_h
#define _world_h

#include "worldmanager.h"

namespace Mezzanine
{
    class ActorManager;
    class AreaEffectManager;
    class TerrainManager;
    namespace Audio
    {
        class SoundScapeManager;
    }
    namespace Graphics
    {
        class CameraManager;
        class SceneManager;
    }
    namespace Physics
    {
        class PhysicsManager;
        class ManagerConstructionInfo;
    }
    ///////////////////////////////////////////////////////////////////////////////
    /// @class World
    /// @headerfile world.h
    /// @brief This class represents a world for objects to interact within.
    /// @details Objects can be inserted and removed from worlds in order to simulate them.  Multiple worlds can
    /// exist but objects can only belong to one world at a time.
    ///////////////////////////////////////
    class MEZZ_LIB World
    {
    public:
        String GetName() const { return String("A PlaceHolder until Ducky no longer needs it"); }

        /// @brief Basic container type for @ref WorldManager storage by this class.
        typedef std::vector< WorldManager* >       WorldManagerContainer;
        /// @brief Iterator type for @ref WorldManager instances stored by this class.
        typedef WorldManagerContainer::iterator            WorldManagerIterator;
        /// @brief Const Iterator type for @ref WorldManager instances stored by this class.
        typedef WorldManagerContainer::const_iterator      ConstWorldManagerContainer;
    protected:
        /// @internal
        /// @brief A container storing all the managers belonging to this world.
        WorldManagerContainer WorldManagers;

        /// @internal
        /// @brief Unique string identifier for world.
        String Name;
    public:
        /// @brief Class constructor.
        /// @param WorldName String name of the world.
        World(const String& WorldName);
        /// @brief Pre-made manager constructor.
        /// @param WorldName String name of the world.
        /// @param Managers A container of pre-made managers to be used by this world.
        World(const String& WorldName, const WorldManagerContainer& Managers);
        /// @brief Descriptive constructor.
        /// @param WorldName String name of the world.
        /// @param PhysicsInfo A ManagerConstructionInfo struct with data on how to configure the physics for this world.
        /// @param SceneType A string containing the name of the underlying scene type for this world.
        World(const String& WorldName, const Physics::ManagerConstructionInfo& PhysicsInfo, const String& SceneType);
        /// @brief Descriptive pre-made manager constructor.
        /// @param WorldName String name of the world.
        /// @param Managers A container of pre-made managers to be used by this world.
        /// @param PhysicsInfo A ManagerConstructionInfo struct with data on how to configure the physics for this world.
        /// @param SceneType A string containing the name of the underlying scene type for this world.
        World(const String& WorldName, const WorldManagerContainer& Managers, const Physics::ManagerConstructionInfo& PhysicsInfo, const String& SceneType);
        /// @brief XML constructor.
        /// @param SelfNode The node that represents the data to populate this world with.
        World(const XML::Node& SelfNode);
        /// @brief class destructor.
        virtual ~World();

        /// @brief Gets the name of this world.
        /// @return Returns a string containing the name used to identify this world.
        const String& GetName() const;

        ///////////////////////////////////////////////////////////////////////////////
        // Initialization

        /// @brief Initializes all managers in this world and performs all the necessary hooks to enable this world.
        void Initialize();
        /// @brief Deinitializes all managers in this world and unhooks all of it's systems, disabling this world.
        void Deinitialize();

        ///////////////////////////////////////////////////////////////////////////////
        // Upper Management

        /// @brief This adds a manager, in the correct order, to the list that the world calls on.
        /// @param ManagerToAdd The pointer to the manager to be added.
        /// @return Returns true if the manager was successfully added, false if the manager or it's type was non-unique.
        Boole AddManager(WorldManager* ManagerToAdd);
        /// @brief This is will find the manager of a given type.
        /// @param ManagerToGet The ManagerBase::ManagerTypeName of the manager to get.
        /// @return This returns a pointer to a WorldManager, or a NULL pointer if no matching manager exists.
        WorldManager* GetManager(const ManagerBase::ManagerType ManagerToGet);
        /// @brief This removes a manager by finding the matching pointer.
        /// @param ToBeRemoved A pointer to the manager to be removed.
        void RemoveManager(WorldManager* ToBeRemoved);
        /// @brief This removes a manager of a specific type from the list
        /// @param ToBeRemoved The ManagerBase::ManagerTypeName of the manager to remove.
        void RemoveManager(const ManagerBase::ManagerType ToBeRemoved);

        /// @brief This gets the ActorManager from the manager list.
        /// @return This returns a pointer to a ActorManager, or a NULL pointer if no matching manager exists.
        ActorManager* GetActorManager();
        /// @brief This gets the AreaEffectManager from the manager list.
        /// @return This returns a pointer to a AreaEffectManager, or a NULL pointer if no matching manager exists.
        AreaEffectManager* GetAreaEffectManager();
        /// @brief This gets the CameraManager from the manager list.
        /// @return This returns a pointer to a CameraManager, or a NULL pointer if no matching manager exists.
        Graphics::CameraManager* GetCameraManager();
        /// @brief This gets the PhysicsManager from the manager list.
        /// @return This returns a pointer to a PhysicsManager, or a NULL pointer if no matching manager exists.
        Physics::PhysicsManager* GetPhysicsManager();
        /// @brief This gets the SceneManager from the manager list.
        /// @return This returns a pointer to a SceneManager, or a NULL pointer if no matching manager exists.
        Graphics::SceneManager* GetSceneManager();
        /// @brief This gets the SoundScapeManager from the manager list.
        /// @return This returns a pointer to a SoundScapeManager, or a NULL pointer if no matching manager exists.
        Audio::SoundScapeManager* GetSoundScapeManager();
        /// @brief This gets the TerrainManager from the manager list.
        /// @return This returns a pointer to a TerrainManager, or a NULL pointer if no matching manager exists.
        TerrainManager* GetTerrainManager();

    private:
        /// @internal
        /// @brief Helper function used to assist construction.
        void Construct(     const Physics::ManagerConstructionInfo& PhysicsInfo,
                            const String& SceneType,
                            const std::vector <WorldManager*>& ManagerToBeAdded );
    };//World
}//Mezzanine

#endif
