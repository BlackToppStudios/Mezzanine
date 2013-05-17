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

#ifndef _exception_h
#define _exception_h

#include "datatypes.h"

SWIG_INFO_BEGINCLASS

namespace Mezzanine
{
    #ifdef SWIG
    %nodefaultctor Foo;
    #endif
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
            IO_EXCEPTION                         = 0x01F00F00,    ///< When used a instance of IOException is thrown. Thrown when there was an issue with IO but very little was known about it.
            IO_FILE_EXCEPTION                    = 0x01F01F00,    ///< When used a instance of FileException is thrown. Thrown when there is an unknown issue with a file.
            IO_FILE_READ_EXCEPTION               = 0x01F01F01,    ///< When used a instance of FileReadException is thrown. Thrown when a file could not be read and permissions seem to not be an issue (Filesystem/hardware issue?)
            IO_FILE_WRITE_EXCEPTION              = 0x01F01F02,    ///< When used a instance of FileWriteException is thrown. Thrown when a file could not be written and perimssions seem not to be an issue.
            IO_FILE_NOT_FOUND_EXCEPTION          = 0x01F01F03,    ///< When used a instance of FileNotFoundException is thrown. Thrown when a file was expected to be there, but was not.
            IO_FILE_PERMISSION_EXCEPTION         = 0x01F01F04,    ///< When used a instance of FilePermissionException is thrown. Thrown when permission was denied to a file.
            IO_DIRECTORY_EXCEPTION               = 0x01F02F00,    ///< When used a instance of DirectoryException is thrown. Thrown when there is an unknown issue with a file.
            IO_DIRECTORY_READ_EXCEPTION          = 0x01F02F01,    ///< When used a instance of DirectoryReadException is thrown. Thrown when a directory could be read and it wasn't a permission issue.
            IO_DIRECTORY_WRITE_EXCEPTION         = 0x01F02F02,    ///< When used a instance of DirectoryWriteException is thrown. Thrown when a directory could be written to and it wasn't a permission issue.
            IO_DIRECTORY_NOT_FOUND_EXCEPTION     = 0x01F02F03,    ///< When used a instance of DirectoryNotFoundException is thrown. Thrown when a directory was expected to be there, but was not.
            IO_DIRECTORY_PERMISSION_EXCEPTION    = 0x01F02F04,    ///< When used a instance of DirectoryPermissionException is thrown. Thrown when permission is denied to a directory.
            IO_NETWORK_EXCEPTION                 = 0x01F03F00,    ///< When used a instance of NetworkException is thrown. Thrown when something unknown causes network IO to fail.
            IO_NETWORK_READ_EXCEPTION            = 0x01F03F01,    ///< When used a instance of NetworkReadException is thrown. Thrown when data could not be read from the network (downloads).
            IO_NETWORK_WRITE_EXCEPTION           = 0x01F03F02,    ///< When used a instance of NetworkWriteException is thrown. Thrown when data could not be read from the network (iloads).
            IO_NETWORK_NOT_FOUND_EXCEPTION       = 0x01F03F03,    ///< When used a instance of NetworkNotFoundException is thrown. Thrown when no network connection is available.
            IO_NETWORK_URL_EXCEPTION             = 0x01F03F04,    ///< When used a instance of NetworkURLException is thrown. Thrown when an address is invalid or could not be found.
            IO_NETWORK_PERMISSION_EXCEPTION      = 0x01F03F05,    ///< When used a instance of NetworkPermissionException is thrown. Thrown when permision was denied to a network interface or network resource.
            IO_WRITE_EXCEPTION                   = 0x01F08F00,    ///< When used a instance of IOWriteException is thrown. Thrown when a write is happening but something has prevented the underlying code from knowing what was writing.
            IO_READ_EXCEPTION                    = 0x01F09F00,    ///< When used a instance of IOReadException is thrown. Thrown when a read is happening but something has prevented the underlying code from knowing what was reading.

            II_EXCEPTION                         = 0x02F00F00,    ///< When used a instance of InstanceIdentityException is thrown. Thrown when an unknown error with using an Identifier and it is invalid.
            II_IDENTITY_INVALID_EXCEPTION        = 0x02F01F00,    ///< When used a instance of InstanceIdentityInvalidException is thrown. Thrown when the identity string wasn't valid at all.
            II_IDENTITY_NOT_FOUND_EXCEPTION      = 0x02F02F00,    ///< When used a instance of InstanceIdentityNotFoundException is thrown. Thrown when the requested identity could not be found.
            II_DUPLICATE_IDENTITY_EXCEPTION      = 0x02F03F00,    ///< When used a instance of InstanceIdentityDuplicateException is thrown. Thrown when duplicates of teh same identity string exist.

            MM_EXCEPTION                         = 0x03F00F00,    ///< When used a instance of MemoryManagementException is thrown. Thrown when an unknown memory management exception occurs.
            MM_OUT_OF_MEMORY_EXCEPTION           = 0x03F01F00,    ///< When used a instance of OutOfMemoryException is thrown. Thrown when A memory allocation was attempted and failed.
            MM_OUT_OF_BOUNDS_EXCEPTION           = 0x03F02F00,    ///< When used a instance of MemoryOutOfBoundsException is thrown. Thrown when attempted to access something that really should note be accessed.

            SYNTAX_ERROR_EXCEPTION               = 0x04F00F00,    ///< When used a instance of SyntaxErrorException is thrown. Thrown when some kind of syntax exception
            SYNTAX_ERROR_EXCEPTION_XML           = 0x04F01F00,    ///< When used a instance of SyntaxErrorXMLException is thrown. Thrown when and XML document is being parsed but is invalid
            SYNTAX_ERROR_EXCEPTION_XPATH         = 0x04F02F00,    ///< When used a instance of SyntaxErrorXPathException is thrown. Thrown when an XPath query is being parsed but is invalid
            SYNTAX_ERROR_EXCEPTION_LUA           = 0x04F03F00,    ///< When used a instance of SyntaxErrorLuaException is thrown. Thrown when lua code in incorrect.

            SCRIPT_EXCEPTION                     = 0x05F00F00,    ///< When used a instance of ScriptException is thrown. Thrown when an unknown error happens with a script.
            SCRIPT_EXCEPTION_LUA                 = 0x05F01F00,    ///< When used a instance of ScriptLuaException is thrown. Thrown when an unknown error happens in a Lua script.
            SCRIPT_EXCEPTION_LUA_YIELD           = 0x05F01F01,    ///< When used a instance of ScriptLuaYieldException is thrown. Thrown when Lua returns a yield and it should not have.
            SCRIPT_EXCEPTION_LUA_RUNTIME         = 0x05F01F02,    ///< When used a instance of ScriptLuaRuntimeException is thrown. Thrown when a Lua script has a runtime error.
            SCRIPT_EXCEPTION_LUA_ERRERR          = 0x05F01F03,    ///< When used a instance of ScriptLuaErrErrException is thrown. Thrown when Lua has an error handling an error.

            ARITHMETIC_EXCEPTION                 = 0x00F01F00,    ///< When used a instance of ArithmeticException is thrown. Thrown when Math has failed.
            INVALID_PARAMETERS_EXCEPTION         = 0x00F02F00,    ///< When used a instance of InvalidParametersException is thrown. Thrown when parameters are checked at runtime and found invalid.
            INVALID_VERSION_EXCEPTION            = 0x00F03F00,    ///< When used a instance of InvalidVersionException is thrown. Thrown when a version is accessed/parsed/required and it cannot work correctly or is missing.
            INVALID_STATE_EXCEPTION              = 0x00F04F00,    ///< When used a instance of InvalidStateException is thrown. Thrown when the available information should have worked but failed for unknown reasons.
            RENDERINGAPI_EXCEPTION               = 0x00F05F00,    ///< When used a instance of RenderingAPIException is thrown. Thrown when the graphics card/DirectX/OpenGL fail.
            RT_ASSERTION_EXCEPTION               = 0x00F06F00,    ///< When used a instance of RuntimeAssertionException is thrown. Thrown when a rutime assertion could have been thrown.
            INTERNAL_EXCEPTION                   = 0x00F07F00,    ///< When used a instance of InternalException is thrown. Thrown when an unknown internal error occurred.
            NOT_IMPLEMENTED_EXCEPTION            = 0x00F08F00,    ///< When used a instance of NotImplementedException is thrown. Thrown when we just have not coded a thing yet, but we knew what the API should look like.
            INVALID_ASSIGNMENT                   = 0x00F09F00     ///< When used a instance of InvalidAssignment is thrown. Then when a complex class is assigned to itself or other invalid assignments occur.
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
    // Exception code class definitions.

    ///////////////////////////////////////////////////////////////////////////////
    // Exception code class definitions.

    ///////////////////////////////////////////////////////////////////////////////
    /// @brief Thrown when there was an issue with IO but very little was known about it.
    ///////////////////
    class MEZZ_LIB IOException : public Exception
    {
    public:
        /// @brief Thrown when there was an issue with IO but very little was known about it.
        static const Whole ExceptionCode = Exception::IO_EXCEPTION;

        /// @brief Class constructor.
        /// @param TypeName The name of this class.
        /// @param Message A basic description of the error.
        /// @param SrcFunction The name of the function from which this originated.
        /// @param SrcFile The name of the file from which this originated.
        /// @param FileLine The line on the named file from which this originated.
        IOException(const String& TypeName, const String& Message, const String& SrcFunction, const String& SrcFile, const Whole& FileLine)
            : Exception("IOException", Message, SrcFunction, SrcFile, FileLine)
            {}

        /// @brief Class constructor.
        /// @param Message A basic description of the error.
        /// @param SrcFunction The name of the function from which this originated.
        /// @param SrcFile The name of the file from which this originated.
        /// @param FileLine The line on the named file from which this originated.
        IOException(const String& Message, const String& SrcFunction, const String& SrcFile, const Whole& FileLine)
            : Exception("IOException", Message, SrcFunction, SrcFile, FileLine)
            {}

        /// @brief Class destructor.
        virtual ~IOException() throw() {}

        /// @copydoc Exception::GetExceptionCode()
        virtual Whole GetExceptionCode() const throw()
            { return IOException::ExceptionCode; }
    }; //IOException

    /// @brief A template metaprogramming construct used to ensure that all required IOException can be created at compile time if the compiler is clever.
    template<>
    struct MEZZ_LIB ExceptionFactory<IOException::ExceptionCode>
        { typedef IOException Type; };


    ///////////////////////////////////////////////////////////////////////////////
    /// @brief Thrown when there is an unknown issue with a file.
    ///////////////////
    class MEZZ_LIB FileException : public IOException
    {
    public:
        /// @brief Thrown when there is an unknown issue with a file.
        static const Whole ExceptionCode = Exception::IO_FILE_EXCEPTION;

        /// @brief Class constructor.
        /// @param TypeName The name of this class.
        /// @param Message A basic description of the error.
        /// @param SrcFunction The name of the function from which this originated.
        /// @param SrcFile The name of the file from which this originated.
        /// @param FileLine The line on the named file from which this originated.
        FileException(const String& TypeName, const String& Message, const String& SrcFunction, const String& SrcFile, const Whole& FileLine)
            : IOException("FileException", Message, SrcFunction, SrcFile, FileLine)
            {}

        /// @brief Class constructor.
        /// @param Message A basic description of the error.
        /// @param SrcFunction The name of the function from which this originated.
        /// @param SrcFile The name of the file from which this originated.
        /// @param FileLine The line on the named file from which this originated.
        FileException(const String& Message, const String& SrcFunction, const String& SrcFile, const Whole& FileLine)
            : IOException("FileException", Message, SrcFunction, SrcFile, FileLine)
            {}

