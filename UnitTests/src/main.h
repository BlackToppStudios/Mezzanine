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
#ifndef _main_h
#define _main_h

#include <utility> //For pair
#include <map>
#include <vector>
#include <iostream>
#include <fstream>

#include <cctype> // For tolower
#include <cstdio> // For remove

#include <mezzanine.h> // For String and all of Mezzanine

#include "testenumerations.h"
#include "consolestringmanipulation.h"
#include "testdata.h"



/////////////////////////////////////////////////////////////////////////////////////
//This next block of code creates a minimal engine environment for testing managers and other ngine components
using namespace Mezzanine;

Entresol* TheEntresol;
Mezzanine::UI::Caption *TheTextW1;
Mezzanine::UI::Caption *TheTextB1;
Mezzanine::UI::Caption *TheTextW2;
Mezzanine::UI::Caption *TheTextB2;
String TheMessage;
String TheMessage2;
SimpleTimer *ThisTimer;
TestResult AnswerToQuestion = Unknown;

void StartEngine()
{
    PhysicsConstructionInfo Info;
    Info.PhysicsFlags = (PhysicsConstructionInfo::PCF_LimitlessWorld | PhysicsConstructionInfo::PCF_SoftRigidWorld);
    TheEntresol = new Entresol(Info,"DefaultSceneManager","plugins.cfg","data/");
    TheEntresol->EngineInit(false);
    ResourceManager::GetSingletonPtr()->AddAssetLocation("data", FileSystem, "files", false);
    GraphicsManager::GetSingletonPtr()->CreateGameWindow("EventManager Test",800,600,0);
    UIManager::GetSingletonPtr()->LoadMTA("dejavu");
    ResourceManager::GetSingletonPtr()->InitAssetGroup("files");
    Mezzanine::UI::Screen* TheScreen = UIManager::GetSingletonPtr()->CreateScreen("Screen","dejavu",GraphicsManager::GetSingletonPtr()->GetGameWindow(0)->GetViewport(0));

    Mezzanine::Entresol::GetSingletonPtr()->Log("Found Resolutions:");
    const std::vector<String>* Resolutions = Mezzanine::Entresol::GetSingletonPtr()->GetGraphicsManager()->GetSupportedResolutions();
    for (Whole Count=0; Count<Resolutions->size(); ++Count)
    {
        Mezzanine::Entresol::GetSingletonPtr()->Log(Resolutions->at(Count));
    }
    Mezzanine::Entresol::GetSingletonPtr()->CommitLog();

    UI::OpenRenderableContainerWidget* HUDContainer = TheScreen->CreateOpenRenderableContainerWidget("HUD");
    TheScreen->AddRootWidget(0,HUDContainer);

    TheTextB1 = HUDContainer->CreateCaption(ConstString("TheTextB1"),UI::RenderableRect(Vector2(0.0016,0.603),Vector2(1,0.25),true), (Whole)24, TheMessage);
    TheTextB1->SetTextColour(ColourValue::Black());
    TheTextB1->SetBackgroundColour(ColourValue::Transparent());
    TheTextW1 = HUDContainer->CreateCaption(ConstString("TheTextW1"),UI::RenderableRect(Vector2(0,0.6),Vector2(1,0.25),true), (Whole)24, TheMessage);
    TheTextW1->SetTextColour(ColourValue::White());
    TheTextW1->SetBackgroundColour(ColourValue::Transparent());

    TheTextB2 = HUDContainer->CreateCaption(ConstString("TheTextB2"),UI::RenderableRect(Vector2(0.0016,0.753),Vector2(1.0,0.25),true), (Whole)24, TheMessage2);
    TheTextB2->SetTextColour(ColourValue::Black());
    TheTextB2->SetBackgroundColour(ColourValue::Transparent());
    TheTextW2 = HUDContainer->CreateCaption(ConstString("TheTextW2"),UI::RenderableRect(Vector2(0,0.75),Vector2(1.0,0.25),true), (Whole)24, TheMessage2);
    TheTextW2->SetTextColour(ColourValue::White());
    TheTextW2->SetBackgroundColour(ColourValue::Transparent());

}

void UpdateMessage( String Message, String Message2)
{
    TheTextB1->SetText( Message );
    TheTextW1->SetText( Message );
    TheTextB2->SetText( Message2 );
    TheTextW2->SetText( Message2 );
}

// Countdown callbacks
bool PostTimerEnd()
{
    return false;
    TimerManager::GetSingletonPtr()->SetPostMainLoopItems(0);
}

bool PostTimerUpdate()
    { UpdateMessage(TheMessage,ToString(int(ThisTimer->GetCurrentTime()/1000000))); return true; }

class TimerEnding : public TimerCallback
{
    virtual void DoCallbackItems()
        { TimerManager::GetSingletonPtr()->SetPostMainLoopItems(&PostTimerEnd); }
};
TimerEnding* CountDownCallback;

