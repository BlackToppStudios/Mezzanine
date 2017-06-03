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
#ifndef _hashedstring_h
#define _hashedstring_h

#include "datatypes.h"
#include "XML/xml.h"

/// @file
/// @brief This implemenation of

namespace Mezzanine
{

#ifndef HASHED_STRING_32_SEED
/// @def HASHED_STRING_32_SEED
/// @brief A value used to tweak the hash function for maximum randomness.
#define HASHED_STRING_32_SEED 1
#endif

    ///////////////////////////////////////////////////////////////////////////////
    /// @brief An identifier that compares as fast as an integer but has a string.
    /// @details This computes a MurmurA(x86 32bit) hash on creation. This is used
    /// in all comparisons, this means that in excedingly rare situations that
    /// hash collisions could cause silent failure. @n @n
    /// Internally programming la
    ///////////////////////////////////////
    class MEZZ_LIB HashedString32
    {
    public:
        /// @brief The type of the computed hash.
        using HashType = Int32;
        /// @brief The hash value of an empty string ("").
        static const HashType EmptyHash = 1364076727;

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        /// @brief Compute a MurmurA aka MurmurX86-32bit has of a string and return it.
        /// @param ToBeHashed The string to hash
        /// @return A 32 bit value approximating unique, which represents the passed string.
        static HashType Murmur32bit(const String& ToBeHashed);
        /// @brief Compute a MurmurA aka MurmurX86-32bit has of a string and return it.
        /// @param ToBeHashed The string to hash
        /// @return A 32 bit value approximating unique, which represents the passed string.
        static HashType Murmur32bit(const char* ToBeHashed);

    protected:
        ///////////////////////////////////////////////////////////////////////////////
        // Data

        /// The Actual string to store.
        String TheString;
        /// @brief The computed hash.
        HashType Hash;

    public:
        ///////////////////////////////////////////////////////////////////////////////
        // Creation

        /// @brief NonComputing Constructor
        /// @details Skip the hashing overhead and just do the work for h
        HashedString32();
        /// @brief String Constructor
        /// @param StartingString The String to work with
        /// @details Compute the hash of the String and hang onto both for future use.
        HashedString32(const String& StartingString);
        /// @brief C-String Constructor
        /// @param StartingString The String to work with
        /// @details Compute the hash of the String and hang onto both for future use.
        HashedString32(const char* StartingString);
        /// @brief Precomputed Constructor
        /// @param StartingString The String to work with
        /// @param PrecomputedHash The value
        /// @details Does not Compute the hash of the String, Just trusts whatever it is given.
        HashedString32(const String& StartingString, HashType PrecomputedHash);
        /// @brief Copy constructor
        /// @param StartingString The other hashed string
        HashedString32(const HashedString32& StartingString) = default;
        /// @brief Move constructor.
        /// @param MovingString The other hashed string.
        HashedString32(HashedString32&& MovingString) = default;

        ///////////////////////////////////////////////////////////////////////////////
        // Non-Comparison Operators

        /// @brief Assignment operator.
        /// @param Other The other string to be copied.
        /// @return Returns a reference to this.
        HashedString32& operator=(const HashedString32& Other) = default;
        /// @brief Move assignment operator.
        /// @param Other The other string to be moved.
        /// @return Returns a reference to this.
        HashedString32& operator=(HashedString32&& Other) = default;

        /// @brief Standard String Assignment operator.
        /// @param Other The other std::string to be copied.
        /// @return Returns a reference to this.
        HashedString32& operator=(const String& Other);
        /// @brief C-String Assignment operator.
        /// @param Other The other c-string to be copied.
        /// @return Returns a reference to this.
        HashedString32& operator=(const char* Other);

        ///////////////////////////////////////////////////////////////////////////////
        // Comparisons

        /// @brief Compare The hash of this and another string for equality.
        /// @param Other The other string to compare this one too.
        /// @return True if the hashes are the same.
        /// @note It is imcredibly unlikely, but this could return true in the event of a hash collision.
        Boole operator==(const HashedString32& Other) const;
        /// @brief Compare The hash of this and another string for inequality.
        /// @param Other The other string to compare this one too.
        /// @return True if the hashes are different.
        Boole operator!=(const HashedString32& Other) const;
        /// @brief Provide a semantically useless, but quite fast sorting order for std containers based on only hashes.
        /// @param Other The other string to compare this one too.
        /// @return true If the Hash of this is smaller than the hash of the other
        Boole operator<(const HashedString32& Other) const;
        /// @brief Provide a semantically useless, but quite fast sorting order for std containers based on only hashes.
        /// @param Other The other string to compare this one too.
        /// @return true If the Hash of this is larger than the hash of the other
        Boole operator>(const HashedString32& Other) const;

        /// @return True if this is equal to "", false otherwise.
        Boole IsEmpty() const;


        ///////////////////////////////////////////////////////////////////////////////
        // Data Modifiers/observers

        /// @brief Gets the hash generated from the provided string.
        /// @return Returns a HashType with the hash in it.
        HashType GetHash() const;
        /// @brief Get the string that was used to calculate the hash.
        /// @return Returns the underlying string.
        const String& GetString() const;
        /// @brief Set a new string and recalculate its hash.
        /// @param NewString The data to hash and store for later use.
        void SetString(const String& NewString);

        ///////////////////////////////////////////////////////////////////////////////
        // Serialization

        /// @brief Convert this class to an XML::Node ready for serialization
        /// @param CurrentRoot The point in the XML hierarchy that all this HashedString32 should be appended to.
        void ProtoSerialize(XML::Node& CurrentRoot) const;
        /// @brief Take the data stored in an XML and overwrite this instance of this object with it
        /// @param OneNode and XML::Node containing the data.
        void ProtoDeSerialize(const XML::Node& OneNode);
        /// @brief Get the name of the the XML tag this class will leave behind as its instances are serialized.
        /// @return A string containing "HashedString32"
        static String GetSerializableName();
    };//HashedString32

    ///////////////////////////////////////////////////////////////////////////////
    // Class External Operators

    /// @brief Creates a combined string between a HashedString32 and a string literal.
    /// @param Hashed The HashedString32 to concatenate.
    /// @param Literal The string literal to concatenate.
    /// @return Returns a string containing the concatenated results of a hashed string and a string literal.
    String MEZZ_LIB operator+(const HashedString32& Hashed, const char* Literal);
    /// @brief Creates a combined string between a HashedString32 and a string literal.
    /// @param Literal The string literal to concatenate.
    /// @param Hashed The HashedString32 to concatenate.
    /// @return Returns a string containing the concatenated results of a hashed string and a string literal.
    String MEZZ_LIB operator+(const char* Literal, const HashedString32& Hashed);

    /// @brief Send a HashedString32 down a stream serialized.
    /// @param stream The std::ostream to send it down.
    /// @param x The HashedString32 to Send.
    /// @return Returns a reference to the stream argument.
    std::ostream& MEZZ_LIB operator << (std::ostream& stream, const Mezzanine::HashedString32& x);
}//Mezzanine


#endif
