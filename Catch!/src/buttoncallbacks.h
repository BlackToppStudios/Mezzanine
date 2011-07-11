#ifndef _buttoncallbacks_h
#define _buttoncallbacks_h

#include <physgame.h>

using namespace phys;

class GSStore : public UI::ButtonCallback
{
    public:
        GSStore(UI::Button* caller);
        virtual ~GSStore();
        virtual void DoHoverItems();
        virtual void DoActivateItems();
};

class GSMenu : public UI::ButtonCallback
{
    public:
        GSMenu(UI::Button* caller);
        virtual ~GSMenu();
        virtual void DoHoverItems();
        virtual void DoActivateItems();
};

class GSReturn : public UI::ButtonCallback
{
    public:
        GSReturn(UI::Button* caller);
        virtual ~GSReturn();
        virtual void DoHoverItems();
        virtual void DoActivateItems();
};

class GSMMReturn : public UI::ButtonCallback
{
    public:
        GSMMReturn(UI::Button* caller);
        virtual ~GSMMReturn();
        virtual void DoHoverItems();
        virtual void DoActivateItems();
};

class MSStart : public UI::ButtonCallback
{
    protected:
        UI::PagedCellGrid* TheGrid;
    public:
        MSStart(UI::Button* caller, UI::PagedCellGrid* LevelGrid);
        virtual ~MSStart();
        virtual void DoHoverItems();
        virtual void DoActivateItems();
};

class AllAppExit : public UI::ButtonCallback
{
    public:
        AllAppExit(UI::Button* caller);
        virtual ~AllAppExit();
        virtual void DoHoverItems();
        virtual void DoActivateItems();
};

#endif
