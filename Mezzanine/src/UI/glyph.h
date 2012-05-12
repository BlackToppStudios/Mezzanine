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
#ifndef _uiglyph_h
#define _uiglyph_h

#include "vector2.h"
#include "UI/kerning.h"
#include <vector>

namespace Mezzanine
{
    namespace UI
    {
        /// @class Glyph
        /// @headerfile uiglyph.h
        /// @brief Class used to describe a single glyph or character available for text operations.
        /// @details
        class MEZZ_LIB Glyph
        {
            public:
                /// @brief Class constructor.
                Glyph() : UVTop(0), UVBottom(0), UVWidth(0), UVHeight(0), UVLeft(0), UVRight(0), GlyphAdvance(0), VerticalOffset(0), GlyphSize(Vector2()) {};
                /// @brief Class destructor.
                ~Glyph() {};

                /// @brief The Character this glyph information represents.
                char GlyphChar;
                /// @brief Top Coordinate on the Texture.
                Real UVTop;
                /// @brief Bottom Coordinate on the Texture.
                Real UVBottom;
                /// @brief Width of the character on the Texture.
                Real UVWidth;
                /// @brief Height of the character on the Texture.
                Real UVHeight;
                /// @brief Left Coordinate on the Texture.
                Real UVLeft;
                /// @brief Right Coordinate on the Texture.
                Real UVRight;
                /// @brief The amount of pixels the glyph is to be adjusted on the Y axis.
                Real VerticalOffset;
                /// @brief The amount of pixels to set the start of the next glyph.
                Real GlyphAdvance;
                /// @brief The overall size of the glyph.
                Vector2 GlyphSize;
                /// @brief The 4 corner coordinates on the Texture.
                Vector2 AtlasCoords[4];
                /// @brief List of all the Kernings that apply to this glyph.
                std::vector<KerningInfo> Kernings;

                /// @brief Convenience function for getting the Kerning information for a given Glyph.
                /// @param Left_of The glyph/character to the left of this one to get the Kerning information for.
                /// @return Returns in pixels the special amount to advance.
                inline const Real GetKerning(const char& Left_of) const
                {
                    if(Kernings.size() == 0)
                        return 0;
                    for(size_t i=0;i < Kernings.size();i++)
                    {
                        if((UInt32)(Kernings[i].Character) == Left_of)
                            return Kernings[i].Kerning;
                    }
                    return 0;
                }
        };//Glyph

        /// @class GlyphData
        /// @headerfile uiglyph.h
        /// @brief Class used to describe a single glyph or character available for text operations.
        /// @details
        class MEZZ_LIB GlyphData
        {
            protected:
            public:
                /// @brief Class constructor.
                GlyphData() : RangeBegin(0), RangeEnd(0), SpaceLength(0), LineHeight(0), Baseline(0), LetterSpacing(0), MonoWidth(0) {};
                /// @brief Class destructor.
                ~GlyphData() {};

                /// @brief The ASCII code for the first character in this set.
                UInt32 RangeBegin;
                /// @brief The ASCII code for the last character in this set.
                UInt32 RangeEnd;
                /// @brief The width in pixels of a space in this set.
                Real SpaceLength;
                /// @brief The height in pixels of glyphs in this set.
                Real LineHeight;
                /// @brief Not really sure, honestly.
                Real Baseline;
                /// @brief Default width in pixels letters have without Kerning or special glyph spacing.
                Real LetterSpacing;
                /// @brief Width in pixels to use when MonoWidth is enabled for Markup Texts.
                Real MonoWidth;
                /// @brief Name of the Atlas that this GlyphData belongs.
                String Atlas;
                /// @brief Vector Storing all the Glyphs contained in this set.
                std::vector<Glyph*> Glyphs;

                /// @brief Gets the glyph corresponding to the provided characters ASCII code.
                /// @param Character The ASCII code for the glyph desired.
                /// @return Returns a pointer to the requested Glyph, or NULL if it doesn't exist.
                inline Glyph* GetGlyph(const UInt32& Character) const
                {
                    UInt32 SafeCharacter = Character - RangeBegin;
                    if (SafeCharacter >= 0 && SafeCharacter <= Glyphs.size())
                        return Glyphs[SafeCharacter];
                    return 0;
                }
        };//GlyphData
    }//UI
}//Mezzanine

#endif
