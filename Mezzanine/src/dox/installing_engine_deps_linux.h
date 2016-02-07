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

#ifndef installing_engine_deps_linux_h
#define installing_engine_deps_linux_h

/**
    @page installing_engine_deps_linux Installing Linux Engine Dependencies
    This projects relies on a number of third party packages. Software packages that you must
    install are kept to a minimum and should be available from your distribution's package manager.

    @section installing_engine_deps_linux_included Included Dependencies
    These are included and built automatically as part of a normal Mezzanine build. These require no
    configuration beyond setting cmake options.

    - Bullet3d – This provides 3d physics.
    - SDL – This can do a number of low level game tasks, but we use it for user input.
    - Ogre3d – This provides us with high performance 3d graphics.
    - OpenAL-Soft – A software based implementation of OpenAL
    - Ogg and Vorbis – Sound encoding and containers.
    - Lua - A scripting language suitable for game development.

    @section installing_engine_deps_linux_external Required External Dependencies
    - libxaw7-dev — A set of simple X11 widgets, this is mandatory for Ogre.
    - libxrandr-dev — Somethings to do with X11, someone please put a real explanation here.
    - libglu1-mesa-dev and libgles1-mesa-dev — Opengl Rendersystem support (may not be required in
        all situations)
    - Any sound library that OpenAL-Soft can find, like libpulse-dev

    @section installing_engine_deps_linux_install Ubuntu Installation
    Installing the required libraries is normally easy on Ubuntu. Run the following command:
    @code
    sudo apt-get install libxaw7-dev libxrandr-dev libglu1-mesa-dev libgles1-mesa-dev libpulse-dev
    @endcode

    I have not needed to go out of way to include a sound dependency. Sometimes I do have issues
    with OpenGL and that is why mesa is included.

    @section installing_engine_deps_linux_optional Optional External Dependencies

    @subsection installing_engine_deps_linux_optiona_bullet Bullet3d
    You can optionally install @b libglu-dev or @b freeglut3-dev to allow building the bullet demos.

    @subsection installing_engine_deps_linux_optiona_ogre Ogre3d
    Ogre has a large number of of optional dependencies that can be installed, the most relevant
    ones are listed below. Ogre 3d is included in the source distribution, and does not need to be
    installed at the OS level. Dependencies required by Ogre (in Ubuntu names):

    - libfreeimage-dev — Used to allow automated load and handling of common image formats (jpg,
        gif, tga, etc…), so they can be used as 3d textures.
    - libfreetype6-dev — Works with fonts
    - nvidia-cg-toolkit — Adds support for Cg shaders, which we removed because they aren’t very
        good.
    - libboost-thread-dev — This adds support for multhreading into Ogre, This allows for
        experimentation, on many linux distrobutions, but introduces a bug in the version of boost
        shipped with Ubuntu 11.04 when using c++0x threads. This should be avoided unless you are
        explicitly experimenting with threads.
    - libois-dev — The Object-oriented Input System, is used by many Ogre projects and by the Ogre
        Demos.
    - libcppunit-dev — For Ogre unit testing.
    - libopengl-es-dev — Not sure on the name, this might be it, But there is a Package that enables
        Opengl 2.0 shaders in Ogre
    - x11proto-core-dev and libx11-dev — For certain window integration features on some systems are
        required as well. These are core components and will likely be installed with some of the
        other development libraries.


*/

#endif
