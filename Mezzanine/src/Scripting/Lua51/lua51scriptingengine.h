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
/// swig2.0 -c++ -v -Wall -lua -importall -includeall -o Scripting/Lua51/scriptbindinglua51.cpp mezzanine.h
/// swig2.0 -c++ -v -Wall -lua -importall -includeall -DSWIG_UNSAFE -o Scripting/Lua51/scriptbindinglua51unsafe.cpp mezzanine.h
/// Need to document "Mezzanine.", nspaces, calling conventions
///
/// script class compilation
///
/// This attempts to create "safe" and "open" version Lua binding to the Mezzanine. The safe version should be suitable
/// for scripts that can be trusted with the state of the simulation, but not the state of of the calling system. Said
/// another way, script run against the "safe" version of the runtime cannot call function or create objects that load
/// modules, read files, write files, access a system shell or access any Mezzanine facility that can indirectly grant
/// access to features like these (the LuaScriptingEngine for example.)

struct lua_State;

namespace Mezzanine
{
    namespace Scripting
    {
        class iScriptCompilationManager;
        class Lua51Script;

        /// @brief This contains the Lua51 and Lua52 resources.
        namespace Lua
        {
            ///////////////////////////////////////////////////////////////////////////////////////
            /// @brief The workhorse of the Lua scripting system. All scripts come here to be executed.
            class MEZZ_LIB Lua51ScriptingEngine : public Mezzanine::Scripting::iScriptCompilationManager
            {
                ///////////////////////////////////////////////////////////////////////////////////////
                // Internal Stuff first
                private:
                    // Makes passing internal data much easier and all Lua51 are logically encapsulated as a single system still.
                    friend class Lua51Script;

                    /// @brief The current state of the Lua runtime.
                    lua_State *State;

                protected:
                    /// @brief This will do nothing if the past integer
                    /// @param LuaReturn The return code from a Lua Compile or execution call
                    /// @throws This Throws ScriptLuaYieldException, ScriptLuaRuntimeException, ScriptLuaRuntimeException, ScriptLuaErrErrException, SyntaxErrorLuaException, OutOfMemoryException, FileException, ScriptLuaException with as much precision as possible when thrown.
                    virtual void ThrowFromLuaErrorCode(int LuaReturn);

                public:
                    /// @brief Intended only to make constructing an @ref Lua51ScriptingEngine with the desired libraries open a little easier.
                    enum Lua51Libraries
                    {
                        NoLib                  = 0,         ///< No libraries
                        BaseLib                = 1,         ///< Correlates to @ref Lua51ScriptingEngine::OpenBaseLibrary
                        PackageLib             = 2,         ///< Correlates to @ref Lua51ScriptingEngine::OpenPackageLibrary
                        StringLib              = 4,         ///< Correlates to @ref Lua51ScriptingEngine::OpenStringLibrary
                        TableLib               = 8,         ///< Correlates to @ref Lua51ScriptingEngine::OpenTableLibrary
                        MathLib                = 16,        ///< Correlates to @ref Lua51ScriptingEngine::OpenMathLibrary
                        IOLib                  = 32,        ///< Correlates to @ref Lua51ScriptingEngine::OpenIOLibrary
                        OSLib                  = 64,        ///< Correlates to @ref Lua51ScriptingEngine::OpenOSLibrary
                        DebugLib               = 128,       ///< Correlates to @ref Lua51ScriptingEngine::OpenDebugLibrary
                        MezzLib                = 256,       ///< Correlates to @ref Lua51ScriptingEngine::OpenMezzanineLibrary
                        MezzSafeLib            = 512,       ///< Correlates to @ref Lua51ScriptingEngine::OpenMezzanineSafeLibrary
                        MezzXMLLib             = 1024,      ///< Correlates to @ref Lua51ScriptingEngine::OpenMezzanineLibrary
                        MezzXMLSafeLib         = 2048,      ///< Correlates to @ref Lua51ScriptingEngine::OpenMezzanineSafeLibrary
                        MezzThreadingLib       = 4096,      ///< Correlates to @ref Lua51ScriptingEngine::OpenMezzanineLibrary
                        MezzThreadingSafeLib   = 8192,      ///< Correlates to @ref Lua51ScriptingEngine::OpenMezzanineSafeLibrary

                        FirstLib    = BaseLib,              ///< Useful for math based ways to work with libraries, This is equal to the numerically lowest lib
                        LastLib     = MezzThreadingSafeLib, ///< Useful for math based ways to work with libraries, This is equal to the numerically highest lib

