// © Copyright 2010 - 2014 BlackTopp Studios Inc.
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
#ifndef _physicsluatests_h
#define _physicsluatests_h

#include "mezztest.h"

#include "mezzanine.h"

/// @file
/// @brief Basic tests of the physics system

using namespace std;
using namespace Mezzanine;
using namespace Mezzanine::Testing;
using namespace Mezzanine::Threading;

/// @brief Tests for the WorkUnit class
class physicsluatests : public UnitTestGroup
{
    public:
        /// @copydoc Mezzanine::Testing::UnitTestGroup::Name
        /// @return Returns a String containing "PhysicsLua"
        virtual String Name()
            { return String("PhysicsLua"); }

        /// @brief Test if the barrier works properly
        void RunAutomaticTests()
        {
            Scripting::Lua::Lua51ScriptingEngine Lua;
            TestOutput << "Creating A creating a rigid body and subjecting it to just a little gravity." << endl;

            // lines in cpp of physics manager tests
            // 0
            Lua.Execute("Simulation = MezzaninePhysicsSafe.PhysicsManager()");
            Lua.Execute("RigidA = Simulation:CreateRigidProxy(10.0)");
            Lua.Execute("Simulation:SetSimulationSubstepModifier(3)");
            Lua.Execute("Ball = MezzaninePhysicsSafe.SphereCollisionShape('Ball',5.0)");
            Lua.Execute("RigidA:SetCollisionShape(Ball)");
            Lua.Execute("RigidA:AddToWorld()");
            // 7
            Lua.Execute("Simulation:SetWorldGravity(MezzanineSafe.Vector3(0.0, 9.8, 0.0))");
            Lua.Execute("FS=MezzanineThreadingSafe.FrameScheduler()");
            Lua.Execute("Ignored=MezzanineThreadingSafe.ThreadSpecificStorage(FS)");
            // 11
            // Lua.Execute("Ignored:GetFrameScheduler():GetFrameTimeRollingAverage():Insert(1000)"); // Can't do this in Lua Won't do it here
            // 13
            Lua.Execute("MezzanineThreadingSafe.sleep_for(100000)");
            Lua.Execute("Simulation:GetSimulationWork():DoWork(Ignored)");
            Lua.Execute("MezzanineThreadingSafe.sleep_for(100000)");
            Lua.Execute("Simulation:GetSimulationWork():DoWork(Ignored)");
            // 11



            Lua.Execute("");

        }

        /// @brief Since RunAutomaticTests is implemented so is this.
        /// @return returns true
        virtual bool HasAutomaticTests() const
            { return true; }


};

#endif

