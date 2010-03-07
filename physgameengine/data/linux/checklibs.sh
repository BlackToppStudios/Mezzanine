#!/bin/bash

#Check for bullet library
	#requires automake freeglut3-dev
if [ -f "data/linux/bullet/libBullet.a" ]
then
	echo Bullet Library present.
else
	echo "Bullet Missing Checking for compiled lib in bulletsrc"
	if [ -f "libincludes/linux/bulletsrc/libBullet.a" ]
	then
		echo "Found, copying to correct location in project"
		cp libincludes/linux/bulletsrc/libBullet.a data/linux/bullet/
	else
		echo Bullet Missing, You should use codeblocks to build Bullet.cbp
		echo libincludes/linux/bulletsrc/BulletWorkspace.cbp
		exit 1
	fi
fi

#sudo apt-get install libfreetype6-dev libboost-date-time-dev libboost-thread-dev nvidia-cg-toolkit libfreeimage-dev zlib1g-dev libzzip-dev libois-dev libcppunit-dev doxygen libxt-dev libxaw7-dev libxxf86vm-dev libxrandr-dev libglu-dev cmake
#This should only be run from the root of the prject directory.
if [ -f "data/linux/ogre/libOgreMain.so" ]
then
	echo Ogre Library present.
else
	echo Ogre Missing, Beginning Ogre Build
	cd libincludes/linux/ogresrc/
	cmake .
	make
	cp -a lib/libOgreMain.* ../../../data/linux/ogre/
	cp lib/RenderSystem_GL.so ../../../data/linux/ogre/
	cd ../../..
fi	

	
