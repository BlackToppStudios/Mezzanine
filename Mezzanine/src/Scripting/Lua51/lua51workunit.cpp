// © Copyright 2010 - 2013 BlackTopp Studios Inc.
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
#ifndef _lua51workunit_cpp
#define _lua51workunit_cpp

#include "datatypes.h"

#ifdef MEZZLUA51

#include "lua51workunit.h"
#include "lua51scriptingengine.h"

/// @file
/// @brief This file has the implementation a workunit that can execute lua script every frame

namespace Mezzanine
{
    namespace Scripting
    {
        namespace Lua
        {
            Lua51WorkUnit::Lua51WorkUnit(Lua51ScriptingEngine* TargetRuntime)
                : LuaRuntime(TargetRuntime)
            {}

            Lua51WorkUnit::~Lua51WorkUnit()
                {}

            void Lua51WorkUnit::push_back(CountedPtr<Lua51Script> ScriptToAdd)
                { ScriptsToRun.push_back(ScriptToAdd); }

            void Lua51WorkUnit::AddScript(CountedPtr<iScript> ScriptToAdd)
            {
                CountedPtr<Lua51Script> ScriptToStow = CountedPtrCast<Lua51Script>(ScriptToAdd);
                if(ScriptToStow)
                {
                    AddScript(ScriptToStow);
                }else{
                    MEZZ_EXCEPTION(Exception::PARAMETERS_CAST_EXCEPTION, "Lua51WorkUnit attempted to store a script, but it did not appear to bea Lua51 script.")
                }
            }

            void Lua51WorkUnit::AddScript(CountedPtr<Lua51Script> ScriptToAdd)
                { push_back(ScriptToAdd); }

            Lua51WorkUnit::iterator Lua51WorkUnit::find(CountedPtr<Lua51Script> Target)
            {
                for(Lua51WorkUnit::iterator Iter=this->begin(); Iter!=this->end() ; Iter++)
                {
                    if(*Iter == Target)
                        { return Iter; }
                }
                return this->end();
            }

            Lua51WorkUnit::const_iterator Lua51WorkUnit::find(CountedPtr<Lua51Script> Target) const
            {
                for(Lua51WorkUnit::const_iterator Iter=this->begin(); Iter!=this->end(); Iter++)
                {
                    if(*Iter == Target)
                        { return Iter; }
                }
                return this->end();
            }

            void Lua51WorkUnit::erase(CountedPtr<Lua51Script> Target)
            {
                Lua51WorkUnit::iterator Iter = this->find(Target);
                if(Iter!=this->end())
                    { ScriptsToRun.erase(Iter); }
            }

            void Lua51WorkUnit::erase(Lua51WorkUnit::iterator Target)
            { ScriptsToRun.erase(Target); }

            void Lua51WorkUnit::RemoveScript(CountedPtr<iScript> ScriptToRemove)
            {
                CountedPtr<Lua51Script> ScriptToBtow = CountedPtrCast<Lua51Script>(ScriptToRemove);
                if(ScriptToBtow)
                {
                    RemoveScript(ScriptToBtow);
                }else{
                    MEZZ_EXCEPTION(Exception::PARAMETERS_CAST_EXCEPTION, "Lua51WorkUnit attempted to removed a script, but it did not appear to bea Lua51 script.")
                }
            }

            void Lua51WorkUnit::RemoveScript(CountedPtr<Lua51Script> ScriptToRemove)
                { erase(ScriptToRemove); }

            void Lua51WorkUnit::RemoveScript(Whole Index)
                { erase(this->begin()+Index); }

            Lua51WorkUnit::iterator Lua51WorkUnit::begin()
                { return ScriptsToRun.begin(); }
            Lua51WorkUnit::const_iterator Lua51WorkUnit::begin() const
                { return ScriptsToRun.begin(); }

            Lua51WorkUnit::iterator Lua51WorkUnit::end()
                { return ScriptsToRun.end(); }
            Lua51WorkUnit::const_iterator Lua51WorkUnit::end() const
                { return ScriptsToRun.end(); }

            Whole Lua51WorkUnit::GetScriptCount() const
                { return ScriptsToRun.size(); }

            void Lua51WorkUnit::ClearScripts()
                { ScriptsToRun.clear(); }

            CountedPtr<iScript> Lua51WorkUnit::GetScript(Whole Index)
                { return CountedPtrCast<iScript>(ScriptsToRun.at(Index)); }

            CountedPtr<Lua51Script> Lua51WorkUnit::GetLua51Script(Whole Index) const
                { return ScriptsToRun.at(Index); }

            void Lua51WorkUnit::DoWork(Threading::DefaultThreadSpecificStorage::Type& CurrentThreadStorage)
            {
                Logger& Log = CurrentThreadStorage.GetUsableLogger();
                Log << "Running Per Frame LuaScripts." << std::endl;
                for(iterator Iter = this->begin(); Iter!=this->end(); Iter++)
                {
                    #ifdef MEZZ_DEBUG
                    Log << "Executing \"" << (*Iter)->GetName() << "\"" << std::endl;
                    #endif
                    LuaRuntime->Execute(*Iter);
                }
            }


        } // Lua
    } // Scripting
} // Mezzanine




#endif //  MEZZLUA51
#endif
