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

#ifndef _networkpacket_cpp
#define _networkpacket_cpp

#include "Network/packet.h"
#include <memory>
#include <cstring>

#ifdef WINDOWS
#include <winsock2.h>
#else
#include <netinet/in.h>
#include <arpa/inet.h>
#endif

namespace Mezzanine
{
    namespace Network
    {
        Packet::Packet()
        {
        }

        Packet::~Packet()
        {
            Clear();
        }

        Boolean Packet::VerifySize(const size_t& Bytes) const
        {
            return RawData.size() >= (ReadPos+Bytes);
        }

        void Packet::Append(const void* Data, size_t Bytes)
        {
            if(!Data || (Bytes <= 0))
                return;
            size_t First = RawData.size();
            RawData.resize(First + Bytes);
            std::memcpy(&RawData[First],Data,Bytes);
        }

        void Packet::Clear()
        {
            RawData.clear();
        }

        size_t Packet::GetPacketSize() const
        {
            return RawData.size();
        }

        Boolean Packet::EndOfPacket() const
        {
            return (ReadPos >= RawData.size());
        }

        const std::vector<char>& Packet::GetRawData() const
        {
            return RawData;
        }

        Packet& Packet::operator >>(Boolean& Data)
        {
            return *this;
        }

        Packet& Packet::operator >>(Int8& Data)
        {
            if(!VerifySize(sizeof(Data)))
                return *this;

            Data = *reinterpret_cast<const Int8*>(RawData[ReadPos]);
            ReadPos+=sizeof(Data);
            return *this;
        }

        Packet& Packet::operator >>(UInt8& Data)
        {
            if(!VerifySize(sizeof(Data)))
                return *this;

            Data = *reinterpret_cast<const UInt8*>(RawData[ReadPos]);
            ReadPos+=sizeof(Data);
            return *this;
        }

        Packet& Packet::operator >>(Int16& Data)
        {
            if(!VerifySize(sizeof(Data)))
                return *this;

            Data = *reinterpret_cast<const Int16*>(RawData[ReadPos]);
            ReadPos+=sizeof(Data);
            return *this;
        }

        Packet& Packet::operator >>(UInt16& Data)
        {
            if(!VerifySize(sizeof(Data)))
                return *this;

            Data = *reinterpret_cast<const UInt16*>(RawData[ReadPos]);
            ReadPos+=sizeof(Data);
            return *this;
        }

        Packet& Packet::operator >>(Int32& Data)
        {
            if(!VerifySize(sizeof(Data)))
                return *this;

            Data = *reinterpret_cast<const Int32*>(RawData[ReadPos]);
            ReadPos+=sizeof(Data);
            return *this;
        }

        Packet& Packet::operator >>(UInt32& Data)
        {
            if(!VerifySize(sizeof(Data)))
                return *this;

            Data = *reinterpret_cast<const UInt32*>(RawData[ReadPos]);
            ReadPos+=sizeof(Data);
            return *this;
        }

        Packet& Packet::operator >>(float& Data)
        {
            if(!VerifySize(sizeof(Data)))
                return *this;

            Data = *reinterpret_cast<const float*>(RawData[ReadPos]);
            ReadPos+=sizeof(Data);
            return *this;
        }

        Packet& Packet::operator >>(double& Data)
        {
            if(!VerifySize(sizeof(Data)))
                return *this;

            Data = *reinterpret_cast<const double*>(RawData[ReadPos]);
            ReadPos+=sizeof(Data);
            return *this;
        }

        Packet& Packet::operator >>(char* Data)
        {
            UInt32 Length = 0;
            *this >> Length;

            if(Length && VerifySize(sizeof(char)*Length))
            {
                Data = new char[Length+1];
                std::memcpy(Data,&RawData[ReadPos],sizeof(char)*Length);
                Data[Length] = '\0';
                ReadPos+=(sizeof(char)*Length);
            }

            return *this;
        }

