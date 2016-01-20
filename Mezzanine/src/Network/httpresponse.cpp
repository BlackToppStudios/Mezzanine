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

#ifndef _networkhttpresponse_cpp
#define _networkhttpresponse_cpp

#include "Network/httpresponse.h"

#include "stringtool.h"

namespace
{
    /// @brief Converts a String containing hex characters to a Whole.
    /// @param ToConvert The Hexadecimal String to be converted.
    /// @return Returns a Whole containing the converted value.
    Mezzanine::Whole ConvertToWholeHex(const Mezzanine::String& ToConvert)
    {
        Mezzanine::Whole Ret = 0;
        Mezzanine::StringStream Converter;
        Converter << std::hex << ToConvert;
        Converter >> Ret;
        return Ret;
    }
}

namespace Mezzanine
{
    namespace Network
    {
        HTTPResponse::HTTPResponse() :
            ResponseCode(Network::HSC_Invalid)
            {  }

        HTTPResponse::HTTPResponse(const String& ToDecompose)
            { this->Decompose(ToDecompose); }

        HTTPResponse::~HTTPResponse()
            {  }

        Boole HTTPResponse::ParseHTTPHeader(StringIterator& CurrIt, const StringIterator EndIt)
        {
            // A response header has 3 components: A version, a response code, and that codes' description.
            Boole EoL = false;
            String HeaderTemp;
            // Version
            EoL = this->GetMessageComponent(CurrIt,EndIt,HeaderTemp);
            this->ParseHTTPVersion(HeaderTemp);
            if( EoL ) {
                return false;
            }
            // Response Code
            HeaderTemp.clear();
            EoL = this->GetMessageComponent(CurrIt,EndIt,HeaderTemp);
            this->ResponseCode = StringTools::ConvertToWhole(HeaderTemp);
            if( EoL ) {
                return false;
            }
            // Response Description
            this->ResponseDescription.clear();
            this->GetMessageComponent(CurrIt,EndIt,this->ResponseDescription);
            // Handle extra trash
            //this->AdvanceToNewline(CurrIt,EndIt);
            return true;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        String HTTPResponse::Compose() const
        {
            StringStream ResponseStream;
            // Header
            ResponseStream << "HTTP/" << this->MessageVersion.Major << "." << this->MessageVersion.Minor << " ";
            ResponseStream << StringTools::ConvertToString( this->ResponseCode ) << " " << this->ResponseDescription << "\r\n";
            // Fields
            for( NameValuePairMap::const_iterator FieldIt = this->MessageFields.begin() ; FieldIt != this->MessageFields.end() ; ++FieldIt )
            {
                ResponseStream << (*FieldIt).first << ": " << (*FieldIt).second << "\r\n";
            }
            ResponseStream << "\r\n";
            // Body
            ResponseStream << this->MessageBody;
            return ResponseStream.str();
        }

        Boole HTTPResponse::Decompose(const String& Response)
        {
            if( !Response.empty() ) {
                // Set up the data
                StringIterator CurrIt = Response.begin();
                const StringIterator EndIt = Response.end();
                // Start decomposing
                if( !this->ParseHTTPHeader(CurrIt,EndIt) ) {
                    return false;
                }
                if( !this->ParseHTTPFields(CurrIt,EndIt) ) {
                    return false;
                }

                // The message body may or may not be chunked
                this->MessageBody.clear();
                if( this->GetField("transfer-encoding") != "chunked" ) {
                    NameValuePairMap::iterator FieldIt = this->MessageFields.find("content-length");
                    if( FieldIt == this->MessageFields.end() ) {
                        return false;
                        //MEZZ_EXCEPTION(ExceptionBase::INVALID_STATE_EXCEPTION,"HTTP Response does not contain a \"Content-Length\" nor \"Transfer-Encoding\" header.  Must contain one of these to be valid.");
                    }
                    this->MessageBody.reserve( StringTools::ConvertToWhole( (*FieldIt).second ) );
                    this->MessageBody.assign(CurrIt,EndIt);
                }else{
                    String ChunkSizeStr;
                    this->GetMessageComponent(CurrIt,EndIt,ChunkSizeStr);
                    size_t ChunkLength = ConvertToWholeHex(ChunkSizeStr);
                    while( ChunkLength != 0 )
                    {
                        this->MessageBody.reserve( this->MessageBody.size() + ChunkLength );
                        while( (*CurrIt) != '\r' && CurrIt != EndIt )
                        {
                            this->MessageBody.push_back(*CurrIt);
                            ++CurrIt;
                        }

                        ChunkSizeStr.clear();
                        this->GetMessageComponent(CurrIt,EndIt,ChunkSizeStr);
                        ChunkLength = ConvertToWholeHex(ChunkSizeStr);
                    }
                    // When the body is chunked, there may be optional trailing headers
                    if( !this->ParseHTTPFields(CurrIt,EndIt) ) {
                        return false;
                    }
                }
                return true;
            }

            return false;
        }

        void HTTPResponse::SetStatusCode(const Whole Code)
            { this->ResponseCode = Code; }

        Whole HTTPResponse::GetStatusCode() const
            { return this->ResponseCode; }

        void HTTPResponse::SetResponseDescription(const String& Message)
            { this->ResponseDescription = Message; }

        const String& HTTPResponse::GetResponseDescription() const
            { return this->ResponseDescription; }

        ///////////////////////////////////////////////////////////////////////////////
        // Recommended Header Field Convenience Methods

        void HTTPResponse::SetAllowHeader(const String& Allow)
            { this->SetField("Allow",Allow); }

        const String& HTTPResponse::GetAllowHeader() const
            { return this->GetField("Allow"); }

        void HTTPResponse::SetContentEncodingHeader(const String& ContentEncoding)
            { this->SetField("Content-Encoding",ContentEncoding); }

        const String& HTTPResponse::GetContentEncodingHeader() const
            { return this->GetField("Content-Encoding"); }

        void HTTPResponse::SetDateHeader(const String& Date)
            { this->SetField("Date",Date); }

        const String& HTTPResponse::GetDateHeader() const
            { return this->GetField("Date"); }

        void HTTPResponse::SetServerHeader(const String& Server)
            { this->SetField("Server",Server); }

        const String& HTTPResponse::GetServerHeader() const
            { return this->GetField("Server"); }
    }//Network
}//Mezzanine

#endif
