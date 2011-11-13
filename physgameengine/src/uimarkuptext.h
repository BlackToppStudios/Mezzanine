//© Copyright 2010 - 2011 BlackTopp Studios Inc.
/* This file is part of The PhysGame Engine.

    The PhysGame Engine is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The PhysGame Engine is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with The PhysGame Engine.  If not, see <http://www.gnu.org/licenses/>.
*/
/* The original authors have included a copy of the license specified above in the
   'Docs' folder. See 'gpl.txt'
*/
/* We welcome the use of The PhysGame anyone, including companies who wish to
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
#include "uibasicrenderable.h"

namespace Gorilla
{
    class MarkupText;
}

namespace phys
{
    class UIManager;
    namespace UI
    {
        class Layer;
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
        class PHYS_LIB MarkupText : public BasicRenderable
        {
            protected:
                Gorilla::MarkupText* GMarkup;
                Whole Glyphs;
                bool AutoScaleText;
                Real RelLineHeight;
                void ConstructMarkupText(const RenderableRect& Rect, const Whole& Glyph, const String& Text);
            public:
                /// @brief Internal constructor.
                /// @details The constructor will ignore the size portion of the Rect passed in, since Markup Texts don't have a default size.
                /// @param name The name of this markup text.
                /// @param Rect The Rect representing the position and size of the markup text.
                /// @param Glyph One of the glyphs specified in your gorilla file.  Must be valid.
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
                /// @brief Sets the visibility of this markup text.
                /// @param Visible Bool determining whether or not this markup text should be visible.
                virtual void SetVisible(bool Visible);
                /// @brief Gets the visibility of this markup text.
                /// @return Returns a bool representing the visibility of this markup text.
                virtual bool IsVisible() const;
                /// @brief Forces this markup text to be shown.
                virtual void Show();
                /// @brief Forces this markup text to hide.
                virtual void Hide();
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
                /// @details The glyph index is defined in your gorilla file.  This class can change which
                /// glyph is uses with it's markup language.  This simply defines which to use when one isn't
                /// specified.
                /// @param DefaultGlyphIndex The index of the glyph to use with this markup text.
                virtual void SetDefaultGlyphIndex(const Whole& DefaultGlyphIndex);
                /// @brief Gets the Default glyph index in use by this markup text.
                /// @details The glyph index is defined in your gorilla file.
                /// @return Returns a Whole representing the index of the glyph in use by this markup text.
                virtual Whole GetDefaultGlyphIndex();
                /*/// @brief Aligns the text of the markup text.
                /// @param Align The enum value representing the horizontal alignment to be set.
                void HorizontallyAlign(UI::TextHorizontalAlign Align);
                /// @brief Aligns the text of the markup text.
                /// @param Align The enum value representing the vertical alignment to be set.
                void VerticallyAlign(UI::TextVerticalAlign Align);*/
                /// @brief Sets the background colour of the caption.
                /// @param Colour A colour value representing the colour to be set.
                virtual void SetBackgroundColour(const ColourValue& Colour);
                /// @brief Sets the background image(if provided in the atlas) of the caption.
                /// @param Name The name of the sprite to set as the background.
                virtual void SetBackgroundSprite(const String& Name);
                /// @brief Sets the background image(if provided in the atlas) of the caption from another atlas then the one currently set.
                /// @param Name The name of the sprite to set as the background.
                /// @param Atlas The Atlas to load the sprite from.
                virtual void SetBackgroundSprite(const String& Name, const String& Atlas);
                /// @brief Sets the relative top left position of this markup text.
                /// @param Position A Vector2 representing the location of this markup text.
                virtual void SetPosition(const Vector2& Position);
                /// @brief Gets the relative top left position of this markup text.
                /// @return Returns a Vector2 representing the location of this markup text.
                virtual Vector2 GetPosition() const;
                /// @brief Sets the top left position of this markup text in pixels.
                /// @param Position A Vector2 representing the location of this markup text.
                virtual void SetActualPosition(const Vector2& Position);
                /// @brief Gets the top left position of this markup text in pixels.
                /// @return Returns a Vector2 representing the location of this markup text.
                virtual Vector2 GetActualPosition() const;
                /// @brief Sets the relative size of this markup text.
                /// @param Size A vector2 representing the size of this markup text.
                virtual void SetSize(const Vector2& Size);
                /// @brief Gets the relative size of this markup text.
                /// @return Returns a vector2 representing the size of this markup text.
                virtual Vector2 GetSize() const;
                /// @brief Sets the size of this markup text in pixels.
                /// @param Size A vector2 representing the size of this markup text.
                virtual void SetActualSize(const Vector2& Size);
                /// @brief Gets the size of this markup text in pixels.
                /// @return Returns a vector2 representing the size of this markup text.
                virtual Vector2 GetActualSize() const;
                /// @brief Sets the priority this markup text should be rendered with.
                /// @details The default value for this is Medium.
                /// @param Priority The priority level to be used when rendering this markup text.
                virtual void SetRenderPriority(const UI::RenderPriority& Priority);
                /// @brief Gets the priority this markup text should be rendered with.
                /// @return Returns an enum value representing this markup text's priority level.
                virtual UI::RenderPriority GetRenderPriority() const;
                /// @brief Sets the Atlas to be assumed when one isn't provided for atlas related tasks.
                /// @param Atlas The name of the atlas to be used.
                virtual void SetPrimaryAtlas(const String& Atlas);
                /// @brief Gets the currently set primary atlas.
                /// @return Returns a string containing the name of the primary atlas that is set.
                virtual String GetPrimaryAtlas() const;
                /// @brief Enables or disables scaling the text automatically on a screen size change.
                /// @param Enable Enables or disables automatic text scaling.
                virtual void SetAutoScaleText(bool Enable);
                /// @brief Gets whether or not text from this caption will automatically scale on a screen size change.
                /// @return Returns a bool indicating whether or not this caption will automatically scale it's text.
                virtual bool GetAutoScaleText();
                /// @brief Updates the dimensions of this markup text to match those of the new screen size.
                /// @details This function is called automatically when a viewport changes in size, and shouldn't need to be called manually.
                virtual void UpdateDimensions();
        };
    }//UI
}//phys

#endif
