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
#ifndef _resourceutilities_h
#define _resourceutilities_h

#include "datatypes.h"
#include "Resource/archiveentry.h"

namespace Mezzanine
{
    namespace Resource
    {
        ///////////////////////////////////////////////////////////////////////////////
        // MainArg Utilities

        /// @brief Store the Main arguments for later use.
        /// @param ArgCount How many arguments will be passed in ArgVars.
        /// @param ArgVars A pointer to an array, with ArgCount elements, of char* which point to null terminated c strings.
        void MEZZ_LIB CacheMainArgs(int ArgCount, char** ArgVars);

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

        ///////////////////////////////////////////////////////////////////////////////
        // Directory Contents

        /// @brief Gets a listing of file and subdirectory names in a directory.
        /// @param DirectoryPath The directory to look in.
        /// @return Returns a vector of strings containing the names of every subdirectory and file in the directory.
        StringVector MEZZ_LIB GetDirectoryContentNames(const String& DirectoryPath);
        /// @brief Gets a listing of file and subdirectory metadata in a directory.
        /// @param DirectoryPath The directory to look in.
        /// @return Returns a vector of archive entries containing metadata on every file and subdirectory in directory specified.
        ArchiveEntryVector MEZZ_LIB GetDirectoryContents(const String& DirectoryPath);

        ///////////////////////////////////////////////////////////////////////////////
        // Path Utilities

        /// @brief Get the directory portion of a string.
        /// @param PathAndFile A complete path and filename.
        /// @return If passed "/a/b/c.txt" or "c:\windirs\crash.exe" this will return "/a/b/" or "c:\windirs\".
        String MEZZ_LIB GetDirName(const char* PathAndFile);
        /// @brief Get the directory portion of a string.
        /// @param PathAndFile A complete path and filename.
        /// @return If passed "/a/b/c.txt" or "c:\windirs\crash.exe" this will return "/a/b/" or "c:\windirs\".
        String MEZZ_LIB GetDirName(const String& PathAndFile);
        /// @brief Get the filename portion of a string.
        /// @param PathAndFile A complete path and filename.
        /// @return If passed "/a/b/c.txt" or "c:\windirs\crash.exe" this will return "c.txt" or "crash.exe".
        String MEZZ_LIB GetBaseName(const char* PathAndFile);
        /// @brief Get the filename portion of a string.
        /// @param PathAndFile A complete path and filename.
        /// @return If passed "/a/b/c.txt" or "c:\windirs\crash.exe" this will return "c.txt" or "crash.exe".
        String MEZZ_LIB GetBaseName(const String& PathAndFile);

        /// @brief Checks to see if the character is used to separate directories.
        /// @param ToCheck The character to check.
        /// @return Returns true if the character specified is '\\' or '/', false otherwise.
        Boole MEZZ_LIB IsDirectorySeparator(const Char8 ToCheck);
        /// @brief Checks to see if the character is used by the client OS to separate directories.
        /// @param ToCheck The character to check.
        /// @return Returns true on windows if the character specified is '\\' or on linux if it's '/', false otherwise.
        Boole MEZZ_LIB IsHostDirectorySeparator(const Char8 ToCheck);
        /// @brief Get the character used by the client OS to separate directories.
        /// @return Returns a backslash '\\' on windows and forward slash '/' on other operating systems.
        Char8 MEZZ_LIB GetHostDirectorySeparator();
        /// @brief Get the character readable by all platforms to separate directories.
        /// @return Retursn a forward slash '/' always.
        Char8 MEZZ_LIB GetUniversalDirectorySeparator();
        /// @brief Get the character used to separate entries in the system PATH.
        /// @return Semicolon ';' on windows and Forward slash ':' on other operating systems.
        Char8 MEZZ_LIB GetSystemPathSeparator();

