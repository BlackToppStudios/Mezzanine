//© Copyright 2010 - 2012 BlackTopp Studios Inc.
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

#ifndef _uitextlayer_cpp
#define _uitextlayer_cpp

#include "UI/textlayer.h"
#include "UI/textline.h"
#include "UI/textcursor.h"
#include "UI/texttoken.h"
#include "UI/defaultmarkupparser.h"
#include "UI/widget.h"
#include "UI/screen.h"

#include "uimanager.h"
#include "unicode.h"
#include "stringtool.h"
#include "mathtool.h"

namespace Mezzanine
{
    namespace UI
    {
        TextLayer::TextLayer(QuadRenderable* ParentRenderable) :
            RenderLayer(ParentRenderable),

            CharScaling(1.0,1.0),
            HighlightStart(-1),
            HighlightEnd(-1),
            HorizontalOrder(UI::TO_Left_To_Right), // Default to english
            VerticalAlign(UI::LA_TopLeft),
            ScalingChanged(false),
            MUParser(NULL),
            Cursor(NULL),
            TextTokens(NULL)
        {
            // Set our colour defaults
            this->DefaultCharTraits.CharColour = ColourValue::White();
            this->ActiveHLColour = ColourValue::Blue();
            this->InactiveHLColour = ColourValue::Gray();
            this->ActiveHLColour.AlphaChannel = 0.5;
            this->InactiveHLColour.AlphaChannel = 0.5;

            // Set relative height default
            this->RelLineHeight = 0.0;

            // Create our one free line for text
            this->CreateTextLine();

            // Set the default parser
            this->MUParser = Parent->GetScreen()->GetMarkupParser("Default");

            // This constructor does not set a font.  A font will need to be set prior to setting any text.
        }

        TextLayer::TextLayer(const String& FontName, QuadRenderable* ParentRenderable) :
            RenderLayer(ParentRenderable),

            CharScaling(1.0,1.0),
            HighlightStart(-1),
            HighlightEnd(-1),
            HorizontalOrder(UI::TO_Left_To_Right), // Default to english
            VerticalAlign(UI::LA_TopLeft),
            ScalingChanged(false),
            MUParser(NULL),
            Cursor(NULL),
            TextTokens(NULL)
        {
            // Set our colour defaults
            this->DefaultCharTraits.CharColour = ColourValue::White();
            this->ActiveHLColour = ColourValue::Blue();
            this->InactiveHLColour = ColourValue::Gray();
            this->ActiveHLColour.AlphaChannel = 0.5;
            this->InactiveHLColour.AlphaChannel = 0.5;

            // Set relative height default
            this->RelLineHeight = 0.0;

            // Set our font
            this->DefaultCharTraits.CharFont = Parent->GetScreen()->GetFont(FontName,PriAtlas);

            // Create our one free line for text
            this->CreateTextLine();

            // Set the default parser
            this->MUParser = Parent->GetScreen()->GetMarkupParser("Default");
        }

        TextLayer::TextLayer(const Real& LineHeight, QuadRenderable* ParentRenderable) :
            RenderLayer(ParentRenderable),

            CharScaling(1.0,1.0),
            HighlightStart(-1),
            HighlightEnd(-1),
            HorizontalOrder(UI::TO_Left_To_Right), // Default to english
            VerticalAlign(UI::LA_TopLeft),
            ScalingChanged(false),
            MUParser(NULL),
            Cursor(NULL),
            TextTokens(NULL)
        {
            // Set our colour defaults
            this->DefaultCharTraits.CharColour = ColourValue::White();
            this->ActiveHLColour = ColourValue::Blue();
            this->InactiveHLColour = ColourValue::Gray();
            this->ActiveHLColour.AlphaChannel = 0.5;
            this->InactiveHLColour.AlphaChannel = 0.5;

            // Set our font
            this->RelLineHeight = LineHeight;
            UIManager::FontResult Result = UIManager::GetSingletonPtr()->SuggestGlyphIndex(static_cast<UInt32>(LineHeight * Parent->GetActualSize().Y),PriAtlas);
            this->DefaultCharTraits.CharFont = this->Parent->GetScreen()->GetFont(Result.first,PriAtlas);
            this->SetTextScale(Vector2(Result.second,Result.second));

            // Create our one free line for text
            this->CreateTextLine();

            // Set the default parser
            this->MUParser = Parent->GetScreen()->GetMarkupParser("Default");
        }

        TextLayer::~TextLayer()
        {
            this->ClearAllTextLines();
            this->DestroyAllCharacters();
            this->DestroyAllTextLines();
        }

