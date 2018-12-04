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

/// @file The implementation for a full featured line noise based REPL

#include <mezzanine.h>
#include "linenoise/linenoise.h"
#include "repllinenoise.h"
#include <algorithm>

using namespace Mezzanine;
using namespace std;
using namespace Mezzanine::Scripting::Lua;

void REPLLineNoise::PopulateCommmandTrie()
{
    try
    {
        PossibleCommands.clear();
        Doer.LuaEngine.PopulateTabCompletionTrie(PossibleCommands);
    } catch (Mezzanine::ExceptionBase& e) {
        PossibleCommands.clear();
        cout << "\r\n" << e.what() << "\r\n";
    }
}

String GetMatchingPrefix(const String& StringA, const String& StringB)
{
    Whole MatchCount;
    for(MatchCount=0; MatchCount<=StringA.size() && MatchCount<=StringB.size(); MatchCount++)
    {
        if(StringA[MatchCount]!=StringB[MatchCount])
        {
            //MatchCount--;
            break;
        }
    }
    return String(StringA.begin(),StringA.begin()+MatchCount);
}

void REPLLineNoise::DoTabStrike1(linenoiseCompletions *lc, const String& CurrentID, const String& CurrentLine)
{
    if(0==lc->len)
    {
        PopulateCommmandTrie();
        Lua51ScriptingEngine::CommandTrie::iterator Iter = PossibleCommands.startsWith(CurrentID.c_str());
        // If there is exactly one possible completion, put it in.
        if(PossibleCommands.end()!=Iter)
        {
            Lua51ScriptingEngine::CommandTrie::iterator IterPlusOne=Iter;
            IterPlusOne++;
            if(PossibleCommands.end()==IterPlusOne)
            {
                //Figure out what is on the line that is not the current ID
                String CompletionPrefix(CurrentLine,0,CurrentLine.size()-CurrentID.size());
                String NewInput(CompletionPrefix+(*Iter).first);
                linenoiseAddCompletion(lc,NewInput.c_str());
                linenoiseAddCompletion(lc,NewInput.c_str());
            }else{
                String Matching((*Iter).first);
                for(; PossibleCommands.end()!=Iter; Iter++)
                {
                    Matching=GetMatchingPrefix(Matching,(*Iter).first);
                    if(Matching.size()==0)
                        { break; }
                }
                String CompletionPrefix(CurrentLine,0,CurrentLine.size()-CurrentID.size());
                String NewInput(CompletionPrefix+Matching);
                linenoiseAddCompletion(lc,NewInput.c_str());
                linenoiseAddCompletion(lc,NewInput.c_str());
            }
        }else{
            // Even if there isn't something to autocomplete, to get the line to redraw
            // we need to put something into the autocomplete buffer.
            linenoiseAddCompletion(lc,CurrentLine.c_str());
            linenoiseAddCompletion(lc,CurrentLine.c_str());
        }
    }
}

void REPLLineNoise::DisplayOtherIdentifiers(const String& Typename, vector<String>& Printing)
{
    if(Printing.size())
    {
        cout << Typename << ":\r\n";
        static const Whole Width = 80;
        static const String Spacer("  ");
        Whole MaxIDLength = 0;
        for(vector<String>::const_iterator Iter = Printing.begin();
            Iter!=Printing.end();
            Iter++)
            { if(Iter->size()>MaxIDLength) { MaxIDLength=Iter->size(); } }
        const Whole PerLineUnadjusted = (Width-Spacer.size())/(MaxIDLength+Spacer.size());
        const Whole PerLine = PerLineUnadjusted ? PerLineUnadjusted : 1;
        const Whole IDWidth = (Width-Spacer.size())/PerLine;
        Whole Counter = 0;
        for(vector<String>::const_iterator Iter = Printing.begin();
            Iter!=Printing.end();
            Iter++)
        {
            cout << Spacer << *Iter;
            for (Whole SpaceCounter=Spacer.size()+Iter->size();
                 SpaceCounter<IDWidth;
                 SpaceCounter++)
                { cout << " "; }
            Counter++;
            if(Counter%PerLine==0)
            {
                cout << "\r\n";
                Counter=0;
            }
        }
        if(Counter%PerLine!=0)
            { cout << "\r\n"; }
    }
}

