// © Copyright 2010 - 2014 BlackTopp Studios Inc.
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
#ifndef _uiglyph_h
#define _uiglyph_h

#include "vector2.h"
#include "UI/uienumerations.h"
#include "UI/kerning.h"
#include "UI/font.h"
#include "UI/textureatlas.h"
#include "exception.h"
#include <vector>

namespace Mezzanine
{
    namespace UI
    {
        ///////////////////////////////////////////////////////////////////////////////
        /// @brief Class used to describe a single glyph or character available for text operations.
        /// @details
        ///////////////////////////////////////
        class MEZZ_LIB Glyph
        {
        public:
            typedef std::vector<KerningInfo>           KerningContainer;
            typedef KerningContainer::iterator         KerningIterator;
            typedef KerningContainer::const_iterator   ConstKerningIterator;

            /// @enum Whitespace
            /// @brief This enum represents the common whitespace characters found in Ascii/UTF-8.
            enum Whitespace
            {
                HT      = 0x0009, ///< Horizontal Tab
                LF      = 0x000A, ///< Line Feed/Newline
                VT      = 0x000B, ///< Vertical Tab
                CR      = 0x000D, ///< Carriage Return
                Space   = 0x0020, ///< Space
                // NEL is a bit hazy, care to be taken until the UI system is completely converted to Unicode
                NEL     = 0x0085  ///< Next Line/Newline
            };
        public:
            /// @brief Default constructor.
            /// @param Data The collection of glyphs this glyph belongs to.
            Glyph(FontData* Data) :
                Atlas(Data->GetAtlas()), Font(Data), GlyphID(0), GlyphAdvance(0), VerticalOffset(0) {  }
            /// @brief Descriptive constructor.
            /// @param Data The collection of glyphs this glyph belongs to.
            /// @param TexAtlas The TextureAtlas this glyph belongs to.
            /// @param ID The Character this glyph information represents.
            /// @param Advance The number of pixels to advance the cursor for the next glyph.
            /// @param VertOffset The amount of pixels the glyph is to be adjusted on the Y axis.
            Glyph(FontData* Data, TextureAtlas* TexAtlas, const UInt32& ID, const Real& Advance, const Real& VertOffset) :
                Atlas(TexAtlas), Font(Data), GlyphID(ID), GlyphAdvance(Advance), VerticalOffset(VertOffset) {  }
            /// @brief Class destructor.
            ~Glyph()
                { this->Kernings.clear(); }

            ///////////////////////////////////////////////////////////////////////////////
            // Public Data Members

            /// @brief The TextureAtlas this glyph belongs to.
            TextureAtlas* Atlas;
            /// @brief The collection of glyphs this glyph belongs to.
            FontData* Font;
            /// @brief The Character this glyph information represents.
            UInt32 GlyphID;
            /// @brief The number of pixels to advance the cursor for the next glyph.
            Real GlyphAdvance;
            /// @brief The amount of pixels the glyph is to be adjusted on the Y axis.
            Real VerticalOffset;
            /// @brief The 4 corner coordinates on the Texture.
            Vector2 AtlasCoords[4];
            /// @brief List of all the Kernings that apply to this glyph.
            KerningContainer Kernings;

            ///////////////////////////////////////////////////////////////////////////////
            // Utility Methods

            /// @brief Gets the name of the atlas this glyph belongs to.
            /// @return Returns a string containing the name of the atlas this glyph belongs to.
            inline const String& GetAtlasName() const
                { return this->Atlas->GetName(); }
            /// @brief Convenience function for getting the Kerning information for a given Glyph.
            /// @note This is the character to the left in languages like English(left to right), or the character to the right in languages like Arabic(right to left).
            /// @param Previous The previous glyph in the sequence.
            /// @return Returns in pixels the special amount to advance.
            inline Real GetKerning(const UInt32 Previous) const
            {
                Whole NumKernings = this->Kernings.size();
                if( NumKernings == 0 || Previous == 0 )
                    return 0;

                for( Whole Index = 0 ; Index < NumKernings ; ++Index )
                {
                    if( this->Kernings[Index].Character == Previous )
                        return this->Kernings[Index].Kerning;
                }
                return 0;
            }

            ///////////////////////////////////////////////////////////////////////////////
            // Whitespace Checks

            /// @brief Checks if this glyph is a horizontal tab.
            inline bool IsHorizontalTab() const
                { return (this->GlyphID == HT); }
            /// @brief Checks if this glyph is a line feed.
            inline bool IsLineFeed() const
                { return (this->GlyphID == LF); }
            /// @brief Checks if this glyph is a vertical tab.
            inline bool IsVerticalTab() const
                { return (this->GlyphID == VT); }
            /// @brief Checks if this glyph is a carriage return.
            inline bool IsCarriageReturn() const
                { return (this->GlyphID == CR); }
            /// @brief Checks if this glyph is a space.
            inline bool IsSpace() const
                { return (this->GlyphID == Space); }
            /// @brief Checks if this glyph is a next line.
            inline bool IsNextLine() const
                { return (this->GlyphID == NEL); }

