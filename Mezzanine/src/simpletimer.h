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
#ifndef _simpletimer_h
#define _simpletimer_h

#include "timer.h"

namespace Mezzanine
{
    ///////////////////////////////////////////////////////////////////////////////
    /// @class SimpleTimer
    /// @headerfile simpletimer.h
    /// @brief A basic timer class that can be used to track short intervals of time.
    /// @details This timer class should be used only for shorter periods, such as tracking ability cooldowns or gun fire rates.
    /// For longer periods of time look at the EnhancedTimer class.
    ///////////////////////////////////////
    class SimpleTimer : public Timer
    {
        protected:
            Whole CurrentTime;
            Whole GoalTime;
            Whole InitialTime;
            virtual void Update(const Whole MicroSecondsElapsed);
            virtual void UpdateAsNormal(const Whole MicroSecondsElapsed);
            virtual void UpdateAsStopWatch(const Whole MicroSecondsElapsed);
            virtual void UpdateAsAlarm(const Whole MicroSecondsElapsed);
            virtual bool GoalReached();
        public:
            /// @brief Standard Constructor.
            /// @param Style The styling/type of timer to be constructed.
            SimpleTimer(const Timer::TimerStyle style);
            /// @brief Class destructor.
            virtual ~SimpleTimer();
            /// @brief Sets the current value to it's initial value.
            virtual void Reset();
            /// @brief Sets whether or not the Timer should reset if it reaches it's goal.  @n
            /// Ex. If a stopwatch reaches 0.
            /// @return Returns a reference to this timer.
            virtual SimpleTimer& SetAutoReset(const bool AutoReset);
            /// @brief Sets the current time in Microseconds.
            /// @return Returns a reference to this timer.
            /// @param Current The value to set as current time in Microseconds.
            virtual SimpleTimer& SetCurrentTime(const Whole Current);
            /// @brief Sets the goal time in Microseconds.
            /// @return Returns a reference to this timer.
            /// @param Goal The value to set as goal time in Microseconds.
            virtual SimpleTimer& SetGoalTime(const Whole Goal);
            /// @brief Sets the initial time in Microseconds. The time that resetting sets the timer to.
            /// @return Returns a reference to this timer.
            /// @param Initial The value to set as initial time in Microseconds.
            virtual SimpleTimer& SetInitialTime(const Whole Initial);
            /// @brief Gets the Current time in Microseconds.
            /// @return Returns a Whole representing the current time in Microseconds.
            virtual Whole GetCurrentTime();
            /// @brief Gets the Goal time in Microseconds.
            /// @return Returns a Whole representing the goal time in Microseconds.
            virtual Whole GetGoalTime();
            /// @brief Gets the Initial time in Microseconds.
            /// @return Returns a Whole representing the initial time in Microseconds.
            virtual Whole GetInitialTime();
            /// @brief Gets the Current time in Milliseconds.
            /// @return Returns a Whole representing the current time in Milliseconds.
            virtual Whole GetCurrentTimeInMilli();
            /// @brief Gets the Goal time in Milliseconds.
            /// @return Returns a Whole representing the goal time in Milliseconds.
            virtual Whole GetGoalTimeInMilli();
            /// @brief Gets the Initial time in Milliseconds.
            /// @return Returns a Whole representing the initial time in Milliseconds.
            virtual Whole GetInitialTimeInMilli();
    };
}

#endif
