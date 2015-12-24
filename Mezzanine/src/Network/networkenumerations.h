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

#ifndef _networkenumerations_h
#define _networkenumerations_h

namespace Mezzanine
{
    namespace Network
    {
        /// @enum HighLevelProtocol
        /// @brief This is an enum listing for protocols used and recognized by software.
        enum HighLevelProtocol
        {
            HLP_Invalid = 0,  ///< Used for error conditions.
            HLP_FTP     = 1,  ///< File Transfer Protocol.
            HLP_HTTP    = 2,  ///< Hyper-Text Transfer Protocol.
            HLP_HTTPS   = 4,  ///< Hyper-Text Transfer Protocol Secure.
            HLP_SMTP    = 8   ///< Simple Mail Transfer Protocol.
        };

        /// @enum HTTPRequestMethod
        /// @brief This enum is a listing of commonly supported HTTP request methods.
        /// @details GET and HEAD are expected to be implemented by all HTTP servers, and generally can be used without much issue.  OPTIONS is also
        /// expected to be available but is a little less often.  Other requests may or may not be available.  OPTIONS can be used to see what is
        /// available on a given server, or you can just make a request and see if it returns "HSC_BadRequest" or "HSC_NotImplemented".
        enum HTTPRequestMethod
        {
            HRM_Invalid   = 0,   ///< Use for error conditions, not an actual request method.
            // HTTP 1.0 Methods
            HRM_GET       = 1,   ///< Requests a representation of the specified resource.
            HRM_HEAD      = 2,   ///< Similar to GET, but only requests the header without the body.
            HRM_POST      = 3,   ///< Requests the server accept enclosed data as a new subordinate of the specified resource/URI.  Such as a blog or forum post.
            // HTTP 1.1 Methods
            HRM_OPTIONS   = 4,   ///< Retrieves a list of the supported HTTP methods on the server.
            HRM_PUT       = 5,   ///< Places or replaces a supplied resource at the specified URI.
            HRM_DELETE    = 6,   ///< Deletes the specified resource.
            HRM_TRACE     = 7,   ///< Requests a list of changes or additions done by other sessions.
            HRM_CONNECT   = 8,   ///< Converts the request connection to a tunnel, useful for sending traffic through a proxy.
            // RFC 5789 Method Extensions
            HRM_PATCH     = 9,   ///< Applies partial modifications to a resource.
            // WebDAV Method Extensions
            HRM_PROPFIND  = 10,  ///< Retrieves a property, stored as XML, from a resource.  Can alternatively be overloaded to retrieve the directory hierarchy of a remote system.
            HRM_PROPPATCH = 11,  ///< Can change and/or delete multiple properties of a resource in a single atomic act.
            HRM_MKCOL     = 12,  ///< Can be used to create a new directory.  "COL" is short for Collection, which is the term commonly used for directories in this context.
            HRM_COPY      = 13,  ///< Copies a resource from one URI to another.
            HRM_MOVE      = 14,  ///< Moves a resource from one URI to another.
            HRM_LOCK      = 15,  ///< Locks a resource to prevent alterations being made to it by another session.  This is similar in function to mutexes.
            HRM_UNLOCK    = 16   ///< Unlocks a resource, freeing it for other purposes.
        };

