#ifndef _widgetcallbacks_cpp
#define _widgetcallbacks_cpp

#include "widgetcallbacks.h"

OptsVolume::~OptsVolume()
{
}

void OptsVolume::SetCaller(UI::Widget* Caller)
{
    if(UI::Widget::W_Scrollbar != Caller->GetType())
        World::GetWorldPointer()->LogAndThrow(Exception("Volume Widget callback was attempted to be applied to a non-scrollbar widget."));
    UI::WidgetCallback::SetCaller(Caller);
    /// @todo This code should eventually be replaced with something that reads from a settings file.
    UI::Scrollbar* Scroll = static_cast<UI::Scrollbar*>(this->Caller);
    Scroll->SetScrollerValue(0.5);
    SetVolume(Scroll->GetScrollerValue());
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
    AudioManager::GetSingletonPtr()->SetAmbientVolume(Volume);
}

Real OptsAmbientVol::GetVolume() const
{
    return AudioManager::GetSingletonPtr()->GetAmbientVolume();
}

void OptsDialogVol::SetVolume(const Real& Volume)
{
    AudioManager::GetSingletonPtr()->SetDialogVolume(Volume);
}

Real OptsDialogVol::GetVolume() const
{
    return AudioManager::GetSingletonPtr()->GetDialogVolume();
}

void OptsEffectVol::SetVolume(const Real& Volume)
{
    AudioManager::GetSingletonPtr()->SetEffectVolume(Volume);
}

Real OptsEffectVol::GetVolume() const
{
    return AudioManager::GetSingletonPtr()->GetEffectVolume();
}

void OptsMusicVol::SetVolume(const Real& Volume)
{
    AudioManager::GetSingletonPtr()->SetMusicVolume(Volume);
}

Real OptsMusicVol::GetVolume() const
{
    return AudioManager::GetSingletonPtr()->GetMusicVolume();
}

OptsAudioMute::~OptsAudioMute()
{
}

void OptsAudioMute::SetCaller(UI::Widget* Caller)
{
    if(UI::Widget::W_CheckBox != Caller->GetType())
        World::GetWorldPointer()->LogAndThrow(Exception("Audio Mute Widget callback was attempted to be applied to a non-checkbox widget."));
    UI::WidgetCallback::SetCaller(Caller);
    /// @todo This code should eventually be replaced with something that reads from a settings file.
    bool Mute = AudioManager::GetSingletonPtr()->IsMuted();
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
    AudioManager* AudioMan = AudioManager::GetSingletonPtr();
    bool Mute = AudioMan->IsMuted();
    UI::CheckBox* MuteCheck = static_cast<UI::CheckBox*>(this->Caller);
    if(Mute != MuteCheck->IsChecked())
        AudioMan->Mute(MuteCheck->IsChecked());
}

void OptsAudioMute::DoVisibilityChangeItems()
{
    bool Mute = AudioManager::GetSingletonPtr()->IsMuted();
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
        World::GetWorldPointer()->LogAndThrow(Exception("Video Resolution Widget callback was attempted to be applied to a non-dropdownlist widget."));
    UI::WidgetCallback::SetCaller(Caller);
    UI::DropDownList* ResList = static_cast<UI::DropDownList*>(this->Caller);
    std::stringstream ResStream;
    GraphicsManager* GraphicsMan = GraphicsManager::GetSingletonPtr();
    GameWindow* PriWin = GraphicsMan->GetPrimaryGameWindow();
    ResStream << PriWin->getRenderWidth() << " x " << PriWin->getRenderHeight();
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
        GameWindow* PriWin = GraphicsManager::GetSingletonPtr()->GetPrimaryGameWindow();
        std::stringstream ResStream;
        ResStream << PriWin->getRenderWidth() << " x " << PriWin->getRenderHeight();
        ResList->SetSelection(ResStream.str());
    }
}

OptsVideoFullscreen::~OptsVideoFullscreen()
{
}

void OptsVideoFullscreen::SetCaller(UI::Widget* Caller)
{
    if(UI::Widget::W_CheckBox != Caller->GetType())
        World::GetWorldPointer()->LogAndThrow(Exception("Video Fullscreen Widget callback was attempted to be applied to a non-checkbox widget."));
    UI::WidgetCallback::SetCaller(Caller);
    /// @todo This code should eventually be replaced with something that reads from a settings file.
    /// @todo Temp code to verify fullscreen setting is set properly.
    bool FullScreen = GraphicsManager::GetSingletonPtr()->GetPrimaryGameWindow()->getFullscreen();
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
    bool FullScreen = GraphicsManager::GetSingletonPtr()->GetPrimaryGameWindow()->getFullscreen();
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
        World::GetWorldPointer()->LogAndThrow(Exception("Video FSAA Widget callback was attempted to be applied to a non-dropdownlist widget."));
    UI::WidgetCallback::SetCaller(Caller);
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
        Whole CurrFSAA = GraphicsManager::GetSingletonPtr()->GetPrimaryGameWindow()->GetFSAALevel();
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
        World::GetWorldPointer()->LogAndThrow(Exception("Video Stats Widget callback was attempted to be applied to a non-checkbox widget."));
    UI::WidgetCallback::SetCaller(Caller);
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
    bool Stats = UIManager::GetSingletonPtr()->GetLayer("StatsLayer")->GetVisible();
    UI::CheckBox* StatsCheck = static_cast<UI::CheckBox*>(this->Caller);
    if(Stats != StatsCheck->IsChecked())
        StatsCheck->ManualCheck(Stats);
}

#endif
