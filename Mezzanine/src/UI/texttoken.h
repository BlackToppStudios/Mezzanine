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
#ifndef _uitexttoken_h
#define _uitexttoken_h

#include "datatypes.h"

namespace Mezzanine
{
    namespace UI
    {
        class MarkupParser;
        ///////////////////////////////////////////////////////////////////////////////
        /// @brief This class represents a normal text segment from the source string.
        /// @details Text tokens are intended for the generation of segments of regular glyphs provided by an atlas
        /// or font file.  They have no special behaviors of their own and will almost always have their render size
        /// match their text size.
        ///////////////////////////////////////
        class MEZZ_LIB TextToken
        {
        public:
            /// @brief The type of token this class represents.
            enum TokenType
            {
                TT_Error          = 0,   ///< Used to describe any generic error with a token.
                TT_Text           = 1,   ///< Used to describe a normal text token with plain text.
                TT_TagInvalid     = 2,   ///< Used to describe a tag token that is syntactically correct, but has another error, such as a range tag missing a partner, or the tag name isn't found.
                TT_RangeTagStart  = 3,   ///< Used to describe either a tag token inserting a character, or the start of a range tag.
                TT_RangeTagEnd    = 4,   ///< Used to describe the end of a range tag.
                TT_InsertTag      = 5    ///< Used to describe a single tag used to insert a special character (such as s sprite).
            };//TokenType
        protected:
            friend class MarkupParser;
            /// @internal
            /// @brief Type of token this is.
            TokenType Type;
            /// @internal
            /// @brief The number of rendered characters this token produced.
            UInt32 RenderSize;
            /// @internal
            /// @brief Container for the converted text.
            String Text;
            /// @internal
            /// @brief Takes a position of a renderable char and converts it to the respective position in the raw string.
            /// @note In cases where ASCII is used, this will always return the same number, this exists for when more exotic unicode characters/glyphs are used.
            /// @param Index The index to be converted.
            /// @return Returns a UInt32 representing the Raw character index of the first character corresponding to the rendered character at the provided index.
            UInt32 ConvertRenderIndexToRawIndex(const UInt32 Index);
        public:
            /// @brief Class constructor.
            /// @note This is mostly used for inheritence purposes.
            TextToken();
            /// @brief Descriptive constructor.
            /// @param RawText A string containing the raw text of this token.
            /// @param TType The type of text token to be created.
            TextToken(const String& RawText, const TokenType TType);
            /// @brief Char8 constructor.
            /// @param Characters A buffer of UTF-8 characters to populate this TextToken with.
            /// @param Size The number of characters that exist in the provided buffer.
            TextToken(const Char8* Characters, const UInt32 Size);
            /// @brief Char8 constructor.
            /// @param Characters A buffer of UTF-32 characters to populate this TextToken with.
            /// @param Size The number of characters that exist in the provided buffer.
            TextToken(const UInt32* Characters, const UInt32 Size);
            /// @brief Class destructor.
            virtual ~TextToken();

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            /// @brief Gets the raw string for this token used to render or manipulate characters.
            /// @return Returns a reference to the source string stored by this token.
            virtual const String& GetRawCharacterString() const;
            /// @brief Gets the type of token this is.
            /// @return Returns a TokenType enum value describing the type of token this is.
            virtual TextToken::TokenType GetTokenType() const;
            /// @brief Gets the number of Char8's that comprise the source text for this tag.
            /// @return Returns a UInt32 containing the size of the source string for this token.
            virtual UInt32 GetRawCharacterSize() const;
            /// @brief Gets the number of rendered characters this token generates.
            /// @note This number is initially generated when the tokens are parsed and the actual characters are generated.  Methods that modify
            /// the source text try to keep this in sync appropriately.
            /// @return Returns a UInt32 containing the number of characters are being rendered via the text from this token.
            virtual UInt32 GetRenderCharacterSize() const;

            ///////////////////////////////////////////////////////////////////////////////
            // Inserting and Removing

