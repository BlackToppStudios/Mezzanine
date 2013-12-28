// © Copyright 2010 - 2013 BlackTopp Studios Inc.
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

#ifdef MEZZNETWORK

#ifndef _networkmanager_h
#define _networkmanager_h

#include "managerbase.h"
#include "managerfactory.h"
#include "singleton.h"

namespace Mezzanine
{
    class Socket;
    ///////////////////////////////////////////////////////////////////////////////
    /// @class NetworkManager
    /// @headerfile networkmanager.h
    /// @brief This is the central management class for all network I/O operations.
    /// @details
    ///////////////////////////////////////
    class MEZZ_LIB NetworkManager : public ManagerBase, public Singleton<NetworkManager>
    {
    protected:
        //std::map<unsigned short int,Socket*> Sockets;
    public:
        /// @brief Class constructor.
        NetworkManager();
        /// @brief XML constructor.
        /// @param XMLNode The node of the xml document to construct from.
        NetworkManager(XML::Node& XMLNode);
        /// @brief Class destructor.
        virtual ~NetworkManager();

        ///////////////////////////////////////////////////////////////////////////////
        // Sockets Management

        ///////////////////////////////////////////////////////////////////////////////
        // Network Utilities

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        /// @copydoc ManagerBase::Initialize()
        virtual void Initialize();
        /// @copydoc ManagerBase::Deinitialize()
        virtual void Deinitialize();

        ///////////////////////////////////////////////////////////////////////////////
        // Type Identifier Methods

        /// @copydoc ManagerBase::GetInterfaceType()
        virtual ManagerType GetInterfaceType() const;
        /// @copydoc ManagerBase::GetImplementationTypeName()
        virtual String GetImplementationTypeName() const;
    };//NetworkManager

    ///////////////////////////////////////////////////////////////////////////////
    /// @class DefaultNetworkManagerFactory
    /// @headerfile networkmanager.h
    /// @brief A factory responsible for the creation and destruction of the default networkmanager.
    ///////////////////////////////////////
    class MEZZ_LIB DefaultNetworkManagerFactory : public ManagerFactory
    {
    public:
        /// @brief Class constructor.
        DefaultNetworkManagerFactory();
        /// @brief Class destructor.
        virtual ~DefaultNetworkManagerFactory();

        /// @copydoc ManagerFactory::GetManagerTypeName()
        String GetManagerTypeName() const;

        /// @copydoc ManagerFactory::CreateManager(NameValuePairList&)
        ManagerBase* CreateManager(NameValuePairList& Params);
        /// @copydoc ManagerFactory::CreateManager(XML::Node&)
        ManagerBase* CreateManager(XML::Node& XMLNode);
        /// @copydoc ManagerFactory::DestroyManager(ManagerBase*)
        void DestroyManager(ManagerBase* ToBeDestroyed);
    };//DefaultNetworkManagerFactory
}//Mezzanine

#endif

#endif //MEZZNETWORK
