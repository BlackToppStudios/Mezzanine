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

#ifndef _networkipv6socket_h
#define _networkipv6socket_h

#include "Network/socket.h"

namespace Mezzanine
{
    namespace Network
    {
        ///////////////////////////////////////////////////////////////////////////////
        /// @brief A socket class using the IP version 6 protocol.
        ///////////////////////////////////////
        class MEZZ_LIB IPv6Socket : virtual public Socket
        {
        protected:
        public:
            /// @brief Class constructor.
            IPv6Socket();
            /// @brief Class destructor.
            virtual ~IPv6Socket();

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            /// @copydoc Socket::GetNetworkLayerProtocol() const
            virtual NetworkLayerProtocol GetNetworkLayerProtocol() const;

            ///////////////////////////////////////////////////////////////////////////////
            // Configuration

            /// @copydoc Socket::SetNumUnicastHops(const Integer)
            void SetNumUnicastHops(const Integer Hops);
            /// @copydoc Socket::GetNumUnicastHops() const
            Integer GetNumUnicastHops() const;
            /// @brief Sets whether this socket is allowed to send or receive IPv4 messages.
            /// @remarks By default IPv6 supports IPv4 addresses mapped onto it, and thus you can communicate with IPv4 hosts even with an IPv6 socket.  This
            /// option forces IPv6 traffic on the socket.  Doing so also allows you to reuse the port the socket is operating on if it is reused by an IPv4
            /// socket.
            /// @param V6Only Whether or not to permit IPv4 addresses on send and/or accept IPv4 messages from remote hosts.
            void SetV6Only(const Boole V6Only);
            /// @brief Gets whether this socket is allowed to send or receive IPv4 messages.
            /// @return Returns true if this socket is only allowed to send and receive IPv6 message, false if it will accept either IPv4 or IPv6.
            Boole GetV6Only() const;
        };//IPv6Socket
    }//Network
}//Mezzanine

#endif
