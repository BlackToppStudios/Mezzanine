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

#ifdef MEZZNETWORK

#ifndef _networktcpsocket_h
#define _networktcpsocket_h

#include "Network/socket.h"

namespace Mezzanine
{
    namespace Network
    {
        ///////////////////////////////////////////////////////////////////////////////
        /// @class TCPSocket
        /// @brief A socket class using the TCP stream protocol.
        /// @details
        ///////////////////////////////////////
        class MEZZ_LIB TCPSocket : virtual public Socket
        {
        protected:
        public:
            /// @brief Class constructor.
            TCPSocket();
            /// @brief Class destructor.
            virtual ~TCPSocket();

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            /// @copydoc Socket::GetTransportLayerProtocol() const
            TransportLayerProtocol GetTransportLayerProtocol() const;

            ///////////////////////////////////////////////////////////////////////////////
            // Core Operations

            /// @brief Prepares this socket for incoming connections.
            /// @param Backlog The amount of pending connections to keep track of in the queue before rejecting other connections.
            /// @return Returns true if this put the socket into a listening state, false if there was a failure.
            Boole Listen(const Integer Backlog = 5);
            /// @brief Accepts an incoming connection from a remote host.
            /// @param Address The address structure to be populated with connection data to the Accepted remote host.
            /// @return Returns a pointer to a new socket specifically configured to talk to the Accepted remote host, or NULL if there was a failure.
            TCPSocket* Accept(SystemAddress& Address);

            ///////////////////////////////////////////////////////////////////////////////
            // Configuration

            /// @brief Sets whether or not a connection on this socket should use periodic packets during inactivity to ensure the connection remains valid.
            /// @param Alive Whether or not the connection this socket has should be kept active.
            void SetKeepAlive(const Boole Alive);
            /// @brief Gets whether or not a connection on this socket should use periodic packets during inactivity to ensure the connection remains valid.
            /// @return Returns true if connections should send periodic packets during periods of inactivity, false otherwise.
            Boole GetKeepAlive() const;
            /// @brief If enabled, the Nagle algorithm is disabled and TCP segments will be sent immediately.
            /// @remarks In most cases you won't want to disable the delay.  TCP is smart, let it do it's job.  Overutilization of the network can create
            /// it's own problems and that can easily be the case by using this option.
            /// @param NoDelay If true, transmissions will be sent ASAP regardless of transmission size.
            void SetNoDelay(const Boole NoDelay);
            /// @brief Gets whether or not the Nagle algorithm is disabled.
            /// @return Returns true if transmissions aren't delayed via the Nagle algorithm, false otherwise.
            Boole GetNoDelay() const;
        };//TCPSocket
    }//Network
}//Mezzanine

#endif

#endif //MEZZNETWORK
