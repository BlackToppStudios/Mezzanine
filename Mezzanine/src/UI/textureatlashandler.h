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
#ifndef _uitextureatlashandler_h
#define _uitextureatlashandler_h

#include "datatypes.h"

namespace Mezzanine
{
    namespace UI
    {
        class TextureAtlas;
        ///////////////////////////////////////////////////////////////////////////////
        /// @brief This handles the creation, storage and overall handling of Texture Atlases.
        /// @details
        ///////////////////////////////////////
        class MEZZ_LIB TextureAtlasHandler
        {
        public:
            /// @brief Container type for TextureAtlas storage within this class.
            typedef std::map< String, UI::TextureAtlas* >  AtlasContainer;
            /// @brief Iterator type for TextureAtlases being stored within this class.
            typedef AtlasContainer::iterator               AtlasIterator;
            /// @brief Const Iterator type for TextureAtlases being stored within this class.
            typedef AtlasContainer::const_iterator         ConstAtlasIterator;
        protected:
            /// @internal
            /// @brief Container storing all currently loaded texture atlases.
            AtlasContainer Atlases;
        public:
            /// @brief Class constructor.
            TextureAtlasHandler();
            /// @brief Class destructor.
            ~TextureAtlasHandler();

            ///////////////////////////////////////////////////////////////////////////////
            // Texture Atlas Management

            /// @brief Loads a Mezzanine Texture Atlas file for use with UI::Screen's.
            /// @param Name The name of the file to be loaded.
            /// @param Group The resource group where the MTA file can be found.
            void LoadAtlasFromFile(const String& Name, const String& Group = "UI");

            /// @brief Gets a loaded Atlas being stored in this manager.
            /// @param AtlasName The name of the Atlas, which is usually the name of the file without the extension.
            /// @return Returns a pointer to the requested Atlas, or NULL if it doesn't exist.
            UI::TextureAtlas* GetAtlas(const String& AtlasName);
            /// @brief Gets a loaded Atlas being stored in this manager.
            /// @param AtlasName The name of the Atlas, which is usually the name of the file without the extension.
            /// @return Returns a pointer to the requested Atlas, or throws an exception if it doesn't exist.
            UI::TextureAtlas* GetAtlasExcept(const String& AtlasName);
            /// @brief Destroys a loaded texture atlas.
            /// @param ToBeDestroyed A pointer to the texture atlas that will be destroyed.
            void DestroyAtlas(TextureAtlas* ToBeDestroyed);
            /// @brief Destroys all loaded texture atlases.
            void DestroyAllAtlases();
        };//TextureAtlasHandler
    }//UI
}//Mezzanine

#endif