void REPLLineNoise::DoTabStrike2(const String& CurrentID)
{
    vector<String> Functions;
    vector<String> Tables;
    vector<String> Numbers;
    vector<String> Nils;
    vector<String> Booleans;
    vector<String> Threads;
    vector<String> UserDatas;
    vector<String> LightUserDatas;
    vector<String> Strings;
    Whole Found = 0;
    for(Lua51ScriptingEngine::CommandTrie::iterator Iter = PossibleCommands.startsWith(CurrentID.c_str());
        Iter!=PossibleCommands.end();
        Iter++)
    {
        Found++;
        if(&Scripting::Lua::Lua51ScriptingEngine::TypeNameFunction==*(Iter->second))
            { Functions.push_back(Iter->first); }
        if(&Scripting::Lua::Lua51ScriptingEngine::TypeNameTable==*(Iter->second))
            { Tables.push_back(Iter->first); }
        if(&Scripting::Lua::Lua51ScriptingEngine::TypeNameNumber==*(Iter->second))
            { Numbers.push_back(Iter->first); }
        if(&Scripting::Lua::Lua51ScriptingEngine::TypeNameNil==*(Iter->second))
            { Nils.push_back(Iter->first); }
        if(&Scripting::Lua::Lua51ScriptingEngine::TypeNameBoolean==*(Iter->second))
            { Booleans.push_back(Iter->first); }
        if(&Scripting::Lua::Lua51ScriptingEngine::TypeNameThread==*(Iter->second))
            { Threads.push_back(Iter->first); }
        if(&Scripting::Lua::Lua51ScriptingEngine::TypeNameUserData==*(Iter->second))
            { UserDatas.push_back(Iter->first); }
        if(&Scripting::Lua::Lua51ScriptingEngine::TypeNameLightUserData==*(Iter->second))
            { LightUserDatas.push_back(Iter->first); }
        if(&Scripting::Lua::Lua51ScriptingEngine::TypeNameString==*(Iter->second))
            { Strings.push_back(Iter->first); }
    }

    if(Found)
        { cout << "\r\n"; }

    DisplayOtherIdentifiers(Scripting::Lua::Lua51ScriptingEngine::TypeNameFunction, Functions);
    DisplayOtherIdentifiers(Scripting::Lua::Lua51ScriptingEngine::TypeNameTable, Tables);
    DisplayOtherIdentifiers(Scripting::Lua::Lua51ScriptingEngine::TypeNameNumber, Numbers);
    DisplayOtherIdentifiers(Scripting::Lua::Lua51ScriptingEngine::TypeNameNil, Nils);
    DisplayOtherIdentifiers(Scripting::Lua::Lua51ScriptingEngine::TypeNameBoolean, Booleans);
    DisplayOtherIdentifiers(Scripting::Lua::Lua51ScriptingEngine::TypeNameThread, Threads);
    DisplayOtherIdentifiers(Scripting::Lua::Lua51ScriptingEngine::TypeNameUserData, UserDatas);
    DisplayOtherIdentifiers(Scripting::Lua::Lua51ScriptingEngine::TypeNameLightUserData, LightUserDatas);
    DisplayOtherIdentifiers(Scripting::Lua::Lua51ScriptingEngine::TypeNameString, Strings);
}

