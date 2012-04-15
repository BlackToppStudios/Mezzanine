//© Copyright 2010 - 2012 BlackTopp Studios Inc.
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
#ifndef _metacodetests_h
#define _metacodetests_h

#include "main.h"
#include <sstream>

class MetaCodeTests : public UnitTestGroup
{
    public:
        virtual void RunTests(bool RunAutomaticTests, bool RunInteractiveTests)
        {
            if (RunAutomaticTests)
            {
                AddTestResult("metacode::operator<< (Empty)", Unknown);         //say what tests will be performed, in case of a crash, this might help tell us were it left off.
                AddTestResult("metacode::operator<< (Filled1)", Unknown);
                AddTestResult("metacode::operator<< (Filled2)", Unknown);

                AddTestResult("metacode::operator>> (Empty)", Unknown);
                AddTestResult("metacode::operator>> (Filled1)", Unknown);
                AddTestResult("metacode::operator>> (Filled2)", Unknown);


                std::stringstream EmptyTest, Filled1Test, Filled2Test;          // create Stringstreams for serialization test
                std::stringstream EmptyTestRS, Filled1TestRS, Filled2TestRS;    // create Stringstreams for deserialization test

                MetaCode Mempty;                                                // create three metacodes for testing
                MetaCode MFilled1(MetaCode::BUTTON_LIFTING,MetaCode::KEY_W);    // The W key being released
                MetaCode MFilled2(-249,(MetaCode::InputCode)491);               // joystick axis 11 being moved down 249 units from center
                MetaCode MemptyRS, MFilled1RS, MFilled2RS;                      // Three empty metacodes for Deserialization;

                EmptyTest << Mempty;                                            //Perform the actual Serialization tests
                Filled1Test << MFilled1;
                Filled2Test << MFilled2;

                String EmptyTestResults("<MetaCode Version=\"1\" MetaValue=\"0\" Code=\"0\" />");               // this is what the serialized metacodes should look like
                String Filled1TestResults("<MetaCode Version=\"1\" MetaValue=\"-1\" Code=\"26\" />");
                String Filled2TestResults("<MetaCode Version=\"1\" MetaValue=\"-249\" Code=\"491\" />");

                //cout << "\n\n Actual: " << Mempty << "\n" << "Desired: " << EmptyTestResults << "\n";
                //cout << "\n\n Actual: " << MFilled1 << "\n" << "Desired: " << Filled1TestResults << "\n";
                //cout << "\n\n Actual: " << MFilled2 << "\n" << "Desired: " << Filled2TestResults << "\n";

                if (EmptyTestResults == EmptyTest.str()) // test the results of the serialization
                {
                    AddTestResult("metacode::operator<< (Empty)", Success, UnitTestGroup::OverWrite);
                }else{
                    AddTestResult("metacode::operator<< (Empty)", Failed, UnitTestGroup::OverWrite);
                }

                if (Filled1TestResults == Filled1Test.str()) // test the results of the serialization with a filled metacode
                {
                    AddTestResult("metacode::operator<< (Filled1)", Success, UnitTestGroup::OverWrite);
                }else{
                    AddTestResult("metacode::operator<< (Filled1)", Failed, UnitTestGroup::OverWrite);
                }

                if (Filled2TestResults == Filled2Test.str()) // test the results of the serialization again
                {
                    AddTestResult("metacode::operator<< (Filled2)", Success, UnitTestGroup::OverWrite);
                }else{
                    AddTestResult("metacode::operator<< (Filled2)", Failed, UnitTestGroup::OverWrite);
                }

                EmptyTest >> MemptyRS;                                            // The  DeSerialization tests
                Filled1Test >> MFilled1RS;
                Filled2Test >> MFilled2RS;

                //cout << "\n\n Actual: " << EmptyTestRS.str() << "\n" << "Desired: " << MemptyRS << "\n";
                //cout << "\n\n Actual: " << Filled1TestRS.str() << "\n" << "Desired: " << Filled1TestResults << "\n";
                //cout << "\n\n Actual: " << Filled2TestRS.str() << "\n" << "Desired: " << Filled2TestResults << "\n";

                EmptyTestRS << MemptyRS;                                            // ReSerialization so we can see if it deserialized correctly
                Filled1TestRS << MFilled1RS;
                Filled2TestRS << MFilled2RS;

                //cout << "\n\n Actual: " << EmptyTestRS.str() << "\n" << "Desired: " << MemptyRS << "\n";
                //cout << "\n\n Actual: " << Filled1TestRS.str() << "\n" << "Desired: " << /*MFilled1RS*/Filled1TestResults << "\n";
                //cout << "\n\n Actual: " << Filled2TestRS.str() << "\n" << "Desire : " << /*MFilled2RS*/Filled2TestResults  << "\n";

                if (EmptyTestResults == EmptyTestRS.str())
                {
                    AddTestResult("metacode::operator>> (Empty)", Success, UnitTestGroup::OverWrite);
                }else{
                    AddTestResult("metacode::operator>> (Empty)", Failed, UnitTestGroup::OverWrite);
                }

                if (Filled1TestResults == Filled1TestRS.str())
                {
                    AddTestResult("metacode::operator>> (Filled1)", Success, UnitTestGroup::OverWrite);
                }else{
                    AddTestResult("metacode::operator>> (Filled1)", Failed, UnitTestGroup::OverWrite);
                }

                if (Filled2TestResults == Filled2TestRS.str())
                {
                    AddTestResult("metacode::operator>> (Filled2)", Success, UnitTestGroup::OverWrite);
                }else{
                    AddTestResult("metacode::operator>> (Filled2)", Failed, UnitTestGroup::OverWrite);
                }

            }else{
                AddTestResult("metacode::operator<< (Empty)", Skipped);         // Say what tests were skipped
                AddTestResult("metacode::operator<< (Filled1)", Skipped);
                AddTestResult("metacode::operator<< (Filled2)", Skipped);

                AddTestResult("metacode::operator>> (Empty)", Skipped);
                AddTestResult("metacode::operator>> (Filled1)", Skipped);
                AddTestResult("metacode::operator>> (Filled2)", Skipped);
            }




        }
};
#endif
