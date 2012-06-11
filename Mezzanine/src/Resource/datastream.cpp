//© Copyright 2010 - 2012 BlackTopp Studios Inc.
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
#ifndef _resourcedatastream_cpp
#define _resourcedatastream_cpp

#include "Resource/datastream.h"
#include "stringtool.h"

#include <cstring>
#include <cstdio>
#include <fstream>
#include <algorithm>

#define TEMP_STREAM_SIZE 128

namespace Mezzanine
{
    namespace Resource
    {
        ///////////////////////////////////////////////////////////////////////////////
        // DataStream Methods
        ///////////////////////////////////////
        DataStream::DataStream(const AccessMode& Mode)
            : ResourceName(""),
              Access(Mode),
              Size(0)
        {
        }

        DataStream::DataStream(const String& Name, const AccessMode& Mode)
            : ResourceName(Name),
              Access(Mode),
              Size(0)
        {
        }

        DataStream::~DataStream()
        {
        }

        ConstString& DataStream::GetName() const
        {
            return ResourceName;
        }

        DataStream::AccessMode DataStream::GetAccessMode() const
        {
            return Access;
        }

        size_t DataStream::GetSize() const
        {
            return Size;
        }

        bool DataStream::IsReadable() const
        {
            return (Access & DataStream::DS_Read);
        }

