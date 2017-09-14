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

#ifndef _networkhttpmessage_cpp
#define _networkhttpmessage_cpp

#include "Network/httpmessage.h"

#include "stringtool.h"

namespace
{
    /// @internal
    /// @brief Convenience String for Header Field returns that fail.
    const Mezzanine::String BlankStr = "";

    /*
    /// @brief A comparison method between individual chars that is case-insensitive.
    /// @param First The first char to be compared.
    /// @param Second The second char to be compared.
    /// @return Returns true if the first char is less than the second char when disregarding case.
    Mezzanine::Boole LowerCaseCompare(const Mezzanine::Char8 First, const Mezzanine::Char8 Second)
        { return ( tolower(First) < tolower(Second) ); }

    /// @brief A comparison method that will compare the names of HeaderFields lexicographically in a case-insensitive way.
    /// @param First The first HeaderField to be compared.
    /// @param Second The second HeaderField to be compared.
    /// @return Returns true if the First HeaderField is less than the Second HeaderField.
    Mezzanine::Boole HeaderFieldCompare(const Mezzanine::Network::HeaderField& First, const Mezzanine::Network::HeaderField& Second)
        { return std::lexicographical_compare(First.HeaderName.begin(),First.HeaderName.end(),Second.HeaderName.begin(),Second.HeaderName.end(),LowerCaseCompare); }
    // */

    /// @brief Compares two Strings to see if they are equal in a case-insensitive way.
    /// @param First The first String to be compared.
    /// @param Second The second String to be compared.
    /// @return Returns true if the two Strings are equal when not counting casing, false otherwise.
    Mezzanine::Boole IsEqualCaseInsensitive(const Mezzanine::String& First, const Mezzanine::String& Second)
    {
        if( First.size() != Second.size() ) {
            return false;
        }

        for( Mezzanine::Whole Index = 0 ; Index < First.size() ; ++Index )
        {
            if( tolower( First[Index] ) != tolower( Second[Index] ) ) {
                return false;
            }
        }

        return true;
    }
}

namespace Mezzanine
{
    namespace Network
    {
        HTTPMessage::HTTPMessage() :
            MaxHeaderFields(100),
            MessageVersion(1,0)
            {  }

        HTTPMessage::HTTPMessage(const UInt16 VerMajor, const UInt16 VerMinor) :
            MaxHeaderFields(100),
            MessageVersion(VerMajor,VerMinor)
            {  }

        HTTPMessage::HTTPMessage(const SimpleVersion& Version) :
            MaxHeaderFields(100),
            MessageVersion(Version)
            {  }

        HTTPMessage::~HTTPMessage()
            {  }

        Boole HTTPMessage::ParseHTTPVersion(const String& ToParse)
        {
            if( ToParse.length() == 8 &&
                ToParse.at(6) == '.' &&
                StringTools::StartsWith( ToParse, "http/", false ) )
            {
                this->MessageVersion.Major = static_cast<UInt16>( ToParse.at(5) - '0' );
                this->MessageVersion.Minor = static_cast<UInt16>( ToParse.at(7) - '0' );
                return true;
            }
            return false;
        }

        Boole HTTPMessage::ParseHTTPFields(StringIterator& CurrIt, const StringIterator EndIt)
        {
            this->RemoveAllFields();
            Boole PreColon = true;
            String FieldName;
            String FieldValue;
            while( CurrIt != EndIt && this->MessageFields.size() < this->MaxHeaderFields )
            {
                switch( *CurrIt )
                {
                    case '\r':
                    {
                        // Just skip
                        break;
                    }
                    case '\n':
                    {
                        PreColon = true;
                        //StringTools::ToLowerCase(FieldName);
                        StringTools::Trim(FieldName,true,true);
                        StringTools::Trim(FieldValue,true,true);
                        this->SetField(FieldName,FieldValue);
                        FieldName.clear();
                        FieldValue.clear();
                        if( *(CurrIt + 1) == '\r' && *(CurrIt + 2) == '\n' ) {
                            CurrIt += 3;
                            return true;
                        }
                        break;
                    }
                    case ':':
                    {
                        if( PreColon ) {
                            PreColon = false;
                        }else{
                            FieldValue.push_back( *CurrIt );
                        }
                        break;
                    }
                    default:
                    {
                        if( PreColon ) FieldName.push_back( *CurrIt );
                        else FieldValue.push_back( *CurrIt );
                        break;
                    }
                }
                ++CurrIt;
            }
            return false;
        }

        Boole HTTPMessage::GetMessageComponent(StringIterator& CurrIt, const StringIterator EndIt, String& Component)
        {
            while( (*CurrIt) != ' ' && (*CurrIt) != '\r' && CurrIt != EndIt )
            {
                Component.push_back( *CurrIt );
                ++CurrIt;
            }
            Boole EoL = false;
            if( CurrIt != EndIt ) {
                // Increment passed the end of the line if we're there.  Or just increment into the next space valid character if we're on a space.
                if( (*CurrIt) == '\r' && *(CurrIt + 1) == '\n' ) {
                    EoL = true;
                    CurrIt += 2;
                }else if( (*CurrIt) == ' ' ) {
                    ++CurrIt;
                }
            }
            return EoL;
        }