        /// @brief Gets whether or not a path is absolute.
        /// @details A path is absolute if it defines an explicit location of a resource. @n @n
        /// This will check if the path is absolute on either windows or posix platforms.
        /// @param ToCheck The path to check.
        /// @return Returns true if the specified path is absolute, false otherwise.
        Boole MEZZ_LIB IsPathAbsolute(const String& ToCheck);
        /// @brief Gets whether or not a path is absolute on the host.
        /// @details A path is absolute if it defines an explicit location of a resource. @n @n
        /// This will call the version of this method that matches the compiled platform.
        /// @param ToCheck The path to check.
        /// @return Returns true if the specified path is considered absolute on the host platform, false otherwise.
        Boole MEZZ_LIB IsPathAbsolute_Host(const String& ToCheck);
        /// @brief Gets whether or not a path is absolute on Posix.
        /// @details A path is absolute if it defines an explicit location of a resource.
        /// @param ToCheck The path to check.
        /// @return Returns true if the specified path is considered absolute on a Posix platform, false otherwise.
        Boole MEZZ_LIB IsPathAbsolute_Posix(const String& ToCheck);
        /// @brief Gets whether or not a path is absolute on Windows.
        /// @details A path is absolute if it defines an explicit location of a resource.
        /// @param ToCheck The path to check.
        /// @return Returns true if the specified path is considered absolute on a Windows platform, false otherwise.
        Boole MEZZ_LIB IsPathAbsolute_Windows(const String& ToCheck);
        /// @brief Gets whether or not a path is relative.
        /// @warning This method is imperfect and doesn't check for a faulty path, just that it's not absolute.
        /// @details A path is relative if it requires using the current working directory to define the location of a resource. @n @n
        /// This will check if the path is absolute on either windows or posix platforms.
        /// @param ToCheck The path to check.
        /// @return Returns true if the specified path is relative, false otherwise.
        Boole MEZZ_LIB IsPathRelative(const String& ToCheck);
        /// @brief Gets whether or not a path is relative on the host.
        /// @warning This method is imperfect and doesn't check for a faulty path, just that it's not absolute.
        /// @details A path is relative if it requires using the current working directory to define the location of a resource. @n @n
        /// This will call the version of this method that matches the compiled platform.
        /// @param ToCheck The path to check.
        /// @return Returns true if the specified path is considered relative on the host platform, false otherwise.
        Boole MEZZ_LIB IsPathRelative_Host(const String& ToCheck);
        /// @brief Gets whether or not a path is relative on Posix.
        /// @warning This method is imperfect and doesn't check for a faulty path, just that it's not absolute.
        /// @details A path is relative if it requires using the current working directory to define the location of a resource.
        /// @param ToCheck The path to check.
        /// @return Returns true if the specified path is considered relative on a Posix platform, false otherwise.
        Boole MEZZ_LIB IsPathRelative_Posix(const String& ToCheck);
        /// @brief Gets whether or not a path is relative on Windows.
        /// @warning This method is imperfect and doesn't check for a faulty path, just that it's not absolute.
        /// @details A path is relative if it requires using the current working directory to define the location of a resource.
        /// @param ToCheck The path to check.
        /// @return Returns true if the specified path is considered relative on a Windows platform, false otherwise.
        Boole MEZZ_LIB IsPathRelative_Windows(const String& ToCheck);

        /// @brief Gets how many directories deep a path is.
        /// @param ToCheck The path to get the depth of.
        /// @param ExitIfNegative If true, this will cause the function to return immediately if the depth count becomes negative.
        /// @return Returns an Integer representing how many directories down (or up, if negative) the path goes.
        Integer MEZZ_LIB GetDirectoryDepth(const String& ToCheck, const Boole ExitIfNegative);
        /// @brief Checks whether or not a given path is a subdirectory or file of a base path.
        /// @note This method will make no attempt to remove dot segments or perform any other normalization routine to the
        /// provided path, thus these transformations must be done manually before calling this method if they are needed.
        /// @param BasePath The base path to check.
        /// @param CheckPath The path being checked against base path to see if it points to a subdirectory or file.
        /// @return Returns true if CheckPath is a subdirectory or file of BasePath.
        Boole MEZZ_LIB IsSubPath(const String& BasePath, const String& CheckPath);

        /// @brief Builds a String path based on a number of directory/file names in a String vector.
        /// @note The directory separators generated by this method are Universal/Unix style. @n
        /// This function will NOT include a drive letter for Windows paths, that must be added manually after.
        /// @param ToBuild A vector of Strings containing the overall path to be built.
        /// @param EndWithSlash Whether or not to end the built path with a Universal separator (for directories).
        /// @return Returns a String containing the rebuilt path.
        String MEZZ_LIB BuildPath(const StringVector& ToBuild, const Boole EndWithSlash);
        /// @brief Removes all instances of "." or ".." and makes appropriate edits to the path.
        /// @details A dot segment is "." or "..".  They often get in the way of path parsing and this method will remove
        /// any extraneous dot segments that may exist in the provided String.
        /// @param ToRemove The String to remove dot segments from.
        /// @return Returns a copy of the ToRemove parameter with all dot segments removed.
        String MEZZ_LIB RemoveDotSegments(const String& ToRemove);

        /// @brief Convenience method to verify the necessary system separator is present when concatenating.
        /// @param FilePath The directory path to the file.
        /// @param FileName The name of the file.
        /// @return Returns a full string that is the concatenated path and filename.
        String MEZZ_LIB CombinePathAndFileName(const String& FilePath, const String& FileName);

        ///////////////////////////////////////////////////////////////////////////////
        // System Path Utilities