String REPLLineNoise::GetCurrentID(const String& CurrentLine)
{
    String::const_iterator Iter = CurrentLine.end();
    if(CurrentLine.begin() == Iter)
        { return String(); }
    String Results;
    Boole Started = false;
    // Start from end of string looking for characters which are valid in Lua Identifiers
    for(Iter--; Iter != CurrentLine.begin(); Iter--)
    {
        if(Scripting::Lua::Lua51ScriptingEngine::IsValidCharInTableIdentifier(*Iter))
        {
            Results = *Iter + Results;
            Started = true;
        }else{
            if(Scripting::Lua::Lua51ScriptingEngine::IsValidCharStartIdentifier(*(Iter+1)) && Started)
                { return String(Iter+1,CurrentLine.end()); }
            return String();
        }
    }
    return CurrentLine;
}

REPLLineNoise::REPLLineNoise(Executor& TargetExecutor, Mezzanine::String StartingPrompt, Mezzanine::String StartingMultiline, Mezzanine::String StartingReturn)
    : REPL(TargetExecutor, StartingPrompt, StartingMultiline, StartingReturn),
      PossibleCommands(0)
{}

void REPLLineNoise::TabCompletion(const char *CurrentInput, linenoiseCompletions *lc)
{
    const String CurrentLine(CurrentInput);
    const String CurrentID(GetCurrentID(CurrentInput));

    // This function gets called once per tab strike, this will tell us which tab strike we are on
    static int TabCount = 0;
    static String LastInput = String();
    if(CurrentLine==LastInput)
    {
        TabCount++;
        if (TabCount==3)
            { TabCount=0; }
    }else{
        TabCount = 1;
        LastInput = CurrentLine;
    }

    if (TabCount==1)
    {
        //One tab is a single suggestion twice to insure rotation displays it correctly
        DoTabStrike1(lc, CurrentID, CurrentLine);
    }

    if (TabCount==2)
    {
        // second tab strike will present options like bash, but with further distinction
        DoTabStrike2(CurrentID);
    }
    // third tab strike will be rotated by linenoise back to original input

}

/// @brief The REPL that the function TabCompletionCallback will work with
REPLLineNoise* TargetRepl=0;
/// @brief Line noise requires a c function pointer for a callback, this is it.
/// @param CurrentInput A character array linenoise provides, that contains what the user has typed so far
/// @param lc A linenoise internal structure that stores possible lines for tab completion
void TabCompletionCallback(const char *CurrentInput, linenoiseCompletions *lc)
    { TargetRepl->TabCompletion(CurrentInput,lc); }

void REPLLineNoise::Launch()
{
    //Prepare Values
    String CurrentInput;
    ExecutionResults CurrentResults = Doer.Do("banner");
    String HistoryDir(Resource::GetCurrentUserDataDir() + Resource::GetDirectorySeparator_Universal() + ".EntreLua" + Resource::GetDirectorySeparator_Universal());
    String HistoryFile("history.txt");
    String HistoryFullName(HistoryDir+HistoryFile);
    TargetRepl=this;

    // Prepare History File
    Resource::CreateDirectoryPath(HistoryDir);
    fstream(HistoryFullName.c_str(),ios_base::app);
    if(-1==linenoiseHistoryLoad(HistoryFullName.c_str()))
        { cerr << "Error loading Command history" << endl; }

    // Actually do the command shell stuff
    linenoiseSetCompletionCallback(TabCompletionCallback);
    char* RawLine;
    while(!CurrentResults.Quit)
    {
        if(CurrentResults.Output.size())
        {
            cout << CurrentResults.Output << endl;
            CurrentResults.Output.clear();
        }
        RawLine = linenoise(Prompt.c_str());
        if(NULL==RawLine)
        {
            CurrentResults.Quit = true;
            cout << endl;
        }
        else // All Good!
        {
            linenoiseHistoryAdd(RawLine);
            linenoiseHistorySave(HistoryFullName.c_str()); /* Save the history on disk. */
            CurrentInput = String(RawLine);
            CurrentResults = Doer.Do(CurrentInput);
            //cout << CurrentResults.Output;
        }
        free(RawLine);
    }


}

