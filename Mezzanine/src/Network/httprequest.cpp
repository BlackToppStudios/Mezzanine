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

#ifdef MEZZNETWORK

#ifndef _networkhttprequest_cpp
#define _networkhttprequest_cpp

#include "Network/httprequest.h"

namespace Mezzanine
{
    namespace Network
    {
        HTTPRequest::HTTPRequest() :
            MajorVersion(1),
            MinorVersion(0),
            RequestMethod(Network::HRM_GET)
            {  }

        HTTPRequest::HTTPRequest(const String& URI, const Network::HTTPRequestMethod Method, const String& Body) :
            MajorVersion(1),
            MinorVersion(0),
            RequestMethod(Method),
            RequestURI(URI),
            RequestBody(Body)
            {  }

        HTTPRequest::~HTTPRequest()
            {  }

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
            if( Method == "GET" )       return Network::HRM_GET;
            if( Method == "HEAD" )      return Network::HRM_HEAD;
            if( Method == "POST" )      return Network::HRM_POST;
            if( Method == "OPTIONS" )   return Network::HRM_OPTIONS;
            if( Method == "PUT" )       return Network::HRM_PUT;
            if( Method == "DELETE" )    return Network::HRM_DELETE;
            if( Method == "TRACE" )     return Network::HRM_TRACE;
            if( Method == "CONNECT" )   return Network::HRM_CONNECT;
            if( Method == "PATCH" )     return Network::HRM_PATCH;
            if( Method == "PROPFIND" )  return Network::HRM_PROPFIND;
            if( Method == "PROPPATCH" ) return Network::HRM_PROPPATCH;
            if( Method == "MKCOL" )     return Network::HRM_MKCOL;
            if( Method == "COPY" )      return Network::HRM_COPY;
            if( Method == "MOVE" )      return Network::HRM_MOVE;
            if( Method == "LOCK" )      return Network::HRM_LOCK;
            if( Method == "UNLOCK" )    return Network::HRM_UNLOCK;
            return Network::HRM_Invalid;
        }

        String HTTPRequest::ComposeRequest() const
        {
            StringStream RequestStream;
            RequestStream << HTTPRequest::ConvertRequestMethod( this->RequestMethod ) << " " << this->RequestURI << " ";
            RequestStream << "HTTP/" << this->MajorVersion << "." << this->MinorVersion << "\r\n";
            for( NameValuePairMap::const_iterator FieldIt = this->RequestFields.begin() ; FieldIt != this->RequestFields.end() ; ++FieldIt )
            {
                RequestStream << (*FieldIt).first << ": " << (*FieldIt).second << "\r\n";
            }
            RequestStream << "\r\n";
            RequestStream << this->RequestBody;
            return RequestStream.str();
        }

        void HTTPRequest::DecomposeRequest(const String& Request)
        {

        }

        void HTTPRequest::SetHTTPVersion(const UInt16 Major, const UInt16 Minor)
            { this->MajorVersion = Major;  this->MinorVersion = Minor; }

        void HTTPRequest::SetHTTPMajorVersion(const UInt16 Major)
            { this->MajorVersion = Major; }

        UInt16 HTTPRequest::GetHTTPMajorVersion() const
            { return this->MajorVersion; }

        void HTTPRequest::SetHTTPMinorVersion(const UInt16 Minor)
            { this->MinorVersion = Minor; }

        UInt16 HTTPRequest::GetHTTPMinorVersion() const
            { return this->MinorVersion; }

        void HTTPRequest::SetURI(const String& URI)
            { this->RequestURI = URI; }

        const String& HTTPRequest::GetURI() const
            { return this->RequestURI; }

        void HTTPRequest::SetMethod(const Network::HTTPRequestMethod Method)
            { this->RequestMethod = Method; }

        Network::HTTPRequestMethod HTTPRequest::GetMethod() const
            { return this->RequestMethod; }

        void HTTPRequest::SetBody(const String& Body)
            { this->RequestBody = Body; }

        const String& HTTPRequest::GetBody() const
            { return this->RequestBody; }

        void HTTPRequest::SetField(const String& FieldName, const String& FieldValue)
        {
            this->RequestFields[FieldName] = FieldValue;
        }

        String HTTPRequest::GetField(const String& FieldName) const
        {
            NameValuePairMap::const_iterator FieldIt = this->RequestFields.find(FieldName);
            if( FieldIt != this->RequestFields.end() ) {
                return (*FieldIt).second;
            }
            return "";
        }

        Boole HTTPRequest::HasField(const String& FieldName) const
        {
            NameValuePairMap::const_iterator FieldIt = this->RequestFields.find(FieldName);
            return ( FieldIt != this->RequestFields.end() );
        }
    }//Network
}//Mezzanine

#endif

#endif //MEZZNETWORK
