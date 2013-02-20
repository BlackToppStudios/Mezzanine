//© Copyright 2010 - 2012 BlackTopp Studios Inc.
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

#ifndef _exception_h
#define _exception_h

#include "datatypes.h"

namespace Mezzanine
{
    ///////////////////////////////////////////////////////////////////////////////
    // Layer 1

    ///////////////////////////////////////////////////////////////////////////////
    /// @class Exception
    /// @headerfile exception.h
    /// @brief This is the exception thrown by most Mezzanine system that can throw exceptions.
    /// @details In general they work like std::exception, but construct a string message from a few source with the help of a macro.
    ///////////////////////////////////////////////////////////////////////////////
    class MEZZ_LIB Exception : public std::exception
    {
        public:
            /// @enum ExceptionCodes
            /// @brief This enum provides mapping/naming for all the core exceptions of Mezzanine.
            /// @details The numbers listed here must be unique to each exception.
            enum ExceptionCodes
            {
                // Input/Output exceptions
                IO_UNKNOWN_EXCEPTION                = 0x01A00,  // 1
                IO_READ_EXCEPTION                   = 0x01A01,  // 2
                IO_WRITE_EXCEPTION                  = 0x01A02,  // 3
                IO_FILE_NOT_FOUND_EXCEPTION         = 0x01A03,  // 4
                IO_DIRECTORY_NOT_FOUND_EXCEPTION    = 0x01A04,  // 5
                // Instance Identity exceptions
                II_UNKNOWN_EXCEPTION                = 0x02A00,  // 6
                II_IDENTITY_INVALID_EXCEPTION       = 0x02A01,  // 7
                II_IDENTITY_NOT_FOUND_EXCEPTION     = 0x02A02,  // 8
                II_DUPLICATE_IDENTITY_EXCEPTION     = 0x02A03,  // 9
                // Memory Management exceptions
                MM_UNKNOWN_EXCEPTION                = 0x03A00,  // 10
                MM_OUT_OF_MEMORY_EXCEPTION          = 0x03A01,  // 11
                MM_OUT_OF_BOUNDS_EXCEPTION          = 0x03A02,  // 12
                // Syntax exceptions
                SYNTAX_ERROR_EXCEPTION              = 0x04A00,  // 13
                SYNTAX_ERROR_EXCEPTION_XML          = 0x04A01,  // 14
                SYNTAX_ERROR_EXCEPTION_LUA          = 0x04A02,  // 15
                // Other exceptions
                ARITHMETIC_EXCEPTION                = 0x00A01,  // 16
                INVALID_PARAMETERS_EXCEPTION        = 0x00A02,  // 17
                INVALID_VERSION_EXCEPTION           = 0x00A03,  // 18
                INVALID_STATE_EXCEPTION             = 0x00A04,  // 19
                RENDERINGAPI_EXCEPTION              = 0x00A05,  // 20
                RT_ASSERTION_EXCEPTION              = 0x00A06,  // 21
                INTERNAL_EXCEPTION                  = 0x00A07,  // 22
                NOT_IMPLEMENTED_EXCEPTION           = 0x00A08   // 23
            };
        private:
            /// @internal
            /// @brief This stores the Error Message
            const String ErrorMessage;
            /// @internal
            /// @brief This stores the exception type as a string.
            const String ExceptionTypeName;
            /// @internal
            /// @brief This stores the function name where the exception originated.
            const String Function;
            /// @internal
            /// @brief This stores the file where the exception originated.
            const String File;
            /// @internal
            /// @brief This stroes the line number where the exception originated.
            const Whole Line;
        public:
            /// @brief Simple Constructor.
            /// @param TypeName The name of the type of exception being thrown.
            /// @param Message A basic description of the error.
            /// @param SrcFunction The name of the function from which this originated.
            /// @param SrcFile The name of the file from which this originated.
            /// @param FileLine The line on the named file from which this originated.
            Exception(const String& TypeName, const String& Message, const String& SrcFunction, const String& SrcFile, const Whole& FileLine);
            /// @brief Class destructor.
            virtual ~Exception() throw();

            ///////////////////////////////////////////////////////////////////////////////
            // Error Information
            /// @brief Gets the exception code for this exception.
            /// @return Returns the exception code for this exception.
            virtual Whole GetExceptionCode() const throw() = 0;

            /// @brief Gets the name of the exception type.
            /// @return Returns the exception type as a string.
            ConstString& GetExceptionTypeName() const throw();
            /// @brief Gets the function of the exception.
            /// @return Returns a const string reference
            ConstString& GetFunction() const throw();
            /// @brief Gets the file of the exception.
            /// @return Returns the name of the file this exception is being thrown from.
            ConstString& GetFile() const throw();
            /// @brief Gets the line number of the exception.
            /// @return Returns the line in the file this exception is being thrown from.
            const Whole& GetLine() const throw();

