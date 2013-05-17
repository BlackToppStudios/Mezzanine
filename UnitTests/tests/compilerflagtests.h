//©opyright 2010 - 2013 BlackTopp Studios Inc.
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
#ifndef _compilerflagtests_h
#define _compilerflagtests_h

#include "main.h"


// A simple test to see if the compiler flag are set correctly
// mostly this is a sample of a very simple, but potentially useful unit test (it helped fix 2 bugs when we wrote it.)
class compilerflagtests : public UnitTestGroup
{
    public:
        virtual String Name()
            { return String("compilerflags"); }

        virtual void RunTests(bool , bool RunInteractiveTests)
        {
            if (RunInteractiveTests)
            {
                TestResult temp;
#ifdef MEZZDEBUG
                temp = GetTestAnswerFromStdin( "Was MEZZDEBUG set to True when this was configured with CMake(where it is called Mezz_BuildTypeDebug) or other config tool? ");
#else
                temp = GetTestAnswerFromStdin( "Was MEZZDEBUG set to False when this was configured with CMake(where it is called Mezz_BuildTypeDebug) or other config tool? ");
#endif
                AddTestResult("CompilerFlag::MEZZDEBUG", temp);


#ifdef MEZZRELEASE
                temp = GetTestAnswerFromStdin( "Was MEZZRELEASE set to True when this was configured with CMake(where it is called Mezz_BuildTypeRelease) or other config tool? ");
#else
                temp = GetTestAnswerFromStdin( "Was MEZZRELEASE set to False when this was configured with CMake(where it is called Mezz_BuildTypeRelease) or other config tool? ");
#endif
                AddTestResult("CompilerFlag::MEZZRELEASE", temp);

#ifdef MEZZSMALL
                temp = GetTestAnswerFromStdin( "Was MEZZSMALL set to True when this was configured with CMake(where it is called Mezz_BuildTypeRelease) or other config tool? ");
#else
                temp = GetTestAnswerFromStdin( "Was MEZZSMALL set to False when this was configured with CMake(where it is called Mezz_BuildTypeRelease) or other config tool? ");
#endif
                AddTestResult("CompilerFlag::MEZZSMALL", temp);

#ifdef MEZZPROFILE
                temp = GetTestAnswerFromStdin( "Was MEZZPROFILE set to True when this was configured with CMake(where it is called Mezz_Profile) or other config tool? ");
#else
                temp = GetTestAnswerFromStdin( "Was MEZZPROFILE set to False when this was configured with CMake(where it is called Mezz_Debug) or other config tool? ");
#endif
                AddTestResult("CompilerFlag::MEZZPROFILE", temp);

#ifdef MEZZNETWORK
                temp = GetTestAnswerFromStdin( "Was MEZZNETWORK set to True when this was configured with CMake(where it is called Mezz_Network) or other config tool? ");
#else
                temp = GetTestAnswerFromStdin( "Was MEZZNETWORK set to False when this was configured with CMake(where it is called Mezz_Network) or other config tool? ");
#endif
                AddTestResult("CompilerFlag::MEZZNETWORK", temp);

#ifdef MEZZLUA51
                temp = GetTestAnswerFromStdin( "Was MEZZLUA51 set to True when this was configured with CMake(where it is called Mezz_Lua51) or other config tool? ");
#else
                temp = GetTestAnswerFromStdin( "Was MEZZLUA51 set to False when this was configured with CMake(where it is called Mezz_Lua51) or other config tool? ");
#endif
                AddTestResult("CompilerFlag::MEZZLUA51", temp);

#ifdef THROW_ON_FETCH_FAIL
                temp = GetTestAnswerFromStdin( "Was THROW_ON_FETCH_FAIL set to True when this was configured with CMake(where it is called Mezz_ThrowOnSingletonFetch) or other config tool? ");
#else
                temp = GetTestAnswerFromStdin( "Was THROW_ON_FETCH_FAIL set to False when this was configured with CMake(where it is called Mezz_ThrowOnSingletonFetch) or other config tool? ");
#endif
                AddTestResult("CompilerFlag::THROW_ON_FETCH_FAIL", temp);

            /// @todo Add compiler unit tests for the cpu type flag and the 5 maybe more rendersystems

            }else{
                AddTestResult("CompilerFlag::MEZZDEBUG", Skipped);
                AddTestResult("CompilerFlag::MEZZRELEASE", Skipped);
                AddTestResult("CompilerFlag::MEZZSMALL", Skipped);
                AddTestResult("CompilerFlag::MEZZPROFILE", Skipped);
                AddTestResult("CompilerFlag::MEZZNETWORK", Skipped);
                AddTestResult("CompilerFlag::THROW_ON_FETCH_FAIL", Skipped);
            }

        }
};
#endif
