// © Copyright 2010 - 2016 BlackTopp Studios Inc.
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
#ifndef _renderablerayquerytests_h
#define _renderablerayquerytests_h

#include "mezztest.h"

#include "entresol.h"
#include "world.h"
#include "Graphics/graphics.h"

#include "sstream"
#include <stdexcept> //only used to throw for TEST_THROW

/// @file
/// @brief Tests the functionality of performing a series of ray tests in the world of the Graphics subsyetem.

using namespace Mezzanine;
using namespace Mezzanine::Testing;

/// @brief Test of the Mezzanine::Graphics::RenderableRayQuery
class renderablerayquerytests : public UnitTestGroup
{
public:
    /// @copydoc Mezzanine::Testing::UnitTestGroup::Name
    /// @return Returns a String containing "RenderableRayQuery"
    virtual String Name()
        { return String("RenderableRayQuery"); }

    /// @brief Verifies the provided result matches expectations.
    /// @param Result The result given by the ray query.
    /// @param ExpectedProxy A pointer to the proxy we expect to be in the result.
    /// @param ExpectedDistance The distance we expect the proxy to be at.
    /// @return Returns true if the result contains the values we expect, false otherwise.
    Boole VerifyResult(const RayQueryHit& Result, WorldProxy* ExpectedProxy, Real ExpectedDistance)
    {
        /// @todo The distance comparison may need to be swapped with something that has more leeway than n-epsilons.
        Boole ObjectMatch = ( Result.Object == ExpectedProxy );
        Boole DistanceMatch = CompareEqualityWithEpsilon(Result.Distance,ExpectedDistance,5);
        return ( ObjectMatch && DistanceMatch );
    }