            /// @brief Constructs the complete message from all the information provided about the exception.
            /// @return Returns a string with the complete error.
            String GetCompleteMessage() const throw();
            /// @brief Retrieves the error message.
            /// @return This returns a string that is the stored error message.
            virtual String what() throw();
    };//Exception

    ///////////////////////////////////////////////////////////////////////////////
    /// @struct ExceptionFactory
    /// @headerfile exception.h
    /// @brief Template class that serves as the base for exception factories.
    /// @details Additional exceptions and their factories have to specialize from this template changing the type
    /// value to the new exception type.  This allows our exception macro to find the appropriate factory and be
    /// extendable with additional exceptions.  Attempting to create an unknown exception simply won't compile on
    /// account of the base exception class being abstract.
    ///////////////////////////////////////////////////////////////////////////////
    template <Whole N>
    struct MEZZ_LIB ExceptionFactory
    {
        typedef Exception Type;
    };//ExceptionFactory

    ///////////////////////////////////////////////////////////////////////////////
    // Layer 2

    ///////////////////////////////////////////////////////////////////////////////
    /// @class IOException
    /// @headerfile exception.h
    /// @brief An exception for input/output failures.
    ///////////////////////////////////////////////////////////////////////////////
    class MEZZ_LIB IOException : public Exception
    {
        public:
            /// @brief The internal code for this exception.
            static const Whole ExceptionCode = Exception::IO_UNKNOWN_EXCEPTION;
        protected:
            /// @brief Inheritance constructor.
            /// @param TypeName The name of the type of exception being thrown.
            /// @param Message A basic description of the error.
            /// @param SrcFunction The name of the function from which this originated.
            /// @param SrcFile The name of the file from which this originated.
            /// @param FileLine The line on the named file from which this originated.
            IOException(const String& TypeName, const String& Message, const String& SrcFunction, const String& SrcFile, const Whole& FileLine)
                : Exception(TypeName,Message,SrcFunction,SrcFile,FileLine)
                {}
        public:
            /// @brief Class constructor.
            /// @param Message A basic description of the error.
            /// @param SrcFunction The name of the function from which this originated.
            /// @param SrcFile The name of the file from which this originated.
            /// @param FileLine The line on the named file from which this originated.
            IOException(const String& Message, const String& SrcFunction, const String& SrcFile, const Whole& FileLine)
                : Exception("IOException",Message,SrcFunction,SrcFile,FileLine)
                {}
            /// @brief Class destructor.
            virtual ~IOException() throw() {}

            /// @copydoc Exception::GetExceptionCode()
            virtual Whole GetExceptionCode() const throw()
                { return IOException::ExceptionCode; }
    };//IOException

    template<>
    struct MEZZ_LIB ExceptionFactory<IOException::ExceptionCode>
        { typedef IOException Type; };

    ///////////////////////////////////////////////////////////////////////////////
    /// @class InstanceIdentityException
    /// @headerfile exception.h
    /// @brief An exception for Instance Identity failures.
    ///////////////////////////////////////////////////////////////////////////////
    class MEZZ_LIB InstanceIdentityException : public Exception
    {
        public:
            /// @brief The internal code for this exception.
            static const Whole ExceptionCode = Exception::II_UNKNOWN_EXCEPTION;
        protected:
            /// @brief Inheritance constructor.
            /// @param TypeName The name of the type of exception being thrown.
            /// @param Message A basic description of the error.
            /// @param SrcFunction The name of the function from which this originated.
            /// @param SrcFile The name of the file from which this originated.
            /// @param FileLine The line on the named file from which this originated.
            InstanceIdentityException(const String& TypeName, const String& Message, const String& SrcFunction, const String& SrcFile, const Whole& FileLine)
                : Exception(TypeName,Message,SrcFunction,SrcFile,FileLine)
                {}
        public:
            /// @brief Class constructor.
            /// @param Message A basic description of the error.
            /// @param SrcFunction The name of the function from which this originated.
            /// @param SrcFile The name of the file from which this originated.
            /// @param FileLine The line on the named file from which this originated.
            InstanceIdentityException(const String& Message, const String& SrcFunction, const String& SrcFile, const Whole& FileLine)
                : Exception("InstanceIdentityException",Message,SrcFunction,SrcFile,FileLine)
                {}
            /// @brief Class destructor.
            virtual ~InstanceIdentityException() throw() {}

