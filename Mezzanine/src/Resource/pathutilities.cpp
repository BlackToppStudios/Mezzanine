// © Copyright 2010 - 2018 BlackTopp Studios Inc.
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
#ifndef _resourcepathutilities_cpp
#define _resourcepathutilities_cpp

#include "Resource/pathutilities.h"
#include "exception.h"
#include "stringtool.h"

namespace Mezzanine
{
    namespace Resource
    {
        ///////////////////////////////////////////////////////////////////////////////
        // Path Utilities

        String GetDirName(const char* FileName)
        {
            size_t SlashPos = String::npos, CurrPos = 0;
            while( FileName[CurrPos] != '\0' )
            {
                if( FileName[CurrPos] == '\\' || FileName[CurrPos] == '/' ) {
                    SlashPos = CurrPos;
                }
                ++CurrPos;
            }
            if( SlashPos != String::npos ) {
                return String(FileName,SlashPos + 1);
            }
            return String();
        }

        String GetDirName(const String& FileName)
        {
            size_t SlashPos = FileName.find_last_of("\\/");
            if( FileName.npos == SlashPos ) {
                return String();
            }else{
                return FileName.substr(0,SlashPos + 1);
            }
        }

        String GetBaseName(const char* FileName)
        {
            size_t SlashPos = String::npos, CurrPos = 0;
            while( FileName[CurrPos] != '\0' )
            {
                if( FileName[CurrPos] == '\\' || FileName[CurrPos] == '/' ) {
                    SlashPos = CurrPos;
                }
                ++CurrPos;
            }
            if( SlashPos != String::npos ) {
                return String(FileName + (SlashPos + 1),(CurrPos - SlashPos) - 1);
            }
            return String(FileName,CurrPos);
        }

