//© Copyright 2010 - 2011 BlackTopp Studios Inc.
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
#ifndef _crossplatform_h
#define _crossplatform_h
///////////////////////////////////////////////////////////////////////////////
/// @namespace Mezzanine::crossplatform
/// @brief All functionality that needs different implemenations per platform will go in here
/// @details If we did our jobs right You not need to change anything to compile on
/// different platforms exvept the build target. If you want, the platform can
/// be manually defined in this section and this should be the only place that you
/// need to change to compile this on a supported platform. Just remark all the
/// lines that are not your platform using "//" and unremark your platform.
/// \n\n
/// Should you choose to port this to your platform, make sure that all the
/// required libraries are installed, then make sure to write an implementation
/// for each of the functions in crossplatform.cpp, then you should get to the
/// nitty gritty of making the minor platforms inconsistencies work.
/// \n\n
/// For most games there will be no need to directly call these functions, however if you decide
/// you game is an exception, there is one key thing to remember about all of these functions.
/// All of these may perform/behave slightly differently.
///////////////////////////////////////////////////////////////////////////////

/// @todo Make main page in documentation with wiki link and listing of required steps (build target and OS define), and dependencies required to build this.

//Unremark one of these when not compiling with code::blocks or cmake
//#define LINUX
//#define MACOSX
//#define WINDOWS

//End Choose Your Platform
///////////////////////////////////////////////////////////////////////////////

// Includes
#include "crossplatformexport.h"
#include "datatypes.h"

// \internal
// Forward declaration for compatibility
namespace Ogre
{
    class RenderWindow;
}

struct SDL_Window;

namespace Mezzanine
{
    //Forward Declarations should go here
    class World;

    namespace crossplatform
    {
        /// @internal
        /// @brief This creates a data structure that can help SDL(User Input Subsystem) with Ogre(graphics subsystem)
        /// @details This creates a data structure that can help SDL(User Input Subsystem) with Ogre(graphics subsystem)
        /// This returns a named parameter list with valid settings to use Ogre rendering on a pre-existing SDL context
        /// @warning This is an engine internal, and shouldn't be used anywhere else. For all practical purposes is return gibberish
        void* GetSDLOgreBinder(SDL_Window* window, const size_t& winGlContext);

        /// @brief Pauses the program for a given period of time.
        /// @details Pauses the program for a given period of time.
        /// @param WaitTime The amount of time in milliseconds to wait
        void MEZZ_LIB WaitMilliseconds(const Whole &WaitTime);

        /// @brief Gets the platform currently being run on.
        /// @return Returns a string based on the platform.  "Windows", "Linux", or "MacOSX".
        String MEZZ_LIB GetPlatform();

        /// @brief Gets cleaned dimensions for a game window.
        /// @param Width The desired width of the window area.
        /// @param Height The desired height of the window area.
        /// @param ActualWidth The modified value of the rendering width, after window decorations have been taken into account.
        /// @param ActualHeight The modified value of the rendering height, after window decorations have been taken into account.
        void MEZZ_LIB SanitizeWindowedRes(const Whole& Width, const Whole& Height, Whole& ActualWidth, Whole& ActualHeight);

        /// @brief Get the working directory as a Mezzanine::String
        /// @return The Directory the game was called from (not nescessarilly the location of the executable), as a Mezzanine::String
        String MEZZ_LIB GetWorkingDir();

        /// @brief Get a Listing of the files and subdirectories in a directory.
        /// @details This follows normal command line conventions, "." is the current directory,
        /// ".." is the parent directory. To access the file system root you will need to use a
        /// leading "c:/", "c:\\", or "/" as appropriate for the operating system the software will run on.
        /// @return this will return a pointer to a set of Strings the caller is responsible for deleting or a null pointer on an error.
        /// @param Dir The directory to check.
        std::set<String>* MEZZ_LIB GetDirContents(const String& Dir = ".");
    }
}
#endif
