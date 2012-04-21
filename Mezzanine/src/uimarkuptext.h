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
#ifndef _uimarkuptext_h
#define _uimarkuptext_h

#include "uienumerations.h"
#include "colourvalue.h"
#include "uirenderablerect.h"
#include "uirectangle.h"
#include "uiglyph.h"

namespace Mezzanine
{
    class UIManager;
    namespace UI
    {
        class Layer;
        ///////////////////////////////////////////////////////////////////////////////
        /// @class Character
        /// @headerfile uimarkuptext.h
        /// @brief This class creates and encapsultes a character that can be used in a Word.
        /// @details
        ///////////////////////////////////////
        struct Character
        {
            Character() : Kerning(0), CharGlyph(NULL), CharSprite(NULL) {};

            Vector2        Positions[4];
            Vector2        UVs[4];
            ColourValue    Colour;
            Whole          Index;
            Real           Kerning;
            String         Atlas;
            Glyph*         CharGlyph;
            Sprite*        CharSprite;
        };//Character
        ///////////////////////////////////////////////////////////////////////////////
        /// @class Word
        /// @headerfile uimarkuptext.h
        /// @brief This class creates and encapsultes the concept of a word in a MarkupText.
        /// @details
        ///////////////////////////////////////
        class Word
        {
            public:
            Word() : NewlineBefore(false), Pxlength(0), Pxheight(0), GlyphDataSpacing(0) {};
            ~Word() { Characters.clear(); };

            bool NewlineBefore;
            Real Pxlength;
            Real Pxheight;
            Real GlyphDataSpacing;
            std::vector<Character*> Characters;
            void AddCharacter(Character* ToAdd, const Real& Scaling)
            {
                Real CharWidth = ToAdd->CharGlyph ? ToAdd->CharGlyph->GlyphAdvance + ToAdd->Kerning : ToAdd->CharSprite->SpriteSize.X;
                CharWidth*=Scaling;
                Pxlength += CharWidth;
                Real CharHeight = ToAdd->CharGlyph ? ToAdd->CharGlyph->GlyphSize.Y : ToAdd->CharSprite->SpriteSize.Y;
                CharHeight*=Scaling;
                if(Pxheight < CharHeight)
                    Pxheight = CharHeight;
                Characters.push_back(ToAdd);
            }
        };//Word
        ///////////////////////////////////////////////////////////////////////////////
        /// @class MarkupText
        /// @headerfile uimarkuptext.h
        /// @brief This is a helper class for use with specialized text display.
        /// @details Markup Texts and Captions are similar in that they both display text messages
        /// that can be altered readily, the primary difference between the two is that Captions are
        /// meant for small simple messages with background functionality built in, where as Markup
        /// Texts have no background functionality, but they use a light markup language to accomplish
        /// special effects with the text.
        ///////////////////////////////////////
        class MEZZ_LIB MarkupText : public Rectangle
        {
            protected:
                friend class Layer;
                GlyphData* DefaultGlyphSet;
                bool AutoScaleText;
                bool TextDirty;
                Real RelLineHeight;
                Real CharScaling;
                Whole ClippedIndex;
                String Text;
                String GlyphAtlas;
                std::vector<Character*> Characters;
                std::vector<Word*> Words;
            public:
                /// @brief Internal constructor.
                /// @details The constructor will ignore the size portion of the Rect passed in, since Markup Texts don't have a default size.
                /// @param name The name of this markup text.
                /// @param Rect The Rect representing the position and size of the markup text.
                /// @param Glyph One of the glyphs specified in your mta file.  Must be valid.
                /// @param Text Any text you want printed on the markup text.
                /// @param Layer Pointer to the layer that created this markup text.
                MarkupText(ConstString& name, const RenderableRect& Rect, const Whole& Glyph, const String& Text, Layer* PLayer);
                /// @brief Internal constructor.
                /// @details The constructor will ignore the size portion of the Rect passed in, since Markup Texts don't have a default size.
                /// @param name The name of this markup text.
                /// @param Rect The Rect representing the position and size of the markup text.
                /// @param LineHeight The lineheight you want the text to have in relative units.  This will automatically select the glyph and scale it for you.
                /// @param Text Any text you want printed on the markup text.
                /// @param Layer Pointer to the layer that created this markup text.
                MarkupText(ConstString& name, const RenderableRect& Rect, const Real& LineHeight, const String& Text, Layer* PLayer);
                /// @brief Class destructor.
                virtual ~MarkupText();

                /// @brief Sets the text displayed within the markup text.
                /// @param Text The text to be displayed.
                virtual void SetText(const String& Text);
                /// @brief Gets the text displayed within the markup text.
                /// @return Returns the text being displayed.
                virtual String GetText();
                /// @brief Sets the scaling to be applied to the text being rendered.
                /// @param Scale A Real value representing the scale to be applied.  <1.0 means smaller, >1.0 means larger.
                virtual void SetTextScale(const Real& Scale);
                /// @brief Gets the scaling currently being applied to the rendered text.
                /// @return Returns a Real value representing the scale applied to the text in this caption.  <1.0 means smaller, >1.0 means larger.
                virtual Real GetTextScale();

                /// @brief Sets the Default glyph index to be used with this markup text.
                /// @details The glyph index is defined in your mta file.  This class can change which
                /// glyph is uses with it's markup language.  This simply defines which to use when one isn't
                /// specified.
                /// @param DefaultGlyphIndex The index of the glyph to use with this markup text.
                virtual void SetDefaultGlyphIndex(const Whole& DefaultGlyphIndex);
                /// @brief Sets the Default glyph index to be used with this markup text.
                /// @details The glyph index is defined in your mta file.  This class can change which
                /// glyph is uses with it's markup language.  This simply defines which to use when one isn't
                /// specified.
                /// @param DefaultGlyphIndex The index of the glyph to use with this markup text.
                /// @param Atlas The different atlas to get the glyphdata from.
                virtual void SetDefaultGlyphIndex(const Whole& DefaultGlyphIndex, const String& Atlas);
                /// @brief Gets the Default glyphdata in use by this markup text.
                /// @details The glyphdata is defined in your mta file.
                /// @return Returns a pointer to the glyphdata in use by this markup text.
                virtual GlyphData* GetDefaultGlyphData();

                /// @brief Enables or disables scaling the text automatically on a screen size change.
                /// @param Enable Enables or disables automatic text scaling.
                virtual void SetAutoScaleText(bool Enable);
                /// @brief Gets whether or not text from this caption will automatically scale on a screen size change.
                /// @return Returns a bool indicating whether or not this caption will automatically scale it's text.
                virtual bool GetAutoScaleText();
                /// @brief Updates the dimensions of this markup text to match those of the new screen size.
                /// @details This function is called automatically when a viewport changes in size, and shouldn't need to be called manually.
                virtual void UpdateDimensions();
                ///////////////////////////////////////////////////////////////////////////////
                // Internal Functions
                ///////////////////////////////////////
                /// @copydoc UI::BasicRenderable::_Redraw()
                virtual void _Redraw();
                /// @brief Calculates the placement of glyphs for this renderable.
                virtual void _CalculateCharacters();
                /// @brief Generates the words for placement in this renderable.
                virtual void _GenerateWords();
                /// @brief Places the generated words in this renderable.
                virtual void _PlaceWords();
                /// @brief Removes all generated words to be regenerated.
                virtual void _ClearWords();
        };
    }//UI
}//Mezzanine

#endif
