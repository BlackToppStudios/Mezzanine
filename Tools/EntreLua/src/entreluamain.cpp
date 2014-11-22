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

/// @file
/// @brief This is the entry point of the EntreLua shell.
/// @details It is a bit messy in this file, but it really just parses
/// the command line options, includes the rest of the EntreLua and
/// dispatches commands to the other components. It doesn't even really
/// parse the command line options, that is done by a library, verbosely


#include <tclap/CmdLine.h>
#include <iostream>
#include <mezzanine.h>
#include <string>

#include "replcppstream.h"
#include "repllinenoise.h"
#include "executor.h"

using namespace Mezzanine;
using namespace Mezzanine::Scripting::Lua;
using namespace std;

/// @brief Possible ways to exit the EntreLua Shell
enum ExitCodes
{
    ExitSuccess             = 0,    ///< Normal exit everthing seemed to work
    ExitInvalidArguments    = 1,    ///< At least some invalid args were passed on the command line
    ExitFailure             = 2     ///< Something failed after command line arguments
};

/// @brief This is used to hold all the options that the user could enter on the command prompt or that Main uses for control flow.
struct EntreLuaOptions
{
        vector<Mezzanine::String> LibraryList;      ///< A List of all Libraries the Mezzanine Lua51ScriptingEngine can load
        vector<String> OpenList;                    ///< Libraries to be opened
        vector<String> CloseList;                   ///< Libraries to close/not open
        vector<String> LoadList;                    ///< External files to load in Lua search path
        vector<String> ScriptFile;                  ///< File in local directory to execute
        Mezzanine::String StatementToExecute;       ///< Commands on the raw prompt to execute
        Mezzanine::Boole Interactive;             ///< Enter an interactive shell even if executing from other sources
        Mezzanine::Boole ReadFromStdIn;           ///< After other execution sources read from stdin
        Mezzanine::Boole SimpleShell;             ///< Use a simple more compatible shell
        Mezzanine::Boole NoMezzanine;             ///< Skip loading of Mezzanine Libraries
        Mezzanine::Boole LoadUnsafeMezzanine;     ///< Don't load the normal Mezzanine Libries load the ones that could break stuff
        Mezzanine::Boole DisplayStackCounts;      ///< Display the amount of items on the Lua stack after each command

        /// @brief Create a Default version of this, with all the fields empty except the library list.
        EntreLuaOptions()
            : StatementToExecute(""),
              Interactive(false),
              ReadFromStdIn(false),
              SimpleShell(false),
              NoMezzanine(false),
              LoadUnsafeMezzanine(false),
              DisplayStackCounts(false)
        {
            LibraryList.push_back(Mezzanine::Scripting::Lua::Lua51ScriptingEngine::NoLibName);
            LibraryList.push_back(Mezzanine::Scripting::Lua::Lua51ScriptingEngine::BaseLibName);
            LibraryList.push_back(Mezzanine::Scripting::Lua::Lua51ScriptingEngine::PackageLibName);
            LibraryList.push_back(Mezzanine::Scripting::Lua::Lua51ScriptingEngine::StringLibName);
            LibraryList.push_back(Mezzanine::Scripting::Lua::Lua51ScriptingEngine::TableLibName);
            LibraryList.push_back(Mezzanine::Scripting::Lua::Lua51ScriptingEngine::MathLibName);
            LibraryList.push_back(Mezzanine::Scripting::Lua::Lua51ScriptingEngine::IOLibName);
            LibraryList.push_back(Mezzanine::Scripting::Lua::Lua51ScriptingEngine::OSLibName);
            LibraryList.push_back(Mezzanine::Scripting::Lua::Lua51ScriptingEngine::DebugLibName);
            LibraryList.push_back(Mezzanine::Scripting::Lua::Lua51ScriptingEngine::MezzLibName);
            LibraryList.push_back(Mezzanine::Scripting::Lua::Lua51ScriptingEngine::MezzSafeLibName);
            LibraryList.push_back(Mezzanine::Scripting::Lua::Lua51ScriptingEngine::MezzXMLSafeLibName);
            LibraryList.push_back(Mezzanine::Scripting::Lua::Lua51ScriptingEngine::MezzThreadingLibName);
            LibraryList.push_back(Mezzanine::Scripting::Lua::Lua51ScriptingEngine::MezzThreadingSafeLibName);
            LibraryList.push_back(Mezzanine::Scripting::Lua::Lua51ScriptingEngine::MezzMathToolsLibName);
            LibraryList.push_back(Mezzanine::Scripting::Lua::Lua51ScriptingEngine::DefaultLibsName);
            LibraryList.push_back(Mezzanine::Scripting::Lua::Lua51ScriptingEngine::AllLibsName);
        }
};


