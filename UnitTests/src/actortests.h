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
#ifndef _actortests_h
#define _actortests_h

#include "main.h"
#include <sstream>
#include <limits>

// A simple test to see if the compiler flag are set correctly
// mostly this is a sample of a very simple, but useful unit test (it fixed 2 bugs when we wrote it.)
class ActorTests : public UnitTestGroup
{
    public:
        virtual void RunTests(bool RunAutomaticTests, bool RunInteractiveTests)
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
                AddTestResult("Actor::operator<< (ActorRigid-empty)", Unknown);
                AddTestResult("Actor::operator<< (ActorRigid-populated)", Unknown);
                AddTestResult("Actor::operator>> (ActorRigid-empty)", Unknown);
                AddTestResult("Actor::operator>> (ActorRigid-populated)", Unknown);
                AddTestResult("ActorRigidDeSerializer::DeSerialize()", Unknown);
                AddTestResult("ActorRigidDeSerializer::DeSerializeAll()", Unknown);

                try{
                    String groupname ("Group1");
                    String filerobot ("robot.mesh");
                    ResourceManager::GetSingletonPtr()->AddResourceLocation("data/common", "FileSystem", groupname, false);

                    ActorRigid *object1 = new ActorRigid (20.0,"ABasicRobot",filerobot,groupname);
                    std::stringstream SerializeTestA1, SerializeTestP1, SerializeTestWhole1;

                    object1->GetPhysicsSettings()->SetCollisionShape( CollisionShapeManager::GetSingletonPtr()->GenerateConvexHull("ABasicRobotShape",filerobot,groupname) );
                    object1->SetLocation(Vector3(400,70,100));
                    object1->SetOrientation(Quaternion(0.5, 0.5, 0.0, 0.9));
                    object1->SetAnimation("Idle", true);
                    object1->EnableAnimation(true);

                    SerializeTestA1 << *(object1->GetGraphicsSettings());                       // Empty Tests
                    SerializeTestP1 << *(object1->GetPhysicsSettings());
                    SerializeTestWhole1 << *object1;

                    object1->GetGraphicsSettings()->SetMaterialAmbient(Mezzanine::ColourValue(0.1,0.2,0.3,0.4));
                    object1->GetGraphicsSettings()->SetMaterialSpecular(Mezzanine::ColourValue(0.2,0.3,0.4,0.5));
                    object1->GetGraphicsSettings()->SetMaterialDiffuse(Mezzanine::ColourValue(0.3,0.4,0.5,0.6));
                    object1->GetPhysicsSettings()->SetDamping(0.9,0.01);
                    object1->GetPhysicsSettings()->SetIndividualGravity(Mezzanine::Vector3(0.9,9.8,0.0));
                    object1->GetPhysicsSettings()->SetFriction(0.01);
                    object1->GetPhysicsSettings()->SetCCDParams(0.02,0.03);

                    std::stringstream SerializeTestA2, SerializeTestP2, SerializeTestWhole2;
                    SerializeTestA2 << *(object1->GetGraphicsSettings());
                    SerializeTestP2 << *(object1->GetPhysicsSettings());
                    //cout  << endl << endl << *(object1->GetGraphicsSettings()) << endl << endl;
                    SerializeTestWhole2 << *object1;

                    String Test1("<ActorGraphicsSettings Version=\"1\" />");
                    if ( Test1 == SerializeTestA1.str())
                    {
                        AddTestResult("Actor::operator<< (ActorGraphicsSetting-empty)", Success, UnitTestGroup::OverWrite);
                    }else{
                        AddTestResult("Actor::operator<< (ActorGraphicsSetting-empty)", Failed, UnitTestGroup::OverWrite);
                    }

                    String Test2("<ActorGraphicsSettings Version=\"1\"><AmbientMaterial Submesh=\"0\"><ColourValue Version=\"1\" Red=\"0.1\" Green=\"0.2\" Blue=\"0.3\" Alpha=\"0.4\" /></AmbientMaterial><SpecularMaterial Submesh=\"0\"><ColourValue Version=\"1\" Red=\"0.2\" Green=\"0.3\" Blue=\"0.4\" Alpha=\"0.5\" /></SpecularMaterial><DiffuseMaterial Submesh=\"0\"><ColourValue Version=\"1\" Red=\"0.3\" Green=\"0.4\" Blue=\"0.5\" Alpha=\"0.6\" /></DiffuseMaterial></ActorGraphicsSettings>");
                    if (Test2 == SerializeTestA2.str())
                    {
                        AddTestResult("Actor::operator<< (ActorGraphicsSetting-3colour)", Success, UnitTestGroup::OverWrite);
                    }else{
                        AddTestResult("Actor::operator<< (ActorGraphicsSetting-3colour)", Failed, UnitTestGroup::OverWrite);
                    }

                    String Testp1("<ActorRigidPhysicsSettings Version=\"1\" Mass=\"20\" AngularDamping=\"0\" LinearDamping=\"0\"><AngularVelocity><Vector3 Version=\"1\" X=\"0\" Y=\"0\" Z=\"0\" /></AngularVelocity><LinearVelocity><Vector3 Version=\"1\" X=\"0\" Y=\"0\" Z=\"0\" /></LinearVelocity><IndividualGravity><Vector3 Version=\"1\" X=\"0\" Y=\"0\" Z=\"0\" /></IndividualGravity><TotalTorque><Vector3 Version=\"1\" X=\"0\" Y=\"0\" Z=\"0\" /></TotalTorque><TotalForce><Vector3 Version=\"1\" X=\"0\" Y=\"0\" Z=\"0\" /></TotalForce><LocalInertia><Vector3 Version=\"1\" X=\"19424.6\" Y=\"4784.22\" Z=\"17336.3\" /></LocalInertia><ActorBasePhysicsSettings Version=\"1\" CollisionShape=\"ABasicRobotShape\" Friction=\"0.5\" Restitution=\"0\" Kinematic=\"false\" Static=\"false\" CollisionResponse=\"true\" CollisionGroup=\"1\" CollisionMask=\"18446744073709551615\" ActivationState=\"1\" CCDMotionThreshold=\"0\" CCDSphereRadius=\"0\" /></ActorRigidPhysicsSettings>");

                    if ( Testp1 == SerializeTestP1.str())
                    {
                        AddTestResult("Actor::operator<< (ActorRigidPhysicsSettings-empty)", Success, UnitTestGroup::OverWrite);
                    }else{
                        AddTestResult("Actor::operator<< (ActorRigidPhysicsSettings-empty)", Failed, UnitTestGroup::OverWrite);
                    }

                    String Testp2("<ActorRigidPhysicsSettings Version=\"1\" Mass=\"20\" AngularDamping=\"0.01\" LinearDamping=\"0.9\"><AngularVelocity><Vector3 Version=\"1\" X=\"0\" Y=\"0\" Z=\"0\" /></AngularVelocity><LinearVelocity><Vector3 Version=\"1\" X=\"0\" Y=\"0\" Z=\"0\" /></LinearVelocity><IndividualGravity><Vector3 Version=\"1\" X=\"0.9\" Y=\"9.8\" Z=\"0\" /></IndividualGravity><TotalTorque><Vector3 Version=\"1\" X=\"0\" Y=\"0\" Z=\"0\" /></TotalTorque><TotalForce><Vector3 Version=\"1\" X=\"0\" Y=\"0\" Z=\"0\" /></TotalForce><LocalInertia><Vector3 Version=\"1\" X=\"19424.6\" Y=\"4784.22\" Z=\"17336.3\" /></LocalInertia><ActorBasePhysicsSettings Version=\"1\" CollisionShape=\"ABasicRobotShape\" Friction=\"0.01\" Restitution=\"0\" Kinematic=\"false\" Static=\"false\" CollisionResponse=\"true\" CollisionGroup=\"1\" CollisionMask=\"18446744073709551615\" ActivationState=\"1\" CCDMotionThreshold=\"0.02\" CCDSphereRadius=\"0.03\" /></ActorRigidPhysicsSettings>");
                    if (Testp2 == SerializeTestP2.str())
                    {
                        AddTestResult("Actor::operator<< (ActorRigidPhysicsSettings-populated)", Success, UnitTestGroup::OverWrite);
                    }else{
                        AddTestResult("Actor::operator<< (ActorRigidPhysicsSettings-populated)", Failed, UnitTestGroup::OverWrite);
                    }

                    String TestWhole1("<ActorRigid Version=\"1\" Name=\"ABasicRobot\" File=\"robot.mesh\" Group=\"Group1\"><LinearMovementFactor><Vector3 Version=\"1\" X=\"1\" Y=\"1\" Z=\"1\" /></LinearMovementFactor><AngularMovementFactor><Vector3 Version=\"1\" X=\"1\" Y=\"1\" Z=\"1\" /></AngularMovementFactor><ActorBase Name=\"ABasicRobot\" Version=\"1\" IsInWorld=\"false\" SoundSet=\"\"><Location><Vector3 Version=\"1\" X=\"400\" Y=\"70\" Z=\"100\" /></Location><Scaling><Vector3 Version=\"1\" X=\"1\" Y=\"1\" Z=\"1\" /></Scaling><Orientation><Quaternion Version=\"1\" X=\"0.436852\" Y=\"0.436852\" Z=\"0\" W=\"0.786334\" /></Orientation><ActorGraphicsSettings Version=\"1\" /><ActorRigidPhysicsSettings Version=\"1\" Mass=\"20\" AngularDamping=\"0\" LinearDamping=\"0\"><AngularVelocity><Vector3 Version=\"1\" X=\"0\" Y=\"0\" Z=\"0\" /></AngularVelocity><LinearVelocity><Vector3 Version=\"1\" X=\"0\" Y=\"0\" Z=\"0\" /></LinearVelocity><IndividualGravity><Vector3 Version=\"1\" X=\"0\" Y=\"0\" Z=\"0\" /></IndividualGravity><TotalTorque><Vector3 Version=\"1\" X=\"0\" Y=\"0\" Z=\"0\" /></TotalTorque><TotalForce><Vector3 Version=\"1\" X=\"0\" Y=\"0\" Z=\"0\" /></TotalForce><LocalInertia><Vector3 Version=\"1\" X=\"19424.6\" Y=\"4784.22\" Z=\"17336.3\" /></LocalInertia><ActorBasePhysicsSettings Version=\"1\" CollisionShape=\"ABasicRobotShape\" Friction=\"0.5\" Restitution=\"0\" Kinematic=\"false\" Static=\"false\" CollisionResponse=\"true\" CollisionGroup=\"1\" CollisionMask=\"18446744073709551615\" ActivationState=\"1\" CCDMotionThreshold=\"0\" CCDSphereRadius=\"0\" /></ActorRigidPhysicsSettings><WorldNode Version=\"1\" Name=\"Unnamed_1\" AttachedTo=\"\" StandingType=\"0\"><Orientation><Quaternion Version=\"1\" X=\"0.436852\" Y=\"0.436852\" Z=\"0\" W=\"0.786334\" /></Orientation><Location><Vector3 Version=\"1\" X=\"400\" Y=\"70\" Z=\"100\" /></Location></WorldNode></ActorBase></ActorRigid>");
                    if ( TestWhole1 == SerializeTestWhole1.str())
                    {
                        AddTestResult("Actor::operator<< (ActorRigid-empty)", Success, UnitTestGroup::OverWrite);
                    }else{
                        AddTestResult("Actor::operator<< (ActorRigid-empty)", Failed, UnitTestGroup::OverWrite);
                    }

                    String TestWhole2("<ActorRigid Version=\"1\" Name=\"ABasicRobot\" File=\"robot.mesh\" Group=\"Group1\"><LinearMovementFactor><Vector3 Version=\"1\" X=\"1\" Y=\"1\" Z=\"1\" /></LinearMovementFactor><AngularMovementFactor><Vector3 Version=\"1\" X=\"1\" Y=\"1\" Z=\"1\" /></AngularMovementFactor><ActorBase Name=\"ABasicRobot\" Version=\"1\" IsInWorld=\"false\" SoundSet=\"\"><Location><Vector3 Version=\"1\" X=\"400\" Y=\"70\" Z=\"100\" /></Location><Scaling><Vector3 Version=\"1\" X=\"1\" Y=\"1\" Z=\"1\" /></Scaling><Orientation><Quaternion Version=\"1\" X=\"0.436852\" Y=\"0.436852\" Z=\"0\" W=\"0.786334\" /></Orientation><ActorGraphicsSettings Version=\"1\"><AmbientMaterial Submesh=\"0\"><ColourValue Version=\"1\" Red=\"0.1\" Green=\"0.2\" Blue=\"0.3\" Alpha=\"0.4\" /></AmbientMaterial><SpecularMaterial Submesh=\"0\"><ColourValue Version=\"1\" Red=\"0.2\" Green=\"0.3\" Blue=\"0.4\" Alpha=\"0.5\" /></SpecularMaterial><DiffuseMaterial Submesh=\"0\"><ColourValue Version=\"1\" Red=\"0.3\" Green=\"0.4\" Blue=\"0.5\" Alpha=\"0.6\" /></DiffuseMaterial></ActorGraphicsSettings><ActorRigidPhysicsSettings Version=\"1\" Mass=\"20\" AngularDamping=\"0.01\" LinearDamping=\"0.9\"><AngularVelocity><Vector3 Version=\"1\" X=\"0\" Y=\"0\" Z=\"0\" /></AngularVelocity><LinearVelocity><Vector3 Version=\"1\" X=\"0\" Y=\"0\" Z=\"0\" /></LinearVelocity><IndividualGravity><Vector3 Version=\"1\" X=\"0.9\" Y=\"9.8\" Z=\"0\" /></IndividualGravity><TotalTorque><Vector3 Version=\"1\" X=\"0\" Y=\"0\" Z=\"0\" /></TotalTorque><TotalForce><Vector3 Version=\"1\" X=\"0\" Y=\"0\" Z=\"0\" /></TotalForce><LocalInertia><Vector3 Version=\"1\" X=\"19424.6\" Y=\"4784.22\" Z=\"17336.3\" /></LocalInertia><ActorBasePhysicsSettings Version=\"1\" CollisionShape=\"ABasicRobotShape\" Friction=\"0.01\" Restitution=\"0\" Kinematic=\"false\" Static=\"false\" CollisionResponse=\"true\" CollisionGroup=\"1\" CollisionMask=\"18446744073709551615\" ActivationState=\"1\" CCDMotionThreshold=\"0.02\" CCDSphereRadius=\"0.03\" /></ActorRigidPhysicsSettings><WorldNode Version=\"1\" Name=\"Unnamed_1\" AttachedTo=\"\" StandingType=\"0\"><Orientation><Quaternion Version=\"1\" X=\"0.436852\" Y=\"0.436852\" Z=\"0\" W=\"0.786334\" /></Orientation><Location><Vector3 Version=\"1\" X=\"400\" Y=\"70\" Z=\"100\" /></Location></WorldNode></ActorBase></ActorRigid>");
                    if (TestWhole2 == SerializeTestWhole2.str())
                    {
                        AddTestResult("Actor::operator<< (ActorRigid-populated)", Success, UnitTestGroup::OverWrite);
                    }else{
                        AddTestResult("Actor::operator<< (ActorRigid-populated)", Failed, UnitTestGroup::OverWrite);
                    }

                    object1->GetGraphicsSettings()->SetMaterialAmbient(Mezzanine::ColourValue(0.0,0.0,0.0,0.0));
                    object1->GetGraphicsSettings()->SetMaterialSpecular(Mezzanine::ColourValue(0.0,0.0,0.0,0.0));
                    object1->GetGraphicsSettings()->SetMaterialDiffuse(Mezzanine::ColourValue(0.0,0.0,0.0,0.0));
                    object1->GetPhysicsSettings()->SetDamping(0.0,0.00);
                    object1->GetPhysicsSettings()->SetIndividualGravity(Mezzanine::Vector3(0.0,0.0,0.0));
                    object1->GetPhysicsSettings()->SetFriction(0.00);
                    object1->GetPhysicsSettings()->SetCCDParams(0.00,0.00);
                    object1->GetObjectNode()->SetLocation(Vector3(10.0,11.0,12.0));

                    SerializeTestA1 >> *(object1->GetGraphicsSettings());
                    std::stringstream DeSerializeTestA1;
                    DeSerializeTestA1 << *(object1->GetGraphicsSettings());

                    SerializeTestA2 >> *(object1->GetGraphicsSettings());
                    std::stringstream DeSerializeTestA2;
                    DeSerializeTestA2 << *(object1->GetGraphicsSettings());

                    if ( Test1 == DeSerializeTestA1.str() )
                    {
                        AddTestResult("Actor::operator>> (ActorGraphicsSetting-empty)", Success, UnitTestGroup::OverWrite);
                    }else{
                        AddTestResult("Actor::operator>> (ActorGraphicsSetting-empty)", Failed, UnitTestGroup::OverWrite);
                    }

                    if ( Test2 == DeSerializeTestA2.str() )
                    {
                        AddTestResult("Actor::operator>> (ActorGraphicsSetting-3colour)", Success, UnitTestGroup::OverWrite);
                    }else{
                        AddTestResult("Actor::operator>> (ActorGraphicsSetting-3colour)", Failed, UnitTestGroup::OverWrite);
                    }

                    object1->SetLocation(Vector3());
                    object1->GetGraphicsSettings()->SetMaterialAmbient(Mezzanine::ColourValue(0.0,0.0,0.0,0.0));
                    object1->GetGraphicsSettings()->SetMaterialSpecular(Mezzanine::ColourValue(0.0,0.0,0.0,0.0));
                    object1->GetGraphicsSettings()->SetMaterialDiffuse(Mezzanine::ColourValue(0.0,0.0,0.0,0.0));
                    object1->GetPhysicsSettings()->SetDamping(0.0,0.00);
                    object1->GetPhysicsSettings()->SetIndividualGravity(Mezzanine::Vector3(0.0,0.0,0.0));
                    object1->GetPhysicsSettings()->SetFriction(0.00);
                    object1->GetPhysicsSettings()->SetCCDParams(0.00,0.00);
                    object1->GetObjectNode()->SetLocation(Vector3(10.0,11.0,12.0));

                    SerializeTestP1 >> *(object1->GetPhysicsSettings());
                    std::stringstream DeSerializeTestP1;
                    DeSerializeTestP1 << *(object1->GetPhysicsSettings());

                    SerializeTestP2 >> *(object1->GetPhysicsSettings());
                    std::stringstream DeSerializeTestP2;
                    DeSerializeTestP2 << *(object1->GetPhysicsSettings());

                    if ( Testp1 == DeSerializeTestP1.str() )
                    {
                        AddTestResult("Actor::operator>> (ActorRigidPhysicsSettings-empty)", Success, UnitTestGroup::OverWrite);
                    }else{
                        AddTestResult("Actor::operator>> (ActorRigidPhysicsSettings-empty)", Failed, UnitTestGroup::OverWrite);
                    }

                    if ( Testp2 == DeSerializeTestP2.str() )
                    {
                        AddTestResult("Actor::operator>> (ActorRigidPhysicsSettings-populated)", Success, UnitTestGroup::OverWrite);
                    }else{
                        AddTestResult("Actor::operator>> (ActorRigidPhysicsSettings-populated)", Failed, UnitTestGroup::OverWrite);
                    }

                    object1->SetLocation(Vector3());
                    object1->GetGraphicsSettings()->SetMaterialAmbient(Mezzanine::ColourValue(0.0,0.0,0.0,0.0));
                    object1->GetGraphicsSettings()->SetMaterialSpecular(Mezzanine::ColourValue(0.0,0.0,0.0,0.0));
                    object1->GetGraphicsSettings()->SetMaterialDiffuse(Mezzanine::ColourValue(0.0,0.0,0.0,0.0));
                    object1->GetPhysicsSettings()->SetDamping(0.0,0.00);
                    object1->GetPhysicsSettings()->SetIndividualGravity(Mezzanine::Vector3(0.0,0.0,0.0));
                    object1->GetPhysicsSettings()->SetFriction(0.00);
                    object1->GetPhysicsSettings()->SetCCDParams(0.00,0.00);

                    SerializeTestWhole1 >> *object1;
                    std::stringstream DeSerializeTestWhole1;
                    DeSerializeTestWhole1 << *object1;

                    SerializeTestWhole2 >> *object1;
                    std::stringstream DeSerializeTestWhole2;
                    DeSerializeTestWhole2 << *object1;

                    //cout  << endl << endl << TestWhole1 << endl << " == " << endl << DeSerializeTestWhole1.str() << endl << endl;
                    if ( TestWhole1 == DeSerializeTestWhole1.str() )
                    {
                        AddTestResult("Actor::operator>> (ActorRigid-empty)", Success, UnitTestGroup::OverWrite);
                    }else{
                        AddTestResult("Actor::operator>> (ActorRigid-empty)", Failed, UnitTestGroup::OverWrite);
                    }

                    //cout  << endl << endl << TestWhole2 << endl << " == " << endl << DeSerializeTestWhole2.str() << endl << endl;
                    if ( TestWhole2 == DeSerializeTestWhole2.str() )
                    {
                        AddTestResult("Actor::operator>> (ActorRigid-populated)", Success, UnitTestGroup::OverWrite);
                    }else{
                        AddTestResult("Actor::operator>> (ActorRigid-populated)", Failed, UnitTestGroup::OverWrite);
                    }

                    std::stringstream DeSerializerTest1, DeSerializerTest2, DeSerializerTest1RS, DeSerializerTest2RS;
                    DeSerializerTest1 << TestWhole1 ;
                    DeSerializerTest2 << "<RigidActors>" << TestWhole2 << "</RigidActors>";

                    ActorRigidDeSerializer ActorFactory(ActorManager::GetSingletonPtr());

                    ActorManager::GetSingletonPtr()->RemoveAllActors();
                    delete object1;
                    ActorFactory.DeSerialize(DeSerializerTest1);
                    DeSerializerTest1RS << *(dynamic_cast<ActorRigid*>(ActorManager::GetSingletonPtr()->GetActor("ABasicRobot")));

                    ActorManager::GetSingletonPtr()->RemoveAllActors();
                    delete object1;
                    ActorFactory.DeSerializeAll(DeSerializerTest2);
                    DeSerializerTest2RS << "<RigidActors>" << *( dynamic_cast<ActorRigid*>(ActorManager::GetSingletonPtr()->GetActor("ABasicRobot")) ) << "</RigidActors>";
                    //cout  << endl << endl << TestWhole1 << endl << " == " << endl << DeSerializerTest1RS.str() << endl << endl;
                    if (TestWhole1 == DeSerializerTest1RS.str())
                    {
                        AddTestResult("ActorRigidDeSerializer::DeSerialize()", Success, UnitTestGroup::OverWrite);
                    }else{
                        AddTestResult("ActorRigidDeSerializer::DeSerialize()", Failed, UnitTestGroup::OverWrite);
                    }

                    //cout  << endl << endl << DeSerializerTest2.str() << endl << " == " << endl << DeSerializerTest2RS.str() << endl << endl;
                    if (DeSerializerTest2.str() == DeSerializerTest2RS.str())
                    {
                        AddTestResult("ActorRigidDeSerializer::DeSerializeAll()", Success, UnitTestGroup::OverWrite);
                    }else{
                        AddTestResult("ActorRigidDeSerializer::DeSerializeAll()", Failed, UnitTestGroup::OverWrite);
                    }
                }
                catch(Mezzanine::Exception e)
                {
                    std::cout << std::endl << std::endl << e.what() << std::endl << std::endl ;
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
                AddTestResult("Actor::operator<< (ActorRigid-empty)", Skipped);
                AddTestResult("Actor::operator<< (ActorRigid-populated)", Skipped);
                AddTestResult("Actor::operator>> (ActorRigid-empty)", Skipped);
                AddTestResult("Actor::operator>> (ActorRigid-populated)", Skipped);
                AddTestResult("ActorRigidDeSerializer::DeSerialize()", Skipped);
                AddTestResult("ActorRigidDeSerializer::DeSerializeAll()", Skipped);
            }

        }
};
#endif

