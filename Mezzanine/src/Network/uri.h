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

#ifndef _networkuri_h
#define _networkuri_h

#include "Network/ipaddress.h"

namespace Mezzanine
{
    namespace Network
    {
        ///////////////////////////////////////////////////////////////////////////////
        /// @brief A helper class for the reading and using of Uniform Resource Identifiers.
        /// @details This is a glorified String parser built according to the RFC 3986 specification.
        /// There are a few quirks about it's use.
        ///     - With only one exception, delimiters are not stored in components.  That exception is slashes in the Path component.
        ///     - This class makes no attempt to distinguish files and folders in the path.  In general if you end the path with a folder you should have a trailing slash.
        ///       It's assumed to be a file otherwise and may get truncated as a result.
        ///     - Ampersands or semi-colons are allowed for separating entries in a Query.  This class only uses ampersands when writing, but can read either notation.
        ///////////////////////////////////////
        class MEZZ_LIB URI
        {
        public:
            /// @brief Convenience typedef for String iterators.
            typedef String::const_iterator StringIterator;
            /// @brief Convenience typedef for storing if a decode was successful and it's result.
            typedef std::pair<Boole,Char8> DecodeResult;

            /// @brief A String containing the generic delimiters for URIs.
            /// @remarks The characters that appear in this String (which are ":/?#[]@") should be avoided when constructing URIs.
            /// If any such characters are found they'll be percent encoded where possible.  In some cases exceptions may be thrown
            /// as a result of their misuse.
            static const String GenDelims;
            /// @brief A String containing the sub-delimiters that some URI schemes may use to delimit scheme specific data.
            /// @remarks The characters in this String (which are "!$&'()*+,;=") are not as specific or rigorous as those in the
            /// GenDelims String.  They may or may not be used by some schemes and in general should be avoided where possible.  Like
            /// characters in the GenDelims String, if these characters are found they'll be percent encoded.
            static const String SubDelims;
        protected:
            /// @internal
            /// @brief The Scheme of the URI, usually noting the type of data the resource is.
            String URIScheme;
            /// @internal
            /// @brief The optional user authentication information to be used when accessing the resource.
            String URIUserInfo;
            /// @internal
            /// @brief The domain or address of the Host where the resource is located.
            String URIHost;
            /// @internal
            /// @brief The path to the resource on the Host machine holding the resource.
            String URIPath;
            /// @internal
            /// @brief The optional Query to provide additional non-hierarchical information about the resource.
            String URIQuery;
            /// @internal
            /// @brief The optional Fragment providing additional direction to a subset of the resource.
            String URIFragment;
            /// @internal
            /// @brief The port number on the host to connect to in order to access the resource.
            UInt16 URIPort;

