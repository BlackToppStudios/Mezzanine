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

using namespace std;
using namespace Mezzanine;

CatchApp* CatchApp::TheRealCatchApp = 0;

CatchApp::CatchApp() :
    LastActorThrown(NULL),
    EndTimer(NULL),
    Paused(false),
    CurrScore(0),
    CurrentState(CatchApp::Catch_Init),
    PlaneOfPlay(Plane(Vector3(2.0,1.0,0.0), Vector3(1.0,2.0,0.0), Vector3(1.0,1.0,0.0)))
{
    assert(0==CatchApp::TheRealCatchApp);
    CatchApp::TheRealCatchApp = this;

    //try{
        TheEntresol = new Entresol( "Data/", AT_FileSystem );
    //}catch(...){
//        throw "";
//    }
    Loader = new LevelLoader();
    Scorer = new LevelScorer();
    Profiles = new ProfileManager("Profiles/");
    Shop = new ItemShop();
    ThrowableGenerator::ParseThrowables("");

    LevelTimer = TimerManager::GetSingletonPtr()->CreateSimpleTimer(Timer::Normal);
}

CatchApp::~CatchApp()
{
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

    ColourValue Transparent(0.0,0.0,0.0,0.0);
    ColourValue Black(0.0,0.0,0.0,1.0);
    ColourValue TransBlack(0.0,0.0,0.0,0.85);
    ColourValue Gray(0.2,0.2,0.2,1.0);

    GUI->EnableButtonAutoRegister(true);
    GUI->AddAutoRegisterCode(Input::MOUSEBUTTON_1);

    //Make the Main Menu screen and associated layers.
    GUI->LoadMTA("Catch_Menu");
    UI::Screen* MainMenuScreen = GUI->CreateScreen("MainMenuScreen", "Catch_Menu", UIViewport);

    //Build the Main Menu Screen
    UI::OpenRenderableContainerWidget* MMBack = MainMenuScreen->CreateOpenRenderableContainerWidget("MM_Back");
    UI::Rectangle* Background = MMBack->CreateRectangle( UI::RenderableRect(Vector2(-0.16667,0), Vector2(1.33334,1), true));
    Background->SetBackgroundSprite("MainMenuBackground");
    MainMenuScreen->AddRootWidget(0,MMBack);

    UI::Menu* MainMenuMenu = MainMenuScreen->CreateMenu( "MS_Menu", UI::RenderableRect(Vector2(0.0,0.915), Vector2(1.0,0.086), true));
    MainMenuMenu->GetRootWindow()->GetWindowBack()->SetBackgroundSprite("MMBrickBackground");
    MainMenuMenu->GetRootWindow()->SetAutoHide(false);

    Real MMStartLineHeight = 0.05;
    Real MMTextLineHeight = 0.04;
    //Real MMSmallTextLineHeight = 0.03;
    UI::Button* MMLevelSelectAccess = MainMenuMenu->GetRootWindow()->CreateAccessorButton( "MS_LevelSelect", UI::RenderableRect(Vector2(0.05, 0.93), Vector2(0.22, 0.06), true), MMTextLineHeight, "Level Select" );
    MMLevelSelectAccess->GetClickable()->SetBackgroundSprite("MMButton");
    MMLevelSelectAccess->GetClickable()->SetHoveredSprite("MMHoveredButton");
    UI::MenuWindow* MMLevelSelectWin = MainMenuMenu->GetRootWindow()->CreateChildMenuWindow("MS_LevelSelectWin", UI::RenderableRect(Vector2(0.05,0.02), Vector2(0.90,0.84), true), MMLevelSelectAccess);
    MMLevelSelectWin->GetWindowBack()->SetBackgroundSprite("MMLSBackground");
    UI::PagedCellGrid* MMLevelSelectGrid = MMLevelSelectWin->CreatePagedCellGrid("MS_LevelGrid", UI::RenderableRect(Vector2(0.14,0.14), Vector2(0.72,0.66), true), UI::RenderableRect(Vector2(0.60,0.85), Vector2(0.24,0.06), true), UI::Spn_Separate, 0.05);
    MMLevelSelectGrid->GetGridBack()->SetBackgroundColour(Transparent);
    MMLevelSelectGrid->GetPageSpinner()->GetIncrement()->GetClickable()->SetBackgroundSprite("MMIncrementPage");
    MMLevelSelectGrid->GetPageSpinner()->GetDecrement()->GetClickable()->SetBackgroundSprite("MMDecrementPage");
    MMLevelSelectGrid->GetPageSpinner()->GetValueDisplay()->SetBackgroundSprite("MMPageBox");
    UI::Button* MMLevelStart = MMLevelSelectWin->CreateButton("MS_LevelStart", UI::RenderableRect(Vector2(0.42,0.85), Vector2(0.16,0.07), true),MMStartLineHeight,"Start");
    MMLevelStart->AddActivatableListener(new MSStart(MMLevelSelectGrid));
    MMLevelStart->GetClickable()->SetBackgroundSprite("MMLevelStart");
    MMLevelStart->GetClickable()->SetHoveredSprite("MMLevelStartHovered");

    UI::Button* MMOptionsAccess = MainMenuMenu->GetRootWindow()->CreateAccessorButton( "MS_Options", UI::RenderableRect(Vector2(0.28, 0.93), Vector2(0.22, 0.06), true), MMTextLineHeight, "Options" );
    MMOptionsAccess->GetClickable()->SetBackgroundSprite("MMButton");
    MMOptionsAccess->GetClickable()->SetHoveredSprite("MMHoveredButton");
    UI::MenuWindow* MMOptionsWin = MainMenuMenu->GetRootWindow()->CreateChildMenuWindow("MS_OptionsWin", UI::RenderableRect(Vector2(0.18, 0.22), Vector2(0.64, 0.55), true), MMOptionsAccess);
    MMOptionsWin->GetWindowBack()->SetBackgroundSprite("MMOptionsBackground");
    UI::TabSet* MMOptionsTabSet = MMOptionsWin->CreateTabSet("MS_OptionsTS", UI::RenderableRect(Vector2(0.20, 0.31), Vector2(0.60, 0.39), true));
    //video options
    UI::RenderableSetData* MMVideoSet = MMOptionsTabSet->CreateRenderableSet("MS_VideoSet",UI::RenderableRect(Vector2(0.25, 0.24),Vector2(0.22, 0.06),true),MMTextLineHeight,"Video Options");
    MMVideoSet->Collection->GetWidgetBack()->SetBackgroundColour(ColourValue::Transparent());
    MMVideoSet->Accessor->GetClickable()->SetBackgroundSprite("MMButton");
    MMVideoSet->Accessor->GetClickable()->SetHoveredSprite("MMHoveredButton");
    UI::Caption* MMResolutionLabel = MMVideoSet->Collection->CreateCaption("MS_ResolutionLabel",UI::RenderableRect(Vector2(0.28, 0.31),Vector2(0.24, 0.05),true),MMTextLineHeight,"Video Resolution");
    MMResolutionLabel->SetBackgroundSprite("MMButton");
    UI::DropDownList* MMResolutionList = MMVideoSet->Collection->CreateDropDownList("MS_ResolutionList",UI::RenderableRect(Vector2(0.25, 0.36),Vector2(0.30, 0.05),true),MMTextLineHeight,UI::SB_Separate);
    MMResolutionList->GetSelection()->SetBackgroundSprite("MMListSelection");
    MMResolutionList->GetListToggle()->GetClickable()->SetBackgroundSprite("MMListScrollDown");
    MMResolutionList->GetSelectionList()->GetBoxBack()->SetBackgroundSprite("MMListBackground");
    MMResolutionList->GetSelectionList()->GetVertScroll()->GetScrollBack()->SetBackgroundSprite("MMListScrollBackground");
    MMResolutionList->GetSelectionList()->GetVertScroll()->GetScroller()->GetClickable()->SetBackgroundSprite("MMListScroller");
    MMResolutionList->GetSelectionList()->GetVertScroll()->GetScroller()->GetClickable()->SetHoveredSprite("MMHoveredListScroller");
    MMResolutionList->GetSelectionList()->GetVertScroll()->GetDownRightButton()->GetClickable()->SetBackgroundSprite("MMListScrollDown");
    MMResolutionList->GetSelectionList()->GetVertScroll()->GetDownRightButton()->GetClickable()->SetHoveredSprite("MMHoveredListScrollDown");
    MMResolutionList->GetSelectionList()->GetVertScroll()->GetUpLeftButton()->GetClickable()->SetBackgroundSprite("MMListScrollUp");
    MMResolutionList->GetSelectionList()->GetVertScroll()->GetUpLeftButton()->GetClickable()->SetHoveredSprite("MMHoveredListScrollUp");
    MMResolutionList->GetSelectionList()->GetVertScroll()->SetIncrementDistance(0.05);
    MMResolutionList->GetSelectionList()->SetTemplateBackgroundColour(ColourValue(0.0,0.0,0.0,0.0));
    MMResolutionList->GetSelectionList()->SetTemplateTextColour(ColourValue::White());
    MMResolutionList->GetSelectionList()->SetMaxDisplayedSelections(4);
    MMResolutionList->AddWidgetListener(new OptsVideoRes());
    UI::CheckBox* MMFullscreenBox = MMVideoSet->Collection->CreateCheckBox("MS_FullscreenBox",UI::RenderableRect(Vector2(0.59, 0.36),Vector2(0.16, 0.05),true),MMTextLineHeight,"Fullscreen");
    MMFullscreenBox->GetLabel()->SetBackgroundSprite("MMAppExitButton");
    MMFullscreenBox->SetCheckedSprite("MMCheckboxChecked","MMHoveredCheckboxChecked");
    MMFullscreenBox->SetUncheckedSprite("MMCheckboxUnchecked","MMHoveredCheckboxUnchecked");
    MMFullscreenBox->AddWidgetListener(new OptsVideoFullscreen());
    UI::Caption* MMFSAALabel = MMVideoSet->Collection->CreateCaption("MS_FSAALabel",UI::RenderableRect(Vector2(0.28, 0.46),Vector2(0.24, 0.05),true),MMTextLineHeight,"Anti-Aliasing");
    MMFSAALabel->SetBackgroundSprite("MMButton");
    UI::DropDownList* MMFSAAList = MMVideoSet->Collection->CreateDropDownList("MS_FSAAList",UI::RenderableRect(Vector2(0.25, 0.51),Vector2(0.30, 0.05),true),MMTextLineHeight,UI::SB_Separate);
    MMFSAAList->GetSelection()->SetBackgroundSprite("MMListSelection");
    MMFSAAList->GetListToggle()->GetClickable()->SetBackgroundSprite("MMListScrollDown");
    MMFSAAList->GetSelectionList()->GetBoxBack()->SetBackgroundSprite("MMListBackground");
    MMFSAAList->GetSelectionList()->GetVertScroll()->GetScrollBack()->SetBackgroundSprite("MMListScrollBackground");
    MMFSAAList->GetSelectionList()->GetVertScroll()->GetScroller()->GetClickable()->SetBackgroundSprite("MMListScroller");
    MMFSAAList->GetSelectionList()->GetVertScroll()->GetScroller()->GetClickable()->SetHoveredSprite("MMHoveredListScroller");
    MMFSAAList->GetSelectionList()->GetVertScroll()->GetDownRightButton()->GetClickable()->SetBackgroundSprite("MMListScrollDown");
    MMFSAAList->GetSelectionList()->GetVertScroll()->GetDownRightButton()->GetClickable()->SetHoveredSprite("MMHoveredListScrollDown");
    MMFSAAList->GetSelectionList()->GetVertScroll()->GetUpLeftButton()->GetClickable()->SetBackgroundSprite("MMListScrollUp");
    MMFSAAList->GetSelectionList()->GetVertScroll()->GetUpLeftButton()->GetClickable()->SetHoveredSprite("MMHoveredListScrollUp");
    MMFSAAList->GetSelectionList()->GetVertScroll()->SetIncrementDistance(0.05);
    MMFSAAList->GetSelectionList()->SetTemplateBackgroundColour(ColourValue(1.0,1.0,1.0,0.2));
    MMFSAAList->GetSelectionList()->SetTemplateTextColour(ColourValue::White());
    MMFSAAList->GetSelectionList()->SetMaxDisplayedSelections(4);
    MMFSAAList->AddWidgetListener(new OptsVideoFSAA());
    UI::CheckBox* MMStatsBox = MMVideoSet->Collection->CreateCheckBox("MS_StatsBox",UI::RenderableRect(Vector2(0.59, 0.51),Vector2(0.16, 0.05),true),MMTextLineHeight,"Show FPS");
    MMStatsBox->GetLabel()->SetBackgroundSprite("MMAppExitButton");
    MMStatsBox->SetCheckedSprite("MMCheckboxChecked","MMHoveredCheckboxChecked");
    MMStatsBox->SetUncheckedSprite("MMCheckboxUnchecked","MMHoveredCheckboxUnchecked");
    MMStatsBox->AddWidgetListener(new OptsVideoStats());
    UI::Button* MMVideoOptsApply = MMVideoSet->Collection->CreateButton("MS_VideoOptsApply",UI::RenderableRect(Vector2(0.68, 0.64), Vector2(0.10, 0.05), true), MMTextLineHeight, "Apply");
    MMVideoOptsApply->GetClickable()->SetBackgroundSprite("MMOptionsApplyButton");
    MMVideoOptsApply->GetClickable()->SetHoveredSprite("MMOptionsApplyHoveredButton");
    MMVideoOptsApply->AddActivatableListener(new OptsVideoApply(MMResolutionList,MMFullscreenBox,MMStatsBox));
    //sound options
    Real MMScrollerSize = 0.09;
    UI::RenderableSetData* MMAudioSet = MMOptionsTabSet->CreateRenderableSet("MS_AudioSet",UI::RenderableRect(Vector2(0.53, 0.24),Vector2(0.22, 0.06),true),MMTextLineHeight,"Sound Options");
    MMAudioSet->Collection->GetWidgetBack()->SetBackgroundColour(ColourValue::Transparent());
    MMAudioSet->Accessor->GetClickable()->SetBackgroundSprite("MMButton");
    MMAudioSet->Accessor->GetClickable()->SetHoveredSprite("MMHoveredButton");
    UI::Caption* MMMusicVolLabel = MMAudioSet->Collection->CreateCaption("MS_MusicVolLabel",UI::RenderableRect(Vector2(0.38, 0.31),Vector2(0.24, 0.05),true),MMTextLineHeight,"Music Volume");
    MMMusicVolLabel->SetBackgroundSprite("MMButton");
    UI::Scrollbar* MMMusicVol = MMAudioSet->Collection->CreateScrollbar("MS_MusicVolume",UI::RenderableRect(Vector2(0.30, 0.36),Vector2(0.40, 0.04),true),UI::SB_Separate);
    MMMusicVol->SetScrollerSize(MMScrollerSize);
    MMMusicVol->SetIncrementDistance(MMScrollerSize * 0.5);
    MMMusicVol->GetScrollBack()->SetBackgroundSprite("MMScrollBackground");
    MMMusicVol->GetScroller()->GetClickable()->SetBackgroundSprite("MMScroller");
    MMMusicVol->GetScroller()->GetClickable()->SetHoveredSprite("MMHoveredScroller");
    MMMusicVol->GetDownRightButton()->GetClickable()->SetBackgroundSprite("MMScrollRight");
    MMMusicVol->GetDownRightButton()->GetClickable()->SetHoveredSprite("MMHoveredScrollRight");
    MMMusicVol->GetUpLeftButton()->GetClickable()->SetBackgroundSprite("MMScrollLeft");
    MMMusicVol->GetUpLeftButton()->GetClickable()->SetHoveredSprite("MMHoveredScrollLeft");
    MMMusicVol->AddWidgetListener(new OptsMusicVol());
    UI::Caption* MMEffectsVolLabel = MMAudioSet->Collection->CreateCaption("MS_EffectsVolLabel",UI::RenderableRect(Vector2(0.38, 0.42),Vector2(0.24, 0.05),true),MMTextLineHeight,"Effects Volume");
    MMEffectsVolLabel->SetBackgroundSprite("MMButton");
    UI::Scrollbar* MMEffectsVol = MMAudioSet->Collection->CreateScrollbar("MS_EffectsVolume",UI::RenderableRect(Vector2(0.30, 0.47),Vector2(0.40, 0.04),true),UI::SB_Separate);
    MMEffectsVol->SetScrollerSize(MMScrollerSize);
    MMEffectsVol->SetIncrementDistance(MMScrollerSize * 0.5);
    MMEffectsVol->GetScrollBack()->SetBackgroundSprite("MMScrollBackground");
    MMEffectsVol->GetScroller()->GetClickable()->SetBackgroundSprite("MMScroller");
    MMEffectsVol->GetScroller()->GetClickable()->SetHoveredSprite("MMHoveredScroller");
    MMEffectsVol->GetDownRightButton()->GetClickable()->SetBackgroundSprite("MMScrollRight");
    MMEffectsVol->GetDownRightButton()->GetClickable()->SetHoveredSprite("MMHoveredScrollRight");
    MMEffectsVol->GetUpLeftButton()->GetClickable()->SetBackgroundSprite("MMScrollLeft");
    MMEffectsVol->GetUpLeftButton()->GetClickable()->SetHoveredSprite("MMHoveredScrollLeft");
    MMEffectsVol->AddWidgetListener(new OptsEffectVol());
    UI::Caption* MMDeviceLabel = MMAudioSet->Collection->CreateCaption("MS_DeviceLabel",UI::RenderableRect(Vector2(0.30, 0.55),Vector2(0.24, 0.05),true),MMTextLineHeight,"Audio Device");
    MMDeviceLabel->SetBackgroundSprite("MMButton");
    UI::DropDownList* MMDeviceList = MMAudioSet->Collection->CreateDropDownList("MS_AudioDeviceList",UI::RenderableRect(Vector2(0.27, 0.60),Vector2(0.30, 0.05),true),MMTextLineHeight,UI::SB_Separate);
    MMDeviceList->GetSelection()->SetBackgroundSprite("MMListSelection");
    MMDeviceList->GetListToggle()->GetClickable()->SetBackgroundSprite("MMListScrollDown");
    MMDeviceList->GetSelectionList()->GetBoxBack()->SetBackgroundSprite("MMListBackground");
    MMDeviceList->GetSelectionList()->GetVertScroll()->GetScrollBack()->SetBackgroundSprite("MMListScrollBackground");
    MMDeviceList->GetSelectionList()->GetVertScroll()->GetScroller()->GetClickable()->SetBackgroundSprite("MMListScroller");
    MMDeviceList->GetSelectionList()->GetVertScroll()->GetScroller()->GetClickable()->SetHoveredSprite("MMHoveredListScroller");
    MMDeviceList->GetSelectionList()->GetVertScroll()->GetDownRightButton()->GetClickable()->SetBackgroundSprite("MMListScrollDown");
    MMDeviceList->GetSelectionList()->GetVertScroll()->GetDownRightButton()->GetClickable()->SetHoveredSprite("MMHoveredListScrollDown");
    MMDeviceList->GetSelectionList()->GetVertScroll()->GetUpLeftButton()->GetClickable()->SetBackgroundSprite("MMListScrollUp");
    MMDeviceList->GetSelectionList()->GetVertScroll()->GetUpLeftButton()->GetClickable()->SetHoveredSprite("MMHoveredListScrollUp");
    UI::CheckBox* MMMuteBox = MMAudioSet->Collection->CreateCheckBox("MS_MuteBox",UI::RenderableRect(Vector2(0.62, 0.60),Vector2(0.11, 0.05),true),MMTextLineHeight,"Mute");
    MMMuteBox->GetLabel()->SetBackgroundSprite("MMAppExitButton");
    MMMuteBox->SetCheckedSprite("MMCheckboxChecked","MMHoveredCheckboxChecked");
    MMMuteBox->SetUncheckedSprite("MMCheckboxUnchecked","MMHoveredCheckboxUnchecked");
    MMMuteBox->AddWidgetListener(new OptsAudioMute());

    UI::Button* MMOptsBack = MMOptionsWin->CreateBackButton(UI::RenderableRect(Vector2(0.68, 0.70), Vector2(0.10, 0.05), true), MMTextLineHeight, "Back");
    MMOptsBack->GetClickable()->SetBackgroundSprite("MMOptionsApplyButton");
    MMOptsBack->GetClickable()->SetHoveredSprite("MMOptionsApplyHoveredButton");

    UI::Button* MMCreditsAccess = MainMenuMenu->GetRootWindow()->CreateAccessorButton( "MS_Credits", UI::RenderableRect(Vector2(0.51, 0.93), Vector2(0.22, 0.06), true), MMTextLineHeight, "Credits" );
    MMCreditsAccess->GetClickable()->SetBackgroundSprite("MMButton");
    MMCreditsAccess->GetClickable()->SetHoveredSprite("MMHoveredButton");
    //UI::MenuWindow* MMCreditsWin = MainMenuMenu->GetRootWindow()->CreateChildMenuWindow("MS_CreditsWin", UI::RenderableRect(Vector2(0.01,0.01), Vector2(0.01,0.01), true), MMCreditsAccess);
    MainMenuMenu->GetRootWindow()->CreateChildMenuWindow("MS_CreditsWin", UI::RenderableRect(Vector2(0.01,0.01), Vector2(0.01,0.01), true), MMCreditsAccess);

    UI::Button* MMAppExitAccess = MainMenuMenu->GetRootWindow()->CreateAccessorButton( "MS_AppExit", UI::RenderableRect(Vector2(0.74, 0.93), Vector2(0.22, 0.06), true), MMTextLineHeight, "Exit Game" );
    MMAppExitAccess->GetClickable()->SetBackgroundSprite("MMButton");
    MMAppExitAccess->GetClickable()->SetHoveredSprite("MMHoveredButton");
    UI::MenuWindow* MMAppExitWin = MainMenuMenu->GetRootWindow()->CreateChildMenuWindow("MS_AppExitWin", UI::RenderableRect(Vector2(0.25, 0.3), Vector2(0.5, 0.3), true), MMAppExitAccess);
    MMAppExitWin->GetWindowBack()->SetBackgroundSprite("MMAppExitBackground");
    UI::Caption* MMAppExitWarn = MMAppExitWin->CreateCaption("MS_AppExitWarn", UI::RenderableRect(Vector2(0.32, 0.35), Vector2(0.36, 0.07), true), Real(0.04), "Are you sure you want to exit?");
    MMAppExitWarn->SetBackgroundSprite("MMAppExitText");
    UI::Button* MMAppExitConf = MMAppExitWin->CreateButton("MS_AppExitConf", UI::RenderableRect(Vector2(0.30, 0.47), Vector2(0.18, 0.06), true), Real(0.04), "Yes");
    MMAppExitConf->AddActivatableListener(new AllAppExit());
    MMAppExitConf->GetClickable()->SetBackgroundSprite("MMAppExitButton");
    MMAppExitConf->GetClickable()->SetHoveredSprite("MMAppExitHoveredButton");
    UI::Button* MMAppExitDeny = MMAppExitWin->CreateBackButton(/*"MS_AppExitDeny", */UI::RenderableRect(Vector2(0.52, 0.47), Vector2(0.18, 0.06), true), Real(0.04), "No");
    MMAppExitDeny->GetClickable()->SetBackgroundSprite("MMAppExitButton");
    MMAppExitDeny->GetClickable()->SetHoveredSprite("MMAppExitHoveredButton");

    MainMenuScreen->AddRootWidget(1,MainMenuMenu);
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
    GameScreen->Hide();
}

