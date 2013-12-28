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
#ifndef _timer_cpp
#define _timer_cpp

#include "timer.h"
#include "crossplatform.h"

namespace Mezzanine
{
    Timer::Timer() :
        StartStamp(0),
        CurrentTime(0),
        InitialTime(0)
        {  }

    Timer::~Timer()
        {  }

    void Timer::Update()
    {
        if( this->StartStamp != 0 ) {
            MaxInt CurrentStamp = crossplatform::GetTimeStamp();
            this->CurrentTime += (CurrentStamp - this->StartStamp);
            this->StartStamp = CurrentStamp;
        }
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Utility

    void Timer::SetCurrentTime(const Whole Current)
        { this->CurrentTime = static_cast<MaxInt>( Current ); }

    void Timer::SetCurrentTimeInMilliseconds(const Whole Current)
        { this->CurrentTime = static_cast<MaxInt>( Current ) * 1000; }

    Whole Timer::GetCurrentTime()
        { this->Update();  return static_cast<Whole>( this->CurrentTime ); }

    Whole Timer::GetCurrentTimeInMilliseconds()
        { return this->GetCurrentTime() * 0.001; }

    void Timer::SetInitialTime(const Whole Initial)
        { this->InitialTime = static_cast<MaxInt>( Initial ); }

    void Timer::SetInitialTimeInMilliseconds(const Whole Initial)
        { this->InitialTime = static_cast<MaxInt>( Initial ) * 1000; }

    Whole Timer::GetInitialTime() const
        { return static_cast<Whole>( this->InitialTime ); }

    Whole Timer::GetInitialTimeInMilliseconds() const
        { return this->GetInitialTime() * 0.001; }

    void Timer::Start()
    {
        if( this->IsStopped() ) {
            this->StartStamp = crossplatform::GetTimeStamp();
        }
    }

    void Timer::Stop()
    {
        if( !this->IsStopped() ) {
            this->StartStamp = 0;
        }
    }

    bool Timer::IsStopped()
    {
        this->Update();
        return this->StartStamp == 0;
    }

    void Timer::Reset()
    {
        this->StartStamp = crossplatform::GetTimeStamp();
        this->CurrentTime = this->InitialTime;
    }

    Timer::TimerType Timer::GetType() const
    {
        return Timer::Normal;
    }

    ///////////////////////////////////////////////////////////////////////////////
    // GoalTimer Methods

    GoalTimer::GoalTimer() :
        GoalTime(0),
        ResetAtGoal(false)
        {  }

    GoalTimer::~GoalTimer()
        {  }

    ///////////////////////////////////////////////////////////////////////////////
    // Utility

    void GoalTimer::SetAutoReset(const bool AutoReset)
        { this->ResetAtGoal = AutoReset; }

    Boolean GoalTimer::GetAutoReset() const
        { return this->ResetAtGoal; }

    void GoalTimer::SetGoalTime(const Whole Goal)
        { this->GoalTime = static_cast<MaxInt>( Goal ); }

    void GoalTimer::SetGoalTimeInMilliseconds(const Whole Goal)
        { this->GoalTime = static_cast<MaxInt>( Goal ) * 1000; }

    Whole GoalTimer::GetGoalTime() const
        { return static_cast<Whole>( this->GoalTime ); }

    Whole GoalTimer::GetGoalTimeInMilliseconds() const
        { return this->GetGoalTime() * 0.001; }

    ///////////////////////////////////////////////////////////////////////////////
    // StopWatchTimer Methods

    StopWatchTimer::StopWatchTimer()
        {  }

    StopWatchTimer::~StopWatchTimer()
        {  }

    void StopWatchTimer::Update()
    {
        if( this->StartStamp != 0 ) {
            MaxInt CurrentStamp = crossplatform::GetTimeStamp();
            if( CurrentStamp - this->StartStamp > this->CurrentTime ) {
                this->CurrentTime = 0;
            }else{
                this->CurrentTime -= ( CurrentStamp - this->StartStamp );
            }
            this->StartStamp = CurrentStamp;
        }

        if( this->GoalReached() ) {
            this->StartStamp = 0;
            if( this->ResetAtGoal ) {
                this->Reset();
            }
        }
    }

    Boolean StopWatchTimer::GoalReached()
        { return this->CurrentTime <= this->GoalTime; }

    ///////////////////////////////////////////////////////////////////////////////
    // Utility

    Timer::TimerType StopWatchTimer::GetType() const
        { return Timer::StopWatch; }

    ///////////////////////////////////////////////////////////////////////////////
    // AlarmTimer Methods

    AlarmTimer::AlarmTimer()
        {  }

    AlarmTimer::~AlarmTimer()
        {  }

    void AlarmTimer::Update()
    {
        this->Timer::Update();
        if( this->GoalReached() ) {
            this->StartStamp = 0;
            if( this->ResetAtGoal ) {
                this->Reset();
            }
        }
    }

    Boolean AlarmTimer::GoalReached()
        { return this->CurrentTime >= this->GoalTime; }

    ///////////////////////////////////////////////////////////////////////////////
    // Utility

    Timer::TimerType AlarmTimer::GetType() const
        { return Timer::Alarm; }
}//Mezzanine

#endif