            /// @internal
            /// @brief Parses the Scheme of a URI String.
            /// @exception If an invalid character or an error is encountered a SYNTAX_ERROR_EXCEPTION will be thrown.
            /// @param CurrIt A modifiable iterator to the current character in the parsing operation.
            /// @param EndIt Then end of the character sequence being parsed.
            void ParseScheme(StringIterator CurrIt, const StringIterator EndIt);
            /// @internal
            /// @brief Parses the User Information of a URI String.
            /// @exception If an invalid character or an error is encountered a SYNTAX_ERROR_EXCEPTION will be thrown.
            /// @param CurrIt A modifiable iterator to the current character in the parsing operation.
            /// @param EndIt Then end of the character sequence being parsed.
            void ParseUserInfo(StringIterator CurrIt, const StringIterator EndIt);
            /// @internal
            /// @brief Parses the Host of a URI String.
            /// @exception If an invalid character or an error is encountered a SYNTAX_ERROR_EXCEPTION will be thrown.
            /// @param CurrIt A modifiable iterator to the current character in the parsing operation.
            /// @param EndIt Then end of the character sequence being parsed.
            void ParseHost(StringIterator CurrIt, const StringIterator EndIt);
            /// @internal
            /// @brief Parses the Port of a URI String.
            /// @exception If an invalid character or an error is encountered a SYNTAX_ERROR_EXCEPTION will be thrown.
            /// @param CurrIt A modifiable iterator to the current character in the parsing operation.
            /// @param EndIt Then end of the character sequence being parsed.
            void ParsePort(StringIterator CurrIt, const StringIterator EndIt);
            /// @internal
            /// @brief Parses the Path of a URI String.
            /// @exception If an invalid character or an error is encountered a SYNTAX_ERROR_EXCEPTION will be thrown.
            /// @param CurrIt A modifiable iterator to the current character in the parsing operation.
            /// @param EndIt Then end of the character sequence being parsed.
            void ParsePath(StringIterator CurrIt, const StringIterator EndIt);
            /// @internal
            /// @brief Parses the Query of a URI String.
            /// @exception If an invalid character or an error is encountered a SYNTAX_ERROR_EXCEPTION will be thrown.
            /// @param CurrIt A modifiable iterator to the current character in the parsing operation.
            /// @param EndIt Then end of the character sequence being parsed.
            void ParseQuery(StringIterator CurrIt, const StringIterator EndIt);
            /// @internal
            /// @brief Parses the Fragment of a URI String.
            /// @exception If an invalid character or an error is encountered a SYNTAX_ERROR_EXCEPTION will be thrown.
            /// @param CurrIt A modifiable iterator to the current character in the parsing operation.
            /// @param EndIt Then end of the character sequence being parsed.
            void ParseFragment(StringIterator CurrIt, const StringIterator EndIt);
            /// @internal
            /// @brief Parses the Authority of a URI String.
            /// @exception If an invalid character or an error is encountered a SYNTAX_ERROR_EXCEPTION will be thrown.
            /// @param CurrIt A modifiable iterator to the current character in the parsing operation.
            /// @param EndIt Then end of the character sequence being parsed.
            void ParseAuthority(StringIterator& CurrIt, const StringIterator EndIt);
            /// @internal
            /// @brief Parses the Authority of a URI String, without checking for the preceding slashes.
            /// @exception If an invalid character or an error is encountered a SYNTAX_ERROR_EXCEPTION will be thrown.
            /// @param CurrIt A modifiable iterator to the current character in the parsing operation.
            /// @param EndIt Then end of the character sequence being parsed.
            void ParseAuthorityNoSlash(StringIterator& CurrIt, const StringIterator EndIt);
            /// @internal
            /// @brief Parses the Path, Query, and Fragment components of a URI String.
            /// @exception If an invalid character or an error is encountered a SYNTAX_ERROR_EXCEPTION will be thrown.
            /// @param CurrIt A modifiable iterator to the current character in the parsing operation.
            /// @param EndIt Then end of the character sequence being parsed.
            void ParsePathQueryFrag(StringIterator& CurrIt, const StringIterator EndIt);
        public:
            /// @brief Blank constructor.
            URI();
            /// @brief Copy constructor.
            /// @param Other The other URI to be copied.
            URI(const URI& Other);
            /// @brief Parsing constructor.
            /// @param ToParse The String to be parsed by this URI.
            URI(const String& ToParse);
            /// @brief Component constructor.
            /// @param Scheme The Scheme component of the URI.
            /// @param UserInfo The User Information component of the URI.
            /// @param Host The Host component of the URI.
            /// @param Port The Port on the Host to use for connections to access the resource.
            /// @param Path The Path on the Host to the resource.
            /// @param Query The Query component of the URI.
            /// @param Fragment The Fragment component of the URI.
            URI(const String& Scheme, const String& UserInfo, const String& Host, const String& Port, const String& Path, const String& Query, const String& Fragment);
            /// @brief Component constructor.
            /// @param Scheme The Scheme component of the URI.
            /// @param UserInfo The User Information component of the URI.
            /// @param Host The Host component of the URI.
            /// @param Port The Port on the Host to use for connections to access the resource.
            /// @param Path The Path on the Host to the resource.
            /// @param Query The Query component of the URI.
            /// @param Fragment The Fragment component of the URI.
            URI(const String& Scheme, const String& UserInfo, const String& Host, const UInt16 Port, const String& Path, const String& Query, const String& Fragment);
            /// @brief Class destructor.
            ~URI();

