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

#ifndef _uitextline_cpp
#define _uitextline_cpp

#include "UI/textline.h"
#include "UI/textlayer.h"
#include "UI/quadrenderable.h"

#include <algorithm>

namespace Mezzanine
{
    namespace UI
    {
        ///////////////////////////////////////////////////////////////////////////////
        // TextLine Methods

        TextLine::TextLine(TextLayer* ParentLayer) :
            Parent(ParentLayer),
            CurrLength(0),
            TallestHeight(0),
            PositionOffset(0)
            {  }

        TextLine::~TextLine()
            {  }

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        void TextLine::SetAlignment(const UI::LinearAlignment Align)
        {
            if( this->Alignment != Align ) {
                this->Alignment = Align;
                this->RecalculateOffsets();
            }
        }

        UI::LinearAlignment TextLine::GetAlignment() const
        {
            return this->Alignment;
        }

        TextLine::OffsetResultPair TextLine::GetCharacterAndCursorPositionAtOffset(const Real& Offset) const
        {
            OffsetResultPair Ret(NULL,Vector2(0,0));
            ConstCharacterIterator CharIt = this->Characters.begin();
            // Check if we're too far to the left side to get anything
            if( Offset < (*CharIt)->GetLengthOffset() ) {
                Ret.first = NULL;
                Ret.second.X = this->GetLeftMostCursorPosition();
                Ret.second.Y = this->PositionOffset;
                return Ret;
            }

            while( CharIt != this->Characters.end() && Offset > (*CharIt)->GetRightEdgeLengthOffset() )
            {
                ++CharIt;
            }

            // Check if we're too far to the right side to get anything
            if( CharIt == (--this->Characters.end()) && Offset > (*CharIt)->GetRightEdgeLengthOffset() ) {
                Ret.first = NULL;
                Ret.second.X = this->GetRightMostCursorPosition();
                Ret.second.Y = this->PositionOffset;
                return Ret;
            }

            // Get our character dimensions for checking which side we should be on
            Real CharXPos = (*CharIt)->GetLengthOffset();
            Real CharXSize = (*CharIt)->GetCharacterSize().X;
            // Fill in our values
            Ret.first = (*CharIt);
            Ret.second.X = ( CharXPos + (CharXSize * 0.5) < Offset ? CharXPos + CharXSize : CharXPos );
            Ret.second.Y = this->PositionOffset;

            return Ret;
        }

        Integer TextLine::GetIndexAtOffset(const Real& Offset) const
        {
            if( !this->Characters.empty() ) {
                ConstCharacterIterator CharIt = this->Characters.begin();
                // Check if we're too far to the left side to get anything
                if( Offset < (*CharIt)->GetLengthOffset() )
                    return 0;

                Integer RetIndex = 0;
                while( CharIt != this->Characters.end() && Offset > (*CharIt)->GetRightEdgeLengthOffset() )
                {
                    ++CharIt;
                    ++RetIndex;
                }

                if( CharIt == this->Characters.end() )
                    return -1;

                // Get our character dimensions for checking which side we should be on
                Real CharXPos = (*CharIt)->GetLengthOffset();
                Real CharXSize = (*CharIt)->GetCharacterSize().X;
                return ( Offset < CharXPos + (CharXSize * 0.5) ? RetIndex : ++RetIndex );
            }else{
                return -1;
            }
        }

        Real TextLine::GetOffsetAtIndex(const Integer& Index) const
        {
            if( !this->Characters.empty() ) {
                if( Index < 0 || static_cast<Whole>(Index) > this->Characters.size() ) {
                    ConstCharacterIterator Last = --(this->Characters.end());
                    return (*Last)->GetLengthOffset() + (*Last)->GetCharacterSize().X;
                }else{
                    Integer IndexCount = 0;
                    ConstCharacterIterator CharIt = this->Characters.begin();

                    while( CharIt != this->Characters.end() && IndexCount < Index )
                    {
                        ++IndexCount;
                        ++CharIt;
                    }

                    return (*CharIt)->GetLengthOffset();
                }
            }else{
                return this->GetCursorStartPosition();
            }
        }

