//© Copyright 2010 - 2012 BlackTopp Studios Inc.
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
#ifndef _scriptinglua51_cpp
#define _scriptinglua51_cpp

#include "datatypes.h"
#ifdef MEZZLUA51

#include "scriptinglua51.h"
#include "exception.h"

/// @file
/// @brief This file has the interface for the Lua based implementation of the Scripting system.

extern "C"
{
    #include "lua.h"            // Lua Core
    #include "lualib.h"         // for opening the base state
    #include "lauxlib.h"        // Extra Lua Goodies like lua_open()

    int luaopen_Mezzanine(lua_State* L);
}

#include <iostream>

namespace Mezzanine
{
    namespace Scripting
    {
        namespace Lua
        {
            namespace
            {
                /// @internal
                /// @brief Used in the LuaScriptingEngine to get data from lua_dump durign script compilation
                /// @param State The lua state as provide by lua_dump
                /// @param Buffer A pointer to the compiled Lua chunk.
                /// @param Size The Size of the Lua chunk in bytes
                /// @param MezzScript A pointer to a Mezzanine::Scripting::Lua::LuaScript
                int LuaScriptWriter(lua_State *State, const void* Buffer, size_t Size, void* MezzScript)
                {
                    //LuaScript* CompilingScript = LuaScript(MezzScript);

                }
            }

            void LuaScriptingEngine::ThrowFromLuaErrorCode(int LuaReturn)
            {
                switch(LuaReturn)
                {
                    case 0:
                        break;
                    case LUA_YIELD:
                        MEZZ_EXCEPTION(Exception::SCRIPT_EXCEPTION_LUA_YIELD, "Lua returned a LUA_YIELD instead of completing.")
                    case LUA_ERRRUN:
                        MEZZ_EXCEPTION(Exception::SCRIPT_EXCEPTION_LUA_RUNTIME, "There was a runtime Error handling the Lua script.")
                    case LUA_ERRSYNTAX:
                        MEZZ_EXCEPTION(Exception::SYNTAX_ERROR_EXCEPTION_LUA, "There was an error with the syntax of the Lua script.")
                    case LUA_ERRERR:
                        MEZZ_EXCEPTION(Exception::SCRIPT_EXCEPTION_LUA_ERRERR, "There was an error when Lua attempted to handle an error.")
                    case LUA_ERRMEM:
                        MEZZ_EXCEPTION(Exception::MM_OUT_OF_MEMORY_EXCEPTION, "Lua could not allocate memory.")
                    case LUA_ERRFILE:
                        MEZZ_EXCEPTION(Exception::IO_EXCEPTION, "Lua had an error with file IO.")
                    default:
                        MEZZ_EXCEPTION(Exception::SCRIPT_EXCEPTION_LUA, "Lua had an error and we are not sure what it was.")
                }
            }

            LuaScriptingEngine::LuaScriptingEngine()
            {
                State = luaL_newstate();
                luaL_openlibs(State);
                luaopen_Mezzanine(State);
            }

            LuaScriptingEngine::~LuaScriptingEngine()
            {
                lua_close(State);
            }

            CountedPtr<iScript> LuaScriptingEngine::Execute(String ScriptSource)
            {
                ThrowFromLuaErrorCode(
                    luaL_dostring(State,ScriptSource.c_str())
                );
                luaopen_base(State);
            }

            void LuaScriptingEngine::Execute(CountedPtr<iScript> ScriptToRun)
            {

            }

            void LuaScriptingEngine::Compile(CountedPtr<iScriptCompilable> ScriptToCompile)
            {

            }

            CountedPtr<iScriptCompilable> LuaScriptingEngine::Compile(String SourceToCompile)
            {

            }

            void LuaScriptingEngine::OpenDefaultLibraries()
            {
                OpenBaseLibrary();
                OpenStringLibrary();
                OpenTableLibrary();
                OpenMathLibrary();
                OpenMezzanineSafeLibrary();
            }

