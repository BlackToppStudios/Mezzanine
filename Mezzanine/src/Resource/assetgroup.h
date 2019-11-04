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
#ifndef _resourceassetgroup_h
#define _resourceassetgroup_h

#include "datastream.h"
#include "Resource/resourceenumerations.h"

namespace Mezzanine
{
    namespace Resource
    {
        class ArchiveReader;
        class ArchiveWriter;
        ///////////////////////////////////////////////////////////////////////////////
        /// @brief This is a class that stores a specific grouping of Assets, usually based on their location.
        ///////////////////////////////////////
        class AssetGroup
        {
        public:
            /// @brief Container type for the storage of ArchiveReader instances.
            using ReaderContainer = std::vector<ArchiveReader*>;
            /// @brief Iterator type for ArchiveReaders in a ReaderContainer.
            using ReaderIterator = ReaderContainer::iterator;
            /// @brief Const Iterator type for ArchiveReaders in a ReaderContainer.
            using ConstReaderIterator = ReaderContainer::const_iterator;
            /// @brief Container type for the storage of IStream instances.
            using IStreamContainer = std::vector<IStreamPtr>;
            /// @brief Iterator type for IStream instances in an IStream container.
            using IStreamIterator = IStreamContainer::iterator;
            /// @brief Const Iterator type for IStream instances in an IStream container.
            using ConstIStreamIterator = IStreamContainer::const_iterator;
        protected:
            /// @brief Container storing ArchiveReaders.
            ReaderContainer Readers;
            /// @brief Container storing input streams.
            IStreamContainer InputStreams;
            /// @brief The name of this group.
            String Name;
        public:
            /// @brief Class constructor.
            /// @param GroupName The name to be given to this group.
            AssetGroup(const String& GroupName);
            /// @brief Class destructor.
            ~AssetGroup();

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            /// @brief Gets the name of this asset group.
            /// @return Returns a const reference to a string containing the name of this group.
            const String& GetName() const;

            /// @brief Gets the path to the specified asset in this AssetGroup.
            /// @param Identifier Usually a path and filename, but can be any unique identifier the archive can use.
            /// @return Returns the path to the specified Asset, or an empty String if it wasn't found.
            String GetAssetPath(const String& Identifier);

            /// @brief Gets an already opened stream.
            /// @param Identifier The identifier for the stream to be retrieved.
            /// @return Returns an IStreamPtr to the specified stream or nullptr if no such stream is currently open.
            IStreamPtr GetInputStream(const String& Identifier);

            /// @brief Convenience method for locating the reader containing the specified asset.
            /// @param Identifier A string identifying (such as path and filename) the asset to locate.
            /// @return Returns a pointer to the ArchiveReader that has the specified Asset, or nullptr if not found.
            ArchiveReader* GetAssetReader(const String& Identifier) const;
            /// @brief Convenience method for locating the reader containing the specified asset.
            /// @exception A IO_FILE_NOT_FOUND_EXCEPTION will be thrown if the asset isn't located.
            /// @param Identifier A string identifying (such as path and filename) the asset to locate.
            /// @return Returns a pointer to the ArchiveReader that has the specified Asset.
            ArchiveReader* GetAssetReaderOrThrow(const String& Identifier) const;

            ///////////////////////////////////////////////////////////////////////////////
            // Locations

            /// @brief Adds a new (memory) location to create read streams from.
            /// @param Identifier A string identifying the buffer to be used as the archive.
            /// @param ArchType The type/format of archive that will be read from.
            /// @param Buffer A string identifying the buffer to be used as the archive.
            /// @param BufferSize The size of the buffer container the archive.
            /// @param Owner Indicates if the reader should become the owner of the buffer, deleting it when it closes.
            void AddReadLocation(const String& Identifier,
                                 const ArchiveType ArchType,
                                 Char8* Buffer,
                                 const size_t BufferSize,
                                 const Boole Owner);
            /// @brief Adds a new (non-memory) location to create read streams from.
            /// @param Identifier The path and name to the archive to be opened.
            /// @param ArchType The type/format of archive that will be read from.
            void AddReadLocation(const String& Identifier,
                                 const ArchiveType ArchType);
            /// @brief Removes a read location from the list of locations to look for assets.
            /// @param Identifier The path and name to the archive to be removed.
            void RemoveReadLocation(const String& Identifier);
            /// @brief Removes all read locations in this group.
            void RemoveAllReadLocations();

            ///////////////////////////////////////////////////////////////////////////////
            // Stream Management

            /// @brief Opens an asset from an archive location in this group.
            /// @remarks Locations are not searched in any particular order, the first match will be returned.
            /// @param Identifier Usually a path and filename, but can be any unique identifier the archive can use.
            /// @param Flags A bitmask of the options to open the stream with.  See StreamFlags enum for more info.
            /// @param Raw If true, the stream will perform no processing on the raw data before returning.
            /// @return Returns a shared pointer to an IStream to the opened asset, or null if no asset was found.
            IStreamPtr OpenAsset(const String& Identifier,
                                 const Whole Flags = SF_Read,
                                 const Boole Raw = false);
            /// @brief Opens an encrypted asset from an archive location in this group.
            /// @remarks Locations are not searched in any particular order, the first match will be returned.
            /// @param Identifier Usually a path and filename, but can be any unique identifier the archive can use.
            /// @param Password The password necessary to decrypt the asset.
            /// @param Flags A bitmask of the options to open the stream with.  See StreamFlags enum for more info.
            /// @param Raw If true, the stream will perform no processing on the raw data before returning.
            /// @return Returns a shared pointer to an IStream to the opened asset, or null if no asset was found.
            IStreamPtr OpenEncryptedAsset(const String& Identifier,
                                          const String& Password,
                                          const Whole Flags = SF_Read,
                                          const Boole Raw = false);

