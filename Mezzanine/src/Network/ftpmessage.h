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

#ifndef _networkftpmessage_h
#define _networkftpmessage_h

#include "Network/networkenumerations.h"
#include "Network/systemaddress.h"

namespace Mezzanine
{
    namespace Network
    {
        ///////////////////////////////////////////////////////////////////////////////
        /// @brief A base class for text based messages exchanged between FTP clients and servers.
        ///////////////////////////////////////
        class MEZZ_LIB FTPMessage
        {
        public:
            /// @brief Convenience typedef for String iterators.
            typedef String::const_iterator                StringIterator;
        protected:
            /// @internal
            /// @brief Checks to see if a parsing iterator is at a Telnet End-Of-Line.
            /// @param CurrIt The iterator to be checked.
            /// @return Returns true if the cursor is currently at the end of line, false otherwise.
            Boole IsEndOfLine(const StringIterator CurrIt) const;
        public:
            ///////////////////////////////////////////////////////////////////////////////
            // Convenience Conversion Methods

            /// @brief Converts a SystemAddress into a String with standard FTP notation.
            /// @remarks Standard FTP notation involves converting each byte to it's decimal form and then placing them into a String with each byte separated by commas. @n
            /// The format looks like: @n
            /// "a1,a2,a3,a4,p1,p2" @n
            /// The primary use of this function is to facilitate use of the PASV and PORT commands.
            /// @param ToConvert The SystemAddress to be converted.
            /// @return Returns a String containing the converted address.
            static String ConvertToAddress(const SystemAddress& ToConvert);
            /// @brief Converts a String expressing an IP address in standard FTP notation into a SystemAddress.
            /// @param ToConvert The String containing the system address to be converted.
            /// @return Returns a SystemAddress containing information from the converted String, which may be invalid if the String could not be parsed.
            static SystemAddress ConvertFromAddress(const String& ToConvert);
            /// @brief Converts a String expressing an IP address in standard FTP notation into a SystemAddress.
            /// @param CurrIt An iterator at the start of the range to be parsed.
            /// @param EndIt An iterator at the end of the range to be parsed.  Parsing may or may not reach this point.
            /// @return Returns a SystemAddress containing information from the converted String, which may be invalid if the String could not be parsed.
            static SystemAddress ConvertFromAddress(StringIterator& CurrIt, const StringIterator EndIt);
            /// @brief Converts a SystemAddress into a String with extended FTP notation.
            /// @remarks Standard FTP notation comes with a limitation, it can only represent IP addresses 4 bytes long.  So IPv4.  If you want to connect to a FTP server that has
            /// an IPv6 address you need to use the extended notation.  Instead of representing individual bytes, it has a more friendly use of Strings delimited by pipes/bars. @n
            /// The format looks like: @n
            /// "|[AF-Num]|[Str-Addr]|[Port]|" @n
            /// AF-Num is a number for the Address Family assigned by the IANA.  In almost all cases it'll be either 1(IPv4) or 2(IPv6) in practice. @n
            /// Str-Addr is just the String representation of the address to connect to. @n
            /// Port is just a String representation of the port to connect to. @n
            /// The primary use of this function is to facilitate use of the EPSV and EPRT commands.
            /// @param ToConvert The SystemAddress to be converted.
            /// @return Returns a String containing the converted address.
            static String ConvertToExtendedAddress(const SystemAddress& ToConvert);
            /// @brief Converts a String expressing an IP address in extended FTP notation into a SystemAddress.
            /// @param ToConvert The String containing the system address to be converted.
            /// @return Returns a SystemAddress containing information from the converted String, which may be invalid if the String could not be parsed.
            static SystemAddress ConvertFromExtendedAddress(const String& ToConvert);
            /// @brief Converts a String expressing an IP address in extended FTP notation into a SystemAddress.
            /// @param CurrIt An iterator at the start of the range to be parsed.
            /// @param EndIt An iterator at the end of the range to be parsed.  Parsing may or may not reach this point.
            /// @return Returns a SystemAddress containing information from the converted String, which may be invalid if the String could not be parsed.
            static SystemAddress ConvertFromExtendedAddress(StringIterator& CurrIt, const StringIterator EndIt);

            ///////////////////////////////////////////////////////////////////////////////
            // Core Operations

            /// @brief Creates a completed message that can be sent across an FTP connection.
            /// @remarks The String generated includes the line ending CLRF required by the spec.
            /// @return Returns a String containing the generated message.
            virtual String Compose() const = 0;
            /// @brief Populates the members of this class with data from a text FTP message.
            /// @param Message The String containing the text FTP message to be decomposed.
            /// @return Returns true if the String was successfully decomposed and the terminating CRLF was detected.
            virtual Boole Decompose(const String& Message) = 0;
            /// @brief Populates the members of this class with data from a text FTP message.
            /// @param CurrIt An iterator at the start of the range to be parsed.
            /// @param EndIt An iterator at the end of the range to be parsed.  Parsing may or may not reach this point.
            /// @return Returns true if the String was successfully decomposed and the terminating CRLF was detected.
            virtual Boole Decompose(StringIterator& CurrIt, const StringIterator EndIt) = 0;
        };//FTPMessage
    }//Network
}//Mezzanine

#endif