        /// @brief Class destructor.
        virtual ~FileException() throw() {}

        /// @copydoc Exception::GetExceptionCode()
        virtual Whole GetExceptionCode() const throw()
            { return FileException::ExceptionCode; }
    }; //FileException

    /// @brief A template metaprogramming construct used to ensure that all required FileException can be created at compile time if the compiler is clever.
    template<>
    struct MEZZ_LIB ExceptionFactory<FileException::ExceptionCode>
        { typedef FileException Type; };


    ///////////////////////////////////////////////////////////////////////////////
    /// @brief Thrown when a file could not be read and permissions seem to not be an issue (Filesystem/hardware issue?)
    ///////////////////
    class MEZZ_LIB FileReadException : public FileException
    {
    public:
        /// @brief Thrown when a file could not be read and permissions seem to not be an issue (Filesystem/hardware issue?)
        static const Whole ExceptionCode = Exception::IO_FILE_READ_EXCEPTION;

        /// @brief Class constructor.
        /// @param TypeName The name of this class.
        /// @param Message A basic description of the error.
        /// @param SrcFunction The name of the function from which this originated.
        /// @param SrcFile The name of the file from which this originated.
        /// @param FileLine The line on the named file from which this originated.
        FileReadException(const String& TypeName, const String& Message, const String& SrcFunction, const String& SrcFile, const Whole& FileLine)
            : FileException("FileReadException", Message, SrcFunction, SrcFile, FileLine)
            {}

        /// @brief Class constructor.
        /// @param Message A basic description of the error.
        /// @param SrcFunction The name of the function from which this originated.
        /// @param SrcFile The name of the file from which this originated.
        /// @param FileLine The line on the named file from which this originated.
        FileReadException(const String& Message, const String& SrcFunction, const String& SrcFile, const Whole& FileLine)
            : FileException("FileReadException", Message, SrcFunction, SrcFile, FileLine)
            {}

        /// @brief Class destructor.
        virtual ~FileReadException() throw() {}

        /// @copydoc Exception::GetExceptionCode()
        virtual Whole GetExceptionCode() const throw()
            { return FileReadException::ExceptionCode; }
    }; //FileReadException

    /// @brief A template metaprogramming construct used to ensure that all required FileReadException can be created at compile time if the compiler is clever.
    template<>
    struct MEZZ_LIB ExceptionFactory<FileReadException::ExceptionCode>
        { typedef FileReadException Type; };


    ///////////////////////////////////////////////////////////////////////////////
    /// @brief Thrown when a file could not be written and perimssions seem not to be an issue.
    ///////////////////
    class MEZZ_LIB FileWriteException : public FileException
    {
    public:
        /// @brief Thrown when a file could not be written and perimssions seem not to be an issue.
        static const Whole ExceptionCode = Exception::IO_FILE_WRITE_EXCEPTION;

        /// @brief Class constructor.
        /// @param TypeName The name of this class.
        /// @param Message A basic description of the error.
        /// @param SrcFunction The name of the function from which this originated.
        /// @param SrcFile The name of the file from which this originated.
        /// @param FileLine The line on the named file from which this originated.
        FileWriteException(const String& TypeName, const String& Message, const String& SrcFunction, const String& SrcFile, const Whole& FileLine)
            : FileException("FileWriteException", Message, SrcFunction, SrcFile, FileLine)
            {}

        /// @brief Class constructor.
        /// @param Message A basic description of the error.
        /// @param SrcFunction The name of the function from which this originated.
        /// @param SrcFile The name of the file from which this originated.
        /// @param FileLine The line on the named file from which this originated.
        FileWriteException(const String& Message, const String& SrcFunction, const String& SrcFile, const Whole& FileLine)
            : FileException("FileWriteException", Message, SrcFunction, SrcFile, FileLine)
            {}

        /// @brief Class destructor.
        virtual ~FileWriteException() throw() {}

        /// @copydoc Exception::GetExceptionCode()
        virtual Whole GetExceptionCode() const throw()
            { return FileWriteException::ExceptionCode; }
    }; //FileWriteException

    /// @brief A template metaprogramming construct used to ensure that all required FileWriteException can be created at compile time if the compiler is clever.
    template<>
    struct MEZZ_LIB ExceptionFactory<FileWriteException::ExceptionCode>
        { typedef FileWriteException Type; };


    ///////////////////////////////////////////////////////////////////////////////
    /// @brief Thrown when a file was expected to be there, but was not.
    ///////////////////
    class MEZZ_LIB FileNotFoundException : public FileException
    {
    public:
        /// @brief Thrown when a file was expected to be there, but was not.
        static const Whole ExceptionCode = Exception::IO_FILE_NOT_FOUND_EXCEPTION;

        /// @brief Class constructor.
        /// @param TypeName The name of this class.
        /// @param Message A basic description of the error.
        /// @param SrcFunction The name of the function from which this originated.
        /// @param SrcFile The name of the file from which this originated.
        /// @param FileLine The line on the named file from which this originated.
        FileNotFoundException(const String& TypeName, const String& Message, const String& SrcFunction, const String& SrcFile, const Whole& FileLine)
            : FileException("FileNotFoundException", Message, SrcFunction, SrcFile, FileLine)
            {}

        /// @brief Class constructor.
        /// @param Message A basic description of the error.
        /// @param SrcFunction The name of the function from which this originated.
        /// @param SrcFile The name of the file from which this originated.
        /// @param FileLine The line on the named file from which this originated.
        FileNotFoundException(const String& Message, const String& SrcFunction, const String& SrcFile, const Whole& FileLine)
            : FileException("FileNotFoundException", Message, SrcFunction, SrcFile, FileLine)
            {}

        /// @brief Class destructor.
        virtual ~FileNotFoundException() throw() {}

        /// @copydoc Exception::GetExceptionCode()
        virtual Whole GetExceptionCode() const throw()
            { return FileNotFoundException::ExceptionCode; }
    }; //FileNotFoundException

    /// @brief A template metaprogramming construct used to ensure that all required FileNotFoundException can be created at compile time if the compiler is clever.
    template<>
    struct MEZZ_LIB ExceptionFactory<FileNotFoundException::ExceptionCode>
        { typedef FileNotFoundException Type; };


    ///////////////////////////////////////////////////////////////////////////////
    /// @brief Thrown when permission was denied to a file.
    ///////////////////
    class MEZZ_LIB FilePermissionException : public FileException
    {
    public:
        /// @brief Thrown when permission was denied to a file.
        static const Whole ExceptionCode = Exception::IO_FILE_PERMISSION_EXCEPTION;

        /// @brief Class constructor.
        /// @param TypeName The name of this class.
        /// @param Message A basic description of the error.
        /// @param SrcFunction The name of the function from which this originated.
        /// @param SrcFile The name of the file from which this originated.
        /// @param FileLine The line on the named file from which this originated.
        FilePermissionException(const String& TypeName, const String& Message, const String& SrcFunction, const String& SrcFile, const Whole& FileLine)
            : FileException("FilePermissionException", Message, SrcFunction, SrcFile, FileLine)
            {}

        /// @brief Class constructor.
        /// @param Message A basic description of the error.
        /// @param SrcFunction The name of the function from which this originated.
        /// @param SrcFile The name of the file from which this originated.
        /// @param FileLine The line on the named file from which this originated.
        FilePermissionException(const String& Message, const String& SrcFunction, const String& SrcFile, const Whole& FileLine)
            : FileException("FilePermissionException", Message, SrcFunction, SrcFile, FileLine)
            {}

        /// @brief Class destructor.
        virtual ~FilePermissionException() throw() {}

        /// @copydoc Exception::GetExceptionCode()
        virtual Whole GetExceptionCode() const throw()
            { return FilePermissionException::ExceptionCode; }
    }; //FilePermissionException

    /// @brief A template metaprogramming construct used to ensure that all required FilePermissionException can be created at compile time if the compiler is clever.
    template<>
    struct MEZZ_LIB ExceptionFactory<FilePermissionException::ExceptionCode>
        { typedef FilePermissionException Type; };


    ///////////////////////////////////////////////////////////////////////////////
    /// @brief Thrown when there is an unknown issue with a file.
    ///////////////////
    class MEZZ_LIB DirectoryException : public IOException
    {
    public:
        /// @brief Thrown when there is an unknown issue with a file.
        static const Whole ExceptionCode = Exception::IO_DIRECTORY_EXCEPTION;

        /// @brief Class constructor.
        /// @param TypeName The name of this class.
        /// @param Message A basic description of the error.
        /// @param SrcFunction The name of the function from which this originated.
        /// @param SrcFile The name of the file from which this originated.
        /// @param FileLine The line on the named file from which this originated.
        DirectoryException(const String& TypeName, const String& Message, const String& SrcFunction, const String& SrcFile, const Whole& FileLine)
            : IOException("DirectoryException", Message, SrcFunction, SrcFile, FileLine)
            {}

        /// @brief Class constructor.
        /// @param Message A basic description of the error.
        /// @param SrcFunction The name of the function from which this originated.
        /// @param SrcFile The name of the file from which this originated.
        /// @param FileLine The line on the named file from which this originated.
        DirectoryException(const String& Message, const String& SrcFunction, const String& SrcFile, const Whole& FileLine)
            : IOException("DirectoryException", Message, SrcFunction, SrcFile, FileLine)
            {}

        /// @brief Class destructor.
        virtual ~DirectoryException() throw() {}

        /// @copydoc Exception::GetExceptionCode()
        virtual Whole GetExceptionCode() const throw()
            { return DirectoryException::ExceptionCode; }
    }; //DirectoryException

    /// @brief A template metaprogramming construct used to ensure that all required DirectoryException can be created at compile time if the compiler is clever.
    template<>
    struct MEZZ_LIB ExceptionFactory<DirectoryException::ExceptionCode>
        { typedef DirectoryException Type; };


    ///////////////////////////////////////////////////////////////////////////////
    /// @brief Thrown when a directory could be read and it wasn't a permission issue.
    ///////////////////
    class MEZZ_LIB DirectoryReadException : public DirectoryException
    {
    public:
        /// @brief Thrown when a directory could be read and it wasn't a permission issue.
        static const Whole ExceptionCode = Exception::IO_DIRECTORY_READ_EXCEPTION;

        /// @brief Class constructor.
        /// @param TypeName The name of this class.
        /// @param Message A basic description of the error.
        /// @param SrcFunction The name of the function from which this originated.
        /// @param SrcFile The name of the file from which this originated.
        /// @param FileLine The line on the named file from which this originated.
        DirectoryReadException(const String& TypeName, const String& Message, const String& SrcFunction, const String& SrcFile, const Whole& FileLine)
            : DirectoryException("DirectoryReadException", Message, SrcFunction, SrcFile, FileLine)
            {}

        /// @brief Class constructor.
        /// @param Message A basic description of the error.
        /// @param SrcFunction The name of the function from which this originated.
        /// @param SrcFile The name of the file from which this originated.
        /// @param FileLine The line on the named file from which this originated.
        DirectoryReadException(const String& Message, const String& SrcFunction, const String& SrcFile, const Whole& FileLine)
            : DirectoryException("DirectoryReadException", Message, SrcFunction, SrcFile, FileLine)
            {}

        /// @brief Class destructor.
        virtual ~DirectoryReadException() throw() {}

        /// @copydoc Exception::GetExceptionCode()
        virtual Whole GetExceptionCode() const throw()
            { return DirectoryReadException::ExceptionCode; }
    }; //DirectoryReadException

    /// @brief A template metaprogramming construct used to ensure that all required DirectoryReadException can be created at compile time if the compiler is clever.
    template<>
    struct MEZZ_LIB ExceptionFactory<DirectoryReadException::ExceptionCode>
        { typedef DirectoryReadException Type; };


