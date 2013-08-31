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
#ifndef _testdatatools_h
#define _testdatatools_h

/// @file
/// @brief TestData, TestDataStorage and UnitTestGroup class definitions.

#include "datatypes.h"

#include "testdata.h"


namespace Mezzanine
{
    namespace Testing
    {
        #ifndef TEST
            /// @def TEST
            /// @brief The easiest way to add a test to the currently running UnitTestGroup.
            /// This captures test location meta data and should be considered the default way to record tests
            /// @note This calls a member function on the UnitTestGroup class, so it can only be used in UnitTestGroup Functions
            /// like UnitTestGroup::RunInteractiveTests or UnitTestGroup::RunAutomaticTests
            /// @param Cond A boolean operand of some kind
            /// @param Name The name of the current test
            #ifdef __FUNCTION__
                #define TEST(Cond, Name) Test( (Cond), (Name), Testing::Failed, Testing::Success, __FUNCTION__, __FILE__, __LINE__ );
            #else
                #define TEST(Cond, Name) Test( (Cond), (Name), Testing::Failed, Testing::Success, __func__, __FILE__, __LINE__ );
            #endif
        #endif

        #ifndef TEST_WARN
            /// @def TEST_WARN
            /// @brief Just like TEST but if the test fails only a warning is added.
            /// This captures test location meta data and should be considered the default way to record tests that warn instead of fail
            /// @note This calls a member function on the UnitTestGroup class, so it can only be used in UnitTestGroup Functions
            /// like UnitTestGroup::RunInteractiveTests or UnitTestGroup::RunAutomaticTests
            /// @param Cond A boolean operand of some kind
            /// @param Name The name of the current test
            #ifdef __FUNCTION__
                #define TEST_WARN(Cond, Name) Test( (Cond), (Name), Testing::Warning, Testing::Success, __FUNCTION__, __FILE__, __LINE__ );
            #else
                #define TEST_WARN(Cond, Name) Test( (Cond), (Name), Testing::Warning, Testing::Success, __func__, __FILE__, __LINE__ );
            #endif
        #endif

        #ifndef TEST_RESULT
            /// @def TEST_RESULT
            /// @brief An easy way to add a test and associated data to the currently running UnitTestGroup
            /// This captures test location meta data and should be considered a good way to record tests that do not easily break down to a single conditional.
            /// @note This calls a member function on the UnitTestGroup class, so it can only be used in UnitTestGroup Functions
            /// like UnitTestGroup::RunInteractiveTests or UnitTestGroup::RunAutomaticTests
            /// @param ExistingResult A TestResult To be added directy
            /// @param Name The name of the current test
            #ifdef __FUNCTION__
                #define TEST_RESULT(ExistingResult, Name) AddTestResult( TestData( (Name), (ExistingResult), __FUNCTION__, __FILE__, __LINE__)) ;
            #else
                #define TEST_RESULT(ExistingResult, Name) AddTestResult( TestData( (Name), (ExistingResult), __func__, __FILE__, __LINE__)) ;
            #endif

        #endif

        #ifndef TEST_THROW
            /// @def TEST_THROW
            /// @brief An easy way to add a test whether or not a function/code snippet throws exceptions (or whatever) the way planned.
            /// @details This captures test location meta data and should be considered the default way to capture exception tests.
            /// @note This calls a member function on the UnitTestGroup class, so it can only be used in UnitTestGroup Functions
            /// like UnitTestGroup::RunInteractiveTests or UnitTestGroup::RunAutomaticTests
            /// @param ExpectThrown The type of the thing that should be thrown
            /// @param CodeThatThrows A snippet of code that throws an exception
            /// @param Name The name of the current test
            #ifdef __FUNCTION__
                #define TEST_THROW(ExpectThrown, CodeThatThrows, Name)                                      \
                try {                                                                                       \
                    CodeThatThrows;                                                                         \
                } catch (ExpectThrown) {                                                                    \
                    AddTestResult( TestData( (Name), Testing::Success, __FUNCTION__, __FILE__, __LINE__)) ; \
                } catch (...) {                                                                             \
                    AddTestResult( TestData( (Name), Testing::Failed, __FUNCTION__, __FILE__, __LINE__)) ;  \
                }
            #else
                #define TEST_THROW(ExpectThrown, CodeThatThrows, Name)                                      \
                try {                                                                                       \
                    CodeThatThrows;                                                                         \
                } catch (ExpectThrown) {                                                                    \
                    AddTestResult( TestData( (Name), Testing::Success, __func__, __FILE__, __LINE__)) ;     \
                } catch (...) {                                                                             \
                    AddTestResult( TestData( (Name), Testing::Failed, __func__, __FILE__, __LINE__)) ;      \
                }
            #endif
        #endif