void CatchApp::CreateLoadingScreen()
{
    UI::UIManager* GUI = UI::UIManager::GetSingletonPtr();
    Graphics::GraphicsManager* GraphicsMan = Graphics::GraphicsManager::GetSingletonPtr();

    GUI->LoadMTA("Catch_Loading");
    Graphics::Viewport* UIViewport = GraphicsMan->GetGameWindow(0)->GetViewport(0);
    UIViewport->SetCamera(CameraManager::GetSingletonPtr()->CreateCamera("Main"));

    UI::Screen* LoadScreen = GUI->CreateScreen("LoadingScreen", "Catch_Loading", UIViewport);
    UI::OpenRenderableContainerWidget* LoadContainer = LoadScreen->CreateOpenRenderableContainerWidget("Load_Rect");
    UI::Rectangle* LoadBackground = LoadContainer->CreateRectangle( UI::RenderableRect(Vector2(-0.16667,0), Vector2(1.33334,1), true));
    LoadBackground->SetBackgroundSprite("BTSBanner");
    LoadScreen->AddRootWidget(0,LoadContainer);

    GraphicsMan->RenderOneFrame();
    LoadBackground->SetBackgroundSprite("LoadingBackground");
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

void CatchApp::ChangeState(const CatchApp::GameState &StateToSet)
{
    if(CurrentState == StateToSet)
        return;
    UI::UIManager* UIMan = UI::UIManager::GetSingletonPtr();
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
        case CatchApp::Catch_ScoreScreen:
        {
            PauseGame(true);
            Whole LevelScore = Scorer->CalculateFinalScore();
            if(LevelScore > Profiles->GetActiveProfile()->GetHighestScore(Loader->GetCurrentLevel()))
            {
                Profiles->GetActiveProfile()->SetNewHighScore(Loader->GetCurrentLevel(),LevelScore);
                (static_cast<LevelSelectCell*>(Profiles->GetLevelGrid()->GetCell(Loader->GetCurrentLevel())))->GetEarnedScore()->SetText(StringTools::ConvertToString(LevelScore));
            }
            break;
        }
        case CatchApp::Catch_Init:
            break;
    }
    CurrentState = StateToSet;
}