            ///////////////////////////////////////////////////////////////////////////////
            // Percent Encoding

            /// @brief Converts a character into a percent encoded string that is safe for use in URI's.
            /// @note Percent encoding permits special characters normally used as delimiters by the standard to be used in other contexts, so that parsing isn't made more difficult.
            /// @param ToEncode The character to be encoded into a percent encode.
            /// @return Returns a String containing the percent encoded character that can be used in a URI.
            static String PercentEncodeSingle(const Char8 ToEncode);
            /// @brief Converts a String that is percent encoded into it's appropriate character.
            /// @remarks This method expects the String parameter to be exactly 3 characters long.
            /// @param ToDecode The String to be decoded into the original character.
            /// @return Returns the decoded Character, or a NULL character (0) in the event of a failure.
            static DecodeResult PercentDecodeSingle(const String& ToDecode);
            /// @brief Encodes a single character to it's percent encoding and appends that to the destination String.
            /// @param ToEncode The character to be percent encoded.
            /// @param Destination The String to append the result to.  Contents of this String are NOT cleared by this method before appending.
            static void PercentEncodeSingle(const Char8 ToEncode, String& Destination);
            /// @brief Decodes a single percent encoding to it's actual character and appends that to the destination String.
            /// @param ToDecode The 3-Character Percent Encoded String to be decoded.
            /// @param Destination The String to append the result to.  Contents of this String are NOT cleared by this method before appending.
            static void PercentDecodeSingle(const String& ToDecode, String& Destination);

            /// @brief Creates a duplicate String with all the reserved characters percent encoded and appends that to the destination String.
            /// @param ToEncode The String to be encoded.
            /// @param AdditionalDelims A String containing additional delimiters to encode.  Characters in the GenDelims static String are always encoded.  In most cases an empty string is appropriate to be passed in.
            /// @return A String containing the encoded contents of "ToEncode".
            static String PercentEncode(const String& ToEncode, const String& AdditionalDelims);
            /// @brief Creates a duplicate String with all percent encodings decoded and appends that to the destination String.
            /// @param ToDecode The String to be decoded.
            /// @param PlusToSpace If true, will decode all Plus signs to spaces.
            /// @return A String containing the decoded contents of "ToDecode".
            static String PercentDecode(const String& ToDecode, const Boole PlusToSpace = false);
            /// @brief Creates a duplicate String with all the reserved characters percent encoded and appends that to the destination String.
            /// @param ToEncode The String to be encoded.
            /// @param Destination The String to append the result to.  Contents of this String are NOT cleared by this method before appending.
            /// @param AdditionalDelims A String containing additional delimiters to encode.  Characters in the GenDelims static String are always encoded.  In most cases an empty string is appropriate to be passed in.
            static void PercentEncode(const String& ToEncode, String& Destination, const String& AdditionalDelims);
            /// @brief Creates a duplicate String with all percent encodings decoded and appends that to the destination String.
            /// @param ToDecode The String to be decoded.
            /// @param Destination The String to append the result to.  Contents of this String are NOT cleared by this method before appending.
            /// @param PlusToSpace If true, will decode all Plus signs to spaces.
            static void PercentDecode(const String& ToDecode, String& Destination, const Boole PlusToSpace = false);

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            /// @brief Gets the default port for a named protocol.
            /// @remarks The String passed in is expected to be in lower case, this operation is not performed for you.  This also only gets the default ports for
            /// protocols that the Mezzanine recognizes and that have URI schemes.  This is not meant to be completely comprehensive.
            /// @param Scheme The name of the Scheme/protocol to get the default port of, in lower case.
            /// @return Returns the Port the specified Scheme defaults to, or 0 if the Scheme was not recognized.
            static UInt16 GetDefaultPort(const String& Scheme);
            /// @brief Removes frivolous parts of the path that mean nothing.
            /// @remarks For example a path like "./[anything]/.." means nothing at all.
            /// @param ToRemove The path component of a URI to remove the dot segments from.
            /// @param Relative Whether or not the URI is relative, in which case it will preserve relative dot segments at the start of the path.
            static void RemoveDotSegments(String& ToRemove, const Boole Relative);

