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
                AddTestResult("Actor::operator<< (ActorGraphicsSetting-empty)", Unknown);
                AddTestResult("Actor::operator<< (ActorGraphicsSetting-3colour)", Unknown);
                AddTestResult("Actor::operator>> (ActorGraphicsSetting-empty)", Unknown);
                AddTestResult("Actor::operator>> (ActorGraphicsSetting-3colour)", Unknown);
                AddTestResult("Actor::operator<< (ActorRigidPhysicsSettings-empty)", Unknown);
                AddTestResult("Actor::operator<< (ActorRigidPhysicsSettings-populated)", Unknown);
                AddTestResult("Actor::operator>> (ActorRigidPhysicsSettings-empty)", Unknown);
                AddTestResult("Actor::operator>> (ActorRigidPhysicsSettings-populated)", Unknown);

                String groupname ("Group1");
                String filerobot ("robot.mesh");
                TheWorld->GetResourceManager()->AddResourceLocation(crossplatform::GetDataDirectory(), "FileSystem", groupname, false);

                ActorRigid *object1 = new ActorRigid (20.0,"ABasicRobot",filerobot,groupname);
                object1->CreateShapeFromMeshDynamic(1);
                object1->SetLocation(Vector3(400,70,100));
                object1->SetOrientation(Quaternion(0.5, 0.5, 0.0, 0.9));
                object1->SetAnimation("Idle", true);
                object1->EnableAnimation(true);

                std::stringstream SerializeTestA1, SerializeTestP1, SerializeTestWhole1;
                SerializeTestA1 << *(object1->GetGraphicsSettings());
                SerializeTestP1 << *(object1->GetPhysicsSettings());
                SerializeTestWhole1 << *object1;

                object1->GetGraphicsSettings()->SetMaterialAmbient(phys::ColourValue(0.1,0.2,0.3,0.4));
                object1->GetGraphicsSettings()->SetMaterialSpecular(phys::ColourValue(0.2,0.3,0.4,0.5));
                object1->GetGraphicsSettings()->SetMaterialDiffuse(phys::ColourValue(0.3,0.4,0.5,0.6));
                object1->GetPhysicsSettings()->SetDamping(0.9,0.01);
                object1->GetPhysicsSettings()->SetIndividualGravity(phys::Vector3(0.9,9.8,0.0));
                object1->GetPhysicsSettings()->SetFriction(0.01);
                std::stringstream SerializeTest2, SerializeTestp2;
                SerializeTest2 << *(object1->GetGraphicsSettings());
                SerializeTestp2 << *(object1->GetPhysicsSettings());

                String Test1("<ActorGraphicsSettings Version=\"1\"></ActorGraphicsSettings>");
                if ( Test1 == SerializeTestA1.str())
                {
                    AddTestResult("Actor::operator<< (ActorGraphicsSetting-empty)", Success, UnitTest::OverWrite);
                }else{
                    AddTestResult("Actor::operator<< (ActorGraphicsSetting-empty)", Failed, UnitTest::OverWrite);
                }

                String Test2("<ActorGraphicsSettings Version=\"1\"><AmbientMaterial Submesh=\"0\"><ColourValue Version=\"1\" Red=\"0.1\" Green=\"0.2\" Blue=\"0.3\" Alpha=\"0.4\" /></AmbientMaterial><SpecularMaterial Submesh=\"0\"><ColourValue Version=\"1\" Red=\"0.2\" Green=\"0.3\" Blue=\"0.4\" Alpha=\"0.5\" /></SpecularMaterial><DiffuseMaterial Submesh=\"0\"><ColourValue Version=\"1\" Red=\"0.3\" Green=\"0.4\" Blue=\"0.5\" Alpha=\"0.6\" /></DiffuseMaterial></ActorGraphicsSettings>");
                if (Test2 == SerializeTest2.str())
                {
                    AddTestResult("Actor::operator<< (ActorGraphicsSetting-3colour)", Success, UnitTest::OverWrite);
                }else{
                    AddTestResult("Actor::operator<< (ActorGraphicsSetting-3colour)", Failed, UnitTest::OverWrite);
                }

                String Testp1("<ActorRigidPhysicsSettings Version=\"1\" AngularDamping=\"0\" LinearDamping=\"0\"><AngularVelocity><Vector3 Version=\"1\" X=\"0\" Y=\"0\" Z=\"0\" /></AngularVelocity><LinearVelocity><Vector3 Version=\"1\" X=\"0\" Y=\"0\" Z=\"0\" /></LinearVelocity><IndividualGravity><Vector3 Version=\"1\" X=\"0\" Y=\"0\" Z=\"0\" /></IndividualGravity><TotalTorque><Vector3 Version=\"1\" X=\"0\" Y=\"0\" Z=\"0\" /></TotalTorque><TotalForce><Vector3 Version=\"1\" X=\"0\" Y=\"0\" Z=\"0\" /></TotalForce><ActorBasePhysicsSettings Version=\"1\" Friction=\"0.5\" Restitution=\"0\" Kinematic=\"0\" Static=\"0\" CollisionResponse=\"1\" CollisionGroup=\"1\" CollisionMask=\"18446744073709551615\" ActivationState=\"1\" /></ActorRigidPhysicsSettings>");
                if ( Testp1 == SerializeTestP1.str())
                {
                    AddTestResult("Actor::operator<< (ActorRigidPhysicsSettings-empty)", Success, UnitTest::OverWrite);
                }else{
                    AddTestResult("Actor::operator<< (ActorRigidPhysicsSettings-empty)", Failed, UnitTest::OverWrite);
                }

                String Testp2("<ActorRigidPhysicsSettings Version=\"1\" AngularDamping=\"0.01\" LinearDamping=\"0.9\"><AngularVelocity><Vector3 Version=\"1\" X=\"0\" Y=\"0\" Z=\"0\" /></AngularVelocity><LinearVelocity><Vector3 Version=\"1\" X=\"0\" Y=\"0\" Z=\"0\" /></LinearVelocity><IndividualGravity><Vector3 Version=\"1\" X=\"0.9\" Y=\"9.8\" Z=\"0\" /></IndividualGravity><TotalTorque><Vector3 Version=\"1\" X=\"0\" Y=\"0\" Z=\"0\" /></TotalTorque><TotalForce><Vector3 Version=\"1\" X=\"0\" Y=\"0\" Z=\"0\" /></TotalForce><ActorBasePhysicsSettings Version=\"1\" Friction=\"0.01\" Restitution=\"0\" Kinematic=\"0\" Static=\"0\" CollisionResponse=\"1\" CollisionGroup=\"1\" CollisionMask=\"18446744073709551615\" ActivationState=\"1\" /></ActorRigidPhysicsSettings>");
                if (Testp2 == SerializeTestp2.str())
                {
                    AddTestResult("Actor::operator<< (ActorRigidPhysicsSettings-populated)", Success, UnitTest::OverWrite);
                }else{
                    AddTestResult("Actor::operator<< (ActorRigidPhysicsSettings-populated)", Failed, UnitTest::OverWrite);
                }

                SerializeTestA1 >> *(object1->GetGraphicsSettings());
                std::stringstream DeSerializeTestA1;
                DeSerializeTestA1 << *(object1->GetGraphicsSettings());

                SerializeTest2 >> *(object1->GetGraphicsSettings());
                std::stringstream DeSerializeTest2;
                DeSerializeTest2 << *(object1->GetGraphicsSettings());

                if ( Test1 == DeSerializeTestA1.str() )
                {
                    AddTestResult("Actor::operator>> (ActorGraphicsSetting-empty)", Success, UnitTest::OverWrite);
                }else{
                    AddTestResult("Actor::operator>> (ActorGraphicsSetting-empty)", Failed, UnitTest::OverWrite);
                }

                if ( Test2 == DeSerializeTest2.str() )
                {
                    AddTestResult("Actor::operator>> (ActorGraphicsSetting-3colour)", Success, UnitTest::OverWrite);
                }else{
                    AddTestResult("Actor::operator>> (ActorGraphicsSetting-3colour)", Failed, UnitTest::OverWrite);
                }

                SerializeTestP1 >> *(object1->GetPhysicsSettings());
                std::stringstream DeSerializeTestP1;
                DeSerializeTestP1 << *(object1->GetPhysicsSettings());

                SerializeTestp2 >> *(object1->GetPhysicsSettings());
                std::stringstream DeSerializeTestp2;
                DeSerializeTestp2 << *(object1->GetPhysicsSettings());

                if ( Testp1 == DeSerializeTestP1.str() )
                {
                    AddTestResult("Actor::operator>> (ActorRigidPhysicsSettings-empty)", Success, UnitTest::OverWrite);
                }else{
                    AddTestResult("Actor::operator>> (ActorRigidPhysicsSettings-empty)", Failed, UnitTest::OverWrite);
                }

                if ( Testp2 == DeSerializeTestp2.str() )
                {
                    AddTestResult("Actor::operator>> (ActorRigidPhysicsSettings-populated)", Success, UnitTest::OverWrite);
                }else{
                    AddTestResult("Actor::operator>> (ActorRigidPhysicsSettings-populated)", Failed, UnitTest::OverWrite);
                }












                StopEngine();

            }else{
                AddTestResult("Actor::operator<< (ActorGraphicsSetting-empty)", Skipped);
                AddTestResult("Actor::operator<< (ActorGraphicsSetting-3colour)", Skipped);
                AddTestResult("Actor::operator>> (ActorGraphicsSetting-empty)", Skipped);
                AddTestResult("Actor::operator>> (ActorGraphicsSetting-3colour)", Skipped);
                AddTestResult("Actor::operator<< (ActorRigidPhysicsSettings-empty)", Skipped);
                AddTestResult("Actor::operator<< (ActorRigidPhysicsSettings-populated)", Skipped);
                AddTestResult("Actor::operator>> (ActorRigidPhysicsSettings-empty)", Skipped);
                AddTestResult("Actor::operator>> (ActorRigidPhysicsSettings-populated)", Skipped);
            }


        }
};
#endif

