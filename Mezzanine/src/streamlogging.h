// © Copyright 2010 - 2017 BlackTopp Studios Inc.
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

#ifndef _logging_h
#define _logging_h

#include "crossplatformexport.h"
#include <iostream>

////////////////////////////////////////////////////////////////////////////////////////////////////
// LogLevel Declaration and Tools
//////////////////////////////////////////////////

/// @brief These are used with IOStreams to control when output is actually written and when it is not.
/// @details This is a bitfield that is used the LogStream to control the output of specific messages.
/// @n This can be edited (and the Mezzanine recompiled). Any replacement must include the members
/// LL_Lowest, LL_Highest and LL_FlagLimit.
enum LogLevel {
    LL_None        = 0,                                                     ///< No flag are set, not intended for use in actual logging, but for use in binary maths.

    LL_Trace       = 1,                                                     ///< This should be used for detailed output that only the craziest corner case requires.
    LL_Debug       = 2,                                                     ///< Output that is generally not useful, unless it is time to fix a bug.
    LL_Warn        = 4,                                                     ///< Stuff that might help a technician figure a problem out or that could indicate a deeper problem while not being a problem itself.
    LL_Error       = 8,                                                     ///< A porblem has happened, but it doesn't require execution to terminate immediately. For example Spelling.
    LL_Fatal       = 16,                                                    ///< A problem has happened and it cannot be recovered from, perhaps memory exhaustion or the GPU barfed or something like that.

    LL_TraceAndHigher      = LL_Trace|LL_Debug|LL_Warn|LL_Error|LL_Fatal,   ///< Includes Trace, Debug, Warn, Error and Fatal, all of them.
    LL_DebugAndHigher      = LL_Debug|LL_Warn|LL_Error|LL_Fatal,            ///< Includes Debug, Warn, Error and Fatal. A reasonable setting while debugging
    LL_WarnAndHigher       = LL_Warn|LL_Error|LL_Fatal,                     ///< Includes Warn, Error and Fatal. Likely useful for troubleshooting.
    LL_ErrorAndHigher      = LL_Error|LL_Fatal,                             ///< Includes Error and Fatal. Probably a good default for shipping code.
    LL_FatalAndHigher      = LL_Fatal,                                      ///< Includes Fatal. Here for naming consistency

    LL_Lowest      = 1,                                                     ///< Is the lowest bitvalue intended to be used for logging.
    LL_Highest     = 16,                                                    ///< IS the highest bitvalue intended to be used for logging.
    LL_FlagLimit   = 32                                                     ///< The next higher power of two past the valid loglevels, will therefor be exactly 1 higher than a full bitfield.
};

/// @brief Merge 2 log levels into 1
/// @details To maintain type safety normal bitwise operations should not be used with loglevels
/// @param First One of many LogLevel instances to merge into one.
/// @param Second Another of many loglevel instances to merge.
LogLevel MEZZ_LIB MergeLogLevel(LogLevel First, LogLevel Second);

/// @brief Merge 3 log levels into 1
/// @param First One of many LogLevel instances to merge into one.
/// @param Second Another of many loglevel instances to merge.
/// @param Third Another of many loglevel instances to merge.
LogLevel MEZZ_LIB MergeLogLevel(LogLevel First, LogLevel Second, LogLevel Third);

/// @brief Merge 4 log levels into 1
/// @param First One of many LogLevel instances to merge into one.
/// @param Second Another of many loglevel instances to merge.
/// @param Third Another of many loglevel instances to merge.
/// @param Fourth Another of many loglevel instances to merge.
LogLevel MEZZ_LIB MergeLogLevel(LogLevel First, LogLevel Second, LogLevel Third, LogLevel Fourth);

