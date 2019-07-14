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
#ifndef _resourceassetgroup_cpp
#define _resourceassetgroup_cpp

#include "Resource/assetgroup.h"
#include "Resource/filesystemarchivereader.h"
#include "Resource/filesystemarchivewriter.h"
#include "Resource/ziparchivereader.h"
#include "Resource/ziparchivewriter.h"

#include "memorystream.h"
#include "filestream.h"
#include "exception.h"

namespace
{
    using namespace Mezzanine;
    using namespace Mezzanine::Resource;

    /// @brief Creates a new ArchiveReader of a specified type.
    /// @exception If the ArchiveType isn't supported, a NOT_IMPLEMENTED_EXCEPTION will be thrown.
    /// @param ArchType The type of ArchiveReader to create.  See ArchiveType enum for more info.
    /// @return Returns a pointer to the new ArchiveReader.
    ArchiveReader* CreateArchiveReader(const ArchiveType ArchType)
    {
        switch( ArchType )
        {
            case Resource::AT_FileSystem:  return new FileSystemArchiveReader();  break;
            case Resource::AT_Zip:         return new ZipArchiveReader();         break;
            //case Resource::AT_7z:          return new SevenZipArchiveReader();    break;
            default:
            {
                String ExceptionMsg("Requesting creation of unknown ArchiveType reader.");
                MEZZ_EXCEPTION(ExceptionBase::NOT_IMPLEMENTED_EXCEPTION,ExceptionMsg);
            }
        }
    }
    /*
    /// @brief Creates a new ArchiveWriter of a specified type.
    /// @exception If the ArchiveType isn't supported, a NOT_IMPLEMENTED_EXCEPTION will be thrown.
    /// @param ArchType The type of ArchiveWriter to create.  See ArchiveType enum for more info.
    /// @return Returns a pointer to the new ArchiveWriter.
    ArchiveWriter* CreateArchiveWriter(const ArchiveType ArchType)
    {
        switch( ArchType )
        {
            //case Resource::AT_FileSystem:  return new FileSystemArchiveWriter();  break;
            //case Resource::AT_Zip:         return new ZipArchiveWriter();         break;
            //case Resource::AT_7z:          return new SevenZipArchiveWriter();    break;
            default:
            {
                String ExceptionMsg("Requesting creation of unknown ArchiveType writer.");
                MEZZ_EXCEPTION(ExceptionBase::NOT_IMPLEMENTED_EXCEPTION,ExceptionMsg);
            }
        }
    }//*/
}

namespace Mezzanine
{
    namespace Resource
    {
        AssetGroup::AssetGroup(const String& GroupName) :
            Name(GroupName)
            {  }

        AssetGroup::~AssetGroup()
        {
            this->RemoveAllReadLocations();
        }

        ArchiveReader* AssetGroup::FindAssetReader(const String& Identifier) const
        {
            for( ArchiveReader* CurrReader : this->Readers )
            {
                if( CurrReader->FileExists(Identifier) ) {
                    return CurrReader;
                }
            }
            return nullptr;
        }

