#ifndef _catchapp_cpp
#define _catchapp_cpp

#include "catchapp.h"
#include "callbacks.h"
#include "buttoncallbacks.h"
#include "levelselectcell.h"
#include "throwablegenerator.h"
#include "widgetcallbacks.h"
#include <cassert>
#include <set>

using namespace phys;

CatchApp* CatchApp::TheRealCatchApp = 0;

CatchApp::CatchApp()
    : CurrScore(0),
      LastActorThrown(NULL),
      EndTimer(NULL),
      CurrentState(CatchApp::Catch_Init),
      PlaneOfPlay(Plane(Vector3(2.0,1.0,0.0), Vector3(1.0,2.0,0.0), Vector3(1.0,1.0,0.0)))
{
    assert(0==CatchApp::TheRealCatchApp);
    CatchApp::TheRealCatchApp = this;

    try
    {
        PhysicsConstructionInfo Info;
        Info.PhysicsFlags = (PhysicsConstructionInfo::PCF_LimitlessWorld | PhysicsConstructionInfo::PCF_SoftRigidWorld);
        TheWorld = new World( Info, SceneManager::Generic );
    }catch( exception x){
        //could not create world
    }
    Loader = new LevelLoader();
}

CatchApp::~CatchApp()
{
    delete TheWorld;
    delete Loader;
    CatchApp::TheRealCatchApp = NULL;
}

