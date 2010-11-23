The Physgame engine uses cmake to configure it's builds. It is pretty easy to use, the first thing you need to do to build the physgame engine is to make sure you have all the required software on your system. All systems will require the following software:
	A Compiler (We used GCC 3.5 and 4.4 and MinGW 4.4, but most compilers should work)
	Cmake version 2.8 or newer

For dependencies, which the Physgame engine has plenty, you will need to install a few extra packages or take a few extra steps to build.
	For Windows, if you don't mind using 32 bit binaries, you can just use the defaults that we have included in this repository.
		Ticpp/TinyMXL and cAudio will be compiled with the engine whenever you compile.
		Before you run cmake for the physgame engine you need to compile bullet from source
			The source is located in physgameengine/libincludes/common/bulletsrc/bulletsvnsrc
			It is configured to use cmake and should leave it's static libraries in physgameengine/data/windows/bullet
			You should be able to run cmake from that directory and then use your build system of choice to compile it.
		
	For Linux, 
		the following development libraries or the equivalent need to be installed:
			libboost-thread-dev libsdl1.2-dev libxaw7-dev zlib1g-dev libfreeimage-dev libzzip-dev libogg-dev
			libwxgtk2.8-dev libxrandr-dev libopenal-dev libvorbis-dev libfreetype6-dev libvorbis-dev
		Ogre and Bullet need to be compiled.
			They are located in the following directories
				physgameengine/libincludes/common/bulletsrc/bulletsvnsrc
				physgameengine/libincludes/linux/ogresrc/ogre
			They are configured to drop their binaries off in the following directories
				physgameengine/data/linux/bullet
				physgameengine/data/linux/ogre
			If they don't wind up there for any reason, or you want to use your own, you can copy or symlink the version you do want to use to these directories.



	For Mac OSX


After you have taken any platform specif build steps(such as building ogre or bullet)
