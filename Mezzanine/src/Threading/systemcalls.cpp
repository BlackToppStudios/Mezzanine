// The DAGFrameScheduler is a Multi-Threaded lock free and wait free scheduling library.
// © Copyright 2010 - 2014 BlackTopp Studios Inc.
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
#ifndef _systemcalls_cpp
#define _systemcalls_cpp

/// @file
/// @brief This file defines some platform specifc functions

#include "datatypes.h"
#include "systemcalls.h"
#include "crossplatformincludes.h"

#ifdef _MEZZ_CPP11_
    #include<chrono>
#else
    #ifdef _MEZZ_THREAD_WIN32_
        #include <windows.h>
    #else
        #ifdef _MEZZ_THREAD_APPLE_
            #include <sys/sysctl.h>
        #endif
        #include <sys/time.h>
        #include <unistd.h>
    #endif
#endif

namespace Mezzanine
{
    #ifdef _MEZZ_CPP11_
    MaxInt GetTimeStamp()
        { return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now()).count(); }
    #else
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

            MaxInt GetTimeStamp()
                { return ATimer.GetTimeStamp(); }

            Whole GetTimeStampResolution()
                { return Whole(ATimer.frequency.QuadPart/1000); }

        #else
            MaxInt GetTimeStamp()
            {
                timeval Now;
                gettimeofday(&Now, NULL); // Posix says this must return 0, so it seems it can't fail
                return (Now.tv_sec * 1000000) + Now.tv_usec;
            }

            Whole GetTimeStampResolution()
                { return 1; } // barring kernel bugs

        #endif
    #endif

    Whole MEZZ_LIB GetCPUCount()
    {
        #ifdef _MEZZ_THREAD_WIN32_
        SYSTEM_INFO sysinfo;
        GetSystemInfo( &sysinfo );

        return sysinfo.dwNumberOfProcessors;
        #else
            return sysconf( _SC_NPROCESSORS_ONLN );
        #endif
    }

    Whole GetCacheSize()
    {
        #ifdef _MEZZ_THREAD_WIN32_
            #ifdef _MSC_VER
                size_t Size = 0;
                DWORD buffer_size = 0;
                DWORD i = 0;
                SYSTEM_LOGICAL_PROCESSOR_INFORMATION * buffer = 0;

                GetLogicalProcessorInformation(0, &buffer_size);
                buffer = (SYSTEM_LOGICAL_PROCESSOR_INFORMATION *)malloc(buffer_size);
                GetLogicalProcessorInformation(&buffer[0], &buffer_size);

                for (i = 0; i != buffer_size / sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION); ++i) {
                    if (buffer[i].Relationship == RelationCache && buffer[i].Cache.Level == 1) {
                        Size = buffer[i].Cache.Size;
                        break;
                    }
                }

                free(buffer);
                return Size;
            #else
                //mingw can't do this, see bug http://sourceforge.net/p/mingw/bugs/1626/
                // assume 32k
                return 32768;
            #endif
        #else
            #ifdef _MEZZ_THREAD_APPLE_
                size_t CacheSize;
                size_t CSSize = sizeof(CacheSize);
                if(0==sysctlbyname("hw.l4icachesize", &CacheSize, &CSSize, NULL, 0))
                {
                    return CacheSize;
                }else{
                    if(0==sysctlbyname("hw.l3icachesize", &CacheSize, &CSSize, NULL, 0))
                    {
                        return CacheSize;
                    }else{
                        if(0==sysctlbyname("hw.l2icachesize", &CacheSize, &CSSize, NULL, 0))
                        {
                            return CacheSize;
                        }else{
                            if(0==sysctlbyname("hw.l1icachesize", &CacheSize, &CSSize, NULL, 0))
                            {
                                return CacheSize;
                            }else{
                                return 0;
                            }
                        }
                    }
                }
            #else
                Whole CSSize = sysconf(_SC_LEVEL4_CACHE_SIZE);
                if(!CSSize)
                {
                    CSSize = sysconf(_SC_LEVEL3_CACHE_SIZE);
                    if(!CSSize)
                    {
                        CSSize = sysconf(_SC_LEVEL2_CACHE_SIZE);
                        if(!CSSize)
                            { CSSize = sysconf(_SC_LEVEL1_DCACHE_SIZE); }
                    }
                }
                return CSSize;
           #endif
        #endif
    }

    Whole GetCachelineSize()
    {
        #ifdef _MEZZ_THREAD_WIN32_
            #ifdef _MSC_VER
                size_t Size = 0;
                DWORD buffer_size = 0;
                DWORD i = 0;
                SYSTEM_LOGICAL_PROCESSOR_INFORMATION * buffer = 0;

                GetLogicalProcessorInformation(0, &buffer_size);
                buffer = (SYSTEM_LOGICAL_PROCESSOR_INFORMATION *)malloc(buffer_size);
                GetLogicalProcessorInformation(&buffer[0], &buffer_size);

                for (i = 0; i != buffer_size / sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION); ++i)
                {
                    if (buffer[i].Relationship == RelationCache && buffer[i].Cache.Level == 1)
                    {
                        Size = buffer[i].Cache.LineSize;
                        break;
                    }
                }

                free(buffer);
                return Size;
            #else
                //mingw can't do this, see bug http://sourceforge.net/p/mingw/bugs/1626/
                // assume 64
                return 64;
            #endif
        #else
            #ifdef _MEZZ_THREAD_APPLE_
                size_t line_size = 0;
                size_t sizeof_line_size = sizeof(line_size);
                sysctlbyname("hw.cachelinesize", &line_size, &sizeof_line_size, 0, 0);
                return line_size;
            #else
                Whole Size = sysconf(_SC_LEVEL1_DCACHE_LINESIZE);
                if(!Size)
                {
                    Size = sysconf(_SC_LEVEL2_CACHE_LINESIZE);
                    if(!Size)
                    {
                        Size = sysconf(_SC_LEVEL3_CACHE_LINESIZE);
                        if(!Size)
                            { Size = sysconf(_SC_LEVEL4_CACHE_LINESIZE); }
                    }
                }
                return Size;
            #endif
        #endif
    }

}//Mezzanine

#endif
