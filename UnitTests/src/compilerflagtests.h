//© Copyright 2010 - 2011 BlackTopp Studios Inc.
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
#ifndef _compilerflagtests_h
#define _compilerflagtests_h

#include "main.h"


// A simple test to see if the compiler flag are set correctly
// mostly this is a sample of a very simple, but useful unit test (it fixed 2 bugs when we wrote it.)
class CompilerFlagTests : public UnitTest
{
    public:
        virtual TestResult RunTests(bool RunAutomaticTests, bool RunInteractiveTests)
        {
            if (RunInteractiveTests)
            {
                TestResult temp;
                temp = GetTestAnswer( phys::StringCat("Was PHYSDEBUG set to \"", BoolToString(PHYSDEBUG), "\" when this was configured with cmake (or other config tool)? "));
                AddTestResult("CompilerFlag::PHYSDEBUG", temp);

                temp = GetTestAnswer( phys::StringCat("Was PHYSXML set to \"", BoolToString(PHYSXML), "\" when this was configured with cmake (or other config tool)? "));
                AddTestResult("CompilerFlag::PHYSXML", temp);

                temp = GetTestAnswer( phys::StringCat("Was PHYSPROFILE set to \"", BoolToString(PHYSPROFILE), "\" when this was configured with cmake (or other config tool)? "));
                AddTestResult("CompilerFlag::PHYSPROFILE", temp);

            }else{
                AddTestResult("CompilerFlag::PHYSDEBUG", Skipped);
                AddTestResult("CompilerFlag::PHYSXML", Skipped);
                AddTestResult("CompilerFlag::PHYSPROFILE", Skipped);
            }

        }
};
#endif
