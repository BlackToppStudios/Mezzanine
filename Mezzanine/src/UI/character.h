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

#ifndef _uicharacter_h
#define _uicharacter_h

#include "UI/glyph.h"
#include "UI/sprite.h"
#include "UI/charactertraits.h"
#include "UI/uienumerations.h"

namespace Mezzanine
{
    namespace UI
    {
        class TextLayer;
        class TextureAtlas;
        ///////////////////////////////////////////////////////////////////////////////
        /// @brief This class creates and encapsultes a character that can be used in text renders.
        /// @details
        ///////////////////////////////////////
        class MEZZ_LIB Character
        {
        protected:
            /// @internal
            /// @brief The rendering traits of this character.
            CharacterTraits CharTraits;
            /// @internal
            /// @brief The forced size of this character, if it is set.
            Vector2 CustomSize;
            /// @internal
            /// @brief A pointer to the layer this character is being rendered by.
            TextLayer* Layer;
            /// @internal
            /// @brief A pointer to the Glyph this character is being rendered as, or NULL if it is a Sprite.
            Glyph* CharGlyph;
            /// @internal
            /// @brief A pointer to the Sprite this character is being rendered as, or NULL if it is a Glyph.
            Sprite* CharSprite;
            /// @internal
            /// @brief The X-axis offset from the left side where this character is being rendered relative to the TextLine position.
            Real LengthOffset;
            /// @internal
            /// @brief Stores whether or not this character is being highlighted.
            Bool Highlighted;
        public:
            /// @brief GlyphID Constructor.
            /// @param GlyphID The ID of the glyph to be used when creating this character.
            /// @param Traits The sharable metadata to be given to this Character.
            /// @param Creator The TextLayer that created this Character.
            Character(const UInt32& GlyphID, const CharacterTraits& Traits, TextLayer* Creator);
            /// @brief Glyph Constructor.
            /// @param CharacterGlyph The glyph used when rendering this Character.
            /// @param Creator The TextLayer that created this Character.
            Character(Glyph* CharacterGlyph, TextLayer* Creator);
            /// @brief Styled Glyph Constructor.
            /// @param CharacterGlyph The glyph used when rendering this Character.
            /// @param Traits The sharable metadata to be given to this Character.
            /// @param Creator The TextLayer that created this Character.
            Character(Glyph* CharacterGlyph, const CharacterTraits& Traits, TextLayer* Creator);
            /// @brief Sprite Constructor.
            /// @param CharacterSprite The sprite used when rendering this character.
            /// @param Creator The TextLayer that created this Character.
            Character(Sprite* CharacterSprite, TextLayer* Creator);
            /// @brief Styled Sprite Constructor.
            /// @param CharacterSprite The sprite used when rendering this character.
            /// @param Traits The sharable metadata to be given to this Character.
            /// @param Creator The TextLayer that created this Character.
            Character(Sprite* CharacterSprite, const CharacterTraits& Traits, TextLayer* Creator);

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            /// @brief Gets the amount of pixels to advance the cursor after placing this character.
            /// @param Prev The glyph prior to this one in the parent TextLine.  Needed for kerning information.
            /// @return Returns a Real representing the number of pixels to advance.
            Real GetCharacterAdvance(Glyph* Prev) const;
            /// @brief Gets the height a line needs to be to support rendering this character.
            /// @return Returns a Real representing how tall a TextLine needs to be to support this character in pixels.
            Real GetLineHeight() const;
            /// @brief Gets the vertical adjustment for this character.
            /// @return Returns a Real representing the position adjustment on the Y axis in pixels.
            Real GetVerticalOffset() const;

            /// @brief Gets the atlas this character is using for rendering.
            /// @return Returns a pointer to this characters atlas.
            TextureAtlas* GetAtlas() const;
            /// @brief Gets the name of the atlas this character is using for rendering.
            /// @return Returns a const String reference containing the name of this characters atlas.
            const String& GetAtlasName() const;
            /// @brief Gets the coordinates to the white pixel on the atlas this character belongs to.
            /// @return Returns a Vector2 containing the relative coordinates of the atlas whitepixel.
            Vector2 GetAtlasWhitePixel() const;
            /// @brief Gets the atlas coordinates of a quad corner.
            /// @param Corner The corner to get the atlas coordinates for.
            /// @return Returns a const Vector2 reference to the atlas coordinates to the requested corner.
            Vector2 GetAtlasCoords(const UI::QuadCorner Corner) const;

