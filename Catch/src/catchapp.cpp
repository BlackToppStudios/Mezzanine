#ifndef _catchapp_cpp
#define _catchapp_cpp

#include "catchapp.h"
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
    PauseWork(NULL),
    HUDUpdateWork(NULL),
    EndLevelWork(NULL),

    TheEntresol(NULL),
    TheWorld(NULL),
    Profiles(NULL),
    LevelMan(NULL),
    Scorer(NULL),
    Shop(NULL),
    LastObjectThrown(NULL),
    ThePlayer(NULL),

    LevelTimer(NULL),
    EndTimer(NULL),

    Paused(false),
    CurrentState(CatchApp::Catch_Init)
{
    assert(0==CatchApp::TheRealCatchApp);
    CatchApp::TheRealCatchApp = this;

    // Initialize the engine
    this->TheEntresol = new Entresol( "Data/", Resource::AT_FileSystem );
    //this->TheWorld = this->TheEntresol->CreateWorld("Catching");
    this->CreateWorld();

    // Now initialize game specific stuff
    this->InitializeFromXML( "Data/", Resource::AT_FileSystem, "Catch.mxi" );

    this->LevelMan = new LevelManager( this->TheEntresol, "Levels/" );
    this->Scorer = new LevelScorer( this, this->TheEntresol );
    this->Shop = new ItemShop();
    ThrowableGenerator::ParseThrowables("");

    Plane PlaneOfPlay(Vector3(2.0,1.0,0.0), Vector3(1.0,2.0,0.0), Vector3(1.0,1.0,0.0));
    this->Picker.Initialize( static_cast<Input::InputManager*>( this->TheEntresol->GetManager(ManagerBase::MT_InputManager) )->GetSystemMouse(),
                             new Graphics::RenderableRayQuery( static_cast<Graphics::SceneManager*>( this->TheWorld->GetManager(ManagerBase::MT_SceneManager) ) ),
                             new PlaneDragger(PlaneOfPlay) );

    if( this->Profiles == NULL ) {
        this->Profiles = new ProfileManager(this->TheEntresol,"$ShareableAppData$/.Catch/Profiles/");
    }
    this->ThePlayer = new CatchPlayer(this->Profiles);

    this->LevelTimer = new Timer();
}

CatchApp::~CatchApp()
{
    this->Picker.Deinitialize(true);
    this->Profiles->Deinitialize();

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

    this->TheEntresol->GetScheduler().RemoveWorkUnitMain( this->PauseWork );
    delete this->PauseWork;

    this->TheEntresol->GetScheduler().RemoveWorkUnitMain( this->HUDUpdateWork );
    delete this->HUDUpdateWork;

    this->TheEntresol->GetScheduler().RemoveWorkUnitMain( this->EndLevelWork );
    delete this->EndLevelWork;

    this->TheEntresol->DestroyWorld(this->TheWorld);

    delete this->ThePlayer;
    delete this->Profiles;
    delete this->LevelMan;
    delete this->Scorer;
    delete this->Shop;
    delete this->TheEntresol;
    CatchApp::TheRealCatchApp = NULL;
}

void CatchApp::InitializeFromXML(const String& CatchDataPath, const Resource::ArchiveType ArchType, const String& InitializerFile)
{
    // Start with the XML initializer file
    Resource::FileStream InitStream(InitializerFile,CatchDataPath);
    XML::Document InitDoc;
    XML::ParseResult DocResult = InitDoc.Load(InitStream);
    if( DocResult.Status != XML::StatusOk ) {
        StringStream ExceptionStream;
        ExceptionStream << "Failed to parse XML file \"" << InitializerFile << "\".";
        MEZZ_EXCEPTION(ExceptionBase::SYNTAX_ERROR_EXCEPTION_XML,ExceptionStream.str());
    }
    XML::Node InitRoot = InitDoc.GetChild("InitializerRoot");
    if( InitRoot.Empty() ) {
        StringStream ExceptionStream;
        ExceptionStream << "Failed to find expected Root node in \"" << InitializerFile << "\".";
        MEZZ_EXCEPTION(ExceptionBase::SYNTAX_ERROR_EXCEPTION_XML,ExceptionStream.str());
    }

    // Create the requested managers and set their necessary values.
    XML::Node Managers = InitRoot.GetChild("Managers");
    for( XML::NodeIterator ManIt = Managers.begin() ; ManIt != Managers.end() ; ++ManIt )
    {
        if( (*ManIt).Name() == String("ProfileManager") && this->Profiles == NULL ) {
            this->Profiles = new ProfileManager(this->TheEntresol,(*ManIt));
        }
    }
}

