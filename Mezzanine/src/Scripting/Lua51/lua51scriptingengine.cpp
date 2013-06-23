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
#ifndef _lua51scriptingengine_cpp
#define _lua51scriptingengine_cpp

#include "datatypes.h"

#ifdef MEZZLUA51

#include "lua51script.h"
#include "lua51scriptargument.h"
#include "lua51scriptingengine.h"
#include "exception.h"

#include <cstring>
/// @file
/// @brief This file has the implemetation for the Lua based Scripting system.

extern "C"
{
    #include "lua.h"            // Lua Core
    #include "lualib.h"         // for opening the base state
    #include "lauxlib.h"        // Extra Lua Goodies like lua_open()

    int luaopen_Mezzanine(lua_State* L);
    int luaopen_MezzanineSafe(lua_State* L);
}

#include <iostream>

namespace Mezzanine
 {
    namespace Scripting
    {
        namespace Lua
        {

            ///////////////////////////////////////////////////////////////////////////////////////
            // LuaScriptEngine and helpers
            namespace
            {
                /// @internal
                /// @brief Used with the Lua API when a chunk name is required.
                const char* DefaultChunkName = "Chunk";

                /// @internal
                /// @brief Used in the LuaScriptingEngine to get data from lua_dump during script compilation
                /// @param State The Lua state as provide by lua_dump
                /// @param Buffer A pointer to the compiled Lua chunk.
                /// @param Size The Size of the Lua chunk in bytes
                /// @param BinBuff A pointer to a BinaryTools::BinaryBuffer that will serve as the real output
                /// @note This is a lua_Writer as per http://www.lua.org/manual/5.1/manual.html#lua_Writer
                int LuaBytecodeDumper(lua_State *State, const void* Buffer, size_t Size, void* BinBuff)
                {
                    BinaryTools::BinaryBuffer* CompilingScript = reinterpret_cast<BinaryTools::BinaryBuffer*>(BinBuff);
                    CompilingScript->Concatenate(
                                    (BinaryTools::BinaryBuffer::Byte*) Buffer,
                                    Size
                                );
                    return 0;
                }

                /// @internal
                /// @param State The Lua state as provided by lua_load()
                /// @param Buffer A BinaryBuffer containing the bytecode to load into Lua
                /// @param Size an output parameter to convey the size of the return to Lua.
                /// @return A pointer to a binary buffer suitable for Lua's use and the size of that buffer in the output parameter Size
                /// @warning The Lua documentation clearly indicates second parameter should be const and the third parameter should be a size_t* but the compiler says long unsigned int*
                /// @note This is a lua_Reader as per http://www.lua.org/manual/5.1/manual.html#lua_Reader
                const char* LuaBytecodeLoader(lua_State* State, void* BinBuff, size_t* Size)
                {
                    //const BinaryTools::BinaryBuffer* LoadingScript = reinterpret_cast<const BinaryTools::BinaryBuffer*>(BinBuff);
                    FlaggedBuffer* LoadingBuffer= reinterpret_cast<FlaggedBuffer*>(BinBuff);
                    if(LoadingBuffer->Loaded)
                    {
                        LoadingBuffer->Loaded = false;
                        return 0;
                    }else{
                        *Size = LoadingBuffer->Size;
                        LoadingBuffer->Loaded = true;
                        return (const char*)LoadingBuffer->Binary;
                    }
                }

                /// @internal
                /// @param State The Lua state as provided by lua_load()
                /// @param Buffer A Lua51Script* containing the source to load into Lua
                /// @param Size an output parameter to convey the size of the return to Lua.
                /// @return A pointer to a binary buffer suitable for Lua's use and the size of that buffer in the output parameter Size
                /// @warning The Lua documentation clearly indicates second parameter should be const and the third parameter should be a size_t* but the compiler says unsigned int*
                /// @note This is a lua_Reader as per http://www.lua.org/manual/5.1/manual.html#lua_Reader
                const char* LuaSourceLoader(lua_State* State, void* BinBuff, size_t* Size)
                {
                    Lua51Script* LoadingScript = reinterpret_cast<Lua51Script*>(BinBuff);
                    if(LoadingScript->GetByteCodeReference().Loaded)
                    {
                        LoadingScript->GetByteCodeReference().Loaded = false;
                        return 0;
                    }else{
                        *Size = LoadingScript->GetSourceCode().size();
                        LoadingScript->GetByteCodeReference().Loaded = true;
                        return LoadingScript->GetSourceCode().c_str();
                    }
                }

            }

            ///////////////////////////////////////////////////////////////////////////////////////
            // Publically visible internals
            void Lua51ScriptingEngine::ThrowFromLuaErrorCode(int LuaReturn)
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
                        MEZZ_EXCEPTION(Exception::IO_FILE_EXCEPTION, "Lua had an error with file IO.")
                    default:
                        MEZZ_EXCEPTION(Exception::SCRIPT_EXCEPTION_LUA, "Lua had an error and we are not sure what it was.")
                }
            }

            ///////////////////////////////////////////////////////////////////////////////////////
            // Construction/Deconstruction
            Lua51ScriptingEngine::Lua51ScriptingEngine(Lua51Libraries LibrariesToOpen) : State(luaL_newstate())
                { OpenLibraries(LibrariesToOpen); }

            Lua51ScriptingEngine::~Lua51ScriptingEngine()
                { lua_close(State); }

            ///////////////////////////////////////////////////////////////////////////////////////
            // Execution
            CountedPtr<iScript> Lua51ScriptingEngine::Execute(const String& ScriptSource)
            {
                CountedPtr<Lua51Script> Results = Compile(ScriptSource);
                Execute(Results);
                return CountedPtrCast<iScript>(Results);
            }

            void Lua51ScriptingEngine::Execute(CountedPtr<iScript>& ScriptToRun)
            {
                CountedPtr<Lua51Script> ScriptToCompile = CountedPtrCast<Lua51Script>(ScriptToRun);
                if(ScriptToCompile)
                {
                    Execute(ScriptToCompile);
                }else{
                    MEZZ_EXCEPTION(Exception::PARAMETERS_CAST_EXCEPTION, "Lua51 Engine attempted to execute a script, but it did not appear to bea Lua51 script.")
                }
            }

            void Lua51ScriptingEngine::Execute(CountedPtr<Lua51Script>& ScriptToRun)
                { Execute(ScriptToRun.Get()); }

            void Lua51ScriptingEngine::Execute(Lua51Script& ScriptToRun)
                { Execute(&ScriptToRun); }

            void Lua51ScriptingEngine::Execute(Lua51Script* ScriptToRun)
            {
                if(ScriptToRun->FunctionCall)
                {
                    lua_getglobal(this->State,ScriptToRun->SourceCode.c_str());
                }else{
                    if(!ScriptToRun->IsCompiled())
                        { Compile(ScriptToRun); }
                    else
                    {
                        ThrowFromLuaErrorCode(
                            lua_load(this->State, LuaBytecodeLoader, &ScriptToRun->GetByteCodeReference(), DefaultChunkName)
                        );
                    }
                    // Since Lua_Dump or lua_load will leave the function on the stack then...
                }

                // We just need to push all the arguments
                LuaArgument* Current;
                for(ArgumentGroup::const_iterator Iter = ScriptToRun->Args.begin();
                    Iter != ScriptToRun->Args.end();
                    Iter++ )
                {
                    Current = dynamic_cast<LuaArgument*>(Iter->Get());
                    if(Current)
                        { Current->Push(this->State); }
                    else
                        { MEZZ_EXCEPTION(Exception::PARAMETERS_CAST_EXCEPTION, "A LuaArgument could not be converted as one for parameter purposes.") }
                }

                // Do the actual script
                ThrowFromLuaErrorCode(
                    //lua_call(this->State, ScriptToRun->Args.size(), ScriptToRun->Returns.size() )
                    lua_pcall(this->State, ScriptToRun->Args.size(), ScriptToRun->Returns.size(), 0)
                );

                // Need to get return values
                for(ArgumentGroup::iterator Iter = ScriptToRun->Returns.begin();
                    Iter != ScriptToRun->Returns.end();
                    Iter++ )
                {
                    Current = dynamic_cast<LuaArgument*>(Iter->Get());
                    if(Current)
                        { Current->Pop(this->State); }
                    else
                        { MEZZ_EXCEPTION(Exception::PARAMETERS_CAST_EXCEPTION, "A LuaArgument could not be converted as one for return value purposes.") }
                }

            }

            ///////////////////////////////////////////////////////////////////////////////////////
            // Compilation
            CountedPtr<iScriptCompilable> Lua51ScriptingEngine::Compile(const String& SourceToCompile)
            {
                CountedPtr<Lua51Script> Results(
                                new Lua51Script(SourceToCompile,this)
                            );
                Compile(Results);
                return Results;
            }

            void Lua51ScriptingEngine::Compile(CountedPtr<iScriptCompilable>& ScriptToCompile)
            {
                CountedPtr<Lua51Script> ConvertedScript = CountedPtrCast<Lua51Script>(ScriptToCompile);
                if(ConvertedScript)
                {
                    Compile(ConvertedScript);
                }else{
                    MEZZ_EXCEPTION(Exception::PARAMETERS_CAST_EXCEPTION, "Lua51 Engine attempted to compile a script, but it did not appear to bea Lua51 script.")
                }
            }

            void Lua51ScriptingEngine::Compile(CountedPtr<Lua51Script>& ScriptToCompile)
                { Compile(ScriptToCompile.Get()); }

            void Lua51ScriptingEngine::Compile(Lua51Script& ScriptToCompile)
                { Compile(&ScriptToCompile); }

            void Lua51ScriptingEngine::Compile(Lua51Script* ScriptToCompile)
            {
                ThrowFromLuaErrorCode(
                            lua_load(this->State, LuaSourceLoader, ScriptToCompile, DefaultChunkName)
                );

                ThrowFromLuaErrorCode(
                            //lua_dump(this->State, LuaBytecodeDumper, &(ScriptToCompile->CompiledByteCode) )
                            lua_dump(this->State, LuaBytecodeDumper, &ScriptToCompile->GetByteCodeReference() )
                );
            }

            ///////////////////////////////////////////////////////////////////////////////////////
            // For Inheritance
            String Lua51ScriptingEngine::GetImplementationTypeName() const
                { return String("Lua51ScriptingEngine"); }

            ///////////////////////////////////////////////////////////////////////////////////////
            // Library Manipulation
            void Lua51ScriptingEngine::OpenLibraries(int LibrariesToOpen)
            {
                if(LibrariesToOpen & BaseLib)
                    { OpenBaseLibrary(); }
                if(LibrariesToOpen & PackageLib)
                    { OpenPackageLibrary(); }
                if(LibrariesToOpen & StringLib)
                    { OpenStringLibrary(); }
                if(LibrariesToOpen & TableLib)
                    { OpenTableLibrary(); }
                if(LibrariesToOpen & MathLib)
                    { OpenMathLibrary(); }
                if(LibrariesToOpen & IOLib)
                    { OpenIOLibrary(); }
                if(LibrariesToOpen & OSLib)
                    { OpenOSLibrary(); }
                if(LibrariesToOpen & DebugLib)
                    { OpenDebugLibrary(); }
                if(LibrariesToOpen & MezzLib)
                    { OpenMezzanineLibrary(); }
                if(LibrariesToOpen & MezzSafeLib)
                    { OpenMezzanineSafeLibrary(); }
            }

            void Lua51ScriptingEngine::OpenDefaultLibraries()
                { OpenLibraries(DefaultLibs); }
            void Lua51ScriptingEngine::OpenAllLibraries()
                { OpenLibraries(AllLibs); }

            void Lua51ScriptingEngine::OpenBaseLibrary()
                { luaopen_base(State); }
            void Lua51ScriptingEngine::OpenPackageLibrary()
                { luaopen_package(State); }
            void Lua51ScriptingEngine::OpenStringLibrary()
                { luaopen_string(State); }
            void Lua51ScriptingEngine::OpenTableLibrary()
                { luaopen_table(State); }
            void Lua51ScriptingEngine::OpenMathLibrary()
                { luaopen_math(State); }
            void Lua51ScriptingEngine::OpenIOLibrary()
                { luaopen_io(State); }
            void Lua51ScriptingEngine::OpenOSLibrary()
                { luaopen_os(State); }
            void Lua51ScriptingEngine::OpenDebugLibrary()
                { luaopen_os(State); }
            void Lua51ScriptingEngine::OpenMezzanineLibrary()
                { luaopen_Mezzanine(State); }
            void Lua51ScriptingEngine::OpenMezzanineSafeLibrary()
                { luaopen_MezzanineSafe(State); }

        } // Lua
    } // Scripting
} // Mezzanine




#endif //  MEZZLUA51
#endif
