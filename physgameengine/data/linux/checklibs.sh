#!/bin/bash
#© Copyright 2010 BlackTopp Studios Inc.
# This file is part of The PhysGame Engine and is used during the build process to copy files and build libraries.
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
# The original authors have included a copy of the license specified above in the
#   'Docs' folder. See 'gpl.txt'
#
# We welcome the use of The PhysGame anyone, including companies who wish to
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


#This should only be run from the root of the project directory (not the root of the workspace),
# and it makes a great deal of assumptions about the nature of the build process.

#Check for bullet library, build if missing, and moved built library to the appropriate place
if [ -f "data/linux/bullet/libBullet.a" ]
then
	echo "Bullet Library present."
else
	echo "Bullet Missing in data directory, Checking for compiled lib in bulletsrc"
	if [ -f "libincludes/common/bulletsrc/libBullet.a" ]
	then
		echo "Found, copying to correct location in project"
		mkdir -p data/linux/bullet/
		cp libincludes/common/bulletsrc/libBullet.a data/linux/bullet/
	else
		echo "Checking to see if we can compile Bullet"
		if [ 0 -eq `cat ~/.codeblocks/default.conf |grep "SINGLE_INSTANCE bool=\"0\"" -c` ]
		then
			echo "Bullet Cannot be automatically compiled, You should use codeblocks to build BulletWorkspace.workspace"
			echo "libincludes/common/bulletsrc/BulletWorkspace.cbp"
			echo "Alternatively you can disable the single instance feature of Code::blocks in the environment settings"
			exit 1
		else
			echo "Bullet Missing and can be built, Beginning Bullet Build."
			cd libincludes/common/bulletsrc/
			codeblocks --build --target=Release BulletWorkspace.workspace
			cd ../../..
			cp libincludes/common/bulletsrc/libBullet.a data/linux/bullet/
		fi
	fi
fi

#Check for ticpp library, build if missing, and moved built library to the appropriate place
if [ -f "data/linux/tinyxmlpp/libticpp.a" ]
then
	echo "TinyXMLpp Library present."
else
	echo "TinyXMLpp Missing in data directory, Checking for compiled lib in tinyxmlppsrc"
	if [ -f "libincludes/common/tinyxmlppsrc/libticpp.a" ]
	then
		echo "Found, copying to correct location in project"
		mkdir -p data/linux/tinyxmlpp/
		cp libincludes/common/tinyxmlppsrc/libticpp.a data/linux/tinyxmlpp/
	else
		echo "Checking to see if we can compile TinyXMLpp"
		if [ 0 -eq `cat ~/.codeblocks/default.conf |grep "SINGLE_INSTANCE bool=\"0\"" -c` ]
		then
			echo "TinyXMLpp Cannot be automatically compiled, You should use codeblocks to build ticpp.workspace"
			echo "libincludes/common/tinyxmlppsrc/ticpp.workspace"
			echo "Alternatively you can disable the single instance feature of Code::blocks in the environment settings"
			exit 1
		else
			echo "TinyXMLpp Missing and can be built, Beginning TinyXMLpp Build."
			cd libincludes/common/tinyxmlppsrc/
			codeblocks --build --target=Release ticpp.workspace
			cd ../../..
			cp libincludes/common/tinyxmlppsrc/libticpp.a data/linux/tinyxmlpp/
		fi
	fi
fi

#Check for cAudio library, build if missing, and moved built library to the appropriate place
if [ -f "data/linux/caudio/libcAudio.so" ]
then
	echo "cAudio Library present."
else
	echo "cAudio Missing in data directory, Checking for compiled lib in caudiosrc"
	if [ -f "libincludes/common/caudiosrc/Source/libcAudio.so" ]
	then
		echo "Found, copying to correct location in project"
		mkdir -p data/linux/caudio/
		cp libincludes/common/caudiosrc/Source/libcAudio* data/linux/caudio/
		cd data/linux/caudio/
		ln -s libcAudio* libcAudio.so
		cd ../../..
	else

		echo "cAudio Missing, Beginning cAudio Build."
		cd libincludes/common/caudiosrc/Source/
		make
		cd ../../../..
		mkdir -p data/linux/caudio/
		cp libincludes/common/caudiosrc/Source/libcAudio* data/linux/caudio/
		cd data/linux/caudio/
		ln -s libcAudio* libcAudio.so
		cd ../../..
	fi
fi

#Check for Ogre3d library, build if missing, and moved built library to the appropriate place
if [ -f "data/linux/ogre/libOgreMain.so" ]
then
	echo Ogre Library present.
else
	if [ -f "lib/RenderSystem_GL.so" ]
	then
		echo "Ogre Compile, but not copied into working directory"
	else
		echo "Ogre Missing, Beginning Ogre Build"
		cd libincludes/linux/ogresrc/
		if [ -f "`which cmake`" ]
	        then
        		echo "Found cmake. Building"
			cmake .
			make
		else
			echo "Cmake missing build will fail. Please install cmake."
        	fi
		
	fi
	cp -a lib/libOgreMain.* ../../../data/linux/ogre/
	cp lib/RenderSystem_GL.so ../../../data/linux/ogre/
	cd ../../..
fi

