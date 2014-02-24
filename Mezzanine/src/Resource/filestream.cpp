// © Copyright 2010 - 2014 BlackTopp Studios Inc.
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
#ifndef _resourcefilestream_cpp
#define _resourcefilestream_cpp

#include "Resource/filestream.h"
#include "stringtool.h"
#include "exception.h"

#include <fstream>
#include <algorithm>

namespace Mezzanine
{
    namespace Resource
    {
#ifdef USENEWDATASTREAM
        /// @todo Implement this
#else //USENEWDATASTREAM
        FileStream::FileStream(std::fstream* Stream, const UInt16 Mode)
            : DataStream( Mode ),
              StandardStream(Stream)
        {
            StandardStream->seekg(0,std::ios_base::end);
            Size = (size_t)StandardStream->tellg();
            StandardStream->flush();
            this->SetStreamPosition(0);
        }

        FileStream::FileStream(const String& FileName, const String& Path, const UInt16 Mode)
            : DataStream( Mode )
        {
            StandardStream = new std::fstream();

            std::ios_base::openmode Options = (std::ios_base::in | std::ios_base::out);
            if(SFlags & DataStream::SF_Binary)
            {
                Options = (Options | std::ios_base::binary);
            }
            if(SFlags & DataStream::SF_Truncate)
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
                FullPath = Path+SysSlash+FileName;
            else
                FullPath = Path+FileName;
            StandardStream->open(FullPath.c_str(),Options);

            if(!StandardStream->is_open())
            {
                MEZZ_EXCEPTION(Exception::IO_FILE_NOT_FOUND_EXCEPTION,"Unable to create or locate file \""+FileName+"\".");
            }

            StandardStream->seekg(0,std::ios_base::end);
            Size = (size_t)StandardStream->tellg();
            StandardStream->flush();
            this->SetStreamPosition(0);
        }

        FileStream::~FileStream()
        {
            Close();
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Stream Access and Manipulation

        size_t FileStream::Read(void* Buffer, const size_t& Count)
        {
            if(IsReadable())
            {
                StandardStream->read(static_cast<char*>(Buffer),static_cast<std::streamsize>(Count));
                StandardStream->seekp(static_cast<std::ifstream::pos_type>(StandardStream->gcount()),std::ios::cur);
                return StandardStream->gcount();
            }else{
                return 0;
            }
        }

        size_t FileStream::Write(const void* Buffer, const size_t& Count)
        {
            size_t Written = 0;
            if(IsWriteable())
            {
                StandardStream->write(static_cast<const char*>(Buffer), static_cast<std::streamsize>(Count));
                StandardStream->seekg(static_cast<std::ifstream::pos_type>(Count),std::ios::cur);
                Written = Count;
            }
            return Written;
        }

        void FileStream::Advance(const StreamOff Count)
        {
            StandardStream->clear();
            this->SetStreamPosition(Count,SO_Current);
        }

        void FileStream::SetStreamPosition(StreamPos Position)
        {
            StandardStream->clear();
            StandardStream->seekg(Position);
            StandardStream->seekp(Position);
        }

        void FileStream::SetStreamPosition(StreamOff Offset, SeekOrigin Origin)
        {
            StandardStream->clear();
            StandardStream->seekg(Offset,std::ios::cur);
            StandardStream->seekp(Offset,std::ios::cur);
        }

        StreamPos FileStream::GetStreamPosition(Boole Read)
        {
            if(Read) return StandardStream->tellg();
            else return StandardStream->tellp();
        }

        Boole FileStream::EoF() const
        {
            return StandardStream->eof();
        }

        void FileStream::Close()
        {
            if(StandardStream->is_open())
            {
                StandardStream->flush();
                StandardStream->close();
            }
        }
#endif
    }//Resource
}//Mezzanine

#endif
