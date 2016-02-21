// © Copyright 2010 - 2015 BlackTopp Studios Inc.
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

#ifndef _version_h
#define _version_h

#include "datatypes.h"

namespace Mezzanine
{
    ///////////////////////////////////////////////////////////////////////////////
    /// @class SimpleVersion
    /// @brief A very basic class for expressing an API or syntax version.
    /// @details
    ///////////////////////////////////////
    class MEZZ_LIB SimpleVersion
    {
    protected:
        /// @brief Convenience method for converting text to numbers.
        /// @remarks Other methods in the mezzanine to do this exist, but creating a basic implementation here to avoid a dependency for one small simple thing.
        /// @param ToConvert The String to be converted.
        /// @param Var The UInt16 to populate from the provided String.
        void ConvertToUInt16(const String& ToConvert, UInt16& Var)
        {
            StringStream NumConverter;
            NumConverter << ToConvert;
            NumConverter >> Var;
        }
    public:
        /// @brief The Major component of the version to be expressed.
        UInt16 Major;
        /// @brief The Minor component of the version to be expressed.
        UInt16 Minor;

        ///////////////////////////////////////////////////////////////////////////////
        // Construction and Destruction

        /// @brief Blank Constructor.
        SimpleVersion() :
            Major(1),
            Minor(0)
            {  }

        /// @brief Copy Constructor.
        /// @param Other The other SimpleVersion to be copied.
        SimpleVersion(const SimpleVersion& Other) :
            Major(Other.Major),
            Minor(Other.Minor)
            {  }

        /// @brief String Constructor.
        /// @param ToParse The String to be parsed.
        SimpleVersion(const String& ToParse)
            { this->FromString(ToParse); }

        /// @brief Number Constructor.
        /// @param VerMajor The Major component of the version to be expressed.
        /// @param VerMinor The Minor component of the version to be expressed.
        SimpleVersion(const UInt16 VerMajor, const UInt16 VerMinor) :
            Major(VerMajor),
            Minor(VerMinor)
            {  }

        ///////////////////////////////////////////////////////////////////////////////
        // String Conversions

        /// @brief Parses a String expressing a simple version.
        /// @remarks This method expects a short character sequence where the middle character is the Delimiter separating the two components.
        /// @param ToParse The String to be parsed.
        /// @param Delim The delimiter to be used for separating the version components.
        /// @return Returns true if the String was successfully parsed, false if there was a failure.
        Boole FromString(const String& ToParse, const Char8 Delim = '.')
        {
            StringStream NumConverter;
            size_t DelimPos = ToParse.find(Delim);
            if( DelimPos != String::npos ) {
                ConvertToUInt16(ToParse.substr(0,DelimPos),this->Major);
                ConvertToUInt16(ToParse.substr(DelimPos + 1),this->Minor);
                return true;
            }
            return false;
        }
        /// @brief Converts this version into a String.
        /// @param Delim The delimiter to be used for separating the version components.
        /// @return Returns a String expressing this version.
        String ToString(const Char8 Delim = '.') const
        {
            StringStream ConvertStream;
            ConvertStream << this->Major << Delim << this->Minor;
            return ConvertStream.str();
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Operators

        /// @brief Assignment operator.
        /// @param Other The other SimpleVersion to assign to this.
        void operator=(const SimpleVersion& Other)
            { this->Major = Other.Major;  this->Minor = Other.Minor; }

        ///////////////////////////////////////////////////////////////////////////////
        // Comparison Operators

        /// @brief Less than comparison operator.
        /// @param Other The other SimpleVersion to be compared to.
        /// @return Returns true if this compares less than the other SimpleVersion.
        Boole operator<(const SimpleVersion& Other) const
        {
            if( this->Major < Other.Major ) return true;
            else if( this->Major > Other.Major ) return false;
            else return ( this->Minor < Other.Minor );
        }
        /// @brief Less than or equal to comparison operator.
        /// @param Other The other SimpleVersion to be compared to.
        /// @return Returns true if this compares less than or equal to the other SimpleVersion.
        Boole operator<=(const SimpleVersion& Other) const
        {
            if( this->Major < Other.Major ) return true;
            else if( this->Major > Other.Major ) return false;
            else return ( this->Minor <= Other.Minor );
        }
        /// @brief Greater than comparison operator.
        /// @param Other The other SimpleVersion to be compared to.
        /// @return Returns true if this compares greater than the other SimpleVersion.
        Boole operator>(const SimpleVersion& Other) const
        {
            if( this->Major > Other.Major ) return true;
            else if( this->Major < Other.Major ) return false;
            else return ( this->Minor > Other.Minor );
        }
        /// @brief Greater than or equal to comparison operator.
        /// @param Other The other SimpleVersion to be compared to.
        /// @return Returns true if this compares greater than or equal to the other SimpleVersion.
        Boole operator>=(const SimpleVersion& Other) const
        {
            if( this->Major > Other.Major ) return true;
            else if( this->Major < Other.Major ) return false;
            else return ( this->Minor >= Other.Minor );
        }

        /// @brief Equality comparison operator.
        /// @param Other The other SimpleVersion to be compared to.
        /// @return Returns true if the two SimpleVersions are equal.
        Boole operator==(const SimpleVersion& Other) const
            { return ( this->Major == Other.Major && this->Minor == Other.Minor ); }
        /// @brief Inequality comparison operator.
        /// @param Other The other SimpleVersion to be compared to.
        /// @return Returns true if the two SimpleVersions are inequal.
        Boole operator!=(const SimpleVersion& Other) const
            { return ( this->Major != Other.Major || this->Minor != Other.Minor ); }
    };//SimpleVersion

