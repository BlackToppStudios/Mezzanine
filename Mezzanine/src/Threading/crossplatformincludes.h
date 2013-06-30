// The DAGFrameScheduler is a Multi-Threaded lock free and wait free scheduling library.
// © Copyright 2010 - 2013 BlackTopp Studios Inc.
/* This file is part of The DAGFrameScheduler.

    The DAGFrameScheduler is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The DAGFrameScheduler is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with The DAGFrameScheduler.  If not, see <http://www.gnu.org/licenses/>.
*/
/* The original authors have included a copy of the license specified above in the
   'doc' folder. See 'gpl.txt'
*/
/* We welcome the use of the DAGFrameScheduler to anyone, including companies who wish to
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
#ifndef _crossplatformincludes_h
#define _crossplatformincludes_h

    /// @file
    /// @brief This file determines what features are offered by the compiler that this library can take advantage of.
    /// @details This file is dropped when integrating with the Mezzanine. Its contents are manually merged with 
    /// crossplatformexport.h instead.

    // Parts of this library use the TinyThread++ libary and those parts are also covered by the following license
    /*
    Copyright (c) 2010-2012 Marcus Geelnard

    This software is provided 'as-is', without any express or implied
    warranty. In no event will the authors be held liable for any damages
    arising from the use of this software.

    Permission is granted to anyone to use this software for any purpose,
    including commercial applications, and to alter it and redistribute it
    freely, subject to the following restrictions:

        1. The origin of this software must not be misrepresented; you must not
        claim that you wrote the original software. If you use this software
        in a product, an acknowledgment in the product documentation would be
        appreciated but is not required.

        2. Altered source versions must be plainly marked as such, and must not be
        misrepresented as being the original software.

        3. This notice may not be removed or altered from any source
        distribution.
    */

#include "crossplatformexport.h"

#ifdef _MEZZ_THREAD_WIN32_
    #ifdef _MSC_VER
        #pragma warning( disable : 4251) // Disable the dll import/export warnings on items that are set correctly.
        #pragma warning( disable : 4244) // Disable the double to float conversions, they are in their by design to minimize floating point rounding during intermediate calculations.
    #endif
    #ifndef WIN32_LEAN_AND_MEAN
        #define WIN32_LEAN_AND_MEAN
        #define __UNDEF_LEAN_AND_MEAN
    #endif
    #ifndef NOMINMAX
        #define NOMINMAX
    #endif
    #include <windows.h>
    #include <process.h>
    #ifdef __UNDEF_LEAN_AND_MEAN
        #undef WIN32_LEAN_AND_MEAN
        #undef __UNDEF_LEAN_AND_MEAN
    #endif
#else
    #include <pthread.h>
    #include <signal.h>
    #include <sched.h>
    #include <unistd.h>
#endif


#endif // include guard
