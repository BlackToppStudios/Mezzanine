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
#ifndef _uitexttoken_cpp
#define _uitexttoken_cpp

#include "UI/texttoken.h"

#include "unicode.h"
#include "exception.h"

namespace Mezzanine
{
    namespace UI
    {
        ///////////////////////////////////////////////////////////////////////////////
        // TextToken Methods

        TextToken::TextToken() :
            Type(TT_Error),
            RenderSize(0)
            {  }

        TextToken::TextToken(const String& RawText, const TokenType TType) :
            Type(TType),
            RenderSize(0),
            Text(RawText)
            {  }

        TextToken::TextToken(const Char8* Characters, const UInt32 Size) :
            Type(TT_Text),
            RenderSize(0)
            { this->InsertCharacters(0,Characters,Size); }

        TextToken::TextToken(const UInt32* Characters, const UInt32 Size) :
            Type(TT_Text),
            RenderSize(0)
            { this->InsertCharacters(0,Characters,Size); }

        TextToken::~TextToken()
            {  }

        UInt32 TextToken::ConvertRenderIndexToRawIndex(const UInt32 Index)
        {
            Int32 BytesAdvance = 0;
            UInt32 CurrRawIndex = 0;
            UInt32 CurrRenderIndex = 0;
            while( CurrRawIndex < this->Text.size() && CurrRenderIndex < Index )
            {
                if( Unicode::GetIntFromCharacter(BytesAdvance,this->Text.data() + CurrRawIndex) < 0 ) {
                    return -1;
                }
                CurrRawIndex += BytesAdvance;
                ++CurrRenderIndex;
            }

            return CurrRawIndex;
        }

        const String& TextToken::GetRawCharacterString() const
            { return this->Text; }

        TextToken::TokenType TextToken::GetTokenType() const
            { return this->Type; }

        UInt32 TextToken::GetRawCharacterSize() const
            { return this->Text.size(); }

        UInt32 TextToken::GetRenderCharacterSize() const
            { return this->RenderSize; }

        UInt32 TextToken::InsertCharacter(const UInt32 Index, UInt32 UChar)
        {
            return this->InsertCharacters(Index,&UChar,1);
        }

        UInt32 TextToken::InsertCharacters(const UInt32 Index, const Char8* Characters, const UInt32 Size)
        {
            if( this->RenderSize >= Index )
            {
                UInt32 RawIndex = this->ConvertRenderIndexToRawIndex(Index);
                if( RawIndex < 0 ) {
                    return 0;
                }

                UInt32 Ret = 0;
                // Verify the encoding and count the characters
                UInt32 BufPos = 0;
                Int32 BytesAdvance = 0;
                //Char8 ConversionBuffer[4];
                while( BufPos < Size )
                {
                    if( Unicode::GetIntFromCharacter(BytesAdvance,Characters + BufPos) < 0 ) {
                        return 0;
                    }
                    BufPos += static_cast<UInt32>(BytesAdvance);
                    ++Ret;
                }
                this->Text.insert(RawIndex,Characters[0],Size);
                return Ret;
            }
            return 0;
        }

        UInt32 TextToken::InsertCharacters(const UInt32 Index, const UInt32* Characters, const UInt32 Size)
        {
            if( this->RenderSize >= Index )
            {
                UInt32 RawIndex = this->ConvertRenderIndexToRawIndex(Index);
                if( RawIndex < 0 ) {
                    return 0;
                }

                Int32 BytesAdvance = 0;
                UInt32 SourceBufPos = 0;
                UInt32 ConvBufPos = 0;
                UInt32 ConvBufSize = Size * 4; // Max size needed
                Char8* ConversionBuffer = new Char8[ConvBufSize];
                while( SourceBufPos < Size )
                {
                    BytesAdvance = Unicode::GetCharacterFromInt(&(ConversionBuffer[ConvBufPos]),ConvBufSize - ConvBufPos,Characters[SourceBufPos]);
                    if( BytesAdvance < 0 ) {
                        return 0;
                    }

                    ConvBufPos += static_cast<UInt32>(BytesAdvance);
                    ++SourceBufPos;
                }

                this->Text.insert(RawIndex,ConversionBuffer,ConvBufPos);
                delete[] ConversionBuffer;
                return Size;
            }
            return 0;
        }

