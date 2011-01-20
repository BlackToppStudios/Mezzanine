//© Copyright 2010 BlackTopp Studios Inc.
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
#ifndef _extendedtimer_cpp
#define _extendedtimer_cpp

#include "extendedtimer.h"
#include "eventmanager.h"

namespace phys
{
    ExtendedTimer::ExtendedTimer(const Timer::TimerStyle style)
        : Timer(style)
    {
        Type = Timer::Extended;
    }

    ExtendedTimer::~ExtendedTimer()
    {
    }

    Time& ExtendedTimer::GetStruct(const ExtendedTimer::TimeStruct Struct)
    {
        switch(Struct)
        {
            case ExtendedTimer::Current:
                return CurrentTime;
                break;
            case ExtendedTimer::Goal:
                return GoalTime;
                break;
            case ExtendedTimer::Initial:
                return InitialTime;
                break;
        }
    }

    void ExtendedTimer::Update(const Whole MicroSecondsElapsed)
    {
        if(Active)
        {
            switch(Style)
            {
                case Timer::Normal:
                    UpdateAsNormal(MicroSecondsElapsed);
                    break;
                case Timer::StopWatch:
                {
                    UpdateAsStopWatch(MicroSecondsElapsed);
                    if(GoalReached())
                    {
                        if(Callback)
                            Callback->DoCallbackItems();
                        if(ResetAtGoal)
                            Reset();
                        Stop();
                    }
                    break;
                }
                case Timer::Alarm:
                    UpdateAsAlarm(MicroSecondsElapsed);
                    break;
            }
        }
    }

    void ExtendedTimer::UpdateAsNormal(const Whole MicroSecondsElapsed)
    {
        CurrentTime.Microseconds+=(Integer)MicroSecondsElapsed;
        if(CurrentTime.Microseconds > 1000)
        {
            Integer GoesInto = CurrentTime.Microseconds * 0.001;
            Integer Remainder = CurrentTime.Microseconds - GoesInto * 1000;
            CurrentTime.Microseconds = Remainder;
            CurrentTime.Milliseconds+=GoesInto;
        }else{ return; /*World::GetWorldPointer()->Log("Holy **** that was a fast frame!");*/ }
        if(CurrentTime.Milliseconds > 1000)
        {
            Integer GoesInto = CurrentTime.Milliseconds * 0.001;
            Integer Remainder = CurrentTime.Milliseconds - GoesInto * 1000;
            CurrentTime.Milliseconds = Remainder;
            CurrentTime.Seconds+=GoesInto;
        }else{ return; }
        if(CurrentTime.Seconds > 60)
        {
            Integer GoesInto = CurrentTime.Seconds / 60;
            Integer Remainder = CurrentTime.Seconds - GoesInto * 60;
            CurrentTime.Seconds = Remainder;
            CurrentTime.Minutes+=GoesInto;
        }else{ return; }
        if(CurrentTime.Minutes > 60)
        {
            Integer GoesInto = CurrentTime.Minutes / 60;
            Integer Remainder = CurrentTime.Minutes - GoesInto * 60;
            CurrentTime.Minutes = Remainder;
            CurrentTime.Hours+=GoesInto;
        }else{ return; }
        if(CurrentTime.Hours > 24)
        {
            Integer GoesInto = CurrentTime.Hours / 24;
            Integer Remainder = CurrentTime.Hours - GoesInto * 24;
            CurrentTime.Hours = Remainder;
            CurrentTime.Days+=GoesInto;
        }else{ return; }
    }

    void ExtendedTimer::UpdateAsStopWatch(const Whole MicroSecondsElapsed)
    {
        CurrentTime.Microseconds-=MicroSecondsElapsed;
        if(CurrentTime.Microseconds < 0)
        {
            Whole GoesInto = CurrentTime.Microseconds * 0.001;
            Whole Remainder = CurrentTime.Microseconds - GoesInto * 1000;
            CurrentTime.Microseconds = Remainder;
            CurrentTime.Milliseconds+=GoesInto;
        }else{ return; /*World::GetWorldPointer()->Log("Holy **** that was a fast frame!");*/ }
        if(CurrentTime.Milliseconds < 0)
        {
            Integer GoesInto = CurrentTime.Milliseconds * 0.001;
            Integer Remainder = CurrentTime.Milliseconds - GoesInto * 1000;
            CurrentTime.Milliseconds = Remainder;
            CurrentTime.Seconds+=GoesInto;
        }else{ return; }
        if(CurrentTime.Seconds < 0)
        {
            Integer GoesInto = CurrentTime.Seconds / 60;
            Integer Remainder = CurrentTime.Seconds - GoesInto * 60;
            CurrentTime.Seconds = Remainder;
            CurrentTime.Minutes+=GoesInto;
        }else{ return; }
        if(CurrentTime.Minutes < 0)
        {
            Integer GoesInto = CurrentTime.Minutes / 60;
            Integer Remainder = CurrentTime.Minutes - GoesInto * 60;
            CurrentTime.Minutes = Remainder;
            CurrentTime.Hours+=GoesInto;
        }else{ return; }
        if(CurrentTime.Hours < 0)
        {
            Integer GoesInto = CurrentTime.Hours / 24;
            Integer Remainder = CurrentTime.Hours - GoesInto * 24;
            CurrentTime.Hours = Remainder;
            CurrentTime.Days+=GoesInto;
        }else{ return; }
    }