        bool DataStream::IsWriteable() const
        {
            return (Access & DataStream::DS_Write);
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Stream Access and Manipulation

        String DataStream::GetAsString()
        {
            size_t BufferSize = (this->Size > 0 ? this->Size : 4096);
            char* Buffer = new char[BufferSize];

            Seek(0);
            String Ret;
            while (!EoF())
            {
                size_t BytesRead = Read(Buffer,BufferSize);
                Ret.append(Buffer,BytesRead);
            }
            delete[] Buffer;
            return Ret;
        }

        size_t DataStream::ReadLine(Character* Buffer, size_t MaxCount, const String& Delim)
        {
            bool TrimCR = false;
            if(Delim.find_first_of('\n') != String::npos)
            {
                TrimCR = true;
            }

            char Temp[TEMP_STREAM_SIZE];
            size_t ChunkSize = std::min(MaxCount,(size_t)TEMP_STREAM_SIZE - 1);
            size_t TotalCount = 0;
            size_t ReadCount = 0;

            while(ChunkSize && (ReadCount = Read(Temp,ChunkSize)) != 0)
            {
                Temp[ReadCount] = '\0';
                size_t Pos = std::strcspn(Temp,Delim.c_str());

                if(Pos < ReadCount)
                {
                    Skip((long)(Pos + 1 - ReadCount));
                }

                if(Buffer)
                {
                    std::memcpy(Buffer + TotalCount,Temp,Pos);
                }
                TotalCount += Pos;

                if(Pos < ReadCount)
                {
                    if(TrimCR && TotalCount && Buffer[TotalCount - 1] == '\r')
                    {
                        --TotalCount;
                    }
                    break;
                }

                ChunkSize = std::min(MaxCount - TotalCount,(size_t)TEMP_STREAM_SIZE - 1);
            }
            Buffer[TotalCount] = '\0';
            return TotalCount;
        }

        String DataStream::GetLine(bool Trim)
        {
            char Temp[TEMP_STREAM_SIZE];
            String Ret;
            size_t ReadCount;

            while( (ReadCount = Read(Temp,TEMP_STREAM_SIZE - 1)) != 0 )
            {
                Temp[ReadCount] = '\0';

                char* Pos = std::strchr(Temp,'\n');
                if(Pos != 0)
                {
                    Skip((long)(Pos + 1 - Temp - ReadCount));
                    *Pos = '\0';
                }

                Ret += Temp;

                if(Pos != 0)
                {
                    if(Ret.length() && Ret[Ret.length() - 1] == '\r')
                    {
                        Ret.erase(Ret.length() - 1, 1);
                    }
                    break;
                }
            }

            if(Trim)
            {
                StringTool::Trim(Ret);
            }

            return Ret;
        }

        size_t DataStream::SkipLine(const String& Delim)
        {
            char Temp[TEMP_STREAM_SIZE];
            size_t TotalBytes = 0;
            size_t ReadCount = 0;

            while( (ReadCount = Read(Temp,TEMP_STREAM_SIZE - 1)) != 0 )
            {
                Temp[ReadCount] = '\0';
                size_t Position = std::strcspn(Temp,Delim.c_str());

                if(Position < ReadCount)
                {
                    Skip((long)(Position + 1 - ReadCount));
                    TotalBytes += Position + 1;
                    break;
                }

                TotalBytes += ReadCount;
            }

            return TotalBytes;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // MemoryDataStream Methods
        ///////////////////////////////////////
        MemoryDataStream::MemoryDataStream(const size_t& BufferSize, bool FreeOnClose, bool ReadOnly)
            : DataStream( ReadOnly ? DataStream::DS_Read : static_cast<DataStream::AccessMode>(DataStream::DS_Read | DataStream::DS_Write) ),
              FreeBuffer(FreeOnClose)
        {
            Size = BufferSize;
            BufferStart = new UInt8[BufferSize];
            BufferPos = BufferStart;
            BufferEnd = BufferStart + BufferSize;

            if(BufferEnd < BufferStart)
                MEZZ_EXCEPTION(Exception::MM_OUT_OF_BOUNDS_EXCEPTION,"Using a zero or negative size buffer");
        }

        MemoryDataStream::MemoryDataStream(const String& Name, const size_t& BufferSize, bool FreeOnClose, bool ReadOnly)
            : DataStream( Name, ReadOnly ? DataStream::DS_Read : static_cast<DataStream::AccessMode>(DataStream::DS_Read | DataStream::DS_Write) ),
              FreeBuffer(FreeOnClose)
        {
            Size = BufferSize;
            BufferStart = new UInt8[BufferSize];
            BufferPos = BufferStart;
            BufferEnd = BufferStart + BufferSize;

            if(BufferEnd < BufferStart)
                MEZZ_EXCEPTION(Exception::MM_OUT_OF_BOUNDS_EXCEPTION,"Using a zero or negative size buffer");
        }

        MemoryDataStream::MemoryDataStream(void* Buffer, const size_t& BufferSize, bool FreeOnClose, bool ReadOnly)
            : DataStream( ReadOnly ? DataStream::DS_Read : static_cast<DataStream::AccessMode>(DataStream::DS_Read | DataStream::DS_Write) ),
              FreeBuffer(FreeOnClose)
        {
            BufferStart = BufferPos = static_cast<UInt8*>(Buffer);
            Size = BufferSize;
            BufferEnd = BufferStart + BufferSize;

            if(BufferEnd < BufferStart)
                MEZZ_EXCEPTION(Exception::MM_OUT_OF_BOUNDS_EXCEPTION,"Using a zero or negative size buffer");
        }

        MemoryDataStream::MemoryDataStream(const String& Name, void* Buffer, const size_t& BufferSize, bool FreeOnClose, bool ReadOnly)
            : DataStream( Name, ReadOnly ? DataStream::DS_Read : static_cast<DataStream::AccessMode>(DataStream::DS_Read | DataStream::DS_Write) ),
              FreeBuffer(FreeOnClose)
        {
            BufferStart = BufferPos = static_cast<UInt8*>(Buffer);
            Size = BufferSize;
            BufferEnd = BufferStart + BufferSize;

            if(BufferEnd < BufferStart)
                MEZZ_EXCEPTION(Exception::MM_OUT_OF_BOUNDS_EXCEPTION,"Using a zero or negative size buffer");
        }

        MemoryDataStream::~MemoryDataStream()
        {
            Close();
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        UInt8* MemoryDataStream::GetBufferStart() const
        {
            return BufferStart;
        }

        UInt8* MemoryDataStream::GetBufferPosition() const
        {
            return BufferPos;
        }

        UInt8* MemoryDataStream::GetBufferEnd() const
        {
            return BufferEnd;
        }

        void MemoryDataStream::SetFreeOnClose(bool FreeOnClose)
        {
            FreeBuffer = FreeOnClose;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Stream Access and Manipulation

        size_t MemoryDataStream::ReadLine(Character* Buffer, size_t MaxCount, const String& Delim)
        {
            bool TrimCR = false;
            if(Delim.find_first_of('\n') != String::npos)
            {
                TrimCR = true;
            }

            size_t BytesRead = 0;
            while(BytesRead < MaxCount && BufferPos < BufferEnd)
            {
                if(Delim.find(*BufferPos) != String::npos)
                {
                    if(TrimCR && BytesRead && Buffer[BytesRead - 1] == '\r')
                    {
                        --BytesRead;
                    }

                    ++BufferPos;
                    break;
                }
                Buffer[BytesRead++] = *BufferPos++;
            }
            Buffer[BytesRead] = '\0';
            return BytesRead;
        }

        size_t MemoryDataStream::SkipLine(const String& Delim)
        {
            size_t BytesSkipped = 0;
            while(BufferPos < BufferEnd)
            {
                ++BytesSkipped;
                if(Delim.find(*BufferPos++) != String::npos)
                {
                    break;
                }
            }
            return BytesSkipped;
        }

        size_t MemoryDataStream::Read(void* Buffer, const size_t& Count)
        {
            size_t RetCount = Count;
            if(BufferPos + RetCount > BufferEnd)
                RetCount = BufferEnd - BufferPos;
            if(RetCount == 0)
                return 0;

            //if(RetCount > Count);
            //    MEZZ_EXCEPTION(Exception::MM_OUT_OF_BOUNDS_EXCEPTION,"Cannot read passed end of stream");

            std::memcpy(Buffer,BufferPos,RetCount);
            BufferPos += RetCount;
            return RetCount;
        }

        size_t MemoryDataStream::Write(const void* Buffer, const size_t& Count)
        {
            size_t Written = 0;
            if(IsWriteable())
            {
                Written = Count;
                if(BufferPos + Written > BufferEnd)
                    Written = BufferEnd - BufferPos;
                if(Written == 0)
                    return 0;

                std::memcpy(BufferPos,Buffer,Written);
                BufferPos += Written;
            }
            return Written;
        }

        void MemoryDataStream::Skip(const Integer& Count)
        {
            size_t NewPos = (size_t)( ( BufferPos - BufferStart ) + Count );
            if( BufferStart + NewPos > BufferEnd )
                NewPos = Size;

            BufferPos = BufferStart + NewPos;
        }

        void MemoryDataStream::Seek(const size_t& Position)
        {
            if( BufferStart + Position > BufferEnd )
                MEZZ_EXCEPTION(Exception::MM_OUT_OF_BOUNDS_EXCEPTION,"Attempting to set position of stream to area outside the bounds of the buffer");

            BufferPos = BufferStart + Position;
        }

        size_t MemoryDataStream::Tell()
        {
            return BufferPos - BufferStart;
        }

        bool MemoryDataStream::EoF() const
        {
            return BufferPos >= BufferEnd;
        }

        void MemoryDataStream::Close()
        {
            if(FreeBuffer && BufferStart)
            {
                delete[] BufferStart;
                BufferStart = NULL;
                BufferPos = NULL;
                BufferEnd = NULL;
            }
        }

        ///////////////////////////////////////////////////////////////////////////////
        // FileHandleDataStream Methods
        ///////////////////////////////////////
        FileHandleDataStream::FileHandleDataStream(FILE* Handle, const DataStream::AccessMode& Mode)
            : DataStream( Mode )
        {
            fseek(FileHandle,0,SEEK_END);
            Size = ftell(FileHandle);
            fseek(FileHandle,0,SEEK_SET);
        }

        FileHandleDataStream::FileHandleDataStream(const String& Name, FILE* Handle, const DataStream::AccessMode& Mode)
            : DataStream( Name, Mode )
        {
            fseek(FileHandle,0,SEEK_END);
            Size = ftell(FileHandle);
            fseek(FileHandle,0,SEEK_SET);
        }

        FileHandleDataStream::FileHandleDataStream(const String& Name, const String& Path, const DataStream::StreamFlags& Flags, const DataStream::AccessMode& Mode)
            : DataStream( Name, Mode )
        {
            String Options;
            if(Flags & DataStream::SF_Truncate)
            {
                Options.append("w+");
            }else{
                Options.append("r+");
            }
            if(Flags & DataStream::SF_Binary)
            {
                Options.append("b");
            }

            String FullPath;
            char Check = Path.at(Path.size() - 1);
            #ifdef WINDOWS
            char SysSlash = '\\';
            #else
            char SysSlash = '/';
            #endif
            if( SysSlash != Check )
                FullPath = Path+SysSlash+Name;
            else
                FullPath = Path+Name;
            FileHandle = fopen(FullPath.c_str(),Options.c_str());

            if(!FileHandle && (Flags & DataStream::SF_CreateOnFail))
            {
                if(Flags & DataStream::SF_Binary) Options.assign("r+b");
                else Options.assign("r+");
                FileHandle = fopen(FullPath.c_str(),Options.c_str());
            }

            if(!FileHandle)
            {
                MEZZ_EXCEPTION(Exception::IO_FILE_NOT_FOUND_EXCEPTION,"Unable to create or locate file \""+Name+"\"");
            }

            fseek(FileHandle,0,SEEK_END);
            Size = ftell(FileHandle);
            fseek(FileHandle,0,SEEK_SET);
        }

        FileHandleDataStream::~FileHandleDataStream()
        {
            Close();
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Stream Access and Manipulation

        size_t FileHandleDataStream::Read(void* Buffer, const size_t& Count)
        {
            if(!IsReadable()) return 0;
            else return fread(Buffer,1,Count,FileHandle);
        }

        size_t FileHandleDataStream::Write(const void* Buffer, const size_t& Count)
        {
            if(!IsWriteable()) return 0;
            else return fwrite(Buffer,1,Count,FileHandle);
        }

        void FileHandleDataStream::Skip(const Integer& Count)
        {
            fseek(FileHandle,Count,SEEK_CUR);
        }

        void FileHandleDataStream::Seek(const size_t& Position)
        {
            fseek(FileHandle,static_cast<long>(Position),SEEK_SET);
        }

        size_t FileHandleDataStream::Tell()
        {
            return ftell(FileHandle);
        }

        bool FileHandleDataStream::EoF() const
        {
            return feof(FileHandle) != 0;
        }

        void FileHandleDataStream::Close()
        {
            if(FileHandle)
            {
                fclose(FileHandle);
                FileHandle = NULL;
            }
        }

        ///////////////////////////////////////////////////////////////////////////////
        // FileStreamDataStream Methods
        ///////////////////////////////////////

        FileStreamDataStream::FileStreamDataStream(std::fstream* Stream, const DataStream::AccessMode& Mode)
            : DataStream( Mode ),
              FileStream(Stream)
        {
            FileStream->seekg(0,std::ios_base::end);
            Size = (size_t)FileStream->tellg();
            FileStream->flush();
            Seek(0);
        }

        FileStreamDataStream::FileStreamDataStream(const String& Name, std::fstream* Stream, const DataStream::AccessMode& Mode)
            : DataStream( Name, Mode ),
              FileStream(Stream)
        {
            FileStream->seekg(0,std::ios_base::end);
            Size = (size_t)FileStream->tellg();
            FileStream->flush();
            Seek(0);
        }

        FileStreamDataStream::FileStreamDataStream(const String& Name, const String& Path, const DataStream::StreamFlags& Flags, const DataStream::AccessMode& Mode)
            : DataStream( Name, Mode )
        {
            FileStream = new std::fstream();

            std::ios_base::openmode Options = (std::ios_base::in | std::ios_base::out);
            if(Flags & DataStream::SF_Binary)
            {
                Options = (Options | std::ios_base::binary);
            }
            if(Flags & DataStream::SF_Truncate)
            {
                Options = (Options | std::ios_base::trunc);
            }

            String FullPath;
            char Check = Path.at(Path.size() - 1);
            #ifdef WINDOWS
            char SysSlash = '\\';
            #else
            char SysSlash = '/';
            #endif
            if( SysSlash != Check )
                FullPath = Path+SysSlash+Name;
            else
                FullPath = Path+Name;
            FileStream->open(FullPath.c_str(),Options);

            if(!FileStream->is_open() && (Flags & DataStream::SF_CreateOnFail))
            {
                Options = (Options | std::ios_base::trunc);
                FileStream->open(FullPath.c_str(),Options);
            }

            if(!FileStream->is_open())
            {
                MEZZ_EXCEPTION(Exception::IO_FILE_NOT_FOUND_EXCEPTION,"Unable to create or locate file \""+Name+"\".");
            }

            FileStream->seekg(0,std::ios_base::end);
            Size = (size_t)FileStream->tellg();
            FileStream->flush();
            Seek(0);
        }

        FileStreamDataStream::~FileStreamDataStream()
        {
            Close();
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Stream Access and Manipulation

        size_t FileStreamDataStream::Read(void* Buffer, const size_t& Count)
        {
            if(IsReadable())
            {
                FileStream->read(static_cast<char*>(Buffer),static_cast<std::streamsize>(Count));
                FileStream->seekp(static_cast<std::ifstream::pos_type>(FileStream->gcount()),std::ios::cur);
                return FileStream->gcount();
            }else{
                return 0;
            }
        }

        size_t FileStreamDataStream::Write(const void* Buffer, const size_t& Count)
        {
            size_t Written = 0;
            if(IsWriteable())
            {
                FileStream->write(static_cast<const char*>(Buffer), static_cast<std::streamsize>(Count));
                FileStream->seekg(static_cast<std::ifstream::pos_type>(Count),std::ios::cur);
                Written = Count;
            }
            return Written;
        }

        void FileStreamDataStream::Skip(const Integer& Count)
        {
            FileStream->clear();
            FileStream->seekg(static_cast<std::ifstream::pos_type>(Count),std::ios::cur);
            FileStream->seekp(static_cast<std::ifstream::pos_type>(Count),std::ios::cur);
        }

        void FileStreamDataStream::Seek(const size_t& Position)
        {
            FileStream->clear();
            FileStream->seekg(static_cast<std::streamoff>(Position),std::ios::beg);
            FileStream->seekp(static_cast<std::streamoff>(Position),std::ios::beg);
        }

        size_t FileStreamDataStream::Tell()
        {
            FileStream->clear();
            return (size_t)FileStream->tellg();
        }

        bool FileStreamDataStream::EoF() const
        {
            return FileStream->eof();
        }

        void FileStreamDataStream::Close()
        {
            if(FileStream->is_open())
            {
                FileStream->flush();
                FileStream->close();
            }
        }
    }//Resource
}//Mezzanine

#endif