            /// @brief Checks if this glyph marks a tab.
            inline bool IsTab() const
                { return this->IsHorizontalTab() || this->IsVerticalTab(); }
            /// @brief Checks if this glyph marks a new line.
            inline bool IsNewLine() const
                { return this->IsLineFeed() || this->IsCarriageReturn() || this->IsNextLine(); }
            /// @brief Checks if this glyph is not renderable.
            inline bool IsWhitespace() const
                { return this->IsTab() || this->IsNewLine() || this->IsSpace(); }

            ///////////////////////////////////////////////////////////////////////////////
            // Position and Size Methods

            /// @brief Gets the Top coordinate on the Texture.
            inline Real GetUVTop() const
                { return this->AtlasCoords[UI::QC_TopLeft].Y * this->Atlas->GetTextureSize().Y; }
            /// @brief Gets the Bottom coordinate on the Texture.
            inline Real GetUVBottom() const
                { return this->AtlasCoords[UI::QC_BottomRight].Y * this->Atlas->GetTextureSize().Y; }
            /// @brief Gets the Left coordinate on the Texture.
            inline Real GetUVLeft() const
                { return this->AtlasCoords[UI::QC_TopLeft].X * this->Atlas->GetTextureSize().X; }
            /// @brief Gets the Right coordinate on the Texture.
            inline Real GetUVRight() const
                { return this->AtlasCoords[UI::QC_BottomRight].X * this->Atlas->GetTextureSize().X; }
            /// @brief Gets the glyphs height on the Texture.
            inline Real GetHeight() const
                { return this->GetSize().Y; }
            /// @brief Gets the glyphs width on the Texture.
            inline Real GetWidth() const
                { return this->GetSize().X; }
            /// @brief Gets the position of the glyph on the Texture
            inline Vector2 GetPosition() const
                { return this->AtlasCoords[UI::QC_TopLeft] * this->Atlas->GetTextureSize(); }
            /// @brief Gets the size of the glyph on the Texture.
            inline Vector2 GetSize() const
            {
                if( this->IsNewLine() ) return Vector2(0,0);
                else if( this->IsSpace() || this->IsTab() ) return Vector2(this->GlyphAdvance,this->Font->GetBaseLine());
                else return (this->AtlasCoords[UI::QC_BottomRight] - this->AtlasCoords[UI::QC_TopLeft]) * this->Atlas->GetTextureSize();
            }
            /// @brief Gets the pixel position on the Atlas of a corner belonging to this glyph.
            /// @param Corner The corner to retrieve the coordinates for.
            /// @return Returns a Vector2 containing the Atlas pixel position of the specific corner.
            inline Vector2 GetAtlasCoords(const UI::QuadCorner Corner) const
            {
                switch(Corner)
                {
                    case UI::QC_TopLeft:      return this->AtlasCoords[UI::QC_TopLeft] * this->Atlas->GetTextureSize();      break;
                    case UI::QC_TopRight:     return this->AtlasCoords[UI::QC_TopRight] * this->Atlas->GetTextureSize();     break;
                    case UI::QC_BottomLeft:   return this->AtlasCoords[UI::QC_BottomLeft] * this->Atlas->GetTextureSize();   break;
                    case UI::QC_BottomRight:  return this->AtlasCoords[UI::QC_BottomRight] * this->Atlas->GetTextureSize();  break;
                }
                return Vector2(0,0);
            }
            /// @brief Gets the relative position on the Atlas of a corner belonging to this glyph.
            /// @param Corner The corner to retrieve the coordinates for.
            /// @return Returns a Vector2 containing the Atlas relative position of the specific corner.
            inline Vector2 GetRelativeAtlasCoords(const UI::QuadCorner Corner) const
            {
                switch(Corner)
                {
                    case UI::QC_TopLeft:      return this->AtlasCoords[UI::QC_TopLeft];      break;
                    case UI::QC_TopRight:     return this->AtlasCoords[UI::QC_TopRight];     break;
                    case UI::QC_BottomLeft:   return this->AtlasCoords[UI::QC_BottomLeft];   break;
                    case UI::QC_BottomRight:  return this->AtlasCoords[UI::QC_BottomRight];  break;
                }
                return Vector2(0,0);
            }
        };//Glyph
    }//UI
}//Mezzanine

#endif
