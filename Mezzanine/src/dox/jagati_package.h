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
#ifndef _jagati_package_h
#define _jagati_package_h

/**
    @page jagati_package Jagati Package and Refactor
    Currently the Mezzanine engine is one monolithic code base. Many of the libaries can work
    independently and still be extemely useful. The Jagati Refactor is the modularizing of these
    libraries.

    @n
    This page describes the contents of one Jagati package. We wil pull a piece of the Mezzanine out
    and make a package from it. It will be refactored and adjusted to meet the standards here.

    @n
    Each package will compile under Clang, GCC, MinGW, TDM-gcc and Visual Studio. Each compilation
    will enable "-werror" or whatever compiler flag turns all warnings into errors. A slew of extra
    compiler warnings will be enabled to minimize the chance of writing bugs.

    @n
    We will keep our current level of documentation. We will keep out curent level of coverage for
    our automated generation

    @section jagati_package_package Modules after Jagati Refactor

    @dot Jagati Packages and Dependencies
    digraph JagatiPackages {

        EntreLua [shape=box];
        Armadillo [shape=box];

        Lua51Bindings [shape=box];
        RubyBindings [shape=box];
        JavaBindings [shape=box];

        Scripting [shape=box];
        Input [shape=box];
        Network [shape=box];
        Resource [shape=box];

        Graphics [shape=box];
        UI [shape=box];
        Physics [shape=box];
        Noise [shape=box];
        Audio [shape=box];

        Entresol [shape=box];
        Threading [shape=box];
        Base [shape=box];
        Math [shape=box];

        StaticFoundation [shape=box];
        Testing [shape=box];
        Foundation [shape=box];

        EntreLua -> Lua51Bindings;
        Armadillo -> UI;

        Lua51Bindings -> Scripting;
        RubyBindings -> Scripting;
        JavaBindings -> Scripting;

        Scripting -> Base;
        Input -> Base;
        Network -> Base;
        Resource -> Base;

        UI -> Graphics;
        Graphics -> Base;
        Graphics -> Math;
        Audio -> Base;
        Audio -> Math;
        Physics -> Base;
        Physics -> Math;
        Noise -> Base;
        Noise -> Math;

        Entresol -> Threading;
        Entresol -> Base;
        Threading -> Foundation;
        Base -> Foundation;
        Math -> Foundation;

        Foundation -> Testing;
        Testing -> StaticFoundation;
        Foundation -> StaticFoundation;
    }
    @enddot

    @section jagati_package_files Files and Folders in a package
    A rough file hiearchy:

    - @ref jagati_package_root – Package Root
        - @ref jagati_package_cmake "CMakeLists.txt"
        - @ref jagati_package_license
        - @ref jagati_package_ci
        - @ref jagati_package_readme
        - @ref jagati_package_dox_dir
            - @ref jagati_package_dox_h
        - @ref jagati_package_include_dir
        - @ref jagati_package_lib_dir
        - @ref jagati_package_src_dir
        - @ref jagati_package_swig_dir
            - @ref jagati_package_swig_config_h
        - @ref jagati_package_test_dir

    @section jagati_package_root / – Package Root
    The root folder.

    @section jagati_package_cmake CMakeLists.txt and Jagati CMake API
    We need to describe variables the variables that the root CMakeLists.txt. This is still being
    decided on and will likely be determined by ruslo/hunter.

    @section jagati_package_gitignores .gitignore
    This file tells git what does not need to be included in the build.

    @section jagati_package_ci .travis.yml and appveyor.yml – Coninuous Integration Config
    Each Jagati package will be built and run automatically on a variety of platforms automatically.

    @section jagati_package_readme README.md – Technical details for Humans
    This should have an overview

    @section jagati_package_license COPYING.md
    This file will have legal details about copying the files and resources in the package. The
    Default for most Jagati packages is the
    @htmlonly<a href="">@endhtmlonly
    GPL v3
    @htmlonly</a>@endhtmlonly
    but the
    @htmlonly<a href="">@endhtmlonly
    BSD License
    @htmlonly</a>@endhtmlonly
    is good to. The are no technical reasons other licenses couldn't be described here.

    @section jagati_package_dox_dir dox/ – Special Considerations for Doxygen.
    Extra docmentation headers and files ought to go here.

    @subsection jagati_package_dox_h dox/dox.h – Doxygen entry point
    If this optional file is present it is the file that Doxygen will read. This should include
    all the files that are Doxygen documented.

    @section jagati_package_include_dir include/ – Header directory for others to use
    Describe why to separate headers from source files here.

    @section jagati_package_lib_dir lib/ – Internal libraray source
    Many Packages provide some kind of functionality provided by external systems or that require
    very complex algorithms. Any external libraries providing such functionality can

    @section jagati_package_src_dir src/ – Actual source code
    The implementation of a package goes here.

    @section jagati_package_swig_dir swig/ – Special Considerations for SWIG.
    Most people do not want to build their whole game or application in C++. Swig reads the C/C++
    source code of the package and generates bindings for other languages to be able to use the
    Mezzanine.

    @subsection jagati_package_swig_config_h swig/SwigConfig.h – SWIG Entry Point.
    If this file is present it is the entry point for Swig. This should include all the headers
    that SWIG is to read and prepare bindings for.

    @section jagati_package_test_dir test/ – Test Headers
    A directory full of files that each contain one class describing tests for one part of the
    system

*/

#endif
