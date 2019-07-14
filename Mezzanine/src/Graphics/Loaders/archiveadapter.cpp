// © Copyright 2010 - 2019 BlackTopp Studios Inc.
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
#ifndef _archiveadapter_cpp
#define _archiveadapter_cpp

// Keeps this file from being documented by doxygen
/// @cond DontDocumentInternal

#include "Graphics/Loaders/archiveadapter.h.cpp"
#include "Graphics/Loaders/iostreamwrapper.h.cpp"

#include "Resource/archivereader.h"
#include "Resource/pathutilities.h"

namespace Mezzanine
{
    namespace Graphics
    {
        ArchiveAdapter::ArchiveAdapter(const String& Name, Resource::ArchiveReader* Reader) :
            Ogre::Archive(Name,"Mezzanine"),
            MezzReader(Reader)
            {  }

        ArchiveAdapter::~ArchiveAdapter()
            {  }

        ///////////////////////////////////////////////////////////////////////////////
        // Ogre Archive Overrides

        bool ArchiveAdapter::exists(const String& FileName)
        {
            return this->MezzReader->FileExists(FileName);
        }

        Ogre::StringVectorPtr ArchiveAdapter::find(const String& Pattern, bool Recursive, bool Dirs)
        {
            Ogre::StringVectorPtr Ret;
            Resource::ArchiveEntryVector Results = this->MezzReader->GetEntries(Pattern,!Dirs);

            for( Resource::ArchiveEntry& CurrEntry : Results )
            {
                if( !Recursive && Resource::GetDirectoryDepth(CurrEntry.Name,true) != 0 ) {
                    continue;
                }
                Ret->push_back(CurrEntry.Name);
            }
            return Ret;
        }

        Ogre::FileInfoListPtr ArchiveAdapter::findFileInfo(const String& Pattern, bool Recursive, bool Dirs)
        {
            Ogre::FileInfoListPtr Ret;
            Resource::ArchiveEntryVector Results = this->MezzReader->GetEntries(Pattern,!Dirs);

            for( Resource::ArchiveEntry& CurrEntry : Results )
            {
                if( !Recursive && Resource::GetDirectoryDepth(CurrEntry.Name,true) != 0 ) {
                    continue;
                }
                Ogre::FileInfo CurrInfo;
                CurrInfo.archive = this;
                CurrInfo.filename = CurrEntry.Name;
                CurrInfo.basename = Resource::GetBaseName(CurrEntry.Name);
                CurrInfo.path = Resource::GetDirName(CurrEntry.Name);
                CurrInfo.compressedSize = CurrEntry.CompressedSize;
                CurrInfo.uncompressedSize = CurrEntry.Size;
                Ret->push_back( std::move(CurrInfo) );
            }
            return Ret;
        }

        time_t ArchiveAdapter::getModifiedTime(const String& FileName)
        {
            Resource::ArchiveEntryPtr Ent = this->MezzReader->GetEntry(FileName);
            if( Ent != nullptr ) {
                return static_cast<time_t>(Ent->ModifyTime);
            }
            return 0;
        }

        bool ArchiveAdapter::isCaseSensitive() const
        {
            return this->MezzReader->IsCaseSensitive();
        }

        Ogre::StringVectorPtr ArchiveAdapter::list(bool Recursive, bool Dirs)
        {
            return this->find("*",Recursive,Dirs);
        }

        Ogre::FileInfoListPtr ArchiveAdapter::listFileInfo(bool Recursive, bool Dirs)
        {
            return this->findFileInfo("*",Recursive,Dirs);
        }

        void ArchiveAdapter::load()
        {
            // Mezzanine system will handle this.  Ogre will not dictate resource lifetimes outside itself.
        }

        Ogre::DataStreamPtr ArchiveAdapter::open(const String& FileName, bool ReadOnly)
        {
            StreamFlags OpenFlags = static_cast<StreamFlags>( ReadOnly ? SF_Read : SF_Read | SF_Write );
            IStreamPtr NewStream = this->MezzReader->OpenIStream(FileName,OpenFlags);
            Ogre::DataStreamPtr WrappedStream( new IStreamWrapper(NewStream) );
            return WrappedStream;
        }

        void ArchiveAdapter::unload()
        {
            // Mezzanine system will handle this.  Ogre will not dictate resource lifetimes outside itself.
        }
    }//Graphics
}//Nezzanine

/// @endcond

#endif
