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
#ifndef _resourcefilesystemarchivereader_h
#define _resourcefilesystemarchivereader_h

#include "datastream.h"
#include "Resource/archivereader.h"

namespace Mezzanine
{
    namespace Resource
    {
        ///////////////////////////////////////////////////////////////////////////////
        /// @brief This is an Archive implementation for reading and writing files as is on disk.
        ///////////////////////////////////////
        class MEZZ_LIB FileSystemArchiveReader final : public ArchiveReader
        {
        protected:
            /// @brief The full path and name of the Archive.
            String ArchiveIdentifier;
            /// @brief The unique name of the group this reader belongs to.
            String ArchiveGroup;

            /// @brief Verifies a given path is valid to use with this archive.
            /// @param PathStr A String containing the path to check.
            /// @return Returns true if the path is valid, false otherwise.
            Boole VerifyValidPath(const String& PathStr) const;
            /// @brief Verifies a given path is valid to use with this archive.
            /// @exception If the Path is not valid, I.E. not a subdirectory or file this archive
            /// points to, a PARAMETERS_EXCEPTION will be thrown.
            /// @param PathStr A String containing the path to check.
            void VerifyValidPathExcept(const String& PathStr) const;
        public:
            /// @brief Class constructor.
            FileSystemArchiveReader() = default;
            /// @brief Class destructor.
            ~FileSystemArchiveReader() = default;

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
            void Open(const String& Identifier, const String& Group, const String& Password) override;
            /// @copydoc ArchiveReader::Open(const String&,const String&,const String&,Char8*,const size_t,const Boole)
            void Open(const String& Identifier,
                      const String& Group,
                      const String& Password,
                      Char8* Buffer,
                      const size_t BufferSize,
                      const Boole Owner) override;

            /// @copydoc ArchiveReader::IsOpen() const
            Boole IsOpen() const override;
            /// @copydoc ArchiveReader::Close()
            void Close();

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
                                   const Whole Flags,
                                   const Boole Raw) override;
            /// @copydoc ArchiveReader::OpenIStream(const String&, const String&, const Whole, const Boole)
            IStreamPtr OpenEncryptedIStream(const String& Identifier,
                                            const String& Password,
                                            const Whole Flags,
                                            const Boole Raw) override;

            ///////////////////////////////////////////////////////////////////////////////
            // Querying

            /// @copydoc ArchiveReader::GetEntryCount() const
            Int64 GetEntryCount() const override;
            /// @copydoc ArchiveReader::GetEntry(const UInt64) const
            ArchiveEntryPtr GetEntry(const UInt64 Index) const override;
            /// @copydoc ArchiveReader::GetEntry(const String&) const
            ArchiveEntryPtr GetEntry(const String& FileName) const override;
            /// @copydoc ArchiveReader::GetEntries(const String& Pattern) const
            ArchiveEntryVector GetEntries(const String& Pattern, const Boole OmitDirs) const override;
        };//FileSystemArchiveReader
    }//Resource
}//Mezzanine

#endif
