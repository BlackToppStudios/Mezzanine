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
#ifndef _eventmanagertests_h
#define _eventmanagertests_h

#include "main.h"

using namespace phys;


// This block of code with have everything required for performing the actual tests.
namespace
{
    bool PostInputTestNull()
    {
        return true;
    }

}

class EventManagerTests : public UnitTest
{
    public:
        virtual TestResult RunTests(bool RunAutomaticTests, bool RunInteractiveTests)
        {
            if (RunAutomaticTests)
            {

            }else{

            }

            if (RunInteractiveTests)
            {

                //Key ordering Test
                StartEngine("Please Press a Button.");
                StartCountdown(15);
                TheWorld->GetEventManager()->SetPostMainLoopItems(&PostInputTestNull);
                TheWorld->MainLoop();

                TestResult Order1 = Success;
                TestResult Order2 = Success;
                TestResult Order3 = Success;
                bool Tested1 = false;
                bool Tested2 = false;
                bool Tested3 = false;

                vector<int> Keymap;
                Keymap.insert(Keymap.end(), MetaCode::KEY_LAST, 0); // make a big empty arraylike thing

                EventUserInput *ThisInput = TheWorld->GetEventManager()->PopNextUserInputEvent();
                while (ThisInput) //for each userinput event
                {
                    for (Whole c=0; c<ThisInput->GetMetaCodeCount(); ++c ) //for each metacode in each userinput
                    {
                        MetaCode::InputCode ThisCode = ThisInput->GetMetaCode(c).GetCode();
                        if(MetaCode::KEY_LAST > ThisCode)
                        {
                            TheWorld->Log(ThisInput->GetMetaCode(c));
                            MetaCode::ButtonState ThisValue = ThisInput->GetMetaCode(c).GetMetaValueAsButtonState();
                            if(MetaCode::BUTTON_UP == Keymap.at(ThisCode))        //Up is stored so this should only be a pressing
                            {
                                Tested1=true;
                                if(MetaCode::BUTTON_PRESSING==ThisValue)
                                    { Keymap.at(ThisCode) = ThisValue; }
                                else
                                    { Order1=Failed; }
                            }else if(MetaCode::BUTTON_PRESSING == Keymap.at(ThisCode))        //Up is stored so this should only be a pressing
                            {
                                Tested2=true;
                                if(MetaCode::BUTTON_DOWN==ThisValue)
                                    { Keymap.at(ThisCode) = ThisValue; }
                                else
                                    { Order2=Failed; }
                            }else if(MetaCode::BUTTON_DOWN == Keymap.at(ThisCode))        //Up is stored so this should only be a pressing
                            {
                                Tested3=true;
                                if(MetaCode::BUTTON_LIFTING==ThisValue || MetaCode::BUTTON_DOWN==ThisValue)
                                    { Keymap.at(ThisCode) = ThisValue; }
                                else
                                    { Order3=Failed; }
                            }
                        }
                    }

                    ThisInput = TheWorld->GetEventManager()->PopNextUserInputEvent();
                }

                if (!Tested1)
                    Order1=Skipped;
                if (!Tested2)
                    Order2=Skipped;
                if (!Tested3)
                    Order3=Skipped;
                //temp = GetTestAnswer( "Does the EventManager Work?" );
                AddTestResult("EventManager::KeyEventOrdering::UpToPressing", Order1);
                AddTestResult("EventManager::KeyEventOrdering::PressingToDown", Order2);
                AddTestResult("EventManager::KeyEventOrdering::DownToLifting", Order3);
                StopEngine();


            }else{
                AddTestResult("EventManager::KeyEventOrdering::UpToPressing", Skipped);
                AddTestResult("EventManager::KeyEventOrdering::PressingToDown", Skipped);
                AddTestResult("EventManager::KeyEventOrdering::DownToLifting", Skipped);
            }
        }
};





#endif