        Packet& Packet::operator >>(String& Data)
        {
            UInt32 Length = 0;
            *this >> Length;

            if(Length && VerifySize(sizeof(char)*Length))
            {
                Data.clear();
                for( Whole X = 0 ; X < Length ; ++X )
                {
                    Data.push_back(static_cast<char>(RawData[ReadPos+X]));
                }
                ReadPos+=(sizeof(char)*Length);
            }

            return *this;
        }

        Packet& Packet::operator >>(wchar_t* Data)
        {
            UInt32 Length = 0;
            *this >> Length;

            if(Length && VerifySize(sizeof(wchar_t)*Length))
            {
                Data = new wchar_t[Length+1];
                std::memcpy(Data,&RawData[ReadPos],sizeof(wchar_t)*Length);
                Data[Length] = '\0';
                ReadPos+=(sizeof(wchar_t)*Length);
            }

            return *this;
        }

        Packet& Packet::operator >>(WideString& Data)
        {
            UInt32 Length = 0;
            *this >> Length;

            if(Length && VerifySize(sizeof(wchar_t)*Length))
            {
                Data.clear();
                wchar_t* Str = new wchar_t[Length+1];
                std::memcpy(Str,&RawData[ReadPos],sizeof(wchar_t)*Length);
                Str[Length+1] = wchar_t('\0');
                Data.assign(Str);
                ReadPos+=(sizeof(wchar_t)*Length);
            }

            return *this;
        }

        /*Packet& Packet::operator >>(const UTF16String& Data)
        {
            return *this;
        }

        Packet& Packet::operator >>(const UTF32String& Data)
        {
            return *this;
        }*/

        Packet& Packet::operator <<(Boolean Data)
        {
            *this << static_cast<UInt8>(Data);
            return *this;
        }

        Packet& Packet::operator <<(const Int8 Data)
        {
            Append(&Data,sizeof(Data));
            return *this;
        }

        Packet& Packet::operator <<(const UInt8 Data)
        {
            Append(&Data,sizeof(Data));
            return *this;
        }

        Packet& Packet::operator <<(Int16 Data)
        {
            Int16 Converted = htons(Data);
            Append(&Converted,sizeof(Converted));
            return *this;
        }

        Packet& Packet::operator <<(UInt16 Data)
        {
            UInt16 Converted = htons(Data);
            Append(&Converted,sizeof(Converted));
            return *this;
        }

        Packet& Packet::operator <<(Int32 Data)
        {
            Int32 Converted = htonl(Data);
            Append(&Converted,sizeof(Converted));
            return *this;
        }

        Packet& Packet::operator <<(UInt32 Data)
        {
            UInt32 Converted = htonl(Data);
            Append(&Converted,sizeof(Converted));
            return *this;
        }

        Packet& Packet::operator <<(const float Data)
        {
            Append(&Data,sizeof(Data));
            return *this;
        }

        Packet& Packet::operator <<(const double Data)
        {
            Append(&Data,sizeof(Data));
            return *this;
        }

        Packet& Packet::operator <<(const char* Data)
        {
            String Converted(Data);
            *this << Converted;
            return *this;
        }

        Packet& Packet::operator <<(const String& Data)
        {
            UInt32 Length = Data.length();

            if(Length > 0)
            {
                *this << Length;
                Append(Data.c_str(),sizeof(char)*Length);
            }

            return *this;
        }

        Packet& Packet::operator <<(const wchar_t* Data)
        {
            WideString Converted(Data);
            *this << Converted;
            return *this;
        }

        Packet& Packet::operator <<(const WideString& Data)
        {
            UInt32 Length = Data.length();

            if(Length > 0)
            {
                *this << Length;
                Append(Data.c_str(),sizeof(char)*Length);
            }

            return *this;
        }

        /*Packet& Packet::operator <<(const UTF16String& Data)
        {
            return *this;
        }

        Packet& Packet::operator <<(const UTF32String& Data)
        {
            return *this;
        }*/
    }//Network
}//Mezzanine

#endif

#endif //MEZZNETWORK
