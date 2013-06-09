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
#ifndef _lua51script_cpp
#define _lua51script_cpp

#include "datatypes.h"

#ifdef MEZZLUA51

#include "lua51script.h"
#include "lua51scriptingengine.h"
#include "exception.h"

#include <algorithm>

/// @file
/// @brief This file has the implemetation for the Lua script storage.

extern "C"
{
    #include "lua.h"            // Lua Core
    #include "lualib.h"         // for opening the base state
    #include "lauxlib.h"        // Extra Lua Goodies like lua_open()

    int luaopen_Mezzanine(lua_State* L);
}

#include <iostream>

namespace Mezzanine
{
    namespace Scripting
    {
        namespace Lua
        {
            ///////////////////////////////////////////////////////////////////////////////////////
            // LuaScript
            void Lua51Script::Compile(Lua51ScriptingEngine* Compiler)
                { Compiler->Compile(this); }

            Lua51Script::Lua51Script()
                { }

            Lua51Script::Lua51Script(const String& InitialSourceCode, Lua51ScriptingEngine* Compiler)
                : SourceCode(InitialSourceCode)
            {
                if(Compiler)
                    { Compile(Compiler); }
            }

            Lua51Script::Lua51Script(const String& InitialSourceCode, Lua51ScriptingEngine& Compiler)
                : SourceCode(InitialSourceCode)
                { Compile(&Compiler); }

            Lua51Script::~Lua51Script()
                {}

            void Lua51Script::AddArgument(CountedPtr<iScriptArgument> Arg)
                { Args.push_back(Arg); }

            void Lua51Script::RemoveArgument(CountedPtr<iScriptArgument> Arg)
                { Args.erase( std::remove(Args.begin(),Args.end(),Arg) ); }

            void Lua51Script::RemoveArgument(Whole ArgNumber)
                { Args.erase(Args.begin()+ArgNumber); }

            Whole Lua51Script::GetArgumentCount() const
                { return Args.size(); }

            void Lua51Script::ClearArguments()
                { Args.clear(); }

            CountedPtr<iScriptArgument> Lua51Script::GetArgument(Whole ArgNumber) const
                { return Args.at(ArgNumber); }

            void Lua51Script::SetSourceCode(const String& Code)
            {
                CompiledByteCode.DeleteBuffer();
                SourceCode = Code;
            }

            void Lua51Script::SetSourceCode(const String& Code, CountedPtr<iScriptArgument> Return1)
            {
                SetSourceCode(Code);
                Returns.clear();
                Returns.push_back(Return1);
            }

            String Lua51Script::GetSourceCode() const
                { return SourceCode; }

            void Lua51Script::SetByteCode(BinaryTools::BinaryBuffer Code)
                { CompiledByteCode = Code; }

            BinaryTools::BinaryBuffer Lua51Script::GetByteCode() const
                { return CompiledByteCode; }

            FlaggedBuffer& Lua51Script::GetByteCodeReference()
                { return CompiledByteCode; }

            bool Lua51Script::IsCompiled() const
                { return CompiledByteCode.Binary != 0; }

            Whole Lua51Script::GetReturnCount() const
                { return Returns.size(); }

            void Lua51Script::AddReturn(CountedPtr<iScriptArgument> ReturnArg)
                { Returns.push_back(ReturnArg); }

            ArgumentGroup Lua51Script::GetAllReturns() const
                { return Returns; }
        } // Lua
    } // Scripting
} // Mezzanine




#endif //  MEZZLUA51
#endif