            /// @brief Inserts a single UTF-32 size character into this token.
            /// @param Index The index at which the character will be inserted.
            /// @param UChar The unicode character to be inserted.  This will be converted to UTF-8 prior to insertion.
            /// @return Returns 1 if the character was successfully inserted, otherwise returns 0.  Note: This is not rendered characters, as they don't get generated until the tokens are re-parsed.
            virtual UInt32 InsertCharacter(const UInt32 Index, UInt32 UChar);
            /// @brief Inserts multiple characters into this token.
            /// @param Index The index at which the characters will be inserted.
            /// @param Characters An array of Char8's encoded in UTF-8 to be inserted.
            /// @param Size The size of the array of Char8's passed in.
            /// @return Returns the number of characters successfully inserted.  Note: This is not rendered characters, as they don't get generated until the tokens are re-parsed.
            virtual UInt32 InsertCharacters(const UInt32 Index, const Char8* Characters, const UInt32 Size);
            /// @brief Inserts multiple characters into this token.
            /// @param Index The index at which the characters will be inserted.
            /// @param Characters An array of UInt32's encoded in UTF-32 to be inserted.
            /// @param Size The size of the array of UInt32's passed in.
            /// @return Returns the number of characters successfully inserted.  Note: This is not rendered characters, as they don't get generated until the tokens are re-parsed.
            virtual UInt32 InsertCharacters(const UInt32 Index, const UInt32* Characters, const UInt32 Size);
            /// @brief Removes a single rendered character from this token.
            /// @param Index The index at which the rendered character will be removed.
            /// @return Returns 1 if the character was successfully removed, otherwise returns 0.
            virtual UInt32 RemoveCharacter(const UInt32 Index);
            /// @brief Removes rendered characters from this token.
            /// @param Index The index at which to start removing characters from this token.
            /// @param Length The number of characters to try and remove from this token.
            /// @return Returns the number of rendered characters that were actually removed.
            virtual UInt32 RemoveCharacters(const UInt32 Index, const UInt32 Length);
            /// @brief Removes all the rendered characters from this token.
            /// @return Returns the number of rendered characters that were actually removed.
            virtual UInt32 ClearAllCharacters();
        };//TextToken

        ///////////////////////////////////////////////////////////////////////////////
        /// @brief This struct represents a markup tag segment from the source string.
        /// @details Tag tokens are tokens that represent text that isn't intended to be rendered, but instead alter
        /// other text around it or create special characters of their own.  The render size on tag token should never
        /// never match the text size, instead being 0 or 1.
        ///////////////////////////////////////
        class MEZZ_LIB TagToken : public TextToken
        {
        protected:
            friend class MarkupParser;
            /// @brief Unconverted version of the tag name.
            String TagName;
            /// @brief The parameters provided for this tag, if any.
            NameValuePairMap Params;
        public:
            /// @brief Class constructor.
            TagToken();
            /// @brief Descriptive constructor.
            /// @param RawText A string containing the raw text of this token.
            /// @param Name The name of this tag.
            /// @param TType The type of text token to be created.
            TagToken(const String& RawText, const String& Name, const TokenType TType);
            /// @brief Class destructor.
            virtual ~TagToken();

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            /// @brief Gets the name of the tag this token represents.
            /// @return Returns a reference to a string containing the name of this tokens tag.
            const String& GetTagName() const;
            /// @brief Gets a parameter specified in this token by name.
            /// @param Param The name of the parameter to retrieve.
            /// @return Returns a string containing the value specified in this token.
            String GetParameter(const String& Param) const;
        };//TagToken

        ///////////////////////////////////////////////////////////////////////////////
        /// @brief This struct represents a markup tag segment from the source string.
        /// @details Tag tokens are tokens that represent text that isn't intended to be rendered, but instead alter
        /// other text around it or create special characters of their own.  The render size on tag token should never
        /// never match the text size, instead being 0 or 1.
        ///////////////////////////////////////
        class MEZZ_LIB RangeTagToken : public TagToken
        {
        protected:
            friend class MarkupParser;
            /// @brief Pointer to the opening/closing tag for this tag.
            RangeTagToken* PartnerTag;
        public:
            /// @brief Class constructor.
            RangeTagToken();
            /// @brief Descriptive constructor.
            /// @param RawText A string containing the raw text of this token.
            /// @param Name The name of this tag.
            /// @param TType The type of text token to be created.
            RangeTagToken(const String& RawText, const String& Name, const TokenType TType);
            /// @brief Class destructor.
            virtual ~RangeTagToken();

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            /// @brief Gets the partner tag to this RangeTag
            /// @return Returns a pointer to the other tag paired with this range tag.
            RangeTagToken* GetPartnerTag() const;

            ///////////////////////////////////////////////////////////////////////////////
            // Inserting and Removing

