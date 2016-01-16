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

#ifndef _networkhttprequest_cpp
#define _networkhttprequest_cpp

#include "Network/httprequest.h"

#include "stringtool.h"

namespace Mezzanine
{
    namespace Network
    {
        HTTPRequest::HTTPRequest() :
            RequestMethod(Network::HRM_GET)
            {  }

        HTTPRequest::HTTPRequest(const String& ToDecompose)
            { this->Decompose(ToDecompose); }

        HTTPRequest::HTTPRequest(const String& URI, const Network::HTTPRequestMethod Method, const String& Body) :
            RequestMethod(Method),
            RequestURI(URI)
            { this->MessageBody = Body; }

        HTTPRequest::~HTTPRequest()
            {  }

        Boole HTTPRequest::ParseHTTPHeader(StringIterator& CurrIt, const StringIterator EndIt)
        {
            // A request header has 3 components: A method, a URI, and a version.
            Boole EoL = false;
            String HeaderTemp;
            // Method
            EoL = this->GetMessageComponent(CurrIt,EndIt,HeaderTemp);
            this->RequestMethod = this->ConvertRequestMethod(HeaderTemp);
            if( EoL ) {
                return false;
            }
            // URI
            HeaderTemp.clear();
            EoL = this->GetMessageComponent(CurrIt,EndIt,HeaderTemp);
            this->RequestURI.ParseFromString(HeaderTemp);
            if( EoL ) {
                return false;
            }
            // Version
            HeaderTemp.clear();
            this->GetMessageComponent(CurrIt,EndIt,HeaderTemp);
            this->ParseHTTPVersion(HeaderTemp);
            // Handle extra trash
            //this->AdvanceToNewline(CurrIt,EndIt);
            return true;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        String HTTPRequest::ConvertRequestMethod(const Network::HTTPRequestMethod Method)
        {
            switch( Method )
            {
                case HRM_GET:           return "GET";
                case HRM_HEAD:          return "HEAD";
                case HRM_POST:          return "POST";
                case HRM_OPTIONS:       return "OPTIONS";
                case HRM_PUT:           return "PUT";
                case HRM_DELETE:        return "DELETE";
                case HRM_TRACE:         return "TRACE";
                case HRM_CONNECT:       return "CONNECT";
                case HRM_PATCH:         return "PATCH";
                case HRM_PROPFIND:      return "PROPFIND";
                case HRM_PROPPATCH:     return "PROPPATCH";
                case HRM_MKCOL:         return "MKCOL";
                case HRM_COPY:          return "COPY";
                case HRM_MOVE:          return "MOVE";
                case HRM_LOCK:          return "LOCK";
                case HRM_UNLOCK:        return "UNLOCK";
                default:
                case HRM_Invalid:       return "";
            }
            return "";
        }

        Network::HTTPRequestMethod HTTPRequest::ConvertRequestMethod(const String& Method)
        {
            if( Method == "GET" )       { return Network::HRM_GET; }
            if( Method == "HEAD" )      { return Network::HRM_HEAD; }
            if( Method == "POST" )      { return Network::HRM_POST; }
            if( Method == "OPTIONS" )   { return Network::HRM_OPTIONS; }
            if( Method == "PUT" )       { return Network::HRM_PUT; }
            if( Method == "DELETE" )    { return Network::HRM_DELETE; }
            if( Method == "TRACE" )     { return Network::HRM_TRACE; }
            if( Method == "CONNECT" )   { return Network::HRM_CONNECT; }
            if( Method == "PATCH" )     { return Network::HRM_PATCH; }
            if( Method == "PROPFIND" )  { return Network::HRM_PROPFIND; }
            if( Method == "PROPPATCH" ) { return Network::HRM_PROPPATCH; }
            if( Method == "MKCOL" )     { return Network::HRM_MKCOL; }
            if( Method == "COPY" )      { return Network::HRM_COPY; }
            if( Method == "MOVE" )      { return Network::HRM_MOVE; }
            if( Method == "LOCK" )      { return Network::HRM_LOCK; }
            if( Method == "UNLOCK" )    { return Network::HRM_UNLOCK; }
            return Network::HRM_Invalid;
        }

        String HTTPRequest::Compose() const
        {
            StringStream RequestStream;
            // Header
            RequestStream << HTTPRequest::ConvertRequestMethod( this->RequestMethod ) << " " << this->RequestURI.ConvertToString() << " ";
            RequestStream << "HTTP/" << this->MessageVersion.Major << "." << this->MessageVersion.Minor << "\r\n";
            // Fields
            for( NameValuePairMap::const_iterator FieldIt = this->MessageFields.begin() ; FieldIt != this->MessageFields.end() ; ++FieldIt )
            {
                RequestStream << (*FieldIt).first << ": " << (*FieldIt).second << "\r\n";
            }
            RequestStream << "\r\n";
            // Body
            RequestStream << this->MessageBody;
            return RequestStream.str();
        }

        Boole HTTPRequest::Decompose(const String& Request)
        {
            if( !Request.empty() ) {
                // Set up the data
                StringIterator CurrIt = Request.begin();
                const StringIterator EndIt = Request.end();
                // Start decomposing
                if( !this->ParseHTTPHeader(CurrIt,EndIt) ) {
                    return false;
                }
                if( !this->ParseHTTPFields(CurrIt,EndIt) ) {
                    return false;
                }
                this->MessageBody.clear();
                this->MessageBody.assign(CurrIt,EndIt);

                return true;
            }
            return false;
        }

        void HTTPRequest::SetURI(const URI& URI)
            { this->RequestURI = URI; }

        const URI& HTTPRequest::GetURI() const
            { return this->RequestURI; }

        void HTTPRequest::SetMethod(const Network::HTTPRequestMethod Method)
            { this->RequestMethod = Method; }

        Network::HTTPRequestMethod HTTPRequest::GetMethod() const
            { return this->RequestMethod; }

        ///////////////////////////////////////////////////////////////////////////////
        // Recommended Header Field Convenience Methods

        void HTTPRequest::SetExpectHeader(const String& Expect)
            { this->SetField("Expect",Expect); }

        const String& HTTPRequest::GetExpectHeader() const
            { return this->GetField("Expect"); }

        void HTTPRequest::SetFromHeader(const String& From)
            { this->SetField("From",From); }

        const String& HTTPRequest::GetFromHeader() const
            { return this->GetField("From"); }

        void HTTPRequest::SetHostHeader(const String& Host)
            { this->SetField("Host",Host); }

        const String& HTTPRequest::GetHostHeader() const
            { return this->GetField("Host"); }

        void HTTPRequest::SetUserAgentHeader(const String& UserAgent)
            { this->SetField("User-Agent",UserAgent); }

        const String& HTTPRequest::GetUserAgentHeader() const
            { return this->GetField("User-Agent"); }
    }//Network
}//Mezzanine

#endif