void CatchApp::CreateWorld()
{
    Physics::ManagerConstructionInfo Info;
    Info.PhysicsFlags = Physics::ManagerConstructionInfo::PCF_LimitlessWorld | Physics::ManagerConstructionInfo::PCF_SoftRigidWorld | Physics::ManagerConstructionInfo::PCF_Multithreaded;

    this->TheWorld = this->TheEntresol->CreateWorld("CatchWorld",Info,"DefaultSceneManager");

    static_cast<Physics::PhysicsManager*>( this->TheWorld->GetManager(ManagerBase::MT_PhysicsManager) )->SetSimulationSubstepModifier(2);
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

    const String StatsScreenText = "Ubuntu-18";

    //GUI->LoadMTA("Catch_Loading.mta","Common");
    UI::Screen* StatsScreen = GUI->CreateScreen("StatsScreen","Catch_Loading",UIViewport,0);

    ////-------------------  FPS  Stats  -------------------////
    // Create the widget container for the current FPS display
    UI::Widget* SSCurrentFPS = StatsScreen->CreateWidget("SS_CurrentFPS",UI::UnifiedRect(0.008,0.065,0.23,0.065));
    SSCurrentFPS->SetAspectRatioLock(UI::ARL_Ratio_Y_Axis);
    SSCurrentFPS->SetMousePassthrough(true);
    // Create the widget for displaying the current FPS explanation text
    UI::Widget* SSCurrentFPSLabel = StatsScreen->CreateWidget("SS_CurrentFPSLabel",UI::UnifiedRect(0.0,0.0,0.65,1.0));
    SSCurrentFPSLabel->SetHorizontalPositioningRules(UI::PF_Anchor_Left);
    SSCurrentFPSLabel->SetVerticalPositioningRules(UI::PF_Anchor_Top);
    SSCurrentFPSLabel->SetMousePassthrough(true);
    UI::SingleLineTextLayer* SSCurrentFPSLabelLayer = SSCurrentFPSLabel->CreateSingleLineTextLayer(StatsScreenText,0,0);
    SSCurrentFPSLabelLayer->SetText("Current FPS: ");
    SSCurrentFPSLabelLayer->HorizontallyAlign(UI::LA_BottomRight);
    SSCurrentFPSLabelLayer->VerticallyAlign(UI::LA_Center);
    SSCurrentFPSLabelLayer->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,SSNormText);
    SSCurrentFPS->AddChild(SSCurrentFPSLabel,1);
    // Create the widget for displaying the current FPS value
    UI::Widget* SSCurrentFPSValue = StatsScreen->CreateWidget("SS_CurrentFPSValue",UI::UnifiedRect(0.0,0.0,0.35,1.0));
    SSCurrentFPSValue->SetHorizontalPositioningRules(UI::PF_Anchor_Right);
    SSCurrentFPSValue->SetVerticalPositioningRules(UI::PF_Anchor_Top);
    SSCurrentFPSValue->SetMousePassthrough(true);
    UI::SingleLineTextLayer* SSCurrentFPSValueLayer = SSCurrentFPSValue->CreateSingleLineTextLayer(StatsScreenText,0,0);
    SSCurrentFPSValueLayer->SetText("0");
    SSCurrentFPSValueLayer->HorizontallyAlign(UI::LA_TopLeft);
    SSCurrentFPSValueLayer->VerticallyAlign(UI::LA_Center);
    SSCurrentFPSValueLayer->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,SSNormText);
    SSCurrentFPS->AddChild(SSCurrentFPSValue,2);
    // Wrap up timer configuration
    StatsScreen->AddChild(SSCurrentFPS,1);

    // Create the widget container for the average FPS display
    UI::Widget* SSAverageFPS = StatsScreen->CreateWidget("SS_AverageFPS",UI::UnifiedRect(0.008,0.12,0.23,0.065));
    SSAverageFPS->SetAspectRatioLock(UI::ARL_Ratio_Y_Axis);
    SSAverageFPS->SetMousePassthrough(true);
    // Create the widget for displaying the average FPS explanation text
    UI::Widget* SSAverageFPSLabel = StatsScreen->CreateWidget("SS_AverageFPSLabel",UI::UnifiedRect(0.0,0.0,0.65,1.0));
    SSAverageFPSLabel->SetHorizontalPositioningRules(UI::PF_Anchor_Left);
    SSAverageFPSLabel->SetVerticalPositioningRules(UI::PF_Anchor_Top);
    SSAverageFPSLabel->SetMousePassthrough(true);
    UI::SingleLineTextLayer* SSAverageFPSLabelLayer = SSAverageFPSLabel->CreateSingleLineTextLayer(StatsScreenText,0,0);
    SSAverageFPSLabelLayer->SetText("Average FPS: ");
    SSAverageFPSLabelLayer->HorizontallyAlign(UI::LA_BottomRight);
    SSAverageFPSLabelLayer->VerticallyAlign(UI::LA_Center);
    SSAverageFPSLabelLayer->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,SSNormText);
    SSAverageFPS->AddChild(SSAverageFPSLabel,1);
    // Create the widget for displaying the average FPS value
    UI::Widget* SSAverageFPSValue = StatsScreen->CreateWidget("SS_AverageFPSValue",UI::UnifiedRect(0.0,0.0,0.35,1.0));
    SSAverageFPSValue->SetHorizontalPositioningRules(UI::PF_Anchor_Right);
    SSAverageFPSValue->SetVerticalPositioningRules(UI::PF_Anchor_Top);
    SSAverageFPSValue->SetMousePassthrough(true);
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

    const Real MMSmallText = 0.60;
    const Real MMNormText = 0.65;
    const Real MMTightText = 0.8;
    const Real MMLargeText = 0.85;

    const String MainMenuScreenText = "Ubuntu-18";

    GUI->LoadMTA("Catch_Menu.mta","Common");
    UI::Screen* MainMenuScreen = GUI->CreateScreen("MainMenuScreen","Catch_Menu",UIViewport,1);

    ////---------------  Screen  Background  ---------------////
    // Create the widget that will hold the background and lock it at a wide screen size
    UI::Widget* MMBackground = MainMenuScreen->CreateWidget("MS_Background",UI::UnifiedRect(0,0,1.7777,1));
    MMBackground->SetHorizontalPositioningRules(UI::PF_Anchor_HorizontalCenter);
    MMBackground->SetVerticalPositioningRules(UI::PF_Anchor_VerticalCenter);
    MMBackground->SetVerticalSizingRules(UI::SR_Unified_Dims);
    MMBackground->SetHorizontalSizingRules(UI::SR_Match_Other_Axis_Unified);
    MMBackground->CreateSingleImageLayer("MainMenuBackground",0,0);
    MainMenuScreen->AddChild(MMBackground,1);

    ////-----------------  Profile Select  -----------------////
    // Create the accessor that will open the profiles configuration
    UI::StackButton* MMProfilesAccess = MainMenuScreen->CreateStackButton("MS_ProfilesAccess",UI::UnifiedRect(0.0,0.0,0.30,0.06));
    MMProfilesAccess->SetHorizontalPositioningRules(UI::PF_Anchor_Right);
    MMProfilesAccess->SetVerticalPositioningRules(UI::PF_Anchor_Top);
    MMProfilesAccess->SetAspectRatioLock(UI::ARL_Ratio_Y_Axis);
    MMProfilesAccess->CreateSingleImageLayer("MMOptionsButton",UI::GroupOrderEntry(UI::Widget::WG_Normal,0));
    MMProfilesAccess->CreateSingleImageLayer("MMOptionsHoveredButton",UI::GroupOrderEntry(UI::Widget::WG_Hovered,0));
    UI::SingleLineTextLayer* MMProfilesAccessText = MMProfilesAccess->CreateSingleLineTextLayer(MainMenuScreenText,1,1);
    MMProfilesAccessText->SetText( this->ThePlayer->GetName() );
    MMProfilesAccessText->HorizontallyAlign(UI::LA_Center);
    MMProfilesAccessText->VerticallyAlign(UI::LA_Center);
    MMProfilesAccessText->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,MMNormText);
    MainMenuScreen->AddChild(MMProfilesAccess,2);

    // Create the root window for profile configuration
    UI::MenuEntry* MMProfilesWin = MainMenuScreen->CreateMenuEntry("MS_ProfilesWin",UI::UnifiedRect(0.0,0.0,0.5,0.40));
    MMProfilesWin->SetHorizontalPositioningRules(UI::PF_Anchor_HorizontalCenter);
    MMProfilesWin->SetVerticalPositioningRules(UI::PF_Anchor_VerticalCenter);
    MMProfilesWin->SetAspectRatioLock(UI::ARL_Ratio_Y_Axis);
    MMProfilesWin->CreateSingleImageLayer("MMOptionsBackground",0,0);
    MMProfilesWin->SetPushButton(MMProfilesAccess);
    MainMenuScreen->AddChild(MMProfilesWin,3);

    // Create the label for the profile window
    UI::Widget* MMProfilesWinLabel = MainMenuScreen->CreateWidget("MS_ProfilesWinLabel",UI::UnifiedRect(0.0,0.08,0.5,0.16));
    MMProfilesWinLabel->SetHorizontalPositioningRules(UI::PF_Anchor_HorizontalCenter);
    MMProfilesWinLabel->CreateSingleImageLayer("MMButton",0,0);
    UI::SingleLineTextLayer* MMProfilesWinLabelText = MMProfilesWinLabel->CreateSingleLineTextLayer(MainMenuScreenText,1,1);
    MMProfilesWinLabelText->SetText("Profile Select");
    MMProfilesWinLabelText->HorizontallyAlign(UI::LA_Center);
    MMProfilesWinLabelText->VerticallyAlign(UI::LA_Center);
    MMProfilesWinLabelText->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,MMNormText);
    MMProfilesWin->AddChild(MMProfilesWinLabel,1);

    // Create the EditBox that will allow players to create new profiles
    UI::EditBox* MMProfilesEntry = MainMenuScreen->CreateEditBox("MS_ProfilesEntry",UI::UnifiedRect(0.08,0.33,0.58,0.14),UI::RLT_SingleLineText,MainMenuScreenText);
    MMProfilesEntry->CreateSingleImageLayer("MMAppExitText",0,0);
    MMProfilesEntry->GetEditLayer()->SetScale(Vector2(0.94,1.0));
    MMProfilesEntry->GetEditLayer()->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,MMTightText);
    //MMProfilesEntry->GetEditLayer()->SetTextLineHorizontalAlignment(UI::LA_Center);
    MMProfilesEntry->GetEditLayer()->SetTextLineVerticalAlignment(UI::LA_Center);
    MMProfilesWin->AddChild(MMProfilesEntry,2);

    // Create the button that will use what is entered into the Editbox to create a profile
    UI::Button* MMProfilesCreate = MainMenuScreen->CreateButton("MS_ProfilesCreate",UI::UnifiedRect(0.68,0.32,0.24,0.16));
    MMProfilesCreate->CreateSingleImageLayer("MMButton",UI::GroupOrderEntry(UI::Widget::WG_Normal,0));
    MMProfilesCreate->CreateSingleImageLayer("MMHoveredButton",UI::GroupOrderEntry(UI::Widget::WG_Hovered,0));
    UI::SingleLineTextLayer* MMProfilesCreateText = MMProfilesCreate->CreateSingleLineTextLayer(MainMenuScreenText,1,1);
    MMProfilesCreateText->SetText("Create");
    MMProfilesCreateText->HorizontallyAlign(UI::LA_Center);
    MMProfilesCreateText->VerticallyAlign(UI::LA_Center);
    MMProfilesCreateText->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,MMNormText);
    MMProfilesCreate->Subscribe(UI::Button::EventDeactivated,new MSProfileCreate(this->Profiles,MainMenuScreen),true);
    MMProfilesWin->AddChild(MMProfilesCreate,3);

    // Create the DropdownList that will display the currently available profiles
    UI::DropDownList* MMProfilesList = MainMenuScreen->CreateDropDownList("MS_ProfilesList",UI::UnifiedRect(0.08,0.57,0.58,0.14),UI::SB_Separate);
    // Configure the selection display
    UI::Widget* MMProfilesDisplay = MMProfilesList->GetSelectionDisplay();
    MMProfilesDisplay->CreateSingleImageLayer("MMListSelection",0,0);
    UI::SingleLineTextLayer* MMProfilesDisplayText = static_cast<UI::SingleLineTextLayer*>( MMProfilesDisplay->GetRenderLayer(0,UI::RLT_SingleLineText) );
    MMProfilesDisplayText->SetDefaultFont(MainMenuScreenText);
    MMProfilesDisplayText->HorizontallyAlign(UI::LA_Center);
    MMProfilesDisplayText->VerticallyAlign(UI::LA_Center);
    MMProfilesDisplayText->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,MMTightText);
    // Configure the list toggle
    UI::CheckBox* MMProfilesToggle = MMProfilesList->GetListToggle();
    UI::SingleImageLayer* MMProfilesToggleNormal = MMProfilesToggle->CreateSingleImageLayer("MMListScrollDown");
    UI::SingleImageLayer* MMProfilesToggleHovered = MMProfilesToggle->CreateSingleImageLayer("MMHoveredListScrollDown");
    MMProfilesToggle->AddLayerToGroup(MMProfilesToggleNormal,UI::GroupOrderEntry(UI::Widget::WG_Normal,0));
    MMProfilesToggle->AddLayerToGroup(MMProfilesToggleHovered,UI::GroupOrderEntry(UI::Widget::WG_Hovered,0));
    MMProfilesToggle->AddLayerToGroup(MMProfilesToggleNormal,0,UI::CheckBox::WG_SelectedNormal);
    MMProfilesToggle->AddLayerToGroup(MMProfilesToggleHovered,0,UI::CheckBox::WG_SelectedHovered);
    // Configure the option list
    UI::ListBox* MMProfilesOptions = MMProfilesList->GetSelectionList();
    MMProfilesOptions->SetListItemFont(MainMenuScreenText);
    MMProfilesOptions->SetListItemTextScale(UI::TextLayer::SM_ParentRelative,MMLargeText);
    // Configure the background for the listbox container
    UI::VerticalContainer* MMProfilesOptionsList = MMProfilesOptions->GetListContainer();
    MMProfilesOptionsList->CreateSingleImageLayer("MMListBackground",0,0);
    MMProfilesOptionsList->SetChildSizing(UI::SizingInfo(UI::UnifiedVec2(1.0,0.19)),UI::LinearContainer::SE_OnAdd);
    // Configure the scrollbar for the option list
    UI::VerticalScrollbar* MMProfilesOptionsScroll = MMProfilesOptions->GetListScroll();
    MMProfilesOptionsScroll->SetIncrementDistance(0.05);
    // Configure the scroller
    MMProfilesOptionsScroll->GetScroller()->CreateSingleImageLayer("MMListScroller",UI::GroupOrderEntry(UI::Widget::WG_Normal,0));
    MMProfilesOptionsScroll->GetScroller()->CreateSingleImageLayer("MMHoveredListScroller",UI::GroupOrderEntry(UI::Widget::WG_Hovered,0));
    // Configure the up button
    MMProfilesOptionsScroll->GetUpLeftButton()->CreateSingleImageLayer("MMListScrollUp",UI::GroupOrderEntry(UI::Widget::WG_Normal,0));
    MMProfilesOptionsScroll->GetUpLeftButton()->CreateSingleImageLayer("MMHoveredListScrollUp",UI::GroupOrderEntry(UI::Widget::WG_Hovered,0));
    // Configure the down button
    MMProfilesOptionsScroll->GetDownRightButton()->CreateSingleImageLayer("MMListScrollDown",UI::GroupOrderEntry(UI::Widget::WG_Normal,0));
    MMProfilesOptionsScroll->GetDownRightButton()->CreateSingleImageLayer("MMHoveredListScrollDown",UI::GroupOrderEntry(UI::Widget::WG_Hovered,0));
    // Configure the scroll back
    MMProfilesOptionsScroll->GetScrollBack()->CreateSingleImageLayer("MMListScrollBackground",0,0);
    // Wrap up listing configuration
    MMProfilesWin->AddChild(MMProfilesList,8);

    // Create the button that will destroy a selected profile
    /*UI::Button* MMProfilesDestroy = MainMenuScreen->CreateButton("MS_ProfilesDestroy",UI::UnifiedRect(0.68,0.56,0.24,0.16));
    MMProfilesDestroy->CreateSingleImageLayer("MMButton",UI::GroupOrderEntry(UI::Widget::WG_Normal,0));
    MMProfilesDestroy->CreateSingleImageLayer("MMHoveredButton",UI::GroupOrderEntry(UI::Widget::WG_Hovered,0));
    UI::SingleLineTextLayer* MMProfilesDestroyText = MMProfilesDestroy->CreateSingleLineTextLayer(MainMenuScreenText,1,1);
    MMProfilesDestroyText->SetText("Destroy");
    MMProfilesDestroyText->HorizontallyAlign(UI::LA_Center);
    MMProfilesDestroyText->VerticallyAlign(UI::LA_Center);
    MMProfilesDestroyText->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,MMNormText);
    //MMLevelStart->Subscribe(UI::Button::EventDeactivated,new MSLevelStart(MMLevelSelectGrid),true);
    MMProfilesWin->AddChild(MMProfilesDestroy,5);// */

    // Create the button that will confirm the switch to another profile
    //UI::Button* MMProfilesSelect = MainMenuScreen->CreateButton("MS_ProfilesSelect",UI::UnifiedRect(0.12,0.76,0.34,0.16));
    UI::Button* MMProfilesSelect = MainMenuScreen->CreateButton("MS_ProfilesSelect",UI::UnifiedRect(0.68,0.56,0.24,0.16));
    MMProfilesSelect->CreateSingleImageLayer("MMButton",UI::GroupOrderEntry(UI::Widget::WG_Normal,0));
    MMProfilesSelect->CreateSingleImageLayer("MMHoveredButton",UI::GroupOrderEntry(UI::Widget::WG_Hovered,0));
    UI::SingleLineTextLayer* MMProfilesSelectText = MMProfilesSelect->CreateSingleLineTextLayer(MainMenuScreenText,1,1);
    MMProfilesSelectText->SetText("Select");
    MMProfilesSelectText->HorizontallyAlign(UI::LA_Center);
    MMProfilesSelectText->VerticallyAlign(UI::LA_Center);
    MMProfilesSelectText->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,MMNormText);
    MMProfilesSelect->Subscribe(UI::Button::EventDeactivated,new MSProfileSelect(this->Profiles,MainMenuScreen),true);
    MMProfilesWin->AddChild(MMProfilesSelect,6);

    // Create the back button for the profile configuration window
    //UI::StackButton* MMProfilesReturn = MainMenuScreen->CreateStackButton("MS_ProfilesReturn",UI::UnifiedRect(0.54,0.76,0.34,0.16));
    UI::StackButton* MMProfilesReturn = MainMenuScreen->CreateStackButton("MS_ProfilesReturn",UI::UnifiedRect(0.32,0.76,0.34,0.16));
    MMProfilesReturn->CreateSingleImageLayer("MMButton",UI::GroupOrderEntry(UI::Widget::WG_Normal,0));
    MMProfilesReturn->CreateSingleImageLayer("MMHoveredButton",UI::GroupOrderEntry(UI::Widget::WG_Hovered,0));
    UI::SingleLineTextLayer* MMProfilesReturnText = MMProfilesReturn->CreateSingleLineTextLayer(MainMenuScreenText,1,1);
    MMProfilesReturnText->SetText("Back");
    MMProfilesReturnText->HorizontallyAlign(UI::LA_Center);
    MMProfilesReturnText->VerticallyAlign(UI::LA_Center);
    MMProfilesReturnText->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,MMNormText);
    MMProfilesWin->SetPopButton(MMProfilesReturn);
    MMProfilesWin->AddChild(MMProfilesReturn,7);

    ////-----------------  Main Menu Root  -----------------////
    // Create the root menu entry
    UI::MenuEntry* MMRootEntry = MainMenuScreen->CreateMenuEntry("MS_MenuRoot",UI::UnifiedRect(0.0,0.914,1.0,0.086));
    MMRootEntry->SetHorizontalPositioningRules(UI::PF_Anchor_HorizontalCenter);
    MMRootEntry->SetVerticalPositioningRules(UI::PF_Anchor_Bottom);
    MMRootEntry->SetAspectRatioLock(UI::ARL_Ratio_Y_Axis);
    MMRootEntry->CreateSingleImageLayer("MMBrickBackground",0,0);
    MMRootEntry->SetAutoHide(false);
    MainMenuScreen->AddChild(MMRootEntry,4);
    MMRootEntry->ForceRootEntryVisible();

    ////------------------  Level Select  ------------------////
    // Start with the accessor button
    UI::StackButton* MMLevelSelectAccess = MainMenuScreen->CreateStackButton("MS_LevelSelect",UI::UnifiedRect(0.05,0.18,0.22,0.7));
    MMLevelSelectAccess->CreateSingleImageLayer("MMButton",UI::GroupOrderEntry(UI::Widget::WG_Normal,0));
    MMLevelSelectAccess->CreateSingleImageLayer("MMHoveredButton",UI::GroupOrderEntry(UI::Widget::WG_Hovered,0));
    UI::SingleLineTextLayer* MMLevelSelectAccessText = MMLevelSelectAccess->CreateSingleLineTextLayer(MainMenuScreenText,1,1);
    MMLevelSelectAccessText->SetText("Level Select");
    MMLevelSelectAccessText->HorizontallyAlign(UI::LA_Center);
    MMLevelSelectAccessText->VerticallyAlign(UI::LA_Center);
    MMLevelSelectAccessText->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,MMNormText);
    MMRootEntry->AddChild(MMLevelSelectAccess,1);

    // Create and configure the "window" that houses the level selections
    UI::MenuEntry* MMLevelSelectWin = MainMenuScreen->CreateMenuEntry("MS_LevelSelectWin",UI::UnifiedRect(0.05,-10.3,0.90,9.68));
    MMLevelSelectWin->CreateSingleImageLayer("MMLSBackground",0,0);
    MMLevelSelectWin->SetPushButton(MMLevelSelectAccess);
    MMRootEntry->AddChild(MMLevelSelectWin,5);

    // Create the spinner which will allow browsing the grid
    UI::Spinner* MMLevelSelectSpinner = MainMenuScreen->CreateSpinner("MS_LevelSelectSpinner",UI::UnifiedRect(0.65,0.87,0.25,0.08),UI::Spn_Separate_Horizontal,MainMenuScreenText);
    MMLevelSelectSpinner->GetIncrement()->CreateSingleImageLayer("MMIncrementPage",0,0);
    MMLevelSelectSpinner->GetDecrement()->CreateSingleImageLayer("MMDecrementPage",0,0);
    MMLevelSelectSpinner->GetValueDisplay()->CreateSingleImageLayer("MMPageBox",0,0);
    MMLevelSelectSpinner->GetValueDisplay()->GetEditLayer()->SetTextLineHorizontalAlignment(UI::LA_Center);
    MMLevelSelectSpinner->GetValueDisplay()->GetEditLayer()->SetTextLineVerticalAlignment(UI::LA_Center);
    MMLevelSelectSpinner->GetValueDisplay()->GetEditLayer()->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,MMNormText);
    MMLevelSelectSpinner->SetSpinValue(1.0);
    MMLevelSelectWin->AddChild(MMLevelSelectSpinner,2);

    // Create the container that will display all of our levels
    UI::GridContainer* MMLevelSelectGrid = MainMenuScreen->CreateGridContainer("MS_LevelSelectGrid",UI::UnifiedRect(0.11,0.10,0.82,0.76));
    MMLevelSelectGrid->SetCellSize(2,4);
    MMLevelSelectGrid->SetCellPadding( UI::UnifiedVec2(0.08,0.10) );
    MMLevelSelectGrid->SetXYProvider(MMLevelSelectSpinner);
    MMLevelSelectGrid->Subscribe(UI::PagedContainer::EventChildSelected,new MSLevelCellSelect(MainMenuScreen),true);
    MMLevelSelectWin->AddChild(MMLevelSelectGrid,3);

    // Create the button that will launch the level
    UI::Button* MMLevelStart = MainMenuScreen->CreateButton("MS_LevelStart",UI::UnifiedRect(0.0,0.86,0.16,0.10));
    MMLevelStart->SetHorizontalPositioningRules(UI::PF_Anchor_HorizontalCenter);
    MMLevelStart->CreateSingleImageLayer("MMLevelStart",UI::GroupOrderEntry(UI::Widget::WG_Normal,0));
    MMLevelStart->CreateSingleImageLayer("MMLevelStartHovered",UI::GroupOrderEntry(UI::Widget::WG_Hovered,0));
    UI::SingleLineTextLayer* MMLevelStartText = MMLevelStart->CreateSingleLineTextLayer(MainMenuScreenText,1,1);
    MMLevelStartText->SetText("Start");
    MMLevelStartText->HorizontallyAlign(UI::LA_Center);
    MMLevelStartText->VerticallyAlign(UI::LA_Center);
    MMLevelStartText->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,MMNormText);
    MMLevelStart->Subscribe(UI::Button::EventDeactivated,new MSLevelStart(MMLevelSelectGrid),true);
    MMLevelSelectWin->AddChild(MMLevelStart,4);

    ////------------------  Options  ------------------////
    // Start with the accessor button
    UI::StackButton* MMOptionsAccess = MainMenuScreen->CreateStackButton("MS_Options",UI::UnifiedRect(0.28,0.18,0.22,0.7));
    MMOptionsAccess->CreateSingleImageLayer("MMButton",UI::GroupOrderEntry(UI::Widget::WG_Normal,0));
    MMOptionsAccess->CreateSingleImageLayer("MMHoveredButton",UI::GroupOrderEntry(UI::Widget::WG_Hovered,0));
    UI::SingleLineTextLayer* MMOptionsAccessText = MMOptionsAccess->CreateSingleLineTextLayer(MainMenuScreenText,1,1);
    MMOptionsAccessText->SetText("Options");
    MMOptionsAccessText->HorizontallyAlign(UI::LA_Center);
    MMOptionsAccessText->VerticallyAlign(UI::LA_Center);
    MMOptionsAccessText->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,MMNormText);
    MMRootEntry->AddChild(MMOptionsAccess,2);

    // Create and configure the "window" that houses the options display
    UI::MenuEntry* MMOptionsWin = MainMenuScreen->CreateMenuEntry("MS_OptionsWin",UI::UnifiedRect(0.18,-8.08,0.64,6.4));
    MMOptionsWin->CreateSingleImageLayer("MMOptionsBackground",0,0);
    MMOptionsWin->SetPushButton(MMOptionsAccess);
    MMRootEntry->AddChild(MMOptionsWin,6);

    // Create the first of the two buttons that will display the two sets of options (video options)
    UI::StackButton* MMVideoSetAccess = MainMenuScreen->CreateStackButton("MS_VideoSetAccess",UI::UnifiedRect(0.11,0.0365,0.34,0.11));
    MMVideoSetAccess->CreateSingleImageLayer("MMButton",UI::GroupOrderEntry(UI::Widget::WG_Normal,0));
    MMVideoSetAccess->CreateSingleImageLayer("MMHoveredButton",UI::GroupOrderEntry(UI::Widget::WG_Hovered,0));
    UI::SingleLineTextLayer* MMVideoSetAccessText = MMVideoSetAccess->CreateSingleLineTextLayer(MainMenuScreenText,1,1);
    MMVideoSetAccessText->SetText("Video Options");
    MMVideoSetAccessText->HorizontallyAlign(UI::LA_Center);
    MMVideoSetAccessText->VerticallyAlign(UI::LA_Center);
    MMVideoSetAccessText->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,MMNormText);
    MMOptionsWin->AddChild(MMVideoSetAccess,1);

    // Create the second of the two buttons that will display the two sets of options (audio options)
    UI::StackButton* MMAudioSetAccess = MainMenuScreen->CreateStackButton("MS_AudioSetAccess",UI::UnifiedRect(0.55,0.0365,0.34,0.11));
    MMAudioSetAccess->CreateSingleImageLayer("MMButton",UI::GroupOrderEntry(UI::Widget::WG_Normal,0));
    MMAudioSetAccess->CreateSingleImageLayer("MMHoveredButton",UI::GroupOrderEntry(UI::Widget::WG_Hovered,0));
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
    MMResolutionLabel->CreateSingleImageLayer("MMButton",0,0);
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
    MMResolutionDisplay->CreateSingleImageLayer("MMListSelection",0,0);
    UI::SingleLineTextLayer* MMResolutionDisplayText = static_cast<UI::SingleLineTextLayer*>( MMResolutionDisplay->GetRenderLayer(0,UI::RLT_SingleLineText) );
    MMResolutionDisplayText->SetDefaultFont(MainMenuScreenText);
    MMResolutionDisplayText->HorizontallyAlign(UI::LA_Center);
    MMResolutionDisplayText->VerticallyAlign(UI::LA_Center);
    MMResolutionDisplayText->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,MMTightText);
    // Configure the list toggle
    UI::CheckBox* MMResolutionToggle = MMResolutionList->GetListToggle();
    UI::SingleImageLayer* MMResolutionToggleNormal = MMResolutionToggle->CreateSingleImageLayer("MMListScrollDown");
    UI::SingleImageLayer* MMResolutionToggleHovered = MMResolutionToggle->CreateSingleImageLayer("MMHoveredListScrollDown");
    MMResolutionToggle->AddLayerToGroup(MMResolutionToggleNormal,UI::GroupOrderEntry(UI::Widget::WG_Normal,0));
    MMResolutionToggle->AddLayerToGroup(MMResolutionToggleHovered,UI::GroupOrderEntry(UI::Widget::WG_Hovered,0));
    MMResolutionToggle->AddLayerToGroup(MMResolutionToggleNormal,0,UI::CheckBox::WG_SelectedNormal);
    MMResolutionToggle->AddLayerToGroup(MMResolutionToggleHovered,0,UI::CheckBox::WG_SelectedHovered);
    // Configure the option list
    UI::ListBox* MMResolutionOptions = MMResolutionList->GetSelectionList();
    MMResolutionOptions->SetListItemFont(MainMenuScreenText);
    MMResolutionOptions->SetListItemTextScale(UI::TextLayer::SM_ParentRelative,MMLargeText);
    // Configure the background for the listbox container
    UI::VerticalContainer* MMResolutionOptionsList = MMResolutionOptions->GetListContainer();
    MMResolutionOptionsList->CreateSingleImageLayer("MMListBackground",0,0);
    MMResolutionOptionsList->SetChildSizing(UI::SizingInfo(UI::UnifiedVec2(1.0,0.19)),UI::LinearContainer::SE_OnAdd);
    // Configure the scrollbar for the option list
    UI::VerticalScrollbar* MMResolutionOptionsScroll = MMResolutionOptions->GetListScroll();
    MMResolutionOptionsScroll->SetIncrementDistance(0.05);
    // Configure the scroller
    MMResolutionOptionsScroll->GetScroller()->CreateSingleImageLayer("MMListScroller",UI::GroupOrderEntry(UI::Widget::WG_Normal,0));
    MMResolutionOptionsScroll->GetScroller()->CreateSingleImageLayer("MMHoveredListScroller",UI::GroupOrderEntry(UI::Widget::WG_Hovered,0));
    // Configure the up button
    MMResolutionOptionsScroll->GetUpLeftButton()->CreateSingleImageLayer("MMListScrollUp",UI::GroupOrderEntry(UI::Widget::WG_Normal,0));
    MMResolutionOptionsScroll->GetUpLeftButton()->CreateSingleImageLayer("MMHoveredListScrollUp",UI::GroupOrderEntry(UI::Widget::WG_Hovered,0));
    // Configure the down button
    MMResolutionOptionsScroll->GetDownRightButton()->CreateSingleImageLayer("MMListScrollDown",UI::GroupOrderEntry(UI::Widget::WG_Normal,0));
    MMResolutionOptionsScroll->GetDownRightButton()->CreateSingleImageLayer("MMHoveredListScrollDown",UI::GroupOrderEntry(UI::Widget::WG_Hovered,0));
    // Configure the scroll back
    MMResolutionOptionsScroll->GetScrollBack()->CreateSingleImageLayer("MMListScrollBackground",0,0);
    // Wrap up listing configuration
    MMResolutionList->Subscribe(UI::Widget::EventVisibilityShown,this->VideoSettingsWork->GetSettingsSubscriber());
    MMVideoSet->AddChild(MMResolutionList,11);

    // Create the checkbox for enabling or disabling fullscreen
    UI::CheckBox* MMFullscreenBox = MainMenuScreen->CreateCheckBox("MS_FullscreenBox",UI::UnifiedRect(0.655,0.145,0,0.12));
    MMFullscreenBox->SetHorizontalSizingRules(UI::SR_Match_Other_Axis);
    MMFullscreenBox->CreateSingleImageLayer("MMCheckboxUnchecked",UI::GroupOrderEntry(UI::Widget::WG_Normal,0));
    MMFullscreenBox->CreateSingleImageLayer("MMHoveredCheckboxUnchecked",UI::GroupOrderEntry(UI::Widget::WG_Hovered,0));
    MMFullscreenBox->CreateSingleImageLayer("MMCheckboxChecked",UI::GroupOrderEntry(UI::CheckBox::WG_SelectedNormal,0));
    MMFullscreenBox->CreateSingleImageLayer("MMHoveredCheckboxChecked",UI::GroupOrderEntry(UI::CheckBox::WG_SelectedHovered,0));
    MMFullscreenBox->Subscribe(UI::Widget::EventVisibilityShown,this->VideoSettingsWork->GetSettingsSubscriber());
    MMVideoSet->AddChild(MMFullscreenBox,3);

    // Create the label for the fullscreen checkbox
    UI::Widget* MMFullscreenLabel = MainMenuScreen->CreateWidget("MS_FullscreenLabel",UI::UnifiedRect(0.72,0.145,0.22,0.12));
    MMFullscreenLabel->CreateSingleImageLayer("MMButton",0,0);
    UI::SingleLineTextLayer* MMFullscreenLabelText = MMFullscreenLabel->CreateSingleLineTextLayer(MainMenuScreenText,1,1);
    MMFullscreenLabelText->SetText("Fullscreen");
    MMFullscreenLabelText->HorizontallyAlign(UI::LA_Center);
    MMFullscreenLabelText->VerticallyAlign(UI::LA_Center);
    MMFullscreenLabelText->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,MMTightText);
    MMVideoSet->AddChild(MMFullscreenLabel,4);

    // Create the label for the FSAA options
    UI::Widget* MMFSAALabel = MainMenuScreen->CreateWidget("MS_FSAALabel",UI::UnifiedRect(0.12,0.365,0.415,0.13));
    MMFSAALabel->CreateSingleImageLayer("MMButton",0,0);
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
    MMFSAADisplay->CreateSingleImageLayer("MMListSelection",0,0);
    UI::SingleLineTextLayer* MMFSAADisplayText = static_cast<UI::SingleLineTextLayer*>( MMFSAADisplay->GetRenderLayer(0,UI::RLT_SingleLineText) );
    MMFSAADisplayText->SetDefaultFont(MainMenuScreenText);
    MMFSAADisplayText->HorizontallyAlign(UI::LA_Center);
    MMFSAADisplayText->VerticallyAlign(UI::LA_Center);
    MMFSAADisplayText->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,MMTightText);
    // Configure the list toggle
    UI::CheckBox* MMFSAAToggle = MMFSAAList->GetListToggle();
    UI::SingleImageLayer* MMFSAAToggleNormal = MMFSAAToggle->CreateSingleImageLayer("MMListScrollDown");
    UI::SingleImageLayer* MMFSAAToggleHovered = MMFSAAToggle->CreateSingleImageLayer("MMHoveredListScrollDown");
    MMFSAAToggle->AddLayerToGroup(MMFSAAToggleNormal,UI::GroupOrderEntry(UI::Widget::WG_Normal,0));
    MMFSAAToggle->AddLayerToGroup(MMFSAAToggleHovered,UI::GroupOrderEntry(UI::Widget::WG_Hovered,0));
    MMFSAAToggle->AddLayerToGroup(MMFSAAToggleNormal,UI::GroupOrderEntry(UI::CheckBox::WG_SelectedNormal,0));
    MMFSAAToggle->AddLayerToGroup(MMFSAAToggleHovered,UI::GroupOrderEntry(UI::CheckBox::WG_SelectedHovered,0));
    // Configure the option list
    UI::ListBox* MMFSAAOptions = MMFSAAList->GetSelectionList();
    MMFSAAOptions->SetListItemFont(MainMenuScreenText);
    MMFSAAOptions->SetListItemTextScale(UI::TextLayer::SM_ParentRelative,MMLargeText);
    // Configure the background for the listbox container
    UI::VerticalContainer* MMFSAAOptionsList = MMFSAAOptions->GetListContainer();
    MMFSAAOptionsList->CreateSingleImageLayer("MMListBackground",0,0);
    MMFSAAOptionsList->SetChildSizing(UI::SizingInfo(UI::UnifiedVec2(1.0,0.19)),UI::LinearContainer::SE_OnAdd);
    // Configure the scrollbar for the option list
    UI::VerticalScrollbar* MMFSAAOptionsScroll = MMFSAAOptions->GetListScroll();
    MMFSAAOptionsScroll->SetIncrementDistance(0.05);
    // Configure the scroller
    MMFSAAOptionsScroll->GetScroller()->CreateSingleImageLayer("MMListScroller",UI::GroupOrderEntry(UI::Widget::WG_Normal,0));
    MMFSAAOptionsScroll->GetScroller()->CreateSingleImageLayer("MMHoveredListScroller",UI::GroupOrderEntry(UI::Widget::WG_Hovered,0));
    // Configure the up button
    MMFSAAOptionsScroll->GetUpLeftButton()->CreateSingleImageLayer("MMListScrollUp",UI::GroupOrderEntry(UI::Widget::WG_Normal,0));
    MMFSAAOptionsScroll->GetUpLeftButton()->CreateSingleImageLayer("MMHoveredListScrollUp",UI::GroupOrderEntry(UI::Widget::WG_Hovered,0));
    // Configure the down button
    MMFSAAOptionsScroll->GetDownRightButton()->CreateSingleImageLayer("MMListScrollDown",UI::GroupOrderEntry(UI::Widget::WG_Normal,0));
    MMFSAAOptionsScroll->GetDownRightButton()->CreateSingleImageLayer("MMHoveredListScrollDown",UI::GroupOrderEntry(UI::Widget::WG_Hovered,0));
    // Configure the scroll back
    MMFSAAOptionsScroll->GetScrollBack()->CreateSingleImageLayer("MMListScrollBackground",0,0);
    // Wrap up listing configuration
    MMFSAAList->Subscribe(UI::Widget::EventVisibilityShown,this->VideoSettingsWork->GetSettingsSubscriber());
    MMVideoSet->AddChild(MMFSAAList,10);

    // Create the checkbox for enabling or disabling FPS stats display
    UI::CheckBox* MMStatsBox = MainMenuScreen->CreateCheckBox("MS_StatsBox",UI::UnifiedRect(0.655,0.50,0,0.12));
    MMStatsBox->SetHorizontalSizingRules(UI::SR_Match_Other_Axis);
    MMStatsBox->CreateSingleImageLayer("MMCheckboxUnchecked",UI::GroupOrderEntry(UI::Widget::WG_Normal,0));
    MMStatsBox->CreateSingleImageLayer("MMHoveredCheckboxUnchecked",UI::GroupOrderEntry(UI::Widget::WG_Hovered,0));
    MMStatsBox->CreateSingleImageLayer("MMCheckboxChecked",UI::GroupOrderEntry(UI::CheckBox::WG_SelectedNormal,0));
    MMStatsBox->CreateSingleImageLayer("MMHoveredCheckboxChecked",UI::GroupOrderEntry(UI::CheckBox::WG_SelectedHovered,0));
    MMStatsBox->Subscribe(UI::Widget::EventVisibilityShown,this->VideoSettingsWork->GetSettingsSubscriber());
    MMVideoSet->AddChild(MMStatsBox,7);

    // Create the label for the FPS stats display checkbox
    UI::Widget* MMStatsLabel = MainMenuScreen->CreateWidget("MS_StatsLabel",UI::UnifiedRect(0.72,0.50,0.22,0.12));
    MMStatsLabel->CreateSingleImageLayer("MMButton",0,0);
    UI::SingleLineTextLayer* MMStatsLabelText = MMStatsLabel->CreateSingleLineTextLayer(MainMenuScreenText,1,1);
    MMStatsLabelText->SetText("Show FPS");
    MMStatsLabelText->HorizontallyAlign(UI::LA_Center);
    MMStatsLabelText->VerticallyAlign(UI::LA_Center);
    MMStatsLabelText->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,MMTightText);
    MMVideoSet->AddChild(MMStatsLabel,8);

    // Create the button that will apply all of the currently displayed video settings
    UI::Button* MMVideoOptsApply = MainMenuScreen->CreateButton("MS_VideoOptsApply",UI::UnifiedRect(0.815,0.84,0.17,0.14));
    MMVideoOptsApply->CreateSingleImageLayer("MMOptionsApplyButton",UI::GroupOrderEntry(UI::Widget::WG_Normal,0));
    MMVideoOptsApply->CreateSingleImageLayer("MMOptionsApplyHoveredButton",UI::GroupOrderEntry(UI::Widget::WG_Hovered,0));
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
    MMMusicVolLabel->SetHorizontalPositioningRules(UI::PF_Anchor_HorizontalCenter);
    MMMusicVolLabel->CreateSingleImageLayer("MMButton",0,0);
    UI::SingleLineTextLayer* MMMusicVolLabelText = MMMusicVolLabel->CreateSingleLineTextLayer(MainMenuScreenText,1,1);
    MMMusicVolLabelText->SetText("Music Volume");
    MMMusicVolLabelText->HorizontallyAlign(UI::LA_Center);
    MMMusicVolLabelText->VerticallyAlign(UI::LA_Center);
    MMMusicVolLabelText->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,MMTightText);
    MMAudioSet->AddChild(MMMusicVolLabel,1);

    // Create the Music volume slider
    UI::HorizontalScrollbar* MMMusicVol = MainMenuScreen->CreateHorizontalScrollbar("MS_MusicVolume",UI::UnifiedRect(0.0,0.145,0.7,0.09),UI::SB_Separate);
    MMMusicVol->SetHorizontalPositioningRules(UI::PF_Anchor_HorizontalCenter);
    MMMusicVol->SetIncrementDistance(MMScrollerSize * 0.5);
    MMMusicVol->SetScrollerSize(MMScrollerSize);
    // Configure the scroller
    MMMusicVol->GetScroller()->CreateSingleImageLayer("MMScroller",UI::GroupOrderEntry(UI::Widget::WG_Normal,0));
    MMMusicVol->GetScroller()->CreateSingleImageLayer("MMHoveredScroller",UI::GroupOrderEntry(UI::Widget::WG_Hovered,0));
    // Configure the up button
    MMMusicVol->GetUpLeftButton()->CreateSingleImageLayer("MMScrollLeft",UI::GroupOrderEntry(UI::Widget::WG_Normal,0));
    MMMusicVol->GetUpLeftButton()->CreateSingleImageLayer("MMHoveredScrollLeft",UI::GroupOrderEntry(UI::Widget::WG_Hovered,0));
    // Configure the down button
    MMMusicVol->GetDownRightButton()->CreateSingleImageLayer("MMScrollRight",UI::GroupOrderEntry(UI::Widget::WG_Normal,0));
    MMMusicVol->GetDownRightButton()->CreateSingleImageLayer("MMHoveredScrollRight",UI::GroupOrderEntry(UI::Widget::WG_Hovered,0));
    // Configure the scroll back
    MMMusicVol->GetScrollBack()->CreateSingleImageLayer("MMScrollBackground",0,0);
    // Wrap up Effects volume ocnfiguration
    MMMusicVol->Subscribe(UI::Scrollbar::EventScrollValueChanged,this->AudioSettingsWork->GetSettingsSubscriber());
    MMMusicVol->Subscribe(UI::Widget::EventVisibilityShown,this->AudioSettingsWork->GetSettingsSubscriber());
    MMAudioSet->AddChild(MMMusicVol,2);

    // Create the label for the Effects volume
    UI::Widget* MMEffectsVolLabel = MainMenuScreen->CreateWidget("MS_EffectsVolLabel",UI::UnifiedRect(0.0,0.275,0.40,0.13));
    MMEffectsVolLabel->SetHorizontalPositioningRules(UI::PF_Anchor_HorizontalCenter);
    MMEffectsVolLabel->CreateSingleImageLayer("MMButton",0,0);
    UI::SingleLineTextLayer* MMEffectsVolLabelText = MMEffectsVolLabel->CreateSingleLineTextLayer(MainMenuScreenText,1,1);
    MMEffectsVolLabelText->SetText("Effects Volume");
    MMEffectsVolLabelText->HorizontallyAlign(UI::LA_Center);
    MMEffectsVolLabelText->VerticallyAlign(UI::LA_Center);
    MMEffectsVolLabelText->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,MMTightText);
    MMAudioSet->AddChild(MMEffectsVolLabel,3);

    // Create the Effects volume slider
    UI::HorizontalScrollbar* MMEffectsVol = MainMenuScreen->CreateHorizontalScrollbar("MS_EffectsVolume",UI::UnifiedRect(0.0,0.41,0.7,0.09),UI::SB_Separate);
    MMEffectsVol->SetHorizontalPositioningRules(UI::PF_Anchor_HorizontalCenter);
    MMEffectsVol->SetIncrementDistance(MMScrollerSize * 0.5);
    MMEffectsVol->SetScrollerSize(MMScrollerSize);
    // Configure the scroller
    MMEffectsVol->GetScroller()->CreateSingleImageLayer("MMScroller",UI::GroupOrderEntry(UI::Widget::WG_Normal,0));
    MMEffectsVol->GetScroller()->CreateSingleImageLayer("MMHoveredScroller",UI::GroupOrderEntry(UI::Widget::WG_Hovered,0));
    // Configure the up button
    MMEffectsVol->GetUpLeftButton()->CreateSingleImageLayer("MMScrollLeft",UI::GroupOrderEntry(UI::Widget::WG_Normal,0));
    MMEffectsVol->GetUpLeftButton()->CreateSingleImageLayer("MMHoveredScrollLeft",UI::GroupOrderEntry(UI::Widget::WG_Hovered,0));
    // Configure the down button
    MMEffectsVol->GetDownRightButton()->CreateSingleImageLayer("MMScrollRight",UI::GroupOrderEntry(UI::Widget::WG_Normal,0));
    MMEffectsVol->GetDownRightButton()->CreateSingleImageLayer("MMHoveredScrollRight",UI::GroupOrderEntry(UI::Widget::WG_Hovered,0));
    // Configure the scroll back
    MMEffectsVol->GetScrollBack()->CreateSingleImageLayer("MMScrollBackground",0,0);
    // Wrap up Effects volume ocnfiguration
    MMEffectsVol->Subscribe(UI::Scrollbar::EventScrollValueChanged,this->AudioSettingsWork->GetSettingsSubscriber());
    MMEffectsVol->Subscribe(UI::Widget::EventVisibilityShown,this->AudioSettingsWork->GetSettingsSubscriber());
    MMAudioSet->AddChild(MMEffectsVol,2);

    /*// Create the label for the Audio Device options
    UI::Widget* MMAudioDeviceLabel = MainMenuScreen->CreateWidget("MS_AudioDeviceLabel",UI::UnifiedRect(0.12,0.56,0.415,0.13));
    MMAudioDeviceLabel->CreateSingleImageLayer("MMButton",0,0);
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
    MMAudioDeviceDisplay->CreateSingleImageLayer("MMListSelection",0,0);
    UI::SingleLineTextLayer* MMAudioDeviceDisplayText = static_cast<UI::SingleLineTextLayer*>( MMAudioDeviceDisplay->GetRenderLayer(0,UI::RLT_SingleLineText) );
    MMAudioDeviceDisplayText->SetDefaultFont(MainMenuScreenText);
    MMAudioDeviceDisplayText->HorizontallyAlign(UI::LA_Center);
    MMAudioDeviceDisplayText->VerticallyAlign(UI::LA_Center);
    MMAudioDeviceDisplayText->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,MMTightText);
    // Configure the list toggle
    UI::CheckBox* MMAudioDeviceToggle = MMAudioDeviceList->GetListToggle();
    UI::SingleImageLayer* MMAudioDeviceToggleNormal = MMAudioDeviceToggle->CreateSingleImageLayer("MMListScrollDown");
    UI::SingleImageLayer* MMAudioDeviceToggleHovered = MMAudioDeviceToggle->CreateSingleImageLayer("MMHoveredListScrollDown");
    MMAudioDeviceToggle->AddLayerToGroup(MMAudioDeviceToggleNormal,UI::GroupOrderEntry(UI::Widget::WG_Normal,0));
    MMAudioDeviceToggle->AddLayerToGroup(MMAudioDeviceToggleHovered,UI::GroupOrderEntry(UI::Widget::WG_Hovered,0));
    MMAudioDeviceToggle->AddLayerToGroup(MMAudioDeviceToggleNormal,0,UI::CheckBox::WG_SelectedNormal);
    MMAudioDeviceToggle->AddLayerToGroup(MMAudioDeviceToggleHovered,0,UI::CheckBox::WG_SelectedHovered);
    // Configure the option list
    UI::ListBox* MMAudioDeviceOptions = MMAudioDeviceList->GetSelectionList();
    MMAudioDeviceOptions->SetListItemFont(MainMenuScreenText);
    MMAudioDeviceOptions->SetListItemTextScale(UI::TextLayer::SM_ParentRelative,MMLargeText);
    // Configure the background for the listbox container
    UI::VerticalContainer* MMAudioDeviceOptionsList = MMAudioDeviceOptions->GetListContainer();
    MMAudioDeviceOptionsList->CreateSingleImageLayer("MMListBackground",0,0);
    MMAudioDeviceOptionsList->SetChildSizing(UI::SizingInfo(UI::UnifiedVec2(1.0,0.19)),UI::LinearContainer::SE_OnAdd);
    // Configure the scrollbar for the option list
    UI::VerticalScrollbar* MMAudioDeviceOptionsScroll = MMAudioDeviceOptions->GetListScroll();
    MMAudioDeviceOptionsScroll->SetIncrementDistance(0.05);
    // Configure the scroller
    MMAudioDeviceOptionsScroll->GetScroller()->CreateSingleImageLayer("MMListScroller",UI::GroupOrderEntry(UI::Widget::WG_Normal,0));
    MMAudioDeviceOptionsScroll->GetScroller()->CreateSingleImageLayer("MMHoveredListScroller",UI::GroupOrderEntry(UI::Widget::WG_Hovered,0));
    // Configure the up button
    MMAudioDeviceOptionsScroll->GetUpLeftButton()->CreateSingleImageLayer("MMListScrollUp",UI::GroupOrderEntry(UI::Widget::WG_Normal,0));
    MMAudioDeviceOptionsScroll->GetUpLeftButton()->CreateSingleImageLayer("MMHoveredListScrollUp",UI::GroupOrderEntry(UI::Widget::WG_Hovered,0));
    // Configure the down button
    MMAudioDeviceOptionsScroll->GetDownRightButton()->CreateSingleImageLayer("MMListScrollDown",UI::GroupOrderEntry(UI::Widget::WG_Normal,0));
    MMAudioDeviceOptionsScroll->GetDownRightButton()->CreateSingleImageLayer("MMHoveredListScrollDown",UI::GroupOrderEntry(UI::Widget::WG_Hovered,0));
    // Configure the scroll back
    MMAudioDeviceOptionsScroll->GetScrollBack()->CreateSingleImageLayer("MMListScrollBackground",0,0);
    // Wrap up listing configuration
    MMAudioDeviceList->Subscribe(UI::Widget::EventVisibilityShown,this->AudioSettingsWork->GetSettingsSubscriber());
    MMAudioSet->AddChild(MMAudioDeviceList,10);// */

    // Create the checkbox for enabling or disabling FPS stats display
    UI::CheckBox* MMMuteBox = MainMenuScreen->CreateCheckBox("MS_MuteBox",UI::UnifiedRect(0.655,0.695,0,0.12));
    MMMuteBox->SetHorizontalSizingRules(UI::SR_Match_Other_Axis);
    MMMuteBox->CreateSingleImageLayer("MMCheckboxUnchecked",UI::GroupOrderEntry(UI::Widget::WG_Normal,0));
    MMMuteBox->CreateSingleImageLayer("MMHoveredCheckboxUnchecked",UI::GroupOrderEntry(UI::Widget::WG_Hovered,0));
    MMMuteBox->CreateSingleImageLayer("MMCheckboxChecked",UI::GroupOrderEntry(UI::CheckBox::WG_SelectedNormal,0));
    MMMuteBox->CreateSingleImageLayer("MMHoveredCheckboxChecked",UI::GroupOrderEntry(UI::CheckBox::WG_SelectedHovered,0));
    MMMuteBox->Subscribe(UI::CheckBox::EventSelected,this->AudioSettingsWork->GetSettingsSubscriber());
    MMMuteBox->Subscribe(UI::CheckBox::EventDeselected,this->AudioSettingsWork->GetSettingsSubscriber());
    MMMuteBox->Subscribe(UI::Widget::EventVisibilityShown,this->AudioSettingsWork->GetSettingsSubscriber());
    MMAudioSet->AddChild(MMMuteBox,5);

    // Create the label for the FPS stats display checkbox
    UI::Widget* MMMuteLabel = MainMenuScreen->CreateWidget("MS_MuteLabel",UI::UnifiedRect(0.72,0.695,0.22,0.12));
    MMMuteLabel->CreateSingleImageLayer("MMButton",0,0);
    UI::SingleLineTextLayer* MMMuteLabelText = MMMuteLabel->CreateSingleLineTextLayer(MainMenuScreenText,1,1);
    MMMuteLabelText->SetText("Mute");
    MMMuteLabelText->HorizontallyAlign(UI::LA_Center);
    MMMuteLabelText->VerticallyAlign(UI::LA_Center);
    MMMuteLabelText->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,MMTightText);
    MMAudioSet->AddChild(MMMuteLabel,6);

    // Create the back button for the options window
    UI::StackButton* MMOptsBack = MainMenuScreen->CreateStackButton("MS_OptsBack",UI::UnifiedRect(0.780,0.870,0.156,0.094));
    MMOptsBack->CreateSingleImageLayer("MMOptionsApplyButton",UI::GroupOrderEntry(UI::Widget::WG_Normal,0));
    MMOptsBack->CreateSingleImageLayer("MMOptionsApplyHoveredButton",UI::GroupOrderEntry(UI::Widget::WG_Hovered,0));
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
    MMCreditsAccess->CreateSingleImageLayer("MMButton",UI::GroupOrderEntry(UI::Widget::WG_Normal,0));
    MMCreditsAccess->CreateSingleImageLayer("MMHoveredButton",UI::GroupOrderEntry(UI::Widget::WG_Hovered,0));
    UI::SingleLineTextLayer* MMCreditsAccessText = MMCreditsAccess->CreateSingleLineTextLayer(MainMenuScreenText,1,1);
    MMCreditsAccessText->SetText("Credits");
    MMCreditsAccessText->HorizontallyAlign(UI::LA_Center);
    MMCreditsAccessText->VerticallyAlign(UI::LA_Center);
    MMCreditsAccessText->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,MMNormText);
    MMRootEntry->AddChild(MMCreditsAccess,3);

    // Create and configure the "window" that houses the options display
    UI::MenuEntry* MMCreditsWin = MainMenuScreen->CreateMenuEntry("MS_CreditsWin",UI::UnifiedRect(0.18,-9.04,0.64,6.4));
    MMCreditsWin->CreateSingleImageLayer("MMOptionsBackground",0,0);
    MMCreditsWin->SetPushButton(MMCreditsAccess);
    MMRootEntry->AddChild(MMCreditsWin,7);

    ////------------------  Exit Game  ------------------////
    // Start with the accessor button
    UI::StackButton* MMAppExitAccess = MainMenuScreen->CreateStackButton("MS_AppExit",UI::UnifiedRect(0.74,0.18,0.22,0.7));
    MMAppExitAccess->CreateSingleImageLayer("MMButton",UI::GroupOrderEntry(UI::Widget::WG_Normal,0));
    MMAppExitAccess->CreateSingleImageLayer("MMHoveredButton",UI::GroupOrderEntry(UI::Widget::WG_Hovered,0));
    UI::SingleLineTextLayer* MMAppExitAccessText = MMAppExitAccess->CreateSingleLineTextLayer(MainMenuScreenText,1,1);
    MMAppExitAccessText->SetText("Exit Game");
    MMAppExitAccessText->HorizontallyAlign(UI::LA_Center);
    MMAppExitAccessText->VerticallyAlign(UI::LA_Center);
    MMAppExitAccessText->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,MMNormText);
    MMRootEntry->AddChild(MMAppExitAccess,4);

    // Create and configure the "window" that houses the exit confirmation
    UI::MenuEntry* MMAppExitWin = MainMenuScreen->CreateMenuEntry("MS_AppExitWin",UI::UnifiedRect(0.25,-8.14,0.5,3.5));
    MMAppExitWin->CreateSingleImageLayer("MMAppExitBackground",0,0);
    MMAppExitWin->SetPushButton(MMAppExitAccess);
    MMRootEntry->AddChild(MMAppExitWin,8);

    // Create and configure the display for the confirmation question
    UI::Widget* MMAppExitWarn = MainMenuScreen->CreateWidget("MS_AppExitWarn",UI::UnifiedRect(0.14,0.18,0.72,0.22));
    MMAppExitWarn->CreateSingleImageLayer("MMAppExitText",0,0);
    UI::SingleLineTextLayer* MMAppExitWarnText = MMAppExitWarn->CreateSingleLineTextLayer(MainMenuScreenText,1,1);
    MMAppExitWarnText->SetText("Are you sure you want to exit?");
    MMAppExitWarnText->HorizontallyAlign(UI::LA_Center);
    MMAppExitWarnText->VerticallyAlign(UI::LA_Center);
    MMAppExitWarnText->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,MMSmallText);
    MMAppExitWin->AddChild(MMAppExitWarn,1);

    // Create and configure the confirm button
    UI::Button* MMAppExitConf = MainMenuScreen->CreateButton("MS_AppExitConf",UI::UnifiedRect(0.10,0.58,0.35,0.22));
    MMAppExitConf->CreateSingleImageLayer("MMAppExitButton",UI::GroupOrderEntry(UI::Widget::WG_Normal,0));
    MMAppExitConf->CreateSingleImageLayer("MMAppExitHoveredButton",UI::GroupOrderEntry(UI::Widget::WG_Hovered,0));
    UI::SingleLineTextLayer* MMAppExitConfText = MMAppExitConf->CreateSingleLineTextLayer(MainMenuScreenText,1,1);
    MMAppExitConfText->SetText("Yes");
    MMAppExitConfText->HorizontallyAlign(UI::LA_Center);
    MMAppExitConfText->VerticallyAlign(UI::LA_Center);
    MMAppExitConfText->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,MMNormText);
    MMAppExitConf->Subscribe(UI::Button::EventDeactivated,&AllAppExit);
    MMAppExitWin->AddChild(MMAppExitConf,2);

    // Create and configure the deny button
    UI::StackButton* MMAppExitDeny = MainMenuScreen->CreateStackButton("MS_AppExitDeny",UI::UnifiedRect(0.55,0.58,0.35,0.22));
    MMAppExitDeny->CreateSingleImageLayer("MMAppExitButton",UI::GroupOrderEntry(UI::Widget::WG_Normal,0));
    MMAppExitDeny->CreateSingleImageLayer("MMAppExitHoveredButton",UI::GroupOrderEntry(UI::Widget::WG_Hovered,0));
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
    const Real GSTightText = 0.8;
    const Real GSLargeText = 0.85;
    const Real GSMultiLineText = 0.20;

    const String GameScreenText = "Ubuntu-18";

    GUI->LoadMTA("Catch_Game.mta","Common");
    UI::Screen* GameScreen = GUI->CreateScreen("GameScreen","Catch_Game",UIViewport,2);

    ////----------------  Heads Up Display  ----------------////
    // Create the widget container for the score display
    UI::Widget* GSScore = GameScreen->CreateWidget("GS_Score",UI::UnifiedRect(0.008,0.006,0.27,0.065));
    GSScore->SetAspectRatioLock(UI::ARL_Ratio_Y_Axis);
    // Create the widget for displaying the timer logo
    UI::Widget* GSScoreLabel = GameScreen->CreateWidget("GS_ScoreLabel",UI::UnifiedRect(0.0,0.0,0.45,1.0));
    GSScoreLabel->SetHorizontalPositioningRules(UI::PF_Anchor_Left);
    GSScoreLabel->SetVerticalPositioningRules(UI::PF_Anchor_Top);
    GSScoreLabel->CreateSingleImageLayer("GSScoreTextArea",0,0);
    UI::SingleLineTextLayer* GSScoreLabelText = GSScoreLabel->CreateSingleLineTextLayer(GameScreenText,1,1);
    GSScoreLabelText->SetText("Score");
    GSScoreLabelText->HorizontallyAlign(UI::LA_Center);
    GSScoreLabelText->VerticallyAlign(UI::LA_Center);
    GSScoreLabelText->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,GSNormText);
    GSScore->AddChild(GSScoreLabel,1);
    // Create the widget for displaying the timer value
    UI::Widget* GSScoreValue = GameScreen->CreateWidget("GS_ScoreValue",UI::UnifiedRect(0.0,0.0,0.55,1.0));
    GSScoreValue->SetHorizontalPositioningRules(UI::PF_Anchor_Right);
    GSScoreValue->SetVerticalPositioningRules(UI::PF_Anchor_Top);
    GSScoreValue->CreateSingleImageLayer("GSScoreValueArea",0,0);
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
    UI::Widget* GSTimerLogo = GameScreen->CreateWidget("GS_TimerLogo",UI::UnifiedRect(0.0,0.0,0.34,1.0));
    GSTimerLogo->SetHorizontalPositioningRules(UI::PF_Anchor_Left);
    GSTimerLogo->SetVerticalPositioningRules(UI::PF_Anchor_Top);
    GSTimerLogo->CreateSingleImageLayer("GSTimerLogo",0,0);
    GSTimer->AddChild(GSTimerLogo,1);
    // Create the widget for displaying the timer value
    UI::Widget* GSTimerValue = GameScreen->CreateWidget("GS_TimerValue",UI::UnifiedRect(0.0,0.0,0.66,1.0));
    GSTimerValue->SetHorizontalPositioningRules(UI::PF_Anchor_Right);
    GSTimerValue->SetVerticalPositioningRules(UI::PF_Anchor_Top);
    GSTimerValue->CreateSingleImageLayer("GSTimerArea",0,0);
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
    GSMenuAccess->CreateSingleImageLayer("GSMenuStoreButton",UI::GroupOrderEntry(UI::Widget::WG_Normal,0));
    GSMenuAccess->CreateSingleImageLayer("GSMenuStoreHoveredButton",UI::GroupOrderEntry(UI::Widget::WG_Hovered,0));
    UI::SingleLineTextLayer* GSMenuAccessText = GSMenuAccess->CreateSingleLineTextLayer(GameScreenText,1,1);
    GSMenuAccessText->SetText("Menu");
    GSMenuAccessText->HorizontallyAlign(UI::LA_Center);
    GSMenuAccessText->VerticallyAlign(UI::LA_Center);
    GSMenuAccessText->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,GSNormText);
    GameScreen->AddChild(GSMenuAccess,3);

    // Create the item shop button
    UI::StackButton* GSItemShopAccess = GameScreen->CreateStackButton("GS_ItemShopAccess",UI::UnifiedRect(0.822,0.935,0.16,0.065));
    GSItemShopAccess->SetAspectRatioLock(UI::ARL_Ratio_Y_Axis);
    GSItemShopAccess->CreateSingleImageLayer("GSMenuStoreButton",UI::GroupOrderEntry(UI::Widget::WG_Normal,0));
    GSItemShopAccess->CreateSingleImageLayer("GSMenuStoreHoveredButton",UI::GroupOrderEntry(UI::Widget::WG_Hovered,0));
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
    GSMenuRoot->CreateSingleImageLayer("GSOptionsMenuBackground",0,0);
    GSMenuRoot->SetPushButton(GSMenuAccess);
    GSMenuRoot->Subscribe(UI::Widget::EventVisibilityShown,this->PauseWork->GetPauseSubscriber());
    GSMenuRoot->Subscribe(UI::Widget::EventVisibilityHidden,this->PauseWork->GetPauseSubscriber());
    GameScreen->AddChild(GSMenuRoot,10);

    // Create the options accessor button
    UI::StackButton* GSOptionsAccess = GameScreen->CreateStackButton("GS_Options",UI::UnifiedRect(0.15,0.075,0.70,0.11));
    GSOptionsAccess->CreateSingleImageLayer("GSOptionsButton",UI::GroupOrderEntry(UI::Widget::WG_Normal,0));
    GSOptionsAccess->CreateSingleImageLayer("GSOptionsHoveredButton",UI::GroupOrderEntry(UI::Widget::WG_Hovered,0));
    UI::SingleLineTextLayer* GSOptionsAccessText = GSOptionsAccess->CreateSingleLineTextLayer(GameScreenText,1,1);
    GSOptionsAccessText->SetText("Options");
    GSOptionsAccessText->HorizontallyAlign(UI::LA_Center);
    GSOptionsAccessText->VerticallyAlign(UI::LA_Center);
    GSOptionsAccessText->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,GSNormText);
    GSMenuRoot->AddChild(GSOptionsAccess,1);

    // Create and configure the "window" that houses the options display
    UI::MenuEntry* GSOptionsWin = GameScreen->CreateMenuEntry("GS_OptionsWin",UI::UnifiedRect(0.0,0.0,1.958,1.237));
    GSOptionsWin->SetHorizontalPositioningRules(UI::PF_Anchor_HorizontalCenter);
    GSOptionsWin->SetVerticalPositioningRules(UI::PF_Anchor_VerticalCenter);
    GSOptionsWin->CreateSingleImageLayer("GSOptionsBackground",0,0);
    GSOptionsWin->SetPushButton(GSOptionsAccess);
    GSMenuRoot->AddChild(GSOptionsWin,7);

    // Create the first of the two buttons that will display the two sets of options (video options)
    UI::StackButton* GSVideoSetAccess = GameScreen->CreateStackButton("GS_VideoSetAccess",UI::UnifiedRect(0.11,0.0365,0.34,0.11));
    GSVideoSetAccess->CreateSingleImageLayer("GSButton",UI::GroupOrderEntry(UI::Widget::WG_Normal,0));
    GSVideoSetAccess->CreateSingleImageLayer("GSHoveredButton",UI::GroupOrderEntry(UI::Widget::WG_Hovered,0));
    UI::SingleLineTextLayer* GSVideoSetAccessText = GSVideoSetAccess->CreateSingleLineTextLayer(GameScreenText,1,1);
    GSVideoSetAccessText->SetText("Video Options");
    GSVideoSetAccessText->HorizontallyAlign(UI::LA_Center);
    GSVideoSetAccessText->VerticallyAlign(UI::LA_Center);
    GSVideoSetAccessText->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,GSNormText);
    GSOptionsWin->AddChild(GSVideoSetAccess,1);

    // Create the second of the two buttons that will display the two sets of options (audio options)
    UI::StackButton* GSAudioSetAccess = GameScreen->CreateStackButton("GS_AudioSetAccess",UI::UnifiedRect(0.55,0.0365,0.34,0.11));
    GSAudioSetAccess->CreateSingleImageLayer("GSButton",UI::GroupOrderEntry(UI::Widget::WG_Normal,0));
    GSAudioSetAccess->CreateSingleImageLayer("GSHoveredButton",UI::GroupOrderEntry(UI::Widget::WG_Hovered,0));
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
    GSResolutionLabel->CreateSingleImageLayer("GSButton",0,0);
    UI::SingleLineTextLayer* GSResolutionLabelText = GSResolutionLabel->CreateSingleLineTextLayer(GameScreenText,1,1);
    GSResolutionLabelText->SetText("Video Resolutions");
    GSResolutionLabelText->HorizontallyAlign(UI::LA_Center);
    GSResolutionLabelText->VerticallyAlign(UI::LA_Center);
    GSResolutionLabelText->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,GSTightText);
    GSVideoSet->AddChild(GSResolutionLabel,1);

    // Create the listing of detected supported resolutions
    UI::DropDownList* GSResolutionList = GameScreen->CreateDropDownList("GS_ResolutionList",UI::UnifiedRect(0.065,0.145,0.515,0.12),UI::SB_Separate);
    // Configure the selection display
    UI::Widget* GSResolutionDisplay = GSResolutionList->GetSelectionDisplay();
    GSResolutionDisplay->CreateSingleImageLayer("GSListSelection",0,0);
    UI::SingleLineTextLayer* GSResolutionDisplayText = static_cast<UI::SingleLineTextLayer*>( GSResolutionDisplay->GetRenderLayer(0,UI::RLT_SingleLineText) );
    GSResolutionDisplayText->SetDefaultFont(GameScreenText);
    GSResolutionDisplayText->HorizontallyAlign(UI::LA_Center);
    GSResolutionDisplayText->VerticallyAlign(UI::LA_Center);
    GSResolutionDisplayText->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,GSLargeText);
    // Configure the list toggle
    UI::CheckBox* GSResolutionToggle = GSResolutionList->GetListToggle();
    UI::SingleImageLayer* GSResolutionToggleNormal = GSResolutionToggle->CreateSingleImageLayer("GSListScrollDown");
    UI::SingleImageLayer* GSResolutionToggleHovered = GSResolutionToggle->CreateSingleImageLayer("GSHoveredListScrollDown");
    GSResolutionToggle->AddLayerToGroup(GSResolutionToggleNormal,UI::GroupOrderEntry(UI::Widget::WG_Normal,0));
    GSResolutionToggle->AddLayerToGroup(GSResolutionToggleHovered,UI::GroupOrderEntry(UI::Widget::WG_Hovered,0));
    GSResolutionToggle->AddLayerToGroup(GSResolutionToggleNormal,0,UI::CheckBox::WG_SelectedNormal);
    GSResolutionToggle->AddLayerToGroup(GSResolutionToggleHovered,0,UI::CheckBox::WG_SelectedHovered);
    // Configure the option list
    UI::ListBox* GSResolutionOptions = GSResolutionList->GetSelectionList();
    GSResolutionOptions->SetListItemFont(GameScreenText);
    GSResolutionOptions->SetListItemTextScale(UI::TextLayer::SM_ParentRelative,GSLargeText);
    // Configure the background for the listbox container
    UI::VerticalContainer* GSResolutionOptionsList = GSResolutionOptions->GetListContainer();
    GSResolutionOptionsList->SetChildSizing(UI::SizingInfo(UI::UnifiedVec2(1.0,0.19)),UI::LinearContainer::SE_OnAdd);
    GSResolutionOptionsList->CreateSingleImageLayer("GSListBackground",0,0);
    // Configure the scrollbar for the option list
    UI::VerticalScrollbar* GSResolutionOptionsScroll = GSResolutionOptions->GetListScroll();
    GSResolutionOptionsScroll->SetIncrementDistance(0.05);
    // Configure the scroller
    GSResolutionOptionsScroll->GetScroller()->CreateSingleImageLayer("GSListScroller",UI::GroupOrderEntry(UI::Widget::WG_Normal,0));
    GSResolutionOptionsScroll->GetScroller()->CreateSingleImageLayer("GSHoveredListScroller",UI::GroupOrderEntry(UI::Widget::WG_Hovered,0));
    // Configure the up button
    GSResolutionOptionsScroll->GetUpLeftButton()->CreateSingleImageLayer("GSListScrollUp",UI::GroupOrderEntry(UI::Widget::WG_Normal,0));
    GSResolutionOptionsScroll->GetUpLeftButton()->CreateSingleImageLayer("GSHoveredListScrollUp",UI::GroupOrderEntry(UI::Widget::WG_Hovered,0));
    // Configure the down button
    GSResolutionOptionsScroll->GetDownRightButton()->CreateSingleImageLayer("GSListScrollDown",UI::GroupOrderEntry(UI::Widget::WG_Normal,0));
    GSResolutionOptionsScroll->GetDownRightButton()->CreateSingleImageLayer("GSHoveredListScrollDown",UI::GroupOrderEntry(UI::Widget::WG_Hovered,0));
    // Configure the scroll back
    GSResolutionOptionsScroll->GetScrollBack()->CreateSingleImageLayer("GSListScrollBackground",0,0);
    // Wrap up listing configuration
    GSResolutionList->Subscribe(UI::Widget::EventVisibilityShown,this->VideoSettingsWork->GetSettingsSubscriber());
    GSVideoSet->AddChild(GSResolutionList,11);

    // Create the checkbox for enabling or disabling fullscreen
    UI::CheckBox* GSFullscreenBox = GameScreen->CreateCheckBox("GS_FullscreenBox",UI::UnifiedRect(0.655,0.145,0,0.12));
    GSFullscreenBox->SetHorizontalSizingRules(UI::SR_Match_Other_Axis);
    GSFullscreenBox->CreateSingleImageLayer("GSCheckboxUnchecked",UI::GroupOrderEntry(UI::Widget::WG_Normal,0));
    GSFullscreenBox->CreateSingleImageLayer("GSHoveredCheckboxUnchecked",UI::GroupOrderEntry(UI::Widget::WG_Hovered,0));
    GSFullscreenBox->CreateSingleImageLayer("GSCheckboxChecked",UI::GroupOrderEntry(UI::CheckBox::WG_SelectedNormal,0));
    GSFullscreenBox->CreateSingleImageLayer("GSHoveredCheckboxChecked",UI::GroupOrderEntry(UI::CheckBox::WG_SelectedHovered,0));
    GSFullscreenBox->Subscribe(UI::Widget::EventVisibilityShown,this->VideoSettingsWork->GetSettingsSubscriber());
    GSVideoSet->AddChild(GSFullscreenBox,3);

    // Create the label for the fullscreen checkbox
    UI::Widget* GSFullscreenLabel = GameScreen->CreateWidget("GS_FullscreenLabel",UI::UnifiedRect(0.72,0.145,0.22,0.12));
    GSFullscreenLabel->CreateSingleImageLayer("GSButton",0,0);
    UI::SingleLineTextLayer* GSFullscreenLabelText = GSFullscreenLabel->CreateSingleLineTextLayer(GameScreenText,1,1);
    GSFullscreenLabelText->SetText("Fullscreen");
    GSFullscreenLabelText->HorizontallyAlign(UI::LA_Center);
    GSFullscreenLabelText->VerticallyAlign(UI::LA_Center);
    GSFullscreenLabelText->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,GSTightText);
    GSVideoSet->AddChild(GSFullscreenLabel,4);

    // Create the label for the FSAA options
    UI::Widget* GSFSAALabel = GameScreen->CreateWidget("GS_FSAALabel",UI::UnifiedRect(0.12,0.365,0.415,0.13));
    GSFSAALabel->CreateSingleImageLayer("GSButton",0,0);
    UI::SingleLineTextLayer* GSFSAALabelText = GSFSAALabel->CreateSingleLineTextLayer(GameScreenText,1,1);
    GSFSAALabelText->SetText("Anti-Aliasing");
    GSFSAALabelText->HorizontallyAlign(UI::LA_Center);
    GSFSAALabelText->VerticallyAlign(UI::LA_Center);
    GSFSAALabelText->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,GSTightText);
    GSVideoSet->AddChild(GSFSAALabel,5);

    // Create the listing of detected anti-aliasing options
    UI::DropDownList* GSFSAAList = GameScreen->CreateDropDownList("GS_FSAAList",UI::UnifiedRect(0.065,0.50,0.515,0.12),UI::SB_Separate);
    // Configure the selection display
    UI::Widget* GSFSAADisplay = GSFSAAList->GetSelectionDisplay();
    GSFSAADisplay->CreateSingleImageLayer("GSListSelection",0,0);
    UI::SingleLineTextLayer* GSFSAADisplayText = static_cast<UI::SingleLineTextLayer*>( GSFSAADisplay->GetRenderLayer(0,UI::RLT_SingleLineText) );
    GSFSAADisplayText->SetDefaultFont(GameScreenText);
    GSFSAADisplayText->HorizontallyAlign(UI::LA_Center);
    GSFSAADisplayText->VerticallyAlign(UI::LA_Center);
    GSFSAADisplayText->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,GSLargeText);
    // Configure the list toggle
    UI::CheckBox* GSFSAAToggle = GSFSAAList->GetListToggle();
    UI::SingleImageLayer* GSFSAAToggleNormal = GSFSAAToggle->CreateSingleImageLayer("GSListScrollDown");
    UI::SingleImageLayer* GSFSAAToggleHovered = GSFSAAToggle->CreateSingleImageLayer("GSHoveredListScrollDown");
    GSFSAAToggle->AddLayerToGroup(GSFSAAToggleNormal,UI::GroupOrderEntry(UI::Widget::WG_Normal,0));
    GSFSAAToggle->AddLayerToGroup(GSFSAAToggleHovered,UI::GroupOrderEntry(UI::Widget::WG_Hovered,0));
    GSFSAAToggle->AddLayerToGroup(GSFSAAToggleNormal,0,UI::CheckBox::WG_SelectedNormal);
    GSFSAAToggle->AddLayerToGroup(GSFSAAToggleHovered,0,UI::CheckBox::WG_SelectedHovered);
    // Configure the option list
    UI::ListBox* GSFSAAOptions = GSFSAAList->GetSelectionList();
    GSFSAAOptions->SetListItemFont(GameScreenText);
    GSFSAAOptions->SetListItemTextScale(UI::TextLayer::SM_ParentRelative,GSLargeText);
    // Configure the background for the listbox container
    UI::VerticalContainer* GSFSAAOptionsList = GSFSAAOptions->GetListContainer();
    GSFSAAOptionsList->SetChildSizing(UI::SizingInfo(UI::UnifiedVec2(1.0,0.19)),UI::LinearContainer::SE_OnAdd);
    GSFSAAOptionsList->CreateSingleImageLayer("GSListBackground",0,0);
    // Configure the scrollbar for the option list
    UI::VerticalScrollbar* GSFSAAOptionsScroll = GSFSAAOptions->GetListScroll();
    GSFSAAOptionsScroll->SetIncrementDistance(0.05);
    // Configure the scroller
    GSFSAAOptionsScroll->GetScroller()->CreateSingleImageLayer("GSListScroller",UI::GroupOrderEntry(UI::Widget::WG_Normal,0));
    GSFSAAOptionsScroll->GetScroller()->CreateSingleImageLayer("GSHoveredListScroller",UI::GroupOrderEntry(UI::Widget::WG_Hovered,0));
    // Configure the up button
    GSFSAAOptionsScroll->GetUpLeftButton()->CreateSingleImageLayer("GSListScrollUp",UI::GroupOrderEntry(UI::Widget::WG_Normal,0));
    GSFSAAOptionsScroll->GetUpLeftButton()->CreateSingleImageLayer("GSHoveredListScrollUp",UI::GroupOrderEntry(UI::Widget::WG_Hovered,0));
    // Configure the down button
    GSFSAAOptionsScroll->GetDownRightButton()->CreateSingleImageLayer("GSListScrollDown",UI::GroupOrderEntry(UI::Widget::WG_Normal,0));
    GSFSAAOptionsScroll->GetDownRightButton()->CreateSingleImageLayer("GSHoveredListScrollDown",UI::GroupOrderEntry(UI::Widget::WG_Hovered,0));
    // Configure the scroll back
    GSFSAAOptionsScroll->GetScrollBack()->CreateSingleImageLayer("GSListScrollBackground",0,0);
    // Wrap up listing configuration
    GSFSAAList->Subscribe(UI::Widget::EventVisibilityShown,this->VideoSettingsWork->GetSettingsSubscriber());
    GSVideoSet->AddChild(GSFSAAList,10);

    // Create the checkbox for enabling or disabling FPS stats display
    UI::CheckBox* GSStatsBox = GameScreen->CreateCheckBox("GS_StatsBox",UI::UnifiedRect(0.655,0.50,0,0.12));
    GSStatsBox->SetHorizontalSizingRules(UI::SR_Match_Other_Axis);
    GSStatsBox->CreateSingleImageLayer("GSCheckboxUnchecked",UI::GroupOrderEntry(UI::Widget::WG_Normal,0));
    GSStatsBox->CreateSingleImageLayer("GSHoveredCheckboxUnchecked",UI::GroupOrderEntry(UI::Widget::WG_Hovered,0));
    GSStatsBox->CreateSingleImageLayer("GSCheckboxChecked",UI::GroupOrderEntry(UI::CheckBox::WG_SelectedNormal,0));
    GSStatsBox->CreateSingleImageLayer("GSHoveredCheckboxChecked",UI::GroupOrderEntry(UI::CheckBox::WG_SelectedHovered,0));
    GSStatsBox->Subscribe(UI::Widget::EventVisibilityShown,this->VideoSettingsWork->GetSettingsSubscriber());
    GSVideoSet->AddChild(GSStatsBox,7);

    // Create the label for the FPS stats display checkbox
    UI::Widget* GSStatsLabel = GameScreen->CreateWidget("GS_StatsLabel",UI::UnifiedRect(0.72,0.50,0.22,0.12));
    GSStatsLabel->CreateSingleImageLayer("GSButton",0,0);
    UI::SingleLineTextLayer* GSStatsLabelText = GSStatsLabel->CreateSingleLineTextLayer(GameScreenText,1,1);
    GSStatsLabelText->SetText("Show FPS");
    GSStatsLabelText->HorizontallyAlign(UI::LA_Center);
    GSStatsLabelText->VerticallyAlign(UI::LA_Center);
    GSStatsLabelText->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,GSTightText);
    GSVideoSet->AddChild(GSStatsLabel,8);

    // Create the button that will apply all of the currently displayed video settings
    UI::Button* GSVideoOptsApply = GameScreen->CreateButton("GS_VideoOptsApply",UI::UnifiedRect(0.815,0.84,0.17,0.14));
    GSVideoOptsApply->CreateSingleImageLayer("GSOptionsApplyButton",UI::GroupOrderEntry(UI::Widget::WG_Normal,0));
    GSVideoOptsApply->CreateSingleImageLayer("GSOptionsApplyHoveredButton",UI::GroupOrderEntry(UI::Widget::WG_Hovered,0));
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
    GSMusicVolLabel->SetHorizontalPositioningRules(UI::PF_Anchor_HorizontalCenter);
    GSMusicVolLabel->CreateSingleImageLayer("GSButton",0,0);
    UI::SingleLineTextLayer* GSMusicVolLabelText = GSMusicVolLabel->CreateSingleLineTextLayer(GameScreenText,1,1);
    GSMusicVolLabelText->SetText("Music Volume");
    GSMusicVolLabelText->HorizontallyAlign(UI::LA_Center);
    GSMusicVolLabelText->VerticallyAlign(UI::LA_Center);
    GSMusicVolLabelText->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,GSTightText);
    GSAudioSet->AddChild(GSMusicVolLabel,1);

    // Create the Music volume slider
    UI::HorizontalScrollbar* GSMusicVol = GameScreen->CreateHorizontalScrollbar("GS_MusicVolume",UI::UnifiedRect(0.0,0.145,0.7,0.09),UI::SB_Separate);
    GSMusicVol->SetHorizontalPositioningRules(UI::PF_Anchor_HorizontalCenter);
    GSMusicVol->SetIncrementDistance(GSScrollerSize * 0.5);
    GSMusicVol->SetScrollerSize(GSScrollerSize);
    // Configure the scroller
    GSMusicVol->GetScroller()->CreateSingleImageLayer("GSScroller",UI::GroupOrderEntry(UI::Widget::WG_Normal,0));
    GSMusicVol->GetScroller()->CreateSingleImageLayer("GSHoveredScroller",UI::GroupOrderEntry(UI::Widget::WG_Hovered,0));
    // Configure the up button
    GSMusicVol->GetUpLeftButton()->CreateSingleImageLayer("GSScrollLeft",UI::GroupOrderEntry(UI::Widget::WG_Normal,0));
    GSMusicVol->GetUpLeftButton()->CreateSingleImageLayer("GSHoveredScrollLeft",UI::GroupOrderEntry(UI::Widget::WG_Hovered,0));
    // Configure the down button
    GSMusicVol->GetDownRightButton()->CreateSingleImageLayer("GSScrollRight",UI::GroupOrderEntry(UI::Widget::WG_Normal,0));
    GSMusicVol->GetDownRightButton()->CreateSingleImageLayer("GSHoveredScrollRight",UI::GroupOrderEntry(UI::Widget::WG_Hovered,0));
    // Configure the scroll back
    GSMusicVol->GetScrollBack()->CreateSingleImageLayer("GSScrollBackground",0,0);
    // Wrap up Effects volume ocnfiguration
    GSMusicVol->Subscribe(UI::Scrollbar::EventScrollValueChanged,this->AudioSettingsWork->GetSettingsSubscriber());
    GSMusicVol->Subscribe(UI::Widget::EventVisibilityShown,this->AudioSettingsWork->GetSettingsSubscriber());
    GSAudioSet->AddChild(GSMusicVol,2);

    // Create the label for the Effects volume
    UI::Widget* GSEffectsVolLabel = GameScreen->CreateWidget("GS_EffectsVolLabel",UI::UnifiedRect(0.0,0.275,0.40,0.13));
    GSEffectsVolLabel->SetHorizontalPositioningRules(UI::PF_Anchor_HorizontalCenter);
    GSEffectsVolLabel->CreateSingleImageLayer("GSButton",0,0);
    UI::SingleLineTextLayer* GSEffectsVolLabelText = GSEffectsVolLabel->CreateSingleLineTextLayer(GameScreenText,1,1);
    GSEffectsVolLabelText->SetText("Effects Volume");
    GSEffectsVolLabelText->HorizontallyAlign(UI::LA_Center);
    GSEffectsVolLabelText->VerticallyAlign(UI::LA_Center);
    GSEffectsVolLabelText->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,GSTightText);
    GSAudioSet->AddChild(GSEffectsVolLabel,3);

    // Create the Effects volume slider
    UI::HorizontalScrollbar* GSEffectsVol = GameScreen->CreateHorizontalScrollbar("GS_EffectsVolume",UI::UnifiedRect(0.0,0.41,0.7,0.09),UI::SB_Separate);
    GSEffectsVol->SetHorizontalPositioningRules(UI::PF_Anchor_HorizontalCenter);
    GSEffectsVol->SetIncrementDistance(GSScrollerSize * 0.5);
    GSEffectsVol->SetScrollerSize(GSScrollerSize);
    // Configure the scroller
    GSEffectsVol->GetScroller()->CreateSingleImageLayer("GSScroller",UI::GroupOrderEntry(UI::Widget::WG_Normal,0));
    GSEffectsVol->GetScroller()->CreateSingleImageLayer("GSHoveredScroller",UI::GroupOrderEntry(UI::Widget::WG_Hovered,0));
    // Configure the up button
    GSEffectsVol->GetUpLeftButton()->CreateSingleImageLayer("GSScrollLeft",UI::GroupOrderEntry(UI::Widget::WG_Normal,0));
    GSEffectsVol->GetUpLeftButton()->CreateSingleImageLayer("GSHoveredScrollLeft",UI::GroupOrderEntry(UI::Widget::WG_Hovered,0));
    // Configure the down button
    GSEffectsVol->GetDownRightButton()->CreateSingleImageLayer("GSScrollRight",UI::GroupOrderEntry(UI::Widget::WG_Normal,0));
    GSEffectsVol->GetDownRightButton()->CreateSingleImageLayer("GSHoveredScrollRight",UI::GroupOrderEntry(UI::Widget::WG_Hovered,0));
    // Configure the scroll back
    GSEffectsVol->GetScrollBack()->CreateSingleImageLayer("GSScrollBackground",0,0);
    // Wrap up Effects volume ocnfiguration
    GSEffectsVol->Subscribe(UI::Scrollbar::EventScrollValueChanged,this->AudioSettingsWork->GetSettingsSubscriber());
    GSEffectsVol->Subscribe(UI::Widget::EventVisibilityShown,this->AudioSettingsWork->GetSettingsSubscriber());
    GSAudioSet->AddChild(GSEffectsVol,2);

    /*// Create the label for the Audio Device options
    UI::Widget* GSAudioDeviceLabel = GameScreen->CreateWidget("GS_AudioDeviceLabel",UI::UnifiedRect(0.12,0.56,0.415,0.13));
    GSAudioDeviceLabel->CreateSingleImageLayer("GSButton",0,0);
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
    GSAudioDeviceDisplay->CreateSingleImageLayer("GSListSelection",0,0);
    UI::SingleLineTextLayer* GSAudioDeviceDisplayText = static_cast<UI::SingleLineTextLayer*>( GSAudioDeviceDisplay->GetRenderLayer(0,UI::RLT_SingleLineText) );
    GSAudioDeviceDisplayText->SetDefaultFont(GameScreenText);
    GSAudioDeviceDisplayText->HorizontallyAlign(UI::LA_Center);
    GSAudioDeviceDisplayText->VerticallyAlign(UI::LA_Center);
    GSAudioDeviceDisplayText->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,GSLargeText);
    // Configure the list toggle
    UI::CheckBox* GSAudioDeviceToggle = GSAudioDeviceList->GetListToggle();
    UI::SingleImageLayer* GSAudioDeviceToggleNormal = GSAudioDeviceToggle->CreateSingleImageLayer("GSListScrollDown");
    UI::SingleImageLayer* GSAudioDeviceToggleHovered = GSAudioDeviceToggle->CreateSingleImageLayer("GSHoveredListScrollDown");
    GSAudioDeviceToggle->AddLayerToGroup(GSAudioDeviceToggleNormal,UI::GroupOrderEntry(UI::Widget::WG_Normal,0));
    GSAudioDeviceToggle->AddLayerToGroup(GSAudioDeviceToggleHovered,UI::GroupOrderEntry(UI::Widget::WG_Hovered,0));
    GSAudioDeviceToggle->AddLayerToGroup(GSAudioDeviceToggleNormal,0,UI::CheckBox::WG_SelectedNormal);
    GSAudioDeviceToggle->AddLayerToGroup(GSAudioDeviceToggleHovered,0,UI::CheckBox::WG_SelectedHovered);
    // Configure the option list
    UI::ListBox* GSAudioDeviceOptions = GSAudioDeviceList->GetSelectionList();
    GSAudioDeviceOptions->SetListItemFont(GameScreenText);
    GSAudioDeviceOptions->SetListItemTextScale(UI::TextLayer::SM_ParentRelative,GSLargeText);
    // Configure the background for the listbox container
    UI::VerticalContainer* GSAudioDeviceOptionsList = GSAudioDeviceOptions->GetListContainer();
    GSAudioDeviceOptionsList->CreateSingleImageLayer("GSListBackground",0,0);
    GSAudioDeviceOptionsList->SetChildSizing(UI::SizingInfo(UI::UnifiedVec2(1.0,0.19)),UI::LinearContainer::SE_OnAdd);
    // Configure the scrollbar for the option list
    UI::VerticalScrollbar* GSAudioDeviceOptionsScroll = GSAudioDeviceOptions->GetListScroll();
    GSAudioDeviceOptionsScroll->SetIncrementDistance(0.05);
    // Configure the scroller
    GSAudioDeviceOptionsScroll->GetScroller()->CreateSingleImageLayer("GSListScroller",UI::GroupOrderEntry(UI::Widget::WG_Normal,0));
    GSAudioDeviceOptionsScroll->GetScroller()->CreateSingleImageLayer("GSHoveredListScroller",UI::GroupOrderEntry(UI::Widget::WG_Hovered,0));
    // Configure the up button
    GSAudioDeviceOptionsScroll->GetUpLeftButton()->CreateSingleImageLayer("GSListScrollUp",UI::GroupOrderEntry(UI::Widget::WG_Normal,0));
    GSAudioDeviceOptionsScroll->GetUpLeftButton()->CreateSingleImageLayer("GSHoveredListScrollUp",UI::GroupOrderEntry(UI::Widget::WG_Hovered,0));
    // Configure the down button
    GSAudioDeviceOptionsScroll->GetDownRightButton()->CreateSingleImageLayer("GSListScrollDown",UI::GroupOrderEntry(UI::Widget::WG_Normal,0));
    GSAudioDeviceOptionsScroll->GetDownRightButton()->CreateSingleImageLayer("GSHoveredListScrollDown",UI::GroupOrderEntry(UI::Widget::WG_Hovered,0));
    // Configure the scroll back
    GSAudioDeviceOptionsScroll->GetScrollBack()->CreateSingleImageLayer("GSListScrollBackground",0,0);
    // Wrap up listing configuration
    GSAudioDeviceList->Subscribe(UI::Widget::EventVisibilityShown,this->AudioSettingsWork->GetSettingsSubscriber());
    GSAudioSet->AddChild(GSAudioDeviceList,10);// */

    // Create the checkbox for enabling or disabling FPS stats display
    UI::CheckBox* GSMuteBox = GameScreen->CreateCheckBox("GS_MuteBox",UI::UnifiedRect(0.655,0.695,0,0.12));
    GSMuteBox->SetHorizontalSizingRules(UI::SR_Match_Other_Axis);
    GSMuteBox->CreateSingleImageLayer("GSCheckboxUnchecked",UI::GroupOrderEntry(UI::Widget::WG_Normal,0));
    GSMuteBox->CreateSingleImageLayer("GSHoveredCheckboxUnchecked",UI::GroupOrderEntry(UI::Widget::WG_Hovered,0));
    GSMuteBox->CreateSingleImageLayer("GSCheckboxChecked",UI::GroupOrderEntry(UI::CheckBox::WG_SelectedNormal,0));
    GSMuteBox->CreateSingleImageLayer("GSHoveredCheckboxChecked",UI::GroupOrderEntry(UI::CheckBox::WG_SelectedHovered,0));
    GSMuteBox->Subscribe(UI::CheckBox::EventSelected,this->AudioSettingsWork->GetSettingsSubscriber());
    GSMuteBox->Subscribe(UI::CheckBox::EventDeselected,this->AudioSettingsWork->GetSettingsSubscriber());
    GSMuteBox->Subscribe(UI::Widget::EventVisibilityShown,this->AudioSettingsWork->GetSettingsSubscriber());
    GSAudioSet->AddChild(GSMuteBox,5);

    // Create the label for the FPS stats display checkbox
    UI::Widget* GSMuteLabel = GameScreen->CreateWidget("GS_MuteLabel",UI::UnifiedRect(0.72,0.695,0.22,0.12));
    GSMuteLabel->CreateSingleImageLayer("GSButton",0,0);
    UI::SingleLineTextLayer* GSMuteLabelText = GSMuteLabel->CreateSingleLineTextLayer(GameScreenText,1,1);
    GSMuteLabelText->SetText("Mute");
    GSMuteLabelText->HorizontallyAlign(UI::LA_Center);
    GSMuteLabelText->VerticallyAlign(UI::LA_Center);
    GSMuteLabelText->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,GSTightText);
    GSAudioSet->AddChild(GSMuteLabel,6);

    // Create the back button for the options window
    UI::StackButton* GSOptsBack = GameScreen->CreateStackButton("GS_OptsBack",UI::UnifiedRect(0.780,0.870,0.156,0.094));
    GSOptsBack->CreateSingleImageLayer("GSOptionsApplyButton",UI::GroupOrderEntry(UI::Widget::WG_Normal,0));
    GSOptsBack->CreateSingleImageLayer("GSOptionsApplyHoveredButton",UI::GroupOrderEntry(UI::Widget::WG_Hovered,0));
    UI::SingleLineTextLayer* GSOptsBackText = GSOptsBack->CreateSingleLineTextLayer(GameScreenText,1,1);
    GSOptsBackText->SetText("Back");
    GSOptsBackText->HorizontallyAlign(UI::LA_Center);
    GSOptsBackText->VerticallyAlign(UI::LA_Center);
    GSOptsBackText->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,GSLargeText);
    GSOptionsWin->SetPopButton(GSOptsBack);
    GSOptionsWin->AddChild(GSOptsBack,5);

    // Second button Y placement: 26

    // Create the game restart button
    UI::Button* GSMenuRestart = GameScreen->CreateButton("GS_MenuRestart",UI::UnifiedRect(0.15,0.445,0.70,0.12));
    GSMenuRestart->CreateSingleImageLayer("GSOptionsButton",UI::GroupOrderEntry(UI::Widget::WG_Normal,0));
    GSMenuRestart->CreateSingleImageLayer("GSOptionsHoveredButton",UI::GroupOrderEntry(UI::Widget::WG_Hovered,0));
    UI::SingleLineTextLayer* GSMenuRestartText = GSMenuRestart->CreateSingleLineTextLayer(GameScreenText,1,1);
    GSMenuRestartText->SetText("Restart Level");
    GSMenuRestartText->HorizontallyAlign(UI::LA_Center);
    GSMenuRestartText->VerticallyAlign(UI::LA_Center);
    GSMenuRestartText->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,GSNormText);
    GSMenuRestart->Subscribe(UI::Button::EventDeactivated,&GSRestart);
    GSMenuRoot->AddChild(GSMenuRestart,3);

    // Create the game return button
    UI::StackButton* GSMenuReturn = GameScreen->CreateStackButton("GS_MenuReturn",UI::UnifiedRect(0.15,0.63,0.70,0.12));
    GSMenuReturn->CreateSingleImageLayer("GSOptionsButton",UI::GroupOrderEntry(UI::Widget::WG_Normal,0));
    GSMenuReturn->CreateSingleImageLayer("GSOptionsHoveredButton",UI::GroupOrderEntry(UI::Widget::WG_Hovered,0));
    UI::SingleLineTextLayer* GSMenuReturnText = GSMenuReturn->CreateSingleLineTextLayer(GameScreenText,1,1);
    GSMenuReturnText->SetText("Return to Game");
    GSMenuReturnText->HorizontallyAlign(UI::LA_Center);
    GSMenuReturnText->VerticallyAlign(UI::LA_Center);
    GSMenuReturnText->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,GSNormText);
    GSMenuRoot->SetPopButton(GSMenuReturn);
    GSMenuRoot->AddChild(GSMenuReturn,4);

    // Create the game exit button
    UI::Button* GSMenuExit = GameScreen->CreateButton("GS_MenuExit",UI::UnifiedRect(0.15,0.815,0.70,0.12));
    GSMenuExit->CreateSingleImageLayer("GSOptionsButton",UI::GroupOrderEntry(UI::Widget::WG_Normal,0));
    GSMenuExit->CreateSingleImageLayer("GSOptionsHoveredButton",UI::GroupOrderEntry(UI::Widget::WG_Hovered,0));
    UI::SingleLineTextLayer* GSMenuExitText = GSMenuExit->CreateSingleLineTextLayer(GameScreenText,1,1);
    GSMenuExitText->SetText("Exit to Main Menu");
    GSMenuExitText->HorizontallyAlign(UI::LA_Center);
    GSMenuExitText->VerticallyAlign(UI::LA_Center);
    GSMenuExitText->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,GSNormText);
    GSMenuExit->Subscribe(UI::Button::EventDeactivated,&GSMMReturn);
    GSMenuRoot->AddChild(GSMenuExit,5);

    ////-----------------  Item Shop Menu  -----------------////
    // Create the root entry for the item shop
    UI::MenuEntry* GSItemShopRoot = GameScreen->CreateMenuEntry("GS_ItemShopRoot",UI::UnifiedRect(0.25,0.11,0.5,0.78125));
    GSItemShopRoot->SetAspectRatioLock(UI::ARL_Ratio_Y_Axis);
    GSItemShopRoot->CreateSingleImageLayer("GSStoreBackground",0,0);
    GSItemShopRoot->SetPushButton(GSItemShopAccess);
    GSItemShopRoot->Subscribe(UI::Widget::EventVisibilityShown,this->PauseWork->GetPauseSubscriber());
    GSItemShopRoot->Subscribe(UI::Widget::EventVisibilityHidden,this->PauseWork->GetPauseSubscriber());
    GameScreen->AddChild(GSItemShopRoot,8);

    // Create the "titlebar" for the item shop window
    UI::Widget* GSItemShopTitle = GameScreen->CreateWidget("GS_ItemShopTitle",UI::UnifiedRect(0.0,0.018,1.0,0.077));
    GSItemShopTitle->SetHorizontalPositioningRules(UI::PF_Anchor_HorizontalCenter);
    UI::SingleLineTextLayer* GSItemShopTitleText = GSItemShopTitle->CreateSingleLineTextLayer(GameScreenText,1,1);
    GSItemShopTitleText->SetText("Item Shop");
    GSItemShopTitleText->HorizontallyAlign(UI::LA_Center);
    GSItemShopTitleText->VerticallyAlign(UI::LA_Center);
    GSItemShopTitleText->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,GSNormText);
    GSItemShopRoot->AddChild(GSItemShopTitle,1);

    // Create the spinner that will determine which items will be displayed
    UI::Spinner* GSItemShopSpinner = GameScreen->CreateSpinner("GS_ItemShopSpinner",UI::UnifiedRect(0.0,0.47,0.35,0.06),UI::Spn_Separate_Horizontal,GameScreenText);
    GSItemShopSpinner->SetHorizontalPositioningRules(UI::PF_Anchor_HorizontalCenter);
    GSItemShopSpinner->GetIncrement()->CreateSingleImageLayer("GSIncrementPage",0,0);
    GSItemShopSpinner->GetDecrement()->CreateSingleImageLayer("GSDecrementPage",0,0);
    GSItemShopSpinner->GetValueDisplay()->CreateSingleImageLayer("GSPageBox",0,0);
    GSItemShopSpinner->GetValueDisplay()->GetEditLayer()->SetTextLineHorizontalAlignment(UI::LA_Center);
    GSItemShopSpinner->GetValueDisplay()->GetEditLayer()->SetTextLineVerticalAlignment(UI::LA_Center);
    GSItemShopSpinner->GetValueDisplay()->GetEditLayer()->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,GSNormText);
    GSItemShopSpinner->SetSpinValue(1.0);
    GSItemShopRoot->AddChild(GSItemShopSpinner,2);

    // Create the grid that will store all of our purchasable items
    UI::GridContainer* GSItemShopGrid = GameScreen->CreateGridContainer("GS_ItemShopGrid",UI::UnifiedRect(0.15,0.08,0.7,0.33));
    GSItemShopGrid->SetCellSize(4,3);
    GSItemShopGrid->SetXYProvider(GSItemShopSpinner);
    GSItemShopRoot->AddChild(GSItemShopGrid,3);

    UI::Widget* GSItemShopDescription = GameScreen->CreateWidget("GS_ItemShopDescription",UI::UnifiedRect(0.15,0.54,0.7,0.33));
    GSItemShopDescription->CreateSingleImageLayer("GSBreakdownBackground",0,0);
    UI::MultiLineTextLayer* GSItemShopDescriptionText = GSItemShopDescription->CreateMultiLineTextLayer(GameScreenText,1,1);
    GSItemShopDescriptionText->SetScale(Vector2(0.88,0.88));
    GSItemShopDescriptionText->SetText("This is a more exhaustive\ntest of the multi-line\ntext layer.");
    GSItemShopDescriptionText->SetTextLineHorizontalAlignment(UI::LA_Center);
    GSItemShopDescriptionText->SetTextLineVerticalAlignment(UI::LA_Center);
    GSItemShopDescriptionText->SetAutoTextScale(UI::TextLayer::SM_LayerRelative,GSMultiLineText);
    GSItemShopRoot->AddChild(GSItemShopDescription,4);

    UI::Widget* GSItemShopMoney = GameScreen->CreateWidget("GS_ItemShopMoney",UI::UnifiedRect(0.15,0.90,0.20,0.07));
    GSItemShopMoney->CreateSingleImageLayer("GSStoreButton",0,0);
    UI::SingleLineTextLayer* GSItemShopMoneyText = GSItemShopMoney->CreateSingleLineTextLayer(GameScreenText,1,1);
    GSItemShopMoneyText->SetText("$0");
    GSItemShopMoneyText->HorizontallyAlign(UI::LA_Center);
    GSItemShopMoneyText->VerticallyAlign(UI::LA_Center);
    GSItemShopMoneyText->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,GSLargeText);
    GSItemShopRoot->AddChild(GSItemShopMoney,5);

    UI::Button* GSItemShopBuy = GameScreen->CreateButton("GS_ItemShopBuy",UI::UnifiedRect(0.40,0.90,0.20,0.07));
    GSItemShopBuy->CreateSingleImageLayer("GSStoreButton",UI::GroupOrderEntry(UI::Widget::WG_Normal,0));
    GSItemShopBuy->CreateSingleImageLayer("GSStoreHoveredButton",UI::GroupOrderEntry(UI::Widget::WG_Hovered,0));
    UI::SingleLineTextLayer* GSItemShopBuyText = GSItemShopBuy->CreateSingleLineTextLayer(GameScreenText,1,1);
    GSItemShopBuyText->SetText("Buy");
    GSItemShopBuyText->HorizontallyAlign(UI::LA_Center);
    GSItemShopBuyText->VerticallyAlign(UI::LA_Center);
    GSItemShopBuyText->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,GSLargeText);
    GSItemShopRoot->AddChild(GSItemShopBuy,6);

    UI::StackButton* GSItemShopBack = GameScreen->CreateStackButton("GS_ItemShopBack",UI::UnifiedRect(0.65,0.90,0.20,0.07));
    GSItemShopBack->CreateSingleImageLayer("GSStoreButton",UI::GroupOrderEntry(UI::Widget::WG_Normal,0));
    GSItemShopBack->CreateSingleImageLayer("GSStoreHoveredButton",UI::GroupOrderEntry(UI::Widget::WG_Hovered,0));
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
    GSLevelReport->CreateSingleImageLayer("GSOptionsBackground",0,0);
    GSLevelReport->Hide();
    GameScreen->AddChild(GSLevelReport,9);

    // Create the total score display
    UI::Widget* GSLevelReportScore = GameScreen->CreateWidget("GS_LevelReportScore",UI::UnifiedRect(0.0,0.08,0.34,0.125));
    GSLevelReportScore->SetHorizontalPositioningRules(UI::PF_Anchor_HorizontalCenter);
    GSLevelReportScore->CreateSingleImageLayer("GSAppExitButton",0,0);
    UI::SingleLineTextLayer* GSLevelReportScoreText = GSLevelReportScore->CreateSingleLineTextLayer(GameScreenText,1,1);
    GSLevelReportScoreText->SetText("0000");
    GSLevelReportScoreText->HorizontallyAlign(UI::LA_Center);
    GSLevelReportScoreText->VerticallyAlign(UI::LA_Center);
    GSLevelReportScoreText->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,GSLargeText);
    GSLevelReport->AddChild(GSLevelReportScore,1);

    UI::Widget* GSLevelReportBreakdownBack = GameScreen->CreateWidget("GS_LevelReportBreakdownBack",UI::UnifiedRect(0.15,0.245,0.70,0.52));
    GSLevelReportBreakdownBack->CreateSingleImageLayer("GSBreakdownBackground",0,0);
    GSLevelReport->AddChild(GSLevelReportBreakdownBack,2);

    UI::VerticalContainer* GSLevelReportBreakdown = GameScreen->CreateVerticalContainer("GS_LevelReportBreakdown",UI::UnifiedRect(0.10,0.10,0.80,0.80));
    GSLevelReportBreakdown->SetChildSizing(UI::SizingInfo(UI::UnifiedVec2(0.9,0.1666)),UI::LinearContainer::SE_OnAdd);
    GSLevelReportBreakdownBack->AddChild(GSLevelReportBreakdown,1);

    UI::Button* GSLevelReportFinish = GameScreen->CreateButton("GS_LevelReportFinish",UI::UnifiedRect(0.21,0.815,0.25,0.125));
    GSLevelReportFinish->CreateSingleImageLayer("GSStoreButton",UI::GroupOrderEntry(UI::Widget::WG_Normal,0));
    GSLevelReportFinish->CreateSingleImageLayer("GSStoreHoveredButton",UI::GroupOrderEntry(UI::Widget::WG_Hovered,0));
    UI::SingleLineTextLayer* GSLevelReportFinishText = GSLevelReportFinish->CreateSingleLineTextLayer(GameScreenText,1,1);
    GSLevelReportFinishText->SetText("Finish");
    GSLevelReportFinishText->HorizontallyAlign(UI::LA_Center);
    GSLevelReportFinishText->VerticallyAlign(UI::LA_Center);
    GSLevelReportFinishText->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,GSNormText);
    GSLevelReportFinish->Subscribe(UI::Button::EventDeactivated,&GSMMReturn);
    GSLevelReport->AddChild(GSLevelReportFinish,3);

    UI::Button* GSLevelReportRetry = GameScreen->CreateButton("GS_LevelReportRetry",UI::UnifiedRect(0.54,0.815,0.25,0.125));
    GSLevelReportRetry->CreateSingleImageLayer("GSStoreButton",UI::GroupOrderEntry(UI::Widget::WG_Normal,0));
    GSLevelReportRetry->CreateSingleImageLayer("GSStoreHoveredButton",UI::GroupOrderEntry(UI::Widget::WG_Hovered,0));
    UI::SingleLineTextLayer* GSLevelReportRetryText = GSLevelReportRetry->CreateSingleLineTextLayer(GameScreenText,1,1);
    GSLevelReportRetryText->SetText("Retry");
    GSLevelReportRetryText->HorizontallyAlign(UI::LA_Center);
    GSLevelReportRetryText->VerticallyAlign(UI::LA_Center);
    GSLevelReportRetryText->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,GSNormText);
    GSLevelReportRetry->Subscribe(UI::Button::EventDeactivated,&GSRestart);
    GSLevelReport->AddChild(GSLevelReportRetry,4);
    GSLevelReport->Hide();
}