    ///////////////////////////////////////////////////////////////////////////////
    /// @brief Thrown when a directory could be written to and it wasn't a permission issue.
    ///////////////////
    class MEZZ_LIB DirectoryWriteException : public DirectoryException
    {
    public:
        /// @brief Thrown when a directory could be written to and it wasn't a permission issue.
        static const Whole ExceptionCode = Exception::IO_DIRECTORY_WRITE_EXCEPTION;

        /// @brief Class constructor.
        /// @param TypeName The name of this class.
        /// @param Message A basic description of the error.
        /// @param SrcFunction The name of the function from which this originated.
        /// @param SrcFile The name of the file from which this originated.
        /// @param FileLine The line on the named file from which this originated.
        DirectoryWriteException(const String& TypeName, const String& Message, const String& SrcFunction, const String& SrcFile, const Whole& FileLine)
            : DirectoryException("DirectoryWriteException", Message, SrcFunction, SrcFile, FileLine)
            {}

        /// @brief Class constructor.
        /// @param Message A basic description of the error.
        /// @param SrcFunction The name of the function from which this originated.
        /// @param SrcFile The name of the file from which this originated.
        /// @param FileLine The line on the named file from which this originated.
        DirectoryWriteException(const String& Message, const String& SrcFunction, const String& SrcFile, const Whole& FileLine)
            : DirectoryException("DirectoryWriteException", Message, SrcFunction, SrcFile, FileLine)
            {}

        /// @brief Class destructor.
        virtual ~DirectoryWriteException() throw() {}

        /// @copydoc Exception::GetExceptionCode()
        virtual Whole GetExceptionCode() const throw()
            { return DirectoryWriteException::ExceptionCode; }
    }; //DirectoryWriteException

    /// @brief A template metaprogramming construct used to ensure that all required DirectoryWriteException can be created at compile time if the compiler is clever.
    template<>
    struct MEZZ_LIB ExceptionFactory<DirectoryWriteException::ExceptionCode>
        { typedef DirectoryWriteException Type; };


    ///////////////////////////////////////////////////////////////////////////////
    /// @brief Thrown when a directory was expected to be there, but was not.
    ///////////////////
    class MEZZ_LIB DirectoryNotFoundException : public DirectoryException
    {
    public:
        /// @brief Thrown when a directory was expected to be there, but was not.
        static const Whole ExceptionCode = Exception::IO_DIRECTORY_NOT_FOUND_EXCEPTION;

        /// @brief Class constructor.
        /// @param TypeName The name of this class.
        /// @param Message A basic description of the error.
        /// @param SrcFunction The name of the function from which this originated.
        /// @param SrcFile The name of the file from which this originated.
        /// @param FileLine The line on the named file from which this originated.
        DirectoryNotFoundException(const String& TypeName, const String& Message, const String& SrcFunction, const String& SrcFile, const Whole& FileLine)
            : DirectoryException("DirectoryNotFoundException", Message, SrcFunction, SrcFile, FileLine)
            {}

        /// @brief Class constructor.
        /// @param Message A basic description of the error.
        /// @param SrcFunction The name of the function from which this originated.
        /// @param SrcFile The name of the file from which this originated.
        /// @param FileLine The line on the named file from which this originated.
        DirectoryNotFoundException(const String& Message, const String& SrcFunction, const String& SrcFile, const Whole& FileLine)
            : DirectoryException("DirectoryNotFoundException", Message, SrcFunction, SrcFile, FileLine)
            {}

        /// @brief Class destructor.
        virtual ~DirectoryNotFoundException() throw() {}

        /// @copydoc Exception::GetExceptionCode()
        virtual Whole GetExceptionCode() const throw()
            { return DirectoryNotFoundException::ExceptionCode; }
    }; //DirectoryNotFoundException

    /// @brief A template metaprogramming construct used to ensure that all required DirectoryNotFoundException can be created at compile time if the compiler is clever.
    template<>
    struct MEZZ_LIB ExceptionFactory<DirectoryNotFoundException::ExceptionCode>
        { typedef DirectoryNotFoundException Type; };


    ///////////////////////////////////////////////////////////////////////////////
    /// @brief Thrown when permission is denied to a directory.
    ///////////////////
    class MEZZ_LIB DirectoryPermissionException : public DirectoryException
    {
    public:
        /// @brief Thrown when permission is denied to a directory.
        static const Whole ExceptionCode = Exception::IO_DIRECTORY_PERMISSION_EXCEPTION;

        /// @brief Class constructor.
        /// @param TypeName The name of this class.
        /// @param Message A basic description of the error.
        /// @param SrcFunction The name of the function from which this originated.
        /// @param SrcFile The name of the file from which this originated.
        /// @param FileLine The line on the named file from which this originated.
        DirectoryPermissionException(const String& TypeName, const String& Message, const String& SrcFunction, const String& SrcFile, const Whole& FileLine)
            : DirectoryException("DirectoryPermissionException", Message, SrcFunction, SrcFile, FileLine)
            {}

        /// @brief Class constructor.
        /// @param Message A basic description of the error.
        /// @param SrcFunction The name of the function from which this originated.
        /// @param SrcFile The name of the file from which this originated.
        /// @param FileLine The line on the named file from which this originated.
        DirectoryPermissionException(const String& Message, const String& SrcFunction, const String& SrcFile, const Whole& FileLine)
            : DirectoryException("DirectoryPermissionException", Message, SrcFunction, SrcFile, FileLine)
            {}

        /// @brief Class destructor.
        virtual ~DirectoryPermissionException() throw() {}

        /// @copydoc Exception::GetExceptionCode()
        virtual Whole GetExceptionCode() const throw()
            { return DirectoryPermissionException::ExceptionCode; }
    }; //DirectoryPermissionException

    /// @brief A template metaprogramming construct used to ensure that all required DirectoryPermissionException can be created at compile time if the compiler is clever.
    template<>
    struct MEZZ_LIB ExceptionFactory<DirectoryPermissionException::ExceptionCode>
        { typedef DirectoryPermissionException Type; };


    ///////////////////////////////////////////////////////////////////////////////
    /// @brief Thrown when something unknown causes network IO to fail.
    ///////////////////
    class MEZZ_LIB NetworkException : public IOException
    {
    public:
        /// @brief Thrown when something unknown causes network IO to fail.
        static const Whole ExceptionCode = Exception::IO_NETWORK_EXCEPTION;

        /// @brief Class constructor.
        /// @param TypeName The name of this class.
        /// @param Message A basic description of the error.
        /// @param SrcFunction The name of the function from which this originated.
        /// @param SrcFile The name of the file from which this originated.
        /// @param FileLine The line on the named file from which this originated.
        NetworkException(const String& TypeName, const String& Message, const String& SrcFunction, const String& SrcFile, const Whole& FileLine)
            : IOException("NetworkException", Message, SrcFunction, SrcFile, FileLine)
            {}

        /// @brief Class constructor.
        /// @param Message A basic description of the error.
        /// @param SrcFunction The name of the function from which this originated.
        /// @param SrcFile The name of the file from which this originated.
        /// @param FileLine The line on the named file from which this originated.
        NetworkException(const String& Message, const String& SrcFunction, const String& SrcFile, const Whole& FileLine)
            : IOException("NetworkException", Message, SrcFunction, SrcFile, FileLine)
            {}

        /// @brief Class destructor.
        virtual ~NetworkException() throw() {}

        /// @copydoc Exception::GetExceptionCode()
        virtual Whole GetExceptionCode() const throw()
            { return NetworkException::ExceptionCode; }
    }; //NetworkException

    /// @brief A template metaprogramming construct used to ensure that all required NetworkException can be created at compile time if the compiler is clever.
    template<>
    struct MEZZ_LIB ExceptionFactory<NetworkException::ExceptionCode>
        { typedef NetworkException Type; };


    ///////////////////////////////////////////////////////////////////////////////
    /// @brief Thrown when data could not be read from the network (downloads).
    ///////////////////
    class MEZZ_LIB NetworkReadException : public NetworkException
    {
    public:
        /// @brief Thrown when data could not be read from the network (downloads).
        static const Whole ExceptionCode = Exception::IO_NETWORK_READ_EXCEPTION;

        /// @brief Class constructor.
        /// @param TypeName The name of this class.
        /// @param Message A basic description of the error.
        /// @param SrcFunction The name of the function from which this originated.
        /// @param SrcFile The name of the file from which this originated.
        /// @param FileLine The line on the named file from which this originated.
        NetworkReadException(const String& TypeName, const String& Message, const String& SrcFunction, const String& SrcFile, const Whole& FileLine)
            : NetworkException("NetworkReadException", Message, SrcFunction, SrcFile, FileLine)
            {}

        /// @brief Class constructor.
        /// @param Message A basic description of the error.
        /// @param SrcFunction The name of the function from which this originated.
        /// @param SrcFile The name of the file from which this originated.
        /// @param FileLine The line on the named file from which this originated.
        NetworkReadException(const String& Message, const String& SrcFunction, const String& SrcFile, const Whole& FileLine)
            : NetworkException("NetworkReadException", Message, SrcFunction, SrcFile, FileLine)
            {}

        /// @brief Class destructor.
        virtual ~NetworkReadException() throw() {}

        /// @copydoc Exception::GetExceptionCode()
        virtual Whole GetExceptionCode() const throw()
            { return NetworkReadException::ExceptionCode; }
    }; //NetworkReadException

    /// @brief A template metaprogramming construct used to ensure that all required NetworkReadException can be created at compile time if the compiler is clever.
    template<>
    struct MEZZ_LIB ExceptionFactory<NetworkReadException::ExceptionCode>
        { typedef NetworkReadException Type; };


    ///////////////////////////////////////////////////////////////////////////////
    /// @brief Thrown when data could not be read from the network (iloads).
    ///////////////////
    class MEZZ_LIB NetworkWriteException : public NetworkException
    {
    public:
        /// @brief Thrown when data could not be read from the network (iloads).
        static const Whole ExceptionCode = Exception::IO_NETWORK_WRITE_EXCEPTION;

        /// @brief Class constructor.
        /// @param TypeName The name of this class.
        /// @param Message A basic description of the error.
        /// @param SrcFunction The name of the function from which this originated.
        /// @param SrcFile The name of the file from which this originated.
        /// @param FileLine The line on the named file from which this originated.
        NetworkWriteException(const String& TypeName, const String& Message, const String& SrcFunction, const String& SrcFile, const Whole& FileLine)
            : NetworkException("NetworkWriteException", Message, SrcFunction, SrcFile, FileLine)
            {}

        /// @brief Class constructor.
        /// @param Message A basic description of the error.
        /// @param SrcFunction The name of the function from which this originated.
        /// @param SrcFile The name of the file from which this originated.
        /// @param FileLine The line on the named file from which this originated.
        NetworkWriteException(const String& Message, const String& SrcFunction, const String& SrcFile, const Whole& FileLine)
            : NetworkException("NetworkWriteException", Message, SrcFunction, SrcFile, FileLine)
            {}

        /// @brief Class destructor.
        virtual ~NetworkWriteException() throw() {}

        /// @copydoc Exception::GetExceptionCode()
        virtual Whole GetExceptionCode() const throw()
            { return NetworkWriteException::ExceptionCode; }
    }; //NetworkWriteException

    /// @brief A template metaprogramming construct used to ensure that all required NetworkWriteException can be created at compile time if the compiler is clever.
    template<>
    struct MEZZ_LIB ExceptionFactory<NetworkWriteException::ExceptionCode>
        { typedef NetworkWriteException Type; };


