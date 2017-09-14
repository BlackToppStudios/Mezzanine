// © Copyright 2010 - 2017 BlackTopp Studios Inc.
/* This file is part of The Mezzanine Engine.

    The Mezzanine Engine is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The Mezzanine Engine is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with The Mezzanine Engine.  If not, see <http://www.gnu.org/licenses/>.
*/
/* The original authors have included a copy of the license specified above in the
   'Docs' folder. See 'gpl.txt'
*/
/* We welcome the use of the Mezzanine engine to anyone, including companies who wish to
   Build professional software and charge for their product.

   However there are some practical restrictions, so if your project involves
   any of the following you should contact us and we will try to work something
   out:
    - DRM or Copy Protection of any kind(except Copyrights)
    - Software Patents You Do Not Wish to Freely License
    - Any Kind of Linking to Non-GPL licensed Works
    - Are Currently In Violation of Another Copyright Holder's GPL License
    - If You want to change our code and not add a few hundred MB of stuff to
        your distribution

   These and other limitations could cause serious legal problems if you ignore
   them, so it is best to simply contact us or the Free Software Foundation, if
   you have any questions.

   Joseph Toppi - toppij@gmail.com
   John Blackwood - makoenergy02@gmail.com
*/

#ifndef building_the_engine_h
#define building_the_engine_h