            /// @brief Gets whether or not this URI points to an absolute resource location.
            /// @return Returns true if this URI is absolute, false otherwise.
            Boole IsAbsolute() const;
            /// @brief Gets whether or not this URI points to a relative resource location.
            /// @return Returns true if this URI is intended to be relative to another URI, false otherwise.
            Boole IsRelative() const;
            /// @brief Gets whether or not this URI lacks a hierarchy.
            /// @return Returns true if this URI lacks a hierarchy, false otherwise.
            Boole IsOpaque() const;
            /// @brief Gets whether or not this URI has hierarchical components.
            /// @return Returns true if this URI has hierarchical components, false otherwise.
            Boole IsHierarchical() const;

            /// @brief Sets each component of this URI explicitly.
            /// @param Scheme The Scheme component of the URI.
            /// @param UserInfo The User Information component of the URI.
            /// @param Host The Host component of the URI.
            /// @param Port The Port on the Host to use for connections to access the resource.
            /// @param Path The Path on the Host to the resource.
            /// @param Query The Query component of the URI.
            /// @param Fragment The Fragment component of the URI.
            void SetComponents(const String& Scheme, const String& UserInfo, const String& Host, const String& Port, const String& Path, const String& Query, const String& Fragment);
            /// @brief Sets each component of this URI explicitly.
            /// @param Scheme The Scheme component of the URI.
            /// @param UserInfo The User Information component of the URI.
            /// @param Host The Host component of the URI.
            /// @param Port The Port on the Host to use for connections to access the resource.
            /// @param Path The Path on the Host to the resource.
            /// @param Query The Query component of the URI.
            /// @param Fragment The Fragment component of the URI.
            void SetComponents(const String& Scheme, const String& UserInfo, const String& Host, const UInt16 Port, const String& Path, const String& Query, const String& Fragment);
            /// @brief Sets each component of this URI explicitly without actually parsing the strings (direct copy).
            /// @remarks Use this method with extreme care, forbidden characters will not be caught with this method.
            /// @param Scheme The Scheme component of the URI.
            /// @param UserInfo The User Information component of the URI.
            /// @param Host The Host component of the URI.
            /// @param Port The Port on the Host to use for connections to access the resource.
            /// @param Path The Path on the Host to the resource.
            /// @param Query The Query component of the URI.
            /// @param Fragment The Fragment component of the URI.
            void SetComponentsNoParse(const String& Scheme, const String& UserInfo, const String& Host, const String& Port, const String& Path, const String& Query, const String& Fragment);
            /// @brief Sets each component of this URI explicitly without actually parsing the strings (direct copy).
            /// @remarks Use this method with extreme care, forbidden characters will not be caught with this method.
            /// @param Scheme The Scheme component of the URI.
            /// @param UserInfo The User Information component of the URI.
            /// @param Host The Host component of the URI.
            /// @param Port The Port on the Host to use for connections to access the resource.
            /// @param Path The Path on the Host to the resource.
            /// @param Query The Query component of the URI.
            /// @param Fragment The Fragment component of the URI.
            void SetComponentsNoParse(const String& Scheme, const String& UserInfo, const String& Host, const UInt16 Port, const String& Path, const String& Query, const String& Fragment);