    ///////////////////////////////////////////////////////////////////////////////
    /// @brief Thrown when no network connection is available.
    ///////////////////
    class MEZZ_LIB NetworkNotFoundException : public NetworkException
    {
    public:
        /// @brief Thrown when no network connection is available.
        static const Whole ExceptionCode = Exception::IO_NETWORK_NOT_FOUND_EXCEPTION;

        /// @brief Class constructor.
        /// @param TypeName The name of this class.
        /// @param Message A basic description of the error.
        /// @param SrcFunction The name of the function from which this originated.
        /// @param SrcFile The name of the file from which this originated.
        /// @param FileLine The line on the named file from which this originated.
        NetworkNotFoundException(const String& TypeName, const String& Message, const String& SrcFunction, const String& SrcFile, const Whole& FileLine)
            : NetworkException("NetworkNotFoundException", Message, SrcFunction, SrcFile, FileLine)
            {}

        /// @brief Class constructor.
        /// @param Message A basic description of the error.
        /// @param SrcFunction The name of the function from which this originated.
        /// @param SrcFile The name of the file from which this originated.
        /// @param FileLine The line on the named file from which this originated.
        NetworkNotFoundException(const String& Message, const String& SrcFunction, const String& SrcFile, const Whole& FileLine)
            : NetworkException("NetworkNotFoundException", Message, SrcFunction, SrcFile, FileLine)
            {}

        /// @brief Class destructor.
        virtual ~NetworkNotFoundException() throw() {}

        /// @copydoc Exception::GetExceptionCode()
        virtual Whole GetExceptionCode() const throw()
            { return NetworkNotFoundException::ExceptionCode; }
    }; //NetworkNotFoundException

    /// @brief A template metaprogramming construct used to ensure that all required NetworkNotFoundException can be created at compile time if the compiler is clever.
    template<>
    struct MEZZ_LIB ExceptionFactory<NetworkNotFoundException::ExceptionCode>
        { typedef NetworkNotFoundException Type; };


    ///////////////////////////////////////////////////////////////////////////////
    /// @brief Thrown when an address is invalid or could not be found.
    ///////////////////
    class MEZZ_LIB NetworkURLException : public NetworkException
    {
    public:
        /// @brief Thrown when an address is invalid or could not be found.
        static const Whole ExceptionCode = Exception::IO_NETWORK_URL_EXCEPTION;

        /// @brief Class constructor.
        /// @param TypeName The name of this class.
        /// @param Message A basic description of the error.
        /// @param SrcFunction The name of the function from which this originated.
        /// @param SrcFile The name of the file from which this originated.
        /// @param FileLine The line on the named file from which this originated.
        NetworkURLException(const String& TypeName, const String& Message, const String& SrcFunction, const String& SrcFile, const Whole& FileLine)
            : NetworkException("NetworkURLException", Message, SrcFunction, SrcFile, FileLine)
            {}

        /// @brief Class constructor.
        /// @param Message A basic description of the error.
        /// @param SrcFunction The name of the function from which this originated.
        /// @param SrcFile The name of the file from which this originated.
        /// @param FileLine The line on the named file from which this originated.
        NetworkURLException(const String& Message, const String& SrcFunction, const String& SrcFile, const Whole& FileLine)
            : NetworkException("NetworkURLException", Message, SrcFunction, SrcFile, FileLine)
            {}

        /// @brief Class destructor.
        virtual ~NetworkURLException() throw() {}

        /// @copydoc Exception::GetExceptionCode()
        virtual Whole GetExceptionCode() const throw()
            { return NetworkURLException::ExceptionCode; }
    }; //NetworkURLException

    /// @brief A template metaprogramming construct used to ensure that all required NetworkURLException can be created at compile time if the compiler is clever.
    template<>
    struct MEZZ_LIB ExceptionFactory<NetworkURLException::ExceptionCode>
        { typedef NetworkURLException Type; };


    ///////////////////////////////////////////////////////////////////////////////
    /// @brief Thrown when permision was denied to a network interface or network resource.
    ///////////////////
    class MEZZ_LIB NetworkPermissionException : public NetworkException
    {
    public:
        /// @brief Thrown when permision was denied to a network interface or network resource.
        static const Whole ExceptionCode = Exception::IO_NETWORK_PERMISSION_EXCEPTION;

        /// @brief Class constructor.
        /// @param TypeName The name of this class.
        /// @param Message A basic description of the error.
        /// @param SrcFunction The name of the function from which this originated.
        /// @param SrcFile The name of the file from which this originated.
        /// @param FileLine The line on the named file from which this originated.
        NetworkPermissionException(const String& TypeName, const String& Message, const String& SrcFunction, const String& SrcFile, const Whole& FileLine)
            : NetworkException("NetworkPermissionException", Message, SrcFunction, SrcFile, FileLine)
            {}

        /// @brief Class constructor.
        /// @param Message A basic description of the error.
        /// @param SrcFunction The name of the function from which this originated.
        /// @param SrcFile The name of the file from which this originated.
        /// @param FileLine The line on the named file from which this originated.
        NetworkPermissionException(const String& Message, const String& SrcFunction, const String& SrcFile, const Whole& FileLine)
            : NetworkException("NetworkPermissionException", Message, SrcFunction, SrcFile, FileLine)
            {}

        /// @brief Class destructor.
        virtual ~NetworkPermissionException() throw() {}

        /// @copydoc Exception::GetExceptionCode()
        virtual Whole GetExceptionCode() const throw()
            { return NetworkPermissionException::ExceptionCode; }
    }; //NetworkPermissionException

    /// @brief A template metaprogramming construct used to ensure that all required NetworkPermissionException can be created at compile time if the compiler is clever.
    template<>
    struct MEZZ_LIB ExceptionFactory<NetworkPermissionException::ExceptionCode>
        { typedef NetworkPermissionException Type; };


    ///////////////////////////////////////////////////////////////////////////////
    /// @brief Thrown when a write is happening but something has prevented the underlying code from knowing what was writing.
    ///////////////////
    class MEZZ_LIB IOWriteException : public IOException
    {
    public:
        /// @brief Thrown when a write is happening but something has prevented the underlying code from knowing what was writing.
        static const Whole ExceptionCode = Exception::IO_WRITE_EXCEPTION;

        /// @brief Class constructor.
        /// @param TypeName The name of this class.
        /// @param Message A basic description of the error.
        /// @param SrcFunction The name of the function from which this originated.
        /// @param SrcFile The name of the file from which this originated.
        /// @param FileLine The line on the named file from which this originated.
        IOWriteException(const String& TypeName, const String& Message, const String& SrcFunction, const String& SrcFile, const Whole& FileLine)
            : IOException("IOWriteException", Message, SrcFunction, SrcFile, FileLine)
            {}

        /// @brief Class constructor.
        /// @param Message A basic description of the error.
        /// @param SrcFunction The name of the function from which this originated.
        /// @param SrcFile The name of the file from which this originated.
        /// @param FileLine The line on the named file from which this originated.
        IOWriteException(const String& Message, const String& SrcFunction, const String& SrcFile, const Whole& FileLine)
            : IOException("IOWriteException", Message, SrcFunction, SrcFile, FileLine)
            {}

        /// @brief Class destructor.
        virtual ~IOWriteException() throw() {}

        /// @copydoc Exception::GetExceptionCode()
        virtual Whole GetExceptionCode() const throw()
            { return IOWriteException::ExceptionCode; }
    }; //IOWriteException

    /// @brief A template metaprogramming construct used to ensure that all required IOWriteException can be created at compile time if the compiler is clever.
    template<>
    struct MEZZ_LIB ExceptionFactory<IOWriteException::ExceptionCode>
        { typedef IOWriteException Type; };


    ///////////////////////////////////////////////////////////////////////////////
    /// @brief Thrown when a read is happening but something has prevented the underlying code from knowing what was reading.
    ///////////////////
    class MEZZ_LIB IOReadException : public IOException
    {
    public:
        /// @brief Thrown when a read is happening but something has prevented the underlying code from knowing what was reading.
        static const Whole ExceptionCode = Exception::IO_READ_EXCEPTION;

        /// @brief Class constructor.
        /// @param TypeName The name of this class.
        /// @param Message A basic description of the error.
        /// @param SrcFunction The name of the function from which this originated.
        /// @param SrcFile The name of the file from which this originated.
        /// @param FileLine The line on the named file from which this originated.
        IOReadException(const String& TypeName, const String& Message, const String& SrcFunction, const String& SrcFile, const Whole& FileLine)
            : IOException("IOReadException", Message, SrcFunction, SrcFile, FileLine)
            {}

        /// @brief Class constructor.
        /// @param Message A basic description of the error.
        /// @param SrcFunction The name of the function from which this originated.
        /// @param SrcFile The name of the file from which this originated.
        /// @param FileLine The line on the named file from which this originated.
        IOReadException(const String& Message, const String& SrcFunction, const String& SrcFile, const Whole& FileLine)
            : IOException("IOReadException", Message, SrcFunction, SrcFile, FileLine)
            {}

        /// @brief Class destructor.
        virtual ~IOReadException() throw() {}

        /// @copydoc Exception::GetExceptionCode()
        virtual Whole GetExceptionCode() const throw()
            { return IOReadException::ExceptionCode; }
    }; //IOReadException

    /// @brief A template metaprogramming construct used to ensure that all required IOReadException can be created at compile time if the compiler is clever.
    template<>
    struct MEZZ_LIB ExceptionFactory<IOReadException::ExceptionCode>
        { typedef IOReadException Type; };


    ///////////////////////////////////////////////////////////////////////////////
    /// @brief Thrown when an unknown error with using an Identifier and it is invalid.
    ///////////////////
    class MEZZ_LIB InstanceIdentityException : public Exception
    {
    public:
        /// @brief Thrown when an unknown error with using an Identifier and it is invalid.
        static const Whole ExceptionCode = Exception::II_EXCEPTION;

        /// @brief Class constructor.
        /// @param TypeName The name of this class.
        /// @param Message A basic description of the error.
        /// @param SrcFunction The name of the function from which this originated.
        /// @param SrcFile The name of the file from which this originated.
        /// @param FileLine The line on the named file from which this originated.
        InstanceIdentityException(const String& TypeName, const String& Message, const String& SrcFunction, const String& SrcFile, const Whole& FileLine)
            : Exception("InstanceIdentityException", Message, SrcFunction, SrcFile, FileLine)
            {}

        /// @brief Class constructor.
        /// @param Message A basic description of the error.
        /// @param SrcFunction The name of the function from which this originated.
        /// @param SrcFile The name of the file from which this originated.
        /// @param FileLine The line on the named file from which this originated.
        InstanceIdentityException(const String& Message, const String& SrcFunction, const String& SrcFile, const Whole& FileLine)
            : Exception("InstanceIdentityException", Message, SrcFunction, SrcFile, FileLine)
            {}

        /// @brief Class destructor.
        virtual ~InstanceIdentityException() throw() {}

        /// @copydoc Exception::GetExceptionCode()
        virtual Whole GetExceptionCode() const throw()
            { return InstanceIdentityException::ExceptionCode; }
    }; //InstanceIdentityException

    /// @brief A template metaprogramming construct used to ensure that all required InstanceIdentityException can be created at compile time if the compiler is clever.
    template<>
    struct MEZZ_LIB ExceptionFactory<InstanceIdentityException::ExceptionCode>
        { typedef InstanceIdentityException Type; };


    ///////////////////////////////////////////////////////////////////////////////
    /// @brief Thrown when the identity string wasn't valid at all.
    ///////////////////
    class MEZZ_LIB InstanceIdentityInvalidException : public InstanceIdentityException
    {
    public:
        /// @brief Thrown when the identity string wasn't valid at all.
        static const Whole ExceptionCode = Exception::II_IDENTITY_INVALID_EXCEPTION;

