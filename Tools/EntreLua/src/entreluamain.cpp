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

using namespace Mezzanine;
using namespace std;

/// @brief This is the entry point for a Lua shell with
/// Here are the Command line options Lua implements:
///      - [ ] -e stat: executes string stat;
///      - [ ] -l mod: "requires" mod;
///      - [ ] -i: enters interactive mode after running script;
///      - [ ] -v: prints version information;
///      - [ ] --: stops handling options;
///      - [ ] -: executes stdin as a file and stops handling options.
///
/// We will try to emulate these with this shell, but we will also add:
///      - [ ] --execute, --load, --interactive, --version, and --stdin: Long version of previous commands
///      - [ ] -n --no-mezzanine: Prevent loading of MezzanineSafe by default.
///      - [ ] -u --unsafe: Load the unlimited (unsafe) version of the Mezzanine instead of MezzanineSafe
///
/// @return This will return EXIT_SUCCESS, it will do more later.
/// @param argc Is interpretted as the amount of passed arguments
/// @param argv Is interpretted as the arguments passed in from the launching shell.
int main (int argc, char** argv)
{
    vector<String> LoadList;
    String StatementToExecute;
    try
    {
        TCLAP::CmdLine cmd("EntreLua - Mezzanine Lua Shell", ' ', "0.01 with Lua5.1");

        TCLAP::MultiArg<String> LoadArg("l", "load", "Requires/Loads a Module", false, "string" );
        cmd.add( LoadArg );

        TCLAP::ValueArg<std::string> StatementArg("e", "execute", "Execute a Lua script entered at the command line", false, "", "string");
        cmd.add( StatementArg );

        cmd.parse( argc, argv );

        LoadList = LoadArg.getValue();
        StatementToExecute = StatementArg.getValue();
    } catch (TCLAP::ArgException &e) {
        cerr << "error: " << e.error() << " for arg " << e.argId() << endl;
    }


    //constrain this list
    cout << "Load libraries: " << endl;
    for(vector<String>::iterator Iter = LoadList.begin(); Iter!=LoadList.end(); Iter++)
    {
        cout << "  -" << *Iter << endl;
    }

    cout << "Execute this statement: " << StatementToExecute << endl;

    return ExitSuccess;
}
