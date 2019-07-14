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
#ifndef _resourceziparchivereader_h
#define _resourceziparchivereader_h

#include "datastream.h"
#include "Resource/archivereader.h"

// Internal Forward Declare
struct zip;
struct zip_source;

namespace Mezzanine
{
    namespace Resource
    {
        ///////////////////////////////////////////////////////////////////////////////
        /// @brief This is an Archive implementation for reading and writing .zip files.
        ///////////////////////////////////////
        class MEZZ_LIB ZipArchiveReader final : public ArchiveReader
        {
        protected:
            /// @brief The full path and name of the Archive.
            String ArchiveIdentifier;
            /// @brief The unique name of the group this reader belongs to.
            String ArchiveGroup;
            /// @brief The Password to use for protected files when one isn't explicitly provided.
            String DefaultPassword;
            /// @brief A pointer to the internal Archive.
            zip* InternalArchive;
            /// @brief A pointer to the source of the Archive data.
            zip_source* InternalSource;
        public:
            /// @brief Class constructor.
            ZipArchiveReader();
            /// @brief Class destructor.
            ~ZipArchiveReader();

            ///////////////////////////////////////////////////////////////////////////////
            // Open / Close

            /// @copydoc ArchiveReader::Open(const String&,const String&)
            void Open(const String& Identifier, const String& Group) override;
            /// @copydoc ArchiveReader::Open(const String&,const String&, Char8*, const size_t, const Boole)
            void Open(const String& Identifier,
                      const String& Group,
                      Char8* Buffer,
                      const size_t BufferSize,
                      const Boole Owner) override;

            /// @copydoc ArchiveReader::Open(const String&,const String&,const String&)
            /// @remarks The Password provided here will also be used as the default password for file I/O.
            void Open(const String& Identifier, const String& Group, const String& Password) override;
            /// @copydoc ArchiveReader::Open(const String&,const String&,const String&,Char8*,const size_t,const Boole)
            /// @remarks The Password provided here will also be used as the default password for file I/O.
            void Open(const String& Identifier,
                      const String& Group,
                      const String& Password,
                      Char8* Buffer,
                      const size_t BufferSize,
                      const Boole Owner) override;

            /// @copydoc ArchiveReader::IsOpen() const
            Boole IsOpen() const override;
            /// @copydoc ArchiveReader::Close()
            void Close() override;

            ///////////////////////////////////////////////////////////////////////////////
            // Default Password

            /// @copydoc ArchiveReader::SetDefaultPassword(const String&)
            void SetDefaultPassword(const String& Password) override;
            /// @copydoc ArchiveReader::GetDefaultPassword() const
            String GetDefaultPassword() const override;

            ///////////////////////////////////////////////////////////////////////////////
            // Utility Queries

            /// @copydoc ArchiveReader::GetIdentifier() const
            const String& GetIdentifier() const override;
            /// @copydoc ArchiveReader::IsCaseSensitive() const
            Boole IsCaseSensitive() const override;

            /// @copydoc ArchiveReader::DirectoryExists(const String&) const
            Boole DirectoryExists(const String& DirectoryPath) const override;
            /// @copydoc ArchiveReader::FileExists(const String&) const
            Boole FileExists(const String& PathAndFile) const override;

            ///////////////////////////////////////////////////////////////////////////////
            // Streaming

            /// @copydoc ArchiveReader::OpenIStream(const String&, const Whole, const Boole)
            IStreamPtr OpenIStream(const String& Identifier,
                                   const Whole Flags = SF_Read,
                                   const Boole Raw = false) override;
            /// @copydoc ArchiveReader::OpenIStream(const String&, const String&, const Whole, const Boole)
            IStreamPtr OpenEncryptedIStream(const String& Identifier,
                                            const String& Password,
                                            const Whole Flags = SF_Read,
                                            const Boole Raw = false) override;

            ///////////////////////////////////////////////////////////////////////////////
            // Reading / Entry Query

            /// @copydoc ArchiveReader::GetEntryCount() const
            Int64 GetEntryCount() const override;
            /// @copydoc ArchiveReader::GetEntry(const UInt64) const
            ArchiveEntryPtr GetEntry(const UInt64 Index) const override;
            /// @copydoc ArchiveReader::GetEntry(const String&) const
            /// @note This will only lookup entries for files, not directories.
            ArchiveEntryPtr GetEntry(const String& FileName) const override;
            /// @copydoc ArchiveReader::GetEntries(const String& Pattern) const
            ArchiveEntryVector GetEntries(const String& Pattern, const Boole OmitDirs) const override;
        };//ZipArchiveReader
    }//Resource
}//Mezzanine

#endif
