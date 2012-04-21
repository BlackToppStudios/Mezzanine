//© Copyright 2010 - 2012 BlackTopp Studios Inc.
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
#ifndef _simpletimer_cpp
#define _simpletimer_cpp

#include "simpletimer.h"

namespace Mezzanine
{
    SimpleTimer::SimpleTimer(const Timer::TimerStyle style)
        : Timer(style), CurrentTime(0), GoalTime(0), InitialTime(0)
    {
        Type = Timer::Simple;
    }

    SimpleTimer::~SimpleTimer()
    {
    }

    void SimpleTimer::Update(const Whole MicroSecondsElapsed)
    {
        if(Active)
        {
            switch(Style)
            {
                case Timer::Normal:
                    UpdateAsNormal(MicroSecondsElapsed);
                    break;
                case Timer::StopWatch:
                    UpdateAsStopWatch(MicroSecondsElapsed);
                    break;
                case Timer::Alarm:
                    UpdateAsAlarm(MicroSecondsElapsed);
                    break;
            }
        }
    }

    void SimpleTimer::UpdateAsNormal(const Whole MicroSecondsElapsed)
    {
        CurrentTime+=MicroSecondsElapsed;
    }

    void SimpleTimer::UpdateAsStopWatch(const Whole MicroSecondsElapsed)
    {
        int Negative = (int)CurrentTime - (int)MicroSecondsElapsed;
        if(0 > Negative)
        {
            CurrentTime = 0;
        }else{
            CurrentTime-=MicroSecondsElapsed;
        }
        if(GoalReached())
        {
            Stop();
            if(Callback)
                Callback->DoCallbackItems();
            if(ResetAtGoal)
                Reset();

        }
    }

    void SimpleTimer::UpdateAsAlarm(const Whole MicroSecondsElapsed)
    {
        UpdateAsNormal(MicroSecondsElapsed);
        if(GoalReached())
        {
            Stop();
            if(Callback)
                Callback->DoCallbackItems();
            if(ResetAtGoal)
                Reset();
        }
    }

    bool SimpleTimer::GoalReached()
    {
        return Timer::StopWatch == Style ? CurrentTime <= GoalTime : CurrentTime >= GoalTime;
    }

    void SimpleTimer::Reset()
    {
        CurrentTime = InitialTime;
    }

    SimpleTimer& SimpleTimer::SetAutoReset(const bool AutoReset)
    {
        ResetAtGoal = AutoReset;
        return *this;
    }

    SimpleTimer& SimpleTimer::SetCurrentTime(const Whole Current)
    {
        CurrentTime = Current;
        return *this;
    }

    SimpleTimer& SimpleTimer::SetGoalTime(const Whole Goal)
    {
        GoalTime = Goal;
        return *this;
    }

    SimpleTimer& SimpleTimer::SetInitialTime(const Whole Initial)
    {
        InitialTime = Initial;
        return *this;
    }

    Whole SimpleTimer::GetCurrentTime()
    {
        return CurrentTime;
    }

    Whole SimpleTimer::GetGoalTime()
    {
        return GoalTime;
    }

    Whole SimpleTimer::GetInitialTime()
    {
        return InitialTime;
    }

    Whole SimpleTimer::GetCurrentTimeInMilli()
    {
        return CurrentTime * 0.001;
    }

    Whole SimpleTimer::GetGoalTimeInMilli()
    {
        return GoalTime * 0.001;
    }

    Whole SimpleTimer::GetInitialTimeInMilli()
    {
        return InitialTime * 0.001;
    }
}

#endif
