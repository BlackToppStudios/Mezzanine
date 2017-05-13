#ifndef _workunits_cpp
#define _workunits_cpp

#include "workunits.h"
#include "catchapp.h"

///////////////////////////////////////////////////////////////////////////////
// AudioSettingsWorkUnit Methods

AudioSettingsWorkUnit::AudioSettingsWorkUnit(UI::UIManager* UITarget, ManagerBase* Target)
    { this->SettingsSubscriber = new QueuedSettingsSubscriber(UITarget,Target); }

AudioSettingsWorkUnit::~AudioSettingsWorkUnit()
    { delete this->SettingsSubscriber; }

QueuedSettingsSubscriber* AudioSettingsWorkUnit::GetSettingsSubscriber() const
    { return this->SettingsSubscriber; }

void AudioSettingsWorkUnit::DoWork(Threading::DefaultThreadSpecificStorage::Type& CurrentThreadStorage)
{
    this->SettingsSubscriber->StartUpdate();
    UI::UIManager* UIMan = this->SettingsSubscriber->UITarget;
    Audio::AudioManager* AudioMan = static_cast<Audio::AudioManager*>( this->SettingsSubscriber->TargetManager );
    CatchQueuedSubscriber::ConstEventIterator EndEvent = this->SettingsSubscriber->GetEndEvent();
    for( CatchQueuedSubscriber::ConstEventIterator EvIt = this->SettingsSubscriber->GetFirstEvent() ; EvIt != EndEvent ; ++EvIt )
    {
        EventPtr Args = (*EvIt);
        UI::WidgetEventPtr WidArgs = std::static_pointer_cast<UI::WidgetEvent>( Args );
        if( WidArgs->EventName == UI::Scrollbar::EventScrollValueChanged ) {
            UI::ScrollValueChangedEventPtr ScrollArgs = std::static_pointer_cast<UI::ScrollValueChangedEvent>( WidArgs );
            // Check for "MS_MusicVolume" or "GS_MusicVolume"
            // Else check for "MS_EffectsVolume" or "GS_EffectsVolume"
            if( StringTools::EndsWith( WidArgs->WidgetName, "_MusicVolume", true ) ) {
                AudioMan->SetMusicVolume( ScrollArgs->NewScrollerValue );
            }else if( StringTools::EndsWith( WidArgs->WidgetName, "_EffectsVolume", true ) ) {
                AudioMan->SetEffectVolume( ScrollArgs->NewScrollerValue );
            }
        }else if( WidArgs->EventName == UI::CheckBox::EventSelected ) {
            // Check for "MS_MuteBox" or "GS_MuteBox"
            if( StringTools::EndsWith( WidArgs->WidgetName, "_MuteBox", true ) ) {
                AudioMan->SetMasterMute(true);
            }
        }else if( WidArgs->EventName == UI::CheckBox::EventDeselected ) {
            // Check for "MS_MuteBox" or "GS_MuteBox"
            if( StringTools::EndsWith( WidArgs->WidgetName, "_MuteBox", true ) ) {
                AudioMan->SetMasterMute(false);
            }
        }else if( WidArgs->EventName == UI::PagedContainer::EventChildSelected ) {
            //
            // Fill in later
            //
        }else if( WidArgs->EventName == UI::Widget::EventVisibilityShown ) {
            // Start by getting the proper screen
            UI::Screen* EventScreen = NULL;
            // Check for "M" from "MS_" prefix on menu screen widgets.
            // Else check for "G" from "GS_" prefix on game screen widgets.
            if( WidArgs->WidgetName.at(0) == 'M' ) {
                EventScreen = UIMan->GetScreen("MainMenuScreen");
            }else if( WidArgs->WidgetName.at(0) == 'G' ) {
                EventScreen = UIMan->GetScreen("GameScreen");
            }
            // Proceed only if we have a valid screen
            if( EventScreen != NULL ) {
                if( StringTools::EndsWith( WidArgs->WidgetName, "_MusicVolume", true ) ) {
                    // Update the widget to the current system music volume
                    UI::Scrollbar* MusicVol = static_cast<UI::Scrollbar*>( EventScreen->GetWidget( WidArgs->WidgetName ) );
                    MusicVol->SetScrollerValue( AudioMan->GetMusicVolume() );
                }else if( StringTools::EndsWith( WidArgs->WidgetName, "_EffectsVolume", true ) ) {
                    // Update the widget to the current system effects volume
                    UI::Scrollbar* EffectsVol = static_cast<UI::Scrollbar*>( EventScreen->GetWidget( WidArgs->WidgetName ) );
                    EffectsVol->SetScrollerValue( AudioMan->GetEffectVolume() );
                }else if( StringTools::EndsWith( WidArgs->WidgetName, "_MuteBox", true ) ) {
                    // Update the widget to the current system mute setting
                    UI::CheckBox* MuteBox = static_cast<UI::CheckBox*>( EventScreen->GetWidget( WidArgs->WidgetName ) );
                    MuteBox->ManualSelect( AudioMan->IsMuted() );
                }/*else if( StringTools::EndsWith( WidArgs->WidgetName, "_AudioDeviceList", true ) ) {
                    UI::DropDownList* AudioDeviceList = static_cast<UI::DropDownList*>( EventScreen->GetWidget( WidArgs->WidgetName ) );
                    UI::ListBox* DeviceContainer = AudioDeviceList->GetSelectionList();
                    // If we have no items, we need to populate them
                    if( DeviceContainer->GetNumListItems() == 0 ) {
                        Whole DetectedDevices = AudioMan->GetAvailablePlaybackDeviceCount();
                        for( Whole DeviceIndex = 0 ; DeviceIndex < DetectedDevices ; ++DeviceIndex )
                        {
                            String DeviceName = AudioMan->GetAvailablePlaybackDeviceName(DeviceIndex);
                            DeviceContainer->CreateSingleLineListItem(DeviceName,DeviceName);
                        }
                        AudioDeviceList->UpdateChildDimensions();
                    }
                    AudioDeviceList->UpdateCurrentSelection( DeviceContainer->GetListItem( AudioMan->GetCurrentPlaybackDeviceName() ) );
                }// */
            }
        }
    }
    this->SettingsSubscriber->ClearEvents();
    this->SettingsSubscriber->EndUpdate();
}

