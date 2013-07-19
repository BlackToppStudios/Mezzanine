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
#ifndef _monopolytests_h
#define _monopolytests_h

#include "mezztest.h"

#include "dagframescheduler.h"
#include "workunittests.h"

/// @file
/// @brief Basic tests of the MonopolyWorkUnit

using namespace std;
using namespace Mezzanine;
using namespace Mezzanine::Testing;
using namespace Mezzanine::Threading;

// forward declaration
class PiMakerMonopoly;

/// @brief Used to pass and accept data from the Pi calculating function with multiple threads.
/// @details Used in @ref MonopolyTest
/// @param PiMakerMonopoly_ a PiMakerMonopoly void pointer that provides all the required meta data for its execution
void PiMakerMonopolyHelper(void* PiMakerMonopoly_);

/// @brief The data a single thread in the Pi make monopoly needs.
struct PiMakerThreadData
{
    /// @brief A pointer to the one Pi maker monopoly the target thread cares about
    PiMakerMonopoly *Maker;

    /// @brief The Resources the target thread should use.
    DefaultThreadSpecificStorage::Type *Storage;

    /// @brief constructor
    /// @param Maker_ A pointer to a PiMakerMonopoly that the targe
    explicit PiMakerThreadData(PiMakerMonopoly *Maker_) : Maker(Maker_), Storage(new DefaultThreadSpecificStorage::Type(0)) // This is not exception safe, if we ran out of memory here this could leak.1
        {}

    /// @brief Deletes allocated data.
    ~PiMakerThreadData()
        { delete Storage; }
};

/// @brief PiMakerMonopoly Consume every thread for a brief period to calculate Pi.
/// @details Used in @ref MonopolyTest
/// @warning Everything on these samples has a public access specifier, for production code that is poor form, encapsulate your stuff.
class PiMakerMonopoly : public MonopolyWorkUnit
{
    public:
        /// @brief How many iterations will we use when calculating Pi
        Mezzanine::Whole Length;

        /// @brief Name used for clarifying output and enhancing logging.
        Mezzanine::String Name;

        /// @brief should this randomly spike execution time.
        bool SpikesOn;

        /// @brief  Count of thread to execute in
        Mezzanine::Whole HowManyThreads;

        /// @brief Set the amount of threads to use
        /// @param AmountToUse A whole indicating the amount of times Pi should uselessly be calculated in parrallel
        virtual void UseThreads(const Whole& AmountToUse)
            { HowManyThreads = AmountToUse; }

        /// @brief Get the current thread count
        /// @return A Whole
        virtual Whole UsingThreadCount()
            { return HowManyThreads; }

        /// @brief Constructor
        /// @param Length_ Defaults to 50 and is the number of iterations to use, more means slower execution
        /// @param Name Defaults to "Default" only used in output logs
        /// @param SpikesOn_ Defaults to true, used to make execution more dificult to predict
        /// @param AmountToUse Defaults to 1
        PiMakerMonopoly(Mezzanine::Whole Length_ = 50, Mezzanine::String Name_ = "Default", bool SpikesOn_=true, Whole AmountToUse = 1)
            : Length(Length_), Name(Name_), SpikesOn(SpikesOn_), HowManyThreads(AmountToUse)
        { }

        /// @brief Spawns several the amount of threads indicated by HowManyThreads then calculates Pi in each and logs teh results
        /// @param CurrentFrameScheduler
        virtual void DoWork(DefaultThreadSpecificStorage::Type&)
        {
            vector<Thread*> ThreadIndex;
            vector<PiMakerThreadData*> ThreadData;
            for (Whole Count=0; Count<HowManyThreads; ++Count)      // Pretend making all this Pi simulates everyone in a Bakery baking at at once as hard as they can
            {
                PiMakerThreadData* Data = new PiMakerThreadData(this);
                ThreadData.push_back( Data );
                ThreadIndex.push_back( new Mezzanine::Threading::Thread (PiMakerMonopolyHelper, Data) );
            }
            for (Whole Count=0; Count<HowManyThreads; ++Count)
            {
                ThreadIndex[Count]->join();
                delete ThreadIndex[Count];
                delete ThreadData[Count];
            }
            //CurrentThreadStorage.GetUsableLogger() << "CalculatingPI in thread" << this_thread::get_id() << endl;
        }
};

// Documented above at prototype line.
void PiMakerMonopolyHelper(void* Data)
{
    PiMakerThreadData* D = (PiMakerThreadData*)Data;
    PiMakerMonopoly* Mono = D->Maker;
    Logger& CurrentLog = D->Storage->GetUsableLogger();
    CurrentLog << "<PiMakerMonopolyHelper Pi=\"" << MakePi(Mono->Length,Mono->SpikesOn) << "\" WorkUnitName=\"" << Mono->Name<< "\" ThreadID=\"" << Mezzanine::Threading::this_thread::get_id() << "\" />" << endl;
    MakePi(Mono->Length);
}


/// @brief Tests for the monopoly
class monopolytests : public UnitTestGroup
{
    public:
        /// @copydoc Mezzanine::Testing::UnitTestGroup::Name
        /// @return Returns a String containing "Monopoly"
        virtual String Name()
            { return String("Monopoly"); }

        /// @brief Only a smoke test, to see if this compiles
        virtual void RunAutomaticTests()
        {
            cout << "Starting MonopolyWorkUnit test. Creating a monopoly that will calculate pi in a number of threads simultaneously." << endl;
            PiMakerMonopoly Pioply(50,"Pioply",false,4);
            FrameScheduler TestSchedulerMono(&cout,1);
            DefaultThreadSpecificStorage::Type PioplyStorage(&TestSchedulerMono);
            for(Whole Counter=0; Counter<20; Counter++)
                { Pioply(PioplyStorage); }
            cout << "Here is the un-aggregated (main thread only) log of Twenty Test Runs" << endl
                 << PioplyStorage.GetResource<DoubleBufferedLogger>(DBRLogger).GetUsable().str() // << endl // logs ends with a newline
                 << "Average Execution Time (Microseconds): " << Pioply.GetPerformanceLog().GetAverage() << endl;
        }

        /// @brief Since RunAutomaticTests is implemented so is this.
        /// @return returns false, because no tests are recorded.
        virtual bool HasAutomaticTests() const
            { return false; }

};

#endif

