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
#ifndef _lua51scriptargument_cpp
#define _lua51scriptargument_cpp

#include "datatypes.h"
#include "lua51scriptargument.h"

#ifdef MEZZLUA51

#include "lua51script.h"
#include "lua51scriptingengine.h"
#include "exception.h"

#include <algorithm>

/// @file
/// @brief This file has the implementation for the Lua script argument.

extern "C"
{
    #include "lua.h"            // Lua Core
    #include "lualib.h"         // for opening the base state
    #include "lauxlib.h"        // Extra Lua Goodies like lua_open()
}

namespace Mezzanine
{
    namespace Scripting
    {
        namespace Lua
        {


            void Lua51IntegerArgument::Push(lua_State* TargetState) const
                { lua_pushinteger(TargetState, this->Datum); }

            void Lua51IntegerArgument::Pop(lua_State* TargetState)
            {
                int Top = lua_gettop(TargetState);
                if(LUA_TNUMBER==lua_type(TargetState,Top))
                    { Datum = lua_tointeger(TargetState,Top); }
                else
                { MEZZ_EXCEPTION(Exception::PARAMETERS_EXCEPTION, "Expected a Lua number(Integer), but found something else.") }
            }

            void Lua51RealArgument::Push(lua_State* TargetState) const
                { lua_pushnumber(TargetState, this->Datum); }
            void Lua51RealArgument::Pop(lua_State* TargetState)
            {
                int Top = lua_gettop(TargetState);
                if(LUA_TNUMBER==lua_type(TargetState,Top))
                    { Datum = lua_tonumber(TargetState,Top); }
                else
                    { MEZZ_EXCEPTION(Exception::PARAMETERS_EXCEPTION, "Expected a Lua number(Real), but found something else.") }
            }

            void Lua51WholeArgument::Push(lua_State* TargetState) const
                { lua_pushnumber(TargetState, this->Datum); }
            void Lua51WholeArgument::Pop(lua_State* TargetState)
            {
                int Top = lua_gettop(TargetState);
                if(LUA_TNUMBER==lua_type(TargetState,Top))
                    { Datum = lua_tointeger(TargetState,Top); }
                else
                    { MEZZ_EXCEPTION(Exception::PARAMETERS_EXCEPTION, "Expected a Lua number(Whole), but found something else.") }
            }

            void Lua51StringArgument::Push(lua_State *TargetState) const
                { lua_pushlstring(TargetState, this->Datum.c_str(), this->Datum.size() ); }
            void Lua51StringArgument::Pop(lua_State *TargetState)
            {
                int Top = lua_gettop(TargetState);
                if(LUA_TSTRING==lua_type(TargetState,Top))
                    { Datum = lua_tostring(TargetState,Top); }
                else
                    { MEZZ_EXCEPTION(Exception::PARAMETERS_EXCEPTION, "Expected a Lua string(String), but found something else.") }
            }

            void Lua51BoolArgument::Push(lua_State *TargetState) const
                { lua_pushboolean(TargetState, this->Datum); }
            void Lua51BoolArgument::Pop(lua_State *TargetState)
            {
                int Top = lua_gettop(TargetState);
                if(LUA_TBOOLEAN==lua_type(TargetState,Top))
                    { Datum = lua_toboolean(TargetState,Top); }
                else
                    { MEZZ_EXCEPTION(Exception::PARAMETERS_EXCEPTION, "Expected a Lua string(String), but found something else.") }
            }

            void Lua51NilArgument::Push(lua_State *TargetState) const
                { lua_pushnil(TargetState); }
            void Lua51NilArgument::Pop(lua_State *TargetState)
            {
                int Top = lua_gettop(TargetState);
                if(LUA_TNIL==lua_type(TargetState,Top))
                    { lua_pop(TargetState,1); }
                else
                    { MEZZ_EXCEPTION(Exception::PARAMETERS_EXCEPTION, "Expected a Lua string(String), but found something else.") }
                // It seems Lua Nils never go on the stack when being returned sometimes.
            }




        } // Lua
    } // Scripting
} // Mezzanine




#endif //  MEZZLUA51
#endif
