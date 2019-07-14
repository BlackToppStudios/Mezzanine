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
#ifndef _archiveadapter_h_cpp
#define _archiveadapter_h_cpp

// Keeps this file from being documented by doxygen
/// @cond DontDocumentInternal

#include "datatypes.h"

#include <OgreArchive.h>

namespace Mezzanine
{
    namespace Resource
    {
        class ArchiveReader;
    }
    namespace Graphics
    {
        ///////////////////////////////////////////////////////////////////////////////
        /// @brief A simple adapter class for connecting Ogres resource system to the Mezzanine resource system.
        ///////////////////////////////////////
        class MEZZ_LIB ArchiveAdapter : public Ogre::Archive
        {
        protected:
            /// @brief A pointer to the Mezzanine group this adapter is wrapping.
            Resource::ArchiveReader* MezzReader;
        public:
            /// @brief Detailed constructor.
            /// @param Name The name/location of the archive.
            ArchiveAdapter(const String& Name, Resource::ArchiveReader* Reader);
            /// @brief Class destructor.
            ~ArchiveAdapter();

            ///////////////////////////////////////////////////////////////////////////////
            // Ogre Archive Overrides

            /// @brief Checks to verify a file exists in the archive.
            /// @param FileName The name of the file to check for.
            /// @return Returns true if the file was found in the archive, false otherwise.
            virtual bool exists(const String& FileName) override;
            /// @brief Gets the name of every entry that matches the specified pattern.
            /// @param Pattern The pattern to search for.  Supports simple wildcards such as '*' and '?'.
            /// @param Recursive Whether or not subdirectories should be searched.
            /// @param Dirs Whether or not directories should be included in the returned results.
            /// @return Returns a shared_ptr to a vector containing all of the matched names.
            virtual Ogre::StringVectorPtr find(const String& Pattern, bool Recursive = true, bool Dirs = false) override;
            /// @brief Gets the file info of every entry that match the specified pattern.
            /// @param Pattern The pattern to search for.  Supports simple wildcards such as '*' and '?'.
            /// @param Recursive Whether or not subdirectories should be searched.
            /// @param Whether or not directories should be included in the returned results.
            /// @return Returns a shared_ptr to a vector containing file info of all the matched entries.
            virtual Ogre::FileInfoListPtr findFileInfo(const String& Pattern, bool Recursive = true, bool Dirs = false) override;
            /// @brief Gets the last time a file in the archive was modified.
            /// @param FileName The name of the file to check.
            /// @return Returns a time_t that is the last modified time of the specified file.
            virtual time_t getModifiedTime(const String& FileName) override;
            /// @brief Gets whether the names of files on this archive are case sensitive.
            /// @return Returns true if entry names in this archive are case sensitive, false otherwise.
            virtual bool isCaseSensitive() const override;
            /// @brief Gets the name of every entry in this archive.
            /// @param Recursive Whether or not subdirectories should be searched.
            /// @param Whether or not directories should be included in the returned results.
            /// @return Returns a shared_ptr to a vector containing all entry names in this archive.
            virtual Ogre::StringVectorPtr list(bool Recursive = true, bool Dirs = false) override;
            /// @brief Gets the file info of every entry in this archive.
            /// @param Recursive Whether or not subdirectories should be searched.
            /// @param Whether or not directories should be included in the returned results.
            /// @return Returns a shared_ptr to a vector containing file info of all entries in this archive.
            virtual Ogre::FileInfoListPtr listFileInfo(bool Recursive = true, bool Dirs = false) override;

            /// @brief Initializes the archive to be ready for use.
            virtual void load() override;
            /// @brief Opens a file for reading/writing.
            /// @param FileName The path and name of the file to be opened/loaded.
            /// @param ReadOnly Whether or not to only allow reading of the file, and disallow writing.
            /// @return Returns a shared_ptr containing a stream to the opened file.
            virtual Ogre::DataStreamPtr open(const String& FileName, bool ReadOnly = true) override;
            /// @brief Deinitializes and closes the archive.
            virtual void unload() override;
        };//ArchiveAdapter
    }//Graphics
}//Mezzanine

/// @endcond

#endif
