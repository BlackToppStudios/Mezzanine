// © Copyright 2010 - 2016 BlackTopp Studios Inc.
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

#ifndef _networkipaddress_h
#define _networkipaddress_h

#include "datatypes.h"

#include "Network/networkenumerations.h"

#include "XML/xml.h"

namespace Mezzanine
{
    namespace Network
    {
        ///////////////////////////////////////////////////////////////////////////////
        /// @class IPAddress
        /// @brief This is a simple class for representing IP addresses used throughout the Network subsystem.
        ///////////////////////////////////////
        class MEZZ_LIB IPAddress
        {
        public:
            /// @brief Convenience type to store the binary representation of IP addresses.
            typedef std::vector<Int8> AddressContainer;

            /// @brief The amount of space needed in a buffer to store the binary representation of an IPv4 address.
            static const UInt16 IPv4BinaryLength;
            /// @brief The amount of space needed in a buffer to store the binary representation of an IPv6 address.
            static const UInt16 IPv6BinaryLength;
            /// @brief The amount of space needed in a string to store the character representation of an IPv4 address.
            static const UInt16 IPv4StringLength;
            /// @brief The amount of space needed in a string to store the character representation of an IPv6 address.
            static const UInt16 IPv6StringLength;

            /// @brief The loopback address for IPv4.
            static const String IPv4LoopbackAddress;
            /// @brief The broadcast address for IPv4.
            static const String IPv4BroadcastAddress;
            /// @brief The loopback address for IPv6.
            static const String IPv6LoopbackAddress;
        protected:
            /// @internal
            /// @brief A resizable vector to store the binary IP address regardless of version.
            AddressContainer InternalAddress;
        public:
            /// @brief Class Constructor.
            IPAddress();
            /// @brief IPv4 Integer Constructor.
            /// @param Address The integer address to be set.
            /// @param NBO Whether or not the passed in v4 address is already in Network-Byte-Order.
            IPAddress(const UInt32 Address, const Boole NBO = false);
            /// @brief IP Agnostic String Constructor.
            /// @param Address The string storing the address to use.
            IPAddress(const String& Address);
            /// @brief IP Agnostic Binary Constructor.
            /// @param Address The binary address to use.
            IPAddress(const AddressContainer& Address);
            /// @brief Copy Constructor.
            /// @param Other The other IPAddress to be copied.
            IPAddress(const IPAddress& Other);
            /// @brief Class Destructor.
            ~IPAddress();

            ///////////////////////////////////////////////////////////////////////////////
            // Static Helpers

            /// @brief Resolves an IPv4 or IPv6 address as a String to it's binary representation.
            /// @remarks There are some sane limitations in how this function can be used/interpreted.  The format for IP addresses that
            /// are acceptable is "XXX.XXX.XXX.XXX" for IPv4, or "XXXX:XXXX:XXXX:XXXX::XXXX" (as an example) for IPv6.  Other formats or
            /// including a port number will cause this to fail.  The returned container should always be of size 0, 4, or 16.  A size of
            /// zero indicates a failure, whereas a size of 4 is an IPv4 address, and a size of 16 is an IPv6 address. @n @n
            /// This method will NOT perform DNS resolutions.  Only numeric representations.
            /// @param Address The address to be resolved to binary.
            /// @return Returns a container storing the binary representation of the address provided.
            static AddressContainer Resolve(const String& Address);
            /// @brief Resolves an IPv4 or IPv6 address in it's binary representation to a human readable String.
            /// @remarks The returned string is straightforward for IPv6, in typical "XXX.XXX.XXX.XXX" notation.  For IPv6 addresses this
            /// function will use the shortest notation that can be expressed with the address, and all letters will be in lower case.
            /// @param Address The address to be resolved to a String.
            /// @return Returns a String containing a human readable IP address, or an empty String if there was a failure.
            static String Resolve(const AddressContainer& Address);

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            /// @brief Gets the type of IP address this is.
            /// @return Returns a NetworkLayerProtocol enum value indicating if this IP address is v4, v6 or invalid.
            Network::NetworkLayerProtocol GetProtocol() const;
            /// @brief Gets this IPAddress in a human readable format.
            /// @return Returns a String containing a human friendly form of this IPAddress.
            String GetAsString() const;

            /// @brief Sets the binary form of the address being stored.
            /// @remarks Only use this if you KNOW your formatting is correct or are returning a container generated by IPAddress::Resolve.
            /// @param Address The binary address to be set.
            void SetBinaryAddress(const AddressContainer& Address);
            /// @brief Gets the stored binary address of this IPAddress.
            /// @return Returns a const AddressContainer reference that is storing the binary address.
            const AddressContainer& GetBinaryAddress() const;

            /// @brief Sets a version 4 IP address value.
            /// @warning This method will allocate for an IPv4 address regardless of it's current contents and assign it's value.
            /// @param Address The v4 IP Address to set.
            /// @param NBO Whether or not the passed in v4 address is already in Network-Byte-Order.
            void SetV4Address(const UInt32 Address, const Boole NBO = false);
            /// @brief Gets a version 4 IP address value.
            /// @param NBO Whether or not the returned v4 address should be returned in Network-Byte-Order.
            /// @return Returns a UInt32 containing the full IPv4 address currently set, or 0 if no IPAddress is allocated or if allocated for an IPv6 address.
            UInt32 GetV4Address(const Boole NBO = false) const;

            ///////////////////////////////////////////////////////////////////////////////
            // Operators

            /// @brief Assignment operator.
            /// @param Other The other IPAddress to be copied from.
            void operator=(const IPAddress& Other);

            /// @brief Equality operator.
            /// @param Other The other IPAddress to compare to.
            /// @return Returns true if the two IPAddresses are identical, false otherwise.
            Boole operator==(const IPAddress& Other) const;
            /// @brief Inequality operator.
            /// @param Other The other IPAddress to compare to.
            /// @return Returns false if the two IPAddresses are identical, true otherwise.
            Boole operator!=(const IPAddress& Other) const;

            ///////////////////////////////////////////////////////////////////////////////
            // Serialization

            /// @brief Convert this class to an XML::Node ready for serialization.
            /// @param ParentNode The point in the XML hierarchy that this IPAddress should be appended to.
            void ProtoSerialize(XML::Node& ParentNode) const;
            /// @brief Take the data stored in an XML Node and overwrite this object with it.
            /// @param SelfRoot An XML::Node containing the data to populate this class with.
            void ProtoDeSerialize(const XML::Node& SelfRoot);

            /// @brief Get the name of the the XML tag the IPAddress class will leave behind as its instances are serialized.
            /// @return A string containing the name of this class.
            static String GetSerializableName();
        };//IPAddress
    }//Network
}//Mezzanine

#endif

#endif //MEZZNETWORK
