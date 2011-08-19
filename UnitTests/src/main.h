//© Copyright 2010 - 2011 BlackTopp Studios Inc.
/* This file is part of The PhysGame Engine.

    The PhysGame Engine is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The PhysGame Engine is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with The PhysGame Engine.  If not, see <http://www.gnu.org/licenses/>.
*/
/* The original authors have included a copy of the license specified above in the
   'Docs' folder. See 'gpl.txt'
*/
/* We welcome the use of The PhysGame anyone, including companies who wish to
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
#include <iostream>

#include <cctype> //for tolower

#include <physgame.h> // For String and all of physgame


// Define some Results of tests
enum TestResult
{
    Success         = 0,        // test was ran and appeared to work
    Skipped         = 1,        // Test was simply not ran
    Cancelled       = 2,        // Was canceled by user, so success is unknown, but user doesn't cared
    Inconclusive    = 3,        // if a user answers that they don't know what happened in a test that involved interaction, it likely worked, but we can't be sure
    Failed          = 4,        // Known failure
    Unknown         = 5,        // Since we don't know what happened this is the worst kind of failure
    NotApplicable   = 6         // This is not even a kind of failure, This is used to when referencing a test, so if this winds up coming out of a test, then something has failed
};

// convert the above enum to a Striong that matchins the in-code name.
phys::String TestResultToString(TestResult Convertable)
{
    switch(Convertable)
    {
        case Success:
            return "Success";
        case Skipped:
            return "Skipped";
        case Cancelled:
            return "Cancelled";
        case Inconclusive:
            return "Inconclusive";
        case Failed:
            return "Failed";
        case Unknown:
            return "Unknown";
        case NotApplicable:
            return "N/A";
        default:
            throw(phys::Exception(phys::StringCat("Cannot convert TestResult with value ", phys::ToString(Convertable))));
    }
}

// Used for padding spaces, after a piece of leader text, such that it always ends at teh expected colum
phys::String MakePadding(phys::String Leader, unsigned int Column)
{
    phys::String Spaces(" ");
    for (unsigned int c=Leader.length(); c<Column;++c)
        { Spaces+=" "; }
    return Spaces;
}

// the return type of tests
typedef std::pair<phys::String,TestResult> TestData;

// The classes for Tests themselves
// inherits from std::map to make storage location of of the TestData obvious
typedef std::map<phys::String,TestResult> TestDataStorage;

class UnitTestGroup : public TestDataStorage
{
    protected:

        // Some basic variable for tracking simple statistics
        unsigned int LongestNameLength;

    public:
        UnitTestGroup() :
            LongestNameLength(0)
        {}
/*
        virtual TestResult RunAutomaticTest() = 0;
        virtual TestResult RunManualTest() = 0;
        virtual TestResult SkipAutomaticTest() = 0;
        virtual TestResult SkipManualTest() = 0;

        virtual TestDataStorage ExecutableTests() = 0;
*/

