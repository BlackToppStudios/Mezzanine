# © Copyright 2010 - 2011 BlackTopp Studios Inc.
# This file is part of The PhysGame Engine.
#
#    The PhysGame Engine is free software: you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    (at your option) any later version.
#
#    The PhysGame Engine is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with The PhysGame Engine.  If not, see <http://www.gnu.org/licenses/>.
#
#   The original authors have included a copy of the license specified above in the
#   'Docs' folder. See 'gpl.txt'
#
#   We welcome the use of The PhysGame anyone, including companies who wish to
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
# The build script for one piece of the physgame engine
cmake_minimum_required (VERSION 2.8)
project (Lua)

message ("${PROJECT_NAME} - Begin")
set(${PROJECT_NAME}_sources
	"lua5.1/src/lapi.h"
	"lua5.1/src/lauxlib.h"
	"lua5.1/src/lcode.h"
	"lua5.1/src/ldebug.h"
	"lua5.1/src/ldo.h"
	"lua5.1/src/lfunc.h"
	"lua5.1/src/lgc.h"
	"lua5.1/src/llex.h"
	"lua5.1/src/llimits.h"
	"lua5.1/src/lmem.h"
	"lua5.1/src/lobject.h"
	"lua5.1/src/lopcodes.h"
	"lua5.1/src/lparser.h"
	"lua5.1/src/lstate.h"
	"lua5.1/src/lstring.h"
	"lua5.1/src/ltable.h"
	"lua5.1/src/ltm.h"
	"lua5.1/src/luaconf.h"
	"lua5.1/src/lua.h"
	"lua5.1/src/lualib.h"
	"lua5.1/src/lundump.h"
	"lua5.1/src/lvm.h"
	"lua5.1/src/lzio.h"

	"lua5.1/src/lapi.c"
	"lua5.1/src/lauxlib.c"
	"lua5.1/src/lbaselib.c"
	"lua5.1/src/lcode.c"
	"lua5.1/src/ldblib.c"
	"lua5.1/src/ldebug.c"
	"lua5.1/src/ldo.c"
	"lua5.1/src/ldump.c"
	"lua5.1/src/lfunc.c"
	"lua5.1/src/lgc.c"
	"lua5.1/src/linit.c"
	"lua5.1/src/liolib.c"
	"lua5.1/src/llex.c"
	"lua5.1/src/lmathlib.c"
	"lua5.1/src/lmem.c"
	"lua5.1/src/loadlib.c"
	"lua5.1/src/lobject.c"
	"lua5.1/src/lopcodes.c"
	"lua5.1/src/loslib.c"
	"lua5.1/src/lparser.c"
	"lua5.1/src/lstate.c"
	"lua5.1/src/lstring.c"
	"lua5.1/src/lstrlib.c"
	"lua5.1/src/ltable.c"
	"lua5.1/src/ltablib.c"
	"lua5.1/src/ltm.c"
	"lua5.1/src/lua.c"
	"lua5.1/src/luac.c"
	"lua5.1/src/lundump.c"
	"lua5.1/src/lvm.c"
	"lua5.1/src/lzio.c"
	"lua5.1/src/print.c"
#	"lua5.1/src/wmain.c"
)

include_directories(
	"${${PROJECT_NAME}_SOURCE_DIR}/lua5.1/include/"
)

add_definitions( ${CommonBuildFlags} )

set (LIBRARY_OUTPUT_PATH "${physgame_ROOT_DIR}data/${BuildPlatform}/lua/")
message ("Putting lua binaries in: ${LIBRARY_OUTPUT_PATH}")

add_library(lua STATIC ${${PROJECT_NAME}_sources})
message ("${PROJECT_NAME} - End")