        UInt32 TextToken::RemoveCharacter(const UInt32 Index)
        {
            if( this->RenderSize >= Index )
            {
                UInt32 RawIndex = this->ConvertRenderIndexToRawIndex(Index);
                if( RawIndex < 0 ) {
                    return 0;
                }

                Int32 BytesAdvance = 0;
                if( RawIndex < this->Text.size() ) { //possibly redundent check
                    if( Unicode::GetIntFromCharacter(BytesAdvance,(this->Text.data() + RawIndex)) < 0 ) {
                        return 0;
                    }
                    this->Text.erase(RawIndex,BytesAdvance);
                    return 1;
                }
            }
            return 0;
        }

        UInt32 TextToken::RemoveCharacters(const UInt32 Index, const UInt32 Length)
        {
            if( this->RenderSize >= Index )
            {
                UInt32 RawIndex = this->ConvertRenderIndexToRawIndex(Index);
                if( RawIndex < 0 ) {
                    return 0;
                }

                // Start another loop to find the length
                Int32 BytesAdvance = 0;
                UInt32 CurrRawLength = 0;
                UInt32 CurrRenderLength = 0;
                while( RawIndex + CurrRawLength < this->Text.size() && CurrRenderLength < Length )
                {
                    if( Unicode::GetIntFromCharacter(BytesAdvance,(this->Text.data() + RawIndex) + CurrRawLength) < 0 ) {
                        return 0;
                    }
                    CurrRawLength += static_cast<UInt32>(BytesAdvance);
                    ++CurrRenderLength;
                }

                this->Text.erase(RawIndex,CurrRawLength);
                return CurrRenderLength;
            }
            return 0;
        }

