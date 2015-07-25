// © Copyright 2010 - 2015 BlackTopp Studios Inc.
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
#ifndef _crossplatformexports_h
#define _crossplatformexports_h

/**
    @file crossplatformexport.h
    @brief This file is used on some platforms to determine what data should be read and written to and from a shared/dynamic library.
    @details Currently this file uses __declspec(dllexport) and __declspec(dllimport) on the windows platform to control what
    is imported from or exported to the Mezzanine DLL. If WINDOWS if defined then MEZZ_LIB will be set to either "__declspec(dllexport)"
    or "__declspec(dllimport)". Every Class, template, function variable, and other item intended to be in Mezzanine is tagged with
    MEZZ_LIB. \n \n

    During compilation of the Mezzanine engine __declspec(dllexport) tells the compiler that a given item (a function, class, template,
    etc...), should be included in the dll and made available for use in games (or whatever kind of applicaitons use Mezzanine). This
    header sets MEZZ_LIB to __declspec(dllexport) if EXPORTINGPHYSGAMEDLL is set. If Mezzanine is compiled using the cmake build
    provided then this is handle automatically. If the cmake build is not used, then this file will attempt to detect if the platform
    it is running on is windows or not. It is preferable that you configure your build environment to define WINDOWS and EXPORTINGMEZZANINEDLL
    if you are compiling this on windows without the provided cmake build. \n \n

    Compilation of applications using Mezzanine __declspec(dllimport), tells the compiler what to expect from the dll and may make
    optimizations available with some compilers. This is automatically set if you use the cmake build, if you aren't this file will
    attempt to determine if you are running windows. It is best to set the build envirnoment of your game or applciation to define
    WINDOWS, if possible copy of our cmake builds for Catch! or the EngineDemo. \n \n

    The Macro MEZZ_LIB is declared as empty if WINDOWS is not defined, as should be the case if LINUX or MACOSX is defined. \n \n


*/
/// @file
/// @brief Sets up some compiler variables that allow the Mezzanine to know what platform it is on.


    // Check for other nonwindows OS
    #ifdef LINUX
        #define NONWINDOWS
    #endif

    #ifdef MACOSX
        #define NONWINDOWS
    #endif

    // Check for windows, but not in a way that overides what is passed on the command prompt
    #ifndef NONWINDOWS
        #ifndef WINDOWS
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
        #endif // WINDOWS
    #endif // \NONWINDOWS

    /// @def MEZZ_LIB
    /// @brief Some platforms require special decorations to denote what is exported/imported in a share library. This is that decoration if when it is needed.
    #ifdef WINDOWS
        /// @def _MEZZ_THREAD_WIN32_
        /// @brief Defined if this is running on windows.
        #define _MEZZ_THREAD_WIN32_

        #ifdef _MSC_VER
            #pragma warning( disable : 4251) // Disable the dll import/export warnings on items that are set correctly.
            #pragma warning( disable : 4244) // Disable the double to float conversions, they are in their by design to minimize floating point rounding during intermediate calculations.
            #pragma warning( disable : 4127) // Disable conditional expression is constant
            #pragma warning( disable : 4800) // pugi xml unspecified bool type coercion has performance cost, used only in unit tests
            #pragma warning( disable : 4221) // interfaces don't generate linkable symbols, well of course they don't
            #pragma warning( disable : 4512) // Could not generate assignment operator for class that doesn't need one
        #endif

        /// @details if this is not defined, then most likely _MEZZ_THREAD_POSIX_ is.
        #ifdef EXPORTINGMEZZANINEDLL
            #define MEZZ_LIB __declspec(dllexport)
        #else
            #define MEZZ_LIB __declspec(dllimport)
        #endif      // \EXPORTINGMEZZANINEDLL
    #else
        #define MEZZ_LIB
        /// @def _MEZZ_THREAD_POSIX_
        /// @brief Defined if this is running on Linux, Mac OS X, Android, or any other sane platform.
        /// @details if this is not defined, then most likely _MEZZ_THREAD_WIN32_ is.
        #define _MEZZ_THREAD_POSIX_
        #if defined(__APPLE__) || defined(__MACH__) || defined(__OSX__)
            /// @def _MEZZ_THREAD_APPLE_
            /// @brief Sometimes specific workarounds are required for Mac OS this is how we detect it.
            #define _MEZZ_THREAD_APPLE_
        #endif
    #endif  // \WINDOWS

    #define _MEZZ_PLATFORM_DEFINED_

    /// @def MEZZ_DEPRECATED
    /// @brief Used to mark old functionality that should not be used as such. In supported compilers using such functionality should produce warnings.
    #ifndef MEZZ_DEPRECATED
        #if defined(__GNUC__)
            #define MEZZ_DEPRECATED __attribute__((deprecated))
        #elif defined(_MSC_VER) && _MSC_VER >= 1300
            #define MEZZ_DEPRECATED __declspec(deprecated)
        #else
            #define MEZZ_DEPRECATED
        #endif
    #endif

    /// @def MEZZ_USEATOMICSTODECACHECOMPLETEWORK
    /// @brief This is used to configure whether to atomically store a shortcut to skip checking all workunits.
    /// @details When this is enabled @ref Mezzanine::Threading::AtomicCompareAndSwap32 "Atomic CAS" operations are used to maintain
    /// a count of the number of complete workunits at the beginning of the work unit listings. Normally these
    /// listings are read-only during frame execution, and the work units store whether or not they are
    /// complete. The default algorithm forces iteration over a large number of work units to simply check for
    /// completion in some situations. If memory bandwidth is slow or limited this can be a large source of
    /// of contention. Enable this option when there are many work units trades atomic operations for memory
    /// bandwidth. This must be tested on a per system basis to determine full preformance ramifications. This
    /// is controlled by the CMake (or other build system) option Mezz_DecacheWorkUnits.
    #ifndef MEZZ_USEATOMICSTODECACHECOMPLETEWORK
        #define MEZZ_USEATOMICSTODECACHECOMPLETEWORK
    #endif
    #ifndef _MEZZ_DECACHEWORKUNIT_
        #undef MEZZ_USEATOMICSTODECACHECOMPLETEWORK
    #endif

    /// @def MEZZ_FRAMESTOTRACK
    /// @brief Used to control how long frames track length and other similar values. This is
    /// controlled by the CMake (or other build system) option Mezz_FramesToTrack.
    #ifndef MEZZ_FRAMESTOTRACK
        #define MEZZ_FRAMESTOTRACK 10
    #endif
    #ifdef _MEZZ_FRAMESTOTRACK_
        #undef MEZZ_FRAMESTOTRACK
        #define MEZZ_FRAMESTOTRACK _MEZZ_FRAMESTOTRACK_
    #endif

    /// @def WINAPI
    /// @brief Calling some win32 api functions require special calling conventions, this is their configuration.
    #ifdef WINDOWS
        #define WINAPI __stdcall
    #else
        #define WINAPI ErrorThisOnlyGoesInwin32Code
    #endif
#endif
