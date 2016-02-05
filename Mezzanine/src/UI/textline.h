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

#ifndef _uitextline_h
#define _uitextline_h

#include "character.h"

namespace Mezzanine
{
    namespace UI
    {
        class TextLayer;
        ///////////////////////////////////////////////////////////////////////////////
        /// @brief This represents a single line of text to be rendered by a TextLayer.
        /// @details While many of the utilities of the TextLine class are exposed in public interfaces,
        /// it is important to note that this class as a whole is not intended to be used directly by game
        /// programmers.  These interfaces are available for those that need that level of control but it
        /// should be noted that manual manipulation of this class may disrupt the behaviors of not only this
        /// class but it's parent layer as well.  Query methods are, of course, safe from this.
        ///////////////////////////////////////
        class MEZZ_LIB TextLine
        {
        public:
            /// @brief Basic container type for @ref Character storage by this class.
            typedef std::list<Character*>                           CharacterContainer;
            /// @brief Iterator type for @ref Character instances stored by this class.
            typedef CharacterContainer::iterator                    CharacterIterator;
            /// @brief Const Iterator type for @ref Character instances stored by this class.
            typedef CharacterContainer::const_iterator              ConstCharacterIterator;
            /// @brief Reverse Iterator type for @ref Character instances stored by this class.
            typedef CharacterContainer::reverse_iterator            ReverseCharacterIterator;
            /// @brief Const Reverse Iterator type for @ref Character instances stored by this class.
            typedef CharacterContainer::const_reverse_iterator      ReverseConstCharacterIterator;
            /// @brief An std::pair storing two CharacterIterators, usually for expressing a range.
            typedef std::pair<CharacterIterator,CharacterIterator>  CharacterIteratorPair;
            /// @brief An std::pair storing a Character and it's offset position (relative to the parent layer).
            typedef std::pair<Character*,Vector2>                   OffsetResultPair;
        protected:
            /// @internal
            /// @brief Vector containing all the characters belonging to this TextLine.
            CharacterContainer Characters;
            /// @internal
            /// @brief The parent layer this text line belongs to.
            TextLayer* Parent;
            /// @internal
            /// @brief The current length of this TextLine.
            Real CurrLength;
            /// @internal
            /// @brief The size of the largest glyph on the Y axis.
            Real TallestHeight;
            /// @internal
            /// @brief The offset in pixels from the parent layer on the Y axis.
            Real PositionOffset;
            /// @internal
            /// @brief The alignment of the text on the X axis in this line.
            UI::LinearAlignment Alignment;

            /// @internal
            /// @brief Gets the character before the last character.
            /// @return Returns a pointer to the requested character.
            virtual Character* GetSecondFromLastCharacter() const = 0;
            /// @internal
            /// @brief Recalculates the offset for every character in this line.
            virtual void RecalculateOffsets() = 0;
            /// @internal
            /// @brief Adds a character such that it becomes the last in the sequence.
            virtual void AppendToBack(Character* ToAppend) = 0;
            /// @internal
            /// @brief Adds a series of characters to the end of this line.
            virtual void AppendToBack(CharacterIterator First, CharacterIterator Last) = 0;
        public:
            /// @brief Class constructor.
            /// @param ParentLayer The TextLayer that this TextLine belongs to.
            TextLine(TextLayer* ParentLayer);
            /// @brief Class destructor.
            virtual ~TextLine();

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            /// @brief Sets the current alignment for the text in this line of text.
            /// @param Align The enum value representing the horizontal alignment to be set.
            void SetAlignment(const UI::LinearAlignment Align);
            /// @brief Gets the current alignment for the text in this line of text.
            /// @return Returns a TextAlignment value representing the alignment the text in this line will have.
            UI::LinearAlignment GetAlignment() const;

            /// @brief Gets the character at the provided offset as well as the nearest valid position the cursor can take.
            /// @param Offset The offset from the left side of this TextLine to use when finding the result.
            /// @return Returns a OffsetResultPair with the "first" member containing a pointer to the character at the offset,
            /// or NULL if there isn't one and the "second" member containing the closest valid position the cursor can be placed at
            /// in the form of an offset from this TextLines left edge.
            OffsetResultPair GetCharacterAndCursorPositionAtOffset(const Real& Offset) const;
            /// @brief Gets the index of the character at the provided offset.
            /// @note The index returned is only among the characters in this line and is not indicative of the overall index among all characters in the parent layer.
            /// @param Offset The offset from the left side of this TextLine to use when finding the result.
            /// @return Returns an Integer representing the index of the character at the given offset.
            Integer GetIndexAtOffset(const Real& Offset) const;
            /// @brief Gets the Offset position of the character at the specified index.
            /// @note The index provided is only among the characters in this line and is not indicative of the overall index among all characters in the parent layer.
            /// @param Index The index of the character in this line to get the offset of.
            /// @return Returns a Real representing the distance in pixels the character at the provided index is from the left side of the TextLine.
            Real GetOffsetAtIndex(const Integer& Index) const;

