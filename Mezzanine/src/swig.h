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
#ifndef _swig_h
#define _swig_h

///////////////////////////////////////////////////////////////////////////////
/// @file
/// @brief Used to give commands specifically to the SWIG preprocessor
/// @details SWIG is a C/C++ source code preprocessor that reads source files
/// and produces an implementation of bindings for that language. Currently it
/// is only used for Lua. It can be used for other items in the future.
///////////////////////////////////////////////////////////////////////////////

// Prevent doxygen parsing of the items to insert in the bindings files

#ifdef SWIG
    // This block of code is only read by swig.

    // Tell SWIG to create a module that scripting languages can use called "mezzanine"
    // and insert a minimum of documentation into the bindingfile
    %{
        // code to be inserted verbatim into the swig file goes here
        #include "mezzanine.h"
        #include "btBulletDynamicsCommon.h"
        #include "Ogre.h"
        #include "SDL.h"

        #ifdef GetCurrentTime
        #undef GetCurrentTime
        #endif

        #ifdef GetExceptionCode
        #undef GetExceptionCode
        #endif

        using namespace Mezzanine;
        //using namespace Mezzanine::Threading;
        //using namespace Mezzanine::Physics;

        // Fix cross module type-usage
        namespace Mezzanine
        {
            namespace Physics
            {
                typedef Mezzanine::Vector3 Vector3;
            }
            namespace Threading
            {

            }
            typedef Threading::RawFile RawFile;
        }
    %}

    // Fix cross module type-usage
    namespace Mezzanine
    {
        namespace Physics
        {
            typedef Mezzanine::Vector3 Vector3;
        }
        namespace Threading
        {

        }
        typedef Threading::RawFile RawFile;
    }

    %include stl.i
    %include stdint.i
    %include std_except.i
    %include std_common.i
    //%include std_container.i
    %include std_deque.i
    %include std_except.i
    //%include std_list.i
    %include std_map.i
    %include std_pair.i
    %include std_string.i
    %include std_vector.i

    // Since swig will process only on language and only the safe or unsafe version of it at a time
    // this is where the naming of the Mezzanine library should occur. This does not affect the
    // ability to load Multiple libraries or even scripting languages at the same time if required.
    /// @def SWIG_MODULE_SET
    /// @brief This is set
    #ifdef SWIG_MAIN
        #ifdef SWIG_UNSAFE
            %module Mezzanine
        #else
            #define SWIG_SAFE
            %module MezzanineSafe
        #endif
        #define SWIG_MODULE_SET
    #endif

    // Fix for: XML/xml.h:XXXX: Warning 403: Class 'Mezzanine::XML::XPathException' might be abstract, no constructors generated
    //%warnfilter(403) Mezzanine::XML::XPathException;
#endif // end SWIG


// Warn SWIG users of the messages that Swig will produce




#endif
