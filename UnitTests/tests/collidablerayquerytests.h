// © Copyright 2010 - 2017 BlackTopp Studios Inc.
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
#ifndef _collidablerayquerytests_h
#define _collidablerayquerytests_h

#include "mezztest.h"

#include "entresol.h"
#include "world.h"
#include "Physics/physics.h"

#include <stdexcept> //only used to throw for TEST_THROW

/// @file
/// @brief Tests the functionality of performing a series of ray tests in the world of the Physics subsyetem.

using namespace Mezzanine;
using namespace Mezzanine::Testing;

/// @brief Test of the Mezzanine::Physics::CollidableRayQuery
class collidablerayquerytests : public UnitTestGroup
{
public:
    /// @copydoc Mezzanine::Testing::UnitTestGroup::Name
    /// @return Returns a String containing "CollidableRayQuery"
    virtual String Name()
        { return String("CollidableRayQuery"); }

    /// @brief Verifies the provided result matches expectations.
    /// @param Result The result given by the ray query.
    /// @param ExpectedProxy A pointer to the proxy we expect to be in the result.
    /// @param ExpectedDistance The distance we expect the proxy to be at.
    /// @return Returns true if the result contains the values we expect, false otherwise.
    Boole VerifyResult(const RayQueryHit& Result, EntityProxy* ExpectedProxy, Real ExpectedDistance)
    {
        Boole ObjectMatch = ( Result.Object == ExpectedProxy );
        Boole DistanceMatch = ( Result.Distance < ExpectedDistance + 0.00001 ) && ( Result.Distance > ExpectedDistance - 0.00001 );
        //Boole DistanceMatch = CompareEqualityWithEpsilon(Result.Distance,ExpectedDistance,5);
        return ( ObjectMatch && DistanceMatch );
    }

