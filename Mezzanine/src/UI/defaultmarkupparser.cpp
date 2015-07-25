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
#ifndef _uidefaultmarkupparser_cpp
#define _uidefaultmarkupparser_cpp

#include "UI/defaultmarkupparser.h"
#include "UI/textlayer.h"
#include "UI/screen.h"
#include "UI/textureatlas.h"
#include "stringtool.h"

namespace Mezzanine
{
    namespace UI
    {
        ///////////////////////////////////////////////////////////////////////////////
        // DefaultColourTag Methods

        const String DefaultColourTag::TagName = "Colour";

        MarkupTag::ProcessResult DefaultColourTag::Process(NameValuePairMap& Params, CharacterTraits& Traits, TextLayer* Layer) const
        {
            NameValuePairMap::iterator Colour = Params.find("Value");
            if( Colour != Params.end() ) {
                if( (*Colour).second.size() < 6 ) return ProcessResult(false,NULL);
                else
                {
                    Traits.CharColour = StringTools::ConvertHexToColourValue( (*Colour).second );
                    return ProcessResult(true,NULL);
                }
            }else{
                return ProcessResult(false,NULL);
            }
        }

        ///////////////////////////////////////////////////////////////////////////////
        // DefaultFontTag Methods

        const String DefaultFontTag::TagName = "Font";

        MarkupTag::ProcessResult DefaultFontTag::Process(NameValuePairMap& Params, CharacterTraits& Traits, TextLayer* Layer) const
        {
            Screen* ParentScreen = Layer->GetScreen();
            String AtlasName = Layer->GetPrimaryAtlas();

            NameValuePairMap::iterator FontName = Params.find("Name");
            if( FontName != Params.end() ) {
                Traits.CharFont = ParentScreen->GetFont( (*FontName).second,AtlasName );
            }else{
                return ProcessResult(false,NULL);
            }

            /*NameValuePairMap::iterator FontSize = Params.find("Size");
            if( FontSize != Params.end() ) {
                Traits.CharFont->
            }else{
                return ProcessResult(false,NULL);
            }//*/

            return ProcessResult(true,NULL);
        }

        ///////////////////////////////////////////////////////////////////////////////
        // DefaultSpriteTag Methods

        const String DefaultSpriteTag::TagName = "Sprite";

        MarkupTag::ProcessResult DefaultSpriteTag::Process(NameValuePairMap& Params, CharacterTraits& Traits, TextLayer* Layer) const
        {
            Screen* ParentScreen = Layer->GetScreen();
            String AtlasName = Layer->GetPrimaryAtlas();
            Character* ToInsert = NULL;

            NameValuePairMap::iterator SpriteName = Params.find("Name");
            if( SpriteName != Params.end() ) {
                Sprite* NewSprite = ParentScreen->GetSprite( (*SpriteName).second,AtlasName );
                if( NewSprite )
                    ToInsert = new Character(NewSprite,Layer);
            }else{
                return ProcessResult(false,NULL);
            }

            NameValuePairMap::iterator SpriteSize = Params.find("Size");
            if( ToInsert && SpriteSize != Params.end() ) {
                String SizeStr = (*SpriteSize).second;
                SizeStr.replace( SizeStr.find_first_of('x'),1," " );
                ToInsert->SetCustomSize( StringTools::ConvertToVector2(SizeStr) );
            }

            return ProcessResult(true,ToInsert);
        }

        ///////////////////////////////////////////////////////////////////////////////
        // DefaultMarkupParser Methods

        DefaultMarkupParser::DefaultMarkupParser()
            { this->Initialize(); }

        DefaultMarkupParser::~DefaultMarkupParser()
        {
            for( TagIterator TagIt = this->Tags.begin() ; TagIt != this->Tags.end() ; ++TagIt )
                { delete (*TagIt).second; }
            this->Tags.clear();
        }

        void DefaultMarkupParser::Initialize()
        {
            this->Tags.insert( std::pair<String,MarkupTag*>( DefaultColourTag::TagName,new DefaultColourTag() ) );
            this->Tags.insert( std::pair<String,MarkupTag*>( DefaultFontTag::TagName,new DefaultFontTag() ) );
            this->Tags.insert( std::pair<String,MarkupTag*>( DefaultSpriteTag::TagName,new DefaultSpriteTag() ) );
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        String DefaultMarkupParser::GetName() const
            { return "Default"; }

        Char8 DefaultMarkupParser::GetMarkupTagStart() const
            { return '['; }

        Char8 DefaultMarkupParser::GetMarkupTagEnd() const
            { return ']'; }
    }//UI
}//Mezzanine

#endif
