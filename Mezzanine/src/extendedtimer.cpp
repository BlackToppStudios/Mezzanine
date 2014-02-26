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
#ifndef _extendedtimer_cpp
#define _extendedtimer_cpp

#include "extendedtimer.h"
#include "eventmanager.h"

namespace Mezzanine
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
            default:
                MEZZ_EXCEPTION(Exception::INVALID_STATE_EXCEPTION,"Invalid Timer type acquisition");
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
                    break;
                }
            }
        }
    }

    void ExtendedTimer::UpdateAsNormal(const Whole MicroSecondsElapsed)
    {
        CurrentTime.Microseconds+=(Integer)MicroSecondsElapsed;
        if(!CheckMicroSeconds(ExtendedTimer::Current)) return;
        if(!CheckMilliSeconds(ExtendedTimer::Current)) return;
        if(!CheckSeconds(ExtendedTimer::Current)) return;
        if(!CheckMinutes(ExtendedTimer::Current)) return;
        if(!CheckHours(ExtendedTimer::Current)) return;
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
        }else{ return; /*Entresol::GetSingletonPtr()->Log("Holy **** that was a fast frame!");*/ }
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

    Boole ExtendedTimer::CheckMicroSeconds(const ExtendedTimer::TimeStruct Struct)
    {
        if(GetStruct(Struct).Microseconds > 1000)
        {
            Integer GoesInto = GetStruct(Struct).Microseconds * 0.001;
            Integer Remainder = GetStruct(Struct).Microseconds - GoesInto * 1000;
            GetStruct(Struct).Microseconds = Remainder;
            GetStruct(Struct).Milliseconds+=GoesInto;
            return true;
        }else{ return false; }
    }

    Boole ExtendedTimer::CheckMilliSeconds(const ExtendedTimer::TimeStruct Struct)
    {
        if(GetStruct(Struct).Milliseconds > 1000)
        {
            Integer GoesInto = GetStruct(Struct).Milliseconds * 0.001;
            Integer Remainder = GetStruct(Struct).Milliseconds - GoesInto * 1000;
            GetStruct(Struct).Milliseconds = Remainder;
            GetStruct(Struct).Seconds+=GoesInto;
            return true;
        }else{ return false; }
    }

    Boole ExtendedTimer::CheckSeconds(const ExtendedTimer::TimeStruct Struct)
    {
        if(GetStruct(Struct).Seconds > 60)
        {
            Integer GoesInto = GetStruct(Struct).Seconds / 60;
            Integer Remainder = GetStruct(Struct).Seconds - GoesInto * 60;
            GetStruct(Struct).Seconds = Remainder;
            GetStruct(Struct).Minutes+=GoesInto;
            return true;
        }else{ return false; }
    }

    Boole ExtendedTimer::CheckMinutes(const ExtendedTimer::TimeStruct Struct)
    {
        if(GetStruct(Struct).Minutes > 60)
        {
            Integer GoesInto = GetStruct(Struct).Minutes / 60;
            Integer Remainder = GetStruct(Struct).Minutes - GoesInto * 60;
            GetStruct(Struct).Minutes = Remainder;
            GetStruct(Struct).Hours+=GoesInto;
            return true;
        }else{ return false; }
    }

    Boole ExtendedTimer::CheckHours(const ExtendedTimer::TimeStruct Struct)
    {
        if(GetStruct(Struct).Hours > 24)
        {
            Integer GoesInto = GetStruct(Struct).Hours / 24;
            Integer Remainder = GetStruct(Struct).Hours - GoesInto * 24;
            GetStruct(Struct).Hours = Remainder;
            GetStruct(Struct).Days+=GoesInto;
            return true;
        }else{ return false; }
    }

    Boole ExtendedTimer::CheckDays(const ExtendedTimer::TimeStruct Struct)
    {
        return false;
    }

    Boole ExtendedTimer::CheckAll(const ExtendedTimer::TimeStruct Struct)
    {
        if(CheckMicroSeconds(Struct) || CheckMilliSeconds(Struct) || CheckSeconds(Struct)
           || CheckMinutes(Struct) || CheckHours(Struct) || CheckDays(Struct))
        {
            return true;
        }else{ return false; }
    }

    Boole ExtendedTimer::CompareCurrentAndGoal(const Integer Current, const Integer Goal)
    {
        return Timer::StopWatch == Style ? Current < Goal : Current > Goal;
        /*if(Timer::StopWatch == Style)
        {
            return Current < Goal;
        }else{
            return Current > Goal;
        }*/
    }

    Boole ExtendedTimer::GoalReached()
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

    ExtendedTimer& ExtendedTimer::SetAutoReset(const Boole AutoReset)
    {
        ResetAtGoal = AutoReset;
        return *this;
    }

    ExtendedTimer& ExtendedTimer::SetMicroseconds(Integer MS, const ExtendedTimer::TimeStruct Struct)
    {
        GetStruct(Struct).Microseconds = MS;
        CheckAll(Struct);
        return *this;
    }

    ExtendedTimer& ExtendedTimer::SetMilliseconds(Integer MS, const ExtendedTimer::TimeStruct Struct)
    {
        GetStruct(Struct).Milliseconds = MS;
        CheckMilliSeconds(Struct);
        CheckSeconds(Struct);
        CheckMinutes(Struct);
        CheckHours(Struct);
        return *this;
    }

    ExtendedTimer& ExtendedTimer::SetSeconds(Integer Sec, const ExtendedTimer::TimeStruct Struct)
    {
        GetStruct(Struct).Seconds = Sec;
        CheckSeconds(Struct);
        CheckMinutes(Struct);
        CheckHours(Struct);
        return *this;
    }

    ExtendedTimer& ExtendedTimer::SetMinutes(Integer Min, const ExtendedTimer::TimeStruct Struct)
    {
        GetStruct(Struct).Minutes = Min;
        CheckMinutes(Struct);
        CheckHours(Struct);
        return *this;
    }

    ExtendedTimer& ExtendedTimer::SetHours(Integer Hr, const ExtendedTimer::TimeStruct Struct)
    {
        GetStruct(Struct).Hours = Hr;
        CheckHours(Struct);
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
