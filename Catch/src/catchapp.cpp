#ifndef _catchapp_cpp
#define _catchapp_cpp

#include "catchapp.h"
#include "levelselectcell.h"
#include "throwablegenerator.h"
#include "uicallbacks.h"

#include <cassert>
#include <set>

using namespace std;
using namespace Mezzanine;

CatchApp* CatchApp::TheRealCatchApp = 0;

CatchApp::CatchApp() :
    PreInputWork(NULL),
    PostInputWork(NULL),
    PostUIWork(NULL),
    PostGraphicsWork(NULL),

    TheEntresol(NULL),
    Profiles(NULL),
    Loader(NULL),
    Scorer(NULL),
    Shop(NULL),
    LastObjectThrown(NULL),

    LevelTimer(NULL),
    EndTimer(NULL),

    Paused(false),
    CurrentState(CatchApp::Catch_Init),
    PlaneOfPlay(Plane(Vector3(2.0,1.0,0.0), Vector3(1.0,2.0,0.0), Vector3(1.0,1.0,0.0)))
{
    assert(0==CatchApp::TheRealCatchApp);
    CatchApp::TheRealCatchApp = this;

	this->TheEntresol = new Entresol( "Data/", AT_FileSystem );
    this->Loader = new LevelLoader();
    this->Scorer = new LevelScorer();
    this->Profiles = new ProfileManager("Profiles/");
    this->Shop = new ItemShop();
    ThrowableGenerator::ParseThrowables("");

    this->LevelTimer = new Timer();
}

CatchApp::~CatchApp()
{
    this->TheEntresol->GetScheduler().RemoveWorkUnitMain( this->PreInputWork );
    delete PreInputWork;

    this->TheEntresol->GetScheduler().RemoveWorkUnitMain( this->PostInputWork );
    delete PostInputWork;

    this->TheEntresol->GetScheduler().RemoveWorkUnitMain( this->PostUIWork );
    delete PostUIWork;

    this->TheEntresol->GetScheduler().RemoveWorkUnitMain( this->PostGraphicsWork );
    delete PostGraphicsWork;

    delete Profiles;
    delete Loader;
    delete Scorer;
    delete Shop;
    delete TheEntresol;
    CatchApp::TheRealCatchApp = NULL;
}