void StopEngine()
{
    delete CountDownCallback;
    delete TheEntresol;
    crossplatform::WaitMilliseconds(1000); // Ogre spawns some stuff in a seperate thread this is more then enough time for it to finish
}

bool PostInputCheck()
{
    UpdateMessage(TheMessage,TheMessage2);
    AnswerToQuestion=Unknown;
    EventManager* EventMan = EventManager::GetSingletonPtr();
    EventUserInput* ThisInput = EventMan->PopNextUserInputEvent();
    while (ThisInput) //for each userinput event
    {
        for (Whole c=0; c<ThisInput->GetMetaCodeCount(); ++c ) //for each metacode in each userinput
        {
            Input::InputCode ThisCode = ThisInput->GetMetaCode(c).GetCode();
            switch(ThisCode)
            {
                case Input::KEY_T: case Input::KEY_Y: // True or Yes
                    AnswerToQuestion = Success;
                    break;
                case Input::KEY_F: case Input::KEY_N: // False or No
                    AnswerToQuestion = Failed;
                    break;
                case Input::KEY_C:                       // Cancel
                    AnswerToQuestion = Cancelled;
                    break;
                case Input::KEY_U: case Input::KEY_I: // Unknown or Inconclusive
                    AnswerToQuestion = Inconclusive;
                    break;
                default:
                    AnswerToQuestion = Unknown;
                    break;
            }
        }
        delete ThisInput;
        ThisInput = EventMan->PopNextUserInputEvent();
    }

    if (Unknown==AnswerToQuestion)
        { return true; }
    else
        { return false; }
}

// Use this to start a countdown in the interactive test
void StartCountdown(Whole Seconds)
{
    ThisTimer = TimerManager::GetSingletonPtr()->CreateSimpleTimer(Timer::StopWatch);
    ThisTimer->SetInitialTime(Seconds * 1000000);
    ThisTimer->Reset();
    ThisTimer->SetGoalTime(0);
    ThisTimer->Start();
    CountDownCallback = new TimerEnding;
    ThisTimer->SetCallback(CountDownCallback);
    TimerManager::GetSingletonPtr()->SetPostMainLoopItems(&PostTimerUpdate);
}

void GetAnswer()
{
    TheMessage2 = "T/Y:Yes  F/N:No  C:Cancel  U/I:Inconclusive";
    UpdateMessage(TheMessage, TheMessage2);
    EventManager::GetSingletonPtr()->SetPostMainLoopItems(&PostInputCheck);
}

///////////////////////////////////////////////////////////////////////////////
// Temp File Manipulation
///////////////////////////////////////

/// @internal
/// @brief This is the name of the file used to communicate results from child processes
/// @warning This variable is used to create temporary files in a percieved insecure way
/// Everything will be fine as long as nothing else writes to this this file during or
/// between Tests. If something does, then you probably have big enough problems you
/// shouldn't be developing software until that is fixed.
static const String TempFile("UnitTestWork.txt");

/// @internal
/// @brief Empty the file specified by @ref TempFile
/// @warning This doesn't ask for permission and can't easily be cancelled or recovered
/// from. This will open, then erase the contents of the file.
/// @throw This can throw any exception that the C++ filestream classes can throw.
void ClearTempFile()
{
    std::ofstream FileToClear;
    FileToClear.open(TempFile.c_str(),std::ios_base::out|std::ios_base::trunc); // Clear the work file.
    FileToClear.close();
}

/// @internal
/// @brief This will open then parse the contents of the file specified by @ref TempFile and interpret any test results located
/// @throw This can throw any exception that the C++ filestream classes can throw.
/// @return This "reads" the temp file and interprets it. It tries to extract the name of the test as the whole of a line minus
/// the last word. The last word is then converted into a @ref TestResult using @ref StringToTestResult . Any Whitespace between
/// between the end of the last word and the end of the test name is dropped. All lines are interpretted this way and returned
/// as a single @ref UnitTestGroup.
UnitTestGroup GetResultsFromTempFile()
{
    UnitTestGroup Results;
    std::vector<Mezzanine::String> FileContents;

    char SingleLine[1024];
    std::ifstream TheSourceFile(TempFile.c_str());
    while( TheSourceFile.good() )
    {
        TheSourceFile.getline(SingleLine,1024);
        FileContents.push_back(Mezzanine::String(SingleLine));
    }

    if(TheSourceFile.eof()) // We successfully reached the end of the file
        { }
    if(TheSourceFile.bad()) // We fail somehow
        { std::cerr << "Error reading temp file." << std::endl; }
    TheSourceFile.close();

    for(std::vector<Mezzanine::String>::iterator Iter=FileContents.begin(); FileContents.end()!=Iter; ++Iter) // for each line in the file, that is now loaded in RAM
    {
        if(rtrim(*Iter).size()) // If there is more than whitespace
        {
            Results.AddTestResult(StringToTestData(*Iter));
        }
    }

    return Results;
}

/// @brief Attempts to delete temp file. Silently fails if not possible.
void DeleteTempFile()
{
    remove(TempFile.c_str());
}

#endif