void CatchApp::CreateLoadingScreen()
{
    UI::UIManager* GUI = UI::UIManager::GetSingletonPtr();
    Graphics::GraphicsManager* GraphicsMan = Graphics::GraphicsManager::GetSingletonPtr();

    GUI->LoadMTA("Catch_Loading.mta","Common");
    Graphics::Viewport* UIViewport = GraphicsMan->GetGameWindow(0)->GetViewport(0);
    UIViewport->SetCamera(static_cast<Graphics::SceneManager*>( this->TheWorld->GetManager(ManagerBase::MT_SceneManager) )->CreateCamera());

    UI::Screen* LoadScreen = GUI->CreateScreen("LoadingScreen","Catch_Loading",UIViewport,9);
    UI::Widget* BackgroundWidget = LoadScreen->CreateWidget("LoadBackground",UI::UnifiedRect(0,0,1.7777,1,0,0,0,0));
    BackgroundWidget->SetHorizontalPositioningRules(UI::PF_Anchor_HorizontalCenter);
    BackgroundWidget->SetVerticalPositioningRules(UI::PF_Anchor_VerticalCenter);
    BackgroundWidget->SetVerticalSizingRules(UI::SR_Unified_Dims);
    BackgroundWidget->SetHorizontalSizingRules(UI::SR_Match_Other_Axis_Unified);
    UI::SingleImageLayer* LoadBackground = BackgroundWidget->CreateSingleImageLayer();
    LoadBackground->SetSprite("BTSBanner");
    BackgroundWidget->AddLayerToGroup(LoadBackground,UI::GroupOrderEntry(UI::Widget::WG_Normal,0));
    BackgroundWidget->AddLayerToGroup(LoadBackground,UI::GroupOrderEntry(UI::Widget::WG_Hovered,0));
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
    if( !AudioSettingFile ) {
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
    if( !GraphicsSettingFile ) {
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
    // Verify the Profile Settings
    String ProfileSaveFileName("ProfileSettings.mxs");
    ObjectSettingFile* ProfileSettingFile = this->Profiles->GetSettingFile(ProfileSaveFileName);
    if( !ProfileSettingFile ) {
        ProfileSettingFile = this->Profiles->CreateSettingFile(ProfileSaveFileName);
        // Set the default path for profile loading
        this->Profiles->SetProfilesDirectory("$ShareableAppData$/.Catch/Profiles/");
        // Flag the file for saving
        ProfileSettingFile->SetNeedsSave(true);
        // Make sure the file saves the "Current" setting group.
        this->Profiles->SetCurrentSettingsSaveFile(ProfileSaveFileName);
    }
}

void CatchApp::RegisterTypes()
{
    AreaEffectManager::AddAreaEffectFactory( new ScoreAreaFactory() );
    AreaEffectManager::AddAreaEffectFactory( new StartAreaFactory() );
}

void CatchApp::ChangeState(const CatchApp::GameState StateToSet)
{
    if(this->CurrentState == StateToSet)
        return;

    this->SetVisibleScreens(StateToSet);
    if( StateToSet == CatchApp::Catch_MenuScreen ) {
        // This code block was created due to cameras being destroyed on every level unload.
        this->ThePlayer->InitWorldObjects(this->TheWorld);
    }
    if( StateToSet == CatchApp::Catch_ScoreScreen ) {
        this->PauseGame(true);
        Whole LevelScore = this->Scorer->PresentFinalScore();
        String LevelName = this->LevelMan->GetCurrentLevel()->GetName();
        CatchProfile* PlayerProfile = this->ThePlayer->GetProfile();
        if( PlayerProfile != NULL && LevelScore > PlayerProfile->GetHighestScore( LevelName ) ) {
            this->Profiles->SetNewHighScoreInUI(LevelName,LevelScore);
            PlayerProfile->SetNewHighScore(LevelName,LevelScore);
        }
    }
    this->CurrentState = StateToSet;
}

Boole CatchApp::CheckEndOfLevel()
{
    if( this->Scorer->GetNumScoreAreas() == 0 )
        return false;
    if( this->CurrentState == CatchApp::Catch_ScoreScreen )
        return true;
    if( this->AllStartZonesEmpty() ) {
        if( !this->EndTimer ) {
            this->EndTimer = new Timer();
            this->EndTimer->SetCountMode(Mezzanine::CM_CountDown);
            this->EndTimer->SetCurrentTimeInMilliseconds(5 * 1000);
            this->EndTimer->Start();
        }

        if( this->Scorer->GetNumAddedThrowables() > 0 ) {
            this->EndTimer->Reset(5 * 1000000);
        }

        return this->EndTimer->IsStopped();
    }else{
        if( this->EndTimer ) {
            delete this->EndTimer;
            this->EndTimer = NULL;
        }
        return false;
    }
}

Boole CatchApp::AllStartZonesEmpty()
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
    //if( "MainMenu" == LevelMan->GetCurrentLevel()->GetName() )
    //    return;

    this->LevelMan->UnloadLevel();

    this->StartAreas.clear();
    this->ThrownItems.clear();

    this->Scorer->ResetLevelData();
    if( this->EndTimer != NULL ) {
        delete this->EndTimer;
        this->EndTimer = NULL;
    }

    UI::UIManager* UIMan = UI::UIManager::GetSingletonPtr();
    UI::Screen* GameScreen = UIMan->GetScreen("GameScreen");
    GameScreen->GetWidget("GS_LevelReport")->Hide();
    GameScreen->GetWidget("GS_MenuRoot")->Hide();
    GameScreen->GetWidget("GS_ItemShopRoot")->Hide();// */
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

    // Get our manager pointers we'll use.
    EventManager* EventMan = static_cast<EventManager*>( this->TheEntresol->GetManager(ManagerBase::MT_EventManager) );
    AreaEffectManager* AreaEffectMan = static_cast<AreaEffectManager*>( this->TheWorld->GetManager(ManagerBase::MT_AreaEffectManager) );
    Audio::AudioManager* AudioMan = static_cast<Audio::AudioManager*>( this->TheEntresol->GetManager(ManagerBase::MT_AudioManager) );
    Audio::SoundScapeManager* SoundScapeMan = static_cast<Audio::SoundScapeManager*>( this->TheWorld->GetManager(ManagerBase::MT_SoundScapeManager) );
    Graphics::GraphicsManager* GraphicsMan = static_cast<Graphics::GraphicsManager*>( this->TheEntresol->GetManager(ManagerBase::MT_GraphicsManager) );
    Graphics::SceneManager* SceneMan = static_cast<Graphics::SceneManager*>( this->TheWorld->GetManager(ManagerBase::MT_SceneManager) );
    Input::InputManager* InputMan = static_cast<Input::InputManager*>( this->TheEntresol->GetManager(ManagerBase::MT_InputManager) );
    Physics::PhysicsManager* PhysicsMan = static_cast<Physics::PhysicsManager*>( this->TheWorld->GetManager(ManagerBase::MT_PhysicsManager) );
    Scripting::Lua::Lua51ScriptingEngine* ScriptingMan = static_cast<Scripting::Lua::Lua51ScriptingEngine*>( this->TheEntresol->GetManager(ManagerBase::MT_ScriptingManager) );
    UI::UIManager* UIMan = static_cast<UI::UIManager*>( this->TheEntresol->GetManager(ManagerBase::MT_UIManager) );

    // WorkUnit configuration
    this->AudioSettingsWork = new AudioSettingsWorkUnit(UIMan,AudioMan);
    this->AudioSettingsWork->AddDependency( UIMan->GetWidgetUpdateWork() );
    AudioMan->GetBufferUpdate2DWork()->AddDependency( this->AudioSettingsWork );
    if( SoundScapeMan != NULL ) {
        SoundScapeMan->GetBufferUpdate3DWork()->AddDependency( this->AudioSettingsWork );
    }
    this->TheEntresol->GetScheduler().AddWorkUnitMain( this->AudioSettingsWork, "AudioSettingsWork" );

    this->VideoSettingsWork = new VideoSettingsWorkUnit(UIMan,GraphicsMan);
    this->VideoSettingsWork->AddDependency( UIMan->GetWidgetUpdateWork() );
    // Add a line here setting the graphics monopoly as a dependency?
    this->TheEntresol->GetScheduler().AddWorkUnitAffinity( this->VideoSettingsWork, "VideoSettingsWork" );

    this->PreInputWork = new CatchPreInputWorkUnit(this);
    EventMan->GetEventPumpWork()->AddDependency( this->PreInputWork );
    this->TheEntresol->GetScheduler().AddWorkUnitMain( this->PreInputWork, "PreInputWork" );

    this->PostInputWork = new CatchPostInputWorkUnit(this);
    this->PostInputWork->AddDependency( InputMan->GetDeviceUpdateWork() );
    this->TheEntresol->GetScheduler().AddWorkUnitMain( this->PostInputWork, "PostInputWork" );

    this->PostUIWork = new CatchPostUIWorkUnit(this);
    this->PostUIWork->AddDependency( UIMan->GetWidgetUpdateWork() );
    this->PostUIWork->AddDependency( PhysicsMan->GetSimulationWork() );
    this->TheEntresol->GetScheduler().AddWorkUnitMain( this->PostUIWork, "PostUIWork" );

    this->PauseWork = new CatchPauseWorkUnit(this,UIMan);
    this->PauseWork->AddDependency( UIMan->GetWidgetUpdateWork() );
    this->PauseWork->AddDependency( AreaEffectMan->GetAreaEffectUpdateWork() );
    this->TheEntresol->GetScheduler().AddWorkUnitMain( this->PauseWork, "PauseWork" );

    this->HUDUpdateWork = new CatchHUDUpdateWorkUnit(this);
    this->HUDUpdateWork->AddDependency( UIMan->GetWidgetUpdateWork() );
    this->HUDUpdateWork->AddDependency( AreaEffectMan->GetAreaEffectUpdateWork() );
    this->TheEntresol->GetScheduler().AddWorkUnitMain( this->HUDUpdateWork, "HUDUpdateWork" );

    this->EndLevelWork = new CatchEndLevelWorkUnit(this);
    this->EndLevelWork->AddDependency( AreaEffectMan->GetAreaEffectUpdateWork() );
    this->TheEntresol->GetScheduler().AddWorkUnitMain( this->EndLevelWork, "EndLevelWork" );

    this->RegisterTypes();

    // Initialize the managers.
    this->TheEntresol->Initialize(false);
    this->TheWorld->Initialize();

    this->LuaScriptWork = new Scripting::Lua::Lua51WorkUnit( ScriptingMan );
    this->LuaScriptWork->AddDependency( AreaEffectMan->GetAreaEffectUpdateWork() );
    this->TheEntresol->GetScheduler().AddWorkUnitMain( this->LuaScriptWork, "LuaWork" );

    this->Profiles->Initialize();
    this->ThePlayer->InitWorldObjects(this->TheWorld); /* Why the hell is this line important? */
    this->ThePlayer->SetIdentity(this->Profiles->GetLastLoadedProfile());

    this->CreateLoadingScreen();

    this->ChangeState(CatchApp::Catch_Loading);

    // Setup the Music
    this->InitMusic();
    // Generate the UI
    this->MakeGUI();
    // Detect the levels and populate our UI
    this->LevelMan->DetectLevels();
    this->LevelMan->PopulateLevelSelectUI();
    // Detect the profiles and populate our UI
    this->Profiles->ApplyProfileDataToLevelSelect(this->ThePlayer->GetProfile());
    this->Profiles->ApplyProfileDataToProfileList(this->ThePlayer->GetProfile());

    AudioMan->GetMusicPlayer()->Play();
    this->LevelMan->SetNextLevel("MainMenu");
    do{
        this->ChangeState(CatchApp::Catch_Loading);
        this->PauseGame(false);
        GraphicsMan->RenderOneFrame();
        //Actually Load the game stuff
        this->TheEntresol->_Log( "Loading Level: " + this->GetLevelManager()->GetNextLevel()->GetName() + ".\n" );
        this->LevelMan->LoadNextLevel();

        CatchApp::GameState NewState = ( "MainMenu" == LevelMan->GetCurrentLevel()->GetName() ? CatchApp::Catch_MenuScreen : CatchApp::Catch_GameScreen );
        this->ChangeState(NewState);

        GraphicsMan->GetGameWindow(0)->GetViewport(0)->SetCamera( this->ThePlayer->GetControl().GetCamera() );

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

CatchApp::GameState CatchApp::GetState() const
{
    return this->CurrentState;
}

void CatchApp::PauseGame(Boole Pause)
{
    if( this->Paused != Pause ) {
        if( CurrentState == CatchApp::Catch_ScoreScreen && !Pause ) {
            return;
        }
        //UI::Screen* GameScreen = UI::UIManager::GetSingletonPtr()->GetScreen("GameScreen");
        //if( !Pause && ( GameScreen->GetWidget("GS_MenuRoot")->IsVisible() || GameScreen->GetWidget("GS_ItemShopRoot")->IsVisible() ) ) {
        //    return;
        //}
        this->TheWorld->PauseWorld(Pause);
        if( Pause ) {
            this->LevelTimer->Stop();
        }else{
            this->LevelTimer->Start();
        }
        if( this->EndTimer ) {
            if(Pause) this->EndTimer->Stop();
            else this->EndTimer->Start();
        }
        this->Paused = Pause;
    }
}

Boole CatchApp::GameIsPaused() const
{
    return Paused;
}

void CatchApp::SetVisibleScreens(const CatchApp::GameState State)
{
    UI::UIManager* UIMan = static_cast<UI::UIManager*>( this->TheEntresol->GetManager(ManagerBase::MT_UIManager) );
    switch( State )
    {
        case CatchApp::Catch_GameScreen:
        case CatchApp::Catch_ScoreScreen:
        {
            UIMan->HideAllScreens();
            UI::Screen* GameScreen = UIMan->GetScreen("GameScreen");
            if( GameScreen != NULL ) {
                GameScreen->Show();
            }
            UI::Screen* StatsScreen = UIMan->GetScreen("StatsScreen");
            if( StatsScreen != NULL ) {
                StatsScreen->Show();
            }
            break;
        }
        case CatchApp::Catch_Loading:
        {
            UIMan->HideAllScreens();
            UI::Screen* LoadingScreen = UIMan->GetScreen("LoadingScreen");
            if( LoadingScreen != NULL ) {
                LoadingScreen->Show();
            }
            UI::Screen* StatsScreen = UIMan->GetScreen("StatsScreen");
            if( StatsScreen != NULL ) {
                StatsScreen->Show();
            }
            break;
        }
        case CatchApp::Catch_MenuScreen:
        {
            UIMan->HideAllScreens();
            UI::Screen* MainMenuScreen = UIMan->GetScreen("MainMenuScreen");
            if( MainMenuScreen != NULL ) {
                MainMenuScreen->Show();
            }
            UI::Screen* StatsScreen = UIMan->GetScreen("StatsScreen");
            if( StatsScreen != NULL ) {
                StatsScreen->Show();
            }
            break;
        }
        case CatchApp::Catch_Init:
        default:
            break;
    }
}

Boole CatchApp::IsAThrowable(WorldObject* Throwable) const
{
    for( ThrowableContainer::const_iterator ObjIt = this->ThrownItems.begin() ; ObjIt != this->ThrownItems.end() ; ObjIt++ )
    {
        if( Throwable == (*ObjIt) )
            return true;
    }
    return false;
}

Boole CatchApp::IsInsideAnyStartZone(Debris* Throwable) const
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

CatchPauseWorkUnit* CatchApp::GetPauseWork() const
    { return this->PauseWork; }

CatchHUDUpdateWorkUnit* CatchApp::GetHUDUpdateWork() const
    { return this->HUDUpdateWork; }

CatchEndLevelWorkUnit* CatchApp::GetEndLevelWork() const
    { return this->EndLevelWork; }

Scripting::Lua::Lua51WorkUnit* CatchApp::GetLuaScriptWork() const
    { return this->LuaScriptWork; }

Entresol* CatchApp::GetTheEntresol() const
    { return this->TheEntresol; }

World* CatchApp::GetTheWorld() const
    { return this->TheWorld; }

MousePicker& CatchApp::GetPicker()
    { return this->Picker; }

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

CatchPlayer* CatchApp::GetPlayer() const
    { return this->ThePlayer; }

Timer* CatchApp::GetLevelTimer() const
    { return this->LevelTimer; }

Timer* CatchApp::GetEndTimer() const
    { return this->EndTimer; }

#endif
