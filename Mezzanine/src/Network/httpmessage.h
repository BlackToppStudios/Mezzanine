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

#ifndef _networkhttpmessage_h
#define _networkhttpmessage_h

#include "version.h"
#include "Network/networkenumerations.h"

namespace Mezzanine
{
    namespace Network
    {
        ///////////////////////////////////////////////////////////////////////////////
        /// @brief Convenience class for storing Header fields in internet messaging protocols.
        ///////////////////////////////////////
        struct MEZZ_LIB HeaderField
        {
            /// @brief The name of the header field.
            String HeaderName;
            /// @brief The value of the header field.
            String HeaderValue;

            /// @brief Constructor.
            /// @param Name The name of the Header Field.
            /// @param Value The value of the Header Field.
            HeaderField(const String& Name, const String& Value) :
                HeaderName(Name),
                HeaderValue(Value)
                {  }
        };//HeaderField

        ///////////////////////////////////////////////////////////////////////////////
        /// @brief A base class for HTTP text parsing classes.
        ///////////////////////////////////////
        class MEZZ_LIB HTTPMessage
        {
        public:
            /// @brief Convenience typedef for String iterators.
            typedef String::const_iterator                StringIterator;
            /// @brief Container type for Header Fields in this class.
            typedef std::vector<HeaderField>              HeaderFieldContainer;
            /// @brief Iterator type for Header Fields in this class.
            typedef HeaderFieldContainer::iterator        HeaderFieldIterator;
            /// @brief Const Iterator type for Header Fields in this class.
            typedef HeaderFieldContainer::const_iterator  ConstHeaderFieldIterator;
        protected:
            /// @internal
            /// @brief The maximum number of allowed header fields in this message.
            Whole MaxHeaderFields;
            /// @internal
            /// @brief The version component for this response.
            SimpleVersion MessageVersion;
            /// @internal
            /// @brief The body of the message.
            String MessageBody;
            /// @internal
            /// @brief A container of fields to populate the message header with.
            HeaderFieldContainer MessageFields;

            /// @internal
            /// @brief Parses the HTTP version from a provided string.
            /// @param ToParse The String to retrieve the version information from.
            /// @return Returns true if the parse was successful, false if it failed.
            Boole ParseHTTPVersion(const String& ToParse);
            /// @internal
            /// @brief Parses the information contained in the Message Header.
            /// @param CurrIt An iterator to the current character being processed in the string being parsed.
            /// @param EndIt An iterator to the last character in the string to be parsed.
            /// @return Returns true if the parse was successful, false if it failed.
            virtual Boole ParseHTTPHeader(StringIterator& CurrIt, const StringIterator EndIt) = 0;
            /// @internal
            /// @brief Parses the HTTP fields from a provided string.
            /// @param CurrIt An iterator to the current character being processed in the string being parsed.
            /// @param EndIt An iterator to the last character in the string to be parsed.
            /// @return Returns true if the parse was successful, false if it failed.
            Boole ParseHTTPFields(StringIterator& CurrIt, const StringIterator EndIt);

            /// @internal
            /// @brief Extracts all characters from CurrIt until the first space encountered or EndIt, whichever comes first.
            /// @param CurrIt An iterator to the current character being processed in the string being parsed.
            /// @param EndIt An iterator to the last character in the string to be parsed.
            /// @param Component The string to which the extracted characters will be appended.
            /// @return Returns true if the end of line was detected while parsing, false otherwise.
            Boole GetMessageComponent(StringIterator& CurrIt, const StringIterator EndIt, String& Component);
            /// @internal
            /// @brief Convenience method that will ignore and advance passed all characters until one passed the first newline character detected.
            /// @param CurrIt An iterator to the current character being processed in the string being parsed.
            /// @param EndIt An iterator to the last character in the string to be parsed.
            void AdvanceToNewline(StringIterator& CurrIt, const StringIterator EndIt);
        //public:
            /// @brief Blank Constructor.
            HTTPMessage();
            /// @brief UInt Version Constructor.
            HTTPMessage(const UInt16 VerMajor, const UInt16 VerMinor);
            /// @brief Version Constructor.
            HTTPMessage(const SimpleVersion& Version);
            /// @brief Class destructor.
            virtual ~HTTPMessage();
        public:
            ///////////////////////////////////////////////////////////////////////////////
            // Core Operations

            /// @brief Creates a completed Message that can be sent across an HTTP connection.
            /// @return Returns a String containing the generated Message.
            virtual String Compose() const = 0;
            /// @brief Populates the members of this class with data from a text HTTP Message.
            /// @param Message The String containing the Message to be decomposed.
            /// @return Returns true if the String was successfully decomposed.
            virtual Boole Decompose(const String& Message) = 0;
            /// @brief Populates the members of this class with data from a text HTTP Message.
            /// @param CurrIt An iterator at the start of the range to be parsed.
            /// @param EndIt An iterator at the end of the range to be parsed.  Parsing may or may not reach this point.
            /// @return Returns true if the String was successfully decomposed.
            virtual Boole Decompose(StringIterator& CurrIt, const StringIterator EndIt) = 0;

