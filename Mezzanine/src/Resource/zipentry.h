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
#ifndef _resourcezipentry_h
#define _resourcezipentry_h

#include "Resource/archiveentry.h"

// Internal forward declares
struct mz_zip_archive_file_stat;

namespace Mezzanine
{
    namespace Resource
    {
        ///////////////////////////////////////////////////////////////////////////////
        /// @brief A compression information class for the ZipEntry.
        ///////////////////////////////////////
        class MEZZ_LIB ZipEntryCompressionInfo : public ArchiveEntryCompressionInfo
        {
        protected:
            /// @brief A pointer to the internal Stat struct containing the information to be queried.
            mz_zip_archive_file_stat* FileStat;
        public:
            ///////////////////////////////////////////////////////////////////////////////
            // Construction, Destruction, and Assignment

            /// @brief Default constructor.
            ZipEntryCompressionInfo() = delete;
            /// @brief Internal constructor.
            /// @param Stat A pointer to the internal struct storing all of our information.
            ZipEntryCompressionInfo(mz_zip_archive_file_stat* Stat);
            /// @brief Copy constructor.
            /// @param Other The other compression info to NOT be copied.
            ZipEntryCompressionInfo(const ZipEntryCompressionInfo& Other) = delete;
            /// @brief Move constructor.
            /// @param Other The other compression info to be moved.
            ZipEntryCompressionInfo(ZipEntryCompressionInfo&& Other) = default;
            /// @brief Class destructor.
            ~ZipEntryCompressionInfo() = default;

            /// @brief Copy assignment operator.
            /// @param Other The other compression info to NOT be copied.
            /// @return Returns a reference to this.
            ZipEntryCompressionInfo& operator=(const ZipEntryCompressionInfo& Other) = delete;
            /// @brief Move assignment operator.
            /// @param Other The other compression info to be moved.
            /// @return Returns a reference to this.
            ZipEntryCompressionInfo& operator=(ZipEntryCompressionInfo&& Other) = default;

            ///////////////////////////////////////////////////////////////////////////////
            // Type Information

            /// @copydoc ArchiveEntryCompressionInfo::GetCompressionMethod() const
            virtual CompressionMethod GetCompressionMethod() const override;

            ///////////////////////////////////////////////////////////////////////////////
            // Basic Metadata

            /// @copydoc ArchiveEntryCompressionInfo::GetCompressedSize() const
            virtual size_t GetCompressedSize() const override;
            /// @copydoc ArchiveEntryCompressionInfo::GetCompressVersion() const
            virtual UInt16 GetCompressVersion() const override;
            /// @copydoc ArchiveEntryCompressionInfo::GetDecompressVersion() const
            virtual UInt16 GetDecompressVersion() const override;
            /// @copydoc ArchiveEntryCompressionInfo::GetCRC() const
            virtual UInt32 GetCRC() const override;
        };//ZipEntryCompressionInfo

        ///////////////////////////////////////////////////////////////////////////////
        /// @brief A encryption information class for the ZipEntry.
        ///////////////////////////////////////
        class MEZZ_LIB ZipEntryEncryptionInfo : public ArchiveEntryEncryptionInfo
        {
        protected:
            /// @brief A pointer to the internal Stat struct containing the information to be queried.
            mz_zip_archive_file_stat* FileStat;
        public:
            ///////////////////////////////////////////////////////////////////////////////
            // Construction, Destruction, and Assignment

            /// @brief Default constructor.
            ZipEntryEncryptionInfo() = delete;
            /// @brief Internal constructor.
            /// @param Stat A pointer to the internal struct storing all of our information.
            ZipEntryEncryptionInfo(mz_zip_archive_file_stat* Stat);
            /// @brief Copy constructor.
            /// @param Other The other encryption info to NOT be copied.
            ZipEntryEncryptionInfo(const ZipEntryEncryptionInfo& Other) = delete;
            /// @brief Move constructor.
            /// @param Other The other encryption info to be moved.
            ZipEntryEncryptionInfo(ZipEntryEncryptionInfo&& Other) = default;
            /// @brief Class destructor.
            ~ZipEntryEncryptionInfo() = default;

            /// @brief Copy assignment operator.
            /// @param Other The other encryption info to NOT be copied.
            /// @return Returns a reference to this.
            ZipEntryEncryptionInfo& operator=(const ZipEntryEncryptionInfo& Other) = delete;
            /// @brief Move assignment operator.
            /// @param Other The other encryption info to be moved.
            /// @return Returns a reference to this.
            ZipEntryEncryptionInfo& operator=(ZipEntryEncryptionInfo&& Other) = default;

