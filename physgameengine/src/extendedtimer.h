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
#ifndef _extendedtimer_h
#define _extendedtimer_h

#include "timer.h"

namespace phys
{
    ///////////////////////////////////////////////////////////////////////////////
    /// @struct Time
    /// @headerfile timer.h
    /// @brief A container for the metrics of time relevant for the timer class.
    /// @details
    ///////////////////////////////////////
    struct Time
    {
        Integer Microseconds;
        Integer Milliseconds;
        Integer Seconds;
        Integer Minutes;
        Integer Hours;
        Integer Days;

        Time() : Microseconds(0),Milliseconds(0),Seconds(0),Minutes(0),Hours(0),Days(0) {};
        ~Time() {};
    };

    ///////////////////////////////////////////////////////////////////////////////
    /// @class ExtendedTimer
    /// @headerfile extendedtimer.h
    /// @brief An enhanced timer class that can store and track many units of time.
    /// @details This timer class should be used only for longer periods, such as tracking game uptime.
    /// For shorter periods of time look at the SimpleTimer class.
    ///////////////////////////////////////
    class PHYS_LIB ExtendedTimer : public Timer
    {
        public:
            /// @enum The internal time struct to be used. @n @n
            ///
            enum TimeStruct
            {
                Current,
                Goal,
                Initial
            };
        protected:
            Time CurrentTime;
            Time GoalTime;
            Time InitialTime;
            virtual Time& GetStruct(const ExtendedTimer::TimeStruct Struct);
            virtual void Update(const Whole MicroSecondsElapsed);
            virtual void UpdateAsNormal(const Whole MicroSecondsElapsed);
            virtual void UpdateAsStopWatch(const Whole MicroSecondsElapsed);
            virtual bool CheckMicroSeconds(const ExtendedTimer::TimeStruct Struct);
            virtual bool CheckMilliSeconds(const ExtendedTimer::TimeStruct Struct);
            virtual bool CheckSeconds(const ExtendedTimer::TimeStruct Struct);
            virtual bool CheckMinutes(const ExtendedTimer::TimeStruct Struct);
            virtual bool CheckHours(const ExtendedTimer::TimeStruct Struct);
            virtual bool CheckDays(const ExtendedTimer::TimeStruct Struct);
            virtual bool CheckAll(const ExtendedTimer::TimeStruct Struct);
            virtual bool CompareCurrentAndGoal(const Integer Current, const Integer Goal);
            virtual bool GoalReached();
        public:
            /// @brief Standard Constructor.
            /// @param Style The styling/type of timer to be constructed.
            ExtendedTimer(const Timer::TimerStyle style);
            /// @brief Class destructor.
            virtual ~ExtendedTimer();
            /// @brief Sets the current values to their initial values.
            virtual void Reset();
            /// @brief Sets whether or not the Timer should reset if it reaches it's goal.  @n
            /// Ex. If a stopwatch reaches 0.
            /// @return Returns a reference to this timer.
            virtual ExtendedTimer& SetAutoReset(const bool AutoReset);
            /// @brief Sets the value for Microseconds of the specified struct.
            /// @details If a number greater then 999 is passed in, it will be reduced to 999.
            /// @return Returns a reference to this timer.
            /// @param MS The number of Microseconds to set.
            virtual ExtendedTimer& SetMicroseconds(Integer MS, const ExtendedTimer::TimeStruct Struct = ExtendedTimer::Current);
            /// @brief Sets the value for Milliseconds of the specified struct.
            /// @details If a number greater then 999 is passed in, it will be reduced to 999.
            /// @return Returns a reference to this timer.
            /// @param MS The number of Milliseconds to set.
            virtual ExtendedTimer& SetMilliseconds(Integer MS, const ExtendedTimer::TimeStruct Struct = ExtendedTimer::Current);
            /// @brief Sets the value for Seconds of the specified struct.
            /// @details If a number greater then 59 is passed in, it will be reduced to 59.
            /// @return Returns a reference to this timer.
            /// @param Sec The number of Seconds to set.
            virtual ExtendedTimer& SetSeconds(Integer Sec, const ExtendedTimer::TimeStruct Struct = ExtendedTimer::Current);
            /// @brief Sets the value for Minutes of the specified struct.
            /// @details If a number greater then 59 is passed in, it will be reduced to 59.
            /// @return Returns a reference to this timer.
            /// @param Min The number of Minutes to set.
            virtual ExtendedTimer& SetMinutes(Integer Min, const ExtendedTimer::TimeStruct Struct = ExtendedTimer::Current);
            /// @brief Sets the value for Hours of the specified struct.
            /// @details If a number greater then 23 is passed in, it will be reduced to 23.
            /// @return Returns a reference to this timer.
            /// @param Hr The number of Hours to set.
            virtual ExtendedTimer& SetHours(Integer Hr, const ExtendedTimer::TimeStruct Struct = ExtendedTimer::Current);
            /// @brief Sets the value for Days of the specified struct.
            /// @return Returns a reference to this timer.
            /// @param Day The number of Days to set.
            virtual ExtendedTimer& SetDays(Integer Day, const ExtendedTimer::TimeStruct Struct = ExtendedTimer::Current);
            /// @brief Gets the current internal Millisecond count.
            /// @return Returns an Integer representing the current Millisecond count of this Timer.
            virtual Integer GetMilliseconds(const ExtendedTimer::TimeStruct Struct = ExtendedTimer::Current);
            /// @brief Gets the current internal Second count.
            /// @return Returns an Integer representing the current Second count of this Timer.
            virtual Integer GetSeconds(const ExtendedTimer::TimeStruct Struct = ExtendedTimer::Current);
            /// @brief Gets the current internal Minute count.
            /// @return Returns an Integer representing the current Minute count of this Timer.
            virtual Integer GetMinutes(const ExtendedTimer::TimeStruct Struct = ExtendedTimer::Current);
            /// @brief Gets the current internal Hour count.
            /// @return Returns an Integer representing the current Hour count of this Timer.
            virtual Integer GetHours(const ExtendedTimer::TimeStruct Struct = ExtendedTimer::Current);
            /// @brief Gets the current internal Day count.
            /// @return Returns an Integer representing the current Day count of this Timer.
            virtual Integer GetDays(const ExtendedTimer::TimeStruct Struct = ExtendedTimer::Current);
    };
}

#endif
