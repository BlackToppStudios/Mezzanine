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

#ifndef _networkhttprequest_h
#define _networkhttprequest_h

#include "datatypes.h"
#include "Network/networkenumerations.h"

namespace Mezzanine
{
    namespace Network
    {
        ///////////////////////////////////////////////////////////////////////////////
        /// @class HTTPRequest
        /// @brief A simple class to facilitate the creation of HTTP requests.
        /// @details Some of the data that can be set is mandatory, some of it is optional.  The URI, major and minor versions, as well as
        /// the request method to be used are mandatory in all cases.  If you are using HTTP 1.1 or higher, then adding a "Host" header field
        /// is also mandatory.  The host should be the domain name of the server you want to request from.  Some request methods make use of
        /// the body, most don't.  If the body is used then a header field may be required, such as sending a shoutbox message with a POST
        /// request, which may require the message be put in the request body, and a "Content-Length" header be supplied with the size of
        /// the body.  Many more nuances exist, and we can't cover all use cases.  So take care when using this class.
        ///////////////////////////////////////
        class MEZZ_LIB HTTPRequest
        {
        protected:
            /// @internal
            /// @brief The major version component for this request.
            UInt16 MajorVersion;
            /// @internal
            /// @brief The minor version component for this request.
            UInt16 MinorVersion;
            /// @internal
            /// @brief The HTTP method to use for the request.
            HTTPRequestMethod RequestMethod;
            /// @internal
            /// @brief The URI of the resource to request.
            String RequestURI;
            /// @internal
            /// @brief The body of the request.
            String RequestBody;
            /// @internal
            /// @brief A container of fields to populate the request header with.
            NameValuePairMap RequestFields;
        public:
            /// @brief Blank constructor.
            HTTPRequest();
            /// @brief Descriptive constructor.
            HTTPRequest(const String& URI, const Network::HTTPRequestMethod Method, const String& Body);
            /// @brief Class destructor.
            ~HTTPRequest();

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            /// @brief Converts a HTTPRequestMethod enum value to it's text representation.
            /// @param Method The HTTPRequestMethod enum value to be converted.
            /// @return Returns a String containing the converted enum value.
            static String ConvertRequestMethod(const Network::HTTPRequestMethod Method);
            /// @brief Converts a text request method to it's HTTPRequestMethod enum value.
            /// @param Method The text representation of the request method to be converted.
            /// @return Returns a HTTPRequestMethod enum value representing the converted String.
            static Network::HTTPRequestMethod ConvertRequestMethod(const String& Method);

            /// @brief Creates a request that can be sent to a HTTP server.
            /// @return Returns a String containing the generated request.
            String ComposeRequest() const;
            /// @brief Populates the members of this class with data from a text HTTP request.
            /// @warning A request can be partially decomposed when it fails and have that data saved to members of this class.  For that
            /// reason you should only utilize the data in this class if the decompose was successful, or completely re-written.
            /// @param Request The String containing the request to be decomposed.
            /// @return Returns true if the String was successfully decomposed.
            Boole DecomposeRequest(const String& Request);

            /// @brief Sets both the major and minor version of the request.
            /// @param Major The major portion of the version to set.  Initial Value: 1.
            /// @param Minor The minor portion of the version to set.  Initial Value: 0.
            void SetHTTPVersion(const UInt16 Major, const UInt16 Minor);

            /// @brief Sets the major version of the request.
            /// @param Major The major portion of the version to set.  Initial Value: 1.
            void SetHTTPMajorVersion(const UInt16 Major);
            /// @brief Gets the major version of the request.
            /// @return Returns the major component of this requests version.
            UInt16 GetHTTPMajorVersion() const;
            /// @brief Sets the minor version of the request.
            /// @param Minor The minor portion of the version to set.  Initial Value: 0.
            void SetHTTPMinorVersion(const UInt16 Minor);
            /// @brief Gets the minor version of the request.
            /// @return Returns the minor component of this requests version.
            UInt16 GetHTTPMinorVersion() const;

            /// @brief Sets the URI of the resource being requested.
            /// @param URI The Uniform Resource Identifier of the resource to request.
            void SetURI(const String& URI);
            /// @brief Gets the URI of the resource being requested.
            /// @return Returns a String containing the Uniform Resource Identifier of the resource being requested.
            const String& GetURI() const;
            /// @brief Sets the method to be used for the request.
            /// @param Method The HTTP method to use for the request.  Initial Value: HRM_GET.
            void SetMethod(const Network::HTTPRequestMethod Method);
            /// @brief Gets the method to be used for the request.
            /// @return Returns a HTTPRequestMethod enum value of the request method to be used.
            Network::HTTPRequestMethod GetMethod() const;
            /// @brief Sets the request body containing additional information.
            /// @remarks Request bodies are optional or just unnecessary for most request methods.  Be sure you need a body for your request before setting one.
            /// @param Body The body of the request.
            void SetBody(const String& Body);
            /// @brief Gets the request body containing additional information.
            /// @return Returns a String containing the body of the request.
            const String& GetBody() const;
            /// @brief Sets a Name-Value pair for a header field in the request.
            /// @param FieldName The name of the field to be set.
            /// @param FieldValue The value to be set for the named header field.
            void SetField(const String& FieldName, const String& FieldValue);
            /// @brief Gets a Name-Value pair for a header field in the request.
            /// @param FieldName The name of the field to retrieve.
            /// @return Returns a String containing the set value for the named header.  If none is set or the named field does not exist then an empty String is returned.
            String GetField(const String& FieldName) const;
            /// @brief Checks if this request has the named header field.
            /// @note Fields should only ever be created when "SetField" is called, but there is no guarantee that the value for that field will be set.
            /// @param FieldName The name of the field to check for.
            /// @return Returns true if this request has the named field, false otherwise.
            Boole HasField(const String& FieldName) const;
        };//HTTPRequest
    }//Network
}//Mezzanine

#endif

#endif //MEZZNETWORK
