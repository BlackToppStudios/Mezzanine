# © Copyright 2010 - 2011 BlackTopp Studios Inc.
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
#   Joseph Toppi - toppij@gmail.c"om
#   John Blackwood - makoenergy02@gmail.c"om


###############################################################################
# The build script for one piece of the Mezzanine engine
cmake_minimum_required (VERSION 2.8)
project (Lua)

message ("${PROJECT_NAME} - Begin")
set(${PROJECT_NAME}_sources
	"lua-5.2.0/src/lapi.c"
	"lua-5.2.0/src/lapi.h"
	"lua-5.2.0/src/lauxlib.c"
	"lua-5.2.0/src/lauxlib.h"
	"lua-5.2.0/src/lbaselib.c"
	"lua-5.2.0/src/lbitlib.c"
	"lua-5.2.0/src/lcode.c"
	"lua-5.2.0/src/lcode.h"
	"lua-5.2.0/src/lcorolib.c"
	"lua-5.2.0/src/lctype.c"
	"lua-5.2.0/src/lctype.h"
	"lua-5.2.0/src/ldblib.c"
	"lua-5.2.0/src/ldebug.c"
	"lua-5.2.0/src/ldebug.h"
	"lua-5.2.0/src/ldo.c"
	"lua-5.2.0/src/ldo.h"
	"lua-5.2.0/src/ldump.c"
	"lua-5.2.0/src/lfunc.c"
	"lua-5.2.0/src/lfunc.h"
	"lua-5.2.0/src/lgc.c"
	"lua-5.2.0/src/lgc.h"
	"lua-5.2.0/src/linit.c"
	"lua-5.2.0/src/liolib.c"
	"lua-5.2.0/src/llex.c"
	"lua-5.2.0/src/llex.h"
	"lua-5.2.0/src/llimits.h"
	"lua-5.2.0/src/lmathlib.c"
	"lua-5.2.0/src/lmem.c"
	"lua-5.2.0/src/lmem.h"
	"lua-5.2.0/src/loadlib.c"
	"lua-5.2.0/src/lobject.c"
	"lua-5.2.0/src/lobject.h"
	"lua-5.2.0/src/lopcodes.c"
	"lua-5.2.0/src/lopcodes.h"
	"lua-5.2.0/src/loslib.c"
	"lua-5.2.0/src/lparser.c"
	"lua-5.2.0/src/lparser.h"
	"lua-5.2.0/src/lstate.c"
	"lua-5.2.0/src/lstate.h"
	"lua-5.2.0/src/lstring.c"
	"lua-5.2.0/src/lstring.h"
	"lua-5.2.0/src/lstrlib.c"
	"lua-5.2.0/src/ltable.c"
	"lua-5.2.0/src/ltable.h"
	"lua-5.2.0/src/ltablib.c"
	"lua-5.2.0/src/ltm.c"
	"lua-5.2.0/src/ltm.h"
	"lua-5.2.0/src/lua.c"
	"lua-5.2.0/src/luac.c"
	"lua-5.2.0/src/luaconf.h"
	"lua-5.2.0/src/lua.h"
	"lua-5.2.0/src/lua.hpp"
	"lua-5.2.0/src/lualib.h"
	"lua-5.2.0/src/lundump.c"
	"lua-5.2.0/src/lundump.h"
	"lua-5.2.0/src/lvm.c"
	"lua-5.2.0/src/lvm.h"
	"lua-5.2.0/src/lzio.c"
	"lua-5.2.0/src/lzio.h"
)

include_directories(
	"${${PROJECT_NAME}_SOURCE_DIR}/lua5.1/include/"
)

add_definitions( ${CommonBuildFlags} )

set (LIBRARY_OUTPUT_PATH "${Mezzanine_ROOT_DIR}data/${BuildPlatform}/lua/")
message ( STATUS "Putting lua binaries in: ${LIBRARY_OUTPUT_PATH}")

add_library(lua STATIC ${${PROJECT_NAME}_sources})
message ("${PROJECT_NAME} - End")