            ///////////////////////////////////////////////////////////////////////////////
            // Parameter setting/configuration

            /// @brief Sets both the major and minor version of this message via SimpleVersion.
            /// @param Version The version to be applied to this message.
            void SetHTTPVersion(const SimpleVersion& Version);
            /// @brief Gets both the major and minor version of this message.
            /// @return Returns a const reference to a SimpleVersion containing the major and minor versions of this message.
            const SimpleVersion& GetHTTPVersion() const;

            /// @brief Sets both the major and minor version of this message.
            /// @param Major The major portion of the version to set.
            /// @param Minor The minor portion of the version to set.
            void SetHTTPVersion(const UInt16 Major, const UInt16 Minor);

            /// @brief Sets the major version of this message.
            /// @param Major The major portion of the version to set.
            void SetHTTPMajorVersion(const UInt16 Major);
            /// @brief Gets the major version of this message.
            /// @return Returns the major version component of this message.
            UInt16 GetHTTPMajorVersion() const;
            /// @brief Sets the minor version of this message.
            /// @param Minor The minor portion of the version to set.
            void SetHTTPMinorVersion(const UInt16 Minor);
            /// @brief Gets the minor version of this message.
            /// @return Returns the minor version component of this message.
            UInt16 GetHTTPMinorVersion() const;

            /// @brief Sets the message body containing additional information.
            /// @remarks Request bodies are optional or just unnecessary for most request methods.  Be sure you need a body for your request before setting one on a request.
            /// @param Body The body of the message.
            void SetBody(const String& Body);
            /// @brief Gets the message body containing additional information.
            /// @return Returns a String containing the body of the message.
            const String& GetBody() const;

            /// @brief Sets how many header fields can be added to this message during parsing.
            /// @remarks Setting a sane maximum number of header fields is designed to prevent certain types of DDOS attacks on servers.
            /// @param MaxFields The maximum number of header fields that can be parsed by this message.  Initial value: 100.
            void SetMaxFields(const Whole MaxFields);
            /// @brief Gets how many header fields can be added to this message during parsing.
            /// @return Returns a whole representing how many header fields can be parsed.
            Whole GetMaxFields() const;

            /// @brief Sets a Name-Value pair for a header field in the message.
            /// @param FieldName The name of the field to be set.
            /// @param FieldValue The value to be set for the named header field.
            void SetField(const String& FieldName, const String& FieldValue);
            /// @brief Gets a Name-Value pair for a header field in the message.
            /// @param FieldName The name of the field to retrieve.
            /// @return Returns a String containing the set value for the named header.  If none is set or the named field does not exist then an empty String is returned.
            const String& GetField(const String& FieldName) const;
            /// @brief Checks if this message has the named header field.
            /// @note Fields should only ever be created when "SetField" is called, but there is no guarantee that the value for that field will be set.
            /// @param FieldName The name of the field to check for.
            /// @return Returns true if this message has the named field, false otherwise.
            Boole HasField(const String& FieldName) const;
            /// @brief Removes a specific field from this message by name.
            /// @param FieldName The name of the field to be removed.
            void RemoveField(const String& FieldName);
            /// @brief Removes every field from this message.
            void RemoveAllFields();

            ///////////////////////////////////////////////////////////////////////////////
            // Recommended Header Field Convenience Methods

            /// @brief Sets the Connection Header.
            /// @remarks In most cases this will just specify whether the connection is expected to be closed immediately after the transaction or kept open (close vs keep-alive).
            /// @param Connection Control options for the current HTTP connection.
            void SetConnectionHeader(const String& Connection);
            /// @brief Gets the Connection Header.
            /// @return Returns a String containing control options for the current HTTP connection, or blank if the field is not set.
            const String& GetConnectionHeader() const;
            /// @brief Sets the Content-Length Header.
            /// @remarks This header is required any time a message body has contents and is not chunked.
            /// @param ContentLength The length of the message body in bytes.
            void SetContentLengthHeader(const String& ContentLength);
            /// @brief Gets the Content-Length Header.
            /// @return Returns a String containing containing the length of the message body in bytes, or blank if the field is not set.
            const String& GetContentLengthHeader() const;
            /// @brief Sets the Content-Type Header.
            /// @remarks This header is required any time a message body has contents.
            /// @param ContentType One or more descriptors on how the data in the message body is presented, such if it's plain text, XML, or a form.
            void SetContentTypeHeader(const String& ContentType);
            /// @brief Gets the Content-Type Header.
            /// @return Returns a String containing one or more descriptors on how the data in the message body is presented, such if it's plain text, XML, or a form.  Can also return blank if not set.
            const String& GetContentTypeHeader() const;
        };//HTTPMessage
    }//Network
}//Mezzanine

#endif
