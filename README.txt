The Physgame engine uses cmake to configure it's builds. It is pretty easy to use, the first thing you need to do to build the physgame engine is to make sure you have all the required software on your system. All systems will require the following software:
	A Compiler (We used GCC 3.5 and 4.4 and MinGW 4.4, but most compilers should work)
	Cmake version 2.8 or newer

For dependencies, which the Physgame engine has plenty, you will need to install a few extra packages or take a few extra steps to build.
	For Windows,
		Bullet Ticpp/TinyMXL, Oggvorbis and cAudio will be compiled with the engine whenever you compile.
	For Linux, 
		the following development libraries or the equivalent need to be installed:
			libboost-thread-dev libsdl1.2-dev libxaw7-dev zlib1g-dev libfreeimage-dev libzzip-dev libogg-dev
			libwxgtk2.8-dev libxrandr-dev libopenal-dev libvorbis-dev libfreetype6-dev 
		Ogre needs to be compiled.
			It is located in physgameengine/libincludes/linux/ogresrc/ogre
			the ogre build distributed with physgame is configured to copy the minimum requred binaries from where ogre builds them and put them in physgameengine/data/linux/ogre
			If they don't wind up there for any reason, or you want to use your own, you can copy or symlink the version you do want to use to these directories.
			An easy way to compile ogre is to use (cmake -G"Unix Makefiles" && make -j8). 				The -j8 says to run make in 8 processes, put your amount of CPUs here so on a quadcore use (cmake -G"Unix Makefiles" && make -j4)
				Here is a sample ogre build from a user named "dev" on a machine named "dev-laptop" who has their copy of the git repository in their home folder
					dev@dev-laptop:~/physgame$ cd physgameengine/libincludes/linux/ogresrc/ogre/
					dev@dev-laptop:~/physgame/physgameengine/libincludes/linux/ogresrc/ogre$ cmake -G"Unix Makefiles"
					-- The C compiler identification is GNU
					-- The CXX compiler identification is GNU
					-- Check for working C compiler: /usr/bin/gcc
					[SNIP]
					-- Configuring done
					-- Generating done
					-- Build files have been written to: /home/dev/physgame/physgameengine/libincludes/linux/ogresrc/ogre
					dev@dev-laptop:~/physgame/physgameengine/libincludes/linux/ogresrc/ogre$ make -j8
					Scanning dependencies of target OgreMain
					[  0%] [  0%] [  1%] [  1%] [  1%] [  1%] Building CXX object OgreMain/CMakeFiles/OgreMain.dir/src/OgreAlignedAllocator.cpp.o
					Building CXX object OgreMain/CMakeFiles/OgreMain.dir/src/OgreAnimable.cpp.o
					Building CXX object OgreMain/CMakeFiles/OgreMain.dir/src/OgreAnimation.cpp.o
					[SNIP]
					[ 98%] Building CXX object RenderSystems/GL/CMakeFiles/RenderSystem_GL.dir/src/GLX/OgreGLXContext.cpp.o
					[100%] Building CXX object RenderSystems/GL/CMakeFiles/RenderSystem_GL.dir/src/GLX/OgreGLXGLSupport.cpp.o
					[100%] Building CXX object RenderSystems/GL/CMakeFiles/RenderSystem_GL.dir/src/GLX/OgreGLXRenderTexture.cpp.o
					[100%] Building CXX object RenderSystems/GL/CMakeFiles/RenderSystem_GL.dir/src/GLX/OgreGLXWindow.cpp.o
					Linking CXX shared library ../../lib/RenderSystem_GL.so
					[100%] Built target RenderSystem_GL
					dev@dev-laptop:~/physgame/physgameengine/libincludes/linux/ogresrc/ogre$ cd ~/physgame
					dev@dev-laptop:~/physgame$ 
	For Mac OSX


After you have taken any platform specific build steps (such as building ogre or bullet), you can cmake to create your build files of choice for you platform. Which you will then use to compile the software.

	Here is a sample build done on a sample linux machine:
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
	The only commands actually run here are (cmake -G"Unix Makefiles") and (make -j8) Together these will create a full copy of the physgame engine and all required files and put it in physgameengine/bin
