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
#ifndef _lua51tests_h
#define _lua51tests_h

#include "main.h"

using namespace Mezzanine;
using namespace Mezzanine::Testing;

/// @brief The group of tests for
class lua51tests : public UnitTestGroup
{
    public:
        /// @copydoc Mezzanine::Testing::UnitTestGroup::Name
        /// @return Returns a String containing "lua51"
        virtual String Name()
            { return String("lua51"); }

        /// @copydoc Mezzanine::Testing::UnitTestGroup::RunTests
        /// @warning Totally incomplete, when run this will prove nothing.
        virtual void RunTests(bool RunAutomaticTests, bool RunInteractiveTests)
        {
            if (RunAutomaticTests)
            {
                {
                    Scripting::Lua::Lua51ScriptingEngine LuaRuntimeSafe(Scripting::Lua::Lua51ScriptingEngine::MezzSafeLib);
                    Scripting::Lua::Lua51ScriptingEngine LuaRuntimeAll(Scripting::Lua::Lua51ScriptingEngine::MezzSafeLib);

                    try
                    {
                        Scripting::Lua::Lua51Script Hello("Print \"Hello World!\"",&LuaRuntimeSafe); //From pointer
                        AddTestResult("Lua51::ScriptCompileOnConstruction1", Success);
                    } catch (ScriptLuaException& e) {
                        AddTestResult("Lua51::ScriptCompileOnConstruction1", Failed);
                    }

                    try
                    {
                        Scripting::Lua::Lua51Script Hello("Print \"Hello World!\"",LuaRuntimeSafe); // from reference
                        AddTestResult("Lua51::ScriptCompileOnConstruction2", Success);
                    } catch (ScriptLuaException& e) {
                        AddTestResult("Lua51::ScriptCompileOnConstruction2", Failed);
                    }




                }


            }else{
                AddTestResult("Lua51::ScriptCompileOnConstruction1", Skipped);
                AddTestResult("Lua51::ScriptCompileOnConstruction1", Skipped);
            }
        }
};

#endif