void CatchApp::MakeGUI()
{
    UI::UIManager* GUI = UI::UIManager::GetSingletonPtr();
    Graphics::Viewport* UIViewport = Graphics::GraphicsManager::GetSingletonPtr()->GetGameWindow(0)->GetViewport(0);

    const ColourValue Transparent(0.0,0.0,0.0,0.0);
    const ColourValue TransBlack(0.0,0.0,0.0,0.85);
    const ColourValue Black(0.0,0.0,0.0,1.0);
    const ColourValue Gray(0.2,0.2,0.2,1.0);

    GUI->EnableButtonAutoRegister(true);
    GUI->AddAutoRegisterCode( Input::MetaCode( Input::BUTTON_LIFTING, Input::MOUSEBUTTON_1 ) );

    // Make the Main Menu screen and associated layers.
    GUI->LoadMTA("Catch_Menu.mta","Common");
    UI::Screen* MainMenuScreen = GUI->CreateScreen("MainMenuScreen","Catch_Menu",UIViewport);

    const Real RoomyText = 0.65;
    const Real TightText = 0.8;
    const Real ScreenScaleText = 0.04;
    ////////////////////////////////////////////////////////////////
    ////------------------  Main Menu Screen  ------------------////
    ////////////////////////////////////////////////////////////////
    UI::Widget* MMBackground = MainMenuScreen->CreateWidget("MS_Background",UI::UnifiedRect(0,0,1.7777,1));
    MMBackground->SetPositioningRules(UI::PF_Anchor_Center);
    MMBackground->SetVerticalSizingRules(UI::SR_Unified_Dims);
    MMBackground->SetHorizontalSizingRules(UI::SR_Match_Other_Axis_Unified);
    UI::ImageLayer* MMBackgroundLayer = MMBackground->CreateImageLayer("MainMenuBackground");
    MMBackground->AddLayerToGroup(MMBackgroundLayer,0,"Normal");
    MMBackground->AddLayerToGroup(MMBackgroundLayer,0,"Hovered");
    MainMenuScreen->AddChild(MMBackground,0);

    UI::MenuEntry* MMRootEntry = MainMenuScreen->CreateMenuEntry("MS_MenuRoot",UI::UnifiedRect(0.0,0.914,1.0,0.086));
    MMRootEntry->SetPositioningRules(UI::PF_Anchor_HorizontalCenter | UI::PF_Anchor_Bottom);
    MMRootEntry->SetAspectRatioLock(UI::ARL_Ratio_Y_Axis);
    UI::ImageLayer* MMRootEntryLayer = MMRootEntry->CreateImageLayer("MMBrickBackground");
    MMRootEntry->AddLayerToGroup(MMRootEntryLayer,0,"Normal");
    MMRootEntry->AddLayerToGroup(MMRootEntryLayer,0,"Hovered");
    MMRootEntry->SetAutoHide(false);
    MainMenuScreen->AddChild(MMRootEntry,1);
    MMRootEntry->ForceRootEntryVisible();

    ////------------------  Level Select  ------------------////
    // Start with the accessor button
    UI::StackButton* MMLevelSelectAccess = MainMenuScreen->CreateStackButton("MS_LevelSelect",UI::UnifiedRect(0.05,0.18,0.22,0.7));
    UI::ImageLayer* MMLevelSelectAccessNormal = MMLevelSelectAccess->CreateImageLayer("MMButton");
    UI::ImageLayer* MMLevelSelectAccessHovered = MMLevelSelectAccess->CreateImageLayer("MMHoveredButton");
    UI::SingleLineTextLayer* MMLevelSelectAccessText = MMLevelSelectAccess->CreateSingleLineTextLayer("Ubuntu-14");
    MMLevelSelectAccessText->SetText("Level Select");
    MMLevelSelectAccessText->HorizontallyAlign(UI::LA_Center);
    MMLevelSelectAccessText->VerticallyAlign(UI::LA_Center);
    MMLevelSelectAccessText->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,RoomyText);
    MMLevelSelectAccess->AddLayerToGroup(MMLevelSelectAccessNormal,0,"Normal");
    MMLevelSelectAccess->AddLayerToGroup(MMLevelSelectAccessText,1,"Normal");
    MMLevelSelectAccess->AddLayerToGroup(MMLevelSelectAccessHovered,0,"Hovered");
    MMLevelSelectAccess->AddLayerToGroup(MMLevelSelectAccessText,1,"Hovered");
    MMRootEntry->AddChild(MMLevelSelectAccess,1);

    // Create and configure the "window" that houses the level selections
    UI::MenuEntry* MMLevelSelectWin = MainMenuScreen->CreateMenuEntry("MS_LevelSelectWin",UI::UnifiedRect(0.05,-10.3,0.90,9.68));
    UI::ImageLayer* MMLevelSelectWinLayer = MMLevelSelectWin->CreateImageLayer("MMLSBackground");
    MMLevelSelectWin->AddLayerToGroup(MMLevelSelectWinLayer,0,"Normal");
    MMLevelSelectWin->AddLayerToGroup(MMLevelSelectWinLayer,0,"Hovered");
    MMLevelSelectWin->SetPushButton(MMLevelSelectAccess);
    MMRootEntry->AddChild(MMLevelSelectWin,5);

    ////------------------  Options  ------------------////
    // Start with the accessor button
    UI::StackButton* MMOptionsAccess = MainMenuScreen->CreateStackButton("MS_Options",UI::UnifiedRect(0.28,0.18,0.22,0.7));
    UI::ImageLayer* MMOptionsAccessNormal = MMOptionsAccess->CreateImageLayer("MMButton");
    UI::ImageLayer* MMOptionsAccessHovered = MMOptionsAccess->CreateImageLayer("MMHoveredButton");
    UI::SingleLineTextLayer* MMOptionsAccessText = MMOptionsAccess->CreateSingleLineTextLayer("Ubuntu-14");
    MMOptionsAccessText->SetText("Options");
    MMOptionsAccessText->HorizontallyAlign(UI::LA_Center);
    MMOptionsAccessText->VerticallyAlign(UI::LA_Center);
    MMOptionsAccessText->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,RoomyText);
    MMOptionsAccess->AddLayerToGroup(MMOptionsAccessNormal,0,"Normal");
    MMOptionsAccess->AddLayerToGroup(MMOptionsAccessText,1,"Normal");
    MMOptionsAccess->AddLayerToGroup(MMOptionsAccessHovered,0,"Hovered");
    MMOptionsAccess->AddLayerToGroup(MMOptionsAccessText,1,"Hovered");
    MMRootEntry->AddChild(MMOptionsAccess,2);

    // Create and configure the "window" that houses the options display
    UI::MenuEntry* MMOptionsWin = MainMenuScreen->CreateMenuEntry("MS_OptionsWin",UI::UnifiedRect(0.18,-8.08,0.64,6.4));
    UI::ImageLayer* MMOptionsWinLayer = MMOptionsWin->CreateImageLayer("MMOptionsBackground");
    MMOptionsWin->AddLayerToGroup(MMOptionsWinLayer,0,"Normal");
    MMOptionsWin->AddLayerToGroup(MMOptionsWinLayer,0,"Hovered");
    MMOptionsWin->SetPushButton(MMOptionsAccess);
    MMRootEntry->AddChild(MMOptionsWin,6);

    // Create the first of the two buttons that will display the two sets of options (video options)
    UI::StackButton* MMVideoSetAccess = MainMenuScreen->CreateStackButton("MS_VideoSetAccess",UI::UnifiedRect(0.11,0.0365,0.34,0.11));
    UI::ImageLayer* MMVideoSetAccessNormal = MMVideoSetAccess->CreateImageLayer("MMButton");
    UI::ImageLayer* MMVideoSetAccessHovered = MMVideoSetAccess->CreateImageLayer("MMHoveredButton");
    UI::SingleLineTextLayer* MMVideoSetAccessText = MMVideoSetAccess->CreateSingleLineTextLayer("Ubuntu-14");
    MMVideoSetAccessText->SetText("Video Options");
    MMVideoSetAccessText->HorizontallyAlign(UI::LA_Center);
    MMVideoSetAccessText->VerticallyAlign(UI::LA_Center);
    MMVideoSetAccessText->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,RoomyText);
    MMVideoSetAccess->AddLayerToGroup(MMVideoSetAccessNormal,0,"Normal");
    MMVideoSetAccess->AddLayerToGroup(MMVideoSetAccessText,1,"Normal");
    MMVideoSetAccess->AddLayerToGroup(MMVideoSetAccessHovered,0,"Hovered");
    MMVideoSetAccess->AddLayerToGroup(MMVideoSetAccessText,1,"Hovered");
    MMOptionsWin->AddChild(MMVideoSetAccess,1);

    // Create the second of the two buttons that will display the two sets of options (audio options)
    UI::StackButton* MMAudioSetAccess = MainMenuScreen->CreateStackButton("MS_AudioSetAccess",UI::UnifiedRect(0.55,0.0365,0.34,0.11));
    UI::ImageLayer* MMAudioSetAccessNormal = MMAudioSetAccess->CreateImageLayer("MMButton");
    UI::ImageLayer* MMAudioSetAccessHovered = MMAudioSetAccess->CreateImageLayer("MMHoveredButton");
    UI::SingleLineTextLayer* MMAudioSetAccessText = MMAudioSetAccess->CreateSingleLineTextLayer("Ubuntu-14");
    MMAudioSetAccessText->SetText("Audio Options");
    MMAudioSetAccessText->HorizontallyAlign(UI::LA_Center);
    MMAudioSetAccessText->VerticallyAlign(UI::LA_Center);
    MMAudioSetAccessText->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,RoomyText);
    MMAudioSetAccess->AddLayerToGroup(MMAudioSetAccessNormal,0,"Normal");
    MMAudioSetAccess->AddLayerToGroup(MMAudioSetAccessText,1,"Normal");
    MMAudioSetAccess->AddLayerToGroup(MMAudioSetAccessHovered,0,"Hovered");
    MMAudioSetAccess->AddLayerToGroup(MMAudioSetAccessText,1,"Hovered");
    MMOptionsWin->AddChild(MMAudioSetAccess,2);

    // Create the TabSet that will hold all of our options widgets
    UI::TabSet* MMOptionsTabSet = MainMenuScreen->CreateTabSet("MS_OptionsTS",UI::UnifiedRect(0.05,0.16,0.90,0.70));
    MMOptionsTabSet->SetButtonConfig(1,MMVideoSetAccess);
    MMOptionsTabSet->SetButtonConfig(2,MMAudioSetAccess);
    MMOptionsWin->AddChild(MMOptionsTabSet,3);

    // Create the TabbedSubSet that will house all our video options
    UI::TabSet::TabbedSubSet* MMVideoSet = MMOptionsTabSet->CreateTabbedSubSet("MS_VideoSet",1);
    // Go ahead and make the video options the one visible by default
    MMOptionsTabSet->SetVisibleSubSet(MMVideoSet);

    // Create the label for the window resolution list
    UI::Widget* MMResolutionLabel = MainMenuScreen->CreateWidget("MS_ResolutionLabel",UI::UnifiedRect(0.12,0.01,0.415,0.13));
    UI::ImageLayer* MMResolutionLabelLayer = MMResolutionLabel->CreateImageLayer("MMButton");
    UI::SingleLineTextLayer* MMResolutionLabelText = MMResolutionLabel->CreateSingleLineTextLayer("Ubuntu-14");
    MMResolutionLabelText->SetText("Video Resolutions");
    MMResolutionLabelText->HorizontallyAlign(UI::LA_Center);
    MMResolutionLabelText->VerticallyAlign(UI::LA_Center);
    MMResolutionLabelText->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,TightText);
    MMResolutionLabel->AddLayerToGroup(MMResolutionLabelLayer,0,"Normal");
    MMResolutionLabel->AddLayerToGroup(MMResolutionLabelText,1,"Normal");
    MMResolutionLabel->AddLayerToGroup(MMResolutionLabelLayer,0,"Hovered");
    MMResolutionLabel->AddLayerToGroup(MMResolutionLabelText,1,"Hovered");
    MMVideoSet->AddChild(MMResolutionLabel,1);

    // Create the listing of detected supported resolutions
    UI::DropDownList* MMResolutionList = MainMenuScreen->CreateDropDownList("MS_ResolutionList",UI::UnifiedRect(0.065,0.145,0.515,0.12),UI::SB_Separate);
    // Configure the selection display
    UI::Widget* MMResolutionDisplay = MMResolutionList->GetSelectionDisplay();
    UI::ImageLayer* MMResolutionDisplayLayer = MMResolutionDisplay->CreateImageLayer("MMListSelection");
    UI::SingleLineTextLayer* MMResolutionDisplayText = static_cast<UI::SingleLineTextLayer*>( MMResolutionDisplay->GetRenderLayer(0,UI::RLT_SingleLineText) );
    MMResolutionDisplayText->SetDefaultFont("Ubuntu-14");
    MMResolutionDisplayText->HorizontallyAlign(UI::LA_Center);
    MMResolutionDisplayText->VerticallyAlign(UI::LA_Center);
    MMResolutionDisplayText->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,TightText);
    MMResolutionDisplay->AddLayerToGroup(MMResolutionDisplayLayer,0,"Normal");
    MMResolutionDisplay->AddLayerToGroup(MMResolutionDisplayLayer,0,"Hovered");
    // Configure the list toggle
    UI::CheckBox* MMResolutionToggle = MMResolutionList->GetListToggle();
    UI::ImageLayer* MMResolutionToggleNormal = MMResolutionToggle->CreateImageLayer("MMListScrollDown");
    UI::ImageLayer* MMResolutionToggleHovered = MMResolutionToggle->CreateImageLayer("MMHoveredListScrollDown");
    MMResolutionToggle->AddLayerToGroup(MMResolutionToggleNormal,0,"Normal");
    MMResolutionToggle->AddLayerToGroup(MMResolutionToggleHovered,0,"Hovered");
    MMResolutionToggle->AddLayerToGroup(MMResolutionToggleNormal,0,"SelectedNormal");
    MMResolutionToggle->AddLayerToGroup(MMResolutionToggleHovered,0,"SelectedHovered");
    // Configure the option list
    UI::ListBox* MMResolutionOptions = MMResolutionList->GetSelectionList();
    // Configure the background for the listbox container
    UI::VerticalContainer* MMResolutionOptionsList = MMResolutionOptions->GetListContainer();
    UI::ImageLayer* MMResolutionOptionsListLayer = MMResolutionOptionsList->CreateImageLayer("MMListBackground");
    MMResolutionOptionsList->AddLayerToGroup(MMResolutionOptionsListLayer,0,"Normal");
    MMResolutionOptionsList->AddLayerToGroup(MMResolutionOptionsListLayer,0,"Hovered");
    // Configure the scrollbar for the option list
    UI::VerticalScrollbar* MMResolutionOptionsScroll = MMResolutionOptions->GetListScroll();
    MMResolutionOptionsScroll->SetIncrementDistance(0.05);
    // Configure the scroller
    UI::Button* MMResolutionOptionsScroller = MMResolutionOptionsScroll->GetScroller();
    UI::ImageLayer* MMResolutionOptionsScrollerNormal = MMResolutionOptionsScroller->CreateImageLayer("MMListScroller");
    UI::ImageLayer* MMResolutionOptionsScrollerHovered = MMResolutionOptionsScroller->CreateImageLayer("MMHoveredListScroller");
    MMResolutionOptionsScroller->AddLayerToGroup(MMResolutionOptionsScrollerNormal,0,"Normal");
    MMResolutionOptionsScroller->AddLayerToGroup(MMResolutionOptionsScrollerHovered,0,"Hovered");
    // Configure the up button
    UI::Button* MMResolutionOptionsScrollUp = MMResolutionOptionsScroll->GetUpLeftButton();
    UI::ImageLayer* MMResolutionOptionsScrollUpNormal = MMResolutionOptionsScrollUp->CreateImageLayer("MMListScrollUp");
    UI::ImageLayer* MMResolutionOptionsScrollUpHovered = MMResolutionOptionsScrollUp->CreateImageLayer("MMHoveredListScrollUp");
    MMResolutionOptionsScrollUp->AddLayerToGroup(MMResolutionOptionsScrollUpNormal,0,"Normal");
    MMResolutionOptionsScrollUp->AddLayerToGroup(MMResolutionOptionsScrollUpHovered,0,"Hovered");
    // Configure the down button
    UI::Button* MMResolutionOptionsScrollDown = MMResolutionOptionsScroll->GetDownRightButton();
    UI::ImageLayer* MMResolutionOptionsScrollDownNormal = MMResolutionOptionsScrollDown->CreateImageLayer("MMListScrollDown");
    UI::ImageLayer* MMResolutionOptionsScrollDownHovered = MMResolutionOptionsScrollDown->CreateImageLayer("MMHoveredListScrollDown");
    MMResolutionOptionsScrollDown->AddLayerToGroup(MMResolutionOptionsScrollDownNormal,0,"Normal");
    MMResolutionOptionsScrollDown->AddLayerToGroup(MMResolutionOptionsScrollDownHovered,0,"Hovered");
    // Configure the scroll back
    UI::Button* MMResolutionOptionsScrollBack = MMResolutionOptionsScroll->GetScrollBack();
    UI::ImageLayer* MMResolutionOptionsScrollBackLayer = MMResolutionOptionsScrollBack->CreateImageLayer("MMListBackground");
    MMResolutionOptionsScrollBack->AddLayerToGroup(MMResolutionOptionsScrollBackLayer,0,"Normal");
    MMResolutionOptionsScrollBack->AddLayerToGroup(MMResolutionOptionsScrollBackLayer,0,"Hovered");
    // Wrap up listing configuration
    MMVideoSet->AddChild(MMResolutionList,11);

    // Create the checkbox for enabling or disabling fullscreen
    UI::CheckBox* MMFullscreenBox = MainMenuScreen->CreateCheckBox("MS_FullscreenBox",UI::UnifiedRect(0.655,0.145,0,0.12));
    MMFullscreenBox->SetHorizontalSizingRules(UI::SR_Match_Other_Axis);
    UI::ImageLayer* MMFullscreenBoxUnchecked = MMFullscreenBox->CreateImageLayer("MMCheckboxUnchecked");
    UI::ImageLayer* MMFullscreenBoxHoverUnchecked = MMFullscreenBox->CreateImageLayer("MMHoveredCheckboxUnchecked");
    UI::ImageLayer* MMFullscreenBoxChecked = MMFullscreenBox->CreateImageLayer("MMCheckboxChecked");
    UI::ImageLayer* MMFullscreenBoxHoverChecked = MMFullscreenBox->CreateImageLayer("MMHoveredCheckboxChecked");
    MMFullscreenBox->AddLayerToGroup(MMFullscreenBoxUnchecked,0,"Normal");
    MMFullscreenBox->AddLayerToGroup(MMFullscreenBoxHoverUnchecked,0,"Hovered");
    MMFullscreenBox->AddLayerToGroup(MMFullscreenBoxChecked,0,"SelectedNormal");
    MMFullscreenBox->AddLayerToGroup(MMFullscreenBoxHoverChecked,0,"SelectedHovered");
    MMVideoSet->AddChild(MMFullscreenBox,3);

    // Create the label for the fullscreen checkbox
    UI::Widget* MMFullscreenLabel = MainMenuScreen->CreateWidget("MS_FullscreenLabel",UI::UnifiedRect(0.72,0.145,0.22,0.12));
    UI::ImageLayer* MMFullscreenLabelLayer = MMFullscreenLabel->CreateImageLayer("MMButton");
    UI::SingleLineTextLayer* MMFullscreenLabelText = MMFullscreenLabel->CreateSingleLineTextLayer("Ubuntu-14");
    MMFullscreenLabelText->SetText("Fullscreen");
    MMFullscreenLabelText->HorizontallyAlign(UI::LA_Center);
    MMFullscreenLabelText->VerticallyAlign(UI::LA_Center);
    MMFullscreenLabelText->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,TightText);
    MMFullscreenLabel->AddLayerToGroup(MMFullscreenLabelLayer,0,"Normal");
    MMFullscreenLabel->AddLayerToGroup(MMFullscreenLabelText,1,"Normal");
    MMFullscreenLabel->AddLayerToGroup(MMFullscreenLabelLayer,0,"Hovered");
    MMFullscreenLabel->AddLayerToGroup(MMFullscreenLabelText,1,"Hovered");
    MMVideoSet->AddChild(MMFullscreenLabel,4);

    // Create the label for the FSAA options
    UI::Widget* MMFSAALabel = MainMenuScreen->CreateWidget("MS_FSAALabel",UI::UnifiedRect(0.12,0.365,0.415,0.13));
    UI::ImageLayer* MMFSAALabelLayer = MMFSAALabel->CreateImageLayer("MMButton");
    UI::SingleLineTextLayer* MMFSAALabelText = MMFSAALabel->CreateSingleLineTextLayer("Ubuntu-14");
    MMFSAALabelText->SetText("Anti-Aliasing");
    MMFSAALabelText->HorizontallyAlign(UI::LA_Center);
    MMFSAALabelText->VerticallyAlign(UI::LA_Center);
    MMFSAALabelText->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,TightText);
    MMFSAALabel->AddLayerToGroup(MMFSAALabelLayer,0,"Normal");
    MMFSAALabel->AddLayerToGroup(MMFSAALabelText,1,"Normal");
    MMFSAALabel->AddLayerToGroup(MMFSAALabelLayer,0,"Hovered");
    MMFSAALabel->AddLayerToGroup(MMFSAALabelText,1,"Hovered");
    MMVideoSet->AddChild(MMFSAALabel,5);

    // Create the listing of detected anti-aliasing options
    UI::DropDownList* MMFSAAList = MainMenuScreen->CreateDropDownList("MM_FSAAList",UI::UnifiedRect(0.065,0.50,0.515,0.12),UI::SB_Separate);
    // Configure the selection display
    UI::Widget* MMFSAADisplay = MMFSAAList->GetSelectionDisplay();
    UI::ImageLayer* MMFSAADisplayLayer = MMFSAADisplay->CreateImageLayer("MMListSelection");
    UI::SingleLineTextLayer* MMFSAADisplayText = static_cast<UI::SingleLineTextLayer*>( MMFSAADisplay->GetRenderLayer(0,UI::RLT_SingleLineText) );
    MMFSAADisplayText->SetDefaultFont("Ubuntu-14");
    MMFSAADisplayText->HorizontallyAlign(UI::LA_Center);
    MMFSAADisplayText->VerticallyAlign(UI::LA_Center);
    MMFSAADisplayText->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,TightText);
    MMFSAADisplay->AddLayerToGroup(MMFSAADisplayLayer,0,"Normal");
    MMFSAADisplay->AddLayerToGroup(MMFSAADisplayLayer,0,"Hovered");
    // Configure the list toggle
    UI::CheckBox* MMFSAAToggle = MMFSAAList->GetListToggle();
    UI::ImageLayer* MMFSAAToggleNormal = MMFSAAToggle->CreateImageLayer("MMListScrollDown");
    UI::ImageLayer* MMFSAAToggleHovered = MMFSAAToggle->CreateImageLayer("MMHoveredListScrollDown");
    MMFSAAToggle->AddLayerToGroup(MMFSAAToggleNormal,0,"Normal");
    MMFSAAToggle->AddLayerToGroup(MMFSAAToggleHovered,0,"Hovered");
    MMFSAAToggle->AddLayerToGroup(MMFSAAToggleNormal,0,"SelectedNormal");
    MMFSAAToggle->AddLayerToGroup(MMFSAAToggleHovered,0,"SelectedHovered");
    // Configure the option list
    UI::ListBox* MMFSAAOptions = MMFSAAList->GetSelectionList();
    // Configure the background for the listbox container
    UI::VerticalContainer* MMFSAAOptionsList = MMFSAAOptions->GetListContainer();
    UI::ImageLayer* MMFSAAOptionsListLayer = MMFSAAOptionsList->CreateImageLayer("MMListBackground");
    MMFSAAOptionsList->AddLayerToGroup(MMFSAAOptionsListLayer,0,"Normal");
    MMFSAAOptionsList->AddLayerToGroup(MMFSAAOptionsListLayer,0,"Hovered");
    // Configure the scrollbar for the option list
    UI::VerticalScrollbar* MMFSAAOptionsScroll = MMFSAAOptions->GetListScroll();
    MMFSAAOptionsScroll->SetIncrementDistance(0.05);
    // Configure the scroller
    UI::Button* MMFSAAOptionsScroller = MMFSAAOptionsScroll->GetScroller();
    UI::ImageLayer* MMFSAAOptionsScrollerNormal = MMFSAAOptionsScroller->CreateImageLayer("MMListScroller");
    UI::ImageLayer* MMFSAAOptionsScrollerHovered = MMFSAAOptionsScroller->CreateImageLayer("MMHoveredListScroller");
    MMFSAAOptionsScroller->AddLayerToGroup(MMFSAAOptionsScrollerNormal,0,"Normal");
    MMFSAAOptionsScroller->AddLayerToGroup(MMFSAAOptionsScrollerHovered,0,"Hovered");
    // Configure the up button
    UI::Button* MMFSAAOptionsScrollUp = MMFSAAOptionsScroll->GetUpLeftButton();
    UI::ImageLayer* MMFSAAOptionsScrollUpNormal = MMFSAAOptionsScrollUp->CreateImageLayer("MMListScrollUp");
    UI::ImageLayer* MMFSAAOptionsScrollUpHovered = MMFSAAOptionsScrollUp->CreateImageLayer("MMHoveredListScrollUp");
    MMFSAAOptionsScrollUp->AddLayerToGroup(MMFSAAOptionsScrollUpNormal,0,"Normal");
    MMFSAAOptionsScrollUp->AddLayerToGroup(MMFSAAOptionsScrollUpHovered,0,"Hovered");
    // Configure the down button
    UI::Button* MMFSAAOptionsScrollDown = MMFSAAOptionsScroll->GetDownRightButton();
    UI::ImageLayer* MMFSAAOptionsScrollDownNormal = MMFSAAOptionsScrollDown->CreateImageLayer("MMListScrollDown");
    UI::ImageLayer* MMFSAAOptionsScrollDownHovered = MMFSAAOptionsScrollDown->CreateImageLayer("MMHoveredListScrollDown");
    MMFSAAOptionsScrollDown->AddLayerToGroup(MMFSAAOptionsScrollDownNormal,0,"Normal");
    MMFSAAOptionsScrollDown->AddLayerToGroup(MMFSAAOptionsScrollDownHovered,0,"Hovered");
    // Configure the scroll back
    UI::Button* MMFSAAOptionsScrollBack = MMFSAAOptionsScroll->GetScrollBack();
    UI::ImageLayer* MMFSAAOptionsScrollBackLayer = MMFSAAOptionsScrollBack->CreateImageLayer("MMListBackground");
    MMFSAAOptionsScrollBack->AddLayerToGroup(MMFSAAOptionsScrollBackLayer,0,"Normal");
    MMFSAAOptionsScrollBack->AddLayerToGroup(MMFSAAOptionsScrollBackLayer,0,"Hovered");
    // Wrap up listing configuration
    MMVideoSet->AddChild(MMFSAAList,10);

    // Create the checkbox for enabling or disabling FPS stats display
    UI::CheckBox* MMStatsBox = MainMenuScreen->CreateCheckBox("MS_StatsBox",UI::UnifiedRect(0.655,0.50,0,0.12));
    MMStatsBox->SetHorizontalSizingRules(UI::SR_Match_Other_Axis);
    UI::ImageLayer* MMStatsBoxUnchecked = MMStatsBox->CreateImageLayer("MMCheckboxUnchecked");
    UI::ImageLayer* MMStatsBoxHoverUnchecked = MMStatsBox->CreateImageLayer("MMHoveredCheckboxUnchecked");
    UI::ImageLayer* MMStatsBoxChecked = MMStatsBox->CreateImageLayer("MMCheckboxChecked");
    UI::ImageLayer* MMStatsBoxHoverChecked = MMStatsBox->CreateImageLayer("MMHoveredCheckboxChecked");
    MMStatsBox->AddLayerToGroup(MMStatsBoxUnchecked,0,"Normal");
    MMStatsBox->AddLayerToGroup(MMStatsBoxHoverUnchecked,0,"Hovered");
    MMStatsBox->AddLayerToGroup(MMStatsBoxChecked,0,"SelectedNormal");
    MMStatsBox->AddLayerToGroup(MMStatsBoxHoverChecked,0,"SelectedHovered");
    MMVideoSet->AddChild(MMStatsBox,7);

    // Create the label for the FPS stats display checkbox
    UI::Widget* MMStatsLabel = MainMenuScreen->CreateWidget("MS_StatsLabel",UI::UnifiedRect(0.72,0.50,0.22,0.12));
    UI::ImageLayer* MMStatsLabelLayer = MMStatsLabel->CreateImageLayer("MMButton");
    UI::SingleLineTextLayer* MMStatsLabelText = MMStatsLabel->CreateSingleLineTextLayer("Ubuntu-14");
    MMStatsLabelText->SetText("Show FPS");
    MMStatsLabelText->HorizontallyAlign(UI::LA_Center);
    MMStatsLabelText->VerticallyAlign(UI::LA_Center);
    MMStatsLabelText->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,TightText);
    MMStatsLabel->AddLayerToGroup(MMStatsLabelLayer,0,"Normal");
    MMStatsLabel->AddLayerToGroup(MMStatsLabelText,1,"Normal");
    MMStatsLabel->AddLayerToGroup(MMStatsLabelLayer,0,"Hovered");
    MMStatsLabel->AddLayerToGroup(MMStatsLabelText,1,"Hovered");
    MMVideoSet->AddChild(MMStatsLabel,8);

    // Create the button that will apply all of the currently displayed video settings
    UI::Button* MMVideoOptsApply = MainMenuScreen->CreateButton("MS_VideoOptsApply",UI::UnifiedRect(0.815,0.84,0.17,0.14));
    UI::ImageLayer* MMVideoOptsApplyNormal = MMVideoOptsApply->CreateImageLayer("MMOptionsApplyButton");
    UI::ImageLayer* MMVideoOptsApplyHovered = MMVideoOptsApply->CreateImageLayer("MMOptionsApplyHoveredButton");
    UI::SingleLineTextLayer* MMVideoOptsApplyText = MMVideoOptsApply->CreateSingleLineTextLayer("Ubuntu-14");
    MMVideoOptsApplyText->SetText("Apply");
    MMVideoOptsApplyText->HorizontallyAlign(UI::LA_Center);
    MMVideoOptsApplyText->VerticallyAlign(UI::LA_Center);
    MMVideoOptsApplyText->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,TightText);
    MMVideoOptsApply->AddLayerToGroup(MMVideoOptsApplyNormal,0,"Normal");
    MMVideoOptsApply->AddLayerToGroup(MMVideoOptsApplyText,1,"Normal");
    MMVideoOptsApply->AddLayerToGroup(MMVideoOptsApplyHovered,0,"Hovered");
    MMVideoOptsApply->AddLayerToGroup(MMVideoOptsApplyText,1,"Hovered");
    MMVideoSet->AddChild(MMVideoOptsApply,12);

    const Real MMScrollerSize = 0.09;
    // Create the TabbedSubSet that will house all our audio options
    UI::TabSet::TabbedSubSet* MMAudioSet = MMOptionsTabSet->CreateTabbedSubSet("MS_AudioSet",2);

    // Create the label for the Music volume
    UI::Widget* MMMusicVolLabel = MainMenuScreen->CreateWidget("MS_MusicVolLabel",UI::UnifiedRect(0.0,0.01,0.40,0.13));
    MMMusicVolLabel->SetPositioningRules(UI::PF_Anchor_HorizontalCenter);
    UI::ImageLayer* MMMusicVolLabelLayer = MMMusicVolLabel->CreateImageLayer("MMButton");
    UI::SingleLineTextLayer* MMMusicVolLabelText = MMMusicVolLabel->CreateSingleLineTextLayer("Ubuntu-14");
    MMMusicVolLabelText->SetText("Music Volume");
    MMMusicVolLabelText->HorizontallyAlign(UI::LA_Center);
    MMMusicVolLabelText->VerticallyAlign(UI::LA_Center);
    MMMusicVolLabelText->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,TightText);
    MMMusicVolLabel->AddLayerToGroup(MMMusicVolLabelLayer,0,"Normal");
    MMMusicVolLabel->AddLayerToGroup(MMMusicVolLabelText,1,"Normal");
    MMMusicVolLabel->AddLayerToGroup(MMMusicVolLabelLayer,0,"Hovered");
    MMMusicVolLabel->AddLayerToGroup(MMMusicVolLabelText,1,"Hovered");
    MMAudioSet->AddChild(MMMusicVolLabel,1);

    // Create the Music volume slider
    UI::HorizontalScrollbar* MMMusicVol = MainMenuScreen->CreateHorizontalScrollbar("MS_MusicVolume",UI::UnifiedRect(0.0,0.145,0.7,0.09),UI::SB_Separate);
    MMMusicVol->SetPositioningRules(UI::PF_Anchor_HorizontalCenter);
    MMMusicVol->SetIncrementDistance(MMScrollerSize * 0.5);
    MMMusicVol->SetScrollerSize(MMScrollerSize);
    // Configure the scroller
    UI::Button* MMMusicVolScroller = MMMusicVol->GetScroller();
    UI::ImageLayer* MMMusicVolScrollerNormal = MMMusicVolScroller->CreateImageLayer("MMScroller");
    UI::ImageLayer* MMMusicVolScrollerHovered = MMMusicVolScroller->CreateImageLayer("MMHoveredScroller");
    MMMusicVolScroller->AddLayerToGroup(MMMusicVolScrollerNormal,0,"Normal");
    MMMusicVolScroller->AddLayerToGroup(MMMusicVolScrollerHovered,0,"Hovered");
    // Configure the up button
    UI::Button* MMMusicVolUp = MMMusicVol->GetUpLeftButton();
    UI::ImageLayer* MMMusicVolUpNormal = MMMusicVolUp->CreateImageLayer("MMScrollLeft");
    UI::ImageLayer* MMMusicVolUpHovered = MMMusicVolUp->CreateImageLayer("MMHoveredScrollLeft");
    MMMusicVolUp->AddLayerToGroup(MMMusicVolUpNormal,0,"Normal");
    MMMusicVolUp->AddLayerToGroup(MMMusicVolUpHovered,0,"Hovered");
    // Configure the down button
    UI::Button* MMMusicVolDown = MMMusicVol->GetDownRightButton();
    UI::ImageLayer* MMMusicVolDownNormal = MMMusicVolDown->CreateImageLayer("MMScrollRight");
    UI::ImageLayer* MMMusicVolDownHovered = MMMusicVolDown->CreateImageLayer("MMHoveredScrollRight");
    MMMusicVolDown->AddLayerToGroup(MMMusicVolDownNormal,0,"Normal");
    MMMusicVolDown->AddLayerToGroup(MMMusicVolDownHovered,0,"Hovered");
    // Configure the scroll back
    UI::Button* MMMusicVolBack = MMMusicVol->GetScrollBack();
    UI::ImageLayer* MMMusicVolBackLayer = MMMusicVolBack->CreateImageLayer("MMScrollBackground");
    MMMusicVolBack->AddLayerToGroup(MMMusicVolBackLayer,0,"Normal");
    MMMusicVolBack->AddLayerToGroup(MMMusicVolBackLayer,0,"Hovered");
    MMAudioSet->AddChild(MMMusicVol,2);

    // Create the label for the Effects volume
    UI::Widget* MMEffectsVolLabel = MainMenuScreen->CreateWidget("MS_EffectsVolLabel",UI::UnifiedRect(0.0,0.275,0.40,0.13));
    MMEffectsVolLabel->SetPositioningRules(UI::PF_Anchor_HorizontalCenter);
    UI::ImageLayer* MMEffectsVolLabelLayer = MMEffectsVolLabel->CreateImageLayer("MMButton");
    UI::SingleLineTextLayer* MMEffectsVolLabelText = MMEffectsVolLabel->CreateSingleLineTextLayer("Ubuntu-14");
    MMEffectsVolLabelText->SetText("Effects Volume");
    MMEffectsVolLabelText->HorizontallyAlign(UI::LA_Center);
    MMEffectsVolLabelText->VerticallyAlign(UI::LA_Center);
    MMEffectsVolLabelText->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,TightText);
    MMEffectsVolLabel->AddLayerToGroup(MMEffectsVolLabelLayer,0,"Normal");
    MMEffectsVolLabel->AddLayerToGroup(MMEffectsVolLabelText,1,"Normal");
    MMEffectsVolLabel->AddLayerToGroup(MMEffectsVolLabelLayer,0,"Hovered");
    MMEffectsVolLabel->AddLayerToGroup(MMEffectsVolLabelText,1,"Hovered");
    MMAudioSet->AddChild(MMEffectsVolLabel,3);

    // Create the Effects volume slider
    UI::HorizontalScrollbar* MMEffectsVol = MainMenuScreen->CreateHorizontalScrollbar("MS_EffectsVolume",UI::UnifiedRect(0.0,0.41,0.7,0.09),UI::SB_Separate);
    MMEffectsVol->SetPositioningRules(UI::PF_Anchor_HorizontalCenter);
    MMEffectsVol->SetIncrementDistance(MMScrollerSize * 0.5);
    MMEffectsVol->SetScrollerSize(MMScrollerSize);
    // Configure the scroller
    UI::Button* MMEffectsVolScroller = MMEffectsVol->GetScroller();
    UI::ImageLayer* MMEffectsVolScrollerNormal = MMEffectsVolScroller->CreateImageLayer("MMScroller");
    UI::ImageLayer* MMEffectsVolScrollerHovered = MMEffectsVolScroller->CreateImageLayer("MMHoveredScroller");
    MMEffectsVolScroller->AddLayerToGroup(MMEffectsVolScrollerNormal,0,"Normal");
    MMEffectsVolScroller->AddLayerToGroup(MMEffectsVolScrollerHovered,0,"Hovered");
    // Configure the up button
    UI::Button* MMEffectsVolUp = MMEffectsVol->GetUpLeftButton();
    UI::ImageLayer* MMEffectsVolUpNormal = MMEffectsVolUp->CreateImageLayer("MMScrollLeft");
    UI::ImageLayer* MMEffectsVolUpHovered = MMEffectsVolUp->CreateImageLayer("MMHoveredScrollLeft");
    MMEffectsVolUp->AddLayerToGroup(MMEffectsVolUpNormal,0,"Normal");
    MMEffectsVolUp->AddLayerToGroup(MMEffectsVolUpHovered,0,"Hovered");
    // Configure the down button
    UI::Button* MMEffectsVolDown = MMEffectsVol->GetDownRightButton();
    UI::ImageLayer* MMEffectsVolDownNormal = MMEffectsVolDown->CreateImageLayer("MMScrollRight");
    UI::ImageLayer* MMEffectsVolDownHovered = MMEffectsVolDown->CreateImageLayer("MMHoveredScrollRight");
    MMEffectsVolDown->AddLayerToGroup(MMEffectsVolDownNormal,0,"Normal");
    MMEffectsVolDown->AddLayerToGroup(MMEffectsVolDownHovered,0,"Hovered");
    // Configure the scroll back
    UI::Button* MMEffectsVolBack = MMEffectsVol->GetScrollBack();
    UI::ImageLayer* MMEffectsVolBackLayer = MMEffectsVolBack->CreateImageLayer("MMScrollBackground");
    MMEffectsVolBack->AddLayerToGroup(MMEffectsVolBackLayer,0,"Normal");
    MMEffectsVolBack->AddLayerToGroup(MMEffectsVolBackLayer,0,"Hovered");
    MMAudioSet->AddChild(MMEffectsVol,2);

    // Create the label for the Audio Device options
    UI::Widget* MMAudioDeviceLabel = MainMenuScreen->CreateWidget("MS_AudioDeviceLabel",UI::UnifiedRect(0.12,0.56,0.415,0.13));
    UI::ImageLayer* MMAudioDeviceLabelLayer = MMAudioDeviceLabel->CreateImageLayer("MMButton");
    UI::SingleLineTextLayer* MMAudioDeviceLabelText = MMAudioDeviceLabel->CreateSingleLineTextLayer("Ubuntu-14");
    MMAudioDeviceLabelText->SetText("Audio Device");
    MMAudioDeviceLabelText->HorizontallyAlign(UI::LA_Center);
    MMAudioDeviceLabelText->VerticallyAlign(UI::LA_Center);
    MMAudioDeviceLabelText->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,TightText);
    MMAudioDeviceLabel->AddLayerToGroup(MMAudioDeviceLabelLayer,0,"Normal");
    MMAudioDeviceLabel->AddLayerToGroup(MMAudioDeviceLabelText,1,"Normal");
    MMAudioDeviceLabel->AddLayerToGroup(MMAudioDeviceLabelLayer,0,"Hovered");
    MMAudioDeviceLabel->AddLayerToGroup(MMAudioDeviceLabelText,1,"Hovered");
    MMAudioSet->AddChild(MMAudioDeviceLabel,3);

    // Create the listing of detected anti-aliasing options
    UI::DropDownList* MMAudioDeviceList = MainMenuScreen->CreateDropDownList("MM_AudioDeviceList",UI::UnifiedRect(0.065,0.695,0.515,0.12),UI::SB_Separate);
    // Configure the selection display
    UI::Widget* MMAudioDeviceDisplay = MMAudioDeviceList->GetSelectionDisplay();
    UI::ImageLayer* MMAudioDeviceDisplayLayer = MMAudioDeviceDisplay->CreateImageLayer("MMListSelection");
    UI::SingleLineTextLayer* MMAudioDeviceDisplayText = static_cast<UI::SingleLineTextLayer*>( MMAudioDeviceDisplay->GetRenderLayer(0,UI::RLT_SingleLineText) );
    MMAudioDeviceDisplayText->SetDefaultFont("Ubuntu-14");
    MMAudioDeviceDisplayText->HorizontallyAlign(UI::LA_Center);
    MMAudioDeviceDisplayText->VerticallyAlign(UI::LA_Center);
    MMAudioDeviceDisplayText->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,TightText);
    MMAudioDeviceDisplay->AddLayerToGroup(MMAudioDeviceDisplayLayer,0,"Normal");
    MMAudioDeviceDisplay->AddLayerToGroup(MMAudioDeviceDisplayLayer,0,"Hovered");
    // Configure the list toggle
    UI::CheckBox* MMAudioDeviceToggle = MMAudioDeviceList->GetListToggle();
    UI::ImageLayer* MMAudioDeviceToggleNormal = MMAudioDeviceToggle->CreateImageLayer("MMListScrollDown");
    UI::ImageLayer* MMAudioDeviceToggleHovered = MMAudioDeviceToggle->CreateImageLayer("MMHoveredListScrollDown");
    MMAudioDeviceToggle->AddLayerToGroup(MMAudioDeviceToggleNormal,0,"Normal");
    MMAudioDeviceToggle->AddLayerToGroup(MMAudioDeviceToggleHovered,0,"Hovered");
    MMAudioDeviceToggle->AddLayerToGroup(MMAudioDeviceToggleNormal,0,"SelectedNormal");
    MMAudioDeviceToggle->AddLayerToGroup(MMAudioDeviceToggleHovered,0,"SelectedHovered");
    // Configure the option list
    UI::ListBox* MMAudioDeviceOptions = MMAudioDeviceList->GetSelectionList();
    // Configure the background for the listbox container
    UI::VerticalContainer* MMAudioDeviceOptionsList = MMAudioDeviceOptions->GetListContainer();
    UI::ImageLayer* MMAudioDeviceOptionsListLayer = MMAudioDeviceOptionsList->CreateImageLayer("MMListBackground");
    MMAudioDeviceOptionsList->AddLayerToGroup(MMAudioDeviceOptionsListLayer,0,"Normal");
    MMAudioDeviceOptionsList->AddLayerToGroup(MMAudioDeviceOptionsListLayer,0,"Hovered");
    // Configure the scrollbar for the option list
    UI::VerticalScrollbar* MMAudioDeviceOptionsScroll = MMAudioDeviceOptions->GetListScroll();
    MMAudioDeviceOptionsScroll->SetIncrementDistance(0.05);
    // Configure the scroller
    UI::Button* MMAudioDeviceOptionsScroller = MMAudioDeviceOptionsScroll->GetScroller();
    UI::ImageLayer* MMAudioDeviceOptionsScrollerNormal = MMAudioDeviceOptionsScroller->CreateImageLayer("MMListScroller");
    UI::ImageLayer* MMAudioDeviceOptionsScrollerHovered = MMAudioDeviceOptionsScroller->CreateImageLayer("MMHoveredListScroller");
    MMAudioDeviceOptionsScroller->AddLayerToGroup(MMAudioDeviceOptionsScrollerNormal,0,"Normal");
    MMAudioDeviceOptionsScroller->AddLayerToGroup(MMAudioDeviceOptionsScrollerHovered,0,"Hovered");
    // Configure the up button
    UI::Button* MMAudioDeviceOptionsScrollUp = MMAudioDeviceOptionsScroll->GetUpLeftButton();
    UI::ImageLayer* MMAudioDeviceOptionsScrollUpNormal = MMAudioDeviceOptionsScrollUp->CreateImageLayer("MMListScrollUp");
    UI::ImageLayer* MMAudioDeviceOptionsScrollUpHovered = MMAudioDeviceOptionsScrollUp->CreateImageLayer("MMHoveredListScrollUp");
    MMAudioDeviceOptionsScrollUp->AddLayerToGroup(MMAudioDeviceOptionsScrollUpNormal,0,"Normal");
    MMAudioDeviceOptionsScrollUp->AddLayerToGroup(MMAudioDeviceOptionsScrollUpHovered,0,"Hovered");
    // Configure the down button
    UI::Button* MMAudioDeviceOptionsScrollDown = MMAudioDeviceOptionsScroll->GetDownRightButton();
    UI::ImageLayer* MMAudioDeviceOptionsScrollDownNormal = MMAudioDeviceOptionsScrollDown->CreateImageLayer("MMListScrollDown");
    UI::ImageLayer* MMAudioDeviceOptionsScrollDownHovered = MMAudioDeviceOptionsScrollDown->CreateImageLayer("MMHoveredListScrollDown");
    MMAudioDeviceOptionsScrollDown->AddLayerToGroup(MMAudioDeviceOptionsScrollDownNormal,0,"Normal");
    MMAudioDeviceOptionsScrollDown->AddLayerToGroup(MMAudioDeviceOptionsScrollDownHovered,0,"Hovered");
    // Configure the scroll back
    UI::Button* MMAudioDeviceOptionsScrollBack = MMAudioDeviceOptionsScroll->GetScrollBack();
    UI::ImageLayer* MMAudioDeviceOptionsScrollBackLayer = MMAudioDeviceOptionsScrollBack->CreateImageLayer("MMListBackground");
    MMAudioDeviceOptionsScrollBack->AddLayerToGroup(MMAudioDeviceOptionsScrollBackLayer,0,"Normal");
    MMAudioDeviceOptionsScrollBack->AddLayerToGroup(MMAudioDeviceOptionsScrollBackLayer,0,"Hovered");
    // Wrap up listing configuration
    MMAudioSet->AddChild(MMAudioDeviceList,10);

    // Create the checkbox for enabling or disabling FPS stats display
    UI::CheckBox* MMMuteBox = MainMenuScreen->CreateCheckBox("MS_MuteBox",UI::UnifiedRect(0.655,0.695,0,0.12));
    MMMuteBox->SetHorizontalSizingRules(UI::SR_Match_Other_Axis);
    UI::ImageLayer* MMMuteBoxUnchecked = MMMuteBox->CreateImageLayer("MMCheckboxUnchecked");
    UI::ImageLayer* MMMuteBoxHoverUnchecked = MMMuteBox->CreateImageLayer("MMHoveredCheckboxUnchecked");
    UI::ImageLayer* MMMuteBoxChecked = MMMuteBox->CreateImageLayer("MMCheckboxChecked");
    UI::ImageLayer* MMMuteBoxHoverChecked = MMMuteBox->CreateImageLayer("MMHoveredCheckboxChecked");
    MMMuteBox->AddLayerToGroup(MMMuteBoxUnchecked,0,"Normal");
    MMMuteBox->AddLayerToGroup(MMMuteBoxHoverUnchecked,0,"Hovered");
    MMMuteBox->AddLayerToGroup(MMMuteBoxChecked,0,"SelectedNormal");
    MMMuteBox->AddLayerToGroup(MMMuteBoxHoverChecked,0,"SelectedHovered");
    MMAudioSet->AddChild(MMMuteBox,5);

    // Create the label for the FPS stats display checkbox
    UI::Widget* MMMuteLabel = MainMenuScreen->CreateWidget("MS_MuteLabel",UI::UnifiedRect(0.72,0.695,0.22,0.12));
    UI::ImageLayer* MMMuteLabelLayer = MMMuteLabel->CreateImageLayer("MMButton");
    UI::SingleLineTextLayer* MMMuteLabelText = MMMuteLabel->CreateSingleLineTextLayer("Ubuntu-14");
    MMMuteLabelText->SetText("Mute");
    MMMuteLabelText->HorizontallyAlign(UI::LA_Center);
    MMMuteLabelText->VerticallyAlign(UI::LA_Center);
    MMMuteLabelText->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,TightText);
    MMMuteLabel->AddLayerToGroup(MMMuteLabelLayer,0,"Normal");
    MMMuteLabel->AddLayerToGroup(MMMuteLabelText,1,"Normal");
    MMMuteLabel->AddLayerToGroup(MMMuteLabelLayer,0,"Hovered");
    MMMuteLabel->AddLayerToGroup(MMMuteLabelText,1,"Hovered");
    MMAudioSet->AddChild(MMMuteLabel,6);

    // Create the back button for the options window
    UI::StackButton* MMOptsBack = MainMenuScreen->CreateStackButton("MS_OptsBack",UI::UnifiedRect(0.780,0.870,0.156,0.094));
    UI::ImageLayer* MMOptsBackNormal = MMOptsBack->CreateImageLayer("MMOptionsApplyButton");
    UI::ImageLayer* MMOptsBackHovered = MMOptsBack->CreateImageLayer("MMOptionsApplyHoveredButton");
    UI::SingleLineTextLayer* MMOptsBackText = MMOptsBack->CreateSingleLineTextLayer("Ubuntu-14");
    MMOptsBackText->SetText("Back");
    MMOptsBackText->HorizontallyAlign(UI::LA_Center);
    MMOptsBackText->VerticallyAlign(UI::LA_Center);
    MMOptsBackText->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,TightText);
    MMOptsBack->AddLayerToGroup(MMOptsBackNormal,0,"Normal");
    MMOptsBack->AddLayerToGroup(MMOptsBackText,1,"Normal");
    MMOptsBack->AddLayerToGroup(MMOptsBackHovered,0,"Hovered");
    MMOptsBack->AddLayerToGroup(MMOptsBackText,1,"Hovered");
    MMOptionsWin->SetPopButton(MMOptsBack);
    MMOptionsWin->AddChild(MMOptsBack,5);

    ////------------------  Credits  ------------------////
    // Start with the accessor button
    UI::StackButton* MMCreditsAccess = MainMenuScreen->CreateStackButton("MS_Credits",UI::UnifiedRect(0.51,0.18,0.22,0.7));
    UI::ImageLayer* MMCreditsAccessNormal = MMCreditsAccess->CreateImageLayer("MMButton");
    UI::ImageLayer* MMCreditsAccessHovered = MMCreditsAccess->CreateImageLayer("MMHoveredButton");
    UI::SingleLineTextLayer* MMCreditsAccessText = MMCreditsAccess->CreateSingleLineTextLayer("Ubuntu-14");
    MMCreditsAccessText->SetText("Credits");
    MMCreditsAccessText->HorizontallyAlign(UI::LA_Center);
    MMCreditsAccessText->VerticallyAlign(UI::LA_Center);
    MMCreditsAccessText->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,RoomyText);
    MMCreditsAccess->AddLayerToGroup(MMCreditsAccessNormal,0,"Normal");
    MMCreditsAccess->AddLayerToGroup(MMCreditsAccessText,1,"Normal");
    MMCreditsAccess->AddLayerToGroup(MMCreditsAccessHovered,0,"Hovered");
    MMCreditsAccess->AddLayerToGroup(MMCreditsAccessText,1,"Hovered");
    MMRootEntry->AddChild(MMCreditsAccess,3);

    // Create and configure the "window" that houses the options display
    UI::MenuEntry* MMCreditsWin = MainMenuScreen->CreateMenuEntry("MS_CreditsWin",UI::UnifiedRect(0.18,-9.04,0.64,6.4));
    UI::ImageLayer* MMCreditsWinLayer = MMCreditsWin->CreateImageLayer("MMOptionsBackground");
    MMCreditsWin->AddLayerToGroup(MMCreditsWinLayer,0,"Normal");
    MMCreditsWin->AddLayerToGroup(MMCreditsWinLayer,0,"Hovered");
    MMCreditsWin->SetPushButton(MMCreditsAccess);
    MMRootEntry->AddChild(MMCreditsWin,7);

    ////------------------  Exit Game  ------------------////
    // Start with the accessor button
    UI::StackButton* MMAppExitAccess = MainMenuScreen->CreateStackButton("MS_AppExit",UI::UnifiedRect(0.74,0.18,0.22,0.7));
    UI::ImageLayer* MMAppExitAccessNormal = MMAppExitAccess->CreateImageLayer("MMButton");
    UI::ImageLayer* MMAppExitAccessHovered = MMAppExitAccess->CreateImageLayer("MMHoveredButton");
    UI::SingleLineTextLayer* MMAppExitAccessText = MMAppExitAccess->CreateSingleLineTextLayer("Ubuntu-14");
    MMAppExitAccessText->SetText("Exit Game");
    MMAppExitAccessText->HorizontallyAlign(UI::LA_Center);
    MMAppExitAccessText->VerticallyAlign(UI::LA_Center);
    MMAppExitAccessText->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,RoomyText);
    MMAppExitAccess->AddLayerToGroup(MMAppExitAccessNormal,0,"Normal");
    MMAppExitAccess->AddLayerToGroup(MMAppExitAccessText,1,"Normal");
    MMAppExitAccess->AddLayerToGroup(MMAppExitAccessHovered,0,"Hovered");
    MMAppExitAccess->AddLayerToGroup(MMAppExitAccessText,1,"Hovered");
    MMRootEntry->AddChild(MMAppExitAccess,4);

    // Create and configure the "window" that houses the exit confirmation
    UI::MenuEntry* MMAppExitWin = MainMenuScreen->CreateMenuEntry("MS_AppExitWin",UI::UnifiedRect(0.25,-8.14,0.5,3.5));
    UI::ImageLayer* MMAppExitWinLayer = MMAppExitWin->CreateImageLayer("MMAppExitBackground");
    MMAppExitWin->AddLayerToGroup(MMAppExitWinLayer,0,"Normal");
    MMAppExitWin->AddLayerToGroup(MMAppExitWinLayer,0,"Hovered");
    MMAppExitWin->SetPushButton(MMAppExitAccess);
    MMRootEntry->AddChild(MMAppExitWin,8);

    // Create and configure the display for the confirmation question
    UI::Widget* MMAppExitWarn = MainMenuScreen->CreateWidget("MS_AppExitWarn",UI::UnifiedRect(0.14,0.18,0.72,0.22));
    UI::ImageLayer* MMAppExitWarnLayer = MMAppExitWarn->CreateImageLayer("MMAppExitText");
    UI::SingleLineTextLayer* MMAppExitWarnText = MMAppExitWarn->CreateSingleLineTextLayer("Ubuntu-14");
    MMAppExitWarnText->SetText("Are you sure you want to exit?");
    MMAppExitWarnText->HorizontallyAlign(UI::LA_Center);
    MMAppExitWarnText->VerticallyAlign(UI::LA_Center);
    MMAppExitWarnText->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,RoomyText);
    MMAppExitWarn->AddLayerToGroup(MMAppExitWarnLayer,0,"Normal");
    MMAppExitWarn->AddLayerToGroup(MMAppExitWarnText,1,"Normal");
    MMAppExitWarn->AddLayerToGroup(MMAppExitWarnLayer,0,"Hovered");
    MMAppExitWarn->AddLayerToGroup(MMAppExitWarnText,1,"Hovered");
    MMAppExitWin->AddChild(MMAppExitWarn,1);

    // Create and configure the confirm button
    UI::Button* MMAppExitConf = MainMenuScreen->CreateButton("MS_AppExitConf",UI::UnifiedRect(0.10,0.58,0.35,0.22));
    UI::ImageLayer* MMAppExitConfNormal = MMAppExitConf->CreateImageLayer("MMAppExitButton");
    UI::ImageLayer* MMAppExitConfHovered = MMAppExitConf->CreateImageLayer("MMAppExitHoveredButton");
    UI::SingleLineTextLayer* MMAppExitConfText = MMAppExitConf->CreateSingleLineTextLayer("Ubuntu-14");
    MMAppExitConfText->SetText("Yes");
    MMAppExitConfText->HorizontallyAlign(UI::LA_Center);
    MMAppExitConfText->VerticallyAlign(UI::LA_Center);
    MMAppExitConfText->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,RoomyText);
    MMAppExitConf->AddLayerToGroup(MMAppExitConfNormal,0,"Normal");
    MMAppExitConf->AddLayerToGroup(MMAppExitConfText,1,"Normal");
    MMAppExitConf->AddLayerToGroup(MMAppExitConfHovered,0,"Hovered");
    MMAppExitConf->AddLayerToGroup(MMAppExitConfText,1,"Hovered");
    MMAppExitConf->Subscribe(UI::Button::EventDeactivated,&AllAppExit);
    MMAppExitWin->AddChild(MMAppExitConf,2);

    // Create and configure the deny button
    UI::StackButton* MMAppExitDeny = MainMenuScreen->CreateStackButton("MS_AppExitDeny",UI::UnifiedRect(0.55,0.58,0.35,0.22));
    UI::ImageLayer* MMAppExitDenyNormal = MMAppExitDeny->CreateImageLayer("MMAppExitButton");
    UI::ImageLayer* MMAppExitDenyHovered = MMAppExitDeny->CreateImageLayer("MMAppExitHoveredButton");
    UI::SingleLineTextLayer* MMAppExitDenyText = MMAppExitDeny->CreateSingleLineTextLayer("Ubuntu-14");
    MMAppExitDenyText->SetText("No");
    MMAppExitDenyText->HorizontallyAlign(UI::LA_Center);
    MMAppExitDenyText->VerticallyAlign(UI::LA_Center);
    MMAppExitDenyText->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,RoomyText);
    MMAppExitDeny->AddLayerToGroup(MMAppExitDenyNormal,0,"Normal");
    MMAppExitDeny->AddLayerToGroup(MMAppExitDenyText,1,"Normal");
    MMAppExitDeny->AddLayerToGroup(MMAppExitDenyHovered,0,"Hovered");
    MMAppExitDeny->AddLayerToGroup(MMAppExitDenyText,1,"Hovered");
    MMAppExitWin->SetPopButton(MMAppExitDeny);
    MMAppExitWin->AddChild(MMAppExitDeny,3);

    ////////////////////////////////////////////////////////////////
    ////--------------------  Game  Screen  --------------------////
    ////////////////////////////////////////////////////////////////


    /*UI::PagedCellGrid* MMLevelSelectGrid = MMLevelSelectWin->CreatePagedCellGrid("MS_LevelGrid", UI::RenderableRect(Vector2(0.14,0.14), Vector2(0.72,0.66), true), UI::RenderableRect(Vector2(0.60,0.85), Vector2(0.24,0.06), true), UI::Spn_Separate, 0.05);
    MMLevelSelectGrid->GetGridBack()->SetBackgroundColour(Transparent);
    MMLevelSelectGrid->GetPageSpinner()->GetIncrement()->GetClickable()->SetBackgroundSprite("MMIncrementPage");
    MMLevelSelectGrid->GetPageSpinner()->GetDecrement()->GetClickable()->SetBackgroundSprite("MMDecrementPage");
    MMLevelSelectGrid->GetPageSpinner()->GetValueDisplay()->SetBackgroundSprite("MMPageBox");
    UI::Button* MMLevelStart = MMLevelSelectWin->CreateButton("MS_LevelStart", UI::RenderableRect(Vector2(0.42,0.85), Vector2(0.16,0.07), true),MMStartLineHeight,"Start");
    MMLevelStart->AddActivatableListener(new MSStart(MMLevelSelectGrid));
    MMLevelStart->GetClickable()->SetBackgroundSprite("MMLevelStart");
    MMLevelStart->GetClickable()->SetHoveredSprite("MMLevelStartHovered");
    //End of Main Menu Screen

    //Make the Game screen and associated layers.
    GUI->LoadMTA("Catch_Game");
    UI::Screen* GameScreen = GUI->CreateScreen("GameScreen", "Catch_Game", UIViewport);

    Real GSTextLineHeight = 0.04;
    Real GSLargeTextLineHeight = 0.07;

    //Build the Game Screen
    //Build the HUD layer
    UI::OpenRenderableContainerWidget* HUDContainer = GameScreen->CreateOpenRenderableContainerWidget("GS_HUD");
    UI::Caption* Timer = HUDContainer->CreateCaption( "GS_Timer", UI::RenderableRect(Vector2(0.8897, 0.006), Vector2(0.1045, 0.065), true), GSTextLineHeight, "0:00");
    Timer->SetBackgroundSprite("GSTimerArea");
    UI::Rectangle* TimerLogo = HUDContainer->CreateRectangle( UI::RenderableRect(Vector2(0.8355, 0.006), Vector2(0.0542, 0.065), true));
    TimerLogo->SetBackgroundSprite("GSTimerLogo");
    UI::Button* MenuButton = HUDContainer->CreateButton( "GS_Menu", UI::RenderableRect(Vector2(0.008, 0.935), Vector2(0.16, 0.065), true),GSTextLineHeight,"Menu");
    MenuButton->AddActivatableListener(new GSMenu());
    MenuButton->GetClickable()->SetBackgroundSprite("GSMenuStoreButton");
    MenuButton->GetClickable()->SetHoveredSprite("GSMenuStoreHoveredButton");
    UI::Button* StoreButton = HUDContainer->CreateButton( "GS_Store", UI::RenderableRect(Vector2(0.822, 0.935), Vector2(0.16, 0.065), true),GSTextLineHeight,"Store");
    StoreButton->AddActivatableListener(new GSStore());
    StoreButton->GetClickable()->SetBackgroundSprite("GSMenuStoreButton");
    StoreButton->GetClickable()->SetHoveredSprite("GSMenuStoreHoveredButton");
    UI::Caption* ScoreAmount = HUDContainer->CreateCaption( "GS_ScoreArea", UI::RenderableRect(Vector2(0.128, 0.006), Vector2(0.15, 0.06), true), GSTextLineHeight, "0");
    ScoreAmount->SetBackgroundSprite("GSScoreValueArea");
    UI::Caption* ScoreText = HUDContainer->CreateCaption( "GS_ScoreText", UI::RenderableRect(Vector2(0.008, 0.006), Vector2(0.12, 0.06), true), GSTextLineHeight, "Score");
    ScoreText->SetBackgroundSprite("GSScoreTextArea");

    GameScreen->AddRootWidget(0,HUDContainer);
    //End of HUD Layer

    //Build the ItemShop Layer
    UI::Window* ItemShopWin = GameScreen->CreateWidgetWindow("GS_ItemShop", UI::RenderableRect(Vector2(0.25, 0.11), Vector2(0.5, 0.78125), true));
    ItemShopWin->GetWindowBack()->SetBackgroundSprite("GSStoreBackground");
    UI::Caption* ShopTitle = ItemShopWin->CreateCaption("GS_StoreTitle",UI::RenderableRect(Vector2(0.40,0.128),Vector2(0.20,0.06),true),Real(0.06),"Store");
    ShopTitle->SetBackgroundColour(ColourValue::Transparent());
    UI::PagedCellGrid* ItemShopGrid = ItemShopWin->CreatePagedCellGrid("GS_ItemShopGrid", UI::RenderableRect(Vector2(0.31,0.205), Vector2(0.38,0.26), true), UI::RenderableRect(Vector2(0.40,0.47), Vector2(0.20,0.05), true), UI::Spn_Separate, 0.05);
    ItemShopGrid->GetGridBack()->SetBackgroundColour(ColourValue::Transparent());
    ItemShopGrid->GetPageSpinner()->GetIncrement()->GetClickable()->SetBackgroundSprite("GSIncrementPage");
    ItemShopGrid->GetPageSpinner()->GetDecrement()->GetClickable()->SetBackgroundSprite("GSDecrementPage");
    ItemShopGrid->GetPageSpinner()->GetValueDisplay()->SetBackgroundSprite("GSPageBox");
    UI::Rectangle* ItemShopDescriptionBoxBack = ItemShopWin->CreateRectangle(UI::RenderableRect(Vector2(0.31,0.525), Vector2(0.38,0.26), true));
    ItemShopDescriptionBoxBack->SetBackgroundSprite("GSListBackground");
    UI::MarkupText* ItemShopDescriptionBox = ItemShopWin->CreateMarkupText("GS_DescBox",UI::RenderableRect(Vector2(0.315,0.53), Vector2(0.37,0.25), true),Real(0.04),"This is a more exhaustive and thorough test of the markup text's capabilities.");
    ItemShopDescriptionBox->SetBackgroundColour(ColourValue::Transparent());
    UI::Caption* ItemShopMoneyBox = ItemShopWin->CreateCaption("GS_ItemShopMoney",UI::RenderableRect(Vector2(0.31,0.795),Vector2(0.12,0.06),true),Real(0.05),"$");
    ItemShopMoneyBox->SetBackgroundSprite("GSStoreButton");
    UI::Button* ItemShopBuyButton = ItemShopWin->CreateButton("GS_ItemShopBuy",UI::RenderableRect(Vector2(0.44,0.795),Vector2(0.12,0.06),true),Real(0.05),"Buy");
    ItemShopBuyButton->GetClickable()->SetBackgroundSprite("GSStoreButton");
    ItemShopBuyButton->GetClickable()->SetHoveredSprite("GSStoreHoveredButton");
    UI::Button* ItemShopBackButton = ItemShopWin->CreateButton("GS_ItemShopBack",UI::RenderableRect(Vector2(0.57,0.795),Vector2(0.12,0.06),true),Real(0.05),"Back");
    ItemShopBackButton->GetClickable()->SetBackgroundSprite("GSStoreButton");
    ItemShopBackButton->GetClickable()->SetHoveredSprite("GSStoreHoveredButton");
    ItemShopBackButton->AddActivatableListener(new GSISReturn());

    GameScreen->AddRootWidget(4,ItemShopWin);
    ItemShopWin->Hide();
    //End of ItemShop Layer

    //Build the Menu Layer
    UI::Menu* GSGameMenu = GameScreen->CreateMenu( "GS_GameMenu", UI::RenderableRect(Vector2(0.35, 0.27), Vector2(0.3, 0.45), true));
    GSGameMenu->GetRootWindow()->GetWindowBack()->SetBackgroundSprite("GSOptionsMenuBackground");
    UI::Button* GSRestartButton = GSGameMenu->GetRootWindow()->CreateButton( "GS_Restart", UI::RenderableRect(Vector2(0.38, 0.48), Vector2(0.24, 0.05), true), GSTextLineHeight, "Restart Level");
    GSRestartButton->AddActivatableListener(new GSRestart());
    GSRestartButton->GetClickable()->SetBackgroundSprite("GSOptionsButton");
    GSRestartButton->GetClickable()->SetHoveredSprite("GSOptionsHoveredButton");
    UI::Button* GSReturnButton = GSGameMenu->GetRootWindow()->CreateButton( "GS_Return", UI::RenderableRect(Vector2(0.38, 0.56), Vector2(0.24, 0.05), true), GSTextLineHeight, "Return To Game");
    GSReturnButton->AddActivatableListener(new GSReturn());
    GSReturnButton->GetClickable()->SetBackgroundSprite("GSOptionsButton");
    GSReturnButton->GetClickable()->SetHoveredSprite("GSOptionsHoveredButton");
    UI::Button* GSGameExitButton = GSGameMenu->GetRootWindow()->CreateButton( "GS_GameExit", UI::RenderableRect(Vector2(0.38, 0.64), Vector2(0.24, 0.05), true), GSTextLineHeight, "Exit To Menu");
    GSGameExitButton->AddActivatableListener(new GSMMReturn());
    GSGameExitButton->GetClickable()->SetBackgroundSprite("GSOptionsButton");
    GSGameExitButton->GetClickable()->SetHoveredSprite("GSOptionsHoveredButton");

    UI::Button* GSOptionsAccess = GSGameMenu->GetRootWindow()->CreateAccessorButton("GS_OptionsButton", UI::RenderableRect(Vector2(0.38, 0.32), Vector2(0.24, 0.05), true), GSTextLineHeight, "Options");
    GSOptionsAccess->GetClickable()->SetBackgroundSprite("GSOptionsButton");
    GSOptionsAccess->GetClickable()->SetHoveredSprite("GSOptionsHoveredButton");
    UI::MenuWindow* GSOptionsWin = GSGameMenu->GetRootWindow()->CreateChildMenuWindow("GS_OptionsWin", UI::RenderableRect(Vector2(0.18, 0.22), Vector2(0.64, 0.55), true), GSOptionsAccess);
    GSOptionsWin->GetWindowBack()->SetBackgroundSprite("GSOptionsBackground");
    UI::TabSet* GSOptionsTabSet = GSOptionsWin->CreateTabSet("GS_OptionsTS", UI::RenderableRect(Vector2(0.20, 0.31), Vector2(0.60, 0.39), true));
    //video options
    UI::RenderableSetData* GSVideoSet = GSOptionsTabSet->CreateRenderableSet("GS_VideoSet",UI::RenderableRect(Vector2(0.25, 0.24),Vector2(0.22, 0.06),true),MMTextLineHeight,"Video Options");
    GSVideoSet->Collection->GetWidgetBack()->SetBackgroundColour(ColourValue::Transparent());
    GSVideoSet->Accessor->GetClickable()->SetBackgroundSprite("GSButton");
    GSVideoSet->Accessor->GetClickable()->SetHoveredSprite("GSHoveredButton");
    UI::Caption* GSResolutionLabel = GSVideoSet->Collection->CreateCaption("GS_ResolutionLabel",UI::RenderableRect(Vector2(0.28, 0.31),Vector2(0.24, 0.05),true),MMTextLineHeight,"Video Resolution");
    GSResolutionLabel->SetBackgroundSprite("GSButton");
    UI::DropDownList* GSResolutionList = GSVideoSet->Collection->CreateDropDownList("GS_ResolutionList",UI::RenderableRect(Vector2(0.25, 0.36),Vector2(0.30, 0.05),true),MMTextLineHeight,UI::SB_Separate);
    GSResolutionList->GetSelection()->SetBackgroundSprite("GSListSelection");
    GSResolutionList->GetListToggle()->GetClickable()->SetBackgroundSprite("GSListScrollDown");
    GSResolutionList->GetSelectionList()->GetBoxBack()->SetBackgroundSprite("GSListBackground");
    GSResolutionList->GetSelectionList()->GetVertScroll()->GetScrollBack()->SetBackgroundSprite("GSListScrollBackground");
    GSResolutionList->GetSelectionList()->GetVertScroll()->GetScroller()->GetClickable()->SetBackgroundSprite("GSListScroller");
    GSResolutionList->GetSelectionList()->GetVertScroll()->GetScroller()->GetClickable()->SetHoveredSprite("GSHoveredListScroller");
    GSResolutionList->GetSelectionList()->GetVertScroll()->GetDownRightButton()->GetClickable()->SetBackgroundSprite("GSListScrollDown");
    GSResolutionList->GetSelectionList()->GetVertScroll()->GetDownRightButton()->GetClickable()->SetHoveredSprite("GSHoveredListScrollDown");
    GSResolutionList->GetSelectionList()->GetVertScroll()->GetUpLeftButton()->GetClickable()->SetBackgroundSprite("GSListScrollUp");
    GSResolutionList->GetSelectionList()->GetVertScroll()->GetUpLeftButton()->GetClickable()->SetHoveredSprite("GSHoveredListScrollUp");
    GSResolutionList->GetSelectionList()->GetVertScroll()->SetIncrementDistance(0.05);
    GSResolutionList->GetSelectionList()->SetTemplateBackgroundColour(ColourValue(0.0,0.0,0.0,0.0));
    GSResolutionList->GetSelectionList()->SetTemplateTextColour(ColourValue::White());
    GSResolutionList->GetSelectionList()->SetMaxDisplayedSelections(4);
    GSResolutionList->AddWidgetListener(new OptsVideoRes());
    UI::CheckBox* GSFullscreenBox = GSVideoSet->Collection->CreateCheckBox("GS_FullscreenBox",UI::RenderableRect(Vector2(0.59, 0.36),Vector2(0.16, 0.05),true),MMTextLineHeight,"Fullscreen");
    GSFullscreenBox->GetLabel()->SetBackgroundSprite("GSAppExitButton");
    GSFullscreenBox->SetCheckedSprite("GSCheckboxChecked","GSHoveredCheckboxChecked");
    GSFullscreenBox->SetUncheckedSprite("GSCheckboxUnchecked","GSHoveredCheckboxUnchecked");
    GSFullscreenBox->AddWidgetListener(new OptsVideoFullscreen());
    UI::Caption* GSFSAALabel = GSVideoSet->Collection->CreateCaption("GS_FSAALabel",UI::RenderableRect(Vector2(0.28, 0.46),Vector2(0.24, 0.05),true),MMTextLineHeight,"Anti-Aliasing");
    GSFSAALabel->SetBackgroundSprite("GSButton");
    UI::DropDownList* GSFSAAList = GSVideoSet->Collection->CreateDropDownList("GS_FSAAList",UI::RenderableRect(Vector2(0.25, 0.51),Vector2(0.30, 0.05),true),MMTextLineHeight,UI::SB_Separate);
    GSFSAAList->GetSelection()->SetBackgroundSprite("GSListSelection");
    GSFSAAList->GetListToggle()->GetClickable()->SetBackgroundSprite("GSListScrollDown");
    GSFSAAList->GetSelectionList()->GetBoxBack()->SetBackgroundSprite("GSListBackground");
    GSFSAAList->GetSelectionList()->GetVertScroll()->GetScrollBack()->SetBackgroundSprite("GSListScrollBackground");
    GSFSAAList->GetSelectionList()->GetVertScroll()->GetScroller()->GetClickable()->SetBackgroundSprite("GSListScroller");
    GSFSAAList->GetSelectionList()->GetVertScroll()->GetScroller()->GetClickable()->SetHoveredSprite("GSHoveredListScroller");
    GSFSAAList->GetSelectionList()->GetVertScroll()->GetDownRightButton()->GetClickable()->SetBackgroundSprite("GSListScrollDown");
    GSFSAAList->GetSelectionList()->GetVertScroll()->GetDownRightButton()->GetClickable()->SetHoveredSprite("GSHoveredListScrollDown");
    GSFSAAList->GetSelectionList()->GetVertScroll()->GetUpLeftButton()->GetClickable()->SetBackgroundSprite("GSListScrollUp");
    GSFSAAList->GetSelectionList()->GetVertScroll()->GetUpLeftButton()->GetClickable()->SetHoveredSprite("GSHoveredListScrollUp");
    GSFSAAList->GetSelectionList()->GetVertScroll()->SetIncrementDistance(0.05);
    GSFSAAList->GetSelectionList()->SetTemplateBackgroundColour(ColourValue(1.0,1.0,1.0,0.2));
    GSFSAAList->GetSelectionList()->SetTemplateTextColour(ColourValue::White());
    GSFSAAList->GetSelectionList()->SetMaxDisplayedSelections(4);
    GSFSAAList->AddWidgetListener(new OptsVideoFSAA());
    UI::CheckBox* GSStatsBox = GSVideoSet->Collection->CreateCheckBox("GS_StatsBox",UI::RenderableRect(Vector2(0.59, 0.51),Vector2(0.16, 0.05),true),MMTextLineHeight,"Show FPS");
    GSStatsBox->GetLabel()->SetBackgroundSprite("GSAppExitButton");
    GSStatsBox->SetCheckedSprite("GSCheckboxChecked","GSHoveredCheckboxChecked");
    GSStatsBox->SetUncheckedSprite("GSCheckboxUnchecked","GSHoveredCheckboxUnchecked");
    GSStatsBox->AddWidgetListener(new OptsVideoStats());
    UI::Button* GSVideoOptsApply = GSVideoSet->Collection->CreateButton("GS_VideoOptsApply",UI::RenderableRect(Vector2(0.68, 0.64), Vector2(0.10, 0.05), true), MMTextLineHeight, "Apply");
    GSVideoOptsApply->GetClickable()->SetBackgroundSprite("GSOptionsApplyButton");
    GSVideoOptsApply->GetClickable()->SetHoveredSprite("GSOptionsApplyHoveredButton");
    GSVideoOptsApply->AddActivatableListener(new OptsVideoApply(GSResolutionList,GSFullscreenBox,GSStatsBox));
    //sound options
    Real GSScrollerSize = 0.09;
    UI::RenderableSetData* GSAudioSet = GSOptionsTabSet->CreateRenderableSet("GS_AudioSet",UI::RenderableRect(Vector2(0.53, 0.24),Vector2(0.22, 0.06),true),MMTextLineHeight,"Sound Options");
    GSAudioSet->Collection->GetWidgetBack()->SetBackgroundColour(ColourValue::Transparent());
    GSAudioSet->Accessor->GetClickable()->SetBackgroundSprite("GSButton");
    GSAudioSet->Accessor->GetClickable()->SetHoveredSprite("GSHoveredButton");
    UI::Caption* GSMusicVolLabel = GSAudioSet->Collection->CreateCaption("GS_MusicVolLabel",UI::RenderableRect(Vector2(0.38, 0.31),Vector2(0.24, 0.05),true),MMTextLineHeight,"Music Volume");
    GSMusicVolLabel->SetBackgroundSprite("GSButton");
    UI::Scrollbar* GSMusicVol = GSAudioSet->Collection->CreateScrollbar("GS_MusicVolume",UI::RenderableRect(Vector2(0.30, 0.36),Vector2(0.40, 0.04),true),UI::SB_Separate);
    GSMusicVol->SetScrollerSize(GSScrollerSize);
    GSMusicVol->SetIncrementDistance(GSScrollerSize * 0.5);
    GSMusicVol->GetScrollBack()->SetBackgroundSprite("GSScrollBackground");
    GSMusicVol->GetScroller()->GetClickable()->SetBackgroundSprite("GSScroller");
    GSMusicVol->GetScroller()->GetClickable()->SetHoveredSprite("GSHoveredScroller");
    GSMusicVol->GetDownRightButton()->GetClickable()->SetBackgroundSprite("GSScrollRight");
    GSMusicVol->GetDownRightButton()->GetClickable()->SetHoveredSprite("GSHoveredScrollRight");
    GSMusicVol->GetUpLeftButton()->GetClickable()->SetBackgroundSprite("GSScrollLeft");
    GSMusicVol->GetUpLeftButton()->GetClickable()->SetHoveredSprite("GSHoveredScrollLeft");
    GSMusicVol->AddWidgetListener(new OptsMusicVol());
    UI::Caption* GSEffectsVolLabel = GSAudioSet->Collection->CreateCaption("GS_EffectsVolLabel",UI::RenderableRect(Vector2(0.38, 0.42),Vector2(0.24, 0.05),true),MMTextLineHeight,"Effects Volume");
    GSEffectsVolLabel->SetBackgroundSprite("GSButton");
    UI::Scrollbar* GSEffectsVol = GSAudioSet->Collection->CreateScrollbar("GS_EffectsVolume",UI::RenderableRect(Vector2(0.30, 0.47),Vector2(0.40, 0.04),true),UI::SB_Separate);
    GSEffectsVol->SetScrollerSize(GSScrollerSize);
    GSEffectsVol->SetIncrementDistance(GSScrollerSize * 0.5);
    GSEffectsVol->GetScrollBack()->SetBackgroundSprite("GSScrollBackground");
    GSEffectsVol->GetScroller()->GetClickable()->SetBackgroundSprite("GSScroller");
    GSEffectsVol->GetScroller()->GetClickable()->SetHoveredSprite("GSHoveredScroller");
    GSEffectsVol->GetDownRightButton()->GetClickable()->SetBackgroundSprite("GSScrollRight");
    GSEffectsVol->GetDownRightButton()->GetClickable()->SetHoveredSprite("GSHoveredScrollRight");
    GSEffectsVol->GetUpLeftButton()->GetClickable()->SetBackgroundSprite("GSScrollLeft");
    GSEffectsVol->GetUpLeftButton()->GetClickable()->SetHoveredSprite("GSHoveredScrollLeft");
    GSEffectsVol->AddWidgetListener(new OptsEffectVol());
    UI::Caption* GSDeviceLabel = GSAudioSet->Collection->CreateCaption("GS_DeviceLabel",UI::RenderableRect(Vector2(0.30, 0.55),Vector2(0.24, 0.05),true),MMTextLineHeight,"Audio Device");
    GSDeviceLabel->SetBackgroundSprite("GSButton");
    UI::DropDownList* GSDeviceList = GSAudioSet->Collection->CreateDropDownList("GS_AudioDeviceList",UI::RenderableRect(Vector2(0.27, 0.60),Vector2(0.30, 0.05),true),MMTextLineHeight,UI::SB_Separate);
    GSDeviceList->GetSelection()->SetBackgroundSprite("GSListSelection");
    GSDeviceList->GetListToggle()->GetClickable()->SetBackgroundSprite("GSListScrollDown");
    GSDeviceList->GetSelectionList()->GetBoxBack()->SetBackgroundSprite("GSListBackground");
    GSDeviceList->GetSelectionList()->GetVertScroll()->GetScrollBack()->SetBackgroundSprite("GSListScrollBackground");
    GSDeviceList->GetSelectionList()->GetVertScroll()->GetScroller()->GetClickable()->SetBackgroundSprite("GSListScroller");
    GSDeviceList->GetSelectionList()->GetVertScroll()->GetScroller()->GetClickable()->SetHoveredSprite("GSHoveredListScroller");
    GSDeviceList->GetSelectionList()->GetVertScroll()->GetDownRightButton()->GetClickable()->SetBackgroundSprite("GSListScrollDown");
    GSDeviceList->GetSelectionList()->GetVertScroll()->GetDownRightButton()->GetClickable()->SetHoveredSprite("GSHoveredListScrollDown");
    GSDeviceList->GetSelectionList()->GetVertScroll()->GetUpLeftButton()->GetClickable()->SetBackgroundSprite("GSListScrollUp");
    GSDeviceList->GetSelectionList()->GetVertScroll()->GetUpLeftButton()->GetClickable()->SetHoveredSprite("GSHoveredListScrollUp");
    UI::CheckBox* GSMuteBox = GSAudioSet->Collection->CreateCheckBox("GS_MuteBox",UI::RenderableRect(Vector2(0.62, 0.60),Vector2(0.11, 0.05),true),MMTextLineHeight,"Mute");
    GSMuteBox->GetLabel()->SetBackgroundSprite("GSAppExitButton");
    GSMuteBox->SetCheckedSprite("GSCheckboxChecked","GSHoveredCheckboxChecked");
    GSMuteBox->SetUncheckedSprite("GSCheckboxUnchecked","GSHoveredCheckboxUnchecked");
    GSMuteBox->AddWidgetListener(new OptsAudioMute());

    UI::Button* GSOptsBack = GSOptionsWin->CreateBackButton(UI::RenderableRect(Vector2(0.68, 0.70), Vector2(0.10, 0.05), true), MMTextLineHeight, "Back");
    GSOptsBack->GetClickable()->SetBackgroundSprite("GSOptionsApplyButton");
    GSOptsBack->GetClickable()->SetHoveredSprite("GSOptionsApplyHoveredButton");

    GameScreen->AddRootWidget(10,GSGameMenu);
    GSGameMenu->Hide();
    //End of Menu Layer

    //Build the Stats Layer
    UI::OpenRenderableContainerWidget* StatsContainer = GameScreen->CreateOpenRenderableContainerWidget("GS_Stats");

    UI::Caption* CurFPS = StatsContainer->CreateCaption( "CurFPS", UI::RenderableRect(Vector2(0.16, 0.06), Vector2(0.06, 0.065), true), Whole(14), "0.0");
    CurFPS->SetBackgroundColour(Transparent);
    CurFPS->HorizontallyAlign(UI::Txt_Left);

    UI::Caption* CurFPSText = StatsContainer->CreateCaption( "CurFPSText", UI::RenderableRect(Vector2(0.008, 0.06), Vector2(0.15, 0.065), true), Whole(14), "Current FPS: ");
    CurFPSText->SetBackgroundColour(Transparent);
    CurFPSText->HorizontallyAlign(UI::Txt_Left);

    UI::Caption* AvFPS = StatsContainer->CreateCaption( "AvFPS", UI::RenderableRect(Vector2(0.16, 0.105), Vector2(0.06, 0.065), true), Whole(14), "0.0");
    AvFPS->SetBackgroundColour(Transparent);
    AvFPS->HorizontallyAlign(UI::Txt_Left);

    UI::Caption* AvFPSText = StatsContainer->CreateCaption( "AvFPSText", UI::RenderableRect(Vector2(0.008, 0.105), Vector2(0.15, 0.065), true), Whole(14), "Average FPS: ");
    AvFPSText->SetBackgroundColour(Transparent);
    AvFPSText->HorizontallyAlign(UI::Txt_Left);

    GameScreen->AddRootWidget(1,StatsContainer);
    //StatsContainer->Hide();
    //End of Stats Layer

    //Build the Report Layer
    UI::Window* LevelReport = GameScreen->CreateWidgetWindow("GS_LevelReport", UI::RenderableRect(Vector2(0.18, 0.18), Vector2(0.64, 0.64), true));
    LevelReport->GetWindowBack()->SetBackgroundSprite("GSOptionsBackground");

    UI::Caption* ScoreDisplay = LevelReport->CreateCaption("GS_ScoreDisplay",UI::RenderableRect(Vector2(0.39, 0.22), Vector2(0.22, 0.08), true),GSLargeTextLineHeight,"0000");
    ScoreDisplay->SetBackgroundSprite("GSAppExitButton");

    UI::ScrolledCellGrid* ScoreBreakdown = LevelReport->CreateScrolledCellGrid("GS_ScoreBreakdown",UI::RenderableRect(Vector2(0.25, 0.32), Vector2(0.5, 0.36), true), 0.02, UI::SB_Separate);
    ScoreBreakdown->GetGridBack()->SetBackgroundSprite("GSBreakdownBackground");
    ScoreBreakdown->SetFixedCellSize(Vector2(0.46,0.04));
    ScoreBreakdown->SetCellSpacing(Vector2(0.00,0.005),true);
    ScoreBreakdown->SetEdgeSpacing(Vector2(0.02,0.03),true);
    ScoreBreakdown->SetGridOrdering(UI::CellGrid::CG_Vertical_Horizontal_Decending);

    UI::Button* GSFinishButton = LevelReport->CreateButton("GS_Finish", UI::RenderableRect(Vector2(0.33, 0.70), Vector2(0.16, 0.08), true), Whole(14), "Finish");
    GSFinishButton->GetClickable()->SetBackgroundSprite("GSStoreButton");
    GSFinishButton->GetClickable()->SetHoveredSprite("GSStoreHoveredButton");
    GSFinishButton->AddActivatableListener(new GSMMReturn());

    UI::Button* GSRetryButton = LevelReport->CreateButton("GS_Retry", UI::RenderableRect(Vector2(0.51, 0.70), Vector2(0.16, 0.08), true), Whole(14), "Retry");
    GSRetryButton->GetClickable()->SetBackgroundSprite("GSStoreButton");
    GSRetryButton->GetClickable()->SetHoveredSprite("GSStoreHoveredButton");
    GSRetryButton->AddActivatableListener(new GSRestart());

    GameScreen->AddRootWidget(8,LevelReport);
    LevelReport->Hide();
    //End of Report Layer
    //End of Game Screen
    GameScreen->Hide();//*/
}

