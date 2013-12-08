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

#ifndef _uisinglelinetextlayer_h
#define _uisinglelinetextlayer_h

#include "UI/textlayer.h"
#include "UI/uienumerations.h"

namespace Mezzanine
{
    namespace UI
    {
        ///////////////////////////////////////////////////////////////////////////////
        /// @brief This is a render layer specializing in single-line text.
        /// @details
        ///////////////////////////////////////
        class MEZZ_LIB SingleLineTextLayer : public TextLayer
        {
        protected:
            /// @internal
            /// @brief The index at which this text layer will start rendering characters.
            UInt32 StartIndex;
            /// @copydoc TextLayer::PopulateTextLinesImpl()
            virtual void PopulateTextLinesImpl();
            /// @copydoc TextLayer::GetCharacterIndexAtPositionImpl(const Vector2& Offset)
            virtual CharIndexPair GetIndexAtOffsetImpl(const Vector2& Offset);
            /// @copydoc TextLayer::GetOffsetAtIndex(const Integer Index)
            virtual CharOffsetPair GetOffsetAtIndexImpl(const Integer Index);
            /// @brief Gets a pointer to the only textline in this layer.
            virtual TextLine* GetLine() const;
        public:
            /// @brief No-Font constructor.
            /// @param ParentRenderable The renderable that created this layer.
            SingleLineTextLayer(QuadRenderable* ParentRenderable);
            /// @brief Non-Scaling constructor.
            /// @param FontName The name of the font to use for this layer.
            /// @param ParentRenderable The renderable that created this layer.
            SingleLineTextLayer(const String& FontName, QuadRenderable* ParentRenderable);
            /// @brief Text-Scaling constructor.
            /// @param LineHeight The height of the text to display relative to the parent renderable's height.
            /// @param ParentRenderable The renderable that created this layer.
            SingleLineTextLayer(const Real& LineHeight, QuadRenderable* ParentRenderable);
            /// @brief Class destructor.
            virtual ~SingleLineTextLayer();

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            /// @copydoc RenderLayer::GetLayerType() const
            virtual RenderLayer::RenderLayerType GetLayerType() const;

            ///////////////////////////////////////////////////////////////////////////////
            // Text Methods

            /// @brief Aligns the text of the caption on the X axis.
            /// @details Default value for this is UI::Txt_Center.
            /// @param Align The enum value representing the vertical alignment to be set.
            virtual void HorizontallyAlign(const UI::LinearAlignment Align);
            /// @brief Aligns the text of the caption on the Y axis.
            /// @details Default value for this is UI::Txt_Center.
            /// @param Align The enum value representing the vertical alignment to be set.
            virtual void VerticallyAlign(const UI::LinearAlignment Align);

            /// @brief Sets the index from the beginning at which characters will start to be rendered.
            /// @param Index The index of the character to start rendering at.
            virtual void SetStartIndex(const UInt32& Index);
            /// @brief Gets the set index for the character that's at the start of the characters being rendered.
            /// @return Returns a UInt32 that is the index of the first character in the rendering sequence.
            virtual UInt32 GetStartIndex() const;

            ///////////////////////////////////////////////////////////////////////////////
            // Serialization

            /// @copydoc SimpleRenderer::ProtoSerializeProperties(XML::Node&) const
            virtual void ProtoSerializeProperties(XML::Node& SelfRoot) const;
            /// @copydoc SimpleRenderer::ProtoDeSerializeProperties(const XML::Node&)
            virtual void ProtoDeSerializeProperties(const XML::Node& SelfRoot);

            /// @copydoc SimpleRenderer::GetDerivedSerializableName() const
            virtual String GetDerivedSerializableName() const;
            /// @copydoc SimpleRenderer::GetSerializableName()
            static String GetSerializableName();
        };//SingleLineTextLayer
    }//UI
}//Mezzanine

#endif
