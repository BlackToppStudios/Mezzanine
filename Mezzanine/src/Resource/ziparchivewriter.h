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
#ifndef _resourceziparchivewriter_h
#define _resourceziparchivewriter_h

#include "datastream.h"
#include "Resource/archivewriter.h"

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
        class MEZZ_LIB ZipArchiveWriter : public ArchiveWriter
        {
        /*
        protected:
            /// @brief The full path and name of the Archive
            String ArchiveIdentifier;
            /// @brief The flags used when opening this archive.
            Whole OpenFlags;
            /// @brief A pointer to the internal Archive.
            zip* InternalArchive;
            /// @brief A pointer to the source of the Archive data.
            zip_source* InternalSource;
        public:
            /// @brief Class constructor.
            ZipArchiveWriter();
            /// @brief Class destructor.
            virtual ~ZipArchiveWriter();

            ///////////////////////////////////////////////////////////////////////////////
            // Open / Close

            /// @copydoc ArchiveWriter::Open(const String&, const Whole)
            /// @remarks The AOF_Write flag is forced and the AOF_Read flag is ignored by this method.
            virtual void Open(const String& Identifier, const Whole Flags);
            /// @copydoc ArchiveWriter::Open(const String&, Char8*, const size_t, const Whole, const Boole)
            /// @remarks The AOF_Write flag is forced and the AOF_Read flag is ignored by this method.
            virtual void Open(const String& Identifier, Char8* Buffer, const size_t BufferSize, const Whole Flags, const Boole Owner);
            /// @copydoc ArchiveWriter::IsOpen() const
            virtual Boole IsOpen() const;
            /// @copydoc ArchiveWriter::Close()
            virtual void Close();

            /// @copydoc ArchiveWriter::GetFlags() const
            virtual Whole GetFlags() const;

            ///////////////////////////////////////////////////////////////////////////////
            // File and Directory Query

            /// @copydoc ArchiveWriter::DirectoryExists(const String&) const
            virtual Boole DirectoryExists(const String& DirectoryPath) const;
            /// @copydoc ArchiveWriter::FileExists(const String&) const
            virtual Boole FileExists(const String& PathAndFile) const;

            ///////////////////////////////////////////////////////////////////////////////
            // Streaming

            /// @copydoc ArchiveWriter::OpenStream(const char*, const Whole)
            virtual OStreamPtr OpenOStream(const char* Identifier, const Whole Flags);
            /// @copydoc ArchiveWriter::OpenStream(const String&, const Whole)
            virtual OStreamPtr OpenOStream(const String& Identifier, const Whole Flags);

            ///////////////////////////////////////////////////////////////////////////////
            // Default Parameters

            /// @copydoc ArchiveWriter::SetDefaultCompressionParameters(const CompressParameters&)
            virtual void SetDefaultCompressionParameters(const CompressParameters& CompParams);
            /// @copydoc ArchiveWriter::GetDefaultCompressionParameters() const
            virtual CompressParameters GetDefaultCompressionParameters() const;
            /// @copydoc ArchiveWriter::SetDefaultEncryptionParameters(const EncryptParameters&)
            virtual void SetDefaultEncryptionParameters(const EncryptParameters& EncryptParams);
            /// @copydoc ArchiveWriter::GetDefaultEncryptionParameters() const
            virtual EncryptParameters GetDefaultEncryptionParameters() const;

            ///////////////////////////////////////////////////////////////////////////////
            // Writing

            /// @copydoc ArchiveWriter::CreateDirectory(const String&)
            virtual Boole CreateDirectory(const String& DirectoryPath);
            /// @copydoc ArchiveWriter::CreateFile(const String&,std::istream&)
            virtual Boole CreateFile(const String& PathAndFile,
                                     std::istream& Input);
            /// @copydoc ArchiveWriter::CreateFile(const String&,std::istream&,const CompressParameters&)
            virtual Boole CreateFile(const String& PathAndFile,
                                     std::istream& Input,
                                     const CompressParameters& CompParams);
            /// @copydoc ArchiveWriter::CreateFile(const String&,std::istream&,const CompressParameters&,const EncryptParameters&)
            virtual Boole CreateFile(const String& PathAndFile,
                                     std::istream& Input,
                                     const CompressParameters& CompParams,
                                     const EncryptParameters& EncryptParams);
            /// @copydoc ArchiveWriter::CreateFile(const String&,void*,const Whole)
            virtual Boole CreateFile(const String& PathAndFile,
                                     void* Buffer,
                                     const Whole BufSize);
            /// @copydoc ArchiveWriter::CreateFile(const String&,void*,const Whole,const CompressParameters&)
            virtual Boole CreateFile(const String& PathAndFile,
                                     void* Buffer,
                                     const Whole BufSize,
                                     const CompressParameters& CompParams);
            /// @copydoc ArchiveWriter::CreateFile(const String&,void*,const Whole,const CompressParameters&,const EncryptParameters&)
            virtual Boole CreateFile(const String& PathAndFile,
                                     void* Buffer,
                                     const Whole BufSize,
                                     const CompressParameters& CompParams,
                                     const EncryptParameters& EncryptParams);
            //// @copydoc ArchiveWriter::MoveFile(const String&, const String&, const Boole)
            //virtual Boole MoveFile(const String& OldPathAndFile, const String& NewPathAndFile, const Boole FailIfExists);
            /// @copydoc ArchiveWriter::DeleteFile(const String&)
            virtual Boole DeleteFile(const String& PathAndFile);
            //*/
        };//ZipArchiveWriter
    }//Resource
}//Mezzanine

#endif
