// © Copyright 2010 - 2013 BlackTopp Studios Inc.
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
#ifndef _timer_h
#define _timer_h

#include "datatypes.h"

namespace Mezzanine
{
    ///////////////////////////////////////////////////////////////////////////////
    /// @class Timer
    /// @headerfile timer.h
    /// @brief A base timer class for the different timers.
    /// @details
    ///////////////////////////////////////
    class MEZZ_LIB Timer
    {
    public:
        /// @brief The style of timer to be used.
        enum TimerType
        {
            Normal,     ///< Counts up forever, ignoring any goal and autoreset
            StopWatch,  ///< Counts down, respecting any goal and autoreset
            Alarm       ///< Counts up, respecting any goal and autoreset
        };
    protected:
        /// @internal
        /// @brief The time stamp from when the timer first started tracking time.
        MaxInt StartStamp;
        /// @internal
        /// @brief The current amount of microseconds that has elapsed since starting to track time.
        MaxInt CurrentTime;
        /// @internal
        /// @brief The amount of microseconds elapsed that is considered the starting point for tracking time.
        MaxInt InitialTime;
        /// @internal
        /// @brief Updates all the timings in this timer.
        virtual void Update();
    public:
        /// @brief Standard Constructor.
        /// @param Style The styling/type of timer to be constructed.
        Timer();
        /// @brief Class Destructor.
        virtual ~Timer();

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        /// @brief Sets the current time in Microseconds.
        /// @return Returns a reference to this timer.
        /// @param Current The value to set as current time in Microseconds.
        virtual void SetCurrentTime(const Whole Current);
        /// @brief Sets the current time in Milliseconds. The time that resetting sets the timer to.
        /// @return Returns a reference to this timer.
        /// @param Current The value to set as current time in Milliseconds.
        virtual void SetCurrentTimeInMilliseconds(const Whole Current);
        /// @brief Gets the Current time in Microseconds.
        /// @return Returns a Whole representing the current time in Microseconds.
        virtual Whole GetCurrentTime();
        /// @brief Gets the Current time in Milliseconds.
        /// @return Returns a Whole representing the current time in Milliseconds.
        virtual Whole GetCurrentTimeInMilliseconds();
        /// @brief Sets the initial time in Microseconds. The time that resetting sets the timer to.
        /// @return Returns a reference to this timer.
        /// @param Initial The value to set as initial time in Microseconds.
        virtual void SetInitialTime(const Whole Initial);
        /// @brief Sets the initial time in Milliseconds. The time that resetting sets the timer to.
        /// @return Returns a reference to this timer.
        /// @param Initial The value to set as initial time in Milliseconds.
        virtual void SetInitialTimeInMilliseconds(const Whole Initial);
        /// @brief Gets the Initial time in Microseconds.
        /// @return Returns a Whole representing the initial time in Microseconds.
        virtual Whole GetInitialTime() const;
        /// @brief Gets the Initial time in Milliseconds.
        /// @return Returns a Whole representing the initial time in Milliseconds.
        virtual Whole GetInitialTimeInMilliseconds() const;

        /// @brief Activates the Timer.
        virtual void Start();
        /// @brief Deactivates the Timer.
        virtual void Stop();
        /// @brief Gets Whether or not this timer is currently running.
        /// @return Returns true if this timer is not currently active, false otherwise.
        virtual Bool IsStopped();
        /// @brief Sets the current values to their initial values.
        virtual void Reset();
        /// @brief Gets the type of timer this is.
        /// @return Returns an enum value representing the type of timer this is.
        virtual Timer::TimerType GetType() const;
    };//Timer

    ///////////////////////////////////////////////////////////////////////////////
    /// @brief This is the base class for timers intended to run only until they reach a specific goal.
    /// @details
    ///////////////////////////////////////
    class MEZZ_LIB GoalTimer : public Timer
    {
    protected:
        /// @internal
        /// @brief The time this timer should stop at.
        MaxInt GoalTime;
        /// @internal
        /// @brief Wether or not this timer will reset itself when it reaches it's goal.
        Bool ResetAtGoal;
        /// @internal
        /// @brief Checks to see if the goal has been attained as dictated by the type of timer this is.
        virtual Bool GoalReached() = 0;
    public:
        /// @brief Class constructor.
        GoalTimer();
        /// @brief Class destructor.
        virtual ~GoalTimer();

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        /// @brief Sets whether or not this Timer should reset if it reaches it's goal.
        /// @param AutoReset Should be true if you want this timer to reset itself back to it's initial time when it reaches it's goal.
        virtual void SetAutoReset(const bool AutoReset);
        /// @brief Gets whether or not this Timer will reset when it reaches it's goal.
        /// @return Returns true if this timer will automatically reset when it reaches it's goal.
        virtual Bool GetAutoReset() const;

        /// @brief Sets the goal time in Microseconds.
        /// @param Goal The value to set as goal time in Microseconds.
        virtual void SetGoalTime(const Whole Goal);
        /// @brief Sets the goal time in Milliseconds.
        /// @param Goal The value to set as goal time in Milliseconds.
        virtual void SetGoalTimeInMilliseconds(const Whole Goal);
        /// @brief Gets the Goal time in Microseconds.
        /// @return Returns a Whole representing the goal time in Microseconds.
        virtual Whole GetGoalTime() const;
        /// @brief Gets the Goal time in Milliseconds.
        /// @return Returns a Whole representing the goal time in Milliseconds.
        virtual Whole GetGoalTimeInMilliseconds() const;
    };//GoalTimer

    ///////////////////////////////////////////////////////////////////////////////
    /// @brief This is a timer class for counting down to a specified time.
    /// @details
    ///////////////////////////////////////
    class MEZZ_LIB StopWatchTimer : public GoalTimer
    {
    protected:
        /// @copydoc Timer::Update()
        virtual void Update();
        /// @copydoc GoalTimer::GoalReached()
        virtual Bool GoalReached();
    public:
        /// @brief Class constructor.
        StopWatchTimer();
        /// @brief Class destructor.
        virtual ~StopWatchTimer();

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        /// @copydoc Timer::GetType() const
        virtual Timer::TimerType GetType() const;
    };//StopWatchTimer

    ///////////////////////////////////////////////////////////////////////////////
    /// @brief This is a timer class that increments normally to a specified time.
    /// @details
    ///////////////////////////////////////
    class MEZZ_LIB AlarmTimer : public GoalTimer
    {
    protected:
        /// @copydoc Timer::Update()
        virtual void Update();
        /// @copydoc GoalTimer::GoalReached()
        virtual Bool GoalReached();
    public:
        /// @brief Class constructor.
        AlarmTimer();
        /// @brief Class destructor.
        virtual ~AlarmTimer();

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        /// @copydoc Timer::GetType() const
        virtual Timer::TimerType GetType() const;
    };//AlarmTimer
}//Mezzanine

#endif