            /// @brief Gets the offset position of the left-most part of this TextLine.
            /// @return Returns the left-most offset from the left side of this TextLine where the cursor can be placed.
            Real GetLeftMostCursorPosition() const;
            /// @brief Gets the offset position of the right-most part of this TextLine.
            /// @return Returns the right-most offset from the right side of this TextLine where the cursor can be placed.
            Real GetRightMostCursorPosition() const;
            /// @brief Gets the offset position of the cursor based on the text order of the layer this text line belongs to.
            /// @return Returns the left most cursor position if text is ordered left to right, and vice versa.
            virtual Real GetCursorStartPosition() const = 0;
            /// @brief Gets the closest valid cursor position to the offset provided.
            /// @param Offset The offset from the left side of the TextLine.
            /// @return Returns a Real containing the closest valid offset to the one provided.
            Real GetClosestCursorPosition(const Real& Offset);

            ///////////////////////////////////////////////////////////////////////////////
            // Transform Related Methods

            /// @brief Sets the offset on the Y axis from the parent layer.
            /// @warning Adjusting this value manually can create visual artifacts.  Only call this manually if you know what you are doing.
            /// @param Offset The offset in pixels from the top of the parent layer to be this TextLines position.
            void SetPositionOffset(const Real& Offset);
            /// @brief Gets the offset on the Y axis from the parent layer.
            /// @return Returns a Real representing the offset in pixels from the parent layer.
            Real GetPositionOffset() const;
            /// @brief Calculates the current length of this text line.
            /// @note This method will return the length of the non-clipped characters.
            /// @return Returns a Real representing the current length of this text line.
            Real GetCurrentLength() const;
            /// @brief Gets the size of this TextLine on the Y axis.
            /// @return Returns a Real representing the height of this TextLine.
            Real GetLineHeight() const;

            ///////////////////////////////////////////////////////////////////////////////
            // Character Management

            /// @brief Adds a Character to the end of this TextLine.
            /// @param ToAdd The Character to be added.
            /// @param MaxWidth The maximum line width to assume for all text lines that are to be populated.
            /// @return Returns true if the character was successfully added, false otherwise.
            Boole AppendCharacter(Character* ToAdd, const Real MaxWidth);
            /// @brief Adds a series of Characters to the end of this TextLine.
            /// @param ToAdd A container storing all Characters to attempt to add.
            /// @param MaxWidth The maximum line width to assume for all text lines that are to be populated.
            /// @return Returns true if all characters were successfully added, false otherwise.
            Boole AppendCharacters(CharacterContainer& ToAdd, const Real MaxWidth);
            /// @brief Adds a series of Characters to the end of this TextLine.
            /// @param Pair An std::pair containing iterators to both the first and last Characters in a sequence to attempt to append.
            /// @param MaxWidth The maximum line width to assume for all text lines that are to be populated.
            /// @return Returns true if all characters were successfully added, false otherwise.
            Boole AppendCharacters(CharacterIteratorPair Pair, const Real MaxWidth);
            /// @brief Adds a series of Characters to the end of this TextLine.
            /// @param First Iterator to the first Character in the series to be added.
            /// @param Last Iterator to one passed the last Character in the series to be added.
            /// @param MaxWidth The maximum line width to assume for all text lines that are to be populated.
            /// @return Returns true if all characters were successfully added, false otherwise.
            virtual Boole AppendCharacters(CharacterIterator First, CharacterIterator Last, const Real MaxWidth) = 0;
            /// @brief Adds as many Characters in a range as will fit to this TextLine.
            /// @param ToAdd A container storing all Characters to attempt to add.
            /// @param MaxWidth The maximum line width to assume for all text lines that are to be populated.
            /// @return Returns a CharacterIterator to the first character that was not added in the range.  This can be the left-most non-added Character when added to
            /// Left-To-Right TextLines, or the right-most non-added Character in Right-To-Left TextLines.
            CharacterIterator AppendFittingCharacters(CharacterContainer& ToAdd, const Real MaxWidth);
            /// @brief Adds as many Characters in a range as will fit to this TextLine.
            /// @param Pair An std::pair containing iterators to both the first and last Characters in a sequence to attempt to append.
            /// @param MaxWidth The maximum line width to assume for all text lines that are to be populated.
            /// @return Returns a CharacterIterator to the first character that was not added in the range.  This can be the left-most non-added Character when added to
            /// Left-To-Right TextLines, or the right-most non-added Character in Right-To-Left TextLines.
            CharacterIterator AppendFittingCharacters(CharacterIteratorPair Pair, const Real MaxWidth);
            /// @brief Adds as many Characters in a range as will fit to this TextLine.
            /// @param First Iterator to the first Character in the series to be added.
            /// @param Last Iterator to one passed the last Character in the series to be added.
            /// @param MaxWidth The maximum line width to assume for all text lines that are to be populated.
            /// @return Returns a CharacterIterator to the first character that was not added in the range.  This can be the left-most non-added Character when added to
            /// Left-To-Right TextLines, or the right-most non-added Character in Right-To-Left TextLines.
            virtual CharacterIterator AppendFittingCharacters(CharacterIterator First, CharacterIterator Last, const Real MaxWidth) = 0;

