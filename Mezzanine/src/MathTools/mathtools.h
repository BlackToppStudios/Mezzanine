// © Copyright 2010 - 2014 BlackTopp Studios Inc.
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
#ifndef _mathtool_h
#define _mathtool_h

#include "vector2.h"
#include "vector3.h"
#include "MathTools/intersections.h"

namespace Mezzanine
{
    /// @namespace Mezzanine::MathTools
    /// @brief This namespace is the home of a number of utility variables and methods to facilitate various math related tasks.
    namespace MathTools
    {
    }
}


#include "swig.h"
#ifdef SWIG_MATHTOOLS
    #ifdef SWIG_UNSAFE
        %module MezzanineMathTools
    #else
        #define SWIG_SAFE
        %module MezzanineMathToolsSafe
    #endif
    #define SWIG_MODULE_SET

    // Tell SWIG to create a module that scripting languages can use called "mezzanine"
    // and insert a minimum of documentation into the bindingfile
    %{
        // code to be inserted verbatim into the swig file goes here
        #include "mezzanine.h"
        using namespace Mezzanine;
        using namespace Mezzanine::MathTools;
    %}
#endif

#include "MathTools/arithmetic.h"
#include "MathTools/constants.h"
#include "MathTools/intersections.h"
#include "MathTools/intersectiontools.h"
#include "MathTools/trigonometry.h"


namespace Mezzanine
{
    /// @namespace Mezzanine::MathTools
    /// @brief This namespace is the home of a number of utility variables and methods to facilitate various math related tasks.
    namespace MathTools
    {

    }//MathTools
}//Mezzanine

#endif
