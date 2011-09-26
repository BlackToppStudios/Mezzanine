#ifndef _widgetcallbacks_h
#define _widgetcallbacks_h

#include <physgame.h>

using namespace phys;

class OptsVolume : public UI::WidgetCallback
{
    protected:
        virtual void SetVolume(const Real& Volume) = 0;
        virtual Real GetVolume() const = 0;
    public:
        OptsVolume();
        virtual ~OptsVolume();
        virtual void SetCaller(UI::Widget* Caller);
        virtual void DoHoverItems();
        virtual void DoPreUpdateItems();
        virtual void DoPostUpdateItems();
        virtual void DoVisibilityChangeItems();
};

class OptsAmbientVol : public OptsVolume
{
    protected:
        void SetVolume(const Real& Volume);
        Real GetVolume() const;
};

class OptsDialogVol : public OptsVolume
{
    protected:
        void SetVolume(const Real& Volume);
        Real GetVolume() const;
};

class OptsEffectVol : public OptsVolume
{
    protected:
        void SetVolume(const Real& Volume);
        Real GetVolume() const;
};

class OptsMusicVol : public OptsVolume
{
    protected:
        void SetVolume(const Real& Volume);
        Real GetVolume() const;
};

class OptsAudioMute : public UI::WidgetCallback
{
    protected:
    public:
        OptsAudioMute();
        virtual ~OptsAudioMute();
        virtual void SetCaller(UI::Widget* Caller);
        virtual void DoHoverItems();
        virtual void DoPreUpdateItems();
        virtual void DoPostUpdateItems();
        virtual void DoVisibilityChangeItems();
};

class OptsVideoRes : public UI::WidgetCallback
{
    protected:
    public:
        OptsVideoRes();
        virtual ~OptsVideoRes();
        virtual void SetCaller(UI::Widget* Caller);
        virtual void DoHoverItems();
        virtual void DoPreUpdateItems();
        virtual void DoPostUpdateItems();
        virtual void DoVisibilityChangeItems();
};

class OptsVideoFullscreen : public UI::WidgetCallback
{
    protected:
    public:
        OptsVideoFullscreen();
        virtual ~OptsVideoFullscreen();
        virtual void SetCaller(UI::Widget* Caller);
        virtual void DoHoverItems();
        virtual void DoPreUpdateItems();
        virtual void DoPostUpdateItems();
        virtual void DoVisibilityChangeItems();
};

#endif