///////////////////////////////////////////////////////////////////////////////
// VideoSettingsWorkUnit Methods

VideoSettingsWorkUnit::VideoSettingsWorkUnit(UI::UIManager* UITarget, ManagerBase* Target)
    { this->SettingsSubscriber = new QueuedSettingsSubscriber(UITarget,Target); }

VideoSettingsWorkUnit::~VideoSettingsWorkUnit()
    { delete this->SettingsSubscriber; }

QueuedSettingsSubscriber* VideoSettingsWorkUnit::GetSettingsSubscriber() const
    { return this->SettingsSubscriber; }

void VideoSettingsWorkUnit::DoWork(Threading::DefaultThreadSpecificStorage::Type& CurrentThreadStorage)
{
    this->SettingsSubscriber->StartUpdate();
    UI::UIManager* UIMan = this->SettingsSubscriber->UITarget;
    Graphics::GraphicsManager* GraphicsMan = static_cast<Graphics::GraphicsManager*>( this->SettingsSubscriber->TargetManager );
    Graphics::GameWindow* CatchWindow = GraphicsMan->GetGameWindow(0);
    CatchQueuedSubscriber::ConstEventIterator EndEvent = this->SettingsSubscriber->GetEndEvent();
    for( CatchQueuedSubscriber::ConstEventIterator EvIt = this->SettingsSubscriber->GetFirstEvent() ; EvIt != EndEvent ; ++EvIt )
    {
        EventPtr Args = (*EvIt);
        UI::WidgetEventPtr WidArgs = std::static_pointer_cast<UI::WidgetEvent>( Args );

        // Start by getting the proper screen
        UI::Screen* EventScreen = NULL;
        // Check for "M" from "MS_" prefix on menu screen widgets.
        // Else check for "G" from "GS_" prefix on game screen widgets.
        if( WidArgs->WidgetName.at(0) == 'M' ) {
            EventScreen = UIMan->GetScreen("MainMenuScreen");
        }else if( WidArgs->WidgetName.at(0) == 'G' ) {
            EventScreen = UIMan->GetScreen("GameScreen");
        }

        // Proceed only if we have a valid screen
        if( EventScreen != NULL ) {
            if( WidArgs->EventName == UI::Button::EventDeactivated ) {
                if( StringTools::EndsWith( WidArgs->WidgetName, "_VideoOptsApply", true ) ) {
                    String NamePrefix;
                    if( EventScreen->GetName() == "MainMenuScreen" ) {
                        NamePrefix = "MS";
                    }else if( EventScreen->GetName() == "GameScreen" ) {
                        NamePrefix = "GS";
                    }

                    // Get our pointers
                    Graphics::GameWindow* CatchWindow = GraphicsMan->GetGameWindow(0);
                    UI::UIManager* UIMan = EventScreen->GetManager();
                    UI::Screen* StatsScreen = UIMan->GetScreen("StatsScreen");
                    // Create our window related data
                    Graphics::WindowSettings NewSettings;
                    NewSettings.Fullscreen = static_cast<UI::CheckBox*>( EventScreen->GetWidget(NamePrefix + "_FullscreenBox") )->IsSelected();
                    // Get render resolution data
                    String SelectedRes = static_cast<UI::DropDownList*>( EventScreen->GetWidget(NamePrefix + "_ResolutionList") )->GetSelectionText();
                    String StrWidth = SelectedRes.substr(0,SelectedRes.find_first_of("x"));
                    String StrHeight = SelectedRes.substr(SelectedRes.find_last_of("x") + 1);
                    NewSettings.WinRes.Width = StringTools::ConvertToWhole(StrWidth);
                    NewSettings.WinRes.Height = StringTools::ConvertToWhole(StrHeight);
                    // Apply the resolution and fullscreen settings
                    CatchWindow->SetRenderOptions(NewSettings);// */
                    // Now get the FSAA setting and pass it along
                    String FSAASetting = static_cast<UI::DropDownList*>( EventScreen->GetWidget(NamePrefix + "_FSAAList") )->GetSelectionText();
                    CatchWindow->SetFSAALevel( StringTools::ConvertToWhole( FSAASetting.substr(1,1) ) );

                    // Apply other settings
                    UI::CheckBox* FPSStatsBox = static_cast<UI::CheckBox*>( EventScreen->GetWidget(NamePrefix + "_StatsBox") );
                    StatsScreen->GetWidget("SS_CurrentFPS")->SetVisible( FPSStatsBox->IsSelected() );
                    StatsScreen->GetWidget("SS_AverageFPS")->SetVisible( FPSStatsBox->IsSelected() );// */
                }
            }else if( WidArgs->EventName == UI::Widget::EventVisibilityShown ) {
                if( StringTools::EndsWith( WidArgs->WidgetName, "_FSAAList", true ) ) {
                    UI::DropDownList* FSAAList = static_cast<UI::DropDownList*>( EventScreen->GetWidget( WidArgs->WidgetName ) );
                    UI::ListBox* FSAAContainer = FSAAList->GetSelectionList();
                    // If we have no items, we need to populate them
                    if( FSAAContainer->GetNumListItems() == 0 ) {
                        FSAAContainer->CreateSingleLineListItem("No AA","No AA");
                        for( Whole FSAALevel = 1 ; FSAALevel <= 4 ; ++FSAALevel )
                        {
                            Whole Temp = 2;
                            for( Whole X = 1 ; X < FSAALevel ; ++X )
                                { Temp <<= 1; }

                            StringStream FSAALevelName;
                            FSAALevelName << "x" << Temp << " AA";
                            FSAAContainer->CreateSingleLineListItem(FSAALevelName.str(),FSAALevelName.str());
                        }
                        FSAAList->UpdateChildDimensions();
                    }
                    Whole FSAALevel = CatchWindow->GetFSAALevel();
                    if( FSAALevel == 0 ) {
                        FSAAList->UpdateCurrentSelection( FSAAContainer->GetListItem( "No AA" ) );
                    }else{
                        StringStream CurrentFSAAName;
                        CurrentFSAAName << "x" << FSAALevel << " AA";
                        FSAAList->UpdateCurrentSelection( FSAAContainer->GetListItem( CurrentFSAAName.str() ) );
                    }
                }else if( StringTools::EndsWith( WidArgs->WidgetName, "_ResolutionList", true ) ) {
                    UI::DropDownList* ResolutionList = static_cast<UI::DropDownList*>( EventScreen->GetWidget( WidArgs->WidgetName ) );
                    UI::ListBox* ResolutionContainer = ResolutionList->GetSelectionList();
                    // If we have no items, we need to populate them
                    if( ResolutionContainer->GetNumListItems() == 0 ) {
                        const Graphics::GraphicsManager::ResolutionContainer& DetectedResolutions = GraphicsMan->GetSupportedResolutions();
                        for( Graphics::GraphicsManager::ConstResolutionIterator ResIt = DetectedResolutions.begin() ; ResIt != DetectedResolutions.end() ; ++ResIt )
                        {
                            String ResolutionName = (*ResIt).GetAsString();
                            ResolutionContainer->CreateSingleLineListItem(ResolutionName,ResolutionName);
                        }
                        ResolutionList->UpdateChildDimensions();
                    }
                    ResolutionList->UpdateCurrentSelection( ResolutionContainer->GetListItem( CatchWindow->GetResolution().GetAsString() ) );
                }else if( StringTools::EndsWith( WidArgs->WidgetName, "_FullscreenBox", true ) ) {
                    UI::CheckBox* FullscreenBox = static_cast<UI::CheckBox*>( EventScreen->GetWidget( WidArgs->WidgetName ) );
                    FullscreenBox->ManualSelect( CatchWindow->GetFullscreen() );
                }else if( StringTools::EndsWith( WidArgs->WidgetName, "_StatsBox", true ) ) {
                    UI::CheckBox* StatsBox = static_cast<UI::CheckBox*>( EventScreen->GetWidget( WidArgs->WidgetName ) );
                    UI::Screen* StatsScreen = UIMan->GetScreen("StatsScreen");
                    StatsBox->ManualSelect( StatsScreen->GetWidget("SS_CurrentFPS")->GetVisible() || StatsScreen->GetWidget("SS_AverageFPS")->GetVisible() );
                }// Is a widget we're looking for
            }// Is an event we're looking for
        }// If we have a screen
    }// For each event
    this->SettingsSubscriber->ClearEvents();
    this->SettingsSubscriber->EndUpdate();
}

