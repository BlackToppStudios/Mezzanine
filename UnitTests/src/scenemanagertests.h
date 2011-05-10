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
#ifndef _scenemanagertests_h
#define _scenemanagertests_h

#include "main.h"

class SceneManagerTests : public UnitTest
{
    public:
        virtual TestResult RunTests(bool RunAutomaticTests, bool RunInteractiveTests)
        {
            if (RunInteractiveTests)
            {
                SceneManager* SM = 0;

                AddTestResult("SceneManager::DisplaySkyPlane", Unknown);
                try
                {
                    StartEngine();
                    TheMessage="Does It Look Like a SkyPlane?";
                    SM = TheWorld->GetSceneManager();
                    SM->SetAmbientLight(1,1,1,1);
                    //SM->CreateSkyPlane(Plane(Vector3(0,1,0),5),"Examples/CloudPlane","files");
                    SM->CreateSkyPlane(Plane(Vector3(2.0,1.0,-5.0), Vector3(1.0,2.0,-5.0), Vector3(1.0,1.0,-5.0)),"Examples/CloudPlane","files");
                    GetAnswer();
                    TheWorld->MainLoop();
                    GetAnswer();
                    TheWorld->MainLoop();
                    GetAnswer();
                    TheWorld->MainLoop();
                    StopEngine();
                    AddTestResult("SceneManager::DisplaySkyPlane", AnswerToQuestion, UnitTest::OverWrite);
                } catch (std::exception e) {
                    cout << e.what();
                    AddTestResult("SceneManager::DisplaySkyPlane", Failed, UnitTest::OverWrite);
                }

                try
                {
                    AddTestResult("SceneManager::DisplaySkyBox", Unknown);
                    StartEngine();
                    TheMessage="Does It Look Like a SkyBox?";
                    SM = TheWorld->GetSceneManager();
                    SM->SetAmbientLight(1,1,1,1);
                    SM->CreateSkyBox("Examples/CloudBox","files",50);
                    GetAnswer();
                    TheWorld->MainLoop();
                    StopEngine();
                    AddTestResult("SceneManager::DisplaySkyBox", AnswerToQuestion, UnitTest::OverWrite);
                } catch (std::exception e) {
                    cout << e.what();
                    AddTestResult("SceneManager::DisplaySkyBox", Failed, UnitTest::OverWrite);
                }

                try
                {
                    AddTestResult("SceneManager::DisplaySkyDome", Unknown);
                    StartEngine();
                    TheMessage="Does It Look Like a SkyDome?";
                    SM = TheWorld->GetSceneManager();
                    SM->SetAmbientLight(1,1,1,1);
                    SM->CreateSkyDome("Examples/CloudPlane","files",50);
                    GetAnswer();
                    TheWorld->MainLoop();
                    AddTestResult("SceneManager::DisplaySkyDome", AnswerToQuestion, UnitTest::OverWrite);
                    StopEngine();
                } catch (std::exception e) {
                    cout << e.what();
                    AddTestResult("SceneManager::DisplaySkyDome", Failed, UnitTest::OverWrite);
                }


            }else{
                AddTestResult("SceneManager::DisplaySkyPlane", Skipped);
                AddTestResult("SceneManager::DisplaySkyBox", Skipped);
                AddTestResult("SceneManager::DisplaySkyDome", Skipped);
            }

            if (RunAutomaticTests)
            {

                StartEngine();
                SceneManager* SM = TheWorld->GetSceneManager();
                SM->SetAmbientLight(1,1,1,1);
                SM->CreateSkyDome("Examples/CloudPlane","files",50);
                AddTestResult("SceneManager::operator<<", Failed);

                stringstream SerializeTest("");
                SerializeTest << *SM;
                TheWorld->LogStream << SerializeTest.str();
                if (""!=SerializeTest.str() )
                {
                    AddTestResult("SceneManager::operator<<", Success, UnitTest::OverWrite);
                }
                TheWorld->CommitLog();


                StopEngine();

            }else{
                AddTestResult("SceneManager::operator<<", Skipped);
            }

        }
};
#endif
