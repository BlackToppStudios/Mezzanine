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
#ifndef _resourcezipstream_cpp
#define _resourcezipstream_cpp

#include "Resource/zipstream.h"

#include "exception.h"

#include <zip.h>

namespace Mezzanine
{
    namespace Resource
    {
        ZipStreamBuffer::ZipStreamBuffer(zip* Arch) :
            InternalArchive(Arch),
            InternalFile(nullptr),
            FileIndex(-1),
            StreamFlags(Mezzanine::SF_None),
            RawStream(false)
            {  }

        ZipStreamBuffer::~ZipStreamBuffer()
            { this->CloseFile(); }

        void ZipStreamBuffer::CheckOpenError()
        {
            if( this->InternalFile == nullptr ) {
                // Ruh Roh
                zip_error_t* ArchError = zip_get_error(this->InternalArchive);
                int ErrorCode = zip_error_code_zip(ArchError);

                // A small subset of file errors.
                StringStream ExceptionStream;
                ExceptionStream << zip_error_strerror(ArchError) << '\n';
                if( ErrorCode == ZIP_ER_COMPNOTSUPP || ErrorCode == ZIP_ER_ENCRNOTSUPP ) {
                    MEZZ_EXCEPTION(ExceptionBase::IO_FILE_EXCEPTION,ExceptionStream.str());
                }else if( ErrorCode == ZIP_ER_NOPASSWD || ErrorCode == ZIP_ER_WRONGPASSWD ) {
                    MEZZ_EXCEPTION(ExceptionBase::IO_FILE_PERMISSION_EXCEPTION,ExceptionStream.str());
                }else if( ErrorCode == ZIP_ER_READ || ErrorCode == ZIP_ER_SEEK ) {
                    MEZZ_EXCEPTION(ExceptionBase::IO_FILE_READ_EXCEPTION,ExceptionStream.str());
                }else if( ErrorCode == ZIP_ER_ZLIB ) {
                    MEZZ_EXCEPTION(ExceptionBase::INTERNAL_EXCEPTION,ExceptionStream.str());
                }else{
                    MEZZ_EXCEPTION(ExceptionBase::IO_FILE_EXCEPTION,ExceptionStream.str());
                }
            }
        }

