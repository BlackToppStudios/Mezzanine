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
            /// @details The numbers listed here must be unique to each exception. If an error code does not identify
            /// the problem exactly it should be considered 'Unknown' for as much as it does specify. For example,
            /// if there is an error opening a file for reading and writing, and we don't know why it failed, useing
            /// @ref IO_EXCEPTION would make sense
            enum ExceptionCodes
            {
                // Input/Output exceptions
                IO_EXCEPTION                        = 0x01F00F00,  ///< Used when There is an Error with IO and it could not be identified. Using this will generate @ref IOException instances.
                IO_READ_EXCEPTION                   = 0x01F01F00,  ///< Using this with will generate @ref IOReadException instances.
                IO_WRITE_EXCEPTION                  = 0x01F02F00,  ///< Using this will generate @ref IOWriteException instances.
                IO_FILE_NOT_FOUND_EXCEPTION         = 0x01F03F00,  ///< Using this will generate @ref FileNotFoundException instances.
                IO_DIRECTORY_NOT_FOUND_EXCEPTION    = 0x01F04F00,  ///< Using this will generate @ref DirectoryNotFoundException instances.
                // Instance Identity exceptions
                II_EXCEPTION                        = 0x02F00F00,  ///< The catchall invalid index exception, Using this will generate @ref InstanceIdentityException instances.
                II_IDENTITY_INVALID_EXCEPTION       = 0x02F01F00,  ///< Using this will generate @ref IdentityInvalidException instances
                II_IDENTITY_NOT_FOUND_EXCEPTION     = 0x02F02F00,  ///< Using this will generate @ref IdentityNotFoundException instances
                II_DUPLICATE_IDENTITY_EXCEPTION     = 0x02F03F00,  ///< Using this will generate @ref DuplicateIdentityException instances
                // Memory Management exceptions
                MM_EXCEPTION                        = 0x03F00F00,  ///< Generic Memory Management Exception, using this will generate @ref MemoryManagementException instances
                MM_OUT_OF_MEMORY_EXCEPTION          = 0x03F01F00,  ///< Using this will generate @ref OutOfMemoryException instances
                MM_OUT_OF_BOUNDS_EXCEPTION          = 0x03F02F00,  ///< Using this will generate @ref OutOfBoundsException instances
                // Syntax exceptions
                SYNTAX_ERROR_EXCEPTION              = 0x04F00F00,  ///< If the kind of syntax is unknown this can be used directly. Using this will generate @ref SyntaxErrorException instances
                SYNTAX_ERROR_EXCEPTION_XML          = 0x04F01F00,  ///< Using this will generate @ref SyntaxErrorXMLException instances
                SYNTAX_ERROR_EXCEPTION_LUA          = 0x04F02F00,  ///< Using this will generate @ref SyntaxErrorLuaException instances
                // Scripting Error
                SCRIPT_EXCEPTION                    = 0x05F00F00,  ///< Using this will generate @ref ScriptException instances
                SCRIPT_EXCEPTION_LUA                = 0x05F01F00,  ///< Using this will generate @ref SyntaxErrorLuaYieldException instances
                SCRIPT_EXCEPTION_LUA_YIELD          = 0x05F01F01,  ///< Using this will generate @ref SyntaxErrorLuaYieldException instances
                SCRIPT_EXCEPTION_LUA_RUNTIME        = 0x05F01F02,  ///< Using this will generate @ref SyntaxErrorLuaRuntimeException instances
                SCRIPT_EXCEPTION_LUA_ERRERR         = 0x05F01F03,  ///< Using this will generate @ref SyntaxErrorLuaErrException instances. This needs further clarification.
                // Other exceptions
                ARITHMETIC_EXCEPTION                = 0x00F01F00,  // 16
                INVALID_PARAMETERS_EXCEPTION        = 0x00F02F00,  // 17
                INVALID_VERSION_EXCEPTION           = 0x00F03F00,  // 18
                INVALID_STATE_EXCEPTION             = 0x00F04F00,  // 19
                RENDERINGAPI_EXCEPTION              = 0x00F05F00,  // 20
                RT_ASSERTION_EXCEPTION              = 0x00F06F00,  // 21
                INTERNAL_EXCEPTION                  = 0x00F07F00,  // 22
                NOT_IMPLEMENTED_EXCEPTION           = 0x00F08F00   // 23
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
            /// @details Don't call this, use @ref MEZZ_EXCEPTION to throw these, it is much simpler. It also might get better with time.
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
            static const Whole ExceptionCode = Exception::IO_EXCEPTION;
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
            static const Whole ExceptionCode = Exception::II_EXCEPTION;
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
            static const Whole ExceptionCode = Exception::MM_EXCEPTION;
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
                : Exception("SyntaxErrorException",Message,SrcFunction,SrcFile,FileLine)
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
    /// @class ScriptException
    /// @headerfile exception.h
    /// @brief Some kind of exception happened in a script.
    ///////////////////////////////////////////////////////////////////////////////
    class MEZZ_LIB ScriptException : public Exception
    {
        public:
            /// @brief The internal code for this exception.
            static const Whole ExceptionCode = Exception::SCRIPT_EXCEPTION;
        protected:
            /// @brief Inheritance constructor.
            /// @param TypeName The name of the type of exception being thrown.
            /// @param Message A basic description of the error.
            /// @param SrcFunction The name of the function from which this originated.
            /// @param SrcFile The name of the file from which this originated.
            /// @param FileLine The line on the named file from which this originated.
            ScriptException(const String& TypeName, const String& Message, const String& SrcFunction, const String& SrcFile, const Whole& FileLine)
                : Exception(TypeName,Message,SrcFunction,SrcFile,FileLine)
                {}
        public:
            /// @brief Class constructor.
            /// @param Message A basic description of the error.
            /// @param SrcFunction The name of the function from which this originated.
            /// @param SrcFile The name of the file from which this originated.
            /// @param FileLine The line on the named file from which this originated.
            ScriptException(const String& Message, const String& SrcFunction, const String& SrcFile, const Whole& FileLine)
                : Exception("ScriptException",Message,SrcFunction,SrcFile,FileLine)
                {}
            /// @brief Class destructor.
            virtual ~ScriptException() throw() {}

            /// @copydoc Exception::GetExceptionCode()
            virtual Whole GetExceptionCode() const throw()
                { return ScriptException::ExceptionCode; }
    };//InvalidSyntaxException

    template<>
    struct MEZZ_LIB ExceptionFactory<ScriptException::ExceptionCode>
        { typedef ScriptException Type; };


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


    ///////////////////////////////////////////////////////////////////////////////
    /// @class SyntaxErrorXMLException
    /// @headerfile exception.h
    /// @brief An exception to be thrown when malformed XML is encountered.
    ///////////////////////////////////////////////////////////////////////////////
    class MEZZ_LIB SyntaxErrorXMLException : public SyntaxErrorException
    {
        public:
            /// @brief The internal code for this exception.
            static const Whole ExceptionCode = Exception::SYNTAX_ERROR_EXCEPTION_XML;

            /// @brief Class constructor.
            /// @param Message A basic description of the error.
            /// @param SrcFunction The name of the function from which this originated.
            /// @param SrcFile The name of the file from which this originated.
            /// @param FileLine The line on the named file from which this originated.
            SyntaxErrorXMLException(const String& Message, const String& SrcFunction, const String& SrcFile, const Whole& FileLine)
                : SyntaxErrorException("SyntaxErrorXMLException",Message,SrcFunction,SrcFile,FileLine)
                {}
            /// @brief Class destructor.
            virtual ~SyntaxErrorXMLException() throw() {}

            /// @copydoc Exception::GetExceptionCode()
            Whole GetExceptionCode() const throw()
                { return SyntaxErrorXMLException::ExceptionCode; }
    };//SyntaxErrorXMLException

    template<>
    struct MEZZ_LIB ExceptionFactory<SyntaxErrorXMLException::ExceptionCode>
        { typedef SyntaxErrorXMLException Type; };


    ///////////////////////////////////////////////////////////////////////////////
    /// @class SyntaxErrorLuaException
    /// @headerfile exception.h
    /// @brief An exception to be thrown when malformed Lua is encountered.
    ///////////////////////////////////////////////////////////////////////////////
    class MEZZ_LIB SyntaxErrorLuaException : public SyntaxErrorException
    {
        protected:
            /// @brief Inheritance constructor.
            /// @param TypeName The name of the type of exception being thrown.
            /// @param Message A basic description of the error.
            /// @param SrcFunction The name of the function from which this originated.
            /// @param SrcFile The name of the file from which this originated.
            /// @param FileLine The line on the named file from which this originated.
            SyntaxErrorLuaException(const String& TypeName, const String& Message, const String& SrcFunction, const String& SrcFile, const Whole& FileLine)
                : SyntaxErrorException(TypeName,Message,SrcFunction,SrcFile,FileLine)
                {}
        public:
            /// @brief The internal code for this exception.
            static const Whole ExceptionCode = Exception::SYNTAX_ERROR_EXCEPTION_LUA;

            /// @brief Class constructor.
            /// @param Message A basic description of the error.
            /// @param SrcFunction The name of the function from which this originated.
            /// @param SrcFile The name of the file from which this originated.
            /// @param FileLine The line on the named file from which this originated.
            SyntaxErrorLuaException(const String& Message, const String& SrcFunction, const String& SrcFile, const Whole& FileLine)
                : SyntaxErrorException("SyntaxErrorLuaException",Message,SrcFunction,SrcFile,FileLine)
                {}
            /// @brief Class destructor.
            virtual ~SyntaxErrorLuaException() throw() {}

            /// @copydoc Exception::GetExceptionCode()
            Whole GetExceptionCode() const throw()
                { return SyntaxErrorLuaException::ExceptionCode; }
    };//SyntaxErrorLuaException

    template<>
    struct MEZZ_LIB ExceptionFactory<SyntaxErrorLuaException::ExceptionCode>
        { typedef SyntaxErrorLuaException Type; };


    ///////////////////////////////////////////////////////////////////////////////
    // Layer 4


    ///////////////////////////////////////////////////////////////////////////////
    /// @class SyntaxErrorLuaYieldException
    /// @headerfile exception.h
    /// @brief Lua returned an error condition of yeild (which is sometimes valid)
    ///////////////////////////////////////////////////////////////////////////////
    class MEZZ_LIB SyntaxErrorLuaYieldException : public SyntaxErrorLuaException
    {
        public:
            /// @brief The internal code for this exception.
            static const Whole ExceptionCode = Exception::SCRIPT_EXCEPTION_LUA_YIELD;

            /// @brief Class constructor.
            /// @param Message A basic description of the error.
            /// @param SrcFunction The name of the function from which this originated.
            /// @param SrcFile The name of the file from which this originated.
            /// @param FileLine The line on the named file from which this originated.
            SyntaxErrorLuaYieldException(const String& Message, const String& SrcFunction, const String& SrcFile, const Whole& FileLine)
                : SyntaxErrorLuaException("SyntaxErrorLuaYieldException",Message,SrcFunction,SrcFile,FileLine)
                {}
            /// @brief Class destructor.
            virtual ~SyntaxErrorLuaYieldException() throw() {}

            /// @copydoc Exception::GetExceptionCode()
            Whole GetExceptionCode() const throw()
                { return SyntaxErrorLuaYieldException::ExceptionCode; }
    };//SyntaxErrorLuaException

    template<>
    struct MEZZ_LIB ExceptionFactory<SyntaxErrorLuaYieldException::ExceptionCode>
        { typedef SyntaxErrorLuaYieldException Type; };


    ///////////////////////////////////////////////////////////////////////////////
    /// @class SyntaxErrorLuaRuntimeException
    /// @headerfile exception.h
    /// @brief Lua returned an error during execution
    ///////////////////////////////////////////////////////////////////////////////
    class MEZZ_LIB SyntaxErrorLuaRuntimeException : public SyntaxErrorLuaException
    {
        public:
            /// @brief The internal code for this exception.
            static const Whole ExceptionCode = Exception::SCRIPT_EXCEPTION_LUA_RUNTIME;

            /// @brief Class constructor.
            /// @param Message A basic description of the error.
            /// @param SrcFunction The name of the function from which this originated.
            /// @param SrcFile The name of the file from which this originated.
            /// @param FileLine The line on the named file from which this originated.
            SyntaxErrorLuaRuntimeException(const String& Message, const String& SrcFunction, const String& SrcFile, const Whole& FileLine)
                : SyntaxErrorLuaException("SyntaxErrorLuaRuntimeException",Message,SrcFunction,SrcFile,FileLine)
                {}
            /// @brief Class destructor.
            virtual ~SyntaxErrorLuaRuntimeException() throw() {}

            /// @copydoc Exception::GetExceptionCode()
            Whole GetExceptionCode() const throw()
                { return SyntaxErrorLuaRuntimeException::ExceptionCode; }
    };//SyntaxErrorLuaException

    template<>
    struct MEZZ_LIB ExceptionFactory<SyntaxErrorLuaRuntimeException::ExceptionCode>
        { typedef SyntaxErrorLuaRuntimeException Type; };


    ///////////////////////////////////////////////////////////////////////////////
    /// @class SyntaxErrorLuaErrException
    /// @headerfile exception.h
    /// @brief Lua suffered an internal error.
    ///////////////////////////////////////////////////////////////////////////////
    class MEZZ_LIB SyntaxErrorLuaErrException : public SyntaxErrorLuaException
    {
        public:
            /// @brief The internal code for this exception.
            static const Whole ExceptionCode = Exception::SCRIPT_EXCEPTION_LUA_ERRERR;

            /// @brief Class constructor.
            /// @param Message A basic description of the error.
            /// @param SrcFunction The name of the function from which this originated.
            /// @param SrcFile The name of the file from which this originated.
            /// @param FileLine The line on the named file from which this originated.
            SyntaxErrorLuaErrException(const String& Message, const String& SrcFunction, const String& SrcFile, const Whole& FileLine)
                : SyntaxErrorLuaException("SyntaxErrorLuaErrException",Message,SrcFunction,SrcFile,FileLine)
                {}
            /// @brief Class destructor.
            virtual ~SyntaxErrorLuaErrException() throw() {}

            /// @copydoc Exception::GetExceptionCode()
            Whole GetExceptionCode() const throw()
                { return SyntaxErrorLuaErrException::ExceptionCode; }
    };//SyntaxErrorLuaErrException

    template<>
    struct MEZZ_LIB ExceptionFactory<SyntaxErrorLuaErrException::ExceptionCode>
        { typedef SyntaxErrorLuaErrException Type; };



    #ifndef MEZZ_EXCEPTION
    /// @brief An easy way to throw exception with rich information.
    /// @details An important part of troubleshooting errors from the users perspective is being able to
    /// tie a specific 'fix' to a specific error message. An important part of ensuring correct exceptional
    /// is catching the right exceptions at the right time. It is also important to not allocate more
    /// memory or other resources while creating an exception.
    /// @n @n
    /// This macro makes doing all of these easy. Every exception thrown by this macro with provide the Function name,
    /// the file name and the line in the file from which it was thrown. That provides all the information the developer
    /// needs to identify the issue. This uses some specific template machinery to generate specifically typed exceptions
    /// static instances at compile to insure the behaviors a programmer needs. Since these are allocated when the
    /// program is first loaded so there will be no allocations when this is called, and the type is controlled by the
    /// error number parameter.
    /// @n @n
    /// As long as the game developer provides a unique string for each failure, then any messages logged or presented
    /// to the game player will uniquely identify that specific problem. This allows the user to perform very specific
    /// web searches and potentially allows troubleshooters/technicians to skip length diagnostics steps.
    /// @param num A specific code from the @ref Exception::ExceptionCodes enum will control the type of exception produced.
    /// @param desc
    /// @todo Get the verion of the Mezzanine in here so that this can pin down errors with extreme specificty across time. (aybe Compilaiton date too)
    #define MEZZ_EXCEPTION(num, desc) throw Mezzanine::ExceptionFactory<num>::Type(desc, __func__, __FILE__, __LINE__ );
    #endif
}//Mezzanine

#endif // \exception_cpp
