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
        /// @brief A base class for querying compression specific information on an entry in an archive.
        /// @details Not all archives will support the storage and/or query of all the information contained here.
        ///////////////////////////////////////
        class MEZZ_LIB ArchiveEntryCompressionInfo
        {
        public:
            ///////////////////////////////////////////////////////////////////////////////
            // Type Information

            /// @brief Gets the method used to compress the file referenced by the entry.
            /// @return Returns an enum value stating the method used to compress the file.
            virtual CompressionMethod GetCompressionMethod() const = 0;

            ///////////////////////////////////////////////////////////////////////////////
            // Basic Metadata

            /// @brief Gets the compressed size of the file.
            /// @return Returns the size of the file in it's compressed state in the archive in bytes.
            virtual size_t GetCompressedSize() const = 0;
            /// @brief Gets the version of the compression method used to create the compressed file.
            /// @return Returns an integer identifying the version of the compression method used, or zero if this information is unsupported.
            virtual UInt16 GetCompressVersion() const = 0;
            /// @brief Gets the version of the compression method needed to decompress the compressed file.
            /// @return Returns an integer identifying the version of the compression method needed, or zero if this information is unsupported.
            virtual UInt16 GetDecompressVersion() const = 0;
            /// @brief Gets the 32-bit Cyclic Redundancy Check (CRC) of the file.
            /// @return Returns the CRC of the file for verifying file integrity, or zero if CRC is not supported/stored by this archive.
            virtual UInt32 GetCRC() const = 0;
        };//ArchiveEntryCompressionInfo

        /// @brief Convenience pointer type for ArchiveEntryCompressionInfo.
        using ArchiveEntryCompressionInfoPtr = std::unique_ptr<ArchiveEntryCompressionInfo>;

        ///////////////////////////////////////////////////////////////////////////////
        /// @brief A base class for querying encryption specific information on an entry in an archive.
        /// @details Not all archives will support the storage and/or query of all the information contained here.
        ///////////////////////////////////////
        class MEZZ_LIB ArchiveEntryEncryptionInfo
        {
        public:
            ///////////////////////////////////////////////////////////////////////////////
            // Type Information

            /// @brief Gets the algorithm used to encrypt the file referenced by the entry.
            /// @return Returns an enum value stating the encryption used to encrypt the file.
            virtual EncryptionAlgorithm GetEncryptionAlgorithm() const = 0;
        };//ArchiveEntryEncryptionInfo

        /// @brief Convenience pointer type for ArchiveEntryEncryptionInfo.
        using ArchiveEntryEncryptionInfoPtr = std::unique_ptr<ArchiveEntryEncryptionInfo>;

        ///////////////////////////////////////////////////////////////////////////////
        /// @brief A base class for querying information on a specific entry in an archive.
        /// @details An archive can be the intuitive compressed archive, the filesystem, a space in memory, or something
        /// over the network.  Any place a collection of files can be stored, really.  An entry is single collection
        /// of metadata relating to that one file.  This class specifically contains abstract methods for the query
        /// of the most common/basic information that is common between data in most or all of these places.  Not
        /// every archive will support all of the information covered by these abstract methods, but they should
        /// contain most of them.
        ///////////////////////////////////////
        class MEZZ_LIB ArchiveEntry
        {
        public:
            ///////////////////////////////////////////////////////////////////////////////
            // Type Information

            /// @brief Gets the type of archive this entry belongs to.
            /// @remarks Generally archives will each have their own Entry type, so this enum can be used to determine
            /// what this entry can be safely casted to in order to get more detailed metadata about the file.
            /// @return Returns an enum value describing what kind of Archive this entry belongs to.
            virtual ArchiveType GetArchiveType() const = 0;
            /// @brief Gets the type of entry this is in the Archive.
            /// @remarks See the EntryType enum for more detailed description of entry types.
            /// @return Returns an enum value describing the type of entry this is.
            virtual EntryType GetEntryType() const = 0;

            ///////////////////////////////////////////////////////////////////////////////
            // Basic Metadata

            /// @brief Gets the name of the file.
            /// @return Returns a String containing the name of the file this entry pertains to.
            virtual String GetName() const = 0;
            /// @brief Gets the path of the file.
            /// @return Returns a String containing the relative path to the file from the root of the Archive.
            virtual String GetPath() const = 0;
            /// @brief Gets a short description of the file.
            /// @return Returns a String containing a brief note about the file.
            virtual String GetComment() const = 0;
            /// @brief Gets the size of the file.
            /// @remarks Usually in compressed Archives, this will be the uncompressed size of the file.
            /// @return Returns the size of the file in the Archive in bytes.
            virtual size_t GetSize() const = 0;
            /// @brief Gets the time the file was created.
            /// @return Returns a UInt64 containing the time stamp of when the file was created.
            virtual UInt64 GetCreateTime() const = 0;
            /// @brief Gets the last time the file was accessed.
            /// @return Returns a UInt64 containing the time stamp of when the file was last opened.
            virtual UInt64 GetAccessTime() const = 0;
            /// @brief Gets the last time the file was modified.
            /// @return Returns a UInt64 containing the time stamp of when the file was last changed.
            virtual UInt64 GetModifyTime() const = 0;

            ///////////////////////////////////////////////////////////////////////////////
            // Simple Queries

            /// @brief Gets whether or not the file can only be read.
            /// @return Returns true if the file can only be read and not written, false otherwise.
            virtual Boole IsReadOnly() const = 0;
            /// @brief Gets whether or not the file is compressed.
            /// @return Returns true if some manner of compression has been applied to the file this entry refers to, false otherwise.
            virtual Boole IsCompressed() const = 0;
            /// @brief Gets whether or not the file is encrypted.
            /// @return Returns true if the file is protected by some encryption scheme, false otherwise.
            virtual Boole IsEncrypted() const = 0;

            ///////////////////////////////////////////////////////////////////////////////
            // Extra Metadata

            /// @brief Gets the metadata related to the compression of the entry.
            /// @return Returns a unique_ptr to the compression metadata if this entry is compressed, or nullptr if it's not.
            virtual ArchiveEntryCompressionInfoPtr GetCompressionInfo() const = 0;
            /// @brief Gets the metadata related to the encryption of the entry.
            /// @return Returns a unique_ptr to the encryption metadata if this entry is encrypted, or nullptr if it's not.
            virtual ArchiveEntryEncryptionInfoPtr GetEncryptionInfo() const = 0;
        };//ArchiveEntry

        /// @brief Convenience pointer type for ArchiveEntry.
        using ArchiveEntryPtr = std::unique_ptr<ArchiveEntry>;
    }//Resource
}//Mezzanine

#endif