        void TextLayer::RedrawImpl(bool Force)
        {
            // Update our text
            if(Force || this->ScalingChanged)
            {
                // In general the only time this will be forced is on a resizing of the parent
                // quad renderable.  So we must repopulate for the change of space.
                this->PopulateTextLines();
                this->ScalingChanged = false;
            }

            // Get the Texel Offsets
            Real TexelOffsetX = this->Parent->GetScreen()->GetTexelOffsetX();
            Real TexelOffsetY = this->Parent->GetScreen()->GetTexelOffsetY();

            // Get the parent rect and apply the scaling
            Rect ActDims = this->GetAreaRect();

            // Setup the text specific data we'll use
            Vector2 TopLeft, TopRight, BottomLeft, BottomRight;
            Vector2 WhitePixelCoords;
            Real CursorX = ActDims.Position.X, CursorY = ActDims.Position.Y;
            ColourValue HighlightColour = this->InactiveHLColour;
            Character* CurrChar = NULL;
            TextLine* CurrLine = NULL;

            // Check if we need the active highlight colour
            if( Renderable::RT_Widget == this->Parent->GetRenderableType() && (static_cast<Widget*>(this->Parent))->HasFocus() )
                HighlightColour = ActiveHLColour;

            for( TextLineIterator LineIt = this->TextLines.begin() ; LineIt != this->TextLines.end() ; ++LineIt )
            {
                CurrLine = (*LineIt);
                CursorY = ActDims.Position.Y + CurrLine->GetPositionOffset();

                // If we have ran out of room, halt rendering
                if( CursorY >= ActDims.Position.Y + ActDims.Size.Y )
                    break;

                // Update the X cursor starting position
                CursorX = ActDims.Position.X + CurrLine->GetCursorStartPosition();

                for( TextLine::CharacterIterator CharIt = CurrLine->BeginCharacter() ; CharIt != CurrLine->EndCharacter() ; ++CharIt )
                {
                    CurrChar = (*CharIt);
                    if( !CurrChar->IsRenderable() )
                        continue;

                    String CharAtlas = CurrChar->GetAtlasName();
                    Real VertOffset = CurrChar->GetVerticalOffset() * this->CharScaling.Y;
                    Vector2 CharSize = CurrChar->GetCharacterSize() * this->CharScaling;
                    ColourValue CharColour = CurrChar->GetCharacterColour();
                    CursorX = ActDims.Position.X + CurrChar->GetLengthOffset();

                    TopLeft.SetValues( MathTools::Floor( CursorX - TexelOffsetX ),
                                       MathTools::Floor( (CursorY + TexelOffsetY + VertOffset) - CharSize.Y ) );
                    BottomRight.SetValues( MathTools::Floor( (CursorX - TexelOffsetX) + CharSize.X ),
                                           MathTools::Floor( CursorY + TexelOffsetY + VertOffset ) );
                    TopRight.SetValues( BottomRight.X, TopLeft.Y );
                    BottomLeft.SetValues( TopLeft.X, BottomRight.Y );

                    this->RotationTransform(TopLeft,TopRight,BottomLeft,BottomRight);

                    if( CurrChar->GetHighlighted() )
                    {
                        WhitePixelCoords = CurrChar->GetAtlasWhitePixel();

                        // Triangle A
                        PushVertex(BottomLeft.X, BottomLeft.Y, WhitePixelCoords, HighlightColour, CharAtlas);     // Left/Bottom  3
                        PushVertex(TopRight.X, TopRight.Y, WhitePixelCoords, HighlightColour, CharAtlas);         // Right/Top    1
                        PushVertex(TopLeft.X, TopLeft.Y, WhitePixelCoords, HighlightColour, CharAtlas);           // Left/Top     0

                        // Triangle B
                        PushVertex(BottomLeft.X, BottomLeft.Y, WhitePixelCoords, HighlightColour, CharAtlas);     // Left/Bottom  3
                        PushVertex(BottomRight.X, BottomRight.Y, WhitePixelCoords, HighlightColour, CharAtlas);   // Right/Bottom 2
                        PushVertex(TopRight.X, TopRight.Y, WhitePixelCoords, HighlightColour, CharAtlas);         // Right/Top    1
                    }

                    // Triangle A
                    PushVertex(BottomLeft.X, BottomLeft.Y, CurrChar->GetAtlasCoords(UI::QC_BottomLeft), CharColour, CharAtlas);    // Left/Bottom  3
                    PushVertex(TopRight.X, TopRight.Y, CurrChar->GetAtlasCoords(UI::QC_TopRight), CharColour, CharAtlas);          // Right/Top    1
                    PushVertex(TopLeft.X, TopLeft.Y, CurrChar->GetAtlasCoords(UI::QC_TopLeft), CharColour, CharAtlas);             // Left/Top     0

                    // Triangle B
                    PushVertex(BottomLeft.X, BottomLeft.Y, CurrChar->GetAtlasCoords(UI::QC_BottomLeft), CharColour, CharAtlas);    // Left/Bottom  3
                    PushVertex(BottomRight.X, BottomRight.Y, CurrChar->GetAtlasCoords(UI::QC_BottomRight), CharColour, CharAtlas); // Right/Bottom 2
                    PushVertex(TopRight.X, TopRight.Y, CurrChar->GetAtlasCoords(UI::QC_TopRight), CharColour, CharAtlas);          // Right/Top    1
                }//for each character
            }//for each text line

            // Last thing to do is rendering the text cursor
            if( this->GetCursorEnabled() ) {
                if( this->Cursor->GetVisible() ) {
                    Rect CursorRect = Cursor->GetCursorRect();
                    ColourValue CursorColour = Cursor->GetColour();
                    WhitePixelCoords = Parent->GetScreen()->GetWhitePixel(PriAtlas);

                    BottomLeft = ActDims.Position + CursorRect.Position;
                    TopRight.SetValues( BottomLeft.X + CursorRect.Size.X , BottomLeft.Y - CursorRect.Size.Y );
                    TopLeft.SetValues( BottomLeft.X , TopRight.Y );
                    BottomRight.SetValues( TopRight.X , BottomLeft.Y );

                    // Triangle A
                    PushVertex(BottomLeft.X, BottomLeft.Y, WhitePixelCoords, CursorColour, PriAtlas);     // Left/Bottom  3
                    PushVertex(TopRight.X, TopRight.Y, WhitePixelCoords, CursorColour, PriAtlas);         // Right/Top    1
                    PushVertex(TopLeft.X, TopLeft.Y, WhitePixelCoords, CursorColour, PriAtlas);           // Left/Top     0

                    // Triangle B
                    PushVertex(BottomLeft.X, BottomLeft.Y, WhitePixelCoords, CursorColour, PriAtlas);     // Left/Bottom  3
                    PushVertex(BottomRight.X, BottomRight.Y, WhitePixelCoords, CursorColour, PriAtlas);   // Right/Bottom 2
                    PushVertex(TopRight.X, TopRight.Y, WhitePixelCoords, CursorColour, PriAtlas);         // Right/Top    1
                }
            }
        }