void CatchApp::CreateLoadingScreen()
{
    UI::UIManager* GUI = UI::UIManager::GetSingletonPtr();
    Graphics::GraphicsManager* GraphicsMan = Graphics::GraphicsManager::GetSingletonPtr();

    GUI->LoadMTA("Catch_Loading.mta","Common");
    Graphics::Viewport* UIViewport = GraphicsMan->GetGameWindow(0)->GetViewport(0);
    UIViewport->SetCamera(this->TheEntresol->GetCameraManager()->CreateCamera("Main"));

    UI::Screen* LoadScreen = GUI->CreateScreen("LoadingScreen","Catch_Loading",UIViewport);
    UI::Widget* BackgroundWidget = LoadScreen->CreateWidget("LoadBackground",UI::UnifiedRect(0,0,1.7777,1,0,0,0,0));
    BackgroundWidget->SetPositioningRules(UI::PF_Anchor_Center);
    BackgroundWidget->SetVerticalSizingRules(UI::SR_Unified_Dims);
    BackgroundWidget->SetHorizontalSizingRules(UI::SR_Match_Other_Axis_Unified);
    UI::ImageLayer* LoadBackground = BackgroundWidget->CreateImageLayer();
    LoadBackground->SetSprite("BTSBanner");
    BackgroundWidget->AddLayerToGroup(LoadBackground,0,"Normal");
    BackgroundWidget->AddLayerToGroup(LoadBackground,0,"Hovered");
    LoadScreen->AddChild(BackgroundWidget,0);

    GraphicsMan->RenderOneFrame();
    LoadBackground->SetSprite("LoadingBackground");
}