// Simply naming tests
// make iterating over tests that could be possible
//      use to set skips and run

        // This is expected to run all the tests that meet the criteria passed in
        // This should return the LeastSuccessful TestResult, this will make it easier for the main to find and report errors
        virtual TestResult RunTests(bool RunAutomaticTests, bool RunInteractiveTests)
        {
            TestResult Answer;

            //

            return Answer;
        }

        enum OverWriteResults{
            OverWriteIfLessSuccessful,
            OverWriteIfMoreSuccessful,
            OverWrite,
            DoNotOverWrite
        };

        // It is expected that tests will be inserted using these, this will automate tracking of the most and least successful tests
        void AddTestResult(TestData FreshMeat, OverWriteResults Behavior=OverWriteIfLessSuccessful)
        {
            bool Added=false;

            TestDataStorage::iterator PreExisting = this->find(FreshMeat.first);
            if(this->end()!=PreExisting)
            {
                switch(Behavior)
                {
                    case OverWriteIfLessSuccessful:
                        if (PreExisting->second <= FreshMeat.second)
                            { PreExisting->second = FreshMeat.second; Added=true; }
                        break;
                    case OverWrite:
                        PreExisting->second = FreshMeat.second;
                        Added=true;
                        break;
                    case OverWriteIfMoreSuccessful:
                        if (PreExisting->second >= FreshMeat.second)
                            { PreExisting->second = FreshMeat.second; Added=true; }
                        break;
                    case DoNotOverWrite:
                        break;
                }
            }else{
                this->insert(FreshMeat);
            }

            if (Added)
            {
                if(FreshMeat.first.length()>LongestNameLength)
                    { LongestNameLength=FreshMeat.first.length(); }
            }
        }

        // It is expected that every member of a class in physgame will be tested and its full name, include scoping operators, namespace,
        // class and function name will here (include argnames if required). Functions outside of classes should use their namespace, functionname
        // and arguments if required as the testname.
        // Example TestNames (The Fresh parameter)
        //      "phys::Vector2::Vector2(Real,Real)"     //Test of the Vector2 Constructor that accepts 2 reals
        //      "phys::Vector2::operator+"              //Test of only operator+ on Vector2
        //      "operator<<(ostream,Vector2)"           //Test of streaming operator for vector2 in root namespace
        void AddTestResult(const phys::String Fresh, TestResult Meat, OverWriteResults Behavior=OverWriteIfLessSuccessful)
            { AddTestResult(TestData(Fresh,Meat),Behavior); }

        // make it a little easier to aggregate answers in one place
        const UnitTestGroup& operator+=(const UnitTestGroup& rhs)
        {
            if(rhs.LongestNameLength>LongestNameLength)
                { LongestNameLength=rhs.LongestNameLength; }

            insert(rhs.begin(),rhs.end());
        }

        void DisplayResults(bool Summary = true, bool FullOutput = true)
        {
            vector<unsigned int> TestCounts;
            TestCounts.insert(TestCounts.end(),1+(unsigned int)Unknown, 0);

            phys::String TestName("Test Name");
            cout << endl << " " << TestName << MakePadding(TestName, LongestNameLength) << "Result" << endl;
            for (TestDataStorage::iterator Iter=this->begin(); Iter!=this->end(); Iter++)
            {
                if(FullOutput)
                {
                    cout << Iter->first << MakePadding(Iter->first, LongestNameLength+1) << TestResultToString(Iter->second) << endl;
                }
                TestCounts.at((unsigned int)Iter->second)++;
            }

            if(Summary)
            {
                cout << endl << " Results Summary:" << endl;
                for(unsigned int c=0; c<TestCounts.size();++c)
                {
                    phys::String ResultName(TestResultToString((TestResult)c));
                    cout << "  " << ResultName << MakePadding(ResultName,16) << TestCounts.at(c) << endl;
                }
                cout << endl;
            }
        }

};

// The list of all the testgroups
map<phys::String, UnitTestGroup*> TestGroups;

// Drops a String to all lower case, changes the string passed in
char* AllLower(char* CString)
{
    locale loc;
    for(int c=0; CString[c]!='\0'; ++c)
    {
        CString[c]=tolower(CString[c],loc);
    }
    return CString;
}

// Some simple functions for formatting user input/output
phys::String BoolToString(bool i)
    { return i?"True":"False" ; }

// Picks up on
//      True, Yes as Success
//      False, No as Failed
//      Cancel as Canceled
//      Unsure, Inconclusive as Inconclusive
TestResult GetTestAnswer(phys::String Question)
{
    phys::String Input;
    char Answer;

    while(true)
    {
        cout << Question;
        getline(cin, Input);
        stringstream InputStream(Input);
        if (InputStream >> Answer)
        {
            Answer=tolower(Answer);
            if (Answer=='t' || Answer=='y' || Answer=='f' || Answer=='n' || Answer=='c' || Answer=='u' || Answer=='i')
                { break; }
        }

        cout << endl << "Expected (T)rue/(Y)es for Success, (F)alse/(N)o for Failure," << endl << " (C)anceled to cancel this test, or (U)nsure/(I)nconclusive if you don't know." << endl << endl;
    }

    switch(Answer)
    {
        case 't': case 'y':
            return Success;
        case 'f': case 'n':
            return Failed;
        case 'c':
            return Cancelled;
        case 'u': case 'i':
            return Inconclusive;
        default:
            return Unknown;
    }

}