    ///////////////////////////////////////////////////////////////////////////////
    /// @class SemanticVersion
    /// @brief A Version class matching the Semantic Version specification.
    /// @details
    ///////////////////////////////////////
    class MEZZ_LIB SemanticVersion
    {
    protected:
        /// @brief Convenience method for converting text to numbers.
        /// @remarks Other methods in the mezzanine to do this exist, but creating a basic implementation here to avoid a dependency for one small simple thing.
        /// @param ToConvert The String to be converted.
        /// @param Var The UInt16 to populate from the provided String.
        void ConvertToUInt16(const String& ToConvert, UInt16& Var)
        {
            StringStream NumConverter;
            NumConverter << ToConvert;
            NumConverter >> Var;
        }
    public:
        /// @brief The Major component of the version to be expressed.
        UInt16 Major;
        /// @brief The Minor component of the version to be expressed.
        UInt16 Minor;
        /// @brief The Patch component of the version to be expressed.
        UInt16 Patch;
        /// @brief An optional pre-release string providing additional context for the version.
        String PreRelease;
        /// @brief An optional component for build meta data associated with the version.
        String MetaData;

        ///////////////////////////////////////////////////////////////////////////////
        // Construction and Destruction

        /// @brief Blank Constructor.
        SemanticVersion() :
            Major(1),
            Minor(0),
            Patch(0)
            {  }
        /// @brief Copy Constructor.
        /// @param Other The other SemanticVersion to be copied.
        SemanticVersion(const SemanticVersion& Other) :
            Major(Other.Major),
            Minor(Other.Minor),
            Patch(Other.Patch),
            PreRelease(Other.PreRelease),
            MetaData(Other.MetaData)
            {  }
        /// @brief Parse Constructor.
        /// @param ToParse The String to be parsed.
        SemanticVersion(const String& ToParse)
            { this->FromString(ToParse); }
        /// @brief Number Component Constructor.
        /// @param VerMajor The Major component of the version to be expressed.
        /// @param VerMinor The Minor component of the version to be expressed.
        /// @param VerPatch The Patch component of the version to be expressed.
        SemanticVersion(const UInt16 VerMajor, const UInt16 VerMinor, const UInt16 VerPatch) :
            Major(VerMajor),
            Minor(VerMinor),
            Patch(VerPatch)
            {  }
        /// @brief Descriptive Constructor.
        /// @param VerMajor The Major component of the version to be expressed.
        /// @param VerMinor The Minor component of the version to be expressed.
        /// @param VerPatch The Patch component of the version to be expressed.
        /// @param VerDesc An optional description string providing additional context for the version.
        /// @param VerMetaData An optional component for build meta data associated with the version.
        SemanticVersion(const UInt16 VerMajor, const UInt16 VerMinor, const UInt16 VerPatch, const String& VerDesc, const String& VerMetaData) :
            Major(VerMajor),
            Minor(VerMinor),
            Patch(VerPatch),
            PreRelease(VerDesc),
            MetaData(VerMetaData)
            {  }

        ///////////////////////////////////////////////////////////////////////////////
        // String Conversions