/// @brief Convert the Command line options into a useful body of data.
/// @param argc The amount of argument from the parent shell.
/// @param argv The argument vector from the parent shell.
/// @return A EntreLuaOptions will all the options the user or OS provided//
EntreLuaOptions ParseCommandLineOption(int argc, char** argv)
{
    EntreLuaOptions Results;
    try
    {
        TCLAP::CmdLine cmd("EntreLua - Mezzanine Lua Shell", ' ', "0.01 with Lua5.1");

        TCLAP::ValuesConstraint<Mezzanine::String> LibaryVals( Results.LibraryList );
        TCLAP::MultiArg<string> OpenlibArg("o", "openlib", "Library to open before shell starts", false, &LibaryVals, cmd);
        TCLAP::MultiArg<string> CloselibArg("c", "closelib", "Do not open a Library that might be opened before shell starts", false, &LibaryVals, cmd);

        TCLAP::MultiArg<String> LoadArg("l", "load", "Requires/Loads a Module. Force opening of Package lib.", false, "filename", cmd);
        TCLAP::ValueArg<std::string> StatementArg("e", "execute", "Execute a Lua script entered at the command line.", false, "", "Lua String", cmd);
        TCLAP::SwitchArg InteractiveSwitch("i","interactive","Enter interactive shell after other items are executed.", cmd, false);
        TCLAP::SwitchArg StdinSwitch(":","stdin","Read from the Standard Input and execute whatever is found there.", cmd, false);
        TCLAP::SwitchArg SimpleSwitch("s","simple","Use a simpler shell input method with fewer features but that is compatible in more places.", cmd, false);
        TCLAP::SwitchArg NoMezzanineSwitch("n", "no-mezzanine", "Do not load/open the Mezzanine by default.", cmd, false);
        TCLAP::SwitchArg UnsafeSwitch("u", "unsafe", "Load the unrestricted Mezzanine library instead of MezzanineSafe.", cmd, false);
        TCLAP::UnlabeledMultiArg<String> ScriptAndArgs( "script", "A script to execute instead of an interactive shell", false, "script [args]", cmd);
        TCLAP::SwitchArg StackSwitch("S", "Stack", "Display stack counts after each command execution.", cmd, false);

        cmd.parse(argc, argv );

        Results.OpenList = OpenlibArg.getValue();
        Results.CloseList = CloselibArg.getValue();
        Results.LoadList = LoadArg.getValue();
        Results.StatementToExecute = StatementArg.getValue();
        Results.Interactive = InteractiveSwitch.getValue();
        Results.ReadFromStdIn = StdinSwitch.getValue();
        Results.SimpleShell = SimpleSwitch.getValue();
        Results.NoMezzanine = NoMezzanineSwitch.getValue();
        Results.LoadUnsafeMezzanine = UnsafeSwitch.getValue();
        Results.ScriptFile = ScriptAndArgs.getValue();
        Results.DisplayStackCounts = StackSwitch.getValue();
    } catch (TCLAP::ArgException &e) {
        cerr << "error: " << e.error() << " for arg " << e.argId() << endl;
    }
    return Results;
}