        Real TextLayer::GetTotalHeight() const
        {
            Real Ret = 0;
            for( ConstTextLineIterator TextIt = TextLines.begin() ; TextIt != TextLines.end() ; ++TextIt )
            {
                Ret += (*TextIt)->GetLineHeight();
            }
            return Ret;
        }

        void TextLayer::RecalculateOffsets()
        {
            // Get the actual starting position on the Y axis
            Real CursorY = 0;
            Real TotalHeight = this->GetTotalHeight();
            Rect ActDims = this->GetAreaRect();
            if( TotalHeight < ActDims.Size.Y )
            {
                switch(VerticalAlign)
                {
                    case UI::LA_TopLeft:      /* do nothing, we're at the top */                                              break;
                    case UI::LA_BottomRight:  CursorY = (ActDims.Position.Y + ActDims.Size.Y) - TotalHeight;                  break;
                    case UI::LA_Center:       CursorY = (ActDims.Position.Y + (ActDims.Size.Y * 0.5)) - (TotalHeight * 0.5);  break;
                }
            }
            // Set all the actual offsets
            for( TextLineIterator TLIt = TextLines.begin() ; TLIt != TextLines.end() ; ++TLIt )
            {
                CursorY += (*TLIt)->GetLineHeight();
                (*TLIt)->SetPositionOffset(CursorY);
            }
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Parser Methods

        bool TextLayer::SetMarkupParser(MarkupParser* Parser)
        {
            if( Parser != NULL )
            {
                this->MUParser = Parser;
                return true;
            }else{
                return false;
            }
        }

        bool TextLayer::SetMarkupParser(const String& ParserName)
        {
            MarkupParser* Parser = Parent->GetScreen()->GetMarkupParser(ParserName);
            return this->SetMarkupParser(Parser);
        }

        MarkupParser* TextLayer::GetMarkupParser() const
        {
            return this->MUParser;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        void TextLayer::SetScale(const Vector2& Scaling)
        {
            RenderLayer::SetScale(Scaling);
            this->ScalingChanged = true;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Offset - Index Conversion Methods

        TextLayer::CharIndexPair TextLayer::GetIndexAtOffset(const Vector2& Offset)
        {
            // Get the parent rect and apply the scaling
            Rect LayerDims = this->GetAreaRect();
            if( !LayerDims.IsInside(LayerDims.Position + Offset) || 0 == this->GetNumTextLines() )
                return CharIndexPair(false,0);

            return this->GetIndexAtOffsetImpl(Offset);
        }

        TextLayer::CharOffsetPair TextLayer::GetOffsetAtIndex(const Integer Index)
        {
            return this->GetOffsetAtIndexImpl(Index);
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Text Methods

        void TextLayer::SetText(const String& Text)
        {
            if( this->DefaultCharTraits.CharFont == NULL ) {
                StringStream ExceptionStream;
                ExceptionStream << "Attempting to set text on a TextLayer without a default font set.  Layer owned by renderable \"" << Parent->GetName() << "\".";
                MEZZ_EXCEPTION(Exception::INVALID_STATE_EXCEPTION,ExceptionStream.str());
            }

            /// @todo Character generation and storage is a good candidate for pool allocation.
            this->DestroyAllCharacters();

            if( this->TextTokens != NULL ) {
                delete this->TextTokens;
                this->TextTokens = NULL;
            }

            this->TextTokens = this->MUParser->Lex(Text);
            this->Characters = this->MUParser->ParseTextTokens(this->TextTokens,this->DefaultCharTraits,this);
            this->PopulateTextLines();

            this->_MarkDirty();
        }

        String TextLayer::GetText() const
        {
            return this->TextTokens->GetRawString();
        }

        void TextLayer::SetTextScale(const Vector2& Scale)
        {
            this->CharScaling = Scale;
            this->_MarkDirty();
        }

        const Vector2& TextLayer::GetTextScale() const
        {
            return CharScaling;
        }

        void TextLayer::SetTextColour(const ColourValue& Colour)
        {
            if( this->DefaultCharTraits.CharColour != Colour )
            {
                this->DefaultCharTraits.CharColour = Colour;
                this->_MarkDirty();
            }
        }

        const ColourValue& TextLayer::GetTextColour() const
        {
            return this->DefaultCharTraits.CharColour;
        }

        void TextLayer::SetAutoScaleText(bool Enable)
        {
            /// @todo Implement This.
        }

        bool TextLayer::GetAutoScaleText() const
        {
            /// @todo Implement This.
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Font Methods

        void TextLayer::SetDefaultFont(FontData* NewFont)
        {
            if(NewFont != this->DefaultCharTraits.CharFont && NewFont)
            {
                this->DefaultCharTraits.CharFont = NewFont;
                this->_MarkDirty();
            }else{
                /// @todo Throw an error?
            }
        }

        void TextLayer::SetDefaultFont(const String& FontName)
        {
            FontData* NewData = Parent->GetScreen()->GetFont(FontName,PriAtlas);
            this->SetDefaultFont(NewData);
        }

        void TextLayer::SetDefaultFont(const String& FontName, const String& Atlas)
        {
            FontData* NewData = Parent->GetScreen()->GetFont(FontName,Atlas);
            this->SetDefaultFont(NewData);
        }

        FontData* TextLayer::GetDefaultFont()
        {
            return this->DefaultCharTraits.CharFont;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Highlight Methods

        void TextLayer::SetActiveHighlightBackgroundColour(const ColourValue& Colour)
        {
            if( this->ActiveHLColour != Colour )
            {
                this->ActiveHLColour = Colour;
                this->_MarkDirty();
            }
        }

        const ColourValue& TextLayer::GetActiveHighlightBackgroundColour() const
        {
            return this->ActiveHLColour;
        }

        void TextLayer::SetInactiveHighlightBackgroundColour(const ColourValue& Colour)
        {
            if( this->InactiveHLColour != Colour )
            {
                this->InactiveHLColour = Colour;
                this->_MarkDirty();
            }
        }

        const ColourValue& TextLayer::GetInactiveHighlightBackgroundColour() const
        {
            return this->InactiveHLColour;
        }

        void TextLayer::Highlight()
        {
            this->HighlightStart = 0;
            this->HighlightEnd = this->Characters.size() - 1;
            this->Highlight(HighlightStart,HighlightEnd);
        }

        void TextLayer::Highlight(const Integer Index)
        {
            this->ClearHighlights();
            if( Index < this->Characters.size() )
            {
                CharacterIterator ToHighlight = this->GetCharacterIteratorAtIndex(Index);
                (*ToHighlight)->SetHighlighted(true);

                this->HighlightStart = Index;
                this->HighlightEnd = Index;
            }
        }

        void TextLayer::Highlight(const Integer StartIndex, const Integer EndIndex)
        {
            this->ClearHighlights();
            CharacterIterator StartIterator = GetCharacterIteratorAtIndex(StartIndex);
            CharacterIterator EndIterator = ( EndIndex + 1 >= this->Characters.size() ? this->Characters.end() : GetCharacterIteratorAtIndex(EndIndex + 1) );

            while( StartIterator != EndIterator )
            {
                (*StartIterator)->SetHighlighted(true);
                ++StartIterator;
            }

            this->HighlightStart = StartIndex;
            this->HighlightEnd = EndIndex;
        }

        Integer TextLayer::GetHighlightStart() const
        {
            return this->HighlightStart;
        }

        Integer TextLayer::GetHighlightEnd() const
        {
            return this->HighlightEnd;
        }

        void TextLayer::ClearHighlights()
        {
            if( this->HighlightStart == -1 || this->HighlightEnd == -1 )
                return;

            CharacterIterator StartIterator = GetCharacterIteratorAtIndex(this->HighlightStart);
            CharacterIterator EndIterator = GetCharacterIteratorAtIndex(this->HighlightEnd + 1);

            while( StartIterator != EndIterator )
            {
                (*StartIterator)->SetHighlighted(false);
                ++StartIterator;
            }

            this->HighlightStart = -1;
            this->HighlightEnd = -1;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Ordering Methods

        void TextLayer::SetTextOrder(const UI::TextOrdering Order)
        {
            if( this->HorizontalOrder == Order )
                return;

            this->DestroyAllTextLines();
            this->CreateTextLine();
            this->PopulateTextLines();

            this->HorizontalOrder = Order;
            this->_MarkDirty();
        }

        UI::TextOrdering TextLayer::GetTextOrder() const
        {
            return this->HorizontalOrder;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Cursor Methods

        void TextLayer::SetCursorEnabled(bool Enable)
        {
            if( Enable == this->GetCursorEnabled() )
                return;

            if(Enable) {
                TextLine* Line = this->TextLines.at(0);
                this->Cursor->SetOffsetPosition( Vector2(Line->GetCursorStartPosition(),Line->GetPositionOffset()) );
            }else{
                delete this->Cursor;
                this->Cursor = NULL;
            }

            this->_MarkDirty();
        }

        bool TextLayer::GetCursorEnabled() const
        {
            return ( this->Cursor != NULL );
        }

        TextCursor* TextLayer::GetCursor() const
        {
            return this->Cursor;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // TextLine Methods

        TextLine* TextLayer::CreateTextLine()
        {
            TextLine* NewLine = NULL;
            if( this->HorizontalOrder == UI::TO_Left_To_Right ) NewLine = new LeftToRightTextLine(this);
            else NewLine = new RightToLeftTextLine(this);

            this->TextLines.push_back(NewLine);
            return NewLine;
        }

        TextLine* TextLayer::GetTextLineAtOffset(const Real& Offset)
        {
            TextLineIterator LineIt = TextLines.begin();
            // Check if we're too far to the top to get anything
            if( Offset < (*LineIt)->GetPositionOffset() - (*LineIt)->GetLineHeight() )
                return NULL;

            while( LineIt != this->TextLines.end() && Offset < (*LineIt)->GetPositionOffset() )
            {
                ++LineIt;
            }

            // Check if we're too low to get anything
            if( LineIt == (--this->TextLines.end()) && Offset > (*LineIt)->GetPositionOffset() )
                return NULL;

            return (*LineIt);
        }

        UInt32 TextLayer::GetNumTextLines() const
        {
            return TextLines.size();
        }

        void TextLayer::PopulateTextLines()
        {
            this->ClearAllTextLines();
            this->PopulateTextLinesImpl();
            this->RecalculateOffsets();
            this->_MarkDirty();
        }

        void TextLayer::ClearAllTextLines()
        {
            for( TextLineIterator TLIt = TextLines.begin() ; TLIt != TextLines.end() ; ++TLIt )
            {
                (*TLIt)->RemoveAllCharacters();
            }
            this->_MarkDirty();
        }

        void TextLayer::DestroyAllTextLines()
        {
            for( TextLineIterator TLIt = TextLines.begin() ; TLIt != TextLines.end() ; ++TLIt )
            {
                delete (*TLIt);
            }
            TextLines.clear();
            this->_MarkDirty();
        }

        void TextLayer::SetTextlineVerticalAlignment(const UI::LinearAlignment Align)
        {
            if( this->VerticalAlign == Align )
                return;

            this->VerticalAlign = Align;
            this->RecalculateOffsets();
            this->_MarkDirty();
        }

        UI::LinearAlignment TextLayer::GetTextlineVerticalAlignment() const
        {
            return this->VerticalAlign;
        }

        TextLayer::TextLineIterator TextLayer::BeginTextLine()
        {
            return this->TextLines.begin();
        }

        TextLayer::TextLineIterator TextLayer::EndTextLine()
        {
            return this->TextLines.end();
        }

        TextLayer::ConstTextLineIterator TextLayer::BeginTextLine() const
        {
            return this->TextLines.begin();
        }

        TextLayer::ConstTextLineIterator TextLayer::EndTextLine() const
        {
            return this->TextLines.end();
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Character Methods

        Character* TextLayer::GetCharacterAtIndex(const Integer Index) const
        {
            return *(this->GetCharacterIteratorAtIndex(Index));
        }

        Character* TextLayer::GetCharacterAtOffset(const Vector2& Offset) const
        {
            // Get the parent rect and apply the scaling
            Rect ActDims = this->GetAreaRect();
            if( !ActDims.IsInside(ActDims.Position + Offset) || this->TextLines.empty() )
                return NULL;

            // Get our line
            ConstTextLineIterator LineIt = this->TextLines.begin();
            while( LineIt != this->TextLines.end() && Offset.Y > (*LineIt)->GetPositionOffset() )
            {
                /// @todo The way this logic works, in the event of a slight bit of clipping at the bottom of the quad space that
                /// isn't enough for the next line, this logic will assume that space is a part of the last line.  Is that a problem?
                ++LineIt;
            }

            // Quickly check to see if we're to the side of any lines.
            if( Offset.X < (*LineIt)->GetLeftMostCursorPosition() ||
                Offset.X > (*LineIt)->GetRightMostCursorPosition() )
            {
                return NULL;
            }

            // Get the character in that line
            CharacterIterator CharIt = (*LineIt)->BeginCharacter();
            while( CharIt != (*LineIt)->EndCharacter() && Offset.X > (*CharIt)->GetLengthOffset() + (*CharIt)->GetCharacterSize().X )
            {
                /// @todo In the event that letter spacing for the font is big ( >0 ), and the point provided rests on a gap in the spacing,
                /// this algorithm will grab the character to the right as the hovered character.  Is that a problem?
                ++CharIt;
            }

            if( CharIt == (*LineIt)->EndCharacter() ) return NULL;
            else return (*CharIt);
        }

        TextLayer::CharacterIterator TextLayer::GetCharacterIteratorAtIndex(const Integer Index)
        {
            if( 0 > Index || Index >= this->Characters.size() )
            {
                return --(this->Characters.end());
            }else{
                UInt32 Count = Index;
                CharacterIterator CharIt = this->Characters.begin();
                while( 0 != Count )
                {
                    ++CharIt;
                    --Count;
                }
                return CharIt;
            }
        }

        TextLayer::ConstCharacterIterator TextLayer::GetCharacterIteratorAtIndex(const Integer Index) const
        {
            if( 0 > Index || Index >= this->Characters.size() )
            {
                return --(this->Characters.end());
            }else{
                UInt32 Count = Index;
                ConstCharacterIterator CharIt = this->Characters.begin();
                while( 0 != Count )
                {
                    ++CharIt;
                    --Count;
                }
                return CharIt;
            }
        }

        Whole TextLayer::GetNumCharacters() const
        {
            return this->Characters.size();
        }

        void TextLayer::InsertCharacterAtIndex(const Integer Index, const UInt32 GlyphID)
        {
            // Do our work
            this->TextTokens->InsertCharacter(Index,GlyphID);
            // Regenerate the text
            String RawText = this->GetText();
            this->SetText(RawText);
        }

        void TextLayer::InsertCharactersAtIndex(const Integer Index, const Char8* Characters, const UInt32 BufSize)
        {
            // Do our work
            this->TextTokens->InsertCharacters(Index,Characters,BufSize);
            // Regenerate the text
            String RawText = this->GetText();
            this->SetText(RawText);
        }

        void TextLayer::InsertCharactersAtIndex(const Integer Index, const UInt32* Characters, const UInt32 BufSize)
        {
            // Do our work
            this->TextTokens->InsertCharacters(Index,Characters,BufSize);
            // Regenerate the text
            String RawText = this->GetText();
            this->SetText(RawText);
        }

        void TextLayer::RemoveCharacterAtIndex(const Integer Index)
        {
            // Do our work
            this->TextTokens->RemoveCharacter(Index);
            // Regenerate the text
            String RawText = this->GetText();
            this->SetText(RawText);
        }

        void TextLayer::RemoveCharactersAtIndex(const Integer Index, const UInt32 Length)
        {
            // Do our work
            this->TextTokens->RemoveCharacters(Index,Length);
            // Regenerate the text
            String RawText = this->GetText();
            this->SetText(RawText);
        }

        void TextLayer::RemoveCharacterRange(const Integer First, const Integer Last)
        {
            this->RemoveCharactersAtIndex(First,(Last - First) + 1);
        }

        void TextLayer::DestroyAllCharacters()
        {
            // Remove from the TextLines first
            this->ClearAllTextLines();
            // Now clean up the characters
            for( CharacterIterator CharIt = this->Characters.begin() ; CharIt != this->Characters.end() ; ++CharIt )
            {
                delete (*CharIt);
            }
            this->Characters.clear();

            this->HighlightStart = -1;
            this->HighlightEnd = -1;

            this->_MarkDirty();
        }

        TextLayer::CharacterIterator TextLayer::BeginCharacter()
        {
            return this->Characters.begin();
        }

        TextLayer::CharacterIterator TextLayer::EndCharacter()
        {
            return this->Characters.end();
        }

        TextLayer::ConstCharacterIterator TextLayer::BeginCharacter() const
        {
            return this->Characters.begin();
        }

        TextLayer::ConstCharacterIterator TextLayer::EndCharacter() const
        {
            return this->Characters.end();
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Serialization

        void TextLayer::ProtoSerialize(XML::Node& ParentNode) const
        {
            XML::Node SelfRoot = ParentNode.AppendChild( this->GetDerivedSerializableName() );

            this->ProtoSerializeProperties(SelfRoot);
            this->ProtoSerializeCursor(SelfRoot);
            this->ProtoSerializeText(SelfRoot);
        }

        void TextLayer::ProtoSerializeProperties(XML::Node& SelfRoot) const
        {
            RenderLayer::ProtoSerializeProperties(SelfRoot);
            XML::Node PropertiesNode = SelfRoot.AppendChild( TextLayer::GetSerializableName() + "Properties" );

            if( PropertiesNode.AppendAttribute("Version").SetValue("1") &&
                PropertiesNode.AppendAttribute("HighlightStart").SetValue(this->HighlightStart) &&
                PropertiesNode.AppendAttribute("HighlightEnd").SetValue(this->HighlightEnd) &&
                PropertiesNode.AppendAttribute("RelLineHeight").SetValue(this->RelLineHeight) &&
                PropertiesNode.AppendAttribute("HorizontalOrder").SetValue(this->HorizontalOrder) &&
                PropertiesNode.AppendAttribute("VerticalAlign").SetValue(this->VerticalAlign) &&
                PropertiesNode.AppendAttribute("ScalingChanged").SetValue(this->ScalingChanged) &&
                PropertiesNode.AppendAttribute("ParserName").SetValue( ( this->MUParser ? this->MUParser->GetName() : "" ) ) &&
                PropertiesNode.AppendAttribute("RawText").SetValue(this->GetText()) )
            {
                XML::Node DefaultCharTraitsNode = PropertiesNode.AppendChild("DefaultCharTraits");
                this->DefaultCharTraits.ProtoSerialize( DefaultCharTraitsNode );
                XML::Node ActiveHLColourNode = PropertiesNode.AppendChild("ActiveHLColour");
                this->ActiveHLColour.ProtoSerialize( ActiveHLColourNode );
                XML::Node InactiveHLColourNode = PropertiesNode.AppendChild("InactiveHLColour");
                this->InactiveHLColour.ProtoSerialize( InactiveHLColourNode );
                XML::Node CharScalingNode = PropertiesNode.AppendChild("CharScaling");
                this->CharScaling.ProtoSerialize( CharScalingNode );

                return;
            }else{
                SerializeError("Create XML Attribute Values",TextLayer::GetSerializableName() + "Properties",true);
            }
        }

        void TextLayer::ProtoSerializeCursor(XML::Node& SelfRoot) const
        {
            if( this->Cursor ) {
                XML::Node CursorNode = SelfRoot.AppendChild("Cursor");
                this->Cursor->ProtoSerialize( CursorNode );
            }
        }

        void TextLayer::ProtoSerializeText(XML::Node& SelfRoot) const
        {
            XML::Node TextNode = SelfRoot.AppendChild( TextLayer::GetSerializableName() + "Text" );

            if( TextNode.AppendAttribute("Version").SetValue(1) &&
                TextNode.AppendAttribute("Text").SetValue( this->GetText() ) ) {
                return;
            }else{
                SerializeError("Create XML Attribute Values",TextLayer::GetSerializableName() + "Text",true);
            }
        }

        void TextLayer::ProtoDeSerialize(const XML::Node& SelfRoot)
        {
            this->ProtoDeSerializeProperties(SelfRoot);
            this->ProtoDeSerializeCursor(SelfRoot);
            this->ProtoDeSerializeText(SelfRoot);

            // Set Highlights
            if( this->HighlightStart > -1 && this->HighlightEnd > -1 ) {
                Integer StartCopy = this->HighlightStart;
                Integer EndCopy = this->HighlightEnd;
                this->Highlight(StartCopy,EndCopy);
            }
        }

        void TextLayer::ProtoDeSerializeProperties(const XML::Node& SelfRoot)
        {
            this->ClearAllTextLines();
            this->DestroyAllCharacters();
            this->DestroyAllTextLines();

            RenderLayer::ProtoDeSerializeProperties(SelfRoot);
            XML::Attribute CurrAttrib;
            XML::Node PropertiesNode = SelfRoot.GetChild( TextLayer::GetSerializableName() + "Properties" );

            if( !PropertiesNode.Empty() ) {
                if( PropertiesNode.GetAttribute("Version").AsInt() == 1 ) {
                    CurrAttrib = PropertiesNode.GetAttribute("HighlightStart");
                    if( !CurrAttrib.Empty() )
                        this->HighlightStart = CurrAttrib.AsInt();

                    CurrAttrib = PropertiesNode.GetAttribute("HighlightEnd");
                    if( !CurrAttrib.Empty() )
                        this->HighlightEnd = CurrAttrib.AsInt();

                    CurrAttrib = PropertiesNode.GetAttribute("RelLineHeight");
                    if( !CurrAttrib.Empty() )
                        this->RelLineHeight = CurrAttrib.AsReal();

                    CurrAttrib = PropertiesNode.GetAttribute("HorizontalOrder");
                    if( !CurrAttrib.Empty() )
                        this->HorizontalOrder = static_cast<UI::TextOrdering>( CurrAttrib.AsWhole() );

                    CurrAttrib = PropertiesNode.GetAttribute("VerticalAlign");
                    if( !CurrAttrib.Empty() )
                        this->VerticalAlign = static_cast<UI::LinearAlignment>( CurrAttrib.AsWhole() );

                    CurrAttrib = PropertiesNode.GetAttribute("ScalingChanged");
                    if( !CurrAttrib.Empty() )
                        this->ScalingChanged = StringTools::ConvertToBool( CurrAttrib.AsString() );

                    XML::Node DefaultCharTraitsNode = PropertiesNode.GetChild("DefaultCharTraits").GetFirstChild();
                    if( !DefaultCharTraitsNode.Empty() )
                        this->DefaultCharTraits.ProtoDeSerialize(DefaultCharTraitsNode);

                    XML::Node ActiveHLColourNode = PropertiesNode.GetChild("ActiveHLColour").GetFirstChild();
                    if( !ActiveHLColourNode.Empty() )
                        this->ActiveHLColour.ProtoDeSerialize(ActiveHLColourNode);

                    XML::Node InactiveHLColourNode = PropertiesNode.GetChild("InactiveHLColour").GetFirstChild();
                    if( !InactiveHLColourNode.Empty() )
                        this->InactiveHLColour.ProtoDeSerialize(InactiveHLColourNode);

                    XML::Node CharScalingNode = PropertiesNode.GetChild("CharScaling").GetFirstChild();
                    if( !CharScalingNode.Empty() )
                        this->CharScaling.ProtoDeSerialize(CharScalingNode);
                }else{
                    MEZZ_EXCEPTION(Exception::INVALID_VERSION_EXCEPTION,"Incompatible XML Version for " + (TextLayer::GetSerializableName() + "Properties") + ": Not Version 1.");
                }
            }else{
                MEZZ_EXCEPTION(Exception::II_IDENTITY_NOT_FOUND_EXCEPTION,TextLayer::GetSerializableName() + "Properties" + " was not found in the provided XML node, which was expected.");
            }
        }

        void TextLayer::ProtoDeSerializeCursor(const XML::Node& SelfRoot)
        {
            XML::Node CursorNode = SelfRoot.GetChild("Cursor");
            if( !CursorNode.Empty() ) {
                if( this->GetCursorEnabled() == false ) {
                    this->SetCursorEnabled(true);
                }
                this->Cursor->ProtoDeSerialize( CursorNode.GetFirstChild() );
            }
        }

        void TextLayer::ProtoDeSerializeText(const XML::Node& SelfRoot)
        {
            XML::Attribute CurrAttrib;
            XML::Node TextNode = SelfRoot.GetChild( TextLayer::GetSerializableName() + "Text" );

            if( !TextNode.Empty() ) {
                if( TextNode.GetAttribute("Version").AsInt() == 1 ) {
                    CurrAttrib = TextNode.GetAttribute("Text");
                    if( !CurrAttrib.Empty() )
                        this->SetText( CurrAttrib.AsString() );
                }else{
                    MEZZ_EXCEPTION(Exception::INVALID_VERSION_EXCEPTION,"Incompatible XML Version for " + (TextLayer::GetSerializableName() + "Text") + ": Not Version 1.");
                }
            }else{
                MEZZ_EXCEPTION(Exception::II_IDENTITY_NOT_FOUND_EXCEPTION,TextLayer::GetSerializableName() + "Text" + " was not found in the provided XML node, which was expected.");
            }
        }

        String TextLayer::GetDerivedSerializableName() const
        {
            return TextLayer::GetSerializableName();
        }

        String TextLayer::GetSerializableName()
        {
            return "TextLayer";
        }
    }//UI
}//Mezzanine

#endif
