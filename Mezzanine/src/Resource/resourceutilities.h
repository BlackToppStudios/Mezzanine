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
#ifndef _resourceutilities_h
#define _resourceutilities_h

#include "datatypes.h"

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
        // File Management

        /*/// @brief Deletes a file existing on the filesystem.
        /// @param PathAndFile A string containing both the path to the directory where the file is located, and the name of the file to be deleted.
        /// @return Returns true if the operation was successful, false if it failed.
        Boole MEZZ_LIB RemoveFile(const String& PathAndFile);// */

        ///////////////////////////////////////////////////////////////////////////////
        // Basic Directory Management

        /// @brief Creates a single new directory.
        /// @remarks This function will only create the directory specified at the end of the path.
        /// @param DirectoryPath The path for the newly created directory.
        /// @return Returns true if the directory was created, false in the case of a non-critical error.
        Boole MEZZ_LIB CreateDirectory(const String& DirectoryPath);
        /// @brief Creates all directories that do not exist in the provided path.
        /// @param DirectoryPath The path for the newly created directory or directories.
        /// @return Returns true if all directories were created, false in the case of a non-critical error.
        Boole MEZZ_LIB CreateDirectoryPath(const String& DirectoryPath);
        /// @brief Checks to see if the given path exists and if it is a folder.
        /// @exception On Error this might throw a Mezzanine::IOException with detail about why it failed.
        /// @param DirectoryPath A String containing the path to test.
        /// @return True if the item indicated by DirectoryPath exists and it is a directory, false if it does not exist or exists but is a file.
        Boole MEZZ_LIB DoesDirectoryExist(const String& DirectoryPath);
        /// @brief Remove an empty directory.
        /// @exception On Error this might throw a Mezzanine::IOException with details about why it failed.
        /// @param DirectoryPath The Path to the directory to remove.
        void MEZZ_LIB RemoveDirectory(const String& DirectoryPath);

        /// @brief Get a Listing of the files and subdirectories in a directory.
        /// @details This follows normal command line conventions, "." is the current directory,
        /// ".." is the parent directory. To access the file system root you will need to use a
        /// leading "c:/", "c:\\", or "/" as appropriate for the operating system the software will run on.
        /// @param Dir The directory to check.
        /// @return This will return a pointer to a set of Strings the caller is responsible for deleting or a null pointer on an error.
        StringVector MEZZ_LIB GetDirContents(const String& Dir = ".");

        ///////////////////////////////////////////////////////////////////////////////
        // Path Utilities

        /// @brief Get the directory portion of a string.
        /// @param FileName A complete path and filename.
        /// @return If passed "/a/b/c.txt" or "c:\windirs\crash.exe" this will return "/a/b/" or "c:\windirs\".
        String MEZZ_LIB DirName(const String& FileName);
        /// @brief Get the filename portion of a string.
        /// @param FileName A complete path and filename.
        /// @return If passed "/a/b/c.txt" or "c:\windirs\crash.exe" this will return "c.txt" or "crash.exe".
        String MEZZ_LIB BaseName(const String& FileName);

        /// @brief Get the character used to separate directories
        /// @return Backslash '\' on windows and Forward slash '/' on other operating systems.
        Char8 MEZZ_LIB GetDirectorySeparator();
        /// @brief Get the character used to separate entries in the system PATH
        /// @return Semicolon ';' on windows and Forward slash ':' on other operating systems.
        Char8 MEZZ_LIB GetPathSeparator();

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
