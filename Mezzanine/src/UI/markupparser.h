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
#ifndef _uimarkupparser_h
#define _uimarkupparser_h

#include "countedptr.h"
#include "colourvalue.h"

namespace Mezzanine
{
    namespace UI
    {
        class FontData;
        class TextLayer;
        class Character;
        class CharacterTraits;
        class TextToken;
        class TagToken;
        class TokenString;
        ///////////////////////////////////////////////////////////////////////////////
        /// @brief This is a base class for tags that implement the functionality of a markup language.
        /// @details When creating markup tags it is important to understand the disinction between range tags
        /// and non-range tags.  Range tags render nothing themselves and instead alter all the characters
        /// between both ends of the tag.  Non-range tags can create characters of their own to be inserted, however
        /// when doing so it should be limited to creating only one per processing of a tag.  This helps to make
        /// specific configurations easier to implement for the end user, as well as simplify the logic used when
        /// editing strings and tokens handled by text layers.
        ///////////////////////////////////////
        class MEZZ_LIB MarkupTag
        {
        public:
            /// @brief Basic container type for the storage of @ref Character instances by this class.
            typedef std::list<Character*>                           CharacterContainer;
            /// @brief Iterator type for @ref Character instances stored by this class.
            typedef CharacterContainer::iterator                    CharacterIterator;
            /// @brief Const Iterator type for @ref Character instances stored by this class.
            typedef CharacterContainer::const_iterator              ConstCharacterIterator;
            /// @brief An std::pair used to report the result of a MarkupTag being processed any the character it may have generated.
            typedef std::pair<Boole,Character*>                     ProcessResult;
        public:
            /// @brief Class constructor.
            MarkupTag() {  }
            /// @brief Class destructor.
            virtual ~MarkupTag() {  }

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            /// @brief Gets the name of this tag.
            /// @return Returns a String containing the name of this tag.
            virtual const String& GetName() const = 0;
            /// @brief Gets whether or not this tag applies to a range of characters.
            /// @return Returns true if this tag requires a closing tag to accompany to be valid, false otherwise.
            virtual Boole IsRangeTag() const = 0;

            /// @brief Processes this tag.
            /// @param Params A NameValuePairMap of all the parameters provided for this tag.
            /// @param Traits The character traits to be modified and used for future character construction.
            /// @param Layer A pointer to the TextLayer currently being parsed.
            /// @return Returns a process result where the first value is true if this tag succeeded in making it's alterations and
            /// the second value contains a pointer to a generated character if one was generated.  The second value can be NULL.
            virtual ProcessResult Process(NameValuePairMap& Params, CharacterTraits& Traits, TextLayer* Layer) const = 0;
        };//MarkupTag

