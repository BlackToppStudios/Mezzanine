// © Copyright 2010 - 2017 BlackTopp Studios Inc.
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
#ifndef _resourceasset_h
#define _resourceasset_h

#include "datatypes.h"

namespace Mezzanine
{
    namespace Resource
    {
        ///////////////////////////////////////////////////////////////////////////////
        /// @brief A convenience base class for objects that may need common IO operations exposed for them.
        /// @details An Asset is just a class that may need saving or loading operations present on the class.  Any
        /// data that has IO performed on it can be an Asset, regardless of whether or not it is represented by
        /// this class. @n @n
        /// The internal IO methods bypass the resource system and access the resource directly.  This should be
        /// avoided in most use cases as you could be opening multiple streams to the same assets or creating race
        /// conditions, or generating large delays for the WorkUnit if it is being executed in a WorkUnit.  They
        /// were primarily created to facilitate testing.
        ///////////////////////////////////////
        class MEZZ_LIB Asset
        {
        protected:
            // Listing of classes to be Assets:
            // Audio::Sound
            // Graphics::Image
        public:
            /// @brief Class constructor.
            Asset();
            /// @brief Class destructor.
            virtual ~Asset();

            ///////////////////////////////////////////////////////////////////////////////
            // Loading Methods

            /// @brief Loads an asset from an input stream.
            /// @param Stream The stream this asset will be read from.
            virtual void LoadAsset(std::istream* Stream) = 0;
            /// @brief Loads an asset from an asset group.
            /// @param FileName The name of the file to search for.
            /// @param GroupName The name of the asset group to look for the resource in.
            virtual void LoadAsset(const String& FileName, const String& GroupName);

            ///////////////////////////////////////////////////////////////////////////////
            // Saving Methods

            /// @brief Saves an asset to an output stream.
            /// @param Stream The stream this asset will written to.
            virtual void SaveAsset(std::ostream* Stream) = 0;
            /// @brief Saves an asset to an asset group.
            /// @param FileName The name of the file to save as.
            /// @param GroupName The name of the asset group to save to.
            virtual void SaveAsset(const String& FileName, const String& GroupName);

            ///////////////////////////////////////////////////////////////////////////////
            // Internal Loading Methods

            /// @internal
            /// @brief Workaround method to load an asset from the filesystem.
            /// @param File The complete file path and a name of the asset to be loaded.
            virtual void _LoadAsset(const String& File) = 0;
            /// @internal
            /// @brief Workaround method to load an asset from the filesystem.
            /// @param FilePath The directory path the file should be loaded from.
            /// @param FileName The name of the file to search for.
            virtual void _LoadAsset(const String& FilePath, const String& FileName);

            ///////////////////////////////////////////////////////////////////////////////
            // Internal Saving Methods

            /// @internal
            /// @brief Workaround method to save an asset to the filesystem.
            /// @param File The complete file path and a name of the asset to be saved.
            virtual void _SaveAsset(const String& File) = 0;
            /// @internal
            /// @brief Workaround method to save an asset to the filesystem.
            /// @param FilePath The directory path the file should be saved to.
            /// @param FileName The name of the file to save as.
            virtual void _SaveAsset(const String& FilePath, const String& FileName);
        };//Asset
    }//Resource
}//Mezzanine

#endif
