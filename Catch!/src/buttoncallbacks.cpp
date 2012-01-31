#ifndef _buttoncallbacks_cpp
#define _buttoncallbacks_cpp

#include "buttoncallbacks.h"
#include "catchapp.h"

GSStore::GSStore()
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
    UIManager* UIMan = UIManager::GetSingletonPtr();
    UI::Layer* layer = UIMan->GetLayer("ItemShopLayer");

    bool LayerVisible = layer->IsVisible();
    layer->SetVisible(!LayerVisible);
    App->PauseGame(!LayerVisible);
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
    UIManager* UIMan = UIManager::GetSingletonPtr();
    UI::Layer* layer = UIMan->GetLayer("MenuLayer");

    bool LayerVisible = layer->IsVisible();
    layer->SetVisible(!LayerVisible);
    App->PauseGame(!LayerVisible);
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
    UIManager* UIMan = UIManager::GetSingletonPtr();
    UI::Layer* layer = UIMan->GetLayer("MenuLayer");
    layer->Hide();
    App->PauseGame(false);
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
    UIManager* UIMan = UIManager::GetSingletonPtr();
    UI::Layer* layer = UIMan->GetLayer("ItemShopLayer");
    layer->Hide();
    App->PauseGame(false);
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
    World::GetWorldPointer()->BreakMainLoop();
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
    World::GetWorldPointer()->BreakMainLoop();
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
    World::GetWorldPointer()->BreakMainLoop();
}

//--------------------------------------------------------------

AllAppExit::AllAppExit()
{
}

AllAppExit::~AllAppExit()
{
}

void AllAppExit::DoHoverItems()
{
}

void AllAppExit::DoActivateItems()
{
    CatchApp::GetCatchAppPointer()->GetLevelLoader()->SetNextLevel("");
    World::GetWorldPointer()->BreakMainLoop();
}

//--------------------------------------------------------------

OptsVideoApply::OptsVideoApply(UI::DropDownList* ResList, UI::CheckBox* FSBox, UI::CheckBox* StatsBox)
    : ResolutionList(ResList),
      FullScreenBox(FSBox),
      FPSStatsBox(StatsBox)
{
}

OptsVideoApply::~OptsVideoApply()
{
}

void OptsVideoApply::DoHoverItems()
{
}

void OptsVideoApply::DoActivateItems()
{
    // Get render resolution data
    Whole Width, Height;
    String SelectedRes = ResolutionList->GetSelection()->GetText();
    String StrWidth = SelectedRes.substr(0,SelectedRes.find_first_of(" "));
    String StrHeight = SelectedRes.substr(SelectedRes.find_last_of(" ") + 1);
    std::stringstream WidthStream;
    std::stringstream HeightStream;
    WidthStream << StrWidth;
    HeightStream << StrHeight;
    WidthStream >> Width;
    HeightStream >> Height;
    // Create the settings
    GraphicsSettings NewSettings;
    NewSettings.Fullscreen = FullScreenBox->IsChecked();
    NewSettings.RenderWidth = Width;
    NewSettings.RenderHeight = Height;
    // Apply the resolution and fullscreen settings
    GraphicsManager::GetSingletonPtr()->GetPrimaryGameWindow()->setRenderOptions(NewSettings);
    // Apply other settings
    UIManager::GetSingletonPtr()->GetLayer("StatsLayer")->SetVisible(FPSStatsBox->IsChecked());
}

#endif