        String GetBaseName(const String& FileName)
        {
            size_t SlashPos = FileName.find_last_of("\\/");
            if( FileName.npos == SlashPos ) {
                return FileName;
            }else{
                return FileName.substr(SlashPos + 1,FileName.npos);
            }
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Separators

        Boole IsDirectorySeparator(const Char8 ToCheck)
        {
            return ( IsDirectorySeparator_Posix(ToCheck) || IsDirectorySeparator_Windows(ToCheck) );
        }

        Boole IsDirectorySeparator_Host(const Char8 ToCheck)
        {
        #ifdef MEZZ_WINDOWS
            return ( ToCheck == '\\' );
        #else
            return ( ToCheck == '/' );
        #endif
        }

        Boole IsDirectorySeparator_Posix(const Char8 ToCheck)
        {
            return ( ToCheck == '/' );
        }

        Boole IsDirectorySeparator_Windows(const Char8 ToCheck)
        {
            return ( ToCheck == '\\' );
        }

        Char8 GetDirectorySeparator_Host()
        {
        #ifdef MEZZ_WINDOWS
            return '\\';
        #else
            return '/';
        #endif
        }

        Char8 GetDirectorySeparator_Posix()
        {
            return '/';
        }

        Char8 GetDirectorySeparator_Windows()
        {
            return '\\';
        }

        Char8 GetDirectorySeparator_Universal()
        {
            return '/';
        }

        Boole IsPathSeparator(const Char8 ToCheck)
        {
            return ( IsPathSeparator_Posix(ToCheck) || IsPathSeparator_Windows(ToCheck) );
        }

        Boole IsPathSeparator_Host(const Char8 ToCheck)
        {
        #ifdef MEZZ_WINDOWS
            return ( ToCheck == ';' );
        #else
            return ( ToCheck == ':' );
        #endif
        }

        Boole IsPathSeparator_Posix(const Char8 ToCheck)
        {
            return ( ToCheck == ':' );
        }

        Boole IsPathSeparator_Windows(const Char8 ToCheck)
        {
            return ( ToCheck == ';' );
        }

        Char8 GetPathSeparator_Host()
        {
        #ifdef MEZZ_WINDOWS
            return ';';
        #else
            return ':';
        #endif
        }

        Char8 GetPathSeparator_Posix()
        {
            return ':';
        }

        Char8 GetPathSeparator_Windows()
        {
            return ';';
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Absolute and Relative Paths

        Boole IsPathAbsolute(const String& ToCheck)
        {
            return IsPathAbsolute_Windows(ToCheck) || IsPathAbsolute_Posix(ToCheck);
        }

        Boole IsPathAbsolute_Host(const String& ToCheck)
        {
        #ifdef MEZZ_WINDOWS
            return IsPathAbsolute_Windows(ToCheck);
        #else
            return IsPathAbsolute_Posix(ToCheck);
        #endif
        }

        Boole IsPathAbsolute_Posix(const String& ToCheck)
        {
            if( !ToCheck.empty() ) {
                return ( ToCheck[0] == '/' && ToCheck.find_first_of('\\') == String::npos );
            }
            return false;
        }

        Boole IsPathAbsolute_Windows(const String& ToCheck)
        {
            if( ToCheck.size() >= 3 ) {
                return ( StringTools::IsAlphaLetter(ToCheck[0]) &&
                         ToCheck[1] == ':' &&
                         ( ToCheck[2] == '\\' || ToCheck[2] == '/' ) );
            }
            return false;
        }

        Boole IsPathRelative(const String& ToCheck)
        {
            return !IsPathAbsolute(ToCheck);
        }

        Boole IsPathRelative_Host(const String& ToCheck)
        {
        #ifdef MEZZ_WINDOWS
            return IsPathRelative_Windows(ToCheck);
        #else
            return IsPathRelative_Posix(ToCheck);
        #endif
        }

        Boole IsPathRelative_Posix(const String& ToCheck)
        {
            if( !ToCheck.empty() ) {
                return ( ToCheck[0] != '/' && ToCheck.find_first_of('\\') == String::npos );
            }
            return false;
        }

        Boole IsPathRelative_Windows(const String& ToCheck)
        {
            return !IsPathAbsolute(ToCheck);
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Path Checks

        Integer GetDirectoryDepth(const String& ToCheck, const Boole ExitIfNegative)
        {
            Integer Depth = 0;
            String DirStr;
            String::const_iterator StrIt = ToCheck.begin();
            if( IsPathAbsolute_Windows(ToCheck) ) {
                StrIt = std::next(StrIt,2);
            }
            while( StrIt != ToCheck.end() )
            {
                if( (*StrIt) == '/' || (*StrIt) == '\\' ) {
                    if( DirStr == ".." ) {
                        Depth -= 1;
                    }else if( !DirStr.empty() && DirStr != "." ) {
                        Depth += 1;
                    }
                    DirStr.clear();

                    if( ExitIfNegative && Depth < 0 ) {
                        break;
                    }
                }else{
                    DirStr.append(1,*StrIt);
                }
                ++StrIt;
            }
            return Depth;
        }

        Boole IsSubPath(const String& BasePath, const String& CheckPath)
        {
            Boole BaseIsAbsolute = IsPathAbsolute(BasePath);
            Boole CheckIsAbsolute = IsPathAbsolute(CheckPath);

            if( BaseIsAbsolute && !CheckIsAbsolute ) {
                MEZZ_EXCEPTION(ExceptionBase::PARAMETERS_EXCEPTION,
                               "Attempting to compare absolute base path with relative subpath.");
            }
            if( !BaseIsAbsolute && CheckIsAbsolute ) {
                MEZZ_EXCEPTION(ExceptionBase::PARAMETERS_EXCEPTION,
                               "Attempting to compare relative base path with absolute subpath.");
            }

            String NormBasePath = RemoveDotSegments(BasePath);
            String NormCheckPath = RemoveDotSegments(CheckPath);
            String::const_iterator BaseIt = NormBasePath.begin();
            String::const_iterator BaseEnd = NormBasePath.end();
            String::const_iterator CheckIt = NormCheckPath.begin();
            String::const_iterator CheckEnd = NormCheckPath.end();

            if( IsPathAbsolute_Windows(NormBasePath) ) {
                std::advance(BaseIt,2);
            }
            if( IsPathAbsolute_Windows(NormCheckPath) ) {
                std::advance(CheckIt,2);
            }

            while( BaseIt != BaseEnd )
            {
                if( CheckIt == CheckEnd || (*BaseIt) != (*CheckIt) ) {
                    return false;
                }

                ++BaseIt;
                ++CheckIt;
            }

            String CheckRemains(CheckIt,CheckEnd);
            return ( GetDirectoryDepth(CheckRemains,true) > 0 );
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Path Utilities

        String BuildPath(const String& PathRoot, const StringVector& ToBuild,
                         const String& FileName, const Boole UseWindowsSlash)
        {
            // Char count gets used to reserve space on the return string
            Whole CharCount = PathRoot.size() + ToBuild.size() + FileName.size();
            StringVector::const_iterator BuildIt = ToBuild.begin();
            std::for_each(BuildIt,ToBuild.cend(),[&CharCount](const String& CurrStr) {
                CharCount += CurrStr.length();
            });
            String Ret;
            Ret.reserve(CharCount);
            Ret.append(PathRoot);
            const Char8 Slash = UseWindowsSlash ? GetDirectorySeparator_Windows() : GetDirectorySeparator_Posix();
            // Build the path
            while( BuildIt != ToBuild.end() )
            {
                Ret.append( *BuildIt );
                Ret.append( 1, Slash );
                ++BuildIt;
            }
            Ret.append(FileName);
            return Ret;
        }

        String RemoveDotSegments(const String& ToRemove)
        {
            Boole AbsolutePath = IsPathAbsolute(ToRemove);
            size_t SplitStartPos = AbsolutePath ? ToRemove.find_first_of("/\\") + 1 : 0;
            size_t SplitEndPos = ToRemove.find_last_of("/\\");
            size_t FileStartPos = ( SplitEndPos != String::npos ? SplitEndPos + 1 : SplitEndPos );
            Boole UseWindowsSlash = ( SplitEndPos != String::npos ? ToRemove[SplitEndPos] == '\\' : false );

            StringVector RetSegments;
            StringVector SplitPath = StringTools::Split(ToRemove.substr(SplitStartPos,SplitEndPos),"/\\");
            for( StringVector::iterator VecIt = SplitPath.begin() ; VecIt != SplitPath.end() ; ++VecIt )
            {
                if( (*VecIt) == ".." ) {
                    if( !RetSegments.empty() ) {
                        RetSegments.pop_back();
                    }else if( !AbsolutePath ) {
                        RetSegments.push_back(*VecIt);
                    }
                }else if( (*VecIt) != "." ) {
                    RetSegments.push_back(*VecIt);
                }
            }
            const String PathRoot = ToRemove.substr(0,SplitStartPos);
            const String FileName = ( FileStartPos != String::npos ? ToRemove.substr(FileStartPos) : String() );
            return std::move( BuildPath(PathRoot,RetSegments,FileName,UseWindowsSlash) );
        }

        String CombinePathAndFileName(const String& FilePath, const String& FileName)
        {
            if( FilePath.empty() ) {
                return FileName;
            }

            String FullPath = FilePath;
            if( !IsDirectorySeparator( FilePath.back() ) ) {
                FullPath.append(1,GetDirectorySeparator_Universal());
            }
            FullPath.append(FileName);
            return FullPath;
        }
    }//Resource
}//Mezzanine

#endif
