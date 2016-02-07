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
#ifndef _uifont_h
#define _uifont_h

#include "datatypes.h"

namespace Mezzanine
{
    namespace UI
    {
        class Glyph;
        class TextureAtlas;
        ///////////////////////////////////////////////////////////////////////////////
        /// @brief This class represents a collection of Glyphs in a common visual style.
        /// @details
        ///////////////////////////////////////
        class MEZZ_LIB FontData
        {
        public:
            /// @brief Container type for Glyph storage in this class.
            typedef std::map<UInt32,Glyph*>          GlyphContainer;
            /// @brief Iterator type for Glyphs stored by this class.
            typedef GlyphContainer::iterator         GlyphIterator;
            /// @brief Const Iterator type for Glyphs stored by this class.
            typedef GlyphContainer::const_iterator   ConstGlyphIterator;
        protected:
            /// @internal
            /// @brief Container storing all the Glyphs contained in this font.
            GlyphContainer Glyphs;
            /// @internal
            /// @brief The name of this font.
            String FontName;
            /// @internal
            /// @brief The Atlas that this GlyphData belongs to.
            TextureAtlas* Atlas;
            /// @internal
            /// @brief The width in pixels of a space in this font.
            Real SpaceLength;
            /// @internal
            /// @brief The height of a line of text in this font.
            Real LineHeight;
            /// @internal
            /// @brief The height of the largest glyph in this font.
            Real BaseLine;
            /// @internal
            /// @brief I don't know.
            Real LetterSpacing;

            /// @brief Sets the dummy coordinates for a generated whitespace glyph.
            void SetWhitespaceAtlasCoords(Glyph* Whitespace);
        public:
            /// @brief Class constructor.
            /// @param ParentAtlas The TextureAtlas this font data belongs to.
            FontData(TextureAtlas* ParentAtlas);
            /// @brief Class destructor.
            ~FontData();

            ///////////////////////////////////////////////////////////////////////////////
            // Utility Methods

            /// @brief Gets the length of a space in this font.
            /// @return Returns a Real containing the length of a space in pixels.
            Real GetSpaceLength() const;
            /// @brief Gets the height of a line of text in this font.
            /// @return Returns a Real containing the height of a line in this font in pixels.
            Real GetLineHeight() const;
            /// @brief Gets the height of the largest glyph in this font.
            /// @return Returns a Real containing the height of the largest glyph in this font in pixels.
            Real GetBaseLine() const;
            /// @brief Gets the spacing to apply between letters in this font.
            /// @return Returns a Real containing the amount of pixels to adjust the advance of every glyph in this font.
            Real GetLetterSpacing() const;
            /// @brief Gets the name of this font.
            /// @return Returns a const String reference containing the name of this font.
            const String& GetName() const;
            /// @brief Gets the TextureAtlas this font belongs to.
            /// @return Returns a pointer to the TextureAtlas this font belongs to.
            TextureAtlas* GetAtlas() const;
            /// @brief Gets the name of the TextureAtlas this font belongs to.
            /// @return Returns a const String reference containing the name of the TextureAtlas this font belongs to.
            const String& GetAtlasName() const;
            /// @brief Gets the glyph corresponding to the provided characters UTF-8 code.
            /// @param GlyphID The UTF-8 code for the glyph desired.
            /// @return Returns a pointer to the requested Glyph, or NULL if it doesn't exist.
            Glyph* GetGlyph(const UInt32& GlyphID) const;

            ///////////////////////////////////////////////////////////////////////////////
            // Internal Methods

            /// @internal
            /// @brief Sets the length of a space for this font.
            /// @param SL The length of a space in pixels.
            void _SetSpaceLength(const Real SL);
            /// @internal
            /// @brief Sets the height of a single line of text in this font.
            /// @param LH The height of a line of text in this font in pixels.
            void _SetLineHeight(const Real LH);
            /// @internal
            /// @brief Sets the height of the largest glyph in this font.
            /// @param BL The maximum height of a glyph in this font in pixels.
            void _SetBaseLine(const Real BL);
            /// @internal
            /// @brief Sets the LetterSpacing of this font.
            /// @param LS The value to be set for this fonts LetterSpacing.
            void _SetLetterSpacing(const Real LS);
            /// @internal
            /// @brief Sets the name of this font.
            /// @param Name The name to be given to this font.
            void _SetName(const String& Name);
            /// @internal
            /// @brief Adds a new glyph to this Font.
            /// @exception An exception will be thrown if a glyph with the same ID is already taken.
            /// @param NewGlyph A pointer to the new glyph to be added.
            void _AddGlyph(Glyph* NewGlyph);
            /// @brief Generates Whitespace Glyphs from this font's data for this font.
            void _GenerateWhitespaceGlyphs();
        };//FontData
    }//UI
}//Mezzanine

#endif