        /// @enum HTTPStatusCode
        /// @brief This enum is a listing of HTTP status codes that can be returned in response to a request.
        enum HTTPStatusCode
        {
            HSC_Invalid                     = 0,    ///< Used for internal error conditions, not an actual status code.
            // 1xx Status Codes - Informational - Only available in HTTP 1.1
            HSC_Continue                    = 100,  ///< Available in HTTP 1.1.  Server has received the request header and client should sent the request body.
            HSC_SwitchingProtocols          = 101,  ///< Available in HTTP 1.1.  Server is acknowledging the requesters desire to switch protocols and is doing so.
            HSC_Processing                  = 102,  ///< Available in WebDAV/RFC 2518.  Server acknowledges the request, but it will take some time.
            // 2xx Status Codes - Success
            HSC_Ok                          = 200,  ///< Available in HTTP 1.0.  Standard successful response to a request.
            HSC_Created                     = 201,  ///< Available in HTTP 1.0.  The request to create a resource was successful.
            HSC_Accepted                    = 202,  ///< Available in HTTP 1.0.  The request was successful, but will take time to process.
            HSC_NonAuthoritativeInfo        = 203,  ///< Available in HTTP 1.1.  The request was successful, but the returned result may be from another source/server.
            HSC_NoContent                   = 204,  ///< Available in HTTP 1.0.  The request was successful, but there is no content to be returned by it.
            HSC_ResetContent                = 205,  ///< Available in HTTP 1.0.  Similar to MSC_NoContent, but additionally the server is requiring the requester reset the document view.
            HSC_PartialContent              = 206,  ///< Available in RFC 7233.  The request was successful, but only part of the content is being delivered due to a range header sent by the client.
            HSC_MultiStatus                 = 207,  ///< Available in WebDAV/RFC 4918.  A WebDAV response that means there are multiple statuses to be reported due to multiple sub-requests being sent by the client.
            HSC_AlreadyReported             = 208,  ///< Available in WebDAV/RFC 5842.  A WebDAV response that means members of a DAV binding have already been reported in a previous reply (to the same request) and won't be reported again.
            HSC_IMUsed                      = 226,  ///< Available in RFC 3229.  The request was successful, and the response is a representation of the result of one or more instance manipulations applied to the current instance.
            // 3xx Status Codes - Redirection
            HSC_MultipleChoices             = 300,  ///< Available in HTTP 1.0.  Indicates multiple options for the resource that the client may follow.
            HSC_MovedPermanently            = 301,  ///< Available in HTTP 1.0.  The resource has been moved and following requests should be directed to following URI.
            HSC_Found                       = 302,  ///< Available in HTTP 1.0.  This code is a mess.  Implementations often used it similarly to HSC_SeeOther, despite the standard saying otherwise.  The standard meant for this to be a temporary redirect.
            HSC_SeeOther                    = 303,  ///< Available in HTTP 1.1.  The response to the request can be found under another URI using the GET HTTP method.
            HSC_NotModified                 = 304,  ///< Available in RFC 7232.  The requested resource has not been modified since the client last downloaded a copy.
            HSC_UseProxy                    = 305,  ///< Available in HTTP 1.1.  The requested resource is only available through the specified proxy.
            HSC_SwitchProxy                 = 306,  ///< Available in HTTP 1.0, deprecated.  The request should be repeated through the provided proxy.
            HSC_TemporaryRedirect           = 307,  ///< Available in HTTP 1.1.  The request should be repeated with the provided URI, but future calls may use the original URI.
            HSC_PermanentRedirect           = 308,  ///< Available in RFC 7538.  The request, and all future requests, should be repeated with the provided URI
            // 4xx Status Codes - Client Error
            HSC_BadRequest                  = 400,  ///< Available in HTTP 1.0.  The request couldn't be fulfilled because of an error with the request that is believed to be client-side.
            HSC_Unauthorized                = 401,  ///< Available in RFC 7235.  Authentication is required to access the resource.  Sometimes a challenge (for authentication) will be provided along with this response.
            HSC_PaymentRequired             = 402,  ///< Available in HTTP 1.0.  Mostly unused, intended to express that a micropayment is necessary to access the requested resource.
            HSC_Forbidden                   = 403,  ///< Available in HTTP 1.0.  The request was received but the server refuses to respond to it.  Similar to HSC_Unauthorized, but authenticating will do nothing.
            HSC_NotFound                    = 404,  ///< Available in HTTP 1.0.  The requested resource does not exist, but may be available in the future.
            HSC_MethodNotAllowed            = 405,  ///< Available in HTTP 1.0.  The request method used was made on a resource that doesn't support that method.
            HSC_NotAcceptable               = 406,  ///< Available in HTTP 1.0.  The requested resource can only generate content that was specified as not acceptable according to the Accept headers specified in the request.
            HSC_ProxyAuthenticationRequired = 407,  ///< Available in RFC 7235.  The requesting client must authenticate itself with a proxy.
            HSC_RequestTimeout              = 408,  ///< Available in HTTP 1.0.  The server timed out while waiting for the clients request.
            HSC_Conflict                    = 409,  ///< Available in HTTP 1.0.  The server detected a conflict in the clients request.
            HSC_Gone                        = 410,  ///< Available in HTTP 1.0.  The requested resource does not exist, and will not be available in the future.
            HSC_LengthRequired              = 411,  ///< Available in HTTP 1.0.  The requested resource requires the request specify it's length, which it requires.
            HSC_PreconditionFailed          = 412,  ///< Available in RFC 7232.  The server does not meet one of the preconditions specified in the request.
            HSC_PayloadTooLarge             = 413,  ///< Available in RFC 7231.  The request is too large for the server to be able or willing to process.
            HSC_RequestURITooLong           = 414,  ///< Available in HTTP 1.0.  The URI specified in the request was too long for the server to be able or willing to process.
            HSC_UnsupportedMediaType        = 415,  ///< Available in HTTP 1.0.  The media type specified in the request is a type that the server does not support.
            HSC_RequestRangeNotSatisfiable  = 416,  ///< Available in RFC 7233.  The request has asked for a portion of a resource, which the server cannot provide.
            HSC_ExpectationFailed           = 417,  ///< Available in HTTP 1.0.  The server cannot meet the requirements set by the "Expect" request header.
            HSC_ImATeapot                   = 418,  ///< Available in RFC 2324.  A joke status code, not intended for any real-world use.
            HSC_AuthenticationTimeout       = 419,  ///< Source of definition unknown.  The authentication that was used for a session has timed out, preventing the request from being processed.
            HSC_MisdirectedRequest          = 421,  ///< Available in HTTP 2.0.  The request was directed at a server that is not able to produce a response.
            HSC_UnprocessableEntity         = 422,  ///< Available in WebDAV/RFC 4918.  The request is well formed, but contains semantic errors preventing it from being processed.
            HSC_Locked                      = 423,  ///< Available in WebDAV/RFC 4918.  The requested resource is currently locked.
            HSC_FailedDependency            = 424,  ///< Available in WebDAV/RFC 4918.  The request failed because of a previous request failure.
            HSC_UpgradeRequired             = 426,  ///< Available in HTTP 1.0.  The server wants the client to change to the protocol specified in the response.
            HSC_PreconditionRequired        = 428,  ///< Available in RFC 6585.  The server requires a request to a resource to be conditional, in order to avoid conflicts.
            HSC_TooManyRequests             = 429,  ///< Available in RFC 6585.  The client has sent too many requests recently to process the current request.
            HSC_RequestHeaderFieldsTooLarge = 431,  ///< Available in RFC 6585.  The server will not process the request because one or more of the header fields are too large.
            // 5xx Status Codes - Server Error
            HSC_InternalServerError         = 500,  ///< Available in HTTP 1.0.  Generic error message for when something goes wrong but the cause is unknown or no suitable error code for it exists.
            HSC_NotImplemented              = 501,  ///< Available in HTTP 1.0.  The request method is not supported or the server otherwise lacks the ability to process the request.
            HSC_BadGateway                  = 502,  ///< Available in HTTP 1.0.  The server is currently acting as a proxy or gateway, and received an invalid response from the upstream server.
            HSC_ServiceUnavailable          = 503,  ///< Available in HTTP 1.0.  The server is temporarily down.
            HSC_GatewayTimeout              = 504,  ///< Available in HTTP 1.0.  The server is currently acting as a proxy or gateway, and didn't receive a timely response from the upstream server.
            HSC_HTTPVersionNotSupported     = 505,  ///< Available in HTTP 1.0.  The server does not support the version of HTTP used in the request.
            HSC_VariantAlsoNegotiates       = 506,  ///< Available in RFC 2295.  A transparent content negotiation for the request would result in a circular reference.
            HSC_InsufficientStorage         = 507,  ///< Available in WebDAV/RFC 4918.  The server does not have enough space to store the representation for the request from the client.
            HSC_LoopDetected                = 508,  ///< Available in WebDAV/RFC 5842.  An infinite loop was detected while processing a request.
            HSC_NotExtended                 = 510,  ///< Available in RFC 2774.  Further extensions are required by this server to fulfill the request.
            HSC_NetAuthenticationRequired   = 511   ///< Available in RFC 6585.  The client needs to authenticate in order to the connected network.  Intended to be used by proxies.
        };

