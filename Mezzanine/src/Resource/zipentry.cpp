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
#ifndef _resourcezipentry_cpp
#define _resourcezipentry_cpp

#include "Resource/zipentry.h"
#include "Resource/resourceutilities.h"

#include <miniz_zip.h>

namespace Mezzanine
{
    namespace Resource
    {
        ///////////////////////////////////////////////////////////////////////////////
        // ZipEntryCompressionInfo Methods
        ///////////////////////////////////////////////////////////////////////////////

        ///////////////////////////////////////////////////////////////////////////////
        // Construction, Destruction, and Assignment

        ZipEntryCompressionInfo::ZipEntryCompressionInfo(mz_zip_archive_file_stat* Stat) :
            FileStat(Stat)
            {  }

        ///////////////////////////////////////////////////////////////////////////////
        // Type Information

        CompressionMethod ZipEntryCompressionInfo::GetCompressionMethod() const

        ///////////////////////////////////////////////////////////////////////////////
        // Basic Metadata

        size_t ZipEntryCompressionInfo::GetCompressedSize() const
        UInt16 ZipEntryCompressionInfo::GetCompressVersion() const
        UInt16 ZipEntryCompressionInfo::GetDecompressVersion() const
        UInt32 ZipEntryCompressionInfo::GetCRC() const

        ///////////////////////////////////////////////////////////////////////////////
        // ZipEntryEncryptionInfo Methods
        ///////////////////////////////////////////////////////////////////////////////

        ///////////////////////////////////////////////////////////////////////////////
        // Construction, Destruction, and Assignment

        ZipEntryEncryptionInfo::ZipEntryEncryptionInfo(mz_zip_archive_file_stat* Stat) :
            FileStat(Stat)
            {  }

        ///////////////////////////////////////////////////////////////////////////////
        // Type Information

        EncryptionAlgorithm ZipEntryEncryptionInfo::GetEncryptionAlgorithm() const

        ///////////////////////////////////////////////////////////////////////////////
        // ZipEntry Methods
        ///////////////////////////////////////////////////////////////////////////////

        ///////////////////////////////////////////////////////////////////////////////
        // Construction, Destruction, and Assignment

        ZipEntry::ZipEntry(mz_zip_archive_file_stat* Stat) :
            FileStat(Stat)
            {  }

        ///////////////////////////////////////////////////////////////////////////////
        // Type Information

        ArchiveType ZipEntry::GetArchiveType() const
            { return AT_Zip; }

        EntryType ZipEntry::GetEntryType() const
        {

        }

        ///////////////////////////////////////////////////////////////////////////////
        // Basic Metadata

        String ZipEntry::GetName() const
        {
            return Resource::GetBaseName(this->FileStat->m_filename);
        }

        String ZipEntry::GetPath() const
        {
            return Resource::GetDirName(this->FileStat->m_filename);
        }

        String ZipEntry::GetComment() const
            { return String(this->FileStat->m_comment,this->FileStat->m_comment_size); }

        size_t ZipEntry::GetSize() const
            { return this->FileStat->m_uncomp_size; }

        UInt64 ZipEntry::GetCreateTime() const
        {

        }

        UInt64 ZipEntry::GetAccessTime() const
        {

        }

        UInt64 ZipEntry::GetModifyTime() const
        {

        }

        ///////////////////////////////////////////////////////////////////////////////
        // Simple Queries

        Boole ZipEntry::IsReadOnly() const
        {

        }

        Boole ZipEntry::IsCompressed() const
        {

        }

        Boole ZipEntry::IsEncrypted() const
        {

        }

        ///////////////////////////////////////////////////////////////////////////////
        // Extra Metadata

        ArchiveEntryCompressionInfoPtr ZipEntry::GetCompressionInfo() const
        {

        }

        ArchiveEntryEncryptionInfoPtr ZipEntry::GetEncryptionInfo() const
        {

        }
    }//Resource
}//Mezzanine

#endif
