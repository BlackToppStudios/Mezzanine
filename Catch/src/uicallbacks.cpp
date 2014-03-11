#ifndef _uicallbacks_cpp
#define _uicallbacks_cpp

#include "uicallbacks.h"
#include "catchapp.h"

void AllAppExit(EventArgumentsPtr Args)
{
    CatchApp::GetCatchAppPointer()->GetLevelManager()->SetNextLevel(NULL);
    Entresol::GetSingletonPtr()->BreakMainLoop();
}

void GSMMReturn(EventArgumentsPtr Args)
{
    CatchApp::GetCatchAppPointer()->GetLevelManager()->SetNextLevel("MainMenu");
    Entresol::GetSingletonPtr()->BreakMainLoop();
}

void GSRestart(EventArgumentsPtr Args)
{
    LevelManager* LevelMan = CatchApp::GetCatchAppPointer()->GetLevelManager();
    LevelMan->SetNextLevel( LevelMan->GetCurrentLevel() );
    Entresol::GetSingletonPtr()->BreakMainLoop();
}

///////////////////////////////////////////////////////////////////////////////
// MSLevelCellSelect Methods

MSLevelCellSelect::MSLevelCellSelect(UI::Screen* HostScreen) :
    LSScreen(HostScreen)
    {  }

MSLevelCellSelect::~MSLevelCellSelect()
    {  }

void MSLevelCellSelect::operator()(EventArgumentsPtr Args)
{
    if( Args->EventName == UI::PagedContainer::EventChildSelected ) {
        UI::ChildSelectedArgumentsPtr FocusArgs = CountedPtrCast<UI::ChildSelectedArguments>( Args );
        UI::Widget* EventWidget = this->LSScreen->GetWidget( FocusArgs->ChildName );
        if( EventWidget != NULL ) {
            UInt32 OldState = EventWidget->GetState();
            UInt32 NewState = ( FocusArgs->Selected ? OldState | UI::Widget::WS_User_State_1 : OldState & ~UI::Widget::WS_User_State_1 );
            if( OldState != NewState ) {
                EventWidget->ForceState(NewState);
            }
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
// MSLevelStart Methods

MSLevelStart::MSLevelStart(UI::GridContainer* LS) :
    LevelSelect(LS)
    {  }

MSLevelStart::~MSLevelStart()
    {  }

void MSLevelStart::operator()(EventArgumentsPtr Args)
{
    CatchApp::GetCatchAppPointer()->GetLevelManager()->SetNextLevel( this->LevelSelect->GetLastSelectedChild()->GetName() );
    Entresol::GetSingletonPtr()->BreakMainLoop();
    this->LevelSelect->ClearSelectedChild();
}

#endif
