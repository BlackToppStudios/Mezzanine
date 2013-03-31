//© Copyright 2010 - 2013 BlackTopp Studios Inc.
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
#ifndef _uimarkuptext_cpp
#define _uimarkuptext_cpp

#include "UI/markuptext.h"
#include "UI/screen.h"
#include "uimanager.h"
#include "UI/viewportupdatetool.h"
#include "entresol.h"
#include "stringtool.h"

namespace Mezzanine
{
    namespace UI
    {
        MarkupText::MarkupText(ConstString& name, const RenderableRect& Rect, const Whole& Glyph, const String& Text, Screen* PScreen)
            : Rectangle(name,Rect,PScreen),
              TextDirty(true),
              CharScaling(0.0)
        {
            AutoScaleText = false;
            RelLineHeight = 0.0;
            DefaultGlyphSet = ParentScreen->GetGlyphData(Glyph,PriAtlas);
            this->Text = Text;
            GlyphAtlas = PriAtlas;
        }

        MarkupText::MarkupText(ConstString& name, const RenderableRect& Rect, const Real& LineHeight, const String& Text, Screen* PScreen)
            : Rectangle(name,Rect,PScreen),
              CharScaling(0.0)
        {
            AutoScaleText = true;
            RelLineHeight = LineHeight;
            std::pair<Whole,Real> Result = Manager->SuggestGlyphIndex(LineHeight * ParentScreen->GetViewportDimensions().Y,ParentScreen->GetPrimaryAtlas());
            DefaultGlyphSet = ParentScreen->GetGlyphData(Result.first,PriAtlas);
            SetTextScale(Result.second);
            this->Text = Text;
            GlyphAtlas = PriAtlas;
        }

        MarkupText::~MarkupText()
        {
        }

        void MarkupText::SetText(const String& Text)
        {
            this->Text = Text;
            TextDirty = true;
            _MarkDirty();
        }

        String MarkupText::GetText()
        {
            return Text;
        }

        void MarkupText::SetTextScale(const Real& Scale)
        {
            CharScaling = Scale;
            TextDirty = true;
            _MarkDirty();
        }

        Real MarkupText::GetTextScale()
        {
            return CharScaling;
        }

        void MarkupText::SetDefaultGlyphIndex(const Whole& DefaultGlyphIndex)
        {
            GlyphData* NewData = ParentScreen->GetGlyphData(DefaultGlyphIndex,PriAtlas);
            if(NewData)
            {
                DefaultGlyphSet = NewData;
                TextDirty = true;
                _MarkDirty();
            }else{
                /// @todo Throw an error?
            }
        }

        void MarkupText::SetDefaultGlyphIndex(const Whole& DefaultGlyphIndex, const String& Atlas)
        {
            GlyphData* NewData = ParentScreen->GetGlyphData(DefaultGlyphIndex,Atlas);
            if(NewData)
            {
                DefaultGlyphSet = NewData;
                GlyphAtlas = Atlas;
                TextDirty = true;
                _MarkDirty();
            }else{
                /// @todo Throw an error?
            }
        }

        GlyphData* MarkupText::GetDefaultGlyphData()
        {
            return DefaultGlyphSet;
        }

        void MarkupText::SetAutoScaleText(bool Enable)
        {
            AutoScaleText = Enable;
        }

        bool MarkupText::GetAutoScaleText()
        {
            return AutoScaleText;
        }

        void MarkupText::UpdateDimensions()
        {
            ViewportUpdateTool::UpdateRectangleRenderable(this);
            TextDirty = true;
            //this->SetActualPosition(RelPosition * ParentScreen->GetViewportDimensions());
            //this->SetActualSize(RelSize * ParentScreen->GetViewportDimensions());
            if(AutoScaleText)
            {
                std::pair<Whole,Real> Result = Manager->SuggestGlyphIndex(RelLineHeight * ParentScreen->GetViewportDimensions().Y,GetPrimaryAtlas());
                SetDefaultGlyphIndex(Result.first);
                SetTextScale(Result.second);
            }
        }

        void MarkupText::_Redraw()
        {
            if(Dirty == false)
                return;
            RenderVertices.clear();
            if(!Visible)
            {
                Dirty = false;
                return;
            }

            if(TextDirty)
                _CalculateCharacters();

            VertexData Temp;
            for ( Whole Index = 0 ; Index < Characters.size() ; Index++ )
            {
                PushQuad2(RenderVertices, Temp, Characters[Index]->Positions, Characters[Index]->UVs, Characters[Index]->Colour, Characters[Index]->Atlas);
            }
            Dirty = false;
        }

        void MarkupText::_CalculateCharacters()
        {
            if(TextDirty == false)
                return;
            Characters.clear();

            _GenerateWords();
            _PlaceWords();

            TextDirty = false;
        }

