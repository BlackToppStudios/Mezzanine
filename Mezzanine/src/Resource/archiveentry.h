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
#ifndef _resourcearchiveentry_h
#define _resourcearchiveentry_h

#include "datatypes.h"
#include "Resource/resourceenumerations.h"

namespace Mezzanine
{
    namespace Resource
    {
        ///////////////////////////////////////////////////////////////////////////////
        /// @brief A base class for conveying information on a specific entry in an archive.
        /// @details An archive can be the intuitive compressed archive, the filesystem, a space in memory, or something
        /// over the network.  Any place a collection of files can be stored, really.  An entry is single collection
        /// of metadata relating to that one file.  This class specifically contains abstract methods for the query
        /// of the most common/basic information that is common between data in most or all of these places.  Not
        /// every archive will support all of the information covered here, but they should contain most of it.
        ///////////////////////////////////////
        class MEZZ_LIB ArchiveEntry
        {
        public:
            ///////////////////////////////////////////////////////////////////////////////
            // Type Information

            /// @brief Gets the type of archive this entry belongs to.
            /// @remarks Generally archives will each have their own Entry type, so this enum can be used to determine
            /// what this entry can be safely casted to in order to get more detailed metadata about the file.
            ArchiveType ArchType = AT_Invalid;
            /// @brief Gets the type of entry this is in the Archive.
            /// @remarks See the EntryType enum for more detailed description of entry types.
            EntryType EntType = ET_Unknown;

            ///////////////////////////////////////////////////////////////////////////////
            // Basic Metadata

            /// @brief The full path to and name of the file.
            String Name;
            /// @brief A short description of the file.
            String Comment;
            /// @brief The size of the file.
            /// @remarks Usually in compressed Archives, this will be the uncompressed size of the file.
            UInt64 Size = 0;
            /// @brief The time the file was created.
            UInt64 CreateTime = 0;
            /// @brief The last time the file was accessed.
            UInt64 AccessTime = 0;
            /// @brief The last time the file was modified.
            UInt64 ModifyTime = 0;

            ///////////////////////////////////////////////////////////////////////////////
            // Simple Queries

            /// @brief Whether or not the file can only be read.
            Boole ReadOnly = true;

            ///////////////////////////////////////////////////////////////////////////////
            // Compression Type Information

            /// @brief Gets the method used to compress the file referenced by the entry.
            CompressionMethod CompressMethod = CM_Unknown;

            ///////////////////////////////////////////////////////////////////////////////
            // Compression Metadata

            /// @brief The compressed size of the file.
            UInt64 CompressedSize = 0;
            /// @brief The 32-bit Cyclic Redundancy Check (CRC) of the file.
            UInt32 CRC = 0;

            ///////////////////////////////////////////////////////////////////////////////
            // Encryption Type Information

            /// @brief The algorithm used to encrypt the file referenced by the entry.
            EncryptionMethod EncryptMethod = EA_Unknown;
        };//ArchiveEntry

        /// @brief Convenience pointer type for ArchiveEntry.
        using ArchiveEntryPtr = std::unique_ptr<ArchiveEntry>;
    }//Resource
}//Mezzanine

#endif
