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
#ifndef _unittestenumerations_h
#define _unittestenumerations_h

/// @file
/// @brief Enumerations and constant values associated with the Unit tests

#include "mezzanine.h"

namespace Mezzanine
{
    namespace Testing
    {
        /// @brief Possible ways to exit the UnitTestGroup Program
        enum ExitCodes
        {
            ExitSuccess             = 0,    ///< Normal exit all tests skipped or better
            ExitInvalidArguments    = 1,    ///< At least some invalid args were passed on the command line
            ExitFailure             = 2     ///< At least one test return worse then skipped
        };



        /// @brief Return values from tests.
        /// @details These are loosely order from best to worse, in terms of
        /// what we want to see from our tests. This takes the midset that
        /// knowing about a failure is better than not knowing about it.
        enum TestResult
        {
            Success         = 0,        ///< Test was ran and appeared to work
            Skipped         = 1,        ///< Test was simply not ran at the behest of the user
            Cancelled       = 2,        ///< Was canceled by user, so success is unknown, but user knows test was canceled.
            Inconclusive    = 3,        ///< If a user answers that with "don't know" in a test that involved interaction, The user knows there is a potential issue.
            Failed          = 4,        ///< Known failure
            Unknown         = 5,        ///< Since we don't know what happened this is the worst kind of failure.
            NotApplicable   = 6         ///< This is not even a kind of failure, This is used to when referencing a test, so if this winds up coming out of a test, then something has failed.
        };

        /// @brief Corresponds to TestResult::Success
        const Mezzanine::String SuccessString("Success");
        /// @brief Corresponds to TestResult::Skipped
        const Mezzanine::String SkippedString("Skipped");
        /// @brief Corresponds to TestResult::Cancelled
        const Mezzanine::String CancelledString("Cancelled");
        /// @brief Corresponds to TestResult::Inconclusive
        const Mezzanine::String InconclusiveString("Inconclusive");
        /// @brief Corresponds to TestResult::Failed
        const Mezzanine::String FailedString("Failed");
        /// @brief Corresponds to TestResult::Unknown
        const Mezzanine::String UnknownString("Unknown");
        /// @brief Corresponds to TestResult::NotApplicable
        const Mezzanine::String NotApplicableString("NotApplicable");

        /// @brief This converts A test result enum value into a String matching the identifier name.
        /// @param Convertable A TestResult inclusively between Success and NotApplicable.
        /// @return  A string like "Success" or "Inconclusive" or similar.
        /// @throw If an int not in the valid range of @ref TestResult is passed in then this will throw a INVALID_PARAMETERS_EXCEPTION
        Mezzanine::String TestResultToString(TestResult Convertable);

        /// @brief Roughly convert a String to a @ref TestResult.
        /// @param Text If this matches a word like "Success", "Skipped", "Cancelled", "Inconclusive", "Failed", "Unknown" or "N/A" exactly as returned by @ref TestResultToString then a valid result can be returned.
        /// @return The @ref TestResult corresponding to the String passed in.
        /// @throw If the string passed in does not match a valid @ref TestResult then this will throw a INVALID_PARAMETERS_EXCEPTION
        TestResult StringToTestResult(Mezzanine::String Text);

    }// Testing
}// Mezzanine

#endif
