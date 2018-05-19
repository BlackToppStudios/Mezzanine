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
#ifndef _resourcearchive_h
#define _resourcearchive_h

#include "Resource/archiveentry.h"

namespace Mezzanine
{
    namespace Resource
    {
        ///////////////////////////////////////////////////////////////////////////////
        /// @brief This is a base class for collections of files that can be accessed/loaded.
        /// @details
        ///////////////////////////////////////
        class MEZZ_LIB Archive
        {
        protected:
        public:
            /// @brief Class constructor.
            Archive() = default;
            /// @brief Class destructor.
            virtual ~Archive() = default;

            ///////////////////////////////////////////////////////////////////////////////
            // Configuration

            /// @brief Sets any and all special configuration flags the archive may need.
            /// @remarks An archive may have many flags, or it may have none at all.  Consult the documentation of
            /// the archive implementation to understand what flags may exist for that archive.
            /// @param Flags The flags to be set for the archive.
            virtual void SetFlags(const Whole Flags) = 0;
            /// @brief Gets the configuration flags used by the archive.
            /// @return Returns a bitmask pertaining to the configuration of this archive.
            virtual Whole GetFlags() const = 0;

            ///////////////////////////////////////////////////////////////////////////////
            // Streaming

            ///////////////////////////////////////////////////////////////////////////////
            // Querying

            /// @brief Gets the total size of the archive.
            /// @return Returns the complete size of the archive in bytes.
            virtual UInt64 GetSize() const = 0;
            /// @brief Gets the number of files contained in the archive.
            /// @return Returns the total number of files stored in this archive.
            virtual Whole GetFileCount() const = 0;
            /// @brief Gets the archive entry for a file at the specified index.
            /// @param Index The index of the entry to retrieve.
            /// @return Returns an ArchiveEntry containing information of the file at the specified index.
            virtual ArchiveEntryPtr GetEntry(const UInt64 Index) const = 0;
            /// @brief Gets the archive entry for a file at the specified index.
            /// @param FileName The name of the file to retrieve the entry for.
            /// @return Returns an ArchiveEntry containing information of the file at the specified index.
            virtual ArchiveEntryPtr GetEntry(const String& FileName) const = 0;
        };//Archive
    }//Resource
}//Mezzanine

#endif
