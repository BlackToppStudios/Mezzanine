	
	***** Physgame Engine Build Procedures *****

	*** Tools You Need ***
	The Physgame engine uses cmake to configure it's builds. It is pretty easy to use, the first thing you need to do to build the physgame engine is to make sure you have all the required software on your system. All systems will require the following software:
		* A Compiler (We used GCC 3.5 and 4.4 and MinGW 4.4,)
		* Cmake version 2.8 or newer

	There are a few gcc specific flags we used in our codeblocks configuration. Every use of AddDefinition used the "-D" switch ms visual studio and its's c++ compiler use"/d" (I think) so this would need to be changed, at a minumum to get this to build in visual studio.

	*** 3rd Party libraries ***
	For dependencies, which the Physgame engine has plenty, you may need to install a few extra packages or take a few extra steps to build. If you do not want to use the version of Ogre3d or SDL provided (or any library for that matter) you can remove our source directories and put you own in their place or you can put your compiled binaries in the appropriate "physgameengine/data" and the corresponding header files in the matching place in the "physgameengine/libincludes/" folder. 

	** For Windows **
		Bullet Ticpp/TinyMXL, Oggvorbis and cAudio will be compiled with the engine whenever you compile. No extra steps should be required. All dependencies are include in the source repository.

	** For MasOS X **
		To be determined

	** For Linux **
		The following development libraries or the equivalent need to be installed:
			libboost-thread-dev libsdl1.2-dev libxaw7-dev zlib1g-dev libfreeimage-dev libzzip-dev libogg-dev
			libwxgtk2.8-dev libxrandr-dev libopenal-dev libvorbis-dev libfreetype6-dev libsdl-dev 
		These are the names on Ubuntu. Similar libraries exist on most popular linux distros. After those are installed, Ogre3d and SDL need to be compiled. These are both include in the source distribution, and have a small script to make compiling them easier. There is a script called "BuildLinuxLibs" that can be run with "./BuildLinuxLibs" from the directory "physgame/physgameengine/libincludes/linux/". This script simply goes to the directories with the source and compiles the libraries using the supplied tools one library at a time.
		To improve performance of this simple script, you may want to edit it to change every line that says "make" to say "make -j8" where 8 is the number of build processes you want to run. The -j8 says to run make in 8 processes, put your amount of CPUs here, so on a quadcore use (cmake -G"Unix Makefiles" && make -j4)
		
	*** Building Physgame Engine ***
	After you have taken any platform specific build steps, you can use cmake to create your build files of choice for you platform. Which you will then use to compile the software. On windows currently there are a few items that will prevent this from working with any compiler other than mingw(or another gcc based compiler). These are list above in the "Tools You Need" section
	You should be able to use cmake-gui, ccmake or cmake to prepare a set of build files. Here we are building unix make files on a linux machine, then compiling them with make.	The only commands actually run here are (cmake -G"Unix Makefiles") and (make -j8), Together these will create a full copy of the physgame engine and all required files and put it in "physgameengine/bin".

	Sample build done on a sample linux machine:
		dev@dev-laptop:~/physgame$ cmake -G"Unix Makefiles"
		-- The C compiler identification is GNU
		-- The CXX compiler identification is GNU
		[SNIP]
		physgame - End
		-- Configuring done
		-- Generating done
		-- Build files have been written to: /home/dev/physgame
		dev@dev-laptop:~/physgame$ make -j8
		Scanning dependencies of target LinearMath
		Scanning dependencies of target ticpp
		Scanning dependencies of target BulletSoftBodySolvers_CPU
		[SNIP]
		[100%] Building CXX object physgameengine/CMakeFiles/physgame.dir/src/xmltext.cpp.o
		Linking CXX shared library bin/libphysgame.so
		Preparing File: /home/dev/physgame/physgameengine/bin/data/common/Ogre-lgpl-3.0.txt
		Preparing File: /home/dev/physgame/physgameengine/bin/data/linux/plugins.cfg
		Preparing File: /home/dev/physgame/physgameengine/bin/data/linux/settings.cfg
		Preparing File: /home/dev/physgame/physgameengine/bin/data/linux/ogre/RenderSystem_GL.so
		Preparing File: /home/dev/physgame/physgameengine/bin/libOgreMain.so.1.8.0
		Preparing Symlink: /home/dev/physgame/physgameengine/bin/libOgreMain.so.1.8.0
		[100%] Built target physgame
		dev@dev-laptop:~/physgame$


