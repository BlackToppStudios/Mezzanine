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
            CommandStream << FTPCommand::ConvertCommand(this->CommandType) << " " << this->CommandArgs << "\r\n";
            return CommandStream.str();
        }

        Boole FTPCommand::Decompose(const String& Message)
        {
            String::const_iterator CurrIt = Message.begin();
            return this->Decompose(CurrIt,Message.end());
        }

        Boole FTPCommand::Decompose(StringIterator& CurrIt, const StringIterator EndIt)
        {
            if( CurrIt != EndIt ) {
                String Component;
                while( CurrIt != EndIt && (*CurrIt) != ' ' )
                    { Component.push_back( *CurrIt ); }
                this->CommandType = ConvertCommand(Component);
                Component.clear();

                while( CurrIt != EndIt && !( (*CurrIt) == '\r' && *(CurrIt + 1) == '\n' ) )
                    { Component.push_back( *CurrIt ); }
                this->CommandArgs.assign(Component);

                return ( (*CurrIt) == '\r' && *(CurrIt + 1) == '\n' );
            }
            return false;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        String FTPCommand::ConvertCommand(const Network::FTPCommandList Command)
        {
            switch( Command )
            {
                case Network::FCL_ABOR:  return "ABOR";
                case Network::FCL_ACCT:  return "ACCT";
                case Network::FCL_ADAT:  return "ADAT";
                case Network::FCL_ALLO:  return "ALLO";
                case Network::FCL_APPE:  return "APPE";
                case Network::FCL_AUTH:  return "AUTH";
                case Network::FCL_CCC:   return "CCC";
                case Network::FCL_CDUP:  return "CDUP";
                case Network::FCL_CONF:  return "CONF";
                case Network::FCL_CWD:   return "CWD";
                case Network::FCL_DELE:  return "DELE";
                case Network::FCL_ENC:   return "ENC";
                case Network::FCL_EPRT:  return "EPRT";
                case Network::FCL_EPSV:  return "EPSV";
                case Network::FCL_FEAT:  return "FEAT";
                case Network::FCL_HELP:  return "HELP";
                case Network::FCL_HOST:  return "HOST";
                case Network::FCL_LANG:  return "LANG";
                case Network::FCL_LIST:  return "LIST";
                case Network::FCL_LPRT:  return "LPRT";
                case Network::FCL_LPSV:  return "LPSV";
                case Network::FCL_MDTM:  return "MDTM";
                case Network::FCL_MIC:   return "MIC";
                case Network::FCL_MKD:   return "MKD";
                case Network::FCL_MLSD:  return "MLSD";
                case Network::FCL_MLST:  return "MLST";
                case Network::FCL_MODE:  return "MODE";
                case Network::FCL_NLST:  return "NLST";
                case Network::FCL_NOOP:  return "NOOP";
                case Network::FCL_OPTS:  return "OPTS";
                case Network::FCL_PASS:  return "PASS";
                case Network::FCL_PASV:  return "PASV";
                case Network::FCL_PBSZ:  return "PBSZ";
                case Network::FCL_PORT:  return "PORT";
                case Network::FCL_PROT:  return "PROT";
                case Network::FCL_PWD:   return "PWD";
                case Network::FCL_QUIT:  return "QUIT";
                case Network::FCL_REIN:  return "REIN";
                case Network::FCL_REST:  return "REST";
                case Network::FCL_RETR:  return "RETR";
                case Network::FCL_RMD:   return "RMD";
                case Network::FCL_RNFR:  return "RNFR";
                case Network::FCL_RNTO:  return "RNTO";
                case Network::FCL_SITE:  return "SITE";
                case Network::FCL_SIZE:  return "SIZE";
                case Network::FCL_SMNT:  return "SMNT";
                case Network::FCL_STAT:  return "STAT";
                case Network::FCL_STOR:  return "STOR";
                case Network::FCL_STOU:  return "STOU";
                case Network::FCL_STRU:  return "STRU";
                case Network::FCL_SYST:  return "SYST";
                case Network::FCL_TYPE:  return "TYPE";
                case Network::FCL_USER:  return "USER";
                case Network::FCL_XCUP:  return "XCUP";
                case Network::FCL_XCWD:  return "XCWD";
                case Network::FCL_XMKD:  return "XMKD";
                case Network::FCL_XPWD:  return "XPWD";
                case Network::FCL_XRCP:  return "XRCP";
                case Network::FCL_XRMD:  return "XRMD";
                case Network::FCL_XRSQ:  return "XRSQ";
                case Network::FCL_XSEM:  return "XSEM";
                case Network::FCL_XSEN:  return "XSEN";
                case Network::FCL_Invalid:
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
                            case 'B':    return Network::FCL_ABOR;
                            case 'C':    return Network::FCL_ACCT;
                            case 'D':    return Network::FCL_ADAT;
                            case 'L':    return Network::FCL_ALLO;
                            case 'P':    return Network::FCL_APPE;
                            case 'U':    return Network::FCL_AUTH;
                        }
                    }
                    case 'C':
                    {
                        switch( UpperCommand[1] )
                        {
                            case 'C':    return Network::FCL_CCC;
                            case 'D':    return Network::FCL_CDUP;
                            case 'O':    return Network::FCL_CONF;
                            case 'W':    return Network::FCL_CWD;
                        }
                    }
                    case 'D':    return Network::FCL_DELE;
                    case 'E':
                    {
                        switch( UpperCommand[1] )
                        {
                            case 'N':    return Network::FCL_ENC;
                            case 'P':
                            {
                                switch( UpperCommand[2] )
                                {
                                    case 'R':    return Network::FCL_EPRT;
                                    case 'S':    return Network::FCL_EPSV;
                                }
                            }
                        }
                    }
                    case 'F':    return Network::FCL_FEAT;
                    case 'H':
                    {
                        switch( UpperCommand[1] )
                        {
                            case 'E':    return Network::FCL_HELP;
                            case 'O':    return Network::FCL_HOST;
                        }
                    }
                    case 'L':
                    {
                        switch( UpperCommand[1] )
                        {
                            case 'A':    return Network::FCL_LANG;
                            case 'I':    return Network::FCL_LIST;
                            case 'P':
                            {
                                switch( UpperCommand[2] )
                                {
                                    case 'R':    return Network::FCL_LPRT;
                                    case 'S':    return Network::FCL_LPSV;
                                }
                            }
                        }
                    }
                    case 'M':
                    {
                        switch( UpperCommand[1] )
                        {
                            case 'D':    return Network::FCL_MDTM;
                            case 'I':    return Network::FCL_MIC;
                            case 'K':    return Network::FCL_MKD;
                            case 'L':
                            {
                                if( UpperCommand.size() > 3 ) {
                                    switch( UpperCommand[3] )
                                    {
                                        case 'D':    return Network::FCL_MLSD;
                                        case 'T':    return Network::FCL_MLST;
                                    }
                                }
                            }
                            case 'O':    return Network::FCL_MODE;
                        }
                    }
                    case 'N':
                    {
                        switch( UpperCommand[1] )
                        {
                            case 'L':    return Network::FCL_NLST;
                            case 'O':    return Network::FCL_NOOP;
                        }
                    }
                    case 'O':    return Network::FCL_OPTS;
                    case 'P':
                    {
                        switch( UpperCommand[1] )
                        {
                            case 'A':
                            {
                                if( UpperCommand.size() > 3 ) {
                                    switch( UpperCommand[3] )
                                    {
                                        case 'S':    return Network::FCL_PASS;
                                        case 'V':    return Network::FCL_PASV;
                                    }
                                }
                            }
                            case 'B':    return Network::FCL_PBSZ;
                            case 'O':    return Network::FCL_PORT;
                            case 'R':    return Network::FCL_PROT;
                            case 'W':    return Network::FCL_PWD;
                        }
                    }
                    case 'Q':    return Network::FCL_QUIT;
                    case 'R':
                    {
                        switch( UpperCommand[1] )
                        {
                            case 'E':
                            {
                                switch( UpperCommand[2] )
                                {
                                    case 'I':    return Network::FCL_REIN;
                                    case 'S':    return Network::FCL_REST;
                                    case 'T':    return Network::FCL_RETR;
                                }
                            }
                            case 'M':    return Network::FCL_RMD;
                            case 'N':
                            {
                                switch( UpperCommand[2] )
                                {
                                    case 'F':    return Network::FCL_RNFR;
                                    case 'T':    return Network::FCL_RNTO;
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
                                    case 'T':    return Network::FCL_SITE;
                                    case 'Z':    return Network::FCL_SIZE;
                                }
                            }
                            case 'M':    return Network::FCL_SMNT;
                            case 'T':
                            {
                                switch( UpperCommand[2] )
                                {
                                    case 'A':    return Network::FCL_STAT;
                                    case 'O':
                                    {
                                        if( UpperCommand.size() > 3 ) {
                                            switch( UpperCommand[3] )
                                            {
                                                case 'R':    return Network::FCL_STOR;
                                                case 'U':    return Network::FCL_STOU;
                                            }
                                        }
                                    }
                                    case 'R':    return Network::FCL_STRU;
                                }
                            }
                            case 'Y':    return Network::FCL_SYST;
                        }
                    }
                    case 'T':    return Network::FCL_TYPE;
                    case 'U':    return Network::FCL_USER;
                    case 'X':
                    {
                        switch( UpperCommand[1] )
                        {
                            case 'C':
                            {
                                switch( UpperCommand[2] )
                                {
                                    case 'U':    return Network::FCL_XCUP;
                                    case 'W':    return Network::FCL_XCWD;
                                }
                            }
                            case 'M':    return Network::FCL_XMKD;
                            case 'P':    return Network::FCL_XPWD;
                            case 'R':
                            {
                                switch( UpperCommand[2] )
                                {
                                    case 'C':    return Network::FCL_XRCP;
                                    case 'M':    return Network::FCL_XRMD;
                                    case 'S':    return Network::FCL_XRSQ;
                                }
                            }
                            case 'S':
                            {
                                if( UpperCommand.size() > 3 ) {
                                    switch( UpperCommand[3] )
                                    {
                                        case 'M':    return Network::FCL_XSEM;
                                        case 'N':    return Network::FCL_XSEN;
                                    }
                                }
                            }// S case
                        }// Second character switch
                    }// X case
                }// First character switch
            }// If we have an appropriate size
            return Network::FCL_Invalid;
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
