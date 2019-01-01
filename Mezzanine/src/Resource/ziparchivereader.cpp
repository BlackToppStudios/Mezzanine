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
#ifndef _resourceziparchivereader_cpp
#define _resourceziparchivereader_cpp

#include "Resource/ziparchivereader.h"
#include "Resource/zipstream.h"

#include "exception.h"
#include "stringtool.h"

#include "zip.h"

#include <regex>

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
}

namespace Mezzanine
{
    namespace Resource
    {
        ZipArchiveReader::ZipArchiveReader() :
            InternalArchive(nullptr),
            InternalSource(nullptr)
            {  }

        ZipArchiveReader::~ZipArchiveReader()
            { this->Close(); }

        ///////////////////////////////////////////////////////////////////////////////
        // Open / Close

        void ZipArchiveReader::Open(const String& Identifier)
        {
            if( this->IsOpen() ) {
                StringStream ExceptionStream;
                ExceptionStream << "Attempting to open an already opened Archive: \"";
                ExceptionStream << this->ArchiveIdentifier << "\".";
                MEZZ_EXCEPTION(ExceptionBase::INVALID_STATE_EXCEPTION,ExceptionStream.str());
            }

            zip_error_t ArchiveError;
            zip_error_init(&ArchiveError);
            zip_source_t* TempSource = zip_source_file_create(Identifier.c_str(),0,-1,&ArchiveError);
            if( ArchiveError.zip_err != ZIP_ER_OK ) {
                MEZZ_EXCEPTION(ExceptionBase::PARAMETERS_EXCEPTION,String(ArchiveError.str));
            }
            zip_t* TempArchive = zip_open_from_source(TempSource,0,&ArchiveError);
            if( ArchiveError.zip_err != ZIP_ER_OK ) {
                MEZZ_EXCEPTION(ExceptionBase::INVALID_STATE_EXCEPTION,String(ArchiveError.str));
            }

            this->InternalSource = TempSource;
            this->InternalArchive = TempArchive;
            this->ArchiveIdentifier.assign(Identifier);
        }

        void ZipArchiveReader::Open(const String& Identifier, Char8* Buffer, const size_t BufferSize, const Boole Owner)
        {
            if( this->IsOpen() ) {
                StringStream ExceptionStream;
                ExceptionStream << "Attempting to open an already opened Archive: \"";
                ExceptionStream << this->ArchiveIdentifier << "\".";
                MEZZ_EXCEPTION(ExceptionBase::INVALID_STATE_EXCEPTION,ExceptionStream.str());
            }

            zip_error_t ArchiveError;
            zip_error_init(&ArchiveError);
            zip_source_t* TempSource = zip_source_buffer_create(Buffer,BufferSize,Owner,&ArchiveError);
            if( ArchiveError.zip_err != ZIP_ER_OK ) {
                MEZZ_EXCEPTION(ExceptionBase::PARAMETERS_EXCEPTION,String(ArchiveError.str));
            }
            zip_t* TempArchive = zip_open_from_source(TempSource,0,&ArchiveError);
            if( ArchiveError.zip_err != ZIP_ER_OK ) {
                MEZZ_EXCEPTION(ExceptionBase::INVALID_STATE_EXCEPTION,String(ArchiveError.str));
            }

            this->InternalSource = TempSource;
            this->InternalArchive = TempArchive;
            this->ArchiveIdentifier.assign(Identifier);
        }

        Boole ZipArchiveReader::IsOpen() const
        {
            return this->InternalArchive != nullptr;
        }

