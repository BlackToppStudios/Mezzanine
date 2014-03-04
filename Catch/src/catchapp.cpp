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
    AudioSettingsWork(NULL),
    VideoSettingsWork(NULL),
    PreInputWork(NULL),
    PostInputWork(NULL),
    PostUIWork(NULL),
    PostGraphicsWork(NULL),

    TheEntresol(NULL),
    Profiles(NULL),
    LevelMan(NULL),
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
    this->LevelMan = new LevelManager( this->TheEntresol, "Levels/" );
    this->Scorer = new LevelScorer();
    this->Profiles = new ProfileManager("Profiles/");
    this->Shop = new ItemShop();
    ThrowableGenerator::ParseThrowables("");

    this->LevelTimer = new Timer();
}

CatchApp::~CatchApp()
{
    this->TheEntresol->GetScheduler().RemoveWorkUnitMain( this->AudioSettingsWork );
    delete this->AudioSettingsWork;

    this->TheEntresol->GetScheduler().RemoveWorkUnitAffinity( this->VideoSettingsWork );
    delete this->VideoSettingsWork;

    this->TheEntresol->GetScheduler().RemoveWorkUnitMain( this->PreInputWork );
    delete this->PreInputWork;

    this->TheEntresol->GetScheduler().RemoveWorkUnitMain( this->PostInputWork );
    delete this->PostInputWork;

    this->TheEntresol->GetScheduler().RemoveWorkUnitMain( this->PostUIWork );
    delete this->PostUIWork;

    this->TheEntresol->GetScheduler().RemoveWorkUnitMain( this->PostGraphicsWork );
    delete this->PostGraphicsWork;

    delete this->Profiles;
    delete this->LevelMan;
    delete this->Scorer;
    delete this->Shop;
    delete this->TheEntresol;
    CatchApp::TheRealCatchApp = NULL;
}

