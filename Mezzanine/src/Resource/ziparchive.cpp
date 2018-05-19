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
#ifndef _resourceziparchive_cpp
#define _resourceziparchive_cpp

#include "Resource/ziparchive.h"
#include "Resource/resourceutilities.h"

#include "zip.h"

namespace
{
    using namespace Mezzanine;
    using namespace Mezzanine::Resource;

    /// @brief Convenience conversion function for compression methods from zip fields.
    /// @param Method The method value specified in the Zip archive.
    /// @return Returns a CompressionMethod value corresponding to the method used.
    Resource::CompressionMethod ConvertCompressionMethod(const zip_uint16_t Method)
    {
        switch( Method )
        {
            case 8:   return CM_Deflate;    break;
            case 9:   return CM_Deflate64;  break;
            case 12:  return CM_BZip2;      break;
            case 14:  return CM_LZMA;       break;
            default:  return CM_Unknown;    break;
        }
    }

    /// @brief Convenience conversion function for encryption methods from zip fields.
    /// @param Method The method value specified in the Zip archive.
    /// @return Returns an EncryptionMethod value corresponding to the method used.
    Resource::EncryptionMethod ConvertEncryptionMethod(const zip_uint64_t Method)
    {
        switch( Method )
        {
            case ZIP_EM_TRAD_PKWARE:  return EA_PKWARE;   break;
            case ZIP_EM_AES_128:      return EA_AES_128;  break;
            case ZIP_EM_AES_192:      return EA_AES_192;  break;
            case ZIP_EM_AES_256:      return EA_AES_256;  break;
            default:                  return EA_Unknown;  break;
        }
    }

    size_t ZipReadFunct(void *pOpaque, zip_int64_t file_ofs, void *pBuf, size_t n)
    {

    }

    size_t ZipWriteFunct(void *pOpaque, zip_int64_t file_ofs, const void *pBuf, size_t n)
    {

    }

    bool ZipKeepAlive(void *pOpaque)
    {

    }
}

namespace Mezzanine
{
    namespace Resource
    {
        ZipArchive::ZipArchive(DataStreamPtr Stream) :
            ArchiveStream(Stream),
            InternalArchive(nullptr)
            {  }

        ZipArchive::ZipArchive(const String& FullName, const Whole Flags, DataStreamPtr Stream) :
            ArchivePath(GetDirName(FullName)),
            ArchiveName(GetBaseName(FullName)),
            ArchiveStream(Stream)
        {
            int ErrorCode = ZIP_ER_OK;
            this->InternalArchive = zip_open(FullName.c_str(),Flags,&ErrorCode);
        }

        ZipArchive::ZipArchive(const String& PathName, const String& FileName, const Whole Flags, DataStreamPtr Stream) :
            ArchivePath(PathName),
            ArchiveName(FileName),
            ArchiveStream(Stream)
        {
            int ErrorCode = ZIP_ER_OK;
            String FullName = CombinePathAndFileName(PathName,FileName);
            this->InternalArchive = zip_open(FullName.c_str(),Flags,&ErrorCode);
        }

        ZipArchive::~ZipArchive()
        {
            zip_close(this->InternalArchive);
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Configuration

        void ZipArchive::SetFlags(const Whole Flags)
        {

        }

        Whole ZipArchive::GetFlags() const
        {

        }

        ///////////////////////////////////////////////////////////////////////////////
        // Reading

        ///////////////////////////////////////////////////////////////////////////////
        // Writing

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        UInt64 ZipArchive::GetSize() const
            { return this->ArchiveStream->GetSize(); }

        Whole ZipArchive::GetFileCount() const
            { return zip_get_num_entries(this->InternalArchive,0); }

        ArchiveEntryPtr ZipArchive::GetEntry(const UInt64 Index) const
        {
            zip_stat_t EntryStat;
            zip_stat_init(&EntryStat);
            zip_stat_index(this->InternalArchive,Index,0,&EntryStat);

            ArchiveEntryPtr Ret = std::make_unique<ArchiveEntry>();
            Ret->ArchType = AT_Zip;
            Ret->Name = EntryStat.name;
            Ret->Size = EntryStat.size;
            Ret->ModifyTime = EntryStat.mtime;
            Ret->CompressedSize = EntryStat.comp_size;
            Ret->CRC = EntryStat.crc;
            Ret->ReadOnly = zip_get_archive_flag(this->InternalArchive,ZIP_AFL_RDONLY,0);

            zip_uint32_t CommentSize = 0;
            const char* Comment = zip_file_get_comment(this->InternalArchive,Index,&CommentSize,0);
            Ret->Comment.assign(Comment,CommentSize);

            Ret->EntType = ( Ret->Name.back() == '/' || Ret->Name.back() == '\\' ? ET_Directory : ET_File );
            Ret->CompressMethod = ConvertCompressionMethod( EntryStat.comp_method );
            Ret->EncryptMethod = ConvertEncryptionMethod( EntryStat.encryption_method );

            return Ret;
        }

        ArchiveEntryPtr ZipArchive::GetEntry(const String& FileName) const
        {
            UInt64 FileIndex = static_cast<UInt64>( zip_name_locate(this->InternalArchive,FileName.c_str(),0) );
            return this->GetEntry(FileIndex);
        }
    }//Resource
}//Mezzanine

#endif