            /// @copydoc Exception::GetExceptionCode()
            virtual Whole GetExceptionCode() const throw()
                { return InstanceIdentityException::ExceptionCode; }
    };//InstanceIdentityException

    template<>
    struct MEZZ_LIB ExceptionFactory<InstanceIdentityException::ExceptionCode>
        { typedef InstanceIdentityException Type; };

    ///////////////////////////////////////////////////////////////////////////////
    /// @class MemoryManagementException
    /// @headerfile exception.h
    /// @brief An exception for Memory related failures.
    ///////////////////////////////////////////////////////////////////////////////
    class MEZZ_LIB MemoryManagementException : public Exception
    {
        public:
            /// @brief The internal code for this exception.
            static const Whole ExceptionCode = Exception::MM_UNKNOWN_EXCEPTION;
        protected:
            /// @brief Inheritance constructor.
            /// @param TypeName The name of the type of exception being thrown.
            /// @param Message A basic description of the error.
            /// @param SrcFunction The name of the function from which this originated.
            /// @param SrcFile The name of the file from which this originated.
            /// @param FileLine The line on the named file from which this originated.
            MemoryManagementException(const String& TypeName, const String& Message, const String& SrcFunction, const String& SrcFile, const Whole& FileLine)
                : Exception(TypeName,Message,SrcFunction,SrcFile,FileLine)
                {}
        public:
            /// @brief Class constructor.
            /// @param Message A basic description of the error.
            /// @param SrcFunction The name of the function from which this originated.
            /// @param SrcFile The name of the file from which this originated.
            /// @param FileLine The line on the named file from which this originated.
            MemoryManagementException(const String& Message, const String& SrcFunction, const String& SrcFile, const Whole& FileLine)
                : Exception("MemoryManagementException",Message,SrcFunction,SrcFile,FileLine)
                {}
            /// @brief Class destructor.
            virtual ~MemoryManagementException() throw() {}

            /// @copydoc Exception::GetExceptionCode()
            virtual Whole GetExceptionCode() const throw()
                { return InstanceIdentityException::ExceptionCode; }
    };//MemoryManagementException

    template<>
    struct MEZZ_LIB ExceptionFactory<MemoryManagementException::ExceptionCode>
        { typedef MemoryManagementException Type; };

    ///////////////////////////////////////////////////////////////////////////////
    /// @class SyntaxErrorException
    /// @headerfile exception.h
    /// @brief An exception for syntax errors when parsing other files, such as Lua files.
    ///////////////////////////////////////////////////////////////////////////////
    class MEZZ_LIB SyntaxErrorException : public Exception
    {
        public:
            /// @brief The internal code for this exception.
            static const Whole ExceptionCode = Exception::SYNTAX_ERROR_EXCEPTION;
        protected:
            /// @brief Inheritance constructor.
            /// @param TypeName The name of the type of exception being thrown.
            /// @param Message A basic description of the error.
            /// @param SrcFunction The name of the function from which this originated.
            /// @param SrcFile The name of the file from which this originated.
            /// @param FileLine The line on the named file from which this originated.
            SyntaxErrorException(const String& TypeName, const String& Message, const String& SrcFunction, const String& SrcFile, const Whole& FileLine)
                : Exception(TypeName,Message,SrcFunction,SrcFile,FileLine)
                {}
        public:
            /// @brief Class constructor.
            /// @param Message A basic description of the error.
            /// @param SrcFunction The name of the function from which this originated.
            /// @param SrcFile The name of the file from which this originated.
            /// @param FileLine The line on the named file from which this originated.
            SyntaxErrorException(const String& Message, const String& SrcFunction, const String& SrcFile, const Whole& FileLine)
                : Exception("InvalidSyntaxException",Message,SrcFunction,SrcFile,FileLine)
                {}
            /// @brief Class destructor.
            virtual ~SyntaxErrorException() throw() {}

            /// @copydoc Exception::GetExceptionCode()
            virtual Whole GetExceptionCode() const throw()
                { return SyntaxErrorException::ExceptionCode; }
    };//InvalidSyntaxException

    template<>
    struct MEZZ_LIB ExceptionFactory<SyntaxErrorException::ExceptionCode>
        { typedef SyntaxErrorException Type; };

    ///////////////////////////////////////////////////////////////////////////////
    /// @class ArithmeticException
    /// @headerfile exception.h
    /// @brief An exception for errors that arise when doing math.
    ///////////////////////////////////////////////////////////////////////////////
    class MEZZ_LIB ArithmeticException : public Exception
    {
        public:
            /// @brief The internal code for this exception.
            static const Whole ExceptionCode = Exception::ARITHMETIC_EXCEPTION;

