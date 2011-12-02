#ifndef _buttoncallbacks_h
#define _buttoncallbacks_h

#include <mezzanine.h>

using namespace Mezzanine;

class GSStore : public UI::ButtonCallback
{
    public:
        GSStore();
        virtual ~GSStore();
        virtual void DoHoverItems();
        virtual void DoActivateItems();
};

class GSMenu : public UI::ButtonCallback
{
    public:
        GSMenu();
        virtual ~GSMenu();
        virtual void DoHoverItems();
        virtual void DoActivateItems();
};

class GSReturn : public UI::ButtonCallback
{
    public:
        GSReturn();
        virtual ~GSReturn();
        virtual void DoHoverItems();
        virtual void DoActivateItems();
};

class GSISReturn : public UI::ButtonCallback
{
    public:
        GSISReturn();
        virtual ~GSISReturn();
        virtual void DoHoverItems();
        virtual void DoActivateItems();
};

class GSMMReturn : public UI::ButtonCallback
{
    public:
        GSMMReturn();
        virtual ~GSMMReturn();
        virtual void DoHoverItems();
        virtual void DoActivateItems();
};

class GSRestart : public UI::ButtonCallback
{
    public:
        GSRestart();
        virtual ~GSRestart();
        virtual void DoHoverItems();
        virtual void DoActivateItems();
};

class MSStart : public UI::ButtonCallback
{
    protected:
        UI::PagedCellGrid* TheGrid;
    public:
        MSStart(UI::PagedCellGrid* LevelGrid);
        virtual ~MSStart();
        virtual void DoHoverItems();
        virtual void DoActivateItems();
};

class AllAppExit : public UI::ButtonCallback
{
    public:
        AllAppExit();
        virtual ~AllAppExit();
        virtual void DoHoverItems();
        virtual void DoActivateItems();
};

class OptsVideoApply : public UI::ButtonCallback
{
    protected:
        UI::DropDownList* ResolutionList;
        UI::CheckBox* FullScreenBox;
        UI::CheckBox* FPSStatsBox;
    public:
        OptsVideoApply(UI::DropDownList* ResList, UI::CheckBox* FSBox, UI::CheckBox* StatsBox);
        virtual ~OptsVideoApply();
        virtual void DoHoverItems();
        virtual void DoActivateItems();
};

#endif
