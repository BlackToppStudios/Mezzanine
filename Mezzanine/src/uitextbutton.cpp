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
#ifndef _uitextbutton_cpp
#define _uitextbutton_cpp

#include "uitextbutton.h"
#include "uilayer.h"
#include "uiscreen.h"
#include "uimanager.h"
#include "mathtool.h"

namespace Mezzanine
{
    namespace UI
    {
        TextButton::TextButton(ConstString& name, const RenderableRect& Rect, const Whole& Glyph, const String& Text, Layer* PLayer)
            : Button(name,Rect,PLayer),
              HoriAlign(UI::Txt_Middle),
              VertAlign(UI::Txt_Center),
              CharScaling(1.0),
              ClippedLeftIndex(0.0),
              ClippedRightIndex(0.0)
        {
            AutoScaleText = false;
            RelLineHeight = 0.0;
            GlyphSet = ParentLayer->GetGlyphData(Glyph,PriAtlas);
            this->Text = Text;
            TextColour = ColourValue::White();
            GlyphAtlas = PriAtlas;
        }

        TextButton::TextButton(ConstString& name, const RenderableRect& Rect, const Real& LineHeight, const String& Text, Layer* PLayer)
            : Button(name,Rect,PLayer),
              HoriAlign(UI::Txt_Middle),
              VertAlign(UI::Txt_Center),
              CharScaling(1.0),
              ClippedLeftIndex(0.0),
              ClippedRightIndex(0.0)
        {
            AutoScaleText = true;
            RelLineHeight = LineHeight;
            std::pair<Whole,Real> Result = Manager->SuggestGlyphIndex((Whole)(LineHeight * ParentLayer->GetParent()->GetViewportDimensions().Y),PriAtlas);
            GlyphSet = ParentLayer->GetGlyphData(Result.first,PriAtlas);
            SetTextScale(Result.second);
            this->Text = Text;
            TextColour = ColourValue::White();
            GlyphAtlas = PriAtlas;
        }

        TextButton::~TextButton()
        {
        }

        void TextButton::CalculateDrawSize(Vector2& Size)
        {
            Real Cursor = 0;
            Real Kerning = 0;

            unsigned char CurrChar = 0, LastChar = 0;
            Glyph* CurrGlyph = 0;
            Size.X = 0;
            Size.Y = GlyphSet->LineHeight;

            for( Whole Index = 0 ; Index < Text.length() ; Index++ )
            {
                CurrChar = Text[Index];

                if(CurrChar == ' ')
                {
                    LastChar = CurrChar;
                    Cursor += GlyphSet->SpaceLength;
                    continue;
                }

                if( CurrChar < GlyphSet->RangeBegin || CurrChar > GlyphSet->RangeEnd )
                {
                    LastChar = 0;
                    continue;
                }

                CurrGlyph = GlyphSet->GetGlyph(CurrChar);
                if(CurrGlyph == 0)
                    continue;
                Kerning = CurrGlyph->GetKerning(LastChar);
                if(Kerning == 0)
                    Kerning = GlyphSet->LetterSpacing;

                if(CharScaling == 1.0f)
                    Cursor += CurrGlyph->GlyphAdvance + Kerning;
                else
                    Cursor += (CurrGlyph->GlyphAdvance + Kerning) * CharScaling;
                LastChar = CurrChar;
            }
            Size.X = Cursor - Kerning;
        }

        bool TextButton::IsTextButton()
        {
            return true;
        }

        void TextButton::SetText(ConstString& Text)
        {
            this->Text = Text;
            _MarkDirty();
        }

        String TextButton::GetText()
        {
            return Text;
        }

        void TextButton::SetTextScale(const Real& Scale)
        {
            CharScaling = Scale;
            _MarkDirty();
        }

        Real TextButton::GetTextScale()
        {
            return CharScaling;
        }

        void TextButton::SetTextColour(const ColourValue& Colour)
        {
            TextColour = Colour;
            _MarkDirty();
        }

        ColourValue TextButton::GetTextColour()
        {
            return TextColour;
        }

        void TextButton::SetGlyphIndex(const Whole& GlyphIndex)
        {
            GlyphData* NewData = ParentLayer->GetGlyphData(GlyphIndex,PriAtlas);
            if(NewData)
            {
                GlyphSet = NewData;
                _MarkDirty();
            }else{
                /// @todo Throw an error?
            }
        }

        void TextButton::SetGlyphIndex(const Whole& GlyphIndex, const String& Atlas)
        {
            GlyphData* NewData = ParentLayer->GetGlyphData(GlyphIndex,Atlas);
            if(NewData)
            {
                GlyphSet = NewData;
                GlyphAtlas = Atlas;
                _MarkDirty();
            }else{
                /// @todo Throw an error?
            }
        }

        GlyphData* TextButton::GetGlyphData()
        {
            return GlyphSet;
        }

        void TextButton::SetCursorOffset(const Whole& Offset)
        {
            CursorOffset = Offset;
            _MarkDirty();
        }

        Whole TextButton::GetCursorOffset()
        {
            return CursorOffset;
        }

        void TextButton::HorizontallyAlign(const UI::TextHorizontalAlign& Align)
        {
            HoriAlign = Align;
            _MarkDirty();
        }

        void TextButton::VerticallyAlign(const UI::TextVerticalAlign& Align)
        {
            VertAlign = Align;
            _MarkDirty();
        }

        void TextButton::SetAutoScaleText(bool Enable)
        {
            AutoScaleText = Enable;
        }

        bool TextButton::GetAutoScaleText()
        {
            return AutoScaleText;
        }

