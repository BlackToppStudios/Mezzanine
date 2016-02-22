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
        /// @brief This is an enum listing for protocols used and recognized by software.
        enum HighLevelProtocol
        {
            HLP_Invalid = 0,  ///< Used for error conditions.
            HLP_FTP     = 1,  ///< File Transfer Protocol.
            HLP_FTPS    = 2,  ///< File Transfer Protocol Secure.
            HLP_HTTP    = 4,  ///< Hyper-Text Transfer Protocol.
            HLP_HTTPS   = 8,  ///< Hyper-Text Transfer Protocol Secure.
            HLP_SMTP    = 16  ///< Simple Mail Transfer Protocol.
        };

        /// @brief This enum is a listing of the commands that can be issued to an FTP server.
        /// @details This is a complete listing of the commands listed for the FTP standard in RFCs 959, 1639, 2228, 2389, 2428, 2640, 7151, and
        /// 3659.  Some commands expect arguments, some do not.  FTP connections must start with the command connection, from the client to the
        /// server.  Once established, the client needs to authenticate with the server using a login.  In most cases using an "anonymous" account
        /// is sufficient.  Once authenticated some information for the data connection is needed.  FTP can operate in active or passive mode.  In
        /// active mode you would send a PORT command providing the IP address and Port number to connect to for the data transfer.  Due to NATs
        /// and Proxies however this is rarely used, instead opting for passive mode.  In passive mode you send a PASV command.  The server will
        /// then respond with an IP address and Port number on which it is listening and will expect an incoming connection.  Once the data
        /// connection is established actual transfers can take place.
        enum FTPCommandList
        {
            FCL_Invalid = 0,   ///< Not an actual command.  Used for error conditions.
            // Access Control Commands
            FCL_ACCT = 1,      ///< Account.  Identifies the users account.  While similar sounding to the USER command, some servers differentiate the two when determining access.
            FCL_CDUP,          ///< Change to Parent Directory.  Changes the directory for file/folder operations to the parent of the current directory.
            FCL_CWD,           ///< Change Working Directory.  Changes the current directory for file/folder operations.
            FCL_HOST,          ///< Host.  Similar to HTTP Host header, allows an FTP client to provide an FTP URI Authority to specify a sub-server(virtual machine, for example) to be directed to.
            FCL_PASS,          ///< Password.  The users password for authentication.
            FCL_QUIT,          ///< Logout.  Logs out of the FTP server and closes all connections.  Pending transfers are allowed to finish and be responded to first.
            FCL_REIN,          ///< Reinitialize.  Sets the connection back to a starting state.  User is logged out but the command connection is preserved.
            FCL_SMNT,          ///< Structure Mount.  Allows the user to mount a file system data structure without requiring the connection to be restarted.  See FTPStructureMount enum.
            FCL_USER,          ///< User Name.  The user name to be used for authentication to the FTP server.  In most cases the user name provided can/will be "anonymous".
            // Transfer Parameter Commands
            FCL_EPRT,          ///< Extended Port.  This is similar to LPRT, but with friendlier syntax.  Takes a "|" delimited String starting with 1(IPv4) or 2(IPv6), then the printable address, then the printable port number.
            FCL_EPSV,          ///< Extended Passive.  This is similar to LPSV, but with friendlier syntax.  The format is identical to the EPRT command.
            FCL_MODE,          ///< Transfer Mode.  Sets how the data will be transferred from the source to destination.  See FTPTransferMode enum.
            FCL_PASV,          ///< Passive.  Requests the server go into passive mode for the connection.  Going into passive mode tells the server to expect an incoming connection from the client, instead of creating one itself.  Format is expressed the same way as the PORT command.
            FCL_PORT,          ///< Data Port.  Specifies the IP/Port on which to establish the data connection.  Expressed in 6 comma separated sections of values 0-255.  First four are IP address, last two are the port bits.
            FCL_STRU,          ///< File Structure.  Specifies the structure being transferred.  Will almost never need to be used as the default is for files.  Can also be set to Record or Page structure.  See FTPStructureMount enum.
            FCL_TYPE,          ///< Data Type.  Sets the type of data requested the file be in for downloads or the type of data that the file being uploaded is in, depending on the operation.  See FTPDataType enum.
            // Data Connection Opening Commands
            FCL_APPE,          ///< Append.  Appends the contents of the transfer to an existing file at the destination.  If the file does not exist it will be created.
            FCL_DELE,          ///< Delete.  Deletes the file specified in the argument.
            FCL_LIST,          ///< List.  Gets information on the current directory.  Either files or directories other than the current one may be specified, in which case it will return information on the specified file/directory.  Format is undefined.
            FCL_NLST,          ///< Name List.  Same as the LIST command, but only specifying names so as to make it easier to parse for machine parsing.
            FCL_RETR,          ///< Retrieve.  Downloads a file from the server, expecting a filename at the source as the argument.
            FCL_STOR,          ///< Store.  Uploads a file to the server, expecting a filename to be given to the file at the destination as the argument.
            FCL_STOU,          ///< Store Unique.  This is the same as STOR (Store), but forces the name of the file at the destination to be unique.
            // Other FTP Service Commands
            FCL_ABOR,          ///< Abort.  Aborts the currently active file transfer.
            FCL_ALLO,          ///< Allocate.  Requests the server allocate space for a file in advance.  Many servers don't require this.  Expects a decimal number expressing the size to allocate as the argument.
            FCL_FEAT,          ///< Features.  Requests a listing of the available features/extensions on the server.  No argument needed.  Format defined in RFC 2389.
            FCL_HELP,          ///< Help.  Another command may be specified as an argument to get details on it's implementation status on the server.  If no argument is specified then it should return a listing of supported commands.
            FCL_LANG,          ///< Language.  Requests the server change it's response message language to a different language.  Use the FEAT command to determine supported languages.
            FCL_MDTM,          ///< Modification Time.  Gets the time a file was last modified.  Argument should be a file name.  The format for the time returned in the response is "YYYYMMDDHHMMSS" with an optional dot separated fraction of a second.
            FCL_MKD,           ///< Make Directory.  Creates a new directory on the server with the name provided in the argument.
            FCL_MLSD,          ///< Machine List Directory.  Gets a machine friendly listing of all the files in the specified directory.  If no directory is specified the current directory is used.  The format is defined in RFC 3659, section 7.2.
            FCL_MLST,          ///< Machine List.  Gets a machine friendly listing of information on the specified file.  The format is defined in RFC 3659, section 7.2.
            FCL_NOOP,          ///< No Operation.  This command does nothing and asks the server to do nothing but send a basic response.  Used to keep connections alive and not timeout.
            FCL_OPTS,          ///< Options.  Enables command specific options for the next command issued.  Argument must be the command name followed by the options to be applied, separated by a space.
            FCL_PWD,           ///< Print Working Directory.  Requests the pathname of the current directory for FTP operations.
            FCL_REST,          ///< Restart.  The point where the file transfer is to begin.  For files, the argument should an integer representing the byte position where the transfer should start.
            FCL_RMD,           ///< Remove Directory.  Deletes the directory (and all files under it) specified in the argument from the server.
            FCL_RNFR,          ///< Rename From.  The pathname of the file to be renamed.  This simple selects the file the operation is to be performed on.  Must be followed by a RNTO command.
            FCL_RNTO,          ///< Rename To.  The new pathname of the file being renamed.  This command must be preceded by a RNFR command to select which file should be renamed.
            FCL_SITE,          ///< Site.  Executes a command that is not in the standard specific to the connected server.  The argument is the command to be executed.
            FCL_SIZE,          ///< File Size.  Gets the size of a specified file.  Argument is a file and the response will contain the size in bytes it would be if it were transferred.  The actual value can change based on the TYPE setting.
            FCL_STAT,          ///< Status.  Requests the current status of the FTP server or the current executing FTP operation on the server.  If a file is specified as an argument then it can act like LIST.
            FCL_SYST,          ///< System.  Requests an identifier for the type of system the server is operating on as well as the default transfer mode used by the server.
            // Security Commands
            FCL_ADAT,          ///< Authentication/Security Data.  A String of Base64 encoded data necessary to complete the authentication/verify the user.
            FCL_AUTH,          ///< Authentication/Security Mechanism.  Declares the desired means of protection on the FTP connection(s), such as "TLS" or "SSL".
            FCL_CCC,           ///< Clear Command Channel.  Clears the command stream of protected status, reverting to plain-text transmissions.  Useful if AUTH is used just to verify identity.
            FCL_CONF,          ///< Confidentiality Protected Command.  Generic command for allowing the configuration of confidentiality settings on the specified security mechanism.  Not used with SSL/TLS.
            FCL_ENC,           ///< Privacy Protected Command.  Generic command for allowing the configuration of privacy settings on the specified security mechanism.  Not used with SSL/TLS.
            FCL_MIC,           ///< Integrity Protected Command.  Generic command for allowing the configuration of integrity settings on the specified security mechanism.  Not used with SSL/TLS.
            FCL_PBSZ,          ///< Protection BufferSize.  Sets the maximum size (in bytes, max of 32-bit UInt) of the encoded blocks to be sent over the command channel.  Must occur after an AUTH command and before a PROT command.
            FCL_PROT,          ///< Data Channel Protection Level.  Sets the security to be used on the data channel.  See FTPSecurityLevel enum.
            // Obsolete/Alias Commands
            FCL_LPRT,          ///< Long Port.  Obsolete command.  Similar to the PORT command, but permits larger addresses.  The format is comma separated bytes.  Starts with an address family, then address size followed by address bytes, then port size followed by port bytes.
            FCL_LPSV,          ///< Long Passive.  Obsolete command.  Similar to the PASV command, but permits larger addresses.  Format is identical to the LPRT command.
            FCL_XCUP,          ///< Change to Parent Directory.  Obsolete command, but can be used an an alias for CDUP to provide backwards compatibility.
            FCL_XCWD,          ///< Change Working Directory.  Obsolete command, but can be used an an alias for CWD to provide backwards compatibility.
            FCL_XMKD,          ///< Make Directory.  Obsolete command, but can be used an an alias for MKD to provide backwards compatibility.
            FCL_XPWD,          ///< Print Working Directory.  Obsolete command, but can be used an an alias for PWD to provide backwards compatibility.
            FCL_XRCP,          ///< Unknown Meaning.  Obsolete command, do not use.  Listed here for error detection server side.  Use of this command is always an error.
            FCL_XRMD,          ///< Remove Directory.  Obsolete command, but can be used an an alias for RMD to provide backwards compatibility.
            FCL_XRSQ,          ///< Unknown Meaning.  Obsolete command, do not use.  Listed here for error detection server side.  Use of this command is always an error.
            FCL_XSEM,          ///< Send to Mail.  Obsolete command, do not use.  Listed here for error detection server side.  Use of this command is always an error.
            FCL_XSEN           ///< Send to Terminal.  Obsolete command, do not use.  Listed here for error detection server side.  Use of this command is always an error.
        };

        /// @brief This enum is a listing of the available data types to transfer as with FTP.
        enum FTPDataType
        {
            FDT_ASCII  = 'A',  ///< Plain text data.  This is the default type.
            FDT_EBCDIC = 'E',  ///< Extended Binary Coded Decimal Interchange Code.  That was a mouthful.  An 8-bit IBM encoding.  FTP supports it, sorta.  So it's here.
            FDT_Binary = 'I',  ///< Binary data.  The "I" is for Image, which when FTP was made was figured to be the most common and often exclusive use of binary transfers with FTP.
            FDT_Local  = 'L'   ///< The local binary type.  Is immediately followed by a number indicating the byte size of the system.  "L8" for example.
        };

        /// @brief This enum is a listing of the response codes that can be used in response to FTP commands.
        enum FTPResponseCode
        {
            FRC_Invalid                       = 0,    ///< Not an actual return code, used in error conditions.
            // 100 Series - The requested action is being initiated, expect another reply before proceeding with a new command.
            FRC_RestartMarkerReply            = 110,  ///< Restart marker reply.
            FRC_ServiceReadySoon              = 120,  ///< Service ready in nnn minutes.
            FRC_DataConnectionAlreadyOpen     = 125,  ///< Data connection already open; transfer starting.
            FRC_DataConnectionOpening         = 150,  ///< File status okay; about to open data connection.
            // 200 Series - The requested action has been successfully completed.
            FRC_Ok                            = 200,  ///< Command acknowledged and completed.
            FRC_PointlessCommand              = 202,  ///< Command not implemented, superfluous at this site.
            FRC_SystemStatus                  = 211,  ///< System status, or system help reply.
            FRC_DirectoryStatus               = 212,  ///< Directory status.
            FRC_FileStatus                    = 213,  ///< File status.
            FRC_Help                          = 214,  ///< Help message.On how to use the server or the meaning of a particular non-standard command.
            FRC_SystemType                    = 215,  ///< Name of system type.
            FRC_ServiceReady                  = 220,  ///< Service ready for new user.
            FRC_ServiceClosing                = 221,  ///< Service closing control connection.
            FRC_DataConnectionOpen            = 225,  ///< Data connection open; no transfer in progress.
            FRC_DataConnectionClosing         = 226,  ///< Closing data connection.  Requested file action successful (for example, file transfer or file abort).
            FRC_EnteringPassive               = 227,  ///< Entering Passive Mode.
            FRC_EnteringLongPassive           = 228,  ///< Entering Long Passive Mode.
            FRC_EnteringExtendedPassive       = 229,  ///< Entering Extended Passive Mode.
            FRC_UserLoggedIn                  = 230,  ///< User logged in, proceed.
            FRC_UserLoggedOut                 = 231,  ///< User logged out; service terminated.
            FRC_LogoutAcknowledged            = 232,  ///< Logout command noted, will complete when transfer done.
            FRC_AuthenticationSuccess         = 234,  ///< Specifies that the server accepts the authentication mechanism specified by the client, and the exchange of security data is complete.
            FRC_FileActionComplete            = 250,  ///< Requested file action okay, completed.
            FRC_PathnameCreated               = 257,  ///< The file or directory was successfully created.
            // 300 Series - The command has been accepted, but the requested action is on hold, pending receipt of further information.
            FRC_NeedPassword                  = 331,  ///< User name okay, need password.
            FRC_NeedAccount                   = 332,  ///< Need account for login.
            FRC_FileActionPending             = 350,  ///< Requested file action pending further information.
            // 400 Series - The command was not accepted and the requested action did not take place, but the error condition is temporary and the action may be requested again.
            FRC_ServiceUnavailable            = 421,  ///< Service not available, closing control connection.
            FRC_DataConnectionFailed          = 425,  ///< Can't open data connection.
            FRC_ConnectionClosedUnexpectedly  = 426,  ///< Connection closed; transfer aborted.
            FRC_BadUserOrPass                 = 430,  ///< Invalid username or password
            FRC_HostUnavailable               = 434,  ///< Requested host unavailable.
            FRC_FileActionFailed              = 450,  ///< Requested file action not taken.
            FRC_LocalError                    = 451,  ///< Requested action aborted.  Local error in processing.
            FRC_InsufficientStorage           = 452,  ///< Requested action not taken due to lack of storage space.
            // 500 Series - Syntax error, command unrecognized and the requested action did not take place. This may include errors such as command line too long.
            FRC_SyntaxError                   = 501,  ///< Syntax error in parameters or arguments.
            FRC_BadCommand                    = 502,  ///< Command not implemented.
            FRC_BadCommandSequence            = 503,  ///< Bad sequence of commands.  A command is issued without a prerequisite command being completed can cause this.
            FRC_BadParameter                  = 504,  ///< Command not implemented for that parameter.
            FRC_NotLoggedIn                   = 530,  ///< Not logged in.
            FRC_InsufficientPermissions       = 532,  ///< Need account for storing files.
            FRC_FileUnavailable               = 550,  ///< Requested action not taken.  File unavailable.
            FRC_PageTypeUnknown               = 551,  ///< Requested action aborted.  Page type unknown.
            FRC_ExceededAllocation            = 552,  ///< Requested file action aborted.  Exceeded storage allocation (for current directory or dataset).
            FRC_NameNotAllowed                = 553,  ///< Requested action not taken.  File name not allowed.
            // 600 Series - Replies regarding confidentiality and integrity.
            FRC_IntegrityReply                = 631,  ///< Integrity protected reply.
            FRC_ConfAndIntegReply             = 632,  ///< Confidentiality and integrity protected reply.
            FRC_ConfidentialityReply          = 633   ///< Confidentiality protected reply.
        };

        /// @brief This enum is a listing of the potential security levels that can be used on an FTPS data channel.
        enum FTPSecurityLevel
        {
            FSL_Clear        = 'C',   ///< No security on the data channel.  This is the default setting.
            FSL_Safe         = 'S',   ///< Data will be integrity protected.  Not used by TLS/SSL.
            FSL_Confidential = 'E',   ///< Data will be confidentiality protected.  Not used by TLS/SSL.
            FSL_Private      = 'P'    ///< Data will be both integrity and confidentiality protected.
        };

        /// @brief This enum is a listing of the available file system structures to be transferred, as recognized by FTP.
        /// @details I'll level with you here...even after some research I'm not sure what exactly the Record and Page values are for.  I mean...I
        /// could guess, but I'd rather document my ignorance than a guess.  In almost all cases (and I say "almost" strictly because once I say
        /// 100% that one case will slap me in the face) File is the only setting you'll use.  This enum exists for completions sake.  I cannot,
        /// and will not, be stopped.
        enum FTPStructureMount
        {
            FSM_File   = 'F',  ///< Configure the FTP server for file transfer.  This is the default setting.
            FSM_Record = 'R',  ///< See detailed description.
            FSM_Page   = 'P'   ///< See detailed description.
        };

        /// @brief This enum is a listing of the available ways data can be transferred over FTP.
        enum FTPTransferMode
        {
            FTM_Stream     = 'S',  ///< Stream the file from the source.  This is the default mode.
            FTM_Block      = 'B',  ///< Send the file over in pre-sized blocks.
            FTM_Compressed = 'C'   ///< Compress the file and then stream it from the source.
        };

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

        /// @brief This is a small enum used for socket shutdown parameters.
        enum SocketShutdown
        {
            SS_DontReceive       = 0,  ///< Socket will discard any data received.
            SS_DontSend          = 1,  ///< Socket will not send any data, regardless of calls to send.
            SS_DontSendOrReceive = 2   ///< Both "SS_DontReceive" and "SS_DontSend".
        };

        /// @brief This is an enum listing for recognized protocols on Layer 4 of the OSI model.
        enum TransportLayerProtocol
        {
            TLP_Invalid = 0,   ///< Used for error conditions.
            TLP_SCTP    = 1,   ///< Stream Control Transmission Protocol.
            TLP_TCP     = 2,   ///< Transmission Control Protocol.
            TLP_UDP     = 4    ///< User Datagram Protocol.
        };

        /// @brief A listing of commonly used ports for various frequently used protocols.
        enum WellKnownPorts
        {
            WKP_FTPData      = 20,
            WKP_FTPControl   = 21,
            WKP_SSH          = 22,
            WKP_Telnet       = 23,
            WKP_SMTP         = 25,
            WKP_HTTP         = 80,
            WKP_NNTP         = 119,
            WKP_LDAP         = 389,
            WKP_HTTPS        = 443,
            WKP_RTSP         = 554,
            WKP_FTPSData     = 989,
            WKP_FTPSControl  = 990,
            WKP_SIP          = 5060,
            WKP_SIPS         = 5061,
            WKP_XMPP         = 5222
        };
    }//Network
}//Mezzanine

#endif
