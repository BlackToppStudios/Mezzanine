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
#ifndef _HashedString_cpp
#define _HashedString_cpp

#include "hashedstring.h"
#include "Internal/murmurhash3.h.cpp"
#include "serialization.h"
#include "exception.h"

#include <cstring>

namespace Mezzanine
{
    HashedString32::HashType HashedString32::Murmur32bit(const String& ToBeHashed)
    {
        HashType Results = 0;
        Internal::MurmurHash3_x86_32(ToBeHashed.data(), ToBeHashed.size(), HASHED_STRING_32_SEED, &Results );
        return Results;
    }

    HashedString32::HashType HashedString32::Murmur32bit(const char* ToBeHashed)
    {
        HashType Results = 0;
        Internal::MurmurHash3_x86_32(ToBeHashed, strlen(ToBeHashed), HASHED_STRING_32_SEED, &Results );
        return Results;
    }

    ///////////////////////////////////////////////////////////////////////////////
    // HashedString32 Methods

    HashedString32::HashedString32() :
        Hash(HashedString32::EmptyHash)
        {  }

    HashedString32::HashedString32(const String& StartingString) :
        TheString(StartingString), Hash(Murmur32bit(StartingString))
        {  }

    HashedString32::HashedString32(const char* StartingString) :
        TheString(StartingString), Hash(Murmur32bit(StartingString))
        {  }

    HashedString32::HashedString32(const String& StartingString, HashType PrecomputedHash) :
        TheString(StartingString), Hash(PrecomputedHash)
        {  }

    ///////////////////////////////////////////////////////////////////////////////
    // Non-Comparison Operators

    HashedString32& HashedString32::operator=(const String& Other)
    {
        this->TheString = Other;
        this->Hash = Murmur32bit(Other);
    }

    HashedString32& HashedString32::operator=(const char* Other)
    {
        this->TheString = Other;
        this->Hash = Murmur32bit(Other);
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Comparisons

    Boole HashedString32::operator==(const HashedString32& Other) const
    {
        //if(Hash==Other.Hash)
        //    { return TheString == Other.TheString; }
        //return false;
        return Hash==Other.Hash;
    }

    Boole HashedString32::operator!=(const HashedString32& Other) const
    {
        //if(Hash!=Other.Hash)
        //    { return TheString != Other.TheString; }
        //return false;
        return Hash!=Other.Hash;
    }

    Boole HashedString32::operator<(const HashedString32& Other) const
    {
        //if(Hash==Other.Hash)
        //    { return TheString < Other.TheString; }
        return Hash < Other.Hash;
    }

    Boole HashedString32::operator>(const HashedString32& Other) const
    {
        //if(Hash==Other.Hash)
        //    { return TheString > Other.TheString; }
        return Hash > Other.Hash;
    }

    Boole HashedString32::IsEmpty() const
        { return HashedString32::EmptyHash == Hash; }


    HashedString32::HashType HashedString32::GetHash() const
        { return Hash; }

    const String& HashedString32::GetString() const
        { return TheString; }

    void HashedString32::SetString(const String& NewString)
    {
        TheString = NewString;
        Hash=Murmur32bit(TheString);
    }

    void HashedString32::ProtoSerialize(XML::Node& CurrentRoot) const
    {
        Mezzanine::XML::Node Hash32Node = CurrentRoot.AppendChild(GetSerializableName());

        if(Hash32Node)
        {
            Mezzanine::XML::Attribute VersionAttr = Hash32Node.AppendAttribute("Version");
            Mezzanine::XML::Attribute HashAttr = Hash32Node.AppendAttribute("Hash");
            Mezzanine::XML::Attribute StringAttr = Hash32Node.AppendAttribute("String");

            if( VersionAttr && HashAttr && StringAttr )
            {
                if( VersionAttr.SetValue("1") && HashAttr.SetValue(ToString(Hash)) && StringAttr.SetValue(TheString))
                {
                    return;
                }else{
                    SerializeError("Create XML Attribute Values", GetSerializableName(),true);
                }
            }else{
                SerializeError("Create XML Attributes", GetSerializableName(),true);
            }
        }else{
            SerializeError("Create XML Serialization Node", GetSerializableName(),true);
        }
    }

    void HashedString32::ProtoDeSerialize(const XML::Node& OneNode)
    {
        if ( Mezzanine::String(OneNode.Name())==Mezzanine::String(GetSerializableName()) )
        {
            if(OneNode.GetAttribute("Version").AsInt() == 1)
            {
                Hash=OneNode.GetAttribute("Hash").AsInteger();
                TheString=OneNode.GetAttribute("String").AsString();
            }else{
                MEZZ_EXCEPTION(ExceptionBase::INVALID_VERSION_EXCEPTION,"Incompatible XML Version for " + GetSerializableName() + ": Not Version 1.");
            }
        }else{
            MEZZ_EXCEPTION(ExceptionBase::II_IDENTITY_INVALID_EXCEPTION,"Attempting to deserialize a " + GetSerializableName() + ", found a " + String(OneNode.Name()) + ".");
        }
    }

    String HashedString32::GetSerializableName()
        { return String("HashedString32"); }

    ///////////////////////////////////////////////////////////////////////////////
    // Class External Operators

    String operator+(const HashedString32& Hashed, const char* Literal)
        { return Hashed.GetString() + Literal; }

    String operator+(const char* Literal, const HashedString32& Hashed)
        { return Literal + Hashed.GetString(); }

    std::ostream& operator << (std::ostream& stream, const Mezzanine::HashedString32& x)
    {
        Serialize(stream,x);
        return stream;
    }
}//Mezzanine



#endif
