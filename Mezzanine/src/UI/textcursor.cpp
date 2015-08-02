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

#ifndef _uitextcursor_cpp
#define _uitextcursor_cpp

#include "UI/textcursor.h"
#include "UI/textline.h"
#include "UI/character.h"
#include "MathTools/mathtools.h"
#include "stringtool.h"
#include "XML/xml.h"

namespace Mezzanine
{
    namespace UI
    {
        TextCursor::TextCursor(TextLayer* Creator) :
            IndexPosition(0),
            Layer(Creator),
            Visibility(true)
            { this->CursorColour = ColourValue::Black(); }

        TextCursor::~TextCursor()
            {  }

        ///////////////////////////////////////////////////////////////////////////////
        // Utility Methods

        void TextCursor::SetVisible(Boole Visible)
            { this->Visibility = Visible; }

        Boole TextCursor::GetVisible() const
            { return this->Visibility; }
            //{ return ( this->GetFlicker() ? ( this->FlickerTimer.GetCurrentTimeInMilliseconds() / 500 ) % 2 : this->Visibility ); }

        /*void TextCursor::SetFlicker(Boole Flicker)
        {
            if( Flicker ) this->FlickerTimer->Start();
            else this->FlickerTimer->Stop();
        }

        Boole TextCursor::GetFlicker() const
            { return !( this->FlickerTimer->IsStopped() ); }//*/

        void TextCursor::SetCursorIndex(const Integer& Index)
        {
            if( this->IndexPosition != Index ) {
                this->IndexPosition = Index;
                this->Layer->_MarkDirty();
            }
        }

        Integer TextCursor::GetCursorIndex() const
        {
            return this->IndexPosition;
        }

        void TextCursor::SetOffsetPosition(const Vector2& Offset)
        {
            TextLayer::CharIndexPair ResultPair = this->Layer->GetIndexAtOffset(Offset);
            if( ResultPair.first ) {
                this->SetCursorIndex( ResultPair.second );
            }
        }

        TextLayer::CharOffsetPair TextCursor::GetOffsetPosition() const
            { return this->Layer->GetOffsetAtIndex(this->IndexPosition); }

        Rect TextCursor::GetCursorRect() const
        {
            Rect Ret;
            if( this->IndexPosition < 0 || static_cast<Whole>( this->IndexPosition ) >= Layer->GetNumCharacters() ) {
                TextLayer::TextLineIterator Last = --(Layer->EndTextLine());

                if( (*Last)->GetNumCharacters() > 0 ) {
                    Ret.Size.Y = (*Last)->GetLineHeight();
                    Ret.Size.X = MathTools::Floor( Ret.Size.Y * 0.1 ) + 1.0;
                    Ret.Position.Y = (*Last)->GetPositionOffset();
                    Ret.Position.X = (*Last)->GetOffsetAtIndex(-1);
                }else{
                    Ret.Size.Y = this->Layer->GetDesiredLineHeight();
                    Ret.Size.X = MathTools::Floor( Ret.Size.Y * 0.1 ) + 1.0;
                    Ret.Position.Y = ( (*Last)->GetPositionOffset() + ( Ret.Size.Y * 0.5 ) );
                    Ret.Position.X = (*Last)->GetOffsetAtIndex(-1);
                }
            }else{
                Integer IndexCount = 0;

                TextLayer::TextLineIterator LineIt = Layer->BeginTextLine();
                while( LineIt != Layer->EndTextLine() && IndexCount + static_cast<Integer>( (*LineIt)->GetNumCharacters() ) < this->IndexPosition )
                {
                    IndexCount += (*LineIt)->GetNumCharacters();
                    ++LineIt;
                }

                if( (*LineIt)->GetNumCharacters() > 0 ) {
                    Ret.Size.Y = (*LineIt)->GetLineHeight();
                    Ret.Size.X = MathTools::Floor( Ret.Size.Y * 0.1 ) + 1.0;
                    Ret.Position.Y = (*LineIt)->GetPositionOffset();
                    Ret.Position.X = (*LineIt)->GetOffsetAtIndex( this->IndexPosition - IndexCount );
                }else{
                    Ret.Size.Y = this->Layer->GetDesiredLineHeight();
                    Ret.Size.X = MathTools::Floor( Ret.Size.Y * 0.1 ) + 1.0;
                    Ret.Position.Y = ( (*LineIt)->GetPositionOffset() + ( Ret.Size.Y * 0.5 ) );
                    Ret.Position.X = (*LineIt)->GetOffsetAtIndex( this->IndexPosition - IndexCount );
                }
            }
            return Ret;
        }

        void TextCursor::SetColour(const ColourValue& Colour)
        {
            if( this->CursorColour != Colour ) {
                this->CursorColour = Colour;
                this->Layer->_MarkDirty();
            }
        }