            /// @brief Makes minor adjustments to the content of this URI to make it easier to process/compare.
            /// @remarks This method makes a small series of adjustments to the URI to make it easier to read by humans and computers alike, these adjustments include:
            ///     1. Drop everything that is case in-sensitive to lower case.
            ///     2. Remove frivolous components in the path.  For example a path like "./[anything]/.." means nothing at all.
            ///     3. Decode unreserved percent encoded characters.  Only certain characters need to be percent encoded.  Encoding more than those wastes space.
            ///     4. If the port is a default port for the Scheme, it will be removed.
            /// @return Returns a reference to this.
            URI& Normalize();
            /// @brief Removes frivolous parts of the path that mean nothing.
            /// @remarks For example a path like "./[anything]/.." means nothing at all.
            /// @param Relative Whether or not this URI is relative, in which case it will preserve relative dot segments at the start of the path.
            /// @return Returns a reference to this.
            URI& RemoveDotSegments(const Boole Relative);
            /// @brief Swaps the contents of this URI with another.
            /// @param Other The other URI to be swapped with.
            /// @return Returns a reference to this.
            URI& Swap(URI& Other);
            /// @brief Reverts this URI to a blank slate.
            /// @return Returns a reference to this.
            URI& Clear();
            /// @brief Checks to see if the URI is currently empty.
            /// @return Returns true if all components of this URI are not set, false otherwise.
            Boole IsEmpty() const;

            /// @brief Computes the absolute referenced URI from a relative URI and the absolute URI it is relative to.
            /// @param Other The other URI to resolve against.  Must me relative.
            /// @return Returns a new absolute URI to the referenced resource.
            URI Resolve(const URI& Relative) const;

            /// @brief Parses the contents of a String into this URI.
            /// @remarks This will overwrite the contents of this URI.  If there is a failure during parsing the contents of this URI will be cleared.
            /// @param ToParse The String to be parsed.
            /// @return Returns a reference to this.
            URI& ParseFromString(const String& ToParse);
            /// @brief Combines the set components of this URI into a usable String for transmission.
            /// @return Returns a valid String representation of this URI.
            String ConvertToString() const;

            /// @brief Adds a single param/value pair to the Query of this URI.
            /// @remarks Delimiters will automatically be generated when using this method, so any reserved characters that are used in either part will be percent encoded.
            /// If anything is already percent encoded, the percent character will get percent encoded.  So use with care.
            /// @param Param The name of the parameter to append.
            /// @param Value The value of the parameter to append.
            /// @return Returns a reference to this.
            URI& AddQueryParameter(const String& Param, const String& Value);
            /// @brief Gets the set value for a specific parameter in the Query of this URI.
            /// @param Param The name of the parameter to retrieve the value of.
            /// @return Returns the currently set value of the named parameter, or an empty String if it does not exist/is not set.
            String GetParameterValue(const String& Param) const;
            /// @brief Checks if the named parameter exists in this URI.
            /// @param Param The name of the parameter to check for.
            /// @return Returns true of the named Parameter exists in this URI.
            Boole HasQueryParameter(const String& Param) const;

            ///////////////////////////////////////////////////////////////////////////////
            // Raw Component Management

            /// @brief Sets the Scheme component of the URI.
            /// @param Scheme The Scheme to be set, which is used to specify the type of resource referenced by this URI.
            /// @return Returns a reference to this.
            URI& SetScheme(const String& Scheme);
            /// @brief Gets the Scheme component of the URI.
            /// @return Returns a const reference to a String containing the Scheme of this URI.
            const String& GetScheme() const;
            /// @brief Gets whether or not the Scheme component of the URI has been set.
            /// @return Returns true if anything has been set as the Scheme, false otherwise.
            Boole HasScheme() const;

            /// @brief Sets the User Information component of the URI.
            /// @warning URI's are often unencrypted, use this feature with care.
            /// @param UserInfo The User Authentication Information to store in the URI.
            /// @return Returns a reference to this.
            URI& SetUserInfo(const String& UserInfo);
            /// @brief Gets the User Information component of the URI.
            /// @return Returns a const reference to a String containing the User Information of this URI.
            const String& GetUserInfo() const;
            /// @brief Gets whether or not the User Information component of the URI has been set.
            /// @return Returns true if anything has been set for the User Information, false otherwise.
            Boole HasUserInfo() const;

