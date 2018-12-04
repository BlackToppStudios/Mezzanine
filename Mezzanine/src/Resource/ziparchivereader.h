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
        class MEZZ_LIB ZipArchiveReader : public ArchiveReader
        {
        protected:
            /// @brief The full path and name of the Archive
            String ArchiveIdentifier;
            /// @brief A pointer to the internal Archive.
            zip* InternalArchive;
            /// @brief A pointer to the source of the Archive data.
            zip_source* InternalSource;
        public:
            /// @brief Class constructor.
            ZipArchiveReader();
            /// @brief Class destructor.
            virtual ~ZipArchiveReader();

            ///////////////////////////////////////////////////////////////////////////////
            // Open / Close

            /// @copydoc ArchiveReader::Open(const String&)
            virtual void Open(const String& Identifier);
            /// @copydoc ArchiveReader::Open(const String&, Char8*, const size_t, const Boole)
            virtual void Open(const String& Identifier, Char8* Buffer, const size_t BufferSize, const Boole Owner);
            /// @copydoc ArchiveReader::IsOpen() const
            virtual Boole IsOpen() const;
            /// @copydoc ArchiveReader::Close()
            virtual void Close();

            ///////////////////////////////////////////////////////////////////////////////
            // File and Directory Query

            /// @copydoc ArchiveReader::DirectoryExists(const String&) const
            virtual Boole DirectoryExists(const String& DirectoryPath) const;
            /// @copydoc ArchiveReader::FileExists(const String&) const
            virtual Boole FileExists(const String& PathAndFile) const;

            ///////////////////////////////////////////////////////////////////////////////
            // Streaming

            /// @copydoc ArchiveReader::OpenIStream(const String&, const Whole, const Boole)
            virtual IStreamPtr OpenIStream(const String& Identifier,
                                           const Whole Flags = SF_Read,
                                           const Boole Raw = false);
            /// @copydoc ArchiveReader::OpenIStream(const String&, const String&, const Whole, const Boole)
            virtual IStreamPtr OpenEncryptedIStream(const String& Identifier,
                                                    const String& Password,
                                                    const Whole Flags = SF_Read,
                                                    const Boole Raw = false);

            ///////////////////////////////////////////////////////////////////////////////
            // Reading / Entry Query

            /// @copydoc ArchiveReader::GetEntryCount() const
            virtual Int64 GetEntryCount() const;
            /// @copydoc ArchiveReader::GetEntry(const UInt64) const
            virtual ArchiveEntryPtr GetEntry(const UInt64 Index) const;
            /// @copydoc ArchiveReader::GetEntry(const String&) const
            /// @note This will only lookup entries for files, not directories.
            virtual ArchiveEntryPtr GetEntry(const String& FileName) const;
            /// @copydoc ArchiveReader::GetEntries(const String& Pattern) const
            virtual ArchiveEntryVector GetEntries(const String& Pattern, const Boole OmitDirs) const;
        };//ZipArchiveReader
    }//Resource
}//Mezzanine

#endif
