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
#ifndef _lua51scriptingengine_h
#define _lua51scriptingengine_h

#include "datatypes.h"

#ifdef MEZZLUA51

#include "Scripting/scriptargument.h"
#include "Scripting/script.h"
#include "Scripting/scriptingmanager.h"

/// @file
/// @brief This file has the interface for the Lua based implementation of the Scripting system.

/// @page LuaManual Mezzanine::Scripting Lua Manual
/// The file that SWIG generated was made with the following command run on an ubuntu machine from the Mezzananine/src directory:
/// swig2.0 -c++ -v -Wall -lua -importall -includeall -o scriptbindinglua51.cpp mezzanine.h
///
/// Need to document "Mezzanine.", nspaces, calling conventions
///
/// script class compilation

struct lua_State;

namespace Mezzanine
{
    // must implement
        // ScriptingManagerLua
        // ScriptLua
        // ScriptArgLua
    namespace Scripting
    {
        class iScriptCompilationManager;
        class Lua51Script;

        /// @brief This contains the Lua51 and Lua52 resources.
        namespace Lua
        {
            ///////////////////////////////////////////////////////////////////////////////////////
            //
            class MEZZ_LIB Lua51ScriptingEngine : public Mezzanine::Scripting::iScriptCompilationManager
            {
                private:
                    // Makes passing internal data much easier and all Lua51 are logically encapsulated as a single system still.
                    friend class Lua51Script;

                    /// @brief The current state of the Lua runtime.
                    lua_State *State;

                protected:
                    /// @brief Performs the compilation on a raw pointer, only used internally
                    /// @param ScriptToCompile A pointer to the Lua51Script to compile.
                    virtual void Compile(Lua51Script* ScriptToCompile);

                    /// @brief Performs the compilation on a raw pointer, only used internally
                    /// @param ScriptToCompile A pointer to the Lua51Script to compile.
                    virtual void Execute(Lua51Script* ScriptTorun);

                    /// @brief This will do nothing if the past integer
                    /// @param LuaReturn The return code from a Lua Compile or execution call
                    /// @throws This Throws ScriptLuaYieldException, ScriptLuaRuntimeException, ScriptLuaRuntimeException, ScriptLuaErrErrException, SyntaxErrorLuaException, OutOfMemoryException, FileException, ScriptLuaException with as much precision as possible when thrown.
                    virtual void ThrowFromLuaErrorCode(int LuaReturn);

                public:
                    /// @brief Intended only to make constructing an @ref Lua51ScriptingEngine with the desired libraries open a little easier.
                    enum Lua51Libraries
                    {
                        BaseLib        = 1,      ///< Correlates to @ref Lua51ScriptingEngine::OpenBaseLibrary
                        PackageLib     = 2,      ///< Correlates to @ref Lua51ScriptingEngine::OpenPackageLibrary
                        StringLib      = 4,      ///< Correlates to @ref Lua51ScriptingEngine::OpenStringLibrary
                        TableLib       = 8,      ///< Correlates to @ref Lua51ScriptingEngine::OpenTableLibrary
                        MathLib        = 16,     ///< Correlates to @ref Lua51ScriptingEngine::OpenMathLibrary
                        IOLib          = 32,     ///< Correlates to @ref Lua51ScriptingEngine::OpenIOLibrary
                        OSLib          = 64,     ///< Correlates to @ref Lua51ScriptingEngine::OpenOSLibrary
                        DebugLib       = 128,    ///< Correlates to @ref Lua51ScriptingEngine::OpenDebugLibrary
                        MezzLib        = 256,    ///< Correlates to @ref Lua51ScriptingEngine::OpenMezzanineLibrary
                        MezzSafeLib    = 512,    ///< Correlates to @ref Lua51ScriptingEngine::OpenMezzanineSafeLibrary

                        DefaultLibs    = BaseLib | StringLib | TableLib | MathLib | MezzSafeLib, ///< A quick way to refer to all the libraries opened by @ref Lua51ScriptingEngine::OpenMezzanineSafeLibrary
                        AllLibs        = BaseLib | PackageLib | StringLib | TableLib | MathLib | IOLib | OSLib | DebugLib | MezzLib ///< A quick way to refer to all the libraries opened by @ref Lua51ScriptingEngine::OpenDefaultLibraries
                    };

                    /// @brief Constructs a Scripting engine with a set of libraries preloaded.
                    /// @param LibrariesToOpen A Lua51Libraries bitmap indicating which libraries to load, this defaults to DefaultLibs
                    explicit Lua51ScriptingEngine(Lua51Libraries LibrariesToOpen=DefaultLibs);

                    /// @brief Virtual Deconstructor
                    virtual ~Lua51ScriptingEngine();

                    /// @brief Compile and execute a passed string.
                    /// @param ScriptSource A String containing the source code to be executed.
                    /// @details This will create a CountPtr to a Lua51Script and assign both its Source and Byte code
                    virtual CountedPtr<iScript> Execute(const String& ScriptSource);

