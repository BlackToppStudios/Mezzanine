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
#ifndef _dox_mainpage_h
#define _dox_mainpage_h

/**
    @mainpage BlackTopp Studios Documentation

    BlackTopp Studios inc. makes video games and video game components. Our flagship library game
    component is the @ref mezzanine_engine. Some components of the can be used on their own, like
    the @ref ThreadingManual "DAGFrameScheduler" a high performance multithreaded replacement for
    single threaded game loops.

    @n Here are some useful links:
    - @ref tech_doc_index
    - @ref blog
    - @ref team_and_credits

    @n
    Currently we are working on a project to modularize the @ref mezzanine_engine. We are calling
    this the Jagati refactor and the project will be split into several
    @ref jagati_package "Jagati Packages".

*/

// The pages below are intended to be put into their own files as they are worked on.
// All of these have links pointing to them.

/**
    @page blog BTS blog

    - @htmlonly<a href="">@endhtmlonly
    TITLE
    @htmlonly</a>@endhtmlonly
    — DEsc
*/

/**
    @page engine_dependencies Mezzanine Engine Dependencies

    C++!
*/

/**
    @page catch_bang Catch!
    describe catch here
*/


/**
    @page best_practices_cmake Best Practices CMake

    CMake is a tool for making dependency graphs.

    @section asdf
    describe "Driving target" pattern
    https://cmake.org/pipermail/cmake/2015-January/059732.html


    @section asdf2
    COMMAND last issue

    @section asdf3
    Target and all
    Build steps requiring multiple input files will not be rerun if by add_custom_command
    if it is a normal file (not exe, not lib not target name)

*/

/**
    @page best_practices_cxx Best Practices C++
    Start with https://github.com/isocpp/CppCoreGuidelines

    don't use std::bind
    https://www.youtube.com/watch?v=zt7ThwVfap0&index=30&list=PLHTh1InhhwT75gykhs7pqcR_uSiG601oh
*/

/**
    @page email_setup Email setup
    mail.blacktoppstudios.com port XXXX


*/



#endif
