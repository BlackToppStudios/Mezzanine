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
#ifndef _lua51scriptingengine_cpp
#define _lua51scriptingengine_cpp

#include "datatypes.h"

#ifdef MEZZLUA51

#include "lua51script.h"
#include "lua51scriptargument.h"
#include "lua51scriptingengine.h"
#include "exception.h"
#include "stringtool.h"

#include <cstring>
#include <cctype>
#include <algorithm>

/// @file
/// @brief This file has the implementation for the Lua based Scripting system.

extern "C"
{
    #include "lua.h"            // Lua Core
    #include "lualib.h"         // for opening the base state
    #include "lauxlib.h"        // Extra Lua Goodies like lua_open()

    int luaopen_Mezzanine(lua_State* L);
    int luaopen_MezzanineSafe(lua_State* L);

    int luaopen_MezzanineXML(lua_State* L);
    int luaopen_MezzanineXMLSafe(lua_State* L);

    int luaopen_MezzanineThreading(lua_State* L);
    int luaopen_MezzanineThreadingSafe(lua_State* L);
}

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
                if(0==LuaReturn)
                    { return; }

                String ErrorMessage;
                size_t Length;
                const char* ErrorCString = lua_tolstring(this->State, -1, &Length);
                ErrorMessage.reserve(Length+2);
                ErrorMessage = String(ErrorCString);
                lua_pop(State,1);
                if(ErrorMessage.length()!=Length)
                    { MEZZ_EXCEPTION(Exception::SCRIPT_EXCEPTION_LUA, "Lua is putting odd things in error messages:\n"+ErrorMessage); }
                ErrorMessage += "\n";

                switch(LuaReturn)
                {
                    case LUA_YIELD:
                        MEZZ_EXCEPTION(Exception::SCRIPT_EXCEPTION_LUA_YIELD, "Lua returned a LUA_YIELD instead of completing:\n"+ErrorMessage);
                    case LUA_ERRRUN:
                        MEZZ_EXCEPTION(Exception::SCRIPT_EXCEPTION_LUA_RUNTIME, "There was a runtime Error handling the Lua script:\n"+ErrorMessage);
                    case LUA_ERRSYNTAX:
                        MEZZ_EXCEPTION(Exception::SYNTAX_ERROR_EXCEPTION_LUA, "There was an error with the syntax of the Lua script:\n"+ErrorMessage);
                    case LUA_ERRERR:
                        MEZZ_EXCEPTION(Exception::SCRIPT_EXCEPTION_LUA_ERRERR, "There was an error when Lua attempted to handle an error:\n"+ErrorMessage);
                    case LUA_ERRMEM:
                        MEZZ_EXCEPTION(Exception::MM_OUT_OF_MEMORY_EXCEPTION, "Lua could not allocate memory:\n"+ErrorMessage);
                    case LUA_ERRFILE:
                        MEZZ_EXCEPTION(Exception::IO_FILE_EXCEPTION, "Lua had an error with file IO:\n"+ErrorMessage);
                    default:
                        MEZZ_EXCEPTION(Exception::SCRIPT_EXCEPTION_LUA, "Lua had an error and we are not sure what it was:\n"+ErrorMessage);
                }
            }

            const String Lua51ScriptingEngine::NoLibName                   = "None";
            const String Lua51ScriptingEngine::BaseLibName                 = "Base";
            const String Lua51ScriptingEngine::PackageLibName              = "Package";
            const String Lua51ScriptingEngine::StringLibName               = "String";
            const String Lua51ScriptingEngine::TableLibName                = "Table";
            const String Lua51ScriptingEngine::MathLibName                 = "Math";
            const String Lua51ScriptingEngine::IOLibName                   = "IO";
            const String Lua51ScriptingEngine::OSLibName                   = "OS";
            const String Lua51ScriptingEngine::DebugLibName                = "Debug";
            const String Lua51ScriptingEngine::MezzLibName                 = "Mezzanine";
            const String Lua51ScriptingEngine::MezzSafeLibName             = "MezzanineSafe";
            const String Lua51ScriptingEngine::MezzXMLLibName              = "MezzanineXML";
            const String Lua51ScriptingEngine::MezzXMLSafeLibName          = "MezzanineXMLSafe";
            const String Lua51ScriptingEngine::MezzThreadingLibName        = "MezzanineThreading";
            const String Lua51ScriptingEngine::MezzThreadingSafeLibName    = "MezzanineThreadingSafe";
            const String Lua51ScriptingEngine::DefaultLibsName             = "Default";
            const String Lua51ScriptingEngine::AllLibsName                 = "All";

            const String Lua51ScriptingEngine::BaseTableName               = "coroutine";
            const String Lua51ScriptingEngine::PackageTableName            = "package";
            const String Lua51ScriptingEngine::StringTableName             = "string";
            const String Lua51ScriptingEngine::TableTableName              = "table";
            const String Lua51ScriptingEngine::MathTableName               = "math";
            const String Lua51ScriptingEngine::IOTableName                 = "io";
            const String Lua51ScriptingEngine::OSTableName                 = "os";
            const String Lua51ScriptingEngine::DebugTableName              = "debug";
            const String Lua51ScriptingEngine::MezzTableName               = "Mezzanine";
            const String Lua51ScriptingEngine::MezzSafeTableName           = "MezzanineSafe";
            const String Lua51ScriptingEngine::MezzXMLTableName            = "MezzanineXML";
            const String Lua51ScriptingEngine::MezzXMLSafeTableName        = "MezzanineXMLSafe";
            const String Lua51ScriptingEngine::MezzThreadingTableName      = "MezzanineThreading";
            const String Lua51ScriptingEngine::MezzThreadingSafeTableName  = "MezzanineThreadingSafe";

            const String Lua51ScriptingEngine::TypeNameNil                 = "Nil";
            const String Lua51ScriptingEngine::TypeNameBoolean             = "Boolean";
            const String Lua51ScriptingEngine::TypeNameLightUserData       = "Light User Data";
            const String Lua51ScriptingEngine::TypeNameNumber              = "Number";
            const String Lua51ScriptingEngine::TypeNameString              = "String";
            const String Lua51ScriptingEngine::TypeNameTable               = "Table";
            const String Lua51ScriptingEngine::TypeNameFunction            = "Function";
            const String Lua51ScriptingEngine::TypeNameUserData            = "User Data";
            const String Lua51ScriptingEngine::TypeNameThread              = "Thread";

            const String& Lua51ScriptingEngine::GetLibName(Lua51ScriptingEngine::Lua51Libraries Lib)
            {
                switch(Lib)
                {
                    case NoLib:                 return NoLibName;
                    case BaseLib:               return BaseLibName;
                    case PackageLib:            return PackageLibName;
                    case StringLib:             return StringLibName;
                    case TableLib:              return TableLibName;
                    case MathLib:               return MathLibName;
                    case IOLib:                 return IOLibName;
                    case OSLib:                 return OSLibName;
                    case DebugLib:              return DebugLibName;
                    case MezzLib:               return MezzLibName;
                    case MezzSafeLib:           return MezzSafeLibName;
                    case MezzXMLLib:            return MezzXMLLibName;
                    case MezzXMLSafeLib:        return MezzXMLSafeLibName;
                    case MezzThreadingLib:      return MezzThreadingLibName;
                    case MezzThreadingSafeLib:  return MezzThreadingSafeLibName;
                    case DefaultLibs:           return DefaultLibsName;
                    case AllLibs:               return AllLibsName;
                    default: MEZZ_EXCEPTION(Exception::PARAMETERS_RANGE_EXCEPTION, "Cannot convert given value to library string: "+ToString(Lib));
                }
            }

            const String& Lua51ScriptingEngine::GetTableName(Lua51ScriptingEngine::Lua51Libraries Lib)
            {
                switch(Lib)
                {
                    case BaseLib:               return BaseTableName;
                    case PackageLib:            return PackageTableName;
                    case StringLib:             return StringTableName;
                    case TableLib:              return TableTableName;
                    case MathLib:               return MathTableName;
                    case IOLib:                 return IOTableName;
                    case OSLib:                 return OSTableName;
                    case DebugLib:              return DebugTableName;
                    case MezzLib:               return MezzTableName;
                    case MezzSafeLib:           return MezzSafeTableName;
                    case MezzXMLLib:            return MezzXMLTableName;
                    case MezzXMLSafeLib:        return MezzXMLSafeTableName;
                    case MezzThreadingLib:      return MezzThreadingTableName;
                    case MezzThreadingSafeLib:  return MezzThreadingSafeTableName;
                    default: MEZZ_EXCEPTION(Exception::PARAMETERS_RANGE_EXCEPTION, "Cannot convert given value to table string: "+ToString(Lib));
                }
            }

            Lua51ScriptingEngine::Lua51Libraries Lua51ScriptingEngine::GetLibFromName(String Name)
            {
                if(!Name.size())
                    { MEZZ_EXCEPTION(Exception::PARAMETERS_RANGE_EXCEPTION, "Cannot convert zero length name to valid lua library name"); }
                using namespace Mezzanine::StringTools;
                ToLowerCase(Name);

                switch(Name[0])
                {
                    case 'n': if(Name==LowerCaseCopy(NoLibName))       { return NoLib; }       else { MEZZ_EXCEPTION(Exception::PARAMETERS_RANGE_EXCEPTION, "Could not convert name starting with n: " + Name); }
                    case 'b': if(Name==LowerCaseCopy(BaseLibName))     { return BaseLib; }     else { MEZZ_EXCEPTION(Exception::PARAMETERS_RANGE_EXCEPTION, "Could not convert name starting with b: " + Name); }
                    case 'p': if(Name==LowerCaseCopy(PackageLibName))  { return PackageLib; }  else { MEZZ_EXCEPTION(Exception::PARAMETERS_RANGE_EXCEPTION, "Could not convert name starting with p: " + Name); }
                    case 's': if(Name==LowerCaseCopy(StringLibName))   { return StringLib; }   else { MEZZ_EXCEPTION(Exception::PARAMETERS_RANGE_EXCEPTION, "Could not convert name starting with s: " + Name); }
                    case 't': if(Name==LowerCaseCopy(TableLibName))    { return TableLib; }    else { MEZZ_EXCEPTION(Exception::PARAMETERS_RANGE_EXCEPTION, "Could not convert name starting with t: " + Name); }
                    case 'i': if(Name==LowerCaseCopy(IOLibName))       { return IOLib; }       else { MEZZ_EXCEPTION(Exception::PARAMETERS_RANGE_EXCEPTION, "Could not convert name starting with i: " + Name); }
                    case 'o': if(Name==LowerCaseCopy(OSLibName))       { return OSLib; }       else { MEZZ_EXCEPTION(Exception::PARAMETERS_RANGE_EXCEPTION, "Could not convert name starting with o: " + Name); }
                    case 'a': if(Name==LowerCaseCopy(AllLibsName))     { return AllLibs; }     else { MEZZ_EXCEPTION(Exception::PARAMETERS_RANGE_EXCEPTION, "Could not convert name starting with a: " + Name); }
                    case 'm':
                        if     (Name==LowerCaseCopy(MathLibName))              { return MathLib; }
                        else if(Name==LowerCaseCopy(MezzLibName))              { return MezzLib; }
                        else if(Name==LowerCaseCopy(MezzSafeLibName))          { return MezzSafeLib; }
                        else if(Name==LowerCaseCopy(MezzXMLLibName))           { return MezzXMLLib; }
                        else if(Name==LowerCaseCopy(MezzXMLSafeLibName))       { return MezzXMLSafeLib; }
                        else if(Name==LowerCaseCopy(MezzThreadingLibName))     { return MezzThreadingLib; }
                        else if(Name==LowerCaseCopy(MezzThreadingSafeLibName)) { return MezzThreadingSafeLib; }
                        else { MEZZ_EXCEPTION(Exception::PARAMETERS_RANGE_EXCEPTION, "Could not convert name starting with m: " + Name); }
                    case 'd':
                        if     (Name==LowerCaseCopy(DebugLibName))             { return DebugLib; }
                        else if(Name==LowerCaseCopy(DefaultLibsName))          { return DefaultLibs; }
                        else { MEZZ_EXCEPTION(Exception::PARAMETERS_RANGE_EXCEPTION, "Could not convert name starting with d: " + Name); }
                    default: MEZZ_EXCEPTION(Exception::PARAMETERS_RANGE_EXCEPTION, "Could not convert: " + Name);
                }
            }

            ///////////////////////////////////////////////////////////////////////////////////////
            // Construction/Deconstruction
            Lua51ScriptingEngine::Lua51ScriptingEngine(Lua51Libraries LibrariesToOpen) : State(luaL_newstate())
            {
                if(NULL==State)
                    { MEZZ_EXCEPTION(Exception::MM_OUT_OF_MEMORY_EXCEPTION, "Could not allocate Memory for Lua interpretter"); }
                OpenLibraries(LibrariesToOpen);
            }

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
                            lua_load(this->State, LuaBytecodeLoader, &ScriptToRun->GetByteCodeReference(), ScriptToRun->GetName().c_str())
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
                //Compile(Results); // It seems the Luq51Script constructor compiles just fine
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
                            lua_load(this->State, LuaSourceLoader, ScriptToCompile, ScriptToCompile->GetName().c_str())
                );
                ThrowFromLuaErrorCode(
                            //lua_dump(this->State, LuaBytecodeDumper, &(ScriptToCompile->CompiledByteCode) )
                            lua_dump(this->State, LuaBytecodeDumper, &ScriptToCompile->GetByteCodeReference() )
                );
                lua_pop(State,1);
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
                if(LibrariesToOpen & MezzXMLLib)
                    { OpenMezzanineXMLLibrary(); }
                if(LibrariesToOpen & MezzXMLSafeLib)
                    { OpenMezzanineXMLSafeLibrary(); }
                if(LibrariesToOpen & MezzThreadingLib)
                    { OpenMezzanineThreadingLibrary(); }
                if(LibrariesToOpen & MezzThreadingSafeLib)
                    { OpenMezzanineThreadingSafeLibrary(); }
            }

            Boole Lua51ScriptingEngine::IsLibraryOpen(Lua51Libraries LibToCheck)
            {
                lua_getglobal(State, GetTableName(LibToCheck).c_str());
                if (lua_istable(State, -1))
                {
                    lua_pop(State,1);
                    return true;
                }
                lua_pop(State,1);
                return false;
            }

            void Lua51ScriptingEngine::AliasLibrary(const String& Base, const String& Sub, const String& Alias)
            {
                lua_getglobal(State, Base.c_str());
                if (lua_istable(State, -1))
                {
                    lua_pushstring(State, Alias.c_str());
                    lua_getglobal(State, Sub.c_str());
                    lua_settable(State, -3); // Set the table a -3, Mezzanine to have the index defined by -2 "XML" set to the value at -1 "The MezzanineXML Table"
                    lua_pop(State,1);
                } //else Fail Silently
            }

            void Lua51ScriptingEngine::OpenDefaultLibraries()
                { OpenLibraries(DefaultLibs); }
            void Lua51ScriptingEngine::OpenAllLibraries()
                { OpenLibraries(AllLibs); }

            void Lua51ScriptingEngine::OpenBaseLibrary()
            {
                lua_pushcfunction(State, luaopen_base);
                lua_pushliteral(State, LUA_COLIBNAME);
                lua_call(State, 1, 0);
            }
            void Lua51ScriptingEngine::OpenPackageLibrary()
            {
                lua_pushcfunction(State, luaopen_package);
                lua_pushliteral(State, LUA_LOADLIBNAME);
                lua_call(State, 1, 0);
            }
            void Lua51ScriptingEngine::OpenStringLibrary()
            {
                lua_pushcfunction(State, luaopen_string);
                lua_pushliteral(State, LUA_STRLIBNAME);
                lua_call(State, 1, 0);
            }
            void Lua51ScriptingEngine::OpenTableLibrary()
            {
                lua_pushcfunction(State, luaopen_table);
                lua_pushliteral(State, LUA_TABLIBNAME);
                lua_call(State, 1, 0);
            }
            void Lua51ScriptingEngine::OpenMathLibrary()
            {
                lua_pushcfunction(State, luaopen_math);
                lua_pushliteral(State, LUA_MATHLIBNAME);
                lua_call(State, 1, 0);
            }
            void Lua51ScriptingEngine::OpenIOLibrary()
            {
                lua_pushcfunction(State, luaopen_io);
                lua_pushliteral(State, LUA_IOLIBNAME);
                lua_call(State, 1, 0);
            }
            void Lua51ScriptingEngine::OpenOSLibrary()
            {
                lua_pushcfunction(State, luaopen_os);
                lua_pushliteral(State, LUA_OSLIBNAME);
                lua_call(State, 1, 0);
            }
            void Lua51ScriptingEngine::OpenDebugLibrary()
            {
                lua_pushcfunction(State, luaopen_debug);
                lua_pushliteral(State, LUA_DBLIBNAME);
                lua_call(State, 1, 0);
            }

            void Lua51ScriptingEngine::OpenMezzanineLibrary()
            {
                lua_pushcfunction(State, luaopen_Mezzanine);
                lua_pushstring(State, (MezzLibName.c_str()) );
                lua_call(State, 1, 0);
                SetXML();
            }
            void Lua51ScriptingEngine::OpenMezzanineSafeLibrary()
            {
                lua_pushcfunction(State, luaopen_MezzanineSafe);
                lua_pushstring(State, (MezzSafeLibName.c_str()) );
                lua_call(State, 1, 0);
                SetXMLSafe();
            }

            void Lua51ScriptingEngine::OpenMezzanineXMLLibrary()
            {
                lua_pushcfunction(State, luaopen_MezzanineXML);
                lua_pushstring(State, (MezzXMLLibName.c_str()) );
                lua_call(State, 1, 0);
                SetXML();
            }

            void Lua51ScriptingEngine::OpenMezzanineXMLSafeLibrary()
            {
                lua_pushcfunction(State, luaopen_MezzanineXMLSafe);
                lua_pushstring(State, (MezzXMLSafeLibName.c_str()) );
                lua_call(State, 1, 0);
                SetXMLSafe();
            }

            void Lua51ScriptingEngine::OpenMezzanineThreadingLibrary()
            {
                lua_pushcfunction(State, luaopen_MezzanineThreading);
                lua_pushstring(State, (MezzThreadingLibName.c_str()) );
                lua_call(State, 1, 0);
                SetThreading();
            }
            void Lua51ScriptingEngine::OpenMezzanineThreadingSafeLibrary()
            {
                lua_pushcfunction(State, luaopen_MezzanineThreadingSafe);
                lua_pushstring(State, (MezzThreadingSafeLibName.c_str()) );
                lua_call(State, 1, 0);
                SetThreadingSafe();
            }

            void Lua51ScriptingEngine::SetXML()
                { AliasLibrary("Mezzanine", "MezzanineXML", "XML"); }
            void Lua51ScriptingEngine::SetXMLSafe()
                { AliasLibrary("MezzanineSafe", "MezzanineXMLSafe", "XML"); }

            void Lua51ScriptingEngine::SetThreading()
                { AliasLibrary("Mezzanine", "MezzanineThreading", "Threading"); }
            void Lua51ScriptingEngine::SetThreadingSafe()
                { AliasLibrary("MezzanineSafe", "MezzanineThreadingSafe", "Threading"); }

            lua_State* Lua51ScriptingEngine::GetRawLuaState()
                { return State; }

            int Lua51ScriptingEngine::GetStackCount()
                { return lua_gettop(State); }

            bool Lua51ScriptingEngine::IsValidCharStartIdentifier(const char IdChar)
                { return isalpha(IdChar) || IdChar=='_'; }

            bool Lua51ScriptingEngine::IsValidCharInIdentifier(const char IdChar)
                { return isalnum(IdChar) || IdChar=='_'; }

            bool Lua51ScriptingEngine::IsValidIdentifier(const String& Id)
            {
                String::const_iterator Iter = Id.begin();
                if( Id.size()==0 || Iter==Id.end() || !IsValidCharStartIdentifier(*Iter) )
                    { return false; }
                for(Iter++; Iter!=Id.end(); Iter++)
                {
                    if(!IsValidCharInIdentifier(*Iter))
                        { return false; }
                }
                return true;
            }

            void Lua51ScriptingEngine::PopulateTabCompletionTrie(CommandTrie& CommandGroup)
            {
                CommandGroup.insert("print", &Scripting::Lua::Lua51ScriptingEngine::TypeNameFunction);
                CommandGroup.insert("pi", &Scripting::Lua::Lua51ScriptingEngine::TypeNameNumber);
                CommandGroup.insert("package", &Scripting::Lua::Lua51ScriptingEngine::TypeNameTable);
                CommandGroup.insert("package.load", &Scripting::Lua::Lua51ScriptingEngine::TypeNameFunction);
                CommandGroup.insert("package.require", &Scripting::Lua::Lua51ScriptingEngine::TypeNameFunction);
                CommandGroup.insert("package.os", &Scripting::Lua::Lua51ScriptingEngine::TypeNameFunction);
                CommandGroup.insert("package.fake", &Scripting::Lua::Lua51ScriptingEngine::TypeNameFunction);
                CommandGroup.insert("package.bs", &Scripting::Lua::Lua51ScriptingEngine::TypeNameFunction);
                CommandGroup.insert("package.a", &Scripting::Lua::Lua51ScriptingEngine::TypeNameFunction);
                CommandGroup.insert("package.load2", &Scripting::Lua::Lua51ScriptingEngine::TypeNameFunction);
                CommandGroup.insert("package.mezzanine", &Scripting::Lua::Lua51ScriptingEngine::TypeNameFunction);
            }