            /// @brief Sets the fill colour for this character.
            /// @note With most text/glyphs this will simply set the colour of the letters.  In the case of
            /// sprites or special coloured text this will blend with the rendered colour giving a third result.
            /// @param Colour The colour to render this character with.
            void SetCharacterColour(const ColourValue& Colour);
            /// @brief Gets the fill colour of this character.
            /// @return Returns a const colourvalue reference containing the fill colour of this character.
            const ColourValue& GetCharacterColour() const;

            /// @brief Sets whether or not this character should be highlighted.
            /// @param Highlight True to highlight this character, false to remove a highlight on it.
            void SetHighlighted(Bool Highlight);
            /// @brief Gets if this character is being highlighted.
            /// @return Returns true if this character is being highlighted, false otherwise.
            Bool GetHighlighted() const;

            /// @brief Sets this Characters sharable traits.
            /// @param Traits The traits to be given to this Character.
            void SetTraits(const CharacterTraits& Traits);
            /// @brief Gets this Characters sharable traits.
            /// @return Returns a const reference to the sharable traits belonging to this Character.
            const CharacterTraits& GetTraits() const;

            /// @brief Gets whether or not this character is a glyph.
            /// @return Returns true if this character is a glyph, false otherwise.
            Bool IsGlyph() const;
            /// @brief Gets whether or not this character is a sprite.
            /// @return Returns true if this character is a sprite, false otherwise.
            Bool IsSprite() const;

            /// @brief Gets if this character contains valid data for rendering.
            /// @return Returns true if this character can be rendered, false otherwise.
            Bool IsRenderable() const;
            /// @brief Gets if this character can be highlighted.
            /// @return Returns true if this character can be highlighted, false otherwise.
            Bool IsHighlightable() const;
            /// @brief Gets if this character is a whitespace glyph.
            /// @return Returns true if this character is a whitespace, false otherwise.
            Bool IsWhiteSpace() const;
            /// @brief Gets if this character is a line break character.
            /// @return Returns true if this character indicates a new line, false otherwise.
            Bool IsNewLine() const;

            ///////////////////////////////////////////////////////////////////////////////
            // Transform Related Methods

            /// @brief Sets the offset from the position of the parent TextLine.
            /// @warning Adjusting this value manually can create visual artifacts.  Only call this manually if you know what you are doing.
            /// @param Offset The placement offset from the parent's origin in pixels.
            void SetLengthOffset(const Real& Offset);
            /// @brief Gets the length offset of this Character.
            /// @return Returns a real representing the number of pixels to offset this from it's parent.
            Real GetLengthOffset() const;
            /// @brief Sets the custom size this character is to be resized to.
            /// @remarks The size set here is still subject to scaling from it's parent, and thus is not final. @n @n
            /// The default for this value is (-1,-1), and this can be set at any time to revert to using the characters default size.
            /// @param Size The custom size to apply to this character in pixels.
            void SetCustomSize(const Vector2& Size);
            /// @brief Whether or not the custom size has been set.
            /// @return Returns true if a custom size has been provided for this character, false if it is using it's default size.
            Bool IsCustomSizeSet() const;
            /// @brief Gets the rendered size of this character.
            /// @return Returns a Vector2 containing the size this character is to be rendered at before scaling.
            Vector2 GetCharacterSize() const;

            ///////////////////////////////////////////////////////////////////////////////
            // Fetch Methods

            /// @brief Gets the glyph being rendered by this character.
            /// @return Returns a pointer to the glyph this character is representing, or NULL if this character is a Sprite.
            Glyph* GetCharGlyph() const;
            /// @brief Gets the sprite being rendered by this character.
            /// @return Returns a pointer to the sprite this character is representing, or NULL if this character is a Glyph.
            Sprite* GetCharSprite() const;
        };//Character
    }//UI
}//Mezzanine

#endif
