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
#ifndef _uimarkupparser_cpp
#define _uimarkupparser_cpp

#include "UI/markupparser.h"
#include "UI/character.h"
#include "UI/glyph.h"
#include "UI/sprite.h"
#include "UI/texttoken.h"

#include "unicode.h"
#include "exception.h"

namespace Mezzanine
{
    namespace UI
    {
        ///////////////////////////////////////////////////////////////////////////////
        // MarkupParser Methods

        MarkupParser::MarkupParser()
            {  }

        MarkupParser::~MarkupParser()
            {  }

        TextToken* MarkupParser::CreateTextToken(const String& Text) const
        {
            TextToken* NewTextToken = new TextToken(Text,TextToken::TT_Text);
            return NewTextToken;
        }

        TextToken* MarkupParser::CreateTagToken(const String& Text) const
        {
            // Set up our return
            TextToken* NewTagToken = NULL;
            const Integer TextSize = Text.size();
            // Do some string processing to get some more data
            UInt32 SlashPos = Text.find_first_of('/');
            if( SlashPos == 1 /* && TextSize > 3 */ ) {
                // We now know we have a valid end tag
                // Get the name
                String TagName = Text.substr(SlashPos + 1,Text.find_first_of(this->GetMarkupTagEnd()) - SlashPos - 1);
                NewTagToken = new RangeTagToken(Text,TagName,TextToken::TT_RangeTagEnd);
            }else if( SlashPos == String::npos ) {
                // No slashes, so this is a start tag or insert tag
                // Setup the data we'll be using to parse the relevant data
                Int32 Position = 0;
                String SeparatorChars(" =");
                SeparatorChars.append(1,this->GetMarkupTagEnd());

                // Get the name
                Position = Text.find_first_of(SeparatorChars);
                String TagName = Text.substr(1,Position - 1);

                // Since we have the tags name, found out if it is a range tag, insert tag, or invalid
                ConstTagIterator TagIt = this->Tags.find(TagName);
                if( TagIt != this->Tags.end() ) {
                    // We got a valid tag, but is it a range or insert tag?
                    if( (*TagIt).second->IsRangeTag() ) {
                        NewTagToken = new RangeTagToken(Text,TagName,TextToken::TT_RangeTagStart);
                    }else{
                        NewTagToken = new InsertTagToken(Text,TagName);
                    }
                }else{
                    // Tag doesn't exist for this parser, mark it as an error
                    NewTagToken = new TextToken(Text,TextToken::TT_Error);
                }

                if( NewTagToken->GetTokenType() != TextToken::TT_Error )
                {
                    TagToken* CastedTag = static_cast<TagToken*>( NewTagToken );
                    NameValuePairMap TagParams;
                    if( Text[Position] == '=' ) {
                        TagParams["Value"] = Text.substr(Position + 1,TextSize - Position - 1);
                    }else if( Text[Position] == ' ' ) {
                        // Get the parameters
                        String ParamEndChars(" ");
                        ParamEndChars.append(1,this->GetMarkupTagEnd());
                        while( Position < TextSize - 1 )
                        {
                            String ParamName, ParamValue;

                            UInt32 SeparatorPos = Text.find_first_of('=');
                            if( SeparatorPos != String::npos ) ParamName = Text.substr(Position + 1,SeparatorPos - Position - 1);
                            else break;

                            UInt32 PairEndPos = Text.find_first_of(ParamEndChars);
                            if( SeparatorPos != String::npos ) ParamValue = Text.substr(SeparatorPos + 1,PairEndPos - SeparatorPos - 1);
                            else break;

                            TagParams[ParamName] = ParamValue;
                            Position = PairEndPos;
                        }
                    }
                    // Populate the generated parameters
                    CastedTag->Params.swap(TagParams);
                }
            }else{
                // If we're here, there is a slash not where it's supposed to be which we won't support, so error it
                NewTagToken = new TextToken(Text,TextToken::TT_Error);
            }
            // Return the result
            return NewTagToken;
        }

        void MarkupParser::GenerateCharactersFromToken(const TextToken* Token, TextLayer* Layer, const CharacterTraits& Traits, CharacterContainer& Characters) const
        {
            // Setup our data
            Int32 Position = 0;
            Int32 BytesAdvance = 0;
            const char* StrBuf = Token->Text.data();
            // Get generat'in
            while( static_cast<UInt32>(Position) < Token->Text.size() )
            {
                Int32 GlyphID = Unicode::GetIntFromCharacter(BytesAdvance,StrBuf + Position);
                if( GlyphID == -1 )
                {
                    MEZZ_EXCEPTION(Exception::PARAMETERS_EXCEPTION,"Attempting to parse non-UTF8 encoded markup text.  Encode in UTF8 and try again.");
                }

                Glyph* TheGlyph = Traits.CharFont->GetGlyph(static_cast<UInt32>(GlyphID));
                if( TheGlyph == NULL )
                {
                    StringStream ExceptionStream;
                    ExceptionStream << "Attempting to parse unknown Glyph ID: " << GlyphID << ".  Provided font (" << Traits.CharFont->GetName() << ") does not contain that Glyph.";
                    MEZZ_EXCEPTION(Exception::PARAMETERS_EXCEPTION,ExceptionStream.str());
                }
                /// @todo As the CharacterTraits class expands, so does this logic.
                Character* NewChar = new Character(TheGlyph,Layer);
                NewChar->SetCharacterColour(Traits.CharColour);
                //NewChar->SetHighlightColour(Traits.HLCharColour);
                Characters.push_back(NewChar);

                Position += BytesAdvance;
            }
        }