        /// @brief Class constructor.
        /// @param TypeName The name of this class.
        /// @param Message A basic description of the error.
        /// @param SrcFunction The name of the function from which this originated.
        /// @param SrcFile The name of the file from which this originated.
        /// @param FileLine The line on the named file from which this originated.
        InstanceIdentityInvalidException(const String& TypeName, const String& Message, const String& SrcFunction, const String& SrcFile, const Whole& FileLine)
            : InstanceIdentityException("InstanceIdentityInvalidException", Message, SrcFunction, SrcFile, FileLine)
            {}

        /// @brief Class constructor.
        /// @param Message A basic description of the error.
        /// @param SrcFunction The name of the function from which this originated.
        /// @param SrcFile The name of the file from which this originated.
        /// @param FileLine The line on the named file from which this originated.
        InstanceIdentityInvalidException(const String& Message, const String& SrcFunction, const String& SrcFile, const Whole& FileLine)
            : InstanceIdentityException("InstanceIdentityInvalidException", Message, SrcFunction, SrcFile, FileLine)
            {}

        /// @brief Class destructor.
        virtual ~InstanceIdentityInvalidException() throw() {}

        /// @copydoc Exception::GetExceptionCode()
        virtual Whole GetExceptionCode() const throw()
            { return InstanceIdentityInvalidException::ExceptionCode; }
    }; //InstanceIdentityInvalidException

    /// @brief A template metaprogramming construct used to ensure that all required InstanceIdentityInvalidException can be created at compile time if the compiler is clever.
    template<>
    struct MEZZ_LIB ExceptionFactory<InstanceIdentityInvalidException::ExceptionCode>
        { typedef InstanceIdentityInvalidException Type; };


    ///////////////////////////////////////////////////////////////////////////////
    /// @brief Thrown when the requested identity could not be found.
    ///////////////////
    class MEZZ_LIB InstanceIdentityNotFoundException : public InstanceIdentityException
    {
    public:
        /// @brief Thrown when the requested identity could not be found.
        static const Whole ExceptionCode = Exception::II_IDENTITY_NOT_FOUND_EXCEPTION;

        /// @brief Class constructor.
        /// @param TypeName The name of this class.
        /// @param Message A basic description of the error.
        /// @param SrcFunction The name of the function from which this originated.
        /// @param SrcFile The name of the file from which this originated.
        /// @param FileLine The line on the named file from which this originated.
        InstanceIdentityNotFoundException(const String& TypeName, const String& Message, const String& SrcFunction, const String& SrcFile, const Whole& FileLine)
            : InstanceIdentityException("InstanceIdentityNotFoundException", Message, SrcFunction, SrcFile, FileLine)
            {}

        /// @brief Class constructor.
        /// @param Message A basic description of the error.
        /// @param SrcFunction The name of the function from which this originated.
        /// @param SrcFile The name of the file from which this originated.
        /// @param FileLine The line on the named file from which this originated.
        InstanceIdentityNotFoundException(const String& Message, const String& SrcFunction, const String& SrcFile, const Whole& FileLine)
            : InstanceIdentityException("InstanceIdentityNotFoundException", Message, SrcFunction, SrcFile, FileLine)
            {}

        /// @brief Class destructor.
        virtual ~InstanceIdentityNotFoundException() throw() {}

        /// @copydoc Exception::GetExceptionCode()
        virtual Whole GetExceptionCode() const throw()
            { return InstanceIdentityNotFoundException::ExceptionCode; }
    }; //InstanceIdentityNotFoundException

    /// @brief A template metaprogramming construct used to ensure that all required InstanceIdentityNotFoundException can be created at compile time if the compiler is clever.
    template<>
    struct MEZZ_LIB ExceptionFactory<InstanceIdentityNotFoundException::ExceptionCode>
        { typedef InstanceIdentityNotFoundException Type; };


    ///////////////////////////////////////////////////////////////////////////////
    /// @brief Thrown when duplicates of teh same identity string exist.
    ///////////////////
    class MEZZ_LIB InstanceIdentityDuplicateException : public InstanceIdentityException
    {
    public:
        /// @brief Thrown when duplicates of teh same identity string exist.
        static const Whole ExceptionCode = Exception::II_DUPLICATE_IDENTITY_EXCEPTION;

        /// @brief Class constructor.
        /// @param TypeName The name of this class.
        /// @param Message A basic description of the error.
        /// @param SrcFunction The name of the function from which this originated.
        /// @param SrcFile The name of the file from which this originated.
        /// @param FileLine The line on the named file from which this originated.
        InstanceIdentityDuplicateException(const String& TypeName, const String& Message, const String& SrcFunction, const String& SrcFile, const Whole& FileLine)
            : InstanceIdentityException("InstanceIdentityDuplicateException", Message, SrcFunction, SrcFile, FileLine)
            {}

        /// @brief Class constructor.
        /// @param Message A basic description of the error.
        /// @param SrcFunction The name of the function from which this originated.
        /// @param SrcFile The name of the file from which this originated.
        /// @param FileLine The line on the named file from which this originated.
        InstanceIdentityDuplicateException(const String& Message, const String& SrcFunction, const String& SrcFile, const Whole& FileLine)
            : InstanceIdentityException("InstanceIdentityDuplicateException", Message, SrcFunction, SrcFile, FileLine)
            {}

        /// @brief Class destructor.
        virtual ~InstanceIdentityDuplicateException() throw() {}

        /// @copydoc Exception::GetExceptionCode()
        virtual Whole GetExceptionCode() const throw()
            { return InstanceIdentityDuplicateException::ExceptionCode; }
    }; //InstanceIdentityDuplicateException

    /// @brief A template metaprogramming construct used to ensure that all required InstanceIdentityDuplicateException can be created at compile time if the compiler is clever.
    template<>
    struct MEZZ_LIB ExceptionFactory<InstanceIdentityDuplicateException::ExceptionCode>
        { typedef InstanceIdentityDuplicateException Type; };


    ///////////////////////////////////////////////////////////////////////////////
    /// @brief Thrown when an unknown memory management exception occurs.
    ///////////////////
    class MEZZ_LIB MemoryManagementException : public Exception
    {
    public:
        /// @brief Thrown when an unknown memory management exception occurs.
        static const Whole ExceptionCode = Exception::MM_EXCEPTION;

        /// @brief Class constructor.
        /// @param TypeName The name of this class.
        /// @param Message A basic description of the error.
        /// @param SrcFunction The name of the function from which this originated.
        /// @param SrcFile The name of the file from which this originated.
        /// @param FileLine The line on the named file from which this originated.
        MemoryManagementException(const String& TypeName, const String& Message, const String& SrcFunction, const String& SrcFile, const Whole& FileLine)
            : Exception("MemoryManagementException", Message, SrcFunction, SrcFile, FileLine)
            {}

        /// @brief Class constructor.
        /// @param Message A basic description of the error.
        /// @param SrcFunction The name of the function from which this originated.
        /// @param SrcFile The name of the file from which this originated.
        /// @param FileLine The line on the named file from which this originated.
        MemoryManagementException(const String& Message, const String& SrcFunction, const String& SrcFile, const Whole& FileLine)
            : Exception("MemoryManagementException", Message, SrcFunction, SrcFile, FileLine)
            {}

        /// @brief Class destructor.
        virtual ~MemoryManagementException() throw() {}

        /// @copydoc Exception::GetExceptionCode()
        virtual Whole GetExceptionCode() const throw()
            { return MemoryManagementException::ExceptionCode; }
    }; //MemoryManagementException

    /// @brief A template metaprogramming construct used to ensure that all required MemoryManagementException can be created at compile time if the compiler is clever.
    template<>
    struct MEZZ_LIB ExceptionFactory<MemoryManagementException::ExceptionCode>
        { typedef MemoryManagementException Type; };


    ///////////////////////////////////////////////////////////////////////////////
    /// @brief Thrown when A memory allocation was attempted and failed.
    ///////////////////
    class MEZZ_LIB OutOfMemoryException : public MemoryManagementException
    {
    public:
        /// @brief Thrown when A memory allocation was attempted and failed.
        static const Whole ExceptionCode = Exception::MM_OUT_OF_MEMORY_EXCEPTION;

        /// @brief Class constructor.
        /// @param TypeName The name of this class.
        /// @param Message A basic description of the error.
        /// @param SrcFunction The name of the function from which this originated.
        /// @param SrcFile The name of the file from which this originated.
        /// @param FileLine The line on the named file from which this originated.
        OutOfMemoryException(const String& TypeName, const String& Message, const String& SrcFunction, const String& SrcFile, const Whole& FileLine)
            : MemoryManagementException("OutOfMemoryException", Message, SrcFunction, SrcFile, FileLine)
            {}

        /// @brief Class constructor.
        /// @param Message A basic description of the error.
        /// @param SrcFunction The name of the function from which this originated.
        /// @param SrcFile The name of the file from which this originated.
        /// @param FileLine The line on the named file from which this originated.
        OutOfMemoryException(const String& Message, const String& SrcFunction, const String& SrcFile, const Whole& FileLine)
            : MemoryManagementException("OutOfMemoryException", Message, SrcFunction, SrcFile, FileLine)
            {}

        /// @brief Class destructor.
        virtual ~OutOfMemoryException() throw() {}

        /// @copydoc Exception::GetExceptionCode()
        virtual Whole GetExceptionCode() const throw()
            { return OutOfMemoryException::ExceptionCode; }
    }; //OutOfMemoryException

    /// @brief A template metaprogramming construct used to ensure that all required OutOfMemoryException can be created at compile time if the compiler is clever.
    template<>
    struct MEZZ_LIB ExceptionFactory<OutOfMemoryException::ExceptionCode>
        { typedef OutOfMemoryException Type; };


    ///////////////////////////////////////////////////////////////////////////////
    /// @brief Thrown when attempted to access something that really should note be accessed.
    ///////////////////
    class MEZZ_LIB MemoryOutOfBoundsException : public MemoryManagementException
    {
    public:
        /// @brief Thrown when attempted to access something that really should note be accessed.
        static const Whole ExceptionCode = Exception::MM_OUT_OF_BOUNDS_EXCEPTION;

        /// @brief Class constructor.
        /// @param TypeName The name of this class.
        /// @param Message A basic description of the error.
        /// @param SrcFunction The name of the function from which this originated.
        /// @param SrcFile The name of the file from which this originated.
        /// @param FileLine The line on the named file from which this originated.
        MemoryOutOfBoundsException(const String& TypeName, const String& Message, const String& SrcFunction, const String& SrcFile, const Whole& FileLine)
            : MemoryManagementException("MemoryOutOfBoundsException", Message, SrcFunction, SrcFile, FileLine)
            {}

        /// @brief Class constructor.
        /// @param Message A basic description of the error.
        /// @param SrcFunction The name of the function from which this originated.
        /// @param SrcFile The name of the file from which this originated.
        /// @param FileLine The line on the named file from which this originated.
        MemoryOutOfBoundsException(const String& Message, const String& SrcFunction, const String& SrcFile, const Whole& FileLine)
            : MemoryManagementException("MemoryOutOfBoundsException", Message, SrcFunction, SrcFile, FileLine)
            {}

        /// @brief Class destructor.
        virtual ~MemoryOutOfBoundsException() throw() {}

        /// @copydoc Exception::GetExceptionCode()
        virtual Whole GetExceptionCode() const throw()
            { return MemoryOutOfBoundsException::ExceptionCode; }
    }; //MemoryOutOfBoundsException

    /// @brief A template metaprogramming construct used to ensure that all required MemoryOutOfBoundsException can be created at compile time if the compiler is clever.
    template<>
    struct MEZZ_LIB ExceptionFactory<MemoryOutOfBoundsException::ExceptionCode>
        { typedef MemoryOutOfBoundsException Type; };


