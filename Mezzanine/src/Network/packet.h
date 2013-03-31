//© Copyright 2010 - 2013 BlackTopp Studios Inc.
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

#ifndef _networkpacket_h
#define _networkpacket_h

#include "datatypes.h"

namespace Mezzanine
{
    namespace Network
    {
        ///////////////////////////////////////////////////////////////////////////////
        /// @class Packet
        /// @headerfile networkpacket.h
        /// @brief SFML style packet for encapsulating data for transmission across a network.
        /// @details
        ///////////////////////////////////////
        class MEZZ_LIB Packet
        {
            protected:
                std::vector<char> RawData;
                size_t ReadPos;
                /// @internal
                /// @brief Verifies the remaining length of the packet for extraction.
                bool VerifySize(const size_t& Bytes) const;
            public:
                /// @brief Class constructor.
                Packet();
                /// @brief Class destructor.
                virtual ~Packet();
                /// @brief Adds data to this packet.
                /// @param Data Pointer to an array of raw data.
                /// @param Bytes The amount of bytes contained in Data.
                /// @warning This will blindly append data without checks as to what it is.  Nor
                /// will this check for endianess.  Use with caution.
                virtual void Append(const void* Data, size_t Bytes);
                /// @brief Clears this packet of all data.
                virtual void Clear();
                /// @brief Gets the size of this packet.
                /// @remarks This is also the size of the Data vector being stored by this class.
                /// @return Returns a size_t indicating the size of this packet in bytes.
                virtual size_t GetPacketSize() const;
                /// @brief Gets whether or not the reading of this packet has reached the end.
                /// @return Returns a bool indicating if the read position has reached the end.
                virtual bool EndOfPacket() const;
                /// @brief Gets the vector storing the raw data contained in this packet.
                /// @return Returns a const reference to the vector storing the data of this packet.
                virtual const std::vector<char>& GetRawData() const;
                //streaming operators
                /// @brief
                /// @param Data
                /// @return Returns a reference to this packet.
                Packet& operator >>(bool& Data);
                /// @brief
                /// @param Data
                /// @return Returns a reference to this packet.
                Packet& operator >>(Int8& Data);
                /// @brief
                /// @param Data
                /// @return Returns a reference to this packet.
                Packet& operator >>(UInt8& Data);
                /// @brief
                /// @param Data
                /// @return Returns a reference to this packet.
                Packet& operator >>(Int16& Data);
                /// @brief
                /// @param Data
                /// @return Returns a reference to this packet.
                Packet& operator >>(UInt16& Data);
                /// @brief
                /// @param Data
                /// @return Returns a reference to this packet.
                Packet& operator >>(Int32& Data);
                /// @brief
                /// @param Data
                /// @return Returns a reference to this packet.
                Packet& operator >>(UInt32& Data);
                /// @brief
                /// @param Data
                /// @return Returns a reference to this packet.
                Packet& operator >>(float& Data);
                /// @brief
                /// @param Data
                /// @return Returns a reference to this packet.
                Packet& operator >>(double& Data);
                /// @brief
                /// @param Data
                /// @return Returns a reference to this packet.
                /// @warning To assure proper array size, this function will create a char array of it's own.
                /// After this function is complete, it is up to the user to clean up the array when they are done.
                Packet& operator >>(char* Data);
                /// @brief
                /// @param Data
                /// @return Returns a reference to this packet.
                Packet& operator >>(String& Data);
                /// @brief
                /// @param Data
                /// @return Returns a reference to this packet.
                /// @warning To assure proper array size, this function will create a char array of it's own.
                /// After this function is complete, it is up to the user to clean up the array when they are done.
                Packet& operator >>(wchar_t* Data);
                /// @brief
                /// @param Data
                /// @return Returns a reference to this packet.
                Packet& operator >>(WideString& Data);
                /*/// @brief Streams a 16-bit unicode string out of the packet.
                /// @param Data The string to be streamed to.
                /// @return Returns a reference to this packet.
                Packet& operator >>(const UTF16String& Data);
                /// @brief Streams a 32-bit unicode string out of the packet.
                /// @param Data The string to be streamed to.
                /// @return Returns a reference to this packet.
                Packet& operator >>(const UTF32String& Data);*/
                /// @brief Streams a bool into the packet.
                /// @param Data The bool to be streamed.
                /// @return Returns a reference to this packet.
                Packet& operator <<(const bool Data);
                /// @brief Streams a 8-bit int into the packet.
                /// @param Data The 8-bit int to be streamed.
                /// @return Returns a reference to this packet.
                Packet& operator <<(const Int8 Data);
                /// @brief Streams a 8-bit unsigned int into the packet.
                /// @param Data The 8-bit unsigned int to be streamed.
                /// @return Returns a reference to this packet.
                Packet& operator <<(const UInt8 Data);
                /// @brief Streams a 16-bit int into the packet.
                /// @param Data The 16-bit int to be streamed.
                /// @return Returns a reference to this packet.
                Packet& operator <<(Int16 Data);
                /// @brief Streams a 16-bit unsigned int into the packet.
                /// @param Data The 16-bit unsigned int to be streamed.
                /// @return Returns a reference to this packet.
                Packet& operator <<(UInt16 Data);
                /// @brief Streams a 32-bit int into the packet.
                /// @param Data The 32-bit int to be streamed.
                /// @return Returns a reference to this packet.
                Packet& operator <<(Int32 Data);
                /// @brief Streams a 32-bit unsigned int into the packet.
                /// @param Data The 32-bit unsigned int to be streamed.
                /// @return Returns a reference to this packet.
                Packet& operator <<(UInt32 Data);
                /// @brief Streams a float into the packet.
                /// @param Data The float to be streamed.
                /// @return Returns a reference to this packet.
                Packet& operator <<(const float Data);
                /// @brief Streams a double into the packet.
                /// @param Data The double-precision float to be streamed.
                /// @return Returns a reference to this packet.
                Packet& operator <<(const double Data);
                /// @brief Streams a character array into the packet.
                /// @param Data The char array to be streamed.
                /// @return Returns a reference to this packet.
                Packet& operator <<(const char* Data);
                /// @brief Streams a String into the packet.
                /// @param Data The string to be streamed.
                /// @return Returns a reference to this packet.
                Packet& operator <<(const String& Data);
                /// @brief Streams a wide character array into the packet.
                /// @param Data The char array to be streamed.
                /// @return Returns a reference to this packet.
                Packet& operator <<(const wchar_t* Data);
                /// @brief Streams a wide String into the packet.
                /// @param Data The string to be streamed.
                /// @return Returns a reference to this packet.
                Packet& operator <<(const WideString& Data);
                /*/// @brief Streams a 16-bit unicode string into the packet.
                /// @param Data The string to be streamed.
                /// @return Returns a reference to this packet.
                Packet& operator <<(const UTF16String& Data);
                /// @brief Streams a 32-bit unicode string into the packet.
                /// @param Data The string to be streamed.
                /// @return Returns a reference to this packet.
                Packet& operator <<(const UTF32String& Data);*/
        };//Packet
    }//Network
}//Mezzanine

#endif

#endif //MEZZNETWORK
