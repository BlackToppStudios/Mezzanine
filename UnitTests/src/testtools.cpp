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
#ifndef _testtools_cpp
#define _testtools_cpp

/// @file
/// @brief The implementation of a few function to help implement tests

#include "mezzanine.h"

#include "testtools.h"

using namespace Mezzanine;

namespace Mezzanine
{
    namespace Testing
    {
        TestResult GetTestAnswerFromStdin(Mezzanine::String Question)
        {
            Mezzanine::String Input;
            char Answer;

            while(true)
            {
                std::cout << Question;
                getline(std::cin, Input);
                std::stringstream InputStream(Input);
                if (InputStream >> Answer)
                {
                    Answer=tolower(Answer);
                    if (Answer=='t' || Answer=='y' || Answer=='f' || Answer=='n' || Answer=='c' || Answer=='u' || Answer=='i')
                        { break; }
                }

                std::cout << std::endl << "Expected (T)rue/(Y)es for Success, (F)alse/(N)o for Failure," << std::endl << " (C)anceled to cancel this test, or (U)nsure/(I)nconclusive if you don't know." << std::endl << std::endl;
            }

            switch(Answer)
            {
                case 't': case 'y':
                    return Success;
                case 'f': case 'n':
                    return Failed;
                case 'c':
                    return Cancelled;
                case 'u': case 'i':
                    return Inconclusive;
                default:
                    return Unknown;
            }
        }

        bool QuittingCallback()
        {
            static Whole Answer=0;
            if( Answer++ == 120 )
            {
                return false;
                Answer=0;
            }
            return true;
        }

        CountedPtr<Entresol> SimpleEngineStartup(std::vector<ManagerBase*> CustomManagers)
        {
            Physics::ManagerConstructionInfo Info;
            Info.PhysicsFlags = Physics::ManagerConstructionInfo::PCF_SoftRigidWorld;
            Info.GeographyLowerBounds = Vector3(-30000.0,-30000.0,-30000.0);
            Info.GeographyUpperBounds = Vector3(30000.0,30000.0,30000.0);
            Info.MaxProxies = 60;

            CountedPtr<Entresol> Results
                    (
                        new Entresol(  Info,
                                       String("DefaultSceneManager"),
                                       String("Data/"),
                                       String("TestLog.txt"),
                                       CustomManagers
                                    )
                    );

            Results->SetTargetFrameRate(60);
            Graphics::GraphicsManager* GraphMan = Results->GetGraphicsManager();
            GraphMan->SetAutoGenFiles(false);
            Results->GetAudioManager()->SetAutoGenFiles(false);
            Graphics::GameWindow* FirstWindow = GraphMan->CreateGameWindow("Window1",800,600,0);
            Camera* FirstCam = CameraManager::GetSingletonPtr()->CreateCamera("Camera1");
            FirstWindow->CreateViewport(FirstCam);
            //Graphics::Viewport* FirstViewport = FirstWindow->CreateViewport(FirstCam);
            FirstCam->SetLocation( Vector3(0,50,900) );
            FirstCam->LookAt( Vector3(0,0,0) );

            Results->GetEventManager()->SetPostMainLoopItems(&QuittingCallback);

            return Results;
        }

    }// Testing
}// Mezzanine

#endif