            /// @copydoc TextToken::InsertCharacter(const UInt32, UInt32)
            virtual UInt32 InsertCharacter(const UInt32 Index, UInt32 UChar);
            /// @copydoc TextToken::InsertCharacters(const UInt32, const Char8*, const UInt32)
            virtual UInt32 InsertCharacters(const UInt32 Index, const Char8* Characters, const UInt32 Size);
            /// @copydoc TextToken::InsertCharacters(const UInt32, const UInt32*, const UInt32)
            virtual UInt32 InsertCharacters(const UInt32 Index, const UInt32* Characters, const UInt32 Size);
            /// @copydoc TextToken::RemoveCharacter(const UInt32)
            virtual UInt32 RemoveCharacter(const UInt32 Index);
            /// @copydoc TextToken::RemoveCharacters(const UInt32, const UInt32)
            virtual UInt32 RemoveCharacters(const UInt32 Index, const UInt32 Length);
            /// @copydoc TextToken::ClearAllCharacters()
            virtual UInt32 ClearAllCharacters();
        };//RangeTagToken

        ///////////////////////////////////////////////////////////////////////////////
        /// @brief This struct represents a markup tag segment from the source string.
        /// @details Tag tokens are tokens that represent text that isn't intended to be rendered, but instead alter
        /// other text around it or create special characters of their own.  The render size on tag token should never
        /// never match the text size, instead being 0 or 1.
        ///////////////////////////////////////
        class MEZZ_LIB InsertTagToken : public TagToken
        {
        protected:
            friend class MarkupParser;
        public:
            /// @brief Class constructor.
            InsertTagToken();
            /// @brief Descriptive constructor.
            /// @param RawText A string containing the raw text of this token.
            /// @param Name The name of this tag.
            InsertTagToken(const String& RawText, const String& Name);
            /// @brief Class destructor.
            virtual ~InsertTagToken();

            ///////////////////////////////////////////////////////////////////////////////
            // Inserting and Removing

            /// @copydoc TextToken::InsertCharacter(const UInt32, UInt32)
            virtual UInt32 InsertCharacter(const UInt32 Index, UInt32 UChar);
            /// @copydoc TextToken::InsertCharacters(const UInt32, Char8*, const UInt32)
            virtual UInt32 InsertCharacters(const UInt32 Index, const Char8* Characters, const UInt32 Size);
            /// @copydoc TextToken::InsertCharacters(const UInt32, const UInt32*, const UInt32)
            virtual UInt32 InsertCharacters(const UInt32 Index, const UInt32* Characters, const UInt32 Size);
            /// @copydoc TextToken::RemoveCharacter(const UInt32)
            virtual UInt32 RemoveCharacter(const UInt32 Index);
            /// @copydoc TextToken::RemoveCharacters(const UInt32, const UInt32)
            virtual UInt32 RemoveCharacters(const UInt32 Index, const UInt32 Length);
            /// @copydoc TextToken::ClearAllCharacters()
            virtual UInt32 ClearAllCharacters();
        };//InsertTagToken

        ///////////////////////////////////////////////////////////////////////////////
        /// @brief This is a helper class that facilitates operations with collections of tokens generated from Markup Parsers.
        /// @details
        ///////////////////////////////////////
        class MEZZ_LIB TokenString
        {
        public:
            /// @brief Basic container type for the storage of @ref TextToken instances by this class.
            typedef std::vector<TextToken*>                 TokenContainer;
            /// @brief Iterator type for @ref TextToken instances being stored by this class.
            typedef TokenContainer::iterator                TokenIterator;
            /// @brief Const Iterator type for @ref TextToken instances being stored by this class.
            typedef TokenContainer::const_iterator          ConstTokenIterator;
            /// @brief Reverse Iterator type for @ref TextToken instances being stored by this class.
            typedef TokenContainer::reverse_iterator        ReverseTokenIterator;
            /// @brief Const Reverse Iterator type for @ref TextToken instances being stored by this class.
            typedef TokenContainer::const_reverse_iterator  ConstReverseTokenIterator;
            /// @brief An std::pair used to report the result of a TextToken and it's local index.
            typedef std::pair<TokenIterator,UInt32>         TokenIndexPair;
        protected:
            /// @internal
            /// @brief Container for TextToken storage.
            TokenContainer Tokens;
            /// @internal
            /// @brief Gets the token at the string index, and it's local index.
            /// @param Index The TokenString index of the character to retrieve.
            /// @return Returns an std::pair containing an iterator to the token at the specified index in the first member, and it's local index in the second member
            TokenIndexPair GetTokenIndex(const UInt32 Index);
        public:
            /// @brief Class constructor.
            TokenString();
            /// @brief Class destructor.
            virtual ~TokenString();

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            /// @brief Gets a string containing all the raw characters of the tokens in this string.
            /// @return Returns a string of all the UTF-8 characters in the tokens of this string.
            virtual String GetRawString() const;
            /// @brief Gets the number of renderable characters that exist in this string.
            /// @return Returns a Whole containing the number of renderable characters this TokenString is storing.
            Whole GetNumCharacters() const;
            /// @brief Gets the number of text tokens in this string.
            /// @return Returns a Whole containing the number of tokens this TokenString is storing.
            Whole GetNumTokens() const;

