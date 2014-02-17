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
#ifndef REPL_H
#define REPL_H

#include <datatypes.h>
#include <Scripting/Lua51/lua51scriptingengine.h>

#include "executor.h"

/// @file
/// @brief The definition of the user interface for this shell.

class REPL;

/// @brief A Functor definition that return true if the REPL should continue and false otherwise
typedef bool(REPLContinueCallback)(REPL*);

/// @brief What a class that will Read input, Evaluate, Print, and Loop back to do it again requires.
class REPL
{
    protected:
        /// @brief What will be display
        Mezzanine::String Prompt;

        /// @brief The actual Lua intrepretter
        Executor& Doer;

    public:
        /// @brief Initializing constructor
        /// @param TargetExecutor A Lua sript command interpretter that the commands will be executed against.
        /// @param StartingPrompt The text to start the line
        REPL(Executor& TargetExecutor, Mezzanine::String StartingPrompt=">");

        /// @brief This is the actual the loop that will do the REPLing
        virtual void Launch()=0;

        /// @brief What text is shown with the prompt.
        /// @return A string containing the prompt text.
        Mezzanine::String GetPrompt() const;
        /// @brief
        /// @param
        void SetPrompt(const Mezzanine::String& Value);
};

#endif