            /// @brief Class constructor.
            /// @param Message A basic description of the error.
            /// @param SrcFunction The name of the function from which this originated.
            /// @param SrcFile The name of the file from which this originated.
            /// @param FileLine The line on the named file from which this originated.
            ArithmeticException(const String& Message, const String& SrcFunction, const String& SrcFile, const Whole& FileLine)
                : Exception("ArithmeticException",Message,SrcFunction,SrcFile,FileLine)
                {}
            /// @brief Class destructor.
            virtual ~ArithmeticException() throw() {}

            /// @copydoc Exception::GetExceptionCode()
            virtual Whole GetExceptionCode() const throw()
                { return ArithmeticException::ExceptionCode; }
    };//ArithmeticException

    template<>
    struct MEZZ_LIB ExceptionFactory<ArithmeticException::ExceptionCode>
        { typedef ArithmeticException Type; };

    ///////////////////////////////////////////////////////////////////////////////
    /// @class InvalidParametersException
    /// @headerfile exception.h
    /// @brief An exception for when improper parameters are passed into an object or method.
    ///////////////////////////////////////////////////////////////////////////////
    class MEZZ_LIB InvalidParametersException : public Exception
    {
        public:
            /// @brief The internal code for this exception.
            static const Whole ExceptionCode = Exception::INVALID_PARAMETERS_EXCEPTION;

            /// @brief Class constructor.
            /// @param Message A basic description of the error.
            /// @param SrcFunction The name of the function from which this originated.
            /// @param SrcFile The name of the file from which this originated.
            /// @param FileLine The line on the named file from which this originated.
            InvalidParametersException(const String& Message, const String& SrcFunction, const String& SrcFile, const Whole& FileLine)
                : Exception("InvalidParametersException",Message,SrcFunction,SrcFile,FileLine)
                {}
            /// @brief Class destructor.
            virtual ~InvalidParametersException() throw() {}

            /// @copydoc Exception::GetExceptionCode()
            virtual Whole GetExceptionCode() const throw()
                { return InvalidParametersException::ExceptionCode; }
    };//InvalidParametersException

    template<>
    struct MEZZ_LIB ExceptionFactory<InvalidParametersException::ExceptionCode>
        { typedef InvalidParametersException Type; };

    ///////////////////////////////////////////////////////////////////////////////
    /// @class InvalidVersionException
    /// @headerfile exception.h
    /// @brief An exception for when version mismatches occur.
    ///////////////////////////////////////////////////////////////////////////////
    class MEZZ_LIB InvalidVersionException : public Exception
    {
        public:
            /// @brief The internal code for this exception.
            static const Whole ExceptionCode = Exception::INVALID_VERSION_EXCEPTION;

            /// @brief Class constructor.
            /// @param Message A basic description of the error.
            /// @param SrcFunction The name of the function from which this originated.
            /// @param SrcFile The name of the file from which this originated.
            /// @param FileLine The line on the named file from which this originated.
            InvalidVersionException(const String& Message, const String& SrcFunction, const String& SrcFile, const Whole& FileLine)
                : Exception("InvalidVersionException",Message,SrcFunction,SrcFile,FileLine)
                {}
            /// @brief Class destructor.
            virtual ~InvalidVersionException() throw() {}

            /// @copydoc Exception::GetExceptionCode()
            virtual Whole GetExceptionCode() const throw()
                { return InvalidVersionException::ExceptionCode; }
    };//InvalidVersionException

    template<>
    struct MEZZ_LIB ExceptionFactory<InvalidVersionException::ExceptionCode>
        { typedef InvalidVersionException Type; };

    ///////////////////////////////////////////////////////////////////////////////
    /// @class InvalidStateException
    /// @headerfile exception.h
    /// @brief An exception for errors that occur when the render system is configured improperly.
    /// @details This exception mostly exists as a wrapper for it's Ogre counterpart.
    ///////////////////////////////////////////////////////////////////////////////
    class MEZZ_LIB InvalidStateException : public Exception
    {
        public:
            /// @brief The internal code for this exception.
            static const Whole ExceptionCode = Exception::INVALID_STATE_EXCEPTION;

            /// @brief Class constructor.
            /// @param Message A basic description of the error.
            /// @param SrcFunction The name of the function from which this originated.
            /// @param SrcFile The name of the file from which this originated.
            /// @param FileLine The line on the named file from which this originated.
            InvalidStateException(const String& Message, const String& SrcFunction, const String& SrcFile, const Whole& FileLine)
                : Exception("InvalidStateException",Message,SrcFunction,SrcFile,FileLine)
                {}
            /// @brief Class destructor.
            virtual ~InvalidStateException() throw() {}

