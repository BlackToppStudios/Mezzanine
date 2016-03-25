// © Copyright 2010 - 2016 BlackTopp Studios Inc.
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

namespace
{
    Mezzanine::MaxInt CountUpFunction(const Mezzanine::MaxInt Current, const Mezzanine::MaxInt Delta)
    {
        return Current + Delta;
    }

    Mezzanine::MaxInt CountDownFunction(const Mezzanine::MaxInt Current, const Mezzanine::MaxInt Delta)
    {
        Mezzanine::MaxInt Result = Current - Delta;
        if( Result < 0 )
            Result = 0;
        return Result;
    }
}

namespace Mezzanine
{
    Timer::Timer() :
        LastStamp(0),
        CurrentTime(0)
        { this->TimerCounter = &CountUpFunction; }

    Timer::~Timer()
        {  }

    MaxInt Timer::UpdateTime()
    {
        if( this->LastStamp != 0 ) {
            MaxInt CurrentStamp = crossplatform::GetTimeStamp();
            this->CurrentTime = this->TimerCounter(this->CurrentTime,CurrentStamp - this->LastStamp);
            this->LastStamp = CurrentStamp;

            // If we've reached the end of a countdown then stop the timer.  But don't actually call Stop(), as that will cause an infinite loop.
            if( this->CurrentTime == 0 && this->GetCountMode() == Mezzanine::CM_CountDown )
                this->LastStamp = 0;
        }
        return this->CurrentTime;
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Utility

    void Timer::SetCurrentTime(const Whole Current)
        { this->CurrentTime = static_cast<MaxInt>( Current ); }

    void Timer::SetCurrentTimeInMilliseconds(const Whole Current)
        { this->CurrentTime = static_cast<MaxInt>( Current ) * 1000; }

    Whole Timer::GetCurrentTime()
        { return static_cast<Whole>( this->UpdateTime() ); }

    Whole Timer::GetCurrentTimeInMilliseconds()
        { return this->GetCurrentTime() * 0.001; }

    void Timer::SetCountMode(const Mezzanine::CountMode Mode)
    {
        if( Mode == Mezzanine::CM_CountDown ) this->TimerCounter = &CountDownFunction;
        else this->TimerCounter = &CountUpFunction;
    }

    Mezzanine::CountMode Timer::GetCountMode() const
    {
        if( this->TimerCounter == &CountDownFunction ) return Mezzanine::CM_CountDown;
        else return Mezzanine::CM_CountUp;
    }

    void Timer::Start()
    {
        if( this->LastStamp == 0 )
            this->LastStamp = crossplatform::GetTimeStamp();
    }

    void Timer::Stop()
    {
        if( this->LastStamp != 0 ) {
            this->UpdateTime();
            this->LastStamp = 0;
        }
    }

    void Timer::Reset(const Whole StartTime)
    {
        this->LastStamp = crossplatform::GetTimeStamp();
        this->SetCurrentTime(StartTime);
    }

    Boole Timer::IsTicking()
    {
        this->UpdateTime();
        return ( this->LastStamp != 0 );
    }

    Boole Timer::IsStopped()
    {
        this->UpdateTime();
        return ( this->LastStamp == 0 );
    }

    String Timer::GetTimeAsText(const Mezzanine::TimeFormat Format)
    {
        StringStream TimeStream;
        switch( Format )
        {
            case Mezzanine::TF_RawMicro:
            {
                TimeStream << this->GetCurrentTime();
                break;
            }
            case Mezzanine::TF_RawMilli:
            {
                TimeStream << this->GetCurrentTimeInMilliseconds();
                break;
            }
            case Mezzanine::TF_Seconds:
            {
                TimeStream << ( this->GetCurrentTimeInMilliseconds() / 1000 );
                break;
            }
            case Mezzanine::TF_SecondsMilli:
            {
                Whole Milli = this->GetCurrentTimeInMilliseconds();
                Whole Seconds = ( Milli / 1000 );
                Whole Remainder = ( Milli % 1000 );
                if( Remainder < 10 ) {
                    TimeStream << Seconds << ".00" << Remainder;
                }else if( Remainder < 100 ) {
                    TimeStream << Seconds << ".0" << Remainder;
                }else{
                    TimeStream << Seconds << "." << Remainder;
                }
                break;
            }
            case Mezzanine::TF_MinutesSeconds:
            {
                Whole Seconds = ( this->GetCurrentTimeInMilliseconds() / 1000 );
                Whole Minutes = Seconds / 60;
                Whole Remainder = Seconds % 60;
                if( Remainder < 10 ) {
                    TimeStream << Minutes << ":0" << Remainder;
                }else{
                    TimeStream << Minutes << ":" << Remainder;
                }
                break;
            }
            default:
            {
                break;
            }
        }
        return TimeStream.str();
    }
}//Mezzanine

#endif
