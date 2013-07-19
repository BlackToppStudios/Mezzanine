//© Copyright 2010 - 2013 BlackTopp Studios Inc.
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
#ifndef _workunittests_h
#define _workunittests_h

#include "mezztest.h"

#include "dagframescheduler.h"
#include "dagrandomtests.h"

/// @file
/// @brief Basic tests of the workunits class

using namespace std;
using namespace Mezzanine;
using namespace Mezzanine::Testing;
using namespace Mezzanine::Threading;

/// @brief An intentionally large and slow to process type chosen once here, to allow easy adjusting of these tests.
typedef long double PreciseFloat;

/// @brief A very inefficient way to calculate Pi with some spikes in execution time
/// @param Length How many iteration to use when calculating Pi
/// @param Spike if not 0 then this function will 1 in twentys increase in execution time by between 2 and 6 times.
/// @return Pi To varying degrees of precision
PreciseFloat MakePi(Mezzanine::Whole Length, Whole Spike = 0)
{
    if(Spike!=0 && 10==DiceD20())          // So one in twenty times
    {
        Spike=Dice2D3();        // Execution will take 2 to 6 times longer
        //* LocalLog << "<Bonuswork Multiple=\"" << Spike << "\" />" << endl;
    } else {
        Spike=1;
    }

    PreciseFloat Pi = 1.0;
    PreciseFloat Taylor = 3;
    for (Whole Count = 0; Count<(Length*Spike); ++Count)
    {
        Pi-=PreciseFloat(1.0)/Taylor;
        Taylor+=PreciseFloat(2.0);
        Pi+=PreciseFloat(1.0)/Taylor;
        Taylor+=PreciseFloat(2.0);
    }
    return PreciseFloat(4.0) * Pi;
}

/// @brief A samplework unit that calculates pi
/// @brief Used in @ref WorkUnitTests and other tests that require actual cpu consumption
/// @warning Everything on these samples has a public access specifier, for production code that is poor form, encapsulate your stuff.
class PiMakerWorkUnit : public Mezzanine::Threading::DefaultWorkUnit
{
    public:
        /// @brief How many iterations will we use when calculating Pi
        Mezzanine::Whole Length;

        /// @brief Name used for clarifying output and enhancing logging.
        Mezzanine::String Name;

        /// @brief should this randomly spike execution time.
        bool SpikesOn;

        /// @brief Create on of these workunit
        /// @param Length_ Defaults to 50 and is the number of iterations to use, more means slower execution
        /// @param Name Defaults to "Default" only used in output logs
        /// @param SpikesOn_ Defaults to true, used to make execution more dificult to predict
        PiMakerWorkUnit(Mezzanine::Whole Length_ = 50, Mezzanine::String Name_ = "Default", bool SpikesOn_=true)
            : Length(Length_), Name(Name_), SpikesOn(SpikesOn_)
            { }

        /// @brief Empty Virtual Deconstructor
        virtual ~PiMakerWorkUnit()
            { }

        /// @brief Calculate Pi and log it.
        /// @paramCurrentThreadStorage used to retrieve a valid logger.
        /// @brief CurrentFrameScheduler ignored
        virtual void DoWork(DefaultThreadSpecificStorage::Type& CurrentThreadStorage)
        {
            DoubleBufferedLogger& CurrentLogger = CurrentThreadStorage.GetResource<DoubleBufferedLogger>(DBRLogger);
            CurrentLogger.GetUsable() << "<MakePi Pi=\"" << MakePi(Length,SpikesOn) << "\" WorkUnitName=\"" << Name << "\" ThreadID=\"" << Mezzanine::Threading::this_thread::get_id() << "\" />" << endl;
        }
};

/// @brief Tests for the WorkUnit class
class workunittests : public UnitTestGroup
{
    public:
        /// @copydoc Mezzanine::Testing::UnitTestGroup::Name
        /// @return Returns a String containing "WorkUnit"
        virtual String Name()
            { return String("WorkUnit"); }

