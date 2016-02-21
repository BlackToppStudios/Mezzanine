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

#ifndef running_doxygen_h
#define running_doxygen_h

/**
    @page running_doxygen Running Doxygen
    The Mezzanine API is nearly 100% described in the Doxygen documentation. This API documentation
    is intended as a reference (rather than tutorial) and comes in the
    @htmlonly<a href="https://github.com/BlackToppStudios/Mezzanine">@endhtmlonly
    Mezzanine github repository
    @htmlonly</a>@endhtmlonly
    . A version of this documentation is periodically updated and made available as the BlackTopp
    Studios API documentation, but this can be out of date by several months. If you want the most
    updated version of the API the most reliable solution is to build the API documentation from
    the source code you have.

    @section running_doxygen_tools Tools Required
    To build the API documentation you will need
    @htmlonly<a href="http://www.blacktoppstudios.com/www.stack.nl/~dimitri/doxygen/">@endhtmlonly
    Doxygen
    @htmlonly</a>@endhtmlonly
    and
    @htmlonly<a href="http://www.graphviz.org">@endhtmlonly
    Graphviz
    @htmlonly</a>@endhtmlonly
    . These are available from
    their respective websites’ download pages:

    - @htmlonly<a href="http://www.stack.nl/~dimitri/doxygen/download.html">@endhtmlonly
    Doxygen Download
    @htmlonly</a>@endhtmlonly
    - @htmlonly<a href="http://www.graphviz.org/Download.php">@endhtmlonly
    Graphviz Download
    @htmlonly</a>@endhtmlonly


    Windows and Mac OS X users will have to manually install these. Linux users may be able to
    automatically install these tools from their distro’s repository with their package manager.
    Here is an example on Ubuntu:
    @code
    sudo apt-get install doxygen graphviz
    @endcode

    @section running_doxygen_cmake Cmake and the Build Target
    When configuring CMake enabling the "Mezz_Doc" will cause doxygen documentation to be built with
    any full build or any build of the “MezzanineDocumentation” build target. Either check the box
    from the cmake-gui and from the command prompt you can use:

    @code
    cmake ../Mezzanine/ -DMezz_Doc:boolean=true
    @endcode

    For more details on how to use cmake see the section on @ref building_the_engine_using_cmake
    in the @ref building_the_engine page.

    @n
    In Code::Blocks You can build the documentation by simply selecting this from the
    “Build target:” menu, then clicking the gear icon to build. If you generated build files that
    use make, you should be able to build the documentation with:

    @code
    make MezzanineDocumentation
    @endcode

    Or if you had cmake emit ninja build scripts

    @code
    ninja MezzanineDocumentation
    @endcode

    Any of these documentation build processes will read the doxygen config file, found at
    ‘docs/doxygen/doxyfiles/Doxyfile’ relative to the root of the source repo. They will output html
    documentation to ‘docs/doxygen/html’ which you can browse in any web browser by opening
    ‘docs/doxygen/html/index.html’.

    If you use a modern IDE and wish to update documenation, any warnings or errors should be
    presented much the same compiler warnings or errors. In the Code::Blocks ‘Build Messages’ tab or
    Qt Creator ‘Issues’ panel you should be able to click or double click a specific item and have
    it take you to the documentation problem in the source code.

*/

#endif
