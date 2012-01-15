#ifndef _itemshopcell_h
#define _itemshopcell_h

#include <mezzanine.h>

using namespace Mezzanine;

class ItemShopCell : public UI::Cell
{
    protected:
        UI::Caption* ItemCaption;
        virtual void UpdateImpl(bool Force = false);
        virtual void SetVisibleImpl(bool visible);
        virtual bool CheckMouseHoverImpl();
    public:
        ItemShopCell(const String& name, const UI::RenderableRect& Rect, const String& ItemName, UI::Layer* parent);
        virtual ~ItemShopCell();
        virtual void SetPosition(const Vector2& Position);
        virtual void SetActualPosition(const Vector2& Position);
        virtual void SetSize(const Vector2& Size);
        virtual void SetActualSize(const Vector2& Size);
        virtual void UpdateDimensions();
        virtual UI::Caption* GetItemCaption();
};

class ItemShopCB : public UI::CellCallback
{
    protected:
    public:
        ItemShopCB();
        virtual ~ItemShopCB();
        virtual void SetCaller(UI::Cell* Caller);
        virtual void DoSelectedItems();
        virtual void DoUnselectedItems();
};

#endif
