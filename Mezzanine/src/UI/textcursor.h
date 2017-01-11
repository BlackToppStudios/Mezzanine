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
#ifndef _uitextcursor_h
#define _uitextcursor_h

#include "colourvalue.h"
#include "timer.h"
#include "UI/rect.h"
#include "UI/textlayer.h"

namespace Mezzanine
{
    namespace UI
    {
        class TextLayer;
        class Glyph;
        class Sprite;
        ///////////////////////////////////////////////////////////////////////////////
        /// @brief Class for encapsulating the functionality of the text cursor/carat navigation in text layers.
        /// @details
        ///////////////////////////////////////
        class TextCursor
        {
        protected:
            /// @internal
            /// @brief The colour to be given to the TextCursor.
            ColourValue CursorColour;
            /*/// @internal
            /// @brief The timer used for cursor flickering.
            Timer FlickerTimer;// */
            /// @internal
            /// @brief The index of the character this TextCursor is to the left of.
            Integer IndexPosition;
            /// @internal
            /// @brief The layer this TextCursor belongs to.
            TextLayer* Layer;
            /// @internal
            /// @brief Stores the cursors current visibility state.
            Boole Visibility;
        public:
            /// @brief Class constructor.
            /// @param Creator The layer that this TextCursor belongs to.
            TextCursor(TextLayer* Creator);
            /// @brief Class destructor.
            virtual ~TextCursor();

            ///////////////////////////////////////////////////////////////////////////////
            // Utility Methods

            /// @brief Sets the visibility of this cursor.
            /// @param Visible True to enable rendering of this cursor, false otherwise.
            virtual void SetVisible(Boole Visible);
            /// @brief Gets the visibility of this cursor.
            /// @return Returns true if this cursor is being rendered along with it's parent TextLayer, false otherwise.
            virtual Boole GetVisible() const;

            /*/// @brief Sets whether or not this cursor will flicker while being rendered.
            /// @note The flicker will occur in half second intervals.
            /// @param Flicker True to enable cursor flickering, false otherwise.
            virtual void SetFlicker(Boole Flicker);
            /// @brief Gets whether or not this cursor will flicker while being rendered.
            /// @return Returns true if this cursor is currently set to flicker every half second.
            virtual Boole GetFlicker() const;// */

            /// @brief Sets the index position of this cursor.
            /// @param Index The index indicating the position of this cursor among the characters in the parent layer.
            virtual void SetCursorIndex(const Integer& Index);
            /// @brief Gets the index position of this cursor.
            /// @return Returns an Integer representing the index position of this cursor.
            virtual Integer GetCursorIndex() const;

            /// @brief Sets the offset position of this cursor to the nearest appropriate point.
            /// @param Offset The position from the top left corner of the parent layer where the bottom left corner of this cursor is to be placed.
            virtual void SetOffsetPosition(const Vector2& Offset);
            /// @brief Gets the current offset position of this cursor.
            /// @return Returns a Vector2 containing the position of this cursor relative to the parent layer.
            virtual TextLayer::CharOffsetPair GetOffsetPosition() const;
            /// @brief Gets a rect representing this cursors dimentions.
            /// @return Returns a Rect containing the position and size of this TextCursor.
            virtual Rect GetCursorRect() const;

            /// @brief Sets the colour that the Text Cursor should be rendered as.
            /// @param Colour The colour to use when rendering the Text Cursor.
            virtual void SetColour(const ColourValue& Colour);
            /// @brief Gets the colour that the Text Cursor will be rendered as.
            /// @return Returns a const ColourValue reference of the Text Cursors colour.
            virtual const ColourValue& GetColour() const;

            ///////////////////////////////////////////////////////////////////////////////
            // Left and Right Methods

            /// @brief Decrements this cursors index position, moving it to the left.
            virtual void MoveCursorLeft();
            /// @brief Increments this cursors index position, moving it to the right.
            virtual void MoveCursorRight();

            /// @brief Creates a character from a Glyph ID and inserts it at the cursor position.
            /// @param GlyphID The ID of the Glyph to be inserted.
            virtual void InsertCharacterAtCursor(const UInt32 GlyphID);
            /// @brief Creates a series of characters from a UTF-8 encoded string to be inserted at the cursor position.
            /// @param Characters An array of Char8's encoded in UTF-8 to be inserted.
            /// @param BufSize The size of the array of Char8's passed in.
            virtual void InsertCharactersAtCursor(const Char8* Characters, const UInt32 BufSize);
            /// @brief Creates a series of characters from a UTF-32 encoded string to be inserted at the cursor position.
            /// @param Characters An array of UInt32's encoded in UTF-32 to be inserted.
            /// @param BufSize The size of the array of Char8's passed in.
            virtual void InsertCharactersAtCursor(const UInt32* Characters, const UInt32 BufSize);
            /// @brief Removes the character to the left (and decrements the index position) of this cursor.
            virtual void RemoveLeftCharacter();
            /// @brief Removes the character to the right of this cursor.
            virtual void RemoveRightCharacter();

            ///////////////////////////////////////////////////////////////////////////////
            // Serialization

            /// @brief Convert this class to an XML::Node ready for serialization.
            /// @param ParentNode The point in the XML hierarchy that this renderable should be appended to.
            virtual void ProtoSerialize(XML::Node& ParentNode) const;
            /// @brief Take the data stored in an XML Node and overwrite this object with it.
            /// @param SelfRoot An XML::Node containing the data to populate the new instance with.
            virtual void ProtoDeSerialize(const XML::Node& SelfRoot);

            /// @brief Get the name of the the XML tag the Renderable class will leave behind as its instances are serialized.
            /// @return A string containing the name of this class.
            static String GetSerializableName();
        };//TextCursor
    }//UI
}//Mezzanine

#endif