void CatchApp::MakeGUI()
{
    UIManager* GUI = TheWorld->GetUIManager();
    Viewport* UIViewport = TheWorld->GetGraphicsManager()->GetPrimaryGameWindow()->GetViewport(0);

    ColourValue Transparent(0.0,0.0,0.0,0.0);
    ColourValue Black(0.0,0.0,0.0,1.0);
    ColourValue TransBlack(0.0,0.0,0.0,0.85);
    ColourValue Gray(0.2,0.2,0.2,1.0);

    //Make the Main Menu screen and associated layers.
    GUI->LoadGorilla("Catch_Menu");
    UI::Screen* MainMenuScreen = GUI->CreateScreen("MainMenuScreen", "Catch_Menu", UIViewport);
    UI::Layer* MainMenuLayer = MainMenuScreen->CreateLayer("MainMenuLayer",0);

    //Build the Main Menu Screen
    UI::Rectangle* Background = MainMenuLayer->CreateRectangle( UI::RenderableRect(Vector2(0,0), Vector2(1,1), true));
    Background->SetBackgroundSprite("MainMenuBackground");

    UI::Menu* MainMenuMenu = MainMenuLayer->CreateMenu( "MS_Menu", UI::RenderableRect(Vector2(0.0,0.915), Vector2(1.0,0.086), true));
    MainMenuMenu->GetRootWindow()->GetWindowBack()->SetBackgroundSprite("MMBrickBackground");
    MainMenuMenu->GetRootWindow()->SetAutoHide(false);

    Real MMTextLineHeight = 0.04;
    Real MMSmallTextLineHeight = 0.03;
    //std::pair<Whole,Real> MainMenuText = GUI->SuggestGlyphIndex(0.04 * MainMenuScreen->GetViewportDimensions().Y,MainMenuScreen->GetPrimaryAtlas());
    UI::TextButton* LevelSelectAccess = MainMenuMenu->GetRootWindow()->CreateAccessorButton( "MS_LevelSelect", UI::RenderableRect(Vector2(0.05, 0.93), Vector2(0.22, 0.06), true), MMTextLineHeight, "Level Select" );
    LevelSelectAccess->SetBackgroundSprite("MMButton");
    LevelSelectAccess->SetHoveredSprite("MMHoveredButton");
    UI::MenuWindow* LevelSelectWin = MainMenuMenu->GetRootWindow()->CreateChildMenuWindow("MS_LevelSelect", UI::RenderableRect(Vector2(0.05,0.02), Vector2(0.90,0.84), true), LevelSelectAccess);
    LevelSelectWin->GetWindowBack()->SetBackgroundSprite("MMLSBackground");
    UI::PagedCellGrid* LevelSelectGrid = LevelSelectWin->CreatePagedCellGrid("MS_LevelGrid", UI::RenderableRect(Vector2(0.14,0.14), Vector2(0.72,0.66), true), UI::RenderableRect(Vector2(0.60,0.85), Vector2(0.24,0.06), true), UI::Spn_Separate, 0.05);
    LevelSelectGrid->GetGridBack()->SetBackgroundColour(Transparent);
    LevelSelectGrid->GetPageSpinner()->GetIncrement()->SetBackgroundSprite("MMIncrementPage");
    LevelSelectGrid->GetPageSpinner()->GetDecrement()->SetBackgroundSprite("MMDecrementPage");
    LevelSelectGrid->GetPageSpinner()->GetValueDisplay()->SetBackgroundSprite("MMPageBox");
    PopulateLevelList(LevelSelectGrid);
    LevelSelectGrid->GenerateGrid();
    UI::TextButton* LevelStart = LevelSelectWin->CreateTextButton("MS_LevelStart", UI::RenderableRect(Vector2(0.42,0.85), Vector2(0.16,0.07), true),Whole(18),"Start");
    LevelStart->SetButtonCallback(new MSStart(LevelSelectGrid));
    LevelStart->SetBackgroundSprite("MMLevelStart");
    LevelStart->SetHoveredSprite("MMLevelStartHovered");

    UI::TextButton* OptionsAccess = MainMenuMenu->GetRootWindow()->CreateAccessorButton( "MS_Options", UI::RenderableRect(Vector2(0.28, 0.93), Vector2(0.22, 0.06), true), MMTextLineHeight, "Options" );
    OptionsAccess->SetBackgroundSprite("MMButton");
    OptionsAccess->SetHoveredSprite("MMHoveredButton");
    UI::MenuWindow* OptionsWin = MainMenuMenu->GetRootWindow()->CreateChildMenuWindow("MS_Options", UI::RenderableRect(Vector2(0.18, 0.22), Vector2(0.64, 0.55), true), OptionsAccess);
    OptionsWin->GetWindowBack()->SetBackgroundSprite("MMOptionsBackground");
    UI::TabSet* OptionsTabSet = OptionsWin->CreateTabSet("MS_OptionsTS", UI::RenderableRect(Vector2(0.20, 0.31), Vector2(0.60, 0.39), true));
    //video options
    UI::RenderableSetData* VideoSet = OptionsTabSet->CreateRenderableSet("MS_VideoSet",UI::RenderableRect(Vector2(0.25, 0.24),Vector2(0.22, 0.06),true),MMTextLineHeight,"Video Options");
    VideoSet->Collection->GetWidgetBack()->SetBackgroundColour(ColourValue::Transparent());
    VideoSet->Accessor->SetBackgroundSprite("MMButton");
    VideoSet->Accessor->SetHoveredSprite("MMHoveredButton");
    UI::Caption* ResolutionLabel = VideoSet->Collection->CreateCaption("MS_ResolutionLabel",UI::RenderableRect(Vector2(0.28, 0.31),Vector2(0.24, 0.05),true),MMTextLineHeight,"Video Resolution");
    ResolutionLabel->SetBackgroundSprite("MMButton");
    UI::DropDownList* ResolutionList = VideoSet->Collection->CreateDropDownList("MS_ResolutionList",UI::RenderableRect(Vector2(0.25, 0.36),Vector2(0.30, 0.05),true),MMTextLineHeight,UI::SB_Separate);
    ResolutionList->GetSelection()->SetBackgroundSprite("MMListSelection");
    ResolutionList->GetListToggle()->SetBackgroundSprite("MMListScrollDown");
    ResolutionList->GetSelectionList()->GetBoxBack()->SetBackgroundSprite("MMListBackground");
    ResolutionList->GetSelectionList()->GetVertScroll()->GetScrollBack()->SetBackgroundSprite("MMListScrollBackground");
    ResolutionList->GetSelectionList()->GetVertScroll()->GetScroller()->SetBackgroundSprite("MMListScroller");
    ResolutionList->GetSelectionList()->GetVertScroll()->GetScroller()->SetHoveredSprite("MMHoveredListScroller");
    ResolutionList->GetSelectionList()->GetVertScroll()->GetDownRightButton()->SetBackgroundSprite("MMListScrollDown");
    ResolutionList->GetSelectionList()->GetVertScroll()->GetDownRightButton()->SetHoveredSprite("MMHoveredListScrollDown");
    ResolutionList->GetSelectionList()->GetVertScroll()->GetUpLeftButton()->SetBackgroundSprite("MMListScrollUp");
    ResolutionList->GetSelectionList()->GetVertScroll()->GetUpLeftButton()->SetHoveredSprite("MMHoveredListScrollUp");
    ResolutionList->GetSelectionList()->GetVertScroll()->SetIncrementDistance(0.05);
    ResolutionList->GetSelectionList()->SetTemplateBackgroundColour(ColourValue(1.0,1.0,1.0,0.2));
    ResolutionList->GetSelectionList()->SetTemplateTextColour(ColourValue::White());
    ResolutionList->GetSelectionList()->SetMaxDisplayedSelections(4);
    ResolutionList->SetWidgetCallback(new OptsVideoRes());
    UI::CheckBox* FullscreenBox = VideoSet->Collection->CreateCheckBox("MS_FullscreenBox",UI::RenderableRect(Vector2(0.59, 0.36),Vector2(0.16, 0.05),true),MMTextLineHeight,"Fullscreen");
    FullscreenBox->GetLabel()->SetBackgroundSprite("MMAppExitButton");
    FullscreenBox->SetCheckedSprite("MMCheckboxChecked","MMHoveredCheckboxChecked");
    FullscreenBox->SetUncheckedSprite("MMCheckboxUnchecked","MMHoveredCheckboxUnchecked");
    UI::TextButton* VideoOptsApply = VideoSet->Collection->CreateTextButton("MS_VideoOptsApply",UI::RenderableRect(Vector2(0.68, 0.64), Vector2(0.10, 0.05), true), MMTextLineHeight, "Apply");
    VideoOptsApply->SetBackgroundSprite("MMOptionsApplyButton");
    VideoOptsApply->SetHoveredSprite("MMOptionsApplyHoveredButton");
    VideoOptsApply->SetButtonCallback(new OptsVideoApply(ResolutionList,FullscreenBox));
    //sound options
    Real ScrollerSize = 0.09;
    UI::RenderableSetData* AudioSet = OptionsTabSet->CreateRenderableSet("MS_AudioSet",UI::RenderableRect(Vector2(0.53, 0.24),Vector2(0.22, 0.06),true),MMTextLineHeight,"Sound Options");
    AudioSet->Collection->GetWidgetBack()->SetBackgroundColour(ColourValue::Transparent());
    AudioSet->Accessor->SetBackgroundSprite("MMButton");
    AudioSet->Accessor->SetHoveredSprite("MMHoveredButton");
    UI::Caption* MusicVolLabel = AudioSet->Collection->CreateCaption("MS_MusicVolLabel",UI::RenderableRect(Vector2(0.38, 0.31),Vector2(0.24, 0.05),true),MMTextLineHeight,"Music Volume");
    MusicVolLabel->SetBackgroundSprite("MMButton");
    UI::Scrollbar* MusicVol = AudioSet->Collection->CreateScrollbar("MS_MusicVolume",UI::RenderableRect(Vector2(0.30, 0.36),Vector2(0.40, 0.04),true),UI::SB_Separate);
    MusicVol->SetScrollerSize(ScrollerSize);
    MusicVol->SetIncrementDistance(ScrollerSize * 0.5);
    MusicVol->GetScrollBack()->SetBackgroundSprite("MMScrollBackground");
    MusicVol->GetScroller()->SetBackgroundSprite("MMScroller");
    MusicVol->GetScroller()->SetHoveredSprite("MMHoveredScroller");
    MusicVol->GetDownRightButton()->SetBackgroundSprite("MMScrollRight");
    MusicVol->GetDownRightButton()->SetHoveredSprite("MMHoveredScrollRight");
    MusicVol->GetUpLeftButton()->SetBackgroundSprite("MMScrollLeft");
    MusicVol->GetUpLeftButton()->SetHoveredSprite("MMHoveredScrollLeft");
    MusicVol->SetWidgetCallback(new OptsMusicVol());
    UI::Caption* EffectsVolLabel = AudioSet->Collection->CreateCaption("MS_EffectsVolLabel",UI::RenderableRect(Vector2(0.38, 0.42),Vector2(0.24, 0.05),true),MMTextLineHeight,"Effects Volume");
    EffectsVolLabel->SetBackgroundSprite("MMButton");
    UI::Scrollbar* EffectsVol = AudioSet->Collection->CreateScrollbar("MS_EffectsVolume",UI::RenderableRect(Vector2(0.30, 0.47),Vector2(0.40, 0.04),true),UI::SB_Separate);
    EffectsVol->SetScrollerSize(ScrollerSize);
    EffectsVol->SetIncrementDistance(ScrollerSize * 0.5);
    EffectsVol->GetScrollBack()->SetBackgroundSprite("MMScrollBackground");
    EffectsVol->GetScroller()->SetBackgroundSprite("MMScroller");
    EffectsVol->GetScroller()->SetHoveredSprite("MMHoveredScroller");
    EffectsVol->GetDownRightButton()->SetBackgroundSprite("MMScrollRight");
    EffectsVol->GetDownRightButton()->SetHoveredSprite("MMHoveredScrollRight");
    EffectsVol->GetUpLeftButton()->SetBackgroundSprite("MMScrollLeft");
    EffectsVol->GetUpLeftButton()->SetHoveredSprite("MMHoveredScrollLeft");
    EffectsVol->SetWidgetCallback(new OptsEffectVol());
    UI::Caption* DeviceLabel = AudioSet->Collection->CreateCaption("MS_DeviceLabel",UI::RenderableRect(Vector2(0.30, 0.55),Vector2(0.24, 0.05),true),MMTextLineHeight,"Audio Device");
    DeviceLabel->SetBackgroundSprite("MMButton");
    UI::DropDownList* DeviceList = AudioSet->Collection->CreateDropDownList("MS_AudioDeviceList",UI::RenderableRect(Vector2(0.27, 0.60),Vector2(0.30, 0.05),true),MMTextLineHeight,UI::SB_Separate);
    DeviceList->GetSelection()->SetBackgroundSprite("MMListSelection");
    DeviceList->GetListToggle()->SetBackgroundSprite("MMListScrollDown");
    DeviceList->GetSelectionList()->GetBoxBack()->SetBackgroundSprite("MMListBackground");
    DeviceList->GetSelectionList()->GetVertScroll()->GetScrollBack()->SetBackgroundSprite("MMListScrollBackground");
    DeviceList->GetSelectionList()->GetVertScroll()->GetScroller()->SetBackgroundSprite("MMListScroller");
    DeviceList->GetSelectionList()->GetVertScroll()->GetScroller()->SetHoveredSprite("MMHoveredListScroller");
    DeviceList->GetSelectionList()->GetVertScroll()->GetDownRightButton()->SetBackgroundSprite("MMListScrollDown");
    DeviceList->GetSelectionList()->GetVertScroll()->GetDownRightButton()->SetHoveredSprite("MMHoveredListScrollDown");
    DeviceList->GetSelectionList()->GetVertScroll()->GetUpLeftButton()->SetBackgroundSprite("MMListScrollUp");
    DeviceList->GetSelectionList()->GetVertScroll()->GetUpLeftButton()->SetHoveredSprite("MMHoveredListScrollUp");
    UI::CheckBox* MuteBox = AudioSet->Collection->CreateCheckBox("MS_MuteBox",UI::RenderableRect(Vector2(0.62, 0.60),Vector2(0.11, 0.05),true),MMTextLineHeight,"Mute");
    MuteBox->GetLabel()->SetBackgroundSprite("MMAppExitButton");
    MuteBox->SetCheckedSprite("MMCheckboxChecked","MMHoveredCheckboxChecked");
    MuteBox->SetUncheckedSprite("MMCheckboxUnchecked","MMHoveredCheckboxUnchecked");

    UI::TextButton* OptsBack = OptionsWin->CreateBackButton(UI::RenderableRect(Vector2(0.68, 0.70), Vector2(0.10, 0.05), true), MMTextLineHeight, "Back");
    OptsBack->SetBackgroundSprite("MMOptionsApplyButton");
    OptsBack->SetHoveredSprite("MMOptionsApplyHoveredButton");

    UI::TextButton* CreditsAccess = MainMenuMenu->GetRootWindow()->CreateAccessorButton( "MS_Credits", UI::RenderableRect(Vector2(0.51, 0.93), Vector2(0.22, 0.06), true), MMTextLineHeight, "Credits" );
    CreditsAccess->SetBackgroundSprite("MMButton");
    CreditsAccess->SetHoveredSprite("MMHoveredButton");
    UI::MenuWindow* CreditsWin = MainMenuMenu->GetRootWindow()->CreateChildMenuWindow("MS_Credits", UI::RenderableRect(Vector2(0.01,0.01), Vector2(0.01,0.01), true), CreditsAccess);

    UI::TextButton* AppExitAccess = MainMenuMenu->GetRootWindow()->CreateAccessorButton( "MS_AppExit", UI::RenderableRect(Vector2(0.74, 0.93), Vector2(0.22, 0.06), true), MMTextLineHeight, "Exit Game" );
    AppExitAccess->SetBackgroundSprite("MMButton");
    AppExitAccess->SetHoveredSprite("MMHoveredButton");
    UI::MenuWindow* AppExitWin = MainMenuMenu->GetRootWindow()->CreateChildMenuWindow("MS_AppExit", UI::RenderableRect(Vector2(0.25, 0.3), Vector2(0.5, 0.3), true), AppExitAccess);
    AppExitWin->GetWindowBack()->SetBackgroundSprite("MMAppExitBackground");
    std::pair<Whole,Real> AppExitText = GUI->SuggestGlyphIndex(0.04 * MainMenuScreen->GetViewportDimensions().Y,MainMenuScreen->GetPrimaryAtlas());
    UI::Caption* AppExitWarn = AppExitWin->CreateCaption("MS_AppExitWarn", UI::RenderableRect(Vector2(0.32, 0.35), Vector2(0.36, 0.07), true), AppExitText.first, "Are you sure you want to exit?");
    AppExitWarn->SetBackgroundSprite("MMAppExitText");
    UI::TextButton* AppExitConf = AppExitWin->CreateTextButton("MS_AppExitConf", UI::RenderableRect(Vector2(0.30, 0.47), Vector2(0.18, 0.06), true), AppExitText.first, "Yes");
    AppExitConf->SetButtonCallback(new AllAppExit());
    AppExitConf->SetBackgroundSprite("MMAppExitButton");
    AppExitConf->SetHoveredSprite("MMAppExitHoveredButton");
    UI::TextButton* AppExitDeny = AppExitWin->CreateBackButton(/*"MS_AppExitDeny", */UI::RenderableRect(Vector2(0.52, 0.47), Vector2(0.18, 0.06), true), AppExitText.first, "No");
    AppExitDeny->SetBackgroundSprite("MMAppExitButton");
    AppExitDeny->SetHoveredSprite("MMAppExitHoveredButton");
    //End of Main Menu Screen

    //Make the Game screen and associated layers.
    GUI->LoadGorilla("Catch_Game");
    UI::Screen* GameScreen = GUI->CreateScreen("GameScreen", "Catch_Game", UIViewport);
    UI::Layer* Menu = GameScreen->CreateLayer("MenuLayer", 10);
    UI::Layer* Report = GameScreen->CreateLayer("ReportLayer", 8);
    UI::Layer* ItemShop = GameScreen->CreateLayer("ItemShopLayer", 4);
    UI::Layer* Stats = GameScreen->CreateLayer("StatsLayer", 1);
    UI::Layer* HUD = GameScreen->CreateLayer("HUDLayer", 0);

    //Build the Game Screen
    //Build the HUD layer
    UI::Caption* Timer = HUD->CreateCaption( "GS_Timer", UI::RenderableRect(Vector2(0.8995, 0.006), Vector2(0.0965, 0.06), true), Whole(20), "0:00");
    Timer->SetBackgroundSprite("TimerArea");

    UI::Rectangle* TIcon = HUD->CreateRectangle( UI::RenderableRect(Vector2(0.8515, 0.006), Vector2(0.0482, 0.06), true));
    TIcon->SetBackgroundSprite("TimerLogo");

    UI::Button* MenuButton = HUD->CreateButton( "GS_Menu", UI::RenderableRect(Vector2(0.008, 0.922), Vector2(0.16, 0.065), true));
    MenuButton->SetButtonCallback(new GSMenu());
    MenuButton->SetBackgroundSprite("MenuButtonHiRes");

    UI::Button* StoreButton = HUD->CreateButton( "GS_Store", UI::RenderableRect(Vector2(0.922, 0.922), Vector2(0.06, 0.065), true));
    StoreButton->SetButtonCallback(new GSStore());
    StoreButton->SetBackgroundSprite("StoreButtonHiRes");

    UI::Rectangle* StoreText = HUD->CreateRectangle( UI::RenderableRect(Vector2(0.767, 0.922), Vector2(0.14, 0.065), true));
    StoreText->SetBackgroundSprite("StoreText");

    UI::Caption* ScoreAmount = HUD->CreateCaption( "GS_ScoreArea", UI::RenderableRect(Vector2(0.135, 0.006), Vector2(0.15, 0.065), true), Whole(20), "0");
    ScoreAmount->SetBackgroundColour(Transparent);

    UI::Rectangle* ScoreText = HUD->CreateRectangle( UI::RenderableRect(Vector2(0.008, 0.006), Vector2(0.12, 0.06), true));
    ScoreText->SetBackgroundSprite("ScoreText");
    //End of HUD Layer

    //Build the ItemShop Layer
    UI::Window* ItemShopWindow = ItemShop->CreateWidgetWindow("ItemShop", UI::RenderableRect(Vector2(0.25, 0.11), Vector2(0.5, 0.78125), true));
    ItemShopWindow->GetWindowBack()->SetBackgroundSprite("WindowVertBack");

    /*UI::ButtonListBox* ItemShopList = ItemShopWindow->CreateButtonListBox("StoreItemList",UI::RenderableRect(Vector2(0.28,0.54),Vector2(0.44,0.32), true),0.02,UI::SB_Separate);
    ItemShopList->SetAutoHideScroll(false);
    ItemShopList->GetBoxBack()->SetBackgroundColour(TransBlack);
    ItemShopList->GetVertScroll()->GetScrollBack()->SetBackgroundColour(TransBlack);//*/
    ItemShop->Hide();
    //End of ItemShop Layer

    //Build the Menu Layer
    UI::Menu* GameMenu = Menu->CreateMenu( "GameMenu", UI::RenderableRect(Vector2(0.35, 0.27), Vector2(0.3, 0.45), true));
    GameMenu->GetRootWindow()->GetWindowBack()->SetBackgroundSprite("MenuBack");
    UI::Button* ReturnButton = GameMenu->GetRootWindow()->CreateButton( "GS_Return", UI::RenderableRect(Vector2(0.37, 0.56), Vector2(0.26, 0.05), true));
    ReturnButton->SetButtonCallback(new GSReturn());
    ReturnButton->SetBackgroundSprite("ReturnButton");
    UI::Button* GameExitButton = GameMenu->GetRootWindow()->CreateButton( "GS_GameExit", UI::RenderableRect(Vector2(0.37, 0.64), Vector2(0.26, 0.05), true));
    GameExitButton->SetButtonCallback(new GSMMReturn());
    GameExitButton->SetBackgroundSprite("ExitButton");

    UI::Button* VideoAccess = GameMenu->GetRootWindow()->CreateAccessorButton("GS_VideoSettingsButton", UI::RenderableRect(Vector2(0.37, 0.32), Vector2(0.26, 0.05), true));
    UI::MenuWindow* VideoSettings = GameMenu->GetRootWindow()->CreateChildMenuWindow("VideoSettings", UI::RenderableRect(Vector2(0.18, 0.22), Vector2(0.64, 0.55), true), VideoAccess);
    VideoSettings->GetWindowBack()->SetBackgroundSprite("WindowHoriBack");
    VideoAccess->SetBackgroundSprite("VideoSetButton");

    UI::Button* VideoBack = VideoSettings->CreateBackButton(UI::RenderableRect(Vector2(0.72, 0.705), Vector2(0.09, 0.05), true));
    VideoBack->SetBackgroundColour(Black);

    UI::Button* SoundAccess = GameMenu->GetRootWindow()->CreateAccessorButton("GS_SoundSettingsButton", UI::RenderableRect(Vector2(0.37, 0.40), Vector2(0.26, 0.05), true));
    UI::MenuWindow* SoundSettings = GameMenu->GetRootWindow()->CreateChildMenuWindow("SoundSettings", UI::RenderableRect(Vector2(0.18, 0.22), Vector2(0.64, 0.55), true), SoundAccess);
    SoundSettings->GetWindowBack()->SetBackgroundSprite("WindowHoriBack");
    SoundAccess->SetBackgroundSprite("SoundSetButton");

    UI::Button* SoundBack = SoundSettings->CreateBackButton(UI::RenderableRect(Vector2(0.72, 0.705), Vector2(0.09, 0.05), true));
    SoundBack->SetBackgroundColour(Black);
    Menu->Hide();
    //End of Menu Layer

    //Build the Stats Layer
    UI::Caption* CurFPS = Stats->CreateCaption( "CurFPS", UI::RenderableRect(Vector2(0.16, 0.06), Vector2(0.06, 0.065), true), Whole(14), "0.0");
    CurFPS->SetBackgroundColour(Transparent);
    CurFPS->HorizontallyAlign(UI::Txt_Left);

    UI::Caption* CurFPSText = Stats->CreateCaption( "CurFPSText", UI::RenderableRect(Vector2(0.008, 0.06), Vector2(0.15, 0.065), true), Whole(14), "Current FPS: ");
    CurFPSText->SetBackgroundColour(Transparent);
    CurFPSText->HorizontallyAlign(UI::Txt_Left);

    UI::Caption* AvFPS = Stats->CreateCaption( "AvFPS", UI::RenderableRect(Vector2(0.16, 0.105), Vector2(0.06, 0.065), true), Whole(14), "0.0");
    AvFPS->SetBackgroundColour(Transparent);
    AvFPS->HorizontallyAlign(UI::Txt_Left);

    UI::Caption* AvFPSText = Stats->CreateCaption( "AvFPSText", UI::RenderableRect(Vector2(0.008, 0.105), Vector2(0.15, 0.065), true), Whole(14), "Average FPS: ");
    AvFPSText->SetBackgroundColour(Transparent);
    AvFPSText->HorizontallyAlign(UI::Txt_Left);
    //End of Stats Layer

    //Build the Report Layer
    UI::Window* LevelReport = Report->CreateWidgetWindow("LevelReport", UI::RenderableRect(Vector2(0.2, 0.2), Vector2(0.6, 0.6), true));
    LevelReport->GetWindowBack()->SetBackgroundColour(Gray);
    //TempCaption
    UI::Caption* TempCapt = LevelReport->CreateCaption("GS_TempWarning", UI::RenderableRect(Vector2(0.25, 0.3), Vector2(0.5, 0.3), true), Whole(18), "Future spot of level reports.");
    TempCapt->SetBackgroundColour(Transparent);
    UI::TextButton* FinishButton = LevelReport->CreateTextButton("GS_Finish", UI::RenderableRect(Vector2(0.42, 0.66), Vector2(0.16, 0.08), true), Whole(14), "Finish");
    FinishButton->SetButtonCallback(new GSMMReturn());
    FinishButton->SetBackgroundColour(TransBlack);
    Report->Hide();
    //End of Report Layer
    //End of Game Screen
    GameScreen->Hide();
}