        ///////////////////////////////////////////////////////////////////////////////
        /// @brief This is a base class for the parsing of markup texts contained in text layers.
        /// @details This class implements every aspect of the markup language except for the tags and their functionality.
        /// It is expected that tags can be broken down into one of two processes, either they will generate a character, or
        /// they will alter the styling used to render an existing range of characters.  @n @n
        /// The styling and syntax of the markup language is similar (but not identical) to BBCode.  Developers can set which
        /// glyphs are to be used to mark the start and end of a markup tag.  Tags can be start tags, end tags, and individual
        /// tags.  Individual tags are the character generators.  They make the character and are done with influencing any other
        /// characters.  Start tags and end tags together define a range of characters they are to influence.  Use of the forward
        /// slash '/' is prohibited unless you are noting an end tag, in which case the slash is to be followed by the tag name
        /// (this is case sensitive) and nothing else, within the markup start and end glyphs.  An end tag cannot have parameters
        /// defined in it, and if any are included they will be parsed as a part of the tag name.  A start tag cannot have a forward
        /// slash '/'.  Instead it must start with the tag name, then a space, and then list the parameter names and values linked by
        /// an equals sign '=', and separated by spaces.  Spaces between the markup tag start and end glyphs and tag names should be
        /// avoided.  @n @n
        /// Relevant parameters and their possibile values are based on the tag being used, and you should check the markup
        /// implemenation for that information.  Depending on the number of parameters needed for the tag this markup system supports
        /// both short notation, and long notation.  Short notation is only supported when there is one parameter, and is implemented
        /// as "{tagname}{equals}{value}".  For example a sprite could be implemented as "[Sprite=Awesome]" where everything after the
        /// equals to the end tag is treated as the value to set, in this case the name of the sprite to be set.  In situations like this
        /// the value is always given the name "Value" in the NameValuePairMap of parameters that is passed around during parsing.  Long
        /// notation involves being explicit with the names of values, and is necessary for tags that require multiple parameters.  In
        /// long notation there should be minimal spaces, used only to separate the tag name and each parameter from each other.
        ///////////////////////////////////////
        class MEZZ_LIB MarkupParser
        {
        public:
            /// @brief Basic container type for the storage of @ref Character instances used during processing by this class.
            typedef std::list<Character*>               CharacterContainer;
            /// @brief Iterator type for @ref Character instances being processed by this class.
            typedef CharacterContainer::iterator        CharacterIterator;
            /// @brief Const Iterator type for @ref Character instances being processed by this class.
            typedef CharacterContainer::const_iterator  ConstCharacterIterator;
            /// @brief Basic container type for the storage of @ref MarkupTag instances by this class.
            typedef std::map<String,MarkupTag*>         TagContainer;
            /// @brief Iterator type for @ref MarkupTag instances stored by this class.
            typedef TagContainer::iterator              TagIterator;
            /// @brief Const Iterator type for @ref MarkupTag instances stored by this class.
            typedef TagContainer::const_iterator        ConstTagIterator;
            /// @brief An std::pair type used to map a TagToken to the MarkupTag implementation it is calling.
            typedef std::pair<TagToken*,MarkupTag*>     TokenTagPair;
            /// @brief Basic container type for the storage of @ref TokenTagPair instances used during processing by this class.
            typedef std::vector<TokenTagPair>           TagVector;
            /// @brief Iterator type for @ref TokenTagPair instances being processed by this class.
            typedef TagVector::iterator                 TagVecIterator;
            /// @brief Const Iterator type for @ref TokenTagPair instances being processed by this class.
            typedef TagVector::const_iterator           ConstTagVecIterator;
            /// @brief Basic container type for the storage of @ref TextToken instances used during processing by this class.
            typedef std::vector<TextToken*>             TokenContainer;
            /// @brief Iterator type for @ref TextToken instances being processed by this class.
            typedef TokenContainer::iterator            TokenIterator;
            /// @brief Const Iterator type for @ref TextToken instances being processed by this class.
            typedef TokenContainer::const_iterator      ConstTokenIterator;
        protected:
            /// @brief Map of tags recognized by this parser.
            TagContainer Tags;
            /// @brief Populates the tag map with the tags recognized by this parser.
            virtual void Initialize() = 0;
            /// @brief Helper method for creating text tokens.
            virtual TextToken* CreateTextToken(const String& Text) const;
            /// @brief Helper method for creating tag tokens.
            virtual TextToken* CreateTagToken(const String& Text) const;
            /// @brief Helper method for converting text tokens into characters.
            virtual void GenerateCharactersFromToken(const TextToken* Token, TextLayer* Layer, const CharacterTraits& Traits, CharacterContainer& Characters) const;
            /// @brief Helper method for regenerating current traits after a tag is disabled.
            virtual void RegenerateTraits(CharacterTraits& Traits, const TagVector& ActiveTags, TextLayer* Layer) const;
        public:
            /// @brief Class constructor.
            MarkupParser();
            /// @brief Class destructor.
            virtual ~MarkupParser();

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            /// @brief Gets the name of this parser implementation.
            /// @return Returns a string containing the name of this markup parser implementation.
            virtual String GetName() const = 0;
            /// @brief Gets the ID for the character that marks the start of a markup section.
            /// @return Returns an Int32 representing the start of a markup section.
            virtual Char8 GetMarkupTagStart() const = 0;
            /// @brief Gets the ID for the character that marks the end of a markup section.
            /// @return Returns an Int32 representing the end of a markup section.
            virtual Char8 GetMarkupTagEnd() const = 0;

            ///////////////////////////////////////////////////////////////////////////////
            // Parsing Methods

            /// @brief Processes a string encoded in UTF8 into a list of renderable characters.
            /// @param Source The source string containing raw text and (maybe) markup tags to convert into characters.
            /// @param InitialTraits The colour, highlight colour, and font characters are to have by default without otherwise being set via markup.
            /// @param CallingLayer The TextLayer instance calling this method.
            /// @return Returns a list of renderable characters generated by this parser.
            virtual CharacterContainer Parse(const String& Source, const CharacterTraits& InitialTraits, TextLayer* CallingLayer) const;
            /// @brief Processes a collection of text tokens into a list of renderable characters.
            /// @param Tokens A TokenString storing all the lex'd text that is to be parsed.
            /// @param InitialTraits The colour, highlight colour, and font characters are to have by default without otherwise being set via markup.
            /// @param CallingLayer The TextLayer instance calling this method.
            /// @return Returns a list of renderable characters generated by this parser.
            virtual CharacterContainer ParseTextTokens(TokenString* Tokens, const CharacterTraits& InitialTraits, TextLayer* CallingLayer) const;
            /// @brief Converts a string into a series of tokens that can be parsed more readily.
            /// @warning This method will naively search each character in the string looking for a match to the character provided by
            /// "GetMarkupTagStart()" and/or "GetMarkupTagEnd()".  If the provided string is properly formatted UTF-8 this will not cause
            /// problems.  If the encoding is wrong however, this can lead to detecting tags improperly and rendering the wrong
            /// characters.  In extreme cases it could try to look for a character that isn't available which will lead to an exception during parsing.
            /// @param Source The source string containing raw text and (maybe) markup tags to convert into tokens.
            /// @return Returns a pointer to a TokenString storing all the tokens generated.
            virtual TokenString* Lex(const String& Source) const;
        };//MarkupParser
    }//UI
}//Mezzanine

#endif