/// @brief Merge 5 log levels into 1
/// @param First One of many LogLevel instances to merge into one.
/// @param Second Another of many loglevel instances to merge.
/// @param Third Another of many loglevel instances to merge.
/// @param Fourth Another of many loglevel instances to merge.
/// @param Fifth Another of many loglevel instances to merge.
LogLevel MEZZ_LIB MergeLogLevel(LogLevel First, LogLevel Second, LogLevel Third, LogLevel Fourth, LogLevel Fifth);

/// @brief Merge 6 log levels into 1
/// @param First One of many LogLevel instances to merge into one.
/// @param Second Another of many loglevel instances to merge.
/// @param Third Another of many loglevel instances to merge.
/// @param Fourth Another of many loglevel instances to merge.
/// @param Fifth Another of many loglevel instances to merge.
/// @param Sixth Another of many loglevel instances to merge.
LogLevel MEZZ_LIB MergeLogLevel(LogLevel First, LogLevel Second, LogLevel Third, LogLevel Fourth, LogLevel Fifth, LogLevel Sixth);

/// @brief Merge 7 log levels into 1
/// @param First One of many LogLevel instances to merge into one.
/// @param Second Another of many loglevel instances to merge.
/// @param Third Another of many loglevel instances to merge.
/// @param Fourth Another of many loglevel instances to merge.
/// @param Fifth Another of many loglevel instances to merge.
/// @param Sixth Another of many loglevel instances to merge.
/// @param Seventh Another of many loglevel instances to merge.
/// @return All of the LogLevels merged into one bitfield that is still a valid LogLevel.
LogLevel MEZZ_LIB MergeLogLevel(LogLevel First, LogLevel Second, LogLevel Third, LogLevel Fourth, LogLevel Fifth, LogLevel Sixth, LogLevel Seventh);

/// @brief Merge 8 log levels into 1
/// @param First This
/// @param Second accepts
/// @param Third a
/// @param Fourth truly
/// @param Fifth obnoxious
/// @param Sixth amount
/// @param Seventh of
/// @param Eighth parameters.
/// @return All of the LogLevels merged into one bitfield that is still a valid LogLevel.
/// @todo Replace this Shenanigans with a variadic template when it becomes available. Though this crao should be easy for even bad optimizers to cleanup.
LogLevel MEZZ_LIB MergeLogLevel(LogLevel First, LogLevel Second, LogLevel Third, LogLevel Fourth, LogLevel Fifth, LogLevel Sixth, LogLevel Seventh, LogLevel Eighth);


////////////////////////////////////////////////////////////////////////////////////////////////////
// STD Streams configuration
//////////////////////////////////////////////////

/// @brief The Standard Streams will Use the log levels set here
/// @details This defaults to Trace and debug
void MEZZ_LIB SetStandardLoggingLevel(LogLevel NewLevel);

/// @brief Retrieve thelog loglevel in use by the stanard
/// @details This defaults to logging everything (Trace and Higher)
LogLevel MEZZ_LIB GetStandardLoggingLevel();


////////////////////////////////////////////////////////////////////////////////////////////////////
// LogStream and Modifiers
//////////////////////////////////////////////////

/// @brief A stream that tracks its own level of logging.
/// @tparam CharType This is kind of character that the stream will work with.
/// @tparam CharTraits The traits (move, equality, assignment, etc...) of the character type passed in
/// @details Instances of this stream can be configured to treat log messages differently depending on the LogLevel
/// of the message and Level actively being logged. In other regards this is a normal output stream.
template<class CharType, class CharTraits = std::char_traits<CharType> >
class  LogStream : public std::basic_ostream<CharType, CharTraits>
{
    /// @brief The LogLevel for stream insertion that will be completed.
    /// @details If messages passed in come in on a level with at least one bit this matching bitfield, then the message is
    /// streamed, otherwise it is dropped silently.
    LogLevel ActivelyLogging;

