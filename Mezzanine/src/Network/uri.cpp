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

#ifndef _networkuri_cpp
#define _networkuri_cpp

#include "Network/uri.h"
#include "Network/networkenumerations.h"

#include "stringtool.h"
#include "exception.h"

// For std::setw
#include <iomanip>

namespace Mezzanine
{
    namespace Network
    {
        const String URI::GenDelims = ":/?#[]@";
        const String URI::SubDelims = "!$&'()*+,;=";

        void BuildURIPath(String& ToBuild, const StringVector& Segments)
        {
            ToBuild.clear();
            StringVector::const_iterator SegIt = Segments.begin();
            const StringVector::const_iterator LastIt = --Segments.end();
            while( SegIt != Segments.end() )
            {
                ToBuild.append( *SegIt );
                if( SegIt != LastIt ) {
                    ToBuild.append(1,'/');
                }
                ++SegIt;
            }
        }

        String MergeURIPaths(const String& Base, const String& Relative)
        {
            String Ret;
            StringVector PathSegments;
            StringVector MergedSegments;
            if( !Base.empty() ) {
                StringVector BaseSegs = StringTools::Split(Base,"/",std::numeric_limits<Whole>::max());
                if( *(Base.rbegin()) != '/' ) {
                    BaseSegs.pop_back();
                }
                PathSegments.insert(PathSegments.end(),BaseSegs.begin(),BaseSegs.end());
            }
            if( !Relative.empty() ) {
                StringVector RelativeSegs = StringTools::Split(Relative,"/",std::numeric_limits<Whole>::max());
                PathSegments.insert(PathSegments.end(),RelativeSegs.begin(),RelativeSegs.end());
            }
            for( StringVector::const_iterator SegIt = PathSegments.begin() ; SegIt != PathSegments.end() ; ++SegIt )
            {
                if( (*SegIt) == ".." ) {
                    if( !MergedSegments.empty() ) {
                        MergedSegments.pop_back();
                    }
                }else if( (*SegIt) != "." ){
                    MergedSegments.push_back( (*SegIt) );
                }
            }
            BuildURIPath(Ret,MergedSegments);
            if( !Relative.empty() && *(Relative.rbegin()) == '/' ) {
                Ret.append(1,'/');
            }
            return Ret;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // URI Methods

        URI::URI() :
            URIPort(0)
            {  }

        URI::URI(const URI& Other) :
            URIScheme(Other.URIScheme),
            URIUserInfo(Other.URIUserInfo),
            URIHost(Other.URIHost),
            URIPath(Other.URIPath),
            URIQuery(Other.URIQuery),
            URIFragment(Other.URIFragment),
            URIPort(Other.URIPort)
            {  }

        URI::URI(const String& ToParse)
            { this->ParseFromString(ToParse); }

        URI::URI(const String& Scheme, const String& UserInfo, const String& Host, const String& Port, const String& Path, const String& Query, const String& Fragment) :
            URIScheme(Scheme),
            URIUserInfo(UserInfo),
            URIHost(Host),
            URIPath(Path),
            URIQuery(Query),
            URIFragment(Fragment),
            URIPort(StringTools::ConvertToUInt16(Port))
            {  }

        URI::URI(const String& Scheme, const String& UserInfo, const String& Host, const UInt16 Port, const String& Path, const String& Query, const String& Fragment) :
            URIScheme(Scheme),
            URIUserInfo(UserInfo),
            URIHost(Host),
            URIPath(Path),
            URIQuery(Query),
            URIFragment(Fragment),
            URIPort(Port)
            {  }

        URI::~URI()
            {  }

        void URI::ParseScheme(StringIterator CurrIt, const StringIterator EndIt)
        {
            this->URIScheme.clear();
            if( !isalpha( (*CurrIt) ) ) {
                MEZZ_EXCEPTION(ExceptionBase::SYNTAX_ERROR_EXCEPTION,"The first character of a URI Scheme must be a letter.");
            }

            while( CurrIt != EndIt && (*CurrIt) != ':' )
            {
                if( StringTools::IsAlphanumeric( (*CurrIt) ) || (*CurrIt) == '+' || (*CurrIt) == '.' || (*CurrIt) == '-' ) {
                    this->URIScheme.append( 1, static_cast<Char8>( tolower( (*CurrIt) ) ) );
                    ++CurrIt;
                }else{
                    MEZZ_EXCEPTION(ExceptionBase::SYNTAX_ERROR_EXCEPTION,"An invalid character was detected when parsing URI Scheme.");
                }
            }
        }

        void URI::ParseUserInfo(StringIterator CurrIt, const StringIterator EndIt)
        {
            this->URIUserInfo.clear();

            while( CurrIt != EndIt && (*CurrIt) != '@' )
            {
                if( (*CurrIt) == ':' || URI::GenDelims.find( (*CurrIt) ) == String::npos ) {
                    this->URIUserInfo.append( 1, (*CurrIt) );
                    ++CurrIt;
                }else{
                    MEZZ_EXCEPTION(ExceptionBase::SYNTAX_ERROR_EXCEPTION,"An invalid character was detected when parsing URI User Information.");
                }
            }
        }

        void URI::ParseHost(StringIterator CurrIt, const StringIterator EndIt)
        {
            this->URIHost.clear();

            while( CurrIt != EndIt && (*CurrIt) != ':' && (*CurrIt) != '/' )
            {
                if( StringTools::IsAlphanumeric( (*CurrIt) ) || (*CurrIt) == '-' || (*CurrIt) == '_' || (*CurrIt) == '.' ) {
                    this->URIHost.append( 1, static_cast<Char8>( tolower( (*CurrIt) ) ) );
                    ++CurrIt;
                }else{
                    MEZZ_EXCEPTION(ExceptionBase::SYNTAX_ERROR_EXCEPTION,"An invalid character was detected when parsing URI Host.");
                }
            }
        }

        void URI::ParsePort(StringIterator CurrIt, const StringIterator EndIt)
        {
            this->URIPort = 0;

            String PortStr;
            while( CurrIt != EndIt && (*CurrIt) != '/' )
            {
                if( StringTools::IsDigit( (*CurrIt) ) ) {
                    PortStr.append( 1, (*CurrIt) );
                    ++CurrIt;
                }else{
                    MEZZ_EXCEPTION(ExceptionBase::SYNTAX_ERROR_EXCEPTION,"An invalid character was detected when parsing URI Port.");
                }
            }

            if( !PortStr.empty() ) {
                this->URIPort = StringTools::ConvertToUInt16( PortStr );
            }
        }

        void URI::ParsePath(StringIterator CurrIt, const StringIterator EndIt)
        {
            this->URIPath.clear();

            while( CurrIt != EndIt && (*CurrIt) != '?' && (*CurrIt) != '#' )
            {
                if( (*CurrIt) != '[' && (*CurrIt) != ']' ) {
                    this->URIPath.append( 1, (*CurrIt) );
                    ++CurrIt;
                }else{
                    MEZZ_EXCEPTION(ExceptionBase::SYNTAX_ERROR_EXCEPTION,"An invalid character was detected when parsing URI Path.");
                }
            }
        }

        void URI::ParseQuery(StringIterator CurrIt, const StringIterator EndIt)
        {
            this->URIQuery.clear();

            while( CurrIt != EndIt && (*CurrIt) != '#' )
            {
                if( (*CurrIt) != '[' && (*CurrIt) != ']' ) {
                    this->URIQuery.append( 1, static_cast<Char8>( (*CurrIt) ) );
                    ++CurrIt;
                }else{
                    MEZZ_EXCEPTION(ExceptionBase::SYNTAX_ERROR_EXCEPTION,"An invalid character was detected when parsing URI Query.");
                }
            }
        }

        void URI::ParseFragment(StringIterator CurrIt, const StringIterator EndIt)
        {
            this->URIFragment.clear();

            while( CurrIt != EndIt )
            {
                if( (*CurrIt) != '#' && (*CurrIt) != '[' && (*CurrIt) != ']' ) {
                    this->URIFragment.append( 1, static_cast<Char8>( (*CurrIt) ) );
                    ++CurrIt;
                }else{
                    MEZZ_EXCEPTION(ExceptionBase::SYNTAX_ERROR_EXCEPTION,"An invalid character was detected when parsing URI Fragment.");
                }
            }
        }

        void URI::ParseAuthority(StringIterator& CurrIt, const StringIterator EndIt)
        {
            // Authorities must start with two slashes
            if( ( CurrIt != EndIt ) && ( (*CurrIt) == '/' && *(CurrIt + 1) == '/' ) ) {
                // Skip over the two slashes we found
                CurrIt += 2;
                // Call the no slash parse
                this->ParseAuthorityNoSlash(CurrIt,EndIt);
            }
        }

        void URI::ParseAuthorityNoSlash(StringIterator& CurrIt, const StringIterator EndIt)
        {
            // Iterators to store the position of the delimiters
            const StringIterator BeginPos = CurrIt;
            StringIterator AtPos = EndIt;
            StringIterator ColonPos = EndIt;
            // This purposefully doesn't check if the delimiter is already set.  For the most part if a delimiter isn't
            // allowed it'll get caught by the specific component parsing.  If we're passed a perfectly legal URI then
            // the only delimiter that could be duplicated is ':', one for the UserInfo component separating the user
            // name and password, and another to denote the start of the port portion of the Authority.  We lump the
            // entire UserInfo component together so we really only care about the second ':'.
            while( CurrIt != EndIt && (*CurrIt) != '/' && (*CurrIt) != '?' && (*CurrIt) != '#' )
            {
                switch( *CurrIt )
                {
                    case '@':  AtPos = CurrIt;     break;
                    case ':':  ColonPos = CurrIt;  break;
                    default:                       break;
                }
                ++CurrIt;
            }

            // By this point the "CurrIt" iterator will have to either be the slash or equal to the end iterator, so
            // use it for the end of the parsed text.
            if( AtPos != EndIt ) {
                this->ParseUserInfo(BeginPos,AtPos);

                if( ColonPos != EndIt ) {
                    this->ParseHost(AtPos+1,ColonPos);
                    this->ParsePort(ColonPos+1,CurrIt);
                }else{
                    this->ParseHost(AtPos+1,CurrIt);
                }
            }else{
                if( ColonPos != EndIt ) {
                    this->ParseHost(BeginPos,ColonPos);
                    this->ParsePort(ColonPos+1,CurrIt);
                }else{
                    this->ParseHost(BeginPos,CurrIt);
                }
            }
        }

        void URI::ParsePathQueryFrag(StringIterator& CurrIt, const StringIterator EndIt)
        {
            if( CurrIt != EndIt ) {
                // Iterators to store the position of the delimiters
                const StringIterator BeginPos = CurrIt;
                StringIterator QuestPos = EndIt;
                StringIterator PoundPos = EndIt;

                while( CurrIt != EndIt )
                {
                    switch( *CurrIt )
                    {
                        case '?':  QuestPos = CurrIt;  break;
                        case '#':  PoundPos = CurrIt;  break;
                        default:                       break;
                    }
                    ++CurrIt;
                }
                // Path components may or may not start with a forward slash, depending on expected parsing or URI scheme used.
                // Some simple paths may not require any slashes at all.  So just go nuts in regards to the path.
                this->ParsePath(BeginPos,( QuestPos != EndIt ? QuestPos : PoundPos ));

                if( QuestPos != EndIt ) {
                    this->ParseQuery(QuestPos+1,( PoundPos != EndIt ? PoundPos : CurrIt ));
                }

                if( PoundPos != EndIt ) {
                    this->ParseFragment(PoundPos+1,CurrIt);
                }
            }
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Percent Encoding

        String URI::PercentEncodeSingle(const Char8 ToEncode)
        {
            StringStream PercentStream;
            PercentStream << '%' << std::hex << std::uppercase << std::setw(2) << int(ToEncode);
            return PercentStream.str();
        }

        URI::DecodeResult URI::PercentDecodeSingle(const String& ToDecode)
        {
            // Hex conversion inspired by POCO.
            DecodeResult Ret(false,0);
            if( ToDecode.size() == 3 ) {
                if( StringTools::IsDigit( ToDecode[1] ) ) {
                    Ret.second += ToDecode[1] - '0';
                }else if( StringTools::IsLowerHexLetter( ToDecode[1] ) ) {
                    Ret.second += ( ToDecode[1] - 'a' ) + 10;
                }else if( StringTools::IsUpperHexLetter( ToDecode[1] ) ) {
                    Ret.second += ( ToDecode[1] - 'A' ) + 10;
                }else{
                    MEZZ_EXCEPTION(ExceptionBase::PARAMETERS_RANGE_EXCEPTION,"Attempting to percent decode non-hexadecimal character.")
                }
                Ret.second = ( Ret.second << 4 );
                if( StringTools::IsDigit( ToDecode[2] ) ) {
                    Ret.second += ToDecode[2] - '0';
                }else if( StringTools::IsLowerHexLetter( ToDecode[2] ) ) {
                    Ret.second += ( ToDecode[2] - 'a' ) + 10;
                }else if( StringTools::IsUpperHexLetter( ToDecode[2] ) ) {
                    Ret.second += ( ToDecode[2] - 'A' ) + 10;
                }else{
                    MEZZ_EXCEPTION(ExceptionBase::PARAMETERS_RANGE_EXCEPTION,"Attempting to percent decode non-hexadecimal character.")
                }
                Ret.first = true;
            }
            return Ret;
        }

        void URI::PercentEncodeSingle(const Char8 ToEncode, String& Destination)
        {
            Destination.append( URI::PercentEncodeSingle(ToEncode) );
        }

        void URI::PercentDecodeSingle(const String& ToDecode, String& Destination)
        {
            DecodeResult ToAppend = URI::PercentDecodeSingle(ToDecode);
            if( ToAppend.first ) {
                Destination.append( 1, ToAppend.second );
            }
        }

        String URI::PercentEncode(const String& ToEncode, const String& AdditionalDelims)
        {
            String Ret;
            URI::PercentEncode(ToEncode,Ret,AdditionalDelims);
            return Ret;
        }

        String URI::PercentDecode(const String& ToDecode, const Boole PlusToSpace)
        {
            String Ret;
            URI::PercentDecode(ToDecode,Ret,PlusToSpace);
            return Ret;
        }

        void URI::PercentEncode(const String& ToEncode, String& Destination, const String& AdditionalDelims)
        {
            StringIterator CurrIt = ToEncode.begin();
            while( CurrIt != ToEncode.end() )
            {
                if( GenDelims.find( *CurrIt ) != String::npos || AdditionalDelims.find( *CurrIt ) != String::npos ) {
                    Destination.append( URI::PercentEncodeSingle( *CurrIt ) );
                }else{
                    Destination.append( 1, (*CurrIt) );
                }
                ++CurrIt;
            }
        }

        void URI::PercentDecode(const String& ToDecode, String& Destination, const Boole PlusToSpace)
        {
            StringIterator CurrIt = ToDecode.begin();
            const StringIterator EndIt = ToDecode.end();
            while( CurrIt != EndIt )
            {
                if( (*CurrIt) == '%' ) {
                    if( ( EndIt - CurrIt >= 4 ) && String(CurrIt,CurrIt+4) == "%%25" ) {
                        Destination.append("%25");
                        CurrIt += 4;
                    }else if( EndIt - CurrIt >= 3 ) {
                        DecodeResult Result = URI::PercentDecodeSingle( String(CurrIt,CurrIt+3) );
                        Destination.append( 1, Result.second );
                        CurrIt += 3;
                    }else{
                        MEZZ_EXCEPTION(ExceptionBase::SYNTAX_ERROR_EXCEPTION,"Percent encode delimiter detected without following characters to decode.");
                    }
                }else if( PlusToSpace && (*CurrIt) == '+' ) {
                    Destination.append( 1, ' ' );
                    ++CurrIt;
                }else{
                    Destination.append( 1, (*CurrIt) );
                    ++CurrIt;
                }
            }
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        UInt16 URI::GetDefaultPort(const String& Scheme)
        {
            if( Scheme == "http" ) {
                return Network::WKP_HTTP;
            }else if( Scheme == "https" ) {
                return Network::WKP_HTTPS;
            }else if( Scheme == "ftp" ) {
                return Network::WKP_FTPControl;
            }else if( Scheme == "ftps" ) {
                return Network::WKP_FTPSControl;
            }else if( Scheme == "ssh" ) {
                return Network::WKP_SSH;
            }else if( Scheme == "telnet" ) {
                return Network::WKP_Telnet;
            }else if( Scheme == "xmpp" ) {
                return Network::WKP_XMPP;
            }else if( Scheme == "nntp" ) {
                return Network::WKP_NNTP;
            }else if( Scheme == "ldap" ) {
                return Network::WKP_LDAP;
            }else if( Scheme == "rtsp" ) {
                return Network::WKP_RTSP;
            }else if( Scheme == "sip" ) {
                return Network::WKP_SIP;
            }else if( Scheme == "sips" ) {
                return Network::WKP_SIPS;
            }else{
                return 0;
            }
        }

        void URI::RemoveDotSegments(String& ToRemove, const Boole Relative)
        {
            if( !ToRemove.empty() ) {
                StringVector RetSegments;
                StringVector PathSegments = StringTools::Split(ToRemove,"/",std::numeric_limits<Whole>::max());
                StringVector::iterator SegIt = PathSegments.begin();

                // If we're operating on a Relative URI, then the content doesn't matter, be it a dot segment or not.
                // If relative is false though, it cannot be a dot segment to be appended.
                // This behavior is explicitly defined in RFC 3986, section 5.2.4, subsection 2A.
                if( Relative ) {
                    RetSegments.push_back( (*SegIt) );
                }else if( (*SegIt) != ".." && (*SegIt) != "." ) {
                    RetSegments.push_back( (*SegIt) );
                }
                ++SegIt;

                // Go over the rest of the path, clearing out frivolous dot segments.
                while( SegIt != PathSegments.end() )
                {
                    if( (*SegIt) == ".." ) {
                        if( !RetSegments.empty() ) {
                            /// @todo This permits dot segments that move up the directory hierarchy to be at the start of the path.
                            /// Research needs to be done to determine if this is explicitly a part of the specification and/or how often it is used.
                            /// Until then some of this code will be commented out to protect from directory traversal attacks.
                            //if( RetSegments.back() == ".." ) {
                            //    RetSegments.push_back( (*SegIt) );
                            //}else{
                                RetSegments.pop_back();
                            //}
                        }
                    }else if( (*SegIt) != "." ) {
                        RetSegments.push_back( (*SegIt) );
                    }
                    ++SegIt;
                }

                // Rebuild the path.
                BuildURIPath(ToRemove,RetSegments);
            }
        }

        Boole URI::IsAbsolute() const
            { return !this->URIScheme.empty(); }

        Boole URI::IsRelative() const
            { return this->URIScheme.empty(); }

        Boole URI::IsOpaque() const
            { return !this->HasAuthority(); }

        Boole URI::IsHierarchical() const
            { return this->HasAuthority(); }

        void URI::SetComponents(const String& Scheme, const String& UserInfo, const String& Host, const String& Port, const String& Path, const String& Query, const String& Fragment)
        {
            this->ParseScheme(Scheme.begin(),Scheme.end());
            this->ParseUserInfo(UserInfo.begin(),UserInfo.end());
            this->ParseHost(Host.begin(),Host.end());
            this->ParsePort(Port.begin(),Port.end());
            this->ParsePath(Path.begin(),Path.end());
            this->ParseQuery(Query.begin(),Query.end());
            this->ParseFragment(Fragment.begin(),Fragment.end());
        }

        void URI::SetComponents(const String& Scheme, const String& UserInfo, const String& Host, const UInt16 Port, const String& Path, const String& Query, const String& Fragment)
        {
            this->ParseScheme(Scheme.begin(),Scheme.end());
            this->ParseUserInfo(UserInfo.begin(),UserInfo.end());
            this->ParseHost(Host.begin(),Host.end());
            this->ParsePath(Path.begin(),Path.end());
            this->ParseQuery(Query.begin(),Query.end());
            this->ParseFragment(Fragment.begin(),Fragment.end());
            this->URIPort = Port;
        }

        void URI::SetComponentsNoParse(const String& Scheme, const String& UserInfo, const String& Host, const String& Port, const String& Path, const String& Query, const String& Fragment)
        {
            this->URIScheme = Scheme;
            this->URIUserInfo = UserInfo;
            this->URIHost = Host;
            this->URIPath = Path;
            this->URIQuery = Query;
            this->URIFragment = Fragment;
            this->URIPort = StringTools::ConvertToUInt16(Port);
        }

        void URI::SetComponentsNoParse(const String& Scheme, const String& UserInfo, const String& Host, const UInt16 Port, const String& Path, const String& Query, const String& Fragment)
        {
            this->URIScheme = Scheme;
            this->URIUserInfo = UserInfo;
            this->URIHost = Host;
            this->URIPort = Port;
            this->URIPath = Path;
            this->URIQuery = Query;
            this->URIFragment = Fragment;
        }

        URI& URI::Normalize()
        {
            // 1) Drop everything that is case in-sensitive to lower case.
            // This should already be handled by the forcing all set methods to go through the parsing methods.
            // An inappropriate upper case letter should never get assigned.

            // 2) Remove frivolous components in the path.  For example a path like "./[anything]/.." means nothing at all.
            this->RemoveDotSegments(this->IsRelative());

            // 3) Decode unreserved percent encoded characters.  Only certain characters need to be percent encoded.  Encoding more than those wastes space.
            if( !this->URIPath.empty() ) {
                String TempPath;
                URI::PercentDecode(this->URIPath,TempPath);
                std::swap(this->URIPath,TempPath);
            }
            if( !this->URIQuery.empty() ) {
                String TempQuery;
                URI::PercentDecode(this->URIQuery,TempQuery);
                std::swap(this->URIQuery,TempQuery);
            }
            if( !this->URIFragment.empty() ) {
                String TempFragment;
                URI::PercentDecode(this->URIFragment,TempFragment);
                std::swap(this->URIFragment,TempFragment);
            }

            // 4) If the port is a default port for the Scheme, it will be removed.
            if( this->URIPort != 0 && this->URIPort == this->GetDefaultPort(this->URIScheme) ) {
                this->URIPort = 0;
            }
            return *this;
        }

        URI& URI::RemoveDotSegments(const Boole Relative)
        {
            URI::RemoveDotSegments(this->URIPath,Relative);
            return *this;
        }

        URI& URI::Swap(URI& Other)
        {
            std::swap(this->URIScheme,Other.URIScheme);
            std::swap(this->URIUserInfo,Other.URIUserInfo);
            std::swap(this->URIHost,Other.URIHost);
            std::swap(this->URIPath,Other.URIPath);
            std::swap(this->URIQuery,Other.URIQuery);
            std::swap(this->URIFragment,Other.URIFragment);
            std::swap(this->URIPort,Other.URIPort);
            return *this;
        }

        URI& URI::Clear()
        {
            this->URIScheme.clear();
            this->URIUserInfo.clear();
            this->URIHost.clear();
            this->URIPath.clear();
            this->URIQuery.clear();
            this->URIFragment.clear();
            this->URIPort = 0;
            return *this;
        }

        Boole URI::IsEmpty() const
        {
            return ( this->URIScheme.empty() &&
                     this->URIUserInfo.empty() &&
                     this->URIHost.empty() &&
                     this->URIPath.empty() &&
                     this->URIQuery.empty() &&
                     this->URIFragment.empty() &&
                     this->URIPort == 0 );
        }

        URI URI::Resolve(const URI& Relative) const
        {
            URI Ret;
            if( Relative.HasScheme() ) {
                Ret.URIScheme = Relative.URIScheme;
                Ret.URIUserInfo = Relative.URIUserInfo;
                Ret.URIHost = Relative.URIHost;
                Ret.URIPort = Relative.URIPort;
                Ret.URIPath = Relative.URIPath;
                Ret.URIQuery = Relative.URIQuery;
            }else{
                if( Relative.HasAuthority() ) {
                    Ret.URIUserInfo = Relative.URIUserInfo;
                    Ret.URIHost = Relative.URIHost;
                    Ret.URIPort = Relative.URIPort;
                    Ret.URIPath = Relative.URIPath;
                    Ret.URIQuery = Relative.URIQuery;
                }else{
                    if( Relative.HasPath() ) {
                        if( Relative.URIPath.at(0) == '/' ) {
                            Ret.URIPath = Relative.URIPath;
                        }else{
                            Ret.URIPath = MergeURIPaths(this->URIPath,Relative.URIPath);
                        }
                        Ret.URIQuery = Relative.URIQuery;
                    }else{
                        Ret.URIPath = this->URIPath;
                        if( Relative.HasQuery() ) {
                            Ret.URIQuery = Relative.URIQuery;
                        }else{
                            Ret.URIQuery = this->URIQuery;
                        }
                    }
                    Ret.URIUserInfo = this->URIUserInfo;
                    Ret.URIHost = this->URIHost;
                    Ret.URIPort = this->URIPort;
                }
                Ret.URIScheme = this->URIScheme;
            }
            Ret.URIFragment = Relative.URIFragment;
            Ret.RemoveDotSegments(Ret.IsRelative());
            return Ret;
        }

        URI& URI::ParseFromString(const String& ToParse)
        {
            this->Clear();
            // Get our iterators
            StringIterator CurrIt = ToParse.begin();
            StringIterator BeginIt = CurrIt;
            const StringIterator EndIt = ToParse.end();

            // Loop until we hit a major delimiter.
            while( CurrIt != EndIt && URI::GenDelims.find( *CurrIt ) == Mezzanine::String::npos )
                { ++CurrIt; }

            // If we caught something then process it, otherwise if no delimiters were found treat everything as the path.
            if( CurrIt != EndIt ) {
                // Hopefully this is a scheme, otherwise this could get a awkward.
                if( (*CurrIt) == ':' ) {
                    this->ParseScheme(BeginIt,CurrIt);
                    // Increment passed the ':'
                    ++CurrIt;
                    // Checks necessary for detecting the appropriate components already exist in these methods, so just pass it in.
                    this->ParseAuthority(CurrIt,EndIt);
                    this->ParsePathQueryFrag(CurrIt,EndIt);
                }else if( (*CurrIt) == '/' ) {
                    // Are there characters between our slash and where we started?  If so we likely have a path.  Otherwise it could be an authority.
                    if( BeginIt == CurrIt ) {
                        this->ParseAuthority(CurrIt,EndIt);
                        this->ParsePathQueryFrag(CurrIt,EndIt);
                    }else{
                        this->ParsePathQueryFrag(BeginIt,EndIt);
                    }
                }else{
                    this->ParsePathQueryFrag(BeginIt,EndIt);
                }
            }else{
                this->ParsePath(BeginIt,EndIt);
            }

            return *this;
        }

        String URI::ConvertToString() const
        {
            StringStream URIStream;
            if( this->HasScheme() ) {
                URIStream << this->URIScheme << ":";
            }
            if( this->HasAuthority() ) {
                URIStream << "//" << this->GetAuthority();
            }
            if( this->HasPath() ) {
                if( this->HasAuthority() && this->URIPath.at(0) != '/' ) {
                    URIStream << "/";
                }
                URIStream << this->GetPath();
            }
            if( this->HasQuery() ) {
                URIStream << "?" << this->GetQuery();
            }
            if( this->HasFragment() ) {
                URIStream << "#" << this->GetFragment();
            }
            return URIStream.str();
        }

        URI& URI::AddQueryParameter(const String& Param, const String& Value)
        {
            StringStream QueryStream;
            QueryStream << "&" << Param << "=" << Value;
            this->URIQuery.append(QueryStream.str());
            return *this;
        }

        String URI::GetParameterValue(const String& Param) const
        {
            size_t ParamStartPos = 0;
            while( ParamStartPos < this->URIQuery.size() )
            {
                size_t ParamEndPos = this->URIQuery.find_first_of("=",ParamStartPos);
                size_t ValueEndPos = this->URIQuery.find_first_of("&;",ParamEndPos + 1);
                if( this->URIQuery.substr(ParamStartPos,ParamEndPos - ParamStartPos) == Param ) {
                    return this->URIQuery.substr(ParamEndPos + 1,ValueEndPos - (ParamEndPos + 1));
                }
                ParamStartPos = ValueEndPos + 1;
            }
            return "";
        }

        Boole URI::HasQueryParameter(const String& Param) const
        {
            size_t ParamStartPos = 0;
            while( ParamStartPos < this->URIQuery.size() )
            {
                size_t ParamEndPos = this->URIQuery.find_first_of("=",ParamStartPos);
                if( this->URIQuery.substr(ParamStartPos,ParamEndPos - ParamStartPos) == Param ) {
                    return true;
                }
                ParamStartPos = this->URIQuery.find_first_of("&;",ParamEndPos) + 1;
            }
            return false;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Raw Component Management

        URI& URI::SetScheme(const String& Scheme)
        {
            this->ParseScheme(Scheme.begin(),Scheme.end());
            return *this;
        }

        const String& URI::GetScheme() const
            { return this->URIScheme; }

        Boole URI::HasScheme() const
            { return !( this->URIScheme.empty() ); }

        URI& URI::SetUserInfo(const String& UserInfo)
        {
            this->ParseUserInfo(UserInfo.begin(),UserInfo.end());
            return *this;
        }

        const String& URI::GetUserInfo() const
            { return this->URIUserInfo; }

        Boole URI::HasUserInfo() const
            { return !( this->URIUserInfo.empty() ); }

        URI& URI::SetHost(const String& Host)
        {
            this->ParseHost(Host.begin(),Host.end());
            return *this;
        }

        URI& URI::SetHost(const IPAddress& Host)
        {
            String IPStr;
            if( Host.GetProtocol() == Network::NLP_IPv4 ) {
                IPStr = Host.GetAsString();
            }else if( Host.GetProtocol() == Network::NLP_IPv6 ) {
                IPStr = ( "[" + Host.GetAsString() + "]" );
            }else{
                MEZZ_EXCEPTION(ExceptionBase::PARAMETERS_EXCEPTION,"Attempting to set URI Host via IP with an invalid IPAddress");
            }
            this->ParseHost(IPStr.begin(),IPStr.end());
            return *this;
        }

        const String& URI::GetHost() const
            { return this->URIHost; }

        Boole URI::HasHost() const
            { return !( this->URIHost.empty() ); }

        URI& URI::SetPort(const UInt16 Port)
            { this->URIPort = Port; return *this; }

        UInt16 URI::GetPort() const
            { return this->URIPort; }

        Boole URI::HasPort() const
            { return ( this->URIPort != 0 ); }

        URI& URI::SetPath(const String& Path)
        {
            this->ParsePath(Path.begin(),Path.end());
            return *this;
        }

        const String& URI::GetPath() const
            { return this->URIPath; }

        Boole URI::HasPath() const
            { return !( this->URIPath.empty() ); }

        URI& URI::SetQuery(const String& Query)
        {
            this->ParseQuery(Query.begin(),Query.end());
            return *this;
        }

        const String& URI::GetQuery() const
            { return this->URIQuery; }

        Boole URI::HasQuery() const
            { return !( this->URIQuery.empty() ); }

        URI& URI::SetFragment(const String& Fragment)
        {
            this->ParseFragment(Fragment.begin(),Fragment.end());
            return *this;
        }

        const String& URI::GetFragment() const
            { return this->URIFragment; }

        Boole URI::HasFragment() const
            { return !( this->URIFragment.empty() ); }

        URI& URI::SetAuthority(const String& Authority)
        {
            StringIterator TempIt = Authority.begin();
            this->ParseAuthorityNoSlash(TempIt,Authority.end());
            return *this;
        }

        String URI::GetAuthority() const
        {
            StringStream URIStream;
            if( !this->URIHost.empty() ) {
                if( !this->URIUserInfo.empty() ) {
                    URIStream << this->URIUserInfo << "@";
                }
                URIStream << this->URIHost;
                if( this->URIPort != 0 ) {
                    URIStream << ":" << this->URIPort;
                }
            }
            return URIStream.str();
        }

        Boole URI::HasAuthority() const
            { return this->HasHost(); } //Other Authority items add description to the host, so without it the Authority means nothing.

        ///////////////////////////////////////////////////////////////////////////////
        // Operators

        void URI::operator=(const URI& Other)
        {
            this->URIScheme = Other.URIScheme;
            this->URIUserInfo = Other.URIUserInfo;
            this->URIHost = Other.URIHost;
            this->URIPath = Other.URIPath;
            this->URIQuery = Other.URIQuery;
            this->URIFragment = Other.URIFragment;
            this->URIPort = Other.URIPort;
        }

        Boole URI::operator==(const URI& Other) const
        {
            return ( this->URIScheme == Other.URIScheme &&
                     this->URIUserInfo == Other.URIUserInfo &&
                     this->URIHost == Other.URIHost &&
                     this->URIPath == Other.URIPath &&
                     this->URIQuery == Other.URIQuery &&
                     this->URIFragment == Other.URIFragment &&
                     this->URIPort == Other.URIPort );
        }

        Boole URI::operator!=(const URI& Other) const
        {
            return ( this->URIScheme != Other.URIScheme ||
                     this->URIUserInfo != Other.URIUserInfo ||
                     this->URIHost != Other.URIHost ||
                     this->URIPath != Other.URIPath ||
                     this->URIQuery != Other.URIQuery ||
                     this->URIFragment != Other.URIFragment ||
                     this->URIPort != Other.URIPort );
        }
    }//Network
}//Mezzanine

#endif
