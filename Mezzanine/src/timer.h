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
#ifndef _timer_h
#define _timer_h

#include "datatypes.h"

namespace Mezzanine
{
    /// @brief An enum describing how the text output of a timer should be formatted.
    enum TimeFormat
    {
        TF_RawMicro       = 1, ///< Outputs the current time in microseconds.
        TF_RawMilli       = 2, ///< Outputs the current time in milliseconds.
        TF_Seconds        = 3, ///< Outputs the current time in seconds.
        TF_SecondsMilli   = 4, ///< Outputs the current time in "seconds.milliseconds".  Milliseconds are out to 3 digits.
        TF_MinutesSeconds = 5  ///< Outputs the current time in "minutes:seconds".
    };

    /// @brief A simple enum describing how the timer should increment time.
    enum CountMode
    {
        CM_CountUp       = 1,  ///< The timer counts up, meaning the Timer current time is showing time elapsed.
        CM_CountDown     = 2   ///< The timer counts down, meaning the Timer current time is showing remaining time.
    };

    ///////////////////////////////////////////////////////////////////////////////
    /// @brief A basic timer class to assist in timed operations.
    ///////////////////////////////////////
    class MEZZ_LIB Timer
    {
    protected:
        /// @internal
        /// @brief Convenience type for processing the current time.
        typedef MaxInt(*CountFunct)(const MaxInt, const MaxInt);

        /// @internal
        /// @brief The time stamp from when the last time the Timer was updated.
        MaxInt LastStamp;
        /// @internal
        /// @brief The current amount of microseconds that has elapsed since starting to track time.
        MaxInt CurrentTime;
        /// @internal
        /// @brief A pointer to the function currently doing the counting for this Timer.
        CountFunct TimerCounter;

        /// @internal
        /// @brief Updates the current time being tracked by this timer.
        /// @return Returns the updated current time.
        MaxInt UpdateTime();
    public:
        /// @brief Class Constructor.
        Timer();
        /// @brief Class Destructor.
        ~Timer();

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        /// @brief Sets the current time in Microseconds.
        /// @return Returns a reference to this Timer.
        /// @param Current The value to set as current time in Microseconds.
        void SetCurrentTime(const Whole Current);
        /// @brief Sets the current time in Milliseconds. The time that resetting sets the Timer to.
        /// @return Returns a reference to this Timer.
        /// @param Current The value to set as current time in Milliseconds.
        void SetCurrentTimeInMilliseconds(const Whole Current);
        /// @brief Gets the Current time in Microseconds.
        /// @return Returns a Whole representing the current time in Microseconds.
        Whole GetCurrentTime();
        /// @brief Gets the Current time in Milliseconds.
        /// @return Returns a Whole representing the current time in Milliseconds.
        Whole GetCurrentTimeInMilliseconds();

        /// @brief Sets the mode the timer should use to increment time.
        /// @param Mode The mode to be set, see the CountMode enum for more details.
        void SetCountMode(const Mezzanine::CountMode Mode);
        /// @brief Gets the mode the timer is using to increment time.
        /// @return Returns a CountMode enum value determining if this timer is counting up or down from it's current time.
        Mezzanine::CountMode GetCountMode() const;

        /// @brief Activates the Timer.
        void Start();
        /// @brief Deactivates the Timer.
        void Stop();
        /// @brief Sets the current time to an initial value and stops the Timer if it is running.
        /// @param StartTime The time in microseconds to be begin the Timer at the next time it is started.
        void Reset(const Whole StartTime = 0);
        /// @brief Gets whether or not this Timer is currently running.
        /// @return Returns true if this Timer is active, false if it is stopped.
        Boole IsTicking();
        /// @brief Gets whether or not this Timer is currently running.
        /// @return Returns true if this Timer is not currently active, false otherwise.
        Boole IsStopped();

        /// @brief Gets the current time of this Timer as a string.
        /// @param Format A TimeFormat enum value representing how the current time should be presented.
        /// @return Returns a string containing a description of the current time in the specified format.
        String GetTimeAsText(const Mezzanine::TimeFormat Format);
    };//Timer
}//Mezzanine

#endif