///////////////////////////////////////////////////////////////////////////////
// CatchPostInputWorkUnit Methods

CatchPostInputWorkUnit::CatchPostInputWorkUnit(CatchApp* Target) :
    CatchApplication(Target) {  }

CatchPostInputWorkUnit::~CatchPostInputWorkUnit()
    {  }

void CatchPostInputWorkUnit::DoWork(Threading::DefaultThreadSpecificStorage::Type& CurrentThreadStorage)
{
    // Setup our pointers
    Input::InputManager* InputMan = Input::InputManager::GetSingletonPtr();
    Input::Mouse* SysMouse = InputMan->GetSystemMouse();
    Input::Keyboard* SysKeyboard = InputMan->GetSystemKeyboard();
    const Whole DeltaTime = this->CatchApplication->TheEntresol->GetLastFrameTimeMilliseconds();
    // Update our player
    this->CatchApplication->GetPlayer()->Update(InputMan,DeltaTime);
    // Determine our Debug drawer visibility
    if( Input::BUTTON_PRESSING == SysKeyboard->GetButtonState(Input::KEY_C) ) {
        Physics::PhysicsManager* PhysMan = static_cast<Physics::PhysicsManager*>( this->CatchApplication->GetTheWorld()->GetManager(ManagerBase::MT_PhysicsManager) );
        if( PhysMan->GetDebugRenderingMode() == Physics::DDM_NoDebug ) {
            PhysMan->SetDebugRenderingMode( Physics::DDM_DrawWireframe );
        }else{
            PhysMan->SetDebugRenderingMode( Physics::DDM_NoDebug );
        }
    }
    // Determine UI screen visibility
    static Boole HideUI = false;
    if( SysKeyboard->IsButtonPressing(Input::KEY_F4) ) {
        Boole Toggled = !HideUI;
        if( Toggled ) {
            static_cast<UI::UIManager*>( this->CatchApplication->TheEntresol->GetManager(ManagerBase::MT_UIManager) )->HideAllScreens();
        }else{
            this->CatchApplication->SetVisibleScreens( this->CatchApplication->GetState() );
        }
        HideUI = Toggled;
    }
}