void CatchApp::InitMusic()
{
    Audio::AudioManager* AudioMan = Audio::AudioManager::GetSingletonPtr();
    Audio::MusicPlayer* MPlayer = AudioMan->GetMusicPlayer();
    String CommonGroup("Common");
    Audio::iSound* Track1 = AudioMan->CreateMusicSound("Track1.ogg",CommonGroup);
    Audio::iSound* Track2 = AudioMan->CreateMusicSound("Track2.ogg",CommonGroup);
    Audio::iSound* Track3 = AudioMan->CreateMusicSound("Track3.ogg",CommonGroup);
    Audio::iSound* Track4 = AudioMan->CreateMusicSound("Track4.ogg",CommonGroup);
    /*Audio::iSound* Track4 = AudioMan->CreateMusicSound("Track4.ogg",CommonGroup);
    Audio::iSound* Track4 = AudioMan->CreateMusicSound("Track4.ogg",CommonGroup);
    Audio::iSound* Track4 = AudioMan->CreateMusicSound("Track4.ogg",CommonGroup);
    Audio::iSound* Track4 = AudioMan->CreateMusicSound("Track4.ogg",CommonGroup);
    Audio::iSound* Track4 = AudioMan->CreateMusicSound("Track4.ogg",CommonGroup);
    Audio::iSound* Track4 = AudioMan->CreateMusicSound("Track4.ogg",CommonGroup);// */
    MPlayer->GetPlaylist()->AddSound(Track1);
    MPlayer->GetPlaylist()->AddSound(Track2);
    MPlayer->GetPlaylist()->AddSound(Track3);
    MPlayer->GetPlaylist()->AddSound(Track4);
    /*MPlayer->GetPlaylist()->AddSound(Track5);
    MPlayer->GetPlaylist()->AddSound(Track6);
    MPlayer->GetPlaylist()->AddSound(Track7);
    MPlayer->GetPlaylist()->AddSound(Track8);
    MPlayer->GetPlaylist()->AddSound(Track9);
    MPlayer->GetPlaylist()->AddSound(Track10);// */
    MPlayer->SetPlaylistRepeat(true);
    MPlayer->SetPlaylistShuffle(true);
    //MPlayer->SwitchToSong(Track4);
}

