The Physgame engine uses cmake to configure it's builds. It is pretty easy to use, the first thing you need to do to build the physgame engine is to make sure you have all the required software on your system. All systems will require the following software:
	A Compiler (We used GCC 3.5 and 4.4 and MinGW 4.4, but most compilers should work)
	Cmake version 2.8 or newer

For dependencies, which the Physgame engine has plenty, you will need to install a few extra packages or take a few extra steps to build.
	For Windows, if you don't mind using 32 bit binaries, you can just use the defaults that we have included in this repository.
		Ticpp/TinyMXL and cAudio will be compiled with the engine whenever you compile.
		
	For Linux, 
		the following development libraries or the equivalent need to be installed:
			libboost-thread-dev libsdl1.2-dev libxaw7-dev zlib1g-dev libfreeimage-dev libzzip-dev libogg-dev
			libwxgtk2.8-dev libxrandr-dev libopenal-dev libvorbis-dev libfreetype6-dev libvorbis-dev
		Ogre needs to be compiled.
			It is located in physgameengine/libincludes/linux/ogresrc/ogre
			It is configured to drop binaries off in physgameengine/data/linux/ogre
			If they don't wind up there for any reason, or you want to use your own, you can copy or symlink the version you do want to use to these directories.
			An easy way to compile ogre is to use (cmake -G"Unix Makefiles" && make -j8).
				The -j8 says to run make in 8 processes, put your amount of CPUs here so on a quadcore use (cmake -G"Unix Makefiles" && make -j4)

	For Mac OSX


After you have taken any platform specific build steps (such as building ogre or bullet), you can cmake to create your build files of choice for you platform. Which you will then use to compile the software.