            /// @brief Gets the character in this textline at the specified index.
            /// @param Index The index of the character to retrieve.
            /// @return Returns a pointer to the character at the requested index, or NULL if out of bounds.
            Character* GetCharacterAtIndex(const UInt32& Index);
            /// @brief Gets the character in this textline at the specified offset.
            /// @param Offset The offset from the parent position to use when checking characters.
            /// @return Returns a pointer to the character at the requested offset, or NULL of none exists.
            Character* GetCharacterAtOffset(const Real& Offset);
            /// @brief Gets the number of characters in this TextLine.
            /// @return Returns a UInt32 containing the number of characters in this TextLine.
            UInt32 GetNumCharacters() const;
            /// @brief Removes all characters from this TextLine.
            void RemoveAllCharacters();

            /// @brief Gets the first character in this TextLine.
            /// @note The first character is based on the order in which the characters are read.  If the characters are
            /// meant to be read left-to-right then this will be the left most character.  If the characters are meant to
            /// be read right-to-left, this will be the right most character.
            /// @return Returns an iterator to the first character in this TextLine.
            virtual CharacterIterator GetFirstCharacter() = 0;
            /// @brief Gets the last character in this TextLine.
            /// @note The last character is based on the order in which the characters are read.  If the characters are
            /// meant to be read left-to-right then this will be the right most character.  If the characters are meant to
            /// be read right-to-left, this will be the left most character.
            /// @return Returns an iterator to the last character in this TextLine.
            virtual CharacterIterator GetLastCharacter() = 0;
            /// @brief Gets an iterator to the next Character.
            /// @param Current An iterator to a valid Character.
            /// @return Returns an iterator to the next Character in the sequence.
            virtual CharacterIterator GetNextCharacter(CharacterIterator Current) = 0;
            /// @brief Removes the character at the start of this TextLine.
            virtual void RemoveFirstCharacter() = 0;
            /// @brief Removes the character at the end of this TextLine.
            virtual void RemoveLastCharacter() = 0;

            /// @brief Gets an iterator to the first Character.
            /// @return Returns an iterator to the first Character being stored by this TextLine.
            CharacterIterator BeginCharacter();
            /// @brief Gets an iterator to one passed the last Character.
            /// @return Returns an iterator to one passed the last Character being stored by this TextLine.
            CharacterIterator EndCharacter();
            /// @brief Gets a const iterator to the first Character.
            /// @return Returns a const iterator to the first Character being stored by this TextLine.
            ConstCharacterIterator BeginCharacter() const;
            /// @brief Gets an iterator to one passed the last Character.
            /// @return Returns an iterator to one passed the last Character being stored by this TextLine.
            ConstCharacterIterator EndCharacter() const;
        };//TextLine