void CatchApp::MakeGUI()
{
    UI::UIManager* GUI = UI::UIManager::GetSingletonPtr();
    Graphics::Viewport* UIViewport = Graphics::GraphicsManager::GetSingletonPtr()->GetGameWindow(0)->GetViewport(0);

    GUI->EnableButtonAutoRegister(true);
    GUI->AddAutoRegisterCode( Input::MetaCode( Input::BUTTON_LIFTING, Input::MOUSEBUTTON_1 ) );

    const ColourValue Transparent(0.0,0.0,0.0,0.0);
    const ColourValue TransBlack(0.0,0.0,0.0,0.85);
    const ColourValue Black(0.0,0.0,0.0,1.0);
    const ColourValue Gray(0.2,0.2,0.2,1.0);

    ////////////////////////////////////////////////////////////////
    ////--------------------  Stats Screen  --------------------////
    ////////////////////////////////////////////////////////////////

    const Real SSNormText = 0.65;
    //const Real SSTightText = 0.8;
    //const Real SSLargeText = 0.85;

    const String StatsScreenText = "Ubuntu-14";

    //GUI->LoadMTA("Catch_Loading.mta","Common");
    UI::Screen* StatsScreen = GUI->CreateScreen("StatsScreen","Catch_Loading",UIViewport,0);

    ////-------------------  FPS  Stats  -------------------////
    // Create the widget container for the current FPS display
    UI::Widget* SSCurrentFPS = StatsScreen->CreateWidget("SS_CurrentFPS",UI::UnifiedRect(0.008,0.006,0.21,0.065));
    SSCurrentFPS->SetAspectRatioLock(UI::ARL_Ratio_Y_Axis);
    // Create the widget for displaying the current FPS explanation text
    UI::Widget* SSCurrentFPSLabel = StatsScreen->CreateWidget("SS_CurrentFPSLabel",UI::UnifiedRect(0.0,0.0,0.55,1.0));
    SSCurrentFPSLabel->SetPositioningRules(UI::PF_Anchor_TopLeft);
    UI::SingleLineTextLayer* SSCurrentFPSLabelLayer = SSCurrentFPSLabel->CreateSingleLineTextLayer(StatsScreenText,0,0);
    SSCurrentFPSLabelLayer->SetText("Current FPS: ");
    SSCurrentFPSLabelLayer->HorizontallyAlign(UI::LA_BottomRight);
    SSCurrentFPSLabelLayer->VerticallyAlign(UI::LA_Center);
    SSCurrentFPSLabelLayer->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,SSNormText);
    SSCurrentFPS->AddChild(SSCurrentFPSLabel,1);
    // Create the widget for displaying the current FPS value
    UI::Widget* SSCurrentFPSValue = StatsScreen->CreateWidget("SS_CurrentFPSValue",UI::UnifiedRect(0.0,0.0,0.45,1.0));
    SSCurrentFPSValue->SetPositioningRules(UI::PF_Anchor_TopRight);
    UI::SingleLineTextLayer* SSCurrentFPSValueLayer = SSCurrentFPSValue->CreateSingleLineTextLayer(StatsScreenText,0,0);
    SSCurrentFPSValueLayer->SetText("0");
    SSCurrentFPSValueLayer->HorizontallyAlign(UI::LA_TopLeft);
    SSCurrentFPSValueLayer->VerticallyAlign(UI::LA_Center);
    SSCurrentFPSValueLayer->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,SSNormText);
    SSCurrentFPS->AddChild(SSCurrentFPSValue,2);
    // Wrap up timer configuration
    StatsScreen->AddChild(SSCurrentFPS,1);

    // Create the widget container for the average FPS display
    UI::Widget* SSAverageFPS = StatsScreen->CreateWidget("SS_AverageFPS",UI::UnifiedRect(0.008,0.105,0.21,0.065));
    SSAverageFPS->SetAspectRatioLock(UI::ARL_Ratio_Y_Axis);
    // Create the widget for displaying the average FPS explanation text
    UI::Widget* SSAverageFPSLabel = StatsScreen->CreateWidget("SS_AverageFPSLabel",UI::UnifiedRect(0.0,0.0,0.55,1.0));
    SSAverageFPSLabel->SetPositioningRules(UI::PF_Anchor_TopLeft);
    UI::SingleLineTextLayer* SSAverageFPSLabelLayer = SSAverageFPSLabel->CreateSingleLineTextLayer(StatsScreenText,0,0);
    SSAverageFPSLabelLayer->SetText("Average FPS: ");
    SSAverageFPSLabelLayer->HorizontallyAlign(UI::LA_BottomRight);
    SSAverageFPSLabelLayer->VerticallyAlign(UI::LA_Center);
    SSAverageFPSLabelLayer->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,SSNormText);
    SSAverageFPS->AddChild(SSAverageFPSLabel,1);
    // Create the widget for displaying the average FPS value
    UI::Widget* SSAverageFPSValue = StatsScreen->CreateWidget("SS_AverageFPSValue",UI::UnifiedRect(0.0,0.0,0.45,1.0));
    SSAverageFPSValue->SetPositioningRules(UI::PF_Anchor_TopRight);
    UI::SingleLineTextLayer* SSAverageFPSValueLayer = SSAverageFPSValue->CreateSingleLineTextLayer(StatsScreenText,0,0);
    SSAverageFPSValueLayer->SetText("0");
    SSAverageFPSValueLayer->HorizontallyAlign(UI::LA_TopLeft);
    SSAverageFPSValueLayer->VerticallyAlign(UI::LA_Center);
    SSAverageFPSValueLayer->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,SSNormText);
    SSAverageFPS->AddChild(SSAverageFPSValue,2);
    // Wrap up timer configuration
    StatsScreen->AddChild(SSAverageFPS,2);

    ////////////////////////////////////////////////////////////////
    ////------------------  Main Menu Screen  ------------------////
    ////////////////////////////////////////////////////////////////

    const Real MMNormText = 0.65;
    const Real MMTightText = 0.8;
    //const Real MMLargeText = 0.85;
    //const Real MMScreenScaleText = 0.04;

    const String MainMenuScreenText = "Ubuntu-14";

    GUI->LoadMTA("Catch_Menu.mta","Common");
    UI::Screen* MainMenuScreen = GUI->CreateScreen("MainMenuScreen","Catch_Menu",UIViewport,1);

    ////-----------------  Main Menu Root  -----------------////
    // Create the widget that will hold the background and lock it at a wide screen size
    UI::Widget* MMBackground = MainMenuScreen->CreateWidget("MS_Background",UI::UnifiedRect(0,0,1.7777,1));
    MMBackground->SetPositioningRules(UI::PF_Anchor_Center);
    MMBackground->SetVerticalSizingRules(UI::SR_Unified_Dims);
    MMBackground->SetHorizontalSizingRules(UI::SR_Match_Other_Axis_Unified);
    MMBackground->CreateImageLayer("MainMenuBackground",0,0);
    MainMenuScreen->AddChild(MMBackground,0);

    // Create the root menu entry
    UI::MenuEntry* MMRootEntry = MainMenuScreen->CreateMenuEntry("MS_MenuRoot",UI::UnifiedRect(0.0,0.914,1.0,0.086));
    MMRootEntry->SetPositioningRules(UI::PF_Anchor_HorizontalCenter | UI::PF_Anchor_Bottom);
    MMRootEntry->SetAspectRatioLock(UI::ARL_Ratio_Y_Axis);
    MMRootEntry->CreateImageLayer("MMBrickBackground",0,0);
    MMRootEntry->SetAutoHide(false);
    MainMenuScreen->AddChild(MMRootEntry,1);
    MMRootEntry->ForceRootEntryVisible();

    ////------------------  Level Select  ------------------////
    // Start with the accessor button
    UI::StackButton* MMLevelSelectAccess = MainMenuScreen->CreateStackButton("MS_LevelSelect",UI::UnifiedRect(0.05,0.18,0.22,0.7));
    MMLevelSelectAccess->CreateImageLayer("MMButton",0,"Normal");
    MMLevelSelectAccess->CreateImageLayer("MMHoveredButton",0,"Hovered");
    UI::SingleLineTextLayer* MMLevelSelectAccessText = MMLevelSelectAccess->CreateSingleLineTextLayer(MainMenuScreenText,1,1);
    MMLevelSelectAccessText->SetText("Level Select");
    MMLevelSelectAccessText->HorizontallyAlign(UI::LA_Center);
    MMLevelSelectAccessText->VerticallyAlign(UI::LA_Center);
    MMLevelSelectAccessText->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,MMNormText);
    MMRootEntry->AddChild(MMLevelSelectAccess,1);

    // Create and configure the "window" that houses the level selections
    UI::MenuEntry* MMLevelSelectWin = MainMenuScreen->CreateMenuEntry("MS_LevelSelectWin",UI::UnifiedRect(0.05,-10.3,0.90,9.68));
    MMLevelSelectWin->CreateImageLayer("MMLSBackground",0,0);
    MMLevelSelectWin->SetPushButton(MMLevelSelectAccess);
    MMRootEntry->AddChild(MMLevelSelectWin,5);

    // Create the container that will display all of our levels
    UI::GridContainer* MMLevelSelectGrid = MainMenuScreen->CreateGridContainer("MS_LevelSelectGrid",UI::UnifiedRect(0.15,0.08,0.70,0.82));
    MMLevelSelectGrid->SetCellSize(2,4);
    MMLevelSelectWin->AddChild(MMLevelSelectGrid,2);

    // Create the spinner which will allow browsing the grid
    UI::Spinner* MMLevelSelectSpinner = MainMenuScreen->CreateSpinner("MS_LevelSelectSpinner",UI::UnifiedRect(0.65,0.87,0.25,0.08),UI::Spn_Separate_Horizontal,MainMenuScreenText);
    MMLevelSelectSpinner->GetIncrement()->CreateImageLayer("MMIncrementPage",0,0);
    MMLevelSelectSpinner->GetDecrement()->CreateImageLayer("MMDecrementPage",0,0);
    MMLevelSelectSpinner->GetValueDisplay()->CreateImageLayer("MMPageBox",0,0);
    MMLevelSelectWin->AddChild(MMLevelSelectSpinner,3);

    // Create the button that will launch the level
    UI::Button* MMLevelStart = MainMenuScreen->CreateButton("MS_LevelStart",UI::UnifiedRect(0.0,0.86,0.16,0.10));
    MMLevelStart->SetPositioningRules(UI::PF_Anchor_HorizontalCenter);
    MMLevelStart->CreateImageLayer("MMLevelStart",0,"Normal");
    MMLevelStart->CreateImageLayer("MMLevelStartHovered",0,"Hovered");
    UI::SingleLineTextLayer* MMLevelStartText = MMLevelStart->CreateSingleLineTextLayer(MainMenuScreenText,1,1);
    MMLevelStartText->SetText("Start");
    MMLevelStartText->HorizontallyAlign(UI::LA_Center);
    MMLevelStartText->VerticallyAlign(UI::LA_Center);
    MMLevelStartText->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,MMNormText);
    MMLevelSelectWin->AddChild(MMLevelStart,4);

    ////------------------  Options  ------------------////
    // Start with the accessor button
    UI::StackButton* MMOptionsAccess = MainMenuScreen->CreateStackButton("MS_Options",UI::UnifiedRect(0.28,0.18,0.22,0.7));
    MMOptionsAccess->CreateImageLayer("MMButton",0,"Normal");
    MMOptionsAccess->CreateImageLayer("MMHoveredButton",0,"Hovered");
    UI::SingleLineTextLayer* MMOptionsAccessText = MMOptionsAccess->CreateSingleLineTextLayer(MainMenuScreenText,1,1);
    MMOptionsAccessText->SetText("Options");
    MMOptionsAccessText->HorizontallyAlign(UI::LA_Center);
    MMOptionsAccessText->VerticallyAlign(UI::LA_Center);
    MMOptionsAccessText->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,MMNormText);
    MMRootEntry->AddChild(MMOptionsAccess,2);

    // Create and configure the "window" that houses the options display
    UI::MenuEntry* MMOptionsWin = MainMenuScreen->CreateMenuEntry("MS_OptionsWin",UI::UnifiedRect(0.18,-8.08,0.64,6.4));
    MMOptionsWin->CreateImageLayer("MMOptionsBackground",0,0);
    MMOptionsWin->SetPushButton(MMOptionsAccess);
    MMRootEntry->AddChild(MMOptionsWin,6);

    // Create the first of the two buttons that will display the two sets of options (video options)
    UI::StackButton* MMVideoSetAccess = MainMenuScreen->CreateStackButton("MS_VideoSetAccess",UI::UnifiedRect(0.11,0.0365,0.34,0.11));
    MMVideoSetAccess->CreateImageLayer("MMButton",0,"Normal");
    MMVideoSetAccess->CreateImageLayer("MMHoveredButton",0,"Hovered");
    UI::SingleLineTextLayer* MMVideoSetAccessText = MMVideoSetAccess->CreateSingleLineTextLayer(MainMenuScreenText,1,1);
    MMVideoSetAccessText->SetText("Video Options");
    MMVideoSetAccessText->HorizontallyAlign(UI::LA_Center);
    MMVideoSetAccessText->VerticallyAlign(UI::LA_Center);
    MMVideoSetAccessText->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,MMNormText);
    MMOptionsWin->AddChild(MMVideoSetAccess,1);

    // Create the second of the two buttons that will display the two sets of options (audio options)
    UI::StackButton* MMAudioSetAccess = MainMenuScreen->CreateStackButton("MS_AudioSetAccess",UI::UnifiedRect(0.55,0.0365,0.34,0.11));
    MMAudioSetAccess->CreateImageLayer("MMButton",0,"Normal");
    MMAudioSetAccess->CreateImageLayer("MMHoveredButton",0,"Hovered");
    UI::SingleLineTextLayer* MMAudioSetAccessText = MMAudioSetAccess->CreateSingleLineTextLayer(MainMenuScreenText,1,1);
    MMAudioSetAccessText->SetText("Audio Options");
    MMAudioSetAccessText->HorizontallyAlign(UI::LA_Center);
    MMAudioSetAccessText->VerticallyAlign(UI::LA_Center);
    MMAudioSetAccessText->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,MMNormText);
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
    MMResolutionLabel->CreateImageLayer("MMButton",0,0);
    UI::SingleLineTextLayer* MMResolutionLabelText = MMResolutionLabel->CreateSingleLineTextLayer(MainMenuScreenText,1,1);
    MMResolutionLabelText->SetText("Video Resolutions");
    MMResolutionLabelText->HorizontallyAlign(UI::LA_Center);
    MMResolutionLabelText->VerticallyAlign(UI::LA_Center);
    MMResolutionLabelText->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,MMTightText);
    MMVideoSet->AddChild(MMResolutionLabel,1);

    // Create the listing of detected supported resolutions
    UI::DropDownList* MMResolutionList = MainMenuScreen->CreateDropDownList("MS_ResolutionList",UI::UnifiedRect(0.065,0.145,0.515,0.12),UI::SB_Separate);
    // Configure the selection display
    UI::Widget* MMResolutionDisplay = MMResolutionList->GetSelectionDisplay();
    MMResolutionDisplay->CreateImageLayer("MMListSelection",0,0);
    UI::SingleLineTextLayer* MMResolutionDisplayText = static_cast<UI::SingleLineTextLayer*>( MMResolutionDisplay->GetRenderLayer(0,UI::RLT_SingleLineText) );
    MMResolutionDisplayText->SetDefaultFont(MainMenuScreenText);
    MMResolutionDisplayText->HorizontallyAlign(UI::LA_Center);
    MMResolutionDisplayText->VerticallyAlign(UI::LA_Center);
    MMResolutionDisplayText->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,MMTightText);
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
    MMResolutionOptions->SetListItemFont(MainMenuScreenText);
    // Configure the background for the listbox container
    UI::VerticalContainer* MMResolutionOptionsList = MMResolutionOptions->GetListContainer();
    MMResolutionOptionsList->CreateImageLayer("MMListBackground",0,0);
    MMResolutionOptionsList->SetChildSizing(UI::SizingInfo(UI::UnifiedVec2(1.0,0.19)),UI::LinearContainer::SE_OnAdd);
    // Configure the scrollbar for the option list
    UI::VerticalScrollbar* MMResolutionOptionsScroll = MMResolutionOptions->GetListScroll();
    MMResolutionOptionsScroll->SetIncrementDistance(0.05);
    // Configure the scroller
    MMResolutionOptionsScroll->GetScroller()->CreateImageLayer("MMListScroller",0,"Normal");
    MMResolutionOptionsScroll->GetScroller()->CreateImageLayer("MMHoveredListScroller",0,"Hovered");
    // Configure the up button
    MMResolutionOptionsScroll->GetUpLeftButton()->CreateImageLayer("MMListScrollUp",0,"Normal");
    MMResolutionOptionsScroll->GetUpLeftButton()->CreateImageLayer("MMHoveredListScrollUp",0,"Hovered");
    // Configure the down button
    MMResolutionOptionsScroll->GetDownRightButton()->CreateImageLayer("MMListScrollDown",0,"Normal");
    MMResolutionOptionsScroll->GetDownRightButton()->CreateImageLayer("MMHoveredListScrollDown",0,"Hovered");
    // Configure the scroll back
    MMResolutionOptionsScroll->GetScrollBack()->CreateImageLayer("MMListScrollBackground",0,0);
    // Wrap up listing configuration
    MMResolutionList->Subscribe(UI::Widget::EventVisibilityShown,this->VideoSettingsWork->GetSettingsSubscriber());
    MMVideoSet->AddChild(MMResolutionList,11);

    // Create the checkbox for enabling or disabling fullscreen
    UI::CheckBox* MMFullscreenBox = MainMenuScreen->CreateCheckBox("MS_FullscreenBox",UI::UnifiedRect(0.655,0.145,0,0.12));
    MMFullscreenBox->SetHorizontalSizingRules(UI::SR_Match_Other_Axis);
    MMFullscreenBox->CreateImageLayer("MMCheckboxUnchecked",0,"Normal");
    MMFullscreenBox->CreateImageLayer("MMHoveredCheckboxUnchecked",0,"Hovered");
    MMFullscreenBox->CreateImageLayer("MMCheckboxChecked",0,"SelectedNormal");
    MMFullscreenBox->CreateImageLayer("MMHoveredCheckboxChecked",0,"SelectedHovered");
    MMFullscreenBox->Subscribe(UI::Widget::EventVisibilityShown,this->VideoSettingsWork->GetSettingsSubscriber());
    MMVideoSet->AddChild(MMFullscreenBox,3);

    // Create the label for the fullscreen checkbox
    UI::Widget* MMFullscreenLabel = MainMenuScreen->CreateWidget("MS_FullscreenLabel",UI::UnifiedRect(0.72,0.145,0.22,0.12));
    MMFullscreenLabel->CreateImageLayer("MMButton",0,0);
    UI::SingleLineTextLayer* MMFullscreenLabelText = MMFullscreenLabel->CreateSingleLineTextLayer(MainMenuScreenText,1,1);
    MMFullscreenLabelText->SetText("Fullscreen");
    MMFullscreenLabelText->HorizontallyAlign(UI::LA_Center);
    MMFullscreenLabelText->VerticallyAlign(UI::LA_Center);
    MMFullscreenLabelText->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,MMTightText);
    MMVideoSet->AddChild(MMFullscreenLabel,4);

    // Create the label for the FSAA options
    UI::Widget* MMFSAALabel = MainMenuScreen->CreateWidget("MS_FSAALabel",UI::UnifiedRect(0.12,0.365,0.415,0.13));
    MMFSAALabel->CreateImageLayer("MMButton",0,0);
    UI::SingleLineTextLayer* MMFSAALabelText = MMFSAALabel->CreateSingleLineTextLayer(MainMenuScreenText,1,1);
    MMFSAALabelText->SetText("Anti-Aliasing");
    MMFSAALabelText->HorizontallyAlign(UI::LA_Center);
    MMFSAALabelText->VerticallyAlign(UI::LA_Center);
    MMFSAALabelText->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,MMTightText);
    MMVideoSet->AddChild(MMFSAALabel,5);

    // Create the listing of detected anti-aliasing options
    UI::DropDownList* MMFSAAList = MainMenuScreen->CreateDropDownList("MS_FSAAList",UI::UnifiedRect(0.065,0.50,0.515,0.12),UI::SB_Separate);
    // Configure the selection display
    UI::Widget* MMFSAADisplay = MMFSAAList->GetSelectionDisplay();
    MMFSAADisplay->CreateImageLayer("MMListSelection",0,0);
    UI::SingleLineTextLayer* MMFSAADisplayText = static_cast<UI::SingleLineTextLayer*>( MMFSAADisplay->GetRenderLayer(0,UI::RLT_SingleLineText) );
    MMFSAADisplayText->SetDefaultFont(MainMenuScreenText);
    MMFSAADisplayText->HorizontallyAlign(UI::LA_Center);
    MMFSAADisplayText->VerticallyAlign(UI::LA_Center);
    MMFSAADisplayText->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,MMTightText);
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
    MMFSAAOptions->SetListItemFont(MainMenuScreenText);
    // Configure the background for the listbox container
    UI::VerticalContainer* MMFSAAOptionsList = MMFSAAOptions->GetListContainer();
    MMFSAAOptionsList->CreateImageLayer("MMListBackground",0,0);
    MMFSAAOptionsList->SetChildSizing(UI::SizingInfo(UI::UnifiedVec2(1.0,0.19)),UI::LinearContainer::SE_OnAdd);
    // Configure the scrollbar for the option list
    UI::VerticalScrollbar* MMFSAAOptionsScroll = MMFSAAOptions->GetListScroll();
    MMFSAAOptionsScroll->SetIncrementDistance(0.05);
    // Configure the scroller
    MMFSAAOptionsScroll->GetScroller()->CreateImageLayer("MMListScroller",0,"Normal");
    MMFSAAOptionsScroll->GetScroller()->CreateImageLayer("MMHoveredListScroller",0,"Hovered");
    // Configure the up button
    MMFSAAOptionsScroll->GetUpLeftButton()->CreateImageLayer("MMListScrollUp",0,"Normal");
    MMFSAAOptionsScroll->GetUpLeftButton()->CreateImageLayer("MMHoveredListScrollUp",0,"Hovered");
    // Configure the down button
    MMFSAAOptionsScroll->GetDownRightButton()->CreateImageLayer("MMListScrollDown",0,"Normal");
    MMFSAAOptionsScroll->GetDownRightButton()->CreateImageLayer("MMHoveredListScrollDown",0,"Hovered");
    // Configure the scroll back
    MMFSAAOptionsScroll->GetScrollBack()->CreateImageLayer("MMListScrollBackground",0,0);
    // Wrap up listing configuration
    MMFSAAList->Subscribe(UI::Widget::EventVisibilityShown,this->VideoSettingsWork->GetSettingsSubscriber());
    MMVideoSet->AddChild(MMFSAAList,10);

    // Create the checkbox for enabling or disabling FPS stats display
    UI::CheckBox* MMStatsBox = MainMenuScreen->CreateCheckBox("MS_StatsBox",UI::UnifiedRect(0.655,0.50,0,0.12));
    MMStatsBox->SetHorizontalSizingRules(UI::SR_Match_Other_Axis);
    MMStatsBox->CreateImageLayer("MMCheckboxUnchecked",0,"Normal");
    MMStatsBox->CreateImageLayer("MMHoveredCheckboxUnchecked",0,"Hovered");
    MMStatsBox->CreateImageLayer("MMCheckboxChecked",0,"SelectedNormal");
    MMStatsBox->CreateImageLayer("MMHoveredCheckboxChecked",0,"SelectedHovered");
    MMStatsBox->Subscribe(UI::Widget::EventVisibilityShown,this->VideoSettingsWork->GetSettingsSubscriber());
    MMVideoSet->AddChild(MMStatsBox,7);

    // Create the label for the FPS stats display checkbox
    UI::Widget* MMStatsLabel = MainMenuScreen->CreateWidget("MS_StatsLabel",UI::UnifiedRect(0.72,0.50,0.22,0.12));
    MMStatsLabel->CreateImageLayer("MMButton",0,0);
    UI::SingleLineTextLayer* MMStatsLabelText = MMStatsLabel->CreateSingleLineTextLayer(MainMenuScreenText,1,1);
    MMStatsLabelText->SetText("Show FPS");
    MMStatsLabelText->HorizontallyAlign(UI::LA_Center);
    MMStatsLabelText->VerticallyAlign(UI::LA_Center);
    MMStatsLabelText->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,MMTightText);
    MMVideoSet->AddChild(MMStatsLabel,8);

    // Create the button that will apply all of the currently displayed video settings
    UI::Button* MMVideoOptsApply = MainMenuScreen->CreateButton("MS_VideoOptsApply",UI::UnifiedRect(0.815,0.84,0.17,0.14));
    MMVideoOptsApply->CreateImageLayer("MMOptionsApplyButton",0,"Normal");
    MMVideoOptsApply->CreateImageLayer("MMOptionsApplyHoveredButton",0,"Hovered");
    UI::SingleLineTextLayer* MMVideoOptsApplyText = MMVideoOptsApply->CreateSingleLineTextLayer(MainMenuScreenText,1,1);
    MMVideoOptsApplyText->SetText("Apply");
    MMVideoOptsApplyText->HorizontallyAlign(UI::LA_Center);
    MMVideoOptsApplyText->VerticallyAlign(UI::LA_Center);
    MMVideoOptsApplyText->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,MMTightText);
    MMVideoOptsApply->Subscribe(UI::Button::EventDeactivated,this->VideoSettingsWork->GetSettingsSubscriber());
    MMVideoSet->AddChild(MMVideoOptsApply,12);

    const Real MMScrollerSize = 0.09;
    // Create the TabbedSubSet that will house all our audio options
    UI::TabSet::TabbedSubSet* MMAudioSet = MMOptionsTabSet->CreateTabbedSubSet("MS_AudioSet",2);

    // Create the label for the Music volume
    UI::Widget* MMMusicVolLabel = MainMenuScreen->CreateWidget("MS_MusicVolLabel",UI::UnifiedRect(0.0,0.01,0.40,0.13));
    MMMusicVolLabel->SetPositioningRules(UI::PF_Anchor_HorizontalCenter);
    MMMusicVolLabel->CreateImageLayer("MMButton",0,0);
    UI::SingleLineTextLayer* MMMusicVolLabelText = MMMusicVolLabel->CreateSingleLineTextLayer(MainMenuScreenText,1,1);
    MMMusicVolLabelText->SetText("Music Volume");
    MMMusicVolLabelText->HorizontallyAlign(UI::LA_Center);
    MMMusicVolLabelText->VerticallyAlign(UI::LA_Center);
    MMMusicVolLabelText->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,MMTightText);
    MMAudioSet->AddChild(MMMusicVolLabel,1);

    // Create the Music volume slider
    UI::HorizontalScrollbar* MMMusicVol = MainMenuScreen->CreateHorizontalScrollbar("MS_MusicVolume",UI::UnifiedRect(0.0,0.145,0.7,0.09),UI::SB_Separate);
    MMMusicVol->SetPositioningRules(UI::PF_Anchor_HorizontalCenter);
    MMMusicVol->SetIncrementDistance(MMScrollerSize * 0.5);
    MMMusicVol->SetScrollerSize(MMScrollerSize);
    // Configure the scroller
    MMMusicVol->GetScroller()->CreateImageLayer("MMScroller",0,"Normal");
    MMMusicVol->GetScroller()->CreateImageLayer("MMHoveredScroller",0,"Hovered");
    // Configure the up button
    MMMusicVol->GetUpLeftButton()->CreateImageLayer("MMScrollLeft",0,"Normal");
    MMMusicVol->GetUpLeftButton()->CreateImageLayer("MMHoveredScrollLeft",0,"Hovered");
    // Configure the down button
    MMMusicVol->GetDownRightButton()->CreateImageLayer("MMScrollRight",0,"Normal");
    MMMusicVol->GetDownRightButton()->CreateImageLayer("MMHoveredScrollRight",0,"Hovered");
    // Configure the scroll back
    MMMusicVol->GetScrollBack()->CreateImageLayer("MMScrollBackground",0,0);
    // Wrap up Effects volume ocnfiguration
    MMMusicVol->Subscribe(UI::Scrollbar::EventScrollValueChanged,this->AudioSettingsWork->GetSettingsSubscriber());
    MMMusicVol->Subscribe(UI::Widget::EventVisibilityShown,this->AudioSettingsWork->GetSettingsSubscriber());
    MMAudioSet->AddChild(MMMusicVol,2);

    // Create the label for the Effects volume
    UI::Widget* MMEffectsVolLabel = MainMenuScreen->CreateWidget("MS_EffectsVolLabel",UI::UnifiedRect(0.0,0.275,0.40,0.13));
    MMEffectsVolLabel->SetPositioningRules(UI::PF_Anchor_HorizontalCenter);
    MMEffectsVolLabel->CreateImageLayer("MMButton",0,0);
    UI::SingleLineTextLayer* MMEffectsVolLabelText = MMEffectsVolLabel->CreateSingleLineTextLayer(MainMenuScreenText,1,1);
    MMEffectsVolLabelText->SetText("Effects Volume");
    MMEffectsVolLabelText->HorizontallyAlign(UI::LA_Center);
    MMEffectsVolLabelText->VerticallyAlign(UI::LA_Center);
    MMEffectsVolLabelText->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,MMTightText);
    MMAudioSet->AddChild(MMEffectsVolLabel,3);

    // Create the Effects volume slider
    UI::HorizontalScrollbar* MMEffectsVol = MainMenuScreen->CreateHorizontalScrollbar("MS_EffectsVolume",UI::UnifiedRect(0.0,0.41,0.7,0.09),UI::SB_Separate);
    MMEffectsVol->SetPositioningRules(UI::PF_Anchor_HorizontalCenter);
    MMEffectsVol->SetIncrementDistance(MMScrollerSize * 0.5);
    MMEffectsVol->SetScrollerSize(MMScrollerSize);
    // Configure the scroller
    MMEffectsVol->GetScroller()->CreateImageLayer("MMScroller",0,"Normal");
    MMEffectsVol->GetScroller()->CreateImageLayer("MMHoveredScroller",0,"Hovered");
    // Configure the up button
    MMEffectsVol->GetUpLeftButton()->CreateImageLayer("MMScrollLeft",0,"Normal");
    MMEffectsVol->GetUpLeftButton()->CreateImageLayer("MMHoveredScrollLeft",0,"Hovered");
    // Configure the down button
    MMEffectsVol->GetDownRightButton()->CreateImageLayer("MMScrollRight",0,"Normal");
    MMEffectsVol->GetDownRightButton()->CreateImageLayer("MMHoveredScrollRight",0,"Hovered");
    // Configure the scroll back
    MMEffectsVol->GetScrollBack()->CreateImageLayer("MMScrollBackground",0,0);
    // Wrap up Effects volume ocnfiguration
    MMEffectsVol->Subscribe(UI::Scrollbar::EventScrollValueChanged,this->AudioSettingsWork->GetSettingsSubscriber());
    MMEffectsVol->Subscribe(UI::Widget::EventVisibilityShown,this->AudioSettingsWork->GetSettingsSubscriber());
    MMAudioSet->AddChild(MMEffectsVol,2);

    /*// Create the label for the Audio Device options
    UI::Widget* MMAudioDeviceLabel = MainMenuScreen->CreateWidget("MS_AudioDeviceLabel",UI::UnifiedRect(0.12,0.56,0.415,0.13));
    MMAudioDeviceLabel->CreateImageLayer("MMButton",0,0);
    UI::SingleLineTextLayer* MMAudioDeviceLabelText = MMAudioDeviceLabel->CreateSingleLineTextLayer(MainMenuScreenText,1,1);
    MMAudioDeviceLabelText->SetText("Audio Device");
    MMAudioDeviceLabelText->HorizontallyAlign(UI::LA_Center);
    MMAudioDeviceLabelText->VerticallyAlign(UI::LA_Center);
    MMAudioDeviceLabelText->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,MMTightText);
    MMAudioSet->AddChild(MMAudioDeviceLabel,3);

    // Create the listing of detected audio device options
    UI::DropDownList* MMAudioDeviceList = MainMenuScreen->CreateDropDownList("MS_AudioDeviceList",UI::UnifiedRect(0.065,0.695,0.515,0.12),UI::SB_Separate);
    // Configure the selection display
    UI::Widget* MMAudioDeviceDisplay = MMAudioDeviceList->GetSelectionDisplay();
    MMAudioDeviceDisplay->CreateImageLayer("MMListSelection",0,0);
    UI::SingleLineTextLayer* MMAudioDeviceDisplayText = static_cast<UI::SingleLineTextLayer*>( MMAudioDeviceDisplay->GetRenderLayer(0,UI::RLT_SingleLineText) );
    MMAudioDeviceDisplayText->SetDefaultFont(MainMenuScreenText);
    MMAudioDeviceDisplayText->HorizontallyAlign(UI::LA_Center);
    MMAudioDeviceDisplayText->VerticallyAlign(UI::LA_Center);
    MMAudioDeviceDisplayText->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,MMTightText);
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
    MMAudioDeviceOptions->SetListItemFont(MainMenuScreenText);
    // Configure the background for the listbox container
    UI::VerticalContainer* MMAudioDeviceOptionsList = MMAudioDeviceOptions->GetListContainer();
    MMAudioDeviceOptionsList->CreateImageLayer("MMListBackground",0,0);
    MMAudioDeviceOptionsList->SetChildSizing(UI::SizingInfo(UI::UnifiedVec2(1.0,0.19)),UI::LinearContainer::SE_OnAdd);
    // Configure the scrollbar for the option list
    UI::VerticalScrollbar* MMAudioDeviceOptionsScroll = MMAudioDeviceOptions->GetListScroll();
    MMAudioDeviceOptionsScroll->SetIncrementDistance(0.05);
    // Configure the scroller
    MMAudioDeviceOptionsScroll->GetScroller()->CreateImageLayer("MMListScroller",0,"Normal");
    MMAudioDeviceOptionsScroll->GetScroller()->CreateImageLayer("MMHoveredListScroller",0,"Hovered");
    // Configure the up button
    MMAudioDeviceOptionsScroll->GetUpLeftButton()->CreateImageLayer("MMListScrollUp",0,"Normal");
    MMAudioDeviceOptionsScroll->GetUpLeftButton()->CreateImageLayer("MMHoveredListScrollUp",0,"Hovered");
    // Configure the down button
    MMAudioDeviceOptionsScroll->GetDownRightButton()->CreateImageLayer("MMListScrollDown",0,"Normal");
    MMAudioDeviceOptionsScroll->GetDownRightButton()->CreateImageLayer("MMHoveredListScrollDown",0,"Hovered");
    // Configure the scroll back
    MMAudioDeviceOptionsScroll->GetScrollBack()->CreateImageLayer("MMListScrollBackground",0,0);
    // Wrap up listing configuration
    MMAudioDeviceList->Subscribe(UI::Widget::EventVisibilityShown,this->AudioSettingsWork->GetSettingsSubscriber());
    MMAudioSet->AddChild(MMAudioDeviceList,10);//*/

    // Create the checkbox for enabling or disabling FPS stats display
    UI::CheckBox* MMMuteBox = MainMenuScreen->CreateCheckBox("MS_MuteBox",UI::UnifiedRect(0.655,0.695,0,0.12));
    MMMuteBox->SetHorizontalSizingRules(UI::SR_Match_Other_Axis);
    MMMuteBox->CreateImageLayer("MMCheckboxUnchecked",0,"Normal");
    MMMuteBox->CreateImageLayer("MMHoveredCheckboxUnchecked",0,"Hovered");
    MMMuteBox->CreateImageLayer("MMCheckboxChecked",0,"SelectedNormal");
    MMMuteBox->CreateImageLayer("MMHoveredCheckboxChecked",0,"SelectedHovered");
    MMMuteBox->Subscribe(UI::CheckBox::EventSelected,this->AudioSettingsWork->GetSettingsSubscriber());
    MMMuteBox->Subscribe(UI::CheckBox::EventDeselected,this->AudioSettingsWork->GetSettingsSubscriber());
    MMMuteBox->Subscribe(UI::Widget::EventVisibilityShown,this->AudioSettingsWork->GetSettingsSubscriber());
    MMAudioSet->AddChild(MMMuteBox,5);

    // Create the label for the FPS stats display checkbox
    UI::Widget* MMMuteLabel = MainMenuScreen->CreateWidget("MS_MuteLabel",UI::UnifiedRect(0.72,0.695,0.22,0.12));
    MMMuteLabel->CreateImageLayer("MMButton",0,0);
    UI::SingleLineTextLayer* MMMuteLabelText = MMMuteLabel->CreateSingleLineTextLayer(MainMenuScreenText,1,1);
    MMMuteLabelText->SetText("Mute");
    MMMuteLabelText->HorizontallyAlign(UI::LA_Center);
    MMMuteLabelText->VerticallyAlign(UI::LA_Center);
    MMMuteLabelText->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,MMTightText);
    MMAudioSet->AddChild(MMMuteLabel,6);

    // Create the back button for the options window
    UI::StackButton* MMOptsBack = MainMenuScreen->CreateStackButton("MS_OptsBack",UI::UnifiedRect(0.780,0.870,0.156,0.094));
    MMOptsBack->CreateImageLayer("MMOptionsApplyButton",0,"Normal");
    MMOptsBack->CreateImageLayer("MMOptionsApplyHoveredButton",0,"Hovered");
    UI::SingleLineTextLayer* MMOptsBackText = MMOptsBack->CreateSingleLineTextLayer(MainMenuScreenText,1,1);
    MMOptsBackText->SetText("Back");
    MMOptsBackText->HorizontallyAlign(UI::LA_Center);
    MMOptsBackText->VerticallyAlign(UI::LA_Center);
    MMOptsBackText->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,MMTightText);
    MMOptionsWin->SetPopButton(MMOptsBack);
    MMOptionsWin->AddChild(MMOptsBack,5);

    ////------------------  Credits  ------------------////
    // Start with the accessor button
    UI::StackButton* MMCreditsAccess = MainMenuScreen->CreateStackButton("MS_Credits",UI::UnifiedRect(0.51,0.18,0.22,0.7));
    MMCreditsAccess->CreateImageLayer("MMButton",0,"Normal");
    MMCreditsAccess->CreateImageLayer("MMHoveredButton",0,"Hovered");
    UI::SingleLineTextLayer* MMCreditsAccessText = MMCreditsAccess->CreateSingleLineTextLayer(MainMenuScreenText,1,1);
    MMCreditsAccessText->SetText("Credits");
    MMCreditsAccessText->HorizontallyAlign(UI::LA_Center);
    MMCreditsAccessText->VerticallyAlign(UI::LA_Center);
    MMCreditsAccessText->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,MMNormText);
    MMRootEntry->AddChild(MMCreditsAccess,3);

    // Create and configure the "window" that houses the options display
    UI::MenuEntry* MMCreditsWin = MainMenuScreen->CreateMenuEntry("MS_CreditsWin",UI::UnifiedRect(0.18,-9.04,0.64,6.4));
    MMCreditsWin->CreateImageLayer("MMOptionsBackground",0,0);
    MMCreditsWin->SetPushButton(MMCreditsAccess);
    MMRootEntry->AddChild(MMCreditsWin,7);

    ////------------------  Exit Game  ------------------////
    // Start with the accessor button
    UI::StackButton* MMAppExitAccess = MainMenuScreen->CreateStackButton("MS_AppExit",UI::UnifiedRect(0.74,0.18,0.22,0.7));
    MMAppExitAccess->CreateImageLayer("MMButton",0,"Normal");
    MMAppExitAccess->CreateImageLayer("MMHoveredButton",0,"Hovered");
    UI::SingleLineTextLayer* MMAppExitAccessText = MMAppExitAccess->CreateSingleLineTextLayer(MainMenuScreenText,1,1);
    MMAppExitAccessText->SetText("Exit Game");
    MMAppExitAccessText->HorizontallyAlign(UI::LA_Center);
    MMAppExitAccessText->VerticallyAlign(UI::LA_Center);
    MMAppExitAccessText->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,MMNormText);
    MMRootEntry->AddChild(MMAppExitAccess,4);

    // Create and configure the "window" that houses the exit confirmation
    UI::MenuEntry* MMAppExitWin = MainMenuScreen->CreateMenuEntry("MS_AppExitWin",UI::UnifiedRect(0.25,-8.14,0.5,3.5));
    MMAppExitWin->CreateImageLayer("MMAppExitBackground",0,0);
    MMAppExitWin->SetPushButton(MMAppExitAccess);
    MMRootEntry->AddChild(MMAppExitWin,8);

    // Create and configure the display for the confirmation question
    UI::Widget* MMAppExitWarn = MainMenuScreen->CreateWidget("MS_AppExitWarn",UI::UnifiedRect(0.14,0.18,0.72,0.22));
    MMAppExitWarn->CreateImageLayer("MMAppExitText",0,0);
    UI::SingleLineTextLayer* MMAppExitWarnText = MMAppExitWarn->CreateSingleLineTextLayer(MainMenuScreenText,1,1);
    MMAppExitWarnText->SetText("Are you sure you want to exit?");
    MMAppExitWarnText->HorizontallyAlign(UI::LA_Center);
    MMAppExitWarnText->VerticallyAlign(UI::LA_Center);
    MMAppExitWarnText->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,MMNormText);
    MMAppExitWin->AddChild(MMAppExitWarn,1);

    // Create and configure the confirm button
    UI::Button* MMAppExitConf = MainMenuScreen->CreateButton("MS_AppExitConf",UI::UnifiedRect(0.10,0.58,0.35,0.22));
    MMAppExitConf->CreateImageLayer("MMAppExitButton",0,"Normal");
    MMAppExitConf->CreateImageLayer("MMAppExitHoveredButton",0,"Hovered");
    UI::SingleLineTextLayer* MMAppExitConfText = MMAppExitConf->CreateSingleLineTextLayer(MainMenuScreenText,1,1);
    MMAppExitConfText->SetText("Yes");
    MMAppExitConfText->HorizontallyAlign(UI::LA_Center);
    MMAppExitConfText->VerticallyAlign(UI::LA_Center);
    MMAppExitConfText->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,MMNormText);
    MMAppExitConf->Subscribe(UI::Button::EventDeactivated,&AllAppExit);
    MMAppExitWin->AddChild(MMAppExitConf,2);

    // Create and configure the deny button
    UI::StackButton* MMAppExitDeny = MainMenuScreen->CreateStackButton("MS_AppExitDeny",UI::UnifiedRect(0.55,0.58,0.35,0.22));
    MMAppExitDeny->CreateImageLayer("MMAppExitButton",0,"Normal");
    MMAppExitDeny->CreateImageLayer("MMAppExitHoveredButton",0,"Hovered");
    UI::SingleLineTextLayer* MMAppExitDenyText = MMAppExitDeny->CreateSingleLineTextLayer(MainMenuScreenText,1,1);
    MMAppExitDenyText->SetText("No");
    MMAppExitDenyText->HorizontallyAlign(UI::LA_Center);
    MMAppExitDenyText->VerticallyAlign(UI::LA_Center);
    MMAppExitDenyText->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,MMNormText);
    MMAppExitWin->SetPopButton(MMAppExitDeny);
    MMAppExitWin->AddChild(MMAppExitDeny,3);

    ////////////////////////////////////////////////////////////////
    ////--------------------  Game  Screen  --------------------////
    ////////////////////////////////////////////////////////////////

    const Real GSNormText = 0.65;
    const Real GSLargeText = 0.85;
    const Real GSMultiLineText = 0.20;

    const String GameScreenText = "Ubuntu-14";

    GUI->LoadMTA("Catch_Game.mta","Common");
    UI::Screen* GameScreen = GUI->CreateScreen("GameScreen","Catch_Game",UIViewport,2);

    ////----------------  Heads Up Display  ----------------////
    // Create the widget container for the score display
    UI::Widget* GSScore = GameScreen->CreateWidget("GS_Score",UI::UnifiedRect(0.008,0.006,0.27,0.065));
    GSScore->SetAspectRatioLock(UI::ARL_Ratio_Y_Axis);
    // Create the widget for displaying the timer logo
    UI::Widget* GSScoreLabel = GameScreen->CreateWidget("GS_ScoreLabel",UI::UnifiedRect(0.0,0.0,0.45,1.0));
    GSScoreLabel->SetPositioningRules(UI::PF_Anchor_TopLeft);
    GSScoreLabel->CreateImageLayer("GSScoreTextArea",0,0);
    GSScore->AddChild(GSScoreLabel,1);
    // Create the widget for displaying the timer value
    UI::Widget* GSScoreValue = GameScreen->CreateWidget("GS_ScoreValue",UI::UnifiedRect(0.0,0.0,0.55,1.0));
    GSScoreValue->SetPositioningRules(UI::PF_Anchor_TopRight);
    GSScoreValue->CreateImageLayer("GSScoreValueArea",0,0);
    UI::SingleLineTextLayer* GSScoreValueText = GSScoreValue->CreateSingleLineTextLayer(GameScreenText,1,1);
    GSScoreValueText->SetText("0:00");
    GSScoreValueText->HorizontallyAlign(UI::LA_Center);
    GSScoreValueText->VerticallyAlign(UI::LA_Center);
    GSScoreValueText->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,GSNormText);
    GSScore->AddChild(GSScoreValue,2);
    // Wrap up timer configuration
    GameScreen->AddChild(GSScore,1);

    // Create the widget container for the timer display
    UI::Widget* GSTimer = GameScreen->CreateWidget("GS_Timer",UI::UnifiedRect(0.8355,0.006,0.16,0.065));
    GSTimer->SetAspectRatioLock(UI::ARL_Ratio_Y_Axis);
    // Create the widget for displaying the timer logo
    UI::Widget* GSTimerLogo = GameScreen->CreateWidget("GS_TimerLogo",UI::UnifiedRect(0.0,0.0,0.66,1.0));
    GSTimerLogo->SetPositioningRules(UI::PF_Anchor_TopLeft);
    GSTimerLogo->CreateImageLayer("GSTimerLogo",0,0);
    GSTimer->AddChild(GSTimerLogo,1);
    // Create the widget for displaying the timer value
    UI::Widget* GSTimerValue = GameScreen->CreateWidget("GS_TimerValue",UI::UnifiedRect(0.0,0.0,0.34,1.0));
    GSTimerValue->SetPositioningRules(UI::PF_Anchor_TopRight);
    GSTimerValue->CreateImageLayer("GSTimerArea",0,0);
    UI::SingleLineTextLayer* GSTimerValueText = GSTimerValue->CreateSingleLineTextLayer(GameScreenText,1,1);
    GSTimerValueText->SetText("0");
    GSTimerValueText->HorizontallyAlign(UI::LA_Center);
    GSTimerValueText->VerticallyAlign(UI::LA_Center);
    GSTimerValueText->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,GSNormText);
    GSTimer->AddChild(GSTimerValue,2);
    // Wrap up timer configuration
    GameScreen->AddChild(GSTimer,2);

    // Create the menu button
    UI::StackButton* GSMenuAccess = GameScreen->CreateStackButton("GS_MenuAccess",UI::UnifiedRect(0.008,0.935,0.16,0.065));
    GSMenuAccess->SetAspectRatioLock(UI::ARL_Ratio_Y_Axis);
    GSMenuAccess->CreateImageLayer("GSMenuStoreButton",0,"Normal");
    GSMenuAccess->CreateImageLayer("GSMenuStoreHoveredButton",0,"Hovered");
    UI::SingleLineTextLayer* GSMenuAccessText = GSMenuAccess->CreateSingleLineTextLayer(GameScreenText,1,1);
    GSMenuAccessText->SetText("Menu");
    GSMenuAccessText->HorizontallyAlign(UI::LA_Center);
    GSMenuAccessText->VerticallyAlign(UI::LA_Center);
    GSMenuAccessText->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,GSNormText);
    GameScreen->AddChild(GSMenuAccess,3);

    // Create the item shop button
    UI::StackButton* GSItemShopAccess = GameScreen->CreateStackButton("GS_ItemShopAccess",UI::UnifiedRect(0.822,0.935,0.16,0.065));
    GSItemShopAccess->SetAspectRatioLock(UI::ARL_Ratio_Y_Axis);
    GSItemShopAccess->CreateImageLayer("GSMenuStoreButton",0,"Normal");
    GSItemShopAccess->CreateImageLayer("GSMenuStoreHoveredButton",0,"Hovered");
    UI::SingleLineTextLayer* GSItemShopAccessText = GSItemShopAccess->CreateSingleLineTextLayer(GameScreenText,1,1);
    GSItemShopAccessText->SetText("Store");
    GSItemShopAccessText->HorizontallyAlign(UI::LA_Center);
    GSItemShopAccessText->VerticallyAlign(UI::LA_Center);
    GSItemShopAccessText->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,GSNormText);
    GameScreen->AddChild(GSItemShopAccess,4);

    ////-------------------  Game  Menu  -------------------////
    // Create the root entry for the game menu
    UI::MenuEntry* GSMenuRoot = GameScreen->CreateMenuEntry("GS_MenuRoot",UI::UnifiedRect(0.35,0.27,0.3,0.45));
    GSMenuRoot->SetAspectRatioLock(UI::ARL_Ratio_Y_Axis);
    GSMenuRoot->CreateImageLayer("GSOptionsMenuBackground",0,0);
    GSMenuRoot->SetPushButton(GSMenuAccess);
    GameScreen->AddChild(GSMenuRoot,10);

    // Create the options accessor button
    UI::StackButton* GSOptionsAccess = GameScreen->CreateStackButton("GS_Options",UI::UnifiedRect(0.15,0.075,0.70,0.11));
    GSOptionsAccess->CreateImageLayer("GSOptionsButton",0,"Normal");
    GSOptionsAccess->CreateImageLayer("GSOptionsHoveredButton",0,"Hovered");
    UI::SingleLineTextLayer* GSOptionsAccessText = GSOptionsAccess->CreateSingleLineTextLayer(GameScreenText,1,1);
    GSOptionsAccessText->SetText("Options");
    GSOptionsAccessText->HorizontallyAlign(UI::LA_Center);
    GSOptionsAccessText->VerticallyAlign(UI::LA_Center);
    GSOptionsAccessText->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,GSNormText);
    GSMenuRoot->AddChild(GSOptionsAccess,1);

    // Create and configure the "window" that houses the options display
    UI::MenuEntry* GSOptionsWin = GameScreen->CreateMenuEntry("GS_OptionsWin",UI::UnifiedRect(0.18,-8.08,0.64,6.4));
    GSOptionsWin->CreateImageLayer("GSOptionsBackground",0,0);
    GSOptionsWin->SetPushButton(GSOptionsAccess);
    GSMenuRoot->AddChild(GSOptionsWin,7);

    // Create the first of the two buttons that will display the two sets of options (video options)
    UI::StackButton* GSVideoSetAccess = GameScreen->CreateStackButton("GS_VideoSetAccess",UI::UnifiedRect(0.11,0.0365,0.34,0.11));
    GSVideoSetAccess->CreateImageLayer("GSButton",0,"Normal");
    GSVideoSetAccess->CreateImageLayer("GSHoveredButton",0,"Hovered");
    UI::SingleLineTextLayer* GSVideoSetAccessText = GSVideoSetAccess->CreateSingleLineTextLayer(GameScreenText,1,1);
    GSVideoSetAccessText->SetText("Video Options");
    GSVideoSetAccessText->HorizontallyAlign(UI::LA_Center);
    GSVideoSetAccessText->VerticallyAlign(UI::LA_Center);
    GSVideoSetAccessText->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,GSNormText);
    GSOptionsWin->AddChild(GSVideoSetAccess,1);

    // Create the second of the two buttons that will display the two sets of options (audio options)
    UI::StackButton* GSAudioSetAccess = GameScreen->CreateStackButton("GS_AudioSetAccess",UI::UnifiedRect(0.55,0.0365,0.34,0.11));
    GSAudioSetAccess->CreateImageLayer("GSButton",0,"Normal");
    GSAudioSetAccess->CreateImageLayer("GSHoveredButton",0,"Hovered");
    UI::SingleLineTextLayer* GSAudioSetAccessText = GSAudioSetAccess->CreateSingleLineTextLayer(GameScreenText,1,1);
    GSAudioSetAccessText->SetText("Audio Options");
    GSAudioSetAccessText->HorizontallyAlign(UI::LA_Center);
    GSAudioSetAccessText->VerticallyAlign(UI::LA_Center);
    GSAudioSetAccessText->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,GSNormText);
    GSOptionsWin->AddChild(GSAudioSetAccess,2);

    // Create the TabSet that will hold all of our options widgets
    UI::TabSet* GSOptionsTabSet = GameScreen->CreateTabSet("GS_OptionsTS",UI::UnifiedRect(0.05,0.16,0.90,0.70));
    GSOptionsTabSet->SetButtonConfig(1,GSVideoSetAccess);
    GSOptionsTabSet->SetButtonConfig(2,GSAudioSetAccess);
    GSOptionsWin->AddChild(GSOptionsTabSet,3);

    // Create the TabbedSubSet that will house all our video options
    UI::TabSet::TabbedSubSet* GSVideoSet = GSOptionsTabSet->CreateTabbedSubSet("GS_VideoSet",1);
    // Go ahead and make the video options the one visible by default
    GSOptionsTabSet->SetVisibleSubSet(GSVideoSet);

    // Create the label for the window resolution list
    UI::Widget* GSResolutionLabel = GameScreen->CreateWidget("GS_ResolutionLabel",UI::UnifiedRect(0.12,0.01,0.415,0.13));
    GSResolutionLabel->CreateImageLayer("GSButton",0,0);
    UI::SingleLineTextLayer* GSResolutionLabelText = GSResolutionLabel->CreateSingleLineTextLayer(GameScreenText,1,1);
    GSResolutionLabelText->SetText("Video Resolutions");
    GSResolutionLabelText->HorizontallyAlign(UI::LA_Center);
    GSResolutionLabelText->VerticallyAlign(UI::LA_Center);
    GSResolutionLabelText->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,GSLargeText);
    GSVideoSet->AddChild(GSResolutionLabel,1);

    // Create the listing of detected supported resolutions
    UI::DropDownList* GSResolutionList = GameScreen->CreateDropDownList("GS_ResolutionList",UI::UnifiedRect(0.065,0.145,0.515,0.12),UI::SB_Separate);
    // Configure the selection display
    UI::Widget* GSResolutionDisplay = GSResolutionList->GetSelectionDisplay();
    GSResolutionDisplay->CreateImageLayer("GSListSelection",0,0);
    UI::SingleLineTextLayer* GSResolutionDisplayText = static_cast<UI::SingleLineTextLayer*>( GSResolutionDisplay->GetRenderLayer(0,UI::RLT_SingleLineText) );
    GSResolutionDisplayText->SetDefaultFont(GameScreenText);
    GSResolutionDisplayText->HorizontallyAlign(UI::LA_Center);
    GSResolutionDisplayText->VerticallyAlign(UI::LA_Center);
    GSResolutionDisplayText->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,GSLargeText);
    // Configure the list toggle
    UI::CheckBox* GSResolutionToggle = GSResolutionList->GetListToggle();
    UI::ImageLayer* GSResolutionToggleNormal = GSResolutionToggle->CreateImageLayer("GSListScrollDown");
    UI::ImageLayer* GSResolutionToggleHovered = GSResolutionToggle->CreateImageLayer("GSHoveredListScrollDown");
    GSResolutionToggle->AddLayerToGroup(GSResolutionToggleNormal,0,"Normal");
    GSResolutionToggle->AddLayerToGroup(GSResolutionToggleHovered,0,"Hovered");
    GSResolutionToggle->AddLayerToGroup(GSResolutionToggleNormal,0,"SelectedNormal");
    GSResolutionToggle->AddLayerToGroup(GSResolutionToggleHovered,0,"SelectedHovered");
    // Configure the option list
    UI::ListBox* GSResolutionOptions = GSResolutionList->GetSelectionList();
    GSResolutionOptions->SetListItemFont(GameScreenText);
    // Configure the background for the listbox container
    UI::VerticalContainer* GSResolutionOptionsList = GSResolutionOptions->GetListContainer();
    GSResolutionOptionsList->SetChildSizing(UI::SizingInfo(UI::UnifiedVec2(1.0,0.19)),UI::LinearContainer::SE_OnAdd);
    GSResolutionOptionsList->CreateImageLayer("GSListBackground",0,0);
    // Configure the scrollbar for the option list
    UI::VerticalScrollbar* GSResolutionOptionsScroll = GSResolutionOptions->GetListScroll();
    GSResolutionOptionsScroll->SetIncrementDistance(0.05);
    // Configure the scroller
    GSResolutionOptionsScroll->GetScroller()->CreateImageLayer("GSListScroller",0,"Normal");
    GSResolutionOptionsScroll->GetScroller()->CreateImageLayer("GSHoveredListScroller",0,"Hovered");
    // Configure the up button
    GSResolutionOptionsScroll->GetUpLeftButton()->CreateImageLayer("GSListScrollUp",0,"Normal");
    GSResolutionOptionsScroll->GetUpLeftButton()->CreateImageLayer("GSHoveredListScrollUp",0,"Hovered");
    // Configure the down button
    GSResolutionOptionsScroll->GetDownRightButton()->CreateImageLayer("GSListScrollDown",0,"Normal");
    GSResolutionOptionsScroll->GetDownRightButton()->CreateImageLayer("GSHoveredListScrollDown",0,"Hovered");
    // Configure the scroll back
    GSResolutionOptionsScroll->GetScrollBack()->CreateImageLayer("GSListScrollBackground",0,0);
    // Wrap up listing configuration
    GSResolutionList->Subscribe(UI::Widget::EventVisibilityShown,this->VideoSettingsWork->GetSettingsSubscriber());
    GSVideoSet->AddChild(GSResolutionList,11);

    // Create the checkbox for enabling or disabling fullscreen
    UI::CheckBox* GSFullscreenBox = GameScreen->CreateCheckBox("GS_FullscreenBox",UI::UnifiedRect(0.655,0.145,0,0.12));
    GSFullscreenBox->SetHorizontalSizingRules(UI::SR_Match_Other_Axis);
    GSFullscreenBox->CreateImageLayer("GSCheckboxUnchecked",0,"Normal");
    GSFullscreenBox->CreateImageLayer("GSHoveredCheckboxUnchecked",0,"Hovered");
    GSFullscreenBox->CreateImageLayer("GSCheckboxChecked",0,"SelectedNormal");
    GSFullscreenBox->CreateImageLayer("GSHoveredCheckboxChecked",0,"SelectedHovered");
    GSFullscreenBox->Subscribe(UI::Widget::EventVisibilityShown,this->VideoSettingsWork->GetSettingsSubscriber());
    GSVideoSet->AddChild(GSFullscreenBox,3);

    // Create the label for the fullscreen checkbox
    UI::Widget* GSFullscreenLabel = GameScreen->CreateWidget("GS_FullscreenLabel",UI::UnifiedRect(0.72,0.145,0.22,0.12));
    GSFullscreenLabel->CreateImageLayer("GSButton",0,0);
    UI::SingleLineTextLayer* GSFullscreenLabelText = GSFullscreenLabel->CreateSingleLineTextLayer(GameScreenText,1,1);
    GSFullscreenLabelText->SetText("Fullscreen");
    GSFullscreenLabelText->HorizontallyAlign(UI::LA_Center);
    GSFullscreenLabelText->VerticallyAlign(UI::LA_Center);
    GSFullscreenLabelText->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,GSLargeText);
    GSVideoSet->AddChild(GSFullscreenLabel,4);

    // Create the label for the FSAA options
    UI::Widget* GSFSAALabel = GameScreen->CreateWidget("GS_FSAALabel",UI::UnifiedRect(0.12,0.365,0.415,0.13));
    GSFSAALabel->CreateImageLayer("GSButton",0,0);
    UI::SingleLineTextLayer* GSFSAALabelText = GSFSAALabel->CreateSingleLineTextLayer(GameScreenText,1,1);
    GSFSAALabelText->SetText("Anti-Aliasing");
    GSFSAALabelText->HorizontallyAlign(UI::LA_Center);
    GSFSAALabelText->VerticallyAlign(UI::LA_Center);
    GSFSAALabelText->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,GSLargeText);
    GSVideoSet->AddChild(GSFSAALabel,5);

    // Create the listing of detected anti-aliasing options
    UI::DropDownList* GSFSAAList = GameScreen->CreateDropDownList("GS_FSAAList",UI::UnifiedRect(0.065,0.50,0.515,0.12),UI::SB_Separate);
    // Configure the selection display
    UI::Widget* GSFSAADisplay = GSFSAAList->GetSelectionDisplay();
    GSFSAADisplay->CreateImageLayer("GSListSelection",0,0);
    UI::SingleLineTextLayer* GSFSAADisplayText = static_cast<UI::SingleLineTextLayer*>( GSFSAADisplay->GetRenderLayer(0,UI::RLT_SingleLineText) );
    GSFSAADisplayText->SetDefaultFont(GameScreenText);
    GSFSAADisplayText->HorizontallyAlign(UI::LA_Center);
    GSFSAADisplayText->VerticallyAlign(UI::LA_Center);
    GSFSAADisplayText->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,GSLargeText);
    // Configure the list toggle
    UI::CheckBox* GSFSAAToggle = GSFSAAList->GetListToggle();
    UI::ImageLayer* GSFSAAToggleNormal = GSFSAAToggle->CreateImageLayer("GSListScrollDown");
    UI::ImageLayer* GSFSAAToggleHovered = GSFSAAToggle->CreateImageLayer("GSHoveredListScrollDown");
    GSFSAAToggle->AddLayerToGroup(GSFSAAToggleNormal,0,"Normal");
    GSFSAAToggle->AddLayerToGroup(GSFSAAToggleHovered,0,"Hovered");
    GSFSAAToggle->AddLayerToGroup(GSFSAAToggleNormal,0,"SelectedNormal");
    GSFSAAToggle->AddLayerToGroup(GSFSAAToggleHovered,0,"SelectedHovered");
    // Configure the option list
    UI::ListBox* GSFSAAOptions = GSFSAAList->GetSelectionList();
    GSFSAAOptions->SetListItemFont(GameScreenText);
    // Configure the background for the listbox container
    UI::VerticalContainer* GSFSAAOptionsList = GSFSAAOptions->GetListContainer();
    GSFSAAOptionsList->SetChildSizing(UI::SizingInfo(UI::UnifiedVec2(1.0,0.19)),UI::LinearContainer::SE_OnAdd);
    GSFSAAOptionsList->CreateImageLayer("GSListBackground",0,0);
    // Configure the scrollbar for the option list
    UI::VerticalScrollbar* GSFSAAOptionsScroll = GSFSAAOptions->GetListScroll();
    GSFSAAOptionsScroll->SetIncrementDistance(0.05);
    // Configure the scroller
    GSFSAAOptionsScroll->GetScroller()->CreateImageLayer("GSListScroller",0,"Normal");
    GSFSAAOptionsScroll->GetScroller()->CreateImageLayer("GSHoveredListScroller",0,"Hovered");
    // Configure the up button
    GSFSAAOptionsScroll->GetUpLeftButton()->CreateImageLayer("GSListScrollUp",0,"Normal");
    GSFSAAOptionsScroll->GetUpLeftButton()->CreateImageLayer("GSHoveredListScrollUp",0,"Hovered");
    // Configure the down button
    GSFSAAOptionsScroll->GetDownRightButton()->CreateImageLayer("GSListScrollDown",0,"Normal");
    GSFSAAOptionsScroll->GetDownRightButton()->CreateImageLayer("GSHoveredListScrollDown",0,"Hovered");
    // Configure the scroll back
    GSFSAAOptionsScroll->GetScrollBack()->CreateImageLayer("GSListScrollBackground",0,0);
    // Wrap up listing configuration
    GSFSAAList->Subscribe(UI::Widget::EventVisibilityShown,this->VideoSettingsWork->GetSettingsSubscriber());
    GSVideoSet->AddChild(GSFSAAList,10);

    // Create the checkbox for enabling or disabling FPS stats display
    UI::CheckBox* GSStatsBox = GameScreen->CreateCheckBox("GS_StatsBox",UI::UnifiedRect(0.655,0.50,0,0.12));
    GSStatsBox->SetHorizontalSizingRules(UI::SR_Match_Other_Axis);
    GSStatsBox->CreateImageLayer("GSCheckboxUnchecked",0,"Normal");
    GSStatsBox->CreateImageLayer("GSHoveredCheckboxUnchecked",0,"Hovered");
    GSStatsBox->CreateImageLayer("GSCheckboxChecked",0,"SelectedNormal");
    GSStatsBox->CreateImageLayer("GSHoveredCheckboxChecked",0,"SelectedHovered");
    GSStatsBox->Subscribe(UI::Widget::EventVisibilityShown,this->VideoSettingsWork->GetSettingsSubscriber());
    GSVideoSet->AddChild(GSStatsBox,7);

    // Create the label for the FPS stats display checkbox
    UI::Widget* GSStatsLabel = GameScreen->CreateWidget("GS_StatsLabel",UI::UnifiedRect(0.72,0.50,0.22,0.12));
    GSStatsLabel->CreateImageLayer("GSButton",0,0);
    UI::SingleLineTextLayer* GSStatsLabelText = GSStatsLabel->CreateSingleLineTextLayer(GameScreenText,1,1);
    GSStatsLabelText->SetText("Show FPS");
    GSStatsLabelText->HorizontallyAlign(UI::LA_Center);
    GSStatsLabelText->VerticallyAlign(UI::LA_Center);
    GSStatsLabelText->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,GSLargeText);
    GSVideoSet->AddChild(GSStatsLabel,8);

    // Create the button that will apply all of the currently displayed video settings
    UI::Button* GSVideoOptsApply = GameScreen->CreateButton("GS_VideoOptsApply",UI::UnifiedRect(0.815,0.84,0.17,0.14));
    GSVideoOptsApply->CreateImageLayer("GSOptionsApplyButton",0,"Normal");
    GSVideoOptsApply->CreateImageLayer("GSOptionsApplyHoveredButton",0,"Hovered");
    UI::SingleLineTextLayer* GSVideoOptsApplyText = GSVideoOptsApply->CreateSingleLineTextLayer(GameScreenText,1,1);
    GSVideoOptsApplyText->SetText("Apply");
    GSVideoOptsApplyText->HorizontallyAlign(UI::LA_Center);
    GSVideoOptsApplyText->VerticallyAlign(UI::LA_Center);
    GSVideoOptsApplyText->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,GSLargeText);
    GSVideoOptsApply->Subscribe(UI::Button::EventDeactivated,this->VideoSettingsWork->GetSettingsSubscriber());
    GSVideoSet->AddChild(GSVideoOptsApply,12);

    const Real GSScrollerSize = 0.09;
    // Create the TabbedSubSet that will house all our audio options
    UI::TabSet::TabbedSubSet* GSAudioSet = GSOptionsTabSet->CreateTabbedSubSet("GS_AudioSet",2);

    // Create the label for the Music volume
    UI::Widget* GSMusicVolLabel = GameScreen->CreateWidget("GS_MusicVolLabel",UI::UnifiedRect(0.0,0.01,0.40,0.13));
    GSMusicVolLabel->SetPositioningRules(UI::PF_Anchor_HorizontalCenter);
    GSMusicVolLabel->CreateImageLayer("GSButton",0,0);
    UI::SingleLineTextLayer* GSMusicVolLabelText = GSMusicVolLabel->CreateSingleLineTextLayer(GameScreenText,1,1);
    GSMusicVolLabelText->SetText("Music Volume");
    GSMusicVolLabelText->HorizontallyAlign(UI::LA_Center);
    GSMusicVolLabelText->VerticallyAlign(UI::LA_Center);
    GSMusicVolLabelText->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,GSLargeText);
    GSAudioSet->AddChild(GSMusicVolLabel,1);

    // Create the Music volume slider
    UI::HorizontalScrollbar* GSMusicVol = GameScreen->CreateHorizontalScrollbar("GS_MusicVolume",UI::UnifiedRect(0.0,0.145,0.7,0.09),UI::SB_Separate);
    GSMusicVol->SetPositioningRules(UI::PF_Anchor_HorizontalCenter);
    GSMusicVol->SetIncrementDistance(GSScrollerSize * 0.5);
    GSMusicVol->SetScrollerSize(GSScrollerSize);
    // Configure the scroller
    GSMusicVol->GetScroller()->CreateImageLayer("GSScroller",0,"Normal");
    GSMusicVol->GetScroller()->CreateImageLayer("GSHoveredScroller",0,"Hovered");
    // Configure the up button
    GSMusicVol->GetUpLeftButton()->CreateImageLayer("GSScrollLeft",0,"Normal");
    GSMusicVol->GetUpLeftButton()->CreateImageLayer("GSHoveredScrollLeft",0,"Hovered");
    // Configure the down button
    GSMusicVol->GetDownRightButton()->CreateImageLayer("GSScrollRight",0,"Normal");
    GSMusicVol->GetDownRightButton()->CreateImageLayer("GSHoveredScrollRight",0,"Hovered");
    // Configure the scroll back
    GSMusicVol->GetScrollBack()->CreateImageLayer("GSScrollBackground",0,0);
    // Wrap up Effects volume ocnfiguration
    GSMusicVol->Subscribe(UI::Scrollbar::EventScrollValueChanged,this->AudioSettingsWork->GetSettingsSubscriber());
    GSMusicVol->Subscribe(UI::Widget::EventVisibilityShown,this->AudioSettingsWork->GetSettingsSubscriber());
    GSAudioSet->AddChild(GSMusicVol,2);

    // Create the label for the Effects volume
    UI::Widget* GSEffectsVolLabel = GameScreen->CreateWidget("GS_EffectsVolLabel",UI::UnifiedRect(0.0,0.275,0.40,0.13));
    GSEffectsVolLabel->SetPositioningRules(UI::PF_Anchor_HorizontalCenter);
    GSEffectsVolLabel->CreateImageLayer("GSButton",0,0);
    UI::SingleLineTextLayer* GSEffectsVolLabelText = GSEffectsVolLabel->CreateSingleLineTextLayer(GameScreenText,1,1);
    GSEffectsVolLabelText->SetText("Effects Volume");
    GSEffectsVolLabelText->HorizontallyAlign(UI::LA_Center);
    GSEffectsVolLabelText->VerticallyAlign(UI::LA_Center);
    GSEffectsVolLabelText->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,GSLargeText);
    GSAudioSet->AddChild(GSEffectsVolLabel,3);

    // Create the Effects volume slider
    UI::HorizontalScrollbar* GSEffectsVol = GameScreen->CreateHorizontalScrollbar("GS_EffectsVolume",UI::UnifiedRect(0.0,0.41,0.7,0.09),UI::SB_Separate);
    GSEffectsVol->SetPositioningRules(UI::PF_Anchor_HorizontalCenter);
    GSEffectsVol->SetIncrementDistance(GSScrollerSize * 0.5);
    GSEffectsVol->SetScrollerSize(GSScrollerSize);
    // Configure the scroller
    GSEffectsVol->GetScroller()->CreateImageLayer("GSScroller",0,"Normal");
    GSEffectsVol->GetScroller()->CreateImageLayer("GSHoveredScroller",0,"Hovered");
    // Configure the up button
    GSEffectsVol->GetUpLeftButton()->CreateImageLayer("GSScrollLeft",0,"Normal");
    GSEffectsVol->GetUpLeftButton()->CreateImageLayer("GSHoveredScrollLeft",0,"Hovered");
    // Configure the down button
    GSEffectsVol->GetDownRightButton()->CreateImageLayer("GSScrollRight",0,"Normal");
    GSEffectsVol->GetDownRightButton()->CreateImageLayer("GSHoveredScrollRight",0,"Hovered");
    // Configure the scroll back
    GSEffectsVol->GetScrollBack()->CreateImageLayer("GSScrollBackground",0,0);
    // Wrap up Effects volume ocnfiguration
    GSEffectsVol->Subscribe(UI::Scrollbar::EventScrollValueChanged,this->AudioSettingsWork->GetSettingsSubscriber());
    GSEffectsVol->Subscribe(UI::Widget::EventVisibilityShown,this->AudioSettingsWork->GetSettingsSubscriber());
    GSAudioSet->AddChild(GSEffectsVol,2);

    /*// Create the label for the Audio Device options
    UI::Widget* GSAudioDeviceLabel = GameScreen->CreateWidget("GS_AudioDeviceLabel",UI::UnifiedRect(0.12,0.56,0.415,0.13));
    GSAudioDeviceLabel->CreateImageLayer("GSButton",0,0);
    UI::SingleLineTextLayer* GSAudioDeviceLabelText = GSAudioDeviceLabel->CreateSingleLineTextLayer(GameScreenText,1,1);
    GSAudioDeviceLabelText->SetText("Audio Device");
    GSAudioDeviceLabelText->HorizontallyAlign(UI::LA_Center);
    GSAudioDeviceLabelText->VerticallyAlign(UI::LA_Center);
    GSAudioDeviceLabelText->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,GSLargeText);
    GSAudioSet->AddChild(GSAudioDeviceLabel,3);

    // Create the listing of detected anti-aliasing options
    UI::DropDownList* GSAudioDeviceList = GameScreen->CreateDropDownList("GS_AudioDeviceList",UI::UnifiedRect(0.065,0.695,0.515,0.12),UI::SB_Separate);
    // Configure the selection display
    UI::Widget* GSAudioDeviceDisplay = GSAudioDeviceList->GetSelectionDisplay();
    GSAudioDeviceDisplay->CreateImageLayer("GSListSelection",0,0);
    UI::SingleLineTextLayer* GSAudioDeviceDisplayText = static_cast<UI::SingleLineTextLayer*>( GSAudioDeviceDisplay->GetRenderLayer(0,UI::RLT_SingleLineText) );
    GSAudioDeviceDisplayText->SetDefaultFont(GameScreenText);
    GSAudioDeviceDisplayText->HorizontallyAlign(UI::LA_Center);
    GSAudioDeviceDisplayText->VerticallyAlign(UI::LA_Center);
    GSAudioDeviceDisplayText->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,GSLargeText);
    // Configure the list toggle
    UI::CheckBox* GSAudioDeviceToggle = GSAudioDeviceList->GetListToggle();
    UI::ImageLayer* GSAudioDeviceToggleNormal = GSAudioDeviceToggle->CreateImageLayer("GSListScrollDown");
    UI::ImageLayer* GSAudioDeviceToggleHovered = GSAudioDeviceToggle->CreateImageLayer("GSHoveredListScrollDown");
    GSAudioDeviceToggle->AddLayerToGroup(GSAudioDeviceToggleNormal,0,"Normal");
    GSAudioDeviceToggle->AddLayerToGroup(GSAudioDeviceToggleHovered,0,"Hovered");
    GSAudioDeviceToggle->AddLayerToGroup(GSAudioDeviceToggleNormal,0,"SelectedNormal");
    GSAudioDeviceToggle->AddLayerToGroup(GSAudioDeviceToggleHovered,0,"SelectedHovered");
    // Configure the option list
    UI::ListBox* GSAudioDeviceOptions = GSAudioDeviceList->GetSelectionList();
    GSAudioDeviceOptions->SetListItemFont(GameScreenText);
    // Configure the background for the listbox container
    UI::VerticalContainer* GSAudioDeviceOptionsList = GSAudioDeviceOptions->GetListContainer();
    GSAudioDeviceOptionsList->CreateImageLayer("GSListBackground",0,0);
    GSAudioDeviceOptionsList->SetChildSizing(UI::SizingInfo(UI::UnifiedVec2(1.0,0.19)),UI::LinearContainer::SE_OnAdd);
    // Configure the scrollbar for the option list
    UI::VerticalScrollbar* GSAudioDeviceOptionsScroll = GSAudioDeviceOptions->GetListScroll();
    GSAudioDeviceOptionsScroll->SetIncrementDistance(0.05);
    // Configure the scroller
    GSAudioDeviceOptionsScroll->GetScroller()->CreateImageLayer("GSListScroller",0,"Normal");
    GSAudioDeviceOptionsScroll->GetScroller()->CreateImageLayer("GSHoveredListScroller",0,"Hovered");
    // Configure the up button
    GSAudioDeviceOptionsScroll->GetUpLeftButton()->CreateImageLayer("GSListScrollUp",0,"Normal");
    GSAudioDeviceOptionsScroll->GetUpLeftButton()->CreateImageLayer("GSHoveredListScrollUp",0,"Hovered");
    // Configure the down button
    GSAudioDeviceOptionsScroll->GetDownRightButton()->CreateImageLayer("GSListScrollDown",0,"Normal");
    GSAudioDeviceOptionsScroll->GetDownRightButton()->CreateImageLayer("GSHoveredListScrollDown",0,"Hovered");
    // Configure the scroll back
    GSAudioDeviceOptionsScroll->GetScrollBack()->CreateImageLayer("GSListScrollBackground",0,0);
    // Wrap up listing configuration
    GSAudioDeviceList->Subscribe(UI::Widget::EventVisibilityShown,this->AudioSettingsWork->GetSettingsSubscriber());
    GSAudioSet->AddChild(GSAudioDeviceList,10);//*/

    // Create the checkbox for enabling or disabling FPS stats display
    UI::CheckBox* GSMuteBox = GameScreen->CreateCheckBox("GS_MuteBox",UI::UnifiedRect(0.655,0.695,0,0.12));
    GSMuteBox->SetHorizontalSizingRules(UI::SR_Match_Other_Axis);
    GSMuteBox->CreateImageLayer("GSCheckboxUnchecked",0,"Normal");
    GSMuteBox->CreateImageLayer("GSHoveredCheckboxUnchecked",0,"Hovered");
    GSMuteBox->CreateImageLayer("GSCheckboxChecked",0,"SelectedNormal");
    GSMuteBox->CreateImageLayer("GSHoveredCheckboxChecked",0,"SelectedHovered");
    GSMuteBox->Subscribe(UI::CheckBox::EventSelected,this->AudioSettingsWork->GetSettingsSubscriber());
    GSMuteBox->Subscribe(UI::CheckBox::EventDeselected,this->AudioSettingsWork->GetSettingsSubscriber());
    GSMuteBox->Subscribe(UI::Widget::EventVisibilityShown,this->AudioSettingsWork->GetSettingsSubscriber());
    GSAudioSet->AddChild(GSMuteBox,5);

    // Create the label for the FPS stats display checkbox
    UI::Widget* GSMuteLabel = GameScreen->CreateWidget("GS_MuteLabel",UI::UnifiedRect(0.72,0.695,0.22,0.12));
    GSMuteLabel->CreateImageLayer("GSButton",0,0);
    UI::SingleLineTextLayer* GSMuteLabelText = GSMuteLabel->CreateSingleLineTextLayer(GameScreenText,1,1);
    GSMuteLabelText->SetText("Mute");
    GSMuteLabelText->HorizontallyAlign(UI::LA_Center);
    GSMuteLabelText->VerticallyAlign(UI::LA_Center);
    GSMuteLabelText->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,GSLargeText);
    GSAudioSet->AddChild(GSMuteLabel,6);

    // Create the back button for the options window
    UI::StackButton* GSOptsBack = GameScreen->CreateStackButton("GS_OptsBack",UI::UnifiedRect(0.780,0.870,0.156,0.094));
    GSOptsBack->CreateImageLayer("GSOptionsApplyButton",0,"Normal");
    GSOptsBack->CreateImageLayer("GSOptionsApplyHoveredButton",0,"Hovered");
    UI::SingleLineTextLayer* GSOptsBackText = GSOptsBack->CreateSingleLineTextLayer(GameScreenText,1,1);
    GSOptsBackText->SetText("Back");
    GSOptsBackText->HorizontallyAlign(UI::LA_Center);
    GSOptsBackText->VerticallyAlign(UI::LA_Center);
    GSOptsBackText->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,GSLargeText);
    GSOptionsWin->SetPopButton(GSOptsBack);
    GSOptionsWin->AddChild(GSOptsBack,5);

    // Second button Y placement: 26

    // Create the game restart button
    UI::Button* GSMenuRestart = GameScreen->CreateButton("GS_MenuRestart",UI::UnifiedRect(0.15,0.445,0.70,0.11));
    GSMenuRestart->CreateImageLayer("GSOptionsButton",0,"Normal");
    GSMenuRestart->CreateImageLayer("GSOptionsHoveredButton",0,"Hovered");
    UI::SingleLineTextLayer* GSMenuRestartText = GSMenuRestart->CreateSingleLineTextLayer(GameScreenText,1,1);
    GSMenuRestartText->SetText("Restart Level");
    GSMenuRestartText->HorizontallyAlign(UI::LA_Center);
    GSMenuRestartText->VerticallyAlign(UI::LA_Center);
    GSMenuRestartText->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,GSNormText);
    GSMenuRoot->AddChild(GSMenuRestart,3);

    // Create the game return button
    UI::StackButton* GSMenuReturn = GameScreen->CreateStackButton("GS_MenuReturn",UI::UnifiedRect(0.15,0.63,0.70,0.11));
    GSMenuReturn->CreateImageLayer("GSOptionsButton",0,"Hovered");
    GSMenuReturn->CreateImageLayer("GSOptionsHoveredButton",0,"Normal");
    UI::SingleLineTextLayer* GSMenuReturnText = GSMenuReturn->CreateSingleLineTextLayer(GameScreenText,1,1);
    GSMenuReturnText->SetText("Return to Game");
    GSMenuReturnText->HorizontallyAlign(UI::LA_Center);
    GSMenuReturnText->VerticallyAlign(UI::LA_Center);
    GSMenuReturnText->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,GSNormText);
    GSMenuRoot->SetPopButton(GSMenuReturn);
    GSMenuRoot->AddChild(GSMenuReturn,4);

    // Create the game restart button
    UI::Button* GSMenuExit = GameScreen->CreateButton("GS_MenuExit",UI::UnifiedRect(0.15,0.815,0.70,0.11));
    GSMenuExit->CreateImageLayer("GSOptionsButton",0,"Normal");
    GSMenuExit->CreateImageLayer("GSOptionsHoveredButton",0,"Hovered");
    UI::SingleLineTextLayer* GSMenuExitText = GSMenuExit->CreateSingleLineTextLayer(GameScreenText,1,1);
    GSMenuExitText->SetText("Exit to Main Menu");
    GSMenuExitText->HorizontallyAlign(UI::LA_Center);
    GSMenuExitText->VerticallyAlign(UI::LA_Center);
    GSMenuExitText->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,GSNormText);
    GSMenuRoot->AddChild(GSMenuExit,5);

    ////-----------------  Item Shop Menu  -----------------////
    // Create the root entry for the item shop
    UI::MenuEntry* GSItemShopRoot = GameScreen->CreateMenuEntry("GS_ItemShopRoot",UI::UnifiedRect(0.25,0.11,0.5,0.78125));
    GSItemShopRoot->SetAspectRatioLock(UI::ARL_Ratio_Y_Axis);
    GSItemShopRoot->CreateImageLayer("GSStoreBackground",0,0);
    GSItemShopRoot->SetPushButton(GSItemShopAccess);
    GameScreen->AddChild(GSItemShopRoot,8);

    // Create the "titlebar" for the item shop window
    UI::Widget* GSItemShopTitle = GameScreen->CreateWidget("GS_ItemShopTitle",UI::UnifiedRect(0.0,0.0,1.0,0.077));
    GSItemShopTitle->SetPositioningRules(UI::PF_Anchor_HorizontalCenter | UI::PF_Anchor_Top);
    UI::SingleLineTextLayer* GSItemShopTitleText = GSItemShopTitle->CreateSingleLineTextLayer(GameScreenText,1,1);
    GSItemShopTitleText->SetText("Item Shop");
    GSItemShopTitleText->HorizontallyAlign(UI::LA_Center);
    GSItemShopTitleText->VerticallyAlign(UI::LA_Center);
    GSItemShopTitleText->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,GSNormText);
    GSItemShopRoot->AddChild(GSItemShopTitle,1);

    // Create the grid that will store all of our purchasable items
    UI::GridContainer* GSItemShopGrid = GameScreen->CreateGridContainer("GS_ItemShopGrid",UI::UnifiedRect(0.15,0.08,0.7,0.33));
    GSItemShopGrid->SetCellSize(4,3);
    GSItemShopRoot->AddChild(GSItemShopGrid,2);

    // Create the spinner that will determine which items will be displayed
    UI::Spinner* GSItemShopSpinner = GameScreen->CreateSpinner("GS_ItemShopSpinner",UI::UnifiedRect(0.0,0.43,0.25,0.05),UI::Spn_Separate_Horizontal,GameScreenText);
    GSItemShopSpinner->SetPositioningRules(UI::PF_Anchor_HorizontalCenter);
    GSItemShopSpinner->GetIncrement()->CreateImageLayer("GSIncrementPage",0,0);
    GSItemShopSpinner->GetDecrement()->CreateImageLayer("GSDecrementPage",0,0);
    GSItemShopSpinner->GetValueDisplay()->CreateImageLayer("GSPageBox",0,0);
    GSItemShopGrid->SetProviders(GSItemShopSpinner,GSItemShopSpinner);
    GSItemShopRoot->AddChild(GSItemShopSpinner,3);

    UI::Widget* GSItemShopDescription = GameScreen->CreateWidget("GS_ItemShopDescription",UI::UnifiedRect(0.15,0.5,0.7,0.33));
    GSItemShopDescription->CreateImageLayer("GSBreakdownBackground",0,0);
    UI::MultiLineTextLayer* GSItemShopDescriptionText = GSItemShopDescription->CreateMultiLineTextLayer(GameScreenText,1,1);
    GSItemShopDescriptionText->SetText("This is a more exhaustive\ntest of the multi-line\ntext layer.");
    GSItemShopDescriptionText->SetTextLineHorizontalAlignment(UI::LA_Center);
    GSItemShopDescriptionText->SetTextLineVerticalAlignment(UI::LA_Center);
    GSItemShopDescriptionText->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,GSMultiLineText);
    GSItemShopRoot->AddChild(GSItemShopDescription,4);

    UI::Widget* GSItemShopMoney = GameScreen->CreateWidget("GS_ItemShopMoney",UI::UnifiedRect(0.15,0.87,0.20,0.09));
    GSItemShopMoney->CreateImageLayer("GSStoreButton",0,0);
    UI::SingleLineTextLayer* GSItemShopMoneyText = GSItemShopMoney->CreateSingleLineTextLayer(GameScreenText,1,1);
    GSItemShopMoneyText->SetText("$0");
    GSItemShopMoneyText->HorizontallyAlign(UI::LA_Center);
    GSItemShopMoneyText->VerticallyAlign(UI::LA_Center);
    GSItemShopMoneyText->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,GSLargeText);
    GSItemShopRoot->AddChild(GSItemShopMoney,5);

    UI::Button* GSItemShopBuy = GameScreen->CreateButton("GS_ItemShopBuy",UI::UnifiedRect(0.40,0.87,0.20,0.09));
    GSItemShopBuy->CreateImageLayer("GSStoreButton",0,"Normal");
    GSItemShopBuy->CreateImageLayer("GSStoreHoveredButton",0,"Hovered");
    UI::SingleLineTextLayer* GSItemShopBuyText = GSItemShopBuy->CreateSingleLineTextLayer(GameScreenText,1,1);
    GSItemShopBuyText->SetText("Buy");
    GSItemShopBuyText->HorizontallyAlign(UI::LA_Center);
    GSItemShopBuyText->VerticallyAlign(UI::LA_Center);
    GSItemShopBuyText->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,GSLargeText);
    GSItemShopRoot->AddChild(GSItemShopBuy,6);

    UI::StackButton* GSItemShopBack = GameScreen->CreateStackButton("GS_ItemShopBack",UI::UnifiedRect(0.65,0.87,0.20,0.09));
    GSItemShopBack->CreateImageLayer("GSStoreButton",0,"Normal");
    GSItemShopBack->CreateImageLayer("GSStoreHoveredButton",0,"Hovered");
    UI::SingleLineTextLayer* GSItemShopBackText = GSItemShopBack->CreateSingleLineTextLayer(GameScreenText,1,1);
    GSItemShopBackText->SetText("Back");
    GSItemShopBackText->HorizontallyAlign(UI::LA_Center);
    GSItemShopBackText->VerticallyAlign(UI::LA_Center);
    GSItemShopBackText->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,GSLargeText);
    GSItemShopRoot->SetPopButton(GSItemShopBack);
    GSItemShopRoot->AddChild(GSItemShopBack,7);

    ////-------------------  End Report  -------------------////
    // Create the "window" that will display the level report
    UI::Widget* GSLevelReport = GameScreen->CreateWidget("GS_LevelReport",UI::UnifiedRect(0.18,0.18,0.64,0.64));
    GSLevelReport->SetAspectRatioLock(UI::ARL_Ratio_Y_Axis);
    GSLevelReport->CreateImageLayer("GSOptionsBackground",0,0);
    GSLevelReport->Hide();
    GameScreen->AddChild(GSLevelReport,9);

    // Create the total score display
    UI::Widget* GSLevelReportScore = GameScreen->CreateWidget("GS_LevelReportScore",UI::UnifiedRect(0.0,0.08,0.34,0.125));
    GSLevelReportScore->SetPositioningRules(UI::PF_Anchor_HorizontalCenter);
    GSLevelReportScore->CreateImageLayer("GSAppExitButton",0,0);
    UI::SingleLineTextLayer* GSLevelReportScoreText = GSLevelReportScore->CreateSingleLineTextLayer(GameScreenText,1,1);
    GSLevelReportScoreText->SetText("0000");
    GSLevelReportScoreText->HorizontallyAlign(UI::LA_Center);
    GSLevelReportScoreText->VerticallyAlign(UI::LA_Center);
    GSLevelReportScoreText->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,GSLargeText);
    GSLevelReport->AddChild(GSLevelReportScore,1);

    UI::VerticalContainer* GSLevelReportBreakdown = GameScreen->CreateVerticalContainer("GS_LevelReportBreakdown",UI::UnifiedRect(0.15,0.245,0.70,0.52));
    GSLevelReportBreakdown->SetPadding(UI::UnifiedDim(0.05,0.0));
    GSLevelReportBreakdown->CreateImageLayer("GSBreakdownBackground",0,0);
    GSLevelReport->AddChild(GSLevelReportBreakdown,2);

    UI::Button* GSLevelReportFinish = GameScreen->CreateButton("GS_LevelReportFinish",UI::UnifiedRect(0.21,0.815,0.25,0.125));
    GSLevelReportFinish->CreateImageLayer("GSStoreButton",0,"Normal");
    GSLevelReportFinish->CreateImageLayer("GSStoreHoveredButton",0,"Hovered");
    UI::SingleLineTextLayer* GSLevelReportFinishText = GSLevelReportFinish->CreateSingleLineTextLayer(GameScreenText,1,1);
    GSLevelReportFinishText->SetText("Finish");
    GSLevelReportFinishText->HorizontallyAlign(UI::LA_Center);
    GSLevelReportFinishText->VerticallyAlign(UI::LA_Center);
    GSLevelReportFinishText->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,GSNormText);
    GSLevelReport->AddChild(GSLevelReportFinish,3);

    UI::Button* GSLevelReportRetry = GameScreen->CreateButton("GS_LevelReportRetry",UI::UnifiedRect(0.54,0.815,0.25,0.125));
    GSLevelReportRetry->CreateImageLayer("GSStoreButton",0,"Normal");
    GSLevelReportRetry->CreateImageLayer("GSStoreHoveredButton",0,"Normal");
    UI::SingleLineTextLayer* GSLevelReportRetryText = GSLevelReportRetry->CreateSingleLineTextLayer(GameScreenText,1,1);
    GSLevelReportRetryText->SetText("Retry");
    GSLevelReportRetryText->HorizontallyAlign(UI::LA_Center);
    GSLevelReportRetryText->VerticallyAlign(UI::LA_Center);
    GSLevelReportRetryText->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,GSNormText);
    GSLevelReport->AddChild(GSLevelReportRetry,4);
}

