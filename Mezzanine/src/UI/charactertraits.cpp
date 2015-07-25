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

#ifndef _uicharactertraits_cpp
#define _uicharactertraits_cpp

#include "UI/charactertraits.h"
#include "UI/font.h"
#include "UI/textureatlas.h"

#include "UI/uimanager.h"

namespace Mezzanine
{
    namespace UI
    {
        CharacterTraits::CharacterTraits() :
            CharFont(NULL),
            CharColour(ColourValue::White())
            {  }

        CharacterTraits::CharacterTraits(FontData* Font, const ColourValue& Character) :
            CharFont(Font),
            CharColour(Character)
            {  }

        CharacterTraits::~CharacterTraits()
            {  }

        ///////////////////////////////////////////////////////////////////////////////
        // Operator Methods

        CharacterTraits& CharacterTraits::operator=(const CharacterTraits& Other)
        {
            this->CharFont = Other.CharFont;
            this->CharColour = Other.CharColour;
            return *this;
        }

        Boole CharacterTraits::operator==(const CharacterTraits& Other) const
        {
            return ( this->CharFont == Other.CharFont &&
                     this->CharColour == Other.CharColour );
        }

        Boole CharacterTraits::operator!=(const CharacterTraits& Other) const
        {
            return ( this->CharFont != Other.CharFont ||
                     this->CharColour != Other.CharColour );
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Serialization

        void CharacterTraits::ProtoSerialize(XML::Node& ParentNode) const
        {
            Mezzanine::XML::Node TraitsNode = ParentNode.AppendChild(this->CharacterTraits::GetSerializableName());

            if( TraitsNode.AppendAttribute("FontName").SetValue(this->CharFont->GetName()) &&
                TraitsNode.AppendAttribute("AtlasName").SetValue(this->CharFont->GetAtlasName()) )
            {
                XML::Node CharColourNode = TraitsNode.AppendChild("CharColour");
                this->CharColour.ProtoSerialize( CharColourNode );

                return;
            }else{
                SerializeError("Create XML Attribute Values",CharacterTraits::GetSerializableName() + "Properties",true);
            }
        }

        void CharacterTraits::ProtoDeSerialize(const XML::Node& SelfRoot)
        {
            XML::Attribute CurrAttrib;
            if( String( SelfRoot.Name()) == this->CharacterTraits::GetSerializableName() )
            {
                if( SelfRoot.GetAttribute("Version").AsInt() == 1 )
                {
                    String FontName, AtlasName;

                    CurrAttrib = SelfRoot.GetAttribute("FontName");
                    if( !CurrAttrib.Empty() )
                        FontName = CurrAttrib.AsString();

                    CurrAttrib = SelfRoot.GetAttribute("AtlasName");
                    if( !CurrAttrib.Empty() )
                        AtlasName = CurrAttrib.AsString();

                    if( !FontName.empty() && !AtlasName.empty() ) {
                        this->CharFont = UIManager::GetSingletonPtr()->GetAtlas(AtlasName)->GetFont(FontName);
                    }else{
                        this->CharFont = NULL;
                    }

                    XML::Node CharColourNode = SelfRoot.GetChild("CharColour").GetFirstChild();
                    if( !CharColourNode.Empty() )
                        this->CharColour.ProtoDeSerialize(CharColourNode);
                }else{
                    MEZZ_EXCEPTION(Exception::INVALID_VERSION_EXCEPTION,"Incompatible XML Version for " + (this->CharacterTraits::GetSerializableName()) + ": Not Version 1");
                }
            }else{
                MEZZ_EXCEPTION(Exception::II_IDENTITY_INVALID_EXCEPTION,"Attempting to deserialize a " + (this->CharacterTraits::GetSerializableName()) + ", found a " + SelfRoot.Name());
            }
        }

        String CharacterTraits::GetSerializableName()
        {
            return "CharacterTraits";
        }
    }//UI
}//Mezzanine

#endif
