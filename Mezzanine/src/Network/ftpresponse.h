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

#ifndef _networkftpresponse_h
#define _networkftpresponse_h

#include "Network/ftpmessage.h"

namespace Mezzanine
{
    namespace Network
    {
        ///////////////////////////////////////////////////////////////////////////////
        /// @brief A response to a command generated by a FTP server.
        /// @details This class supports single-line and multi-line responses.  When accessing lines in
        /// a multi-line response keep in mind that the first and last lines contain the response code
        /// and any additional/optional text associated with the response.  Intermediary lines containing
        /// additional information regarding the command being responded to begin at index 1, and end at
        /// one before the last line.
        ///////////////////////////////////////
        class MEZZ_LIB FTPResponse : public FTPMessage
        {
        public:
            /// @brief Container type for one or more lines in a response.
            typedef std::vector<String>            LineContainer;
            /// @brief Iterator type for each line in a response.
            typedef LineContainer::iterator        LineIterator;
            /// @brief Const Iterator type for each line in a response.
            typedef LineContainer::const_iterator  ConstLineIterator;
        protected:
            /// @internal
            /// @brief Container storing each line in this response.
            LineContainer ResponseLines;
            /// @internal
            /// @brief The 3 digit response code that identifies the nature of the response from the server.
            FTPResponseCode ResponseCode;

            /// @internal
            /// @brief Converts the first three characters in the range into a 3 digit response code.
            /// @param CurrIt An iterator at the start of the range to be parsed.
            /// @param EndIt An iterator at the end of the range to be parsed.  Parsing may or may not reach this point.
            /// @return Returns true if the parse was successful or false if there was an error, such as fewer than 3 characters in the range, or a non-digit is found.
            Boole ParseResponseCode(StringIterator CurrIt, const StringIterator EndIt);

            /// @internal
            /// @brief Parses/verifies a single line of a FTP response.
            /// @param CurrIt An iterator at the start of the range to be parsed.
            /// @param EndIt An iterator at the end of the range to be parsed.  Parsing may or may not reach this point.
            /// @param Line The String to be populated with the response line that was retrieved.
            /// @return Returns true if the parse was successful or false if there was an error.
            Boole GetResponseLine(StringIterator& CurrIt, const StringIterator EndIt, String& Line);
        public:
            /// @brief Invalid constructor.
            /// @remarks This will initialize with an invalid response code.
            FTPResponse();
            /// @brief One-line response constructor.
            /// @remarks This constructor will not insert a hyphen into the response line generated, so it is not
            /// appropriate for use with multi-line responses.
            /// @param Code The code describing the type of response.
            /// @param Description The optional description text to go along with the status code.
            FTPResponse(const Network::FTPResponseCode Code, const String& Description);
            /// @brief String constructor.
            /// @param Response The complete response to be parsed.
            FTPResponse(const String& Response);
            /// @brief Class destructor.
            ~FTPResponse();

            ///////////////////////////////////////////////////////////////////////////////
            // Core Operations

            /// @copydoc FTPMessage::Compose() const
            String Compose() const;
            /// @copydoc FTPMessage::Decompose(const String&)
            Boole Decompose(const String& Message);
            /// @copydoc FTPMessage::Decompose(StringIterator&,const StringIterator)
            Boole Decompose(StringIterator& CurrIt, const StringIterator EndIt);

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            /// @brief Checks if a number is a valid response code.
            /// @param Code The code, as a Whole, to be checked.
            /// @return Returns true if the number provided is a valid response code defined by an RFC, false otherwise.
            static Boole CodeIsValid(const Whole Code);

            /// @brief Sets the code describing this response.
            /// @param Code The response code to be set.
            void SetCode(const Network::FTPResponseCode Code);
            /// @brief Gets the code describing this response.
            /// @return Returns a FTPResponseCode enum value describing the type of response this is.
            Network::FTPResponseCode GetCode() const;

            /// @brief Adds a new line to this response.
            /// @note This should be used with expert care.  No sanity checks are performed in this method
            /// and the standard mandates some basic things be provided. @n
            /// For example the first line must start with 3 digits and then either a space or hyphen
            /// (depending on multi-line) then possibly some additional text.  If the response is multi-line
            /// then the final line must contain the 3 digit response code then may contain optional text. @n
            /// If any text follows the end line then it will be ignored and may cause problems when future
            /// parsing is expected.  In short, there are a lot of wrong ways to use this method.  An
            /// understanding of the FTP specification helps.  Use with care.
            /// @param NewLine The line to be added.
            void AddLine(const String& NewLine);
            /// @brief Gets a line in this response
            /// @param Index The index of the line to be retrieved.
            /// @return Returns the line at the specified index.
            const String& GetLine(const Whole Index) const;
            /// @brief Gets the number of lines in this response.
            /// @return Returns the number of lines currently in this response.  0 would indicate an invalid state.
            Whole GetNumLines() const;
            /// @brief Clears all lines from this response.
            void ClearLines();

            /// @brief Checks to see if this is a multi-line response.
            /// @return Returns true if this response contains multiple lines of text, false otherwise.
            Boole IsMultiline() const;
        };//FTPResponse
    }//Network
}//Mezzanine

#endif
