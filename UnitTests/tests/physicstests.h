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
#ifndef _physicstests_h
#define _physicstests_h

#include "mezztest.h"

#include "mezzanine.h"

/// @file
/// @brief Basic tests of the physics system

using namespace std;
using namespace Mezzanine;
using namespace Mezzanine::Testing;
using namespace Mezzanine::Threading;

/// @brief Tests for the WorkUnit class
class physicstests : public UnitTestGroup
{
    public:
        /// @copydoc Mezzanine::Testing::UnitTestGroup::Name
        /// @return Returns a String containing "Physics"
        virtual String Name()
            { return String("Physics"); }

        /// @brief Test if the barrier works properly
        void RunAutomaticTests()
        {
            {
                TestOutput << "Creating A creating a rigid body and subjecting it to just a little gravity." << endl;
                // 0
                Mezzanine::Physics::PhysicsManager Simulation(NULL);
                Simulation.SetWorldGravity(Vector3(0.0,9.8,0.0));
                Mezzanine::Physics::RigidProxy* RigidA = Simulation.CreateRigidProxy(10.0);
                Simulation.SetSimulationSubstepModifier(3);
                Physics::SphereCollisionShape Ball("Ball",5.0);
                RigidA->SetCollisionShape(&Ball);
                RigidA->SetLocation(Vector3(1.0, 1.0, 1.0));
                RigidA->AddToWorld();
                // 9
                FrameScheduler FS;
                FS.AddWorkUnitMain(Simulation.GetSimulationWork(),"Physics");
                FS.DoOneFrame();
                FS.DoOneFrame();
                FS.RemoveWorkUnitMain(Simulation.GetSimulationWork());
                // 15

                TestOutput << "Location: " << RigidA->GetLocation() << endl;
                TEST_EQUAL_MULTI_EPSILON(Vector3(1.0, 1.00272, 1.0), RigidA->GetLocation(), "DeterminismGravityTest", 100);
            }
            {
                TestOutput << "Creating A creating a rigid body and subjecting it to just a little gravity 10,000 times." << endl;
                MaxInt StartTime = GetTimeStamp();
                Real X = 0.0; Real Y = 0.0; Real Z = 0.0;
                for(Whole Counter=0; Counter<10000; Counter++)
                {
                    // 0
                    Mezzanine::Physics::PhysicsManager Simulation(NULL);
                    Simulation.SetWorldGravity(Vector3(0.0,9.8,0.0));
                    Mezzanine::Physics::RigidProxy* RigidA = Simulation.CreateRigidProxy(10.0);
                    Simulation.SetSimulationSubstepModifier(3);
                    Physics::SphereCollisionShape Ball("Ball",5.0);
                    RigidA->SetCollisionShape(&Ball);
                    RigidA->SetLocation(Vector3(1.0, 1.0, 1.0));
                    RigidA->AddToWorld();
                    // 9
                    FrameScheduler FS;
                    FS.SetFrameLength(0);
                    FS.AddWorkUnitMain(Simulation.GetSimulationWork(),"Physics");
                    FS.DoOneFrame();
                    FS.DoOneFrame();
                    FS.RemoveWorkUnitMain(Simulation.GetSimulationWork());
                    // 15
                    X=RigidA->GetLocation().X;
                    Y=RigidA->GetLocation().Y;
                    Z=RigidA->GetLocation().Z;
                }
                MaxInt EndTime = GetTimeStamp();

                Vector3 Results(X,Y,Z);
                TestOutput << "Location: " << Results << endl << "It Took: " << (EndTime-StartTime) << " microseconds" << endl;
            }
        }

        /// @brief Since RunAutomaticTests is implemented so is this.
        /// @return returns true
        virtual bool HasAutomaticTests() const
            { return true; }


};

#endif

