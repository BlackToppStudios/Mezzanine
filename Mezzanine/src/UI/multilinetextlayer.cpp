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
#ifndef _uimultilinetextlayer_cpp
#define _uimultilinetextlayer_cpp

#include "UI/multilinetextlayer.h"
#include "UI/uimanager.h"
#include "UI/textline.h"
#include "UI/character.h"
#include "countedptr.h"

#include <algorithm>

namespace Mezzanine
{
    namespace UI
    {
        MultiLineTextLayer::MultiLineTextLayer(QuadRenderable* ParentRenderable) :
            TextLayer(ParentRenderable)
            {  }

        MultiLineTextLayer::MultiLineTextLayer(const String& FontName, QuadRenderable* ParentRenderable) :
            TextLayer(FontName,ParentRenderable)
            {  }

        MultiLineTextLayer::MultiLineTextLayer(const Real& LineHeight, QuadRenderable* ParentRenderable) :
            TextLayer(LineHeight,ParentRenderable)
            {  }

        MultiLineTextLayer::~MultiLineTextLayer()
            {  }

        void MultiLineTextLayer::PopulateTextLinesImpl(const Real MaxWidth)
        {
            //if( MaxWidth <= 0 )
            //    return;

            Whole LineIndex = 0;
            Boole NewLineDetected = false;
            TextLine* CurrLine = this->GetOrCreateTextLine(LineIndex);
            if( HorizontalOrder == UI::TO_Left_To_Right ) {
                CharacterIterator Start = this->Characters.begin();
                CharacterIterator End = this->Characters.begin();
                const CharacterIterator CharEnd = this->Characters.end();

                do{
                    if( (*Start)->IsWhiteSpace() ) {
                        // Find the next non-whitespace character.
                        while( End != CharEnd )
                        {
                            if( (*End)->IsNewLine() ) {
                                NewLineDetected = true;
                                ++End;
                                break;
                            }else if( !(*End)->IsWhiteSpace() ) {
                                break;
                            }

                            ++End;
                        }

                        // We got our range, append what we can.
                        CharacterIterator Result = CurrLine->AppendFittingCharacters(Start,End,MaxWidth);
                        if( Result != End || NewLineDetected ) {
                            CurrLine = this->GetOrCreateTextLine(++LineIndex);
                            End = CurrLine->AppendFittingCharacters(Result,End,MaxWidth);
                            if( Start == End ) {
                                // If these are the same, then we lack the space to append anything.
                                break;
                            }
                        }
                        // Clear out the newline for the next iteration.
                        NewLineDetected = false;
                    }else{
                        // Find the next whitespace character.
                        while( End != CharEnd )
                        {
                            if( (*End)->IsWhiteSpace() )
                                break;

                            ++End;
                        }

                        // We got our range, so lets try to insert it.
                        if( !CurrLine->AppendCharacters(Start,End,MaxWidth) )
                        {
                            // If we failed to insert, get the next line and try again.
                            CurrLine = this->GetOrCreateTextLine(++LineIndex);
                            if( !CurrLine->AppendCharacters(Start,End,MaxWidth) ) {
                                End = CurrLine->AppendFittingCharacters(Start,End,MaxWidth);
                                if( Start == End ) {
                                    // If these are the same, then we lack the space to append anything.
                                    break;
                                }
                            }
                        }
                    }
                    Start = End;
                }while( Start != CharEnd );
            }
            else if( HorizontalOrder == UI::TO_Right_To_Left )
            {
                CharacterIterator Start = --this->Characters.end();
                CharacterIterator End = --this->Characters.end();
                const CharacterIterator CharEnd = --this->Characters.begin();

                CharacterIteratorPair AppendPair;

                do{
                    if( (*End)->IsWhiteSpace() ) {
                        // Find the next non-whitespace character.
                        while( Start != CharEnd )
                        {
                            if( (*Start)->IsNewLine() ) {
                                NewLineDetected = true;
                                ++Start;
                                break;
                            }else if( !(*Start)->IsWhiteSpace() ) {
                                break;
                            }

                            ++Start;
                        }

                        AppendPair.first = Start;
                        AppendPair.second = End;
                        ++AppendPair.first;
                        ++AppendPair.second;

                        // We got our range, append what we can.
                        CharacterIterator Result = CurrLine->AppendFittingCharacters(AppendPair,MaxWidth);
                        if( Result != Start || NewLineDetected ) {
                            CurrLine = this->GetOrCreateTextLine(++LineIndex);
                            Start = CurrLine->AppendFittingCharacters(Result,End,MaxWidth);
                            if( End == Start ) {
                                // If these are the same, then we lack the space to append anything.
                                break;
                            }
                        }
                        NewLineDetected = false;
                    }else{
                        // Find the next whitespace character.
                        while( Start != CharEnd )
                        {
                            if( (*Start)->IsWhiteSpace() )
                                break;

                            ++Start;
                        }

                        AppendPair.first = Start;
                        AppendPair.second = End;
                        ++AppendPair.first;
                        ++AppendPair.second;

                        // We got our range, so lets try to insert it.
                        if( !CurrLine->AppendCharacters(AppendPair,MaxWidth) ) {
                            // If we failed to insert, get the next line and try again.
                            CurrLine = this->GetOrCreateTextLine(++LineIndex);
                            if( !CurrLine->AppendCharacters(AppendPair,MaxWidth) ) {
                                Start = CurrLine->AppendFittingCharacters(AppendPair,MaxWidth);
                                if( End == Start ) {
                                    // If these are the same, then we lack the space to append anything.
                                    break;
                                }
                            }
                        }
                    }
                    End = Start;
                }while( CharEnd != CharEnd );
            }

            // Clean up unused text lines.
            if( this->TextLines.size() > 1 && this->TextLines.size() >= LineIndex ) {
                TextLines.erase(this->TextLines.begin() + LineIndex + 1, this->TextLines.end());
            }
        }

