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

/// @file The implementation for a full featured line noise based REPL

#include <mezzanine.h>
#include "linenoise/linenoise.h"
#include "repllinenoise.h"

using namespace Mezzanine;
using namespace std;

REPLLineNoise::REPLLineNoise(Executor& TargetExecutor, Mezzanine::String StartingPrompt, Mezzanine::String StartingMultiline, Mezzanine::String StartingReturn)
    : REPL(TargetExecutor, StartingPrompt, StartingMultiline, StartingReturn)
{}

Trie<char,String> BigList('+');

String GetCurrentID(const String& CurrentLine)
{
    String::const_iterator Iter = CurrentLine.end();
    if(CurrentLine.begin() == Iter)
        { return String(); }
    String Results;
    Boole Started = false;
    // Start from end of string looking for characters which are valid in Lua Identifiers
    for(Iter--; Iter != CurrentLine.begin(); Iter--)
    {
        if(Scripting::Lua::Lua51ScriptingEngine::IsValidCharInIdentifier(*Iter))
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

/// @brief Tab completion callback
/// @param CurrentInput The Current Input on the command line
/// @param lc Line completionforthis current iteration of thetab press
void TabCompletion(const char *CurrentInput, linenoiseCompletions *lc)
{
    const String CurrentLine(CurrentInput);
    const String CurrentID(GetCurrentID(CurrentInput));
    //cout << "\n\r" << CurrentID;


    // This function gets called once per tab strike, this will tell us which tab hit we are on
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
        if(0==lc->len)
        {
            linenoiseAddCompletion(lc,(CurrentLine+"hello").c_str());
            linenoiseAddCompletion(lc,(CurrentLine+"hello").c_str());
        }
    }
    if (TabCount==2)
    {
        // second tab strike will present options like bash
        cout << "\r\n" << "\tASdf1\tASdf2" << "\r\n";
    }
    // third will be rotated by linenoise back to original input

}

void REPLLineNoise::Launch()
{
    //Prepare Values
    String CurrentInput;
    ExecutionResults CurrentResults = Doer.Do("banner");
    String HistoryDir(ResourceManager::GetCurrentUserDataDir() + ResourceManager::GetDirectorySeparator() + ".EntreLua" + ResourceManager::GetDirectorySeparator());
    String HistoryFile("history.txt");
    String HistoryFullName(HistoryDir+HistoryFile);

    // Prepare History File
    ResourceManager::CreateDirectoryPath(HistoryDir);
    fstream(HistoryFullName.c_str(),ios_base::app);
    if(-1==linenoiseHistoryLoad(HistoryFullName.c_str()))
        { cerr << "Error loading Command history" << endl; }

    // Actually do the command shell stuff
    linenoiseSetCompletionCallback(TabCompletion);
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

