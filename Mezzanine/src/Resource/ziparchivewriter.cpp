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
#ifndef _resourceziparchivewriter_cpp
#define _resourceziparchivewriter_cpp

#include "Resource/ziparchivewriter.h"

#include "exception.h"
#include "stringtool.h"

#include "zip.h"

namespace Mezzanine
{
    namespace Resource
    {
        /*
        ZipArchiveWriter::ZipArchiveWriter() :
            InternalArchive(nullptr),
            InternalSource(nullptr)
            {  }

        ZipArchiveWriter::~ZipArchiveWriter()
            { this->Close(); }

        ///////////////////////////////////////////////////////////////////////////////
        // Open / Close

        void ZipArchiveWriter::Open(const String& Identifier, const Whole Flags)
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
            zip_t* TempArchive = zip_open_from_source(this->InternalSource,0,&ArchiveError);
            if( ArchiveError.zip_err != ZIP_ER_OK ) {
                MEZZ_EXCEPTION(ExceptionBase::INVALID_STATE_EXCEPTION,String(ArchiveError.str));
            }

            this->OpenFlags = Flags;
            this->InternalSource = TempSource;
            this->InternalArchive = TempArchive;
            this->ArchiveIdentifier.assign(Identifier);
        }

        void ZipArchiveWriter::Open(const String& Identifier, Char8* Buffer, const size_t BufferSize, const Whole Flags, const Boole Owner)
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
            zip_t* TempArchive = zip_open_from_source(this->InternalSource,0,&ArchiveError);
            if( ArchiveError.zip_err != ZIP_ER_OK ) {
                MEZZ_EXCEPTION(ExceptionBase::INVALID_STATE_EXCEPTION,String(ArchiveError.str));
            }

            this->OpenFlags = Flags;
            this->InternalSource = TempSource;
            this->InternalArchive = TempArchive;
            this->ArchiveIdentifier.assign(Identifier);
        }

        Boole ZipArchiveWriter::IsOpen() const
        {
            return this->InternalArchive != nullptr;
        }

        void ZipArchiveWriter::Close()
        {
            if( this->IsOpen() ) {
                zip_close(this->InternalArchive);
                this->InternalArchive = nullptr;
                this->InternalSource = nullptr;
                this->OpenFlags = AOF_None;
                this->ArchiveIdentifier.clear();
            }
        }

        Whole ZipArchiveWriter::GetFlags() const
        {
            return this->OpenFlags;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // File and Directory Query

        Boole ZipArchiveWriter::DirectoryExists(const String& DirectoryPath) const
        {
            if( this->InternalArchive == nullptr ) {
                return false;
            }

            zip_stat_t EntryStat;
            zip_stat_init(&EntryStat);
            zip_flags_t LocateFlags = ZIP_FL_ENC_GUESS;
            zip_uint64_t EntryCount = static_cast<zip_uint64_t>( zip_get_num_entries(this->InternalArchive,0) );
            for( zip_uint64_t CurrIdx = 0 ; CurrIdx < EntryCount ; ++CurrIdx )
            {
                zip_stat_index(this->InternalArchive,CurrIdx,LocateFlags,&EntryStat);
                if( EntryStat.valid && DirectoryPath == EntryStat.name ) {
                    return true;
                }
            }
            return false;
        }

        Boole ZipArchiveWriter::FileExists(const String& PathAndFile) const
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

        OStreamPtr ZipArchiveWriter::OpenOStream(const char* Identifier, const Whole Flags)
        {
            return nullptr;
        }

        OStreamPtr ZipArchiveWriter::OpenOStream(const String& Identifier, const Whole Flags)
        {
            return nullptr;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Default Parameters

        ///////////////////////////////////////////////////////////////////////////////
        // Writing

        Boole ZipArchiveWriter::CreateDirectory(const String& DirectoryPath)
        {
            zip_flags_t LocateFlags = ZIP_FL_ENC_GUESS;
            return ( zip_dir_add(this->InternalArchive,DirectoryPath.c_str(),LocateFlags) > 0 );
        }

        Boole ZipArchiveWriter::CreateFile(const String& PathAndFile, std::istream& Input, const Whole CompressType, const Whole EncryptType)
        {
            zip_flags_t LocateFlags = ZIP_FL_ENC_GUESS;
            zip_uint64_t FileIdx = zip_name_locate(this->InternalArchive,PathAndFile.c_str(),LocateFlags);
            if( FileIdx >= 0 ) {
                return false;
            }
            // Create the source
            return true;
        }

        Boole ZipArchiveWriter::CreateFile(const String& PathAndFile, void* Buffer, const Whole BufSize, const Whole CompressType, const Whole EncryptType)
        {
            zip_flags_t LocateFlags = ZIP_FL_ENC_GUESS;
            zip_uint64_t FileIdx = zip_name_locate(this->InternalArchive,PathAndFile.c_str(),LocateFlags);
            if( FileIdx >= 0 ) {
                return false;
            }
            // Create the source
            zip_source_t* BufferSource = zip_source_buffer(this->InternalArchive,Buffer,BufSize,0);
            if( BufferSource == nullptr ) {
                return false;
            }
            // Add the file
            FileIdx = zip_file_add(this->InternalArchive,PathAndFile.c_str(),BufferSource,LocateFlags);
            if( FileIdx < 0 ) {
                zip_source_free(BufferSource);
                return false;
            }
            // Set compression and encryption
            //Boole AfterCreateSuccess = false;
            //AfterCreateSuccess = ( zip_set_file_compression(this->InternalArchive,FileIdx,CompressType,) < 0 );
            //AfterCreateSuccess = ( zip_file_set_encryption(this->InternalArchive,FileIdx,,) < 0 );

            zip_source_free(BufferSource);
            return true;
        }

        Boole ZipArchiveWriter::MoveFile(const String& OldPathAndFile, const String& NewPathAndFile, const Boole FailIfExists)
        {
            zip_flags_t LocateFlags = ZIP_FL_ENC_GUESS;
            zip_uint64_t FileIdx = zip_name_locate(this->InternalArchive,OldPathAndFile.c_str(),LocateFlags);
            if( FileIdx >= 0 ) {
                return false;
            }
            return ( zip_file_rename(this->InternalArchive,FileIdx,NewPathAndFile.c_str(),LocateFlags) == 0 );
        }

        Boole ZipArchiveWriter::DeleteFile(const String& PathAndFile)
        {
            zip_flags_t LocateFlags = ZIP_FL_ENC_GUESS;
            zip_uint64_t FileIdx = zip_name_locate(this->InternalArchive,PathAndFile.c_str(),LocateFlags);
            return ( zip_delete(this->InternalArchive,FileIdx) == 0 );
        }//*/
    }//Resource
}//Mezzanine

#endif