            /// @copydoc Exception::GetExceptionCode()
            virtual Whole GetExceptionCode() const throw()
                { return InvalidStateException::ExceptionCode; }
    };//InvalidStateException

    template<>
    struct MEZZ_LIB ExceptionFactory<InvalidStateException::ExceptionCode>
        { typedef InvalidStateException Type; };

    ///////////////////////////////////////////////////////////////////////////////
    /// @class RenderingAPIException
    /// @headerfile exception.h
    /// @brief An exception for errors that occur in the render system.
    /// @details This exception mostly exists as a wrapper for it's Ogre counterpart.
    ///////////////////////////////////////////////////////////////////////////////
    class MEZZ_LIB RenderingAPIException : public Exception
    {
        public:
            /// @brief The internal code for this exception.
            static const Whole ExceptionCode = Exception::RENDERINGAPI_EXCEPTION;

            /// @brief Class constructor.
            /// @param Message A basic description of the error.
            /// @param SrcFunction The name of the function from which this originated.
            /// @param SrcFile The name of the file from which this originated.
            /// @param FileLine The line on the named file from which this originated.
            RenderingAPIException(const String& Message, const String& SrcFunction, const String& SrcFile, const Whole& FileLine)
                : Exception("RenderingAPIException",Message,SrcFunction,SrcFile,FileLine)
                {}
            /// @brief Class destructor.
            virtual ~RenderingAPIException() throw() {}

            /// @copydoc Exception::GetExceptionCode()
            virtual Whole GetExceptionCode() const throw()
                { return RenderingAPIException::ExceptionCode; }
    };//RenderingAPIException

    template<>
    struct MEZZ_LIB ExceptionFactory<RenderingAPIException::ExceptionCode>
        { typedef RenderingAPIException Type; };

    ///////////////////////////////////////////////////////////////////////////////
    /// @class RTAssertionException
    /// @headerfile exception.h
    /// @brief An exception for failures in run-time asserts.
    /// @details This exception mostly exists as a wrapper for it's Ogre counterpart.
    ///////////////////////////////////////////////////////////////////////////////
    class MEZZ_LIB RTAssertionException : public Exception
    {
        public:
            /// @brief The internal code for this exception.
            static const Whole ExceptionCode = Exception::RT_ASSERTION_EXCEPTION;

            /// @brief Class constructor.
            /// @param Message A basic description of the error.
            /// @param SrcFunction The name of the function from which this originated.
            /// @param SrcFile The name of the file from which this originated.
            /// @param FileLine The line on the named file from which this originated.
            RTAssertionException(const String& Message, const String& SrcFunction, const String& SrcFile, const Whole& FileLine)
                : Exception("RTAssertionException",Message,SrcFunction,SrcFile,FileLine)
                {}
            /// @brief Class destructor.
            virtual ~RTAssertionException() throw() {}

            /// @copydoc Exception::GetExceptionCode()
            virtual Whole GetExceptionCode() const throw()
                { return RTAssertionException::ExceptionCode; }
    };//RTAssertionException

    template<>
    struct MEZZ_LIB ExceptionFactory<RTAssertionException::ExceptionCode>
        { typedef RTAssertionException Type; };

    ///////////////////////////////////////////////////////////////////////////////
    /// @class InternalException
    /// @headerfile exception.h
    /// @brief An exception for internal aspects of the engine that can't be otherwise covered.
    ///////////////////////////////////////////////////////////////////////////////
    class MEZZ_LIB InternalException : public Exception
    {
        public:
            /// @brief The internal code for this exception.
            static const Whole ExceptionCode = Exception::INTERNAL_EXCEPTION;

            /// @brief Class constructor.
            /// @param Message A basic description of the error.
            /// @param SrcFunction The name of the function from which this originated.
            /// @param SrcFile The name of the file from which this originated.
            /// @param FileLine The line on the named file from which this originated.
            InternalException(const String& Message, const String& SrcFunction, const String& SrcFile, const Whole& FileLine)
                : Exception("InternalException",Message,SrcFunction,SrcFile,FileLine)
                {}
            /// @brief Class destructor.
            virtual ~InternalException() throw() {}

            /// @copydoc Exception::GetExceptionCode()
            virtual Whole GetExceptionCode() const throw()
                { return InternalException::ExceptionCode; }
    };//NotImplementedException

    template<>
    struct MEZZ_LIB ExceptionFactory<InternalException::ExceptionCode>
        { typedef InternalException Type; };

    ///////////////////////////////////////////////////////////////////////////////
    /// @class NotImplementedException
    /// @headerfile exception.h
    /// @brief An exception for area's of the engine that are used but not yet implemented.
    ///////////////////////////////////////////////////////////////////////////////
    class MEZZ_LIB NotImplementedException : public Exception
    {
        public:
            /// @brief The internal code for this exception.
            static const Whole ExceptionCode = Exception::NOT_IMPLEMENTED_EXCEPTION;