        void ZipArchiveReader::Close()
        {
            if( this->IsOpen() ) {
                zip_close(this->InternalArchive);
                this->InternalArchive = nullptr;
                this->InternalSource = nullptr;
                this->ArchiveIdentifier.clear();
            }
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Utility Queries

        const String& ZipArchiveReader::GetIdentifier() const
        {
            return this->ArchiveIdentifier;
        }

        Boole ZipArchiveReader::DirectoryExists(const String& DirectoryPath) const
        {
            if( this->InternalArchive == nullptr ) {
                return false;
            }

            zip_stat_t EntryStat;
            zip_stat_init(&EntryStat);
            zip_flags_t LocateFlags = ZIP_FL_ENC_GUESS;
            zip_uint64_t EntryCount = static_cast<zip_uint64_t>(this->GetEntryCount());
            for( zip_uint64_t CurrIdx = 0 ; CurrIdx < EntryCount ; ++CurrIdx )
            {
                zip_stat_index(this->InternalArchive,CurrIdx,LocateFlags,&EntryStat);
                if( EntryStat.valid && DirectoryPath == EntryStat.name ) {
                    return true;
                }
            }
            return false;
        }

        Boole ZipArchiveReader::FileExists(const String& PathAndFile) const
        {
            if( this->InternalArchive == nullptr ) {
                return false;
            }

            zip_stat_t EntryStat;
            zip_stat_init(&EntryStat);
            zip_flags_t LocateFlags = ZIP_FL_ENC_GUESS;
            if( zip_stat(this->InternalArchive,PathAndFile.c_str(),LocateFlags,&EntryStat) < 0 ) {
                return false;
            }
            return ( EntryStat.valid & ZIP_STAT_INDEX && EntryStat.index >= 0 );
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Streaming

        IStreamPtr ZipArchiveReader::OpenIStream(const String& Identifier, const Whole Flags, const Boole Raw)
        {
            if( this->InternalArchive == nullptr ) {
                StringStream ExceptionStream;
                ExceptionStream << "Attempting to open stream \"" << Identifier << "\" in uninitialized zip archive.\n";
                MEZZ_EXCEPTION(ExceptionBase::INVALID_STATE_EXCEPTION,ExceptionStream.str());
            }
            ZipIStreamPtr NewStream = std::make_shared<ZipIStream>(this->InternalArchive);
            NewStream->OpenFile(Identifier,Flags,Raw);
            return NewStream;
        }

        IStreamPtr ZipArchiveReader::OpenEncryptedIStream(const String& Identifier, const String& Password,
                                                          const Whole Flags, const Boole Raw)
        {
            if( this->InternalArchive == nullptr ) {
                StringStream ExceptionStream;
                ExceptionStream << "Attempting to open stream \"" << Identifier << "\" in uninitialized zip archive.\n";
                MEZZ_EXCEPTION(ExceptionBase::INVALID_STATE_EXCEPTION,ExceptionStream.str());
            }
            ZipIStreamPtr NewStream = std::make_shared<ZipIStream>(this->InternalArchive);
            NewStream->OpenEncryptedFile(Identifier,Password,Flags,Raw);
            return NewStream;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Querying

        Int64 ZipArchiveReader::GetEntryCount() const
            { return zip_get_num_entries(this->InternalArchive,0); }

        ArchiveEntryPtr ZipArchiveReader::GetEntry(const UInt64 Index) const
        {
            zip_stat_t EntryStat;
            zip_stat_init(&EntryStat);
            if( zip_stat_index(this->InternalArchive,Index,0,&EntryStat) < 0 ) {
                return nullptr;
            }

            ArchiveEntryPtr Ret = std::make_unique<ArchiveEntry>();
            Ret->ArchType = AT_Zip;
            Ret->Name = EntryStat.name;
            Ret->Size = EntryStat.size;
            Ret->ModifyTime = EntryStat.mtime;
            Ret->CompressedSize = EntryStat.comp_size;
            Ret->CRC = EntryStat.crc;
            Ret->Attributes = Resource::FP_Read;
            if( !zip_get_archive_flag(this->InternalArchive,ZIP_AFL_RDONLY,0) ) {
                Ret->Attributes |= Resource::FP_Write;
            }

            zip_uint32_t CommentSize = 0;
            const char* Comment = zip_file_get_comment(this->InternalArchive,Index,&CommentSize,0);
            Ret->Comment.assign(Comment,CommentSize);

            Ret->EntType = ( Ret->Name.back() == '/' || Ret->Name.back() == '\\' ? ET_Directory : ET_File );
            Ret->CompressMethod = ConvertCompressionMethod( EntryStat.comp_method );
            Ret->EncryptMethod = ConvertEncryptionMethod( EntryStat.encryption_method );

            return Ret;
        }

        ArchiveEntryPtr ZipArchiveReader::GetEntry(const String& FileName) const
        {
            UInt64 FileIndex = static_cast<UInt64>( zip_name_locate(this->InternalArchive,FileName.c_str(),0) );
            return this->GetEntry(FileIndex);
        }

        ArchiveEntryVector ZipArchiveReader::GetEntries(const String& Pattern, const Boole OmitDirs) const
        {
            ArchiveEntryVector Ret;

            for( Int64 FileIdx = 0 ; FileIdx < this->GetEntryCount() ; ++FileIdx )
            {
                ArchiveEntry Ent;
                zip_stat_t EntryStat;
                zip_stat_init(&EntryStat);
                zip_stat_index(this->InternalArchive,FileIdx,0,&EntryStat);

                Ent.Name = EntryStat.name;
                if( !StringTools::PatternMatch(Ent.Name,Pattern,true) ) {
                    continue;
                }

                Ent.EntType = ( Ent.Name.back() == '/' || Ent.Name.back() == '\\' ? ET_Directory : ET_File );
                if( Ent.EntType == ET_Directory && OmitDirs ) {
                    continue;
                }

                Ent.ArchType = AT_Zip;
                Ent.Name = EntryStat.name;
                Ent.Size = EntryStat.size;
                Ent.ModifyTime = EntryStat.mtime;
                Ent.CompressedSize = EntryStat.comp_size;
                Ent.CRC = EntryStat.crc;
                Ent.Attributes = Resource::FP_Read;
                if( !zip_get_archive_flag(this->InternalArchive,ZIP_AFL_RDONLY,0) ) {
                    Ent.Attributes |= Resource::FP_Write;
                }

                zip_uint32_t CommentSize = 0;
                const char* Comment = zip_file_get_comment(this->InternalArchive,FileIdx,&CommentSize,0);
                Ent.Comment.assign(Comment,CommentSize);

                Ent.CompressMethod = ConvertCompressionMethod( EntryStat.comp_method );
                Ent.EncryptMethod = ConvertEncryptionMethod( EntryStat.encryption_method );

                Ret.push_back( std::move(Ent) );
            }

            return Ret;
        }
    }//Resource
}//Mezzanine

#endif
