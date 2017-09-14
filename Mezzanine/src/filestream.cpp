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
#ifndef _filestream_cpp
#define _filestream_cpp

#include "filestream.h"
#include "stringtool.h"
#include "exception.h"

#include <fstream>
#include <algorithm>

namespace Mezzanine
{
    FileStream::FileStream() :
        IOStream(&this->FileBuffer)
        { /*this->init(&this->FileBuffer);*/ }

    FileStream::FileStream(const String& File, const Whole Mode) :
        IOStream(&this->FileBuffer)
        { /*this->init(&this->FileBuffer);*/  this->OpenFile(File,Mode); }

    FileStream::FileStream(const String& FileName, const String& FilePath, const Whole Mode) :
        IOStream(&this->FileBuffer)
        { /*this->init(&this->FileBuffer);*/  this->OpenFile(FileName,FilePath,Mode); }

    FileStream::~FileStream()
        { if( this->IsOpenToFile() ) this->CloseFile(); }

    ///////////////////////////////////////////////////////////////////////////////
    // Utility Methods

    void FileStream::OpenFile(const String& File, const Whole Mode)
    {
        //this->open(File.c_str(),static_cast<const std::ios_base::openmode>(Mode));
        if( !this->FileBuffer.open(File.c_str(),static_cast<const std::ios_base::openmode>(Mode)) ) {
            this->setstate(ios_base::failbit);
        }else{
            this->clear();
        }

        if( !this->IsOpenToFile() ) {
            MEZZ_EXCEPTION(ExceptionBase::IO_FILE_NOT_FOUND_EXCEPTION,"Unable to create or locate file \"" + File + "\".");
        }

        this->SetReadPosition(0,Mezzanine::SO_End);
        this->Size = (StreamSize)this->GetReadPosition();
        this->flush();
        this->SetStreamPosition(0);
    }

    void FileStream::OpenFile(const String& FileName, const String& FilePath, const Whole Mode)
    {
        String FullPath;
        char Check = FilePath.at(FilePath.size() - 1);
        #ifdef MEZZ_WINDOWS
        char SysSlash = '\\';
        #else
        char SysSlash = '/';
        #endif
        if( SysSlash != Check ) {
            FullPath = FilePath + SysSlash + FileName;
        }else{
            FullPath = FilePath + FileName;
        }

        this->OpenFile(FullPath,Mode);
    }

    Boole FileStream::IsOpenToFile() const
    {
        return this->FileBuffer.is_open();
    }

    void FileStream::CloseFile()
    {
        this->OpenFileName.clear();
        this->Flags = Mezzanine::SF_None;

        //this->close();
        if( !this->FileBuffer.close() ) {
            this->setstate(std::ios_base::failbit);
        }
    }

    const String& FileStream::GetFilePathAndName() const
        { return this->OpenFileName; }

    Whole FileStream::GetSteamFlags() const
        { return this->Flags; }

    ///////////////////////////////////////////////////////////////////////////////
    // Stream Base Operations

    StreamSize FileStream::GetSize() const
        { return this->Size; }
}//Mezzanine

#endif
