#!/bin/bash

#Check for bullet library
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


if [ -f "data/linux/caudio/libcAudio.so" ]
then
	echo "cAudio Library present."
else
	caudiodir="caudio-`uname -m`"
	echo "cAudio Missing in data directory, Checking for compiled lib in caudiosrc"
	if [ -f "libincludes/common/caudiosrc/$caudiodir/Source/libcAudio.so" ]
	then
		echo "Found, copying to correct location in project"
		mkdir -p data/linux/caudio/
		cp libincludes/linux/caudiosrc/$caudiodir/Source/libcAudio* data/linux/caudio/
		cd data/linux/caudio/
		ln -s libcAudio* libcAudio.so
		cd ../../..
	else

		echo "cAudio Missing, Beginning cAudio Build."
		cd libincludes/linux/caudiosrc/
		ln -s caudio-`uname -m` caudio
		cd $caudiodir/Source/
		make
		cd ../../../../..
		mkdir -p data/linux/caudio/
		cp libincludes/linux/caudiosrc/$caudiodir/Source/libcAudio* data/linux/caudio/
		cd data/linux/caudio/
		ln -s libcAudio* libcAudio.so
		cd ../../..
	fi
fi


#This should only be run from the root of the project directory.
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