    void ExtendedTimer::UpdateAsAlarm(const Whole MicroSecondsElapsed)
    {
        UpdateAsNormal(MicroSecondsElapsed);
        if(GoalReached())
        {
            if(Callback)
                Callback->DoCallbackItems();
            if(ResetAtGoal)
                Reset();
            Stop();
        }
    }

    bool ExtendedTimer::CompareCurrentAndGoal(const Integer Current, const Integer Goal)
    {
        return Timer::StopWatch == Style ? Current < Goal : Current > Goal;
        /*if(Timer::StopWatch == Style)
        {
            return Current < Goal;
        }else{
            return Current > Goal;
        }*/
    }

    bool ExtendedTimer::GoalReached()
    {
        if(CurrentTime.Days == GoalTime.Days)
        {
            if(CurrentTime.Hours == GoalTime.Hours)
            {
                if(CurrentTime.Minutes == GoalTime.Minutes)
                {
                    if(CurrentTime.Seconds == GoalTime.Seconds)
                    {
                        if(CurrentTime.Milliseconds == GoalTime.Milliseconds)
                        {
                            if(CurrentTime.Microseconds == GoalTime.Microseconds)
                            {
                                return true;
                            }
                            else if (CompareCurrentAndGoal(CurrentTime.Microseconds,GoalTime.Microseconds))
                            { return true; }else{ return false; }
                        }
                        else if (CompareCurrentAndGoal(CurrentTime.Milliseconds,GoalTime.Milliseconds))
                        { return true; }else{ return false; }
                    }
                    else if (CompareCurrentAndGoal(CurrentTime.Seconds,GoalTime.Seconds))
                    { return true; }else{ return false; }
                }
                else if (CompareCurrentAndGoal(CurrentTime.Minutes,GoalTime.Minutes))
                { return true; }else{ return false; }
            }
            else if (CompareCurrentAndGoal(CurrentTime.Hours,GoalTime.Hours))
            { return true; }else{ return false; }
        }
        else if (CompareCurrentAndGoal(CurrentTime.Days,GoalTime.Days))
        { return true; }else{ return false; }
    }

    void ExtendedTimer::Reset()
    {
        CurrentTime.Milliseconds = InitialTime.Milliseconds;
        CurrentTime.Seconds      = InitialTime.Seconds;
        CurrentTime.Minutes      = InitialTime.Minutes;
        CurrentTime.Hours        = InitialTime.Hours;
        CurrentTime.Days         = InitialTime.Days;
    }

    ExtendedTimer& ExtendedTimer::SetAutoReset(const bool AutoReset)
    {
        ResetAtGoal = AutoReset;
        return *this;
    }

    ExtendedTimer& ExtendedTimer::SetMicroseconds(Integer MS, const ExtendedTimer::TimeStruct Struct)
    {
        if(999 < MS) MS = 999;
        GetStruct(Struct).Microseconds = MS;
        return *this;
    }

    ExtendedTimer& ExtendedTimer::SetMilliseconds(Integer MS, const ExtendedTimer::TimeStruct Struct)
    {
        if(999 < MS) MS = 999;
        GetStruct(Struct).Milliseconds = MS;
        return *this;
    }

    ExtendedTimer& ExtendedTimer::SetSeconds(Integer Sec, const ExtendedTimer::TimeStruct Struct)
    {
        if(59 < Sec) Sec = 59;
        GetStruct(Struct).Seconds = Sec;
        return *this;
    }

    ExtendedTimer& ExtendedTimer::SetMinutes(Integer Min, const ExtendedTimer::TimeStruct Struct)
    {
        if(59 < Min) Min = 59;
        GetStruct(Struct).Minutes = Min;
        return *this;
    }

    ExtendedTimer& ExtendedTimer::SetHours(Integer Hr, const ExtendedTimer::TimeStruct Struct)
    {
        if(23 < Hr) Hr = 23;
        GetStruct(Struct).Hours = Hr;
        return *this;
    }

    ExtendedTimer& ExtendedTimer::SetDays(Integer Day, const ExtendedTimer::TimeStruct Struct)
    {
        GetStruct(Struct).Days = Day;
        return *this;
    }

    Integer ExtendedTimer::GetMilliseconds(const ExtendedTimer::TimeStruct Struct)
    {
        return GetStruct(Struct).Milliseconds;
    }

    Integer ExtendedTimer::GetSeconds(const ExtendedTimer::TimeStruct Struct)
    {
        return GetStruct(Struct).Seconds;
    }

    Integer ExtendedTimer::GetMinutes(const ExtendedTimer::TimeStruct Struct)
    {
        return GetStruct(Struct).Minutes;
    }

    Integer ExtendedTimer::GetHours(const ExtendedTimer::TimeStruct Struct)
    {
        return GetStruct(Struct).Hours;
    }

    Integer ExtendedTimer::GetDays(const ExtendedTimer::TimeStruct Struct)
    {
        return GetStruct(Struct).Days;
    }
}

#endif