        Real TextLine::GetLeftMostCursorPosition() const
        {
            Real MaxWidth = this->Parent->GetMaxLineWidth();
            switch( this->Alignment )
            {
                case UI::LA_TopLeft:      return 0;                                      break;
                case UI::LA_BottomRight:  return MaxWidth - CurrLength;                  break;
                case UI::LA_Center:       return (MaxWidth * 0.5) - (CurrLength * 0.5);  break;
            }
            return 0;
        }

        Real TextLine::GetRightMostCursorPosition() const
        {
            Real MaxWidth = this->Parent->GetMaxLineWidth();
            switch( this->Alignment )
            {
                case UI::LA_TopLeft:      return CurrLength;                             break;
                case UI::LA_BottomRight:  return MaxWidth;                               break;
                case UI::LA_Center:       return (MaxWidth * 0.5) + (CurrLength * 0.5);  break;
            }
            return MaxWidth;
        }

        Real TextLine::GetClosestCursorPosition(const Real& Position)
        {
            CharacterIterator CharIt = this->Characters.begin();
            // Check if we're too far to the left side to get anything
            if( Position < (*CharIt)->GetLengthOffset() )
                return this->GetLeftMostCursorPosition();

            while( CharIt != this->Characters.end() && Position < (*CharIt)->GetLengthOffset() )
            {
                if( (*CharIt)->IsNewLine() )
                    continue;

                ++CharIt;
            }

            // Get some values to check/math with
            Real CharXPos = (*CharIt)->GetLengthOffset();
            Real CharXSize = (*CharIt)->GetCharacterSize().X;

            // Check if we're too far to the right side to get anything
            if( CharIt == (--(this->Characters.end())) && Position > CharXPos + CharXSize )
                return this->GetRightMostCursorPosition();

            return ( Position < CharXPos + (CharXSize * 0.5) ? CharXPos : CharXPos + CharXSize );
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Transform Related Methods

        void TextLine::SetPositionOffset(const Real& Offset)
            { this->PositionOffset = Offset; }

        Real TextLine::GetPositionOffset() const
            { return this->PositionOffset; }

        Real TextLine::GetCurrentLength() const
            { return this->CurrLength; }

        Real TextLine::GetLineHeight() const
            { return ( Characters.empty() ? 0 : this->TallestHeight ); }

        ///////////////////////////////////////////////////////////////////////////////
        // Character Management

        Boole TextLine::AppendCharacter(Character* ToAdd, const Real MaxWidth)
        {
            CharacterIterator Last = this->GetLastCharacter();
            Real CharAdvance = ToAdd->GetCharacterAdvance( Last != this->Characters.end() ? (*Last)->GetCharGlyph() : NULL );
            Real AddedLength = this->CurrLength + CharAdvance;

            if( AddedLength <= MaxWidth ) {
                this->CurrLength = AddedLength;
                this->TallestHeight = std::max(this->TallestHeight,ToAdd->GetLineHeight());
                this->AppendToBack(ToAdd);
                this->RecalculateOffsets();
                return true;
            }

            return false;
        }

        Boole TextLine::AppendCharacters(TextLine::CharacterContainer& ToAdd, const Real MaxWidth)
            { return this->AppendCharacters(ToAdd.begin(),ToAdd.end(),MaxWidth); }

        Boole TextLine::AppendCharacters(TextLine::CharacterIteratorPair Pair, const Real MaxWidth)
            { return this->AppendCharacters(Pair.first,Pair.second,MaxWidth); }

        TextLine::CharacterIterator TextLine::AppendFittingCharacters(TextLine::CharacterContainer& ToAdd, const Real MaxWidth)
            { return this->AppendFittingCharacters(ToAdd.begin(),ToAdd.end(),MaxWidth); }

        TextLine::CharacterIterator TextLine::AppendFittingCharacters(TextLine::CharacterIteratorPair Pair, const Real MaxWidth)
            { return this->AppendFittingCharacters(Pair.first,Pair.second,MaxWidth); }

        Character* TextLine::GetCharacterAtIndex(const UInt32& Index)
        {
            CharacterIterator CharIt = this->Characters.begin();
            // Check to see if the index is out of bounds
            if( Index >= this->Characters.size() )
                return NULL;

            UInt32 Count = 0;
            while( CharIt != this->Characters.end() && Count < Index )
            {
                ++CharIt;
            }

            return (*CharIt);
        }

        Character* TextLine::GetCharacterAtOffset(const Real& Offset)
        {
            CharacterIterator CharIt = this->Characters.begin();
            // Check if we're too far to the left side to get anything
            if( Offset < (*CharIt)->GetLengthOffset() )
                return NULL;

            while( CharIt != this->Characters.end() && Offset < (*CharIt)->GetLengthOffset() )
            {
                if( (*CharIt)->IsNewLine() )
                    continue;

                ++CharIt;
            }

            // Check if we're too far to the right side to get anything
            if( CharIt == (--(this->Characters.end())) && Offset > (*CharIt)->GetLengthOffset() + (*CharIt)->GetCharacterSize().X )
                return NULL;

            return (*CharIt);
        }

        UInt32 TextLine::GetNumCharacters() const
        {
            return this->Characters.size();
        }

        void TextLine::RemoveAllCharacters()
        {
            this->CurrLength = 0;
            this->TallestHeight = 0;

            this->Characters.clear();
            this->Parent->_MarkDirty();
        }

        TextLine::CharacterIterator TextLine::BeginCharacter()
            { return this->Characters.begin(); }

        TextLine::CharacterIterator TextLine::EndCharacter()
            { return this->Characters.end(); }

        TextLine::ConstCharacterIterator TextLine::BeginCharacter() const
            { return this->Characters.begin(); }

        TextLine::ConstCharacterIterator TextLine::EndCharacter() const
            { return this->Characters.end(); }

        ///////////////////////////////////////////////////////////////////////////////
        // LeftToRightTextLine Methods

        LeftToRightTextLine::LeftToRightTextLine(TextLayer* ParentLayer) :
            TextLine(ParentLayer)
            { this->Alignment = UI::LA_TopLeft; }

        LeftToRightTextLine::~LeftToRightTextLine()
            {  }

        Character* LeftToRightTextLine::GetSecondFromLastCharacter() const
        {
            if( this->Characters.size() > 1 ) return *(++(this->Characters.rbegin()));
            else return NULL;
        }

        void LeftToRightTextLine::RecalculateOffsets()
        {
            Real CursorPosition = this->GetLeftMostCursorPosition();
            Character* Previous = NULL;
            for( CharacterIterator CharIt = this->Characters.begin() ; CharIt != this->Characters.end() ; ++CharIt )
            {
                (*CharIt)->SetLengthOffset(CursorPosition);
                CursorPosition += (*CharIt)->GetCharacterAdvance( Previous != NULL ? Previous->GetCharGlyph() : NULL );
                Previous = (*CharIt);
            }
            this->Parent->_MarkDirty();
        }

        void LeftToRightTextLine::AppendToBack(Character* ToAppend)
        {
            this->Characters.push_back(ToAppend);
        }

        void LeftToRightTextLine::AppendToBack(TextLine::CharacterIterator First, TextLine::CharacterIterator Last)
        {
            this->Characters.insert(this->Characters.end(),First,Last);
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        Real LeftToRightTextLine::GetCursorStartPosition() const
        {
            return this->GetLeftMostCursorPosition();
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Character Management

        Boole LeftToRightTextLine::AppendCharacters(TextLine::CharacterIterator First, TextLine::CharacterIterator Last, const Real MaxWidth)
        {
            if( First == Last )
                return false;

            // Set up data to be used
            Real Tallest = 0, SequenceLength = 0;
            Character* Previous = ( this->GetLastCharacter() != this->Characters.end() ? *(this->GetLastCharacter()) : NULL  );

            // Collect the data we need from the provided characters
            for( CharacterIterator CharIt = First ; CharIt != Last ; ++CharIt )
            {
                SequenceLength += (*CharIt)->GetCharacterAdvance( Previous != NULL ? Previous->GetCharGlyph() : NULL );
                Tallest = std::max(Tallest,(*CharIt)->GetLineHeight());
                Previous = (*CharIt);
            }

            Real AddedLength = SequenceLength + this->CurrLength;
            if( AddedLength <= MaxWidth )
            {
                this->CurrLength = AddedLength;
                this->TallestHeight = std::max(TallestHeight,Tallest);
                this->AppendToBack(First,Last);
                this->RecalculateOffsets();
                return true;
            }

            return false;
        }

        TextLine::CharacterIterator LeftToRightTextLine::AppendFittingCharacters(TextLine::CharacterIterator First, TextLine::CharacterIterator Last, const Real MaxWidth)
        {
            if( First == Last )
                return First;

            // Set up data to be used
            Character* Previous = ( this->GetLastCharacter() != this->Characters.end() ? *(this->GetLastCharacter()) : NULL  );

            // Setup our iterator that will be used with our return and append operations
            CharacterIterator CharIt = First;

            // Loop through our range
            for(  ; CharIt != Last ; ++CharIt )
            {
                Real CharAdvance = (*CharIt)->GetCharacterAdvance( Previous != NULL ? Previous->GetCharGlyph() : NULL );
                Real AddedLength = CurrLength + CharAdvance;
                if( AddedLength <= MaxWidth ) {
                    this->CurrLength = AddedLength;
                    this->TallestHeight = std::max(this->TallestHeight,(*CharIt)->GetLineHeight());
                }else{
                    break;
                }
                Previous = (*CharIt);
            }

            // Append, adjust offsets, and return
            this->AppendToBack(First,CharIt);
            this->RecalculateOffsets();
            return CharIt;
        }

        TextLine::CharacterIterator LeftToRightTextLine::GetFirstCharacter()
        {
            if( !this->Characters.empty() ) return this->Characters.begin();
            else return this->Characters.end();
        }

        TextLine::CharacterIterator LeftToRightTextLine::GetLastCharacter()
        {
            if( !this->Characters.empty() ) return --(this->Characters.end());
            else return this->Characters.end();
        }

        TextLine::CharacterIterator LeftToRightTextLine::GetNextCharacter(TextLine::CharacterIterator Current)
        {
            if( Current != this->Characters.end() ) {
                CharacterIterator Copy = Current;
                return ++Copy;
            }
            return this->Characters.end();
        }

        void LeftToRightTextLine::RemoveFirstCharacter()
        {
            this->Characters.pop_front();
            //Characters.erase(Characters.begin());
            this->RecalculateOffsets();
        }

        void LeftToRightTextLine::RemoveLastCharacter()
        {
            this->Characters.pop_back();
            this->RecalculateOffsets();
        }

        ///////////////////////////////////////////////////////////////////////////////
        // RightToLeftTextLine Methods

        RightToLeftTextLine::RightToLeftTextLine(TextLayer* ParentLayer) :
            TextLine(ParentLayer)
            { this->Alignment = UI::LA_BottomRight; }

        RightToLeftTextLine::~RightToLeftTextLine()
            {  }

        Character* RightToLeftTextLine::GetSecondFromLastCharacter() const
        {
            if( this->Characters.size() > 1 ) return *(++(this->Characters.begin()));
            else return NULL;
        }

        void RightToLeftTextLine::RecalculateOffsets()
        {
            Real CursorPosition = this->GetRightMostCursorPosition();
            Character* Previous = NULL;
            for( ReverseCharacterIterator CharIt = this->Characters.rbegin() ; CharIt != this->Characters.rend() ; ++CharIt )
            {
                CursorPosition -= (*CharIt)->GetCharacterAdvance( Previous != NULL ? Previous->GetCharGlyph() : NULL );
                (*CharIt)->SetLengthOffset(CursorPosition);
                Previous = (*CharIt);
            }
            this->Parent->_MarkDirty();
        }

        void RightToLeftTextLine::AppendToBack(Character* ToAppend)
        {
            this->Characters.insert(this->Characters.begin(),ToAppend);
        }

        void RightToLeftTextLine::AppendToBack(TextLine::CharacterIterator First, TextLine::CharacterIterator Last)
        {
            this->Characters.insert(this->Characters.begin(),First,Last);
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        Real RightToLeftTextLine::GetCursorStartPosition() const
        {
            return this->GetRightMostCursorPosition();
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Character Management

        Boole RightToLeftTextLine::AppendCharacters(TextLine::CharacterIterator First, TextLine::CharacterIterator Last, const Real MaxWidth)
        {
            if( First == Last )
                return false;

            // Set up data to be used
            Real Tallest = 0, SequenceLength = 0;
            Character* Previous = ( this->GetLastCharacter() != this->Characters.end() ? *(this->GetLastCharacter()) : NULL  );

            // Setup our iterators for iteration (format provided is for insertion)
            CharacterIterator FirstCopy = First;
            CharacterIterator LastCopy = Last;
            --FirstCopy;
            --LastCopy;

            // Collect the data we need from the provided characters
            for( CharacterIterator CharIt = LastCopy ; CharIt != FirstCopy ; --CharIt )
            {
                SequenceLength += (*CharIt)->GetCharacterAdvance( Previous != NULL ? Previous->GetCharGlyph() : NULL );
                Tallest = std::max(Tallest,(*CharIt)->GetLineHeight());
                Previous = (*CharIt);
            }

            Real AddedLength = SequenceLength + this->CurrLength;
            if( AddedLength <= MaxWidth )
            {
                this->CurrLength = AddedLength;
                this->TallestHeight = std::max(TallestHeight,Tallest);
                this->AppendToBack(First,Last);
                this->RecalculateOffsets();
                return true;
            }

            return false;
        }

        TextLine::CharacterIterator RightToLeftTextLine::AppendFittingCharacters(TextLine::CharacterIterator First, TextLine::CharacterIterator Last, const Real MaxWidth)
        {
            if( First == Last )
                return First;

            // Set up data to be used
            Character* Previous = ( this->GetLastCharacter() != this->Characters.end() ? *(this->GetLastCharacter()) : NULL  );

            // Setup our iterators for iteration (format provided is for insertion)
            CharacterIterator FirstCopy = First;
            CharacterIterator LastCopy = Last;
            --FirstCopy;
            --LastCopy;

            // Setup our iterator that will be used with our return and append operations
            CharacterIterator CharIt = LastCopy;

            // Loop through our range
            for(  ; CharIt != FirstCopy ; --CharIt )
            {
                Real CharAdvance = (*CharIt)->GetCharacterAdvance( Previous != NULL ? Previous->GetCharGlyph() : NULL );
                Real AddedLength = CurrLength + CharAdvance;
                if( AddedLength <= MaxWidth ) {
                    this->CurrLength = AddedLength;
                    this->TallestHeight = std::max(this->TallestHeight,(*CharIt)->GetLineHeight());
                }else{
                    break;
                }
                Previous = (*CharIt);
            }

            // Append, adjust offsets, and return
            this->AppendToBack(CharIt,Last);
            this->RecalculateOffsets();
            return CharIt;
        }

        TextLine::CharacterIterator RightToLeftTextLine::GetFirstCharacter()
        {
            if( !this->Characters.empty() ) return --(this->Characters.end());
            else return this->Characters.end();
        }

        TextLine::CharacterIterator RightToLeftTextLine::GetLastCharacter()
        {
            if( !this->Characters.empty() ) return this->Characters.begin();
            else return this->Characters.end();
        }

        TextLine::CharacterIterator RightToLeftTextLine::GetNextCharacter(TextLine::CharacterIterator Current)
        {
            CharacterIterator Copy = Current;
            if( *Current != this->Characters.front() ) return --Copy;
            else return Copy;
        }

        void RightToLeftTextLine::RemoveFirstCharacter()
        {
            this->Characters.pop_back();
            this->RecalculateOffsets();
        }

        void RightToLeftTextLine::RemoveLastCharacter()
        {
            this->Characters.pop_front();
            //Characters.erase(Characters.begin());
            this->RecalculateOffsets();
        }
    }//UI
}//Mezzanine

#endif