bool CatchApp::CheckEndOfLevel()
{
    if(ScoreAreas.empty())
        return false;
    if(CurrentState == CatchApp::Catch_ScoreScreen)
        return true;
    if(AllStartZonesEmpty())
    {
        if(!EndTimer)
        {
            EndTimer = TimerManager::GetSingletonPtr()->CreateSimpleTimer(Timer::StopWatch);
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
        return 0 == EndTimer->GetCurrentTime();
    }else{
        if(EndTimer)
        {
            TimerManager::GetSingletonPtr()->DestroyTimer(EndTimer);
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
    Physics::PhysicsManager* PhysMan = Physics::PhysicsManager::GetSingletonPtr();
    ResourceManager* ResMan = ResourceManager::GetSingletonPtr();
    SceneManager* SceneMan = SceneManager::GetSingletonPtr();
    ActorManager* ActorMan = ActorManager::GetSingletonPtr();
    TimerManager* TimeMan = TimerManager::GetSingletonPtr();
    EventManager* EventMan = EventManager::GetSingletonPtr();
    CollisionShapeManager* CShapeMan = CollisionShapeManager::GetSingletonPtr();
    MeshManager* MeshMan = MeshManager::GetSingletonPtr();
    UI::UIManager* UIMan = UI::UIManager::GetSingletonPtr();

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
    StartAreas.clear();
    ScoreAreas.clear();
    ThrownItems.clear();

    ResMan->DestroyAssetGroup(Loader->GetCurrentLevel());
    PhysMan->ClearPhysicsMetaData();
    /// @todo Probably should make a "RemoveAll" for the events as well.
    EventCollision* OneCollision = EventMan->PopNextCollisionEvent();
    while( NULL != OneCollision )
    {
        delete OneCollision;
        OneCollision = EventMan->PopNextCollisionEvent();
    }
    CurrScore = 0;
    Scorer->ResetLevelData();
    TimeMan->DestroyTimer(EndTimer);
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

class CatchPreInputWorkUnit : public Threading::DefaultWorkUnit
{
        CatchApp* CatchApplication;
    public:
        CatchPreInputWorkUnit(CatchApp* Target)
            : CatchApplication(Target)
            {}

        virtual void DoWork(Threading::DefaultThreadSpecificStorage::Type& CurrentThreadStorage)
            { CatchApplication->CheckForStuff(); }
};

int CatchApp::GetCatchin()
{
    EventManager::GetSingletonPtr()->SetPreMainLoopItems(&CPreInput);
    Input::InputManager::GetSingletonPtr()->SetPostMainLoopItems(&CPostInput);
    Physics::PhysicsManager::GetSingletonPtr()->SetPreMainLoopItems(&CPrePhysics);
    Physics::PhysicsManager::GetSingletonPtr()->SetPostMainLoopItems(&CPostPhysics);
    Graphics::GraphicsManager::GetSingletonPtr()->SetPostMainLoopItems(&CPostRender);
    UI::UIManager::GetSingletonPtr()->SetPreMainLoopItems(&CPreUI);
    UI::UIManager::GetSingletonPtr()->SetPostMainLoopItems(&CPostUI);

    // Verify all the settings are there, and generate defaults if they aren't.
    this->VerifySettings();

    // Make sure our work runs before events are gathered from OS
    CatchPreInputWorkUnit* CatchPreInputWork = new CatchPreInputWorkUnit(this);
    TheEntresol->GetEventManager()->GetEventWorkUnit()->AddDependency(CatchPreInputWork);
    TheEntresol->GetScheduler().AddWorkUnit(CatchPreInputWork);

    // Initialize the managers.
	TheEntresol->EngineInit(false);

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
    Loader->SetNextLevel("MainMenu");
    do{
        ChangeState(CatchApp::Catch_Loading);
        PauseGame(false);
        Graphics::GraphicsManager::GetSingletonPtr()->RenderOneFrame();
        //Actually Load the game stuff
        Loader->LoadLevel();

        if("MainMenu"==Loader->GetCurrentLevel())
            this->ChangeState(CatchApp::Catch_MenuScreen);
        else
            this->ChangeState(CatchApp::Catch_GameScreen);
        LevelTimer->Reset();
        LevelTimer->Start();

        //if("Ferris"==Loader->GetCurrentLevel())
        //    UIManager::GetSingletonPtr()->GetScreen("GameScreen")->Hide();
        //PhysicsManager::GetSingletonPtr()->PauseSimulation(true);
        //Start the Main Loop
        TheEntresol->MainLoop();
        this->UnloadLevel();
        LevelTimer->Stop();
    } while(Loader->HasALevelToLoad());

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

bool CatchApp::GameIsPaused()
{
    return Paused;
}

bool CatchApp::PostInput()
{
    Input::InputManager* InputMan = Input::InputManager::GetSingletonPtr();
    Input::Mouse* SysMouse = InputMan->GetSystemMouse();
    Input::Keyboard* SysKeyboard = InputMan->GetSystemKeyboard();
    CameraController* DefaultControl = CameraManager::GetSingletonPtr()->GetOrCreateCameraController(CameraManager::GetSingletonPtr()->GetCamera(0));
    if( SysKeyboard->IsButtonPressed(Input::KEY_LEFT) || SysKeyboard->IsButtonPressed(Input::KEY_A))
        DefaultControl->StrafeLeft(300 * (TheEntresol->GetFrameTimeMilliseconds() * 0.001));
    if( SysKeyboard->IsButtonPressed(Input::KEY_RIGHT) || SysKeyboard->IsButtonPressed(Input::KEY_D))
        DefaultControl->StrafeRight(300 * (TheEntresol->GetFrameTimeMilliseconds() * 0.001));
    if( SysKeyboard->IsButtonPressed(Input::KEY_UP) || SysKeyboard->IsButtonPressed(Input::KEY_W))
        DefaultControl->MoveForward(300 * (TheEntresol->GetFrameTimeMilliseconds() * 0.001));
    if( SysKeyboard->IsButtonPressed(Input::KEY_DOWN)  || SysKeyboard->IsButtonPressed(Input::KEY_S))
        DefaultControl->MoveBackward(300 * (TheEntresol->GetFrameTimeMilliseconds() * 0.001));
    static bool MouseCam=false;
    if( SysKeyboard->IsButtonPressed(Input::KEY_HOME) )
        MouseCam=true;
    if( SysKeyboard->IsButtonPressed(Input::KEY_END))
        MouseCam=false;
    Vector2 Offset = SysMouse->GetMouseDelta();
    if( MouseCam && Vector2(0,0) != Offset )
        DefaultControl->Rotate(Offset.X * 0.01,Offset.Y * 0.01,0);

    if( Input::BUTTON_PRESSING == SysKeyboard->GetButtonState(Input::KEY_C) )
    {
        Physics::PhysicsManager* PhysMan = Physics::PhysicsManager::GetSingletonPtr();
        if(PhysMan->GetDebugPhysicsRendering())
        {
            PhysMan->SetDebugPhysicsWireCount(0);
            PhysMan->SetDebugPhysicsRendering(0);
        }else{
            PhysMan->SetDebugPhysicsWireCount(2);
            PhysMan->SetDebugPhysicsRendering(1);
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
    Input::InputManager* InputMan = Input::InputManager::GetSingletonPtr();
    Input::Mouse* SysMouse = InputMan->GetSystemMouse();
    static RayQueryTool* RayQueryer = new RayQueryTool();
    static Physics::Point2PointConstraint* Dragger = NULL;

    if( SysMouse->IsButtonPressed(1) )
    {
        if( !UI::UIManager::GetSingletonPtr()->MouseIsInUISystem() )
        {
            Vector3WActor* ClickOnActor = 0;
            Ray* MouseRay = RayQueryer->GetMouseRay(5000);
            if(MouseRay)
            {
                ClickOnActor = RayQueryer->GetFirstActorOnRayByPolygon(*MouseRay,Mezzanine::WSO_ActorRigid);
            }
            //ActorBase *temp = ClickOnActor->Actor;

            bool firstframe=false;
            if( 0 != ClickOnActor &&
                0 != ClickOnActor->Actor &&
                IsInsideAnyStartZone(ClickOnActor->Actor) )
            {
                if( !(ClickOnActor->Actor->IsStaticOrKinematic()) &&
                    ClickOnActor->Actor->GetType() == Mezzanine::WSO_ActorRigid &&
                    !Dragger )
                {
                    Vector3 LocalPivot = ClickOnActor->Vector;
                    ActorRigid* rigid = static_cast<ActorRigid*>(ClickOnActor->Actor);
                    rigid->GetPhysicsSettings()->SetActivationState(Mezzanine::Physics::WOAS_DisableDeactivation);
                    Dragger = new Physics::Point2PointConstraint(rigid, LocalPivot);
                    Dragger->SetTAU(0.001);
                    Physics::PhysicsManager::GetSingletonPtr()->AddConstraint(Dragger);
                    Dragger->SetParam(Physics::Con_Stop_CFM,0.8,-1);
                    Dragger->SetParam(Physics::Con_CFM,0.8,-1);
                    //Dragger->SetParam(Physics::Con_Stop_CFM,0.8,0); Dragger->SetParam(Physics::Con_Stop_CFM,0.8,1); Dragger->SetParam(Physics::Con_Stop_CFM,0.8,2); //Dragger->SetParam(4,0.8,3); Dragger->SetParam(4,0.8,4); Dragger->SetParam(4,0.8,5);
                    Dragger->SetParam(Physics::Con_Stop_ERP,0.1,-1);
                    Dragger->SetParam(Physics::Con_ERP,0.1,-1);
                    //Dragger->SetParam(Physics::Con_Stop_ERP,0.1,0); Dragger->SetParam(Physics::Con_Stop_ERP,0.1,1); Dragger->SetParam(Physics::Con_Stop_ERP,0.1,2); //Dragger->SetParam(2,0.1,3); Dragger->SetParam(2,0.1,4); Dragger->SetParam(2,0.1,5);
                    firstframe = true;
                    LastActorThrown = rigid;
                }
            }

            Vector3* DragTo = 0;
            if( MouseRay )
            {
                // This chunk of code calculates the 3d point that the actor needs to be dragged to
                DragTo = RayQueryer->RayPlaneIntersection(*MouseRay, PlaneOfPlay);
                if( DragTo != NULL )
                {
                    if( Dragger && !firstframe )
                    {
                        Dragger->SetPivotBLocation(*DragTo);
                    }
                }
            }

            if(Dragger && !IsInsideAnyStartZone(LastActorThrown))
            {
                ActorRigid* Act = Dragger->GetActorA();
                Physics::PhysicsManager::GetSingletonPtr()->RemoveConstraint(Dragger);
                delete Dragger;
                Dragger = NULL;
                Act->GetPhysicsSettings()->SetActivationState(Mezzanine::Physics::WOAS_DisableDeactivation);
            }

            // Here we cleanup everything we needed for the clicking/dragging
            if( DragTo )
                { delete DragTo; }
            if( MouseRay )
                { delete MouseRay; }
        }

    }else{  //Since we are no longer clicking we need to setup for the next clicking
        if(Dragger)
        {
            ActorRigid* Act = Dragger->GetActorA();
            Physics::PhysicsManager::GetSingletonPtr()->RemoveConstraint(Dragger);
            delete Dragger;
            Dragger = NULL;
            Act->GetPhysicsSettings()->SetActivationState(Mezzanine::Physics::WOAS_DisableDeactivation);
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
    Scorer->CalculateCurrentScore(CurrScore);
    return true;
}

bool CatchApp::PostRender()
{
    // Update the timer
    UI::Screen* GameScreen = UI::UIManager::GetSingletonPtr()->GetScreen("GameScreen");
    UI::OpenRenderableContainerWidget* HUDCont = static_cast<UI::OpenRenderableContainerWidget*>(GameScreen->GetWidget("GS_HUD"));
    UI::Caption* Timer = static_cast<UI::Caption*>(HUDCont->GetAreaRenderable("GS_Timer"));
    std::stringstream time;
    Whole TotalSeconds = LevelTimer->GetCurrentTimeInMilli() / 1000;
    Whole Minutes = TotalSeconds / 60;
    Whole Seconds;

    if(60 > TotalSeconds) Seconds = TotalSeconds;
    else Seconds = TotalSeconds % 60;

    if(10 > Seconds) time << Minutes << ":" << 0 << Seconds;
    else time << Minutes << ":" << Seconds;

    Timer->SetText(time.str());

    // Update the score
    UI::Caption* ScoreAmount = static_cast<UI::Caption*>(HUDCont->GetAreaRenderable("GS_ScoreArea"));
    ScoreAmount->SetText(StringTools::ConvertToString(CurrScore));

    // Update Stat information
    Graphics::GraphicsManager* GraphicsMan = Graphics::GraphicsManager::GetSingletonPtr();
    UI::OpenRenderableContainerWidget* StatsCont = static_cast<UI::OpenRenderableContainerWidget*>(GameScreen->GetWidget("GS_Stats"));
    UI::Caption* CurFPS = static_cast<UI::Caption*>(StatsCont->GetAreaRenderable("CurFPS"));
    UI::Caption* AvFPS = static_cast<UI::Caption*>(StatsCont->GetAreaRenderable("AvFPS"));
    CurFPS->SetText(StringTools::ConvertToString(GraphicsMan->GetGameWindow(0)->GetLastFPS()));
    AvFPS->SetText(StringTools::ConvertToString(GraphicsMan->GetGameWindow(0)->GetAverageFPS()));

    StringStream FPSStream;
    FPSStream << "-------------------------- Current FPS: " << GraphicsMan->GetGameWindow(0)->GetLastFPS() << ", Average FPS: " << GraphicsMan->GetGameWindow(0)->GetAverageFPS() << " --------------------------";
    this->TheEntresol->Log(FPSStream.str());
    this->TheEntresol->DoMainLoopLogging();

    //See if the level is over
    if(CurrentState != CatchApp::Catch_ScoreScreen)
    {
        if(CheckEndOfLevel())
        {
            ChangeState(CatchApp::Catch_ScoreScreen);
        }
    }

    return true;
}

bool CatchApp::CheckForStuff()
{
    EventManager* EventMan = EventManager::GetSingletonPtr();
    //this will either set the pointer to 0 or return a valid pointer to work with.
    EventUserInput* OneInput = EventMan->PopNextUserInputEvent();

    //We check each Event
    while(0 != OneInput)
    {
        if(OneInput->GetType()!=EventBase::UserInput)
            { MEZZ_EXCEPTION(Exception::PARAMETERS_EXCEPTION,"Trying to process a non-EventUserInput as an EventUserInput."); }

        //we check each MetaCode in each Event
        /*for (unsigned int c=0; c<OneInput->GetMetaCodeCount(); c++ )
        {
            //Is the key we just pushed ESCAPE
            if(Input::KEY_ESCAPE == OneInput->GetMetaCode(c).GetCode() && Input::BUTTON_PRESSING == OneInput->GetMetaCode(c).GetMetaValue())
                { return false; }
        }// */

        delete OneInput;
        OneInput = EventMan->PopNextUserInputEvent();
    }

    EventGameWindow* OneWindowEvent = EventMan->PopNextGameWindowEvent();
    while(0 != OneWindowEvent)
    {
        if(OneWindowEvent->GetType()!=EventBase::GameWindow)
            { MEZZ_EXCEPTION(Exception::PARAMETERS_EXCEPTION,"Trying to process a non-EventGameWindow as an EventGameWindow."); }

        if(!OneWindowEvent->IsEventIDValid())
        {
            MEZZ_EXCEPTION(Exception::PARAMETERS_EXCEPTION,"Invalid EventID on GameWindow Event: " + OneWindowEvent->GetEventID());
        }

        delete OneWindowEvent;
        OneWindowEvent = EventMan->PopNextGameWindowEvent();
    }

	EventRenderTime* CurrentTime = EventMan->PopNextRenderTimeEvent();
    while(0 != CurrentTime)
    {
        if(CurrentTime->GetType()!=EventBase::RenderTime)
            { MEZZ_EXCEPTION(Exception::PARAMETERS_EXCEPTION,"Trying to process a non-EventRenderTime as an EventRenderTime."); }

        delete CurrentTime;
        CurrentTime = EventMan->GetNextRenderTimeEvent();
    }

    return true;
}

bool CatchApp::IsAThrowable(ActorBase* Actor)
{
    for( std::vector<ActorBase*>::iterator it = ThrownItems.begin() ; it != ThrownItems.end() ; it++ )
    {
        if( Actor == (*it) )
            return true;
    }
    return false;
}

void CatchApp::RegisterScoreArea(ScoreArea* Score)
{
    ScoreAreas.push_back(Score);
    Scorer->RegisterScoreArea(Score);
}

void CatchApp::RegisterStartArea(StartArea* Start)
{
    StartAreas.push_back(Start);
}

void CatchApp::AddThrowable(ActorBase* Throwable)
{
    ThrownItems.push_back(Throwable);
}

std::vector<ActorBase*>& CatchApp::GetThrowables()
{
    return ThrownItems;
}

std::vector<ScoreArea*>& CatchApp::GetScoreAreas()
{
    return ScoreAreas;
}

LevelLoader* CatchApp::GetLevelLoader()
{
    return Loader;
}

LevelScorer* CatchApp::GetLevelScorer()
{
    return Scorer;
}

ProfileManager* CatchApp::GetProfiles()
{
    return Profiles;
}

ItemShop* CatchApp::GetItemShop()
{
    return Shop;
}

SimpleTimer* CatchApp::GetLevelTimer()
{
    return LevelTimer;
}

SimpleTimer* CatchApp::GetEndTimer()
{
    return EndTimer;
}

#endif