void CatchApp::CreateLoadingScreen()
{
    UI::UIManager* GUI = UI::UIManager::GetSingletonPtr();
    Graphics::GraphicsManager* GraphicsMan = Graphics::GraphicsManager::GetSingletonPtr();

    GUI->LoadMTA("Catch_Loading.mta","Common");
    Graphics::Viewport* UIViewport = GraphicsMan->GetGameWindow(0)->GetViewport(0);
    UIViewport->SetCamera(this->TheEntresol->GetCameraManager()->CreateCamera("Main"));

    UI::Screen* LoadScreen = GUI->CreateScreen("LoadingScreen","Catch_Loading",UIViewport,9);
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
    UI::Screen* StatsScreen = UIMan->GetScreen("StatsScreen");
    switch(StateToSet)
    {
        case CatchApp::Catch_GameScreen:
        {
            UIMan->HideAllScreens(StatsScreen);
            UI::Screen* CurrScreen = UIMan->GetScreen("GameScreen");
            CurrScreen->Show();
            break;
        }
        case CatchApp::Catch_Loading:
        {
            UIMan->HideAllScreens(StatsScreen);
            UI::Screen* CurrScreen = UIMan->GetScreen("LoadingScreen");
            CurrScreen->Show();
            break;
        }
        case CatchApp::Catch_MenuScreen:
        {
            UIMan->HideAllScreens(StatsScreen);
            UI::Screen* CurrScreen = UIMan->GetScreen("MainMenuScreen");
            CurrScreen->Show();
            break;
        }
        case CatchApp::Catch_ScoreScreen:
        {
            this->PauseGame(true);
            Whole LevelScore = this->Scorer->PresentFinalScore();
            if( LevelScore > this->Profiles->GetActiveProfile()->GetHighestScore( this->LevelMan->GetCurrentLevel()->GetName() ) )
            {
                this->Profiles->GetActiveProfile()->SetNewHighScore(this->LevelMan->GetCurrentLevel()->GetName(),LevelScore);
                /// @todo UI Update
                //(static_cast<LevelSelectCell*>(this->Profiles->GetLevelGrid()->GetCell(LevelMan->GetCurrentLevel())))->GetEarnedScore()->SetText(StringTools::ConvertToString(LevelScore));
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
    if( "MainMenu" == LevelMan->GetCurrentLevel()->GetName() )
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

    ResMan->DestroyAssetGroup(LevelMan->GetCurrentLevel()->GetGroupName());
    PhysMan->ClearPhysicsMetaData();
    Scorer->ResetLevelData();
    delete EndTimer;
    EndTimer = NULL;

    UI::Screen* GameScreen = UIMan->GetScreen("GameScreen");
    GameScreen->GetWidget("GS_LevelReport")->Hide();
    GameScreen->GetWidget("GS_MenuRoot")->Hide();
    GameScreen->GetWidget("GS_ItemShopRoot")->Hide();
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
    this->AudioSettingsWork = new AudioSettingsWorkUnit(this->TheEntresol->GetUIManager(),this->TheEntresol->GetAudioManager());
    this->AudioSettingsWork->AddDependency( this->TheEntresol->GetUIManager()->GetWidgetUpdateWork() );
    this->TheEntresol->GetAudioManager()->GetBufferUpdate2DWork()->AddDependency( this->AudioSettingsWork );
    Audio::SoundScapeManager* SoundScapeMan = this->TheEntresol->GetSoundScapeManager();
    if( SoundScapeMan != NULL ) {
        SoundScapeMan->GetBufferUpdate3DWork()->AddDependency( this->AudioSettingsWork );
    }
    this->TheEntresol->GetScheduler().AddWorkUnitMain( this->AudioSettingsWork, "AudioSettingsWork" );

    this->VideoSettingsWork = new VideoSettingsWorkUnit(this->TheEntresol->GetUIManager(),this->TheEntresol->GetGraphicsManager());
    this->VideoSettingsWork->AddDependency( this->TheEntresol->GetUIManager()->GetWidgetUpdateWork() );
    // Add a line here setting the graphics monopoly as a dependency?
    this->TheEntresol->GetScheduler().AddWorkUnitAffinity( this->VideoSettingsWork, "VideoSettingsWork" );

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
    this->LevelMan->SetNextLevel("MainMenu");
    do{
        this->ChangeState(CatchApp::Catch_Loading);
        this->PauseGame(false);
        Graphics::GraphicsManager::GetSingletonPtr()->RenderOneFrame();
        //Actually Load the game stuff
        this->LevelMan->LoadNextLevel();

        if( "MainMenu" == LevelMan->GetCurrentLevel()->GetName() )
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
    }while( this->LevelMan->HasALevelToLoad() );

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

AudioSettingsWorkUnit* CatchApp::GetAudioSettingsWork() const
    { return this->AudioSettingsWork; }

VideoSettingsWorkUnit* CatchApp::GetVideoSettingsWork() const
    { return this->VideoSettingsWork; }

CatchPreInputWorkUnit* CatchApp::GetPreInputWork() const
    { return this->PreInputWork; }

CatchPostInputWorkUnit* CatchApp::GetPostInputWork() const
    { return this->PostInputWork; }

CatchPostUIWorkUnit* CatchApp::GetPostUIWork() const
    { return this->PostUIWork; }

CatchPostGraphicsWorkUnit* CatchApp::GetPostGraphicsWork() const
    { return this->PostGraphicsWork; }

Entresol* CatchApp::GetTheEntresol() const
    { return this->TheEntresol; }

CatchApp::ThrowableContainer& CatchApp::GetThrowables()
    { return this->ThrownItems; }

LevelManager* CatchApp::GetLevelManager() const
    { return this->LevelMan; }

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