        #ifndef TEST_NO_THROW
            /// @def TEST_THROW
            /// @brief An easy way to add a test whether or not a function/code snippet throws exceptions (or whatever) the way planned.
            /// @details This captures test location meta data and should be considered the default way to capture exception tests
            /// @note This calls a member function on the UnitTestGroup class, so it can only be used in UnitTestGroup Functions
            /// like UnitTestGroup::RunInteractiveTests or UnitTestGroup::RunAutomaticTests
            /// @param CodeThatMightThrow The type of the thing that should be thrown
            /// @param Name The name of the current test
            #ifdef __FUNCTION__
                #define TEST_NO_THROW(CodeThatMightThrow, Name)                                             \
                try {                                                                                       \
                    CodeThatMightThrow;                                                                     \
                    AddTestResult( TestData( (Name), Testing::Success, __FUNCTION__, __FILE__, __LINE__)) ; \
                } catch (...) {                                                                             \
                    AddTestResult( TestData( (Name), Testing::Failed, __FUNCTION__, __FILE__, __LINE__)) ;  \
                }
            #else
                #define TEST_NO_THROW(CodeThatMightThrow, Name)                                             \
                try {                                                                                       \
                    CodeThatMightThrow;                                                                     \
                    AddTestResult( TestData( (Name), Testing::Success, __func__, __FILE__, __LINE__)) ;     \
                } catch (...) {                                                                             \
                    AddTestResult( TestData( (Name), Testing::Failed, __func__, __FILE__, __LINE__)) ;      \
                }
            #endif
        #endif

        /// @brief Get a timestamp, in microseconds. This will generally be some multiple of the GetTimeStampResolution return value.
        /// @warning On some platforms this requires a static initialization, an can cause undefined behavior if called before static initializations are complete
        /// @return The largest size integer containing a timestamp that can be compared to other timestamps, but hads no guarantees for external value.
        MaxInt Now();

        /// @brief Get the resolution of the timestamp in microseconds. This is the smallest amount of time that the GetTimeStamp can accurately track.
        /// @return A Whole which returns in millionths of a second the smallest unit of time that GetTimeStamp can measure.
        Whole NowResolution();

        /// @brief An easy way to get the time something took to execute.
        class TimedTest
        {
            /// @brief The time this was constructed.
            MaxInt BeginTimer;
            public:
                /// @brief Simply Creating this starts the timer
                TimedTest()
                    : BeginTimer(Mezzanine::Testing::Now())
                    {}

                /// @brief How long since this started.
                /// @return A MaxInt containing the Now - BeginTimer.
                MaxInt GetLength()
                    { return Mezzanine::Testing::Now() - BeginTimer; }
        };

        #ifndef TEST_TIMED
            /// @def TEST_TIMED
            /// @brief An easy way to add a test whether or not a function/code snippet takes the expected amount of time
            /// @details This starts a timer just before the CodeToTime is execute and stops that time right after it finishes
            /// @note This calls a member function on the UnitTestGroup class, so it can only be used in UnitTestGroup Functions
            /// like UnitTestGroup::RunInteractiveTests or UnitTestGroup::RunAutomaticTests
            /// @param CodeToTime The code to time
            /// @param ExpectedTime The Expected amount if time in microseconds.
            /// @param Variance A fraction 0.00 of how far off, long or short, the execution time can be and still pass. For example .02 is 2%
            /// @param Name The name of the current test
            #ifdef __FUNCTION__
                #define TEST_TIMED(CodeToTime, ExpectedTime, Variance, Name)                                \
                {                                                                                           \
                    TimedTest TESTDuration;                                                                 \
                    CodeToTime;                                                                             \
                    MaxInt TESTLength = TESTDuration.GetLength();                                           \
                    MaxInt TESTTargetTime = ExpectedTime;                                                   \
                    MaxInt TESTVariance= Variance * double(ExpectedTime);                                   \
                    if( MaxInt(TESTTargetTime-TESTVariance) < TESTLength &&                                 \
                        TESTLength < MaxInt(TESTTargetTime+TESTVariance))                                   \
                    { AddTestResult( TestData( (Name), Testing::Success, __FUNCTION__, __FILE__, __LINE__) ); } \
                    else                                                                                    \
                    { AddTestResult( TestData( (Name), Testing::Failed, __FUNCTION__, __FILE__, __LINE__) ); }  \
                }
            #else
                #define TEST_TIMED(CodeToTime, ExpectedTime, Variance, Name)                                \
                {                                                                                           \
                    TimedTest TESTDuration;                                                                 \
                    CodeToTime;                                                                             \
                    MaxInt TESTLength = TESTDuration.GetLength();                                           \
                    MaxInt TESTTargetTime = ExpectedTime;                                                   \
                    MaxInt TESTVariance= Variance * double(ExpectedTime);                                   \
                    if( MaxInt(TESTTargetTime-TESTVariance) < TESTLength &&                                 \
                        TESTLength < MaxInt(TESTTargetTime+TESTVariance))                                   \
                    { AddTestResult( TestData( (Name), Testing::Success, __func__, __FILE__, __LINE__) ); } \
                    else                                                                                    \
                    { AddTestResult( TestData( (Name), Testing::Failed, __func__, __FILE__, __LINE__) ); }  \
                }
            #endif
        #endif
    }// Testing
}// Mezzanine

#endif
