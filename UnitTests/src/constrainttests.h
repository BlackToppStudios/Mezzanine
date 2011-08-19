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
                AddTestResult("Point2PointConstraint::operator<< (Point2PointConstraint-Empty)", Unknown);
                AddTestResult("Point2PointConstraint::operator<< (Point2PointConstraint-1Param)", Unknown);
                AddTestResult("Point2PointConstraint::operator<< (Point2PointConstraint-WithParams)", Unknown);

                AddTestResult("Point2PointConstraint::operator>> (Point2PointConstraint-Empty)", Unknown);
                AddTestResult("Point2PointConstraint::operator>> (Point2PointConstraint-1Param)", Unknown);
                AddTestResult("Point2PointConstraint::operator>> (Point2PointConstraint-WithParams)", Unknown);

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
                TheWorld->GetResourceManager()->AddResourceLocation(crossplatform::GetDataDirectory(), "FileSystem", groupname, false);

                ActorRigid *ActorA = new ActorRigid (20.0,"RobotA",filerobot,groupname);
                ActorA->GetPhysicsSettings();//->SetCollisionShape();
                ActorA->GetPhysicsSettings()->SetCollisionShape( TheWorld->GetCollisionShapeManager()->GenerateConvexHull("ABasicRobotShape",filerobot,groupname) );
                ActorA->SetLocation(Vector3(100,0,0));

                ActorRigid *ActorB = new ActorRigid (20.0,"RobotB",filerobot,groupname);
                ActorB->GetPhysicsSettings()->SetCollisionShape( TheWorld->GetCollisionShapeManager()->GenerateConvexHull("ABasicRobotShape",filerobot,groupname) );
                ActorB->SetLocation(Vector3(110,0,0));

                Point2PointConstraint Testee( ActorA, ActorB, Vector3(0.0,0.0,5.0), Vector3(0.0,5.0,0.0) );

                // make some places to store results
                std::stringstream SerializeTestP2PEmpty, SerializeTestP2PWithOneParam, SerializeTestP2PWithAllParams;
                bool SetCFM=false,      SetERP=false,   SetStopCFM=false,   SetStopERP=false;       // Should store True on successful test
                bool UnsetCFM=true,     UnsetERP=true,  UnsetStopCFM=false, UnsetStopERP=false;     // Should store False on successful test



                SerializeTestP2PEmpty << Testee;    // Serialize Test1

                UnsetCFM = Testee.HasParamBeenSet(Con_CFM,-1);          // All of the Unset test
                UnsetStopCFM = Testee.HasParamBeenSet(Con_Stop_CFM,-1);
                UnsetERP = Testee.HasParamBeenSet(Con_ERP,-1);
                UnsetStopERP = Testee.HasParamBeenSet(Con_Stop_ERP,-1);


                Testee.SetParam(Con_CFM,0.1,-1);             // Serialize Test2
                SerializeTestP2PWithOneParam << Testee;

                Testee.SetParam(Con_ERP,0.2,-1);             // Serialize Test3
                SerializeTestP2PWithAllParams << Testee;

                SetCFM = Testee.HasParamBeenSet(Con_CFM,-1);          // All of the Set test
                SetStopCFM = Testee.HasParamBeenSet(Con_Stop_CFM,-1);
                SetERP = Testee.HasParamBeenSet(Con_ERP,-1);
                SetStopERP = Testee.HasParamBeenSet(Con_Stop_ERP,-1);


                cout << std::endl << std::endl << Testee << std::endl << std::endl;



                //Check Test results
                String Test1("<Point2PointConstraint Version=\"1\" Tau=\"0.3\" ImpulseClamping=\"0\"><ActorA Name=\"RobotA\"><Vector3 Version=\"1\" X=\"0\" Y=\"0\" Z=\"5\" /></ActorA><ActorB Name=\"RobotB\"><Vector3 Version=\"1\" X=\"0\" Y=\"5\" Z=\"0\" /></ActorB><TypedConstraint Version=\"1\" ActorNameA=\"RobotA\" ActorNameB=\"RobotB\" /></Point2PointConstraint>");
                if ( Test1 == SerializeTestP2PEmpty.str())
                {
                    AddTestResult("Point2PointConstraint::operator<< (Point2PointConstraint-Empty)", Success, UnitTestGroup::OverWrite);
                }else{
                    AddTestResult("Point2PointConstraint::operator<< (Point2PointConstraint-Empty)", Failed, UnitTestGroup::OverWrite);
                }

                String Test2("<Point2PointConstraint Version=\"1\" Tau=\"0.3\" ImpulseClamping=\"0\"><ActorA Name=\"RobotA\"><Vector3 Version=\"1\" X=\"0\" Y=\"0\" Z=\"5\" /></ActorA><ActorB Name=\"RobotB\"><Vector3 Version=\"1\" X=\"0\" Y=\"5\" Z=\"0\" /></ActorB><TypedConstraint Version=\"1\" ActorNameA=\"RobotA\" ActorNameB=\"RobotB\"><Axis-1 Con_CFM=\"0.1\" Con_Stop_CFM=\"0.1\" /></TypedConstraint></Point2PointConstraint>");
                if (Test2 == SerializeTestP2PWithOneParam.str())
                {
                    AddTestResult("Point2PointConstraint::operator<< (Point2PointConstraint-1Param)", Success, UnitTestGroup::OverWrite);
                }else{
                    AddTestResult("Point2PointConstraint::operator<< (Point2PointConstraint-1Param)", Failed, UnitTestGroup::OverWrite);
                }

                String Test3("<Point2PointConstraint Version=\"1\" Tau=\"0.3\" ImpulseClamping=\"0\"><ActorA Name=\"RobotA\"><Vector3 Version=\"1\" X=\"0\" Y=\"0\" Z=\"5\" /></ActorA><ActorB Name=\"RobotB\"><Vector3 Version=\"1\" X=\"0\" Y=\"5\" Z=\"0\" /></ActorB><TypedConstraint Version=\"1\" ActorNameA=\"RobotA\" ActorNameB=\"RobotB\"><Axis-1 Con_ERP=\"0.2\" Con_Stop_ERP=\"0.2\" Con_CFM=\"0.1\" Con_Stop_CFM=\"0.1\" /></TypedConstraint></Point2PointConstraint>");
                if (Test3 == SerializeTestP2PWithAllParams.str())
                {
                    AddTestResult("Point2PointConstraint::operator<< (Point2PointConstraint-WithParams)", Success, UnitTestGroup::OverWrite);
                }else{
                    AddTestResult("Point2PointConstraint::operator<< (Point2PointConstraint-WithParams)", Failed, UnitTestGroup::OverWrite);
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
                StopEngine();

            }else{
                AddTestResult("Point2PointConstraint::operator<< (Point2PointConstraint-Empty)", Skipped);
                AddTestResult("Point2PointConstraint::operator<< (Point2PointConstraint-1Param)", Skipped);
                AddTestResult("Point2PointConstraint::operator<< (Point2PointConstraint-WithParams)", Skipped);
                AddTestResult("Point2PointConstraint::operator>> (Point2PointConstraint-Empty)", Skipped);
                AddTestResult("Point2PointConstraint::operator>> (Point2PointConstraint-1Param)", Skipped);
                AddTestResult("Point2PointConstraint::operator>> (Point2PointConstraint-WithParams)", Skipped);

                AddTestResult("Point2PointConstraint::HasParamBeenSet-UnsetCFM", Skipped);
                AddTestResult("Point2PointConstraint::HasParamBeenSet-SetCFM", Skipped);
                AddTestResult("Point2PointConstraint::HasParamBeenSet-UnsetERP", Skipped);
                AddTestResult("Point2PointConstraint::HasParamBeenSet-SetERP", Skipped);
                AddTestResult("Point2PointConstraint::HasParamBeenSet-UnsetStopCFM", Skipped);
                AddTestResult("Point2PointConstraint::HasParamBeenSet-SetStopCFM", Skipped);
                AddTestResult("Point2PointConstraint::HasParamBeenSet-UnsetStopERP", Skipped);
                AddTestResult("Point2PointConstraint::HasParamBeenSet-SetStopERP", Skipped);
            }


        }
};
#endif

