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

#include "datastream.h"
#include "Resource/archiveentry.h"

namespace Mezzanine
{
    namespace Resource
    {
        /// @brief A small collection of flags for the opening settings of an Archive.
        enum ArchiveOpenFlags
        {
            AOF_None           = 0,          ///< No flags.
            AOF_Read           = EnumBit(1), ///< Enable reading from the archive.
            AOF_Write          = EnumBit(2), ///< Enable writing to the archive.
            AOF_Create         = EnumBit(3), ///< If the file or directory(ies) don't exist, create them.
            AOF_RecurseSubDirs = EnumBit(4)  ///< Enable operations on more than the root directory of the archive, if more directories exist.
        };

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
            // Open / Close

            /// @brief Opens an archive for IO operations.
            /// @param Identifier The path and name to the archive to be opened.
            /// @param Flags A bitmask of the options to open the archive with.
            virtual void Open(const char* Identifier, const Whole Flags) = 0;
            /// @brief Opens an archive for IO operations.
            /// @param Identifier The path and name to the archive to be opened.
            /// @param Flags A bitmask of the options to open the archive with.
            virtual void Open(const String& Identifier, const Whole Flags) = 0;
            /// @brief Open an archive to a memory buffer.
            /// @param Identifier A string identifying the buffer to be used as the archive.
            /// @param Buffer A pointer to the buffer containing the archive.
            /// @param BufferSize The size of the buffer container the archive.
            /// @param Flags A bitmask of the options to open the archive with.
            /// @param Owner A bool to indicate if the archive should become the owner of the buffer, deleting it when it closes.
            virtual void Open(const String& Identifier, Char8* Buffer, const size_t BufferSize, const Whole Flags, const Boole Owner) = 0;
            /// @brief Gets whether or not this archive is open for I/O operations.
            /// @return Returns true if this archive is open and in use, false otherwise.
            virtual Boole IsOpen() const = 0;
            /// @brief Closes the archive and removes all streams to it.
            /// @remarks The archive will be set back to a pre-open state and can be reopened.
            virtual void Close() = 0;

            /// @brief Gets the configuration flags used to open the archive.
            /// @return Returns the bitmask that was used to open the archive.
            virtual Whole GetFlags() const = 0;

            ///////////////////////////////////////////////////////////////////////////////
            // Streaming

            /// @brief Opens a stream to a resource in the archive.
            /// @param Identifier The unique identifier for the resource in the archive.
            /// @param Flags A bitmask of the options to open the stream with.  See StreamFlags enum for information on valid flags.
            /// @return Returns a pointer to the opened stream.
            virtual DataStreamPtr OpenStream(const char* Identifier, const Whole Flags) = 0;
            /// @brief Opens a stream to a resource in the archive.
            /// @param Identifier The unique identifier for the resource in the archive.
            /// @param Flags A bitmask of the options to open the stream with.  See StreamFlags enum for information on valid flags.
            /// @return Returns a pointer to the opened stream.
            virtual DataStreamPtr OpenStream(const String& Identifier, const Whole Flags) = 0;

            ///////////////////////////////////////////////////////////////////////////////
            // Querying

            /// @brief Gets the number of entries contained in the archive.
            /// @remarks Not all archives support getting the file count up front.  A file count can still be retrieved
            /// by getting all entries in the archive (pattern "*"), but doing so may be slow depending on the number
            /// of entries retrieved.  This method is meant to be a simple and fast number retrieval.  If it can't do
            /// that with the archive in question, it'll return -1.
            /// @return Returns the total number of entries stored in this archive, -1 if the archive doesn't support known file counts.
            virtual Int64 GetEntryCount() const = 0;
            /// @brief Gets the archive entry for a file at the specified index.
            /// @param Index The index of the entry to retrieve.
            /// @return Returns an ArchiveEntryPtr to file metadata at the specified index, or a nullptr if this is not supported.
            virtual ArchiveEntryPtr GetEntry(const UInt64 Index) const = 0;
            /// @brief Gets the archive entry for a file at the specified index.
            /// @param FileName The name of the file to retrieve the entry for.
            /// @return Returns an ArchiveEntryPtr to file metadata belonging to the specified file, or a nullptr if the file was not found.
            virtual ArchiveEntryPtr GetEntry(const String& FileName) const = 0;
            /// @brief Gets all the archive entries matching a name pattern.
            /// @remarks The Pattern works best when the path pattern is relative (not absolute).
            /// @param Pattern The pattern to search for among the archive entries.
            /// @param OmitDirs True to not include directory entries in the results.
            /// @return Returns a vector of archives entries matching the provided pattern.
            virtual ArchiveEntryVector GetEntries(const String& Pattern, const Boole OmitDirs) const = 0;
        };//Archive
    }//Resource
}//Mezzanine

#endif
