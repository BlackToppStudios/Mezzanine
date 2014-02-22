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

    GUI->EnableButtonAutoRegister(true);
    GUI->AddAutoRegisterCode( Input::MetaCode( Input::BUTTON_LIFTING, Input::MOUSEBUTTON_1 ) );

    const ColourValue Transparent(0.0,0.0,0.0,0.0);
    const ColourValue TransBlack(0.0,0.0,0.0,0.85);
    const ColourValue Black(0.0,0.0,0.0,1.0);
    const ColourValue Gray(0.2,0.2,0.2,1.0);

    ////////////////////////////////////////////////////////////////
    ////------------------  Main Menu Screen  ------------------////
    ////////////////////////////////////////////////////////////////

    const Real MMRoomyText = 0.65;
    const Real MMTightText = 0.8;
    //const Real MMScreenScaleText = 0.04;

    GUI->LoadMTA("Catch_Menu.mta","Common");
    UI::Screen* MainMenuScreen = GUI->CreateScreen("MainMenuScreen","Catch_Menu",UIViewport);

    ////-----------------  Main Menu Root  -----------------////
    // Create the widget that will hold the background and lock it at a wide screen size
    UI::Widget* MMBackground = MainMenuScreen->CreateWidget("MS_Background",UI::UnifiedRect(0,0,1.7777,1));
    MMBackground->SetPositioningRules(UI::PF_Anchor_Center);
    MMBackground->SetVerticalSizingRules(UI::SR_Unified_Dims);
    MMBackground->SetHorizontalSizingRules(UI::SR_Match_Other_Axis_Unified);
    UI::ImageLayer* MMBackgroundLayer = MMBackground->CreateImageLayer("MainMenuBackground");
    MMBackground->AddLayerToGroup(MMBackgroundLayer,0,"Normal");
    MMBackground->AddLayerToGroup(MMBackgroundLayer,0,"Hovered");
    MainMenuScreen->AddChild(MMBackground,0);

    // Create the root menu entry
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
    MMLevelSelectAccessText->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,MMRoomyText);
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

    /*UI::PagedCellGrid* MMLevelSelectGrid = MMLevelSelectWin->CreatePagedCellGrid("MS_LevelGrid", UI::RenderableRect(Vector2(0.14,0.14), Vector2(0.72,0.66), true), UI::RenderableRect(Vector2(0.60,0.85), Vector2(0.24,0.06), true), UI::Spn_Separate, 0.05);
    MMLevelSelectGrid->GetGridBack()->SetBackgroundColour(Transparent);
    MMLevelSelectGrid->GetPageSpinner()->GetIncrement()->GetClickable()->SetBackgroundSprite("MMIncrementPage");
    MMLevelSelectGrid->GetPageSpinner()->GetDecrement()->GetClickable()->SetBackgroundSprite("MMDecrementPage");
    MMLevelSelectGrid->GetPageSpinner()->GetValueDisplay()->SetBackgroundSprite("MMPageBox");
    UI::Button* MMLevelStart = MMLevelSelectWin->CreateButton("MS_LevelStart", UI::RenderableRect(Vector2(0.42,0.85), Vector2(0.16,0.07), true),MMStartLineHeight,"Start");
    MMLevelStart->AddActivatableListener(new MSStart(MMLevelSelectGrid));
    MMLevelStart->GetClickable()->SetBackgroundSprite("MMLevelStart");
    MMLevelStart->GetClickable()->SetHoveredSprite("MMLevelStartHovered");//*/

    ////------------------  Options  ------------------////
    // Start with the accessor button
    UI::StackButton* MMOptionsAccess = MainMenuScreen->CreateStackButton("MS_Options",UI::UnifiedRect(0.28,0.18,0.22,0.7));
    UI::ImageLayer* MMOptionsAccessNormal = MMOptionsAccess->CreateImageLayer("MMButton");
    UI::ImageLayer* MMOptionsAccessHovered = MMOptionsAccess->CreateImageLayer("MMHoveredButton");
    UI::SingleLineTextLayer* MMOptionsAccessText = MMOptionsAccess->CreateSingleLineTextLayer("Ubuntu-14");
    MMOptionsAccessText->SetText("Options");
    MMOptionsAccessText->HorizontallyAlign(UI::LA_Center);
    MMOptionsAccessText->VerticallyAlign(UI::LA_Center);
    MMOptionsAccessText->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,MMRoomyText);
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
    MMVideoSetAccessText->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,MMRoomyText);
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
    MMAudioSetAccessText->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,MMRoomyText);
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
    MMResolutionLabelText->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,MMTightText);
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
    MMResolutionDisplayText->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,MMTightText);
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
    UI::ImageLayer* MMResolutionOptionsScrollBackLayer = MMResolutionOptionsScrollBack->CreateImageLayer("MMListScrollBackground");
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
    MMFullscreenLabelText->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,MMTightText);
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
    MMFSAALabelText->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,MMTightText);
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
    MMFSAADisplayText->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,MMTightText);
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
    UI::ImageLayer* MMFSAAOptionsScrollBackLayer = MMFSAAOptionsScrollBack->CreateImageLayer("MMListScrollBackground");
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
    MMStatsLabelText->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,MMTightText);
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
    MMVideoOptsApplyText->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,MMTightText);
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
    MMMusicVolLabelText->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,MMTightText);
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
    MMEffectsVolLabelText->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,MMTightText);
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
    MMAudioDeviceLabelText->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,MMTightText);
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
    MMAudioDeviceDisplayText->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,MMTightText);
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
    UI::ImageLayer* MMAudioDeviceOptionsScrollBackLayer = MMAudioDeviceOptionsScrollBack->CreateImageLayer("MMListScrollBackground");
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
    MMMuteLabelText->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,MMTightText);
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
    MMOptsBackText->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,MMTightText);
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
    MMCreditsAccessText->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,MMRoomyText);
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
    MMAppExitAccessText->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,MMRoomyText);
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
    MMAppExitWarnText->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,MMRoomyText);
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
    MMAppExitConfText->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,MMRoomyText);
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
    MMAppExitDenyText->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,MMRoomyText);
    MMAppExitDeny->AddLayerToGroup(MMAppExitDenyNormal,0,"Normal");
    MMAppExitDeny->AddLayerToGroup(MMAppExitDenyText,1,"Normal");
    MMAppExitDeny->AddLayerToGroup(MMAppExitDenyHovered,0,"Hovered");
    MMAppExitDeny->AddLayerToGroup(MMAppExitDenyText,1,"Hovered");
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
    UI::Screen* GameScreen = GUI->CreateScreen("GameScreen","Catch_Game",UIViewport);

    ////----------------  Heads Up Display  ----------------////
    // Create the widget container for the score display
    UI::Widget* GSScore = GameScreen->CreateWidget("GS_Score",UI::UnifiedRect(0.008,0.006,0.27,0.065));
    GSScore->SetAspectRatioLock(UI::ARL_Ratio_Y_Axis);
    // Create the widget for displaying the timer logo
    UI::Widget* GSScoreLabel = GameScreen->CreateWidget("GS_ScoreLabel",UI::UnifiedRect(0.0,0.0,0.45,1.0));
    GSScoreLabel->SetPositioningRules(UI::PF_Anchor_TopLeft);
    UI::ImageLayer* GSScoreLabelLayer = GSScoreLabel->CreateImageLayer("GSScoreTextArea");
    GSScoreLabel->AddLayerToGroup(GSScoreLabelLayer,0,"Normal");
    GSScoreLabel->AddLayerToGroup(GSScoreLabelLayer,0,"Hovered");
    GSScore->AddChild(GSScoreLabel,1);
    // Create the widget for displaying the timer value
    UI::Widget* GSScoreValue = GameScreen->CreateWidget("GS_ScoreValue",UI::UnifiedRect(0.0,0.0,0.55,1.0));
    GSScoreValue->SetPositioningRules(UI::PF_Anchor_TopRight);
    UI::ImageLayer* GSScoreValueLayer = GSScoreValue->CreateImageLayer("GSScoreValueArea");
    UI::SingleLineTextLayer* GSScoreValueText = GSScoreValue->CreateSingleLineTextLayer(GameScreenText);
    GSScoreValueText->SetText("0:00");
    GSScoreValueText->HorizontallyAlign(UI::LA_Center);
    GSScoreValueText->VerticallyAlign(UI::LA_Center);
    GSScoreValueText->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,GSNormText);
    GSScoreValue->AddLayerToGroup(GSScoreValueLayer,0,"Normal");
    GSScoreValue->AddLayerToGroup(GSScoreValueText,1,"Normal");
    GSScoreValue->AddLayerToGroup(GSScoreValueLayer,0,"Hovered");
    GSScoreValue->AddLayerToGroup(GSScoreValueText,1,"Hovered");
    GSScore->AddChild(GSScoreValue,2);
    // Wrap up timer configuration
    GameScreen->AddChild(GSScore,1);

    // Create the widget container for the timer display
    UI::Widget* GSTimer = GameScreen->CreateWidget("GS_Timer",UI::UnifiedRect(0.8355,0.006,0.16,0.065));
    GSTimer->SetAspectRatioLock(UI::ARL_Ratio_Y_Axis);
    // Create the widget for displaying the timer logo
    UI::Widget* GSTimerLogo = GameScreen->CreateWidget("GS_TimerLogo",UI::UnifiedRect(0.0,0.0,0.66,1.0));
    GSTimerLogo->SetPositioningRules(UI::PF_Anchor_TopLeft);
    UI::ImageLayer* GSTimerLogoLayer = GSTimerLogo->CreateImageLayer("GSTimerLogo");
    GSTimerLogo->AddLayerToGroup(GSTimerLogoLayer,0,"Normal");
    GSTimerLogo->AddLayerToGroup(GSTimerLogoLayer,0,"Hovered");
    GSTimer->AddChild(GSTimerLogo,1);
    // Create the widget for displaying the timer value
    UI::Widget* GSTimerValue = GameScreen->CreateWidget("GS_TimerValue",UI::UnifiedRect(0.0,0.0,0.34,1.0));
    GSTimerValue->SetPositioningRules(UI::PF_Anchor_TopRight);
    UI::ImageLayer* GSTimerValueLayer = GSTimerValue->CreateImageLayer("GSTimerArea");
    UI::SingleLineTextLayer* GSTimerValueText = GSTimerValue->CreateSingleLineTextLayer(GameScreenText);
    GSTimerValueText->SetText("0");
    GSTimerValueText->HorizontallyAlign(UI::LA_Center);
    GSTimerValueText->VerticallyAlign(UI::LA_Center);
    GSTimerValueText->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,GSNormText);
    GSTimerValue->AddLayerToGroup(GSTimerValueLayer,0,"Normal");
    GSTimerValue->AddLayerToGroup(GSTimerValueText,1,"Normal");
    GSTimerValue->AddLayerToGroup(GSTimerValueLayer,0,"Hovered");
    GSTimerValue->AddLayerToGroup(GSTimerValueText,1,"Hovered");
    GSTimer->AddChild(GSTimerValue,2);
    // Wrap up timer configuration
    GameScreen->AddChild(GSTimer,2);

    // Create the menu button
    UI::StackButton* GSMenuAccess = GameScreen->CreateStackButton("GS_MenuAccess",UI::UnifiedRect(0.008,0.935,0.16,0.065));
    GSMenuAccess->SetAspectRatioLock(UI::ARL_Ratio_Y_Axis);
    UI::ImageLayer* GSMenuAccessNormal = GSMenuAccess->CreateImageLayer("GSMenuStoreButton");
    UI::ImageLayer* GSMenuAccessHovered = GSMenuAccess->CreateImageLayer("GSMenuStoreHoveredButton");
    UI::SingleLineTextLayer* GSMenuAccessText = GSMenuAccess->CreateSingleLineTextLayer(GameScreenText);
    GSMenuAccessText->SetText("Menu");
    GSMenuAccessText->HorizontallyAlign(UI::LA_Center);
    GSMenuAccessText->VerticallyAlign(UI::LA_Center);
    GSMenuAccessText->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,GSNormText);
    GSMenuAccess->AddLayerToGroup(GSMenuAccessNormal,0,"Normal");
    GSMenuAccess->AddLayerToGroup(GSMenuAccessText,1,"Normal");
    GSMenuAccess->AddLayerToGroup(GSMenuAccessHovered,0,"Hovered");
    GSMenuAccess->AddLayerToGroup(GSMenuAccessText,1,"Hovered");
    GameScreen->AddChild(GSMenuAccess,3);

    // Create the item shop button
    UI::StackButton* GSItemShopAccess = GameScreen->CreateStackButton("GS_ItemShopAccess",UI::UnifiedRect(0.822,0.935,0.16,0.065));
    GSItemShopAccess->SetAspectRatioLock(UI::ARL_Ratio_Y_Axis);
    UI::ImageLayer* GSItemShopAccessNormal = GSItemShopAccess->CreateImageLayer("GSMenuStoreButton");
    UI::ImageLayer* GSItemShopAccessHovered = GSItemShopAccess->CreateImageLayer("GSMenuStoreHoveredButton");
    UI::SingleLineTextLayer* GSItemShopAccessText = GSItemShopAccess->CreateSingleLineTextLayer(GameScreenText);
    GSItemShopAccessText->SetText("Store");
    GSItemShopAccessText->HorizontallyAlign(UI::LA_Center);
    GSItemShopAccessText->VerticallyAlign(UI::LA_Center);
    GSItemShopAccessText->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,GSNormText);
    GSItemShopAccess->AddLayerToGroup(GSItemShopAccessNormal,0,"Normal");
    GSItemShopAccess->AddLayerToGroup(GSItemShopAccessText,1,"Normal");
    GSItemShopAccess->AddLayerToGroup(GSItemShopAccessHovered,0,"Hovered");
    GSItemShopAccess->AddLayerToGroup(GSItemShopAccessText,1,"Hovered");
    GameScreen->AddChild(GSItemShopAccess,4);

    ////-------------------  Game  Menu  -------------------////
    // Create the root entry for the game menu
    UI::MenuEntry* GSMenuRoot = GameScreen->CreateMenuEntry("GS_MenuRoot",UI::UnifiedRect(0.35,0.27,0.3,0.45));
    GSMenuRoot->SetAspectRatioLock(UI::ARL_Ratio_Y_Axis);
    UI::ImageLayer* GSMenuRootLayer = GSMenuRoot->CreateImageLayer("GSOptionsMenuBackground");
    GSMenuRoot->AddLayerToGroup(GSMenuRootLayer,0,"Normal");
    GSMenuRoot->AddLayerToGroup(GSMenuRootLayer,0,"Hovered");
    GSMenuRoot->SetPushButton(GSMenuAccess);
    GameScreen->AddChild(GSMenuRoot,10);

    // Create the options accessor button
    UI::StackButton* GSOptionsAccess = GameScreen->CreateStackButton("GS_Options",UI::UnifiedRect(0.15,0.075,0.70,0.11));
    UI::ImageLayer* GSOptionsAccessNormal = GSOptionsAccess->CreateImageLayer("GSOptionsButton");
    UI::ImageLayer* GSOptionsAccessHovered = GSOptionsAccess->CreateImageLayer("GSOptionsHoveredButton");
    UI::SingleLineTextLayer* GSOptionsAccessText = GSOptionsAccess->CreateSingleLineTextLayer(GameScreenText);
    GSOptionsAccessText->SetText("Options");
    GSOptionsAccessText->HorizontallyAlign(UI::LA_Center);
    GSOptionsAccessText->VerticallyAlign(UI::LA_Center);
    GSOptionsAccessText->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,GSNormText);
    GSOptionsAccess->AddLayerToGroup(GSOptionsAccessNormal,0,"Normal");
    GSOptionsAccess->AddLayerToGroup(GSOptionsAccessText,1,"Normal");
    GSOptionsAccess->AddLayerToGroup(GSOptionsAccessHovered,0,"Hovered");
    GSOptionsAccess->AddLayerToGroup(GSOptionsAccessText,1,"Hovered");
    GSMenuRoot->AddChild(GSOptionsAccess,1);

    // Create and configure the "window" that houses the options display
    UI::MenuEntry* GSOptionsWin = GameScreen->CreateMenuEntry("GS_OptionsWin",UI::UnifiedRect(0.18,-8.08,0.64,6.4));
    UI::ImageLayer* GSOptionsWinLayer = GSOptionsWin->CreateImageLayer("GSOptionsBackground");
    GSOptionsWin->AddLayerToGroup(GSOptionsWinLayer,0,"Normal");
    GSOptionsWin->AddLayerToGroup(GSOptionsWinLayer,0,"Hovered");
    GSOptionsWin->SetPushButton(GSOptionsAccess);
    GSMenuRoot->AddChild(GSOptionsWin,7);

    // Create the first of the two buttons that will display the two sets of options (video options)
    UI::StackButton* GSVideoSetAccess = GameScreen->CreateStackButton("GS_VideoSetAccess",UI::UnifiedRect(0.11,0.0365,0.34,0.11));
    UI::ImageLayer* GSVideoSetAccessNormal = GSVideoSetAccess->CreateImageLayer("GSButton");
    UI::ImageLayer* GSVideoSetAccessHovered = GSVideoSetAccess->CreateImageLayer("GSHoveredButton");
    UI::SingleLineTextLayer* GSVideoSetAccessText = GSVideoSetAccess->CreateSingleLineTextLayer(GameScreenText);
    GSVideoSetAccessText->SetText("Video Options");
    GSVideoSetAccessText->HorizontallyAlign(UI::LA_Center);
    GSVideoSetAccessText->VerticallyAlign(UI::LA_Center);
    GSVideoSetAccessText->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,GSNormText);
    GSVideoSetAccess->AddLayerToGroup(GSVideoSetAccessNormal,0,"Normal");
    GSVideoSetAccess->AddLayerToGroup(GSVideoSetAccessText,1,"Normal");
    GSVideoSetAccess->AddLayerToGroup(GSVideoSetAccessHovered,0,"Hovered");
    GSVideoSetAccess->AddLayerToGroup(GSVideoSetAccessText,1,"Hovered");
    GSOptionsWin->AddChild(GSVideoSetAccess,1);

    // Create the second of the two buttons that will display the two sets of options (audio options)
    UI::StackButton* GSAudioSetAccess = GameScreen->CreateStackButton("GS_AudioSetAccess",UI::UnifiedRect(0.55,0.0365,0.34,0.11));
    UI::ImageLayer* GSAudioSetAccessNormal = GSAudioSetAccess->CreateImageLayer("GSButton");
    UI::ImageLayer* GSAudioSetAccessHovered = GSAudioSetAccess->CreateImageLayer("GSHoveredButton");
    UI::SingleLineTextLayer* GSAudioSetAccessText = GSAudioSetAccess->CreateSingleLineTextLayer(GameScreenText);
    GSAudioSetAccessText->SetText("Audio Options");
    GSAudioSetAccessText->HorizontallyAlign(UI::LA_Center);
    GSAudioSetAccessText->VerticallyAlign(UI::LA_Center);
    GSAudioSetAccessText->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,GSNormText);
    GSAudioSetAccess->AddLayerToGroup(GSAudioSetAccessNormal,0,"Normal");
    GSAudioSetAccess->AddLayerToGroup(GSAudioSetAccessText,1,"Normal");
    GSAudioSetAccess->AddLayerToGroup(GSAudioSetAccessHovered,0,"Hovered");
    GSAudioSetAccess->AddLayerToGroup(GSAudioSetAccessText,1,"Hovered");
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
    UI::ImageLayer* GSResolutionLabelLayer = GSResolutionLabel->CreateImageLayer("GSButton");
    UI::SingleLineTextLayer* GSResolutionLabelText = GSResolutionLabel->CreateSingleLineTextLayer(GameScreenText);
    GSResolutionLabelText->SetText("Video Resolutions");
    GSResolutionLabelText->HorizontallyAlign(UI::LA_Center);
    GSResolutionLabelText->VerticallyAlign(UI::LA_Center);
    GSResolutionLabelText->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,GSLargeText);
    GSResolutionLabel->AddLayerToGroup(GSResolutionLabelLayer,0,"Normal");
    GSResolutionLabel->AddLayerToGroup(GSResolutionLabelText,1,"Normal");
    GSResolutionLabel->AddLayerToGroup(GSResolutionLabelLayer,0,"Hovered");
    GSResolutionLabel->AddLayerToGroup(GSResolutionLabelText,1,"Hovered");
    GSVideoSet->AddChild(GSResolutionLabel,1);

    // Create the listing of detected supported resolutions
    UI::DropDownList* GSResolutionList = GameScreen->CreateDropDownList("GS_ResolutionList",UI::UnifiedRect(0.065,0.145,0.515,0.12),UI::SB_Separate);
    // Configure the selection display
    UI::Widget* GSResolutionDisplay = GSResolutionList->GetSelectionDisplay();
    UI::ImageLayer* GSResolutionDisplayLayer = GSResolutionDisplay->CreateImageLayer("GSListSelection");
    UI::SingleLineTextLayer* GSResolutionDisplayText = static_cast<UI::SingleLineTextLayer*>( GSResolutionDisplay->GetRenderLayer(0,UI::RLT_SingleLineText) );
    GSResolutionDisplayText->SetDefaultFont(GameScreenText);
    GSResolutionDisplayText->HorizontallyAlign(UI::LA_Center);
    GSResolutionDisplayText->VerticallyAlign(UI::LA_Center);
    GSResolutionDisplayText->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,GSLargeText);
    GSResolutionDisplay->AddLayerToGroup(GSResolutionDisplayLayer,0,"Normal");
    GSResolutionDisplay->AddLayerToGroup(GSResolutionDisplayLayer,0,"Hovered");
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
    // Configure the background for the listbox container
    UI::VerticalContainer* GSResolutionOptionsList = GSResolutionOptions->GetListContainer();
    UI::ImageLayer* GSResolutionOptionsListLayer = GSResolutionOptionsList->CreateImageLayer("GSListBackground");
    GSResolutionOptionsList->AddLayerToGroup(GSResolutionOptionsListLayer,0,"Normal");
    GSResolutionOptionsList->AddLayerToGroup(GSResolutionOptionsListLayer,0,"Hovered");
    // Configure the scrollbar for the option list
    UI::VerticalScrollbar* GSResolutionOptionsScroll = GSResolutionOptions->GetListScroll();
    GSResolutionOptionsScroll->SetIncrementDistance(0.05);
    // Configure the scroller
    UI::Button* GSResolutionOptionsScroller = GSResolutionOptionsScroll->GetScroller();
    UI::ImageLayer* GSResolutionOptionsScrollerNormal = GSResolutionOptionsScroller->CreateImageLayer("GSListScroller");
    UI::ImageLayer* GSResolutionOptionsScrollerHovered = GSResolutionOptionsScroller->CreateImageLayer("GSHoveredListScroller");
    GSResolutionOptionsScroller->AddLayerToGroup(GSResolutionOptionsScrollerNormal,0,"Normal");
    GSResolutionOptionsScroller->AddLayerToGroup(GSResolutionOptionsScrollerHovered,0,"Hovered");
    // Configure the up button
    UI::Button* GSResolutionOptionsScrollUp = GSResolutionOptionsScroll->GetUpLeftButton();
    UI::ImageLayer* GSResolutionOptionsScrollUpNormal = GSResolutionOptionsScrollUp->CreateImageLayer("GSListScrollUp");
    UI::ImageLayer* GSResolutionOptionsScrollUpHovered = GSResolutionOptionsScrollUp->CreateImageLayer("GSHoveredListScrollUp");
    GSResolutionOptionsScrollUp->AddLayerToGroup(GSResolutionOptionsScrollUpNormal,0,"Normal");
    GSResolutionOptionsScrollUp->AddLayerToGroup(GSResolutionOptionsScrollUpHovered,0,"Hovered");
    // Configure the down button
    UI::Button* GSResolutionOptionsScrollDown = GSResolutionOptionsScroll->GetDownRightButton();
    UI::ImageLayer* GSResolutionOptionsScrollDownNormal = GSResolutionOptionsScrollDown->CreateImageLayer("GSListScrollDown");
    UI::ImageLayer* GSResolutionOptionsScrollDownHovered = GSResolutionOptionsScrollDown->CreateImageLayer("GSHoveredListScrollDown");
    GSResolutionOptionsScrollDown->AddLayerToGroup(GSResolutionOptionsScrollDownNormal,0,"Normal");
    GSResolutionOptionsScrollDown->AddLayerToGroup(GSResolutionOptionsScrollDownHovered,0,"Hovered");
    // Configure the scroll back
    UI::Button* GSResolutionOptionsScrollBack = GSResolutionOptionsScroll->GetScrollBack();
    UI::ImageLayer* GSResolutionOptionsScrollBackLayer = GSResolutionOptionsScrollBack->CreateImageLayer("GSListScrollBackground");
    GSResolutionOptionsScrollBack->AddLayerToGroup(GSResolutionOptionsScrollBackLayer,0,"Normal");
    GSResolutionOptionsScrollBack->AddLayerToGroup(GSResolutionOptionsScrollBackLayer,0,"Hovered");
    // Wrap up listing configuration
    GSVideoSet->AddChild(GSResolutionList,11);

    // Create the checkbox for enabling or disabling fullscreen
    UI::CheckBox* GSFullscreenBox = GameScreen->CreateCheckBox("GS_FullscreenBox",UI::UnifiedRect(0.655,0.145,0,0.12));
    GSFullscreenBox->SetHorizontalSizingRules(UI::SR_Match_Other_Axis);
    UI::ImageLayer* GSFullscreenBoxUnchecked = GSFullscreenBox->CreateImageLayer("GSCheckboxUnchecked");
    UI::ImageLayer* GSFullscreenBoxHoverUnchecked = GSFullscreenBox->CreateImageLayer("GSHoveredCheckboxUnchecked");
    UI::ImageLayer* GSFullscreenBoxChecked = GSFullscreenBox->CreateImageLayer("GSCheckboxChecked");
    UI::ImageLayer* GSFullscreenBoxHoverChecked = GSFullscreenBox->CreateImageLayer("GSHoveredCheckboxChecked");
    GSFullscreenBox->AddLayerToGroup(GSFullscreenBoxUnchecked,0,"Normal");
    GSFullscreenBox->AddLayerToGroup(GSFullscreenBoxHoverUnchecked,0,"Hovered");
    GSFullscreenBox->AddLayerToGroup(GSFullscreenBoxChecked,0,"SelectedNormal");
    GSFullscreenBox->AddLayerToGroup(GSFullscreenBoxHoverChecked,0,"SelectedHovered");
    GSVideoSet->AddChild(GSFullscreenBox,3);

    // Create the label for the fullscreen checkbox
    UI::Widget* GSFullscreenLabel = GameScreen->CreateWidget("GS_FullscreenLabel",UI::UnifiedRect(0.72,0.145,0.22,0.12));
    UI::ImageLayer* GSFullscreenLabelLayer = GSFullscreenLabel->CreateImageLayer("GSButton");
    UI::SingleLineTextLayer* GSFullscreenLabelText = GSFullscreenLabel->CreateSingleLineTextLayer(GameScreenText);
    GSFullscreenLabelText->SetText("Fullscreen");
    GSFullscreenLabelText->HorizontallyAlign(UI::LA_Center);
    GSFullscreenLabelText->VerticallyAlign(UI::LA_Center);
    GSFullscreenLabelText->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,GSLargeText);
    GSFullscreenLabel->AddLayerToGroup(GSFullscreenLabelLayer,0,"Normal");
    GSFullscreenLabel->AddLayerToGroup(GSFullscreenLabelText,1,"Normal");
    GSFullscreenLabel->AddLayerToGroup(GSFullscreenLabelLayer,0,"Hovered");
    GSFullscreenLabel->AddLayerToGroup(GSFullscreenLabelText,1,"Hovered");
    GSVideoSet->AddChild(GSFullscreenLabel,4);

    // Create the label for the FSAA options
    UI::Widget* GSFSAALabel = GameScreen->CreateWidget("GS_FSAALabel",UI::UnifiedRect(0.12,0.365,0.415,0.13));
    UI::ImageLayer* GSFSAALabelLayer = GSFSAALabel->CreateImageLayer("GSButton");
    UI::SingleLineTextLayer* GSFSAALabelText = GSFSAALabel->CreateSingleLineTextLayer(GameScreenText);
    GSFSAALabelText->SetText("Anti-Aliasing");
    GSFSAALabelText->HorizontallyAlign(UI::LA_Center);
    GSFSAALabelText->VerticallyAlign(UI::LA_Center);
    GSFSAALabelText->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,GSLargeText);
    GSFSAALabel->AddLayerToGroup(GSFSAALabelLayer,0,"Normal");
    GSFSAALabel->AddLayerToGroup(GSFSAALabelText,1,"Normal");
    GSFSAALabel->AddLayerToGroup(GSFSAALabelLayer,0,"Hovered");
    GSFSAALabel->AddLayerToGroup(GSFSAALabelText,1,"Hovered");
    GSVideoSet->AddChild(GSFSAALabel,5);

    // Create the listing of detected anti-aliasing options
    UI::DropDownList* GSFSAAList = GameScreen->CreateDropDownList("GS_FSAAList",UI::UnifiedRect(0.065,0.50,0.515,0.12),UI::SB_Separate);
    // Configure the selection display
    UI::Widget* GSFSAADisplay = GSFSAAList->GetSelectionDisplay();
    UI::ImageLayer* GSFSAADisplayLayer = GSFSAADisplay->CreateImageLayer("GSListSelection");
    UI::SingleLineTextLayer* GSFSAADisplayText = static_cast<UI::SingleLineTextLayer*>( GSFSAADisplay->GetRenderLayer(0,UI::RLT_SingleLineText) );
    GSFSAADisplayText->SetDefaultFont(GameScreenText);
    GSFSAADisplayText->HorizontallyAlign(UI::LA_Center);
    GSFSAADisplayText->VerticallyAlign(UI::LA_Center);
    GSFSAADisplayText->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,GSLargeText);
    GSFSAADisplay->AddLayerToGroup(GSFSAADisplayLayer,0,"Normal");
    GSFSAADisplay->AddLayerToGroup(GSFSAADisplayLayer,0,"Hovered");
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
    // Configure the background for the listbox container
    UI::VerticalContainer* GSFSAAOptionsList = GSFSAAOptions->GetListContainer();
    UI::ImageLayer* GSFSAAOptionsListLayer = GSFSAAOptionsList->CreateImageLayer("GSListBackground");
    GSFSAAOptionsList->AddLayerToGroup(GSFSAAOptionsListLayer,0,"Normal");
    GSFSAAOptionsList->AddLayerToGroup(GSFSAAOptionsListLayer,0,"Hovered");
    // Configure the scrollbar for the option list
    UI::VerticalScrollbar* GSFSAAOptionsScroll = GSFSAAOptions->GetListScroll();
    GSFSAAOptionsScroll->SetIncrementDistance(0.05);
    // Configure the scroller
    UI::Button* GSFSAAOptionsScroller = GSFSAAOptionsScroll->GetScroller();
    UI::ImageLayer* GSFSAAOptionsScrollerNormal = GSFSAAOptionsScroller->CreateImageLayer("GSListScroller");
    UI::ImageLayer* GSFSAAOptionsScrollerHovered = GSFSAAOptionsScroller->CreateImageLayer("GSHoveredListScroller");
    GSFSAAOptionsScroller->AddLayerToGroup(GSFSAAOptionsScrollerNormal,0,"Normal");
    GSFSAAOptionsScroller->AddLayerToGroup(GSFSAAOptionsScrollerHovered,0,"Hovered");
    // Configure the up button
    UI::Button* GSFSAAOptionsScrollUp = GSFSAAOptionsScroll->GetUpLeftButton();
    UI::ImageLayer* GSFSAAOptionsScrollUpNormal = GSFSAAOptionsScrollUp->CreateImageLayer("GSListScrollUp");
    UI::ImageLayer* GSFSAAOptionsScrollUpHovered = GSFSAAOptionsScrollUp->CreateImageLayer("GSHoveredListScrollUp");
    GSFSAAOptionsScrollUp->AddLayerToGroup(GSFSAAOptionsScrollUpNormal,0,"Normal");
    GSFSAAOptionsScrollUp->AddLayerToGroup(GSFSAAOptionsScrollUpHovered,0,"Hovered");
    // Configure the down button
    UI::Button* GSFSAAOptionsScrollDown = GSFSAAOptionsScroll->GetDownRightButton();
    UI::ImageLayer* GSFSAAOptionsScrollDownNormal = GSFSAAOptionsScrollDown->CreateImageLayer("GSListScrollDown");
    UI::ImageLayer* GSFSAAOptionsScrollDownHovered = GSFSAAOptionsScrollDown->CreateImageLayer("GSHoveredListScrollDown");
    GSFSAAOptionsScrollDown->AddLayerToGroup(GSFSAAOptionsScrollDownNormal,0,"Normal");
    GSFSAAOptionsScrollDown->AddLayerToGroup(GSFSAAOptionsScrollDownHovered,0,"Hovered");
    // Configure the scroll back
    UI::Button* GSFSAAOptionsScrollBack = GSFSAAOptionsScroll->GetScrollBack();
    UI::ImageLayer* GSFSAAOptionsScrollBackLayer = GSFSAAOptionsScrollBack->CreateImageLayer("GSListScrollBackground");
    GSFSAAOptionsScrollBack->AddLayerToGroup(GSFSAAOptionsScrollBackLayer,0,"Normal");
    GSFSAAOptionsScrollBack->AddLayerToGroup(GSFSAAOptionsScrollBackLayer,0,"Hovered");
    // Wrap up listing configuration
    GSVideoSet->AddChild(GSFSAAList,10);

    // Create the checkbox for enabling or disabling FPS stats display
    UI::CheckBox* GSStatsBox = GameScreen->CreateCheckBox("GS_StatsBox",UI::UnifiedRect(0.655,0.50,0,0.12));
    GSStatsBox->SetHorizontalSizingRules(UI::SR_Match_Other_Axis);
    UI::ImageLayer* GSStatsBoxUnchecked = GSStatsBox->CreateImageLayer("GSCheckboxUnchecked");
    UI::ImageLayer* GSStatsBoxHoverUnchecked = GSStatsBox->CreateImageLayer("GSHoveredCheckboxUnchecked");
    UI::ImageLayer* GSStatsBoxChecked = GSStatsBox->CreateImageLayer("GSCheckboxChecked");
    UI::ImageLayer* GSStatsBoxHoverChecked = GSStatsBox->CreateImageLayer("GSHoveredCheckboxChecked");
    GSStatsBox->AddLayerToGroup(GSStatsBoxUnchecked,0,"Normal");
    GSStatsBox->AddLayerToGroup(GSStatsBoxHoverUnchecked,0,"Hovered");
    GSStatsBox->AddLayerToGroup(GSStatsBoxChecked,0,"SelectedNormal");
    GSStatsBox->AddLayerToGroup(GSStatsBoxHoverChecked,0,"SelectedHovered");
    GSVideoSet->AddChild(GSStatsBox,7);

    // Create the label for the FPS stats display checkbox
    UI::Widget* GSStatsLabel = GameScreen->CreateWidget("GS_StatsLabel",UI::UnifiedRect(0.72,0.50,0.22,0.12));
    UI::ImageLayer* GSStatsLabelLayer = GSStatsLabel->CreateImageLayer("GSButton");
    UI::SingleLineTextLayer* GSStatsLabelText = GSStatsLabel->CreateSingleLineTextLayer(GameScreenText);
    GSStatsLabelText->SetText("Show FPS");
    GSStatsLabelText->HorizontallyAlign(UI::LA_Center);
    GSStatsLabelText->VerticallyAlign(UI::LA_Center);
    GSStatsLabelText->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,GSLargeText);
    GSStatsLabel->AddLayerToGroup(GSStatsLabelLayer,0,"Normal");
    GSStatsLabel->AddLayerToGroup(GSStatsLabelText,1,"Normal");
    GSStatsLabel->AddLayerToGroup(GSStatsLabelLayer,0,"Hovered");
    GSStatsLabel->AddLayerToGroup(GSStatsLabelText,1,"Hovered");
    GSVideoSet->AddChild(GSStatsLabel,8);

    // Create the button that will apply all of the currently displayed video settings
    UI::Button* GSVideoOptsApply = GameScreen->CreateButton("GS_VideoOptsApply",UI::UnifiedRect(0.815,0.84,0.17,0.14));
    UI::ImageLayer* GSVideoOptsApplyNormal = GSVideoOptsApply->CreateImageLayer("GSOptionsApplyButton");
    UI::ImageLayer* GSVideoOptsApplyHovered = GSVideoOptsApply->CreateImageLayer("GSOptionsApplyHoveredButton");
    UI::SingleLineTextLayer* GSVideoOptsApplyText = GSVideoOptsApply->CreateSingleLineTextLayer(GameScreenText);
    GSVideoOptsApplyText->SetText("Apply");
    GSVideoOptsApplyText->HorizontallyAlign(UI::LA_Center);
    GSVideoOptsApplyText->VerticallyAlign(UI::LA_Center);
    GSVideoOptsApplyText->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,GSLargeText);
    GSVideoOptsApply->AddLayerToGroup(GSVideoOptsApplyNormal,0,"Normal");
    GSVideoOptsApply->AddLayerToGroup(GSVideoOptsApplyText,1,"Normal");
    GSVideoOptsApply->AddLayerToGroup(GSVideoOptsApplyHovered,0,"Hovered");
    GSVideoOptsApply->AddLayerToGroup(GSVideoOptsApplyText,1,"Hovered");
    GSVideoSet->AddChild(GSVideoOptsApply,12);

    const Real GSScrollerSize = 0.09;
    // Create the TabbedSubSet that will house all our audio options
    UI::TabSet::TabbedSubSet* GSAudioSet = GSOptionsTabSet->CreateTabbedSubSet("GS_AudioSet",2);

    // Create the label for the Music volume
    UI::Widget* GSMusicVolLabel = GameScreen->CreateWidget("GS_MusicVolLabel",UI::UnifiedRect(0.0,0.01,0.40,0.13));
    GSMusicVolLabel->SetPositioningRules(UI::PF_Anchor_HorizontalCenter);
    UI::ImageLayer* GSMusicVolLabelLayer = GSMusicVolLabel->CreateImageLayer("GSButton");
    UI::SingleLineTextLayer* GSMusicVolLabelText = GSMusicVolLabel->CreateSingleLineTextLayer(GameScreenText);
    GSMusicVolLabelText->SetText("Music Volume");
    GSMusicVolLabelText->HorizontallyAlign(UI::LA_Center);
    GSMusicVolLabelText->VerticallyAlign(UI::LA_Center);
    GSMusicVolLabelText->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,GSLargeText);
    GSMusicVolLabel->AddLayerToGroup(GSMusicVolLabelLayer,0,"Normal");
    GSMusicVolLabel->AddLayerToGroup(GSMusicVolLabelText,1,"Normal");
    GSMusicVolLabel->AddLayerToGroup(GSMusicVolLabelLayer,0,"Hovered");
    GSMusicVolLabel->AddLayerToGroup(GSMusicVolLabelText,1,"Hovered");
    GSAudioSet->AddChild(GSMusicVolLabel,1);

    // Create the Music volume slider
    UI::HorizontalScrollbar* GSMusicVol = GameScreen->CreateHorizontalScrollbar("GS_MusicVolume",UI::UnifiedRect(0.0,0.145,0.7,0.09),UI::SB_Separate);
    GSMusicVol->SetPositioningRules(UI::PF_Anchor_HorizontalCenter);
    GSMusicVol->SetIncrementDistance(GSScrollerSize * 0.5);
    GSMusicVol->SetScrollerSize(GSScrollerSize);
    // Configure the scroller
    UI::Button* GSMusicVolScroller = GSMusicVol->GetScroller();
    UI::ImageLayer* GSMusicVolScrollerNormal = GSMusicVolScroller->CreateImageLayer("GSScroller");
    UI::ImageLayer* GSMusicVolScrollerHovered = GSMusicVolScroller->CreateImageLayer("GSHoveredScroller");
    GSMusicVolScroller->AddLayerToGroup(GSMusicVolScrollerNormal,0,"Normal");
    GSMusicVolScroller->AddLayerToGroup(GSMusicVolScrollerHovered,0,"Hovered");
    // Configure the up button
    UI::Button* GSMusicVolUp = GSMusicVol->GetUpLeftButton();
    UI::ImageLayer* GSMusicVolUpNormal = GSMusicVolUp->CreateImageLayer("GSScrollLeft");
    UI::ImageLayer* GSMusicVolUpHovered = GSMusicVolUp->CreateImageLayer("GSHoveredScrollLeft");
    GSMusicVolUp->AddLayerToGroup(GSMusicVolUpNormal,0,"Normal");
    GSMusicVolUp->AddLayerToGroup(GSMusicVolUpHovered,0,"Hovered");
    // Configure the down button
    UI::Button* GSMusicVolDown = GSMusicVol->GetDownRightButton();
    UI::ImageLayer* GSMusicVolDownNormal = GSMusicVolDown->CreateImageLayer("GSScrollRight");
    UI::ImageLayer* GSMusicVolDownHovered = GSMusicVolDown->CreateImageLayer("GSHoveredScrollRight");
    GSMusicVolDown->AddLayerToGroup(GSMusicVolDownNormal,0,"Normal");
    GSMusicVolDown->AddLayerToGroup(GSMusicVolDownHovered,0,"Hovered");
    // Configure the scroll back
    UI::Button* GSMusicVolBack = GSMusicVol->GetScrollBack();
    UI::ImageLayer* GSMusicVolBackLayer = GSMusicVolBack->CreateImageLayer("GSScrollBackground");
    GSMusicVolBack->AddLayerToGroup(GSMusicVolBackLayer,0,"Normal");
    GSMusicVolBack->AddLayerToGroup(GSMusicVolBackLayer,0,"Hovered");
    GSAudioSet->AddChild(GSMusicVol,2);

    // Create the label for the Effects volume
    UI::Widget* GSEffectsVolLabel = GameScreen->CreateWidget("GS_EffectsVolLabel",UI::UnifiedRect(0.0,0.275,0.40,0.13));
    GSEffectsVolLabel->SetPositioningRules(UI::PF_Anchor_HorizontalCenter);
    UI::ImageLayer* GSEffectsVolLabelLayer = GSEffectsVolLabel->CreateImageLayer("GSButton");
    UI::SingleLineTextLayer* GSEffectsVolLabelText = GSEffectsVolLabel->CreateSingleLineTextLayer(GameScreenText);
    GSEffectsVolLabelText->SetText("Effects Volume");
    GSEffectsVolLabelText->HorizontallyAlign(UI::LA_Center);
    GSEffectsVolLabelText->VerticallyAlign(UI::LA_Center);
    GSEffectsVolLabelText->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,GSLargeText);
    GSEffectsVolLabel->AddLayerToGroup(GSEffectsVolLabelLayer,0,"Normal");
    GSEffectsVolLabel->AddLayerToGroup(GSEffectsVolLabelText,1,"Normal");
    GSEffectsVolLabel->AddLayerToGroup(GSEffectsVolLabelLayer,0,"Hovered");
    GSEffectsVolLabel->AddLayerToGroup(GSEffectsVolLabelText,1,"Hovered");
    GSAudioSet->AddChild(GSEffectsVolLabel,3);

    // Create the Effects volume slider
    UI::HorizontalScrollbar* GSEffectsVol = GameScreen->CreateHorizontalScrollbar("GS_EffectsVolume",UI::UnifiedRect(0.0,0.41,0.7,0.09),UI::SB_Separate);
    GSEffectsVol->SetPositioningRules(UI::PF_Anchor_HorizontalCenter);
    GSEffectsVol->SetIncrementDistance(GSScrollerSize * 0.5);
    GSEffectsVol->SetScrollerSize(GSScrollerSize);
    // Configure the scroller
    UI::Button* GSEffectsVolScroller = GSEffectsVol->GetScroller();
    UI::ImageLayer* GSEffectsVolScrollerNormal = GSEffectsVolScroller->CreateImageLayer("GSScroller");
    UI::ImageLayer* GSEffectsVolScrollerHovered = GSEffectsVolScroller->CreateImageLayer("GSHoveredScroller");
    GSEffectsVolScroller->AddLayerToGroup(GSEffectsVolScrollerNormal,0,"Normal");
    GSEffectsVolScroller->AddLayerToGroup(GSEffectsVolScrollerHovered,0,"Hovered");
    // Configure the up button
    UI::Button* GSEffectsVolUp = GSEffectsVol->GetUpLeftButton();
    UI::ImageLayer* GSEffectsVolUpNormal = GSEffectsVolUp->CreateImageLayer("GSScrollLeft");
    UI::ImageLayer* GSEffectsVolUpHovered = GSEffectsVolUp->CreateImageLayer("GSHoveredScrollLeft");
    GSEffectsVolUp->AddLayerToGroup(GSEffectsVolUpNormal,0,"Normal");
    GSEffectsVolUp->AddLayerToGroup(GSEffectsVolUpHovered,0,"Hovered");
    // Configure the down button
    UI::Button* GSEffectsVolDown = GSEffectsVol->GetDownRightButton();
    UI::ImageLayer* GSEffectsVolDownNormal = GSEffectsVolDown->CreateImageLayer("GSScrollRight");
    UI::ImageLayer* GSEffectsVolDownHovered = GSEffectsVolDown->CreateImageLayer("GSHoveredScrollRight");
    GSEffectsVolDown->AddLayerToGroup(GSEffectsVolDownNormal,0,"Normal");
    GSEffectsVolDown->AddLayerToGroup(GSEffectsVolDownHovered,0,"Hovered");
    // Configure the scroll back
    UI::Button* GSEffectsVolBack = GSEffectsVol->GetScrollBack();
    UI::ImageLayer* GSEffectsVolBackLayer = GSEffectsVolBack->CreateImageLayer("GSScrollBackground");
    GSEffectsVolBack->AddLayerToGroup(GSEffectsVolBackLayer,0,"Normal");
    GSEffectsVolBack->AddLayerToGroup(GSEffectsVolBackLayer,0,"Hovered");
    GSAudioSet->AddChild(GSEffectsVol,2);

    // Create the label for the Audio Device options
    UI::Widget* GSAudioDeviceLabel = GameScreen->CreateWidget("GS_AudioDeviceLabel",UI::UnifiedRect(0.12,0.56,0.415,0.13));
    UI::ImageLayer* GSAudioDeviceLabelLayer = GSAudioDeviceLabel->CreateImageLayer("GSButton");
    UI::SingleLineTextLayer* GSAudioDeviceLabelText = GSAudioDeviceLabel->CreateSingleLineTextLayer(GameScreenText);
    GSAudioDeviceLabelText->SetText("Audio Device");
    GSAudioDeviceLabelText->HorizontallyAlign(UI::LA_Center);
    GSAudioDeviceLabelText->VerticallyAlign(UI::LA_Center);
    GSAudioDeviceLabelText->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,GSLargeText);
    GSAudioDeviceLabel->AddLayerToGroup(GSAudioDeviceLabelLayer,0,"Normal");
    GSAudioDeviceLabel->AddLayerToGroup(GSAudioDeviceLabelText,1,"Normal");
    GSAudioDeviceLabel->AddLayerToGroup(GSAudioDeviceLabelLayer,0,"Hovered");
    GSAudioDeviceLabel->AddLayerToGroup(GSAudioDeviceLabelText,1,"Hovered");
    GSAudioSet->AddChild(GSAudioDeviceLabel,3);

    // Create the listing of detected anti-aliasing options
    UI::DropDownList* GSAudioDeviceList = GameScreen->CreateDropDownList("GS_AudioDeviceList",UI::UnifiedRect(0.065,0.695,0.515,0.12),UI::SB_Separate);
    // Configure the selection display
    UI::Widget* GSAudioDeviceDisplay = GSAudioDeviceList->GetSelectionDisplay();
    UI::ImageLayer* GSAudioDeviceDisplayLayer = GSAudioDeviceDisplay->CreateImageLayer("GSListSelection");
    UI::SingleLineTextLayer* GSAudioDeviceDisplayText = static_cast<UI::SingleLineTextLayer*>( GSAudioDeviceDisplay->GetRenderLayer(0,UI::RLT_SingleLineText) );
    GSAudioDeviceDisplayText->SetDefaultFont(GameScreenText);
    GSAudioDeviceDisplayText->HorizontallyAlign(UI::LA_Center);
    GSAudioDeviceDisplayText->VerticallyAlign(UI::LA_Center);
    GSAudioDeviceDisplayText->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,GSLargeText);
    GSAudioDeviceDisplay->AddLayerToGroup(GSAudioDeviceDisplayLayer,0,"Normal");
    GSAudioDeviceDisplay->AddLayerToGroup(GSAudioDeviceDisplayLayer,0,"Hovered");
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
    // Configure the background for the listbox container
    UI::VerticalContainer* GSAudioDeviceOptionsList = GSAudioDeviceOptions->GetListContainer();
    UI::ImageLayer* GSAudioDeviceOptionsListLayer = GSAudioDeviceOptionsList->CreateImageLayer("GSListBackground");
    GSAudioDeviceOptionsList->AddLayerToGroup(GSAudioDeviceOptionsListLayer,0,"Normal");
    GSAudioDeviceOptionsList->AddLayerToGroup(GSAudioDeviceOptionsListLayer,0,"Hovered");
    // Configure the scrollbar for the option list
    UI::VerticalScrollbar* GSAudioDeviceOptionsScroll = GSAudioDeviceOptions->GetListScroll();
    GSAudioDeviceOptionsScroll->SetIncrementDistance(0.05);
    // Configure the scroller
    UI::Button* GSAudioDeviceOptionsScroller = GSAudioDeviceOptionsScroll->GetScroller();
    UI::ImageLayer* GSAudioDeviceOptionsScrollerNormal = GSAudioDeviceOptionsScroller->CreateImageLayer("GSListScroller");
    UI::ImageLayer* GSAudioDeviceOptionsScrollerHovered = GSAudioDeviceOptionsScroller->CreateImageLayer("GSHoveredListScroller");
    GSAudioDeviceOptionsScroller->AddLayerToGroup(GSAudioDeviceOptionsScrollerNormal,0,"Normal");
    GSAudioDeviceOptionsScroller->AddLayerToGroup(GSAudioDeviceOptionsScrollerHovered,0,"Hovered");
    // Configure the up button
    UI::Button* GSAudioDeviceOptionsScrollUp = GSAudioDeviceOptionsScroll->GetUpLeftButton();
    UI::ImageLayer* GSAudioDeviceOptionsScrollUpNormal = GSAudioDeviceOptionsScrollUp->CreateImageLayer("GSListScrollUp");
    UI::ImageLayer* GSAudioDeviceOptionsScrollUpHovered = GSAudioDeviceOptionsScrollUp->CreateImageLayer("GSHoveredListScrollUp");
    GSAudioDeviceOptionsScrollUp->AddLayerToGroup(GSAudioDeviceOptionsScrollUpNormal,0,"Normal");
    GSAudioDeviceOptionsScrollUp->AddLayerToGroup(GSAudioDeviceOptionsScrollUpHovered,0,"Hovered");
    // Configure the down button
    UI::Button* GSAudioDeviceOptionsScrollDown = GSAudioDeviceOptionsScroll->GetDownRightButton();
    UI::ImageLayer* GSAudioDeviceOptionsScrollDownNormal = GSAudioDeviceOptionsScrollDown->CreateImageLayer("GSListScrollDown");
    UI::ImageLayer* GSAudioDeviceOptionsScrollDownHovered = GSAudioDeviceOptionsScrollDown->CreateImageLayer("GSHoveredListScrollDown");
    GSAudioDeviceOptionsScrollDown->AddLayerToGroup(GSAudioDeviceOptionsScrollDownNormal,0,"Normal");
    GSAudioDeviceOptionsScrollDown->AddLayerToGroup(GSAudioDeviceOptionsScrollDownHovered,0,"Hovered");
    // Configure the scroll back
    UI::Button* GSAudioDeviceOptionsScrollBack = GSAudioDeviceOptionsScroll->GetScrollBack();
    UI::ImageLayer* GSAudioDeviceOptionsScrollBackLayer = GSAudioDeviceOptionsScrollBack->CreateImageLayer("GSListScrollBackground");
    GSAudioDeviceOptionsScrollBack->AddLayerToGroup(GSAudioDeviceOptionsScrollBackLayer,0,"Normal");
    GSAudioDeviceOptionsScrollBack->AddLayerToGroup(GSAudioDeviceOptionsScrollBackLayer,0,"Hovered");
    // Wrap up listing configuration
    GSAudioSet->AddChild(GSAudioDeviceList,10);

    // Create the checkbox for enabling or disabling FPS stats display
    UI::CheckBox* GSMuteBox = GameScreen->CreateCheckBox("GS_MuteBox",UI::UnifiedRect(0.655,0.695,0,0.12));
    GSMuteBox->SetHorizontalSizingRules(UI::SR_Match_Other_Axis);
    UI::ImageLayer* GSMuteBoxUnchecked = GSMuteBox->CreateImageLayer("GSCheckboxUnchecked");
    UI::ImageLayer* GSMuteBoxHoverUnchecked = GSMuteBox->CreateImageLayer("GSHoveredCheckboxUnchecked");
    UI::ImageLayer* GSMuteBoxChecked = GSMuteBox->CreateImageLayer("GSCheckboxChecked");
    UI::ImageLayer* GSMuteBoxHoverChecked = GSMuteBox->CreateImageLayer("GSHoveredCheckboxChecked");
    GSMuteBox->AddLayerToGroup(GSMuteBoxUnchecked,0,"Normal");
    GSMuteBox->AddLayerToGroup(GSMuteBoxHoverUnchecked,0,"Hovered");
    GSMuteBox->AddLayerToGroup(GSMuteBoxChecked,0,"SelectedNormal");
    GSMuteBox->AddLayerToGroup(GSMuteBoxHoverChecked,0,"SelectedHovered");
    GSAudioSet->AddChild(GSMuteBox,5);

    // Create the label for the FPS stats display checkbox
    UI::Widget* GSMuteLabel = GameScreen->CreateWidget("GS_MuteLabel",UI::UnifiedRect(0.72,0.695,0.22,0.12));
    UI::ImageLayer* GSMuteLabelLayer = GSMuteLabel->CreateImageLayer("GSButton");
    UI::SingleLineTextLayer* GSMuteLabelText = GSMuteLabel->CreateSingleLineTextLayer(GameScreenText);
    GSMuteLabelText->SetText("Mute");
    GSMuteLabelText->HorizontallyAlign(UI::LA_Center);
    GSMuteLabelText->VerticallyAlign(UI::LA_Center);
    GSMuteLabelText->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,GSLargeText);
    GSMuteLabel->AddLayerToGroup(GSMuteLabelLayer,0,"Normal");
    GSMuteLabel->AddLayerToGroup(GSMuteLabelText,1,"Normal");
    GSMuteLabel->AddLayerToGroup(GSMuteLabelLayer,0,"Hovered");
    GSMuteLabel->AddLayerToGroup(GSMuteLabelText,1,"Hovered");
    GSAudioSet->AddChild(GSMuteLabel,6);

    // Create the back button for the options window
    UI::StackButton* GSOptsBack = GameScreen->CreateStackButton("GS_OptsBack",UI::UnifiedRect(0.780,0.870,0.156,0.094));
    UI::ImageLayer* GSOptsBackNormal = GSOptsBack->CreateImageLayer("GSOptionsApplyButton");
    UI::ImageLayer* GSOptsBackHovered = GSOptsBack->CreateImageLayer("GSOptionsApplyHoveredButton");
    UI::SingleLineTextLayer* GSOptsBackText = GSOptsBack->CreateSingleLineTextLayer(GameScreenText);
    GSOptsBackText->SetText("Back");
    GSOptsBackText->HorizontallyAlign(UI::LA_Center);
    GSOptsBackText->VerticallyAlign(UI::LA_Center);
    GSOptsBackText->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,GSLargeText);
    GSOptsBack->AddLayerToGroup(GSOptsBackNormal,0,"Normal");
    GSOptsBack->AddLayerToGroup(GSOptsBackText,1,"Normal");
    GSOptsBack->AddLayerToGroup(GSOptsBackHovered,0,"Hovered");
    GSOptsBack->AddLayerToGroup(GSOptsBackText,1,"Hovered");
    GSOptionsWin->SetPopButton(GSOptsBack);
    GSOptionsWin->AddChild(GSOptsBack,5);

    // Second button Y placement: 26

    // Create the game restart button
    UI::Button* GSMenuRestart = GameScreen->CreateButton("GS_MenuRestart",UI::UnifiedRect(0.15,0.445,0.70,0.11));
    UI::ImageLayer* GSMenuRestartNormal = GSMenuRestart->CreateImageLayer("GSOptionsButton");
    UI::ImageLayer* GSMenuRestartHovered = GSMenuRestart->CreateImageLayer("GSOptionsHoveredButton");
    UI::SingleLineTextLayer* GSMenuRestartText = GSMenuRestart->CreateSingleLineTextLayer(GameScreenText);
    GSMenuRestartText->SetText("Restart Level");
    GSMenuRestartText->HorizontallyAlign(UI::LA_Center);
    GSMenuRestartText->VerticallyAlign(UI::LA_Center);
    GSMenuRestartText->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,GSNormText);
    GSMenuRestart->AddLayerToGroup(GSMenuRestartNormal,0,"Normal");
    GSMenuRestart->AddLayerToGroup(GSMenuRestartText,1,"Normal");
    GSMenuRestart->AddLayerToGroup(GSMenuRestartHovered,0,"Hovered");
    GSMenuRestart->AddLayerToGroup(GSMenuRestartText,1,"Hovered");
    GSMenuRoot->AddChild(GSMenuRestart,3);

    // Create the game return button
    UI::StackButton* GSMenuReturn = GameScreen->CreateStackButton("GS_MenuReturn",UI::UnifiedRect(0.15,0.63,0.70,0.11));
    UI::ImageLayer* GSMenuReturnNormal = GSMenuReturn->CreateImageLayer("GSOptionsButton");
    UI::ImageLayer* GSMenuReturnHovered = GSMenuReturn->CreateImageLayer("GSOptionsHoveredButton");
    UI::SingleLineTextLayer* GSMenuReturnText = GSMenuReturn->CreateSingleLineTextLayer(GameScreenText);
    GSMenuReturnText->SetText("Return to Game");
    GSMenuReturnText->HorizontallyAlign(UI::LA_Center);
    GSMenuReturnText->VerticallyAlign(UI::LA_Center);
    GSMenuReturnText->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,GSNormText);
    GSMenuReturn->AddLayerToGroup(GSMenuReturnNormal,0,"Normal");
    GSMenuReturn->AddLayerToGroup(GSMenuReturnText,1,"Normal");
    GSMenuReturn->AddLayerToGroup(GSMenuReturnHovered,0,"Hovered");
    GSMenuReturn->AddLayerToGroup(GSMenuReturnText,1,"Hovered");
    GSMenuRoot->SetPopButton(GSMenuReturn);
    GSMenuRoot->AddChild(GSMenuReturn,4);

    // Create the game restart button
    UI::Button* GSMenuExit = GameScreen->CreateButton("GS_MenuExit",UI::UnifiedRect(0.15,0.815,0.70,0.11));
    UI::ImageLayer* GSMenuExitNormal = GSMenuExit->CreateImageLayer("GSOptionsButton");
    UI::ImageLayer* GSMenuExitHovered = GSMenuExit->CreateImageLayer("GSOptionsHoveredButton");
    UI::SingleLineTextLayer* GSMenuExitText = GSMenuExit->CreateSingleLineTextLayer(GameScreenText);
    GSMenuExitText->SetText("Exit to Main Menu");
    GSMenuExitText->HorizontallyAlign(UI::LA_Center);
    GSMenuExitText->VerticallyAlign(UI::LA_Center);
    GSMenuExitText->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,GSNormText);
    GSMenuExit->AddLayerToGroup(GSMenuExitNormal,0,"Normal");
    GSMenuExit->AddLayerToGroup(GSMenuExitText,1,"Normal");
    GSMenuExit->AddLayerToGroup(GSMenuExitHovered,0,"Hovered");
    GSMenuExit->AddLayerToGroup(GSMenuExitText,1,"Hovered");
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
    GSItemShopDescriptionText->SetTextlineVerticalAlignment(UI::LA_Center);
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

    ////-------------------  FPS  Stats  -------------------////
    // Create the widget container for the current FPS display
    UI::Widget* GSCurrentFPS = GameScreen->CreateWidget("GS_CurrentFPS",UI::UnifiedRect(0.008,0.006,0.21,0.065));
    GSCurrentFPS->SetAspectRatioLock(UI::ARL_Ratio_Y_Axis);
    // Create the widget for displaying the current FPS explanation text
    UI::Widget* GSCurrentFPSLabel = GameScreen->CreateWidget("GS_CurrentFPSLabel",UI::UnifiedRect(0.0,0.0,0.55,1.0));
    GSCurrentFPSLabel->SetPositioningRules(UI::PF_Anchor_TopLeft);
    UI::SingleLineTextLayer* GSCurrentFPSLabelLayer = GSCurrentFPSLabel->CreateSingleLineTextLayer(GameScreenText);
    GSCurrentFPSLabelLayer->SetText("Current FPS: ");
    GSCurrentFPSLabelLayer->HorizontallyAlign(UI::LA_BottomRight);
    GSCurrentFPSLabelLayer->VerticallyAlign(UI::LA_Center);
    GSCurrentFPSLabelLayer->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,GSNormText);
    GSCurrentFPSLabel->AddLayerToGroup(GSCurrentFPSLabelLayer,0,"Normal");
    GSCurrentFPSLabel->AddLayerToGroup(GSCurrentFPSLabelLayer,0,"Hovered");
    GSCurrentFPS->AddChild(GSCurrentFPSLabel,1);
    // Create the widget for displaying the current FPS value
    UI::Widget* GSCurrentFPSValue = GameScreen->CreateWidget("GS_CurrentFPSValue",UI::UnifiedRect(0.0,0.0,0.45,1.0));
    GSCurrentFPSValue->SetPositioningRules(UI::PF_Anchor_TopRight);
    UI::SingleLineTextLayer* GSCurrentFPSValueLayer = GSCurrentFPSValue->CreateSingleLineTextLayer(GameScreenText);
    GSCurrentFPSValueLayer->SetText("0");
    GSCurrentFPSValueLayer->HorizontallyAlign(UI::LA_TopLeft);
    GSCurrentFPSValueLayer->VerticallyAlign(UI::LA_Center);
    GSCurrentFPSValueLayer->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,GSNormText);
    GSCurrentFPSValue->AddLayerToGroup(GSCurrentFPSValueLayer,0,"Normal");
    GSCurrentFPSValue->AddLayerToGroup(GSCurrentFPSValueLayer,0,"Hovered");
    GSCurrentFPS->AddChild(GSCurrentFPSValue,2);
    // Wrap up timer configuration
    GameScreen->AddChild(GSCurrentFPS,5);

    // Create the widget container for the average FPS display
    UI::Widget* GSAverageFPS = GameScreen->CreateWidget("GS_AverageFPS",UI::UnifiedRect(0.008,0.105,0.21,0.065));
    GSAverageFPS->SetAspectRatioLock(UI::ARL_Ratio_Y_Axis);
    // Create the widget for displaying the average FPS explanation text
    UI::Widget* GSAverageFPSLabel = GameScreen->CreateWidget("GS_AverageFPSLabel",UI::UnifiedRect(0.0,0.0,0.55,1.0));
    GSAverageFPSLabel->SetPositioningRules(UI::PF_Anchor_TopLeft);
    UI::SingleLineTextLayer* GSAverageFPSLabelLayer = GSAverageFPSLabel->CreateSingleLineTextLayer(GameScreenText);
    GSAverageFPSLabelLayer->SetText("Average FPS: ");
    GSAverageFPSLabelLayer->HorizontallyAlign(UI::LA_BottomRight);
    GSAverageFPSLabelLayer->VerticallyAlign(UI::LA_Center);
    GSAverageFPSLabelLayer->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,GSNormText);
    GSAverageFPSLabel->AddLayerToGroup(GSAverageFPSLabelLayer,0,"Normal");
    GSAverageFPSLabel->AddLayerToGroup(GSAverageFPSLabelLayer,0,"Hovered");
    GSAverageFPS->AddChild(GSAverageFPSLabel,1);
    // Create the widget for displaying the average FPS value
    UI::Widget* GSAverageFPSValue = GameScreen->CreateWidget("GS_AverageFPSValue",UI::UnifiedRect(0.0,0.0,0.45,1.0));
    GSAverageFPSValue->SetPositioningRules(UI::PF_Anchor_TopRight);
    UI::SingleLineTextLayer* GSAverageFPSValueLayer = GSAverageFPSValue->CreateSingleLineTextLayer(GameScreenText);
    GSAverageFPSValueLayer->SetText("0");
    GSAverageFPSValueLayer->HorizontallyAlign(UI::LA_TopLeft);
    GSAverageFPSValueLayer->VerticallyAlign(UI::LA_Center);
    GSAverageFPSValueLayer->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,GSNormText);
    GSAverageFPSValue->AddLayerToGroup(GSAverageFPSValueLayer,0,"Normal");
    GSAverageFPSValue->AddLayerToGroup(GSAverageFPSValueLayer,0,"Hovered");
    GSAverageFPS->AddChild(GSAverageFPSValue,2);
    // Wrap up timer configuration
    GameScreen->AddChild(GSAverageFPS,6);

    ////-------------------  End Report  -------------------////
    // Create the "window" that will display the level report
    UI::Widget* GSLevelReport = GameScreen->CreateWidget("GS_LevelReport",UI::UnifiedRect(0.18,0.18,0.64,0.64));
    GSLevelReport->SetAspectRatioLock(UI::ARL_Ratio_Y_Axis);
    UI::ImageLayer* GSLevelReportLayer = GSLevelReport->CreateImageLayer("GSOptionsBackground");
    GSLevelReport->AddLayerToGroup(GSLevelReportLayer,0,"Normal");
    GSLevelReport->AddLayerToGroup(GSLevelReportLayer,0,"Hovered");
    GSLevelReport->Hide();
    GameScreen->AddChild(GSLevelReport,9);

    // Create the total score display
    UI::Widget* GSLevelReportScore = GameScreen->CreateWidget("GS_LevelReportScore",UI::UnifiedRect(0.0,0.08,0.34,0.125));
    GSLevelReportScore->SetPositioningRules(UI::PF_Anchor_HorizontalCenter);
    UI::ImageLayer* GSLevelReportScoreLayer = GSLevelReportScore->CreateImageLayer("GSAppExitButton");
    UI::SingleLineTextLayer* GSLevelReportScoreText = GSLevelReportScore->CreateSingleLineTextLayer(GameScreenText);
    GSLevelReportScoreText->SetText("0000");
    GSLevelReportScoreText->HorizontallyAlign(UI::LA_Center);
    GSLevelReportScoreText->VerticallyAlign(UI::LA_Center);
    GSLevelReportScoreText->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,GSLargeText);
    GSLevelReportScore->AddLayerToGroup(GSLevelReportScoreLayer,0,"Normal");
    GSLevelReportScore->AddLayerToGroup(GSLevelReportScoreText,1,"Normal");
    GSLevelReportScore->AddLayerToGroup(GSLevelReportScoreLayer,0,"Hovered");
    GSLevelReportScore->AddLayerToGroup(GSLevelReportScoreText,1,"Hovered");
    GSLevelReport->AddChild(GSLevelReportScore,1);

    UI::VerticalContainer* GSLevelReportBreakdown = GameScreen->CreateVerticalContainer("GS_LevelReportBreakdown",UI::UnifiedRect(0.15,0.245,0.70,0.52));
    GSLevelReportBreakdown->SetPadding(UI::UnifiedDim(0.05,0.0));
    UI::ImageLayer* GSLevelReportBreakdownLayer = GSLevelReportBreakdown->CreateImageLayer("GSBreakdownBackground");
    GSLevelReportBreakdown->AddLayerToGroup(GSLevelReportBreakdownLayer,0,"Normal");
    GSLevelReportBreakdown->AddLayerToGroup(GSLevelReportBreakdownLayer,0,"Hovered");
    GSLevelReport->AddChild(GSLevelReportBreakdown,2);

    UI::Button* GSLevelReportFinish = GameScreen->CreateButton("GS_LevelReportFinish",UI::UnifiedRect(0.21,0.815,0.25,0.125));
    UI::ImageLayer* GSLevelReportFinishNormal = GSLevelReportFinish->CreateImageLayer("GSStoreButton");
    UI::ImageLayer* GSLevelReportFinishHovered = GSLevelReportFinish->CreateImageLayer("GSStoreHoveredButton");
    UI::SingleLineTextLayer* GSLevelReportFinishText = GSLevelReportFinish->CreateSingleLineTextLayer(GameScreenText);
    GSLevelReportFinishText->SetText("Finish");
    GSLevelReportFinishText->HorizontallyAlign(UI::LA_Center);
    GSLevelReportFinishText->VerticallyAlign(UI::LA_Center);
    GSLevelReportFinishText->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,GSNormText);
    GSLevelReportFinish->AddLayerToGroup(GSLevelReportFinishNormal,0,"Normal");
    GSLevelReportFinish->AddLayerToGroup(GSLevelReportFinishText,1,"Normal");
    GSLevelReportFinish->AddLayerToGroup(GSLevelReportFinishHovered,0,"Hovered");
    GSLevelReportFinish->AddLayerToGroup(GSLevelReportFinishText,1,"Hovered");
    GSLevelReport->AddChild(GSLevelReportFinish,3);

    UI::Button* GSLevelReportRetry = GameScreen->CreateButton("GS_LevelReportRetry",UI::UnifiedRect(0.54,0.815,0.25,0.125));
    UI::ImageLayer* GSLevelReportRetryNormal = GSLevelReportRetry->CreateImageLayer("GSStoreButton");
    UI::ImageLayer* GSLevelReportRetryHovered = GSLevelReportRetry->CreateImageLayer("GSStoreHoveredButton");
    UI::SingleLineTextLayer* GSLevelReportRetryText = GSLevelReportRetry->CreateSingleLineTextLayer(GameScreenText);
    GSLevelReportRetryText->SetText("Retry");
    GSLevelReportRetryText->HorizontallyAlign(UI::LA_Center);
    GSLevelReportRetryText->VerticallyAlign(UI::LA_Center);
    GSLevelReportRetryText->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,GSNormText);
    GSLevelReportRetry->AddLayerToGroup(GSLevelReportRetryNormal,0,"Normal");
    GSLevelReportRetry->AddLayerToGroup(GSLevelReportRetryText,1,"Normal");
    GSLevelReportRetry->AddLayerToGroup(GSLevelReportRetryHovered,0,"Hovered");
    GSLevelReportRetry->AddLayerToGroup(GSLevelReportRetryText,1,"Hovered");
    GSLevelReport->AddChild(GSLevelReportRetry,4);
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