    ///////////////////////////////////////////////////////////////////////////////
    /// @brief Thrown when some kind of syntax exception
    ///////////////////
    class MEZZ_LIB SyntaxErrorException : public Exception
    {
    public:
        /// @brief Thrown when some kind of syntax exception
        static const Whole ExceptionCode = Exception::SYNTAX_ERROR_EXCEPTION;

        /// @brief Class constructor.
        /// @param TypeName The name of this class.
        /// @param Message A basic description of the error.
        /// @param SrcFunction The name of the function from which this originated.
        /// @param SrcFile The name of the file from which this originated.
        /// @param FileLine The line on the named file from which this originated.
        SyntaxErrorException(const String& TypeName, const String& Message, const String& SrcFunction, const String& SrcFile, const Whole& FileLine)
            : Exception("SyntaxErrorException", Message, SrcFunction, SrcFile, FileLine)
            {}

        /// @brief Class constructor.
        /// @param Message A basic description of the error.
        /// @param SrcFunction The name of the function from which this originated.
        /// @param SrcFile The name of the file from which this originated.
        /// @param FileLine The line on the named file from which this originated.
        SyntaxErrorException(const String& Message, const String& SrcFunction, const String& SrcFile, const Whole& FileLine)
            : Exception("SyntaxErrorException", Message, SrcFunction, SrcFile, FileLine)
            {}

        /// @brief Class destructor.
        virtual ~SyntaxErrorException() throw() {}

        /// @copydoc Exception::GetExceptionCode()
        virtual Whole GetExceptionCode() const throw()
            { return SyntaxErrorException::ExceptionCode; }
    }; //SyntaxErrorException

    /// @brief A template metaprogramming construct used to ensure that all required SyntaxErrorException can be created at compile time if the compiler is clever.
    template<>
    struct MEZZ_LIB ExceptionFactory<SyntaxErrorException::ExceptionCode>
        { typedef SyntaxErrorException Type; };


    ///////////////////////////////////////////////////////////////////////////////
    /// @brief Thrown when and XML document is being parsed but is invalid
    ///////////////////
    class MEZZ_LIB SyntaxErrorXMLException : public SyntaxErrorException
    {
    public:
        /// @brief Thrown when and XML document is being parsed but is invalid
        static const Whole ExceptionCode = Exception::SYNTAX_ERROR_EXCEPTION_XML;

        /// @brief Class constructor.
        /// @param TypeName The name of this class.
        /// @param Message A basic description of the error.
        /// @param SrcFunction The name of the function from which this originated.
        /// @param SrcFile The name of the file from which this originated.
        /// @param FileLine The line on the named file from which this originated.
        SyntaxErrorXMLException(const String& TypeName, const String& Message, const String& SrcFunction, const String& SrcFile, const Whole& FileLine)
            : SyntaxErrorException("SyntaxErrorXMLException", Message, SrcFunction, SrcFile, FileLine)
            {}

        /// @brief Class constructor.
        /// @param Message A basic description of the error.
        /// @param SrcFunction The name of the function from which this originated.
        /// @param SrcFile The name of the file from which this originated.
        /// @param FileLine The line on the named file from which this originated.
        SyntaxErrorXMLException(const String& Message, const String& SrcFunction, const String& SrcFile, const Whole& FileLine)
            : SyntaxErrorException("SyntaxErrorXMLException", Message, SrcFunction, SrcFile, FileLine)
            {}

        /// @brief Class destructor.
        virtual ~SyntaxErrorXMLException() throw() {}

        /// @copydoc Exception::GetExceptionCode()
        virtual Whole GetExceptionCode() const throw()
            { return SyntaxErrorXMLException::ExceptionCode; }
    }; //SyntaxErrorXMLException

    /// @brief A template metaprogramming construct used to ensure that all required SyntaxErrorXMLException can be created at compile time if the compiler is clever.
    template<>
    struct MEZZ_LIB ExceptionFactory<SyntaxErrorXMLException::ExceptionCode>
        { typedef SyntaxErrorXMLException Type; };


    ///////////////////////////////////////////////////////////////////////////////
    /// @brief Thrown when an XPath query is being parsed but is invalid
    ///////////////////
    class MEZZ_LIB SyntaxErrorXPathException : public SyntaxErrorException
    {
    public:
        /// @brief Thrown when an XPath query is being parsed but is invalid
        static const Whole ExceptionCode = Exception::SYNTAX_ERROR_EXCEPTION_XPATH;

        /// @brief Class constructor.
        /// @param TypeName The name of this class.
        /// @param Message A basic description of the error.
        /// @param SrcFunction The name of the function from which this originated.
        /// @param SrcFile The name of the file from which this originated.
        /// @param FileLine The line on the named file from which this originated.
        SyntaxErrorXPathException(const String& TypeName, const String& Message, const String& SrcFunction, const String& SrcFile, const Whole& FileLine)
            : SyntaxErrorException("SyntaxErrorXPathException", Message, SrcFunction, SrcFile, FileLine)
            {}

        /// @brief Class constructor.
        /// @param Message A basic description of the error.
        /// @param SrcFunction The name of the function from which this originated.
        /// @param SrcFile The name of the file from which this originated.
        /// @param FileLine The line on the named file from which this originated.
        SyntaxErrorXPathException(const String& Message, const String& SrcFunction, const String& SrcFile, const Whole& FileLine)
            : SyntaxErrorException("SyntaxErrorXPathException", Message, SrcFunction, SrcFile, FileLine)
            {}

        /// @brief Class destructor.
        virtual ~SyntaxErrorXPathException() throw() {}

        /// @copydoc Exception::GetExceptionCode()
        virtual Whole GetExceptionCode() const throw()
            { return SyntaxErrorXPathException::ExceptionCode; }
    }; //SyntaxErrorXPathException

    /// @brief A template metaprogramming construct used to ensure that all required SyntaxErrorXPathException can be created at compile time if the compiler is clever.
    template<>
    struct MEZZ_LIB ExceptionFactory<SyntaxErrorXPathException::ExceptionCode>
        { typedef SyntaxErrorXPathException Type; };


    ///////////////////////////////////////////////////////////////////////////////
    /// @brief Thrown when lua code in incorrect.
    ///////////////////
    class MEZZ_LIB SyntaxErrorLuaException : public SyntaxErrorException
    {
    public:
        /// @brief Thrown when lua code in incorrect.
        static const Whole ExceptionCode = Exception::SYNTAX_ERROR_EXCEPTION_LUA;

        /// @brief Class constructor.
        /// @param TypeName The name of this class.
        /// @param Message A basic description of the error.
        /// @param SrcFunction The name of the function from which this originated.
        /// @param SrcFile The name of the file from which this originated.
        /// @param FileLine The line on the named file from which this originated.
        SyntaxErrorLuaException(const String& TypeName, const String& Message, const String& SrcFunction, const String& SrcFile, const Whole& FileLine)
            : SyntaxErrorException("SyntaxErrorLuaException", Message, SrcFunction, SrcFile, FileLine)
            {}

        /// @brief Class constructor.
        /// @param Message A basic description of the error.
        /// @param SrcFunction The name of the function from which this originated.
        /// @param SrcFile The name of the file from which this originated.
        /// @param FileLine The line on the named file from which this originated.
        SyntaxErrorLuaException(const String& Message, const String& SrcFunction, const String& SrcFile, const Whole& FileLine)
            : SyntaxErrorException("SyntaxErrorLuaException", Message, SrcFunction, SrcFile, FileLine)
            {}

        /// @brief Class destructor.
        virtual ~SyntaxErrorLuaException() throw() {}

        /// @copydoc Exception::GetExceptionCode()
        virtual Whole GetExceptionCode() const throw()
            { return SyntaxErrorLuaException::ExceptionCode; }
    }; //SyntaxErrorLuaException

    /// @brief A template metaprogramming construct used to ensure that all required SyntaxErrorLuaException can be created at compile time if the compiler is clever.
    template<>
    struct MEZZ_LIB ExceptionFactory<SyntaxErrorLuaException::ExceptionCode>
        { typedef SyntaxErrorLuaException Type; };


    ///////////////////////////////////////////////////////////////////////////////
    /// @brief Thrown when an unknown error happens with a script.
    ///////////////////
    class MEZZ_LIB ScriptException : public Exception
    {
    public:
        /// @brief Thrown when an unknown error happens with a script.
        static const Whole ExceptionCode = Exception::SCRIPT_EXCEPTION;

        /// @brief Class constructor.
        /// @param TypeName The name of this class.
        /// @param Message A basic description of the error.
        /// @param SrcFunction The name of the function from which this originated.
        /// @param SrcFile The name of the file from which this originated.
        /// @param FileLine The line on the named file from which this originated.
        ScriptException(const String& TypeName, const String& Message, const String& SrcFunction, const String& SrcFile, const Whole& FileLine)
            : Exception("ScriptException", Message, SrcFunction, SrcFile, FileLine)
            {}

        /// @brief Class constructor.
        /// @param Message A basic description of the error.
        /// @param SrcFunction The name of the function from which this originated.
        /// @param SrcFile The name of the file from which this originated.
        /// @param FileLine The line on the named file from which this originated.
        ScriptException(const String& Message, const String& SrcFunction, const String& SrcFile, const Whole& FileLine)
            : Exception("ScriptException", Message, SrcFunction, SrcFile, FileLine)
            {}

        /// @brief Class destructor.
        virtual ~ScriptException() throw() {}

        /// @copydoc Exception::GetExceptionCode()
        virtual Whole GetExceptionCode() const throw()
            { return ScriptException::ExceptionCode; }
    }; //ScriptException

    /// @brief A template metaprogramming construct used to ensure that all required ScriptException can be created at compile time if the compiler is clever.
    template<>
    struct MEZZ_LIB ExceptionFactory<ScriptException::ExceptionCode>
        { typedef ScriptException Type; };


    ///////////////////////////////////////////////////////////////////////////////
    /// @brief Thrown when an unknown error happens in a Lua script.
    ///////////////////
    class MEZZ_LIB ScriptLuaException : public ScriptException
    {
    public:
        /// @brief Thrown when an unknown error happens in a Lua script.
        static const Whole ExceptionCode = Exception::SCRIPT_EXCEPTION_LUA;

        /// @brief Class constructor.
        /// @param TypeName The name of this class.
        /// @param Message A basic description of the error.
        /// @param SrcFunction The name of the function from which this originated.
        /// @param SrcFile The name of the file from which this originated.
        /// @param FileLine The line on the named file from which this originated.
        ScriptLuaException(const String& TypeName, const String& Message, const String& SrcFunction, const String& SrcFile, const Whole& FileLine)
            : ScriptException("ScriptLuaException", Message, SrcFunction, SrcFile, FileLine)
            {}

        /// @brief Class constructor.
        /// @param Message A basic description of the error.
        /// @param SrcFunction The name of the function from which this originated.
        /// @param SrcFile The name of the file from which this originated.
        /// @param FileLine The line on the named file from which this originated.
        ScriptLuaException(const String& Message, const String& SrcFunction, const String& SrcFile, const Whole& FileLine)
            : ScriptException("ScriptLuaException", Message, SrcFunction, SrcFile, FileLine)
            {}

        /// @brief Class destructor.
        virtual ~ScriptLuaException() throw() {}

        /// @copydoc Exception::GetExceptionCode()
        virtual Whole GetExceptionCode() const throw()
            { return ScriptLuaException::ExceptionCode; }
    }; //ScriptLuaException

