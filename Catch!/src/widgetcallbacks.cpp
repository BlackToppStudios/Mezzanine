#ifndef _widgetcallbacks_cpp
#define _widgetcallbacks_cpp

#include "widgetcallbacks.h"

OptsVolume::~OptsVolume()
{
}

void OptsVolume::SetCaller(UI::Widget* Caller)
{
    if(UI::Widget::W_Scrollbar != Caller->GetType())
        { MEZZ_EXCEPTION(Mezzanine::Exception::PARAMETERS_EXCEPTION,"Volume Widget callback was attempted to be applied to a non-scrollbar widget."); }
    UI::WidgetListener::SetCaller(Caller);
    UI::Scrollbar* Scroll = static_cast<UI::Scrollbar*>(this->Caller);
    Scroll->SetScrollerValue(GetVolume());
}

void OptsVolume::DoHoverItems()
{
}

void OptsVolume::DoPreUpdateItems()
{
}

void OptsVolume::DoPostUpdateItems()
{
    UI::Scrollbar* Scroll = static_cast<UI::Scrollbar*>(this->Caller);
    SetVolume(Scroll->GetScrollerValue());
}

void OptsVolume::DoVisibilityChangeItems()
{
    if(Caller->IsVisible())
    {
        UI::Scrollbar* Scroll = static_cast<UI::Scrollbar*>(this->Caller);
        Scroll->SetScrollerValue(this->GetVolume());
    }
}

void OptsAmbientVol::SetVolume(const Real& Volume)
{
    Audio::AudioManager::GetSingletonPtr()->SetAmbientVolume(Volume);
}

Real OptsAmbientVol::GetVolume() const
{
    return Audio::AudioManager::GetSingletonPtr()->GetAmbientVolume();
}

void OptsDialogVol::SetVolume(const Real& Volume)
{
    Audio::AudioManager::GetSingletonPtr()->SetDialogVolume(Volume);
}

Real OptsDialogVol::GetVolume() const
{
    return Audio::AudioManager::GetSingletonPtr()->GetDialogVolume();
}

void OptsEffectVol::SetVolume(const Real& Volume)
{
    Audio::AudioManager::GetSingletonPtr()->SetEffectVolume(Volume);
}

Real OptsEffectVol::GetVolume() const
{
    return Audio::AudioManager::GetSingletonPtr()->GetEffectVolume();
}

void OptsMusicVol::SetVolume(const Real& Volume)
{
    Audio::AudioManager::GetSingletonPtr()->SetMusicVolume(Volume);
}

Real OptsMusicVol::GetVolume() const
{
    return Audio::AudioManager::GetSingletonPtr()->GetMusicVolume();
}

OptsAudioMute::~OptsAudioMute()
{
}

void OptsAudioMute::SetCaller(UI::Widget* Caller)
{
    if(UI::Widget::W_CheckBox != Caller->GetType())
        { MEZZ_EXCEPTION(Mezzanine::Exception::PARAMETERS_EXCEPTION,"Audio Mute Widget callback was attempted to be applied to a non-checkbox widget."); }
    UI::WidgetListener::SetCaller(Caller);
    /// @todo This code should eventually be replaced with something that reads from a settings file.
    bool Mute = Audio::AudioManager::GetSingletonPtr()->IsMuted();
    UI::CheckBox* MuteCheck = static_cast<UI::CheckBox*>(this->Caller);
    if(Mute != MuteCheck->IsChecked())
        MuteCheck->ManualCheck(Mute);
}

void OptsAudioMute::DoHoverItems()
{
}

void OptsAudioMute::DoPreUpdateItems()
{
}

void OptsAudioMute::DoPostUpdateItems()
{
    Audio::AudioManager* AudioMan = Audio::AudioManager::GetSingletonPtr();
    bool Mute = AudioMan->IsMuted();
    UI::CheckBox* MuteCheck = static_cast<UI::CheckBox*>(this->Caller);
    if(Mute != MuteCheck->IsChecked())
        AudioMan->SetMasterMute(MuteCheck->IsChecked());
}

void OptsAudioMute::DoVisibilityChangeItems()
{
    bool Mute = Audio::AudioManager::GetSingletonPtr()->IsMuted();
    UI::CheckBox* MuteCheck = static_cast<UI::CheckBox*>(this->Caller);
    if(Mute != MuteCheck->IsChecked())
        MuteCheck->ManualCheck(Mute);
}

OptsVideoRes::~OptsVideoRes()
{
}

void OptsVideoRes::SetCaller(UI::Widget* Caller)
{
    if(UI::Widget::W_DropDownList != Caller->GetType())
        { MEZZ_EXCEPTION(Mezzanine::Exception::PARAMETERS_EXCEPTION,"Video Resolution Widget callback was attempted to be applied to a non-dropdownlist widget."); }
    UI::WidgetListener::SetCaller(Caller);
    UI::DropDownList* ResList = static_cast<UI::DropDownList*>(this->Caller);
    std::stringstream ResStream;
    Graphics::GraphicsManager* GraphicsMan = Graphics::GraphicsManager::GetSingletonPtr();
    Graphics::GameWindow* Win = GraphicsMan->GetGameWindow(0);
    ResStream << Win->GetRenderWidth() << " x " << Win->GetRenderHeight();
    String CurrentRes(ResStream.str());
    const std::vector<String>* Resolutions = GraphicsMan->GetSupportedResolutions();
    for( Whole X = 0 ; X < Resolutions->size() ; ++X )
    {
        UI::Caption* CurrSelection = ResList->GetSelectionList()->AddSelection((*Resolutions)[X],(*Resolutions)[X]);
        if(CurrentRes == (*Resolutions)[X])
        {
            ResList->SetSelection(CurrSelection);
        }
    }
}