void CatchApp::VerifySettings()
{
    // Verify the Audio Settings
    Audio::AudioManager* AudioMan = Audio::AudioManager::GetSingletonPtr();
    // Ensure file exists
    String AudioSaveFileName("AudioSettings.mxs");
    ObjectSettingFile* AudioSettingFile = AudioMan->GetSettingFile(AudioSaveFileName);
    if(!AudioSettingFile)
    {
        AudioSettingFile = AudioMan->CreateSettingFile(AudioSaveFileName);
        // This is where you would set any addition settings as game specific defaults that are the same as engine defaults,
        // but Catch is simple and doesn't need that.  However since nothing is being altered, we have to set the save flag manually.
        AudioSettingFile->SetNeedsSave(true);
        // Make sure the file saves the "Current" setting group.
        AudioMan->SetCurrentSettingsSaveFile(AudioSaveFileName);
    }
    // Verify the Audio Settings
    Graphics::GraphicsManager* GraphicsMan = Graphics::GraphicsManager::GetSingletonPtr();
    // Ensure file exists
    String GraphicsSaveFileName("GraphicsSettings.mxs");
    ObjectSettingFile* GraphicsSettingFile = GraphicsMan->GetSettingFile(GraphicsSaveFileName);
    if(!GraphicsSettingFile)
    {
        GraphicsSettingFile = GraphicsMan->CreateSettingFile(GraphicsSaveFileName);
        // Create the window
        Graphics::GameWindow* MainWin = GraphicsMan->CreateGameWindow("Catch!",800,600,Graphics::GameWindow::WF_FSAA_4);
        //Graphics::Viewport* MainWinView = MainWin->CreateViewport(NULL,0);
        MainWin->CreateViewport(NULL,0);
        // Set the file to save.
        GraphicsSettingFile->SetNeedsSave(true);
        // Make sure the file saves the "Current" setting group.
        GraphicsMan->SetCurrentSettingsSaveFile(GraphicsSaveFileName);
    }
}