    /// @brief A template metaprogramming construct used to ensure that all required ScriptLuaException can be created at compile time if the compiler is clever.
    template<>
    struct MEZZ_LIB ExceptionFactory<ScriptLuaException::ExceptionCode>
        { typedef ScriptLuaException Type; };


    ///////////////////////////////////////////////////////////////////////////////
    /// @brief Thrown when Lua returns a yield and it should not have.
    ///////////////////
    class MEZZ_LIB ScriptLuaYieldException : public ScriptLuaException
    {
    public:
        /// @brief Thrown when Lua returns a yield and it should not have.
        static const Whole ExceptionCode = Exception::SCRIPT_EXCEPTION_LUA_YIELD;

        /// @brief Class constructor.
        /// @param TypeName The name of this class.
        /// @param Message A basic description of the error.
        /// @param SrcFunction The name of the function from which this originated.
        /// @param SrcFile The name of the file from which this originated.
        /// @param FileLine The line on the named file from which this originated.
        ScriptLuaYieldException(const String& TypeName, const String& Message, const String& SrcFunction, const String& SrcFile, const Whole& FileLine)
            : ScriptLuaException("ScriptLuaYieldException", Message, SrcFunction, SrcFile, FileLine)
            {}

        /// @brief Class constructor.
        /// @param Message A basic description of the error.
        /// @param SrcFunction The name of the function from which this originated.
        /// @param SrcFile The name of the file from which this originated.
        /// @param FileLine The line on the named file from which this originated.
        ScriptLuaYieldException(const String& Message, const String& SrcFunction, const String& SrcFile, const Whole& FileLine)
            : ScriptLuaException("ScriptLuaYieldException", Message, SrcFunction, SrcFile, FileLine)
            {}

        /// @brief Class destructor.
        virtual ~ScriptLuaYieldException() throw() {}

        /// @copydoc Exception::GetExceptionCode()
        virtual Whole GetExceptionCode() const throw()
            { return ScriptLuaYieldException::ExceptionCode; }
    }; //ScriptLuaYieldException

    /// @brief A template metaprogramming construct used to ensure that all required ScriptLuaYieldException can be created at compile time if the compiler is clever.
    template<>
    struct MEZZ_LIB ExceptionFactory<ScriptLuaYieldException::ExceptionCode>
        { typedef ScriptLuaYieldException Type; };


    ///////////////////////////////////////////////////////////////////////////////
    /// @brief Thrown when a Lua script has a runtime error.
    ///////////////////
    class MEZZ_LIB ScriptLuaRuntimeException : public ScriptLuaException
    {
    public:
        /// @brief Thrown when a Lua script has a runtime error.
        static const Whole ExceptionCode = Exception::SCRIPT_EXCEPTION_LUA_RUNTIME;

        /// @brief Class constructor.
        /// @param TypeName The name of this class.
        /// @param Message A basic description of the error.
        /// @param SrcFunction The name of the function from which this originated.
        /// @param SrcFile The name of the file from which this originated.
        /// @param FileLine The line on the named file from which this originated.
        ScriptLuaRuntimeException(const String& TypeName, const String& Message, const String& SrcFunction, const String& SrcFile, const Whole& FileLine)
            : ScriptLuaException("ScriptLuaRuntimeException", Message, SrcFunction, SrcFile, FileLine)
            {}

        /// @brief Class constructor.
        /// @param Message A basic description of the error.
        /// @param SrcFunction The name of the function from which this originated.
        /// @param SrcFile The name of the file from which this originated.
        /// @param FileLine The line on the named file from which this originated.
        ScriptLuaRuntimeException(const String& Message, const String& SrcFunction, const String& SrcFile, const Whole& FileLine)
            : ScriptLuaException("ScriptLuaRuntimeException", Message, SrcFunction, SrcFile, FileLine)
            {}

        /// @brief Class destructor.
        virtual ~ScriptLuaRuntimeException() throw() {}

        /// @copydoc Exception::GetExceptionCode()
        virtual Whole GetExceptionCode() const throw()
            { return ScriptLuaRuntimeException::ExceptionCode; }
    }; //ScriptLuaRuntimeException

    /// @brief A template metaprogramming construct used to ensure that all required ScriptLuaRuntimeException can be created at compile time if the compiler is clever.
    template<>
    struct MEZZ_LIB ExceptionFactory<ScriptLuaRuntimeException::ExceptionCode>
        { typedef ScriptLuaRuntimeException Type; };


    ///////////////////////////////////////////////////////////////////////////////
    /// @brief Thrown when Lua has an error handling an error.
    ///////////////////
    class MEZZ_LIB ScriptLuaErrErrException : public ScriptLuaException
    {
    public:
        /// @brief Thrown when Lua has an error handling an error.
        static const Whole ExceptionCode = Exception::SCRIPT_EXCEPTION_LUA_ERRERR;

        /// @brief Class constructor.
        /// @param TypeName The name of this class.
        /// @param Message A basic description of the error.
        /// @param SrcFunction The name of the function from which this originated.
        /// @param SrcFile The name of the file from which this originated.
        /// @param FileLine The line on the named file from which this originated.
        ScriptLuaErrErrException(const String& TypeName, const String& Message, const String& SrcFunction, const String& SrcFile, const Whole& FileLine)
            : ScriptLuaException("ScriptLuaErrErrException", Message, SrcFunction, SrcFile, FileLine)
            {}

        /// @brief Class constructor.
        /// @param Message A basic description of the error.
        /// @param SrcFunction The name of the function from which this originated.
        /// @param SrcFile The name of the file from which this originated.
        /// @param FileLine The line on the named file from which this originated.
        ScriptLuaErrErrException(const String& Message, const String& SrcFunction, const String& SrcFile, const Whole& FileLine)
            : ScriptLuaException("ScriptLuaErrErrException", Message, SrcFunction, SrcFile, FileLine)
            {}

        /// @brief Class destructor.
        virtual ~ScriptLuaErrErrException() throw() {}

        /// @copydoc Exception::GetExceptionCode()
        virtual Whole GetExceptionCode() const throw()
            { return ScriptLuaErrErrException::ExceptionCode; }
    }; //ScriptLuaErrErrException

    /// @brief A template metaprogramming construct used to ensure that all required ScriptLuaErrErrException can be created at compile time if the compiler is clever.
    template<>
    struct MEZZ_LIB ExceptionFactory<ScriptLuaErrErrException::ExceptionCode>
        { typedef ScriptLuaErrErrException Type; };


    ///////////////////////////////////////////////////////////////////////////////
    /// @brief Thrown when Math has failed.
    ///////////////////
    class MEZZ_LIB ArithmeticException : public Exception
    {
    public:
        /// @brief Thrown when Math has failed.
        static const Whole ExceptionCode = Exception::ARITHMETIC_EXCEPTION;

        /// @brief Class constructor.
        /// @param TypeName The name of this class.
        /// @param Message A basic description of the error.
        /// @param SrcFunction The name of the function from which this originated.
        /// @param SrcFile The name of the file from which this originated.
        /// @param FileLine The line on the named file from which this originated.
        ArithmeticException(const String& TypeName, const String& Message, const String& SrcFunction, const String& SrcFile, const Whole& FileLine)
            : Exception("ArithmeticException", Message, SrcFunction, SrcFile, FileLine)
            {}

        /// @brief Class constructor.
        /// @param Message A basic description of the error.
        /// @param SrcFunction The name of the function from which this originated.
        /// @param SrcFile The name of the file from which this originated.
        /// @param FileLine The line on the named file from which this originated.
        ArithmeticException(const String& Message, const String& SrcFunction, const String& SrcFile, const Whole& FileLine)
            : Exception("ArithmeticException", Message, SrcFunction, SrcFile, FileLine)
            {}

        /// @brief Class destructor.
        virtual ~ArithmeticException() throw() {}

        /// @copydoc Exception::GetExceptionCode()
        virtual Whole GetExceptionCode() const throw()
            { return ArithmeticException::ExceptionCode; }
    }; //ArithmeticException

    /// @brief A template metaprogramming construct used to ensure that all required ArithmeticException can be created at compile time if the compiler is clever.
    template<>
    struct MEZZ_LIB ExceptionFactory<ArithmeticException::ExceptionCode>
        { typedef ArithmeticException Type; };


    ///////////////////////////////////////////////////////////////////////////////
    /// @brief Thrown when parameters are checked at runtime and found invalid.
    ///////////////////
    class MEZZ_LIB InvalidParametersException : public Exception
    {
    public:
        /// @brief Thrown when parameters are checked at runtime and found invalid.
        static const Whole ExceptionCode = Exception::INVALID_PARAMETERS_EXCEPTION;

        /// @brief Class constructor.
        /// @param TypeName The name of this class.
        /// @param Message A basic description of the error.
        /// @param SrcFunction The name of the function from which this originated.
        /// @param SrcFile The name of the file from which this originated.
        /// @param FileLine The line on the named file from which this originated.
        InvalidParametersException(const String& TypeName, const String& Message, const String& SrcFunction, const String& SrcFile, const Whole& FileLine)
            : Exception("InvalidParametersException", Message, SrcFunction, SrcFile, FileLine)
            {}

        /// @brief Class constructor.
        /// @param Message A basic description of the error.
        /// @param SrcFunction The name of the function from which this originated.
        /// @param SrcFile The name of the file from which this originated.
        /// @param FileLine The line on the named file from which this originated.
        InvalidParametersException(const String& Message, const String& SrcFunction, const String& SrcFile, const Whole& FileLine)
            : Exception("InvalidParametersException", Message, SrcFunction, SrcFile, FileLine)
            {}

        /// @brief Class destructor.
        virtual ~InvalidParametersException() throw() {}

        /// @copydoc Exception::GetExceptionCode()
        virtual Whole GetExceptionCode() const throw()
            { return InvalidParametersException::ExceptionCode; }
    }; //InvalidParametersException

    /// @brief A template metaprogramming construct used to ensure that all required InvalidParametersException can be created at compile time if the compiler is clever.
    template<>
    struct MEZZ_LIB ExceptionFactory<InvalidParametersException::ExceptionCode>
        { typedef InvalidParametersException Type; };


    ///////////////////////////////////////////////////////////////////////////////
    /// @brief Thrown when a version is accessed/parsed/required and it cannot work correctly or is missing.
    ///////////////////
    class MEZZ_LIB InvalidVersionException : public Exception
    {
    public:
        /// @brief Thrown when a version is accessed/parsed/required and it cannot work correctly or is missing.
        static const Whole ExceptionCode = Exception::INVALID_VERSION_EXCEPTION;

        /// @brief Class constructor.
        /// @param TypeName The name of this class.
        /// @param Message A basic description of the error.
        /// @param SrcFunction The name of the function from which this originated.
        /// @param SrcFile The name of the file from which this originated.
        /// @param FileLine The line on the named file from which this originated.
        InvalidVersionException(const String& TypeName, const String& Message, const String& SrcFunction, const String& SrcFile, const Whole& FileLine)
            : Exception("InvalidVersionException", Message, SrcFunction, SrcFile, FileLine)
            {}

        /// @brief Class constructor.
        /// @param Message A basic description of the error.
        /// @param SrcFunction The name of the function from which this originated.
        /// @param SrcFile The name of the file from which this originated.
        /// @param FileLine The line on the named file from which this originated.
        InvalidVersionException(const String& Message, const String& SrcFunction, const String& SrcFile, const Whole& FileLine)
            : Exception("InvalidVersionException", Message, SrcFunction, SrcFile, FileLine)
            {}

        /// @brief Class destructor.
        virtual ~InvalidVersionException() throw() {}

        /// @copydoc Exception::GetExceptionCode()
        virtual Whole GetExceptionCode() const throw()
            { return InvalidVersionException::ExceptionCode; }
    }; //InvalidVersionException

