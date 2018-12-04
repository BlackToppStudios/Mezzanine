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
#ifndef _resourcespecialdirectoryutilities_h
#define _resourcespecialdirectoryutilities_h

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

        /// @brief Used a system call to get the current Directory the executable is in. This make an external system call and is likely slower than GetExecutableDirFromArg
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
        /// @return The Directory the game was called from (not necessarily the location of the executable), as a Mezzanine::String
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
