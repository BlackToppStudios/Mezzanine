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
#ifndef _uitextbutton_h
#define _uitextbutton_h

#include "uibutton.h"
#include "uiglyph.h"

namespace Mezzanine
{
    namespace UI
    {
        class Layer;
        ///////////////////////////////////////////////////////////////////////////////
        /// @class TextButton
        /// @headerfile uitextbutton.h
        /// @brief This is a button class that provides text capabilities.
        /// @details
        ///////////////////////////////////////
        class MEZZ_LIB TextButton : public Button
        {
            protected:
                GlyphData* GlyphSet;
                bool AutoScaleText;
                UI::TextHorizontalAlign HoriAlign;
                UI::TextVerticalAlign VertAlign;
                Real CursorOffset;
                Real CharScaling;
                Real RelLineHeight;
                Real ClippedLeftIndex;
                Real ClippedRightIndex;
                ColourValue TextColour;
                String Text;
                String GlyphAtlas;
                void CalculateDrawSize(Vector2& Size);
            public:
                /// @brief Internal constructor
                /// @param name The name of the button.
                /// @param Rect The Rect representing the position and size of the button.
                /// @param Glyph One of the glyphs specified in your mta file.  Must be valid.
                /// @param Text Any text you want printed on the button.
                /// @param Layer Pointer to the Layer that created this button.
                TextButton(ConstString& name, const RenderableRect& Rect, const Whole& Glyph, const String& Text, Layer* PLayer);
                /// @brief Internal constructor
                /// @param name The name of the button.
                /// @param Rect The Rect representing the position and size of the button.
                /// @param LineHeight The lineheight you want the text to have in relative units.  This will automatically select the glyph and scale it for you.
                /// @param Text Any text you want printed on the button.
                /// @param Layer Pointer to the Layer that created this button.
                TextButton(ConstString& name, const RenderableRect& Rect, const Real& LineHeight, const String& Text, Layer* PLayer);
                /// @brief Class destructor.
                virtual ~TextButton();

                /// @brief Gets whether this is a text button.
                /// @return Returns a bool representing whether or not this is a text button.
                virtual bool IsTextButton();
                /// @brief Sets the text displayed within the button.
                /// @param Text The text to be displayed.
                virtual void SetText(ConstString& Text);
                /// @brief Gets the text displayed within the button.
                /// @return Returns the text being displayed.
                virtual String GetText();
                /// @brief Sets the scaling to be applied to the text being rendered.
                /// @param Scale A Real value representing the scale to be applied.  <1.0 means smaller, >1.0 means larger.
                virtual void SetTextScale(const Real& Scale);
                /// @brief Gets the scaling currently being applied to the rendered text.
                /// @return Returns a Real value representing the scale applied to the text in this button.  <1.0 means smaller, >1.0 means larger.
                virtual Real GetTextScale();
                /// @brief Sets the colour for the text in this button.
                /// @param Colour A colourvalue representing the desired colour.
                virtual void SetTextColour(const ColourValue& Colour);
                /// @brief Gets the colour for the text in this button.
                /// @return Returns a colourvalue representing the currently set colour of the text in this button.
                virtual ColourValue GetTextColour();

                /// @brief Sets the glyph index to be used with this button.
                /// @details The glyph index is defined in your MTA file.
                /// @param GlyphIndex The index of the glyph to use with this button.
                virtual void SetGlyphIndex(const Whole& GlyphIndex);
                /// @brief Sets the glyph index to be used with this button from another atlas then the one currently set.
                /// @details The glyph index is defined in your MTA file.
                /// @param GlyphIndex The index of the glyph to use with this button.
                /// @param Atlas The Atlas to load the glyphs from
                virtual void SetGlyphIndex(const Whole& GlyphIndex, const String& Atlas);
                /// @brief Gets the glyphdata in use by this button.
                /// @details The glyphdata is defined in your MTA file.
                /// @param Returns a pointer to the glyphdata in use by this button.
                virtual GlyphData* GetGlyphData();

                /// @brief Sets the number of pixels text should be offset from the side when rendering.
                /// @details This doesn't apply when text is being rendered with a Middle alignment.
                /// @param Offset The number of pixels from the side text should be offset.
                virtual void SetCursorOffset(const Whole& Offset);
                /// @brief Gets the number of pixels text should be offset from the side when rendering.
                /// @return Returns a Whole representing the number of pixels text is currently being offset from the side.
                virtual Whole GetCursorOffset();

                /// @brief Aligns the text of the button.
                /// @details Default value for this is UI::Txt_Middle.
                /// @param Align The enum value representing the horizontal alignment to be set.
                virtual void HorizontallyAlign(const UI::TextHorizontalAlign& Align);
                /// @brief Aligns the text of the button.
                /// @details Default value for this is UI::Txt_Center.
                /// @param Align The enum value representing the vertical alignment to be set.
                virtual void VerticallyAlign(const UI::TextVerticalAlign& Align);

                /// @brief Enables or disables scaling the text automatically on a screen size change.
                /// @param Enable Enables or disables automatic text scaling.
                virtual void SetAutoScaleText(bool Enable);
                /// @brief Gets whether or not text from this button will automatically scale on a screen size change.
                /// @return Returns a bool indicating whether or not this button will automatically scale it's text.
                virtual bool GetAutoScaleText();
                /// @brief Updates the dimensions of this button to match those of the new screen size.
                /// @details This function is called automatically when a viewport changes in size, and shouldn't need to be called manually.
                virtual void UpdateDimensions();
                ///////////////////////////////////////////////////////////////////////////////
                // Internal Functions
                ///////////////////////////////////////
                /// @copydoc UI::BasicRenderable::_Redraw()
                virtual void _Redraw();
        };
    }//UI
}//Mezzanine

#endif