        /// @brief Parses a String expressing a simple version.
        /// @remarks This method expects a 3 character sequence where the middle character is the Delimiter separating the two components.
        /// @param ToParse The String to be parsed.
        /// @return Returns true if the String was successfully parsed, false if there was a failure.
        Boole FromString(const String& ToParse)
        {
            Boole Ret = false;
            StringStream NumConverter;
            SemanticVersion TempVer;
            // Find the first dot so we can parse the Major component.
            size_t StartPos = 0;
            size_t DelimPos = ToParse.find('.');
            if( DelimPos != String::npos ) {
                ConvertToUInt16(ToParse.substr(StartPos,DelimPos),TempVer.Major);
                // Find the second dot so we can parse the Minor component.
                StartPos = DelimPos + 1;
                DelimPos = ToParse.find('.',StartPos);
                if( DelimPos != String::npos ) {
                    ConvertToUInt16(ToParse.substr(StartPos,DelimPos - StartPos),TempVer.Minor);
                    // From here, it could end or there could be more delimiters.  Search for the delims but don't freak out if we don't find them.
                    StartPos = DelimPos + 1;
                    DelimPos = ToParse.find_first_of("-+",StartPos);
                    ConvertToUInt16(ToParse.substr(StartPos,DelimPos - StartPos),TempVer.Patch);
                    if( DelimPos != String::npos && ToParse.at(DelimPos) == '-' ) {
                        StartPos = DelimPos + 1;
                        DelimPos = ToParse.find('+',StartPos);
                        TempVer.PreRelease = ToParse.substr(StartPos,DelimPos - StartPos);
                    }
                    if( DelimPos != String::npos && ToParse.at(DelimPos) == '+' ) {
                        StartPos = DelimPos + 1;
                        TempVer.MetaData = ToParse.substr(StartPos);
                    }
                    Ret = true;
                }
            }

            if( Ret ) {
                *this = TempVer;
            }
            return Ret;
        }
        /// @brief Converts this version into a String.
        /// @return Returns a String expressing this version.
        String ToString() const
        {
            StringStream ConvertStream;
            ConvertStream << this->Major << '.' << this->Minor << '.' << this->Patch;
            if( !this->PreRelease.empty() ) {
                ConvertStream << '-' << this->PreRelease;
            }
            if( !this->MetaData.empty() ) {
                ConvertStream << '+' << this->MetaData;
            }
            return ConvertStream.str();
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Operators

        /// @brief Assignment operator.
        /// @param Other The other SemanticVersion to assign to this.
        void operator=(const SemanticVersion& Other)
        {
            this->Major = Other.Major;
            this->Minor = Other.Minor;
            this->Patch = Other.Patch;
            this->PreRelease = Other.PreRelease;
            this->MetaData = Other.MetaData;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Comparison Operators

        /// @brief Less than comparison operator.
        /// @param Other The other SemanticVersion to be compared to.
        /// @return Returns true if this compares less than the other SemanticVersion.
        Boole operator<(const SemanticVersion& Other) const
        {
            // Check the Major component
            if( this->Major < Other.Major ) return true;
            else if( this->Major > Other.Major ) return false;
            // Check the Minor component
            if( this->Minor < Other.Minor ) return true;
            else if( this->Minor > Other.Minor ) return false;
            // Check the Patch component
            if( this->Patch < Other.Patch ) return true;
            else if( this->Patch > Other.Patch ) return false;
            // Check the PreRelease component
            return ( this->PreRelease < Other.PreRelease );
        }
        /// @brief Less than or equal to comparison operator.
        /// @param Other The other SemanticVersion to be compared to.
        /// @return Returns true if this compares less than or equal to the other SemanticVersion.
        Boole operator<=(const SemanticVersion& Other) const
        {
            // Check the Major component
            if( this->Major < Other.Major ) return true;
            else if( this->Major > Other.Major ) return false;
            // Check the Minor component
            if( this->Minor < Other.Minor ) return true;
            else if( this->Minor > Other.Minor ) return false;
            // Check the Patch component
            if( this->Patch < Other.Patch ) return true;
            else if( this->Patch > Other.Patch ) return false;
            // Check the PreRelease component
            return ( this->PreRelease <= Other.PreRelease );
        }
        /// @brief Greater than comparison operator.
        /// @param Other The other SemanticVersion to be compared to.
        /// @return Returns true if this compares greater than the other SemanticVersion.
        Boole operator>(const SemanticVersion& Other) const
        {
            // Check the Major component
            if( this->Major > Other.Major ) return true;
            else if( this->Major < Other.Major ) return false;
            // Check the Minor component
            if( this->Minor > Other.Minor ) return true;
            else if( this->Minor < Other.Minor ) return false;
            // Check the Patch component
            if( this->Patch > Other.Patch ) return true;
            else if( this->Patch < Other.Patch ) return false;
            // Check the PreRelease component
            return ( this->PreRelease > Other.PreRelease );
        }
        /// @brief Greater than or equal to comparison operator.
        /// @param Other The other SemanticVersion to be compared to.
        /// @return Returns true if this compares greater than or equal to the other SemanticVersion.
        Boole operator>=(const SemanticVersion& Other) const
        {
            // Check the Major component
            if( this->Major > Other.Major ) return true;
            else if( this->Major < Other.Major ) return false;
            // Check the Minor component
            if( this->Minor > Other.Minor ) return true;
            else if( this->Minor < Other.Minor ) return false;
            // Check the Patch component
            if( this->Patch > Other.Patch ) return true;
            else if( this->Patch < Other.Patch ) return false;
            // Check the PreRelease component
            return ( this->PreRelease >= Other.PreRelease );
        }

        /// @brief Equality comparison operator.
        /// @param Other The other SemanticVersion to be compared to.
        /// @return Returns true if the two SimpleVersions are equal.
        Boole operator==(const SemanticVersion& Other) const
            { return ( this->Major == Other.Major && this->Minor == Other.Minor && this->Patch == Other.Patch && this->PreRelease == Other.PreRelease ); }
        /// @brief Inequality comparison operator.
        /// @param Other The other SemanticVersion to be compared to.
        /// @return Returns true if the two SimpleVersions are inequal.
        Boole operator!=(const SemanticVersion& Other) const
            { return ( this->Major != Other.Major || this->Minor != Other.Minor || this->Patch != Other.Patch || this->PreRelease != Other.PreRelease ); }
    };//SemanticVersion
}//Mezzanine

#endif