///////////////////////////////////////////////////////////////////////////////
// CatchPostUIWorkUnit Methods

CatchPostUIWorkUnit::CatchPostUIWorkUnit(CatchApp* Target) :
    CatchApplication(Target),
    UIMan( static_cast<UI::UIManager*>( Target->GetTheEntresol()->GetManager(ManagerBase::MT_UIManager) ) )
    {  }

CatchPostUIWorkUnit::~CatchPostUIWorkUnit()
    {  }

void CatchPostUIWorkUnit::DoWork(Threading::DefaultThreadSpecificStorage::Type& CurrentThreadStorage)
{
    if( !UIMan->MouseIsInUISystem() ) {
        this->CatchApplication->GetPicker().Execute();
    }
}

///////////////////////////////////////////////////////////////////////////////
// CatchPauseWorkUnit Methods

CatchPauseWorkUnit::CatchPauseWorkUnit(CatchApp* Target, UI::UIManager* UITarget)
    { this->PauseSubscriber = new QueuedPauseSubscriber(Target,UITarget); }

CatchPauseWorkUnit::~CatchPauseWorkUnit()
    { delete this->PauseSubscriber; }

QueuedPauseSubscriber* CatchPauseWorkUnit::GetPauseSubscriber() const
    { return this->PauseSubscriber; }

