#!/bin/bash
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
#   We welcome the use of The Mezzanine anyone, including companies who wish to
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

# This relies on the following tar, uname, pwd, which, make(and the other autotools), and cmake

#########################################################
# Functions
function Usage {
	echo
	echo "Usage: `basename $0` Debug|Release [ThreadCount [OutputDir] ] "
	echo "   Release - If the word Debug is the first argument, then The software will be compiled with Debug Symbols"
	echo "   Debug - If the word Debug is the first argument, then The software will be compiled with Debug Symbols"
	echo "   ThreadCount is the number of threads to compile with. You usually want to enter you amount of CPU cores."
	echo "   OutputDir - Before compilation the source is copied to prevent interference with revision tools, where should these copies be placed"
}

#########################################################
# Gather Data from the platform
WorkingDir=`pwd`
SystemName=`uname`
MakeLocation=`which make`
Linux=0
MinGW32=0
MacOSX=0
DetectedPlatform=0				#eventually I would like to make this work from a git bash prompt.
PlatformDirName=""
BinaryRecievingDir="data/"

#########################################################
# Work with values gathered from the system so this script knows what is going on.
if [ "Darwin" = "$SystemName" ]
then
	echo "System Detected: Mac OS X"
	MacOSX=1
	DetectedPlatform=1
	PlatformDirName="macosx"
fi

if [ "Linux" = "$SystemName" ]
then
	echo "System Detected: Linux"
	Linux=1
	DetectedPlatform=1
	PlatformDirName="linux"
fi

if [ "MINGW32" = ${SystemName:0:7} ]
then
	echo "System Detected: 32bit MinGW"
	MinGW32=1
	DetectedPlatform=1
	PlatformDirName="windows"
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

BinaryRecievingDir="data/$PlatformDirName"

#########################################################
# Prepare command line input
ArgDebug=`echo "$1" | tr a-z A-Z`
DebugSymbols="-O2"
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
# Check arg2 for thread count
OutputDir="."
if [ -n "$3" ]
then
	if [ -d $3 ]
	then
		OutputDir=$3
	else
		echo "\"$3\" Does not exist or is not a directory, cannot proceed"
		Usage
		exit

	fi
fi
echo "All Output will be in \"$OutputDir\"."

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
SDLOutputDir="$OutputDir/SDLBuild"
SDLCompileDir="$OutputDir/SDLBuild/SDL/"
SDLRelOutputDir=".."
echo "Preparing SDL source Files in: \"$SDLOutputDir\""
cd $WorkingDir			#next line needs to be executed from the working dir or it could fail if the user entered a relative path
mkdir -p $SDLOutputDir
cd $SDLOutputDir
tar xf $WorkingDir/libincludes/common/sdlsrc/SDL.tar.gz
cd $WorkingDir
cd $SDLCompileDir		# could be relative, wo I switched back to first directory first

if [ 0 -eq $MinGW32 ]		# Do not run autogen.sh when using MinGW
then
	echo "Running ./autogen.sh and putting output in: $SDLOutputDir/Autogenlog.txt"
	./autogen.sh > $SDLRelOutputDir/Autogenlog.txt
fi

echo "Configuring SDL, putting log in: $SDLOutputDir/Configurelog.txt"
./configure $LDfPIC CFLAGS="$DebugSymbols $fPIC"  > $SDLRelOutputDir/Configurelog.txt

echo "Compiling SDL, putting logs in: $SDLOutputDir/Compilelog.txt"
make -j$ThreadCount > $SDLRelOutputDir/Compilelog.txt

cd $WorkingDir
echo "Putting Compiled SDL binaries in: $WorkingDir/$BinaryRecievingDir/sdl/"
cp -a $SDLCompileDir/build/.libs/libSDL.a $WorkingDir/$BinaryRecievingDir/sdl/

if [ 1 -eq $MinGW32 ]		# Do not run autogen.sh when using MinGW
then
	cp -a $SDLCompileDir/build/.libs/libSDL.la $WorkingDir/$BinaryRecievingDir/sdl/
	cp -a $SDLCompileDir/build/libSDLmain.a $WorkingDir/$BinaryRecievingDir/sdl/
fi


