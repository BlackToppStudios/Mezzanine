//© Copyright 2010 - 2013 BlackTopp Studios Inc.
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
#ifndef _rubyscriptargument_cpp
#define _rubyscriptargument_cpp

#include "datatypes.h"
#include "rubyscriptargument.h"

#ifdef MEZZRUBY

#include "rubyscript.h"
#include "rubyscriptingengine.h"
#include "exception.h"

#include <algorithm>

/// @file
/// @brief This file has the implementation for the Ruby script argument.

extern "C"
{
    #include "ruby.h"            // Ruby Core
    #include "rubylib.h"         // for opening the base state
    #include "lauxlib.h"        // Extra Ruby Goodies like ruby_open()
}

namespace Mezzanine
{
    namespace Scripting
    {
        namespace Ruby
        {


            void RubyIntegerArgument::Push(ruby_State* TargetState) const
                { ruby_pushinteger(TargetState, this->Datum); }

            void RubyIntegerArgument::Pop(ruby_State* TargetState)
            {
                int Top = ruby_gettop(TargetState);
                if(RUBY_TNUMBER==ruby_type(TargetState,Top))
                    { Datum = ruby_tointeger(TargetState,Top); }
                else
                { MEZZ_EXCEPTION(Exception::PARAMETERS_EXCEPTION, "Expected a Ruby number(Integer), but found something else.") }
            }

            void RubyRealArgument::Push(ruby_State* TargetState) const
                { ruby_pushnumber(TargetState, this->Datum); }
            void RubyRealArgument::Pop(ruby_State* TargetState)
            {
                int Top = ruby_gettop(TargetState);
                if(RUBY_TNUMBER==ruby_type(TargetState,Top))
                    { Datum = ruby_tonumber(TargetState,Top); }
                else
                    { MEZZ_EXCEPTION(Exception::PARAMETERS_EXCEPTION, "Expected a Ruby number(Real), but found something else.") }
            }

            void RubyWholeArgument::Push(ruby_State* TargetState) const
                { ruby_pushnumber(TargetState, this->Datum); }
            void RubyWholeArgument::Pop(ruby_State* TargetState)
            {
                int Top = ruby_gettop(TargetState);
                if(RUBY_TNUMBER==ruby_type(TargetState,Top))
                    { Datum = ruby_tointeger(TargetState,Top); }
                else
                    { MEZZ_EXCEPTION(Exception::PARAMETERS_EXCEPTION, "Expected a Ruby number(Whole), but found something else.") }
            }

            void RubyStringArgument::Push(ruby_State *TargetState) const
                { ruby_pushlstring(TargetState, this->Datum.c_str(), this->Datum.size() ); }
            void RubyStringArgument::Pop(ruby_State *TargetState)
            {
                int Top = ruby_gettop(TargetState);
                if(RUBY_TSTRING==ruby_type(TargetState,Top))
                    { Datum = ruby_tostring(TargetState,Top); }
                else
                    { MEZZ_EXCEPTION(Exception::PARAMETERS_EXCEPTION, "Expected a Ruby string(String), but found something else.") }
            }

            void RubyBoolArgument::Push(ruby_State *TargetState) const
                { ruby_pushboolean(TargetState, this->Datum); }
            void RubyBoolArgument::Pop(ruby_State *TargetState)
            {
                int Top = ruby_gettop(TargetState);
                if(RUBY_TBOOLEAN==ruby_type(TargetState,Top))
                    { Datum = ruby_toboolean(TargetState,Top); }
                else
                    { MEZZ_EXCEPTION(Exception::PARAMETERS_EXCEPTION, "Expected a Ruby string(String), but found something else.") }
            }

            void RubyNilArgument::Push(ruby_State *TargetState) const
                { ruby_pushnil(TargetState); }
            void RubyNilArgument::Pop(ruby_State *TargetState)
            {
                int Top = ruby_gettop(TargetState);
                if(RUBY_TNIL==ruby_type(TargetState,Top))
                    { ruby_pop(TargetState,1); }
                else
                    { MEZZ_EXCEPTION(Exception::PARAMETERS_EXCEPTION, "Expected a Ruby string(String), but found something else.") }
                // It seems Ruby Nils never go on the stack when being returned sometimes.
            }




        } // Ruby
    } // Scripting
} // Mezzanine




#endif //  MEZZRUBY
#endif
