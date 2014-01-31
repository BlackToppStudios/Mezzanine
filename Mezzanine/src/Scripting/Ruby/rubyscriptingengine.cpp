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
#ifndef _rubyscriptingengine_cpp
#define _rubyscriptingengine_cpp

#include "datatypes.h"

#ifdef MEZZRUBY

#include "rubyscript.h"
#include "rubyscriptargument.h"
#include "rubyscriptingengine.h"
#include "exception.h"

#include <cstring>
/// @file
/// @brief This file has the implementation for the Ruby based Scripting system.

extern "C"
{
    #include "ruby.h"            // Ruby Core
    #include "rubylib.h"         // for opening the base state
    #include "lauxlib.h"        // Extra Ruby Goodies like ruby_open()

    int rubyopen_Mezzanine(ruby_State* L);
    int rubyopen_MezzanineSafe(ruby_State* L);

    int rubyopen_MezzanineXML(ruby_State* L);
    int rubyopen_MezzanineXMLSafe(ruby_State* L);

    int rubyopen_MezzanineThreading(ruby_State* L);
    int rubyopen_MezzanineThreadingSafe(ruby_State* L);
}

namespace Mezzanine
 {
    namespace Scripting
    {
        namespace Ruby
        {

            ///////////////////////////////////////////////////////////////////////////////////////
            // RubyScriptEngine and helpers
            namespace
            {


            }

            ///////////////////////////////////////////////////////////////////////////////////////
            // Publically visible internals

            ///////////////////////////////////////////////////////////////////////////////////////
            // Construction/Deconstruction
            RubyScriptingEngine::RubyScriptingEngine(RubyLibraries LibrariesToOpen) : State(rubyL_newstate())
                { OpenLibraries(LibrariesToOpen); }

            RubyScriptingEngine::~RubyScriptingEngine()
                { ruby_close(State); }

            ///////////////////////////////////////////////////////////////////////////////////////
            // Execution
            CountedPtr<iScript> RubyScriptingEngine::Execute(const String& ScriptSource)
            {
                CountedPtr<RubyScript> Results = Compile(ScriptSource);
                Execute(Results);
                return CountedPtrCast<iScript>(Results);
            }

            void RubyScriptingEngine::Execute(CountedPtr<iScript>& ScriptToRun)
            {
                CountedPtr<RubyScript> ScriptToCompile = CountedPtrCast<RubyScript>(ScriptToRun);
                if(ScriptToCompile)
                {
                    Execute(ScriptToCompile);
                }else{
                    MEZZ_EXCEPTION(Exception::PARAMETERS_CAST_EXCEPTION, "Ruby Engine attempted to execute a script, but it did not appear to bea Ruby script.")
                }
            }

            void RubyScriptingEngine::Execute(CountedPtr<RubyScript>& ScriptToRun)
                { Execute(ScriptToRun.Get()); }

            void RubyScriptingEngine::Execute(RubyScript& ScriptToRun)
                { Execute(&ScriptToRun); }

            void RubyScriptingEngine::Execute(RubyScript* ScriptToRun)
            {
                // We need to push all the arguments
                RubyArgument* Current;
                for(ArgumentGroup::const_iterator Iter = ScriptToRun->Args.begin();
                    Iter != ScriptToRun->Args.end();
                    Iter++ )
                {

                }

                // Do the actual script

                // Need to get return value
            }

            ///////////////////////////////////////////////////////////////////////////////////////
            // For Inheritance
            String RubyScriptingEngine::GetImplementationTypeName() const
                { return String("RubyScriptingEngine"); }

        } // Ruby
    } // Scripting
} // Mezzanine


#endif //  MEZZRUBY
#endif
