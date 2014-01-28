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
#ifndef _rubyworkunit_cpp
#define _rubyworkunit_cpp

#include "datatypes.h"

#ifdef MEZZRUBY

#include "rubyworkunit.h"
#include "rubyscriptingengine.h"

/// @file
/// @brief This file has the implementation a workunit that can execute ruby script every frame

namespace Mezzanine
{
    namespace Scripting
    {
        namespace Ruby
        {
            RubyWorkUnit::RubyWorkUnit(RubyScriptingEngine* TargetRuntime)
                : RubyRuntime(TargetRuntime)
            {}

            RubyWorkUnit::~RubyWorkUnit()
                {}

            void RubyWorkUnit::push_back(CountedPtr<RubyScript> ScriptToAdd)
                { ScriptsToRun.push_back(ScriptToAdd); }

            void RubyWorkUnit::AddScript(CountedPtr<iScript> ScriptToAdd)
            {
                CountedPtr<RubyScript> ScriptToStow = CountedPtrCast<RubyScript>(ScriptToAdd);
                if(ScriptToStow)
                {
                    AddScript(ScriptToStow);
                }else{
                    MEZZ_EXCEPTION(Exception::PARAMETERS_CAST_EXCEPTION, "RubyWorkUnit attempted to store a script, but it did not appear to bea Ruby script.")
                }
            }

            void RubyWorkUnit::AddScript(CountedPtr<RubyScript> ScriptToAdd)
                { push_back(ScriptToAdd); }

            RubyWorkUnit::iterator RubyWorkUnit::find(CountedPtr<RubyScript> Target)
            {
                for(RubyWorkUnit::iterator Iter=this->begin(); Iter!=this->end() ; Iter++)
                {
                    if(*Iter == Target)
                        { return Iter; }
                }
                return this->end();
            }

            RubyWorkUnit::const_iterator RubyWorkUnit::find(CountedPtr<RubyScript> Target) const
            {
                for(RubyWorkUnit::const_iterator Iter=this->begin(); Iter!=this->end(); Iter++)
                {
                    if(*Iter == Target)
                        { return Iter; }
                }
                return this->end();
            }

            void RubyWorkUnit::erase(CountedPtr<RubyScript> Target)
            {
                RubyWorkUnit::iterator Iter = this->find(Target);
                if(Iter!=this->end())
                    { ScriptsToRun.erase(Iter); }
            }

            void RubyWorkUnit::erase(RubyWorkUnit::iterator Target)
            { ScriptsToRun.erase(Target); }

            void RubyWorkUnit::RemoveScript(CountedPtr<iScript> ScriptToRemove)
            {
                CountedPtr<RubyScript> ScriptToBtow = CountedPtrCast<RubyScript>(ScriptToRemove);
                if(ScriptToBtow)
                {
                    RemoveScript(ScriptToBtow);
                }else{
                    MEZZ_EXCEPTION(Exception::PARAMETERS_CAST_EXCEPTION, "RubyWorkUnit attempted to removed a script, but it did not appear to bea Ruby script.")
                }
            }

            void RubyWorkUnit::RemoveScript(CountedPtr<RubyScript> ScriptToRemove)
                { erase(ScriptToRemove); }

            void RubyWorkUnit::RemoveScript(Whole Index)
                { erase(this->begin()+Index); }

            RubyWorkUnit::iterator RubyWorkUnit::begin()
                { return ScriptsToRun.begin(); }
            RubyWorkUnit::const_iterator RubyWorkUnit::begin() const
                { return ScriptsToRun.begin(); }

            RubyWorkUnit::iterator RubyWorkUnit::end()
                { return ScriptsToRun.end(); }
            RubyWorkUnit::const_iterator RubyWorkUnit::end() const
                { return ScriptsToRun.end(); }

            Whole RubyWorkUnit::GetScriptCount() const
                { return ScriptsToRun.size(); }

            void RubyWorkUnit::ClearScripts()
                { ScriptsToRun.clear(); }

            CountedPtr<iScript> RubyWorkUnit::GetScript(Whole Index)
                { return CountedPtrCast<iScript>(ScriptsToRun.at(Index)); }

            CountedPtr<RubyScript> RubyWorkUnit::GetRubyScript(Whole Index) const
                { return ScriptsToRun.at(Index); }

            void RubyWorkUnit::DoWork(Threading::DefaultThreadSpecificStorage::Type& CurrentThreadStorage)
            {
                Logger& Log = CurrentThreadStorage.GetUsableLogger();
                Log << "Running Per Frame RubyScripts." << std::endl;
                for(iterator Iter = this->begin(); Iter!=this->end(); Iter++)
                {
                    #ifdef MEZZ_DEBUG
                    Log << "Executing \"" << (*Iter)->GetName() << "\"" << std::endl;
                    #endif
                    RubyRuntime->Execute(*Iter);
                }
            }


        } // Ruby
    } // Scripting
} // Mezzanine




#endif //  MEZZRUBY
#endif