        /// @copydoc Mezzanine::Testing::UnitTestGroup::RunTests
        /// @detail Mostly tests whether or not dependencies are tracked correctly.
        void RunAutomaticTests()
        {
            cout << "Starting WorkUnit Tests, 20 runs with WorkUnitSample1" << endl;
            PiMakerWorkUnit WorkUnitSample1(5000,"WorkUnitSample1",false);
            FrameScheduler TestScheduler(&cout,1);
            Mezzanine::Threading::DefaultThreadSpecificStorage::Type TestThreadStorage(&TestScheduler);
            // run work unit
            for(Whole Counter=0; Counter<20; Counter++)
                { WorkUnitSample1(TestThreadStorage); }
            cout << "Here is the complete log of Twenty Test Runs" << endl
                 << TestThreadStorage.GetResource<DoubleBufferedLogger>(DBRLogger).GetUsable().str() // << endl // logs ends with a newline
                 << "Average Execution Time (Microseconds): " << WorkUnitSample1.GetPerformanceLog().GetAverage() << endl;

            cout << endl << "Starting WorkUnit Dependent and Dependency count Tests. Creating a chain in which C depends on B which depends on A."
                 << endl << "C --> B --> A" << endl;
            PiMakerWorkUnit* WorkUnitA = new PiMakerWorkUnit(50,"A",false);
            PiMakerWorkUnit* WorkUnitB = new PiMakerWorkUnit(50,"B",false);
            PiMakerWorkUnit* WorkUnitC = new PiMakerWorkUnit(50,"C",false);
            WorkUnitC->AddDependency(WorkUnitB);
            WorkUnitB->AddDependency(WorkUnitA);
            TestScheduler.AddWorkUnitMain(WorkUnitA);
            TestScheduler.AddWorkUnitMain(WorkUnitB);
            TestScheduler.AddWorkUnitMain(WorkUnitC);
            TestScheduler.UpdateDependentGraph();
            cout << "A dependency count: " << WorkUnitA->GetDependencyCount() << " \t A dependent count: " << WorkUnitA->GetDependentCount(TestScheduler) << endl;
            cout << "B dependency count: " << WorkUnitB->GetDependencyCount() << " \t B dependent count: " << WorkUnitB->GetDependentCount(TestScheduler) << endl;
            cout << "C dependency count: " << WorkUnitC->GetDependencyCount() << " \t C dependent count: " << WorkUnitC->GetDependentCount(TestScheduler) << endl;
            TEST(WorkUnitA->GetDependencyCount()==0,"ADependencyCount");
            TEST(WorkUnitA->GetDependentCount(TestScheduler)==2,"ADependentCount");
            TEST(WorkUnitB->GetDependencyCount()==1,"BDependencyCount");
            TEST(WorkUnitB->GetDependentCount(TestScheduler)==1,"BDependentCount");
            TEST(WorkUnitC->GetDependencyCount()==2,"CDependencyCount");
            TEST(WorkUnitC->GetDependentCount(TestScheduler)==0,"CDependentCount");

            cout << "Creating a WorkUnit D which depends on B, So we should have:"
                    << endl << "D --"
                    << endl << "   |"
                    << endl << "   v"
                    << endl << "   B --> A"
                    << endl << "   ^"
                    << endl << "   |"
                    << endl << "C --" << endl;
            PiMakerWorkUnit* WorkUnitD = new PiMakerWorkUnit(50,"D",false);
            WorkUnitD->AddDependency(WorkUnitB);
            TestScheduler.AddWorkUnitMain(WorkUnitD);
            TestScheduler.UpdateDependentGraph();
            cout << "A dependency count: " << WorkUnitA->GetDependencyCount() << " \t A dependent count: " << WorkUnitA->GetDependentCount(TestScheduler) << endl;
            cout << "B dependency count: " << WorkUnitB->GetDependencyCount() << " \t B dependent count: " << WorkUnitB->GetDependentCount(TestScheduler) << endl;
            cout << "C dependency count: " << WorkUnitC->GetDependencyCount() << " \t C dependent count: " << WorkUnitC->GetDependentCount(TestScheduler) << endl;
            cout << "D dependency count: " << WorkUnitD->GetDependencyCount() << " \t D dependent count: " << WorkUnitD->GetDependentCount(TestScheduler) << endl;
            TEST(WorkUnitA->GetDependencyCount()==0,"A2DependencyCount");
            TEST(WorkUnitA->GetDependentCount(TestScheduler)==3,"A2DependentCount");
            TEST(WorkUnitB->GetDependencyCount()==1,"B2DependencyCount");
            TEST(WorkUnitB->GetDependentCount(TestScheduler)==2,"B2DependentCount");
            TEST(WorkUnitC->GetDependencyCount()==2,"C2DependencyCount");
            TEST(WorkUnitC->GetDependentCount(TestScheduler)==0,"C2DependentCount");
            TEST(WorkUnitD->GetDependencyCount()==2,"D2DependencyCount");
            TEST(WorkUnitD->GetDependentCount(TestScheduler)==0,"D2DependentCount");
        }

        /// @brief Since RunAutomaticTests is implemented so is this.
        /// @return returns true
        virtual bool HasAutomaticTests() const
            { return true; }
};

#endif

