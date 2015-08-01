# © Copyright 2010 - 2015 BlackTopp Studios Inc.
# This file is part of The Mezzanine Engine.
#
#    The Mezzanine Engine is free software: you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    (at your option) any later version.
#
#    The Mezzanine Engine is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with The Mezzanine Engine.  If not, see <http://www.gnu.org/licenses/>.
#
#   The original authors have included a copy of the license specified above in the
#   'Docs' folder. See 'gpl.txt'
#
#   We welcome the use of the Mezzanine engine to anyone, including companies who wish to
#   Build professional software and charge for their product.
#
#   However there are some practical restrictions, so if your project involves
#   any of the following you should contact us and we will try to work something
#   out:
#    - DRM or Copy Protection of any kind(except Copyrights)
#    - Software Patents You Do Not Wish to Freely License
#    - Any Kind of Linking to Non-GPL licensed Works
#    - Are Currently In Violation of Another Copyright Holder's GPL License
#    - If You want to change our code and not add a few hundred MB of stuff to
#        your distribution
#
#   These and other limitations could cause serious legal problems if you ignore
#   them, so it is best to simply contact us or the Free Software Foundation, if
#   you have any questions.
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
#file(READ ${InputDirectory}exceptionfile.in ExceptionFormat)

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

add_exception(IO_EXCEPTION              0x01F00F00 IOException                  ExceptionBase
    "Thrown when there was an issue with IO but very little was known about it."                "java/io/IOException")
add_exception(SYNTAX_EXCEPTION          0x02F00F00 SyntaxException              ExceptionBase
    "Thrown when parsing something human readable and it wasn't structured work as expected."   "javax/script/ScriptException")
add_exception(UNIMPLEMENTED_EXCEPTION   0x03F00F00 UnimplementedException       ExceptionBase
    "Thrown when there was an issue with IO but very little was known about it."                "java/lang/UnsupportedOperationException")
add_exception(ARGUMENT_EXCEPTION        0x04F00F00 ArgumentException            ExceptionBase
    "Thrown when something invalid is passed into a method."                                    "java/lang/IllegalArgumentException")

add_exception(WIDGET_ARGUMENT_EXCEPTION 0x04F01F00 WidgetArgumentException      ArgumentException
    "Thrown when something invalid is passed into a method on the Widget Class."                "java/lang/IllegalArgumentException")
add_exception(WIDGET_NO_SUCH_METHOD     0x04F01F01 WidgetNoSuchMethodException  WidgetArgumentException
    "Thrown when something invalid is passed into a method on the Widget Class."                "java/lang/IllegalArgumentException")

#add_exception(IO_EXCEPTION 0x01F00F00 IOException ExceptionBase "Thrown when there was an issue with IO but very little was known about it.")
#add_exception(IO_FILE_EXCEPTION 0x01F01F00 FileException IOException "Thrown when there is an unknown issue with a file.")
#add_exception(IO_FILE_READ_EXCEPTION 0x01F01F01 FileReadException FileException "Thrown when a file could not be read and permissions seem to not be an issue (Filesystem/hardware issue?)")
#add_exception(IO_FILE_WRITE_EXCEPTION 0x01F01F02 FileWriteException FileException "Thrown when a file could not be written and perimssions seem not to be an issue.")
#add_exception(IO_FILE_NOT_FOUND_EXCEPTION 0x01F01F03 FileNotFoundException FileException "Thrown when a file was expected to be there, but was not.")
#add_exception(IO_FILE_PERMISSION_EXCEPTION 0x01F01F04 FilePermissionException FileException "Thrown when permission was denied to a file.")
#add_exception(IO_DIRECTORY_EXCEPTION 0x01F02F00 DirectoryException IOException "Thrown when there is an unknown issue with a file.")
#add_exception(IO_DIRECTORY_READ_EXCEPTION 0x01F02F01 DirectoryReadException DirectoryException "Thrown when a directory could be read and it wasn't a permission issue.")
#add_exception(IO_DIRECTORY_WRITE_EXCEPTION 0x01F02F02 DirectoryWriteException DirectoryException "Thrown when a directory could be written to and it wasn't a permission issue.")
#add_exception(IO_DIRECTORY_NOT_FOUND_EXCEPTION 0x01F02F03 DirectoryNotFoundException DirectoryException "Thrown when a directory was expected to be there, but was not.")
#add_exception(IO_DIRECTORY_PERMISSION_EXCEPTION 0x01F02F04 DirectoryPermissionException DirectoryException "Thrown when permission is denied to a directory.")
#add_exception(IO_NETWORK_EXCEPTION 0x01F03F00 NetworkException IOException "Thrown when something unknown causes network IO to fail.")
#add_exception(IO_NETWORK_READ_EXCEPTION 0x01F03F01 NetworkReadException NetworkException "Thrown when data could not be read from the network (downloads).")
#add_exception(IO_NETWORK_WRITE_EXCEPTION 0x01F03F02 NetworkWriteException NetworkException "Thrown when data could not be read from the network (uploads).")
#add_exception(IO_NETWORK_NOT_FOUND_EXCEPTION 0x01F03F03 NetworkNotFoundException NetworkException "Thrown when no network connection is available.")
#add_exception(IO_NETWORK_URL_EXCEPTION 0x01F03F04 NetworkURLException NetworkException "Thrown when an address is invalid or could not be found.")
#add_exception(IO_NETWORK_PERMISSION_EXCEPTION 0x01F03F05 NetworkPermissionException NetworkException "Thrown when permision was denied to a network interface or network resource.")
add_enum(LAST_EXCEPTION 0x01F00F00 "To allow some inspection of the Exception system this can be use to identify the highest numbered Exception")


#message(STATUS "Writing exception hiearchy to ${DestinationDirectory}GeneratedExceptions.h")
#message(STATUS "Enumerations double check ${ExceptionEnumerations}")
#message(STATUS "Exceptions double check ${Exceptions}")
configure_file(${InputDirectory}exceptionfile.in ${DestinationDirectory}GeneratedExceptions.h)