void CatchApp::CreateLoadingScreen()
{
    UIManager* GUI = TheWorld->GetUIManager();
    GUI->LoadGorilla("Catch_Loading");
    Viewport* UIViewport = TheWorld->GetGraphicsManager()->GetPrimaryGameWindow()->GetViewport(0);
    UI::Screen* LoadScreen = GUI->CreateScreen("LoadingScreen", "Catch_Loading", UIViewport);
    UI::Layer* LoadLayer = LoadScreen->CreateLayer("LoadingLayer", 0);
    UI::Rectangle* Load = LoadLayer->CreateRectangle( UI::RenderableRect(Vector2(0, 0), Vector2(1, 1), true));
    Load->SetBackgroundSprite("BTSBanner");
    //LoadScreen->RenderOnce();
    crossplatform::RenderPhysWorld();
    Load->SetBackgroundSprite("LoadingBackground");
}

void CatchApp::ConfigResources()
{
    ResourceManager* ResourceMan = TheWorld->GetResourceManager();
    String CommonGroup("Common");
    String datadir = crossplatform::GetDataDirectory();
    ResourceMan->AddResourceLocation(datadir, "FileSystem", CommonGroup, false);
    ResourceMan->AddResourceLocation(datadir+"Common.zip", "Zip", CommonGroup, false);
    ResourceMan->AddResourceLocation(datadir+"AdvThrowables.zip", "Zip", CommonGroup, false);
    ResourceMan->AddResourceLocation("Previews/", "FileSystem", CommonGroup, false);
    ResourceMan->InitResourceGroup(CommonGroup);
    /// @todo Change this to parse an actual file
    ThrowableGenerator::ParseThrowables("");
}

