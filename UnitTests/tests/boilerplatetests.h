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
#ifndef _boilerplatetests_h
#define _boilerplatetests_h

#include "main.h"

/// @file
/// @brief this file should be used as template for building future Unit Tests

using namespace Mezzanine;
using namespace Mezzanine::Testing;

/// @brief A small series of sample tests, which can be used as a boilerplate so creating new test groups
class boilerplatetests : public UnitTestGroup
{
    public:
        /// @copydoc Mezzanine::Testing::UnitTestGroup::Name
        /// @return Returns a String containing "boilerplate"
        virtual String Name()
            { return String("boilerplate"); }

        /// @copydoc Mezzanine::Testing::UnitTestGroup::RunTests
        /// @detail provides on Sample interactive and one sample automatic test.
        virtual void RunTests(bool RunAutomaticTests, bool RunInteractiveTests)
        {
            if (RunAutomaticTests)
            {
                TestResult temp;

                if(1)
                    { temp=Success; }
                else
                    { temp=Failed; }
                AddTestResult("BoilerPlate::Automatic", temp);

            }else{
                AddTestResult("BoilerPlate::Automatic", Skipped);
            }

            if (RunInteractiveTests)
            {
                TestResult temp;
                temp = GetTestAnswerFromStdin( "Is this a question? ");
                AddTestResult("BoilerPlate::Interactive", temp);
            }else{
                AddTestResult("BoilerPlate::Interactive", Skipped);
            }
        }
};

#endif

