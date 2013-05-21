//© Copyright 2010 - 2013 BlackTopp Studios Inc.
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
#ifndef _consolelogic_h
#define _consolelogic_h

/// @file
/// @brief The defintion of items important a commandline tool to work correctly without need to be available to test developers.

#include "mezzanine.h"

#include "testdata.h"

namespace Mezzanine
{
    namespace Testing
    {

        /// @brief Print a message for the user onf the standard output that briefly describes hwo to use this.
        /// @param ThisName The name of the current executable
        /// @param TestGroups This is searched for the current list of valid test names.
        /// @return Always returns ExitInvalidArguments
        int Usage(Mezzanine::String ThisName, CoreTestGroup& TestGroups);

        /// @brief This is the name of the file used to communicate results from child processes
        /// @warning This variable is used to create temporary files in a percieved insecure way
        /// Everything will be fine as long as nothing else writes to this this file during or
        /// between Tests. If something does, then you probably have big enough problems you
        /// shouldn't be developing software until that is fixed.
        static const String TempFile("UnitTestWork.txt");

        /// @brief Empty the file specified by TempFile
        /// @warning This doesn't ask for permission and can't easily be cancelled or recovered
        /// from. This will open, then erase the contents of the file.
        /// @throw This can throw any exception that the C++ filestream classes can throw.
        void ClearTempFile();

        /// @internal
        /// @brief This will open then parse the contents of the file specified by TempFile and interpret any test results located
        /// @throw This can throw any exception that the C++ filestream classes can throw.
        /// @return This "reads" the temp file and interprets it. It tries to extract the name of the test as the whole of a line minus
        /// the last word. The last word is then converted into a @ref TestResult using @ref StringToTestResult . Any Whitespace between
        /// between the end of the last word and the end of the test name is dropped. All lines are interpretted this way and returned
        /// as a single @ref UnitTestGroup.
        UnitTestGroup GetResultsFromTempFile();

        /// @brief Attempts to delete TempFile. Silently fails if not possible.
        void DeleteTempFile();

    }// Testing
}// Mezzanine

#endif
