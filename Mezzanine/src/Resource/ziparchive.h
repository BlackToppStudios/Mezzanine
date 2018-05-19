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
#ifndef _resourceziparchive_h
#define _resourceziparchive_h

#include "datastream.h"
#include "Resource/archive.h"

// Internal Forward Declare
struct zip;
struct zip_file;

namespace Mezzanine
{
    namespace Resource
    {
        /// @brief An enum listing the codecs directly supported by the Zip format with their IDs.
        /// @remarks In addition to the values listed here, there are additional values in the spec that
        /// are reserved for future use by PKWare. Those values are: 11, 13, 15, 16, and 17. @n
        /// It is also important to note that these are only what is in the spec and thus what other programs
        /// are likely to recognize, however little stops you from adding a custom codec to a Zip archive.
        /// The only potential issue is other archive tools recognizing the value. This isn't a concern if
        /// you only plan to use that archive with your own custom application.
        enum ZipCodecSupport
        {
            ZCS_Raw                 = 0,  ///< No compression.
            ZCS_Shrunk              = 1,
            ZCS_ReducedCompression1 = 2,
            ZCS_ReducedCompression2 = 3,
            ZCS_ReducedCompression3 = 4,
            ZCS_ReducedCompression4 = 5,
            ZCS_Imploded            = 6,
            ZCS_Tokenizing          = 7,
            ZCS_Deflate             = 8,  ///< Standard DEFLATE, which is the default for Zip files.
            ZCS_Deflate64           = 9,
            ZCS_IBM_TERSE_Old       = 10,
            ZCS_BZip2               = 12,
            ZCS_LZMA                = 14,
            ZCS_IBM_TERSE_New       = 18,
            ZCS_LZ77                = 19,
            ZCS_WavPack             = 97,
            ZCS_PPMd                = 98
        };

        ///////////////////////////////////////////////////////////////////////////////
        /// @brief This is an Archive implementation for reading and writing .zip files.
        /// @details
        ///////////////////////////////////////
        class MEZZ_LIB ZipArchive : public Archive
        {
        protected:
            /// @brief The path to the Archive, if applicable.
            String ArchivePath;
            /// @brief The name of the Archive file, if applicable.
            String ArchiveName;
            /// @brief The stream to the raw data of the Archive.
            DataStreamPtr ArchiveStream;
            /// @brief A pointer to the internal Archive.
            zip* InternalArchive;
        public:
            /// @brief Class constructor.
            /// @param Stream The stream to the raw data of the archive.
            ZipArchive(DataStreamPtr Stream);
            // ? Should this accept a URI instead ?
            /// @brief Filesystem constructor.
            /// @param FullName The full path and file name of the Archive.
            /// @param Flags Special configuration flags for the opening or operation of the Archive.
            /// @param Stream The stream to the raw data of the archive.
            ZipArchive(const String& FullName, const Whole Flags, DataStreamPtr Stream);
            /// @brief Filesystem constructor.
            /// @param PathName The path to the Archive.
            /// @param FileName The name of the Archive file at the specified path.
            /// @param Flags Special configuration flags for the opening or operation of the Archive.
            /// @param Stream The stream to the raw data of the archive.
            ZipArchive(const String& PathName, const String& FileName, const Whole Flags, DataStreamPtr Stream);
            /// @brief Class destructor.
            ~ZipArchive();

            ///////////////////////////////////////////////////////////////////////////////
            // Configuration

            /// @copydoc Archive::SetFlags(const Whole)
            virtual void SetFlags(const Whole Flags);
            /// @copydoc Archive::GetFlags() const
            virtual Whole GetFlags() const;

            ///////////////////////////////////////////////////////////////////////////////
            // Streaming

            ///////////////////////////////////////////////////////////////////////////////
            // Querying

            /// @copydoc Archive::GetSize() const
            virtual UInt64 GetSize() const;
            /// @copydoc Archive::GetFileCount() const
            virtual Whole GetFileCount() const;
            /// @copydoc Archive::GetEntry(const UInt64) const
            virtual ArchiveEntryPtr GetEntry(const UInt64 Index) const;
            /// @copydoc Archive::GetEntry(const String&) const
            virtual ArchiveEntryPtr GetEntry(const String& FileName) const;
        };//ZipArchive
    }//Resource
}//Mezzanine

#endif