        void MarkupText::_GenerateWords()
        {
            _ClearWords();
            Word* CurrWord = NULL;
            bool MarkupMode = false;
            bool FixedWidth = false;
            Glyph* CurrGlyph = NULL;
            GlyphData* CurrGlyphData = DefaultGlyphSet;
            ColourValue Colour = ParentScreen->GetMarkupColour(0,PriAtlas);
            Real LineHeight = CurrGlyphData->LineHeight;
            Real Kerning = 0;
            unsigned int ThisChar = 0, LastChar = 0;

            for( Whole Index = 0 ; Index < Text.length() ; Index++ )
            {
                ThisChar = Text[Index];

                if( ThisChar == ' ' )
                {
                    LastChar = ThisChar;
                    if(CurrWord)
                    {
                        CurrWord->GlyphDataSpacing = CurrGlyphData->SpaceLength;
                        Words.push_back(CurrWord);
                        CurrWord = NULL;
                    }
                    continue;
                }

                if( ThisChar == '\n' )
                {
                    LastChar = ThisChar;
                    if(CurrWord)
                    {
                        //CurrWord->NewlineAfter = true;
                        Words.push_back(CurrWord);
                        CurrWord = NULL;
                    }
                    CurrWord = new Word();
                    CurrWord->NewlineBefore = true;
                    continue;
                }

                if( ThisChar < CurrGlyphData->RangeBegin || ThisChar > CurrGlyphData->RangeEnd )
                {
                    LastChar = 0;
                    continue;
                }

                if( ThisChar == '%' && !MarkupMode )
                {
                    MarkupMode = true;
                    continue;
                }

                if( MarkupMode == true )
                {
                    if (ThisChar == '%')
                    {
                        // Escape Character.
                    }else{
                        MarkupMode = false;

                        // Check if the user wants a different colour to use for all further glyphs.
                        if(ThisChar >= '0' && ThisChar <= '9')
                        {
                            Colour = ParentScreen->GetMarkupColour(ThisChar - 48,PriAtlas);
                        }
                        // Check if the user wants to revert the current selected colour back to default.
                        else if(ThisChar == 'R' || ThisChar == 'r')
                        {
                            Colour = ParentScreen->GetMarkupColour(0,PriAtlas);
                        }
                        // Fixed Width?  Like Notepad?
                        else if(ThisChar == 'M' || ThisChar == 'm')
                        {
                            FixedWidth = !FixedWidth;
                        }
                        // Check if the user wants to change the glyph set being rendered.
                        else if( ThisChar == '@' )
                        {
                            MarkupMode = false;
                            bool FoundIt = false;
                            Whole Begin = Index;
                            while( Index < Text.size() )
                            {
                                if( Text[Index] == '%' )
                                {
                                    FoundIt = true;
                                    break;
                                }
                                Index++;
                            }

                            if( FoundIt == false )
                                return;

                            UInt32 ID = StringTools::ConvertToUInt32(Text.substr(Begin+1, Index - Begin - 1));
                            CurrGlyphData = ParentScreen->GetGlyphData(ID,PriAtlas);
                            if( CurrGlyphData == 0 )
                                return;
                            continue;
                        }
                        // Check to see if the user wants to change the glyph with a sprite.
                        else if( ThisChar == ':' )
                        {
                            MarkupMode = false;
                            bool FoundIt = false;
                            size_t Begin = Index;
                            size_t Middle = 0;
                            while( Index < Text.size() )
                            {
                                if( Text[Index] == ',' )
                                {
                                    Middle = Index;
                                }
                                if( Text[Index] == '%' )
                                {
                                    FoundIt = true;
                                    break;
                                }
                                Index++;
                            }

                            if( FoundIt == false )
                                return;

                            String SpriteName = Text.substr(Begin+1, Middle - Begin - 1);
                            String SpriteAtlas = Text.substr(Middle+1, Index - Middle - 1);
                            Sprite* NewSprite = ParentScreen->GetSprite(SpriteName,SpriteAtlas);
                            if( NewSprite == 0 )
                                continue;

                            if( !CurrWord )
                                CurrWord = new Word();

                            Character* c = new Character();
                            c->Index = Index;
                            c->Colour = Colour;
                            c->CharSprite = NewSprite;
                            c->Atlas = NewSprite->Atlas;

                            CurrWord->AddCharacter(c,CharScaling);

                            continue;
                        }
                        continue;
                    }
                    MarkupMode = false;
                }

                CurrGlyph = CurrGlyphData->GetGlyph(ThisChar);

                if( !CurrWord )
                    CurrWord = new Word();

                if( !FixedWidth )
                {
                    Kerning = CurrGlyph->GetKerning(LastChar);
                    if (Kerning == 0)
                        Kerning = CurrGlyphData->LetterSpacing;
                }

                Character* c = new Character();
                c->Index = Index;
                c->Colour = Colour;
                c->CharGlyph = CurrGlyph;
                c->Kerning = Kerning;
                c->Atlas = CurrGlyphData->Atlas;

                CurrWord->AddCharacter(c,CharScaling);

                LastChar = ThisChar;
            }

            if( CurrWord )
            {
                if(CurrWord->Characters.size()) Words.push_back(CurrWord);
                else delete CurrWord;
            }
        }

