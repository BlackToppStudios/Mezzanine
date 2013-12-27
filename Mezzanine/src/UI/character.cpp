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

#ifndef _uicharacter_cpp
#define _uicharacter_cpp

#include "UI/character.h"
#include "UI/textlayer.h"

namespace Mezzanine
{
    namespace UI
    {
        ///////////////////////////////////////////////////////////////////////////////
        // Character Methods

        Character::Character(const UInt32& GlyphID, const CharacterTraits& Traits, TextLayer* Creator) :
            CustomSize(-1,-1),
            Layer(Creator),
            CharSprite(NULL),
            LengthOffset(0),
            Highlighted(false)
        {
            // Assign the style
            this->CharTraits = Traits;

            if( CharTraits.CharFont == NULL ) {
                MEZZ_EXCEPTION(Exception::PARAMETERS_EXCEPTION,"A Font is not defined in CharacterTraits when it is expected to be during Character initialization.");
            }
            this->CharGlyph = CharTraits.CharFont->GetGlyph(GlyphID);
        }

        Character::Character(Glyph* CharacterGlyph, TextLayer* Creator) :
            CustomSize(-1,-1),
            Layer(Creator),
            CharGlyph(CharacterGlyph),
            CharSprite(NULL),
            LengthOffset(0),
            Highlighted(false)
        {
            // Since none was specified, create a default
            this->CharTraits.CharFont = CharGlyph->Font;
            this->CharTraits.CharColour = ColourValue::White();
        }

        Character::Character(Glyph* CharacterGlyph, const CharacterTraits& Traits, TextLayer* Creator) :
            CustomSize(-1,-1),
            Layer(Creator),
            CharGlyph(CharacterGlyph),
            CharSprite(NULL),
            LengthOffset(0),
            Highlighted(false)
        {
            // Assign the style
            this->CharTraits = Traits;

            if( CharTraits.CharFont != CharGlyph->Font ) {
                MEZZ_EXCEPTION(Exception::PARAMETERS_EXCEPTION,"Font provided in traits does not match the font defined in provided glyph during Character initialization.");
            }
        }

        Character::Character(Sprite* CharacterSprite, TextLayer* Creator) :
            CustomSize(-1,-1),
            Layer(Creator),
            CharGlyph(NULL),
            CharSprite(CharacterSprite),
            LengthOffset(0),
            Highlighted(false)
        {
            // Since none was specified, create a default
            this->CharTraits.CharFont = NULL;
            this->CharTraits.CharColour = ColourValue::White();
        }

