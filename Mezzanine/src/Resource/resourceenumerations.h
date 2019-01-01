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

#include "enumerations.h"

namespace Mezzanine
{
    namespace Resource
    {
        /// @brief Used to indicate what kind of resources the Entresol should look for
        enum ArchiveType
        {
            AT_Invalid     = 0,    ///< Indicates an error condition of some kind.
            AT_FileSystem  = 1,    ///< No archive format, raw binary on disk.
            AT_Zip         = 2,    ///< Archive abiding by the PKWARE Zip format.
            AT_7z          = 3     ///< Archive abiding by the 7zip format.
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
            ET_Hardlink,
            ET_Symlink
        };

        /// @brief Used to identify the permissions a file has in an archive.
        /// @remarks These permissions are based on posix/std::filesystem permissions.
        enum FilePermissions
        {
            FP_None          = 0,
            FP_Other_Execute = EnumBit(1),
            FP_Other_Write   = EnumBit(2),
            FP_Other_Read    = EnumBit(3),
            FP_Group_Execute = EnumBit(4),
            FP_Group_Write   = EnumBit(5),
            FP_Group_Read    = EnumBit(6),
            FP_Owner_Execute = EnumBit(7),
            FP_Owner_Write   = EnumBit(8),
            FP_Owner_Read    = EnumBit(9),
            FP_Read = FP_Owner_Read | FP_Group_Read | FP_Other_Read,
            FP_Write = FP_Owner_Write | FP_Group_Write | FP_Other_Write,
            FP_Execute = FP_Owner_Execute | FP_Group_Execute | FP_Other_Execute,
            FP_Other = FP_Other_Read | FP_Other_Write | FP_Other_Execute,
            FP_Group = FP_Group_Read | FP_Group_Write | FP_Group_Execute,
            FP_Owner = FP_Owner_Read | FP_Owner_Write | FP_Owner_Execute
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

        /// @brief An enum listing the codecs directly supported by the Zip format implementation with their IDs.
        /// @remarks In addition to the values listed here, there are additional values in the spec that
        /// are reserved for future use by PKWare. Those values are: 11, 13, 15, 16, and 17. @n
        /// It is also important to note that these are only what is in the spec and thus what other programs
        /// are likely to recognize, however little stops you from adding a custom codec to a Zip archive.
        /// The only potential issue is other archive tools recognizing the value. This isn't a concern if
        /// you only plan to use that archive with your own custom application.
        enum ZipCodecSupport
        {
            ZCS_Raw                 = 0,  ///< Supported.  No compression.
            ZCS_Shrunk              = 1,  ///< Not Supported.
            ZCS_ReducedCompression1 = 2,  ///< Not Supported.
            ZCS_ReducedCompression2 = 3,  ///< Not Supported.
            ZCS_ReducedCompression3 = 4,  ///< Not Supported.
            ZCS_ReducedCompression4 = 5,  ///< Not Supported.
            ZCS_Imploded            = 6,  ///< Not Supported.
            ZCS_Tokenizing          = 7,  ///< Not Supported.
            ZCS_Deflate             = 8,  ///< Supported.  Standard DEFLATE, which is the default for Zip files.
            ZCS_Deflate64           = 9,  ///< Not Supported.
            ZCS_IBM_TERSE_Old       = 10, ///< Not Supported.
            ZCS_BZip2               = 12, ///< Supported.
            ZCS_LZMA                = 14, ///< Not Supported.
            ZCS_IBM_TERSE_New       = 18, ///< Not Supported.
            ZCS_LZ77                = 19, ///< Not Supported.
            ZCS_WavPack             = 97, ///< Not Supported.
            ZCS_PPMd                = 98  ///< Not Supported.
        };
    }//Resource
}//Mezzanine

#endif
