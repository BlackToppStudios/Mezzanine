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
#ifndef _collisionshapetests_h
#define _collisionshapetests_h

#include "main.h"


class CollisionShapeTests : public UnitTestGroup
{
    public:
        virtual TestResult RunTests(bool RunAutomaticTests, bool RunInteractiveTests)
        {
            if (RunAutomaticTests)
            {
                StartEngine();

                AddTestResult("BoxCollisionShape::operator<< (Case1)", Unknown);
                AddTestResult("CapsuleCollisionShape::operator<< (Case1)", Unknown);
                AddTestResult("CompoundCollisionShape::operator<< (Case1)", Unknown);
                AddTestResult("ConeCollisionShape::operator<< (Case1)", Unknown);
                AddTestResult("ConvexHullCollisionShape::operator<< (Case1)", Unknown);
                AddTestResult("CylinderCollisionShape::operator<< (Case1)", Unknown);
                AddTestResult("MultiSphereCollisionShape::operator<< (Case1)", Unknown);
                AddTestResult("SphereCollisionShape::operator<< (Case1)", Unknown);
                AddTestResult("DynamicMeshCollisionShape::operator<< (Case1)", Unknown);
                AddTestResult("HeightfieldCollisionShape::operator<< (Case1)", Unknown);
                AddTestResult("PlaneCollisionShape::operator<< (Case1)", Unknown);
                AddTestResult("ActorSoftCollisionShape::operator<< (Case1)", Unknown);
                AddTestResult("StaticMeshCollisionShape::operator<< (Case1)", Unknown);

                AddTestResult("BoxCollisionShape::operator>> (Case1)", Unknown);
                AddTestResult("CapsuleCollisionShape::operator>> (Case1)", Unknown);
                AddTestResult("CompoundCollisionShape::operator>> (Case1)", Unknown);
                AddTestResult("ConeCollisionShape::operator>> (Case1)", Unknown);
                AddTestResult("ConvexHullCollisionShape::operator>> (Case1)", Unknown);
                AddTestResult("CylinderCollisionShape::operator>> (Case1)", Unknown);
                AddTestResult("MultiSphereCollisionShape::operator>> (Case1)", Unknown);
                AddTestResult("SphereCollisionShape::operator>> (Case1)", Unknown);
                AddTestResult("DynamicMeshCollisionShape::operator>> (Case1)", Unknown);
                AddTestResult("HeightfieldCollisionShape::operator>> (Case1)", Unknown);
                AddTestResult("PlaneCollisionShape::operator>> (Case1)", Unknown);
                AddTestResult("ActorSoftCollisionShape::operator>> (Case1)", Unknown);
                AddTestResult("StaticMeshCollisionShape::operator>> (Case1)", Unknown);

                AddTestResult("BoxCollisionShape::operator<< (Case2)", Unknown);
                AddTestResult("CapsuleCollisionShape::operator<< (Case2)", Unknown);
                AddTestResult("CompoundCollisionShape::operator<< (Case2)", Unknown);
                AddTestResult("ConeCollisionShape::operator<< (Case2)", Unknown);
                AddTestResult("ConvexHullCollisionShape::operator<< (Case2)", Unknown);
                AddTestResult("CylinderCollisionShape::operator<< (Case2)", Unknown);
                AddTestResult("MultiSphereCollisionShape::operator<< (Case2)", Unknown);
                AddTestResult("SphereCollisionShape::operator<< (Case2)", Unknown);
                AddTestResult("DynamicMeshCollisionShape::operator<< (Case2)", Unknown);
                AddTestResult("HeightfieldCollisionShape::operator<< (Case2)", Unknown);
                AddTestResult("PlaneCollisionShape::operator<< (Case2)", Unknown);
                AddTestResult("ActorSoftCollisionShape::operator<< (Case2)", Unknown);
                AddTestResult("StaticMeshCollisionShape::operator<< (Case2)", Unknown);

                AddTestResult("BoxCollisionShape::operator>> (Case2)", Unknown);
                AddTestResult("CapsuleCollisionShape::operator>> (Case2)", Unknown);
                AddTestResult("CompoundCollisionShape::operator>> (Case2)", Unknown);
                AddTestResult("ConeCollisionShape::operator>> (Case2)", Unknown);
                AddTestResult("ConvexHullCollisionShape::operator>> (Case2)", Unknown);
                AddTestResult("CylinderCollisionShape::operator>> (Case2)", Unknown);
                AddTestResult("MultiSphereCollisionShape::operator>> (Case2)", Unknown);
                AddTestResult("SphereCollisionShape::operator>> (Case2)", Unknown);
                AddTestResult("DynamicMeshCollisionShape::operator>> (Case2)", Unknown);
                AddTestResult("HeightfieldCollisionShape::operator>> (Case2)", Unknown);
                AddTestResult("PlaneCollisionShape::operator>> (Case2)", Unknown);
                AddTestResult("ActorSoftCollisionShape::operator>> (Case2)", Unknown);
                AddTestResult("StaticMeshCollisionShape::operator>> (Case2)", Unknown);

                try
                {
                    stringstream Case1Stream, Case2Stream, Case1StreamRS, Case2StreamRS;                                    //create streams for the output of each test

                    BoxCollisionShape TestObject1("ABox",Vector3(1,2,3));                                                   //create two objects one initial and functionally empty data and one with many(every) change
                    BoxCollisionShape TestObject2("GeorgeWashington",Vector3(4,5,6));
                        TestObject2.SetMargin(0.09); // CollisionShape
                        TestObject2.SetScaling(Vector3(7.0,8.5,9.9)); // CollisionShape
                    CollisionShape* TestObject1RS = 0;
                    CollisionShape* TestObject2RS = 0;
                    //cout << endl << endl << TestObject1 << endl << endl << TestObject2 << endl << endl;

                    Case1Stream << TestObject1;                                                                             //Streaming test 1 and compare output with results for results
                    String TestCase1Target("<BoxCollisionShape Version=\"1\"><PrimitiveCollisionShape Version=\"1\"><ImplicitShape><Vector3 Version=\"1\" X=\"0.96\" Y=\"1.96\" Z=\"2.96\" /></ImplicitShape><CollisionShape Version=\"1\" Name=\"ABox\" Margin=\"0.04\"><Scaling><Vector3 Version=\"1\" X=\"1\" Y=\"1\" Z=\"1\" /></Scaling></CollisionShape></PrimitiveCollisionShape></BoxCollisionShape>");
                    if ( TestCase1Target == Case1Stream.str())
                        { AddTestResult("BoxCollisionShape::operator<< (Case1)", Success, UnitTestGroup::OverWrite); }
                    else
                        { AddTestResult("BoxCollisionShape::operator<< (Case1)", Failed, UnitTestGroup::OverWrite); }

                    Case2Stream << TestObject2;                                                                             //Streaming test 2
                    String TestCase2Target("<BoxCollisionShape Version=\"1\"><PrimitiveCollisionShape Version=\"1\"><ImplicitShape><Vector3 Version=\"1\" X=\"27.91\" Y=\"42.41\" Z=\"59.31\" /></ImplicitShape><CollisionShape Version=\"1\" Name=\"GeorgeWashington\" Margin=\"0.09\"><Scaling><Vector3 Version=\"1\" X=\"7\" Y=\"8.5\" Z=\"9.9\" /></Scaling></CollisionShape></PrimitiveCollisionShape></BoxCollisionShape>");
                    if ( TestCase2Target == Case2Stream.str())
                        { AddTestResult("BoxCollisionShape::operator<< (Case2)", Success, UnitTestGroup::OverWrite); }
                    else
                        { AddTestResult("BoxCollisionShape::operator<< (Case2)", Failed, UnitTestGroup::OverWrite); }

                    TestObject1RS = World::GetWorldPointer()->GetCollisionShapeManager()->ShapeDeserializer.DeSerializeAndRetrieve(Case1Stream);
                    if (TestObject1RS)
                        { Case1StreamRS << *TestObject1RS; }
                    if ( TestCase1Target == Case1StreamRS.str())
                        { AddTestResult("BoxCollisionShape::operator>> (Case1)", Success, UnitTestGroup::OverWrite); }
                    else
                        { AddTestResult("BoxCollisionShape::operator>> (Case1)", Failed, UnitTestGroup::OverWrite); }

                    TestObject2RS = World::GetWorldPointer()->GetCollisionShapeManager()->ShapeDeserializer.DeSerializeAndRetrieve(Case2Stream);
                    if (TestObject2RS)
                        { Case2StreamRS << *TestObject2RS; }
                    if ( TestCase2Target == Case2StreamRS.str())
                        { AddTestResult("BoxCollisionShape::operator>> (Case2)", Success, UnitTestGroup::OverWrite); }
                    else
                        { AddTestResult("BoxCollisionShape::operator>> (Case2)", Failed, UnitTestGroup::OverWrite); }
                    //cout << endl << endl << *TestObject1RS << endl << endl << *TestObject2RS << endl << endl;

                }catch (phys::Exception e){
                    cout << endl << endl << "Phys Expection: " << e.what() << endl << endl;
                }catch (exception e){
                    cout << endl << endl << "Non-Phys Expection: " << e.what() << endl << endl;
                }

                try
                {
                    stringstream Case1Stream, Case2Stream, Case1StreamRS, Case2StreamRS;                                    //create streams for the output of each test

                    CapsuleCollisionShape TestObject1("ABox",1,2,Vector3(0,1,0));                                           //create two objects one initial and functionally empty data and one with many(every) change
                    CapsuleCollisionShape TestObject2("GeorgeWashington",3,4,Axis_Z);
                        TestObject2.SetMargin(0.09); // CollisionShape
                        TestObject2.SetScaling(Vector3(7.0,8.0,9.9)); // CollisionShape
                    CollisionShape* TestObject1RS = 0;
                    CollisionShape* TestObject2RS = 0;
                    //cout << endl << endl << TestObject1 << endl << endl << TestObject2 << endl << endl;

                    Case1Stream << TestObject1;                                                                             //Streaming test 1 and compare output with results for results
                    String TestCase1Target("<CapsuleCollisionShape Version=\"1\" Axis=\"1\"><PrimitiveCollisionShape Version=\"1\"><ImplicitShape><Vector3 Version=\"1\" X=\"1\" Y=\"1\" Z=\"1\" /></ImplicitShape><CollisionShape Version=\"1\" Name=\"ABox\" Margin=\"0.04\"><Scaling><Vector3 Version=\"1\" X=\"1\" Y=\"1\" Z=\"1\" /></Scaling></CollisionShape></PrimitiveCollisionShape></CapsuleCollisionShape>");
                    if ( TestCase1Target == Case1Stream.str())
                        { AddTestResult("CapsuleCollisionShape::operator<< (Case1)", Success, UnitTestGroup::OverWrite); }
                    else
                        { AddTestResult("CapsuleCollisionShape::operator<< (Case1)", Failed, UnitTestGroup::OverWrite); }

                    Case2Stream << TestObject2;                                                                             //Streaming test 2
                    String TestCase2Target("<CapsuleCollisionShape Version=\"1\" Axis=\"2\"><PrimitiveCollisionShape Version=\"1\"><ImplicitShape><Vector3 Version=\"1\" X=\"21.19\" Y=\"24.23\" Z=\"20.106\" /></ImplicitShape><CollisionShape Version=\"1\" Name=\"GeorgeWashington\" Margin=\"0.09\"><Scaling><Vector3 Version=\"1\" X=\"7\" Y=\"8\" Z=\"9.9\" /></Scaling></CollisionShape></PrimitiveCollisionShape></CapsuleCollisionShape>");
                    if ( TestCase2Target == Case2Stream.str())
                        { AddTestResult("CapsuleCollisionShape::operator<< (Case2)", Success, UnitTestGroup::OverWrite); }
                    else
                        { AddTestResult("CapsuleCollisionShape::operator<< (Case2)", Failed, UnitTestGroup::OverWrite); }

                    TestObject1RS = World::GetWorldPointer()->GetCollisionShapeManager()->ShapeDeserializer.DeSerializeAndRetrieve(Case1Stream);
                    if (TestObject1RS)
                        { Case1StreamRS << *TestObject1RS; }
                    if ( TestCase1Target == Case1StreamRS.str())
                        { AddTestResult("CapsuleCollisionShape::operator>> (Case1)", Success, UnitTestGroup::OverWrite); }
                    else
                        { AddTestResult("CapsuleCollisionShape::operator>> (Case1)", Failed, UnitTestGroup::OverWrite); }

                    TestObject2RS = World::GetWorldPointer()->GetCollisionShapeManager()->ShapeDeserializer.DeSerializeAndRetrieve(Case2Stream);
                    if (TestObject2RS)
                        { Case2StreamRS << *TestObject2RS; }
                    if ( TestCase2Target == Case2StreamRS.str())
                        { AddTestResult("CapsuleCollisionShape::operator>> (Case2)", Success, UnitTestGroup::OverWrite); }
                    else
                        { AddTestResult("CapsuleCollisionShape::operator>> (Case2)", Failed, UnitTestGroup::OverWrite); }
                    //cout << endl << endl << *TestObject1RS << endl << endl << *TestObject2RS << endl << endl;

                }catch (phys::Exception e){
                    cout << endl << endl << "Phys Expection: " << e.what() << endl << endl;
                }catch (exception e){
                    cout << endl << endl << "Non-Phys Expection: " << e.what() << endl << endl;
                }

                try
                {
                    stringstream Case1Stream, Case2Stream, Case1StreamRS, Case2StreamRS;                                    //create streams for the output of each test

                    ConeCollisionShape TestObject1("ABox",1,2,Vector3(0,1,0));                                              //create two objects one initial and functionally empty data and one with many(every) change
                    ConeCollisionShape TestObject2("GeorgeWashington",3,4,Axis_Z);
                        TestObject2.SetMargin(0.09); // CollisionShape
                        TestObject2.SetScaling(Vector3(7.0,8.0,9.9)); // CollisionShape
                    CollisionShape* TestObject1RS = 0;
                    CollisionShape* TestObject2RS = 0;
                    //cout << endl << endl << TestObject1 << endl << endl << TestObject2 << endl << endl;

                    Case1Stream << TestObject1;                                                                             //Streaming test 1 and compare output with results for results
                    String TestCase1Target("<ConeCollisionShape Version=\"1\" Radius=\"1\" Height=\"2\" Axis=\"1\"><PrimitiveCollisionShape Version=\"1\"><ImplicitShape><Vector3 Version=\"1\" X=\"0\" Y=\"0\" Z=\"0\" /></ImplicitShape><CollisionShape Version=\"1\" Name=\"ABox\" Margin=\"0.04\"><Scaling><Vector3 Version=\"1\" X=\"1\" Y=\"1\" Z=\"1\" /></Scaling></CollisionShape></PrimitiveCollisionShape></ConeCollisionShape>");
                    if ( TestCase1Target == Case1Stream.str())
                        { AddTestResult("ConeCollisionShape::operator<< (Case1)", Success, UnitTestGroup::OverWrite); }
                    else
                        { AddTestResult("ConeCollisionShape::operator<< (Case1)", Failed, UnitTestGroup::OverWrite); }

                    Case2Stream << TestObject2;                                                                             //Streaming test 2
                    String TestCase2Target("<ConeCollisionShape Version=\"1\" Radius=\"3\" Height=\"4\" Axis=\"2\"><PrimitiveCollisionShape Version=\"1\"><ImplicitShape><Vector3 Version=\"1\" X=\"0\" Y=\"0\" Z=\"0\" /></ImplicitShape><CollisionShape Version=\"1\" Name=\"GeorgeWashington\" Margin=\"0.09\"><Scaling><Vector3 Version=\"1\" X=\"7\" Y=\"8\" Z=\"9.9\" /></Scaling></CollisionShape></PrimitiveCollisionShape></ConeCollisionShape>");
                    if ( TestCase2Target == Case2Stream.str())
                        { AddTestResult("ConeCollisionShape::operator<< (Case2)", Success, UnitTestGroup::OverWrite); }
                    else
                        { AddTestResult("ConeCollisionShape::operator<< (Case2)", Failed, UnitTestGroup::OverWrite); }

                    TestObject1RS = World::GetWorldPointer()->GetCollisionShapeManager()->ShapeDeserializer.DeSerializeAndRetrieve(Case1Stream);
                    if (TestObject1RS)
                        { Case1StreamRS << *TestObject1RS; }
                    if ( TestCase1Target == Case1StreamRS.str())
                        { AddTestResult("ConeCollisionShape::operator>> (Case1)", Success, UnitTestGroup::OverWrite); }
                    else
                        { AddTestResult("ConeCollisionShape::operator>> (Case1)", Failed, UnitTestGroup::OverWrite); }

                    TestObject2RS = World::GetWorldPointer()->GetCollisionShapeManager()->ShapeDeserializer.DeSerializeAndRetrieve(Case2Stream);
                    if (TestObject2RS)
                        { Case2StreamRS << *TestObject2RS; }
                    if ( TestCase2Target == Case2StreamRS.str())
                        { AddTestResult("ConeCollisionShape::operator>> (Case2)", Success, UnitTestGroup::OverWrite); }
                    else
                        { AddTestResult("ConeCollisionShape::operator>> (Case2)", Failed, UnitTestGroup::OverWrite); }
                    //cout << endl << endl << *TestObject1RS << endl << endl << *TestObject2RS << endl << endl;

                }catch (phys::Exception e){
                    cout << endl << endl << "Phys Expection: " << e.what() << endl << endl;
                }catch (exception e){
                    cout << endl << endl << "Non-Phys Expection: " << e.what() << endl << endl;
                }

                ///Copy for A fresh Set of Tests \/
                try
                {
                    stringstream Case1Stream, Case2Stream, Case1StreamRS, Case2StreamRS;                                    //create streams for the output of each test

                    std::vector<Vector3> SimplePoints;
                    SimplePoints.push_back(Vector3(0,0,0));
                    SimplePoints.push_back(Vector3(1,0,0));
                    SimplePoints.push_back(Vector3(0,1,0));
                    SimplePoints.push_back(Vector3(0,0,1));

                    std::vector<Vector3> ManyPoints;
                    ManyPoints.push_back(Vector3(0,0,0));   // will wind up completely inside
                    ManyPoints.push_back(Vector3(1,0,0));
                    ManyPoints.push_back(Vector3(0,1,0));
                    ManyPoints.push_back(Vector3(0,0,1));
                    ManyPoints.push_back(Vector3(-1,0,0));
                    ManyPoints.push_back(Vector3(0,-1,0));
                    ManyPoints.push_back(Vector3(0,0,-1));
                    ManyPoints.push_back(Vector3(0.5,0,0)); //another point completel inside

                    ConvexHullCollisionShape TestObject1("d4",SimplePoints);                                                      //create two objects one initial and functionally empty data and one with many(every) change
                    ConvexHullCollisionShape TestObject2("IThinkItsAd8WithAgravityPrblem",ManyPoints);
                        TestObject2.SetMargin(0.09); // CollisionShape
                        TestObject2.SetScaling(Vector3(7.0,8.0,9.9)); // CollisionShape
                    CollisionShape* TestObject1RS = 0;
                    CollisionShape* TestObject2RS = 0;
                    cout << endl << endl << TestObject1 << endl << endl << TestObject2 << endl << endl;

                    Case1Stream << TestObject1;                                                                             //Streaming test 1 and compare output with results for results
                    String TestCase1Target("<ConvexHullCollisionShape Version=\"1\"><UnscaledPoints><Vector3 Version=\"1\" X=\"0\" Y=\"0\" Z=\"0\" /><Vector3 Version=\"1\" X=\"1\" Y=\"0\" Z=\"0\" /><Vector3 Version=\"1\" X=\"0\" Y=\"1\" Z=\"0\" /><Vector3 Version=\"1\" X=\"0\" Y=\"0\" Z=\"1\" /></UnscaledPoints><PrimitiveCollisionShape Version=\"1\"><ImplicitShape><Vector3 Version=\"1\" X=\"0\" Y=\"0\" Z=\"0\" /></ImplicitShape><CollisionShape Version=\"1\" Name=\"d4\" Margin=\"0.04\"><Scaling><Vector3 Version=\"1\" X=\"1\" Y=\"1\" Z=\"1\" /></Scaling></CollisionShape></PrimitiveCollisionShape></ConvexHullCollisionShape>");
                    if ( TestCase1Target == Case1Stream.str())
                        { AddTestResult("ConvexHullCollisionShape::operator<< (Case1)", Success, UnitTestGroup::OverWrite); }
                    else
                        { AddTestResult("ConvexHullCollisionShape::operator<< (Case1)", Failed, UnitTestGroup::OverWrite); }

                    Case2Stream << TestObject2;                                                                             //Streaming test 2
                    String TestCase2Target("<ConvexHullCollisionShape Version=\"1\"><UnscaledPoints><Vector3 Version=\"1\" X=\"0\" Y=\"0\" Z=\"0\" /><Vector3 Version=\"1\" X=\"1\" Y=\"0\" Z=\"0\" /><Vector3 Version=\"1\" X=\"0\" Y=\"1\" Z=\"0\" /><Vector3 Version=\"1\" X=\"0\" Y=\"0\" Z=\"1\" /><Vector3 Version=\"1\" X=\"-1\" Y=\"0\" Z=\"0\" /><Vector3 Version=\"1\" X=\"0\" Y=\"-1\" Z=\"0\" /><Vector3 Version=\"1\" X=\"0\" Y=\"0\" Z=\"-1\" /><Vector3 Version=\"1\" X=\"0.5\" Y=\"0\" Z=\"0\" /></UnscaledPoints><PrimitiveCollisionShape Version=\"1\"><ImplicitShape><Vector3 Version=\"1\" X=\"0\" Y=\"0\" Z=\"0\" /></ImplicitShape><CollisionShape Version=\"1\" Name=\"IThinkItsAd8WithAgravityPrblem\" Margin=\"0.09\"><Scaling><Vector3 Version=\"1\" X=\"7\" Y=\"8\" Z=\"9.9\" /></Scaling></CollisionShape></PrimitiveCollisionShape></ConvexHullCollisionShape>");
                    if ( TestCase2Target == Case2Stream.str())
                        { AddTestResult("ConvexHullCollisionShape::operator<< (Case2)", Success, UnitTestGroup::OverWrite); }
                    else
                        { AddTestResult("ConvexHullCollisionShape::operator<< (Case2)", Failed, UnitTestGroup::OverWrite); }

                    TestObject1RS = World::GetWorldPointer()->GetCollisionShapeManager()->ShapeDeserializer.DeSerializeAndRetrieve(Case1Stream);
                    if (TestObject1RS)
                        { Case1StreamRS << *TestObject1RS; }
                    if ( TestCase1Target == Case1StreamRS.str())
                        { AddTestResult("ConvexHullCollisionShape::operator>> (Case1)", Success, UnitTestGroup::OverWrite); }
                    else
                        { AddTestResult("ConvexHullCollisionShape::operator>> (Case1)", Failed, UnitTestGroup::OverWrite); }

                    TestObject2RS = World::GetWorldPointer()->GetCollisionShapeManager()->ShapeDeserializer.DeSerializeAndRetrieve(Case2Stream);
                    if (TestObject2RS)
                        { Case2StreamRS << *TestObject2RS; }
                    if ( TestCase2Target == Case2StreamRS.str())
                        { AddTestResult("ConvexHullCollisionShape::operator>> (Case2)", Success, UnitTestGroup::OverWrite); }
                    else
                        { AddTestResult("ConvexHullCollisionShape::operator>> (Case2)", Failed, UnitTestGroup::OverWrite); }
                    cout << endl << endl << *TestObject1RS << endl << endl << *TestObject2RS << endl << endl;

                }catch (phys::Exception e){
                    cout << endl << endl << "Phys Expection: " << e.what() << endl << endl;
                }catch (exception e){
                    cout << endl << endl << "Non-Phys Expection: " << e.what() << endl << endl;
                } //*/
                ///Copy for A fresh Set of Tests /\

                StopEngine();

            }else{
                AddTestResult("BoxCollisionShape::operator<< (Case1)", Skipped);
                AddTestResult("CapsuleCollisionShape::operator<< (Case1)", Skipped);
                AddTestResult("CompoundCollisionShape::operator<< (Case1)", Skipped);
                AddTestResult("ConeCollisionShape::operator<< (Case1)", Skipped);
                AddTestResult("ConvexHullCollisionShape::operator<< (Case1)", Skipped);
                AddTestResult("CylinderCollisionShape::operator<< (Case1)", Skipped);
                AddTestResult("MultiSphereCollisionShape::operator<< (Case1)", Skipped);
                AddTestResult("SphereCollisionShape::operator<< (Case1)", Skipped);
                AddTestResult("DynamicMeshCollisionShape::operator<< (Case1)", Skipped);
                AddTestResult("HeightfieldCollisionShape::operator<< (Case1)", Skipped);
                AddTestResult("PlaneCollisionShape::operator<< (Case1)", Skipped);
                AddTestResult("ActorSoftCollisionShape::operator<< (Case1)", Skipped);
                AddTestResult("StaticMeshCollisionShape::operator<< (Case1)", Skipped);

                AddTestResult("BoxCollisionShape::operator>> (Case1)", Skipped);
                AddTestResult("CapsuleCollisionShape::operator>> (Case1)", Skipped);
                AddTestResult("CompoundCollisionShape::operator>> (Case1)", Skipped);
                AddTestResult("ConeCollisionShape::operator>> (Case1)", Skipped);
                AddTestResult("ConvexHullCollisionShape::operator>> (Case1)", Skipped);
                AddTestResult("CylinderCollisionShape::operator>> (Case1)", Skipped);
                AddTestResult("MultiSphereCollisionShape::operator>> (Case1)", Skipped);
                AddTestResult("SphereCollisionShape::operator>> (Case1)", Skipped);
                AddTestResult("DynamicMeshCollisionShape::operator>> (Case1)", Skipped);
                AddTestResult("HeightfieldCollisionShape::operator>> (Case1)", Skipped);
                AddTestResult("PlaneCollisionShape::operator>> (Case1)", Skipped);
                AddTestResult("ActorSoftCollisionShape::operator>> (Case1)", Skipped);
                AddTestResult("StaticMeshCollisionShape::operator>> (Case1)", Skipped);

                AddTestResult("BoxCollisionShape::operator<< (Case2)", Skipped);
                AddTestResult("CapsuleCollisionShape::operator<< (Case2)", Skipped);
                AddTestResult("CompoundCollisionShape::operator<< (Case2)", Skipped);
                AddTestResult("ConeCollisionShape::operator<< (Case2)", Skipped);
                AddTestResult("ConvexHullCollisionShape::operator<< (Case2)", Skipped);
                AddTestResult("CylinderCollisionShape::operator<< (Case2)", Skipped);
                AddTestResult("MultiSphereCollisionShape::operator<< (Case2)", Skipped);
                AddTestResult("SphereCollisionShape::operator<< (Case2)", Skipped);
                AddTestResult("DynamicMeshCollisionShape::operator<< (Case2)", Skipped);
                AddTestResult("HeightfieldCollisionShape::operator<< (Case2)", Skipped);
                AddTestResult("PlaneCollisionShape::operator<< (Case2)", Skipped);
                AddTestResult("ActorSoftCollisionShape::operator<< (Case2)", Skipped);
                AddTestResult("StaticMeshCollisionShape::operator<< (Case2)", Skipped);

                AddTestResult("BoxCollisionShape::operator>> (Case2)", Skipped);
                AddTestResult("CapsuleCollisionShape::operator>> (Case2)", Skipped);
                AddTestResult("CompoundCollisionShape::operator>> (Case2)", Skipped);
                AddTestResult("ConeCollisionShape::operator>> (Case2)", Skipped);
                AddTestResult("ConvexHullCollisionShape::operator>> (Case2)", Skipped);
                AddTestResult("CylinderCollisionShape::operator>> (Case2)", Skipped);
                AddTestResult("MultiSphereCollisionShape::operator>> (Case2)", Skipped);
                AddTestResult("SphereCollisionShape::operator>> (Case2)", Skipped);
                AddTestResult("DynamicMeshCollisionShape::operator>> (Case2)", Skipped);
                AddTestResult("HeightfieldCollisionShape::operator>> (Case2)", Skipped);
                AddTestResult("PlaneCollisionShape::operator>> (Case2)", Skipped);
                AddTestResult("ActorSoftCollisionShape::operator>> (Case2)", Skipped);
                AddTestResult("StaticMeshCollisionShape::operator>> (Case2)", Skipped);
            }
        }
};
#endif
