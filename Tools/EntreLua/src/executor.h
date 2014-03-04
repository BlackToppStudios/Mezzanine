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
#ifndef EXECUTOR_H
#define EXECUTOR_H

/// @file
/// @brief A class to comtain the complexity of executing the lua commands.

#include <datatypes.h>
#include <Scripting/Lua51/lua51scriptingengine.h>

/// @brief The information required for the REPL to act on the results of command excution.
struct ExecutionResults
{
    /// @brief The Results the REPL should display.
    Mezzanine::String Output;
    /// @brief Should the REPL quit.
    Mezzanine::Boole Quit;
    /// @brief Should this be part of a multiline continuation?
    Mezzanine::Boole Multiline;
    /// @brief What did the executed script return?
    std::vector<Mezzanine::String> Returns;

    /// @brief An initializing constructor
    /// @param DesiredOutput What does the language think the REPL should print
    /// @param Exit Does the executor think the REPL shold Quit.
    ExecutionResults(const Mezzanine::String& DesiredOutput="",
                     Mezzanine::Boole Exit = false,
                     Mezzanine::Boole MultilineContinuation = false)
        : Output(DesiredOutput),
          Quit(Exit),
          Multiline(MultilineContinuation)
    {}
};

/// @brief What a class that will Read input, Evaluate, Print, and Loop back to do it again requires.
class Executor
{
    private:
        /// @brief Should troublshooting stack counts be displayed
        Mezzanine::Boole StackCounts;

    public:
        /// @brief The actual Lua intrepretter
        Mezzanine::Scripting::Lua::Lua51ScriptingEngine& LuaEngine;

        /// @brief Initializing constructor
        /// @param TargetExecutor A Lua sript command interpretter that the commands will be executed against.
        Executor(Mezzanine::Scripting::Lua::Lua51ScriptingEngine& TargetEngine, Mezzanine::Boole DisplayStackCounts=false);

        /// @brief Actually Do one command line worth of stuff.
        /// @param CommandLine A string of lua or commands to execute.
        ExecutionResults Do(Mezzanine::String CommandLine);
};

#endif