    /// @brief This is called when Automatic tests are run
    void RunAutomaticTests()
    {
        Entresol TheEntresol;
        World* TheWorld = TheEntresol.CreateWorld("CollidableRayTestWorld");
        Physics::PhysicsManager* PhysMan = static_cast<Physics::PhysicsManager*>( TheWorld->GetManager(ManagerBase::MT_PhysicsManager) );

        Physics::BoxCollisionShape* TestBox = new Physics::BoxCollisionShape("TestBox",Vector3(5.0,5.0,5.0));
        Physics::BoxCollisionShape* DoubleScaleTestBox = new Physics::BoxCollisionShape("DoubleScaleTestBox",Vector3(5.0,5.0,5.0));
        Physics::BoxCollisionShape* HalfScaleTestBox = new Physics::BoxCollisionShape("HalfScaleTestBox",Vector3(5.0,5.0,5.0));
        Physics::SphereCollisionShape* TestSphere = new Physics::SphereCollisionShape("TestSphere",5.0);
        Physics::SphereCollisionShape* DoubleScaleTestSphere = new Physics::SphereCollisionShape("DoubleScaleTestSphere",5.0);
        Whole Mass = 1.0;

        // Create entities.  Descriptions are relative to default camera perspective.
        Physics::RigidProxy* Rigid01 = PhysMan->CreateRigidProxy(Mass,TestBox);
        Rigid01->SetLocation(25.0,0.0,0.0);// 25 units to the left.
        Rigid01->AddToWorld();
        Physics::RigidProxy* Rigid02 = PhysMan->CreateRigidProxy(Mass,TestBox);
        Rigid02->SetLocation(-25.0,0.0,0.0);// 25 units to the right.
        Rigid02->AddToWorld();
        Physics::RigidProxy* Rigid03 = PhysMan->CreateRigidProxy(Mass,TestBox);
        Rigid03->SetLocation(0.0,25.0,0.0);// 25 units above.
        Rigid03->AddToWorld();
        Physics::RigidProxy* Rigid04 = PhysMan->CreateRigidProxy(Mass,TestBox);
        Rigid04->SetLocation(0.0,0.0,-25.0);// 25 units in front of.
        Rigid04->AddToWorld();
        Physics::RigidProxy* Rigid05 = PhysMan->CreateRigidProxy(Mass,DoubleScaleTestBox);
        Rigid05->SetLocation(0.0,-7.5,-50.0);// 50 units in front of and 7.5 units down.
        Rigid05->SetScale(2.0,2.0,2.0);
        Rigid05->AddToWorld();
        Physics::RigidProxy* Rigid06 = PhysMan->CreateRigidProxy(Mass,HalfScaleTestBox);
        Rigid06->SetLocation(0.0,3.0,-75.0);// 75 units in front of and 3 units up.
        Rigid06->SetScale(0.5,0.5,0.5);
        Rigid06->AddToWorld();
        Physics::RigidProxy* Rigid07 = PhysMan->CreateRigidProxy(Mass,TestBox);
        Rigid07->SetLocation(50.0,50.0,50.0);// 50 units to the left, up, and back.
        Rigid07->AddToWorld();
        Physics::RigidProxy* Rigid08 = PhysMan->CreateRigidProxy(Mass,TestSphere);
        Rigid08->SetLocation(-50.0,-50.0,-50.0);// 50 units to the right, down, and front.
        Rigid08->AddToWorld();
        Physics::RigidProxy* Rigid09 = PhysMan->CreateRigidProxy(Mass,TestSphere);
        Rigid09->SetLocation(-50.0,50.0,-50.0);// 50 units to the right, up, and front.
        Rigid09->AddToWorld();
        Physics::RigidProxy* Rigid10 = PhysMan->CreateRigidProxy(Mass,TestSphere);
        Rigid10->SetLocation(4.0,4.0,25.0);// 4 units to the left and up, and 25 units behind.
        Rigid10->AddToWorld();
        Physics::RigidProxy* Rigid11 = PhysMan->CreateRigidProxy(Mass,TestSphere);
        Rigid11->SetLocation(-4.0,-4.0,50.0);// 4 units to the right and down, and 50 units behind.
        Rigid11->AddToWorld();
        Physics::RigidProxy* Rigid12 = PhysMan->CreateRigidProxy(Mass,DoubleScaleTestSphere);
        Rigid12->SetLocation(5.0,100.0,5.0);// 5 units to the left and back, and 100 units up.
        Rigid12->SetScale(2.0,2.0,2.0);
        Rigid12->AddToWorld();

        Physics::GhostProxy* Ghost = PhysMan->CreateGhostProxy(TestSphere);
        Ghost->SetLocation(75.0,0.0,0.0);
        Ghost->AddToWorld();

        Physics::CollidableRayQuery TestRayQuery(PhysMan);

        Vector3 ZeroVec(0.0,0.0,0.0);
        {//AABB Tests
            {//Test 1
                Ray AABBTestRay(ZeroVec,Vector3::Unit_X());
                RayQuery::ResultContainer TestResults = TestRayQuery.GetAllAABBResults(AABBTestRay);
                TEST( TestResults.size() == 2 &&
                      this->VerifyResult( TestResults[0], Rigid01, 20.0 ) &&
                      this->VerifyResult( TestResults[1], Ghost, 70.0 ),
                      "AABB_Unit_X_RayTest" );
            }//Test 1

            {//Test 2
                Ray AABBTestRay(ZeroVec,Vector3::Neg_Unit_X());
                RayQuery::ResultContainer TestResults = TestRayQuery.GetAllAABBResults(AABBTestRay);
                TEST( TestResults.size() == 1 &&
                      this->VerifyResult( TestResults[0], Rigid02, 20.0 ),
                      "AABB_Neg_Unit_X_RayTest" );
            }//Test 2

            {//Test 3
                Ray AABBTestRay(ZeroVec,Vector3::Unit_Y());
                RayQuery::ResultContainer TestResults = TestRayQuery.GetAllAABBResults(AABBTestRay);
                TEST( TestResults.size() == 2 &&
                      this->VerifyResult( TestResults[0], Rigid03, 20.0 ) &&
                      this->VerifyResult( TestResults[1], Rigid12, 90.0 ),
                      "AABB_Unit_Y_RayTest" );
            }//Test 3

            {//Test 4
                Ray AABBTestRay(ZeroVec,Vector3::Neg_Unit_Y());
                RayQuery::ResultContainer TestResults = TestRayQuery.GetAllAABBResults(AABBTestRay);
                TEST( TestResults.size() == 0,
                      "AABB_Neg_Unit_Y_RayTest" );
            }//Test 4

            {//Test 5
                Ray AABBTestRay(ZeroVec,Vector3::Unit_Z());
                RayQuery::ResultContainer TestResults = TestRayQuery.GetAllAABBResults(AABBTestRay);
                TEST( TestResults.size() == 2 &&
                      this->VerifyResult( TestResults[0], Rigid10, 20.0 ) &&
                      this->VerifyResult( TestResults[1], Rigid11, 45.0 ),
                      "AABB_Unit_Z_RayTest_MultiHit" );
            }//Test 5

            {//Test 6
                Ray AABBTestRay(ZeroVec,Vector3::Unit_Z());
                RayQueryHit TestResult = TestRayQuery.GetFirstAABBResult(AABBTestRay);
                TEST( this->VerifyResult( TestResult, Rigid10, 20.0 ),
                      "AABB_Unit_Z_RayTest_SingleHit" );
            }//Test 6

            {//Test 7
                Ray AABBTestRay(ZeroVec,Vector3::Neg_Unit_Z());
                RayQuery::ResultContainer TestResults = TestRayQuery.GetAllAABBResults(AABBTestRay);
                TEST( TestResults.size() == 2 &&
                      this->VerifyResult( TestResults[0], Rigid04, 20.0 ) &&
                      this->VerifyResult( TestResults[1], Rigid05, 40.0 ),
                      "AABB_Neg_Unit_Z_RayTest_MultiHit" );
            }//Test 7

            {//Test 8
                Ray AABBTestRay(ZeroVec,Vector3::Neg_Unit_Z());
                RayQueryHit TestResult = TestRayQuery.GetFirstAABBResult(AABBTestRay);
                TEST( this->VerifyResult( TestResult, Rigid04, 20.0 ),
                      "AABB_Neg_Unit_Z_RayTest_SingleHit" );
            }//Test 8

            {//Test 9
                Ray AABBTestRay(ZeroVec,Vector3(1.0,1.0,1.0).Normalize());
                RayQuery::ResultContainer TestResults = TestRayQuery.GetAllAABBResults(AABBTestRay);
                TEST( TestResults.size() == 1 &&
                      this->VerifyResult( TestResults[0], Rigid07, 77.9422913 ),
                      "AABB_Unit_X_Y_Z_RayTest" );
            }//Test 9
        }//AABB Tests

        {//Shape Tests
            {//Test 1
                Ray ShapeTestRay(ZeroVec,Vector3::Unit_X());
                RayQuery::ResultContainer TestResults = TestRayQuery.GetAllShapeResults(ShapeTestRay);
                TEST( TestResults.size() == 2 &&
                      this->VerifyResult( TestResults[0], Rigid01, 20.0 ) &&
                      this->VerifyResult( TestResults[1], Ghost, 70.0 ),
                      "Shape_Unit_X_RayTest" );
            }//Test 1

            {//Test 2
                Ray ShapeTestRay(ZeroVec,Vector3::Neg_Unit_X());
                RayQuery::ResultContainer TestResults = TestRayQuery.GetAllShapeResults(ShapeTestRay);
                TEST( TestResults.size() == 1 &&
                      this->VerifyResult( TestResults[0], Rigid02, 20.0 ),
                      "Shape_Neg_Unit_X_RayTest" );
            }//Test 2

            {//Test 3
                Ray ShapeTestRay(ZeroVec,Vector3::Unit_Y());
                RayQuery::ResultContainer TestResults = TestRayQuery.GetAllShapeResults(ShapeTestRay);
                TEST( TestResults.size() == 2 &&
                      this->VerifyResult( TestResults[0], Rigid03, 20.0 ) &&
                      this->VerifyResult( TestResults[1], Rigid12, 92.9289246 ),
                      "Shape_Unit_Y_RayTest" );
            }//Test 3

            {//Test 4
                Ray ShapeTestRay(ZeroVec,Vector3::Neg_Unit_Y());
                RayQuery::ResultContainer TestResults = TestRayQuery.GetAllShapeResults(ShapeTestRay);
                TEST( TestResults.size() == 0,
                      "Shape_Neg_Unit_Y_RayTest" );
            }//Test 4

            {//Test 5
                Ray ShapeTestRay(ZeroVec,Vector3::Unit_Z());
                RayQuery::ResultContainer TestResults = TestRayQuery.GetAllShapeResults(ShapeTestRay);
                TEST( TestResults.size() == 0,
                      "Shape_Unit_Z_RayTest_MultiHit" );
            }//Test 5

            {//Test 6
                Ray ShapeTestRay(ZeroVec,Vector3::Unit_Z());
                RayQueryHit TestResult = TestRayQuery.GetFirstShapeResult(ShapeTestRay);
                TEST( !TestResult.IsValid(),
                      "Shape_Unit_Z_RayTest_SingleHit" );
            }//Test 6

            {//Test 7
                Ray ShapeTestRay(ZeroVec,Vector3::Neg_Unit_Z());
                RayQuery::ResultContainer TestResults = TestRayQuery.GetAllShapeResults(ShapeTestRay);
                TEST( TestResults.size() == 2 &&
                      this->VerifyResult( TestResults[0], Rigid04, 20.0 ) &&
                      this->VerifyResult( TestResults[1], Rigid05, 40.0000038 ),// <- This was annoying, 10 epsilons of tolerance couldn't account for this.
                      "Shape_Neg_Unit_Z_RayTest_MultiHit" );
            }//Test 7

            {//Test 8
                Ray ShapeTestRay(ZeroVec,Vector3::Neg_Unit_Z());
                RayQueryHit TestResult = TestRayQuery.GetFirstShapeResult(ShapeTestRay);
                TEST( this->VerifyResult( TestResult, Rigid04, 20.0 ),
                      "Shape_Neg_Unit_Z_RayTest_SingleHit" );
            }//Test 8

            {//Test 9
                Ray ShapeTestRay(ZeroVec,Vector3(1.0,1.0,1.0).Normalize());
                RayQuery::ResultContainer TestResults = TestRayQuery.GetAllShapeResults(ShapeTestRay);
                TEST( TestResults.size() == 1 &&
                      this->VerifyResult( TestResults[0], Rigid07, 77.9422836 ),// <- This was the number I math'd in advance for the AABB check.  I suspect the bullet margin is factoring in.
                      "Shape_Unit_X_Y_Z_RayTest" );
            }//Test 9
        }//Shape Tests

        {//Utility Tests
            {//Get/SetTypes Test
                RayQuery::FilterFunction ProxFilter = [](EntityProxy* ToFilter) {
                    return ( ToFilter->GetComponentType() == Mezzanine::CT_Physics_GhostProxy );
                };
                TestRayQuery.SetFilterFunction(ProxFilter);
                Ray FilterTestRay(ZeroVec,Vector3::Unit_X());

                RayQuery::ResultContainer MultiTestResult = TestRayQuery.GetAllShapeResults(FilterTestRay);
                TEST( MultiTestResult.size() == 1 &&
                      this->VerifyResult( MultiTestResult[0], Ghost, 70.0 ),
                      "SetFilterFunction(const_FilterFunction)_Multi" );

                RayQueryHit SingleTestResult = TestRayQuery.GetFirstShapeResult(FilterTestRay);
                TEST( this->VerifyResult( SingleTestResult, Ghost, 70.0 ),
                      "SetFilterFunction(const_FilterFunction)_Single" );

                TestRayQuery.SetFilterFunction(RayQuery::FilterFunction());
            }//Get/SetTypes Test //*/

            {//Get/SetQuery Test
                UInt32 QueryFilter = Physics::CF_SensorFilter;
                TestRayQuery.SetQueryFilter(QueryFilter);
                Ray FilterTestRay(ZeroVec,Vector3::Unit_X());

                RayQuery::ResultContainer MultiTestResult = TestRayQuery.GetAllShapeResults(FilterTestRay);
                TEST( MultiTestResult.size() == 1 &&
                      this->VerifyResult( MultiTestResult[0], Ghost, 70.0 ),
                      "SetQueryFilter(const_UInt32)_Multi" );

                RayQueryHit SingleTestResult = TestRayQuery.GetFirstShapeResult(FilterTestRay);
                TEST( this->VerifyResult( SingleTestResult, Ghost, 70.0 ),
                      "SetQueryFilter(const_UInt32)_Single" );

                TEST( TestRayQuery.GetQueryFilter() == QueryFilter,
                      "GetQueryFilter()_const" );

                TestRayQuery.SetQueryFilter(std::numeric_limits<UInt32>::max());
            }//Get/SetQuery Test
        }//Utility Tests

        {//Serialize Test
            String Expected( "<?xml version=\"1.0\"?><CollidableRayQuery Version=\"1\" WorldName=\"CollidableRayTestWorld\" RayCastLength=\"15000\" QueryFilter=\"4294967295\" />" );

            XML::Document Doc;
            Physics::CollidableRayQuery TestRayQuery(PhysMan);
            TestRayQuery.ProtoSerialize(Doc);
            StringStream Buffer;
            Doc.Save(Buffer);

            TEST( Expected == String( Buffer.str() ) ,"ProtoSerialize(XML::Node&)_const");
        }//Serialize Test

        {//Deserialize Test
            String Source( "<?xml version=\"1.0\"?><CollidableRayQuery Version=\"1\" WorldName=\"CollidableRayTestWorld\" RayCastLength=\"15000\" QueryFilter=\"4294967295\" />" );

            XML::Document Doc;
            StringStream Buffer;
            Buffer.str(Source);
            Doc.Load(Buffer);

            Physics::CollidableRayQuery TestRayQuery(NULL);
            TestRayQuery.ProtoDeSerialize(Doc.GetFirstChild());

            Boole WorldMatch = TestRayQuery.GetWorld() == TheWorld;
            Boole CollisionFilterMatch = TestRayQuery.GetQueryFilter() == std::numeric_limits<UInt32>::max();
            TEST( WorldMatch && CollisionFilterMatch ,"ProtoDeSerialize(const_XML::Node&)");
        }//Deserialize Test
    }

    /// @brief Since RunAutomaticTests is implemented so is this.
    /// @return returns true
    virtual bool HasAutomaticTests() const
        { return true; }
};//collidableRayQuery

#endif