        void HTTPMessage::AdvanceToNewline(StringIterator& CurrIt, const StringIterator EndIt)
        {
            while( CurrIt != EndIt )
            {
                ++CurrIt;
                if( (*CurrIt) == '\n' ) {
                    ++CurrIt;
                    break;
                }
            }
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Core Operations

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        void HTTPMessage::SetHTTPVersion(const SimpleVersion& Version)
            { this->MessageVersion = Version; }

        const SimpleVersion& HTTPMessage::GetHTTPVersion() const
            { return this->MessageVersion; }

        void HTTPMessage::SetHTTPVersion(const UInt16 Major, const UInt16 Minor)
            { this->MessageVersion.Major = Major;  this->MessageVersion.Minor = Minor; }

        void HTTPMessage::SetHTTPMajorVersion(const UInt16 Major)
            { this->MessageVersion.Major = Major; }

        UInt16 HTTPMessage::GetHTTPMajorVersion() const
            { return this->MessageVersion.Major; }

        void HTTPMessage::SetHTTPMinorVersion(const UInt16 Minor)
            { this->MessageVersion.Minor = Minor; }

        UInt16 HTTPMessage::GetHTTPMinorVersion() const
            { return this->MessageVersion.Minor; }

        void HTTPMessage::SetBody(const String& Body)
            { this->MessageBody = Body; }

        const String& HTTPMessage::GetBody() const
            { return this->MessageBody; }

        void HTTPMessage::SetMaxFields(const Whole MaxFields)
            { this->MaxHeaderFields = MaxFields; }

        Whole HTTPMessage::GetMaxFields() const
            { return this->MaxHeaderFields; }

        void HTTPMessage::SetField(const String& FieldName, const String& FieldValue)
        {
            // Code for using std::map
            /*this->MessageFields[ StringTools::LowerCaseCopy(FieldName) ] = FieldValue;// */

            for( HeaderFieldIterator FieldIt = this->MessageFields.begin() ; FieldIt != this->MessageFields.end() ; ++FieldIt )
            {
                if( IsEqualCaseInsensitive( (*FieldIt).HeaderName, FieldName ) ) {
                    (*FieldIt).HeaderValue = FieldValue;
                    return;
                }
            }
            this->MessageFields.push_back( HeaderField(FieldName,FieldValue) );
        }

        const String& HTTPMessage::GetField(const String& FieldName) const
        {
            // Code for using std::map
            /*NameValuePairMap::const_iterator FieldIt = this->MessageFields.find( StringTools::LowerCaseCopy(FieldName) );
            if( FieldIt != this->MessageFields.end() ) {
                return (*FieldIt).second;
            }
            return BlankStr;// */

            for( ConstHeaderFieldIterator FieldIt = this->MessageFields.begin() ; FieldIt != this->MessageFields.end() ; ++FieldIt )
            {
                if( IsEqualCaseInsensitive( (*FieldIt).HeaderName, FieldName ) ) {
                    return (*FieldIt).HeaderValue;
                }
            }
            return BlankStr;
        }

        Boole HTTPMessage::HasField(const String& FieldName) const
        {
            // Code for using std::map
            /*NameValuePairMap::const_iterator FieldIt = this->MessageFields.find( StringTools::LowerCaseCopy(FieldName) );
            return ( FieldIt != this->MessageFields.end() );// */

            for( ConstHeaderFieldIterator FieldIt = this->MessageFields.begin() ; FieldIt != this->MessageFields.end() ; ++FieldIt )
            {
                if( IsEqualCaseInsensitive( (*FieldIt).HeaderName, FieldName ) ) {
                    return true;
                }
            }
            return false;
        }

        void HTTPMessage::RemoveField(const String& FieldName)
        {
            // Code for using std::map
            /*NameValuePairMap::iterator FieldIt = this->MessageFields.find( StringTools::LowerCaseCopy(FieldName) );
            if( FieldIt != this->MessageFields.end() ) {
                this->MessageFields.erase(FieldIt);
            }// */

            for( HeaderFieldIterator FieldIt = this->MessageFields.begin() ; FieldIt != this->MessageFields.end() ; ++FieldIt )
            {
                if( IsEqualCaseInsensitive( (*FieldIt).HeaderName, FieldName ) ) {
                    this->MessageFields.erase(FieldIt);
                    return;
                }
            }
        }

        void HTTPMessage::RemoveAllFields()
        {
            this->MessageFields.clear();
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Recommended Header Field Convenience Methods

        void HTTPMessage::SetConnectionHeader(const String& Connection)
            { this->SetField("Connection",Connection); }

        const String& HTTPMessage::GetConnectionHeader() const
            { return this->GetField("Connection"); }

        void HTTPMessage::SetContentLengthHeader(const String& ContentLength)
            { this->SetField("Content-Length",ContentLength); }

        const String& HTTPMessage::GetContentLengthHeader() const
            { return this->GetField("Content-Length"); }

        void HTTPMessage::SetContentTypeHeader(const String& ContentType)
            { this->SetField("Content-Type",ContentType); }

        const String& HTTPMessage::GetContentTypeHeader() const
            { return this->GetField("Content-Type"); }
    }//Network
}//Mezzanine

#endif