            /// @brief Opens an asset from a location in this group and pre-loads it all into a memory buffer.
            /// @warning This will completely load the asset into memory.  Be mindful of file sizes.
            /// @remarks Locations are not searched in any particular order, the first match will be returned.
            /// @param Identifier Usually a path and filename, but can be any unique identifier the archive can use.
            /// @param Flags A bitmask of the options to open the stream with.  See StreamFlags enum for more info.
            /// @param Raw If true, the stream will perform no processing on the raw data before returning.
            /// @return Returns a shared pointer to an IStream to the opened asset, or null if no asset was found.
            IStreamPtr BufferAsset(const String& Identifier,
                                   const Whole Flags = SF_Read,
                                   const Boole Raw = false);
            /// @brief Opens an encrypted asset from a location in this group and pre-loads it all into a memory buffer.
            /// @warning This will completely load the asset into memory.  Be mindful of file sizes.
            /// @remarks Locations are not searched in any particular order, the first match will be returned.
            /// @param Identifier Usually a path and filename, but can be any unique identifier the archive can use.
            /// @param Password The password necessary to decrypt the asset.
            /// @param Flags A bitmask of the options to open the stream with.  See StreamFlags enum for more info.
            /// @param Raw If true, the stream will perform no processing on the raw data before returning.
            /// @return Returns a shared pointer to an IStream to the opened asset, or null if no asset was found.
            IStreamPtr BufferEncryptedAsset(const String& Identifier,
                                            const String& Password,
                                            const Whole Flags = SF_Read,
                                            const Boole Raw = false);

            ///////////////////////////////////////////////////////////////////////////////
            // Stream Management With Throwing

            /// @brief Opens an asset from an archive location in this group.
            /// @remarks Locations are not searched in any particular order, the first match will be returned.
            /// @exception A IO_FILE_NOT_FOUND_EXCEPTION will be thrown if the asset isn't located.
            /// @param Identifier Usually a path and filename, but can be any unique identifier the archive can use.
            /// @param Flags A bitmask of the options to open the stream with.  See StreamFlags enum for more info.
            /// @param Raw If true, the stream will perform no processing on the raw data before returning.
            /// @return Returns a shared pointer to an IStream to the opened asset, or null if no asset was found.
            IStreamPtr OpenAssetOrThrow(const String& Identifier,
                                        const Whole Flags = SF_Read,
                                        const Boole Raw = false);
            /// @brief Opens an encrypted asset from an archive location in this group.
            /// @remarks Locations are not searched in any particular order, the first match will be returned.
            /// @exception A IO_FILE_NOT_FOUND_EXCEPTION will be thrown if the asset isn't located.
            /// @param Identifier Usually a path and filename, but can be any unique identifier the archive can use.
            /// @param Password The password necessary to decrypt the asset.
            /// @param Flags A bitmask of the options to open the stream with.  See StreamFlags enum for more info.
            /// @param Raw If true, the stream will perform no processing on the raw data before returning.
            /// @return Returns a shared pointer to an IStream to the opened asset, or null if no asset was found.
            IStreamPtr OpenEncryptedAssetOrThrow(const String& Identifier,
                                                 const String& Password,
                                                 const Whole Flags = SF_Read,
                                                 const Boole Raw = false);

            /// @brief Opens an asset from a location in this group and pre-loads it all into a memory buffer.
            /// @warning This will completely load the asset into memory.  Be mindful of file sizes.
            /// @remarks Locations are not searched in any particular order, the first match will be returned.
            /// @exception A IO_FILE_NOT_FOUND_EXCEPTION will be thrown if the asset isn't located.
            /// @param Identifier Usually a path and filename, but can be any unique identifier the archive can use.
            /// @param Flags A bitmask of the options to open the stream with.  See StreamFlags enum for more info.
            /// @param Raw If true, the stream will perform no processing on the raw data before returning.
            /// @return Returns a shared pointer to an IStream to the opened asset, or null if no asset was found.
            IStreamPtr BufferAssetOrThrow(const String& Identifier,
                                          const Whole Flags = SF_Read,
                                          const Boole Raw = false);
            /// @brief Opens an encrypted asset from a location in this group and pre-loads it all into a memory buffer.
            /// @warning This will completely load the asset into memory.  Be mindful of file sizes.
            /// @remarks Locations are not searched in any particular order, the first match will be returned.
            /// @exception A IO_FILE_NOT_FOUND_EXCEPTION will be thrown if the asset isn't located.
            /// @param Identifier Usually a path and filename, but can be any unique identifier the archive can use.
            /// @param Password The password necessary to decrypt the asset.
            /// @param Flags A bitmask of the options to open the stream with.  See StreamFlags enum for more info.
            /// @param Raw If true, the stream will perform no processing on the raw data before returning.
            /// @return Returns a shared pointer to an IStream to the opened asset, or null if no asset was found.
            IStreamPtr BufferEncryptedAssetOrThrow(const String& Identifier,
                                                   const String& Password,
                                                   const Whole Flags = SF_Read,
                                                   const Boole Raw = false);
        };//AssetGroup
    }//Resource
}//Mezzanine

#endif