/// @brief Deal the with many options that load/prevent loading of different libraries.
/// @param Options a reference to a EntreLuaOptions that contains the library options to work with.
/// @return An Integer containing a Bit Field representing the libraries to load.
Integer GetLibsToLoad(EntreLuaOptions& Options)
{
    Integer LibsToLoad = Lua51ScriptingEngine::DefaultLibs;
    for(vector<String>::iterator Iter = Options.OpenList.begin(); Iter!=Options.OpenList.end(); Iter++)
        { LibsToLoad = LibsToLoad | Lua51ScriptingEngine::GetLibFromName(*Iter); }
    for(vector<String>::iterator Iter = Options.CloseList.begin(); Iter!=Options.CloseList.end(); Iter++)
        { LibsToLoad = LibsToLoad & ~Lua51ScriptingEngine::GetLibFromName(*Iter); }

    if(Options.LoadUnsafeMezzanine)
    {
        LibsToLoad = LibsToLoad | Lua51ScriptingEngine::MezzLib;
        LibsToLoad = LibsToLoad & ~Lua51ScriptingEngine::MezzSafeLib;
        LibsToLoad = LibsToLoad | Lua51ScriptingEngine::MezzXMLLib;
        LibsToLoad = LibsToLoad & ~Lua51ScriptingEngine::MezzXMLSafeLib;
        LibsToLoad = LibsToLoad | Lua51ScriptingEngine::MezzThreadingLib;
        LibsToLoad = LibsToLoad & ~Lua51ScriptingEngine::MezzThreadingSafeLib;
    }

    if(Options.NoMezzanine)
    {
        LibsToLoad = LibsToLoad & ~Lua51ScriptingEngine::MezzLib;
        LibsToLoad = LibsToLoad & ~Lua51ScriptingEngine::MezzSafeLib;
        LibsToLoad = LibsToLoad & ~Lua51ScriptingEngine::MezzXMLLib;
        LibsToLoad = LibsToLoad & ~Lua51ScriptingEngine::MezzXMLSafeLib;
        LibsToLoad = LibsToLoad & ~Lua51ScriptingEngine::MezzThreadingLib;
        LibsToLoad = LibsToLoad & ~Lua51ScriptingEngine::MezzThreadingSafeLib;
    }

    if(Options.LoadList.size()) // The require statement is... required... for requiring things
        { LibsToLoad = LibsToLoad | Lua51ScriptingEngine::PackageLib; }

    return LibsToLoad;
}

/// @brief This is the entry point for a Lua shell with
/// Here are the Command line options Lua implements and our differences from them
///      - -e stat: executes string stat.
///      - -l mod: "requires" mod. Forces the Package Library to be loaded
///      - -i: enters interactive mode after running other scripts.
///      - -v: prints version information;
///      - --: stops handling options;
///      - -: executes stdin as a file. Lua stops handling other options after this, keep handling.
///      - Lua script names and args
///
/// With these this can be a drop in replacement for Lua in some situations. We will also add:
///      - --execute, --load, --interactive, --version, and --stdin: Long version of previous commands
///      - -n --no-mezzanine: Prevent loading of MezzanineSafe by default.
///      - -u --unsafe: Load the unlimited (unsafe) version of the Mezzanine instead of MezzanineSafe
///      - -o --open: Open a lua or Mezzanine library before the shell starts.
///      - -s --simple: Use a shell based on cin, cout instead of linenoise (Which I suspect will have issues on windows)
///      - -S --Stack: Display stack counts after each command execution
///
/// @return This will return EXIT_SUCCESS, it will do more later.
/// @param argc Is interpretted as the amount of passed arguments
/// @param argv Is interpretted as the arguments passed in from the launching shell.
int main (int argc, char** argv)
{
    // All the arg interpretion is done here.
    EntreLuaOptions Options=ParseCommandLineOption(argc, argv);
    Integer LibsToLoad = GetLibsToLoad(Options);

    // all option creation is here.
    Lua51ScriptingEngine TheLua( (Lua51ScriptingEngine::Lua51Libraries)LibsToLoad );
    Executor Hooded(TheLua, Options.DisplayStackCounts);

    // The rest is comparatively detailed control flow.
    for(vector<String>::iterator Iter = Options.LoadList.begin(); Iter!=Options.LoadList.end(); Iter++)
        { Hooded.Do(String("require(\"") + *Iter + String("\")\n")); }

    if(Options.StatementToExecute.size())
        { Hooded.Do(Options.StatementToExecute); }

    // This needs to be implmented still
    /*cout << "Should Execute: ";
    for(vector<String>::iterator Iter = ScriptFile.begin(); Iter!=ScriptFile.end(); Iter++)
        { cout << *Iter << " "; }
    cout << endl;*/
    if(Options.ScriptFile.size())
        { cerr << "File execution not implemented yet" << endl; }

    if(Options.ReadFromStdIn)
    {
        String Total;
        String Current;
        while(!cin.eof() && cin.good())
        {
            getline(cin, Current);
            Total += Current;
        }
        Hooded.Do(Total);
    }

    Boole OtherCommands = (Options.StatementToExecute.size()||Options.ReadFromStdIn);
    if( !OtherCommands || (OtherCommands && Options.Interactive) )
    {
        if(Options.SimpleShell)
        {
            REPLCppStream Shell(Hooded);
            Shell.Launch();
        }else{
            REPLLineNoise Shell(Hooded);
            Shell.Launch();
        }
    }

    return ExitSuccess;
}