void CatchApp::PopulateScoreValues()
{
    ItemScoreValues["Gold"] = 100;
    ItemScoreValues["Pyrite"] = 50;
    ItemScoreValues["Clay"] = 10;
    ItemScoreValues["Uranium"] = 100;
    ItemScoreValues["Rubber"] = 30;
    ItemScoreValues["Lead"] = 30;
    ItemScoreValues["Styrofoam"] = 10;
    ItemScoreValues["Wood"] = 20;
}

void CatchApp::PopulateShopValues()
{
    ShopCostValues["Wooden Plank"] = 50;
}

void CatchApp::PopulateLevelList(UI::PagedCellGrid* Grid)
{
    ResourceManager* ResourceMan = World::GetWorldPointer()->GetResourceManager();
    UIManager* UIMan = World::GetWorldPointer()->GetUIManager();
    ResourceMan->AddResourceLocation("/Previews","FileSystem","Previews",false);
    ResourceMan->InitResourceGroup("Previews");
    UI::RenderableRect CellRect(Vector2(0.1,0.1),Vector2(0.33,0.11),true);
    Vector2 CellSpacing(0.06,0.04);
    Grid->SetFixedCellSize(CellRect.Size);
    Grid->SetCellSpacing(CellSpacing);
    UI::Layer* ParentLayer = Grid->GetLayer();

    std::set<String>* Files = crossplatform::GetDirContents("./Levels");
    if(Files->empty())
        return;
    std::set<String>* Previews = crossplatform::GetDirContents("./Previews");
    for( std::set<String>::iterator it = Files->begin() ; it != Files->end() ; it++ )
    {
        const String& FileName = (*it);
        if(String::npos == FileName.find(".lvl"))
            continue;

        String LevelName = FileName.substr(0,(*it).size() - 4);
        LevelSelectCell* CurrCell = new LevelSelectCell(LevelName,CellRect,ParentLayer);
        CurrCell->GetCellBack()->SetBackgroundSprite("MMLevelCellBack");
        CurrCell->GetPreviewBorder()->SetBackgroundSprite("MMLevelPreviewBox");
        CurrCell->GetLevelTitle()->SetText(LevelName);
        CurrCell->GetLevelTitle()->SetBackgroundColour(ColourValue(0.0,0.0,0.0,0.0));
        /// @todo Here's where we would get existing scores and put them in the cells.
        /*String Score;
        String MaxScore;
        String EarnedScore;
        Score+=(EarnedScore+"/"+MaxScore);
        CurrCell->GetEarnedMaxScore()->SetText(Score);// */
        CurrCell->GetEarnedMaxScore()->SetBackgroundColour(ColourValue(0.0,0.0,0.0,0.0));
        if(Previews->count(LevelName+".gorilla"))
        {
            UIMan->LoadGorilla(LevelName);
            CurrCell->GetPreviewImage()->SetBackgroundSprite("LevelPreview",LevelName);
        }
        CurrCell->SetCellCallback(new LevelSelectCB());
        Grid->AddCell(CurrCell);
    }
}

