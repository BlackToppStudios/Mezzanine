# © Copyright 2010 - 2015 BlackTopp Studios Inc
# This file is part of The Mezzanine Engine
#
#    The Mezzanine Engine is free software: you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    (at your option) any later version
#
#    The Mezzanine Engine is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more detail
#
#    You should have received a copy of the GNU General Public License
#    along with The Mezzanine Engine.  If not, see <http://www.gnu.org/licenses/>
#
#   The original authors have included a copy of the license specified above in the
#   'Docs' folder. See 'gpl.txt'
#
#   We welcome the use of the Mezzanine engine to anyone, including companies who wish to
#   Build professional software and charge for their product
#
#   However there are some practical restrictions, so if your project involves
#   any of the following you should contact us and we will try to work something
#   out:
#    - DRM or Copy Protection of any kind(except Copyrights
#    - Software Patents You Do Not Wish to Freely License
#    - Any Kind of Linking to Non-GPL licensed Works
#    - Are Currently In Violation of Another Copyright Holder's GPL License
#    - If You want to change our code and not add a few hundred MB of stuff to
#        your distribution
#
#   These and other limitations could cause serious legal problems if you ignore
#   them, so it is best to simply contact us or the Free Software Foundation, if
#   you have any questions
#
#   Joseph Toppi - toppij@gmail.com
#   John Blackwood - makoenergy02@gmail.com

project(ExceptionGeneration)
cmake_minimum_required(VERSION 3.0)

# Identify root folders
set(InputDirectory ${${PROJECT_NAME}_SOURCE_DIR}/)
set(DestinationDirectory ${${PROJECT_NAME}_BINARY_DIR}/)

# Prepare some formats - Be sure each of these files ends in a newline or enjoy syntax errors
file(READ ${InputDirectory}enum.in EnumFormat)
file(READ ${InputDirectory}enumlast.in EnumLastFormat)
file(READ ${InputDirectory}exception.in ExceptionFormat)

#Some places to store
set(ExceptionEnumerations "")
set(Exceptions "")

function(add_enum EnumName HexValue BriefDox)
    string(CONFIGURE ${EnumFormat} results @ONLY)
    set(ExceptionEnumerations "${ExceptionEnumerations}${results}" PARENT_SCOPE)
endfunction(add_enum)

function(add_final_enum EnumName HexValue BriefDox)
    string(CONFIGURE ${EnumLastFormat} results @ONLY)
    set(ExceptionEnumerations "${ExceptionEnumerations}${results}" PARENT_SCOPE)
endfunction(add_final_enum)

function(add_exception EnumName HexValue ExceptionClass ExceptionParent BriefDox JavaException)
    string(REPLACE "/" "." JavaExceptionDot ${JavaException})
    message(STATUS "${JavaException} -> ${JavaExceptionDot}")
    add_enum(${EnumName} ${HexValue} ${BriefDox})
    set(ExceptionEnumerations "${ExceptionEnumerations}" PARENT_SCOPE)
    string(CONFIGURE "${ExceptionFormat}" results @ONLY)
    set(Exceptions "${Exceptions}\n${results}" PARENT_SCOPE)
endfunction(add_exception)

add_enum(FIRST_EXCEPTION 0x01F00F00 "To allow some inspection of the Exception system this can be use to identify the lower numbered Exception")

add_exception(  IO_EXCEPTION                          0x01F00F00    IOException                               ExceptionBase
                "Thrown when there was an issue with IO but very little was known about it."
                "java/io/IOException")

add_exception(	IO_FILE_EXCEPTION                      0x01F01F00   FileException                             IOException
                "Thrown when there is an unknown issue with a file"
                "java/lang/XXXXX")
add_exception(	IO_FILE_READ_EXCEPTION                 0x01F01F01   FileReadException                         FileException
                "Thrown when a file could not be read and permissions seem to not be an issue (Filesystem/hardware issue?"
                "java/lang/XXXXX")
add_exception(	IO_FILE_WRITE_EXCEPTION                0x01F01F02   FileWriteException                        FileException
                "Thrown when a file could not be written and perimssions seem not to be an issue"
                "java/lang/XXXXX")
add_exception(	IO_FILE_NOT_FOUND_EXCEPTION            0x01F01F03   FileNotFoundException                     FileException
                "Thrown when a file was expected to be there, but was not"
                "java/lang/XXXXX")
add_exception(	IO_FILE_PERMISSION_EXCEPTION           0x01F01F04   FilePermissionException                   FileException
                "Thrown when permission was denied to a file"
                "java/lang/XXXXX")
add_exception(	IO_DIRECTORY_EXCEPTION                 0x01F02F00   DirectoryException                        IOException
                "Thrown when there is an unknown issue with a file"
                "java/lang/XXXXX")
