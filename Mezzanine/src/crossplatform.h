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
/// your game is an exception, there is one key thing to remember about all of these functions.
/// All of these may perform/behave slightly differently.
///////////////////////////////////////////////////////////////////////////////

//Unremark one of these when not compiling with code::blocks or cmake
//#define MEZZ_LINUX
//#define MEZZ_MACOSX
//#define MEZZ_WINDOWS

//End Choose Your Platform
///////////////////////////////////////////////////////////////////////////////

// Includes
#include "crossplatformexport.h"
#include "datatypes.h"


// Forward declaration for compatibility
/// @internal
namespace Ogre
{
    class RenderWindow;
}

struct SDL_Window;

namespace Mezzanine
{
    //Forward Declarations should go here
    class Entresol;

    namespace crossplatform
    {
        /// @brief Gets the platform currently being run on.
        /// @return Returns a string based on the platform.  "Windows", "Linux", or "MacOSX".
        String MEZZ_LIB GetPlatform();

        /// @brief Get a timestamp.
        /// @warning On some platforms this requires a static initialization, an can cause undefined behavior if called before static initializations are complete
        /// @return The largest size integer containing a timestamp that can be compared to other timestamps, but hads no guarantees for external value.
        MaxInt MEZZ_LIB GetTimeStamp();

        #ifndef SWIG
        /// @brief Get the resolution of the timestamp in microseconds.
        /// @return A Whole which returns in millionths of a second the smallest unit of time that GetTimeStamp can measure.
        Whole MEZZ_LIB GetTimeStampResolution();

        /// @brief Get the amount of logical processors, a reasononable amount to use for thread creation.
        /// @details This returns whatever your OS thinks is the count of CPUs. This could include
        /// Hyperthreading unit on Intel's chip, or it might not, it could include the threads from
        /// Niagra CPUs or it might not, it could return just about any value on a given piece of
        /// and should return a reasonable value for how many threads should be used.
        /// @return A Whole containing the amount of processors.
        Whole MEZZ_LIB GetCPUCount();
        #endif //SWIG
    }// Crossplatform
}//Mezzanine

#endif