            ///////////////////////////////////////////////////////////////////////////////
            // Type Information

            /// @copydoc ArchiveEntryEncryptionInfo::GetEncryptionAlgorithm() const
            virtual EncryptionAlgorithm GetEncryptionAlgorithm() const override;
        };//ZipEntryEncryptionInfo

        ///////////////////////////////////////////////////////////////////////////////
        /// @brief An archive entry for the ZIP file format.
        ///////////////////////////////////////
        class MEZZ_LIB ZipEntry : public ArchiveEntry
        {
        protected:
            /// @brief A pointer to the internal Stat struct containing the information to be queried.
            mz_zip_archive_file_stat* FileStat;
            /// @brief A pointer to the query class for our entry's compression info.
            ZipEntryCompressionInfoPtr CompressInfo;
            /// @brief A pointer to the query class for our entry's encryption info.
            ZipEntryEncryptionInfoPtr EncryptInfo;
        public:
            ///////////////////////////////////////////////////////////////////////////////
            // Construction, Destruction, and Assignment

            /// @brief Default constructor.
            ZipEntry() = delete;
            /// @brief Internal constructor.
            /// @param Stat A pointer to the internal struct storing all of our information.
            ZipEntry(mz_zip_archive_file_stat* Stat);
            /// @brief Copy constructor.
            /// @param Other The other entry to NOT be copied.
            ZipEntry(const ZipEntry& Other) = delete;
            /// @brief Move constructor.
            /// @param Other The other entry to be moved.
            ZipEntry(ZipEntry&& Other) = default;
            /// @brief Class destructor.
            ~ZipEntry() = default;

            /// @brief Copy assignment operator.
            /// @param Other The other entry to NOT be copied.
            /// @return Returns a reference to this.
            ZipEntry& operator=(const ZipEntry& Other) = delete;
            /// @brief Move assignment operator.
            /// @param Other The other entry to be moved.
            /// @return Returns a reference to this.
            ZipEntry& operator=(ZipEntry&& Other) = default;

            ///////////////////////////////////////////////////////////////////////////////
            // Type Information

            /// @copydoc ArchiveEntry::GetArchiveType() const
            virtual ArchiveType GetArchiveType() const override;
            /// @copydoc ArchiveEntry::GetEntryType() const
            virtual EntryType GetEntryType() const override;

            ///////////////////////////////////////////////////////////////////////////////
            // Basic Metadata

            /// @copydoc ArchiveEntry::GetName() const
            virtual String GetName() const override;
            /// @copydoc ArchiveEntry::GetPath() const
            virtual String GetPath() const override;
            /// @copydoc ArchiveEntry::GetComment() const
            virtual String GetComment() const override;
            /// @copydoc ArchiveEntry::GetSize() const
            virtual size_t GetSize() const override;
            /// @copydoc ArchiveEntry::GetCreateTime() const
            virtual UInt64 GetCreateTime() const override;
            /// @copydoc ArchiveEntry::GetAccessTime() const
            virtual UInt64 GetAccessTime() const override;
            /// @copydoc ArchiveEntry::GetModifyTime() const
            virtual UInt64 GetModifyTime() const override;

            ///////////////////////////////////////////////////////////////////////////////
            // Simple Queries

            /// @copydoc ArchiveEntry::IsReadOnly() const
            virtual Boole IsReadOnly() const override;
            /// @copydoc ArchiveEntry::IsCompressed() const
            virtual Boole IsCompressed() const override;
            /// @copydoc ArchiveEntry::IsEncrypted() const
            virtual Boole IsEncrypted() const override;

            ///////////////////////////////////////////////////////////////////////////////
            // Extra Metadata

            /// @copydoc ArchiveEntry::GetCompressionInfo() const
            ArchiveEntryCompressionInfoPtr GetCompressionInfo() const override;
            /// @copydoc ArchiveEntry::GetEncryptionInfo() const
            ArchiveEntryEncryptionInfoPtr GetEncryptionInfo() const override;

            ///////////////////////////////////////////////////////////////////////////////
            // Zip Metadata

            /// @brief Gets the
            /// @return
            virtual StreamPos GetHeaderOffset() const;
        };//ZipEntry
    }//Resource
}//Mezzanine

#endif