            /// @brief Class constructor.
            /// @param Message A basic description of the error.
            /// @param SrcFunction The name of the function from which this originated.
            /// @param SrcFile The name of the file from which this originated.
            /// @param FileLine The line on the named file from which this originated.
            NotImplementedException(const String& Message, const String& SrcFunction, const String& SrcFile, const Whole& FileLine)
                : Exception("NotImplementedException",Message,SrcFunction,SrcFile,FileLine)
                {}
            /// @brief Class destructor.
            virtual ~NotImplementedException() throw() {}

            /// @copydoc Exception::GetExceptionCode()
            virtual Whole GetExceptionCode() const throw()
                { return NotImplementedException::ExceptionCode; }
    };//NotImplementedException

    template<>
    struct MEZZ_LIB ExceptionFactory<NotImplementedException::ExceptionCode>
        { typedef NotImplementedException Type; };

    ///////////////////////////////////////////////////////////////////////////////
    // Layer 3

    ///////////////////////////////////////////////////////////////////////////////
    /// @class IOReadException
    /// @headerfile exception.h
    /// @brief An exception for the event of not being able to read from an IO operation.
    ///////////////////////////////////////////////////////////////////////////////
    class MEZZ_LIB IOReadException : public IOException
    {
        public:
            /// @brief The internal code for this exception.
            static const Whole ExceptionCode = Exception::IO_READ_EXCEPTION;

            /// @brief Class constructor.
            /// @param Message A basic description of the error.
            /// @param SrcFunction The name of the function from which this originated.
            /// @param SrcFile The name of the file from which this originated.
            /// @param FileLine The line on the named file from which this originated.
            IOReadException(const String& Message, const String& SrcFunction, const String& SrcFile, const Whole& FileLine)
                : IOException("IOReadException",Message,SrcFunction,SrcFile,FileLine)
                {}
            /// @brief Class destructor.
            virtual ~IOReadException() throw() {}

            /// @copydoc Exception::GetExceptionCode()
            Whole GetExceptionCode() const throw()
                { return IOReadException::ExceptionCode; }
    };//IOReadException

    template<>
    struct MEZZ_LIB ExceptionFactory<IOReadException::ExceptionCode>
        { typedef IOReadException Type; };

    ///////////////////////////////////////////////////////////////////////////////
    /// @class IOWriteException
    /// @headerfile exception.h
    /// @brief An exception for the event of not being able to write to an IO operation.
    ///////////////////////////////////////////////////////////////////////////////
    class MEZZ_LIB IOWriteException : public IOException
    {
        public:
            /// @brief The internal code for this exception.
            static const Whole ExceptionCode = Exception::IO_WRITE_EXCEPTION;

            /// @brief Class constructor.
            /// @param Message A basic description of the error.
            /// @param SrcFunction The name of the function from which this originated.
            /// @param SrcFile The name of the file from which this originated.
            /// @param FileLine The line on the named file from which this originated.
            IOWriteException(const String& Message, const String& SrcFunction, const String& SrcFile, const Whole& FileLine)
                : IOException("IOWriteException",Message,SrcFunction,SrcFile,FileLine)
                {}
            /// @brief Class destructor.
            virtual ~IOWriteException() throw() {}

            /// @copydoc Exception::GetExceptionCode()
            Whole GetExceptionCode() const throw()
                { return IOWriteException::ExceptionCode; }
    };//IOWriteException

    template<>
    struct MEZZ_LIB ExceptionFactory<IOWriteException::ExceptionCode>
        { typedef IOWriteException Type; };

    ///////////////////////////////////////////////////////////////////////////////
    /// @class FileNotFoundException
    /// @headerfile exception.h
    /// @brief An exception for the event of not finding a requested file.
    ///////////////////////////////////////////////////////////////////////////////
    class MEZZ_LIB FileNotFoundException : public IOException
    {
        public:
            /// @brief The internal code for this exception.
            static const Whole ExceptionCode = Exception::IO_FILE_NOT_FOUND_EXCEPTION;

            /// @brief Class constructor.
            /// @param Message A basic description of the error.
            /// @param SrcFunction The name of the function from which this originated.
            /// @param SrcFile The name of the file from which this originated.
            /// @param FileLine The line on the named file from which this originated.
            FileNotFoundException(const String& Message, const String& SrcFunction, const String& SrcFile, const Whole& FileLine)
                : IOException("FileNotFoundException",Message,SrcFunction,SrcFile,FileLine)
                {}
            /// @brief Class destructor.
            virtual ~FileNotFoundException() throw() {}

