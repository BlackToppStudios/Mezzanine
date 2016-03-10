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

#ifndef _networkftpresponse_cpp
#define _networkftpresponse_cpp

#include "Network/ftpresponse.h"

#include "stringtool.h"

namespace Mezzanine
{
    namespace Network
    {
        FTPResponse::FTPResponse() :
            ResponseCode(FRC_Invalid)
            {  }

        FTPResponse::FTPResponse(const Network::FTPResponseCode Code, const String& Description) :
            ResponseCode(Code)
        {
            String ConvertedCode = StringTools::ConvertToString(Code);
            this->AddLine(ConvertedCode + " " + Description);
        }

        FTPResponse::FTPResponse(const String& Response)
            { this->Decompose(Response); }

        FTPResponse::~FTPResponse()
            {  }

        Boole FTPResponse::ParseResponseCode(StringIterator CurrIt, const StringIterator EndIt)
        {
            String NumStr;
            StringIterator ActualEnd = ( EndIt - CurrIt > 3 ? CurrIt + 3 : EndIt );
            while( CurrIt != ActualEnd )
            {
                if( StringTools::IsDigit( *CurrIt ) ) {
                    NumStr.push_back( *CurrIt );
                    ++CurrIt;
                }else{
                    break;
                }
            }
            if( NumStr.size() == 3 ) {
                Whole ParsedCode = StringTools::ConvertToWhole(NumStr);
                this->ResponseCode = ( FTPResponse::CodeIsValid(ParsedCode) ? static_cast<FTPResponseCode>(ParsedCode) : FRC_Invalid );
                return true;
            }
            return false;
        }

        Boole FTPResponse::GetResponseLine(StringIterator& CurrIt, const StringIterator EndIt, String& Line)
        {
            while( CurrIt != EndIt )
            {
                if( this->IsEndOfLine(CurrIt) ) {
                    CurrIt += 2;
                    return true;
                }
                Line.push_back( *CurrIt );
                ++CurrIt;
            }
            return true;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Core Operations

        String FTPResponse::Compose() const
        {
            StringStream ResponseStream;
            for( ConstLineIterator LineIt = this->ResponseLines.begin() ; LineIt != this->ResponseLines.end() ; ++LineIt )
                { ResponseStream << (*LineIt) << "/r/n"; }
            return ResponseStream.str();
        }

        Boole FTPResponse::Decompose(const String& Message)
        {
            StringIterator CurrIt = Message.begin();
            return this->Decompose(CurrIt,Message.end());
        }

        Boole FTPResponse::Decompose(StringIterator& CurrIt, const StringIterator EndIt)
        {
            this->ResponseLines.clear();
            this->ResponseCode = FRC_Invalid;

            String NewLine;
            // Do a size check here, size the first line must contain at least 4 characters in all situations.
            if( !this->GetResponseLine(CurrIt,EndIt,NewLine) && NewLine.size() > 3 )
                return false;

            // Extract the response code.
            if( !this->ParseResponseCode(NewLine.begin(),NewLine.end()) )
                return false;

            // Determine if this is a multi-line response or not.  If not, we're done parsing.
            if( NewLine[3] == '-' ) {
                this->ResponseLines.push_back(NewLine);
                while( CurrIt != EndIt )
                {
                    // Clear our line and fetch a new one.
                    NewLine.clear();
                    if( !this->GetResponseLine(CurrIt,EndIt,NewLine) )
                        return false;

                    this->ResponseLines.push_back(NewLine);
                    // Check if it's the last line.
                    if( NewLine.size() >= 3 &&
                        NewLine[0] == this->ResponseLines[0][0] &&
                        NewLine[1] == this->ResponseLines[0][1] &&
                        NewLine[2] == this->ResponseLines[0][2] )
                    {
                        return true;
                    }
                }
                return false;
            }else if( NewLine[3] == ' ' ) {
                this->ResponseLines.push_back(NewLine);
                return true;
            }
            return false;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        Boole FTPResponse::CodeIsValid(const Whole Code)
        {
            switch( Code )
            {
                // 100 Series
                case 110:    case 120:    case 125:    case 150:
                // 200 Series
                case 200:    case 202:    case 211:    case 212:
                case 213:    case 214:    case 215:    case 220:
                case 221:    case 225:    case 226:    case 227:
                case 228:    case 229:    case 230:    case 231:
                case 232:    case 234:    case 250:    case 257:
                // 300 Series
                case 331:    case 332:    case 350:
                // 400 Series
                case 421:    case 425:    case 426:    case 430:
                case 434:    case 450:    case 451:    case 452:
                // 500 Series
                case 501:    case 502:    case 503:    case 504:
                case 530:    case 532:    case 550:    case 551:
                case 552:    case 553:
                // 600 Series
                case 631:    case 632:    case 633:
                {
                    return true;
                }
                default:
                {
                    return false;
                }
            }
            return false;
        }

        void FTPResponse::SetCode(const Network::FTPResponseCode Code)
            { this->ResponseCode = Code; }

        Network::FTPResponseCode FTPResponse::GetCode() const
            { return this->ResponseCode; }

        void FTPResponse::AddLine(const String& NewLine)
            { this->ResponseLines.push_back(NewLine); }

        const String& FTPResponse::GetLine(const Whole Index) const
            { return this->ResponseLines.at(Index); }

        Whole FTPResponse::GetNumLines() const
            { return this->ResponseLines.size(); }

        void FTPResponse::ClearLines()
            { this->ResponseLines.clear(); }

        Boole FTPResponse::IsMultiline() const
            { return this->ResponseLines.size() > 1; }
    }//Network
}//Mezzanine

#endif