/*
            String Lua51ScriptingEngine::tests(String Returns)
            {
                int Top = lua_gettop(State);
                if(0==Top)
                    { return Returns;}

                Returns += " " + ToString(lua_type(State,-1)) + ":";

                if(LUA_TNIL==lua_type(State,Top))
                {
                    Returns += " nil";
                    lua_pop(State,1);
                    return tests(Returns);
                }

                if(LUA_TBOOLEAN==lua_type(State,Top))
                {
                    Returns += " Boole:";
                    Returns += ToString(lua_toboolean(State,Top));
                    lua_pop(State,1);
                    return tests(Returns);
                }

                if(LUA_TLIGHTUSERDATA==lua_type(State,Top))
                {
                    Returns += " light_data";
                    lua_pop(State,1);
                    return tests(Returns);
                }

                if(LUA_TNUMBER==lua_type(State,Top))
                {
                    Returns += " Number:";
                    Returns += ToString(lua_tonumber(State,Top));
                    lua_pop(State,1);
                    return tests(Returns);
                }

                if(LUA_TSTRING==lua_type(State,Top))
                {
                    Returns += " String:";
                    Returns += ToString(lua_tostring(State,Top));
                    lua_pop(State,1);
                    return tests(Returns);
                }

                if(LUA_TTABLE==lua_type(State,Top))
                {
                    Returns += " table";
                    lua_pop(State,1);
                    return tests(Returns);
                }

                if(LUA_TFUNCTION==lua_type(State,Top))
                {
                    Returns += " Function";
                    lua_pop(State,1);
                    return tests(Returns);
                }

                if(LUA_TUSERDATA==lua_type(State,Top))
                {
                    Returns += " user_data";
                    lua_pop(State,1);
                    return tests(Returns);
                }

                if(LUA_TTHREAD==lua_type(State,Top))
                {
                    Returns += " thread";
                    lua_pop(State,1);
                    return tests(Returns);
                }

                //    #define LUA_TNIL		0
                //    #define LUA_TBOOLEAN		1
                //    #define LUA_TLIGHTUSERDATA	2
                //    #define LUA_TNUMBER		3
                //    #define LUA_TSTRING		4
                //    #define LUA_TTABLE		5
                //    #define LUA_TFUNCTION		6
                //    #define LUA_TUSERDATA		7
                //    #define LUA_TTHREAD		8

                return Returns;
            }*/

        } // Lua
    } // Scripting
} // Mezzanine


#endif //  MEZZLUA51
#endif