        ArchiveReader* AssetGroup::FindAssetReaderOrThrow(const String& Identifier) const
        {
            ArchiveReader* FoundReader = this->FindAssetReader(Identifier);
            if( FoundReader == nullptr ) {
                StringStream ExceptionStream;
                ExceptionStream << "Asset \"" << Identifier << "\" was not found in group \"" << this->Name << "\".";
                MEZZ_EXCEPTION(ExceptionBase::IO_FILE_NOT_FOUND_EXCEPTION,ExceptionStream.str());
            }
            return FoundReader;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        const String& AssetGroup::GetName() const
            { return this->Name; }

        String AssetGroup::GetAssetPath(const String& Identifier)
        {
            ArchiveReader* FoundReader = this->FindAssetReader(Identifier);
            if( FoundReader != nullptr ) {
                return FoundReader->GetIdentifier();
            }else{
                return String();
            }
        }

        IStreamPtr AssetGroup::GetInputStream(const String& Identifier)
        {
            for( IStreamPtr& Stream : this->InputStreams )
            {
                if( Stream->GetIdentifier() == Identifier ) {
                    return Stream;
                }
            }
            return nullptr;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Locations

        void AssetGroup::AddReadLocation(const String& Identifier, const ArchiveType ArchType,
                                         Char8* Buffer, const size_t BufferSize, const Boole Owner)
        {
            ArchiveReader* NewReader = CreateArchiveReader(ArchType);
            NewReader->Open(Identifier,this->Name,Buffer,BufferSize,Owner);
            this->Readers.push_back(NewReader);
        }

        void AssetGroup::AddReadLocation(const String& Identifier, const ArchiveType ArchType)
        {
            ArchiveReader* NewReader = CreateArchiveReader(ArchType);
            NewReader->Open(Identifier,this->Name);
            this->Readers.push_back(NewReader);
        }

        void AssetGroup::RemoveReadLocation(const String& Identifier)
        {
            for( ReaderIterator ReaderIt = this->Readers.begin() ; ReaderIt != this->Readers.end() ; ++ReaderIt )
            {
                if( (*ReaderIt)->GetIdentifier() == Identifier ) {
                    (*ReaderIt)->Close();
                    delete (*ReaderIt);
                    return;
                }
            }
        }

        void AssetGroup::RemoveAllReadLocations()
        {
            for( ArchiveReader* CurrReader : this->Readers )
                { delete CurrReader; }
            this->Readers.clear();
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Stream Management

        IStreamPtr AssetGroup::OpenAsset(const String& Identifier, const Whole Flags, const Boole Raw )
        {
            IStreamPtr ReturnStream = GetInputStream(Identifier);
            if( ReturnStream ) {
                return ReturnStream;
            }
            ArchiveReader* FoundReader = this->FindAssetReader(Identifier);
            if( FoundReader != nullptr ) {
                ReturnStream = FoundReader->OpenIStream(Identifier,Flags,Raw);
                this->InputStreams.push_back(ReturnStream);
                return ReturnStream;
            }
            return nullptr;
        }

        IStreamPtr AssetGroup::OpenEncryptedAsset(const String& Identifier, const String& Password,
                                                  const Whole Flags, const Boole Raw)
        {
            IStreamPtr ReturnStream = GetInputStream(Identifier);
            if( ReturnStream ) {
                return ReturnStream;
            }
            ArchiveReader* FoundReader = this->FindAssetReader(Identifier);
            if( FoundReader != nullptr ) {
                ReturnStream = FoundReader->OpenEncryptedIStream(Identifier,Password,Flags,Raw);
                this->InputStreams.push_back(ReturnStream);
                return ReturnStream;
            }
            return nullptr;
        }

        IStreamPtr AssetGroup::BufferAsset(const String& Identifier, const Whole Flags, const Boole Raw)
        {
            if( IStreamPtr ReturnStream = GetInputStream(Identifier) ) {
                return ReturnStream;
            }
            ArchiveReader* FoundReader = this->FindAssetReader(Identifier);
            if( FoundReader != nullptr ) {
                IStreamPtr TempStream = FoundReader->OpenIStream(Identifier,Flags,Raw);

                MemoryIStreamPtr BufferedStream( new MemoryIStream() );
                BufferedStream->CreateBuffer( Identifier, this->Name, TempStream->GetSize() );
                TempStream->Read( BufferedStream->GetBufferStart(), BufferedStream->GetSize() );

                this->InputStreams.push_back(BufferedStream);
                return BufferedStream;
            }
            return nullptr;
        }

        IStreamPtr AssetGroup::BufferEncryptedAsset(const String& Identifier, const String& Password,
                                                    const Whole Flags, const Boole Raw)
        {
            if( IStreamPtr ReturnStream = GetInputStream(Identifier) ) {
                return ReturnStream;
            }
            ArchiveReader* FoundReader = this->FindAssetReader(Identifier);
            if( FoundReader != nullptr ) {
                IStreamPtr TempStream = FoundReader->OpenEncryptedIStream(Identifier,Password,Flags,Raw);

                MemoryIStreamPtr BufferedStream( new MemoryIStream() );
                BufferedStream->CreateBuffer( Identifier, this->Name, TempStream->GetSize() );
                TempStream->Read( BufferedStream->GetBufferStart(), BufferedStream->GetSize() );

                this->InputStreams.push_back(BufferedStream);
                return BufferedStream;
            }
            return nullptr;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Stream Management With Throwing

        IStreamPtr AssetGroup::OpenAssetOrThrow(const String& Identifier, const Whole Flags, const Boole Raw )
        {
            IStreamPtr ReturnStream = GetInputStream(Identifier);
            if( ReturnStream ) {
                return ReturnStream;
            }
            ArchiveReader* FoundReader = this->FindAssetReaderOrThrow(Identifier);
            ReturnStream = FoundReader->OpenIStream(Identifier,Flags,Raw);
            this->InputStreams.push_back(ReturnStream);
            return ReturnStream;
        }

        IStreamPtr AssetGroup::OpenEncryptedAssetOrThrow(const String& Identifier, const String& Password,
                                                         const Whole Flags, const Boole Raw)
        {
            IStreamPtr ReturnStream = GetInputStream(Identifier);
            if( ReturnStream ) {
                return ReturnStream;
            }
            ArchiveReader* FoundReader = this->FindAssetReaderOrThrow(Identifier);
            ReturnStream = FoundReader->OpenEncryptedIStream(Identifier,Password,Flags,Raw);
            this->InputStreams.push_back(ReturnStream);
            return ReturnStream;
        }

        IStreamPtr AssetGroup::BufferAssetOrThrow(const String& Identifier, const Whole Flags, const Boole Raw)
        {
            if( IStreamPtr ReturnStream = GetInputStream(Identifier) ) {
                return ReturnStream;
            }
            ArchiveReader* FoundReader = this->FindAssetReaderOrThrow(Identifier);
            IStreamPtr TempStream = FoundReader->OpenIStream(Identifier,Flags,Raw);

            MemoryIStreamPtr BufferedStream( new MemoryIStream() );
            BufferedStream->CreateBuffer( Identifier, this->Name, TempStream->GetSize() );
            TempStream->Read( BufferedStream->GetBufferStart(), BufferedStream->GetSize() );

            this->InputStreams.push_back(BufferedStream);
            return BufferedStream;
        }

        IStreamPtr AssetGroup::BufferEncryptedAssetOrThrow(const String& Identifier, const String& Password,
                                                           const Whole Flags, const Boole Raw)
        {
            if( IStreamPtr ReturnStream = GetInputStream(Identifier) ) {
                return ReturnStream;
            }
            ArchiveReader* FoundReader = this->FindAssetReaderOrThrow(Identifier);
            IStreamPtr TempStream = FoundReader->OpenEncryptedIStream(Identifier,Password,Flags,Raw);

            MemoryIStreamPtr BufferedStream( new MemoryIStream() );
            BufferedStream->CreateBuffer( Identifier, this->Name, TempStream->GetSize() );
            TempStream->Read( BufferedStream->GetBufferStart(), BufferedStream->GetSize() );

            this->InputStreams.push_back(BufferedStream);
            return BufferedStream;
        }
    }//Resource
}//Mezzanine

#endif
