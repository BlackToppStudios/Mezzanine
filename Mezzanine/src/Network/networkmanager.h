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

#ifndef _networkmanager_h
#define _networkmanager_h

#include "entresolmanager.h"
#include "entresolmanagerfactory.h"
#include "singleton.h"

namespace Mezzanine
{
    namespace Network
    {
        class Socket;
        ///////////////////////////////////////////////////////////////////////////////
        /// @brief This is the central management class for all network I/O operations.
        ///////////////////////////////////////
        class MEZZ_LIB NetworkManager : public EntresolManager, public Singleton<NetworkManager>
        {
        public:
            /// @brief A String containing the name of this manager implementation.
            static const String ImplementationName;
            /// @brief A ManagerType enum value used to describe the type of interface/functionality this manager provides.
            static const ManagerBase::ManagerType InterfaceType;
        protected:
            //std::map<unsigned short int,Socket*> Sockets;
        public:
            /// @brief Class constructor.
            NetworkManager();
            /// @brief XML constructor.
            /// @param XMLNode The node of the xml document to construct from.
            NetworkManager(const XML::Node& XMLNode);
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
        /// @brief A factory responsible for the creation and destruction of the default networkmanager.
        ///////////////////////////////////////
        class MEZZ_LIB DefaultNetworkManagerFactory : public EntresolManagerFactory
        {
        public:
            /// @brief Class constructor.
            DefaultNetworkManagerFactory();
            /// @brief Class destructor.
            virtual ~DefaultNetworkManagerFactory();

            /// @copydoc ManagerFactory::GetManagerImplName()
            String GetManagerImplName() const;
            /// @copydoc ManagerFactory::GetManagerType() const
            ManagerBase::ManagerType GetManagerType() const;

            /// @copydoc EntresolManagerFactory::CreateManager(const NameValuePairList&)
            EntresolManager* CreateManager(const NameValuePairList& Params);
            /// @copydoc EntresolManagerFactory::CreateManager(const XML::Node&)
            EntresolManager* CreateManager(const XML::Node& XMLNode);
            /// @copydoc EntresolManagerFactory::DestroyManager(EntresolManager*)
            void DestroyManager(EntresolManager* ToBeDestroyed);
        };//DefaultNetworkManagerFactory
    }//Network
}//Mezzanine

#endif
