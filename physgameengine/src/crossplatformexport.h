//© Copyright 2010 BlackTopp Studios Inc.
/* This file is part of The PhysGame Engine.

    The PhysGame Engine is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The PhysGame Engine is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with The PhysGame Engine.  If not, see <http://www.gnu.org/licenses/>.
*/
/* The original authors have included a copy of the license specified above in the
   'Docs' folder. See 'gpl.txt'
*/
/* We welcome the use of The PhysGame anyone, including companies who wish to
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
#ifndef _crossplatformexports_h
#define _crossplatformexports_h

/**
    @file crossplatformexport.h
    @brief This file is used on some platforms to determine what data should be read and written to and from a shared/dynamic library.
    @details Currently this file uses __declspec(dllexport) and __declspec(dllimport) on the windows platform to control what
    is imported from or exported to the Physgame DLL. If WINDOWS if defined then PHYS_LIB will be set to either "__declspec(dllexport)"
    or "__declspec(dllimport)". Every Class, template, function variable, and other item intended to be in physgame is tagged with
    PHYS_LIB. \n \n

    During compilation of the Physgame engine __declspec(dllexport) tells the compiler that a given item (a function, class, template,
    etc...), should be included in the dll and made available for use in games (or whatever kind of applicaitons use Physgame). This
    header sets PHYS_LIB to __declspec(dllexport) if EXPORTINGPHYSGAMEDLL is set. If Physgame is compiled using the cmake build
    provided then this is handle automatically. If the cmake build is not used, then this file will attempt to detect if the platform
    it is running on is windows or not. It is preferable that you configure your build environment to define WINDOWS and EXPORTINGPHYSGAMEDLL
    if you are compiling this on windows without the provided cmake build. \n \n

    Compilation of applications using Physgame __declspec(dllimport), tells the compiler what to expect from the dll and may make
    optimizations available with some compilers. This is automatically set if you use the cmake build, if you aren't this file will
    attempt to determine if you are running windows. It is best to set the build envirnoment of your game or applciation to define
    WINDOWS, if possible copy of our cmake builds for Catch! or the EngineDemo. \n \n

    The Macro PHYS_LIB is declared as empty if WINDOWS is not defined, as should be the case if LINUX or MACOSX is defined. \n \n


*/

// Check for other nonwindows OS
#ifdef LINUX
    #define NONWINDOWS
#endif

#ifdef MACOSX
    #define NONWINDOWS
#endif

//Check for windows, but not in a way that overides what is passed on the command prompt
#ifdef NONWINDOWS
    #ifdef __WIN32__
    #define WINDOWS
    #endif

    #ifdef _WIN32
    #define WINDOWS
    #endif

    #ifdef __CYGWIN32__
    #define WINDOWS
    #endif

    #ifdef _MSC_VER
    #define WINDOWS
    #endif
#endif // \NONWINDOWS

//Used to define whether a class is exported as part of the shared library when compiled
#ifdef WINDOWS
    #ifdef EXPORTINGPHYSGAMEDLL
        #define PHYS_LIB __declspec(dllexport)
    #else
        #define PHYS_LIB __declspec(dllimport)
    #endif      // \MAKINGPHYSGAMEDLL
#else
    #define PHYS_LIB
#endif  // \WINDOWS

#endif
