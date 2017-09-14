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
#ifndef _uifont_cpp
#define _uifont_cpp

#include "UI/font.h"
#include "UI/glyph.h"
#include "UI/textureatlas.h"

namespace Mezzanine
{
    namespace UI
    {
        FontData::FontData(TextureAtlas* ParentAtlas) :
            Atlas(ParentAtlas),
            SpaceLength(0),
            LineHeight(0),
            BaseLine(0),
            LetterSpacing(0)
            {  }

        FontData::~FontData()
        {
            for( GlyphIterator GlyphIt = this->Glyphs.begin() ; GlyphIt != this->Glyphs.end() ; ++GlyphIt )
            {
                delete (*GlyphIt).second;
            }
            this->Glyphs.clear();
        }

        void FontData::SetWhitespaceAtlasCoords(Glyph* Whitespace)
        {
            Whitespace->AtlasCoords[UI::QC_TopLeft] = Atlas->GetWhitePixel();
            Whitespace->AtlasCoords[UI::QC_TopRight] = Atlas->GetWhitePixel();
            Whitespace->AtlasCoords[UI::QC_BottomLeft] = Atlas->GetWhitePixel();
            Whitespace->AtlasCoords[UI::QC_BottomRight] = Atlas->GetWhitePixel();
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Utility Methods

        Real FontData::GetSpaceLength() const
            { return this->SpaceLength; }

        Real FontData::GetLineHeight() const
            { return this->LineHeight; }

        Real FontData::GetBaseLine() const
            { return this->BaseLine; }

        Real FontData::GetLetterSpacing() const
            { return this->LetterSpacing; }

        const String& FontData::GetName() const
            { return this->FontName; }

        TextureAtlas* FontData::GetAtlas() const
            { return this->Atlas; }

        const String& FontData::GetAtlasName() const
            { return this->Atlas->GetName(); }

        Glyph* FontData::GetGlyph(const UInt32& GlyphID) const
        {
            ConstGlyphIterator GlyphIt = this->Glyphs.find(GlyphID);
            if( GlyphIt != this->Glyphs.end() ) return (*GlyphIt).second;
            else return NULL;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Internal Methods

        void FontData::_SetSpaceLength(const Real SL)
            { this->SpaceLength = SL; }

        void FontData::_SetLineHeight(const Real LH)
            { this->LineHeight = LH; }

        void FontData::_SetBaseLine(const Real BL)
            { this->BaseLine = BL; }

        void FontData::_SetLetterSpacing(const Real LS)
            { this->LetterSpacing = LS; }

        void FontData::_SetName(const String& Name)
            { this->FontName = Name; }

        void FontData::_AddGlyph(Glyph* NewGlyph)
        {
            GlyphIterator GlyphIt = this->Glyphs.find(NewGlyph->GlyphID);
            if( GlyphIt == this->Glyphs.end() ) { this->Glyphs.insert(std::pair<UInt32,Glyph*>(NewGlyph->GlyphID,NewGlyph)); }
            else
            {
                StringStream ExceptionStream;
                ExceptionStream << "Glyph of ID: " << NewGlyph->GlyphID << " already exists in Font: \"" << this->FontName << "\".";
                MEZZ_EXCEPTION(ExceptionBase::II_DUPLICATE_IDENTITY_EXCEPTION,ExceptionStream.str());
            }
        }

        void FontData::_GenerateWhitespaceGlyphs()
        {
            Glyph* WhiteSpace = NULL;
            // Create a glyph for a space
            WhiteSpace = new Glyph(this,this->Atlas,Glyph::Space,this->SpaceLength,0);
            this->SetWhitespaceAtlasCoords(WhiteSpace);
            this->_AddGlyph(WhiteSpace);
            // Create a glyph for a tab
            WhiteSpace = new Glyph(this,this->Atlas,Glyph::HT,this->SpaceLength * 4,0);
            this->SetWhitespaceAtlasCoords(WhiteSpace);
            this->_AddGlyph(WhiteSpace);
            // Create a glyph for a vertical tab
            WhiteSpace = new Glyph(this,this->Atlas,Glyph::VT,this->SpaceLength * 4,0);
            this->SetWhitespaceAtlasCoords(WhiteSpace);
            this->_AddGlyph(WhiteSpace);
            // Create a glyph for a line feed
            WhiteSpace = new Glyph(this,this->Atlas,Glyph::LF,0,0);
            this->SetWhitespaceAtlasCoords(WhiteSpace);
            this->_AddGlyph(WhiteSpace);
            // Create a glyph for carriage return
            WhiteSpace = new Glyph(this,this->Atlas,Glyph::CR,0,0);
            this->SetWhitespaceAtlasCoords(WhiteSpace);
            this->_AddGlyph(WhiteSpace);
            // Create a glyph for next line
            WhiteSpace = new Glyph(this,this->Atlas,Glyph::NEL,0,0);
            this->SetWhitespaceAtlasCoords(WhiteSpace);
            this->_AddGlyph(WhiteSpace);
        }
    }//UI
}//Mezzanine

#endif