    /// @brief A template metaprogramming construct used to ensure that all required InvalidVersionException can be created at compile time if the compiler is clever.
    template<>
    struct MEZZ_LIB ExceptionFactory<InvalidVersionException::ExceptionCode>
        { typedef InvalidVersionException Type; };


    ///////////////////////////////////////////////////////////////////////////////
    /// @brief Thrown when the available information should have worked but failed for unknown reasons.
    ///////////////////
    class MEZZ_LIB InvalidStateException : public Exception
    {
    public:
        /// @brief Thrown when the available information should have worked but failed for unknown reasons.
        static const Whole ExceptionCode = Exception::INVALID_STATE_EXCEPTION;

        /// @brief Class constructor.
        /// @param TypeName The name of this class.
        /// @param Message A basic description of the error.
        /// @param SrcFunction The name of the function from which this originated.
        /// @param SrcFile The name of the file from which this originated.
        /// @param FileLine The line on the named file from which this originated.
        InvalidStateException(const String& TypeName, const String& Message, const String& SrcFunction, const String& SrcFile, const Whole& FileLine)
            : Exception("InvalidStateException", Message, SrcFunction, SrcFile, FileLine)
            {}

        /// @brief Class constructor.
        /// @param Message A basic description of the error.
        /// @param SrcFunction The name of the function from which this originated.
        /// @param SrcFile The name of the file from which this originated.
        /// @param FileLine The line on the named file from which this originated.
        InvalidStateException(const String& Message, const String& SrcFunction, const String& SrcFile, const Whole& FileLine)
            : Exception("InvalidStateException", Message, SrcFunction, SrcFile, FileLine)
            {}

        /// @brief Class destructor.
        virtual ~InvalidStateException() throw() {}

        /// @copydoc Exception::GetExceptionCode()
        virtual Whole GetExceptionCode() const throw()
            { return InvalidStateException::ExceptionCode; }
    }; //InvalidStateException

    /// @brief A template metaprogramming construct used to ensure that all required InvalidStateException can be created at compile time if the compiler is clever.
    template<>
    struct MEZZ_LIB ExceptionFactory<InvalidStateException::ExceptionCode>
        { typedef InvalidStateException Type; };


    ///////////////////////////////////////////////////////////////////////////////
    /// @brief Thrown when the graphics card/DirectX/OpenGL fail.
    ///////////////////
    class MEZZ_LIB RenderingAPIException : public Exception
    {
    public:
        /// @brief Thrown when the graphics card/DirectX/OpenGL fail.
        static const Whole ExceptionCode = Exception::RENDERINGAPI_EXCEPTION;

        /// @brief Class constructor.
        /// @param TypeName The name of this class.
        /// @param Message A basic description of the error.
        /// @param SrcFunction The name of the function from which this originated.
        /// @param SrcFile The name of the file from which this originated.
        /// @param FileLine The line on the named file from which this originated.
        RenderingAPIException(const String& TypeName, const String& Message, const String& SrcFunction, const String& SrcFile, const Whole& FileLine)
            : Exception("RenderingAPIException", Message, SrcFunction, SrcFile, FileLine)
            {}

        /// @brief Class constructor.
        /// @param Message A basic description of the error.
        /// @param SrcFunction The name of the function from which this originated.
        /// @param SrcFile The name of the file from which this originated.
        /// @param FileLine The line on the named file from which this originated.
        RenderingAPIException(const String& Message, const String& SrcFunction, const String& SrcFile, const Whole& FileLine)
            : Exception("RenderingAPIException", Message, SrcFunction, SrcFile, FileLine)
            {}

        /// @brief Class destructor.
        virtual ~RenderingAPIException() throw() {}

        /// @copydoc Exception::GetExceptionCode()
        virtual Whole GetExceptionCode() const throw()
            { return RenderingAPIException::ExceptionCode; }
    }; //RenderingAPIException

    /// @brief A template metaprogramming construct used to ensure that all required RenderingAPIException can be created at compile time if the compiler is clever.
    template<>
    struct MEZZ_LIB ExceptionFactory<RenderingAPIException::ExceptionCode>
        { typedef RenderingAPIException Type; };


    ///////////////////////////////////////////////////////////////////////////////
    /// @brief Thrown when a rutime assertion could have been thrown.
    ///////////////////
    class MEZZ_LIB RuntimeAssertionException : public Exception
    {
    public:
        /// @brief Thrown when a rutime assertion could have been thrown.
        static const Whole ExceptionCode = Exception::RT_ASSERTION_EXCEPTION;

        /// @brief Class constructor.
        /// @param TypeName The name of this class.
        /// @param Message A basic description of the error.
        /// @param SrcFunction The name of the function from which this originated.
        /// @param SrcFile The name of the file from which this originated.
        /// @param FileLine The line on the named file from which this originated.
        RuntimeAssertionException(const String& TypeName, const String& Message, const String& SrcFunction, const String& SrcFile, const Whole& FileLine)
            : Exception("RuntimeAssertionException", Message, SrcFunction, SrcFile, FileLine)
            {}

        /// @brief Class constructor.
        /// @param Message A basic description of the error.
        /// @param SrcFunction The name of the function from which this originated.
        /// @param SrcFile The name of the file from which this originated.
        /// @param FileLine The line on the named file from which this originated.
        RuntimeAssertionException(const String& Message, const String& SrcFunction, const String& SrcFile, const Whole& FileLine)
            : Exception("RuntimeAssertionException", Message, SrcFunction, SrcFile, FileLine)
            {}

        /// @brief Class destructor.
        virtual ~RuntimeAssertionException() throw() {}

        /// @copydoc Exception::GetExceptionCode()
        virtual Whole GetExceptionCode() const throw()
            { return RuntimeAssertionException::ExceptionCode; }
    }; //RuntimeAssertionException

    /// @brief A template metaprogramming construct used to ensure that all required RuntimeAssertionException can be created at compile time if the compiler is clever.
    template<>
    struct MEZZ_LIB ExceptionFactory<RuntimeAssertionException::ExceptionCode>
        { typedef RuntimeAssertionException Type; };


    ///////////////////////////////////////////////////////////////////////////////
    /// @brief Thrown when an unknown internal error occurred.
    ///////////////////
    class MEZZ_LIB InternalException : public Exception
    {
    public:
        /// @brief Thrown when an unknown internal error occurred.
        static const Whole ExceptionCode = Exception::INTERNAL_EXCEPTION;

        /// @brief Class constructor.
        /// @param TypeName The name of this class.
        /// @param Message A basic description of the error.
        /// @param SrcFunction The name of the function from which this originated.
        /// @param SrcFile The name of the file from which this originated.
        /// @param FileLine The line on the named file from which this originated.
        InternalException(const String& TypeName, const String& Message, const String& SrcFunction, const String& SrcFile, const Whole& FileLine)
            : Exception("InternalException", Message, SrcFunction, SrcFile, FileLine)
            {}

        /// @brief Class constructor.
        /// @param Message A basic description of the error.
        /// @param SrcFunction The name of the function from which this originated.
        /// @param SrcFile The name of the file from which this originated.
        /// @param FileLine The line on the named file from which this originated.
        InternalException(const String& Message, const String& SrcFunction, const String& SrcFile, const Whole& FileLine)
            : Exception("InternalException", Message, SrcFunction, SrcFile, FileLine)
            {}

        /// @brief Class destructor.
        virtual ~InternalException() throw() {}

        /// @copydoc Exception::GetExceptionCode()
        virtual Whole GetExceptionCode() const throw()
            { return InternalException::ExceptionCode; }
    }; //InternalException

    /// @brief A template metaprogramming construct used to ensure that all required InternalException can be created at compile time if the compiler is clever.
    template<>
    struct MEZZ_LIB ExceptionFactory<InternalException::ExceptionCode>
        { typedef InternalException Type; };


    ///////////////////////////////////////////////////////////////////////////////
    /// @brief Thrown when we just have not coded a thing yet, but we knew what the API should look like.
    ///////////////////
    class MEZZ_LIB NotImplementedException : public Exception
    {
    public:
        /// @brief Thrown when we just have not coded a thing yet, but we knew what the API should look like.
        static const Whole ExceptionCode = Exception::NOT_IMPLEMENTED_EXCEPTION;

        /// @brief Class constructor.
        /// @param TypeName The name of this class.
        /// @param Message A basic description of the error.
        /// @param SrcFunction The name of the function from which this originated.
        /// @param SrcFile The name of the file from which this originated.
        /// @param FileLine The line on the named file from which this originated.
        NotImplementedException(const String& TypeName, const String& Message, const String& SrcFunction, const String& SrcFile, const Whole& FileLine)
            : Exception("NotImplementedException", Message, SrcFunction, SrcFile, FileLine)
            {}

        /// @brief Class constructor.
        /// @param Message A basic description of the error.
        /// @param SrcFunction The name of the function from which this originated.
        /// @param SrcFile The name of the file from which this originated.
        /// @param FileLine The line on the named file from which this originated.
        NotImplementedException(const String& Message, const String& SrcFunction, const String& SrcFile, const Whole& FileLine)
            : Exception("NotImplementedException", Message, SrcFunction, SrcFile, FileLine)
            {}

        /// @brief Class destructor.
        virtual ~NotImplementedException() throw() {}

        /// @copydoc Exception::GetExceptionCode()
        virtual Whole GetExceptionCode() const throw()
            { return NotImplementedException::ExceptionCode; }
    }; //NotImplementedException

    /// @brief A template metaprogramming construct used to ensure that all required NotImplementedException can be created at compile time if the compiler is clever.
    template<>
    struct MEZZ_LIB ExceptionFactory<NotImplementedException::ExceptionCode>
        { typedef NotImplementedException Type; };


    ///////////////////////////////////////////////////////////////////////////////
    /// @brief Then when a complex class is assigned to itself or other invalid assignments occur.
    ///////////////////
    class MEZZ_LIB InvalidAssignment : public Exception
    {
    public:
        /// @brief Then when a complex class is assigned to itself or other invalid assignments occur.
        static const Whole ExceptionCode = Exception::INVALID_ASSIGNMENT;

        /// @brief Class constructor.
        /// @param TypeName The name of this class.
        /// @param Message A basic description of the error.
        /// @param SrcFunction The name of the function from which this originated.
        /// @param SrcFile The name of the file from which this originated.
        /// @param FileLine The line on the named file from which this originated.
        InvalidAssignment(const String& TypeName, const String& Message, const String& SrcFunction, const String& SrcFile, const Whole& FileLine)
            : Exception("InvalidAssignment", Message, SrcFunction, SrcFile, FileLine)
            {}

        /// @brief Class constructor.
        /// @param Message A basic description of the error.
        /// @param SrcFunction The name of the function from which this originated.
        /// @param SrcFile The name of the file from which this originated.
        /// @param FileLine The line on the named file from which this originated.
        InvalidAssignment(const String& Message, const String& SrcFunction, const String& SrcFile, const Whole& FileLine)
            : Exception("InvalidAssignment", Message, SrcFunction, SrcFile, FileLine)
            {}

        /// @brief Class destructor.
        virtual ~InvalidAssignment() throw() {}

        /// @copydoc Exception::GetExceptionCode()
        virtual Whole GetExceptionCode() const throw()
            { return InvalidAssignment::ExceptionCode; }
    }; //InvalidAssignment

    /// @brief A template metaprogramming construct used to ensure that all required InvalidAssignment can be created at compile time if the compiler is clever.
    template<>
    struct MEZZ_LIB ExceptionFactory<InvalidAssignment::ExceptionCode>
        { typedef InvalidAssignment Type; };





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

SWIG_INFO_ENDCLASS

#endif // \exception_cpp
