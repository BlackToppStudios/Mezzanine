// © Copyright 2010 - 2015 BlackTopp Studios Inc.
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
#ifndef _dox_dev_tools_h
#define _dox_dev_tools_h


/**
    @page tech_docs Technical documents
    Walkthroughs and human level explanations of our tools, processes and anything else useful.
    @section dev_tools Development Tools
    @ref windows_dev_tools
    @n @n
    @ref linux_dev_tools
    @n @n
    @ref mac_dev_tools

*/

/**
    @page windows_dev_tools Windows Development Tools
    This is a walk-through that Sqeaky, MakoEnergy and HydroWolfy have made for so other people can
    use the same tools as them. If anyone cares to make Project files for Visual Studio, Eclipse or
    any other development platform please let us know your steps, and where possible please allow us
    to use a copy of the files.

    @section windows_dev_tools_core The Core Tools
    These are the main resources to get this working. You can most likely use different version of
    these tools and they will most likely work just fine. We recommend that you go to each of these
    sites and find the installation packages before continuing any further.

    - @htmlonly<a href="http://tdm-gcc.tdragon.net">@endhtmlonly
    TDM-MinGW64
    @htmlonly</a>@endhtmlonly
    — This is the compiler package using a newer version of GCC and enables the compilation of
    64-bit binaries with MinGW.  This is the compiler we recommend to use on Windows.

    - @htmlonly<a href="http://www.codeblocks.org/downloads/26">@endhtmlonly
    Code::blocks IDE
    @htmlonly</a>@endhtmlonly
    — This is the development environment we use. The version used at the time of this writing was
    12.11 (the version is the year and month of the release).  Do not get the Code::Blocks/MinGW
    download bundle offered on the Code::Blocks webpage.  They have an older version of MinGW which
    will not compile the engine.

    - @htmlonly<a href="http://code.google.com/p/msysgit/downloads/list">@endhtmlonly
    GIT
    @htmlonly</a>@endhtmlonly
    — We used the newest version of msysgit. This tool will allow you to get the most recent version
    of the Source Code and Stay in sync with everyone else. Primarily used with
    @htmlonly<a href="https://github.com/BlackToppStudios/Mezzanine">@endhtmlonly
    github.com
    @htmlonly</a>@endhtmlonly
    but can be to directly sync with other developers.

    - @htmlonly<a href="http://www.cmake.org/cmake/resources/software.html">@endhtmlonly
    CMake
    @htmlonly</a>@endhtmlonly
    — This is used to construct project or make files to instruct the compiler on how to do it’s
    job. We used 2.8.3, 2.8.6, 3.0.2 and 3.3.2 but any version newer than 2.8.0 should work. (There
    is a bug in some versions of 3.4), so maybe avoid that)

    @subsection windows_dev_tools_core_alternatives Dev Tool Alternatives

    - @htmlonly<a href="http://sourceforge.net/projects/mingw/files/Automated%20MinGW%20Installer/">
    @endhtmlonly
    MinGW32 from MSYS
    @htmlonly</a>@endhtmlonly
    — These are the compiler, debugger and other closely associated tools for building 32-bit
    binaries with MinGW. We have used mingw-get-inst-20110316 and mingw-get-inst-20120426 (the
    version is date), but newer versions should work too. If you have the installer download the
    latest version be ready for a long download.

    - @htmlonly<a href="http://www.qt.io/ide/">@endhtmlonly
    Qt Creator
    @htmlonly</a>@endhtmlonly
    — This flexible IDE can be used with either MinGW or Microsoft’s compiler.

    - @htmlonly<a href="http://www.microsoft.com/visualstudio/eng/downloads">@endhtmlonly
    Visual Studio
    @htmlonly</a>@endhtmlonly
    — This IDE should be usable. Cmake produces an odd project structure, Two extra projects are
    added “ALL_BUILD” and “ZERO_CHECK”. These can be ignored safely. Be sure to set your startup
    project from the default. Another oddity is that Visual Studio strongly wants to set whether a
    build has debug symbols immediately before compilation. To accomodate this “CMAKE_BUILD_TYPE”
    cannot be set with Visual Studio, this could affect automated build processes.

    - @htmlonly<a href="https://git-for-windows.github.io/">@endhtmlonly
    Git Gui
    @htmlonly</a>@endhtmlonly
    — This graphical tool can be used instead of the command line tool. It is basic and to the
    point. It can generate SSH keys too.

    @section windows_dev_tools_installing Installing
    Like most windows installers you can get most of the work done by double clicking on it then
    ‘next’ until it is done. But thereare some specific option and extra steps you should be aware
    of.

    @subsection windows_dev_tools_mingw_special_config MinGW32 Special configuration
    During the installation of MinGW be sure to include the C++ compiler, MSYS Basic System and the
    MinGW Developer toolkit. This will include tool like Make and a Bash shell will be used by other
    tools later and may be useful if you want to dig into the gritty details.

    @subsection windows_dev_tools_mingw_32_64_config MinGW32 and MinGW64 Special configuration
    For some reason on Windows this compilers doesn’t work well with any spaces in its installation
    path. The default install directory is “c:\\mingw” this works well most the time, but you can
    select any folder without spaces.
    @n @n
    For the compiler to work correctly the system needs to made aware of it. The system
    @htmlonly<a href="http://en.wikipedia.org/wiki/PATH_(variable)">@endhtmlonly
    PATH
    @htmlonly</a>@endhtmlonly
    is a listing of folders that Windows (and most other operating systems) will search for programs
    when it is told run a program without being told its location. You can add it to just your
    current user’s PATH if you do not want to interfere with other users or to the system wide
    PATH if you think there will be other Mezzanine developers using the system. Here is how you
    add the minGW tools to windows PATH variable:

    @subsection windows_dev_tools_mingw_xp_path Window XP PATH Setup
    1. Right-click “My Computer” or “Computer”, click “properties”
    2. Click “Advanced” Tab
    3. Click “Environment Variables” Button
    4. Find the “PATH” variable and add “;C:\\MinGW\\bin” to the end (do not erase what is already
    in there, it _will_ screw your computer)
    5. Click “OK” 3 times

    @subsection windows_dev_tools_mingw_vista_path Vista and Windows 7 PATH Setup
    1. Right-click “My Computer” or “Computer”, click “properties”
    2. Click “Advanced System Settings” Link
    3. If a UAC Prompt requests you permission to continue click “Continue”
    4. Click “Environment Variables” Button
    5. Find the “PATH” variable and add “;C:\\MinGW\\bin” to the end (do not erase what is already in
    there, it _will_ screw your computer)
    6. Click “OK” 3 times

    @section windows_dev_tools_ide_configuration IDE Special configuration
    Most development environments have some configuration that they require.

    @subsection windows_dev_tools_codeblocks32_config Code::Blocks – Using MinGW32
    If you chose to use the version of Code::Blocks that came with the compiler you will still have
    to follow the instructions for editing the system PATH as listed above. The following steps
    assume you have installed MinGW64 to “C:\\MinGW”.  If you have it installed elsewhere, adjust
    the paths provided below accordingly.
    @n @n
    Also, by default Code::Blocks looks for “make.exe” as it’s primary make program.  MinGW doesn’t
    call it’s make executable “make.exe” though.  So you will have to tell Code::Blocks to look for
    MinGW’s make executable.  To do this:
    @n @n
    1. At the menu across the top, click “Settings” and go to “Compiler…”.
    2. In the window that appears, ensure the “Selected Compiler” is set to “GNU GCC Compiler”.
    3. Just below the “Selected Compiler”, there should be a row of tabs.  Click on the “Toolchain
    Executables” tab.
    4. You should see a list of executables, and at the bottom of the list should have a box labeled
    “Make Program:”, with “make.exe” listed.  To the right of that click the “…” button.
    5. Navigate to the “mingw32-make.exe” executable.  This should be located in “C:\\MinGW\\bin”.
    6. Select the “mingw32-make.exe” executable and click “Open”.  Then click “Ok” to close the
    configuration window

    @subsection windows_dev_tools_codeblocks64_config Code::Blocks – Using MinGW64
    MinGW64 needs a bit more help when it comes with integration with Code::Blocks. Ensure the
    MinGW64 install location has been added to the system
    @htmlonly<a href="http://en.wikipedia.org/wiki/PATH_(variable)">@endhtmlonly
    PATH
    @htmlonly</a>@endhtmlonly
    before proceeding. The following steps assume you have installed MinGW64 to “C:\\MinGW”. If you
    have it installed elsewhere, adjust the paths provided below accordingly.
    @n
    1. At the menu across the top, click “Settings” and go to “Compiler…”.
    2. In the window that appears, ensure the “Selected Compiler” is set to “GNU GCC Compiler”.
    3. In the “Compiler Settings” tab, select the “Other options” sub-tab and then enter “-m64”.
    This will force 64-bit builds.  Entering “-m32” will force 32-bit builds.
    4. In the “Search directories” tab, select the “Compiler” sub-tab and enter the following 3
    directories:  “C:\\MinGW\\include”, “C:\\MinGW\\x86_64-w64-mingw32\\include”, and
    “C:\\MinGW\\lib\\gcc\\x86_64-w64-mingw32\\4.8.1\\include”.
    5. Still in the “Search directories” tab, select the “Linker” sub-tab and enter the following 2
    directories: “C:\\MinGW\\lib” and “C:\\MinGW\\x86_64-w64-mingw32\\lib”.
    6. In the “Toolchain executables” tab, ensure the “Compiler’s installation directory” is set
    properly (for example “C:\\MinGW”).
    7. In the “Toolchain executables” tab, most of the executables defined have to be changed. Use
    the quoted text below to set each executable appropriately:
    @verbatim
    C Compiler:  x86_64-w64-mingw32-gcc.exe
    C++ Compiler:  x86_64-w64-mingw32-g++.exe
    Linker for dynamic libs: x86_64-w64-mingw32-g++.exe
    Linker for static libs:  ar.exe
    Debugger:  GDB/CDB debugger : Default
    Resource compiler:  windres.exe
    Make Program:  mingw32-make.exe
    @endverbatim

    @subsection windows_dev_tools_qt_creator_config Qt Creator
    For the debugger to work with any compiler make sure that the “CMAKE_BUILD_TYPE” option in Cmake
    is set too “Debug”. Additionally, some versions of the microsoft compiler do not come with a
    debugger. Be sure to install a copy of the
    @htmlonly<a href="http://msdn.microsoft.com/en-us/windows/hardware/hh852363.aspx">@endhtmlonly
    windows software development kit
    @htmlonly</a>@endhtmlonly
    if you are using some version of
    @htmlonly<a href="http://www.microsoft.com/visualstudio/eng/downloads">@endhtmlonly
    Visual Studio
    @htmlonly</a>@endhtmlonly
    for the compiler.

    @subsection windows_dev_tools_visual_studio_creator_config Visual Studio
    In theory this should just work. In practice we have only shown that this can be used to compile
    the source. There hasn’t been thorough testing of game compilation, bug parity or build
    configuration options.

    @section windows_dev_tools_git_config Git Special configuration
    When installing you will be asked where you will want to use the git tools from. This again,
    works with the system PATH. Use the default settings. If you select an option that puts the msys
    git tools into the windows path, it will conflict with cmake. Git can go into your PATH, but its
    tools cannot.

    @subsection windows_dev_tools_git_push_config Git Push
    If you plan on making pushes to
    @htmlonly<a href="https://github.com/BlackToppStudios/Mezzanine">@endhtmlonly
    Github.com
    @htmlonly</a>@endhtmlonly
    you will need to configure an account and an ssh key with them. From the git bash command prompt
    you can type “ssh-keygen” to make a set of keys. Alternatively if you are using the
    @htmlonly<a href="https://git-for-windows.github.io/">@endhtmlonly
    Git Gui
    @htmlonly</a>@endhtmlonly
    installed with msysgit, with a repo opened you can go to the “Help” pulldown menu at the top of
    the window, then “Show SSH Key”. This will open a new small window that should have a “Generate
    Key” button in the top right corner, and a “Copy to Clipboard” button in the bottom left corner.
    @n @n
    You will need to copy your public sshkey into your Github profile. When creating your keypair,
    let it use the default location to save your keys. That way you won’t have to configure git
    with the locations. It will output the location of the file, and you can open it notepad/wordpad
    or any text editor and copy/paste into the webpage.
    @n @n
    During your first clone/pull of the repository you will want to verify the authenticity of the
    ssh servers RSA fingerprint, which can be found in
    @htmlonly<a href="https://help.github.com/articles/generating-ssh-keys">@endhtmlonly
    Github’s documentation
    @htmlonly</a>@endhtmlonly
    , as well as a more detailed explanation of the key creation process.

    @section windows_dev_tools_cmake_config CMake Special configuration
    During the Installation you will be asked about the system
    @htmlonly<a href="http://en.wikipedia.org/wiki/PATH_(variable)">@endhtmlonly
    PATH
    @htmlonly</a>@endhtmlonly
    . even though it is not strictly required, it can be useful to set CMake to be in the path for
    the current user (or all users if other will develop software). This will simplify the running
    of command line commands and enable you to copy/paste commands published by others. This is an
    option you will be presented during the installation.

    @section windows_dev_tools_other Other Tools
    Currently these tools aren’t required, but they are popular and you might find them useful.

    - @htmlonly<a href="http://notepad-plus-plus.org/">@endhtmlonly
    Notepad++
    @htmlonly</a>@endhtmlonly
    — A really good text editor with syntax highlighting.

    - @htmlonly<a href="https://tortoisegit.org/">@endhtmlonly
    TortioseGIT
    @htmlonly</a>@endhtmlonly
    — Make sure to grab the right version for your platform (32 vs 64 bit). A better user interface
    to Git, it makes staying up to data much easier.

    - @htmlonly<a href="http://winmerge.org/">@endhtmlonly
    WinMerge
    @htmlonly</a>@endhtmlonly
    — A tool for comparing to similar files.

    - Visual Studio
    — More info or even CMake configuration adjustments would be welcome and helpful

    - @htmlonly<a href="https://eclipse.org/downloads/">@endhtmlonly
    Eclipse
    @htmlonly</a>@endhtmlonly
    — Any special project files that would allow users of this IDE to work more easily with the
    engine would be appreciated.

    @section windows_dev_tools_next What’s next
    If you aren’t comfortable with your codings skills @ref cxx_training is a good idea. If you are
    comfortable with you coding skill and have the tools installed check out the
    @ref engine_dependencies, if you already have what you need from that you could take a look at
    what it takes for @ref building_the_engine.


*/

/**
    @page linux_dev_tools Linux Development Tools
    adsf
*/

/**
    @page mac_dev_tools Mac OS X Development Tools
    adsf
*/


#endif