        Character::Character(Sprite* CharacterSprite, const CharacterTraits& Traits, TextLayer* Creator) :
            CustomSize(-1,-1),
            Layer(Creator),
            CharGlyph(NULL),
            CharSprite(CharacterSprite),
            LengthOffset(0),
            Highlighted(false)
        {
            // Assign the style
            this->CharTraits = Traits;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        Real Character::GetCharacterAdvance(Glyph* Prev) const
        {
            if( this->IsGlyph() ) {
                if( this->IsCustomSizeSet() ) return this->CustomSize.X;
                else return this->CharGlyph->GlyphAdvance + ( Prev != NULL ? this->CharGlyph->GetKerning(Prev->GlyphID) : 0 );
            }else if( this->IsSprite() ) {
                return this->GetCharacterSize().X;
            }else{
                return 0;
            }
        }

        Real Character::GetLineHeight() const
        {
            if( this->IsGlyph() ) return this->CharGlyph->Font->GetLineHeight();
            else if( this->IsSprite() ) return this->GetCharacterSize().Y;
            else return 0;
        }

        Real Character::GetVerticalOffset() const
        {
            // If this is a glyph, combine the manual vertical offset for low hanging letters as well as the difference between the baseline and lineheight.
            // In almost all cases the combined offset should be a positive number, elevating the normal letters and thus making everything more centered.
            /// @todo If we want to change how normal text is aligned on the Y axis, we need to update this.
            if( this->IsGlyph() ) {
                return this->CharGlyph->VerticalOffset;// + (this->CharGlyph->Font->GetLineHeight() - this->CharGlyph->Font->GetBaseLine());
            }
            return 0;
        }

        TextureAtlas* Character::GetAtlas() const
        {
            if( this->IsGlyph() ) {
                return this->CharGlyph->Atlas;
            }else if( this->IsSprite() ) {
                return this->CharSprite->Atlas;
            }else{
                return NULL;
            }
        }

        const String& Character::GetAtlasName() const
        {
            if( this->IsGlyph() ) {
                return this->CharGlyph->GetAtlasName();
            }else if( this->IsSprite() ) {
                return this->CharSprite->GetAtlasName();
            }
        }

        Vector2 Character::GetAtlasWhitePixel() const
        {
            return this->GetAtlas()->GetWhitePixel();
        }

        Vector2 Character::GetAtlasCoords(const UI::QuadCorner Corner) const
        {
            if( this->IsGlyph() ) return CharGlyph->GetAtlasCoords(Corner);
            else if( this->IsSprite() ) return CharSprite->GetAtlasCoords(Corner);
            else return Vector2();
        }

        Vector2 Character::GetRelativeAtlasCoords(const UI::QuadCorner Corner) const
        {
            if( this->IsGlyph() ) return CharGlyph->GetRelativeAtlasCoords(Corner);
            else if( this->IsSprite() ) return CharSprite->GetRelativeAtlasCoords(Corner);
            else return Vector2();
        }

        void Character::SetCharacterColour(const ColourValue& Colour)
        {
            if( this->CharTraits.CharColour != Colour )
            {
                this->CharTraits.CharColour = Colour;

                if( !Highlighted )
                   this->Layer->_MarkDirty();
            }
        }

        const ColourValue& Character::GetCharacterColour() const
        {
            return this->CharTraits.CharColour;
        }

        void Character::SetHighlighted(Bool Highlight)
        {
            this->Highlighted = ( this->IsHighlightable() && Highlight );
        }

        Bool Character::GetHighlighted() const
        {
            return this->Highlighted;
        }

        void Character::SetTraits(const CharacterTraits& Traits)
        {
            if( this->CharTraits != Traits ) {
                this->CharTraits = Traits;
                this->Layer->_MarkDirty();
            }
        }

        const CharacterTraits& Character::GetTraits() const
        {
            return this->CharTraits;
        }

        Bool Character::IsGlyph() const
        {
            return (NULL != this->CharGlyph);
        }

        Bool Character::IsSprite() const
        {
            return (NULL != this->CharSprite);
        }

        Bool Character::IsRenderable() const
        {
            if( this->CharGlyph && this->CharGlyph->IsWhitespace() ) return false;
            else return true;
        }

        Bool Character::IsHighlightable() const
        {
            if( this->CharGlyph && this->CharGlyph->IsNewLine() ) return false;
            else return true;
        }

        Bool Character::IsWhiteSpace() const
        {
            if( this->CharGlyph && this->CharGlyph->IsWhitespace() ) return true;
            else return false;
        }

        Bool Character::IsNewLine() const
        {
            if( this->CharGlyph && this->CharGlyph->IsNewLine() ) return true;
            else return false;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Transform Related Methods

        void Character::SetLengthOffset(const Real& Offset)
        {
            this->LengthOffset = Offset;
        }

        Real Character::GetLengthOffset() const
        {
            return this->LengthOffset;
        }

        void Character::SetCustomSize(const Vector2& Size)
        {
            if( this->CustomSize != Size ) {
                this->CustomSize = Size;
                this->Layer->_MarkDirty();
            }
        }

        Bool Character::IsCustomSizeSet() const
        {
            const Vector2 Test(-1,-1);
            return (this->CustomSize != Test);
        }

        Vector2 Character::GetCharacterSize() const
        {
            if( this->IsCustomSizeSet() ) return CustomSize;
            else return ( this->CharGlyph ? this->CharGlyph->GetSize() : this->CharSprite->GetSize() );
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Fetch Methods

        Glyph* Character::GetCharGlyph() const
            { return this->CharGlyph; }

        Sprite* Character::GetCharSprite() const
            { return this->CharSprite; }
    }//UI
}//Mezzanine

#endif