            /// @brief Gets an iterator to the first TextToken.
            /// @return Returns an iterator to the first TextToken being stored by this TokenString.
            TokenIterator BeginToken();
            /// @brief Gets an iterator to one passed the last TextToken.
            /// @return Returns an iterator to one passed the last TextToken being stored by this TokenString.
            TokenIterator EndToken();
            /// @brief Gets a const iterator to the first TextToken.
            /// @return Returns a const iterator to the first TextToken being stored by this TokenString.
            ConstTokenIterator BeginToken() const;
            /// @brief Gets a const iterator to one passed the last TextToken.
            /// @return Returns a const iterator to one passed the last TextToken being stored by this TokenString.
            ConstTokenIterator EndToken() const;

            /// @brief Gets a reverse iterator to the last TextToken.
            /// @return Returns a reverse iterator to the last TextToken being stored by this TokenString.
            ReverseTokenIterator ReverseBeginToken();
            /// @brief Gets a reverse iterator to one before the first TextToken.
            /// @return Returns a reverse iterator to one before the first TextToken being stored by this TokenString.
            ReverseTokenIterator ReverseEndToken();
            /// @brief Gets a const reverse iterator to the last TextToken.
            /// @return Returns a const reverse iterator to the last TextToken being stored by this TokenString.
            ConstReverseTokenIterator ReverseBeginToken() const;
            /// @brief Gets a const reverse iterator to one before the first TextToken.
            /// @return Returns a const reverse iterator to one before the first TextToken being stored by this TokenString.
            ConstReverseTokenIterator ReverseEndToken() const;

            ///////////////////////////////////////////////////////////////////////////////
            // Generating and Destroying the String

            /// @brief Appends a new token to the end of this string.
            /// @note Tokens appended to this string will become owned by this string and deleted when this string is destructed.
            /// @param ToBePushed The token being added to this string.
            virtual void PushToken(TextToken* ToBePushed);
            /// @brief Appends a group of tokens to the end of this string.
            /// @note Tokens appended to this string will become owned by this string and deleted when this string is destructed.
            /// @param ToBePushed A container
            virtual void PushTokens(const TokenContainer& ToBePushed);
            /// @brief Destroys all tokens currently in this string.
            virtual void DestroyAllTokens();

            ///////////////////////////////////////////////////////////////////////////////
            // Inserting and Removing

            /// @brief Inserts a single UTF-32 size character into this string.
            /// @param Index The index at which the character will be inserted.
            /// @param UChar The unicode character to be inserted.  This will be converted to UTF-8 prior to insertion.
            /// @return Returns 1 if the character was successfully inserted, otherwise returns 0.
            virtual UInt32 InsertCharacter(const UInt32 Index, UInt32 UChar);
            /// @brief Inserts multiple characters into this string.
            /// @param Index The index at which the characters will be inserted.
            /// @param Characters An array of Char8's encoded in UTF-8 to be inserted.
            /// @param Size The size of the array of Char8's passed in.
            /// @return Returns the number of characters successfully inserted.
            virtual UInt32 InsertCharacters(const UInt32 Index, const Char8* Characters, const UInt32 Size);
            /// @brief Inserts multiple characters into this string.
            /// @param Index The index at which the characters will be inserted.
            /// @param Characters An array of UInt32's encoded in UTF-32 to be inserted.
            /// @param Size The size of the array of UInt32's passed in.
            /// @return Returns the number of characters successfully inserted.
            virtual UInt32 InsertCharacters(const UInt32 Index, const UInt32* Characters, const UInt32 Size);
            /// @brief Removes a single rendered character from this string.
            /// @param Index The index at which the rendered character will be removed.
            /// @return Returns 1 if the character was successfully removed, otherwise returns 0.
            virtual UInt32 RemoveCharacter(const UInt32 Index);
            /// @brief Removes rendered characters from this string.
            /// @param Index The index at which to start removing characters from this string.
            /// @param Length The number of characters to try and remove from this string.
            /// @return Returns the number of rendered characters that were actually removed.
            virtual UInt32 RemoveCharacters(const UInt32 Index, const UInt32 Length);
            /// @brief Removes all the rendered characters from this string.
            /// @return Returns the number of rendered characters that were actually removed.
            virtual UInt32 ClearAllCharacters();
        };//TokenString
    }//UI
}//Mezzanine

#endif