    /// @brief This is called when Automatic tests are run
    void RunAutomaticTests()
    {
        Entresol TheEntresol;
        World* TheWorld = TheEntresol.CreateWorld("RenderableRayTestWorld");
        Graphics::GraphicsManager* GraphicsMan = static_cast<Graphics::GraphicsManager*>( TheEntresol.GetManager(ManagerBase::MT_GraphicsManager) );
        Graphics::GameWindow* TestWindow = GraphicsMan->CreateGameWindow("Test",10,10,Graphics::GameWindow::WF_Hidden);
        Graphics::Viewport* TestViewport = TestWindow->CreateViewport(NULL,0);
        TheEntresol.Initialize(true);
        Graphics::SceneManager* SceneMan = static_cast<Graphics::SceneManager*>( TheWorld->GetManager(ManagerBase::MT_SceneManager) );

        Graphics::CameraProxy* TestCamera = SceneMan->CreateCamera();
        TestCamera->SetLocation(200.0,-200.0,200.0);
        TestCamera->LookAt(Vector3(0.0,0.0,0.0));
        TestViewport->SetCamera(TestCamera);

        Graphics::Procedural::BoxGenerator BoxGen(5.0,5.0,5.0);
        Graphics::Mesh* TestBox = BoxGen.GenerateMesh("TestBox","Internal","BaseWhiteNoLighting","Internal");
        Graphics::Procedural::SphereGenerator SphereGen(5.0);
        Graphics::Mesh* TestSphere = SphereGen.GenerateMesh("TestSphere","Internal","BaseWhiteNoLighting","Internal");

        // Create entities.  Descriptions are relative to default camera perspective.
        Graphics::EntityProxy* Entity01 = SceneMan->CreateEntityProxy(TestBox);
        Entity01->SetLocation(25.0,0.0,0.0);// 25 units to the left.
        Entity01->AddToWorld();
        Graphics::EntityProxy* Entity02 = SceneMan->CreateEntityProxy(TestBox);
        Entity02->SetLocation(-25.0,0.0,0.0);// 25 units to the right.
        Entity02->AddToWorld();
        Graphics::EntityProxy* Entity03 = SceneMan->CreateEntityProxy(TestBox);
        Entity03->SetLocation(0.0,25.0,0.0);// 25 units above.
        Entity03->AddToWorld();
        Graphics::EntityProxy* Entity04 = SceneMan->CreateEntityProxy(TestBox);
        Entity04->SetLocation(0.0,0.0,-25.0);// 25 units in front of.
        Entity04->AddToWorld();
        Graphics::EntityProxy* Entity05 = SceneMan->CreateEntityProxy(TestBox);
        Entity05->SetLocation(0.0,-7.5,-50.0);// 50 units in front of and 7.5 units down.
        Entity05->SetScale(2.0,2.0,2.0);
        Entity05->AddToWorld();
        Graphics::EntityProxy* Entity06 = SceneMan->CreateEntityProxy(TestBox);
        Entity06->SetLocation(0.0,3.0,-75.0);// 75 units in front of and 3 units up.
        Entity06->SetScale(0.5,0.5,0.5);
        Entity06->AddToWorld();
        Graphics::EntityProxy* Entity07 = SceneMan->CreateEntityProxy(TestBox);
        Entity07->SetLocation(50.0,50.0,50.0);// 50 units to the left, up, and back.
        Entity07->AddToWorld();
        Graphics::EntityProxy* Entity08 = SceneMan->CreateEntityProxy(TestSphere);
        Entity08->SetLocation(-50.0,-50.0,-50.0);// 50 units to the right, down, and front.
        Entity08->AddToWorld();
        Graphics::EntityProxy* Entity09 = SceneMan->CreateEntityProxy(TestSphere);
        Entity09->SetLocation(-50.0,50.0,-50.0);// 50 units to the right, up, and front.
        Entity09->AddToWorld();
        Graphics::EntityProxy* Entity10 = SceneMan->CreateEntityProxy(TestSphere);
        Entity10->SetLocation(4.0,4.0,25.0);// 4 units to the left and up, and 25 units behind.
        Entity10->AddToWorld();
        Graphics::EntityProxy* Entity11 = SceneMan->CreateEntityProxy(TestSphere);
        Entity11->SetLocation(-4.0,-4.0,50.0);// 4 units to the right and down, and 50 units behind.
        Entity11->AddToWorld();
        Graphics::EntityProxy* Entity12 = SceneMan->CreateEntityProxy(TestSphere);
        Entity12->SetLocation(5.0,100.0,5.0);// 5 units to the left and back, and 100 units up.
        Entity12->SetScale(2.0,2.0,2.0);
        Entity12->AddToWorld();

        Graphics::BillboardSetProxy* BBSet = SceneMan->CreateBillboardSetProxy(1);
        BBSet->SetLocation(75.0,0.0,0.0);
        BBSet->SetDefaultDimensions(5.0,5.0);
        BBSet->CreateBillboard(Vector3(0.0,0.0,0.0));
        BBSet->AddToWorld();

        GraphicsMan->RenderOneFrame();
        Graphics::RenderableRayQuery TestRayQuery(SceneMan);

        Vector3 ZeroVec(0.0,0.0,0.0);
        {//AABB Tests
            {//Test 1
                Ray AABBTestRay(ZeroVec,Vector3::Unit_X());
                RayQuery::ResultContainer TestResults = TestRayQuery.GetAllAABBResults(AABBTestRay);
                TEST( TestResults.size() == 2 &&
                      this->VerifyResult( TestResults[0], Entity01, 20.0 ) &&
                      this->VerifyResult( TestResults[1], BBSet, 70.0 ),
                      "AABB_Unit_X_RayTest" );
            }//Test 1

            {//Test 2
                Ray AABBTestRay(ZeroVec,Vector3::Neg_Unit_X());
                RayQuery::ResultContainer TestResults = TestRayQuery.GetAllAABBResults(AABBTestRay);
                TEST( TestResults.size() == 1 &&
                      this->VerifyResult( TestResults[0], Entity02, 20.0 ),
                      "AABB_Neg_Unit_X_RayTest" );
            }//Test 2

            {//Test 3
                Ray AABBTestRay(ZeroVec,Vector3::Unit_Y());
                RayQuery::ResultContainer TestResults = TestRayQuery.GetAllAABBResults(AABBTestRay);
                TEST( TestResults.size() == 2 &&
                      this->VerifyResult( TestResults[0], Entity03, 20.0 ) &&
                      this->VerifyResult( TestResults[1], Entity12, 90.0 ),
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
                      this->VerifyResult( TestResults[0], Entity10, 20.0 ) &&
                      this->VerifyResult( TestResults[1], Entity11, 45.0 ),
                      "AABB_Unit_Z_RayTest_MultiHit" );
            }//Test 5

            {//Test 6
                Ray AABBTestRay(ZeroVec,Vector3::Unit_Z());
                RayQueryHit TestResult = TestRayQuery.GetFirstAABBResult(AABBTestRay);
                TEST( this->VerifyResult( TestResult, Entity10, 20.0 ),
                      "AABB_Unit_Z_RayTest_SingleHit" );
            }//Test 6

            {//Test 7
                Ray AABBTestRay(ZeroVec,Vector3::Neg_Unit_Z());
                RayQuery::ResultContainer TestResults = TestRayQuery.GetAllAABBResults(AABBTestRay);
                TEST( TestResults.size() == 2 &&
                      this->VerifyResult( TestResults[0], Entity04, 20.0 ) &&
                      this->VerifyResult( TestResults[1], Entity05, 40.0 ),
                      "AABB_Neg_Unit_Z_RayTest_MultiHit" );
            }//Test 7

            {//Test 8
                Ray AABBTestRay(ZeroVec,Vector3::Neg_Unit_Z());
                RayQueryHit TestResult = TestRayQuery.GetFirstAABBResult(AABBTestRay);
                TEST( this->VerifyResult( TestResult, Entity04, 20.0 ),
                      "AABB_Neg_Unit_Z_RayTest_SingleHit" );
            }//Test 8

            {//Test 9
                Ray AABBTestRay(ZeroVec,Vector3(45.0,50.0,50.0).Normalize());//Setting 1,1,1 here doesn't work because Ogre's ray test method is more susceptible to precision loss than Bullet.
                RayQuery::ResultContainer TestResults = TestRayQuery.GetAllAABBResults(AABBTestRay);
                TEST( TestResults.size() == 1 &&
                      this->VerifyResult( TestResults[0], Entity07, 83.8152618 ),
                      "AABB_Unit_X_Y_Z_RayTest" );
            }//Test 9
        }//AABB Tests

        {//Shape (mesh) Tests
            {//Test 1
                Ray ShapeTestRay(ZeroVec,Vector3::Unit_X());
                RayQuery::ResultContainer TestResults = TestRayQuery.GetAllShapeResults(ShapeTestRay);
                TEST( TestResults.size() == 2 &&
                      this->VerifyResult( TestResults[0], Entity01, 20.0 ) &&
                      this->VerifyResult( TestResults[1], BBSet, 75.0 ),
                      "Shape_Unit_X_RayTest" );
            }//Test 1

            {//Test 2
                Ray ShapeTestRay(ZeroVec,Vector3::Neg_Unit_X());
                RayQuery::ResultContainer TestResults = TestRayQuery.GetAllShapeResults(ShapeTestRay);
                TEST( TestResults.size() == 1 &&
                      this->VerifyResult( TestResults[0], Entity02, 20.0 ),
                      "Shape_Neg_Unit_X_RayTest" );
            }//Test 2

            {//Test 3
                Ray ShapeTestRay(ZeroVec,Vector3::Unit_Y());
                RayQuery::ResultContainer TestResults = TestRayQuery.GetAllShapeResults(ShapeTestRay);
                TEST( TestResults.size() == 2 &&
                      this->VerifyResult( TestResults[0], Entity03, 20.0 ) &&
                      this->VerifyResult( TestResults[1], Entity12, 92.9289398 ),
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
                      this->VerifyResult( TestResults[0], Entity04, 20.0 ) &&
                      this->VerifyResult( TestResults[1], Entity05, 40.0 ),
                      "Shape_Neg_Unit_Z_RayTest_MultiHit" );
            }//Test 7

            {//Test 8
                Ray ShapeTestRay(ZeroVec,Vector3::Neg_Unit_Z());
                RayQueryHit TestResult = TestRayQuery.GetFirstShapeResult(ShapeTestRay);
                TEST( this->VerifyResult( TestResult, Entity04, 20.0 ),
                      "Shape_Neg_Unit_Z_RayTest_SingleHit" );
            }//Test 8

            {//Test 9
                Ray ShapeTestRay(ZeroVec,Vector3(45.0,50.0,50.0).Normalize());
                RayQuery::ResultContainer TestResults = TestRayQuery.GetAllShapeResults(ShapeTestRay);
                TEST( TestResults.size() == 1 &&
                      this->VerifyResult( TestResults[0], Entity07, 83.8152695 ),
                      "Shape_Unit_X_Y_Z_RayTest" );
            }//Test 9
        }//Shape (mesh) Tests

        {//Utility Tests
            {//Get/SetTypes Test
                UInt32 ProxFilter = Mezzanine::PT_Graphics_BillboardSetProxy;
                TestRayQuery.SetProxyTypes(ProxFilter);
                Ray FilterTestRay(ZeroVec,Vector3::Unit_X());

                RayQuery::ResultContainer MultiTestResult = TestRayQuery.GetAllShapeResults(FilterTestRay);
                TEST( MultiTestResult.size() == 1 &&
                      this->VerifyResult( MultiTestResult[0], BBSet, 75.0 ),
                      "SetProxyTypes(const_UInt32)_Multi" );

                RayQueryHit SingleTestResult = TestRayQuery.GetFirstShapeResult(FilterTestRay);
                TEST( this->VerifyResult( SingleTestResult, BBSet, 75.0 ),
                      "SetProxyTypes(const_UInt32)_Single" );

                TEST( TestRayQuery.GetProxyTypes() == ProxFilter,
                      "GetProxyTypes()_const" );

                TestRayQuery.SetProxyTypes(std::numeric_limits<UInt32>::max());
            }//Get/SetTypes Test

            {//Get/SetQuery Test
                UInt32 QueryFilter = 2;
                Entity01->SetQueryMask( Entity01->GetQueryMask() & ~QueryFilter );
                BBSet->SetQueryMask( QueryFilter );
                TestRayQuery.SetQueryFilter(QueryFilter);
                Ray FilterTestRay(ZeroVec,Vector3::Unit_X());

                RayQuery::ResultContainer MultiTestResult = TestRayQuery.GetAllShapeResults(FilterTestRay);
                TEST( MultiTestResult.size() == 1 &&
                      this->VerifyResult( MultiTestResult[0], BBSet, 75.0 ),
                      "SetQueryFilter(const_UInt32)_Multi" );

                RayQueryHit SingleTestResult = TestRayQuery.GetFirstShapeResult(FilterTestRay);
                TEST( this->VerifyResult( SingleTestResult, BBSet, 75.0 ),
                      "SetQueryFilter(const_UInt32)_Single" );

                TEST( TestRayQuery.GetQueryFilter() == QueryFilter,
                      "GetQueryFilter()_const" );

                TestRayQuery.SetQueryFilter(std::numeric_limits<UInt32>::max());
            }//Get/SetQuery Test
        }//Utility Tests

        {//Serialize Test
            String Expected( "<?xml version=\"1.0\"?><RenderableRayQuery Version=\"1\" WorldName=\"RenderableRayTestWorld\" ProxyTypesFilter=\"4294967295\" QueryFilter=\"4294967295\" />" );

            XML::Document Doc;
            Graphics::RenderableRayQuery TestRayQuery(SceneMan);
            TestRayQuery.ProtoSerialize(Doc);
            StringStream Buffer;
            Doc.Save(Buffer);

            TEST( Expected == String( Buffer.str() ) ,"ProtoSerialize(XML::Node&)_const");
        }//Serialize Test

        {//Deserialize Test
            String Source( "<?xml version=\"1.0\"?><RenderableRayQuery Version=\"1\" WorldName=\"RenderableRayTestWorld\" ProxyTypesFilter=\"4294967295\" QueryFilter=\"4294967295\" />" );

            XML::Document Doc;
            StringStream Buffer;
            Buffer.str(Source);
            Doc.Load(Buffer);

            Graphics::RenderableRayQuery TestRayQuery(NULL);// <- This is #Dicey
            TestRayQuery.ProtoDeSerialize(Doc.GetFirstChild());

            Boole WorldMatch = TestRayQuery.GetWorld() == TheWorld;
            Boole ProxyTypesMatch = TestRayQuery.GetProxyTypes() == std::numeric_limits<UInt32>::max();
            Boole QueryFilterMatch = TestRayQuery.GetQueryFilter() == std::numeric_limits<UInt32>::max();
            TEST( WorldMatch && ProxyTypesMatch && QueryFilterMatch ,"ProtoDeSerialize(const_XML::Node&)");
        }//Deserialize Test
    }

    /// @brief Since RunAutomaticTests is implemented so is this.
    /// @return returns true
    virtual bool HasAutomaticTests() const
        { return true; }
};//renderablerayquerytests

#endif