        TextLayer::CharIndexPair MultiLineTextLayer::GetIndexAtOffsetImpl(const Vector2& Offset)
        {
            CharIndexPair Ret(true,0);
            TextLineIterator LineIt = this->TextLines.begin();
            // Check if we're too far to the top to get anything
            if( Offset.Y < (*LineIt)->GetPositionOffset() - (*LineIt)->GetLineHeight() )
                return CharIndexPair(false,0);

            Integer IndexCount = 0;
            while( LineIt != TextLines.end() && Offset.Y < (*LineIt)->GetPositionOffset() )
            {
                IndexCount += static_cast<Integer>( (*LineIt)->GetNumCharacters() );
                ++LineIt;
            }

            // Check if we're too low to get anything
            if( LineIt == (--TextLines.end()) && Offset.Y > (*LineIt)->GetPositionOffset() )
                return CharIndexPair(false,0);

            Ret.second = IndexCount + (*LineIt)->GetIndexAtOffset(Offset.X);
            return Ret;
        }

        TextLayer::CharOffsetPair MultiLineTextLayer::GetOffsetAtIndexImpl(const Integer Index)
        {
            CharOffsetPair Ret;
            Ret.first = true;
            if( Index < 0 || static_cast<Whole>( Index ) >= this->Characters.size() ) {
                TextLineIterator Last = --this->TextLines.end();
                Ret.second.Y = (*Last)->GetPositionOffset();
                Ret.second.X = (*Last)->GetOffsetAtIndex(-1);
                return Ret;
            }else{
                Integer IndexCount = 0;

                TextLineIterator LineIt = this->TextLines.begin();
                while( LineIt != this->TextLines.end() && IndexCount + static_cast<Integer>( (*LineIt)->GetNumCharacters() ) < Index )
                {
                    IndexCount += (*LineIt)->GetNumCharacters();
                    ++LineIt;
                }
                Ret.second.Y = (*LineIt)->GetPositionOffset();
                Ret.second.X = (*LineIt)->GetOffsetAtIndex( Index - IndexCount );
                return Ret;
            }
        }

        TextLine* MultiLineTextLayer::GetOrCreateTextLine(const UInt32 Index)
        {
            if( Index >= this->TextLines.size() ) return this->CreateTextLine();
            else return this->TextLines.at(Index);
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        UI::RenderLayerType MultiLineTextLayer::GetLayerType() const
        {
            return UI::RLT_MultiLineText;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Text Methods

        ///////////////////////////////////////////////////////////////////////////////
        // Serialization

        void MultiLineTextLayer::ProtoSerializeProperties(XML::Node& SelfRoot) const
        {
            TextLayer::ProtoSerializeProperties(SelfRoot);
        }

        void MultiLineTextLayer::ProtoDeSerializeProperties(const XML::Node& SelfRoot)
        {
            TextLayer::ProtoDeSerializeProperties(SelfRoot);
        }

        String MultiLineTextLayer::GetDerivedSerializableName() const
        {
            return this->MultiLineTextLayer::GetSerializableName();
        }

        String MultiLineTextLayer::GetSerializableName()
        {
            return "MultiLineTextLayer";
        }
    }//UI
}//Mezzanine

#endif