        ///////////////////////////////////////////////////////////////////////////////
        /// @brief This is a TextLine specialization class for text read from the left to the right.
        /// @details
        ///////////////////////////////////////
        class MEZZ_LIB LeftToRightTextLine : public TextLine
        {
        protected:
            /// @copydoc TextLine::GetSecondFromLastCharacter()
            Character* GetSecondFromLastCharacter() const;
            /// @copydoc TextLine::RecalculateOffsets()
            void RecalculateOffsets();
            /// @copydoc TextLine::AppendToBack(Character* ToAppend)
            void AppendToBack(Character* ToAppend);
            /// @copydoc TextLine::AppendToBack(CharacterIterator First, CharacterIterator Last)
            void AppendToBack(CharacterIterator First, CharacterIterator Last);
        public:
            /// @brief Class constructor.
            /// @param ParentLayer The TextLayer that this TextLine belongs to.
            LeftToRightTextLine(TextLayer* ParentLayer);
            /// @brief Class destructor.
            virtual ~LeftToRightTextLine();

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            /// @copydoc TextLine::GetCursorStartPosition() const
            Real GetCursorStartPosition() const;

            ///////////////////////////////////////////////////////////////////////////////
            // Character Management

            /// @copydoc TextLine::AppendCharacters(CharacterIterator, CharacterIterator, const Real)
            Boole AppendCharacters(CharacterIterator First, CharacterIterator Last, const Real MaxWidth);
            /// @copydoc TextLine::AppendCharactersAppendFittingCharacters(CharacterIterator, CharacterIterator, const Real)
            CharacterIterator AppendFittingCharacters(CharacterIterator First, CharacterIterator Last, const Real MaxWidth);

            /// @copydoc TextLine::GetFirstCharacter()
            CharacterIterator GetFirstCharacter();
            /// @copydoc TextLine::GetLastCharacter()
            CharacterIterator GetLastCharacter();
            /// @copydoc TextLine::GetNextCharacter(CharacterIterator Current)
            CharacterIterator GetNextCharacter(CharacterIterator Current);
            /// @copydoc TextLine::RemoveFirstCharacter()
            void RemoveFirstCharacter();
            /// @copydoc TextLine::RemoveLastCharacter()
            void RemoveLastCharacter();
        };//LeftToRightTextLine

        ///////////////////////////////////////////////////////////////////////////////
        /// @brief This is a TextLine specialization class for text read from the right to the left.
        /// @details
        ///////////////////////////////////////
        class MEZZ_LIB RightToLeftTextLine : public TextLine
        {
        protected:
            /// @copydoc TextLine::GetSecondFromLastCharacter()
            Character* GetSecondFromLastCharacter() const;
            /// @copydoc TextLine::RecalculateOffsets()
            void RecalculateOffsets();
            /// @copydoc TextLine::AppendToBack(Character* ToAppend)
            void AppendToBack(Character* ToAppend);
            /// @copydoc TextLine::AppendToBack(CharacterIterator First, CharacterIterator Last)
            void AppendToBack(CharacterIterator First, CharacterIterator Last);
        public:
            /// @brief Class constructor.
            /// @param ParentLayer The TextLayer that this TextLine belongs to.
            RightToLeftTextLine(TextLayer* ParentLayer);
            /// @brief Class destructor.
            virtual ~RightToLeftTextLine();

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            /// @copydoc TextLine::GetCursorStartPosition() const
            Real GetCursorStartPosition() const;

            ///////////////////////////////////////////////////////////////////////////////
            // Character Management

            /// @copydoc TextLine::AppendCharacters(CharacterIterator, CharacterIterator, const Real)
            Boole AppendCharacters(CharacterIterator First, CharacterIterator Last, const Real MaxWidth);
            /// @copydoc TextLine::AppendCharactersAppendFittingCharacters(CharacterIterator, CharacterIterator, const Real)
            CharacterIterator AppendFittingCharacters(CharacterIterator First, CharacterIterator Last, const Real MaxWidth);

            /// @copydoc TextLine::GetFirstCharacter()
            CharacterIterator GetFirstCharacter();
            /// @copydoc TextLine::GetLastCharacter()
            CharacterIterator GetLastCharacter();
            /// @copydoc TextLine::GetNextCharacter(CharacterIterator Current)
            CharacterIterator GetNextCharacter(CharacterIterator Current);
            /// @copydoc TextLine::RemoveFirstCharacter()
            void RemoveFirstCharacter();
            /// @copydoc TextLine::RemoveLastCharacter()
            void RemoveLastCharacter();
        };//RightToLeftTextLine
    }//UI
}//Mezzanine

#endif
