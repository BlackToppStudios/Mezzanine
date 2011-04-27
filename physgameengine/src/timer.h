//© Copyright 2010 - 2011 BlackTopp Studios Inc.
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
#ifndef _timer_h
#define _timer_h

#include "datatypes.h"

namespace phys
{
    class TimerManager;
    ///////////////////////////////////////////////////////////////////////////////
    /// @class TimerCallback
    /// @headerfile timer.h
    /// @brief A callback class for use automated and timed-event based timers.
    ///////////////////////////////////////
    class TimerCallback
    {
        protected:
        public:
            /// @brief Empty constructor
            TimerCallback() {};
            /// @brief Empty deconstructor
            ~TimerCallback() {};
            /// @brief The function called for this callback.
            virtual void DoCallbackItems() = 0;
    };

    ///////////////////////////////////////////////////////////////////////////////
    /// @class Timer
    /// @headerfile timer.h
    /// @brief A base timer class for the different timers.
    /// @details
    ///////////////////////////////////////
    class Timer
    {
        public:
            /// @brief The style of timer to be used.
            enum TimerStyle
            {
                Normal,     ///< Counts up forever, ignoring any goal and autoreset
                StopWatch,  ///< Counts down, respecting any goal and autoreset
                Alarm       ///< Counts up, respecting any goal and autoreset
            };

            /// @brief The type of timer to be used.
            enum TimerType
            {
                Simple,     ///< Measures only Micro Seconds, 1/1,000,000 of a second
                Extended    ///< Measures in any unit smaller than day and larger than Micro Seconds
            };
        protected:
            friend class TimerManager;
            Timer::TimerStyle Style;
            Timer::TimerType Type;
            bool IsSimple;
            bool ResetAtGoal;
            bool Active;
            TimerCallback* Callback;
            virtual void Update(const Whole MicroSecondsElapsed) = 0;
            virtual bool GoalReached() = 0;
        public:
            /// @brief Standard Constructor.
            /// @param Style The styling/type of timer to be constructed.
            Timer(const Timer::TimerStyle style) : Style(style), ResetAtGoal(false), Active(false), Callback(0) {};
            /// @brief Class Destructor.
            ~Timer()
            {
                if (Callback)
                    { delete Callback; }
            }
            /// @brief Sets the callback to be used with this timer.
            /// @details This function is called when a goal is reached on a Timer::StopWatch or a Timer::Alarm.
            /// @param Call The callback to be set.
            virtual void SetCallback(TimerCallback* Call) { Callback = Call; };
            /// @brief Activates the Timer.
            virtual void Start() { Active = true; };
            /// @brief Deactivates the Timer.
            virtual void Stop() { Active = false; };
            /// @brief Sets the current values to their initial values.
            virtual void Reset() = 0;
            /// @brief Gets the type of timer this is.
            /// @return Returns an enum value representing the type of timer this is.
            virtual Timer::TimerType GetType() { return Type; };
    };


}

#endif