########################################################
# Prepare Ogre Library
CMakeOutput="CodeBlocks - Unix Makefiles"
OgreDepsLocation=""
CmakeMinGWPATH=""
OriginalPATH=$PATH

OgreOutputDir="$OutputDir/OgreBuild"
OgreCompileDir="$OutputDir/OgreBuild/ogre"
OgreRelOutputDir=".."

if [ 1 -eq $MinGW32 ]
then
	CMakeOutput="CodeBlocks - MinGW Makefiles"
	OgreDepsLocation="-DOGRE_DEPENDENCIES_DIR=../ogredepsbuild"
	exit	#this needs to stay until the ogre build is somehow in the unified build system
fi

echo "Preparing Ogre source Files in: \"$OgreOutputDir\""
cd $WorkingDir
mkdir -p $OgreOutputDir
#cp -a libincludes/common/ogresrc/ogre/ $OgreOutputDir/
cd $OgreOutputDir
tar xf $WorkingDir/libincludes/common/ogresrc/ogre.tar.gz
cd $WorkingDir
cd $OgreCompileDir

echo "Configuring Ogre3d, putting output in: $OgreOutputDir/Configurelog.txt"
cmake -G"$CMakeOutput" $DebugCMake -DOGRE_STATIC=false $OgreDepsLocation > ../Configurelog.txt

echo "Compiling Ogre3d, OgreMain, putting output in: $OgreOutputDir/Compilelog-OgreMain.txt"
make -j$ThreadCount OgreMain > $OgreRelOutputDir/Compilelog-OgreMain.txt

echo "Compiling Ogre3d, RenderSystem_GL, putting output in: $OgreOutputDir/Compilelog-RenderSystem_GL.txt"
make -j$ThreadCount RenderSystem_GL > $OgreRelOutputDir/Compilelog-RenderSystem_GL.txt

echo "Compiling Ogre3d, Plugin_CgProgramManager, putting output in: $OgreOutputDir/Compilelog-Plugin_CgProgramManager.txt"
make -j$ThreadCount Plugin_CgProgramManager > $OgreRelOutputDir/Compilelog-Plugin_CgProgramManager.txt

echo "Compiling Ogre3d, Plugin_ParticleFX, putting output in: $OgreOutputDir/Compilelog-Plugin_ParticleFX.txt"
make -j$ThreadCount Plugin_ParticleFX > $OgreRelOutputDir/Compilelog-Plugin_ParticleFX.txt

cd $WorkingDir
echo "Putting Compiled Ogre binaries in: $WorkingDir/$BinaryRecievingDir/ogre/"
cp -a $OgreCompileDir/lib/* $WorkingDir/$BinaryRecievingDir/ogre/

exit

#if [ 1 -eq $MinGW32 ]		#MinGW does not work well with a cluttered $PATH
#then
#	echo "Preparing to Compile Ogre3 dependencies"
#	cp -a ogredeps ogredepsbuild
#	cd ogredepsbuild
#	CmakeMinGWPATH=`which gcc`
#	if [ -a /c/Program\ Files/CMake\ 2.8/bin/cmake.exe ]
#	then
#		CmakeMinGWPATH="/c/Program Files/CMake 2.8/bin/cmake.exe"
#	else
#		CmakeMinGWPATH="`which cmake`"	
#	fi
#	
#	if [ "" = $CmakeMinGWPATH ]
#	then
#		echo "Could not find Cmake 2.8, is it installed? Is it in the system PATH?"
#		exit
#	fi
#
#	CmakeMinGWPATH="$CmakeMinGWPATH:`which gcc`"
#	CmakeMinGWPATH=${CmakeMinGWPATH/cmake.exe/}
#	CmakeMinGWPATH=${CmakeMinGWPATH/cmake/}
#	CmakeMinGWPATH=${CmakeMinGWPATH/gcc.exe/}
#	CmakeMinGWPATH=${CmakeMinGWPATH/gcc/}
#	PATH=$CmakeMinGWPATH
#	echo "Configuring Ogre3d dependencies"
#	cmake -G"$CMakeOutput" $DebugCMake
#	echo "Compiling Ogre3d dependencies"
#	PATH=$OriginalPATH
#	make
#	cd ..
#fi

