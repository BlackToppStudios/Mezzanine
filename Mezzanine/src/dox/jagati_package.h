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
    @page jagati_package Jagati Package
    This page describes the contents of one package.

    @section jagati_package_files Files and Folders in a package

    - @ref jagati_package_root – Package Root
        - @ref jagati_package_cmake "CMakeLists.txt"
        - @ref jagati_package_license
        - @ref jagati_package_manifest
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
    The root folder,

    @section jagati_package_cmake CMakeLists.txt and Jagati CMake API
    We need to describe variables the variables that the root CMakeLists.txt

    @section jagati_package_manifest Manifest.xml – Technical details for machines
    Need to decide and describe how to define dependencies and other meta-data

    @section jagati_package_readme Readme.md – Technical details for Humans
    This should have an overview

    @section jagati_package_license COPYING.md
    This file will have legal details about copying the files and resources in the pacakge. The
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
    source of

    @subsection jagati_package_swig_config_h swig/SwigConfig.h – SWIG Entry Point.
    If this file is present it is the entry point for swig. This should include all the headers
    that SWIG is to read and prepare bindings for.

    @section jagati_package_test_dir test/ – Test Headers
    A directory full of files that each contain one class describing tests for one part of the
    system

*/

#endif