///Possible ways to exit the UnitTestGroup Program
enum ExitCodes
{
    ExitSuccess             = 0,
    ExitInvalidArguments    = 1
};

int Usage(phys::String ThisName)
{
    std::cout   << endl << "Usage: " << ThisName << " [help] [summary] [testlist] [interactive|automatic] [all] Test Group Names ..." << endl << endl
                << "All:         All test groups will be run." << endl
                << "Interactive: Only interactive tests will be performed on specified test groups." << endl
                << "Automatic:   Only automated tests will be performed on specified test groups." << endl
                //<< "Interactive and Automatic: All tests will be run on specificied test groups." << endl << endl
                << "Summary:     Only display a count of failures and successes" << endl
                << "testlist:    Output a list of all tests, one per line" << endl
                << "Help:        Display this message"<< endl << endl
                << "If only test group names are entered, then all tests in those groups are run." << endl
                << "This command is not case sensitive." << endl << endl
                << "Current Test Groups: " << endl;
    phys::Whole c = 0;
    for(map<phys::String,UnitTestGroup*>::iterator Iter=TestGroups.begin(); Iter!=TestGroups.end(); ++Iter)
    {
        cout << "\t" << Iter->first << " ";
        ++c;        //enforce 4 names per line
        if (4==c)
            { cout<<endl; c=0; }
    }
    cout << endl;

    return ExitInvalidArguments;
}

int PrintList()
{
    for(map<phys::String,UnitTestGroup*>::iterator Iter=TestGroups.begin(); Iter!=TestGroups.end(); ++Iter)
        { cout << Iter->first << endl; }
    return ExitSuccess;
}

/////////////////////////////////////////////////////////////////////////////////////
//This next block of code creates a minimal engine environment for testing managers and other ngine components
using namespace phys;

World *TheWorld;
phys::UI::Caption *TheTextW1;
phys::UI::Caption *TheTextB1;
phys::UI::Caption *TheTextW2;
phys::UI::Caption *TheTextB2;
String TheMessage;
String TheMessage2;
SimpleTimer *ThisTimer;
TestResult AnswerToQuestion = Unknown;

void StartEngine()
{
    PhysicsConstructionInfo Info;
    Info.PhysicsFlags = (PhysicsConstructionInfo::PCF_LimitlessWorld | PhysicsConstructionInfo::PCF_SoftRigidWorld);
    TheWorld = new World(Info,SceneManager::Generic);
    TheWorld->GameInit(false);
    TheWorld->GetResourceManager()->AddResourceLocation(crossplatform::GetDataDirectory(), "FileSystem", "files", false);
    TheWorld->GetGraphicsManager()->GetPrimaryGameWindow()->SetWindowCaption("EventManager Test");
    TheWorld->GetUIManager()->LoadGorilla("dejavu");
    TheWorld->GetResourceManager()->InitResourceGroup("files");
    phys::UI::Screen *TheScreen = TheWorld->GetUIManager()->CreateScreen("Screen","dejavu",TheWorld->GetGraphicsManager()->GetPrimaryGameWindow()->GetViewport(0));
    phys::UI::Layer *TheLayer = TheScreen->CreateLayer("Layer",0);

    TheTextB1 = TheLayer->CreateCaption(ConstString("TheTextB1"),UI::RenderableRect(Vector2(0.0016,0.603),Vector2(1,0.25),true), (Whole)24, TheMessage);
    TheTextB1->SetTextColour(ColourValue::Black());
    TheTextB1->SetBackgroundColour(ColourValue::Transparent());
    TheTextW1 = TheLayer->CreateCaption(ConstString("TheTextW1"),UI::RenderableRect(Vector2(0,0.6),Vector2(1,0.25),true), (Whole)24, TheMessage);
    TheTextW1->SetTextColour(ColourValue::White());
    TheTextW1->SetBackgroundColour(ColourValue::Transparent());

    TheTextB2 = TheLayer->CreateCaption(ConstString("TheTextB2"),UI::RenderableRect(Vector2(0.0016,0.753),Vector2(1.0,0.25),true), (Whole)24, TheMessage2);
    TheTextB2->SetTextColour(ColourValue::Black());
    TheTextB2->SetBackgroundColour(ColourValue::Transparent());
    TheTextW2 = TheLayer->CreateCaption(ConstString("TheTextW2"),UI::RenderableRect(Vector2(0,0.75),Vector2(1.0,0.25),true), (Whole)24, TheMessage2);
    TheTextW2->SetTextColour(ColourValue::White());
    TheTextW2->SetBackgroundColour(ColourValue::Transparent());

}