                        DefaultLibs = BaseLib | StringLib | TableLib | MathLib |
                                      MezzSafeLib | MezzXMLSafeLib | MezzThreadingSafeLib, ///< A quick way to refer to all the libraries opened by @ref Lua51ScriptingEngine::OpenMezzanineSafeLibrary
                        AllLibs     = BaseLib | PackageLib | StringLib | TableLib |
                                      MathLib | IOLib | OSLib | DebugLib | MezzLib |
                                      MezzSafeLib | MezzXMLLib | MezzXMLSafeLib |
                                      MezzThreadingLib | MezzThreadingSafeLib              ///< A quick way to refer to all the libraries opened by @ref Lua51ScriptingEngine::OpenDefaultLibraries
                    };

                    static const String NoLibName;                    ///< @brief The name used to identify No libraries, "None"
                    static const String BaseLibName;                  ///< @brief The name used to identify the Base library, "Base"
                    static const String PackageLibName;               ///< @brief The name used to identify the Package library, "Package"
                    static const String StringLibName;                ///< @brief The name used to identify the String library, "String"
                    static const String TableLibName;                 ///< @brief The name used to identify the Table library, "Table"
                    static const String MathLibName;                  ///< @brief The name used to identify the Math library, "Math"
                    static const String IOLibName;                    ///< @brief The name used to identify the IO library, "IO"
                    static const String OSLibName;                    ///< @brief The name used to identify the OS library, "OS"
                    static const String DebugLibName;                 ///< @brief The name used to identify the Debug library, "Debug"
                    static const String MezzLibName;                  ///< @brief The name used to identify the Mezzanine library, "Mezzanine"
                    static const String MezzSafeLibName;              ///< @brief The name used to identify the MezzanineSafe library, "MezzanineSafe"
                    static const String MezzXMLLibName;               ///< @brief The name used to identify the MezzanineXML library, "MezzanineXML"
                    static const String MezzXMLSafeLibName;           ///< @brief The name used to identify the MezzanineXMLSafe library, "MezzanineXMLSafe"
                    static const String MezzThreadingLibName;         ///< @brief The name used to identify the MezzanineThreading library, "MezzanineThreading"
                    static const String MezzThreadingSafeLibName;     ///< @brief The name used to identify the MezzanineThreadingSafe library, "MezzanineThreadingSafe"
                    static const String DefaultLibsName;              ///< @brief The name used to identify the Default set of libraries, "Default"
                    static const String AllLibsName;                  ///< @brief The name used to identify the set of all libraries, "All"

                    static const String BaseTableName;                ///< @brief The name used to identify a table loaded by the Base library, "coroutine"
                    static const String PackageTableName;             ///< @brief The name used to identify a table loaded by the Package library, "package"
                    static const String StringTableName;              ///< @brief The name used to identify a table loaded by the String library, "string"
                    static const String TableTableName;               ///< @brief The name used to identify a table loaded by the Table library, "table"
                    static const String MathTableName;                ///< @brief The name used to identify a table loaded by the Math library, "math"
                    static const String IOTableName;                  ///< @brief The name used to identify a table loaded by the IO library, "io"
                    static const String OSTableName;                  ///< @brief The name used to identify a table loaded by the OS library, "os"
                    static const String DebugTableName;               ///< @brief The name used to identify a table loaded by the Debug library, "debug"
                    static const String MezzTableName;                ///< @brief The name used to identify a table loaded by the Mezzanine library, "Mezzanine"
                    static const String MezzSafeTableName;            ///< @brief The name used to identify a table loaded by the MezzanineSafe library, "MezzanineSafe"
                    static const String MezzXMLTableName;             ///< @brief The name used to identify a table loaded by the MezzanineXML library, "MezzanineXML"
                    static const String MezzXMLSafeTableName;         ///< @brief The name used to identify a table loaded by the MezzanineXMLSafe library, "MezzanineXMLSafe"
                    static const String MezzThreadingTableName;       ///< @brief The name used to identify a table loaded by the MezzanineThreading library, "MezzanineThreading"
                    static const String MezzThreadingSafeTableName;   ///< @brief The name used to identify a table loaded by the MezzanineThreadingSafe library, "MezzanineThreadingSafe"

