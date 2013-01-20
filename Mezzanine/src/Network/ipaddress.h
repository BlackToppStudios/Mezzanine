//© Copyright 2010 - 2012 BlackTopp Studios Inc.
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

namespace Mezzanine
{
    namespace Network
    {
        ///////////////////////////////////////////////////////////////////////////////
        /// @class IPAddress
        /// @headerfile networkipaddress.h
        /// @brief
        /// @details
        ///////////////////////////////////////
        class MEZZ_LIB IPAddress
        {
            public:
                /// @enum IPVersion
                /// @brief Specifies the version of IP this class encapsulates.
                enum IPVersion
                {
                    IP_v4,
                    IP_v6
                };
            protected:
                String HostName;
                //String SocAddress;
                virtual bool VerifyAddress(const String& Addr) = 0;
            public:
                /// @brief Class Constructor.
                IPAddress();
                /// @brief Class Destructor.
                virtual ~IPAddress();
                /// @brief Sets the address via string.
                /// @param Address A string containing the address to be set.
                virtual void SetAddress(const String& Address);
                /// @brief Gets the address as a string.
                /// @return Returns a string containing the address.
                virtual String GetAddressAsString() const;
                /// @brief Gets the address as a c-style string.
                /// @return Returns a C string containing the address.
                virtual char* GetAddressAsCString() const;
                /// @brief Gets the hostname of this address if one was set.
                /// @remarks This class does not do DNS lookup's on it's own.  If you want that then
                /// look to utilities on the NetworkManager.  If those utilities were used then the
                /// original hostname that was used to search with will be set, and you can retrieve it
                /// using this function.
                /// @return Returns a String containing the hostname associated with this address, or an empty string if one wasn't set.
                virtual const String& GetHostName() const;
                /// @brief Gets which version of IP this class is encapsulating.
                /// @return Returns an IPVersion enum value indicating which version of IP this class is using.
                virtual IPAddress::IPVersion GetVersion() const = 0;
        };//IPAddress

        ///////////////////////////////////////////////////////////////////////////////
        /// @class IPAddress_4
        /// @headerfile networkipaddress.h
        /// @brief
        /// @details
        ///////////////////////////////////////
        class MEZZ_LIB IPAddress_4 : public IPAddress
        {
            protected:
                //UInt32 SocAddress;
                virtual bool VerifyAddress(const String& Addr);
            public:
                /// @brief Class Constructor.
                IPAddress_4();
                /// @brief Class Destructor.
                virtual ~IPAddress_4();
                /*/// @brief Sets the Address via a 32-bit int.
                /// @param Address A 32-bit unsigned int representing the IPv4 address.
                virtual void SetAddress(const UInt32& Address);// */
                /// @copydoc IPAddress::GetVersion()
                virtual IPAddress::IPVersion GetVersion() const;
        };//IPAddress_4

        ///////////////////////////////////////////////////////////////////////////////
        /// @class IPAddress_6
        /// @headerfile networkipaddress.h
        /// @brief
        /// @details
        ///////////////////////////////////////
        class MEZZ_LIB IPAddress_6 : public IPAddress
        {
            protected:
                //UInt8 SocAddress[16];
                virtual bool VerifyAddress(const String& Addr);
            public:
                /// @brief Class Constructor.
                IPAddress_6();
                /// @brief Class Destructor.
                virtual ~IPAddress_6();
                /// @copydoc IPAddress::GetVersion()
                virtual IPAddress::IPVersion GetVersion() const;
        };//IPAddress_6
    }//Network
}//Mezzanine

#endif

#endif //MEZZNETWORK
