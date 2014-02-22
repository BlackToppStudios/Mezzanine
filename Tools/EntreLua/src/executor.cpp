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
///  @brief The imlementation of the command interpretter

#include <iostream>
#include <mezzanine.h>

#include "executor.h"

using namespace Mezzanine;
using namespace Mezzanine::Scripting;
using namespace std;

Executor::Executor(Mezzanine::Scripting::Lua::Lua51ScriptingEngine& TargetEngine)
    : LuaEngine(TargetEngine)
{}

ExecutionResults Executor::Do(Mezzanine::String CommandLine)
{
    if(CommandLine=="exit"||CommandLine=="quit")    // Any shell that does not do this is shit.
        { return ExecutionResults("Exiting",true); }
    if(CommandLine=="banner")
    {
        String Banner("Mezzanine Entrelua © Copyright 2010-2014 BlackTopp Studios Inc.\nLua 5.1 Copyright (C) 1994-2013 Lua.org, PUC-Rio\nLoaded libs:");
        for(int lib = Lua::Lua51ScriptingEngine::FirstLib;
            lib <= Lua::Lua51ScriptingEngine::LastLib;
            lib*=2)
        {
            if(LuaEngine.IsLibraryOpen( (Lua::Lua51ScriptingEngine::Lua51Libraries)lib ))
                { Banner += " " + LuaEngine.GetLibName((Lua::Lua51ScriptingEngine::Lua51Libraries)lib); }
        }
        return ExecutionResults(Banner,false);
    }

    //cout << CommandLine << endl;
    ExecutionResults Results;
    try
    {
        CountedPtr<iScript> CompletedScript=LuaEngine.Execute(CommandLine);
        ArgumentGroup Returned = CompletedScript->GetAsiScriptMultipleReturn()->GetAllReturns();
        for(ArgumentGroup::iterator Iter = Returned.begin(); Iter!=Returned.end(); Iter++)
        {
            Results.Returns.push_back( (*Iter)->GetString() );
            cout << (*Iter)->GetString() << endl;
        }
        //cout << LuaEngine.test() << " " << LuaEngine.tests() << endl;
    } catch (Exception& e) {
        Results.Output = e.what();
    }



    return Results;
}