void CatchPauseWorkUnit::DoWork(Threading::DefaultThreadSpecificStorage::Type& CurrentThreadStorage)
{
    this->PauseSubscriber->StartUpdate();
    UI::Screen* GameScreen = this->PauseSubscriber->UITarget->GetScreen("GameScreen");
    CatchQueuedSubscriber::ConstEventIterator EndEvent = this->PauseSubscriber->GetEndEvent();
    for( CatchQueuedSubscriber::ConstEventIterator EvIt = this->PauseSubscriber->GetFirstEvent() ; EvIt != EndEvent ; ++EvIt )
    {
        EventPtr Args = (*EvIt);
        UI::WidgetEventPtr WidArgs = std::static_pointer_cast<UI::WidgetEvent>( Args );

        if( (*EvIt)->EventName == UI::Widget::EventVisibilityShown ) {
            UI::MenuEntry* EventMenu = static_cast<UI::MenuEntry*>( GameScreen->GetWidget( WidArgs->WidgetName ) );
            if( EventMenu != NULL ) {
                UI::MenuEntry::MenuEntryContainer* Stack = EventMenu->_GetMenuStack();
                if( Stack != NULL && !Stack->empty() ) {
                    this->PauseSubscriber->CatchApplication->PauseGame(true);
                }
            }
        }else if( (*EvIt)->EventName == UI::Button::EventVisibilityHidden ) {
            UI::MenuEntry* EventMenu = static_cast<UI::MenuEntry*>( GameScreen->GetWidget( WidArgs->WidgetName ) );
            if( EventMenu != NULL ) {
                UI::MenuEntry::MenuEntryContainer* Stack = EventMenu->_GetMenuStack();
                if( Stack != NULL && Stack->empty() ) {
                    this->PauseSubscriber->CatchApplication->PauseGame(false);
                }
            }
        }
        /*if( (*EvIt)->EventName == UI::Button::EventActivated ) {

        }else if( (*EvIt)->EventName == UI::Button::EventDeactivated ) {

        }// */
    }// For each event
    this->PauseSubscriber->ClearEvents();
    this->PauseSubscriber->EndUpdate();
}

