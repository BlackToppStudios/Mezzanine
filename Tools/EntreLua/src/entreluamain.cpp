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

/// @brief Possible ways to exit the EntreLua Shell
enum ExitCodes
{
    ExitSuccess             = 0,    ///< Normal exit everthing seemed to work
    ExitInvalidArguments    = 1,    ///< At least some invalid args were passed on the command line
    ExitFailure             = 2     ///< Something failed after command line arguments
};

#include <tclap/CmdLine.h>
#include <iostream>
#include <mezzanine.h>
#include <string>

#include "repl.h"
#include "replcppstream.h"
#include "executor.h"

using namespace Mezzanine;
using namespace Mezzanine::Scripting::Lua;
using namespace std;

/// @brief This is the entry point for a Lua shell with
/// Here are the Command line options Lua implements and our differences from them
///      - -e stat: executes string stat.
///      - -l mod: "requires" mod. We check the system path.
///      - -i: enters interactive mode after running other scripts.
///      - -v: prints version information;
///      - --: stops handling options;
///      - -: executes stdin as a file. Lua stops handling other options after this, keep handling.
///
/// With these this can be a drop in replacement for Lua in some situations. We will also add:
///      - --execute, --load, --interactive, --version, and --stdin: Long version of previous commands
///      - -n --no-mezzanine: Prevent loading of MezzanineSafe by default.
///      - -u --unsafe: Load the unlimited (unsafe) version of the Mezzanine instead of MezzanineSafe
///      - -o --open: Open a lua or Mezzanine library before the shell starts.
///      - -s --simple: Use a shell based on cin, cout instead of linenoise (Which I suspect will have issues on windows)
///
/// @return This will return EXIT_SUCCESS, it will do more later.
/// @param argc Is interpretted as the amount of passed arguments
/// @param argv Is interpretted as the arguments passed in from the launching shell.
int main (int argc, char** argv)
{
    vector<Mezzanine::String> LibaryList;
    vector<String> OpenList;
    vector<String> CloseList;
    vector<String> LoadList;
    String StatementToExecute;
    bool Interactive;
    bool ReadFromStdIn;
    bool SimpleShell;
    bool NoMezzanine;
    bool LoadUnsafeMezzanine;
    try
    {
        TCLAP::CmdLine cmd("EntreLua - Mezzanine Lua Shell", ' ', "0.01 with Lua5.1");

        LibaryList.push_back(Mezzanine::Scripting::Lua::Lua51ScriptingEngine::NoLibName);
        LibaryList.push_back(Mezzanine::Scripting::Lua::Lua51ScriptingEngine::BaseLibName);
        LibaryList.push_back(Mezzanine::Scripting::Lua::Lua51ScriptingEngine::PackageLibName);
        LibaryList.push_back(Mezzanine::Scripting::Lua::Lua51ScriptingEngine::StringLibName);
        LibaryList.push_back(Mezzanine::Scripting::Lua::Lua51ScriptingEngine::TableLibName);
        LibaryList.push_back(Mezzanine::Scripting::Lua::Lua51ScriptingEngine::MathLibName);
        LibaryList.push_back(Mezzanine::Scripting::Lua::Lua51ScriptingEngine::IOLibName);
        LibaryList.push_back(Mezzanine::Scripting::Lua::Lua51ScriptingEngine::OSLibName);
        LibaryList.push_back(Mezzanine::Scripting::Lua::Lua51ScriptingEngine::DebugLibName);
        LibaryList.push_back(Mezzanine::Scripting::Lua::Lua51ScriptingEngine::MezzLibName);
        LibaryList.push_back(Mezzanine::Scripting::Lua::Lua51ScriptingEngine::MezzSafeLibName);
        LibaryList.push_back(Mezzanine::Scripting::Lua::Lua51ScriptingEngine::MezzXMLSafeLibName);
        LibaryList.push_back(Mezzanine::Scripting::Lua::Lua51ScriptingEngine::MezzThreadingLibName);
        LibaryList.push_back(Mezzanine::Scripting::Lua::Lua51ScriptingEngine::MezzThreadingSafeLibName);
        LibaryList.push_back(Mezzanine::Scripting::Lua::Lua51ScriptingEngine::DefaultLibsName);
        LibaryList.push_back(Mezzanine::Scripting::Lua::Lua51ScriptingEngine::AllLibsName);

        TCLAP::ValuesConstraint<Mezzanine::String> LibaryVals( LibaryList );
        TCLAP::MultiArg<string> OpenlibArg("o", "openlib", "Library to open before shell starts", false, &LibaryVals, cmd);
        TCLAP::MultiArg<string> CloselibArg("c", "closelib", "Do not open a Library that might be opened before shell starts", false, &LibaryVals, cmd);

         TCLAP::MultiArg<String> LoadArg("l", "load", "Requires/Loads a Module.", false, "filename", cmd);
        TCLAP::ValueArg<std::string> StatementArg("e", "execute", "Execute a Lua script entered at the command line.", false, "", "Lua String", cmd);
        TCLAP::SwitchArg InteractiveSwitch("i","interactive","Enter interactive shell after other items are executed.", cmd, false);
        TCLAP::SwitchArg StdinSwitch(":","stdin","Read from the Standard Input and execute whatever is found there.", cmd, false);
        TCLAP::SwitchArg SimpleSwitch("s","simple","Use a simpler shell input method with fewer features but that works in more places.", cmd, false);
        TCLAP::SwitchArg NoMezzanineSwitch("n", "no-mezzanine", "Do not load/open the Mezzanine by default.", cmd, false);
        TCLAP::SwitchArg UnsafeSwitch("u", "unsafe", "Load the unrestricted Mezzanine library instead of MezzanineSafe.", cmd, false);

        cmd.parse( argc, argv );

        OpenList = OpenlibArg.getValue();
        CloseList = CloselibArg.getValue();
        LoadList = LoadArg.getValue();
        StatementToExecute = StatementArg.getValue();
        Interactive = InteractiveSwitch.getValue();
        ReadFromStdIn = StdinSwitch.getValue();
        SimpleShell = SimpleSwitch.getValue();
        NoMezzanine = NoMezzanineSwitch.getValue();
        LoadUnsafeMezzanine = UnsafeSwitch.getValue();
    } catch (TCLAP::ArgException &e) {
        cerr << "error: " << e.error() << " for arg " << e.argId() << endl;
    }

    ///////////////
    // Argument test
    /*
    cout << "Load libraries: " << endl;
    for(vector<String>::iterator Iter = OpenList.begin(); Iter!=OpenList.end(); Iter++)
        { cout << "  -" << Lua51ScriptingEngine::GetLibFromName(*Iter) << " - " << *Iter << endl; }
    cout << "Do not Load libraries: " << endl;
    for(vector<String>::iterator Iter = CloseList.begin(); Iter!=CloseList.end(); Iter++)
        { cout << "  -" << Lua51ScriptingEngine::GetLibFromName(*Iter) << " - " << *Iter << endl; }
    cout << "Execute this statement: " << StatementToExecute << endl;
    cout << "Requested to manually enter interactive shell: " << Interactive << endl;
    cout << "Requested to read from stdin: " << ReadFromStdIn << endl;
    cout << "Use Simple Shell: " << SimpleShell << endl;
    cout << "Don't load the Mezzanine: " << NoMezzanine  << endl;
    cout << "Use Unsafe: " << LoadUnsafeMezzanine << endl;
    // */

    //////////////////////////////////////////////////////////////////////////////////////////
    // Determine the Libraries to load

    Integer LibsToLoad = Lua51ScriptingEngine::DefaultLibs;
    for(vector<String>::iterator Iter = OpenList.begin(); Iter!=OpenList.end(); Iter++)
        { LibsToLoad = LibsToLoad | Lua51ScriptingEngine::GetLibFromName(*Iter); }
    for(vector<String>::iterator Iter = CloseList.begin(); Iter!=CloseList.end(); Iter++)
        { LibsToLoad = LibsToLoad & ~Lua51ScriptingEngine::GetLibFromName(*Iter); }

    if(LoadUnsafeMezzanine)
    {
        LibsToLoad = LibsToLoad | Lua51ScriptingEngine::MezzLib;
        LibsToLoad = LibsToLoad & ~Lua51ScriptingEngine::MezzSafeLib;
        LibsToLoad = LibsToLoad | Lua51ScriptingEngine::MezzXMLLib;
        LibsToLoad = LibsToLoad & ~Lua51ScriptingEngine::MezzXMLSafeLib;
        LibsToLoad = LibsToLoad | Lua51ScriptingEngine::MezzThreadingLib;
        LibsToLoad = LibsToLoad & ~Lua51ScriptingEngine::MezzThreadingSafeLib;
    }

    if(NoMezzanine)
    {
        LibsToLoad = LibsToLoad & ~Lua51ScriptingEngine::MezzLib;
        LibsToLoad = LibsToLoad & ~Lua51ScriptingEngine::MezzSafeLib;
        LibsToLoad = LibsToLoad & ~Lua51ScriptingEngine::MezzXMLLib;
        LibsToLoad = LibsToLoad & ~Lua51ScriptingEngine::MezzXMLSafeLib;
        LibsToLoad = LibsToLoad & ~Lua51ScriptingEngine::MezzThreadingLib;
        LibsToLoad = LibsToLoad & ~Lua51ScriptingEngine::MezzThreadingSafeLib;
    }

    Lua51ScriptingEngine TheLua( (Lua51ScriptingEngine::Lua51Libraries)LibsToLoad );
    Executor Hooded(TheLua);
    // need to set arg table here

    //////////////////////////////////////////////////////////////////////////////////////////
    // Choose and launch shell if required

    // check LUA_INIT here

    if(StatementToExecute.size())
        { Hooded.Do(StatementToExecute); }

    //need to load external scripts here
    for(vector<String>::iterator Iter = LoadList.begin(); Iter!=LoadList.end(); Iter++)
    {
        // look in current dir, PATH

    }

    if(ReadFromStdIn)
    {
        String Total;
        String Current;
        while(!cin.eof() && cin.good())
        {
            getline(cin,Current);
            Total += Current;
        }
        Hooded.Do(Total);
    }

    Boolean OtherCommands = (StatementToExecute.size()||ReadFromStdIn);
    if( !OtherCommands || (OtherCommands && Interactive) )
    {
        if(SimpleShell)
        {
            REPLCppStream Shell(Hooded);
            Shell.Launch();
        }else
            { cout << "Full Featured Shell Not implemented Use -s, --simple for now" << endl; }
    }



    return ExitSuccess;
}