            /// @copydoc Exception::GetExceptionCode()
            Whole GetExceptionCode() const throw()
                { return FileNotFoundException::ExceptionCode; }
    };//FileNotFoundException

    template<>
    struct MEZZ_LIB ExceptionFactory<FileNotFoundException::ExceptionCode>
        { typedef FileNotFoundException Type; };

    ///////////////////////////////////////////////////////////////////////////////
    /// @class DirectoryNotFoundException
    /// @headerfile exception.h
    /// @brief An exception for the event of not finding a requested directory or path.
    ///////////////////////////////////////////////////////////////////////////////
    class MEZZ_LIB DirectoryNotFoundException : public IOException
    {
        public:
            /// @brief The internal code for this exception.
            static const Whole ExceptionCode = Exception::IO_DIRECTORY_NOT_FOUND_EXCEPTION;

            /// @brief Class constructor.
            /// @param Message A basic description of the error.
            /// @param SrcFunction The name of the function from which this originated.
            /// @param SrcFile The name of the file from which this originated.
            /// @param FileLine The line on the named file from which this originated.
            DirectoryNotFoundException(const String& Message, const String& SrcFunction, const String& SrcFile, const Whole& FileLine)
                : IOException("DirectoryNotFoundException",Message,SrcFunction,SrcFile,FileLine)
                {}
            /// @brief Class destructor.
            virtual ~DirectoryNotFoundException() throw() {}

            /// @copydoc Exception::GetExceptionCode()
            Whole GetExceptionCode() const throw()
                { return DirectoryNotFoundException::ExceptionCode; }
    };//DirectoryNotFoundException

    template<>
    struct MEZZ_LIB ExceptionFactory<DirectoryNotFoundException::ExceptionCode>
        { typedef DirectoryNotFoundException Type; };

    ///////////////////////////////////////////////////////////////////////////////
    /// @class IdentityInvalidException
    /// @headerfile exception.h
    /// @brief An exception for the event of encountering an invalid identity.
    ///////////////////////////////////////////////////////////////////////////////
    class MEZZ_LIB IdentityInvalidException : public InstanceIdentityException
    {
        public:
            /// @brief The internal code for this exception.
            static const Whole ExceptionCode = Exception::II_IDENTITY_INVALID_EXCEPTION;

            /// @brief Class constructor.
            /// @param Message A basic description of the error.
            /// @param SrcFunction The name of the function from which this originated.
            /// @param SrcFile The name of the file from which this originated.
            /// @param FileLine The line on the named file from which this originated.
            IdentityInvalidException(const String& Message, const String& SrcFunction, const String& SrcFile, const Whole& FileLine)
                : InstanceIdentityException("IdentityInvalidException",Message,SrcFunction,SrcFile,FileLine)
                {}
            /// @brief Class destructor.
            virtual ~IdentityInvalidException() throw() {}

            /// @copydoc Exception::GetExceptionCode()
            Whole GetExceptionCode() const throw()
                { return IdentityInvalidException::ExceptionCode; }
    };//IdentityInvalidException

    template<>
    struct MEZZ_LIB ExceptionFactory<IdentityInvalidException::ExceptionCode>
        { typedef IdentityInvalidException Type; };

    ///////////////////////////////////////////////////////////////////////////////
    /// @class IdentityNotFoundException
    /// @headerfile exception.h
    /// @brief An exception for the event of not finding a requested object by it's identity.
    ///////////////////////////////////////////////////////////////////////////////
    class MEZZ_LIB IdentityNotFoundException : public InstanceIdentityException
    {
        public:
            /// @brief The internal code for this exception.
            static const Whole ExceptionCode = Exception::II_IDENTITY_NOT_FOUND_EXCEPTION;

            /// @brief Class constructor.
            /// @param Message A basic description of the error.
            /// @param SrcFunction The name of the function from which this originated.
            /// @param SrcFile The name of the file from which this originated.
            /// @param FileLine The line on the named file from which this originated.
            IdentityNotFoundException(const String& Message, const String& SrcFunction, const String& SrcFile, const Whole& FileLine)
                : InstanceIdentityException("IdentityNotFoundException",Message,SrcFunction,SrcFile,FileLine)
                {}
            /// @brief Class destructor.
            virtual ~IdentityNotFoundException() throw() {}

            /// @copydoc Exception::GetExceptionCode()
            Whole GetExceptionCode() const throw()
                { return IdentityNotFoundException::ExceptionCode; }
    };//IdentityNotFoundException

