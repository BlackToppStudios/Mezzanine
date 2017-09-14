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

#ifndef _networkutilities_h
#define _networkutilities_h

#include "Network/socketdescription.h"

struct addrinfo;

namespace Mezzanine
{
    namespace Network
    {
        ///////////////////////////////////////////////////////////////////////////////
        // Byte Order Conversion

        /// @brief Converts a UInt16 from the host byte order to network byte order.
        /// @param ToConvert The integer to be converted.
        /// @return Returns the converted integer.
        UInt16 MEZZ_LIB ConvertHostToNetworkByteOrder(const UInt16 ToConvert);
        /// @brief Converts a UInt32 from the host byte order to network byte order.
        /// @param ToConvert The integer to be converted.
        /// @return Returns the converted integer.
        UInt32 MEZZ_LIB ConvertHostToNetworkByteOrder(const UInt32 ToConvert);
        /// @brief Converts a UInt16 from the network byte order to host byte order.
        /// @param ToConvert The integer to be converted.
        /// @return Returns the converted integer.
        UInt16 MEZZ_LIB ConvertNetworkToHostByteOrder(const UInt16 ToConvert);
        /// @brief Converts a UInt32 from the network byte order to host byte order.
        /// @param ToConvert The integer to be converted.
        /// @return Returns the converted integer.
        UInt32 MEZZ_LIB ConvertNetworkToHostByteOrder(const UInt32 ToConvert);

        ///////////////////////////////////////////////////////////////////////////////
        // DNS Lookup

        ///////////////////////////////////////////////////////////////////////////////
        /// @brief A special container class used to store the results from a call to "ResolveDomainName".
        ///////////////////////////////////////
        class MEZZ_LIB SocketResultContainer
        {
        protected:
            /// @internal
            /// @brief The count of instances that exist for these results.
            Integer* RefCount;
            /// @internal
            /// @brief A pointer to the internal structure holding the resulting addresses.
            addrinfo* Addresses;
            /// @internal
            /// @brief A pointer to the current address being presented.
            addrinfo* CurrAddress;
            /// @internal
            /// @brief The converted values for the current address being presented.
            SocketDescription CurrDescription;

            /// @internal
            /// @brief Duplicates the necessary data when creating a copy instance of a set of socket results.
            /// @param Other The other SocketResultContainer being duplicated.
            void Aquire(const SocketResultContainer& Other);
            /// @internal
            /// @brief Decrements the ref count and destroys all internal data if necessary.
            void Release();
        public:
            /// @brief Internal constructor.
            /// @param Results A pointer to the internal address result structures.
            SocketResultContainer(addrinfo* Results);
            /// @brief Copy constructor.
            /// @param Other The other SocketResultContainer to be constructed.
            SocketResultContainer(const SocketResultContainer& Other);
            /// @brief Class destructor.
            ~SocketResultContainer();

            ///////////////////////////////////////////////////////////////////////////////
            // Element Access

            /// @brief Gets/converts the current result and increments the cursor to the next result if able.
            /// @warning Do not delete the pointer returned by this method, it's pointing to a data member of this class and will cause you to have a bad time.
            /// @return Returns a const pointer to the currently pointed to element, or NULL if the end of the container has been reached.
            const SocketDescription* GetNextResult();
            /// @brief Resets the current elements pointed to back to the beginning.
            void Reset();
        };//SocketResultContainer

        /// @brief Performs a DNS lookup with the provided domain name and port.
        /// @param Address The numeric address of the target system.
        /// @param Port The port on which the domain name is accepting connections.
        /// @param TLP The transport layer protocol the target system is using to accept connections.
        /// @return Returns a SocketResultContainer containing all matching valid socket configurations.
        SocketResultContainer MEZZ_LIB ResolveNumericHost(const IPAddress& Address, const UInt16 Port = 0, const TransportLayerProtocol TLP = Network::TLP_Invalid);
        /// @brief Performs a DNS lookup with the provided domain name and service.
        /// @remarks Examples of valid service identifiers that can be passed in here can be found at:
        /// http://www.iana.org/assignments/service-names-port-numbers/service-names-port-numbers.xhtml
        /// @param DomainName The domain name to lookup, must not have a trailing slash, whitespace, or service identifier.  For example "www.google.com" is valid.
        /// @param Port The port on which the domain name is operating.
        /// @param NLP The IP version to use when connecting to the target system.
        /// @param TLP The transport layer protocol the target system is using to accept connections.
        /// @return Returns a SocketResultContainer containing all matching valid socket configurations.
        SocketResultContainer MEZZ_LIB ResolveDomainName(const String& DomainName, const UInt16 Port = 0, const NetworkLayerProtocol NLP = Network::NLP_Invalid, const TransportLayerProtocol TLP = Network::TLP_Invalid);
    }//Network
}//Mezzanine

#endif