        UInt32 TextToken::ClearAllCharacters()
        {
            UInt32 PrevSize = this->RenderSize;
            this->Text.clear();
            this->RenderSize = 0;
            return PrevSize;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // TagToken Methods

        TagToken::TagToken()
            {  }

        TagToken::TagToken(const String& RawText, const String& Name, const TokenType TType) :
            TextToken(RawText,TType),
            TagName(Name)
            {  }

        TagToken::~TagToken()
            {  }

        const String& TagToken::GetTagName() const
            { return this->TagName; }

        String TagToken::GetParameter(const String& Param) const
        {
            NameValuePairMap::const_iterator ParamIt = this->Params.find(Param);
            if( ParamIt != this->Params.end() ) return (*ParamIt).second;
            else return "";
        }

        ///////////////////////////////////////////////////////////////////////////////
        // RangeTagToken Methods

        RangeTagToken::RangeTagToken() :
            PartnerTag(NULL)
            {  }

        RangeTagToken::RangeTagToken(const String& RawText, const String& Name, const TokenType TType) :
            TagToken(RawText,Name,TType),
            PartnerTag(NULL)
            {  }

        RangeTagToken::~RangeTagToken()
            {  }

        RangeTagToken* RangeTagToken::GetPartnerTag() const
            { return this->PartnerTag; }

        UInt32 RangeTagToken::InsertCharacter(const UInt32 Index, UInt32 UChar)
        {
            if( this->PartnerTag == NULL ) {
                if( this->RenderSize != 0 ) {
                    return this->TextToken::InsertCharacter(Index,UChar);
                }
            }
            return 0;
        }

        UInt32 RangeTagToken::InsertCharacters(const UInt32 Index, const Char8* Characters, const UInt32 Size)
        {
            if( this->PartnerTag == NULL ) {
                if( this->RenderSize != 0 ) {
                    return this->TextToken::InsertCharacters(Index,Characters,Size);
                }
            }
            return 0;
        }

        UInt32 RangeTagToken::InsertCharacters(const UInt32 Index, const UInt32* Characters, const UInt32 Size)
        {
            if( this->PartnerTag == NULL ) {
                if( this->RenderSize != 0 ) {
                    return this->TextToken::InsertCharacters(Index,Characters,Size);
                }
            }
            return 0;
        }

        UInt32 RangeTagToken::RemoveCharacter(const UInt32 Index)
        {
            if( this->PartnerTag == NULL ) {
                if( this->RenderSize != 0 ) {
                    return this->TextToken::RemoveCharacter(Index);
                }
            }
            return 0;
        }

        UInt32 RangeTagToken::RemoveCharacters(const UInt32 Index, const UInt32 Length)
        {
            if( this->PartnerTag == NULL ) {
                if( this->RenderSize != 0 ) {
                    return this->TextToken::RemoveCharacters(Index,Length);
                }
            }
            return 0;
        }

        UInt32 RangeTagToken::ClearAllCharacters()
        {
            if( this->PartnerTag == NULL ) {
                if( this->RenderSize != 0 ) {
                    return this->TextToken::ClearAllCharacters();
                }
            }
            return 0;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // InsertTagToken Methods

        InsertTagToken::InsertTagToken()
            {  }

        InsertTagToken::InsertTagToken(const String& RawText, const String& Name) :
            TagToken(RawText,Name,TextToken::TT_InsertTag)
            {  }

        InsertTagToken::~InsertTagToken()
            {  }

        UInt32 InsertTagToken::InsertCharacter(const UInt32 Index, UInt32 UChar)
        {
            // Glyphs can't be inserted into this type of token.
            return 0;
        }

        UInt32 InsertTagToken::InsertCharacters(const UInt32 Index, const Char8* Characters, const UInt32 Size)
        {
            // Glyphs can't be inserted into this type of token.
            return 0;
        }

        UInt32 InsertTagToken::InsertCharacters(const UInt32 Index, const UInt32* Characters, const UInt32 Size)
        {
            // Glyphs can't be inserted into this type of token.
            return 0;
        }

        UInt32 InsertTagToken::RemoveCharacter(const UInt32 Index)
        {
            if( this->RenderSize != 0 && Index == 0 ) {
                this->Text.clear();
                this->RenderSize = 0;
                return 1;
            }
            return 0;
        }

        UInt32 InsertTagToken::RemoveCharacters(const UInt32 Index, const UInt32 Length)
        {
            if( this->RenderSize != 0 && Index == 0 && Length > 0 ) {
                this->Text.clear();
                this->RenderSize = 0;
                return 1;
            }
            return 0;
        }

        UInt32 InsertTagToken::ClearAllCharacters()
        {
            if( this->RenderSize != 0 ) {
                this->Text.clear();
                this->RenderSize = 0;
                return 1;
            }
            return 0;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // TokenString Methods

        TokenString::TokenString()
            {  }

        TokenString::~TokenString()
            { this->DestroyAllTokens(); }

        TokenString::TokenIndexPair TokenString::GetTokenIndex(const UInt32 Index)
        {
            TokenIndexPair Ret;
            if( this->GetNumTokens() > 0 ) {
                UInt32 IndexCount = 0;
                for( TokenIterator TokIt = this->Tokens.begin() ; TokIt != this->Tokens.end() ; ++TokIt )
                {
                    if( IndexCount + (*TokIt)->GetRenderCharacterSize() < Index ) {
                        IndexCount += (*TokIt)->GetRenderCharacterSize();
                        continue;
                    }else{
                        Ret.first = TokIt;
                        Ret.second = Index - IndexCount;
                        return Ret;
                    }
                }
            }else{
                // Fix for manipulating empty strings.
                this->Tokens.push_back( new TextToken(String(""),TextToken::TT_Text) );
            }
            // If we get to this point, then the index requested is out of bounds.
            // Just return the last token and it's max index.
            Ret.first = --(this->Tokens.end());
            Ret.second = (*Ret.first)->GetRenderCharacterSize();
            return Ret;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        String TokenString::GetRawString() const
        {
            String Ret;
            for( ConstTokenIterator TokIt = this->Tokens.begin() ; TokIt != this->Tokens.end() ; ++TokIt )
            {
                Ret.append( (*TokIt)->GetRawCharacterString() );
            }
            return Ret;
        }

        Whole TokenString::GetNumCharacters() const
        {
            Whole Ret = 0;
            for( ConstTokenIterator TokIt = this->Tokens.begin() ; TokIt != this->Tokens.end() ; ++TokIt )
            {
                Ret += (*TokIt)->GetRenderCharacterSize();
            }
            return Ret;
        }

        Whole TokenString::GetNumTokens() const
        {
            return this->Tokens.size();
        }

        TokenString::TokenIterator TokenString::BeginToken()
            { return this->Tokens.begin(); }

        TokenString::TokenIterator TokenString::EndToken()
            { return this->Tokens.end(); }

        TokenString::ConstTokenIterator TokenString::BeginToken() const
            { return this->Tokens.begin(); }

        TokenString::ConstTokenIterator TokenString::EndToken() const
            { return this->Tokens.end(); }

        TokenString::ReverseTokenIterator TokenString::ReverseBeginToken()
            { return this->Tokens.rbegin(); }

        TokenString::ReverseTokenIterator TokenString::ReverseEndToken()
            { return this->Tokens.rend(); }

        TokenString::ConstReverseTokenIterator TokenString::ReverseBeginToken() const
            { return this->Tokens.rbegin(); }

        TokenString::ConstReverseTokenIterator TokenString::ReverseEndToken() const
            { return this->Tokens.rend(); }

        ///////////////////////////////////////////////////////////////////////////////
        // Generating the String

        void TokenString::PushToken(TextToken* ToBePushed)
            { this->Tokens.push_back(ToBePushed); }

        void TokenString::PushTokens(const TokenContainer& ToBePushed)
            { this->Tokens.insert(this->Tokens.end(),ToBePushed.begin(),ToBePushed.end()); }

        void TokenString::DestroyAllTokens()
        {
            for( TokenIterator TokIt = this->Tokens.begin() ; TokIt != this->Tokens.end() ; ++TokIt )
            {
                delete (*TokIt);
            }
            this->Tokens.clear();
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Inserting and Removing

        UInt32 TokenString::InsertCharacter(const UInt32 Index, UInt32 UChar)
        {
            TokenIndexPair Result = this->GetTokenIndex(Index);
            UInt32 Ret = (*Result.first)->InsertCharacter(Result.second,UChar);
            if( Ret == 0 ) {
                // If we're here, then we're probably on a token that isn't a text token, or behaving like one.
                // There are a number of conditions depending on tag types that could cause this, so instead of trying to troubleshoot them, lets just insert a new text token.
                TextToken* NewToken = new TextToken(&UChar,1);
                this->Tokens.insert(Result.first,NewToken);
                Ret = 1;
            }
            return Ret;
        }

        UInt32 TokenString::InsertCharacters(const UInt32 Index, const Char8* Characters, const UInt32 Size)
        {
            TokenIndexPair Result = this->GetTokenIndex(Index);
            UInt32 Ret = (*Result.first)->InsertCharacters(Result.second,Characters,Size);
            if( Ret == 0 ) {
                // If we're here, then we're probably on a token that isn't a text token, or behaving like one.
                // There are a number of conditions depending on tag types that could cause this, so instead of trying to troubleshoot them, lets just insert a new text token.
                TextToken* NewToken = new TextToken(Characters,Size);
                this->Tokens.insert(Result.first,NewToken);
                Ret = Size;
            }
            return Ret;
        }

        UInt32 TokenString::InsertCharacters(const UInt32 Index, const UInt32* Characters, const UInt32 Size)
        {
            TokenIndexPair Result = this->GetTokenIndex(Index);
            UInt32 Ret = (*Result.first)->InsertCharacters(Result.second,Characters,Size);
            if( Ret == 0 ) {
                // If we're here, then we're probably on a token that isn't a text token, or behaving like one.
                // There are a number of conditions depending on tag types that could cause this, so instead of trying to troubleshoot them, lets just insert a new text token.
                TextToken* NewToken = new TextToken(Characters,Size);
                this->Tokens.insert(Result.first,NewToken);
                Ret = Size;
            }
            return Ret;
        }

        UInt32 TokenString::RemoveCharacter(const UInt32 Index)
        {
            TokenIndexPair Result = this->GetTokenIndex(Index);
            return (*Result.first)->RemoveCharacter(Result.second);
        }

        UInt32 TokenString::RemoveCharacters(const UInt32 Index, const UInt32 Length)
        {
            TokenIndexPair Result = this->GetTokenIndex(Index);
            UInt32 Removed = (*Result.first)->RemoveCharacters(Result.second,Length);
            while( Removed < Length ) {
                ++(Result.first);
                if( Result.first == this->Tokens.end() )
                    break;

                Removed += (*Result.first)->RemoveCharacters(Result.second,Length - Removed);
            }
            return Removed;
        }

        UInt32 TokenString::ClearAllCharacters()
        {
            UInt32 Ret = 0;
            for( TokenIterator TokIt = this->Tokens.begin() ; TokIt != this->Tokens.end() ; ++TokIt )
            {
                Ret += (*TokIt)->ClearAllCharacters();
            }
            return Ret;
        }
    }//UI
}//Mezzanine

#endif