        const ColourValue& TextCursor::GetColour() const
        {
            return this->CursorColour;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Left and Right Methods

        void TextCursor::MoveCursorLeft()
        {
            if( this->IndexPosition > 0 ) {
                --(this->IndexPosition);
                this->Layer->_MarkDirty();
            }else if( this->IndexPosition == -1 ) {
                Whole NumChars = this->Layer->GetNumCharacters();
                this->IndexPosition = NumChars - 1;
                this->Layer->_MarkDirty();
            }
        }

        void TextCursor::MoveCursorRight()
        {
            Whole NumChars = this->Layer->GetNumCharacters();
            if( this->IndexPosition >= 0 && static_cast<Whole>( this->IndexPosition ) < NumChars ) {
                ++(this->IndexPosition);
                this->Layer->_MarkDirty();
            }
        }

        void TextCursor::InsertCharacterAtCursor(const UInt32 GlyphID)
        {
            Integer Added = this->Layer->InsertCharacterAtIndex(this->IndexPosition,GlyphID);
            if( this->IndexPosition >= 0 ) {
                this->IndexPosition += Added;
            }
        }

        void TextCursor::InsertCharactersAtCursor(const Char8* Characters, const UInt32 BufSize)
        {
            Integer Added = this->Layer->InsertCharactersAtIndex(this->IndexPosition,Characters,BufSize);
            if( this->IndexPosition >= 0 ) {
                this->IndexPosition += Added;
            }
        }

        void TextCursor::InsertCharactersAtCursor(const UInt32* Characters, const UInt32 BufSize)
        {
            Integer Added = this->Layer->InsertCharactersAtIndex(this->IndexPosition,Characters,BufSize);
            if( this->IndexPosition >= 0 ) {
                this->IndexPosition += Added;
            }
        }

        void TextCursor::RemoveLeftCharacter()
        {
            UInt32 NumChars = this->Layer->GetNumCharacters();
            if( NumChars > 0 ) {
                if( this->IndexPosition < 0 || static_cast<Whole>( this->IndexPosition ) > this->Layer->GetNumCharacters() ) {
                    this->Layer->RemoveCharacterAtIndex( NumChars - 1 );
                }else{
                    this->Layer->RemoveCharacterAtIndex(--(this->IndexPosition));
                }
            }
        }

        void TextCursor::RemoveRightCharacter()
        {
            Whole NumChars = this->Layer->GetNumCharacters();
            if( NumChars > 0 ) {
                if( this->IndexPosition < 0 || static_cast<Whole>( this->IndexPosition ) > NumChars ) {
                    // Nothing to be done, may fill out other logic later.
                }else{
                    this->Layer->RemoveCharacterAtIndex(this->IndexPosition);
                }
            }
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Serialization

        void TextCursor::ProtoSerialize(XML::Node& ParentNode) const
        {
            XML::Node SelfRoot = ParentNode.AppendChild( this->TextCursor::GetSerializableName() );

            if( SelfRoot.AppendAttribute("Version").SetValue("1") &&
                SelfRoot.AppendAttribute("IndexPosition").SetValue(this->IndexPosition) &&
                SelfRoot.AppendAttribute("Visibility").SetValue( ( this->Visibility ? "True" : "False" ) ) )
            {
                XML::Node CursorColourNode = SelfRoot.AppendChild("CursorColour");
                this->CursorColour.ProtoSerialize( CursorColourNode );

                return;
            }else{
                SerializeError("Create XML Attribute Values",TextCursor::GetSerializableName(),true);
            }
        }

        void TextCursor::ProtoDeSerialize(const XML::Node& SelfRoot)
        {
            XML::Attribute CurrAttrib;

            if( !SelfRoot.Empty() ) {
                if( SelfRoot.GetAttribute("Version").AsInt() == 1 ) {
                    CurrAttrib = SelfRoot.GetAttribute("IndexPosition");
                    if( !CurrAttrib.Empty() )
                        this->IndexPosition = CurrAttrib.AsWhole();

                    CurrAttrib = SelfRoot.GetAttribute("Visibility");
                    if( !CurrAttrib.Empty() )
                        this->Visibility = StringTools::ConvertToBool( CurrAttrib.AsString() );

                    XML::Node CursorColourNode = SelfRoot.GetChild("CursorColour").GetFirstChild();
                    if( !CursorColourNode.Empty() )
                        this->CursorColour.ProtoDeSerialize(CursorColourNode);
                }else{
                    MEZZ_EXCEPTION(ExceptionBase::INVALID_VERSION_EXCEPTION,"Incompatible XML Version for " + TextCursor::GetSerializableName() + ": Not Version 1.");
                }
            }else{
                MEZZ_EXCEPTION(ExceptionBase::II_IDENTITY_NOT_FOUND_EXCEPTION,TextCursor::GetSerializableName() + " was not found in the provided XML node, which was expected.");
            }
        }

        String TextCursor::GetSerializableName()
            { return "TextCursor"; }
    }//UI
}//Mezzanine

#endif