        int ZipStreamBuffer::underflow()
        {
            if( this->gptr() && ( this->gptr() < this->egptr() ) ) {
                return *reinterpret_cast<Char8*>( this->gptr() );
            }

            if( !(this->StreamFlags & SF_Read) || !this->IsOpenToFile() ) {
                return traits_type::eof();
            }

            static const Integer PutBackMax = 6;
            Integer PutBack = this->gptr() - this->eback();
            if( PutBack > PutBackMax ) {
                PutBack = PutBackMax;
            }
            memcpy(this->Buffer,this->gptr() - PutBack,static_cast<size_t>(PutBack));

            zip_int64_t AmountRead = zip_fread(this->InternalFile,this->Buffer + PutBack,BufferSize - PutBack);
            if( AmountRead <= 0 ) {
                return traits_type::eof();
            }

            this->setg( this->Buffer, this->Buffer + PutBack, this->Buffer + BufferSize );
            return *reinterpret_cast<Char8*>( this->gptr() );
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        void ZipStreamBuffer::OpenFile(const String& Identifier, const Whole Flags, const Boole Raw)
        {
            zip_flags_t LocateFlags = ZIP_FL_ENC_GUESS;
            zip_int64_t FileIdx = zip_name_locate(this->InternalArchive,Identifier.c_str(),LocateFlags);
            if( FileIdx < 0 ) {
                StringStream ExceptionStream;
                ExceptionStream << "File \"" << Identifier << "\" not found in ZIP archive.\n";
                MEZZ_EXCEPTION(ExceptionBase::IO_FILE_NOT_FOUND_EXCEPTION,ExceptionStream.str());
            }

            zip_flags_t FOpenFlags = ( Raw ? ZIP_FL_COMPRESSED : 0 );
            this->InternalFile = zip_fopen_index(this->InternalArchive,FileIdx,FOpenFlags);

            this->CheckOpenError();

            this->FileIndex = FileIdx;
            this->StreamFlags = Flags;
            this->RawStream = Raw;
        }

        void ZipStreamBuffer::OpenEncryptedFile(const String& Identifier, const String& Password,
                                                const Whole Flags, const Boole Raw)
        {
            zip_flags_t LocateFlags = ZIP_FL_ENC_GUESS;
            zip_int64_t FileIdx = zip_name_locate(this->InternalArchive,Identifier.c_str(),LocateFlags);
            if( FileIdx < 0 ) {
                StringStream ExceptionStream;
                ExceptionStream << "File \"" << Identifier << "\" not found in ZIP archive.\n";
                MEZZ_EXCEPTION(ExceptionBase::IO_FILE_NOT_FOUND_EXCEPTION,ExceptionStream.str());
            }

            zip_flags_t FOpenFlags = ( Raw ? ZIP_FL_COMPRESSED : 0 );
            this->InternalFile = zip_fopen_index_encrypted(this->InternalArchive,FileIdx,FOpenFlags,Password.c_str());

            this->CheckOpenError();

            this->FileIndex = FileIdx;
            this->StreamFlags = Flags;
            this->RawStream = Raw;
        }

        Boole ZipStreamBuffer::IsOpenToFile() const
            { return ( this->InternalFile != nullptr ); }

        void ZipStreamBuffer::CloseFile()
        {
            if( this->IsOpenToFile() ) {
                zip_fclose(this->InternalFile);

                this->InternalFile = nullptr;
                this->FileIndex = -1;
                this->StreamFlags = Mezzanine::SF_None;
                this->RawStream = false;
            }
        }

        Whole ZipStreamBuffer::GetStreamFlags() const
            { return this->StreamFlags; }

        StreamSize ZipStreamBuffer::GetCompressedSize() const
        {
            if( this->FileIndex >= 0 ) {
                zip_stat_t FileStat;
                zip_stat_init(&FileStat);
                zip_stat_index(this->InternalArchive,this->FileIndex,0,&FileStat);
                if( FileStat.valid & ZIP_STAT_COMP_SIZE ) {
                    return static_cast<StreamSize>(FileStat.comp_size);
                }
            }
            return 0;
        }

        StreamSize ZipStreamBuffer::GetUncompressedSize() const
        {
            if( this->FileIndex >= 0 ) {
                zip_stat_t FileStat;
                zip_stat_init(&FileStat);
                zip_stat_index(this->InternalArchive,this->FileIndex,0,&FileStat);
                if( FileStat.valid & ZIP_STAT_SIZE ) {
                    return static_cast<StreamSize>(FileStat.size);
                }
            }
            return 0;
        }

        String ZipStreamBuffer::GetIdentifier() const
        {
            String Ret;
            if( this->FileIndex >= 0 ) {
                zip_flags_t NameFlags = ZIP_FL_ENC_GUESS;
                Ret.assign( zip_get_name(this->InternalArchive,this->FileIndex,NameFlags) );
            }
            return Ret;
        }

        StreamSize ZipStreamBuffer::GetSize() const
        {
            if( this->RawStream ) {
                return this->GetCompressedSize();
            }else{
                return this->GetUncompressedSize();
            }
        }

        Boole ZipStreamBuffer::CanSeek() const
        {
            if( this->FileIndex >= 0 ) {
                zip_stat_t FileStat;
                zip_stat_init(&FileStat);
                zip_stat_index(this->InternalArchive,this->FileIndex,0,&FileStat);
                Boole CompressValid = ( FileStat.valid & ZIP_STAT_COMP_METHOD ) && ( FileStat.comp_method == ZIP_CM_STORE );
                Boole EncryptValid = ( FileStat.valid & ZIP_STAT_ENCRYPTION_METHOD ) && ( FileStat.encryption_method == ZIP_EM_NONE );
                return ( CompressValid && EncryptValid );
            }
            return false;
        }

        Boole ZipStreamBuffer::IsEncrypted() const
        {
            if( this->FileIndex >= 0 ) {
                zip_stat_t FileStat;
                zip_stat_init(&FileStat);
                zip_stat_index(this->InternalArchive,this->FileIndex,0,&FileStat);
                return ( FileStat.encryption_method != ZIP_EM_NONE );
            }
            return false;
        }

        Boole ZipStreamBuffer::IsRaw() const
        {
            return this->RawStream;
        }
    }//Resource
}//Mezzanine

#endif