///////////////////////////////////////////////////////////////////////////////
// CatchHUDUpdateWorkUnit Methods

CatchHUDUpdateWorkUnit::CatchHUDUpdateWorkUnit(CatchApp* Target) :
    CatchApplication(Target) {  }

CatchHUDUpdateWorkUnit::~CatchHUDUpdateWorkUnit()
    {  }

void CatchHUDUpdateWorkUnit::DoWork(Threading::DefaultThreadSpecificStorage::Type& CurrentThreadStorage)
{
    if( this->CatchApplication->GetState() == CatchApp::Catch_GameScreen ) {
        // Get our UI pointers
        Graphics::GraphicsManager* GraphicsMan = static_cast<Graphics::GraphicsManager*>( this->CatchApplication->TheEntresol->GetManager(ManagerBase::MT_GraphicsManager) );
        UI::UIManager* UIMan = static_cast<UI::UIManager*>( this->CatchApplication->TheEntresol->GetManager(ManagerBase::MT_UIManager) );
        UI::Screen* GameScreen = UIMan->GetScreen("GameScreen");
        UI::Screen* StatsScreen = UIMan->GetScreen("StatsScreen");

        // Update the timer
        UI::Widget* TimerWid = GameScreen->GetWidget("GS_TimerValue");
        static_cast<UI::SingleLineTextLayer*>( TimerWid->GetRenderLayer(0,UI::RLT_SingleLineText) )->SetText( this->CatchApplication->GetLevelTimer()->GetTimeAsText(Mezzanine::TF_MinutesSeconds) );

        // Update the score display
        LevelScorer::ScorePair LevelScores = this->CatchApplication->GetLevelScorer()->CalculateThrowableScore();
        UI::Widget* ScoreWid = GameScreen->GetWidget("GS_ScoreValue");
        static_cast<UI::SingleLineTextLayer*>( ScoreWid->GetRenderLayer(0,UI::RLT_SingleLineText) )->SetText( StringTools::ConvertToString( LevelScores.first + LevelScores.second ) );

        // Update framerate stat display
        UI::Widget* CurrFPSWid = StatsScreen->GetWidget("SS_CurrentFPSValue");
        static_cast<UI::SingleLineTextLayer*>( CurrFPSWid->GetRenderLayer(0,UI::RLT_SingleLineText) )->SetText( StringTools::ConvertToString( GraphicsMan->GetGameWindow(0)->GetLastFPS() ) );

        UI::Widget* AverageFPSWid = StatsScreen->GetWidget("SS_AverageFPSValue");
        static_cast<UI::SingleLineTextLayer*>( AverageFPSWid->GetRenderLayer(0,UI::RLT_SingleLineText) )->SetText( StringTools::ConvertToString( GraphicsMan->GetGameWindow(0)->GetAverageFPS() ) );
    }
}

///////////////////////////////////////////////////////////////////////////////
// CatchEndLevelWorkUnit Methods

CatchEndLevelWorkUnit::CatchEndLevelWorkUnit(CatchApp* Target) :
    CatchApplication(Target) {  }

CatchEndLevelWorkUnit::~CatchEndLevelWorkUnit()
    {  }

void CatchEndLevelWorkUnit::DoWork(Threading::DefaultThreadSpecificStorage::Type& CurrentThreadStorage)
{
    //See if the level is over
    CatchApp::GameState State = this->CatchApplication->GetState();
    if( State == CatchApp::Catch_GameScreen && State != CatchApp::Catch_ScoreScreen ) {
        if( this->CatchApplication->CheckEndOfLevel() ) {
            this->CatchApplication->ChangeState(CatchApp::Catch_ScoreScreen);
        }
    }
}

#endif