void CatchApp::RegisterTypes()
{
    AreaEffectManager* AEMan = this->TheEntresol->GetAreaEffectManager();
    if( AEMan != NULL ) {
        AEMan->AddAreaEffectFactory( new ScoreAreaFactory() );
        AEMan->AddAreaEffectFactory( new StartAreaFactory() );
    }
}

void CatchApp::ChangeState(const CatchApp::GameState StateToSet)
{
    if(this->CurrentState == StateToSet)
        return;
    UI::UIManager* UIMan = UI::UIManager::GetSingletonPtr();
    switch(StateToSet)
    {
        case CatchApp::Catch_GameScreen:
        {
            for( Whole Screen = 0 ; Screen < UIMan->GetNumScreens() ; Screen++ )
            {
                UI::Screen* CurrScreen = UIMan->GetScreen(Screen);
                if("GameScreen" == CurrScreen->GetName())
                    CurrScreen->Show();
                else
                    CurrScreen->Hide();
            }
            break;
        }
        case CatchApp::Catch_Loading:
        {
            for( Whole Screen = 0 ; Screen < UIMan->GetNumScreens() ; Screen++ )
            {
                UI::Screen* CurrScreen = UIMan->GetScreen(Screen);
                if("LoadingScreen" == CurrScreen->GetName())
                    CurrScreen->Show();
                else
                    CurrScreen->Hide();
            }
            break;
        }
        case CatchApp::Catch_MenuScreen:
        {
            for( Whole Screen = 0 ; Screen < UIMan->GetNumScreens() ; Screen++ )
            {
                UI::Screen* CurrScreen = UIMan->GetScreen(Screen);
                if("MainMenuScreen" == CurrScreen->GetName())
                    CurrScreen->Show();
                else
                    CurrScreen->Hide();
            }
            break;
        }
        case CatchApp::Catch_ScoreScreen:
        {
            this->PauseGame(true);
            Whole LevelScore = this->Scorer->PresentFinalScore();
            if(LevelScore > this->Profiles->GetActiveProfile()->GetHighestScore(this->Loader->GetCurrentLevel()))
            {
                this->Profiles->GetActiveProfile()->SetNewHighScore(this->Loader->GetCurrentLevel(),LevelScore);
                /// @todo UI Update
                //(static_cast<LevelSelectCell*>(this->Profiles->GetLevelGrid()->GetCell(Loader->GetCurrentLevel())))->GetEarnedScore()->SetText(StringTools::ConvertToString(LevelScore));
            }
            break;
        }
        case CatchApp::Catch_Init:
            break;
    }
    this->CurrentState = StateToSet;
}