    public:
        /// @brief Buffer Stealing Constructor
        /// @details When used to create a LogStream this will use the rdbuf of another std::basic_ostream
        /// @param OutputStream This is the stream to steal the buffer from
        /// @param WillLog The starting LogLevel to capture, which Defaults to LL_Highest.
        LogStream(std::basic_ostream<CharType, CharTraits>& OutputStream, LogLevel WillLog = LL_Highest)
            : std::basic_ostream<CharType, CharTraits>(OutputStream.rdbuf()),
              ActivelyLogging(WillLog)
            { }

        /// @brief Change the level of what this Stream is logging.
        /// @param NewLevel After this returns any messages sent to this stream will be compared against this.
        void SetLoggingLevel(LogLevel NewLevel)
            { ActivelyLogging = NewLevel; }

        /// @brief What will currently be logged?
        /// @return A LogLevel that is the bitmap of what will be logged.
        LogLevel GetLoggingLevel()
            { return ActivelyLogging; }

        /// @brief Custom Manipulator inserter
        /// @param pf Accepts a pointer to a LogStream Manipulator
        /// @return Whatever stream the manipulator returns which should be *this.
        LogStream<CharType, CharTraits>& operator<< (LogStream<CharType, CharTraits>& (*pf) (LogStream<CharType, CharTraits>&))
            { return pf(*this); }
};

/// @brief This is a Stream modifier that sets the level of following messages to be inserted into the stream.
/// @details This specific method is for LogStream, but it is intended to appear outwardly similar to
/// standard streams Versions.
/// @tparam CharType This is used as type inference to allow type safety when inserting this into a stream.
/// @tparam CharTraits This is used as type inference to allow type safety when inserting this into a stream.
/// @param OutputStream This is the stream to be modified and is the left hand operand of operator<<.
/// @param CurrentLevel The new level the receiving stream should log.
/// @return A reference to the stream being modified.
/// @note When inserted into any standard this affects all standard streams.
template<class CharType, class CharTraits>
LogStream<CharType, CharTraits>& MessageLogLevel(LogStream<CharType, CharTraits>& OutputStream, LogLevel CurrentLevel)
{
    if(CurrentLevel & OutputStream.GetLoggingLevel())
        { OutputStream.clear(std::basic_ios<CharType, CharTraits>::goodbit); }
    else
        { OutputStream.setstate(std::basic_ios<CharType, CharTraits>::failbit); }
    return OutputStream;
}

/// @brief The stream this is inserted into will treat this following inserted messages as traces.
/// @tparam CharType This is used as type inference to allow type safety when inserting this into a stream.
/// @tparam CharTraits This is used as type inference to allow type safety when inserting this into a stream.
/// @param OutputStream This is the stream to be modified and is the left hand operand of operator<<.
/// @return A reference to the stream being modified.
/// @note When inserted into any standard this affects all standard streams.
/// @details Sample usage:
/// @code
/// LogStream Log(cout);
/// Log << LogTrace << "Entering function int MeaningOfLifeUniverseAndEverything(42).";
/// @endcode
template<class CharType, class CharTraits>
LogStream<CharType, CharTraits>& LogTrace(LogStream<CharType, CharTraits>& OutputStream)
    { return MessageLogLevel(OutputStream, LL_Trace); }

/// @brief The stream this is inserted into will treat this following inserted messages as debug messages.
/// @tparam CharType This is used as type inference to allow type safety when inserting this into a stream.
/// @tparam CharTraits This is used as type inference to allow type safety when inserting this into a stream.
/// @param OutputStream This is the stream to be modified and is the left hand operand of operator<<.
/// @return A reference to the stream being modified.
/// @note When inserted into any standard this affects all standard streams.
/// @details Sample usage:
/// @code
/// LogStream Log(cout);
/// Log << LogDebug << "Sample Log Message.";
/// @endcode
template<class CharType, class CharTraits>
LogStream<CharType, CharTraits>& LogDebug(LogStream<CharType, CharTraits>& OutputStream)
    { return MessageLogLevel(OutputStream, LL_Debug); }

