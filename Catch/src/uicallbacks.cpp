#ifndef _uicallbacks_cpp
#define _uicallbacks_cpp

#include "uicallbacks.h"
#include "catchapp.h"

void AllAppExit(UI::WidgetEventPtr Args)
{
    CatchApp::GetCatchAppPointer()->GetLevelManager()->SetNextLevel(NULL);
    Entresol::GetSingletonPtr()->BreakMainLoop();
}

void GSMMReturn(UI::WidgetEventPtr Args)
{
    CatchApp::GetCatchAppPointer()->GetLevelManager()->SetNextLevel("MainMenu");
    Entresol::GetSingletonPtr()->BreakMainLoop();
}

void GSRestart(UI::WidgetEventPtr Args)
{
    LevelManager* LevelMan = CatchApp::GetCatchAppPointer()->GetLevelManager();
    LevelMan->SetNextLevel( LevelMan->GetCurrentLevel() );
    Entresol::GetSingletonPtr()->BreakMainLoop();
}

///////////////////////////////////////////////////////////////////////////////
// MSProfileCreate Methods

MSProfileCreate::MSProfileCreate(ProfileManager* Manager, UI::Screen* HostScreen) :
    MainScreen(HostScreen),
    ProfileMan(Manager)
    {  }

MSProfileCreate::~MSProfileCreate()
    {  }

void MSProfileCreate::operator()(UI::WidgetEventPtr Args)
{
    UI::EditBox* ProfilesEntry = static_cast<UI::EditBox*>( this->MainScreen->GetWidget("MS_ProfilesEntry") );
    this->ProfileMan->CreateNewProfile( ProfilesEntry->GetText() );
    this->ProfileMan->ApplyProfileDataToProfileList(CatchApp::GetCatchAppPointer()->GetPlayer()->GetProfile());
    ProfilesEntry->SetText("");
}

///////////////////////////////////////////////////////////////////////////////
// MSProfileSelect Methods

MSProfileSelect::MSProfileSelect(ProfileManager* Manager, UI::Screen* HostScreen) :
    MainScreen(HostScreen),
    ProfileMan(Manager)
    {  }

MSProfileSelect::~MSProfileSelect()
    {  }

void MSProfileSelect::operator()(UI::WidgetEventPtr Args)
{
    UI::DropDownList* ProfilesList = static_cast<UI::DropDownList*>( this->MainScreen->GetWidget("MS_ProfilesList") );
    GameProfile* NewSelected = this->ProfileMan->GetProfile( ProfilesList->GetSelectionText() );
    CatchApp::GetCatchAppPointer()->GetPlayer()->SetIdentity( NewSelected );
    this->ProfileMan->ApplyProfileDataToLevelSelect(NewSelected);

    UI::StackButton* ProfilesDisplay = static_cast<UI::StackButton*>( this->MainScreen->GetWidget("MS_ProfilesAccess") );
    UI::SingleLineTextLayer* ProfileDisplayText = static_cast<UI::SingleLineTextLayer*>( ProfilesDisplay->GetRenderLayer(0,UI::RLT_SingleLineText) );
    ProfileDisplayText->SetText( NewSelected->GetName() );
}

///////////////////////////////////////////////////////////////////////////////
// MSLevelCellSelect Methods

MSLevelCellSelect::MSLevelCellSelect(UI::Screen* HostScreen) :
    MainScreen(HostScreen)
    {  }

MSLevelCellSelect::~MSLevelCellSelect()
    {  }

void MSLevelCellSelect::operator()(UI::WidgetEventPtr Args)
{
    if( Args->EventName == UI::PagedContainer::EventChildSelected ) {
        UI::ChildSelectedEventPtr FocusArgs = std::static_pointer_cast<UI::ChildSelectedEvent>( Args );
        UI::Widget* EventWidget = this->MainScreen->GetWidget( FocusArgs->ChildName );
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

void MSLevelStart::operator()(UI::WidgetEventPtr Args)
{
    CatchApp::GetCatchAppPointer()->GetLevelManager()->SetNextLevel( this->LevelSelect->GetLastSelectedChild()->GetName() );
    Entresol::GetSingletonPtr()->BreakMainLoop();
    this->LevelSelect->ClearSelectedChild();
}

#endif
