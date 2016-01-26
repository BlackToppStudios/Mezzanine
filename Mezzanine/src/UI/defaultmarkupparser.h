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
#ifndef _uidefaultmarkupparser_h
#define _uidefaultmarkupparser_h

#include "UI/markupparser.h"

namespace Mezzanine
{
    namespace UI
    {
        ///////////////////////////////////////////////////////////////////////////////
        /// @brief This class implements a character colour changing tag.
        /// @details This tag supports both short and long notation.  For example:
        /// "[Colour=FFFFFFFF]" is valid and equivalent to "[Colour Value=FFFFFFFF]".  "Value" is the only
        /// parameter name supported by this class/tag.  The actual value of "Value" is a colour expressed in
        /// Hexadecimal.  Each two characters are one colour, in the following order: Red, Green, Blue, and
        /// Alpha.  The alpha portion does not need to be defined and in it's absence an alpha value of 1.0 will
        /// be filled in.  If the parameter is less than 6 characters long this will fail and "Process" will
        /// return false.
        ///////////////////////////////////////
        class MEZZ_LIB DefaultColourTag : public MarkupTag
        {
        public:
            /// @brief Class constructor.
            DefaultColourTag() {}
            /// @brief Class destructor.
            virtual ~DefaultColourTag() {}

            ///////////////////////////////////////////////////////////////////////////////
            // Public Data Members

            /// @brief The name of this tag as it should appear in markup.
            static const String TagName;

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            /// @copydoc MarkupTag::GetName() const
            virtual const String& GetName() const
                { return DefaultColourTag::TagName; }
            /// @copydoc MarkupTag::IsRangeTag() const
            virtual Boole IsRangeTag() const
                { return true; }

            /// @copydoc MarkupTag::Process(NameValuePairMap&, CharacterTraits&, TextLayer*) const
            virtual ProcessResult Process(NameValuePairMap& Params, CharacterTraits& Traits, TextLayer* Layer) const;
        };//DefaultColourTag

        ///////////////////////////////////////////////////////////////////////////////
        /// @brief This class implements a character font changing tag.
        /// @details This tag only supports long notation.  Example: "[Font Name=Ubuntu]".  A size can also be specified
        /// such as in this example: "[Font Name=Ubuntu Size=12]".  The name is the same name as specified in the texture
        /// atlas the calling layer has set as it's primary.  The size is the vertical height in pixels (before scaling)
        /// the glyphs should be rendered as.  Glyphs with their height set have their aspect ratio's preserved.  @n @n
        /// Currently the size parameter is not implemented.
        ///////////////////////////////////////
        class MEZZ_LIB DefaultFontTag : public MarkupTag
        {
        public:
            /// @brief Class constructor.
            DefaultFontTag() {}
            /// @brief Class destructor.
            virtual ~DefaultFontTag() {}

            ///////////////////////////////////////////////////////////////////////////////
            // Public Data Members

            /// @brief The name of this tag as it should appear in markup.
            static const String TagName;

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            /// @copydoc MarkupTag::GetName() const
            virtual const String& GetName() const
                { return DefaultFontTag::TagName; }
            /// @copydoc MarkupTag::IsRangeTag() const
            virtual Boole IsRangeTag() const
                { return true; }

            /// @copydoc MarkupTag::Process(NameValuePairMap&, CharacterTraits&, TextLayer*) const
            virtual ProcessResult Process(NameValuePairMap& Params, CharacterTraits& Traits, TextLayer* Layer) const;
        };//DefaultFontTag

        ///////////////////////////////////////////////////////////////////////////////
        /// @brief This class implements a sprite inserting tag.
        /// @details This tag only supports long notation.  Example: "[Sprite Name=Background]".  A size can also be specified
        /// such as in this example: "[Sprite Name=Background Size=12x12]".  The name is the same name as specified in the texture
        /// atlas the calling layer has set as it's primary.  The size is the actual dimensions to have set (before scaling) for
        /// the character that will be generated using the named sprite.
        ///////////////////////////////////////
        class MEZZ_LIB DefaultSpriteTag : public MarkupTag
        {
        public:
            /// @brief Class constructor.
            DefaultSpriteTag() {}
            /// @brief Class destructor.
            virtual ~DefaultSpriteTag() {}

            ///////////////////////////////////////////////////////////////////////////////
            // Public Data Members

            /// @brief The name of this tag as it should appear in markup.
            static const String TagName;

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            /// @copydoc MarkupTag::GetName() const
            virtual const String& GetName() const
                { return DefaultSpriteTag::TagName; }
            /// @copydoc MarkupTag::IsRangeTag() const
            virtual Boole IsRangeTag() const
                { return false; }

            /// @copydoc MarkupTag::Process(NameValuePairMap&, CharacterTraits&, TextLayer*) const
            virtual ProcessResult Process(NameValuePairMap& Params, CharacterTraits& Traits, TextLayer* Layer) const;
        };//DefaultSpriteTag

        ///////////////////////////////////////////////////////////////////////////////
        /// @brief This class implements the default set of tags used by the UI markup system.
        /// @details
        ///////////////////////////////////////
        class MEZZ_LIB DefaultMarkupParser : public MarkupParser
        {
        protected:
            /// @copydoc MarkupParser::Initialize()
            virtual void Initialize();
        public:
            /// @brief Class constructor.
            DefaultMarkupParser();
            /// @brief Class destructor.
            virtual ~DefaultMarkupParser();

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            /// @copydoc MarkupParser::GetName() const
            virtual String GetName() const;
            /// @copydoc MarkupParser::GetMarkupTagStart() const
            virtual Char8 GetMarkupTagStart() const;
            /// @copydoc MarkupParser::GetMarkupTagEnd() const
            virtual Char8 GetMarkupTagEnd() const;
        };//DefaultMarkupParser
    }//UI
}//Mezzanine

#endif
