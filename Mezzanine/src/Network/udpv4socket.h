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

#ifndef _networkudpv4socket_h
#define _networkudpv4socket_h

#include "Network/udpsocket.h"
#include "Network/ipv4socket.h"

namespace Mezzanine
{
    namespace Network
    {
        ///////////////////////////////////////////////////////////////////////////////
        /// @brief A UDP socket that uses IPv4 internet addresses for transmissions.
        ///////////////////////////////////////
        class MEZZ_LIB UDPv4Socket : public IPv4Socket, public UDPSocket
        {
        protected:
        public:
            /// @brief Class constructor.
            UDPv4Socket();
            /// @brief Internal constructor.
            /// @param A pointer to the internal socket to wrap.
            UDPv4Socket(PlatformSocket* InternalSock);
            /// @brief Binding constructor.
            /// @param Port The port number to bind this socket to.
            UDPv4Socket(const UInt16 Port);
            /// @brief Class destructor.
            virtual ~UDPv4Socket();

            ///////////////////////////////////////////////////////////////////////////////
            // Configuration

            // Commented out due to lack of support on MacOSX.  Additionally there may be better ways to moderate transmissions.
            /*/// @brief Sets whether or not messages sent by this socket can be fragmented by routers.
            /// @remarks In a situation where message fragmentation would occur but isn't allowed to because of this setting
            /// the router will discard the message and send an error back to the sending host.
            /// @param DontFrag Whether or not messages from this socket can be fragmented.
            void SetDontFragment(const Boole DontFrag);
            /// @brief Gets whether or not messages sent by this socket can be fragmented by routers.
            /// @return Returns true if messages sent by this socket can be fragmented, false otherwise.
            Boole GetDontFragment() const;// */

            /// @copydoc UDPSocket::SetNumMulticastHops(const Integer)
            void SetNumMulticastHops(const Integer Hops);
            /// @copydoc UDPSocket::GetNumMulticastHops() const
            Integer GetNumMulticastHops() const;
            /// @copydoc UDPSocket::SetMulticastLoop(const Boole)
            void SetMulticastLoop(const Boole Loop);
            /// @copydoc UDPSocket::GetMulticastLoop() const
            Boole GetMulticastLoop() const;
        };//UDPv4Socket
    }//Network
}//Mezzanine

#endif