                    /// @brief This will execute the passed script, compiling it if not present
                    /// @param ScriptToRun The script to execute.
                    /// @details If a bytecode is present on ScriptToRun then it is executed. Otherwise the Source is
                    /// compiled and the result is set as the bytecode and it is executed.
                    virtual void Execute(CountedPtr<iScript> ScriptToRun);

                    /// @brief Accepts an Counted ptr to a script and compiles it.
                    /// @param ScriptToCompile The CountedPtr to compile
                    /// @details The ByteCode member on the passed script is erased, if present,
                    /// and sets it to corresponding lua binary.
                    virtual void Compile(CountedPtr<iScriptCompilable> ScriptToCompile);

                    /// @brief Calls Compile(CountedPtr<iScriptCompilable>) and returns a CountedPtr to the script created.
                    /// @param SourceToCompile A string Containing valid lua source code.
                    /// @return A CountedPtr<iScriptCompilable> pointing to a created Script object that contains the source and compile binary.
                    virtual CountedPtr<iScriptCompilable> Compile(const String& SourceToCompile);

                    /// @copydoc ManagerBase::GetImplementationTypeName()
                    /// @return A String containing "Lua51ScriptingEngine"
                    virtual String GetImplementationTypeName() const;


                    virtual void Execute(CountedPtr<Lua51Script> ScriptToRun);

                    virtual void Compile(CountedPtr<Lua51Script> ScriptToCompile);

                    virtual void OpenLibraries(int LibrariesToOpen);

                    /// @brief Prepare most Mezzanine and some Lua functionality for use in Lua scripts.
                    /// @details This will load a the Lua Base, String, Table and Math libraries. This
                    /// will make Mezzanine functionality available that does not provide any arbitrary file
                    /// output or execution abilities.
                    /// @warning This enables lua scripts to load and execute arbitrary Lua scripts.
                    virtual void OpenDefaultLibraries();

                    /// @brief Make all Mezzanine and Lua functionality
                    /// @warning This makes arbitrary execution of programs and file IO available to scripts. This is not suitable if untrusted scripts will be run.
                    virtual void OpenAllLibraries();

                    /// @brief Make some eof the more core functionality available to lua scripts.
                    /// @details See the Lua manual at http://www.lua.org/manual/5.1/manual.html#5.1 and http://www.lua.org/manual/5.1/manual.html#5.2 for further details on the
                    /// functionality this enables.
                    /// @warning This enables lua scripts to load and execute arbitrary Lua scripts.
                    virtual void OpenBaseLibrary();

                    /// @brief Allow Lua scripts to load other libraries.
                    /// @details see the Lua manual at http://www.lua.org/manual/5.1/manual.html#5.3 for details on what this enables. This does allow Lua scripts to load
                    /// other libraries, inlcuding the IO or OS libraries.
                    /// @warning This can makes full OS and file IO available to scripts. This is not suitable if untrusted scripts will be run.
                    virtual void OpenPackageLibrary();

                    /// @brief Allow Lua scripts access to the Lua string manipulation libary.
                    /// @details The details of what this enables can be read about in the Lua manual at http://www.lua.org/manual/5.1/manual.html#5.4 .
                    virtual void OpenStringLibrary();

                    /// @brief Enable Lua table manipulation functionality.
                    /// @details See the Lua manual at http://www.lua.org/manual/5.1/manual.html#5.5 for details.
                    virtual void OpenTableLibrary();

                    /// @brief Enable Lua math and random number functionality
                    /// @details The Lua manual at http://www.lua.org/manual/5.1/manual.html#5.6 as further details.
                    virtual void OpenMathLibrary();

                    /// @brief Enable Input/Output in lua scripts for reading and writing files
                    /// @detail See http://www.lua.org/manual/5.1/manual.html#5.7 in the Lua manual for details
                    /// @warning This makes file IO available to scripts. This is not suitable if untrustedscripts will be run.
                    virtual void OpenIOLibrary();

                    /// @brief EnableOS facilities in lua scriptsm such as file managements, time and shell execution.
                    /// @detail See http://www.lua.org/manual/5.1/manual.html#5.7 in the Lua manual for details
                    /// @warning This makes arbitrary execution of programs and file management available to scripts. This is not suitable if untrusted scripts will be run.
                    virtual void OpenOSLibrary();

                    /// @brief Enable Debugging Lua features.
                    /// @details This is the only Lua library omitted from the Defaults for functionality rather than security reasons. This
                    /// unimportant for most game uses and is primarily include for completeness. More details in the
                    /// Lua manuak at http://www.lua.org/manual/5.1/manual.html#5.9 .
                    virtual void OpenDebugLibrary();


                    virtual void OpenMezzanineLibrary();
                    virtual void OpenMezzanineSafeLibrary();
            };

            //simplistic error checking function, to be replace with proper exception driven code later.
            int MEZZ_LIB PrintErrorMessageOrNothing(int ErrorCode);

            // super simple lua test
            int MEZZ_LIB test();

        } // Lua
    } // Scripting
} // Mezzanine


#endif // MEZZLUA51
#endif // \_scriptinglua_h