void UpdateMessage( String Message, String Message2)
{
    TheTextB1->SetText( Message );
    TheTextW1->SetText( Message );
    TheTextB2->SetText( Message2 );
    TheTextW2->SetText( Message2 );
    //TheText->SetText( StringCat(TheMessage," - ", ToString(ThisTimer->GetCurrentTime())  ));
}

// Countdown callbacks
bool PostTimerEnd()
{
    return false;
    TheWorld->GetTimerManager()->SetPostMainLoopItems(0);
}

bool PostTimerUpdate()
    { UpdateMessage(TheMessage,ToString(int(ThisTimer->GetCurrentTime()/1000000))); return true; }

class TimerEnding : public TimerCallback
{
    virtual void DoCallbackItems()
        { TheWorld->GetTimerManager()->SetPostMainLoopItems(&PostTimerEnd); }
};
TimerEnding* CountDownCallback;

void StopEngine()
{
    delete CountDownCallback;
    delete TheWorld;
    crossplatform::WaitMilliseconds(1000); // Ogre spawns some stuff in a seperate thread this is more then enough time for it to finish
}

bool PostInputCheck()
{
    UpdateMessage(TheMessage,TheMessage2);
    AnswerToQuestion=Unknown;
    EventUserInput *ThisInput = TheWorld->GetEventManager()->PopNextUserInputEvent();
    while (ThisInput) //for each userinput event
    {
        for (Whole c=0; c<ThisInput->GetMetaCodeCount(); ++c ) //for each metacode in each userinput
        {
            MetaCode::InputCode ThisCode = ThisInput->GetMetaCode(c).GetCode();
            switch(ThisCode)
            {
                case MetaCode::KEY_T: case MetaCode::KEY_Y: // True or Yes
                    AnswerToQuestion = Success;
                    break;
                case MetaCode::KEY_F: case MetaCode::KEY_N: // False or No
                    AnswerToQuestion = Failed;
                    break;
                case MetaCode::KEY_C:                       // Cancel
                    AnswerToQuestion = Cancelled;
                    break;
                case MetaCode::KEY_U: case MetaCode::KEY_I: // Unknown or Inconclusive
                    AnswerToQuestion = Inconclusive;
                    break;
                default:
                    AnswerToQuestion = Unknown;
                    break;
            }
        }
        delete ThisInput;
        ThisInput = TheWorld->GetEventManager()->PopNextUserInputEvent();
    }


    if (Unknown==AnswerToQuestion)
        { return true; }
    else
        { return false; }
}

// Use this to start a countdown in the interactive test
void StartCountdown(Whole Seconds)
{
    ThisTimer = World::GetWorldPointer()->GetTimerManager()->CreateSimpleTimer(Timer::StopWatch);
    ThisTimer->SetInitialTime(Seconds * 1000000);
    ThisTimer->Reset();
    ThisTimer->SetGoalTime(0);
    ThisTimer->Start();
    CountDownCallback = new TimerEnding;
    ThisTimer->SetCallback(CountDownCallback);
    TheWorld->GetTimerManager()->SetPostMainLoopItems(&PostTimerUpdate);
}

void GetAnswer()
{
    TheMessage2 = "T/Y:Yes  F/N:No  C:Cancel  U/I:Inconclusive";
    UpdateMessage(TheMessage, TheMessage2);
    World::GetWorldPointer()->GetEventManager()->SetPostMainLoopItems(&PostInputCheck);
}

#endif