add_exception(	IO_DIRECTORY_READ_EXCEPTION            0x01F02F01   DirectoryReadException                    DirectoryException
                "Thrown when a directory could be read and it wasn't a permission issue"
                "java/lang/XXXXX")
add_exception(	IO_DIRECTORY_WRITE_EXCEPTION           0x01F02F02   DirectoryWriteException                   DirectoryException
                "Thrown when a directory could be written to and it wasn't a permission issue"
                "java/lang/XXXXX")
add_exception(	IO_DIRECTORY_NOT_FOUND_EXCEPTION       0x01F02F03   DirectoryNotFoundException                DirectoryException
                "Thrown when a directory was expected to be there, but was not"
                "java/lang/XXXXX")
add_exception(	IO_DIRECTORY_PERMISSION_EXCEPTION      0x01F02F04   DirectoryPermissionException              DirectoryException
                "Thrown when permission is denied to a directory"
                "java/lang/XXXXX")
add_exception(	IO_NETWORK_EXCEPTION                   0x01F03F00   NetworkException                          IOException
                "Thrown when something unknown causes network IO to fail"
                "java/lang/XXXXX")
add_exception(	IO_NETWORK_READ_EXCEPTION              0x01F03F01   NetworkReadException                      NetworkException
                "Thrown when data could not be read from the network (downloads)"
                "java/lang/XXXXX")
add_exception(	IO_NETWORK_WRITE_EXCEPTION             0x01F03F02   NetworkWriteException                     NetworkException
                "Thrown when data could not be read from the network (iloads)"
                "java/lang/XXXXX")
add_exception(	IO_NETWORK_NOT_FOUND_EXCEPTION         0x01F03F03   NetworkNotFoundException                  NetworkException
                "Thrown when no network connection is available"
                "java/lang/XXXXX")
add_exception(	IO_NETWORK_URL_EXCEPTION               0x01F03F04   NetworkURLException                       NetworkException
                "Thrown when an address is invalid or could not be found"
                "java/lang/XXXXX")
add_exception(	IO_NETWORK_PERMISSION_EXCEPTION        0x01F03F05   NetworkPermissionException                NetworkException
                "Thrown when permision was denied to a network interface or network resource"
                "java/lang/XXXXX")
##
add_exception(	IO_WRITE_EXCEPTION                     0x01F08F00   IOWriteException                          IOException
                "Thrown when a write is happening but something has prevented the underlying code from knowing what was writing"
                "java/lang/XXXXX")
add_exception(	IO_READ_EXCEPTION                      0x01F09F00   IOReadException                           IOException
                "Thrown when a read is happening but something has prevented the underlying code from knowing what was reading"
                "java/lang/XXXXX")
##
add_exception(	II_EXCEPTION                           0x02F00F00   InstanceIdentityException                 ExceptionBase
                "Thrown when an unknown error with using an Identifier and it is invalid"
                "java/lang/XXXXX")
add_exception(	II_IDENTITY_INVALID_EXCEPTION          0x02F01F00   InstanceIdentityInvalidException          InstanceIdentityException
                "Thrown when the identity string wasn't valid at all"
                "java/lang/XXXXX")
add_exception(	II_IDENTITY_NOT_FOUND_EXCEPTION        0x02F02F00   InstanceIdentityNotFoundException         InstanceIdentityException
                "Thrown when the requested identity could not be found"
                "java/lang/XXXXX")
add_exception(	II_DUPLICATE_IDENTITY_EXCEPTION        0x02F03F00   InstanceIdentityDuplicateException        InstanceIdentityException
                "Thrown when duplicates of teh same identity string exist"
                "java/lang/XXXXX")
##
add_exception(	MM_EXCEPTION                           0x03F00F00   MemoryManagementException                 ExceptionBase
                "Thrown when an unknown memory management exception occurs"
                "java/lang/XXXXX")
add_exception(	MM_OUT_OF_MEMORY_EXCEPTION             0x03F01F00   OutOfMemoryException                      MemoryManagementException
                "Thrown when A memory allocation was attempted and failed"
                "java/lang/XXXXX")
add_exception(	MM_OUT_OF_BOUNDS_EXCEPTION             0x03F02F00   MemoryOutOfBoundsException                MemoryManagementException
                "Thrown when attempted to access something that really should note be accessed"
                "java/lang/XXXXX")
##
add_exception(	SYNTAX_ERROR_EXCEPTION                 0x04F00F00   SyntaxErrorException                      ExceptionBase
                "Thrown when some kind of syntax exception"
                "java/lang/XXXXX")
add_exception(	SYNTAX_ERROR_EXCEPTION_XML             0x04F01F00   SyntaxErrorXMLException                   SyntaxErrorException
                "Thrown when and XML document is being parsed but is invalid"
                "java/lang/XXXXX")
