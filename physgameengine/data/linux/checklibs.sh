#!/bin/bash

#Check for bullet library
	#requires automake freeglut3-dev
if [ -f "data/linux/bullet/libBullet.a" ]
then
	echo "Bullet Library present."
else
	echo "Bullet Missing in data direcectory, Checking for compiled lib in bulletsrc"
	if [ -f "libincludes/common/bulletsrc/libBullet.a" ]
	then
		echo "Found, copying to correct location in project"
		cp libincludes/common/bulletsrc/libBullet.a data/linux/bullet/
	else
		echo "Checking to see if we can compile Bullet"
		if [ 0 -eq `cat ~/.codeblocks/default.conf |grep "SINGLE_INSTANCE bool=\"0\"" -c` ]
		then
			echo "Bullet Cannot be automatically compiled, You should use codeblocks to build Bullet.cbp"
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


#This should only be run from the root of the prject directory.
if [ -f "data/linux/ogre/libOgreMain.so" ]
then
	echo Ogre Library present.
else
	if [ -f "lib/RenderSystem_GL.so" ]
	then
		echo Ogre Compile, but not copied into working directory
	else
		echo Ogre Missing, Beginning Ogre Build
		cd libincludes/linux/ogresrc/
		cmake .
		make
	fi
	cp -a lib/libOgreMain.* ../../../data/linux/ogre/
	cp lib/RenderSystem_GL.so ../../../data/linux/ogre/
	cd ../../..
fi	

