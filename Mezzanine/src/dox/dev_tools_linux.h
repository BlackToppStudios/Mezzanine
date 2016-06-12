// © Copyright 2010 - 2016 BlackTopp Studios Inc.
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
#ifndef _dox_dev_tools_linux_h
#define _dox_dev_tools_linux_h


/**
    @page dev_tools_linux Linux Development Tools
    Installing the developer tool is pretty easy on most Linux workstations, we have only performed
    it on Ubuntu recently. There is no reason why it wouldn’t work on any Linux distrobution as long
    as it provide the required packages or you are able to compile them from source. Here we detail
    the setups we have performed.

    @section dev_tools_ubuntu_install Ubuntu Install
    Installing the developer tools on Ubuntu is incredibly easy. Just run this one command from a
    terminal and it will install a minimum set of required tools at once for you.
    @code
    sudo apt install g++ git cmake
    @endcode
    This leaves you with compiler, tools to download our code from
    @htmlonly<a href="https://github.com/BlackToppStudios/Mezzanine">@endhtmlonly
    Github
    @htmlonly</a>@endhtmlonly
    , and a way to create build scripts to simplify compilation. We use a few more tools than that
    in a typical day working on the Mezzanine and Catch!. To get a set of tools similar to our
    please run the following:
    @code
    sudo apt install g++ gdb codeblocks qtcreator doxygen texlive-font-utils graphviz git pkg-config cmake-gui cmake autoconf swig ninja-build mercurial blender audacity clang
    @endcode
    On some older systems you may want to use "apt-get" instead of apt.

    @section dev_tools_linux_summary What Are the Tools
    Most of the Tools are explained on the @ref cxx_training_and_tools page, but some we have
    included here because they seem to get more use on Linux workstations.

    - @htmlonly<a href="https://gcc.gnu.org/">@endhtmlonly
    GCC — G++
    @htmlonly</a>@endhtmlonly
    — This is a C++ compiler,
    @htmlonly<a href="http://clang.llvm.org/">@endhtmlonly Clang @htmlonly</a>@endhtmlonly
    works also but at the time of this writing created slightly less optimal results.

    - @htmlonly<a href="https://www.gnu.org/software/gdb/">@endhtmlonly
    GCC – gdb
    @htmlonly</a>@endhtmlonly
    — The debugger that works with g++.

    - @htmlonly<a href="http://www.cmake.org/">@endhtmlonly
    CMake
    @htmlonly</a>@endhtmlonly
    — Cmake is a tool to aid in the compiling of specially setup programs, like the configuration of
    the Ogre 3d library. If you are building any of the libraries that require CMake 3.2 and are
    using Ubuntu 14.04 you will need to get a newer version of CMake than what the default Ubuntu
    repos ship. The PPA https://launchpad.net/~george-edison55/+archive/ubuntu/cmake-3.x has 3.2
    and some newer builds, you also have the option of building CMake from source or using another
    PPA.

    - @htmlonly<a href="http://www.codeblocks.org/">@endhtmlonly
    Code::Blocks
    @htmlonly</a>@endhtmlonly
    or
    @htmlonly<a href="http://www.qt.io/ide/">@endhtmlonly
    QT Creator
    @htmlonly</a>@endhtmlonly
    — Both are Integrated Development Environments IDEs that bring most of these other tools
    together into one friendly UI.

    - @htmlonly<a href="http://www.stack.nl/~dimitri/doxygen/index.html">@endhtmlonly
    Doxygen
    @htmlonly</a>@endhtmlonly
    — This is an tool for extracting documentation from source code. This is optional, as you can
    use the documentation that we have built. But if you want the latest docs you might need to
    built it yourself.

    - @htmlonly<a href="http://graphviz.org/">@endhtmlonly
    Graphviz
    @htmlonly</a>@endhtmlonly
    — Doxygen uses thiswhen building the API documentation to draw charts and graphs.

    - @htmlonly<a href="https://launchpad.net/ubuntu/trusty/+package/texlive-font-utils">
    @endhtmlonly
    Texlive-font-utils
    @htmlonly</a>@endhtmlonly
    — Fonts used when creating PDF versions of the documentation.


    - @htmlonly<a href="https://git-scm.com/">@endhtmlonly
    Git
    @htmlonly</a>@endhtmlonly
    — Git is a powerful revision control system. We use to merge our progress and be certain
    that we have the most recent version of the source code.

    - @htmlonly<a href="http://www.gnu.org/software/autoconf/autoconf.html">@endhtmlonly
    autoconf
    @htmlonly</a>@endhtmlonly
    and
    @htmlonly<a href="http://www.freedesktop.org/wiki/Software/pkg-config/">@endhtmlonly
    pkg-config
    @htmlonly</a>@endhtmlonly
    — These are older style tools used to create build scripts. SDL and maybe a few other libraies
    require these for its build process. These are only of limited use and might even be optional
    now that most libraries are integrated under the CMake build system.

    - @htmlonly<a href="https://ninja-build.org/">@endhtmlonly
    Ninja
    @htmlonly</a>@endhtmlonly
    — This is an optional faster alterative to using make. This seems to reduce build times by
    around 20% from the command line, and a little less from IDEs.

    - @htmlonly<a href="http://swig.org/">@endhtmlonly
    SWIG
    @htmlonly</a>@endhtmlonly
    — Swig is used to create the bindings that connect scripting languages like Lua to the rest of
    the Mezzanine.

    - @htmlonly<a href="https://www.mercurial-scm.org/">@endhtmlonly
    Mercurial
    @htmlonly</a>@endhtmlonly
    — SDL uses this as their primary source control tool.

    @section dev_tools_linux_getting_source Getting the Source Code
    Once you have the tools installed all you should have to do to get the code repository is run
    the following command:
    @code
    git clone git://github.com/BlackToppStudios/Mezzanine.git
    @endcode

    @section dev_tools_linux_prepare_compilation Preparing for Compilation
    Before you can compile the @ref mezzanine_engine and @ref catch_bang you will need to install
    the libraries that the engine is built on top of. See the page on
    @ref installing_engine_deps_linux for more details.

    @section dev_tools_linux_what_next What’s next
    If you aren’t comfortable with your coding skills you should read about
    @ref cxx_training_and_tools. If you are comfortable with your coding skills and have the tools
    installed reading up on the @ref engine_dependencies could be useful or you could go right onto
    @ref building_the_engine if you think already have all the software installed.

*/


#endif