        /// @brief Get the $PATH or %PATH% split and order for easy checking of how the OS does it.
        /// @param PATH Defaults to the PATH environment variable. But any value like a system path will be split the return of GetPathSeparator().
        /// @return A collection of directories that this system will for executables in the order they will be checked.
        StringVector MEZZ_LIB GetSystemPATH(const String& PATH = String(getenv("PATH")));

        /// @brief Search the system path the same way most systems do to find an executable.
        /// @warning This function is case sensitive and not all operating systems are.
        /// @todo Add support for extension handling on windows. "cmd" should find "cmd.exe" in system32, but currently "cmd.exe" needs to be searched.
        /// @param ExecutableName The executable to look for.
        /// @return If the executable is not found "" is returned otherwise the first directory in the PATH containing it is returned.
        String MEZZ_LIB Which(const String& ExecutableName);

        ///////////////////////////////////////////////////////////////////////////////
        // Execution Directory Utilities

        /// @brief Get the Path to the current executable, in a fast way if possible.
        /// @return A String containing the path to the current executable.
        String MEZZ_LIB GetExecutableDir();
        /// @brief Get the Path to the current executable, fast from Args if Possible or from a system call otherwise.
        /// @param ArgCount How many arguments will be passed in ArgVars.
        /// @param ArgVars A pointer to an array, with ArgCount elements, of char* which point to null terminated c strings.
        /// @warning If you pass bogus arguments to this bad things can and will happen. Infinite loops, segfaults etc... Just pass what main gives you
        /// @warning Not all system provide all the needed information to determine the executable directory
        /// @return A String containing the path to the current executable.
        String MEZZ_LIB GetExecutableDir(int ArgCount, char** ArgVars);

        /// @brief Used a system call to get the curent Directory the executable is in. This make an external system call and is likely slower than GetExecutableDirFromArg
        /// @return This will return the current path this executable is stored in.
        String MEZZ_LIB GetExecutableDirFromSystem();

        /// @brief Uses the main parameters stored using "CacheMainArgs" to attempt determine executable directory.
        /// @return Either a valid Path, '.' if the working dir is likely correct or "" if nothing could be determined.
        String MEZZ_LIB GetExecutableDirFromArg();
        /// @brief Attempt to get the executable directory from the a set of variables like those passed into Main.
        /// @note This is the fastest way to get the Executable location, but might not work on all platforms.
        /// @warning If you pass bogus arguments to this bad things can and will happen. Infinite loops, segfaults etc... Just pass what main gives you.
        /// @warning Not all systems provide all the needed information to determine the executable directory.
        /// @param ArgCount How many arguments will be passed in ArgVars.
        /// @param ArgVars A pointer to an array, with ArgCount elements, of char* which point to null terminated c strings.
        /// @return If a whole path is present in ArgVars[0] this returns the directory part of that path, if this uses the executable file this returns '.', otherwise this with return "" indicating it is not usable.
        String MEZZ_LIB GetExecutableDirFromArg(int ArgCount, char** ArgVars);

        ///////////////////////////////////////////////////////////////////////////////
        // Working Directory Utilities

        /// @brief Change directory, to the directory indicated.
        /// @param ChangeTo The new directory to work from.
        void MEZZ_LIB ChangeWorkingDirectory(const String& ChangeTo);
        /// @brief Get the working directory as a Mezzanine::String
        /// @return The Directory the game was called from (not nescessarilly the location of the executable), as a Mezzanine::String
        String MEZZ_LIB GetWorkingDirectory();

        ///////////////////////////////////////////////////////////////////////////////
        // Application Data Directory Utilities

        /// @brief Resolves a string describing one of the platform data paths to the actual path it is.
        /// @remarks Currently there are only 4 preset path variables, and depending on platform they two or more may go to the same location.
        /// The valid variables are as follows: LocalAppData, ShareableAppData, CurrentUserData, and CommonUserData.  These are not case sensative.
        /// @param PathVar String containing the name of the path variable.
        /// @return Returns the actual path of the variable provided.
        String MEZZ_LIB ResolveDataPathFromString(const String& PathVar);

        /// @brief Gets the path to the directory intended for game and engine config data that is not meant to be shared.
        /// @return Returns a string containing the path to the Local Application Data Directory.
        String MEZZ_LIB GetLocalAppDataDir();
        /// @brief Gets the path to the directory intended for game and engine config data that is allowed to be shared.
        /// @return Returns a string containing the path to the Shareable Application Data Directory.
        String MEZZ_LIB GetShareableAppDataDir();
        /// @brief Gets the path to the directory intended for game saves and user profile data for the current user.
        /// @return Returns a string containing the path to the Current User Data Directory.
        String MEZZ_LIB GetCurrentUserDataDir();
        /// @brief Gets the path to the directory intended for game saves and user profile data for all users.
        /// @return Returns a string containing the path to the Common User Data Directory.
        String MEZZ_LIB GetCommonUserDataDir();
    }//Resource
}//Mezzanine

#endif
