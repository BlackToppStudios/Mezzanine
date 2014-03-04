#ifndef _buttoncallbacks_cpp
#define _buttoncallbacks_cpp

#include "buttoncallbacks.h"
#include "catchapp.h"

/*GSStore::GSStore()
{
}

GSStore::~GSStore()
{
}

void GSStore::DoHoverItems()
{
}

void GSStore::DoActivateItems()
{
    CatchApp* App = CatchApp::GetCatchAppPointer();
    UI::UIManager* UIMan = UI::UIManager::GetSingletonPtr();
    UI::Screen* screen = UIMan->GetScreen("GameScreen");
    UI::Widget* itemshop = screen->GetWidget("GS_ItemShop");

    bool ToSet = !itemshop->IsVisible();
    itemshop->SetVisible(ToSet);
    App->PauseGame(ToSet);
}

void GSStore::DoPreUpdateItems()
{
}

void GSStore::DoPostUpdateItems()
{
}

void GSStore::DoVisibilityChangeItems()
{
}

//--------------------------------------------------------------

GSMenu::GSMenu()
{
}

GSMenu::~GSMenu()
{
}

void GSMenu::DoHoverItems()
{
}

void GSMenu::DoActivateItems()
{
    CatchApp* App = CatchApp::GetCatchAppPointer();
    UI::UIManager* UIMan = UI::UIManager::GetSingletonPtr();
    UI::Screen* screen = UIMan->GetScreen("GameScreen");
    UI::Widget* gamemenu = screen->GetWidget("GS_GameMenu");

    bool ToSet = !gamemenu->IsVisible();
    gamemenu->SetVisible(ToSet);
    App->PauseGame(ToSet);
}

void GSMenu::DoPreUpdateItems()
{
}

void GSMenu::DoPostUpdateItems()
{
}

void GSMenu::DoVisibilityChangeItems()
{
}

//--------------------------------------------------------------

GSReturn::GSReturn()
{
}

GSReturn::~GSReturn()
{
}

void GSReturn::DoHoverItems()
{
}

void GSReturn::DoActivateItems()
{
    CatchApp* App = CatchApp::GetCatchAppPointer();
    UI::UIManager* UIMan = UI::UIManager::GetSingletonPtr();
    UI::Screen* screen = UIMan->GetScreen("GameScreen");
    UI::Widget* gamemenu = screen->GetWidget("GS_GameMenu");
    gamemenu->Hide();
    App->PauseGame(false);
}

void GSReturn::DoPreUpdateItems()
{
}

void GSReturn::DoPostUpdateItems()
{
}

void GSReturn::DoVisibilityChangeItems()
{
}

//--------------------------------------------------------------

GSISReturn::GSISReturn()
{
}

GSISReturn::~GSISReturn()
{
}

void GSISReturn::DoHoverItems()
{
}

void GSISReturn::DoActivateItems()
{
    CatchApp* App = CatchApp::GetCatchAppPointer();
    UI::UIManager* UIMan = UI::UIManager::GetSingletonPtr();
    UI::Screen* screen = UIMan->GetScreen("GameScreen");
    UI::Widget* itemshop = screen->GetWidget("GS_ItemShop");
    itemshop->Hide();
    App->PauseGame(false);
}

void GSISReturn::DoPreUpdateItems()
{
}

void GSISReturn::DoPostUpdateItems()
{
}

void GSISReturn::DoVisibilityChangeItems()
{
}

//--------------------------------------------------------------

GSMMReturn::GSMMReturn()
{
}

GSMMReturn::~GSMMReturn()
{
}

void GSMMReturn::DoHoverItems()
{
}

void GSMMReturn::DoActivateItems()
{
    CatchApp::GetCatchAppPointer()->GetLevelLoader()->SetNextLevel("MainMenu");
    Entresol::GetSingletonPtr()->BreakMainLoop();
}

void GSMMReturn::DoPreUpdateItems()
{
}

void GSMMReturn::DoPostUpdateItems()
{
}

void GSMMReturn::DoVisibilityChangeItems()
{
}

//--------------------------------------------------------------

GSRestart::GSRestart()
{
}

GSRestart::~GSRestart()
{
}

void GSRestart::DoHoverItems()
{
}

void GSRestart::DoActivateItems()
{
    LevelLoader* Loader = CatchApp::GetCatchAppPointer()->GetLevelLoader();
    Loader->SetNextLevel(Loader->GetCurrentLevel());
    Entresol::GetSingletonPtr()->BreakMainLoop();
}

void GSRestart::DoPreUpdateItems()
{
}

void GSRestart::DoPostUpdateItems()
{
}

void GSRestart::DoVisibilityChangeItems()
{
}

//--------------------------------------------------------------

MSStart::MSStart(UI::PagedCellGrid* LevelGrid) : TheGrid(LevelGrid)
{
}

MSStart::~MSStart()
{
}

void MSStart::DoHoverItems()
{
}

void MSStart::DoActivateItems()
{
    UI::Cell* Select = TheGrid->GetSelected();
    if(!Select)
        return;
    CatchApp::GetCatchAppPointer()->GetLevelLoader()->SetNextLevel(Select->GetName());
    TheGrid->ClearSelected();
    Entresol::GetSingletonPtr()->BreakMainLoop();
}

void MSStart::DoPreUpdateItems()
{
}

void MSStart::DoPostUpdateItems()
{
}

void MSStart::DoVisibilityChangeItems()
{
}
//*/

#endif
