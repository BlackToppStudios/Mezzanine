#ifndef _buttoncallbacks_cpp
#define _buttoncallbacks_cpp

#include "buttoncallbacks.h"
#include "catchapp.h"

GSStore::GSStore(UI::Button* caller) : UI::ButtonCallback(caller)
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
    UIManager* UIMan = World::GetWorldPointer()->GetUIManager();
    UI::Layer* layer = UIMan->GetLayer("ItemShopLayer");
    layer->SetVisible(!layer->IsVisible());
}

//--------------------------------------------------------------

GSMenu::GSMenu(UI::Button* caller) : UI::ButtonCallback(caller)
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
    UIManager* UIMan = World::GetWorldPointer()->GetUIManager();
    UI::Layer* layer = UIMan->GetLayer("MenuLayer");
    layer->SetVisible(!layer->IsVisible());
}

//--------------------------------------------------------------

GSReturn::GSReturn(UI::Button* caller) : UI::ButtonCallback(caller)
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
    UIManager* UIMan = World::GetWorldPointer()->GetUIManager();
    UI::Layer* layer = UIMan->GetLayer("MenuLayer");
    layer->Hide();
}

//--------------------------------------------------------------

GSMMReturn::GSMMReturn(UI::Button* caller) : UI::ButtonCallback(caller)
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
    World::GetWorldPointer()->BreakMainLoop();
}

//--------------------------------------------------------------

MSStart::MSStart(UI::Button* caller, UI::PagedCellGrid* LevelGrid) : UI::ButtonCallback(caller), TheGrid(LevelGrid)
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
    World::GetWorldPointer()->BreakMainLoop();
}

#endif
