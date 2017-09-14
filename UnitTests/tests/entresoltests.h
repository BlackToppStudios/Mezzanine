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
#ifndef _entrsoltests_h
#define _entrsoltests_h

#include "mezztest.h"

#include "entresol.h"

#include <stdexcept> //only used to throw for TEST_THROW

/// @file
/// @brief Tests some of the simplr features on the entresol, will test more in the future

using namespace Mezzanine;
using namespace Mezzanine::Testing;

/// @brief Test the manager container in the Mezzanine
class entresoltests : public UnitTestGroup
{
    public:
        /// @copydoc Mezzanine::Testing::UnitTestGroup::Name
        /// @return Returns a String containing "Entresol"
        virtual String Name()
            { return String("Entresol"); }

        /// @brief This is called when Automatic tests are run
        void RunAutomaticTests()
        {
            { // Target Times
                Mezzanine::Entresol TimingEnt;
                TestOutput << endl << "Default Entresol Timing in microseconds: " << TimingEnt.GetTargetFrameTimeMicroseconds() << endl;
                TEST(TimingEnt.GetTargetFrameTimeMicroseconds()==16666, "DefaultFrameTimeMicro");

                TestOutput << "Default Entresol Timing in milliseconds: " << TimingEnt.GetTargetFrameTimeMilliseconds() << endl;
                TEST(TimingEnt.GetTargetFrameTimeMilliseconds()==16, "DefaultFrameTimeMilli");

                TestOutput << "Attemping to set Entresol timing to have a single frame last 10 microseconds: ";
                TimingEnt.SetTargetFrameTimeMicroseconds(10);
                TestOutput << TimingEnt.GetTargetFrameTimeMicroseconds() << endl;
                TEST(TimingEnt.GetTargetFrameTimeMicroseconds()==10, "SetFrameTimeMicro");

                TestOutput << "Attemping to set Entresol timing to have a single frame last 10 milliseconds: ";
                TimingEnt.SetTargetFrameTimeMilliseconds(10);
                TestOutput << TimingEnt.GetTargetFrameTimeMilliseconds() << endl;
                TEST(TimingEnt.GetTargetFrameTimeMicroseconds()==10000, "SetFrameTimeMilli");

                TestOutput << "Attemping to set Entresol timing to 40 frames per second (should be 2500): ";
                TimingEnt.SetTargetFrameRate(40);
                TestOutput << TimingEnt.GetTargetFrameTimeMicroseconds() << endl;
                TEST(TimingEnt.GetTargetFrameTimeMicroseconds()==25000, "SetFrameTimeMilli");
            }

            {
                /*Mezzanine::Entresol TimingEnt;
                TimingEnt.SetTargetFrameTimeMicroseconds(100);
                TimingEnt.PreMainLoopInit();

                TimedTest HundredFrames;
                for(Whole I=0; I<100; I++)
                {
                    TimingEnt.DoOneFrame();
                }
                Whole Length = HundredFrames.GetLength();
                Whole TimePerFrame = Length/100;
                TEST(9800 < TimePerFrame && TimePerFrame < 10200, "FrameTiming2%Variance");
*/
            }

        }

        /// @brief Since RunAutomaticTests is implemented so is this.
        /// @return returns true
        virtual bool HasAutomaticTests() const
            { return true; }

};

#endif