            void LuaScriptingEngine::OpenAllLibraries()
            {
                OpenBaseLibrary();
                OpenPackageLibrary();
                OpenStringLibrary();
                OpenTableLibrary();
                OpenMathLibrary();
                OpenIOLibrary();
                OpenOSLibrary();
                OpenDebugLibrary();
                OpenMezzanineLibrary();
            }

            void LuaScriptingEngine::OpenBaseLibrary()
                { luaopen_base(State); }
            void LuaScriptingEngine::OpenPackageLibrary()
                { luaopen_package(State); }
            void LuaScriptingEngine::OpenStringLibrary()
                { luaopen_string(State); }
            void LuaScriptingEngine::OpenTableLibrary()
                { luaopen_table(State); }
            void LuaScriptingEngine::OpenMathLibrary()
                { luaopen_math(State); }
            void LuaScriptingEngine::OpenIOLibrary()
                { luaopen_io(State); }
            void LuaScriptingEngine::OpenOSLibrary()
                { luaopen_os(State); }
            void LuaScriptingEngine::OpenDebugLibrary()
                { luaopen_os(State); }
            void LuaScriptingEngine::OpenMezzanineLibrary()
                { luaopen_Mezzanine(State); }
            void LuaScriptingEngine::OpenMezzanineSafeLibrary()
                { }

            //simplistic error checking function, to be replace with proper exception driven code later.
            int PrintErrorMessageOrNothing(int ErrorCode)
            {
                switch(ErrorCode)
                {
                    case 0:             // Fine
                        break;
                    case LUA_YIELD:     // most errors seem to be this.
                        std::cout << std::endl << "Lua Error Code(LUA_YIELD): " << ErrorCode << std::endl;
                        break;
                    case LUA_ERRRUN:
                        std::cout << std::endl << "Lua Error Code(LUA_ERRRUN): " << ErrorCode << std::endl;
                        break;
                    case LUA_ERRSYNTAX:
                        std::cout << std::endl << "Lua Error Code(LUA_ERRSYNTAX): " << ErrorCode << std::endl;
                        break;
                    case LUA_ERRERR:
                        std::cout << std::endl << "Lua Error Code(LUA_ERRERR): " << ErrorCode << std::endl;
                        break;
                    default:
                        std::cout << std::endl << "Lua Error Code(Unknown Error): " << ErrorCode << std::endl;
                }
                return ErrorCode;
            }

            //
            int test()
            {
                lua_State *State;           // create a pointer to Track the Lua State
                State = luaL_newstate();    // Create a Lua State
                luaL_openlibs(State);
                luaopen_Mezzanine(State);

                std::cout << std::endl << "Let's try running a Lua command." << std::endl;
                int Error = luaL_dostring(State,"print \"Hello from Lua\"");    // run a very simple Lua script.
                PrintErrorMessageOrNothing(Error);

                std::cout << std::endl << "Now for Lua to call a function in the Mezzanine." << std::endl;
                Error = luaL_dostring(State,"Mezzanine.PrintHello()");
                PrintErrorMessageOrNothing(Error);

                std::cout << std::endl << "Now for some Lua class creation." << std::endl;
                Error = luaL_dostring(State,"                                                                   \
                                      print \"First We need some vectors, lets make A(1,2,3) a and b(4,5,6)\"   \
                                      A = Mezzanine.Vector3(1,2,3)                                              \
                                      B = Mezzanine.Vector3(4,5,6)                                              \
                                      print \"Now lets add them and see what happens\"                          \
                                      C = A+B                                                                   \
                                      print(C.X, C.Y, C.Z)                                                      \
                                      ");
                PrintErrorMessageOrNothing(Error);

                lua_close(State);           // Close the Lua state
            }
        } // Lua
    } // Scripting
} // Mezzanine




#endif //  MEZZLUA51
#endif
