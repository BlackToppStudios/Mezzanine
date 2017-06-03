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
#ifndef _worldproxymanager_h
#define _worldproxymanager_h

#include "worldmanager.h"

namespace Mezzanine
{
    class WorldProxy;
    ///////////////////////////////////////////////////////////////////////////////
    /// @brief This is an interface class for a world manager responsible for the management of WorldProxy instances.
    ///////////////////////////////////////
    class MEZZ_LIB WorldProxyManager : public WorldManager
    {
    public:
        /// @brief Convenience type for a vector of stored WorldProxy instances.
        typedef std::vector<WorldProxy*> WorldProxyVec;
    protected:
    public:
        /// @brief Class constructor.
        /// @param Creator The parent world that is creating the manager.
        WorldProxyManager(World* Creator) :
            WorldManager(Creator)
            {  }
        /// @brief Class destructor.
        virtual ~WorldProxyManager()
            {  }

        ///////////////////////////////////////////////////////////////////////////////
        // Proxy Management

        /// @brief Creates a new WorldProxy via XML.
        /// @param SelfRoot An XML::Node containing the data to populate the new instance with.
        /// @return Returns a pointer to the created proxy.
        virtual WorldProxy* CreateProxy(const XML::Node& SelfRoot) = 0;

        /// @brief Gets the WorldProxy via its ID.
        /// @param ID The unique identifier belonging to the Proxy.
        /// @return Returns a pointer to the WorldProxy with the specified ID.
        virtual WorldProxy* GetProxyByID(const UInt32 ID) const = 0;

        /// @brief Gets the number of WorldProxy instances in this manager.
        /// @return Returns a UInt32 representing the number of WorldProxy instances contained in this manager.
        virtual UInt32 GetNumProxies() const = 0;
        /// @brief Gets the number of WorldProxy instances of a certain type(s) in this manager.
        /// @param Types A bitmask of all the WorldProxy types to check for.  See the ProxyType enum for more information.
        /// @return Returns the number of WorldProxy instances of the specified types are being stored by this manager.
        virtual UInt32 GetNumProxies(const UInt32 Types) const = 0;
        /// @brief Gets all the proxies being stored by this manager.
        /// @warning DO NOT delete any of the proxies returned by this method directly.  You will have a bad time.
        /// Use the DestroyProxy method on the manager if you want to delete any.
        /// @return Returns a vector of all the proxies in this manager.
        virtual WorldProxyManager::WorldProxyVec GetProxies() const = 0;

        /// @brief Deletes a WorldProxy.
        /// @param ToBeDestroyed A pointer to the WorldProxy you want deleted.
        virtual void DestroyProxy(WorldProxy* ToBeDestroyed) = 0;
        /// @brief Deletes all stored WorldProxy instances of the specified type(s).
        /// @param Types A bitmask of all the WorldProxy types to be destroyed.
        virtual void DestroyAllProxies(const UInt32 Types) = 0;
        /// @brief Deletes all stored WorldProxy instances.
        virtual void DestroyAllProxies() = 0;
    };//WorldProxyManager
}//Mezzanine

#endif
