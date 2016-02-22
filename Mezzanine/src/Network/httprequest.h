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

#ifndef _networkhttprequest_h
#define _networkhttprequest_h

#include "Network/networkenumerations.h"
#include "Network/httpmessage.h"
#include "Network/uri.h"

namespace Mezzanine
{
    namespace Network
    {
        ///////////////////////////////////////////////////////////////////////////////
        /// @brief A simple class to facilitate the creation of HTTP requests.
        /// @details Some of the data that can be set is mandatory, some of it is optional.  The URI, major and minor versions, as well as
        /// the request method to be used are mandatory in all cases.  If you are using HTTP 1.1 or higher, then adding a "Host" header field
        /// is also mandatory.  The host should be the domain name of the server you want to request from.  Some request methods make use of
        /// the body, most don't.  If the body is used then a header field may be required, such as sending a shoutbox message with a POST
        /// request, which may require the message be put in the request body, and a "Content-Length" header be supplied with the size of
        /// the body.  Many more nuances exist, and we can't cover all use cases.  So take care when using this class.
        ///////////////////////////////////////
        class MEZZ_LIB HTTPRequest : public HTTPMessage
        {
        protected:
            /// @internal
            /// @brief The HTTP method to use for the request.
            HTTPRequestMethod RequestMethod;
            /// @internal
            /// @brief The URI of the resource to request.
            URI RequestURI;

            /// @copydoc HTTPMessage::ParseHTTPHeader(StringIterator&,const StringIterator)
            Boole ParseHTTPHeader(StringIterator& CurrIt, const StringIterator EndIt);
        public:
            /// @brief Blank constructor.
            HTTPRequest();
            /// @brief String constructor.
            /// @param ToDecompose The String to construct this request from.
            /// @remarks For normal operation this constructor should seldom be used, since this class is intended to aid in building the string.
            HTTPRequest(const String& ToDecompose);
            /// @brief Descriptive constructor.
            HTTPRequest(const String& URI, const Network::HTTPRequestMethod Method, const String& Body);
            /// @brief Class destructor.
            ~HTTPRequest();

            ///////////////////////////////////////////////////////////////////////////////
            // Core Operations

            /// @copydoc HTTPMessage::Compose() const
            String Compose() const;
            /// @copydoc HTTPMessage::Decompose(const String&)
            /// @warning A request can be partially decomposed when it fails and have that data saved to members of this class.  For that
            /// reason you should only utilize the data in this class if the decompose was successful, or completely re-written.
            Boole Decompose(const String& Message);
            /// @copydoc HTTPMessage::Decompose(StringIterator&, const StringIterator)
            /// @warning A request can be partially decomposed when it fails and have that data saved to members of this class.  For that
            /// reason you should only utilize the data in this class if the decompose was successful, or completely re-written.
            Boole Decompose(StringIterator& CurrIt, const StringIterator EndIt);

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

            /// @brief Sets the URI of the resource being requested.
            /// @param URI The Uniform Resource Identifier of the resource to request.
            void SetURI(const URI& URI);
            /// @brief Gets the URI of the resource being requested.
            /// @return Returns a String containing the Uniform Resource Identifier of the resource being requested.
            const URI& GetURI() const;
            /// @brief Sets the method to be used for the request.
            /// @param Method The HTTP method to use for the request.  Initial Value: HRM_GET.
            void SetMethod(const Network::HTTPRequestMethod Method);
            /// @brief Gets the method to be used for the request.
            /// @return Returns a HTTPRequestMethod enum value of the request method to be used.
            Network::HTTPRequestMethod GetMethod() const;

            ///////////////////////////////////////////////////////////////////////////////
            // Recommended Header Field Convenience Methods

            /// @brief Sets the Expect Header.
            /// @remarks An optional header that most commonly is used when the expectation is "100-continue".  When that expectation is set a partial request (without a body)
            /// can be sent to the HTTP server and will check the headers to see if anything is malformed there.  If nothing is it will succeed and the client can then send
            /// a full request with the body.  Useful for requests with very large bodies.
            /// @param Expect Used to describe to the server one or more expectations being made on it by the client.
            void SetExpectHeader(const String& Expect);
            /// @brief Gets the Expect Header.
            /// @return Returns a String containing one or more expectations being made on the server by the client.
            const String& GetExpectHeader() const;
            /// @brief Sets the From Header.
            /// @remarks This field is entirely optional.  Some smaller web services can make use of this however.
            /// @param From Contains an email address that uniquely identifies the sender of the request.
            void SetFromHeader(const String& From);
            /// @brief Gets the From Header.
            /// @return Returns a String containing an email address that uniquely identifies the sender of the request.
            const String& GetFromHeader() const;
            /// @brief Sets the Host Header.
            /// @remarks This header is required for all requests being made via HTTP 1.1 and up.  It is useful for servers when resolving specific hosts behind proxies.
            /// @param Host The URI-Authority of the host to be connected to.
            void SetHostHeader(const String& Host);
            /// @brief Gets the Host Header.
            /// @return Returns a String containing the URI-Authority of the host to be connected to, or blank if the field is not set.
            const String& GetHostHeader() const;
            /// @brief Sets the User-Agent Header.
            /// @remarks An optional header used to provide slightly more context about what the requester is running.
            /// @param UserAgent The name of the platform/software being used to generate this request.
            void SetUserAgentHeader(const String& UserAgent);
            /// @brief Gets the User-Agent Header.
            /// @return Returns a String containing the name of the platform/software being used to generate this request.
            const String& GetUserAgentHeader() const;
        };//HTTPRequest
    }//Network
}//Mezzanine

#endif