        void MarkupParser::RegenerateTraits(CharacterTraits& Traits, const TagVector& ActiveTags, TextLayer* Layer) const
        {
            for( ConstTagVecIterator TagIt = ActiveTags.begin() ; TagIt != ActiveTags.end() ; ++TagIt )
            {
                // Ignore the return
                (*TagIt).second->Process( (*TagIt).first->Params, Traits, Layer );
            }
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Parsing Methods

        MarkupParser::CharacterContainer MarkupParser::Parse(const String& Source, const CharacterTraits& InitialTraits, TextLayer* CallingLayer) const
        {
            // Tokenize our string
            TokenString* Tokens = this->Lex(Source);

            // Do our processing
            CharacterContainer GeneratedCharacters = this->ParseTextTokens(Tokens,InitialTraits,CallingLayer);

            // Cleanup and return
            delete Tokens;
            Tokens = NULL;

            return GeneratedCharacters;
        }

        MarkupParser::CharacterContainer MarkupParser::ParseTextTokens(TokenString* Tokens, const CharacterTraits& InitialTraits, TextLayer* CallingLayer) const
        {
            // Character Data
            CharacterContainer GeneratedCharacters;
            CharacterTraits CurrentTraits = InitialTraits;

            // Setup our container that will cache the active tags for our position
            TagVector ActiveTags;

            // Lets process some tokens
            for( TokenString::TokenIterator TokIt = Tokens->BeginToken() ; TokIt != Tokens->EndToken() ; ++TokIt )
            {
                CharacterContainer CharacterSegment;
                switch( (*TokIt)->Type )
                {
                    // If this is an error token, just treat it as normal text
                    case TextToken::TT_Text:
                    case TextToken::TT_Error:
                    case TextToken::TT_TagInvalid:
                    {
                        // Generate and append
                        TextToken* CurrToken = (*TokIt);
                        this->GenerateCharactersFromToken( (*TokIt),CallingLayer,CurrentTraits,CharacterSegment );
                        GeneratedCharacters.splice(GeneratedCharacters.end(),CharacterSegment);
                        CurrToken->RenderSize = CharacterSegment.size();
                        break;
                    }
                    case TextToken::TT_RangeTagStart:
                    {
                        RangeTagToken* CurrToken = static_cast<RangeTagToken*>( (*TokIt) );
                        // See if this tag is supported by this implementation
                        ConstTagIterator TagIt = this->Tags.find(CurrToken->GetTagName());
                        if( TagIt != Tags.end() ) {
                            // If it is, verify it applies to a range
                            if( (*TagIt).second->IsRangeTag() ) {
                                // Ensure it has a partner
                                if( CurrToken->PartnerTag != NULL ) {
                                    // If it's a range token and has a partner then we should be good
                                    MarkupTag::ProcessResult Res = (*TagIt).second->Process(CurrToken->Params,CurrentTraits,CallingLayer);
                                    if( Res.first ) {
                                        if( Res.second ) {
                                            // This shouldn't really ever execute
                                            CharacterSegment.push_back(Res.second);
                                        }

                                        ActiveTags.push_back( TokenTagPair( CurrToken,(*TagIt).second ) );
                                    }else{
                                        this->GenerateCharactersFromToken( CurrToken,CallingLayer,CurrentTraits,CharacterSegment );
                                    }
                                }else{
                                    this->GenerateCharactersFromToken( CurrToken,CallingLayer,CurrentTraits,CharacterSegment );
                                }
                            }else{
                                this->GenerateCharactersFromToken( CurrToken,CallingLayer,CurrentTraits,CharacterSegment );
                            }
                        }else{
                            this->GenerateCharactersFromToken( CurrToken,CallingLayer,CurrentTraits,CharacterSegment );
                        }

                        // If anything was generated, append it
                        if( !CharacterSegment.empty() )
                        {
                            GeneratedCharacters.splice(GeneratedCharacters.end(),CharacterSegment);
                            CurrToken->RenderSize = CharacterSegment.size();
                        }

                        break;
                    }
                    case TextToken::TT_RangeTagEnd:
                    {
                        RangeTagToken* CurrToken = static_cast<RangeTagToken*>( (*TokIt) );
                        if( CurrToken->PartnerTag != NULL ) {
                            // If the tag has a partner, then it's time it's effects come to an end
                            for( TagVector::reverse_iterator TagIt = ActiveTags.rbegin() ; TagIt != ActiveTags.rend() ; ++TagIt )
                            {
                                if( (*TagIt).first == CurrToken->PartnerTag )
                                    // erase only accepts normal iterators which reverse iterators are offset from, adjust before passing in
                                    ActiveTags.erase( --(TagIt.base()) );
                            }
                            // Update our traits based on remaining active tags
                            CurrentTraits = InitialTraits;
                            this->RegenerateTraits(CurrentTraits,ActiveTags,CallingLayer);
                        }else{
                            // End tags always belong to a range, without a partner tag this is just an error
                            this->GenerateCharactersFromToken( CurrToken,CallingLayer,CurrentTraits,CharacterSegment );
                            GeneratedCharacters.splice(GeneratedCharacters.end(),CharacterSegment);
                            CurrToken->RenderSize = CharacterSegment.size();
                        }
                        break;
                    }
                    case TextToken::TT_InsertTag:
                    {
                        InsertTagToken* CurrToken = static_cast<InsertTagToken*>( (*TokIt) );
                        // See if this tag is supported by this implementation
                        ConstTagIterator TagIt = this->Tags.find(CurrToken->GetTagName());
                        if( TagIt != Tags.end() ) {
                            MarkupTag::ProcessResult Res = (*TagIt).second->Process(CurrToken->Params,CurrentTraits,CallingLayer);
                            if( Res.first ) {
                                if( Res.second ) {
                                    CharacterSegment.push_back(Res.second);
                                }
                            }else{
                                this->GenerateCharactersFromToken( CurrToken,CallingLayer,CurrentTraits,CharacterSegment );
                            }
                        }else{
                            this->GenerateCharactersFromToken( CurrToken,CallingLayer,CurrentTraits,CharacterSegment );
                            GeneratedCharacters.splice(GeneratedCharacters.end(),CharacterSegment);
                            CurrToken->RenderSize = CharacterSegment.size();
                        }
                    }// case insert tag
                }// switch token type
            }// for each token
            return GeneratedCharacters;
        }

        TokenString* MarkupParser::Lex(const String& Source) const
        {
            // Return Data
            TokenString* RetTokens = new TokenString();

            size_t Position = 0;
            // Lets process some data
            while( Position < Source.size() )
            {
                // Find our start tag
                size_t TagStartPos = Source.find_first_of( this->GetMarkupTagStart(), Position );

                // Deal with non-tag text if needed
                if( TagStartPos == String::npos )
                {
                    // No tag for the remainder of the string, make our token and then exit
                    RetTokens->PushToken( CreateTextToken( Source.substr(Position) ) );
                    break;
                }
                else if( TagStartPos - Position > 0 )
                {
                    // We have text between our current position and our tag, make a token for it
                    RetTokens->PushToken( CreateTextToken( Source.substr(Position,TagStartPos - Position) ) );
                }

                // If we get this far, we got a tag so find the closing character
                size_t TagEndPos = Source.find_first_of( this->GetMarkupTagEnd(), TagStartPos );

                // if we didn't get a match, this isn't a valid token so make a text token instead
                if( TagEndPos == String::npos )
                {
                    RetTokens->PushToken( CreateTextToken( Source.substr(TagStartPos) ) );
                    break;
                }

                // Otherwise make a proper tag token and link it if it's an end tag
                TextToken* NewTagToken = CreateTagToken( Source.substr(TagStartPos,TagEndPos - TagStartPos + 1) );
                if( NewTagToken->Type == TextToken::TT_RangeTagEnd )
                {
                    RangeTagToken* EndToken = static_cast<RangeTagToken*>( NewTagToken );
                    for( TokenString::ReverseTokenIterator TokIt = RetTokens->ReverseBeginToken() ; TokIt != RetTokens->ReverseEndToken() ; ++TokIt )
                    {
                        if( (*TokIt)->Type == TextToken::TT_RangeTagStart )
                        {
                            RangeTagToken* TagCheck = static_cast<RangeTagToken*>( (*TokIt) );
                            if( TagCheck->PartnerTag == NULL && TagCheck->TagName == EndToken->TagName )
                            {
                                TagCheck->PartnerTag = EndToken;
                                EndToken->PartnerTag = TagCheck;
                            }
                        }
                    }
                }
                RetTokens->PushToken( NewTagToken );

                // Update our position for the next loop
                Position = TagEndPos + 1;
            }
            // Got our tokens, return 'em
            return RetTokens;
        }
    }//UI
}//Mezzanine

#endif
