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
#ifndef _constrainttests_h
#define _constrainttests_h

#include "main.h"
#include <sstream>

/*void ttt(String temp1, String temp2)
{
    for (Whole Counter = 0; Counter < temp1.length() || Counter < temp2.length(); ++Counter)
    {
        if (Counter < temp1.length() && Counter < temp2.length())
        {
            if (temp1.at(Counter)==temp2.at(Counter))
                cout << temp1.at(Counter);
            else{
                cout << endl << "Mismatch - 1:" << temp1.at(Counter) << " 2:" << temp2.at(Counter) << endl << endl;
            }
        }else{
            cout << endl << "Length Mismatch - ";
            if (temp1.length() < temp2.length())
            {
                cout << "2:\"" << temp2.at(Counter) << "\" - " << (int)temp2.at(Counter) << endl;
            }else{

                cout << "1:\"" << temp1.at(Counter) << "\" - " << (int)temp1.at(Counter) << endl;
            }
        }
    }
}*/

// A simple test to see if the compiler flag are set correctly
// mostly this is a sample of a very simple, but useful unit test (it fixed 2 bugs when we wrote it.)
class ConstraintTests : public UnitTestGroup
{
    public:
        virtual TestResult RunTests(bool RunAutomaticTests, bool RunInteractiveTests)
        {
            if (RunAutomaticTests)
            {
                StartEngine();
                AddTestResult("Point2PointConstraint::operator<< (Params-Empty)", Unknown);
                AddTestResult("Point2PointConstraint::operator<< (Params-1Param)", Unknown);
                AddTestResult("Point2PointConstraint::operator<< (Params-WithParams)", Unknown);
                AddTestResult("Point2PointConstraint::operator>> (Params-Empty)", Unknown);
                AddTestResult("Point2PointConstraint::operator>> (Params-1Param)", Unknown);
                AddTestResult("Point2PointConstraint::operator>> (Params-WithParams)", Unknown);

                AddTestResult("HingeConstraint::operator<< (Params-Empty)", Unknown);
                AddTestResult("HingeConstraint::operator<< (Params-1Param)", Unknown);
                AddTestResult("HingeConstraint::operator<< (Params-WithParams)", Unknown);
                AddTestResult("HingeConstraint::operator>> (Params-Empty)", Unknown);
                AddTestResult("HingeConstraint::operator>> (Params-1Param)", Unknown);
                AddTestResult("HingeConstraint::operator>> (Params-WithParams)", Unknown);

                AddTestResult("Generic6DofConstraint::operator<< (Empty)", Unknown);
                AddTestResult("Generic6DofConstraint::operator<< (Values)", Unknown);
                AddTestResult("Generic6DofConstraint::operator<< (ParamsAndValues)", Unknown);
                AddTestResult("Generic6DofConstraint::operator>> (Empty)", Unknown);
                AddTestResult("Generic6DofConstraint::operator>> (Values)", Unknown);
                AddTestResult("Generic6DofConstraint::operator>> (ParamsAndValues)", Unknown);

                AddTestResult("Generic6DofSpringConstraint::operator<< (Empty)", Unknown);
                AddTestResult("Generic6DofSpringConstraint::operator<< (Values)", Unknown);
                AddTestResult("Generic6DofSpringConstraint::operator>> (Empty)", Unknown);
                AddTestResult("Generic6DofSpringConstraint::operator>> (Values)", Unknown);

                AddTestResult("Point2PointConstraint::HasParamBeenSet-UnsetCFM", Unknown);
                AddTestResult("Point2PointConstraint::HasParamBeenSet-SetCFM", Unknown);
                AddTestResult("Point2PointConstraint::HasParamBeenSet-UnsetERP", Unknown);
                AddTestResult("Point2PointConstraint::HasParamBeenSet-SetERP", Unknown);
                AddTestResult("Point2PointConstraint::HasParamBeenSet-UnsetStopCFM", Unknown);
                AddTestResult("Point2PointConstraint::HasParamBeenSet-SetStopCFM", Unknown);
                AddTestResult("Point2PointConstraint::HasParamBeenSet-UnsetStopERP", Unknown);
                AddTestResult("Point2PointConstraint::HasParamBeenSet-SetStopERP", Unknown);

                // Make minimal set for constraint
                String groupname ("Group1");
                String filerobot ("robot.mesh");
                TheWorld->GetResourceManager()->AddResourceLocation("data/common", "FileSystem", groupname, false);
                CollisionShape* RobotShape = TheWorld->GetCollisionShapeManager()->GenerateConvexHull("ABasicRobotShape",filerobot,groupname);

                ActorRigid *ActorA = new ActorRigid (20.0,"RobotA",filerobot,groupname);
                ActorA->GetPhysicsSettings();//->SetCollisionShape();
                ActorA->GetPhysicsSettings()->SetCollisionShape(RobotShape);
                ActorA->SetLocation(Vector3(100,0,0));
                TheWorld->GetActorManager()->AddActor(ActorA);

                ActorRigid *ActorB = new ActorRigid (20.0,"RobotB",filerobot,groupname);
                ActorB->GetPhysicsSettings()->SetCollisionShape(RobotShape);
                ActorB->SetLocation(Vector3(110,0,0));
                TheWorld->GetActorManager()->AddActor(ActorB);



                Point2PointConstraint Testee( ActorA, ActorB, Vector3(0.0,0.0,5.0), Vector3(0.0,5.0,0.0) );
                Point2PointConstraint TesteeRS( ActorA, ActorB, Vector3(0.0,0.0,0.0), Vector3(0.0,0.0,0.0) );
                Testee.SetDamping(0.1);

                // make some places to store results
                std::stringstream SerializeTestP2PEmpty, SerializeTestP2PWithOneParam, SerializeTestP2PWithAllParams;
                std::stringstream SerializeTestP2PEmptyRS, SerializeTestP2PWithOneParamRS, SerializeTestP2PWithAllParamsRS;
                bool SetCFM=false,      SetERP=false,   SetStopCFM=false,   SetStopERP=false;       // Should store True on successful test
                bool UnsetCFM=true,     UnsetERP=true,  UnsetStopCFM=false, UnsetStopERP=false;     // Should store False on successful test

                SerializeTestP2PEmpty << Testee;                        // P2P Serialize Test1
                try{
                    SerializeTestP2PEmpty >> TesteeRS;
                    SerializeTestP2PEmptyRS <<TesteeRS;
                }catch (Exception e){ cerr << endl << endl << e.what() << endl << endl; }

                UnsetCFM = Testee.HasParamBeenSet(Con_CFM,-1);          // P2P All of the Unset test
                UnsetStopCFM = Testee.HasParamBeenSet(Con_Stop_CFM,-1);
                UnsetERP = Testee.HasParamBeenSet(Con_ERP,-1);
                UnsetStopERP = Testee.HasParamBeenSet(Con_Stop_ERP,-1);


                Testee.SetParam(Con_CFM,0.1,-1);                        // P2P Serialize Test2
                SerializeTestP2PWithOneParam << Testee;
                try{
                    SerializeTestP2PWithOneParam >> TesteeRS;
                    SerializeTestP2PWithOneParamRS << TesteeRS;
                }catch (Exception e){ cerr << endl << endl << e.what() << endl << endl; }

                Testee.SetParam(Con_ERP,0.2,-1);                        // P2P Serialize Test3
                SerializeTestP2PWithAllParams << Testee;
                try{
                    SerializeTestP2PWithAllParams >> TesteeRS;
                    SerializeTestP2PWithAllParamsRS << TesteeRS;
                }catch (Exception e){ cerr << endl << endl << e.what() << endl << endl; }

                SetCFM = Testee.HasParamBeenSet(Con_CFM,-1);            // All of the Set tests for P2P
                SetStopCFM = Testee.HasParamBeenSet(Con_Stop_CFM,-1);
                SetERP = Testee.HasParamBeenSet(Con_ERP,-1);
                SetStopERP = Testee.HasParamBeenSet(Con_Stop_ERP,-1);

                //Check Test results
                String Test1("<Point2PointConstraint Version=\"1\" Tau=\"0.3\" ImpulseClamping=\"0\" Damping=\"0.1\"><ActorA><Vector3 Version=\"1\" X=\"0\" Y=\"0\" Z=\"5\" /></ActorA><ActorB><Vector3 Version=\"1\" X=\"0\" Y=\"5\" Z=\"0\" /></ActorB><TypedConstraint Version=\"1\" ActorNameA=\"RobotA\" ActorNameB=\"RobotB\" /></Point2PointConstraint>");
                if ( Test1 == SerializeTestP2PEmpty.str())
                {
                    AddTestResult("Point2PointConstraint::operator<< (Params-Empty)", Success, UnitTestGroup::OverWrite);
                }else{
                    AddTestResult("Point2PointConstraint::operator<< (Params-Empty)", Failed, UnitTestGroup::OverWrite);
                }

                String Test2("<Point2PointConstraint Version=\"1\" Tau=\"0.3\" ImpulseClamping=\"0\" Damping=\"0.1\"><ActorA><Vector3 Version=\"1\" X=\"0\" Y=\"0\" Z=\"5\" /></ActorA><ActorB><Vector3 Version=\"1\" X=\"0\" Y=\"5\" Z=\"0\" /></ActorB><TypedConstraint Version=\"1\" ActorNameA=\"RobotA\" ActorNameB=\"RobotB\"><Axis-1 Con_CFM=\"0.1\" Con_Stop_CFM=\"0.1\" /></TypedConstraint></Point2PointConstraint>");
                if (Test2 == SerializeTestP2PWithOneParam.str())
                {
                    AddTestResult("Point2PointConstraint::operator<< (Params-1Param)", Success, UnitTestGroup::OverWrite);
                }else{
                    AddTestResult("Point2PointConstraint::operator<< (Params-1Param)", Failed, UnitTestGroup::OverWrite);
                }

                String Test3("<Point2PointConstraint Version=\"1\" Tau=\"0.3\" ImpulseClamping=\"0\" Damping=\"0.1\"><ActorA><Vector3 Version=\"1\" X=\"0\" Y=\"0\" Z=\"5\" /></ActorA><ActorB><Vector3 Version=\"1\" X=\"0\" Y=\"5\" Z=\"0\" /></ActorB><TypedConstraint Version=\"1\" ActorNameA=\"RobotA\" ActorNameB=\"RobotB\"><Axis-1 Con_ERP=\"0.2\" Con_Stop_ERP=\"0.2\" Con_CFM=\"0.1\" Con_Stop_CFM=\"0.1\" /></TypedConstraint></Point2PointConstraint>");
                if (Test3 == SerializeTestP2PWithAllParams.str())
                {
                    AddTestResult("Point2PointConstraint::operator<< (Params-WithParams)", Success, UnitTestGroup::OverWrite);
                }else{
                    AddTestResult("Point2PointConstraint::operator<< (Params-WithParams)", Failed, UnitTestGroup::OverWrite);
                }


                if ( Test1 == SerializeTestP2PEmptyRS.str())
                {
                    AddTestResult("Point2PointConstraint::operator>> (Params-Empty)", Success, UnitTestGroup::OverWrite);
                }else{
                    AddTestResult("Point2PointConstraint::operator>> (Params-Empty)", Failed, UnitTestGroup::OverWrite);
                }

                if (Test2 == SerializeTestP2PWithOneParamRS.str())
                {
                    AddTestResult("Point2PointConstraint::operator>> (Params-1Param)", Success, UnitTestGroup::OverWrite);
                }else{
                    AddTestResult("Point2PointConstraint::operator>> (Params-1Param)", Failed, UnitTestGroup::OverWrite);
                }
                //cout <<endl <<endl << "Expected: \t"<< Test3 <<endl << endl << "Recieved: \t"<< SerializeTestP2PWithAllParamsRS.str() <<endl <<endl;
                if (Test3 == SerializeTestP2PWithAllParamsRS.str())
                {
                    AddTestResult("Point2PointConstraint::operator>> (Params-WithParams)", Success, UnitTestGroup::OverWrite);
                }else{
                    AddTestResult("Point2PointConstraint::operator>> (Params-WithParams)", Failed, UnitTestGroup::OverWrite);
                }


                if ( !UnsetCFM )
                {
                    AddTestResult("Point2PointConstraint::HasParamBeenSet-UnsetCFM", Success, UnitTestGroup::OverWrite);
                }else{
                    AddTestResult("Point2PointConstraint::HasParamBeenSet-UnsetCFM", Failed, UnitTestGroup::OverWrite);
                }

                if ( !UnsetStopCFM )
                {
                    AddTestResult("Point2PointConstraint::HasParamBeenSet-UnsetStopCFM", Success, UnitTestGroup::OverWrite);
                }else{
                    AddTestResult("Point2PointConstraint::HasParamBeenSet-UnsetStopCFM", Failed, UnitTestGroup::OverWrite);
                }

                if ( !UnsetERP )
                {
                    AddTestResult("Point2PointConstraint::HasParamBeenSet-UnsetERP", Success, UnitTestGroup::OverWrite);
                }else{
                    AddTestResult("Point2PointConstraint::HasParamBeenSet-UnsetERP", Failed, UnitTestGroup::OverWrite);
                }

                if ( !UnsetStopERP )
                {
                    AddTestResult("Point2PointConstraint::HasParamBeenSet-UnsetStopERP", Success, UnitTestGroup::OverWrite);
                }else{
                    AddTestResult("Point2PointConstraint::HasParamBeenSet-UnsetStopERP", Failed, UnitTestGroup::OverWrite);
                }

                if ( SetCFM )
                {
                    AddTestResult("Point2PointConstraint::HasParamBeenSet-SetCFM", Success, UnitTestGroup::OverWrite);
                }else{
                    AddTestResult("Point2PointConstraint::HasParamBeenSet-SetCFM", Failed, UnitTestGroup::OverWrite);
                }

                if ( SetStopCFM )
                {
                    AddTestResult("Point2PointConstraint::HasParamBeenSet-SetStopCFM", Success, UnitTestGroup::OverWrite);
                }else{
                    AddTestResult("Point2PointConstraint::HasParamBeenSet-SetStopCFM", Failed, UnitTestGroup::OverWrite);
                }

                if ( SetERP )
                {
                    AddTestResult("Point2PointConstraint::HasParamBeenSet-SetERP", Success, UnitTestGroup::OverWrite);
                }else{
                    AddTestResult("Point2PointConstraint::HasParamBeenSet-SetERP", Failed, UnitTestGroup::OverWrite);
                }

                if ( SetStopERP )
                {
                    AddTestResult("Point2PointConstraint::HasParamBeenSet-SetStopERP", Success, UnitTestGroup::OverWrite);
                }else{
                    AddTestResult("Point2PointConstraint::HasParamBeenSet-SetStopERP", Failed, UnitTestGroup::OverWrite);
                }





                HingeConstraint HingeTestee( ActorA, ActorB,                    // Prepare for the hinge test.
                                Transform(Vector3(1.0, 2.0, 3.0), Quaternion(0.0, 1.0, 0.0, 0.4)),
                                Transform(Vector3(5.0, 6.0, 7.0), Quaternion(1.0, 0.0, 0.0, 0.8))
                                );
                HingeConstraint HingeTesteeRS( ActorA, ActorB,                    // Prepare for the hinge test.
                                Transform(Vector3(0.0, 0.0, 0.0), Quaternion(0.0, 0.0, 0.0, 0.0)),
                                Transform(Vector3(0.0, 0.0, 0.0), Quaternion(0.0, 0.0, 0.0, 0.0))
                            );
                HingeTestee.EnableMotor(true, 0.05, .01);
                std::stringstream SerializeTestHinge1, SerializeTestHinge2, SerializeTestHinge3;
                std::stringstream SerializeTestHinge1RS, SerializeTestHinge2RS, SerializeTestHinge3RS;

                SerializeTestHinge1 << HingeTestee;                             // Hinge test empty
                try{
                    SerializeTestHinge1 >> HingeTesteeRS;
                    SerializeTestHinge1RS << HingeTesteeRS;
                }catch (Exception e){ cerr << endl << endl << e.what() << endl << endl; }
                //cout <<endl <<endl << HingeTestee <<endl <<endl;

                HingeTestee.SetParam(Con_CFM,0.1,-1);
                SerializeTestHinge2 << HingeTestee;                             // Hinge test 1 param
                try{
                    SerializeTestHinge2 >> HingeTesteeRS;
                    SerializeTestHinge2RS << HingeTesteeRS;
                }catch (Exception e){ cerr << endl << endl << e.what() << endl << endl; }

                HingeTestee.SetParam(Con_CFM,0.2,5);
                HingeTestee.SetParam(Con_Stop_ERP,0.3,-1);
                HingeTestee.SetParam(Con_Stop_ERP,0.4,5);
                HingeTestee.SetParam(Con_Stop_CFM,0.5,-1);
                HingeTestee.SetParam(Con_Stop_CFM,0.6,5);
                SerializeTestHinge3 << HingeTestee;                             // Hinge test All Params
                try{
                    SerializeTestHinge3 >> HingeTesteeRS;
                    SerializeTestHinge3RS << HingeTesteeRS;
                }catch (Exception e){ cerr << endl << endl << e.what() << endl << endl; }


                String HingeTest1("<HingeConstraint Version=\"1\" ReferenceInA=\"false\" UseFrameOffset=\"true\"><Motor Enabled=\"true\" MaxImpulse=\"0.01\" TargetVelocity=\"0.05\" /><Limits Low=\"1\" High=\"-1\" Softness=\"0.9\" BiasFactor=\"0.3\" RelaxationFactor=\"1\" /><DualTransformConstraint Version=\"1\"><ActorA><Transform Version=\"1\"><Vector3 Version=\"1\" X=\"1\" Y=\"2\" Z=\"3\" /><Quaternion Version=\"1\" X=\"0\" Y=\"0.928477\" Z=\"0\" W=\"0.371391\" /></Transform></ActorA><ActorB><Transform Version=\"1\"><Vector3 Version=\"1\" X=\"5\" Y=\"6\" Z=\"7\" /><Quaternion Version=\"1\" X=\"0.780869\" Y=\"0\" Z=\"0\" W=\"0.624695\" /></Transform></ActorB><TypedConstraint Version=\"1\" ActorNameA=\"RobotA\" ActorNameB=\"RobotB\" /></DualTransformConstraint></HingeConstraint>");
                if ( HingeTest1 == SerializeTestHinge1.str())
                {
                    AddTestResult("HingeConstraint::operator<< (Params-Empty)", Success, UnitTestGroup::OverWrite);
                }else{
                    AddTestResult("HingeConstraint::operator<< (Params-Empty)", Failed, UnitTestGroup::OverWrite);
                }

                String HingeTest2("<HingeConstraint Version=\"1\" ReferenceInA=\"false\" UseFrameOffset=\"true\"><Motor Enabled=\"true\" MaxImpulse=\"0.01\" TargetVelocity=\"0.05\" /><Limits Low=\"1\" High=\"-1\" Softness=\"0.9\" BiasFactor=\"0.3\" RelaxationFactor=\"1\" /><DualTransformConstraint Version=\"1\"><ActorA><Transform Version=\"1\"><Vector3 Version=\"1\" X=\"1\" Y=\"2\" Z=\"3\" /><Quaternion Version=\"1\" X=\"0\" Y=\"0.928477\" Z=\"0\" W=\"0.371391\" /></Transform></ActorA><ActorB><Transform Version=\"1\"><Vector3 Version=\"1\" X=\"5\" Y=\"6\" Z=\"7\" /><Quaternion Version=\"1\" X=\"0.780869\" Y=\"0\" Z=\"0\" W=\"0.624695\" /></Transform></ActorB><TypedConstraint Version=\"1\" ActorNameA=\"RobotA\" ActorNameB=\"RobotB\"><Axis-1 Con_CFM=\"0.1\" /><Axis5 Con_CFM=\"0.1\" /></TypedConstraint></DualTransformConstraint></HingeConstraint>");
                if ( HingeTest2 == SerializeTestHinge2.str())
                {
                    AddTestResult("HingeConstraint::operator<< (Params-1Param)", Success, UnitTestGroup::OverWrite);
                }else{
                    AddTestResult("HingeConstraint::operator<< (Params-1Param)", Failed, UnitTestGroup::OverWrite);
                }

                String HingeTest3("<HingeConstraint Version=\"1\" ReferenceInA=\"false\" UseFrameOffset=\"true\"><Motor Enabled=\"true\" MaxImpulse=\"0.01\" TargetVelocity=\"0.05\" /><Limits Low=\"1\" High=\"-1\" Softness=\"0.9\" BiasFactor=\"0.3\" RelaxationFactor=\"1\" /><DualTransformConstraint Version=\"1\"><ActorA><Transform Version=\"1\"><Vector3 Version=\"1\" X=\"1\" Y=\"2\" Z=\"3\" /><Quaternion Version=\"1\" X=\"0\" Y=\"0.928477\" Z=\"0\" W=\"0.371391\" /></Transform></ActorA><ActorB><Transform Version=\"1\"><Vector3 Version=\"1\" X=\"5\" Y=\"6\" Z=\"7\" /><Quaternion Version=\"1\" X=\"0.780869\" Y=\"0\" Z=\"0\" W=\"0.624695\" /></Transform></ActorB><TypedConstraint Version=\"1\" ActorNameA=\"RobotA\" ActorNameB=\"RobotB\"><Axis-1 Con_Stop_ERP=\"0.4\" Con_CFM=\"0.2\" Con_Stop_CFM=\"0.6\" /><Axis5 Con_Stop_ERP=\"0.4\" Con_CFM=\"0.2\" Con_Stop_CFM=\"0.6\" /></TypedConstraint></DualTransformConstraint></HingeConstraint>");
                if ( HingeTest3 == SerializeTestHinge3.str())
                {
                    AddTestResult("HingeConstraint::operator<< (Params-WithParams)", Success, UnitTestGroup::OverWrite);
                }else{
                    AddTestResult("HingeConstraint::operator<< (Params-WithParams)", Failed, UnitTestGroup::OverWrite);
                }

                //cout <<endl <<endl << "Expected: \t"<< HingeTest1 <<endl << endl << "Recieved: \t"<< SerializeTestHinge1RS.str() <<endl <<endl;
                if ( HingeTest1 == SerializeTestHinge1RS.str())
                {
                    AddTestResult("HingeConstraint::operator>> (Params-Empty)", Success, UnitTestGroup::OverWrite);
                }else{
                    AddTestResult("HingeConstraint::operator>> (Params-Empty)", Failed, UnitTestGroup::OverWrite);
                }

                if ( HingeTest2 == SerializeTestHinge2RS.str())
                {
                    AddTestResult("HingeConstraint::operator>> (Params-1Param)", Success, UnitTestGroup::OverWrite);
                }else{
                    AddTestResult("HingeConstraint::operator>> (Params-1Param)", Failed, UnitTestGroup::OverWrite);
                }

                if ( HingeTest3 == SerializeTestHinge3RS.str())
                {
                    AddTestResult("HingeConstraint::operator>> (Params-WithParams)", Success, UnitTestGroup::OverWrite);
                }else{
                    AddTestResult("HingeConstraint::operator>> (Params-WithParams)", Failed, UnitTestGroup::OverWrite);
                }



                stringstream SerializeTestG6dof1, SerializeTestG6dof2, SerializeTestG6dof3; // Prepare for the G6dof test.
                stringstream SerializeTestG6dof1RS, SerializeTestG6dof2RS, SerializeTestG6dof3RS;

                Generic6DofConstraint G6dofTestee( ActorA, ActorB,
                                Transform(Vector3(1.0, 2.0, 3.0), Quaternion(0.0, 1.0, 0.0, 0.4)),
                                Transform(Vector3(5.0, 6.0, 7.0), Quaternion(1.0, 0.0, 0.0, 0.8))
                            );
                Generic6DofConstraint G6dofTesteeRS( ActorA, ActorB,
                                Transform(Vector3(0.0, 0.0, 0.0), Quaternion(0.0, 0.0, 0.0, 0.0)),
                                Transform(Vector3(0.0, 0.0, 0.0), Quaternion(0.0, 0.0, 0.0, 0.0))
                            );
                SerializeTestG6dof1 << G6dofTestee;                                         // 6dof test1
                try{
                    SerializeTestG6dof1 >> G6dofTesteeRS;
                    SerializeTestG6dof1RS << G6dofTesteeRS;
                }catch (Exception e){ cerr << endl << endl << e.what() << endl << endl; }

                G6dofTestee.SetLinearLimitUpper(Vector3(10.0, 11.0, 12.0));                 // 6dof test2
                G6dofTestee.SetLinearLimitLower(Vector3(-10.0, -11.0, -12.0));
                G6dofTestee.SetAngularLimitUpper(Vector3(0.1, 0.2, 0.3));
                G6dofTestee.SetAngularLimitLower(Vector3(-0.1, -0.2, -0.3));

                G6dofTestee.SetAngularLimitMaxForce(Vector3(13.0, 14.0, 15.0));
                G6dofTestee.SetAngularMotorTargetVelocity(Vector3(16.0, 17.0, 18.0));
                G6dofTestee.SetAngularMotorMaxForce(Vector3(19.0, 20.0, 21.0));
                G6dofTestee.SetAngularMotorDamping(Vector3(22.0, 23.0, 24.0));
                G6dofTestee.SetAngularMotorRestitution(Vector3(0.01, 0.02, 0.03));
                G6dofTestee.SetAngularMotorEnabled(Vector3(1, 0, 1));

                G6dofTestee.SetLinearLimitSoftness(0.8);
                G6dofTestee.SetLinearLimitDamping(25);
                G6dofTestee.SetLinearLimitRestitution(0.9);
                G6dofTestee.SetLinearMotorMaxForce(Vector3(26.0, 27.0, 28.0));
                G6dofTestee.SetLinearMotorTargetVelocity(Vector3(29.0, 30.0, 31.0));
                G6dofTestee.SetLinearMotorEnabled(Vector3(0, 1, 0));

                SerializeTestG6dof2 << G6dofTestee;
                try{
                    SerializeTestG6dof2 >> G6dofTesteeRS;
                    SerializeTestG6dof2RS << G6dofTesteeRS;
                }catch (Exception e){ cerr << endl << endl << e.what() << endl << endl; }

                G6dofTestee.SetParam(Con_Stop_ERP,0.01,0);                                        // 6dof Test3
                G6dofTestee.SetParam(Con_CFM,0.02,0);
                G6dofTestee.SetParam(Con_Stop_CFM,0.03,0);
                G6dofTestee.SetParam(Con_Stop_ERP,0.04,1);
                G6dofTestee.SetParam(Con_CFM,0.05,1);
                G6dofTestee.SetParam(Con_Stop_CFM,0.06,1);
                G6dofTestee.SetParam(Con_Stop_ERP,0.07,2);
                G6dofTestee.SetParam(Con_CFM,0.08,2);
                G6dofTestee.SetParam(Con_Stop_CFM,0.09,2);
                G6dofTestee.SetParam(Con_Stop_ERP,0.10,3);
                G6dofTestee.SetParam(Con_CFM,0.11,3);
                G6dofTestee.SetParam(Con_Stop_CFM,0.12,3);
                G6dofTestee.SetParam(Con_Stop_ERP,0.13,4);
                G6dofTestee.SetParam(Con_CFM,0.14,4);
                G6dofTestee.SetParam(Con_Stop_CFM,0.15,4);
                G6dofTestee.SetParam(Con_Stop_ERP,0.16,5);
                G6dofTestee.SetParam(Con_CFM,0.17,5);
                G6dofTestee.SetParam(Con_Stop_CFM,0.18,5);
                SerializeTestG6dof3 << G6dofTestee;
                try{
                    SerializeTestG6dof3 >> G6dofTesteeRS;
                    SerializeTestG6dof3RS << G6dofTesteeRS;
                }catch (Exception e){ cerr << endl << endl << e.what() << endl << endl; }

                //cout << endl << endl << G6dofTestee << endl << endl;

                String G6dofTest1("<Generic6DofConstraint Version=\"1\" LinearLimitSoftness=\"0.7\" LinearLimitDamping=\"1\" LinearLimitRestitution=\"0.5\"><LinearLimitUpper><Vector3 Version=\"1\" X=\"0\" Y=\"0\" Z=\"0\" /></LinearLimitUpper><LinearLimitLower><Vector3 Version=\"1\" X=\"0\" Y=\"0\" Z=\"0\" /></LinearLimitLower><AngularLimitUpper><Vector3 Version=\"1\" X=\"-1\" Y=\"-1\" Z=\"-1\" /></AngularLimitUpper><AngularLimitLower><Vector3 Version=\"1\" X=\"1\" Y=\"1\" Z=\"1\" /></AngularLimitLower><AngularLimitMaxForce><Vector3 Version=\"1\" X=\"300\" Y=\"300\" Z=\"300\" /></AngularLimitMaxForce><AngularMotorTargetVelocity><Vector3 Version=\"1\" X=\"0\" Y=\"0\" Z=\"0\" /></AngularMotorTargetVelocity><AngularMotorMaxForce><Vector3 Version=\"1\" X=\"0.1\" Y=\"0.1\" Z=\"0.1\" /></AngularMotorMaxForce><AngularMotorDamping><Vector3 Version=\"1\" X=\"1\" Y=\"1\" Z=\"1\" /></AngularMotorDamping><AngularMotorRestitution><Vector3 Version=\"1\" X=\"0\" Y=\"0\" Z=\"0\" /></AngularMotorRestitution><AngularMotorEnabled><Vector3 Version=\"1\" X=\"0\" Y=\"0\" Z=\"0\" /></AngularMotorEnabled><LinearMotorMaxForce><Vector3 Version=\"1\" X=\"0\" Y=\"0\" Z=\"0\" /></LinearMotorMaxForce><LinearMotorTargetVelocity><Vector3 Version=\"1\" X=\"0\" Y=\"0\" Z=\"0\" /></LinearMotorTargetVelocity><LinearMotorEnabled><Vector3 Version=\"1\" X=\"0\" Y=\"0\" Z=\"0\" /></LinearMotorEnabled><DualTransformConstraint Version=\"1\"><ActorA><Transform Version=\"1\"><Vector3 Version=\"1\" X=\"1\" Y=\"2\" Z=\"3\" /><Quaternion Version=\"1\" X=\"0\" Y=\"0.928477\" Z=\"0\" W=\"0.371391\" /></Transform></ActorA><ActorB><Transform Version=\"1\"><Vector3 Version=\"1\" X=\"5\" Y=\"6\" Z=\"7\" /><Quaternion Version=\"1\" X=\"0.780869\" Y=\"0\" Z=\"0\" W=\"0.624695\" /></Transform></ActorB><TypedConstraint Version=\"1\" ActorNameA=\"RobotA\" ActorNameB=\"RobotB\" /></DualTransformConstraint></Generic6DofConstraint>");
                if ( G6dofTest1 == SerializeTestG6dof1.str())
                {
                    AddTestResult("Generic6DofConstraint::operator<< (Empty)", Success, UnitTestGroup::OverWrite);
                }else{
                    AddTestResult("Generic6DofConstraint::operator<< (Empty)", Failed, UnitTestGroup::OverWrite);
                }

                String G6dofTest2("<Generic6DofConstraint Version=\"1\" LinearLimitSoftness=\"0.8\" LinearLimitDamping=\"25\" LinearLimitRestitution=\"0.9\"><LinearLimitUpper><Vector3 Version=\"1\" X=\"10\" Y=\"11\" Z=\"12\" /></LinearLimitUpper><LinearLimitLower><Vector3 Version=\"1\" X=\"-10\" Y=\"-11\" Z=\"-12\" /></LinearLimitLower><AngularLimitUpper><Vector3 Version=\"1\" X=\"0.1\" Y=\"0.2\" Z=\"0.3\" /></AngularLimitUpper><AngularLimitLower><Vector3 Version=\"1\" X=\"-0.1\" Y=\"-0.2\" Z=\"-0.3\" /></AngularLimitLower><AngularLimitMaxForce><Vector3 Version=\"1\" X=\"13\" Y=\"14\" Z=\"15\" /></AngularLimitMaxForce><AngularMotorTargetVelocity><Vector3 Version=\"1\" X=\"16\" Y=\"17\" Z=\"18\" /></AngularMotorTargetVelocity><AngularMotorMaxForce><Vector3 Version=\"1\" X=\"19\" Y=\"20\" Z=\"21\" /></AngularMotorMaxForce><AngularMotorDamping><Vector3 Version=\"1\" X=\"22\" Y=\"23\" Z=\"24\" /></AngularMotorDamping><AngularMotorRestitution><Vector3 Version=\"1\" X=\"0.01\" Y=\"0.02\" Z=\"0.03\" /></AngularMotorRestitution><AngularMotorEnabled><Vector3 Version=\"1\" X=\"1\" Y=\"0\" Z=\"1\" /></AngularMotorEnabled><LinearMotorMaxForce><Vector3 Version=\"1\" X=\"26\" Y=\"27\" Z=\"28\" /></LinearMotorMaxForce><LinearMotorTargetVelocity><Vector3 Version=\"1\" X=\"29\" Y=\"30\" Z=\"31\" /></LinearMotorTargetVelocity><LinearMotorEnabled><Vector3 Version=\"1\" X=\"0\" Y=\"1\" Z=\"0\" /></LinearMotorEnabled><DualTransformConstraint Version=\"1\"><ActorA><Transform Version=\"1\"><Vector3 Version=\"1\" X=\"1\" Y=\"2\" Z=\"3\" /><Quaternion Version=\"1\" X=\"0\" Y=\"0.928477\" Z=\"0\" W=\"0.371391\" /></Transform></ActorA><ActorB><Transform Version=\"1\"><Vector3 Version=\"1\" X=\"5\" Y=\"6\" Z=\"7\" /><Quaternion Version=\"1\" X=\"0.780869\" Y=\"0\" Z=\"0\" W=\"0.624695\" /></Transform></ActorB><TypedConstraint Version=\"1\" ActorNameA=\"RobotA\" ActorNameB=\"RobotB\" /></DualTransformConstraint></Generic6DofConstraint>");
                if ( G6dofTest2 == SerializeTestG6dof2.str())
                {
                    AddTestResult("Generic6DofConstraint::operator<< (Values)", Success, UnitTestGroup::OverWrite);
                }else{
                    AddTestResult("Generic6DofConstraint::operator<< (Values)", Failed, UnitTestGroup::OverWrite);
                }

                String G6dofTest3("<Generic6DofConstraint Version=\"1\" LinearLimitSoftness=\"0.8\" LinearLimitDamping=\"25\" LinearLimitRestitution=\"0.9\"><LinearLimitUpper><Vector3 Version=\"1\" X=\"10\" Y=\"11\" Z=\"12\" /></LinearLimitUpper><LinearLimitLower><Vector3 Version=\"1\" X=\"-10\" Y=\"-11\" Z=\"-12\" /></LinearLimitLower><AngularLimitUpper><Vector3 Version=\"1\" X=\"0.1\" Y=\"0.2\" Z=\"0.3\" /></AngularLimitUpper><AngularLimitLower><Vector3 Version=\"1\" X=\"-0.1\" Y=\"-0.2\" Z=\"-0.3\" /></AngularLimitLower><AngularLimitMaxForce><Vector3 Version=\"1\" X=\"13\" Y=\"14\" Z=\"15\" /></AngularLimitMaxForce><AngularMotorTargetVelocity><Vector3 Version=\"1\" X=\"16\" Y=\"17\" Z=\"18\" /></AngularMotorTargetVelocity><AngularMotorMaxForce><Vector3 Version=\"1\" X=\"19\" Y=\"20\" Z=\"21\" /></AngularMotorMaxForce><AngularMotorDamping><Vector3 Version=\"1\" X=\"22\" Y=\"23\" Z=\"24\" /></AngularMotorDamping><AngularMotorRestitution><Vector3 Version=\"1\" X=\"0.01\" Y=\"0.02\" Z=\"0.03\" /></AngularMotorRestitution><AngularMotorEnabled><Vector3 Version=\"1\" X=\"1\" Y=\"0\" Z=\"1\" /></AngularMotorEnabled><LinearMotorMaxForce><Vector3 Version=\"1\" X=\"26\" Y=\"27\" Z=\"28\" /></LinearMotorMaxForce><LinearMotorTargetVelocity><Vector3 Version=\"1\" X=\"29\" Y=\"30\" Z=\"31\" /></LinearMotorTargetVelocity><LinearMotorEnabled><Vector3 Version=\"1\" X=\"0\" Y=\"1\" Z=\"0\" /></LinearMotorEnabled><DualTransformConstraint Version=\"1\"><ActorA><Transform Version=\"1\"><Vector3 Version=\"1\" X=\"1\" Y=\"2\" Z=\"3\" /><Quaternion Version=\"1\" X=\"0\" Y=\"0.928477\" Z=\"0\" W=\"0.371391\" /></Transform></ActorA><ActorB><Transform Version=\"1\"><Vector3 Version=\"1\" X=\"5\" Y=\"6\" Z=\"7\" /><Quaternion Version=\"1\" X=\"0.780869\" Y=\"0\" Z=\"0\" W=\"0.624695\" /></Transform></ActorB><TypedConstraint Version=\"1\" ActorNameA=\"RobotA\" ActorNameB=\"RobotB\"><Axis0 Con_Stop_ERP=\"0.01\" Con_CFM=\"0.02\" Con_Stop_CFM=\"0.03\" /><Axis1 Con_Stop_ERP=\"0.04\" Con_CFM=\"0.05\" Con_Stop_CFM=\"0.06\" /><Axis2 Con_Stop_ERP=\"0.07\" Con_CFM=\"0.08\" Con_Stop_CFM=\"0.09\" /><Axis3 Con_Stop_ERP=\"0.1\" Con_CFM=\"0.11\" Con_Stop_CFM=\"0.12\" /><Axis4 Con_Stop_ERP=\"0.13\" Con_CFM=\"0.14\" Con_Stop_CFM=\"0.15\" /><Axis5 Con_Stop_ERP=\"0.16\" Con_CFM=\"0.17\" Con_Stop_CFM=\"0.18\" /></TypedConstraint></DualTransformConstraint></Generic6DofConstraint>");
                if ( G6dofTest3 == SerializeTestG6dof3.str())
                {
                    AddTestResult("Generic6DofConstraint::operator<< (ParamsAndValues)", Success, UnitTestGroup::OverWrite);
                }else{
                    AddTestResult("Generic6DofConstraint::operator<< (ParamsAndValues)", Failed, UnitTestGroup::OverWrite);
                }

                //cout <<endl <<endl << "Expected: \t"<< G6dofTest2 <<endl << endl << "Recieved: \t"<< SerializeTestG6dof2RS.str() <<endl <<endl;
                if ( G6dofTest1 == SerializeTestG6dof1RS.str())
                {
                    AddTestResult("Generic6DofConstraint::operator>> (Empty)", Success, UnitTestGroup::OverWrite);
                }else{
                    AddTestResult("Generic6DofConstraint::operator>> (Empty)", Failed, UnitTestGroup::OverWrite);
                }

                if ( G6dofTest2 == SerializeTestG6dof2RS.str())
                {
                    AddTestResult("Generic6DofConstraint::operator>> (Values)", Success, UnitTestGroup::OverWrite);
                }else{
                    AddTestResult("Generic6DofConstraint::operator>> (Values)", Failed, UnitTestGroup::OverWrite);
                }

                if ( G6dofTest3 == SerializeTestG6dof3RS.str())
                {
                    AddTestResult("Generic6DofConstraint::operator>> (ParamsAndValues)", Success, UnitTestGroup::OverWrite);
                }else{
                    AddTestResult("Generic6DofConstraint::operator>> (ParamsAndValues)", Failed, UnitTestGroup::OverWrite);
                }

                stringstream SerializeTestG6dofSpring1, SerializeTestG6dofSpring2;                                      // Begin 6dof Spring tests
                stringstream SerializeTestG6dofSpring1RS, SerializeTestG6dofSpring2RS;

                Generic6DofSpringConstraint G6dofSpringTestee( ActorA, ActorB,
                                Transform(Vector3(1.0, 2.0, 3.0), Quaternion(0.0, 1.0, 0.0, 0.4)),
                                Transform(Vector3(5.0, 6.0, 7.0), Quaternion(1.0, 0.0, 0.0, 0.8))
                            );
                Generic6DofSpringConstraint G6dofSpringTesteeRS( ActorA, ActorB,
                                Transform(Vector3(0.0, 0.0, 0.0), Quaternion(0.0, 0.0, 0.0, 0.0)),
                                Transform(Vector3(0.0, 0.0, 0.0), Quaternion(0.0, 0.0, 0.0, 0.0))
                            );
                SerializeTestG6dofSpring1 << G6dofSpringTestee;                                                         // Test 1
                try{
                    SerializeTestG6dofSpring1 >> G6dofSpringTesteeRS;
                    SerializeTestG6dofSpring1RS << G6dofSpringTesteeRS;
                }catch (Exception e){ cerr << endl << endl << e.what() << endl << endl; }

                G6dofSpringTestee.SetLinearLimitUpper(Vector3(10.0, 11.0, 12.0));                                       // 6dof spring test2
                G6dofSpringTestee.SetLinearLimitLower(Vector3(-10.0, -11.0, -12.0));
                G6dofSpringTestee.SetAngularLimitUpper(Vector3(0.1, 0.2, 0.3));
                G6dofSpringTestee.SetAngularLimitLower(Vector3(-0.1, -0.2, -0.3));
                G6dofSpringTestee.SetAngularLimitMaxForce(Vector3(13.0, 14.0, 15.0));
                G6dofSpringTestee.SetAngularMotorTargetVelocity(Vector3(16.0, 17.0, 18.0));
                G6dofSpringTestee.SetAngularMotorMaxForce(Vector3(19.0, 20.0, 21.0));
                G6dofSpringTestee.SetAngularMotorDamping(Vector3(22.0, 23.0, 24.0));
                G6dofSpringTestee.SetAngularMotorRestitution(Vector3(0.01, 0.02, 0.03));
                G6dofSpringTestee.SetAngularMotorEnabled(Vector3(1, 0, 1));
                G6dofSpringTestee.SetLinearLimitSoftness(0.8);
                G6dofSpringTestee.SetLinearLimitDamping(25);
                G6dofSpringTestee.SetLinearLimitRestitution(0.9);
                G6dofSpringTestee.SetLinearMotorMaxForce(Vector3(26.0, 27.0, 28.0));
                G6dofSpringTestee.SetLinearMotorTargetVelocity(Vector3(29.0, 30.0, 31.0));
                G6dofSpringTestee.SetLinearMotorEnabled(Vector3(0, 1, 0));
                G6dofSpringTestee.SetParam(Con_Stop_ERP,0.01,0);
                G6dofSpringTestee.SetParam(Con_CFM,0.02,0);
                G6dofSpringTestee.SetParam(Con_Stop_CFM,0.03,0);
                G6dofSpringTestee.SetParam(Con_Stop_ERP,0.04,1);
                G6dofSpringTestee.SetParam(Con_CFM,0.05,1);
                G6dofSpringTestee.SetParam(Con_Stop_CFM,0.06,1);
                G6dofSpringTestee.SetParam(Con_Stop_ERP,0.07,2);
                G6dofSpringTestee.SetParam(Con_CFM,0.08,2);
                G6dofSpringTestee.SetParam(Con_Stop_CFM,0.09,2);
                G6dofSpringTestee.SetParam(Con_Stop_ERP,0.10,3);
                G6dofSpringTestee.SetParam(Con_CFM,0.11,3);
                G6dofSpringTestee.SetParam(Con_Stop_CFM,0.12,3);
                G6dofSpringTestee.SetParam(Con_Stop_ERP,0.13,4);
                G6dofSpringTestee.SetParam(Con_CFM,0.14,4);
                G6dofSpringTestee.SetParam(Con_Stop_CFM,0.15,4);
                G6dofSpringTestee.SetParam(Con_Stop_ERP,0.16,5);
                G6dofSpringTestee.SetParam(Con_CFM,0.17,5);
                G6dofSpringTestee.SetParam(Con_Stop_CFM,0.18,5);

                G6dofSpringTestee.SetSpringStiffness(0,0.51);
                G6dofSpringTestee.SetSpringStiffness(1,0.52);
                G6dofSpringTestee.SetSpringStiffness(2,0.53);
                G6dofSpringTestee.SetSpringStiffness(3,0.54);
                G6dofSpringTestee.SetSpringStiffness(4,0.55);
                G6dofSpringTestee.SetSpringStiffness(5,0.56);
                G6dofSpringTestee.SetSpringDamping(0,0.61);
                G6dofSpringTestee.SetSpringDamping(1,0.62);
                G6dofSpringTestee.SetSpringDamping(2,0.63);
                G6dofSpringTestee.SetSpringDamping(3,0.64);
                G6dofSpringTestee.SetSpringDamping(4,0.65);
                G6dofSpringTestee.SetSpringDamping(5,0.66);
                G6dofSpringTestee.SetSpringEnabled(0,1);
                G6dofSpringTestee.SetSpringEnabled(1,1);
                G6dofSpringTestee.SetSpringEnabled(2,0);
                G6dofSpringTestee.SetSpringEnabled(3,0);
                G6dofSpringTestee.SetSpringEnabled(4,1);
                G6dofSpringTestee.SetSpringEnabled(5,0);
                SerializeTestG6dofSpring2 << G6dofSpringTestee;
                try{
                    SerializeTestG6dofSpring2 >> G6dofSpringTesteeRS;
                    SerializeTestG6dofSpring2RS << G6dofSpringTesteeRS;
                }catch (Exception e){ cerr << endl << endl << e.what() << endl << endl; }
                //cout << endl << endl << G6dofSpringTestee << endl << endl;

                String G6dofSpringTest1("<Generic6DofSpringConstraint Version=\"1\"><Generic6DofConstraint Version=\"1\" LinearLimitSoftness=\"0.7\" LinearLimitDamping=\"1\" LinearLimitRestitution=\"0.5\"><LinearLimitUpper><Vector3 Version=\"1\" X=\"0\" Y=\"0\" Z=\"0\" /></LinearLimitUpper><LinearLimitLower><Vector3 Version=\"1\" X=\"0\" Y=\"0\" Z=\"0\" /></LinearLimitLower><AngularLimitUpper><Vector3 Version=\"1\" X=\"-1\" Y=\"-1\" Z=\"-1\" /></AngularLimitUpper><AngularLimitLower><Vector3 Version=\"1\" X=\"1\" Y=\"1\" Z=\"1\" /></AngularLimitLower><AngularLimitMaxForce><Vector3 Version=\"1\" X=\"300\" Y=\"300\" Z=\"300\" /></AngularLimitMaxForce><AngularMotorTargetVelocity><Vector3 Version=\"1\" X=\"0\" Y=\"0\" Z=\"0\" /></AngularMotorTargetVelocity><AngularMotorMaxForce><Vector3 Version=\"1\" X=\"0.1\" Y=\"0.1\" Z=\"0.1\" /></AngularMotorMaxForce><AngularMotorDamping><Vector3 Version=\"1\" X=\"1\" Y=\"1\" Z=\"1\" /></AngularMotorDamping><AngularMotorRestitution><Vector3 Version=\"1\" X=\"0\" Y=\"0\" Z=\"0\" /></AngularMotorRestitution><AngularMotorEnabled><Vector3 Version=\"1\" X=\"0\" Y=\"0\" Z=\"0\" /></AngularMotorEnabled><LinearMotorMaxForce><Vector3 Version=\"1\" X=\"0\" Y=\"0\" Z=\"0\" /></LinearMotorMaxForce><LinearMotorTargetVelocity><Vector3 Version=\"1\" X=\"0\" Y=\"0\" Z=\"0\" /></LinearMotorTargetVelocity><LinearMotorEnabled><Vector3 Version=\"1\" X=\"0\" Y=\"0\" Z=\"0\" /></LinearMotorEnabled><DualTransformConstraint Version=\"1\"><ActorA><Transform Version=\"1\"><Vector3 Version=\"1\" X=\"1\" Y=\"2\" Z=\"3\" /><Quaternion Version=\"1\" X=\"0\" Y=\"0.928477\" Z=\"0\" W=\"0.371391\" /></Transform></ActorA><ActorB><Transform Version=\"1\"><Vector3 Version=\"1\" X=\"5\" Y=\"6\" Z=\"7\" /><Quaternion Version=\"1\" X=\"0.780869\" Y=\"0\" Z=\"0\" W=\"0.624695\" /></Transform></ActorB><TypedConstraint Version=\"1\" ActorNameA=\"RobotA\" ActorNameB=\"RobotB\" /></DualTransformConstraint></Generic6DofConstraint><SpringStiffness Axis0=\"0\" Axis1=\"0\" Axis2=\"0\" Axis3=\"0\" Axis4=\"0\" Axis5=\"0\" /><SpringDamping Axis0=\"1\" Axis1=\"1\" Axis2=\"1\" Axis3=\"1\" Axis4=\"1\" Axis5=\"1\" /><SpringEnabled Axis0=\"false\" Axis1=\"false\" Axis2=\"false\" Axis3=\"false\" Axis4=\"false\" Axis5=\"false\" /></Generic6DofSpringConstraint>");
                if ( G6dofSpringTest1 == SerializeTestG6dofSpring1.str())
                {
                    AddTestResult("Generic6DofSpringConstraint::operator<< (Empty)", Success, UnitTestGroup::OverWrite);
                }else{
                    AddTestResult("Generic6DofSpringConstraint::operator<< (Empty)", Failed, UnitTestGroup::OverWrite);
                }

                String G6dofSpringTest2("<Generic6DofSpringConstraint Version=\"1\"><Generic6DofConstraint Version=\"1\" LinearLimitSoftness=\"0.8\" LinearLimitDamping=\"25\" LinearLimitRestitution=\"0.9\"><LinearLimitUpper><Vector3 Version=\"1\" X=\"10\" Y=\"11\" Z=\"12\" /></LinearLimitUpper><LinearLimitLower><Vector3 Version=\"1\" X=\"-10\" Y=\"-11\" Z=\"-12\" /></LinearLimitLower><AngularLimitUpper><Vector3 Version=\"1\" X=\"0.1\" Y=\"0.2\" Z=\"0.3\" /></AngularLimitUpper><AngularLimitLower><Vector3 Version=\"1\" X=\"-0.1\" Y=\"-0.2\" Z=\"-0.3\" /></AngularLimitLower><AngularLimitMaxForce><Vector3 Version=\"1\" X=\"13\" Y=\"14\" Z=\"15\" /></AngularLimitMaxForce><AngularMotorTargetVelocity><Vector3 Version=\"1\" X=\"16\" Y=\"17\" Z=\"18\" /></AngularMotorTargetVelocity><AngularMotorMaxForce><Vector3 Version=\"1\" X=\"19\" Y=\"20\" Z=\"21\" /></AngularMotorMaxForce><AngularMotorDamping><Vector3 Version=\"1\" X=\"22\" Y=\"23\" Z=\"24\" /></AngularMotorDamping><AngularMotorRestitution><Vector3 Version=\"1\" X=\"0.01\" Y=\"0.02\" Z=\"0.03\" /></AngularMotorRestitution><AngularMotorEnabled><Vector3 Version=\"1\" X=\"0\" Y=\"1\" Z=\"0\" /></AngularMotorEnabled><LinearMotorMaxForce><Vector3 Version=\"1\" X=\"26\" Y=\"27\" Z=\"28\" /></LinearMotorMaxForce><LinearMotorTargetVelocity><Vector3 Version=\"1\" X=\"29\" Y=\"30\" Z=\"31\" /></LinearMotorTargetVelocity><LinearMotorEnabled><Vector3 Version=\"1\" X=\"1\" Y=\"1\" Z=\"0\" /></LinearMotorEnabled><DualTransformConstraint Version=\"1\"><ActorA><Transform Version=\"1\"><Vector3 Version=\"1\" X=\"1\" Y=\"2\" Z=\"3\" /><Quaternion Version=\"1\" X=\"0\" Y=\"0.928477\" Z=\"0\" W=\"0.371391\" /></Transform></ActorA><ActorB><Transform Version=\"1\"><Vector3 Version=\"1\" X=\"5\" Y=\"6\" Z=\"7\" /><Quaternion Version=\"1\" X=\"0.780869\" Y=\"0\" Z=\"0\" W=\"0.624695\" /></Transform></ActorB><TypedConstraint Version=\"1\" ActorNameA=\"RobotA\" ActorNameB=\"RobotB\"><Axis0 Con_Stop_ERP=\"0.01\" Con_CFM=\"0.02\" Con_Stop_CFM=\"0.03\" /><Axis1 Con_Stop_ERP=\"0.04\" Con_CFM=\"0.05\" Con_Stop_CFM=\"0.06\" /><Axis2 Con_Stop_ERP=\"0.07\" Con_CFM=\"0.08\" Con_Stop_CFM=\"0.09\" /><Axis3 Con_Stop_ERP=\"0.1\" Con_CFM=\"0.11\" Con_Stop_CFM=\"0.12\" /><Axis4 Con_Stop_ERP=\"0.13\" Con_CFM=\"0.14\" Con_Stop_CFM=\"0.15\" /><Axis5 Con_Stop_ERP=\"0.16\" Con_CFM=\"0.17\" Con_Stop_CFM=\"0.18\" /></TypedConstraint></DualTransformConstraint></Generic6DofConstraint><SpringStiffness Axis0=\"0.51\" Axis1=\"0.52\" Axis2=\"0.53\" Axis3=\"0.54\" Axis4=\"0.55\" Axis5=\"0.56\" /><SpringDamping Axis0=\"0.61\" Axis1=\"0.62\" Axis2=\"0.63\" Axis3=\"0.64\" Axis4=\"0.65\" Axis5=\"0.66\" /><SpringEnabled Axis0=\"true\" Axis1=\"true\" Axis2=\"false\" Axis3=\"false\" Axis4=\"true\" Axis5=\"false\" /></Generic6DofSpringConstraint>");
                if ( G6dofSpringTest2 == SerializeTestG6dofSpring2.str())
                {
                    AddTestResult("Generic6DofSpringConstraint::operator<< (Values)", Success, UnitTestGroup::OverWrite);
                }else{
                    AddTestResult("Generic6DofSpringConstraint::operator<< (Values)", Failed, UnitTestGroup::OverWrite);
                }

                //cout <<endl <<endl << "Expected: \t"<< G6dofSpringTest2 << endl << endl << "Recieved: \t"<< SerializeTestG6dofSpring2RS.str() <<endl <<endl;
                if ( G6dofSpringTest1 == SerializeTestG6dofSpring1RS.str())
                {
                    AddTestResult("Generic6DofSpringConstraint::operator>> (Empty)", Success, UnitTestGroup::OverWrite);
                }else{
                    AddTestResult("Generic6DofSpringConstraint::operator>> (Empty)", Failed, UnitTestGroup::OverWrite);
                }

                if ( G6dofSpringTest2 == SerializeTestG6dofSpring2RS.str())
                {
                    AddTestResult("Generic6DofSpringConstraint::operator>> (Values)", Success, UnitTestGroup::OverWrite);
                }else{
                    AddTestResult("Generic6DofSpringConstraint::operator>> (Values)", Failed, UnitTestGroup::OverWrite);
                }
                StopEngine();

            }else{
                AddTestResult("Point2PointConstraint::operator<< (Params-Empty)", Skipped);
                AddTestResult("Point2PointConstraint::operator<< (Params-1Param)", Skipped);
                AddTestResult("Point2PointConstraint::operator<< (Params-WithParams)", Skipped);
                AddTestResult("Point2PointConstraint::operator>> (Params-Empty)", Skipped);
                AddTestResult("Point2PointConstraint::operator>> (Params-1Param)", Skipped);
                AddTestResult("Point2PointConstraint::operator>> (Params-WithParams)", Skipped);

                AddTestResult("HingeConstraint::operator<< (Params-Empty)", Skipped);
                AddTestResult("HingeConstraint::operator<< (Params-1Param)", Skipped);
                AddTestResult("HingeConstraint::operator<< (Params-WithParams)", Skipped);
                AddTestResult("HingeConstraint::operator>> (Params-Empty)", Skipped);
                AddTestResult("HingeConstraint::operator>> (Params-1Param)", Skipped);
                AddTestResult("HingeConstraint::operator>> (Params-WithParams)", Skipped);

                AddTestResult("Point2PointConstraint::HasParamBeenSet-UnsetCFM", Skipped);
                AddTestResult("Point2PointConstraint::HasParamBeenSet-SetCFM", Skipped);
                AddTestResult("Point2PointConstraint::HasParamBeenSet-UnsetERP", Skipped);
                AddTestResult("Point2PointConstraint::HasParamBeenSet-SetERP", Skipped);
                AddTestResult("Point2PointConstraint::HasParamBeenSet-UnsetStopCFM", Skipped);
                AddTestResult("Point2PointConstraint::HasParamBeenSet-SetStopCFM", Skipped);
                AddTestResult("Point2PointConstraint::HasParamBeenSet-UnsetStopERP", Skipped);
                AddTestResult("Point2PointConstraint::HasParamBeenSet-SetStopERP", Skipped);

                AddTestResult("Generic6DofConstraint::operator<< (Empty)", Skipped);
                AddTestResult("Generic6DofConstraint::operator<< (Values)", Skipped);
                AddTestResult("Generic6DofConstraint::operator<< (ParamsAndValues)", Skipped);
                AddTestResult("Generic6DofConstraint::operator>> (Empty)", Skipped);
                AddTestResult("Generic6DofConstraint::operator>> (Values)", Skipped);
                AddTestResult("Generic6DofConstraint::operator>> (ParamsAndValues)", Skipped);

                AddTestResult("Generic6DofSpringConstraint::operator<< (Empty)", Skipped);
                AddTestResult("Generic6DofSpringConstraint::operator<< (Values)", Skipped);
                AddTestResult("Generic6DofSpringConstraint::operator>> (Empty)", Skipped);
                AddTestResult("Generic6DofSpringConstraint::operator>> (Values)", Skipped);

            }


        }
};
#endif

