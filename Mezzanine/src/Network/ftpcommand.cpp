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

#ifndef _networkftpcommand_cpp
#define _networkftpcommand_cpp

#include "Network/ftpcommand.h"

#include "stringtool.h"

namespace Mezzanine
{
    namespace Network
    {
        FTPCommand::FTPCommand() :
            CommandType(Network::FCL_Invalid)
            {  }

        FTPCommand::FTPCommand(const String& Command)
            { this->Decompose(Command); }

        FTPCommand::FTPCommand(const Network::FTPCommandList Type, const String& Args) :
            CommandArgs(Args),
            CommandType(Type)
            {  }

        FTPCommand::~FTPCommand()
            {  }

        ///////////////////////////////////////////////////////////////////////////////
        // Core Operations

        String FTPCommand::Compose() const
        {
            StringStream CommandStream;
            CommandStream << FTPCommand::ConvertCommand(this->CommandType);
            if( !this->CommandArgs.empty() ) {
                CommandStream << " " << this->CommandArgs;
            }
            CommandStream << "\r\n";
            return CommandStream.str();
        }

        Boole FTPCommand::Decompose(const String& Message)
        {
            StringIterator CurrIt = Message.begin();
            return this->Decompose(CurrIt,Message.end());
        }

        Boole FTPCommand::Decompose(StringIterator& CurrIt, const StringIterator EndIt)
        {
            this->CommandArgs.clear();
            this->CommandType = FCL_Invalid;

            if( CurrIt != EndIt ) {
                String Component;
                while( CurrIt != EndIt && !this->IsEndOfLine(CurrIt) )
                {
                    if( (*CurrIt) == ' ' ) {
                        ++CurrIt;
                        break;
                    }else{
                        Component.push_back( *CurrIt );
                        ++CurrIt;
                    }
                }
                this->CommandType = ConvertCommand(Component);
                Component.clear();

                while( CurrIt != EndIt )
                {
                    if( this->IsEndOfLine(CurrIt) ) {
                        CurrIt += 2;
                        break;
                    }else{
                        Component.push_back( *CurrIt );
                        ++CurrIt;
                    }
                }
                this->CommandArgs.assign(Component);

                return this->IsEndOfLine(CurrIt);
            }
            return false;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        String FTPCommand::ConvertCommand(const Network::FTPCommandList Command)
        {
            switch( Command )
            {
                case FCL_ABOR:  return "ABOR";
                case FCL_ACCT:  return "ACCT";
                case FCL_ADAT:  return "ADAT";
                case FCL_ALLO:  return "ALLO";
                case FCL_APPE:  return "APPE";
                case FCL_AUTH:  return "AUTH";
                case FCL_CCC:   return "CCC";
                case FCL_CDUP:  return "CDUP";
                case FCL_CONF:  return "CONF";
                case FCL_CWD:   return "CWD";
                case FCL_DELE:  return "DELE";
                case FCL_ENC:   return "ENC";
                case FCL_EPRT:  return "EPRT";
                case FCL_EPSV:  return "EPSV";
                case FCL_FEAT:  return "FEAT";
                case FCL_HELP:  return "HELP";
                case FCL_HOST:  return "HOST";
                case FCL_LANG:  return "LANG";
                case FCL_LIST:  return "LIST";
                case FCL_LPRT:  return "LPRT";
                case FCL_LPSV:  return "LPSV";
                case FCL_MDTM:  return "MDTM";
                case FCL_MIC:   return "MIC";
                case FCL_MKD:   return "MKD";
                case FCL_MLSD:  return "MLSD";
                case FCL_MLST:  return "MLST";
                case FCL_MODE:  return "MODE";
                case FCL_NLST:  return "NLST";
                case FCL_NOOP:  return "NOOP";
                case FCL_OPTS:  return "OPTS";
                case FCL_PASS:  return "PASS";
                case FCL_PASV:  return "PASV";
                case FCL_PBSZ:  return "PBSZ";
                case FCL_PORT:  return "PORT";
                case FCL_PROT:  return "PROT";
                case FCL_PWD:   return "PWD";
                case FCL_QUIT:  return "QUIT";
                case FCL_REIN:  return "REIN";
                case FCL_REST:  return "REST";
                case FCL_RETR:  return "RETR";
                case FCL_RMD:   return "RMD";
                case FCL_RNFR:  return "RNFR";
                case FCL_RNTO:  return "RNTO";
                case FCL_SITE:  return "SITE";
                case FCL_SIZE:  return "SIZE";
                case FCL_SMNT:  return "SMNT";
                case FCL_STAT:  return "STAT";
                case FCL_STOR:  return "STOR";
                case FCL_STOU:  return "STOU";
                case FCL_STRU:  return "STRU";
                case FCL_SYST:  return "SYST";
                case FCL_TYPE:  return "TYPE";
                case FCL_USER:  return "USER";
                case FCL_XCUP:  return "XCUP";
                case FCL_XCWD:  return "XCWD";
                case FCL_XMKD:  return "XMKD";
                case FCL_XPWD:  return "XPWD";
                case FCL_XRCP:  return "XRCP";
                case FCL_XRMD:  return "XRMD";
                case FCL_XRSQ:  return "XRSQ";
                case FCL_XSEM:  return "XSEM";
                case FCL_XSEN:  return "XSEN";
                case FCL_Invalid:
                default:                 return "";
            }
            return "";
        }

        Network::FTPCommandList FTPCommand::ConvertCommand(const String& Command)
        {
            /// @todo This switch-case, while certainly fast, is massive and not pretty.  It could be possible to generate a hash for each command and assign the hash
            /// value to the enum and convert it directly.  That could reduce the size of this method to about 5 or less lines of code at the cost of some processing
            /// speed IF it can be verified there would be no collisions in the hashes.  It should be decided later if such a thing would be worthwhile.
            Whole ComSize = Command.size();
            if( ComSize == 3 || ComSize == 4 ) {
                String UpperCommand = Command;
                StringTools::ToUpperCase(UpperCommand);
                switch( UpperCommand[0] )
                {
                    case 'A':
                    {
                        switch( UpperCommand[1] )
                        {
                            case 'B':    return FCL_ABOR;
                            case 'C':    return FCL_ACCT;
                            case 'D':    return FCL_ADAT;
                            case 'L':    return FCL_ALLO;
                            case 'P':    return FCL_APPE;
                            case 'U':    return FCL_AUTH;
                        }
                    }
                    case 'C':
                    {
                        switch( UpperCommand[1] )
                        {
                            case 'C':    return FCL_CCC;
                            case 'D':    return FCL_CDUP;
                            case 'O':    return FCL_CONF;
                            case 'W':    return FCL_CWD;
                        }
                    }
                    case 'D':    return FCL_DELE;
                    case 'E':
                    {
                        switch( UpperCommand[1] )
                        {
                            case 'N':    return FCL_ENC;
                            case 'P':
                            {
                                switch( UpperCommand[2] )
                                {
                                    case 'R':    return FCL_EPRT;
                                    case 'S':    return FCL_EPSV;
                                }
                            }
                        }
                    }
                    case 'F':    return FCL_FEAT;
                    case 'H':
                    {
                        switch( UpperCommand[1] )
                        {
                            case 'E':    return FCL_HELP;
                            case 'O':    return FCL_HOST;
                        }
                    }
                    case 'L':
                    {
                        switch( UpperCommand[1] )
                        {
                            case 'A':    return FCL_LANG;
                            case 'I':    return FCL_LIST;
                            case 'P':
                            {
                                switch( UpperCommand[2] )
                                {
                                    case 'R':    return FCL_LPRT;
                                    case 'S':    return FCL_LPSV;
                                }
                            }
                        }
                    }
                    case 'M':
                    {
                        switch( UpperCommand[1] )
                        {
                            case 'D':    return FCL_MDTM;
                            case 'I':    return FCL_MIC;
                            case 'K':    return FCL_MKD;
                            case 'L':
                            {
                                if( UpperCommand.size() > 3 ) {
                                    switch( UpperCommand[3] )
                                    {
                                        case 'D':    return FCL_MLSD;
                                        case 'T':    return FCL_MLST;
                                    }
                                }
                            }
                            case 'O':    return FCL_MODE;
                        }
                    }
                    case 'N':
                    {
                        switch( UpperCommand[1] )
                        {
                            case 'L':    return FCL_NLST;
                            case 'O':    return FCL_NOOP;
                        }
                    }
                    case 'O':    return FCL_OPTS;
                    case 'P':
                    {
                        switch( UpperCommand[1] )
                        {
                            case 'A':
                            {
                                if( UpperCommand.size() > 3 ) {
                                    switch( UpperCommand[3] )
                                    {
                                        case 'S':    return FCL_PASS;
                                        case 'V':    return FCL_PASV;
                                    }
                                }
                            }
                            case 'B':    return FCL_PBSZ;
                            case 'O':    return FCL_PORT;
                            case 'R':    return FCL_PROT;
                            case 'W':    return FCL_PWD;
                        }
                    }
                    case 'Q':    return FCL_QUIT;
                    case 'R':
                    {
                        switch( UpperCommand[1] )
                        {
                            case 'E':
                            {
                                switch( UpperCommand[2] )
                                {
                                    case 'I':    return FCL_REIN;
                                    case 'S':    return FCL_REST;
                                    case 'T':    return FCL_RETR;
                                }
                            }
                            case 'M':    return FCL_RMD;
                            case 'N':
                            {
                                switch( UpperCommand[2] )
                                {
                                    case 'F':    return FCL_RNFR;
                                    case 'T':    return FCL_RNTO;
                                }
                            }
                        }
                    }
                    case 'S':
                    {
                        switch( UpperCommand[1] )
                        {
                            case 'I':
                            {
                                switch( UpperCommand[2] )
                                {
                                    case 'T':    return FCL_SITE;
                                    case 'Z':    return FCL_SIZE;
                                }
                            }
                            case 'M':    return FCL_SMNT;
                            case 'T':
                            {
                                switch( UpperCommand[2] )
                                {
                                    case 'A':    return FCL_STAT;
                                    case 'O':
                                    {
                                        if( UpperCommand.size() > 3 ) {
                                            switch( UpperCommand[3] )
                                            {
                                                case 'R':    return FCL_STOR;
                                                case 'U':    return FCL_STOU;
                                            }
                                        }
                                    }
                                    case 'R':    return FCL_STRU;
                                }
                            }
                            case 'Y':    return FCL_SYST;
                        }
                    }
                    case 'T':    return FCL_TYPE;
                    case 'U':    return FCL_USER;
                    case 'X':
                    {
                        switch( UpperCommand[1] )
                        {
                            case 'C':
                            {
                                switch( UpperCommand[2] )
                                {
                                    case 'U':    return FCL_XCUP;
                                    case 'W':    return FCL_XCWD;
                                }
                            }
                            case 'M':    return FCL_XMKD;
                            case 'P':    return FCL_XPWD;
                            case 'R':
                            {
                                switch( UpperCommand[2] )
                                {
                                    case 'C':    return FCL_XRCP;
                                    case 'M':    return FCL_XRMD;
                                    case 'S':    return FCL_XRSQ;
                                }
                            }
                            case 'S':
                            {
                                if( UpperCommand.size() > 3 ) {
                                    switch( UpperCommand[3] )
                                    {
                                        case 'M':    return FCL_XSEM;
                                        case 'N':    return FCL_XSEN;
                                    }
                                }
                            }// S case
                        }// Second character switch
                    }// X case
                }// First character switch
            }// If we have an appropriate size
            return FCL_Invalid;
        }

        void FTPCommand::SetCommand(const Network::FTPCommandList Command)
            { this->CommandType = Command; }

        Network::FTPCommandList FTPCommand::GetCommand() const
            { return this->CommandType; }

        void FTPCommand::SetArguments(const String& Args)
            { this->CommandArgs = Args; }

        const String& FTPCommand::GetArguments() const
            { return this->CommandArgs; }
    }//Network
}//Mezzanine

#endif