        void MarkupText::_PlaceWords()
        {
            if(Words.empty())
                return;

            Real CursorX = ActPosition.X, CursorY = 0;
            Real TexelX = ParentScreen->GetTexelOffsetX(), TexelY = ParentScreen->GetTexelOffsetY();
            Real Kerning = 0, Top = 0, Left = 0, Bottom = 0, Right = 0;

            std::vector<Real> Lineheights;
            Real CurrLineHeight = 0;
            for( UInt32 words = 0 ; words < Words.size() ; ++words )
            {
                Word* CurrWord = Words.at(words);
                if(CursorX+CurrWord->Pxlength > ActPosition.X+ActSize.X)
                {
                    CurrWord->NewlineBefore = true;
                }
                if(CurrWord->NewlineBefore)
                {
                    Lineheights.push_back(CurrLineHeight);
                    CurrLineHeight = 0;
                    CursorX = ActPosition.X;
                }else{
                    CursorX += (CurrWord->Pxlength + CurrWord->GlyphDataSpacing);
                }
                if(CurrWord->Pxheight > CurrLineHeight)
                    CurrLineHeight = CurrWord->Pxheight;
            }
            Lineheights.push_back(CurrLineHeight);

            CursorX = ActPosition.X;
            CursorY = ActPosition.Y + Lineheights[0];
            for( UInt32 words = 0, lineindex = 0 ; words < Words.size() ; ++words )
            {
                Word* CurrWord = Words[words];
                UInt32 NumChars = CurrWord->Characters.size();

                if(CurrWord->NewlineBefore)
                {
                    lineindex++;
                    CursorX = ActPosition.X;
                    CursorY += Lineheights[lineindex];
                }// */

                for( UInt32 chars = 0 ; chars < NumChars ; ++chars )
                {
                    Character* CurrChar = CurrWord->Characters[chars];

                    if(CurrChar->CharGlyph)
                    {
                        Left = CursorX;
                        Top = CursorY - (CurrChar->CharGlyph->GlyphSize.Y * CharScaling) - TexelY;
                        Right = CursorX + (CurrChar->CharGlyph->GlyphSize.X * CharScaling) + TexelX;
                        Bottom = CursorY - (CurrChar->CharGlyph->VerticalOffset * CharScaling);
                    }
                    else if(CurrChar->CharSprite)
                    {
                        Left = CursorX - TexelX;
                        Top = CursorY - (CurrChar->CharSprite->SpriteSize.Y * CharScaling) - TexelY;
                        Right = CursorX + (CurrChar->CharSprite->SpriteSize.X * CharScaling) + TexelX;
                        Bottom = CursorY + TexelY;
                    }
                    else
                    {
                        // If this happens, just skip, cause I don't know wtf is going on.
                        continue;
                    }

                    CurrChar->Positions[UI::QC_TopLeft].X = Left;
                    CurrChar->Positions[UI::QC_TopLeft].Y = Top;
                    CurrChar->Positions[UI::QC_TopRight].X = Right;
                    CurrChar->Positions[UI::QC_TopRight].Y = Top;
                    CurrChar->Positions[UI::QC_BottomLeft].X = Left;
                    CurrChar->Positions[UI::QC_BottomLeft].Y = Bottom;
                    CurrChar->Positions[UI::QC_BottomRight].X = Right;
                    CurrChar->Positions[UI::QC_BottomRight].Y = Bottom;
                    CurrChar->UVs[0] = (CurrChar->CharGlyph ? CurrChar->CharGlyph->AtlasCoords[0] : CurrChar->CharSprite->AtlasCoords[0]);
                    CurrChar->UVs[1] = (CurrChar->CharGlyph ? CurrChar->CharGlyph->AtlasCoords[1] : CurrChar->CharSprite->AtlasCoords[1]);
                    CurrChar->UVs[2] = (CurrChar->CharGlyph ? CurrChar->CharGlyph->AtlasCoords[2] : CurrChar->CharSprite->AtlasCoords[2]);
                    CurrChar->UVs[3] = (CurrChar->CharGlyph ? CurrChar->CharGlyph->AtlasCoords[3] : CurrChar->CharSprite->AtlasCoords[3]);

                    if(CurrChar->CharGlyph) CursorX += ((CurrChar->CharGlyph->GlyphAdvance + CurrChar->Kerning) * CharScaling);
                    else if(CurrChar->CharSprite) CursorX += (CurrChar->CharSprite->SpriteSize.X * CharScaling);

                    Characters.push_back(CurrChar);
                }

                CursorX += (CurrWord->GlyphDataSpacing * CharScaling);

                /*if(CurrWord->mNewlineAfter)
                {
                    lineindex++;
                    CursorX = ActPosition.X;
                    CursorY += Lineheights[lineindex];
                }// */
            }
        }

        void MarkupText::_ClearWords()
        {
            for( Whole X = 0 ; X < Words.size() ; ++X )
                delete Words[X];
            Words.clear();
        }
    }//UI
}//Mezzanine

#endif