            /// @brief Sets the Host component of the URI.
            /// @param Host The Host on which the resource specified by the URI is located.
            /// @return Returns a reference to this.
            URI& SetHost(const String& Host);
            /// @brief Sets the Host component of the URI.
            /// @param Host The IP address of the Host on which the resource specified by the URI is located.
            /// @return Returns a reference to this.
            URI& SetHost(const IPAddress& Host);
            /// @brief Gets the Host component of the URI.
            /// @return Returns a const reference to a String containing the Host of this URI.
            const String& GetHost() const;
            /// @brief Gets whether or not the Host component of the URI has been set.
            /// @return Returns true if anything has been set for the Host, false otherwise.
            Boole HasHost() const;

            /// @brief Sets the Port component of the URI.
            /// @param Port The Port to connect to on the specified Host.
            /// @return Returns a reference to this.
            URI& SetPort(const UInt16 Port);
            /// @brief Gets the Port component of the URI.
            /// @return Returns a const reference to a String containing the Port of this URI.
            UInt16 GetPort() const;
            /// @brief Gets whether or not the Port component of the URI has been set.
            /// @return Returns true if anything has been set for the Port, false otherwise.
            Boole HasPort() const;

            /// @brief Sets the Path component of the URI.
            /// @param Port The Path on the host where the resource is located.
            /// @return Returns a reference to this.
            URI& SetPath(const String& Port);
            /// @brief Gets the Path component of the URI.
            /// @return Returns a const reference to a String containing the Path of this URI.
            const String& GetPath() const;
            /// @brief Gets whether or not the Path component of the URI has been set.
            /// @return Returns true if anything has been set for the Path, false otherwise.
            Boole HasPath() const;

            /// @brief Sets the Query component of the URI.
            /// @param Query Additional non-hierarchical data that is optional.
            /// @return Returns a reference to this.
            URI& SetQuery(const String& Query);
            /// @brief Gets the Query component of the URI.
            /// @return Returns a const reference to a String containing the Query of this URI.
            const String& GetQuery() const;
            /// @brief Gets whether or not the Query component of the URI has been set.
            /// @return Returns true if anything has been set as a Query, false otherwise.
            Boole HasQuery() const;

            /// @brief Sets the Fragment component of the URI.
            /// @param Fragment The identifier for the specific subsection of the resource to retrieve.
            /// @return Returns a reference to this.
            URI& SetFragment(const String& Fragment);
            /// @brief Gets the Fragment component of the URI.
            /// @return Returns a const reference to a String containing the Fragment of this URI.
            const String& GetFragment() const;
            /// @brief Gets whether or not the Fragment component of the URI has been set.
            /// @return Returns true if anything has been set as a Fragment, false otherwise.
            Boole HasFragment() const;

            /// @brief Sets the Authority Fragments of the URI.
            /// @note The Authority of a URI is the User Information, Host, and Port combined.
            /// @exception If the delimiters are improperly set or there is otherwise a parsing error an exception will be thrown.
            /// @param Authority A String containing properly delimited and encoded User Information, Host, and Port values.
            /// @return Returns a reference to this.
            URI& SetAuthority(const String& Authority);
            /// @brief Gets the Authority Fragments of the URI.
            /// @return Returns a String containing the full Authority of this URI.
            String GetAuthority() const;
            /// @brief Gets whether or not any Authority component of the URI has been set.
            /// @return Returns true if anything has been set for the Authority, false otherwise.
            Boole HasAuthority() const;

            ///////////////////////////////////////////////////////////////////////////////
            // Operators

            /// @brief Assignment Operator.
            /// @param Other The other URI to be assigned to this.
            void operator=(const URI& Other);

            /// @brief Equality Comparison Operator.
            /// @param Other The other URI to compare to.
            /// @return Returns true if the two URI's are equal, false otherwise.
            Boole operator==(const URI& Other) const;
            /// @brief Inequality Comparison Operator.
            /// @param Other The other URI to compare to.
            /// @return Returns true if the two URI's are not equal, false otherwise.
            Boole operator!=(const URI& Other) const;
        };//URI
    }//Network
}//Mezzanine

#endif
