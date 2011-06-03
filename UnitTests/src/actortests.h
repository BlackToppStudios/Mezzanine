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
#ifndef _actortests_h
#define _actortests_h

#include "main.h"
#include <sstream>

// A simple test to see if the compiler flag are set correctly
// mostly this is a sample of a very simple, but useful unit test (it fixed 2 bugs when we wrote it.)
class ActorTests : public UnitTest
{
    public:
        virtual TestResult RunTests(bool RunAutomaticTests, bool RunInteractiveTests)
        {
            if (RunAutomaticTests)
            {
                StartEngine();
                AddTestResult("SceneManager::operator<< (ActorGraphicsSetting-empty)", Unknown);
                AddTestResult("SceneManager::operator<< (ActorGraphicsSetting-3colour)", Unknown);
                AddTestResult("SceneManager::operator>> (ActorGraphicsSetting-empty)", Unknown);
                AddTestResult("SceneManager::operator>> (ActorGraphicsSetting-3colour)", Unknown);
                String groupname ("Group1");
                String filerobot ("robot.mesh");
                TheWorld->GetResourceManager()->AddResourceLocation(crossplatform::GetDataDirectory(), "FileSystem", groupname, false);

                ActorRigid *object1 = new ActorRigid (20.0,"ABasicRobot",filerobot,groupname);
                object1->CreateShapeFromMeshDynamic(1);
                object1->SetLocation(Vector3(400,70,100));
                object1->SetOrientation(Quaternion(0.5, 0.5, 0.0, 0.9));
                object1->SetAnimation("Idle", true);
                object1->EnableAnimation(true);

                std::stringstream SerializeTest1;
                SerializeTest1 << *(object1->GetGraphicsSettings());

                object1->GetGraphicsSettings()->SetMaterialAmbient(phys::ColourValue(0.1,0.2,0.3,0.4));
                object1->GetGraphicsSettings()->SetMaterialSpecular(phys::ColourValue(0.2,0.3,0.4,0.5));
                object1->GetGraphicsSettings()->SetMaterialDiffuse(phys::ColourValue(0.3,0.4,0.5,0.6));
                std::stringstream SerializeTest2;
                SerializeTest2 << *(object1->GetGraphicsSettings());

                String Test1("<ActorGraphicsSettings Version=\"1\"></ActorGraphicsSettings>");
                if ( Test1 == SerializeTest1.str())
                {
                    AddTestResult("SceneManager::operator<< (ActorGraphicsSetting-empty)", Success, UnitTest::OverWrite);
                }else{
                    AddTestResult("SceneManager::operator<< (ActorGraphicsSetting-empty)", Failed, UnitTest::OverWrite);
                }

                String Test2("<ActorGraphicsSettings Version=\"1\"><AmbientMaterial Submesh=\"0\"><ColourValue Version=\"1\" Red=\"0.1\" Green=\"0.2\" Blue=\"0.3\" Alpha=\"0.4\" /></AmbientMaterial><SpecularMaterial Submesh=\"0\"><ColourValue Version=\"1\" Red=\"0.2\" Green=\"0.3\" Blue=\"0.4\" Alpha=\"0.5\" /></SpecularMaterial><DiffuseMaterial Submesh=\"0\"><ColourValue Version=\"1\" Red=\"0.3\" Green=\"0.4\" Blue=\"0.5\" Alpha=\"0.6\" /></DiffuseMaterial></ActorGraphicsSettings>");
                if (Test2 == SerializeTest2.str())
                {
                    AddTestResult("SceneManager::operator<< (ActorGraphicsSetting-3colour)", Success, UnitTest::OverWrite);
                }else{
                    AddTestResult("SceneManager::operator<< (ActorGraphicsSetting-3colour)", Failed, UnitTest::OverWrite);
                }

                SerializeTest1 >> *(object1->GetGraphicsSettings());
                std::stringstream DeSerializeTest1;
                DeSerializeTest1 << *(object1->GetGraphicsSettings());

                SerializeTest2 >> *(object1->GetGraphicsSettings());
                std::stringstream DeSerializeTest2;
                DeSerializeTest2 << *(object1->GetGraphicsSettings());

                if ( Test1 == DeSerializeTest1.str() )
                {
                    AddTestResult("SceneManager::operator>> (ActorGraphicsSetting-empty)", Success, UnitTest::OverWrite);
                }else{
                    AddTestResult("SceneManager::operator>> (ActorGraphicsSetting-empty)", Failed, UnitTest::OverWrite);
                }

                if ( Test2 == DeSerializeTest2.str() )
                {
                    AddTestResult("SceneManager::operator>> (ActorGraphicsSetting-3colour)", Success, UnitTest::OverWrite);
                }else{
                    AddTestResult("SceneManager::operator>> (ActorGraphicsSetting-3colour)", Failed, UnitTest::OverWrite);
                }
                StopEngine();


            }else{
                AddTestResult("SceneManager::operator<< (ActorGraphicsSetting-empty)", Skipped);
                AddTestResult("SceneManager::operator<< (ActorGraphicsSetting-3colour)", Skipped);
                AddTestResult("SceneManager::operator>> (ActorGraphicsSetting-empty)", Skipped);
                AddTestResult("SceneManager::operator>> (ActorGraphicsSetting-3colour)", Skipped);

            }


        }
};
#endif