void OptsVideoRes::DoHoverItems()
{
}

void OptsVideoRes::DoPreUpdateItems()
{
}

void OptsVideoRes::DoPostUpdateItems()
{
}

void OptsVideoRes::DoVisibilityChangeItems()
{
    if(Caller->IsVisible())
    {
        UI::DropDownList* ResList = static_cast<UI::DropDownList*>(this->Caller);
        Graphics::GameWindow* Win = Graphics::GraphicsManager::GetSingletonPtr()->GetGameWindow(0);
        std::stringstream ResStream;
        ResStream << Win->GetRenderWidth() << " x " << Win->GetRenderHeight();
        ResList->SetSelection(ResStream.str());
    }
}

OptsVideoFullscreen::~OptsVideoFullscreen()
{
}

void OptsVideoFullscreen::SetCaller(UI::Widget* Caller)
{
    if(UI::Widget::W_CheckBox != Caller->GetType())
        { MEZZ_EXCEPTION(Mezzanine::Exception::PARAMETERS_EXCEPTION,"Video Fullscreen Widget callback was attempted to be applied to a non-checkbox widget."); }
    UI::WidgetListener::SetCaller(Caller);
    /// @todo This code should eventually be replaced with something that reads from a settings file.
    /// @todo Temp code to verify fullscreen setting is set properly.
    bool FullScreen = Graphics::GraphicsManager::GetSingletonPtr()->GetGameWindow(0)->GetFullscreen();
    UI::CheckBox* FSCheck = static_cast<UI::CheckBox*>(this->Caller);
    if(FullScreen != FSCheck->IsChecked())
        FSCheck->ManualCheck(FullScreen);
}

void OptsVideoFullscreen::DoHoverItems()
{
}

void OptsVideoFullscreen::DoPreUpdateItems()
{
}

void OptsVideoFullscreen::DoPostUpdateItems()
{
}

void OptsVideoFullscreen::DoVisibilityChangeItems()
{
    bool FullScreen = Graphics::GraphicsManager::GetSingletonPtr()->GetGameWindow(0)->GetFullscreen();
    UI::CheckBox* FSCheck = static_cast<UI::CheckBox*>(this->Caller);
    if(FullScreen != FSCheck->IsChecked())
        FSCheck->ManualCheck(FullScreen);
}

OptsVideoFSAA::~OptsVideoFSAA()
{
}

void OptsVideoFSAA::SetCaller(UI::Widget* Caller)
{
    if(UI::Widget::W_DropDownList != Caller->GetType())
        { MEZZ_EXCEPTION(Mezzanine::Exception::PARAMETERS_EXCEPTION,"Video FSAA Widget callback was attempted to be applied to a non-dropdownlist widget."); }
    UI::WidgetListener::SetCaller(Caller);
    UI::DropDownList* FSAAList = static_cast<UI::DropDownList*>(this->Caller);
    /// @todo This code should eventually be replaced with something that reads from a settings file.
    UI::Caption* FSAADefault = FSAAList->GetSelectionList()->AddSelection("No AA","No AA");
    FSAAList->SetSelection(FSAADefault);
    for( Whole X = 1 ; X < 5 ; ++X )
    {
        Whole FSAA = 0;
        if(X==1) FSAA = 2;
        else if(X==2) FSAA = 4;
        else if(X==3) FSAA = 8;
        else if(X==4) FSAA = 16;
        std::stringstream AAstream;
        AAstream << "x" << FSAA << " AA";
        UI::Caption* CurrSelection = FSAAList->GetSelectionList()->AddSelection(AAstream.str(),AAstream.str());
    }
}

void OptsVideoFSAA::DoHoverItems()
{
}

void OptsVideoFSAA::DoPreUpdateItems()
{
}

void OptsVideoFSAA::DoPostUpdateItems()
{
}

void OptsVideoFSAA::DoVisibilityChangeItems()
{
    if(Caller->IsVisible())
    {
        UI::DropDownList* FSAAList = static_cast<UI::DropDownList*>(this->Caller);
        Whole CurrFSAA = Graphics::GraphicsManager::GetSingletonPtr()->GetGameWindow(0)->GetFSAALevel();
        std::stringstream AAstream;
        AAstream << "x" << CurrFSAA << " AA";
        FSAAList->SetSelection(CurrFSAA ? AAstream.str() : "No AA");
    }
}

OptsVideoStats::~OptsVideoStats()
{
}

void OptsVideoStats::SetCaller(UI::Widget* Caller)
{
    if(UI::Widget::W_CheckBox != Caller->GetType())
        { MEZZ_EXCEPTION(Mezzanine::Exception::PARAMETERS_EXCEPTION,"Video Stats Widget callback was attempted to be applied to a non-checkbox widget."); }
    UI::WidgetListener::SetCaller(Caller);
    /// @todo This code should eventually be replaced with something that reads from a settings file.
    /// @todo Temp code to verify fullscreen setting is set properly.
    bool Stats = false;
    UI::CheckBox* StatsCheck = static_cast<UI::CheckBox*>(this->Caller);
    if(Stats != StatsCheck->IsChecked())
        StatsCheck->ManualCheck(Stats);
}

void OptsVideoStats::DoHoverItems()
{
}

void OptsVideoStats::DoPreUpdateItems()
{
}

void OptsVideoStats::DoPostUpdateItems()
{
}

void OptsVideoStats::DoVisibilityChangeItems()
{
    bool Stats = UIManager::GetSingletonPtr()->GetScreen("GameScreen")->GetWidget("GS_Stats")->GetVisible();
    UI::CheckBox* StatsCheck = static_cast<UI::CheckBox*>(this->Caller);
    if(Stats != StatsCheck->IsChecked())
        StatsCheck->ManualCheck(Stats);
}

#endif
