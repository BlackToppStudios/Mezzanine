//© Copyright 2010 - 2011 BlackTopp Studios Inc.
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
#ifndef _notatest_h // 2. When adding new tests - Change the Include Guard
#define _notatest_h

#include "main.h"

// When adding a new group of Unit Tests there a X things you need to do the following:
// 1. Copy this File to a new name
// 2. Change the Include Guard
// 3. Change the Name of the Class
// 4. Add the new file to the includes in unittests/main.cpp
// 5. Add the line to create a new UnitTest in the TestGroup Container in the main function
// you can do a search for "When adding new tests" to find steps 2 through 5


// 3. When adding new tests - Change the Name of the Class
class notatestTests : public UnitTestGroup
{
    public:
        virtual void RunTests(bool RunAutomaticTests, bool RunInteractiveTests)
        {
            if (RunAutomaticTests)
            {
                TestResult temp = Success;
                AddTestResult("notatest::GotThisFar-ThisJustExistsAsABlankSlateForOtherTests", temp);


            }else{
                AddTestResult("notatest::GotThisFar-ThisJustExistsAsABlankSlateForOtherTests", Skipped);

            }

        }
};
#endif