bool CatchApp::CheckEndOfLevel()
{
    if( this->Scorer->GetNumScoreAreas() == 0 )
        return false;
    if(CurrentState == CatchApp::Catch_ScoreScreen)
        return true;
    if(AllStartZonesEmpty())
    {
        if(!EndTimer)
        {
            EndTimer = new StopWatchTimer();
            EndTimer->SetInitialTime(5 * 1000000);
            EndTimer->SetCurrentTime(5 * 1000000);
            EndTimer->SetGoalTime(0);
            EndTimer->Start();
        }

        if( this->Scorer->GetNumAddedThrowables() > 0 )
        {
            EndTimer->Reset();
        }

        return 0 == EndTimer->GetCurrentTime();
    }else{
        if(EndTimer)
        {
            delete EndTimer;
            EndTimer = NULL;
        }
        return false;
    }
}

bool CatchApp::AllStartZonesEmpty()
{
    if(StartAreas.empty())
        return false;
    for( Whole X = 0 ; X < StartAreas.size() ; X++ )
    {
        if(!StartAreas[X]->IsEmpty())
            return false;
    }
    return true;
}

void CatchApp::UnloadLevel()
{
    if("MainMenu"==Loader->GetCurrentLevel())
        return;
    ResourceManager* ResMan = ResourceManager::GetSingletonPtr();
    Physics::CollisionShapeManager* CShapeMan = Physics::CollisionShapeManager::GetSingletonPtr();
    Graphics::MeshManager* MeshMan = Graphics::MeshManager::GetSingletonPtr();
    UI::UIManager* UIMan = UI::UIManager::GetSingletonPtr();
    Physics::PhysicsManager* PhysMan = this->TheEntresol->GetPhysicsManager();
    Graphics::SceneManager* SceneMan = this->TheEntresol->GetSceneManager();
    ActorManager* ActorMan = this->TheEntresol->GetActorManager();
    AreaEffectManager* AreaEffectMan = this->TheEntresol->GetAreaEffectManager();
    DebrisManager* DebrisMan = this->TheEntresol->GetDebrisManager();

    PhysMan->DestroyAllConstraints();
    ActorMan->DestroyAllActors();
    AreaEffectMan->DestroyAllAreaEffects();
    DebrisMan->DestroyAllDebris();
    SceneMan->DestroyAllProxies();
    SceneMan->DisableSky();
    PhysMan->DestroyAllProxies();
    PhysMan->DestroyAllWorldTriggers();
    CShapeMan->DestroyAllShapes();
    MeshMan->DestroyAllGeneratedMeshes();
    StartAreas.clear();
    ThrownItems.clear();

    ResMan->DestroyAssetGroup(Loader->GetCurrentLevel());
    PhysMan->ClearPhysicsMetaData();
    Scorer->ResetLevelData();
    delete EndTimer;
    EndTimer = NULL;

    UI::Screen* GameScreen = UIMan->GetScreen("GameScreen");
    GameScreen->GetWidget("GS_LevelReport")->Hide();
    GameScreen->GetWidget("GS_GameMenu")->Hide();
    GameScreen->GetWidget("GS_ItemShop")->Hide();
}

