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
#ifndef _resourceenumerations_h
#define _resourceenumerations_h
///////////////////////////////////////////////////////////////////////////////
//Any global enumerations shared between multiple classes in the Resource namespace is to be declared here.
///////////////////////////////////////

namespace Mezzanine
{
    namespace Resource
    {
        /// @brief Used to indicate what kind of resources the Entresol should look for
        enum ArchiveType
        {
            AT_Invalid     = 0,    ///< Indicates this valid was messed up unrecoverably, most likely by a bug.
            AT_FileSystem  = 1,    ///< Look for raw files
            AT_Zip         = 2,    ///< Look for stuff in zip files even if the extension is not '.zip'.
        };

        /// @brief Used to indicate an algorithm of compression.
        enum CompressionMethod
        {
            CM_Unknown = 0,
            CM_None,
            CM_Deflate,
            CM_Deflate64,
            CM_BZip2,
            CM_ZPAQ,
            CM_Brotli9,
            CM_Brotli11,
            CM_LZMA,
            CM_LZ4,
            CM_LZ4F
        };

        /// @brief Used to indicate an algorithm of encryption.
        enum EncryptionMethod
        {
            EA_Unknown = 0,
            EA_PKWARE,
            EA_AES_128,
            EA_AES_192,
            EA_AES_256,
            EA_RC2,
            EA_RC4,
            EA_3DES_112,
            EA_3DES_168
        };

        /// @brief Used to differentiate entries in some archive systems.
        enum EntryType
        {
            ET_Unknown,
            ET_File,
            ET_Directory,
            ET_Symlink
        };

        /// @brief This enum describes the current state of loading for an asset.
        enum LoadingState
        {
            LS_Uninitialized = 0,
            LS_Initializing = 1,
            LS_Initialized = 2,
            LS_Loading = 3,
            LS_Loaded = 4
        };
    }//Resource
}//Mezzanine

#endif