/**
    @page building_the_engine Building the %Mezzanine Engine
    @section building_the_engine_tools Build Tools
    First thing you will want to do is get the right tools for building and working with the
    @ref mezzanine_engine. We have pages for each supported platforms tool set, pick the appropriate
    set for your uses:
     - @ref dev_tools_windows
     - @ref dev_tools_linux
     - @ref dev_tools_mac

    And be sure you have any optional dependencies installed that you want or need

     - @ref installing_engine_deps_windows
     - @ref installing_engine_deps_linux
     - @ref installing_engine_deps_macosx

    If you are planning on developing software too it may be a good idea to install a copy of
    @htmlonly<a href="http://www.codeblocks.org/downloads/26">@endhtmlonly
    Code::blocks IDE
    @htmlonly</a>@endhtmlonly
    ,
    @htmlonly<a href="http://www.qt.io/ide/">@endhtmlonly
    Qt Creator
    @htmlonly</a>@endhtmlonly
    or another IDE (Integrated Development Environment). You can use any IDE you like, but if you
    want to ask us IDE questions we will only have answers for the ones we have used. On Mac OS x
    you will probably want to use xcode.

    @section building_the_engine_get_source Get the Source Code
    From a command prompt you can run a few commands to get a copy of the source on your
    workstation. If you are using windows you could use the
    @htmlonly<a href="https://git-scm.com/">@endhtmlonly
    git bash prompt
    @htmlonly</a>@endhtmlonly
    , and on Mac OS or Linux the normal terminal or bash prompts should work. If you are using some
    @htmlonly<a href="https://desktop.github.com/">@endhtmlonly
    graphical tool
    @htmlonly</a>@endhtmlonly
    the steps are the same, but you will have to figure out how to make the software you use perform
    them.
    @n @n
    These commands if copied verbatim on windows will navigate to your C: Drive and create a folder
    called “Mezzanine” with the source code in it. The actual commands you will type are in
    italics/highlighted. You can change “c:” to a directory of your choice, such as your desktop or
    documents folder. You will have to change it if you are not using windows. One thing to note is
    that one of the linker’s used, windres.exe, does not like spaces in the paths that are passed to
    it. If you place the Mezzanine repo in a path that has a space in one of the folder names your
    compile will fail. Avoid paths with spaces in directory names. I.E., a folder named
    “Documents/Mezzanine” will be ok, but “My Documents/Mezzanine” will fail during compilation.
    @code
    dev@DEV-PC ~
    $ cd c:
    dev@DEV-PC /c
    $ git clone git://github.com/BlackToppStudios/Mezzanine.git
    Cloning into Mezzanine...
    remote: Counting objects: 117277, done.
    remote: Compressing objects: 100% (51597/51597), done.
    remote: Total 117277 (delta 65098), reused 114360 (delta 62444)
    Receiving objects: 100% (117277/117277), 572.91 MiB | 1.60 MiB/s, done.
    Resolving deltas: 100% (65098/65098), done.
    @endcode

    @n
    Here is an example of similar commands on Linux:

    @code
    sqeaky@Tachyon:~$ mkdir projects
    sqeaky@Tachyon:~$ cd projects/
    sqeaky@Tachyon:~/projects$ git clone git://github.com/BlackToppStudios/Mezzanine.git
    Cloning into 'Mezzanine'...
    remote: Counting objects: 117277, done.
    remote: Compressing objects: 100% (51597/51597), done.
    remote: Total 117277 (delta 65102), reused 114356 (delta 62444)
    Receiving objects: 100% (117277/117277), 572.90 MiB | 1.94 MiB/s, done.
    Resolving deltas: 100% (65102/65102), done.
    @endcode

    @section building_the_engine_git_urls Git URLs
    There are a few places and ways you can download the source code. The ones that provide
    “read only” access refer only to the ability to upload to changes. With any of these methods you
     can make as many changes as you like to the copy on your machine:

    - git://github.com/BlackToppStudios/Mezzanine.git – For read only access.
    - git@github.com:BlackToppStudios/Mezzanine.git – For read/write access (must as us to get
        write access).
    @n
    When using git “git://”, “https://” and “http://” URLs will work with no extra configuration but
    generally will be read-only. To use a URL that starts with “git@", an SSH URL, you will need an
    account with the people maintaining the URL (like
    @htmlonly<a href="https://github.com/join?source=header">@endhtmlonly
    github.com
    @htmlonly</a>@endhtmlonly
    ), setup an RSA-Keypair, and upload your public key to them. See the
    @htmlonly<a href="https://help.github.com/articles/set-up-git/">@endhtmlonly
    Github setup page
    @htmlonly</a>@endhtmlonly
    setup page for more info, and be certain that the RSA fingerprint matches if it asks you.
    Additionally, to upload changes a BTS administrator will have to give you permissions on Github
    to contribute.

    @n
    Once you have cloned the Mezzanine repository (or physgame repository if from the older
    Gitorious backup) the URL setting is stored in “.git/config” and it can be changed with any text
    editor. The “.git” folder is hidden by default.

    @n
    You can also download a zip archive of the source from
    https://github.com/BlackToppStudios/Mezzanine/. You may have to adjust certain steps if this is
    the case.

    @subsection building_the_engine_git_urls_gitorious Gitorious is dead
    @htmlonly<a href="https://gitorious.org/">@endhtmlonly
    Gitorious
    @htmlonly</a>@endhtmlonly
    , the original hosting site for BTS source code seemed to be non longer functional at the time
    of this writing. For historic interest here are the old links to those repositories.
    - git://gitorious.org/physgame/physgame.git – Read only access to the backup
    - git@gitorious.org:physgame/physgame.git – Read/write access to the backup
    - http://git.gitorious.org/physgame/physgame.git – Read Only

    @section building_the_engine_deps Dependencies
    After you get the source code there may be some steps you need to take to get any software the
    @ref mezzanine_engine depends on installed and configured. Please take a look at any appropriate
    steps for your platform:

    - @ref installing_engine_deps_windows
    - @ref installing_engine_deps_linux
    - @ref installing_engine_deps_macosx

    @subsection building_the_engine_deps_building Building the Shipped Dependencies
    In the past this was a significantly complex procedure, now almost all of our dependencies have
    been integrated tightly. Ogre, SDL, Bullet3d and a few others are all built during the CMake
    build process.

    The Mezzanine source ships with a copy of SDL and Ogre3d (and a few other) libraries source code
    that are used internally by it. You won’t need to use these unless you are making advanced
    changes to the Mezzanine itself, but they will need to be compiled. This will be handled
    automatically as part of the main source build process.

    @section building_the_engine_using_cmake Using CMake
    Cmake will create a set of project files that will tell the compiler how to compile the source
    you just downloaded. The steps listed here assume that you have the source files in
    “c:\\Mezzanine\\” and want to have your compiled binaries in “c:\\Mezzbuild\\”. There are a
    couple of way to do this. You can use the graphical Cmake program or the command line tool.

    @subsection building_the_engine_cmake_options CMake Options
    In the graphical version these appear as series of checkboxes, in the command line version you
    can use the “-D” flag to set these:

    - Mezz_Debug – Enables Debuging symbols in the Mezzanine library and enable extra logging
        message. Defaults to false.
    - Mezz_Doc – Enable Bulding the doxygen documentation
    - Mezz_Lua51 – Enable/Disable the whole Lua system. Defaults to true/enabled.
    - Mezz_Network – Enable Networking support. Defaults to true.
    - Mezz_BuildRenderSystemDirectX9 – This controls support for DirectX 9 a common RenderSystem on
        all microsoft operating systems. This is experimental.
    - Mezz_BuildRenderSystemDirectX11 – Controls support DirectX 11 a more advanced RenderSystem
        only available on newer microsoft operating systems. This is experimental.
    - Mezz_BuildRenderSystemOpenGL – This is default Render system on Windows, Linux and Mac OS X.
    - Mezz_BuildRenderSystemOpenGLES – Experimental, this is the render that will be used on some
        mobiles platforms.
    - Mezz_BuildRenderSystemOpenGLES2 – Experimental, this is the render that will be used on some
        mobiles platforms.

    @subsubsection building_the_engine_advanced_cmake_options Advanced CMake Options
    Here are some of the advanced features:

    - Mezz_Profile – Enable compiler level profiling support. Defaults to false.
    - Mezz_ForceBuildType – Forces The CMake build type to match the Mezz_Debug flag, useful if you
        to not have debug symbols but want the extra logging Mezz_Debug enables.
    - Mezz_ThrowOnSingletonFetch – Controls whether getting a singleton when it is invalid throws or
        returns null.
    - Mezz_UseOgreVersion – This must be set to match the Ogre3d version if you want to use one not
        integrated you will need to adjust this.

    @subsubsection building_the_engine_cmake_option_caveats CMake Option Caveats
    Currently the Mezz_Profile cmake flag does not work on windows, and will give errors while
    trying to compile it. Avoid enabling it on windows.
    @n
    Mezz_XML Has been deprecated and removed. The XML system is central to many other features, and
    maintaining this option takes too much time for the value it adds.

    @subsection building_the_engine_cmake_graphical Graphical Tool
    First thing to do is open Cmake. The default installation in windows should have put a Cmake
    icon on you desktop, and another in your start menu. In Ubuntu (and most other modern Linux
    distributions) when you install “cmake-gui” an entry will be made in the KDE or Gnome menu or
    Unity panel. Use that icon or menu entry to start it or use "cmake-gui" from a terminal.
    @n
    The first box at the top of the screen is called “Where is the source code:”. Select the folder
    using the “Browse Source” button, or manully type in “c:/Mezzanine”. The next box asks “Where
    to build binaries:” enter “c:/Mezzbuild”. After that click the configure button in the lower
    left corner. This will prompt you for what kind of project files to make. If you just want to
    build the engine and not work with it’s source directly select “Unix Makefiles” or “MinGW
    Makefiles”, if you do directly want to work with the source code select “Codeblocks – Unix
    Makefiles” or “Codeblocks – MinGW Makefiles”. When you click finish on this screen Cmake will
    have figured out most of the information it needs to make the files.
    @n
    The last information required before creating project files is any options you want to select.
    Read below about “The Options” then click “Configure” again after you have made your choice.
    Then click “Generate” to actually make the project files.

    @subsection building_the_engine_cmake_cmd Command Line Tool
    There are a few different ways to use CMake from the command line. Ideally this should be run
    from the directory the compiled binaries are desired in, this is called the build directory.
    Cmake should be passed the directory with the Mezzanine repository. On Linux, but otherwise
    similar to our previous example, you could:

    @code
    cmake ../Mezzanine/
    @endcode

    @n
    When run from windows it might look the same if run from a Git bash prompt, or it could like the
    following if run from a DOS prompt:

    @code
    cmake ..\\Mezzanine\\
    @endcode

    @subsubsection building_the_engine_cmake_generators CMake Generators
    There are many kinds of files CMake can output. The way you select these are by passing cmake a
    generator. Most commonly we will recommend Makefiles or Codeblocks project files. There are
    several other kinds of files available for creation, but they are beyond the scope of this
    document. The most common generators we use are:
    @n
    On Linux:

    - CodeBlocks - Unix Makefiles
    - CodeBlocks - Ninja
    - Unix Makefiles
    - Ninja

    @n
    On Windows:

    - Codeblocks – MinGW Makefiles
    - MinGW Makefiles

    @n
    These can be entered with the “-G” switch (also called an argument) for example:

    @code
    cmake ../Mezzanine/ -G"CodeBlocks - Unix Makefiles"
    @endcode

    @subsection building_the_engine_entering_option Entering options
    All of the options listed above can be entered with the “-D” switch. After the name of the name
    of the option you need to include either “:boolean=true” or “:boolean=false”, for example:

    @code
    cmake ../Mezzanine/ -G"CodeBlocks - Unix Makefiles" -DMezz_Debug:boolean=true
    @endcode
    Game Developers will probably want the default options, which leave Profiling support and
    debugging support off.

    If you read the output of cmake, it will tell you what is enabled or disabled in plain
    English. With messages being similar to

    @code
    -- MezzanineAndCatch - Disabling gcc profiling and unsetting MEZZPROFILE
    -- MezzanineAndCatch - Enabling Network subsystem, and setting MEZZNETWORK
    -- MezzanineAndCatch - Enabling Support for Lua Scripting Language, and setting MEZZLUA51
    -- MezzanineAndCatch - Disabling Support for Ruby Scripting Language, and unsetting MEZZRUBY
    -- MezzanineAndCatch - Disabling Swig execution each build.
    -- MezzanineAndCatch - Enabling EntreLua build.
    @endcode

    You might want to disable an option, or use multiple. Here is an example of enabling debug
    support and disabling DirectX 11.

    @code
    cmake ../Mezzanine/ -G"CodeBlocks - Unix Makefiles" -DMezz_Debug:boolean=true -DMezz_BuildRenderSystemDirectX11:boolean=false
    @endcode

    @subsection building_the_engine_common_cmake_issues Common Issues with CMake
    On windows, if CMake complains about finding “sh.exe” in the system path then you will need to
    edit the system path variable to remove “c:\\program files\\git\\bin” from your PATH. See above
    for instructions on Editing your PATH variable.

    @section building_the_engine_actually Compiling the Engine
    Once you have your project files or makefiles you can compile the engine. There are two way to
    do this. People interested in the engine code can use the Code::Blocks IDE to compile the engine
    (Engine Developers and some Game Developers). People who aren’t interested in engine code can
    use make or ninja from a command line (some Game Developers, people installing the engine and
    Package Maintainers)

    @n
    This can take anywhere from 10 seconds to a couple of hourse depending on the speed of your
    computer. Various netbooks have taken between 30 and 45 minutes for a fresh compile all build
    targets, and 20 seconds to 90 seconds for a partial compile of one build target that uses the
    Mezzanine. Our i7 machine took around two minutes for a full build and less than a second for
    a partial build.

    @subsection building_the_engine_using_codeblocks Build with Code::Blocks
    All you have to do is open Code::Blocks by double clicking on the “MezzanineAndCatch.cbp” icon
    in your build directory, selecting the Build Target you want to build, and finally clicking on
    the green gear to compile.

    @n
    To speed this up or better understand the process here is a little more information that might
    be useful.

    @subsection building_the_engine_using_codeblocks_cpus Code::Blocks and Multiple CPUs
    When Code::Blocks is already open you can go to the “Settings” menu and select “Compiler and
    Debugger”, which will open a screen with a “Build Options” tab (you may need to scroll left or
    right to see it), which will have the “Number of parallel builds” option. This should be set to
    on or around the amount of hardware threads (CPUs + hyperthreading cores) your computer
    supports. On our i7 it is set to 9 and on the netbooks it was set to two or three.

    @n
    In an IDE that uses make for the build process, you just add an argument in the menu that
    defines how make is called. In QT Creator, for example, you can configure it “-j9” for nine
    compilation threads, in the “Projects” Sidebar in the “Build Steps” section. You can use any
    amount of threads that you see fit, try numbers around the amount of “Cores” your CPU provides.

    @subsection building_the_engine_terminal Build with Command Line
    If you open a Git Bash prompt on a window, or terminal on Mac OS X or Linux, then cd to your
    build directory and enter the “make” command, you have started the engine compiling. (This only
    works if using GCC, MinGW or Clang compilers)

    @n
    This runs slightly faster than a compilation from Code::Blocks, but not by much. The easiest way
    to speed this up is to tell make to use multiple thread to perform the compilation. This can be
    done with the “-j” switch, and the number of threads you want to run.

    @n
    The following examples will run the compilition in 4 threads which would be suitable for a
    typical quad-core machine:

    @n On Linux or Mac OS X:
    @code
    make -j4
    @endcode

    @n On windows:
    @code
    mingw32-make -j4
    @endcode

    @n On any OS with ninja installed:
    @code
    ninja -j4
    @endcode

    @section building_the_engine_build_targets Build Targets
    There are many build targets to choose from, here is a brief synopsis of the interesting ones.

    - All – Builds Everything
    - Mezzanine – The engine and what it requires
    - Catch – Just the Catch game and the engine if the files present aren’t current
    - UnitTests – the Testing suite and the engine if the files present aren’t current
    - EngineDemo – A simple Engine Demo and the engine if the files present aren’t current
    - /fast – (Code::Blocks only) Builds whatever * is, but skips any checks of the engine
    - Anything else – A component of the engine

    @n In Code::Blocks you can just select these from the build menu, from the command prompt you
    can add these to the end the build command to build just one target. For example these will all
    build the EngineDemo and only what it requires (So it won't build Catch!):

    @n On Linux or Mac OS X:
    @code
    make -j4 EngineDemo
    @endcode

    @n On windows:
    @code
    mingw32-make -j4 EngineDemo
    @endcode

    @n On any OS with ninja installed:
    @code
    ninja -j4 EngineDemo
    @endcode

    @section building_the_engine_compilation_issues Compilation Issues
    The are a ton of waystis can fail, here are some.

    @subsection building_the_engine_issue_ogre_endian Ogre Little Endian issue
    If you get the following warning, there is a workaround. This seems to have stopped happening
    since the release of the 1.7 branch of cmake.

    @code
    libincludes/linux/ogresrc/include/OgreBuildSettings.h:19:1: warning: "OGRE_CONFIG_LITTLE_ENDIAN" redefined
    : warning: this is the location of the previous definition
    @endcode

    You could remark our one of those definitions like this one on line 19 of OgreBuildSettings.h.
    Double clicking on the warning message in the Messages Window which is at the bottom of
    Code::Blocks will open the file to the error by default.

    @code
    #define OGRE_CONFIG_LITTLE_ENDIAN
    All you need to do is change the line like this:
    //#define OGRE_CONFIG_LITTLE_ENDIAN
    @endcode

    Then any time you compile the engine you won’t get anymore warnings for these bugs in the Ogre
    build process.

    @subsection building_the_engine_issue_rendersystem_copy Rendersystem copy errors
    Sometimes it is possible for Ogre to skip building a rendersystem, but does not to let you know
    in a clear way. In general this happens if it have been tricked into compiling against a
    software open renderer. The Error looks something like:

    @code
    Preparing File: /home/sqeaky/Projects/MezzanineCurrent/Build/Mezzanine/bin/data/linux/ogre/RenderSystem_GL.so
    Error copying file (if different) from “/home/sqeaky/Projects/MezzanineCurrent/Mezzanine/Mezzanine/data/linux/ogre/RenderSystem_GL.so” to “/home/sqeaky/Projects/MezzanineCurrent/Build/Mezzanine/bin/data/linux/ogre/RenderSystem_GL.so”.
    @endcode

    This has only been experience on Ubuntu and installing the Mesa GL headers fixes it:
    @code
    sudo apt-get install libgles1-mesa-dev
    @endcode
*/

#endif