                    /// @brief Convert a Lua51Libraries value to its name
                    /// @param Lib A number indicating what libraries a Lua51ScriptingEngine could load
                    /// @return A reference to one of the library names listed on the Lua51ScriptingEngine
                    static const String& GetLibName(Lua51Libraries Lib);
                    /// @brief Convert a Lua51Libraries value to the name of a table it loads
                    /// @param Lib A number indicating what libraries a Lua51ScriptingEngine could load
                    /// @return A reference to one of the table names listed on the Lua51ScriptingEngine
                    static const String& GetTableName(Lua51Libraries Lib);
                    /// @brief Convert a string similar to one of the names on the Lua51ScriptingEngine to number
                    /// @param Name A string containing a name of one of the libraries (without regard to case)
                    /// @return The correlating
                    static Lua51Libraries GetLibFromName(String Name);

                ///////////////////////////////////////////////////////////////////////////////////////
                // Construction/Deconstruction
                    /// @brief Constructs a Scripting engine with a set of libraries preloaded.
                    /// @param LibrariesToOpen A Lua51Libraries bitmap indicating which libraries to load, this defaults to DefaultLibs
                    explicit Lua51ScriptingEngine(Lua51Libraries LibrariesToOpen=DefaultLibs);

                    /// @brief Virtual Deconstructor
                    virtual ~Lua51ScriptingEngine();

                ///////////////////////////////////////////////////////////////////////////////////////
                // Execution
                    /// @brief Compile and execute a passed string.
                    /// @param ScriptSource A String containing the source code to be executed.
                    /// @details This will create a CountPtr to a Lua51Script and assign both its Source and Byte code
                    virtual CountedPtr<iScript> Execute(const String& ScriptSource);

                    /// @brief Implements a required for iScriptManager, Calls Execute(CountedPtr<Lua51Script>)
                    /// @param ScriptToRun A CountedPtr<iScript> to be run. This is cast to an CountedPtr<Lua51Script> and called if possible.
                    /// @throw If this cannot be cast this throws a ParametersCastException.
                    /// @todo fill in the kind of exception thrown.
                    virtual void Execute(CountedPtr<iScript>& ScriptToRun);

                    /// @brief This will execute the passed script, compiling it if not present
                    /// @param ScriptToRun The script to execute.
                    /// @details If a bytecode is present on ScriptToRun then it is executed. Otherwise the Source is
                    /// compiled and the result is set as the bytecode and it is executed.
                    virtual void Execute(CountedPtr<Lua51Script>& ScriptToRun);

                    /// @brief Performs the compilation on a script reference
                    /// @param ScriptToCompile A reference to the Lua51Script to compile.
                    virtual void Execute(Lua51Script& ScriptToRun);

                    /// @brief Performs the compilation on a raw pointer
                    /// @param ScriptToCompile A pointer to the Lua51Script to compile.
                    virtual void Execute(Lua51Script* ScriptToRun);

                ///////////////////////////////////////////////////////////////////////////////////////
                // Compilation
                    /// @brief Calls Compile(CountedPtr<iScriptCompilable>) and returns a CountedPtr to the script created.
                    /// @param SourceToCompile A string Containing valid lua source code.
                    /// @return A CountedPtr<iScriptCompilable> pointing to a created Script object that contains the source and compile binary.
                    virtual CountedPtr<iScriptCompilable> Compile(const String& SourceToCompile);

                    /// @brief Accepts an Counted ptr to a script and compiles it.
                    /// @param ScriptToCompile The CountedPtr to compile
                    /// @details The ByteCode member on the passed script is erased, if present,
                    /// and sets it to corresponding lua binary.
                    /// @throw If an invalid script is passed this throws ParametersCastException.
                    virtual void Compile(CountedPtr<iScriptCompilable>& ScriptToCompile);
                    /// @brief Compile a Lua51 script.
                    /// @param ScriptToCompile A CountedPtr to a Lua51Script.
                    /// @details Compiles the source code present in ScriptToCompile and puts the results back in
                    /// the ByteCode member on the script.
                    virtual void Compile(CountedPtr<Lua51Script>& ScriptToCompile);
                    /// @brief Performs the compilation on a script reference
                    /// @param ScriptToCompile A reference to the Lua51Script to compile.
                    virtual void Compile(Lua51Script& ScriptToCompile);
                    /// @brief Performs the compilation on a raw pointer.
                    /// @param ScriptToCompile A pointer to the Lua51Script to compile.
                    virtual void Compile(Lua51Script* ScriptToCompile);

