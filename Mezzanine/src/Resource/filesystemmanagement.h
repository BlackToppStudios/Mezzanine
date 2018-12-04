// © Copyright 2010 - 2018 BlackTopp Studios Inc.
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
#ifndef _resourcefilesystemmanagement_h
#define _resourcefilesystemmanagement_h

#include "datatypes.h"

namespace Mezzanine
{
    namespace Resource
    {
        ///////////////////////////////////////////////////////////////////////////////
        // Basic File Management

        /// @brief Verifies the existence of a file.
        /// @param PathAndFile The path and name of the file to check for.
        /// @return Returns true if the file at the specified location exists, false otherwise.
        Boole MEZZ_LIB FileExists(const String& PathAndFile);

        /// @brief Copies a file on disk to a new location.
        /// @note This function makes no attempt to copy file permissions or attributes, only data.
        /// @param OldPathAndFile The existing path to the file (including the filename) to be copied.
        /// @param NewPathAndFile The path (including the filename) to where the file should be copied.
        /// @param FailIfExists If true the operation will fail if a file with the target name already exists.
        /// @return Returns true if the file was successfully copied, false otherwise.
        Boole MEZZ_LIB CopyFile(const String& OldPathAndFile, const String& NewPathAndFile, const Boole FailIfExists);
        /// @brief Moves a file on disk from one location to another.
        /// @remarks This function can be used to rename files.
        /// @param OldPathAndFile The existing path to the file (including the filename) to be moved.
        /// @param NewPathAndFile The path (including the filename) to where the file should be named.
        /// @param FailIfExists If true the operation will fail if a file with the target name already exists.
        /// @return Returns true if the file was successfully moved, false otherwise.
        Boole MEZZ_LIB MoveFile(const String& OldPathAndFile, const String& NewPathAndFile, const Boole FailIfExists);
        /// @brief Deletes a file existing on the filesystem.
        /// @param PathAndFile A string containing both the path to the directory where the file is located, and the name of the file to be deleted.
        /// @return Returns true if the operation was successful, false if it failed.
        Boole MEZZ_LIB RemoveFile(const String& PathAndFile);

        ///////////////////////////////////////////////////////////////////////////////
        // Symlinks

        /// @brief Creates a symbolic link to a file on disk.
        /// @param SymPath A path (including the name of the symbolic link) to where the link should be placed.
        /// @param TargetPath A path to where the symbolic link will point to.
        /// @return Returns true if the operation was successful, false otherwise.
        Boole MEZZ_LIB CreateSymlink(const String& SymPath, const String& TargetPath);
        /// @brief Creates a symbolic link to a directory on disk.
        /// @param SymPath A path (including the name of the symbolic link) to where the link should be placed.
        /// @param TargetPath A path to where the symbolic link will point to.
        /// @return Returns true if the operation was successful, false otherwise.
        Boole MEZZ_LIB CreateDirectorySymlink(const String& SymPath, const String& TargetPath);

        ///////////////////////////////////////////////////////////////////////////////
        // Basic Directory Management

        /// @brief Verifies the existence of a folder.
        /// @param DirectoryPath The path and name of the folder to check for.
        /// @return Returns true if the folder at the specified location exists, false otherwise.
        Boole MEZZ_LIB DirectoryExists(const String& DirectoryPath);

        /// @brief Creates a single new directory.
        /// @remarks This function will only create the directory specified at the end of the path.
        /// @param DirectoryPath The path for the newly created directory.
        /// @return Returns true if the directory was created, false in the case of a non-critical error.
        Boole MEZZ_LIB CreateDirectory(const String& DirectoryPath);
        /// @brief Creates all directories that do not exist in the provided path.
        /// @param DirectoryPath The path for the newly created directory or directories.
        /// @return Returns true if all directories were created, false in the case of a non-critical error.
        Boole MEZZ_LIB CreateDirectoryPath(const String& DirectoryPath);
        /// @brief Remove an empty directory.
        /// @param DirectoryPath The Path to the directory to remove.
        /// @return Returns true if the directory was successfully removed, false otherwise.
        Boole MEZZ_LIB RemoveDirectory(const String& DirectoryPath);
    }//Resource
}//Mezzanine

#endif