        void TextButton::UpdateDimensions()
        {
            Button::UpdateDimensions();
            if(AutoScaleText)
            {
                std::pair<Whole,Real> Result = Manager->SuggestGlyphIndex(RelLineHeight * ParentLayer->GetParent()->GetViewportDimensions().Y,GetPrimaryAtlas());
                SetGlyphIndex(Result.first);
                SetTextScale(Result.second);
            }
        }

        void TextButton::_Redraw()
        {
            if(Dirty == false)
                return;
            Rectangle::_Redraw();
            if(!Visible)
            {
                Dirty = false;
                return;
            }

            Real Left = 0, Top = 0, Right = 0, Bottom = 0, CursorX = 0, CursorY = 0, Kerning = 0;
            Real TexelOffsetX = ParentLayer->GetTexelX();
            Real TexelOffsetY = ParentLayer->GetTexelY();
            Vector2 KnownSize;
            Glyph* CurrGlyph = 0;

            bool ClipLeft = false, ClipRight = false;
            Real ClipLeftPos = 0, ClipRightPos = 0;

            if(HoriAlign == UI::Txt_Left)
            {
                CursorX = ActPosition.X + CursorOffset;

                ClipRight = true;
                ClipRightPos = ActPosition.X + ActSize.X;
            }
            else if(HoriAlign == UI::Txt_Middle)
            {
                CalculateDrawSize(KnownSize);
                CursorX = ActPosition.X + (ActSize.X * 0.5f) - (KnownSize.X * 0.5f);

                ClipLeft = true;
                ClipLeftPos = ActPosition.X;
                ClipRight = true;
                ClipRightPos = ActPosition.X + ActSize.X;
            }
            else if(HoriAlign == UI::Txt_Right)
            {
                CalculateDrawSize(KnownSize);
                CursorX = (ActPosition.X + ActSize.X) - (KnownSize.X + CursorOffset);

                ClipLeft = true;
                ClipLeftPos = ActPosition.X;
            }

            if(VertAlign == UI::Txt_Top)
                CursorY = ActPosition.Y;
            else if(VertAlign == UI::Txt_Center)
                CursorY = ActPosition.Y + (ActSize.Y * 0.5) - ((GlyphSet->LineHeight * CharScaling) * 0.5);
            else if(VertAlign == UI::Txt_Bottom)
                CursorY = ActPosition.Y + ActSize.Y - (GlyphSet->LineHeight * CharScaling);

            unsigned char ThisChar = 0, LastChar = 0;
            ClippedLeftIndex = String::npos;
            ClippedRightIndex = String::npos;

            CursorX = MathTool::Floor( CursorX );
            CursorY = MathTool::Floor( CursorY );

            for( Whole Index = 0 ; Index < Text.size() ; Index++ )
            {
                ThisChar = Text[Index];

                if( ThisChar == ' ' )
                {
                    LastChar = ThisChar;
                    CursorX += GlyphSet->SpaceLength * CharScaling;
                    continue;
                }

                if( ThisChar < GlyphSet->RangeBegin || ThisChar > GlyphSet->RangeEnd )
                {
                    LastChar = 0;
                    continue;
                }

                CurrGlyph = GlyphSet->GetGlyph(ThisChar);
                if(CurrGlyph == 0)
                    continue;
                Kerning = CurrGlyph->GetKerning(LastChar);
                if(Kerning == 0)
                    Kerning = GlyphSet->LetterSpacing;

                Left = CursorX - TexelOffsetX;
                Top = CursorY - TexelOffsetY + CurrGlyph->VerticalOffset;
                Right = Left + (CurrGlyph->UVWidth * CharScaling) + TexelOffsetX;
                Bottom = Top + (CurrGlyph->UVHeight * CharScaling) + TexelOffsetY;

                if(ClipLeft)
                {
                    if (Left < ClipLeftPos)
                    {
                        if(ClippedLeftIndex == String::npos)
                            ClippedLeftIndex = Index;
                        CursorX += (CurrGlyph->GlyphAdvance + Kerning) * CharScaling;
                        LastChar = ThisChar;
                        continue;
                    }
                }

                if(ClipRight)
                {
                    if(Right > ClipRightPos)
                    {
                        if(ClippedRightIndex == String::npos)
                            ClippedRightIndex = Index;
                        CursorX += (CurrGlyph->GlyphAdvance + Kerning) * CharScaling;
                        LastChar = ThisChar;
                        continue;
                    }
                }

                VertexData Temp;

                // Triangle A
                PushVertex(RenderVertices, Temp, Left, Bottom, CurrGlyph->AtlasCoords[UI::QC_BottomLeft], TextColour,GlyphAtlas);  // Left/Bottom  3
                PushVertex(RenderVertices, Temp, Right, Top, CurrGlyph->AtlasCoords[UI::QC_TopRight], TextColour,GlyphAtlas);    // Right/Top    1
                PushVertex(RenderVertices, Temp, Left, Top, CurrGlyph->AtlasCoords[UI::QC_TopLeft], TextColour,GlyphAtlas);     // Left/Top     0

                // Triangle B
                PushVertex(RenderVertices, Temp, Left, Bottom, CurrGlyph->AtlasCoords[UI::QC_BottomLeft], TextColour,GlyphAtlas);  // Left/Bottom  3
                PushVertex(RenderVertices, Temp, Right, Bottom, CurrGlyph->AtlasCoords[UI::QC_BottomRight], TextColour,GlyphAtlas); // Right/Bottom 2
                PushVertex(RenderVertices, Temp, Right, Top, CurrGlyph->AtlasCoords[UI::QC_TopRight], TextColour,GlyphAtlas);    // Right/Top    1

                CursorX += (CurrGlyph->GlyphAdvance + Kerning) * CharScaling;
                LastChar = ThisChar;
            }
            Dirty = false;
        }
    }
}

#endif