/// @brief The stream this is inserted into will treat this following inserted messages as warnings.
/// @tparam CharType This is used as type inference to allow type safety when inserting this into a stream.
/// @tparam CharTraits This is used as type inference to allow type safety when inserting this into a stream.
/// @param OutputStream This is the stream to be modified and is the left hand operand of operator<<.
/// @return A reference to the stream being modified.
/// @note When inserted into any standard this affects all standard streams.
/// @details Sample usage:
/// @code
/// LogStream Log(cout);
/// Log << LogWarn << "Danger Will Robinson, Danger!!!";
/// @endcode
template<class CharType, class CharTraits>
LogStream<CharType, CharTraits>& LogWarn(LogStream<CharType, CharTraits>& OutputStream)
    { return MessageLogLevel(OutputStream, LL_Warn); }

/// @brief The stream this is inserted into will treat this following inserted messages as errors.
/// @tparam CharType This is used as type inference to allow type safety when inserting this into a stream.
/// @tparam CharTraits This is used as type inference to allow type safety when inserting this into a stream.
/// @param OutputStream This is the stream to be modified and is the left hand operand of operator<<.
/// @return A reference to the stream being modified.
/// @note When inserted into any standard this affects all standard streams.
/// @details Sample usage:
/// @code
/// LogStream Log(cout);
/// Log << LogError << "The System is dowm, the system is down, Bwoo Dee boo do do.";
/// @endcode
template<class CharType, class CharTraits>
LogStream<CharType, CharTraits>& LogError(LogStream<CharType, CharTraits>& OutputStream)
    { return MessageLogLevel(OutputStream, LL_Error); }

/// @brief The stream this is inserted into will treat this following inserted messages as fatal.
/// @tparam CharType This is used as type inference to allow type safety when inserting this into a stream.
/// @tparam CharTraits This is used as type inference to allow type safety when inserting this into a stream.
/// @param OutputStream This is the stream to be modified and is the left hand operand of operator<<.
/// @return A reference to the stream being modified.
/// @note When inserted into any standard this affects all standard streams.
/// @details Sample usage:
/// @code
/// LogStream Log(cout);
/// Log << LogFatal << "The Tuna contains too much Flipper!!!";
/// @endcode
template<class CharType, class CharTraits>
LogStream<CharType, CharTraits>& LogFatal(LogStream<CharType, CharTraits>& OutputStream)
    { return MessageLogLevel(OutputStream, LL_Fatal); }


////////////////////////////////////////////////////////////////////////////////////////////////////
// Standard Stream Modifiers
//////////////////////////////////////////////////


/// @brief This is a Stream modifier that sets the level of following messages to be inserted into the stream.
/// @details This specific method is for standard streams, but it is intended to appear outwardly similar to
/// LogStream Versions.
/// @tparam CharType This is used as type inference to allow type safety when inserting this into a stream.
/// @tparam CharTraits This is used as type inference to allow type safety when inserting this into a stream.
/// @param OutputStream This is the stream to be modified and is the left hand operand of operator<<.
/// @param CurrentLevel The new level the receiving stream should log.
/// @return A reference to the stream being modified.
/// @note When inserted into any standard this affects all standard streams.
template<class CharType, class CharTraits>
std::basic_ostream<CharType, CharTraits>& MessageLogLevel(std::basic_ostream<CharType, CharTraits>& OutputStream, LogLevel CurrentLevel)
{
    if(CurrentLevel & GetStandardLoggingLevel())
        { OutputStream.clear(std::basic_ios<CharType, CharTraits>::goodbit); }
    else
        { OutputStream.setstate(std::basic_ios<CharType, CharTraits>::failbit); }
    return OutputStream;
}

