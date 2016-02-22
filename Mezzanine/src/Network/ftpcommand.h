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

#ifndef _networkftpcommand_h
#define _networkftpcommand_h

#include "Network/ftpmessage.h"

namespace Mezzanine
{
    namespace Network
    {
        ///////////////////////////////////////////////////////////////////////////////
        /// @brief A command that can be issued to an FTP server.
        ///////////////////////////////////////
        class MEZZ_LIB FTPCommand : public FTPMessage
        {
        protected:
            /// @internal
            /// @brief The arguments of the command.  Not all commands require this.
            String CommandArgs;
            /// @internal
            /// @brief The command being issued.
            FTPCommandList CommandType;
        public:
            /// @brief Invalid constructor.
            /// @remarks This will initialize with an invalid command.
            FTPCommand();
            /// @brief String constructor.
            /// @param Command The complete command to be parsed.
            FTPCommand(const String& Command);
            /// @brief Descriptive constructor.
            /// @param Type The command to be issued.
            /// @param Args The arguments of the command.  Not all commands require this.
            FTPCommand(const Network::FTPCommandList Type, const String& Args = "");
            /// @brief Class destructor.
            ~FTPCommand();

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

            /// @brief Converts a FTPCommandList enum value to it's text representation.
            /// @remarks This will only return the base command, without any arguments.
            /// @param Command The command enum value to be converted.
            /// @return Returns a String containing the converted enum value.
            static String ConvertCommand(const Network::FTPCommandList Command);
            /// @brief Converts a text FTP command to it's FTPCommandList enum value.
            /// @remarks This expects just the command without any arguments.
            /// @param Command The text representation of the command to be converted.
            /// @return Returns a FTPCommandList enum value representing the converted String.
            static Network::FTPCommandList ConvertCommand(const String& Command);

            /// @brief Sets the command to be sent.
            /// @param Command The command to be set.
            void SetCommand(const Network::FTPCommandList Command);
            /// @brief Gets the comment to be sent.
            /// @return Returns an FTPCommandList enum value representing the specific command to be sent.
            Network::FTPCommandList GetCommand() const;
            /// @brief Sets the arguments for the command to be issued.
            /// @remarks Not all commands require arguments, and some commands require very specific formats for their arguments.
            /// @param Args The arguments for the command.
            void SetArguments(const String& Args);
            /// @brief Gets the arguments for the command to be issued.
            /// @return Returns a const String reference to the arguments for the command.
            const String& GetArguments() const;
        };//FTPCommand
    }//Network
}//Mezzanine

#endif
