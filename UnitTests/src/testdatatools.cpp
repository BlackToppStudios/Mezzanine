// © Copyright 2010 - 2014 BlackTopp Studios Inc.
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
#ifndef _testdatatools_cpp
#define _testdatatools_cpp

/// @file
/// @brief The implementation of stuff that must be run in the context of a TestData

#include "testdata.h"

#include <vector>
#include <stdexcept>
#include <sstream>

#ifdef _MEZZ_THREAD_WIN32_
    #include <windows.h>
#else
    #ifdef _MEZZ_THREAD_APPLE_
        #include <sys/sysctl.h>
    #endif
    #include <sys/time.h>
    #include <unistd.h>
#endif

using namespace Mezzanine;
using namespace std;

namespace Mezzanine
{
    namespace Testing
    {
        #ifdef _MEZZ_THREAD_WIN32_
            namespace
            {
                /// @internal
                class Timer
                {
                    public:
                        LARGE_INTEGER frequency;

                        Timer()
                            { QueryPerformanceFrequency(&frequency); }

                        MaxInt GetTimeStamp()
                        {
                            LARGE_INTEGER Current;
                            QueryPerformanceCounter(&Current);
                            return MaxInt(Current.QuadPart * (1000000.0 / frequency.QuadPart));
                        }
                };

                static Timer ATimer;
            }

            MaxInt Now()
                { return ATimer.GetTimeStamp(); }

            Whole NowResolution()
                { return Whole(ATimer.frequency.QuadPart/1000); }

        #else
            MaxInt Now()
            {
                timeval Now;
                gettimeofday(&Now, NULL); // Posix says this must return 0, so it seems it can't fail
                return (Now.tv_sec * 1000000) + Now.tv_usec;
            }

            Whole NowResolution()
                { return 1; } // barring kernel bugs

        #endif
    }// Testing
}// Mezzanine

#endif

