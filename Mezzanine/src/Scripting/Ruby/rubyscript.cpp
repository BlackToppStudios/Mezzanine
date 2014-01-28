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
#ifndef _rubyscript_cpp
#define _rubyscript_cpp

#include "datatypes.h"

#ifdef MEZZRUBY

#include "rubyscript.h"
#include "rubyscriptargument.h"
#include "rubyscriptingengine.h"
#include "exception.h"

#include <algorithm>

/// @file
/// @brief This file has the implemetation for the Ruby script storage.

extern "C"
{
    #include "ruby.h"            // Ruby Core
    #include "rubylib.h"         // for opening the base state
    #include "lauxlib.h"        // Extra Ruby Goodies like ruby_open()

    int rubyopen_Mezzanine(ruby_State* L);
}

namespace Mezzanine
{
    namespace Scripting
    {
        namespace Ruby
        {

            ///////////////////////////////////////////////////////////////////////////////////////
            // RubyScript basics
            RubyScript::RubyScript() : FunctionCall(false)
                { }

            RubyScript::RubyScript(const String& InitialSourceCode, RubyScriptingEngine* Compiler, Bool JustAFunctionCall, String ScriptName)
                : SourceCode(InitialSourceCode),
                  Name(ScriptName),
                  FunctionCall(JustAFunctionCall)
            {

            }

            RubyScript::RubyScript(const String& InitialSourceCode, RubyScriptingEngine& Compiler, Bool JustAFunctionCall, String ScriptName)
                : SourceCode(InitialSourceCode),
                  Name(ScriptName),
                  FunctionCall(JustAFunctionCall)
            {
                if(!FunctionCall)
                    { Compile(&Compiler); }
            }

            RubyScript::~RubyScript()
                {}

            ///////////////////////////////////////////////////////////////////////////////////////
            // Arguments
            void RubyScript::AddArgument(CountedPtr<iScriptArgument> Arg)
                { Args.push_back(Arg); }

            void RubyScript::AddArgument(RubyIntegerArgument Arg)
                { Args.push_back(CountedPtr<iScriptArgument>(new RubyIntegerArgument(Arg))); }

            void RubyScript::AddArgument(RubyRealArgument Arg)
                { Args.push_back(CountedPtr<iScriptArgument>(new RubyRealArgument(Arg))); }

            void RubyScript::AddArgument(RubyWholeArgument Arg)
                { Args.push_back(CountedPtr<iScriptArgument>(new RubyWholeArgument(Arg))); }

            void RubyScript::AddArgument(RubyStringArgument Arg)
                { Args.push_back(CountedPtr<iScriptArgument>(new RubyStringArgument(Arg))); }

            void RubyScript::AddArgument(RubyBoolArgument Arg)
                { Args.push_back(CountedPtr<iScriptArgument>(new RubyBoolArgument(Arg))); }

            void RubyScript::AddArgument(RubyNilArgument Arg)
                { Args.push_back(CountedPtr<iScriptArgument>(new RubyNilArgument(Arg))); }

            void RubyScript::AddArgument(Integer Arg)
                { AddArgument(RubyIntegerArgument(Arg)); }

            void RubyScript::AddArgument(Real Arg)
                { AddArgument((RubyRealArgument(Arg))); }

            void RubyScript::AddArgument(Whole Arg)
                { AddArgument((RubyWholeArgument(Arg))); }

            void RubyScript::AddArgument(String Arg)
                { AddArgument((RubyStringArgument(Arg))); }

            void RubyScript::AddArgument(Char8 *Arg)
                { AddArgument((String(Arg))); }

            void RubyScript::AddArgument(Bool Arg)
                { AddArgument((RubyBoolArgument(Arg))); }

            void RubyScript::AddArgument()
                { Args.push_back(CountedPtr<iScriptArgument>(new RubyNilArgument)); }

            void RubyScript::RemoveArgument(CountedPtr<iScriptArgument> Arg)
                { Args.erase( std::remove(Args.begin(),Args.end(),Arg) ); }

            void RubyScript::RemoveArgument(Whole ArgNumber)
                { Args.erase(Args.begin()+ArgNumber); }

            Whole RubyScript::GetArgumentCount() const
                { return Args.size(); }

            void RubyScript::ClearArguments()
                { Args.clear(); }

            CountedPtr<iScriptArgument> RubyScript::GetArgument(Whole ArgNumber) const
                { return Args.at(ArgNumber); }

            ///////////////////////////////////////////////////////////////////////////////////////
            // Source code
            void RubyScript::SetSourceCode(const String& Code)
            {
                CompiledByteCode.DeleteBuffer();
                SourceCode = Code;
            }

            void RubyScript::SetSourceCode(const String& Code, CountedPtr<iScriptArgument> Return1)
            {
                SetSourceCode(Code);
                Returns.clear();
                Returns.push_back(Return1);
            }

            String RubyScript::GetSourceCode() const
                { return SourceCode; }

            void RubyScript::SetByteCode(BinaryTools::BinaryBuffer Code)
                { CompiledByteCode = Code; }

            BinaryTools::BinaryBuffer RubyScript::GetByteCode() const
                { return CompiledByteCode; }

            FlaggedBuffer& RubyScript::GetByteCodeReference()
                { return CompiledByteCode; }

            bool RubyScript::IsCompiled() const
                { return CompiledByteCode.Binary != 0; }

            void RubyScript::Compile(RubyScriptingEngine* Compiler)
                { Compiler->Compile(this); }

            void RubyScript::Compile(RubyScriptingEngine& Compiler)
                { Compiler.Compile(this); }

            ///////////////////////////////////////////////////////////////////////////////////////
            // Name
            String RubyScript::GetName() const
                { return Name; }

            void RubyScript::SetName(const String& NewName)
                { Name = NewName; }

            ///////////////////////////////////////////////////////////////////////////////////////
            // Returns
            Whole RubyScript::GetReturnCount() const
                { return Returns.size(); }

            void RubyScript::AddReturn(CountedPtr<iScriptArgument> ReturnArg)
                { Returns.push_back(ReturnArg); }

            ArgumentGroup RubyScript::GetAllReturns() const
                { return Returns; }
        } // Ruby
    } // Scripting
} // Mezzanine




#endif //  MEZZRUBY
#endif