/// @brief The messages following the insertion of this into a standard stream will be treated as traces.
/// @tparam CharType This is used as type inference to allow type safety when inserting this into a stream.
/// @tparam CharTraits This is used as type inference to allow type safety when inserting this into a stream.
/// @param OutputStream This is the stream to be modified and is the left hand operand of operator<<.
/// @return A reference to the stream being modified.
/// @note When inserted into any standard this affects all standard streams.
/// @details Here is a sample of use:
/// @code
/// cout << LogTrace << "Entering function int MeaningOfLifeUniverseAndEverything(42).";
/// @endcode
template<class CharType, class CharTraits>
std::basic_ostream<CharType, CharTraits>& LogTrace(std::basic_ostream<CharType, CharTraits>& OutputStream)
    { return MessageLogLevel(OutputStream, LL_Trace); }

/// @brief The messages following the insertion of this into a standard stream will be treated as debug messages.
/// @tparam CharType This is used as type inference to allow type safety when inserting this into a stream.
/// @tparam CharTraits This is used as type inference to allow type safety when inserting this into a stream.
/// @param OutputStream This is the stream to be modified and is the left hand operand of operator<<.
/// @return A reference to the stream being modified.
/// @note When inserted into any standard this affects all standard streams.
/// @details Here is a sample of use:
/// @code
/// cout << LogDebug << "Opening file foo.txt.";
/// @endcode
template<class CharType, class CharTraits>
std::basic_ostream<CharType, CharTraits>& LogDebug(std::basic_ostream<CharType, CharTraits>& OutputStream)
    { return MessageLogLevel(OutputStream, LL_Debug); }

/// @brief The messages following the insertion of this into a standard stream will be treated as warnings.
/// @tparam CharType This is used as type inference to allow type safety when inserting this into a stream.
/// @tparam CharTraits This is used as type inference to allow type safety when inserting this into a stream.
/// @param OutputStream This is the stream to be modified and is the left hand operand of operator<<.
/// @return A reference to the stream being modified.
/// @note When inserted into any standard this affects all standard streams.
/// @details Here is a sample of use:
/// @code
/// cout << LogWarn << "Danger Will Robinson, Danger!!!";
/// @endcode
template<class CharType, class CharTraits>
std::basic_ostream<CharType, CharTraits>& LogWarn(std::basic_ostream<CharType, CharTraits>& OutputStream)
    { return MessageLogLevel(OutputStream, LL_Warn); }

/// @brief The messages following the insertion of this into a standard stream will be treated as Errors.
/// @tparam CharType This is used as type inference to allow type safety when inserting this into a stream.
/// @tparam CharTraits This is used as type inference to allow type safety when inserting this into a stream.
/// @param OutputStream This is the stream to be modified and is the left hand operand of operator<<.
/// @return A reference to the stream being modified.
/// @note When inserted into any standard this affects all standard streams.
/// @details Here is a sample of use:
/// @code
/// cout << LogError << "The System is dowm, the system is down, Bwoo Dee boo do do.";
/// @endcode
template<class CharType, class CharTraits>
std::basic_ostream<CharType, CharTraits>& LogError(std::basic_ostream<CharType, CharTraits>& OutputStream)
    { return MessageLogLevel(OutputStream, LL_Error); }

/// @brief The messages following the insertion of this into a standard stream will be treated as fatal.
/// @tparam CharType This is used as type inference to allow type safety when inserting this into a stream.
/// @tparam CharTraits This is used as type inference to allow type safety when inserting this into a stream.
/// @param OutputStream This is the stream to be modified and is the left hand operand of operator<<.
/// @return A reference to the stream being modified.
/// @note When inserted into any standard this affects all standard streams.
/// @details Here is a sample of use:
/// @code
/// cout << LogFatal << "The Tuna contains too much Flipper!!!";
/// @endcode
template<class CharType, class CharTraits>
std::basic_ostream<CharType, CharTraits>& LogFatal(std::basic_ostream<CharType, CharTraits>& OutputStream)
    { return MessageLogLevel(OutputStream, LL_Fatal); }


#endif //_logging_h
