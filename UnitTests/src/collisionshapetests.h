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


                ///Copy for A fresh Set of Tests \/
                try
                {
                    stringstream Case1Stream, Case2Stream, Case1StreamRS, Case2StreamRS;                                    //create streams for the output of each test

                    BoxCollisionShape TestObject1("ABox",Vector3(1,2,3));                                                   //create all the object with the initial and functionally empty daya
                    BoxCollisionShape TestObject2("GeorgeWashington",Vector3(4,5,6));
                        TestObject2.SetMargin(0.09); // CollisionShape
                        TestObject2.SetScaling(Vector3(7.0,8.5,9.9)); // CollisionShape
                        // Shape Specific
                    BoxCollisionShape TestObject1RS("A",Vector3(0,0,0));
                    BoxCollisionShape TestObject2RS("B",Vector3(0,0,0));

                    Case1Stream << TestObject1;                                                                             //Streaming test 1 and compare output with results for results
                    String TestCase1Target("xxx");
                    if ( TestCase1Target == Case1Stream.str())
                        { AddTestResult("BoxCollisionShape::operator<< (Case1)", Success, UnitTestGroup::OverWrite); }
                    else
                        { AddTestResult("BoxCollisionShape::operator<< (Case1)", Failed, UnitTestGroup::OverWrite); }

                    Case2Stream << TestObject2;                                                                             //Streaming test 2
                    String TestCase2Target("xxx");
                    if ( TestCase2Target == Case2Stream.str())
                        { AddTestResult("BoxCollisionShape::operator<< (Case2)", Success, UnitTestGroup::OverWrite); }
                    else
                        { AddTestResult("BoxCollisionShape::operator<< (Case2)", Failed, UnitTestGroup::OverWrite); }

                    Case1Stream >> TestObject1RS;                                                                           //ReStreaming test 1
                    Case1StreamRS << TestObject1RS;
                    if ( TestCase1Target == Case1StreamRS.str())
                        { AddTestResult("BoxCollisionShape::operator>> (Case1)", Success, UnitTestGroup::OverWrite); }
                    else
                        { AddTestResult("BoxCollisionShape::operator>> (Case1)", Failed, UnitTestGroup::OverWrite); }

                    Case2Stream >> TestObject2RS;                                                                           //ReStreaming test 2
                    Case2StreamRS << TestObject2RS;
                    if ( TestCase2Target == Case2StreamRS.str())
                        { AddTestResult("BoxCollisionShape::operator>> (Case2)", Success, UnitTestGroup::OverWrite); }
                    else
                        { AddTestResult("BoxCollisionShape::operator>> (Case2)", Failed, UnitTestGroup::OverWrite); }

                }catch (phys::Exception e){
                    cout << endl << endl << e.what() << endl << endl;
                }
                ///Copy for A fresh Set of Tests /\

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
