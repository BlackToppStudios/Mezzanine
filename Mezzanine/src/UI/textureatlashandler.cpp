// © Copyright 2010 - 2014 BlackTopp Studios Inc.
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
#ifndef _uitextureatlashandler_cpp
#define _uitextureatlashandler_cpp

#include "UI/textureatlashandler.h"
#include "UI/textureatlas.h"

#include "resourcemanager.h"
#include "Resource/datastream.h"
#include "XML/xml.h"

namespace Mezzanine
{
    namespace UI
    {
        TextureAtlasHandler::TextureAtlasHandler()
            {  }

        TextureAtlasHandler::~TextureAtlasHandler()
            {  }

        ///////////////////////////////////////////////////////////////////////////////
        // Texture Atlas Management

        void TextureAtlasHandler::LoadAtlasFromFile(const String& Name, const String& Group)
        {
            /// @todo Update after we have refactored the resource system if needed.
            Resource::DataStreamPtr AtlasStream = ResourceManager::GetSingletonPtr()->OpenAssetStream(Name,Group);
            AtlasStream->SetStreamPosition(0);
            XML::Document AtlasDoc;
            AtlasDoc.Load( *AtlasStream.Get() );

            XML::Node RootNode = AtlasDoc.GetChild("Atlases");
            if( !RootNode.Empty() )
            {
                for( XML::NodeIterator AtlasIt = RootNode.begin() ; AtlasIt != RootNode.end() ; ++AtlasIt )
                {
                    // Parse the Atlas
                    TextureAtlas* NewAtlas = new TextureAtlas( (*AtlasIt) );
                    // Verify we don't already have one of the same name
                    AtlasIterator AtIt = this->Atlases.find( NewAtlas->GetName() );
                    if( AtIt != Atlases.end() )
                    {
                        MEZZ_EXCEPTION(Exception::II_DUPLICATE_IDENTITY_EXCEPTION,"Texture Atlas with the name \"" + NewAtlas->GetName() + "\" already exists.");
                    }
                    // Add the unique Atlas
                    this->Atlases[NewAtlas->GetName()] = NewAtlas;
                }
            }else{
                MEZZ_EXCEPTION(Exception::INVALID_STATE_EXCEPTION,"Mezzanine Texture Atlas file \"" + Name + "\"does not contain expected \"Atlases\" root node.  File is not valid and cannot be parsed.");
            }
        }

        UI::TextureAtlas* TextureAtlasHandler::GetAtlas(const String& AtlasName)
        {
            AtlasIterator It = this->Atlases.find(AtlasName);
            if(It != this->Atlases.end()) return (*It).second;
            else { MEZZ_EXCEPTION(Exception::II_IDENTITY_NOT_FOUND_EXCEPTION,"Unabled to find Texture Atlas named \"" + AtlasName + "\".") };
        }
    }//UI
}//Mezzanine

#endif