                ///////////////////////////////////////////////////////////////////////////////////////
                // For Inheritance
                    /// @copydoc ManagerBase::GetImplementationTypeName()
                    /// @return A String containing "Lua51ScriptingEngine".
                    virtual String GetImplementationTypeName() const;

                ///////////////////////////////////////////////////////////////////////////////////////
                // Library Manipulation
                    /// @brief Makes Lua function calls in Lua standard libraries available for use in Lua scripts.
                    /// @param LibrariesToOpen A Lua51Libraries bitmap indicating which libraries to loads
                    virtual void OpenLibraries(int LibrariesToOpen);
                    /// @brief Check the Lua state to see if a table exists.
                    /// @param LibToCheck Indicator of which library to ch
                    virtual Boolean IsLibraryOpen(Lua51Libraries LibToCheck);

                    /// @brief Nest Lua modules to put libraries in more clean positions
                    /// @details this is used to create the syntax "Mezzanine.XML.Document()" for example.
                    /// "Mezzanine" is the base, "MezzanineXML" another module in the root lua environment
                    /// is the Sub and "XML" is the Alias. This does not "move" the library/module, this
                    /// Simply creates a reference with a new name and location.
                    /// @param Base The libary that will have another nested in it.
                    /// @param Sub The library to be nest into another.
                    /// @param Alias The new name of the library when referenced from its nested location.
                    /// @warning If this fails this fails silently.
                    virtual void AliasLibrary(const String& Base, const String& Sub, const String& Alias);

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
                    /// Lua manual at http://www.lua.org/manual/5.1/manual.html#5.9 .
                    virtual void OpenDebugLibrary();

                    /// @brief Make everything in the Mezzanine Libary available for use in Lua51 scripts.
                    /// @warning This makes arbitrary execution of programs and file management available to scripts. This is not suitable if untrusted scripts will be run.
                    virtual void OpenMezzanineLibrary();
                    /// @brief Make a subset of the Mezzanine Library available for use in Lua51 scripts.
                    /// @details This should not allow access to any functions, methods or classes than can execute code or manage files.
                    virtual void OpenMezzanineSafeLibrary();

                    /// @brief Make the XML parts of the Mezzanine Libary available for use in Lua51 scripts.
                    /// @warning This makes arbitrary execution of programs and file management available to scripts. This is not suitable if untrusted scripts will be run.
                    virtual void OpenMezzanineXMLLibrary();
                    /// @brief Make the XML parts of the Mezzanine Libary available for use in Lua51 scripts.
                    /// @details This should not allow access to any functions, methods or classes than can execute code or manage files.
                    virtual void OpenMezzanineXMLSafeLibrary();

                    /// @brief Make the Threading parts of the Mezzanine Libary available for use in Lua51 scripts.
                    /// @warning This makes a number of powerful feature available that could cause Denial of Service if untrusted scripts are run.
                    virtual void OpenMezzanineThreadingLibrary();
                    /// @brief Make the Threading parts of the Mezzanine Libary available for use in Lua51 scripts.
                    /// @details This should not allow access to any functions, methods or classes than can execute code or manage files or crash the client malicious ways.
                    virtual void OpenMezzanineThreadingSafeLibrary();

                protected:
                    /// @brief Set The MezzanineXML library as the XML member of the Mezzanine library or fail silently
                    void SetXML();
                    /// @brief Set The MezzanineXMLSafe library as the XML member of the MezzanineSafe library or fail silently
                    void SetXMLSafe();

                    /// @brief Set The MezzanineThreading library as the Threading member of the Mezzanine library or fail silently
                    void SetThreading();
                    /// @brief Set The MezzanineThreadingSafe library as the Threading member of the MezzanineSafe library or fail silently
                    void SetThreadingSafe();

                ///////////////////////////////////////////////////////////////////////////////////////
                // Library Manipulation
                public:
                    // @brief Get the underlying Lua state that can be used with lua api calls directly.
                    // @return The lua_State this uses
                    //virtual lua_State* GetRawLuaState();

                    //int test();
                    //String tests(String Returns="");
            };

            //simplistic error checking function, to be replace with proper exception driven code later.
            //int MEZZ_LIB PrintErrorMessageOrNothing(int ErrorCode);

            // super simple lua test
            //int MEZZ_LIB test();

        } // Lua
    } // Scripting
} // Mezzanine


#endif // MEZZLUA51
#endif // \_scriptinglua_h
