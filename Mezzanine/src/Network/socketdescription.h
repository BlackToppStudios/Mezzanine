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

#ifndef _networksocketdescription_h
#define _networksocketdescription_h

#include "Network/ipaddress.h"
#include "Network/systemaddress.h"

namespace Mezzanine
{
    namespace Network
    {
        ///////////////////////////////////////////////////////////////////////////////
        /// @brief A convenience class storing socket data that can be returned from utility methods and used to create and init new sockets.
        /// @remarks Extreme care should be taken when handling these values.  They are set to Network-Byte-Order and any necessary conversions
        /// to Host-Byte-Order are not automatically done by this struct.
        ///////////////////////////////////////
        struct MEZZ_LIB SocketDescription
        {
            ///////////////////////////////////////////////////////////////////////////////
            // Public Data Members

            /// @brief The IP address
            IPAddress SocketAddress;
            /// @brief An identifier used for routing IPv6 packets.
            UInt32 V6FlowInformation;
            /// @brief An identifier used to describe how accessible the address is to other hosts.
            UInt32 V6ScopeID;
            /// @brief The port on which the socket sends and receives.
            UInt16 SocketPort;
            /// @brief The protocol to used by a given socket.
            TransportLayerProtocol SocketProtocol;

            ///////////////////////////////////////////////////////////////////////////////
            // Construction and Destruction

            /// @brief Class constructor.
            SocketDescription() :
                V6FlowInformation(0),
                V6ScopeID(0),
                SocketPort(0),
                SocketProtocol(Network::TLP_Invalid)
                {  }
            /// @brief Copy constructor.
            SocketDescription(const SocketDescription& Other) :
                SocketAddress(Other.SocketAddress),
                V6FlowInformation(Other.V6FlowInformation),
                V6ScopeID(Other.V6ScopeID),
                SocketPort(Other.SocketPort),
                SocketProtocol(Other.SocketProtocol)
                {  }
            /// @brief Class destructor.
            ~SocketDescription()
                {  }

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            /// @brief Converts the applicable data in this description into a SystemAddress which can be used by other parts of the Network system.
            /// @return Returns a new SystemAddress class containing much of the same information in this SocketDescription.
            SystemAddress ConvertToSystemAddress() const
            {
                SystemAddress Ret(this->SocketAddress,this->SocketPort,true);
                Ret.SetV6Flow(this->V6FlowInformation);
                Ret.SetV6Scope(this->V6ScopeID);
                return Ret;
            }
        };//SocketDescription
    }//Network
}//Mezzanine

#endif
