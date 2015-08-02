// © Copyright 2010 - 2015 BlackTopp Studios Inc.
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
#ifndef _uisinglelinetextlayer_cpp
#define _uisinglelinetextlayer_cpp

#include "UI/singlelinetextlayer.h"
#include "UI/quadrenderable.h"
#include "UI/screen.h"
#include "UI/glyph.h"
#include "UI/textline.h"
#include "UI/character.h"
#include "UI/uimanager.h"
#include "MathTools/mathtools.h"

namespace Mezzanine
{
    namespace UI
    {
        SingleLineTextLayer::SingleLineTextLayer(QuadRenderable* ParentRenderable) :
            TextLayer(ParentRenderable),
            StartIndex(0)
            {  }

        SingleLineTextLayer::SingleLineTextLayer(const String& FontName, QuadRenderable* ParentRenderable) :
            TextLayer(FontName,ParentRenderable),
            StartIndex(0)
            {  }

        SingleLineTextLayer::SingleLineTextLayer(const Real& LineHeight, QuadRenderable* ParentRenderable) :
            TextLayer(LineHeight,ParentRenderable),
            StartIndex(0)
            {  }

        SingleLineTextLayer::~SingleLineTextLayer()
            {  }

        void SingleLineTextLayer::PopulateTextLinesImpl(const Real MaxWidth)
        {
            Integer Count = 0;
            TextLine* Caption = this->GetLine();
            if( HorizontalOrder == UI::TO_Left_To_Right )
            {
                for( CharacterIterator CharIt = this->Characters.begin() ; CharIt != this->Characters.end() ; ++CharIt )
                {
                    if( Count < StartIndex )
                        continue;

                    if( !Caption->AppendCharacter((*CharIt),MaxWidth) )
                        break;
                }
            }
            else if( HorizontalOrder == UI::TO_Right_To_Left )
            {
                for( ReverseCharacterIterator CharIt = this->Characters.rbegin() ; CharIt != this->Characters.rend() ; ++CharIt )
                {
                    if( Count < StartIndex )
                        continue;

                    if( !Caption->AppendCharacter((*CharIt),MaxWidth) )
                        break;
                }
            }
        }

        TextLayer::CharIndexPair SingleLineTextLayer::GetIndexAtOffsetImpl(const Vector2& Offset)
        {
            TextLine* Line = this->GetLine();

            if( Offset.Y < Line->GetPositionOffset() - Line->GetLineHeight() || Offset.Y > Line->GetPositionOffset() )
                return CharIndexPair(false,0);

            return CharIndexPair(true,this->StartIndex + Line->GetIndexAtOffset(Offset.X));
        }

        TextLayer::CharOffsetPair SingleLineTextLayer::GetOffsetAtIndexImpl(const Integer Index)
        {
            TextLine* Line = this->GetLine();

            if( Index < this->StartIndex || static_cast<Whole>( Index ) > this->StartIndex + Line->GetNumCharacters() )
                return CharOffsetPair(false,Vector2(0,0));

            return CharOffsetPair(true,Vector2(Line->GetOffsetAtIndex(Index - this->StartIndex),Line->GetPositionOffset()));
        }

        TextLine* SingleLineTextLayer::GetLine() const
            { return this->TextLines.at(0); }

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        UI::RenderLayerType SingleLineTextLayer::GetLayerType() const
            { return UI::RLT_SingleLineText; }

        ///////////////////////////////////////////////////////////////////////////////
        // Text Methods

        void SingleLineTextLayer::HorizontallyAlign(const UI::LinearAlignment Align)
            { this->SetTextLineHorizontalAlignment(Align); }

        void SingleLineTextLayer::VerticallyAlign(const UI::LinearAlignment Align)
            { this->SetTextLineVerticalAlignment(Align); }

        void SingleLineTextLayer::SetStartIndex(const Integer& Index)
            { this->StartIndex = Index; }

        Integer SingleLineTextLayer::GetStartIndex() const
            { return this->StartIndex; }

        ///////////////////////////////////////////////////////////////////////////////
        // Serialization

        void SingleLineTextLayer::ProtoSerializeProperties(XML::Node& SelfRoot) const
        {
            TextLayer::ProtoSerializeProperties(SelfRoot);
            XML::Node PropertiesNode = SelfRoot.AppendChild( SingleLineTextLayer::GetSerializableName() + "Properties" );

            if( PropertiesNode.AppendAttribute("Version").SetValue("1") &&
                PropertiesNode.AppendAttribute("StartIndex").SetValue(this->StartIndex) )
            {
                return;
            }else{
                SerializeError("Create XML Attribute Values",SingleLineTextLayer::GetSerializableName() + "Properties",true);
            }
        }

        void SingleLineTextLayer::ProtoDeSerializeProperties(const XML::Node& SelfRoot)
        {
            TextLayer::ProtoDeSerializeProperties(SelfRoot);
            XML::Attribute CurrAttrib;
            XML::Node PropertiesNode = SelfRoot.GetChild( SingleLineTextLayer::GetSerializableName() + "Properties" );

            if( !PropertiesNode.Empty() ) {
                if(PropertiesNode.GetAttribute("Version").AsInt() == 1) {
                    CurrAttrib = PropertiesNode.GetAttribute("StartIndex");
                    if( !CurrAttrib.Empty() )
                        this->StartIndex = CurrAttrib.AsInt();
                }else{
                    MEZZ_EXCEPTION(ExceptionBase::INVALID_VERSION_EXCEPTION,"Incompatible XML Version for " + (SingleLineTextLayer::GetSerializableName() + "Properties") + ": Not Version 1.");
                }
            }else{
                MEZZ_EXCEPTION(ExceptionBase::II_IDENTITY_NOT_FOUND_EXCEPTION,SingleLineTextLayer::GetSerializableName() + "Properties" + " was not found in the provided XML node, which was expected.");
            }
        }

        String SingleLineTextLayer::GetDerivedSerializableName() const
            { return this->SingleLineTextLayer::GetSerializableName(); }

        String SingleLineTextLayer::GetSerializableName()
            { return "SingleLineTextLayer"; }
    }//UI
}//Mezzanine

#endif
