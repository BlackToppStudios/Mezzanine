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

#ifdef MEZZNETWORK

#ifndef _networksystemaddress_h
#define _networksystemaddress_h

#include "Network/ipaddress.h"

namespace Mezzanine
{
    namespace Network
    {
        ///////////////////////////////////////////////////////////////////////////////
        /// @class SystemAddress
        /// @brief A simple class that stores a complete set of information for establishing connections.
        /// @details
        ///////////////////////////////////////
        class MEZZ_LIB SystemAddress
        {
        protected:
            /// @internal
            /// @brief The IPAddress of the target system.
            IPAddress Address;
            /// @internal
            /// @brief The (optional) IPv6 Flow value, used for routing IPv6 packets.
            UInt32 V6Flow;
            /// @internal
            /// @brief The (optional) IPv6 Scope-ID, used to describe the reach of the address.
            UInt32 V6Scope;
            /// @internal
            /// @brief The open port of the target system.
            UInt16 Port;
        public:
            ///////////////////////////////////////////////////////////////////////////////
            // Construction and Destruction

            /// @brief Class constructor.
            SystemAddress();
            /// @brief IP constructor.
            /// @param TargetAddress The IP address of the target.
            SystemAddress(const IPAddress& TargetAddress);
            /// @brief Port constructor.
            /// @param TargetPort The port of the target in Host-Byte-Order.
            SystemAddress(const UInt16 TargetPort);
            /// @brief Descriptive constructor.
            /// @param TargetAddress The IP address of the target.
            /// @param TargetPort The port of the target in Host-Byte-Order.
            SystemAddress(const IPAddress& TargetAddress, const UInt16 TargetPort);
            /// @brief Copy constructor.
            /// @param Other The other SystemAddress to copy from.
            SystemAddress(const SystemAddress& Other);
            /// @brief Class destructor.
            ~SystemAddress();

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            /// @brief Sets the IP portion of the target system address.
            /// @param TargetAddress The IP address of the target.
            void SetAddress(const IPAddress& TargetAddress);
            /// @brief Gets the IP portion of the target system address.
            /// @return Returns a const reference to an IPAddress of the target system.
            const IPAddress& GetAddress() const;
            /// @brief Gets the IP portion of the target system address.
            /// @return Returns a const reference to an IPAddress of the target system.
            IPAddress& GetAddress();
            /// @brief Sets the flow information to be used.
            /// @param Flow The flow value to use for determining packet routing.
            void SetV6Flow(const UInt32 Flow);
            /// @brief Gets the flow information to be used for routing packets to the target system.
            /// @return Returns a UInt32 representing the flow information for IPv6 transmissions.
            UInt32 GetV6Flow() const;
            /// @brief Sets the scope-id for the address.
            /// @param Scope The scope-id to be used for local IPv6 devices.
            void SetV6Scope(const UInt32 Scope);
            /// @brief Gets the scope-id for the address.
            /// @return Returns a UInt32 representing the scope-id for the IPv6 address.
            UInt32 GetV6Scope() const;
            /// @brief Sets the Port portion of the target system address.
            /// @param TargetPort The port of the target.
            /// @param NBO Whether or not the passed in port number is already in Network-Byte-Order.
            void SetPort(const UInt16 TargetPort, const Boole NBO = false);
            /// @brief Gets the Port portion of the target system address.
            /// @param NBO Whether or not the returned port number should be returned in Network-Byte-Order.
            /// @return Returns a UInt16 storing the port of the target system.
            UInt16 GetPort(const Boole NBO = false) const;

            ///////////////////////////////////////////////////////////////////////////////
            // Operators

            /// @brief Assignment operator.
            /// @param Other The other SystemAddress to be copied from.
            void operator=(const SystemAddress& Other);

            /// @brief Equality operator.
            /// @param Other The other SystemAddress to compare to.
            /// @return Returns true if the two SystemAddress' are identical, false otherwise.
            Boole operator==(const SystemAddress& Other) const;
            /// @brief Inequality operator.
            /// @param Other The other SystemAddress to compare to.
            /// @return Returns false if the two SystemAddress' are identical, true otherwise.
            Boole operator!=(const SystemAddress& Other) const;

            ///////////////////////////////////////////////////////////////////////////////
            // Serialization

            /// @brief Convert this class to an XML::Node ready for serialization.
            /// @param ParentNode The point in the XML hierarchy that this SystemAddress should be appended to.
            void ProtoSerialize(XML::Node& ParentNode) const;
            /// @brief Take the data stored in an XML Node and overwrite this object with it.
            /// @param SelfRoot An XML::Node containing the data to populate this class with.
            void ProtoDeSerialize(const XML::Node& SelfRoot);

            /// @brief Get the name of the the XML tag the SystemAddress class will leave behind as its instances are serialized.
            /// @return A string containing the name of this class.
            static String GetSerializableName();
        };//SystemAddress
    }//Network
}//Mezzanine

#endif

#endif //MEZZNETWORK
