// © Copyright 2010 - 2017 BlackTopp Studios Inc.
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
#ifndef REPL_LINENOISE_H
#define REPL_LINENOISE_H

#include "repl.h"
#include "executor.h"

#include <trie.h>

#include<vector>


/// @file
/// @brief The definition of an implementation of a feature rich REPL written with linenoise

struct linenoiseCompletions;

/// @brief A class that will take user input from a terminal and try to take full advantage of the features the terminal offers
class REPLLineNoise : public REPL
{
    private:

        /// @brief Filled out during the first tab strike and consumed in the second, this will efficiently store the current identifiers Lua manages
        Mezzanine::Scripting::Lua::Lua51ScriptingEngine::CommandTrie PossibleCommands;

        /// @brief calls the Mezzanine to get the list of commands and identifiers to put in PossibleCommands
        void PopulateCommmandTrie();

        /// @brief This controls the specific behavior that occurs during the first strike of the tab key in tab completion
        /// @details If there is exactly on match for a Lua identifier this should find its match and type it or partially if multiple
        /// @param lc The list of line noise completions
        /// @param CurrentID The current Identifer at the end of the line
        /// @param CurrentLine The current typed contents of the command line
        void DoTabStrike1(linenoiseCompletions *lc, const Mezzanine::String& CurrentID, const Mezzanine::String& CurrentLine);

        /// @brief Display one block in the tab completion output of possibl identifiers
        /// @param Typename A string that will be the title of the block
        /// @param Printing A vector strings of Identifiers to print
        void DisplayOtherIdentifiers(const Mezzanine::String& Typename, std::vector<Mezzanine::String>& Printing);

        /// @brief This controls the specific behavior that occurs during the second strike of the tab key in tab completion
        /// @details If there are valid identifiers in the Lua runtime this should Display a list of them
        /// @param CurrentID The current Identifer at the end of the line
        void DoTabStrike2(const Mezzanine::String& CurrentID);

        /// @brief Identify the last lua identifier on a line and return that
        /// @param CurrentLine The line of input to investigate
        /// @return One valid but potentially partial Lua identifier at the at end of CurrentLine or an empty String
        Mezzanine::String GetCurrentID(const Mezzanine::String& CurrentLine);

    public:
        /// @brief Initializing constructor
        /// @param TargetExecutor A Lua sripting engine that the commands will be executed against.
        /// @param StartingPrompt The text to start the line
        /// @param StartingMultiline What to display during multiline input
        /// @param StartingReturn What to display when returning values
        REPLLineNoise(Executor& TargetExecutor,
                      Mezzanine::String StartingPrompt="> ",
                      Mezzanine::String StartingMultiline=">> ",
                      Mezzanine::String StartingReturn="=> ");

        /// @brief Tab completion callback
        /// @param CurrentInput The Current Input on the command line
        /// @param lc Line completionforthis current iteration of thetab press
        void TabCompletion(const char *CurrentInput, linenoiseCompletions *lc);

        /// @brief This is the actual the loop that will do the REPLing
        virtual void Launch();
};

#endif