    template<>
    struct MEZZ_LIB ExceptionFactory<IdentityNotFoundException::ExceptionCode>
        { typedef IdentityNotFoundException Type; };

    ///////////////////////////////////////////////////////////////////////////////
    /// @class DuplicateIdentityException
    /// @headerfile exception.h
    /// @brief An exception for the event of finding duplicate of a unique identity.
    ///////////////////////////////////////////////////////////////////////////////
    class MEZZ_LIB DuplicateIdentityException : public InstanceIdentityException
    {
        public:
            /// @brief The internal code for this exception.
            static const Whole ExceptionCode = Exception::II_DUPLICATE_IDENTITY_EXCEPTION;

            /// @brief Class constructor.
            /// @param Message A basic description of the error.
            /// @param SrcFunction The name of the function from which this originated.
            /// @param SrcFile The name of the file from which this originated.
            /// @param FileLine The line on the named file from which this originated.
            DuplicateIdentityException(const String& Message, const String& SrcFunction, const String& SrcFile, const Whole& FileLine)
                : InstanceIdentityException("DuplicateIdentityException",Message,SrcFunction,SrcFile,FileLine)
                {}
            /// @brief Class destructor.
            virtual ~DuplicateIdentityException() throw() {}

            /// @copydoc Exception::GetExceptionCode()
            Whole GetExceptionCode() const throw()
                { return FileNotFoundException::ExceptionCode; }
    };//DuplicateIdentityException

    template<>
    struct MEZZ_LIB ExceptionFactory<DuplicateIdentityException::ExceptionCode>
        { typedef DuplicateIdentityException Type; };

    ///////////////////////////////////////////////////////////////////////////////
    /// @class OutOfMemoryException
    /// @headerfile exception.h
    /// @brief An exception for the event of a failure to allocate new memory.
    ///////////////////////////////////////////////////////////////////////////////
    class MEZZ_LIB OutOfMemoryException : public MemoryManagementException
    {
        public:
            /// @brief The internal code for this exception.
            static const Whole ExceptionCode = Exception::MM_OUT_OF_MEMORY_EXCEPTION;

            /// @brief Class constructor.
            /// @param Message A basic description of the error.
            /// @param SrcFunction The name of the function from which this originated.
            /// @param SrcFile The name of the file from which this originated.
            /// @param FileLine The line on the named file from which this originated.
            OutOfMemoryException(const String& Message, const String& SrcFunction, const String& SrcFile, const Whole& FileLine)
                : MemoryManagementException("OutOfMemoryException",Message,SrcFunction,SrcFile,FileLine)
                {}
            /// @brief Class destructor.
            virtual ~OutOfMemoryException() throw() {}

            /// @copydoc Exception::GetExceptionCode()
            Whole GetExceptionCode() const throw()
                { return OutOfMemoryException::ExceptionCode; }
    };//OutOfMemoryException

    template<>
    struct MEZZ_LIB ExceptionFactory<OutOfMemoryException::ExceptionCode>
        { typedef OutOfMemoryException Type; };

    ///////////////////////////////////////////////////////////////////////////////
    /// @class OutOfBoundsException
    /// @headerfile exception.h
    /// @brief An exception for the event of accessing memory outside of a specified area of memory.
    ///////////////////////////////////////////////////////////////////////////////
    class MEZZ_LIB OutOfBoundsException : public MemoryManagementException
    {
        public:
            /// @brief The internal code for this exception.
            static const Whole ExceptionCode = Exception::MM_OUT_OF_BOUNDS_EXCEPTION;

            /// @brief Class constructor.
            /// @param Message A basic description of the error.
            /// @param SrcFunction The name of the function from which this originated.
            /// @param SrcFile The name of the file from which this originated.
            /// @param FileLine The line on the named file from which this originated.
            OutOfBoundsException(const String& Message, const String& SrcFunction, const String& SrcFile, const Whole& FileLine)
                : MemoryManagementException("OutOfBoundsException",Message,SrcFunction,SrcFile,FileLine)
                {}
            /// @brief Class destructor.
            virtual ~OutOfBoundsException() throw() {}

            /// @copydoc Exception::GetExceptionCode()
            Whole GetExceptionCode() const throw()
                { return OutOfBoundsException::ExceptionCode; }
    };//OutOfBoundsException

    template<>
    struct MEZZ_LIB ExceptionFactory<OutOfBoundsException::ExceptionCode>
        { typedef OutOfBoundsException Type; };

    #ifndef MEZZ_EXCEPTION
    #define MEZZ_EXCEPTION(num, desc) throw Mezzanine::ExceptionFactory<num>::Type(desc, __func__, __FILE__, __LINE__ );
    #endif
}//Mezzanine

#endif // \exception_cpp