add_exception(	SYNTAX_ERROR_EXCEPTION_XPATH           0x04F02F00   SyntaxErrorXPathException                 SyntaxErrorException
                "Thrown when an XPath query is being parsed but is invalid"
                "java/lang/XXXXX")
add_exception(	SYNTAX_ERROR_EXCEPTION_LUA             0x04F03F00   SyntaxErrorLuaException                   SyntaxErrorException
                "Thrown when lua code in incorrect"
                "java/lang/XXXXX")
##
add_exception(	SCRIPT_EXCEPTION                       0x05F00F00   ScriptException                           ExceptionBase
                "Thrown when an unknown error happens with a script"
                "java/lang/XXXXX")
add_exception(	SCRIPT_EXCEPTION_LUA                   0x05F01F00   ScriptLuaException                        ScriptException
                "Thrown when an unknown error happens in a Lua script"
                "java/lang/XXXXX")
add_exception(	SCRIPT_EXCEPTION_LUA_YIELD             0x05F01F01   ScriptLuaYieldException                   ScriptLuaException
                "Thrown when Lua returns a yield and it should not have"
                "java/lang/XXXXX")
add_exception(	SCRIPT_EXCEPTION_LUA_RUNTIME           0x05F01F02   ScriptLuaRuntimeException                 ScriptLuaException
                "Thrown when a Lua script has a runtime error"
                "java/lang/XXXXX")
add_exception(	SCRIPT_EXCEPTION_LUA_ERRERR            0x05F01F03   ScriptLuaErrErrException                  ScriptLuaException
                "Thrown when Lua has an error handling an error"
                "java/lang/XXXXX")
##
add_exception(	PARAMETERS_EXCEPTION                   0x06F01F00   ParametersException                       ExceptionBase
                "Thrown when parameters are checked at runtime and found invalid"
                "java/lang/XXXXX")
add_exception(	PARAMETERS_CAST_EXCEPTION              0x06F01F01   ParametersCastException                   ParametersException
                "Thrown when a pointer parameter is checked at runtime and cannot be cast as expected"
                "java/lang/XXXXX")
add_exception(	PARAMETERS_RANGE_EXCEPTION             0x06F01F02   ParametersRangeException                  ParametersException
                "Thrown when a passed parameter is checked at runtime and not in the expected range"
                "java/lang/XXXXX")
##
add_exception(	ARITHMETIC_EXCEPTION                   0x00F01F00   ArithmeticException                       ExceptionBase
                "Thrown when Math has failed"
                "java/lang/XXXXX")
add_exception(	INVALID_VERSION_EXCEPTION              0x00F02F00   InvalidVersionException                   ExceptionBase
                "Thrown when a version is accessed/parsed/required and it cannot work correctly or is missing"
                "java/lang/XXXXX")
add_exception(	INVALID_STATE_EXCEPTION                0x00F03F00   InvalidStateException                     ExceptionBase
                "Thrown when the available information should have worked but failed for unknown reasons"
                "java/lang/XXXXX")
add_exception(	RENDERINGAPI_EXCEPTION                 0x00F04F00   RenderingAPIException                     ExceptionBase
                "Thrown when the graphics card/DirectX/OpenGL fail"
                "java/lang/XXXXX")
add_exception(	RT_ASSERTION_EXCEPTION                 0x00F05F00   RuntimeAssertionException                 ExceptionBase
                "Thrown when a rutime assertion could have been Thrown"
                "java/lang/XXXXX")
add_exception(	INTERNAL_EXCEPTION                     0x00F06F00   InternalException                         ExceptionBase
                "Thrown when an unknown internal error occurred"
                "java/lang/XXXXX")
add_exception(	NOT_IMPLEMENTED_EXCEPTION              0x00F07F00   NotImplementedException                   ExceptionBase
                "Thrown when we just have not coded a thing yet, but we knew what the API should look like"
                "java/lang/XXXXX")
add_exception(	INVALID_ASSIGNMENT                     0x00F08F00   InvalidAssignment                         ExceptionBase
                "Thrown when a complex class is assigned to itself or other invalid assignments occur"
                "java/lang/XXXXX")

add_final_enum(LAST_EXCEPTION 0x01F00F00 "To allow some inspection of the Exception system this can be use to identify the highest numbered Exception")


# message(STATUS "Writing exception hiearchy to ${DestinationDirectory}GeneratedExceptions.h")#
# message(STATUS "Enumerations double check ${ExceptionEnumerations}")#
# message(STATUS "Exceptions double check ${Exceptions}")#
configure_file(${InputDirectory}exceptionfile.in ${DestinationDirectory}GeneratedExceptions.h)
