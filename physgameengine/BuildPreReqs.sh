#!/bin/bash
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
#   Joseph Toppi - toppij@gmail.com
#   John Blackwood - makoenergy02@gmail.com

# This will build ogre and SDL and make sure their files wind up were CMake Expects them to be
# This will not copy files to the data directory. That is up to you or to cmake.

#########################################################
# Functions
function Usage {
	echo
	echo "Usage: `basename $0` Debug|Release [ThreadCount]"
	echo "   Release - If the word Debug is the first argument, then The software will be compiled with Debug Symbols"
	echo "   Debug - If the word Debug is the first argument, then The software will be compiled with Debug Symbols"
	echo "   ThreadCount is the number of threads to compile with. You usually want to enter you amount of CPU cores."
	echo
}

#########################################################
# Gather Data from the platform
SystemName=`uname`
MakeLocation=`which make`
Linux=0
MinGW32=0
DetectedPlatform=0				#eventually I would like to make this work from a git bash prompt.

#########################################################
# Work with values gathered from the system so this script knows what is going on.
if [ "Linux" = "$SystemName" ]
then
	echo "System Detected: Linux"
	Linux=1
	DetectedPlatform=1
fi

if [ "MINGW32" = ${SystemName:0:7} ]
then
	echo "System Detected: 32bit MinGW"
	MinGW32=1
	DetectedPlatform=1
fi

if [ ! ${#MakeLocation} -gt 0 ]
then
	echo "Command 'make' not found"
	if [ 1 -eq $Linux ]
	then
		echo "Are 'make', a compiler an proper build tools installed?"
	fi
	if [ 1 -eq $MinGW32 ]
	then
		echo "Are you running this from a Git bash prompt instead of a MinGW bash prompt?"
	fi
	exit
fi

#########################################################
# Prepare command line input
ArgDebug=`echo "$1" | tr a-z A-Z`
DebugSymbols=""
DebugCMake="-DCMAKE_BUILD_TYPE=Release"
DebugMode=0

if [ ! -n "$1" ]
then
	Usage
	exit
else
	if [ "DEBUG" == "$ArgDebug" ]
	then
		echo "Compiling with Debug symbols"
		DebugMode=1
		DebugSymbols="-g"
		DebugCMake="-DCMAKE_BUILD_TYPE=Debug"
	elif [ "RELEASE" == "$ArgDebug" ]
	then
		echo "Compiling with full optimizations for Release"
	else
		Usage
		exit
	fi
fi

########################################################
# Check arg1 for thread count
ThreadCount=1
if [ -n "$2" ]
then
	if [[ $2 != [0-9]* ]]
	then
		echo "\"$2\" is not a valid ThreadCount"
		Usage
		exit
	else
		ThreadCount=$2
		echo "All compilations will be run in $ThreadCount Threads."
	fi
fi

########################################################
# Compilation variables
fPIC=""
if [ 1 -eq $Linux ]
then
	fPIC="-fPIC"
fi

LDfPIC=""
if [ 1 -eq $Linux ]
then
	LDfPIC="LDFLAGS=-fPIC"
fi

########################################################
# SDL compilation
echo "Preparing to Compile SDL"
cd libincludes/common/sdlsrc/
cp -a SDL SDLbuild
cd SDLbuild

if [ 0 -eq $MinGW32 ]		# Do not run autogen.sh when using MinGW
then
	./autogen.sh
fi

./configure $LDfPIC CFLAGS="-O2 $DebugSymbols $fPIC"
make -j$ThreadCount

cd ../..
pwd

if [ 1 -eq $Linux ]
then
	########################################################
	# Prepare Ogre Library
	cd ogresrc/ogre
	cmake -G"CodeBlocks - Unix Makefiles" $DebugCMake -DOGRE_STATIC=false
	make -j$ThreadCount OgreMain
	make -j$ThreadCount RenderSystem_GL
	make -j$ThreadCount Plugin_CgProgramManager
	make -j$ThreadCount Plugin_ParticleFX
fi



