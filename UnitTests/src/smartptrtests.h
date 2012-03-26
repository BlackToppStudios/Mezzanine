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
#ifndef _smartptrtests_h
#define _smartptrtests_h

#include "main.h"

#include <iostream>

class SmartPtrTests : public UnitTestGroup
{
    public:
        virtual void RunTests(bool RunAutomaticTests, bool RunInteractiveTests)
        {
            if (RunAutomaticTests)
            {
                TestResult temp = Success;

                std::cout << std::endl << "Creating A Vector3 in a counted pointed." << std::endl;
                Mezzanine::CountedPtr<Mezzanine::Vector3> Stabby (new Mezzanine::Vector3);
                std::cout << "Count of references: " << Stabby.use_count() << std::endl;
                if (1 == Stabby.use_count())
                {
                    AddTestResult("SmartPtr::Create", Success, UnitTestGroup::OverWrite);
                }else{
                    AddTestResult("SmartPtr::Create", Failed, UnitTestGroup::OverWrite);
                }


                std::cout << "Creating creating a scoped copy of counted pointer." << std::endl;
                bool notfailed=true;
                {
                    Mezzanine::CountedPtr<Mezzanine::Vector3> StabbyScoped (Stabby);
                    std::cout << "In scope count of references: " << Stabby.use_count() << std::endl;
                    if (2 != Stabby.use_count())
                        { notfailed=false; }
                }
                std::cout << "Post scope count of references: " << Stabby.use_count() << std::endl;
                if (1 == Stabby.use_count() && notfailed)
                {
                    AddTestResult("SmartPtr::ScopedChanged", Success, UnitTestGroup::OverWrite);
                }else{
                    AddTestResult("SmartPtr::ScopedChanged", Failed, UnitTestGroup::OverWrite);
                }


            }else{
                AddTestResult("SmartPtr::Create", Skipped);
                AddTestResult("SmartPtr::ScopedChanged", Skipped);


            }

        }
};
#endif