        /// @enum NetworkLayerProtocol
        /// @brief This is an enum listing for recognized protocols on Layer 3 of the OSI model.
        enum NetworkLayerProtocol
        {
            NLP_Invalid = 0,   ///< Used for error conditions.
            NLP_ICMP    = 1,   ///< Internet Control Message Protocol.
            NLP_ICMPv6  = 2,   ///< Internet Control Message Protocol version 6.
            NLP_IGMP    = 4,   ///< Internet Group Management Protocol.
            NLP_IPv4    = 8,   ///< Internet Protocol version 4.
            NLP_IPv6    = 16   ///< Internet Protocol version 6.
        };

        /// @enum SocketShutdown
        /// @brief This is a small enum used for socket shutdown parameters.
        enum SocketShutdown
        {
            SS_DontReceive       = 0,  ///< Socket will discard any data received.
            SS_DontSend          = 1,  ///< Socket will not send any data, regardless of calls to send.
            SS_DontSendOrReceive = 2   ///< Both "SS_DontReceive" and "SS_DontSend".
        };

        /// @enum TransportLayerProtocol
        /// @brief This is an enum listing for recognized protocols on Layer 4 of the OSI model.
        enum TransportLayerProtocol
        {
            TLP_Invalid = 0,   ///< Used for error conditions.
            TLP_SCTP    = 1,   ///< Stream Control Transmission Protocol.
            TLP_TCP     = 2,   ///< Transmission Control Protocol.
            TLP_UDP     = 4    ///< User Datagram Protocol.
        };

        /// @enum WellKnownPorts
        /// @brief A listing of commonly used ports for various frequently used protocols.
        enum WellKnownPorts
        {
            WKP_FTPData = 20,
            WKP_FTPControl = 21,
            WKP_SSH = 22,
            WKP_Telnet = 23,
            WKP_SMTP = 25,
            WKP_HTTP = 80,
            WKP_NNTP = 119,
            WKP_LDAP = 389,
            WKP_HTTPS = 443,
            WKP_RTSP = 554,
            WKP_FTPSData = 989,
            WKP_FTPSControl = 990,
            WKP_SIP = 5060,
            WKP_SIPS = 5061,
            WKP_XMPP = 5222
        };
    }//Network
}//Mezzanine

#endif
