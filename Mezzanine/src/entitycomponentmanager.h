// © Copyright 2010 - 2017 BlackTopp Studios Inc.
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
#ifndef _entitycomponentmanager_h
#define _entitycomponentmanager_h

#include "worldmanager.h"

namespace Mezzanine
{
    class WorldEntityComponent;
    ///////////////////////////////////////////////////////////////////////////////
    /// @brief This is an interface class for a world manager responsible for the management of WorldEntityComponent instances.
    ///////////////////////////////////////
    class MEZZ_LIB EntityComponentManager : public WorldManager
    {
    public:
        /// @brief Convenience type for a vector of stored WorldEntityComponent instances.
        using ComponentVec = std::vector<WorldEntityComponent*>;
    protected:
    public:
        /// @brief Class constructor.
        /// @param Creator The parent world that is creating the manager.
        EntityComponentManager(World* Creator) :
            WorldManager(Creator)
            {  }
        /// @brief Class destructor.
        virtual ~EntityComponentManager() = default;

        ///////////////////////////////////////////////////////////////////////////////
        // Component Management

        /// @brief Creates a new WorldEntityComponent via XML.
        /// @param SelfRoot An XML::Node containing the data to populate the new instance with.
        /// @return Returns a pointer to the created component.
        virtual WorldEntityComponent* CreateComponent(const XML::Node& SelfRoot) = 0;

        /// @brief Gets the WorldEntityComponent via its ID.
        /// @param ID The unique identifier belonging to the component.
        /// @return Returns a pointer to the WorldEntityComponent with the specified ID.
        virtual WorldEntityComponent* GetComponentByID(const UInt32 ID) const = 0;

        /// @brief Gets the number of WorldEntityComponent instances in this manager.
        /// @return Returns a UInt32 representing the number of WorldEntityComponent instances contained in this manager.
        virtual UInt32 GetNumComponents() const = 0;
        /// @brief Gets the number of WorldEntityComponent instances of a certain type(s) in this manager.
        /// @param Types A bitmask of all the WorldEntityComponent types to check for.  See the ComponentType enum for more information.
        /// @return Returns the number of WorldEntityComponent instances of the specified types are being stored by this manager.
        virtual UInt32 GetNumComponents(const UInt32 Types) const = 0;
        /// @brief Gets all the components being stored by this manager.
        /// @warning DO NOT delete any of the components returned by this method directly.  You will have a bad time.
        /// Use the DestroyComponent method on the manager if you want to delete any.
        /// @return Returns a vector of all the components in this manager.
        virtual EntityComponentManager::ComponentVec GetComponents() const = 0;

        /// @brief Deletes a WorldEntityComponent.
        /// @param ToBeDestroyed A pointer to the WorldEntityComponent you want deleted.
        virtual void DestroyComponent(WorldEntityComponent* ToBeDestroyed) = 0;
        /// @brief Deletes all stored WorldEntityComponent instances of the specified type(s).
        /// @param Types A bitmask of all the WorldEntityComponent types to be destroyed.
        virtual void DestroyAllComponents(const UInt32 Types) = 0;
        /// @brief Deletes all stored WorldEntityComponent instances.
        virtual void DestroyAllComponents() = 0;
    };//EntityComponentManager
}//Mezzanine

#endif