CatchApp* CatchApp::GetCatchAppPointer()
{
    assert(0!=CatchApp::TheRealCatchApp);
    return TheRealCatchApp;
}

int CatchApp::GetCatchin()
{
    // Verify all the settings are there, and generate defaults if they aren't.
    this->VerifySettings();

    // WorkUnit configuration
    this->PreInputWork = new CatchPreInputWorkUnit(this);
    this->TheEntresol->GetEventManager()->GetEventPumpWork()->AddDependency( this->PreInputWork );
    this->TheEntresol->GetScheduler().AddWorkUnitMain( this->PreInputWork, "PreInputWork" );

    this->PostInputWork = new CatchPostInputWorkUnit(this);
    this->PostInputWork->AddDependency( this->TheEntresol->GetInputManager()->GetDeviceUpdateWork() );
    this->TheEntresol->GetScheduler().AddWorkUnitMain( this->PostInputWork, "PostInputWork" );

    this->PostUIWork = new CatchPostUIWorkUnit(this);
    this->PostUIWork->AddDependency( this->TheEntresol->GetUIManager()->GetWidgetUpdateWork() );
    this->TheEntresol->GetScheduler().AddWorkUnitMain( this->PostUIWork, "PostUIWork" );

    this->PostGraphicsWork = new CatchPostGraphicsWorkUnit(this);
    this->PostGraphicsWork->AddDependency( this->TheEntresol->GetGraphicsManager()->GetRenderWork() );
    this->PostGraphicsWork->AddDependency( this->TheEntresol->GetAreaEffectManager()->GetAreaEffectUpdateWork() );
    this->TheEntresol->GetScheduler().AddWorkUnitMain( this->PostGraphicsWork, "PostGraphicsWork" );

    this->RegisterTypes();

    // Initialize the managers.
	this->TheEntresol->EngineInit(false);

	this->CreateLoadingScreen();
	this->ChangeState(CatchApp::Catch_Loading);

    //Setup the Music
    this->InitMusic();
    //Generate the UI
    this->MakeGUI();

    //Setup the Profile
    if(1 == Profiles->GetNumLoadedProfiles())
        Profiles->SetActiveProfile(Profiles->GetProfile(0));

    Audio::AudioManager::GetSingletonPtr()->GetMusicPlayer()->Play();
    this->Loader->SetNextLevel("MainMenu");
    do{
        this->ChangeState(CatchApp::Catch_Loading);
        this->PauseGame(false);
        Graphics::GraphicsManager::GetSingletonPtr()->RenderOneFrame();
        //Actually Load the game stuff
        this->Loader->LoadLevel();

        if("MainMenu"==Loader->GetCurrentLevel())
            this->ChangeState(CatchApp::Catch_MenuScreen);
        else
            this->ChangeState(CatchApp::Catch_GameScreen);
        this->LevelTimer->Reset();
        this->LevelTimer->Start();

        //if("Ferris"==Loader->GetCurrentLevel())
        //    UIManager::GetSingletonPtr()->GetScreen("GameScreen")->Hide();
        //this->TheEntresol->GetPhysicsManager()->PauseSimulation(true);
        //Start the Main Loop
        this->TheEntresol->MainLoop();
        this->UnloadLevel();
        this->LevelTimer->Stop();
    }while( this->Loader->HasALevelToLoad() );

	return 0;
}

void CatchApp::PauseGame(bool Pause)
{
    UI::Screen* GameScreen = UI::UIManager::GetSingletonPtr()->GetScreen("GameScreen");
    if(Paused == Pause)
        return;
    if(CurrentState == CatchApp::Catch_ScoreScreen && !Pause)
        return;
    if( !Pause && (GameScreen->GetWidget("GS_GameMenu")->IsVisible() || GameScreen->GetWidget("GS_ItemShop")->IsVisible()) )
        return;
    TheEntresol->PauseWorld(Pause);
    if(Pause) LevelTimer->Stop();
    else LevelTimer->Start();
    if(EndTimer)
    {
        if(Pause) EndTimer->Stop();
        else EndTimer->Start();
    }
    Paused = Pause;
}

bool CatchApp::GameIsPaused() const
{
    return Paused;
}

bool CatchApp::IsAThrowable(WorldObject* Throwable) const
{
    for( ThrowableContainer::const_iterator ObjIt = this->ThrownItems.begin() ; ObjIt != this->ThrownItems.end() ; ObjIt++ )
    {
        if( Throwable == (*ObjIt) )
            return true;
    }
    return false;
}

bool CatchApp::IsInsideAnyStartZone(Debris* Throwable) const
{
    if( this->StartAreas.empty() )
        return false;
    for( Whole X = 0 ; X < StartAreas.size() ; X++ )
    {
        if( StartAreas[X]->IsInside(Throwable) )
            return true;
    }
    return false;
}

void CatchApp::RegisterStartArea(StartArea* Start)
    { this->StartAreas.push_back(Start); }

void CatchApp::AddThrowable(Debris* Throwable)
    { this->ThrownItems.push_back(Throwable); }

CatchApp::ThrowableContainer& CatchApp::GetThrowables()
    { return this->ThrownItems; }

LevelLoader* CatchApp::GetLevelLoader() const
    { return this->Loader; }

LevelScorer* CatchApp::GetLevelScorer() const
    { return this->Scorer; }

ProfileManager* CatchApp::GetProfiles() const
    { return this->Profiles; }

ItemShop* CatchApp::GetItemShop() const
    { return this->Shop; }

Timer* CatchApp::GetLevelTimer() const
    { return this->LevelTimer; }

StopWatchTimer* CatchApp::GetEndTimer() const
    { return this->EndTimer; }

#endif
