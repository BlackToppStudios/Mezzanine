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
        int Usage(Mezzanine::String ThisName, const CoreTestGroup& TestGroups)
        {
            std::cout   << std::endl << "Usage: " << ThisName << " [help] [summary] [testlist] [interactive|automatic] [all] Test Group Names ..." << std::endl << std::endl
                        << "All:         All test groups will be run." << std::endl
                        << "Interactive: Only interactive tests will be performed on specified test groups." << std::endl
                        << "Automatic:   Only automated tests will be performed on specified test groups." << std::endl
                        //<< "Interactive and Automatic: All tests will be run on specificied test groups." << std::endl << std::endl
                        << "Summary:     Only display a count of failures and successes" << std::endl
                        << "testlist:    Output a list of all tests, one per line" << std::endl
                        << "Help:        Display this message"<< std::endl << std::endl
                        << "If only test group names are entered, then all tests in those groups are run." << std::endl
                        << "This command is not case sensitive." << std::endl << std::endl
                        << "Current Test Groups: " << std::endl;
            Mezzanine::Whole c = 0;
            for(std::map<Mezzanine::String,UnitTestGroup*>::iterator Iter=TestGroups.begin(); Iter!=TestGroups.end(); ++Iter)
            {
                std::cout << "\t" << Iter->first << (Iter->first.size()<7?"\t":"") << " ";
                ++c;        //enforce 4 names per line
                if (4==c)
                    { std::cout << std::endl; c=0; }
            }
            std::cout << std::endl;

            return ExitInvalidArguments;
        }

    }// Testing
}// Mezzanine

#endif
