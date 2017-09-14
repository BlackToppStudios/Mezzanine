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

#ifndef _networkftpmessage_cpp
#define _networkftpmessage_cpp

#include "Network/ftpmessage.h"

#include "stringtool.h"
#include "exception.h"

namespace Mezzanine
{
    namespace Network
    {
        Boole FTPMessage::IsEndOfLine(const StringIterator CurrIt) const
            { return ( (*CurrIt) == '\r' && *(CurrIt + 1) == '\n' ); }

        ///////////////////////////////////////////////////////////////////////////////
        // Convenience Conversion Methods

        String FTPMessage::ConvertToAddress(const SystemAddress& ToConvert)
        {
            StringStream Ret;
            IPAddress IPToConvert = ToConvert.GetAddress();
            if( IPToConvert.GetProtocol() == Network::NLP_IPv4 ) {
                // Format is a1,a2,a3,a4,p1,p2
                // Get the address in it's raw form.  Then loop over it and convert the binary values to String.
                const IPAddress::AddressContainer& IPContainer = IPToConvert.GetBinaryAddress();
                UInt16 PortNum = ToConvert.GetPort(true);
                // Address
                Ret << IPContainer[0] << ',';
                Ret << IPContainer[1] << ',';
                Ret << IPContainer[2] << ',';
                Ret << IPContainer[3] << ',';
                // Port
                Ret << PortNum / 256 << ',';
                Ret << PortNum % 256;
            }
            return Ret.str();
        }

        SystemAddress FTPMessage::ConvertFromAddress(const String& ToConvert)
        {
            String::const_iterator CurrIt = ToConvert.begin();
            return FTPMessage::ConvertFromAddress(CurrIt,ToConvert.end());
        }

        SystemAddress FTPMessage::ConvertFromAddress(StringIterator& CurrIt, const StringIterator EndIt)
        {
            SystemAddress Ret;
            String Component;
            Whole ComponentIndex = 0;
            UInt16 ConvertedPort = 0;
            IPAddress::AddressContainer ConvertedIP(IPAddress::IPv4BinaryLength,0);

            while( CurrIt != EndIt && ComponentIndex < 6 )
            {
                if( StringTools::IsDigit( *CurrIt ) ) {
                    Component.push_back( *CurrIt );
                }else if( (*CurrIt) == ',' ) {
                    Int8 ComponentVal = StringTools::ConvertToInt8(Component);
                    if( ComponentIndex <= 3 ) {
                        ConvertedIP[ComponentIndex] = ComponentVal;
                        Ret.GetAddress().SetBinaryAddress(ConvertedIP);
                    }else if( ComponentIndex == 4 ) {
                        ConvertedPort = ComponentVal * 256;
                    }else{
                        ConvertedPort += ComponentVal;
                        Ret.SetPort(ConvertedPort,true);
                    }
                    ++ComponentIndex;
                }else{
                    MEZZ_EXCEPTION(ExceptionBase::PARAMETERS_EXCEPTION,"Only digits and commas are expected in an IPv4 address expressed in a PORT/PASV command according to RFC 959.");
                }
                ++CurrIt;
            }
            return Ret;
        }

        String FTPMessage::ConvertToExtendedAddress(const SystemAddress& ToConvert)
        {
            StringStream Ret;
            IPAddress IPToConvert = ToConvert.GetAddress();
            if( IPToConvert.GetProtocol() == Network::NLP_IPv4 || IPToConvert.GetProtocol() == Network::NLP_IPv6 ) {
                Ret << '|';
                Ret << ( IPToConvert.GetProtocol() == Network::NLP_IPv4 ? 1 : 2 );
                Ret << '|';
                Ret << IPToConvert.GetAsString();
                Ret << '|';
                Ret << ToConvert.GetPort();
                Ret << '|';
            }
            return Ret.str();
        }

        SystemAddress FTPMessage::ConvertFromExtendedAddress(const String& ToConvert)
        {
            String::const_iterator CurrIt = ToConvert.begin();
            return FTPMessage::ConvertFromExtendedAddress(CurrIt,ToConvert.end());
        }

        SystemAddress FTPMessage::ConvertFromExtendedAddress(StringIterator& CurrIt, const StringIterator EndIt)
        {
            SystemAddress Ret;
            if( CurrIt != EndIt && (*CurrIt) == '|' ) {
                // Handle the Address Family
                StringIterator NextSeparator = std::find(++CurrIt,EndIt,'|');
                if( NextSeparator != EndIt ) {
                    //UInt16 AddressFamily = StringTools::ConvertToUInt16( String(CurrIt,NextSeparator) );
                    CurrIt = NextSeparator + 1;
                }
                // Handle the Address
                if( CurrIt != EndIt ) {
                    NextSeparator = std::find(++CurrIt,EndIt,'|');
                    if( NextSeparator != EndIt ) {
                        String AddressStr(CurrIt,NextSeparator);
                        Ret.SetAddress( IPAddress( AddressStr ) );
                        CurrIt = NextSeparator + 1;
                    }
                }
                // Handle the Port
                if( CurrIt != EndIt ) {
                    NextSeparator = std::find(++CurrIt,EndIt,'|');
                    if( NextSeparator != EndIt ) {
                        String PortStr(CurrIt,NextSeparator);
                        Ret.SetPort( StringTools::ConvertToUInt16(PortStr) );
                        CurrIt = NextSeparator + 1;
                    }
                }
            }
            return Ret;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Core Operations
    }//Network
}//Mezzanine

#endif
