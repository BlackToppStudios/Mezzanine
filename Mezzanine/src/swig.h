//© Copyright 2010 - 2011 BlackTopp Studios Inc.
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
#ifndef _swig_h
#define _swig_h

///////////////////////////////////////////////////////////////////////////////
/// @file
/// @brief Used to give commands specifically to the SWIG preprocessor
/// @details SWIG is a C/C++ source code preprocessor that reads source files
/// and produces an implemtation of bindings for that language. Currently it
/// is only used for Lua. It can be used for other items in the future.
/// \n This file defines a number of messages to make troubleshooting SWIG
/// and the script bindings a little easier.
///     - SWIG_INFO_WARN - 999: Warning 990 to 999 are informational messages.
///     - SWIG_INFO_BEGINCLASS - 990: Begining parsing of class.
///     - SWIG_INFO_ENDCLASS - 991: Completing parsing of class.
///////////////////////////////////////////////////////////////////////////////

// Prevent doxygen parsing of the items to insert in the bindings files

#ifdef SWIG
    // Tell SWIG to create a module that scripting languages can use called "mezzanine"
    // and insert a minimum of documentation into the bindingfile
    %{
        // code to be inserted verbatim into the swig file that does not go at the top or bottom like the build script does it
    %}

    %include stl.i
    %module Mezzanine

    #define SWIG_INFO_BEGINCLASS        %warn "990: Begining parsing of class."
    #define SWIG_INFO_ENDCLASS          %warn "991: Completing parsing of class."

    #define SWIG_INFO_WARN              %warn "999: Warning 990 to 999 are informational messages."
#else
    // Define warnings as nothing as to not fuck with other classes
    #define SWIG_INFO_BEGINCLASS
    #define SWIG_INFO_ENDCLASS
    #define SWIG_INFO_WARN
#endif


// Warn SWIG users of the messages that Swig will produce
SWIG_INFO_WARN



#endif