void CatchApp::ChangeState(const CatchApp::GameState &StateToSet)
{
    if(CurrentState == StateToSet)
        return;
    UIManager* UIMan = TheWorld->GetUIManager();
    switch (StateToSet)
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
    }
    CurrentState = StateToSet;
}

bool CatchApp::CheckEndOfLevel()
{
    if(ScoreAreas.empty())
        return false;
    if(!EndTimer)
    {
        EndTimer = TheWorld->GetTimerManager()->CreateSimpleTimer(Timer::StopWatch);
        EndTimer->SetInitialTime(5 * 1000000);
        EndTimer->SetCurrentTime(5 * 1000000);
        EndTimer->SetGoalTime(0);
        EndTimer->Start();
    }
    for( Whole SA = 0 ; SA < ScoreAreas.size() ; SA++ )
    {
        std::vector<ActorBase*>& FinalTest = ScoreAreas[SA]->GetAddedActors();
        for( Whole F = 0 ; F < FinalTest.size() ; F++ )
        {
            if(LastActorThrown == FinalTest[F])
                EndTimer->Reset();
        }
    }
    if(EndTimer->IsStopped())
        return true;
    else
        return false;
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

bool CatchApp::IsInsideAnyStartZone(ActorBase* Actor)
{
    if(StartAreas.empty())
        return false;
    for( Whole X = 0 ; X < StartAreas.size() ; X++ )
    {
        if(StartAreas[X]->IsInside(Actor))
            return true;
    }
    return false;
}

void CatchApp::UnloadLevel()
{
    if("MainMenu"==Loader->GetCurrentLevel())
        return;
    PhysicsManager* PhysMan = TheWorld->GetPhysicsManager();
    ResourceManager* ResMan = TheWorld->GetResourceManager();
    SceneManager* SceneMan = TheWorld->GetSceneManager();
    ActorManager* ActorMan = TheWorld->GetActorManager();
    TimerManager* TimeMan = TheWorld->GetTimerManager();
    EventManager* EventMan = TheWorld->GetEventManager();
    CollisionShapeManager* CShapeMan = TheWorld->GetCollisionShapeManager();
    MeshManager* MeshMan = TheWorld->GetMeshManager();
    UIManager* UIMan = TheWorld->GetUIManager();

    PhysMan->DestroyAllConstraints();
    ActorMan->DestroyAllActors();
    PhysMan->DestroyAllWorldTriggers();
    PhysMan->DestroyAllAreaEffects();
    CShapeMan->DestroyAllShapes();
    MeshMan->DestroyAllGeneratedMeshes();
    SceneMan->DestroyAllLights();
    SceneMan->DestroyAllParticleEffects();
    SceneMan->DestroyAllWorldNodes();
    SceneMan->DisableSky();
    //ActorMan->DestroyAllActors();
    StartAreas.clear();
    ScoreAreas.clear();
    PlayZone = NULL;

    ResMan->DestroyResourceGroup(Loader->GetCurrentLevel());
    PhysMan->ClearPhysicsMetaData();
    /// @todo Probably should make a "RemoveAll" for the events as well.
    for( EventCollision* OneCollision = EventMan->PopNextCollisionEvent() ; NULL != OneCollision ;  )
    {
        delete OneCollision;
        OneCollision = EventMan->PopNextCollisionEvent();
    }
    CurrScore = 0;
    TimeMan->DestroyTimer(EndTimer);
    EndTimer = NULL;
    UIMan->GetLayer("ReportLayer")->Hide();
    UIMan->GetLayer("MenuLayer")->Hide();
    PhysMan->PauseSimulation(false);
}

CatchApp* CatchApp::GetCatchAppPointer()
{
    assert(0!=CatchApp::TheRealCatchApp);
    return TheRealCatchApp;
}

int CatchApp::GetCatchin()
{
    //Give the world functions to run before and after input and physics
    TheWorld->GetEventManager()->SetPreMainLoopItems(&CPreInput);
    TheWorld->GetEventManager()->SetPostMainLoopItems(&CPostInput);
    TheWorld->GetPhysicsManager()->SetPreMainLoopItems(&CPrePhysics);
    TheWorld->GetPhysicsManager()->SetPostMainLoopItems(&CPostPhysics);
    TheWorld->GetGraphicsManager()->SetPostMainLoopItems(&CPostRender);
    TheWorld->GetUIManager()->SetPreMainLoopItems(&CPreUI);
    TheWorld->GetUIManager()->SetPostMainLoopItems(&CPostUI);

    //Set the Make the RenderWindow and load system stuff
	TheWorld->GameInit(false);

	TheWorld->GetUIManager()->EnableButtonAutoRegister(true);
    TheWorld->GetUIManager()->AddAutoRegisterCode(MetaCode::MOUSEBUTTON_1);

    ConfigResources();
	CreateLoadingScreen();
	ChangeState(CatchApp::Catch_Loading);

	PopulateScoreValues();
    PopulateShopValues();

    //Set logging frequency
    TheWorld->SetLoggingFrequency(World::LogNever);

	// Set the Title
    TheWorld->GetGraphicsManager()->GetPrimaryGameWindow()->SetWindowCaption("Catch!");
    TheWorld->SetTargetFrameRate(60);

    // Create the Timer
    LevelTimer = TheWorld->GetTimerManager()->CreateSimpleTimer(Timer::Normal);

    //Generate the UI
    MakeGUI();

    Loader->SetNextLevel("MainMenu");
    do{
        ChangeState(CatchApp::Catch_Loading);
        crossplatform::RenderPhysWorld();
        //Actually Load the game stuff
        Loader->LoadLevel();

        if("MainMenu"==Loader->GetCurrentLevel())
            ChangeState(CatchApp::Catch_MenuScreen);
        else
            ChangeState(CatchApp::Catch_GameScreen);
        LevelTimer->Reset();
        LevelTimer->Start();

        //if("Ferris"==Loader->GetCurrentLevel())
        //    TheWorld->GetUIManager()->GetScreen("GameScreen")->Hide();
        //TheWorld->GetPhysicsManager()->PauseSimulation(true);
        //Start the Main Loop
        TheWorld->MainLoop();
        UnloadLevel();
        LevelTimer->Stop();
    } while(Loader->HasALevelToLoad());

	return 0;
}

bool CatchApp::PreInput()
{
    // using the Raw Event Manager, and deleting the events
    if( !CheckForStuff() )
        return false;
    return true;
}

bool CatchApp::PostInput()
{
    CameraController* DefaultControl = TheWorld->GetCameraManager()->GetOrCreateCameraController(TheWorld->GetCameraManager()->GetDefaultCamera());
    if( InputQueryTool::IsKeyboardButtonPushed(MetaCode::KEY_LEFT) || InputQueryTool::IsKeyboardButtonPushed(MetaCode::KEY_A))
        DefaultControl->StrafeLeft(300 * (TheWorld->GetFrameTime() * 0.001));
    if( InputQueryTool::IsKeyboardButtonPushed(MetaCode::KEY_RIGHT) || InputQueryTool::IsKeyboardButtonPushed(MetaCode::KEY_D))
        DefaultControl->StrafeRight(300 * (TheWorld->GetFrameTime() * 0.001));
    if( InputQueryTool::IsKeyboardButtonPushed(MetaCode::KEY_UP) || InputQueryTool::IsKeyboardButtonPushed(MetaCode::KEY_W))
        DefaultControl->MoveForward(300 * (TheWorld->GetFrameTime() * 0.001));
    if( InputQueryTool::IsKeyboardButtonPushed(MetaCode::KEY_DOWN)  || InputQueryTool::IsKeyboardButtonPushed(MetaCode::KEY_S))
        DefaultControl->MoveBackward(300 * (TheWorld->GetFrameTime() * 0.001));
    static bool MouseCam=false;
    if( InputQueryTool::IsKeyboardButtonPushed(MetaCode::KEY_HOME) )
        MouseCam=true;
    if( InputQueryTool::IsKeyboardButtonPushed(MetaCode::KEY_END))
        MouseCam=false;
    Vector2 Offset = InputQueryTool::GetMousePrevFrameOffset();
    if( MouseCam && Vector2(0,0) != Offset )
        DefaultControl->Rotate(Offset.X * 0.01,Offset.Y * 0.01,0);

    if( MetaCode::BUTTON_PRESSING == InputQueryTool::GetKeyboardButtonState(MetaCode::KEY_C) )
    {
        if(TheWorld->GetPhysicsManager()->GetDebugPhysicsRendering())
        {
            TheWorld->GetPhysicsManager()->SetDebugPhysicsWireCount(0);
            TheWorld->GetPhysicsManager()->SetDebugPhysicsRendering(0);
        }else{
            TheWorld->GetPhysicsManager()->SetDebugPhysicsWireCount(2);
            TheWorld->GetPhysicsManager()->SetDebugPhysicsRendering(1);
        }
    }

    return true;
}

bool CatchApp::PreUI()
{
    return true;
}

bool CatchApp::PostUI()
{
    //User Input through a WorldQueryTool
    static RayQueryTool* RayQueryer = new RayQueryTool();
    static InputQueryTool* InputQueryer = new InputQueryTool();

    //Queryer.GatherEvents();
    TheWorld->Log("Mouse location From WorldQueryTool X/Y");
    TheWorld->Log(InputQueryer->GetMouseX());
    TheWorld->Log(InputQueryer->GetMouseY());

    static Point2PointConstraint* Dragger=NULL;

    if( InputQueryer->IsMouseButtonPushed(1) )
    {
        if(!TheWorld->GetUIManager()->MouseIsInUISystem())
        {
            Ray *MouseRay = RayQueryer->GetMouseRay(5000);
            //*MouseRay *= 1000;
            //Ray *MouseRay = new Ray(Vector3(500.0, 0.0, 0.0),Vector3(-500.0, 0.0, 0.0));

            Vector3WActor *ClickOnActor = RayQueryer->GetFirstActorOnRayByPolygon(*MouseRay,phys::WOT_ActorRigid);
            #ifdef PHYSDEBUG
            TheWorld->LogStream << "MouseRay: " << *MouseRay << "| Length: " << MouseRay->Length() << endl;
            TheWorld->Log("PlaneOfPlay"); TheWorld->Log(PlaneOfPlay);
            #endif

            //ActorBase *temp = ClickOnActor->Actor;

            bool firstframe=false;
            if(0 == ClickOnActor || 0 == ClickOnActor->Actor)
            {
                #ifdef PHYSDEBUG
                TheWorld->Log("No Actor Clicked on");
                #endif
            }else if(!IsInsideAnyStartZone(ClickOnActor->Actor)){
                #ifdef PHYSDEBUG
                TheWorld->Log("Actor is not in any starting zone");
                #endif
            }else{
                #ifdef PHYSDEBUG
                TheWorld->Log("Actor Clicked on");
                TheWorld->Log(*ClickOnActor);
                TheWorld->Log(*ClickOnActor);
                //TheWorld->Log("ClickOnActor"); TheWorld->Log(*ClickOnActor);
                #endif
                if(!(ClickOnActor->Actor->IsStaticOrKinematic()))
                {
                    if(!Dragger) //If we have a dragger, then this is dragging, not clicking
                    {
                        if(ClickOnActor->Actor->GetType()==ActorBase::Actorrigid) //This is Dragging let's do some checks for sanity
                        {
                            Vector3 LocalPivot = ClickOnActor->Vector;
                            ActorRigid* rigid = static_cast<ActorRigid*>(ClickOnActor->Actor);
                            rigid->GetPhysicsSettings()->SetActivationState(phys::AAS_DisableDeactivation);
                            //Dragger = new Generic6DofConstraint(rigid, LocalPivot, Quaternion(0,0,0,1), false);
                            Dragger = new Point2PointConstraint(rigid, LocalPivot);
                            Dragger->SetTAU(0.001);
                            //Dragger->SetLinearLowerLimit(Vector3(0.f,0.f,0.f));
                            //Dragger->SetLinearUpperLimit(Vector3(0.f,0.f,0.f));
                            //Dragger->SetAngularLowerLimit(Vector3(0.f,0.f,0.f));
                            //Dragger->SetAngularUpperLimit(Vector3(0.f,0.f,0.f));
                            TheWorld->GetPhysicsManager()->AddConstraint(Dragger);
                            Dragger->SetParam(Con_Stop_CFM,0.8,0); Dragger->SetParam(Con_Stop_CFM,0.8,1); Dragger->SetParam(Con_Stop_CFM,0.8,2); //Dragger->SetParam(4,0.8,3); Dragger->SetParam(4,0.8,4); Dragger->SetParam(4,0.8,5);
                            Dragger->SetParam(Con_Stop_ERP,0.1,0); Dragger->SetParam(Con_Stop_ERP,0.1,1); Dragger->SetParam(Con_Stop_ERP,0.1,2); //Dragger->SetParam(2,0.1,3); Dragger->SetParam(2,0.1,4); Dragger->SetParam(2,0.1,5);
                            firstframe=true;
                            LastActorThrown = rigid;
                        }else{  // since we don't
                            #ifdef PHYSDEBUG
                            TheWorld->Log("Actor is not an ActorRigid.  Aborting.");
                            #endif
                        }
                    }
                }else{
                    #ifdef PHYSDEBUG
                    TheWorld->Log("Actor is Static/Kinematic.  Aborting.");
                    #endif
                }
            }

            // This chunk of code calculates the 3d point that the actor needs to be dragged to
            Vector3 *DragTo = RayQueryer->RayPlaneIntersection(*MouseRay, PlaneOfPlay);
            if (0 == DragTo)
            {
                #ifdef PHYSDEBUG
                TheWorld->Log("PlaneOfPlay Not Clicked on");
                #endif
            }else{
                if(Dragger && !firstframe)
                {
                    #ifdef PHYSDEBUG
                    TheWorld->Log("Dragged To");
                    TheWorld->Log(*DragTo);
                    #endif
                    Dragger->SetPivotBLocation(*DragTo);
                }
            }

            if(Dragger && !IsInsideAnyStartZone(LastActorThrown))
            {
                ActorRigid* Act = Dragger->GetActorA();
                TheWorld->GetPhysicsManager()->RemoveConstraint(Dragger);
                delete Dragger;
                Dragger=NULL;
                Act->GetPhysicsSettings()->SetActivationState(phys::AAS_DisableDeactivation);
            }

            // Here we cleanup everything we needed for the clicking/dragging
            if ( DragTo )
                { delete DragTo; }
            if ( MouseRay )
                { delete MouseRay; }
        }

    }else{  //Since we are no longer clicking we need to setup for the next clicking
        if(Dragger)
        {
            ActorRigid* Act = Dragger->GetActorA();
            TheWorld->GetPhysicsManager()->RemoveConstraint(Dragger);
            delete Dragger;
            Dragger=NULL;
            Act->GetPhysicsSettings()->SetActivationState(phys::AAS_DisableDeactivation);
        }
    }
    return true;
}

bool CatchApp::PrePhysics()
{
    return true;
}

bool CatchApp::PostPhysics()
{
    //// Updating functions to be used when a suitable mesh is found/created.
    //ActorSoft* ActS = static_cast< ActorSoft* > (TheWorld->Actors->FindActor("Column1"));
    //ActS->UpdateSoftBody();

    for( Whole S = 0 ; S < ScoreAreas.size() ; S++ )
    {
        std::vector<ActorBase*>& Added = ScoreAreas[S]->GetAddedActors();
        std::vector<ActorBase*>& Removed = ScoreAreas[S]->GetRemovedActors();
        for( Whole A = 0 ; A < Added.size() ; A++ )
        {
            String ItemName = Added[A]->GetName();
            for( std::map<String,Whole>::iterator Ait = ItemScoreValues.begin() ; Ait != ItemScoreValues.end() ; Ait++ )
            {
                if(ItemName.find((*Ait).first) != String::npos)
                    CurrScore += (*Ait).second;
            }
        }
        for( Whole R = 0 ; R < Removed.size() ; R++ )
        {
            String ItemName = Removed[R]->GetName();
            for( std::map<String,Whole>::iterator Rit = ItemScoreValues.begin() ; Rit != ItemScoreValues.end() ; Rit++ )
            {
                if(ItemName.find((*Rit).first) != String::npos)
                    CurrScore -= (*Rit).second;
            }
        }
    }

    return true;
}

bool CatchApp::PostRender()
{
	//Lets set a variable for the time
	static Whole gametime = 0;

	TheWorld->Log(String("---------- Starting PosterRender CallBack -------------"));
    TheWorld->Log(String("Current Game Time "));

	//getting a message from the event manager)
	EventRenderTime* CurrentTime = TheWorld->GetEventManager()->PopNextRenderTimeEvent();
    Whole LastFrame = 0;

    // Is CurrentTime a valid event?
    while(0 != CurrentTime)
    {
        LastFrame = CurrentTime->getMilliSecondsSinceLastFrame();

        TheWorld->Log(gametime);
        gametime+=CurrentTime->getMilliSecondsSinceLastFrame();

        delete CurrentTime;
        CurrentTime = TheWorld->GetEventManager()->GetNextRenderTimeEvent();
    }

    // Update the timer
    UI::Caption* Timer = TheWorld->GetUIManager()->GetScreen("GameScreen")->GetLayer("HUDLayer")->GetCaption("GS_Timer");
    std::stringstream time;
    Whole TotalSeconds = LevelTimer->GetCurrentTimeInMilli() / 1000;
    Whole Minutes = TotalSeconds / 60;
    Whole Seconds;
    if(60 > TotalSeconds)
    {
        Seconds = TotalSeconds;
    }else{
        Seconds = TotalSeconds % 60;
    }
    if(10 > Seconds)
    {
        time << Minutes << ":" << 0 << Seconds;
    }else{
        time << Minutes << ":" << Seconds;
    }
    Timer->SetText(time.str());

    // Update the score
    UI::Caption* ScoreAmount = TheWorld->GetUIManager()->GetScreen("GameScreen")->GetLayer("HUDLayer")->GetCaption("GS_ScoreArea");
    std::stringstream Score;
    Score << CurrScore;
    String ScoreOut = Score.str();
    ScoreAmount->SetText(ScoreOut);

    // Update Stat information
    UI::Caption* CurFPS = TheWorld->GetUIManager()->GetScreen("GameScreen")->GetLayer("StatsLayer")->GetCaption("CurFPS");
    UI::Caption* AvFPS = TheWorld->GetUIManager()->GetScreen("GameScreen")->GetLayer("StatsLayer")->GetCaption("AvFPS");
    std::stringstream CFPSstream;
    std::stringstream AFPSstream;
    CFPSstream << TheWorld->GetGraphicsManager()->GetPrimaryGameWindow()->GetLastFPS();
    AFPSstream << TheWorld->GetGraphicsManager()->GetPrimaryGameWindow()->GetAverageFPS();
    String CFPS = CFPSstream.str();
    String AFPS = AFPSstream.str();
    CurFPS->SetText(CFPS);
    AvFPS->SetText(AFPS);

    //See if the level is over
    if(AllStartZonesEmpty())
    {
        if(CheckEndOfLevel())
        {
            TheWorld->GetPhysicsManager()->PauseSimulation(true);
            TheWorld->GetUIManager()->GetLayer("ReportLayer")->Show();
        }
    }

    return true;
}

bool CatchApp::CheckForStuff()
{
    //this will either set the pointer to 0 or return a valid pointer to work with.
    EventUserInput* OneInput = TheWorld->GetEventManager()->PopNextUserInputEvent();

    //We check each Event
    while(0 != OneInput)
    {
        if(OneInput->GetType()!=EventBase::UserInput)
            { TheWorld->LogAndThrow("Trying to process a non-EventUserInput as an EventUserInput."); }

        //we check each MetaCode in each Event
        for (unsigned int c=0; c<OneInput->GetMetaCodeCount(); c++ )
        {
            //Is the key we just pushed ESCAPE
            if(MetaCode::KEY_ESCAPE == OneInput->GetMetaCode(c).GetCode() && MetaCode::BUTTON_PRESSING == OneInput->GetMetaCode(c).GetMetaValue())
                { return false; }
        }

        delete OneInput;
        OneInput = TheWorld->GetEventManager()->PopNextUserInputEvent();
    }

    EventGameWindow* OneWindowEvent = TheWorld->GetEventManager()->PopNextGameWindowEvent();
    while(0 != OneWindowEvent)
    {
        if(OneWindowEvent->GetType()!=EventBase::GameWindow)
            { TheWorld->LogAndThrow("Trying to process a non-EventGameWindow as an EventGameWindow."); }

        if(!OneWindowEvent->IsEventIDValid())
        {
            TheWorld->Log("Invalid EventID on GameWindow Event");
            TheWorld->LogAndThrow(OneWindowEvent->GetEventID());
        }

        TheWorld->Log(*OneWindowEvent);
        TheWorld->Log(EventGameWindow::GameWindowEventIDToString(OneWindowEvent->GetEventID()));
        stringstream eventxml;
        eventxml << *OneWindowEvent;    // Test XML conversion and reconstruction
        EventGameWindow AnotherWindowEvent(EventGameWindow::GAME_WINDOW_NONE,0,0);
        eventxml >> AnotherWindowEvent;
        TheWorld->Log(AnotherWindowEvent);

        if (OneWindowEvent->GetEventID()==EventGameWindow::GAME_WINDOW_MINIMIZED)
        {
            Audio::Sound* Welcome = NULL;
            Welcome = TheWorld->GetAudioManager()->GetSoundByName("Welcome");
            if(Welcome)
            {
                Welcome->Play2d(false);
            }
        }

        delete OneWindowEvent;
        OneWindowEvent = TheWorld->GetEventManager()->PopNextGameWindowEvent();
    }

    EventCollision* OneCollision = TheWorld->GetEventManager()->PopNextCollisionEvent();
    EventCollision SecondCollision(0,0,Vector3(0.5,0.5,0.5),Vector3(1.5,1.5,1.5),Vector3(1,1,1),2.5);
    while(0 != OneCollision)
    {
        if(OneCollision->GetType() != EventBase::Collision)
            { TheWorld->LogAndThrow("Trying to process a non-EventCollision as an EventCollision."); }

        stringstream temp;
        temp << *OneCollision;
        temp >> SecondCollision;
        TheWorld->Log(SecondCollision);

        delete OneCollision;
        OneCollision = TheWorld->GetEventManager()->PopNextCollisionEvent();
    }

    return true;
}

void CatchApp::SetPlayArea(AreaOfPlay* PlayArea)
{
    PlayZone = PlayArea;
}

void CatchApp::RegisterScoreArea(ScoreArea* Score)
{
    ScoreAreas.push_back(Score);
}

void CatchApp::RegisterStartArea(StartingArea* Start)
{
    StartAreas.push_back(Start);
}

void CatchApp::AddThrowable(ActorBase* Throwable)
{
    ThrownItems.push_back(Throwable);
}

LevelLoader* CatchApp::GetLevelLoader()
{
    return Loader;
}

#endif
