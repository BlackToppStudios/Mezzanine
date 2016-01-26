// © Copyright 2010 - 2016 BlackTopp Studios Inc.
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

#include "Resource/datastream.h"

namespace Mezzanine
{
    namespace Resource
    {
        ///////////////////////////////////////////////////////////////////////////////
        /// @brief This is a class that stores a specific grouping of Assets, usually based on thier location.
        /// @details
        ///////////////////////////////////////
        class AssetGroup
        {
        public:
            /// @brief Basic container type for @ref DataStream storage by this class.
            typedef std::vector<Resource::DataStreamPtr>    UnnamedAssetContainer;
            /// @brief Iterator type for @ref DataStream instances stored by this class.
            typedef UnnamedAssetContainer::iterator         UnnamedAssetIterator;
            /// @brief Const Iterator type for @ref DataStream instances stored by this class.
            typedef UnnamedAssetContainer::const_iterator   ConstUnnamedAssetIterator;
            /// @brief Container class for storing @ref DataStream instances in this class.
            typedef std::map<String,DataStreamPtr>          AssetContainer;
            /// @brief Iterator type for @ref DataStream instances stored in this class.
            typedef AssetContainer::iterator                AssetIterator;
            /// @brief Const Iterator type for @ref DataStream instances stored in this class.
            typedef AssetContainer::const_iterator          ConstAssetIterator;
        protected:
            /// @internal
            /// @brief Container storing all of the unnamed open streams bleonging to this group.
            UnnamedAssetContainer UnnamedGroupAssets;
            /// @internal
            /// @brief Container storing all of the named open streams belonging to this group.
            AssetContainer GroupAssets;
            /// @internal
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

            /// @brief Prepares some Assets for use within this group.
            /// @details After adding all of your assets and declaring them as nessessary, this function
            /// is the final step.  After calling this function any and all assets within the defined group
            /// will be ready to use.  Do not initialize any more groups then you need to however, as that will
            /// take up memory and drop performance.
            void InitializeAssets();

            ///////////////////////////////////////////////////////////////////////////////
            // Stream Management

            /// @brief Opens a stream to an asset in an AssetGroup.
            /// @param AssetName The identity of the asset to be opened (commonly a file name).
            Resource::DataStreamPtr OpenAssetStream(const String& AssetName);

            /// @brief Creates a stream from a memory buffer.
            /// @note The created stream will take ownership of the buffer you provide.  If you want it to have a separate buffer then create a copy and pass that in.
            /// @param Buffer A pointer to the memory to stream from.
            /// @param BufferSize The size of the provided buffer in bytes.
            /// @return Returns a @ref CountedPtr to the stream to the provided buffer.
            Resource::DataStreamPtr CreateDataStream(void* Buffer, const UInt32 BufferSize);
            /// @brief Creates a named stream from a memory buffer.
            /// @note The created stream will take ownership of the buffer you provide.  If you want it to have a separate buffer then create a copy and pass that in.
            /// @param AssetName The name to be given to the created stream.
            /// @param Buffer A pointer to the memory to stream from.
            /// @param BufferSize The size of the provided buffer in bytes.
            /// @return Returns a @ref CountedPtr to the stream to the provided buffer.
            Resource::DataStreamPtr CreateDataStream(const String& AssetName, void* Buffer, const UInt32 BufferSize);
        };//AssetGroup
    }//Resource
}//Mezzanine

#endif
