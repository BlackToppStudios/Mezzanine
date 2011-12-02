#ifndef _widgetcallbacks_h
#define _widgetcallbacks_h

#include <mezzanine.h>

using namespace Mezzanine;

class OptsVolume : public UI::WidgetCallback
{
    protected:
        virtual void SetVolume(const Real& Volume) = 0;
        virtual Real GetVolume() const = 0;
    public:
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
        virtual ~OptsVideoFullscreen();
        virtual void SetCaller(UI::Widget* Caller);
        virtual void DoHoverItems();
        virtual void DoPreUpdateItems();
        virtual void DoPostUpdateItems();
        virtual void DoVisibilityChangeItems();
};

class OptsVideoFSAA : public UI::WidgetCallback
{
    protected:
    public:
        virtual ~OptsVideoFSAA();
        virtual void SetCaller(UI::Widget* Caller);
        virtual void DoHoverItems();
        virtual void DoPreUpdateItems();
        virtual void DoPostUpdateItems();
        virtual void DoVisibilityChangeItems();
};

class OptsVideoStats : public UI::WidgetCallback
{
    protected:
    public:
        virtual ~OptsVideoStats();
        virtual void SetCaller(UI::Widget* Caller);
        virtual void DoHoverItems();
        virtual void DoPreUpdateItems();
        virtual void DoPostUpdateItems();
        virtual void DoVisibilityChangeItems();
};

#endif
