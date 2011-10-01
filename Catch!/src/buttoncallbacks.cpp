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
    UIManager* UIMan = World::GetWorldPointer()->GetUIManager();
    UI::Layer* layer = UIMan->GetLayer("ItemShopLayer");
    layer->SetVisible(!layer->IsVisible());
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
    UIManager* UIMan = World::GetWorldPointer()->GetUIManager();
    UI::Layer* layer = UIMan->GetLayer("MenuLayer");
    layer->SetVisible(!layer->IsVisible());
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
    UIManager* UIMan = World::GetWorldPointer()->GetUIManager();
    UI::Layer* layer = UIMan->GetLayer("MenuLayer");
    layer->Hide();
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
    World::GetWorldPointer()->GetGraphicsManager()->GetPrimaryGameWindow()->setRenderOptions(NewSettings);
    // Apply other settings
    World::GetWorldPointer()->GetUIManager()->GetLayer("StatsLayer")->SetVisible(FPSStatsBox->IsChecked());
}

#endif
