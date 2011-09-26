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
    UI::Scrollbar* Scroll = static_cast<UI::Scrollbar*>(this->Caller);
    Scroll->SetScrollerValue(1.0);
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
    World::GetWorldPointer()->GetAudioManager()->SetAmbientVolume(Volume);
}

Real OptsAmbientVol::GetVolume() const
{
    return World::GetWorldPointer()->GetAudioManager()->GetAmbientVolume();
}

void OptsDialogVol::SetVolume(const Real& Volume)
{
    World::GetWorldPointer()->GetAudioManager()->SetDialogVolume(Volume);
}

Real OptsDialogVol::GetVolume() const
{
    return World::GetWorldPointer()->GetAudioManager()->GetDialogVolume();
}

void OptsEffectVol::SetVolume(const Real& Volume)
{
    World::GetWorldPointer()->GetAudioManager()->SetEffectVolume(Volume);
}

Real OptsEffectVol::GetVolume() const
{
    return World::GetWorldPointer()->GetAudioManager()->GetEffectVolume();
}

void OptsMusicVol::SetVolume(const Real& Volume)
{
    World::GetWorldPointer()->GetAudioManager()->SetMusicVolume(Volume);
}

Real OptsMusicVol::GetVolume() const
{
    return World::GetWorldPointer()->GetAudioManager()->GetMusicVolume();
}

OptsAudioMute::~OptsAudioMute()
{

}

void OptsAudioMute::SetCaller(UI::Widget* Caller)
{

}

void OptsAudioMute::DoHoverItems()
{

}

void OptsAudioMute::DoPreUpdateItems()
{

}

void OptsAudioMute::DoPostUpdateItems()
{

}

void OptsAudioMute::DoVisibilityChangeItems()
{

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
    GraphicsManager* GraphicsMan = World::GetWorldPointer()->GetGraphicsManager();
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
        GameWindow* PriWin = World::GetWorldPointer()->GetGraphicsManager()->GetPrimaryGameWindow();
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
    UI::WidgetCallback::SetCaller(Caller);
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
    bool FullScreen = World::GetWorldPointer()->GetGraphicsManager()->GetPrimaryGameWindow()->getFullscreen();
    UI::CheckBox* FSCheck = static_cast<UI::CheckBox*>(this->Caller);
    if(FullScreen != FSCheck->IsChecked())
        FSCheck->ManualCheck(FullScreen);
}


#endif
