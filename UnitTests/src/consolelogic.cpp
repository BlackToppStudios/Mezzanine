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
#ifndef _consolelogic_cpp
#define _consolelogic_cpp

/// @file
/// @brief The implementation of items important a commandline tool to work correctly without need to be available to test developers.

#include "consolelogic.h"
#include "consolestringmanipulation.h"

#include <fstream>
#include <vector>

using namespace Mezzanine;

namespace Mezzanine
{
    namespace Testing
    {

        int Usage(Mezzanine::String ThisName, CoreTestGroup& TestGroups)
        {
            std::cout   << std::endl << "Usage: " << ThisName << " [help] [summary] [testlist] [interactive|automatic] [all]\n\t[skipfile] Test Group Names ..." << std::endl << std::endl
                        << "All:         All test groups will be run." << std::endl
                        << "Interactive: Only interactive tests will be performed on specified test groups." << std::endl
                        << "Automatic:   Only automated tests will be performed on specified test groups." << std::endl
                        //<< "Interactive and Automatic: All tests will be run on specificied test groups." << std::endl << std::endl
                        << "Summary:     Only display a count of failures and successes" << std::endl
                        << "testlist:    Output a list of all tests, one per line" << std::endl
                        << "skipfile:    Do not store a copy of the results in TestResults.txt" << std::endl
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

        void ClearTempFile()
        {
            std::ofstream FileToClear;
            FileToClear.open(TempFile.c_str(),std::ios_base::out|std::ios_base::trunc); // Clear the work file.
            FileToClear.close();
        }

        UnitTestGroup GetResultsFromTempFile()
        {
            UnitTestGroup Results;
            std::vector<Mezzanine::String> FileContents;

            char SingleLine[1024];
            std::ifstream TheSourceFile(TempFile.c_str());
            while( TheSourceFile.good() )
            {
                TheSourceFile.getline(SingleLine,1024);
                FileContents.push_back(Mezzanine::String(SingleLine));
            }

            if(TheSourceFile.eof()) // We successfully reached the end of the file
                { }
            if(TheSourceFile.bad()) // We fail somehow
                { std::cerr << "Error reading temp file." << std::endl; }
            TheSourceFile.close();

            for(std::vector<Mezzanine::String>::iterator Iter=FileContents.begin(); FileContents.end()!=Iter; ++Iter) // for each line in the file, that is now loaded in RAM
            {
                if(rtrim(*Iter).size()) // If there is more than whitespace
                {
                    Results.AddTestResult(StringToTestData(*Iter));
                }
            }

            return Results;
        }

        void DeleteTempFile()
        {
            remove(TempFile.c_str());
        }

    }// Testing
}// Mezzanine

#endif
